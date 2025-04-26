// #define DEBUG 1

#include "BQ25798.h"

#include <Wire.h>

BQ25798::BQ25798(uint8_t chip_address) {
  _chip_address = chip_address;
  _clearRegs();
}

BQ25798::BQ25798() : BQ25798(DEFAULT_I2C_ADDRESS) {}

const BQ25798::RegisterDefinition& BQ25798::getRegisterDefinition(regaddr_t address) {
  const RegisterDefinition& invalid_reg_def = RegisterDefinition::invalid();

  if (address >= PHYSICAL_REGISTERS_COUNT) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), address);
    return invalid_reg_def;
  }

  const RegisterDefinition& reg_def = _registerDefinitions[address];

  if (reg_def.address != address) {
    ERROR_PRINT(F("Invalid register definition 0x%02X: register 0x%02X references, probably skipped something in the .h file\n"), address, reg_def.address);
    return invalid_reg_def;
  }

  return reg_def;
};

const BQ25798::Setting& BQ25798::getSetting(int id) {
  const Setting& invalid_setting = Setting::invalid();

  if (id < 0 || id >= SETTINGS_COUNT) {
    ERROR_PRINT(F("Invalid setting ID %d\n"), id);
    return invalid_setting;
  }

  const Setting& setting = _settingsList[id];
  return setting;
};

void BQ25798::_clearRegs() {
  for (int i = 0; i < PHYSICAL_REGISTERS_COUNT; i++) {
    _physicalReg8Values[i] = 0;
  }
}

bool BQ25798::begin() { return readAll(); }

bool BQ25798::readAll() {
  DEBUG_PRINT("[readAll] Reading all BQ25798 registers\n");

  Wire.beginTransmission(_chip_address);
  Wire.write(REG00_Minimal_System_Voltage);  // Start reading from the first register
  Wire.endTransmission();
  if (Wire.requestFrom(_chip_address, PHYSICAL_REGISTERS_COUNT) != PHYSICAL_REGISTERS_COUNT) {
    ERROR_PRINT(F("Error batch reading all (%d) BQ25798 registers\n"), PHYSICAL_REGISTERS_COUNT);
    return false;
  }

  for (int i = 0; i < PHYSICAL_REGISTERS_COUNT; i++) {
    _physicalReg8Values[i] = Wire.read();
  }

  DEBUG_PRINT("[readAll] -> success\n");
  return true;
}

bool BQ25798::writeReg8ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), reg);
    return false;
  };
  DEBUG_PRINT("[writeReg8ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X\n", reg, reg_def.name, _physicalReg8Values[reg]);
#endif

  Wire.beginTransmission(_chip_address);
  if (Wire.write(reg) != 1) {
    ERROR_PRINT(F("Error writing to BQ25798 register %02X\n"), reg);
    return false;
  }
  if (Wire.write(_physicalReg8Values[reg]) != 1) {
    ERROR_PRINT(F("Error writing to BQ25798 register %02X\n"), reg);
    return false;
  }
  Wire.endTransmission();

  DEBUG_PRINT("[writeReg8ToI2C] -> success\n");
  return true;
}

bool BQ25798::writeReg16ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), reg);
    return false;
  };
  DEBUG_PRINT("[writeReg16ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X 0x%02X\n", reg, reg_def.name, _physicalReg8Values[reg],
              _physicalReg8Values[reg + 1]);
#endif

  Wire.beginTransmission(_chip_address);
  if (Wire.write(reg) != 1) {
    ERROR_PRINT(F("Error writing to BQ25798 register %02X\n"), reg);
    return false;
  }
  if (Wire.write(_physicalReg8Values[reg]) != 1) {
    ERROR_PRINT(F("Error writing to BQ25798 register %02X\n"), reg);
    return false;
  }
  if (Wire.write(_physicalReg8Values[reg + 1]) != 1) {
    ERROR_PRINT(F("Error writing to BQ25798 register %02X\n"), reg);
    return false;
  }
  Wire.endTransmission();

  DEBUG_PRINT("[writeReg16ToI2C] -> success\n");
  return true;
}

uint8_t BQ25798::getReg8(int reg, int bitMask, int bitShift) { return (_physicalReg8Values[reg] >> bitShift) & bitMask; }

void BQ25798::setReg8(int reg, uint8_t value, int bitMask, int bitShift) {
  uint8_t shiftedMask = bitMask << bitShift;
  uint8_t shiftedValue = (value & bitMask) << bitShift;
  uint8_t negatedMask = 0xFF ^ shiftedMask;

  _physicalReg8Values[reg] &= negatedMask;
  _physicalReg8Values[reg] |= shiftedValue;
}

