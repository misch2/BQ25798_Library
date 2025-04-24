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
  // Serial.printf("[DEBUG] Writing to BQ25798 register 0x%02X: 0x%02X\n", reg, _regs[reg]);
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
  // Serial.printf("[DEBUG] Writing to BQ25798 register 0x%02X: 0x%02X 0x%02X\n", reg, _regs[reg], _regs[reg + 1]);
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
  uint8_t negatedMask = 0xFF ^ shiftedMask;

  _regs[reg] &= negatedMask;
  _regs[reg] |= shiftedValue;
}

uint16_t BQ25798::getReg16(int widereg, int bitMask, int bitShift) { return ((_regs[widereg] << 8) | _regs[widereg + 1]) >> bitShift & bitMask; }

void BQ25798::setReg16(int widereg, uint16_t value, int bitMask, int bitShift) {
  uint16_t shiftedMask = bitMask << bitShift;
  uint16_t shiftedValue = (value & bitMask) << bitShift;
  uint16_t negatedMask = 0xFFFF ^ shiftedMask;

  _regs[widereg] &= negatedMask >> 8;
  _regs[widereg + 1] &= negatedMask & 0xFF;
  _regs[widereg] |= shiftedValue >> 8;
  _regs[widereg + 1] |= shiftedValue & 0xFF;
}

bool BQ25798::_flagIsSet(flags_t flagset, flags_t flag) { return (static_cast<uint8_t>(flagset) & static_cast<uint8_t>(flag)) != 0; }

int BQ25798::getInt(Setting setting) {
  uint16_t raw_value = 0;  // getReg* always returns unsigned values, so we need to use unsigned type here
  if (setting.size == regsize_t::SHORT) {
    raw_value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (setting.size == regsize_t::LONG) {
    raw_value = getReg16(setting.reg, setting.mask, setting.shift);
  }

  int value;
  if (_flagIsSet(setting.flags, flags_t::IS_2COMPLEMENT)) {
    if (setting.size != regsize_t::LONG) {
      Serial.println("Error: 2's complement flag set for non-long register size");
      return 0;  // Handle error case
    };

    if (raw_value & (1 << (static_cast<int>(setting.size) - 1))) {  // Check if the sign bit is set
      // Serial.printf("[DEBUG] getInt() 2's complement detected, value=0x%04X\n", raw_value);
      int16_t adjusted_value = static_cast<int16_t>(raw_value);  // Cast to signed type
      // Serial.printf("[DEBUG]  -> adjusted value: 0x%04X\n", adjusted_value);
      value = adjusted_value;
    } else {
      value = raw_value;  // No adjustment needed for positive values
    }
  } else {
    value = raw_value;  // No adjustment needed for unsigned values
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
  // Serial.printf("[DEBUG] setInt(reg=0x%02X, bitMask=0x%02X, bitShift=%d, value=%d)\n", setting.reg, setting.mask, setting.shift, value);
  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      Serial.printf("Value %d out of range (%.3f, %.3f) for register 0x%02X\n", value, setting.range_low, setting.range_high, setting.reg);
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

  // if (_flagIsSet(setting.flags, flags_t::IS_2COMPLEMENT)) {
  //   if (value < 0) {
  //     value = (1 << setting.size) + value;  // Convert to unsigned value
  //   }
  // }

  if (setting.size == regsize_t::SHORT) {
    // Serial.printf("[DEBUG]   - setReg8(reg=0x%02X, value=0x%02X, bitMask=0x%02X, bitShift=%d)\n", setting.reg, value, setting.mask, setting.shift);
    // Serial.printf("[DEBUG]     -> old value=0x%02X\n", _regs[setting.reg]);
    setReg8(setting.reg, value, setting.mask, setting.shift);
    // Serial.printf("[DEBUG]     -> new value=0x%02X\n", _regs[setting.reg]);
    // Serial.printf("[DEBUG]   - writeReg8ToI2C(reg=0x%02X)\n", setting.reg);
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
    // Serial.printf("[DEBUG] getFloat() adjusting raw value %.3f by bit step size %.3f\n", value, setting.bit_step_size);
    value *= setting.bit_step_size;
    // Serial.printf("[DEBUG]  -> adjusted to %.3f\n", value);
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  return value;
};

bool BQ25798::faultDetected() {
  // Check if any fault flags are set

  if (getInt(IBAT_REG_STAT) || getInt(VBUS_OVP_STAT) || getInt(VBAT_OVP_STAT) || getInt(IBUS_OCP_STAT) || getInt(IBAT_OCP_STAT) || getInt(CONV_OCP_STAT) ||
      getInt(VAC2_OVP_STAT) || getInt(VAC1_OVP_STAT) || getInt(VSYS_SHORT_STAT) || getInt(VSYS_OVP_STAT) || getInt(OTG_OVP_STAT) || getInt(OTG_UVP_STAT) ||
      getInt(TSHUT_STAT)) {
    return true;
  }

  if (getInt(IBAT_REG_FLAG) || getInt(VBUS_OVP_FLAG) || getInt(VBAT_OVP_FLAG) || getInt(IBUS_OCP_FLAG) || getInt(IBAT_OCP_FLAG) || getInt(CONV_OCP_FLAG) ||
      getInt(VAC2_OVP_FLAG) || getInt(VAC1_OVP_FLAG) || getInt(VSYS_SHORT_FLAG) || getInt(VSYS_OVP_FLAG) || getInt(OTG_OVP_FLAG) || getInt(OTG_UVP_FLAG) ||
      getInt(TSHUT_FLAG)) {
    return true;
  }

  return false;
}

// Generic method to convert an integer to a string using a map
const char* BQ25798::toString(int value, strings_vector_t map) {
  if (value >= 0 && value < map.size()) {
    return map[value].c_str();
  } else {
    return "Unknown";
  }
}
