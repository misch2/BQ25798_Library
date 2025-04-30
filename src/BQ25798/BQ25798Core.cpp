#include "BQ25798Core.h"

BQ25798Core::BQ25798Core() {}

const BQ25798Core::RegisterDefinition& BQ25798Core::getRegisterDefinition(regaddr_t address) {
  static const RegisterDefinition& invalid_reg_def = RegisterDefinition::invalid();

  if (address >= PHYSICAL_REGISTERS_COUNT) {
    _setError(ERROR_INVALID_REGISTER);
    return invalid_reg_def;
  }

  const RegisterDefinition& reg_def = _registerDefinitions[address];

  if (reg_def.address != address) {
    _setError(ERROR_INVALID_REGISTER);
    return invalid_reg_def;
  }

  return reg_def;
};

const BQ25798Core::Setting& BQ25798Core::getSetting(int id) {
  static const Setting& invalid_setting = Setting::invalid();

  if (id < 0 || id >= SETTINGS_COUNT) {
    _setError(ERROR_INVALID_SETTING);
    return invalid_setting;
  }

  const Setting& setting = _settingsList[id];
  return setting;
};

uint8_t BQ25798Core::getReg8(int reg, int bitMask, int bitShift) { return (_physicalReg8Values[reg] >> bitShift) & bitMask; }

void BQ25798Core::setReg8(int reg, uint8_t value, int bitMask, int bitShift) {
  uint8_t shiftedMask = bitMask << bitShift;
  uint8_t shiftedValue = (value & bitMask) << bitShift;
  uint8_t negatedMask = 0xFF ^ shiftedMask;

  _physicalReg8Values[reg] &= negatedMask;
  _physicalReg8Values[reg] |= shiftedValue;
}

uint16_t BQ25798Core::getReg16(int widereg, int bitMask, int bitShift) {
  return ((_physicalReg8Values[widereg] << 8) | _physicalReg8Values[widereg + 1]) >> bitShift & bitMask;
}

void BQ25798Core::setReg16(int widereg, uint16_t value, int bitMask, int bitShift) {
  uint16_t shiftedMask = bitMask << bitShift;
  uint16_t shiftedValue = (value & bitMask) << bitShift;
  uint16_t negatedMask = 0xFFFF ^ shiftedMask;

  _physicalReg8Values[widereg] &= negatedMask >> 8;
  _physicalReg8Values[widereg + 1] &= negatedMask & 0xFF;
  _physicalReg8Values[widereg] |= shiftedValue >> 8;
  _physicalReg8Values[widereg + 1] |= shiftedValue & 0xFF;
}

bool BQ25798Core::_flagIsSet(settings_flags_t flagset, settings_flags_t flag) { return (static_cast<uint8_t>(flagset) & static_cast<uint8_t>(flag)) != 0; }