uint16_t BQ25798::getReg16(int widereg, int bitMask, int bitShift) {
  return ((_physicalReg8Values[widereg] << 8) | _physicalReg8Values[widereg + 1]) >> bitShift & bitMask;
}

void BQ25798::setReg16(int widereg, uint16_t value, int bitMask, int bitShift) {
  uint16_t shiftedMask = bitMask << bitShift;
  uint16_t shiftedValue = (value & bitMask) << bitShift;
  uint16_t negatedMask = 0xFFFF ^ shiftedMask;

  _physicalReg8Values[widereg] &= negatedMask >> 8;
  _physicalReg8Values[widereg + 1] &= negatedMask & 0xFF;
  _physicalReg8Values[widereg] |= shiftedValue >> 8;
  _physicalReg8Values[widereg + 1] |= shiftedValue & 0xFF;
}

bool BQ25798::_flagIsSet(settings_flags_t flagset, settings_flags_t flag) { return (static_cast<uint8_t>(flagset) & static_cast<uint8_t>(flag)) != 0; }

uint16_t BQ25798::getRaw(const Setting& setting) {
  uint16_t raw_value = 0;  // getReg* always returns unsigned values, so we need to use unsigned type here

  if (!setting.isValid()) {
    ERROR_PRINT(F("Invalid setting\n"));
    return -1;
  }

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), setting.reg);
    return -1;
  };

  DEBUG_PRINT("[getRaw] [reg=0x%02X, bitMask=0x%02X, bitShift=%d]\n", setting.reg, setting.mask, setting.shift);
  DEBUG_PRINT("[getRaw] setting=%s [reg=0x%02X (%s), bitMask=0x%02X, bitShift=%d]\n", setting.name, setting.reg, reg_def.name, setting.mask, setting.shift);

  if (reg_def.size == regsize_t::SHORT) {
    raw_value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (reg_def.size == regsize_t::LONG) {
    raw_value = getReg16(setting.reg, setting.mask, setting.shift);
  }

  DEBUG_PRINT("[getRaw] -> raw 0x%04X\n", raw_value);

  return raw_value;
};

int BQ25798::getInt(const Setting& setting) {
  DEBUG_PRINT("[getInt] Setting:[reg=0x%02X, bitMask=0x%02X, bitShift=%d]\n", setting.reg, setting.mask, setting.shift);
  uint16_t raw_value = getRaw(setting);

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  int value;
  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    if (reg_def.size != regsize_t::LONG) {
      ERROR_PRINT(F("2's complement flag set for non-long register 0x%02X (%s)\n"), setting.reg, reg_def.name);
      return 0;  // Handle error case
    };

    if (raw_value & (1 << (static_cast<int>(reg_def.size) - 1))) {  // Check if the sign bit is set
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
  DEBUG_PRINT("[getInt]  - intermediate 0x%04X\n", value);

  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    value *= setting.bit_step_size;
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  DEBUG_PRINT("[getInt] -> final %d\n", value);

  return value;
};

void BQ25798::setInt(const Setting& setting, int value) {
  // Serial.printf("[DEBUG] setInt(reg=0x%02X, bitMask=0x%02X, bitShift=%d, value=%d)\n", setting.reg, setting.mask, setting.shift, value);

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Error: Invalid register address 0x%02X\n"), setting.reg);
    return;
  };

  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      ERROR_PRINT(F("Value %d out of range (%.3f, %.3f) for register 0x%02X\n"), value, setting.range_low, setting.range_high, setting.reg);
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

  // if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
  //   if (value < 0) {
  //     value = (1 << reg_def.size) + value;  // Convert to unsigned value
  //   }
  // }

  if (reg_def.size == regsize_t::SHORT) {
    // DEBUG_PRINT(F("- setReg8(reg=0x%02X, value=0x%02X, bitMask=0x%02X, bitShift=%d)\n"), setting.reg, value, setting.mask, setting.shift);
    // DEBUG_PRINT(F("  -> old value=0x%02X\n"), _physicalReg8Values[setting.reg]);
    setReg8(setting.reg, value, setting.mask, setting.shift);
    // DEBUG_PRINT(F("    -> new value=0x%02X\n"), _physicalReg8Values[setting.reg]);
    // DEBUG_PRINT(F("  - writeReg8ToI2C(reg=0x%02X)\n"), setting.reg);
    writeReg8ToI2C(setting.reg);
  } else if (reg_def.size == regsize_t::LONG) {
    setReg16(setting.reg, value, setting.mask, setting.shift);
    writeReg16ToI2C(setting.reg);
  }
};

float BQ25798::getFloat(const Setting& setting) {
  float value = getRaw(setting);

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
  // FIXME
  // // Check if any fault flags are set

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
