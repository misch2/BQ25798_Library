#include "BQ25798.h"

#include <Wire.h>

BQ25798::BQ25798() {
  _address = DEFAULT_I2C_ADDRESS;
  clearRegs();
}

BQ25798::BQ25798(uint8_t address) {
  _address = address;
  clearRegs();
}

void BQ25798::clearRegs() {
  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = 0;
  }
}

bool BQ25798::begin() { return readAll(); }

bool BQ25798::readAll() {
  Wire.beginTransmission(_address);
  Wire.write(MIN_REGISTER_NUMBER);
  Wire.endTransmission();
  if (Wire.requestFrom(_address, registersCount) != registersCount) {
    Serial.println("Error reading BQ25798 registers");
    return false;
  }

  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = Wire.read();
  }

  return true;
}

bool BQ25798::writeReg8ToI2C(int reg) {
  Serial.printf("[DEBUG] Writing to BQ25798 register %02X: %02X\n", reg, _regs[reg]);
  Wire.beginTransmission(_address);
  if (Wire.write(reg) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  Wire.endTransmission();
  return true;
}

bool BQ25798::writeReg16ToI2C(int reg) {
  Serial.printf("[DEBUG] Writing to BQ25798 register %02X: %02X %02X\n", reg, _regs[reg], _regs[reg + 1]);
  Wire.beginTransmission(_address);
  if (Wire.write(reg) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg + 1]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  Wire.endTransmission();
  return true;
}

uint8_t BQ25798::getReg8(int reg, int bitMask, int bitShift) { return (_regs[reg] >> bitShift) & bitMask; }

void BQ25798::setReg8(int reg, uint8_t value, int bitMask, int bitShift) {
  uint8_t shiftedMask = bitMask << bitShift;
  uint8_t shiftedValue = (value & bitMask) << bitShift;

  _regs[reg] &= shiftedMask;
  _regs[reg] |= (shiftedValue & shiftedMask);
}

uint16_t BQ25798::getReg16(int widereg, int bitMask, int bitShift) { return ((_regs[widereg] << 8) | _regs[widereg + 1]) >> bitShift & bitMask; }

void BQ25798::setReg16(int widereg, uint16_t value, int bitMask, int bitShift) {
  uint16_t shiftedMask = bitMask << bitShift;
  uint16_t shiftedValue = (value & bitMask) << bitShift;

  _regs[widereg] &= shiftedMask >> 8;
  _regs[widereg + 1] &= shiftedMask & 0xFF;
  _regs[widereg] |= (shiftedValue & shiftedMask) >> 8;
  _regs[widereg + 1] |= (shiftedValue & shiftedMask) & 0xFF;
}

// ==================== new functions ====================

int BQ25798::getInt(Setting setting) {
  int value = 0;
  if (setting.size == regsize_t::SHORT) {
    value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (setting.size == regsize_t::LONG) {
    value = getReg16(setting.reg, setting.mask, setting.shift);
  }

  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    value *= setting.bit_step_size;
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  return value;
};

void BQ25798::setInt(Setting setting, int value) {
  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      Serial.printf("Value %d out of range (%d, %d) for register %d\n", value, setting.range_low, setting.range_high, setting.reg);
      return;
    }
  }
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.fixed_offset != 0) {
    value -= trunc(setting.fixed_offset);
  };
  if (setting.bit_step_size != 0) {
    value /= trunc(setting.bit_step_size);
  };

  if (setting.size == regsize_t::SHORT) {
    setReg8(setting.reg, value, setting.mask, setting.shift);
    writeReg8ToI2C(setting.reg);
  } else if (setting.size == regsize_t::LONG) {
    setReg16(setting.reg, value, setting.mask, setting.shift);
    writeReg16ToI2C(setting.reg);
  }
};

float BQ25798::getFloat(Setting setting) {
  float value = 0;
  if (setting.size == regsize_t::SHORT) {
    value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (setting.size == regsize_t::LONG) {
    value = getReg16(setting.reg, setting.mask, setting.shift);
  }

  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    value *= setting.bit_step_size;
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  return value;
};

// Generic method to convert an integer to a string using a map
const char* BQ25798::toString(int value, strings_vector_t map) {
  if (value >= 0 && value < map.size()) {
    return map[value].c_str();
  } else {
    return "Unknown";
  }
}