uint16_t BQ25798Core::getRaw(const Setting& setting) {
  uint16_t raw_value = 0;  // getReg* always returns unsigned values, so we need to use unsigned type here

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  DEBUG_PRINT(F("[getRaw] [reg=0x%02X, bitMask=0x%02X, bitShift=%d]\n"), setting.reg, setting.mask, setting.shift);
  DEBUG_PRINT(F("[getRaw] setting=%s [reg=0x%02X (%s), bitMask=0x%02X, bitShift=%d]\n"), setting.name, setting.reg, reg_def.name, setting.mask, setting.shift);

  if (reg_def.size == regsize_t::SHORT) {
    raw_value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (reg_def.size == regsize_t::LONG) {
    raw_value = getReg16(setting.reg, setting.mask, setting.shift);
  }

  DEBUG_PRINT(F("[getRaw] -> raw 0x%04X\n"), raw_value);

  return raw_value;
};

int BQ25798Core::getInt(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToInt(raw_value, setting);
}

bool BQ25798Core::getBool(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToBool(raw_value, setting);
}

float BQ25798Core::getFloat(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToFloat(raw_value, setting);
}

const char* BQ25798Core::getString(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToString(raw_value, setting);
}

bool BQ25798Core::faultFlagRaised() {
  // Check if any fault flags are set
  if (getInt(IBAT_REG_FLAG) || getInt(VBUS_OVP_FLAG) || getInt(VBAT_OVP_FLAG) || getInt(IBUS_OCP_FLAG) || getInt(IBAT_OCP_FLAG) || getInt(CONV_OCP_FLAG) ||
      getInt(VAC2_OVP_FLAG) || getInt(VAC1_OVP_FLAG) || getInt(VSYS_SHORT_FLAG) || getInt(VSYS_OVP_FLAG) || getInt(OTG_OVP_FLAG) || getInt(OTG_UVP_FLAG) ||
      getInt(TSHUT_FLAG)) {
    return true;
  }

  return false;
}

// Generic method to convert an integer to a string using a map
const char* BQ25798Core::toString(int value, strings_vector_t map) {
  if (value >= 0 && value < map.size()) {
    return map[value];
  } else {
    return "Unknown";
  }
}

int BQ25798Core::rawToInt(uint16_t raw, const Setting& setting) {
  // anything can be converted to int, no need to check the type

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  int value;
  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    if (reg_def.size != regsize_t::LONG) {
      _setError(ERROR_INVALID_SETTING);
      return 0;
    };

    if (raw & (1 << (static_cast<int>(reg_def.size) - 1))) {  // Check if the sign bit is set
      // DEBUG_PRINT(F("getInt() 2's complement detected, value=0x%04X\n"), raw_value);
      int16_t adjusted_value = static_cast<int16_t>(raw);  // Cast to signed type
      // DEBUG_PRINT(F(" -> adjusted value: 0x%04X\n"), adjusted_value);
      value = adjusted_value;
    } else {
      value = raw;  // No adjustment needed for positive values
    }
  } else {
    value = raw;  // No adjustment needed for unsigned values
  }
  DEBUG_PRINT(F("[rawToInt]  - intermediate 0x%04X\n"), value);

  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    value *= setting.bit_step_size;
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  DEBUG_PRINT(F("[rawToInt] -> final %d\n"), value);

  return value;
}

uint16_t BQ25798Core::intToRaw(int value, const Setting& setting) {
  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      _setError(ERROR_INVALID_VALUE);
      return 0;
    }
  }
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.fixed_offset != 0) {
    value -= std::trunc(setting.fixed_offset);
  };
  if (setting.bit_step_size != 0) {
    value /= std::trunc(setting.bit_step_size);
  };

  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    if (reg_def.size != regsize_t::LONG) {
      _setError(ERROR_INVALID_SETTING);
      return 0;  // Handle error case
    };

    if (value < 0) {
      value = static_cast<uint16_t>(value);  // Cast to unsigned type
    }
  }

  return value;
}

float BQ25798Core::rawToFloat(uint16_t raw, const Setting& setting) {
  // anything can be converted to float, no need to check the type

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  float value = raw;
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    // DEBUG_PRINT(F("getFloat() adjusting raw value %.3f by bit step size %.3f\n"), value, setting.bit_step_size);
    value *= setting.bit_step_size;
    // DEBUG_PRINT(F(" -> adjusted to %.3f\n"), value);
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  return value;
}

uint16_t BQ25798Core::floatToRaw(float value, const Setting& setting) {
  if (setting.type != settings_type_t::FLOAT) {
    _setError(ERROR_INVALID_VALUE);
    return 0;
  }

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      _setError(ERROR_INVALID_VALUE);
      return 0;
    }
  }
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.fixed_offset != 0) {
    value -= setting.fixed_offset;
  };
  if (setting.bit_step_size != 0) {
    value /= setting.bit_step_size;
  };

  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    _setError(ERROR_INVALID_SETTING);
    return 0.0f;
  }

  return value;
}

bool BQ25798Core::rawToBool(uint16_t raw, const Setting& setting) {
  // anything can be converted to bool, no need to check the type
  return raw != 0;
}

uint16_t BQ25798Core::boolToRaw(bool value, const Setting& setting) {
  if (setting.type != settings_type_t::BOOL) {
    _setError(ERROR_INVALID_VALUE);
    return 0;
  }

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  return value ? 1 : 0;
}

const char* BQ25798Core::rawToString(uint16_t raw, const Setting& setting) {
  // anything can be converted to string, no need to check the type
  return toString(raw, setting.strings_vector);
}

void BQ25798Core::_clearRegs() {
  for (int i = 0; i < PHYSICAL_REGISTERS_COUNT; i++) {
    _physicalReg8Values[i] = 0;
  }
}

void BQ25798Core::clearError() { _setError(ERROR_NONE); }
int BQ25798Core::getError() { return _errorCode; }
void BQ25798Core::_setError(int errorCode) { _errorCode = errorCode; }