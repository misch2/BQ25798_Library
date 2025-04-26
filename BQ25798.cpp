#include "BQ25798.h"

#include <Wire.h>

#define DEFINE_REGISTER(propname, size) {propname, regsize_t::size, F(#propname)}

#define DEFINE_SETTING(id, propname, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  {                                                                                                            \
    id, { regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, F(#propname) }             \
  }

BQ25798::BQ25798(uint8_t chip_address)
    : _registerDefinitions{{
          //  There must be all 8-bit registers in the list (no skipping for 16-bit registers, no skipping for unused registers)!
          DEFINE_REGISTER(REG00_Minimal_System_Voltage, SHORT),
          DEFINE_REGISTER(REG01_Charge_Voltage_Limit, LONG),
          DEFINE_REGISTER(REG02_dummy_fill, SHORT),
          DEFINE_REGISTER(REG03_Charge_Current_Limit, LONG),
          DEFINE_REGISTER(REG04_dummy_fill, SHORT),
          DEFINE_REGISTER(REG05_Input_Voltage_Limit, SHORT),
          DEFINE_REGISTER(REG06_Input_Current_Limit, LONG),
          DEFINE_REGISTER(REG07_dummy_fill, SHORT),
          DEFINE_REGISTER(REG08_Precharge_Control, SHORT),
          DEFINE_REGISTER(REG09_Termination_Control, SHORT),
          DEFINE_REGISTER(REG0A_Recharge_Control, SHORT),
          DEFINE_REGISTER(REG0B_VOTG_regulation, LONG),
          DEFINE_REGISTER(REG0C_dummy_fill, SHORT),
          DEFINE_REGISTER(REG0D_IOTG_regulation, SHORT),
          DEFINE_REGISTER(REG0E_Timer_Control, SHORT),
          DEFINE_REGISTER(REG0F_Charger_Control_0, SHORT),
          DEFINE_REGISTER(REG10_Charger_Control_1, SHORT),
          DEFINE_REGISTER(REG11_Charger_Control_2, SHORT),
          DEFINE_REGISTER(REG12_Charger_Control_3, SHORT),
          DEFINE_REGISTER(REG13_Charger_Control_4, SHORT),
          DEFINE_REGISTER(REG14_Charger_Control_5, SHORT),
          DEFINE_REGISTER(REG15_MPPT_Control, SHORT),
          DEFINE_REGISTER(REG16_Temperature_Control, SHORT),
          DEFINE_REGISTER(REG17_NTC_Control_0, SHORT),
          DEFINE_REGISTER(REG18_NTC_Control_1, SHORT),
          DEFINE_REGISTER(REG19_ICO_Current_Limit, LONG),
          DEFINE_REGISTER(REG1A_dummy_fill, SHORT),
          DEFINE_REGISTER(REG1B_Charger_Status_0, SHORT),
          DEFINE_REGISTER(REG1C_Charger_Status_1, SHORT),
          DEFINE_REGISTER(REG1D_Charger_Status_2, SHORT),
          DEFINE_REGISTER(REG1E_Charger_Status_3, SHORT),
          DEFINE_REGISTER(REG1F_Charger_Status_4, SHORT),
          DEFINE_REGISTER(REG20_FAULT_Status_0, SHORT),
          DEFINE_REGISTER(REG21_FAULT_Status_1, SHORT),
          DEFINE_REGISTER(REG22_Charger_Flag_0, SHORT),
          DEFINE_REGISTER(REG23_Charger_Flag_1, SHORT),
          DEFINE_REGISTER(REG24_Charger_Flag_2, SHORT),
          DEFINE_REGISTER(REG25_Charger_Flag_3, SHORT),
          DEFINE_REGISTER(REG26_FAULT_Flag_0, SHORT),
          DEFINE_REGISTER(REG27_FAULT_Flag_1, SHORT),
          DEFINE_REGISTER(REG28_Charger_Mask_0, SHORT),
          DEFINE_REGISTER(REG29_Charger_Mask_1, SHORT),
          DEFINE_REGISTER(REG2A_Charger_Mask_2, SHORT),
          DEFINE_REGISTER(REG2B_Charger_Mask_3, SHORT),
          DEFINE_REGISTER(REG2C_FAULT_Mask_0, SHORT),
          DEFINE_REGISTER(REG2D_FAULT_Mask_1, SHORT),
          DEFINE_REGISTER(REG2E_ADC_Control, SHORT),
          DEFINE_REGISTER(REG2F_ADC_Function_Disable_0, SHORT),
          DEFINE_REGISTER(REG30_ADC_Function_Disable_1, SHORT),
          DEFINE_REGISTER(REG31_IBUS_ADC, LONG),
          DEFINE_REGISTER(REG32_dummy_fill, SHORT),
          DEFINE_REGISTER(REG33_IBAT_ADC, LONG),
          DEFINE_REGISTER(REG34_dummy_fill, SHORT),
          DEFINE_REGISTER(REG35_VBUS_ADC, LONG),
          DEFINE_REGISTER(REG36_dummy_fill, SHORT),
          DEFINE_REGISTER(REG37_VAC1_ADC, LONG),
          DEFINE_REGISTER(REG38_dummy_fill, SHORT),
          DEFINE_REGISTER(REG39_VAC2_ADC, LONG),
          DEFINE_REGISTER(REG3A_dummy_fill, SHORT),
          DEFINE_REGISTER(REG3B_VBAT_ADC, LONG),
          DEFINE_REGISTER(REG3C_dummy_fill, SHORT),
          DEFINE_REGISTER(REG3D_VSYS_ADC, LONG),
          DEFINE_REGISTER(REG3E_dummy_fill, SHORT),
          DEFINE_REGISTER(REG3F_TS_ADC, LONG),
          DEFINE_REGISTER(REG40_dummy_fill, SHORT),
          DEFINE_REGISTER(REG41_TDIE_ADC, LONG),
          DEFINE_REGISTER(REG42_dummy_fill, SHORT),
          DEFINE_REGISTER(REG43_DPLUS_ADC, LONG),
          DEFINE_REGISTER(REG44_dummy_fill, SHORT),
          DEFINE_REGISTER(REG45_DMINUS_ADC, LONG),
          DEFINE_REGISTER(REG46_dummy_fill, SHORT),
          DEFINE_REGISTER(REG47_DPDM_Driver, SHORT),
          DEFINE_REGISTER(REG48_Part_Information, SHORT)
          //
      }},

      _settingsList{{
          VSYSMIN,
          VREG,
          ICHG,
          VINDPM,
          IINDPM,
          VBAT_LOWV,
          IPRECHG,
          REG_RST,
          STOP_WD_CHG,
          ITERM,
          CELL,
          TRECHG,
          VRECHG,
          VOTG,
          PRECHG_TMR,
          IOTG,
          TOPOFF_TMR,
          EN_TRICHG_TMR,
          EN_PRECHG_TMR,
          EN_CHG_TMR,
          CHG_TMR,
          TMR2X_EN,
          EN_AUTO_IBATDIS,
          FORCE_IBATDIS,
          EN_CHG,
          EN_ICO,
          FORCE_ICO,
          EN_HIZ,
          EN_TERM,
          EN_BACKUP,
          VBUS_BACKUP,
          VAC_OVP,
          WD_RST,
          WATCHDOG,
          FORCE_INDET,
          AUTO_INDET_EN,
          EN_12V,
          EN_9V,
          HVDCP_EN,
          SDRV_CTRL,
          SDRV_DLY,
          DIS_ACDRV,
          EN_OTG,
          PFM_OTG_DIS,
          PFM_FWD_DIS,
          WKUP_DLY,
          DIS_LDO,
          DIS_OTG_OOA,
          DIS_FWD_OOA,
          EN_ACDRV2,
          EN_ACDRV1,
          PWM_FREQ,
          DIS_STAT,
          DIS_VSYS_SHORT,
          DIS_VOTG_UVP,
          FORCE_VINDPM_DET,
          EN_IBUS_OCP,
          SFET_PRESENT,
          EN_IBAT,
          IBAT_REG,
          EN_IINDPM,
          EN_EXTILIM,
          EN_BATOC,
          VOC_PCT,
          VOC_DLY,
          VOC_RATE,
          EN_MPPT,
          TREG,
          TSHUT,
          VBUS_PD_EN,
          VAC1_PD_EN,
          VAC2_PD_EN,
          BKUP_ACFET1_ON,
          JEITA_VSET,
          JEITA_ISETH,
          JEITA_ISETC,
          TS_COOL,
          TS_WARM,
          BHOT,
          BCOLD,
          TS_IGNORE,
          ICO_ILIM,
          IINDPM_STAT,
          VINDPM_STAT,
          WD_STAT,
          PG_STAT,
          AC2_PRESENT_STAT,
          AC1_PRESENT_STAT,
          VBUS_PRESENT_STAT,
          CHG_STAT,
          VBUS_STAT,
          BC12_DONE_STAT,
          ICO_STAT,
          TREG_STAT,
          DPDM_STAT,
          VBAT_PRESENT_STAT,
          ACRB2_STAT,
          ACRB1_STAT,
          ADC_DONE_STAT,
          VSYS_STAT,
          CHG_TMR_STAT,
          TRICHG_TMR_STAT,
          PRECHG_TMR_STAT,
          VBATOTG_LOW_STAT,
          TS_COLD_STAT,
          TS_COOL_STAT,
          TS_WARM_STAT,
          TS_HOT_STAT,
          IBAT_REG_STAT,
          VBUS_OVP_STAT,
          VBAT_OVP_STAT,
          IBUS_OCP_STAT,
          IBAT_OCP_STAT,
          CONV_OCP_STAT,
          VAC2_OVP_STAT,
          VAC1_OVP_STAT,
          VSYS_SHORT_STAT,
          VSYS_OVP_STAT,
          OTG_OVP_STAT,
          OTG_UVP_STAT,
          TSHUT_STAT,
          IINDPM_FLAG,
          VINDPM_FLAG,
          WD_FLAG,
          POORSRC_FLAG,
          PG_FLAG,
          AC2_PRESENT_FLAG,
          AC1_PRESENT_FLAG,
          VBUS_PRESENT_FLAG,
          CHG_FLAG,
          ICO_FLAG,
          VBUS_FLAG,
          TREG_FLAG,
          VBAT_PRESENT_FLAG,
          BC1_2_DONE_FLAG,
          DPDM_DONE_FLAG,
          ADC_DONE_FLAG,
          VSYS_FLAG,
          CHG_TMR_FLAG,
          TRICHG_TMR_FLAG,
          PRECHG_TMR_FLAG,
          TOPOFF_TMR_FLAG,
          VBATOTG_LOW_FLAG,
          TS_COLD_FLAG,
          TS_COOL_FLAG,
          TS_WARM_FLAG,
          TS_HOT_FLAG,
          IBAT_REG_FLAG,
          VBUS_OVP_FLAG,
          VBAT_OVP_FLAG,
          IBUS_OCP_FLAG,
          IBAT_OCP_FLAG,
          CONV_OCP_FLAG,
          VAC2_OVP_FLAG,
          VAC1_OVP_FLAG,
          VSYS_SHORT_FLAG,
          VSYS_OVP_FLAG,
          OTG_OVP_FLAG,
          OTG_UVP_FLAG,
          TSHUT_FLAG,
          ADC_EN,
          ADC_RATE,
          ADC_SAMPLE,
          ADC_AVG,
          ADC_AVG_INIT,
          IBUS_ADC_DIS,
          IBAT_ADC_DIS,
          VBUS_ADC_DIS,
          VBAT_ADC_DIS,
          VSYS_ADC_DIS,
          TS_ADC_DIS,
          TDIE_ADC_DIS,
          DPLUS_ADC_DIS,
          DMINUS_ADC_DIS,
          VAC2_ADC_DIS,
          VAC1_ADC_DIS,
          IBUS_ADC,
          IBAT_ADC,
          VBUS_ADC,
          VAC1_ADC,
          VAC2_ADC,
          VBAT_ADC,
          VSYS_ADC,
          TS_ADC,
          TDIE_ADC,
          DPLUS_ADC,
          DMINUS_ADC,
          DPLUS_DAC,
          DMINUS_DAC,
          PN,
          DEV_REV
          //
      }} {
  _chip_address = chip_address;
  _clearRegs();
}

BQ25798::BQ25798() : BQ25798(DEFAULT_I2C_ADDRESS) {}

BQ25798::RegisterDefinition BQ25798::getRegisterDefinition(regaddr_t address) {
  if (address >= PHYSICAL_REGISTERS_COUNT) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), address);
    return RegisterDefinition::invalid();
  }
  RegisterDefinition reg_def = _registerDefinitions[address];

  if (reg_def.address != address) {
    ERROR_PRINT(F("Invalid register definition 0x%02X: register 0x%02X references, probably skipped something in the .h file\n"), address, reg_def.address);
    return RegisterDefinition::invalid();
  }

  return reg_def;
};

BQ25798::Setting BQ25798::getSetting(uint16_t id) {
  if (id >= SETTINGS_COUNT) {
    ERROR_PRINT(F("Invalid setting ID %d\n"), id);
    return Setting::invalid();
  }

  return _settingsList[id];
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

uint16_t BQ25798::getRaw(Setting setting) {
  uint16_t raw_value = 0;  // getReg* always returns unsigned values, so we need to use unsigned type here

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Invalid register address 0x%02X\n"), setting.reg);
    return -1;
  };

  DEBUG_PRINT("[getRaw] setting=%s [reg=0x%02X (%s), bitMask=0x%02X, bitShift=%d]\n", setting.name, setting.reg, reg_def.name, setting.mask, setting.shift);

  if (reg_def.size == regsize_t::SHORT) {
    raw_value = getReg8(setting.reg, setting.mask, setting.shift);
  } else if (reg_def.size == regsize_t::LONG) {
    raw_value = getReg16(setting.reg, setting.mask, setting.shift);
  }
  DEBUG_PRINT("[getRaw] -> raw 0x%04X\n", raw_value);

  return raw_value;
};

int BQ25798::getInt(Setting setting) {
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

void BQ25798::setInt(Setting setting, int value) {
  // Serial.printf("[DEBUG] setInt(reg=0x%02X, bitMask=0x%02X, bitShift=%d, value=%d)\n", setting.reg, setting.mask, setting.shift, value);

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);
  if (!reg_def.isValid()) {
    ERROR_PRINT(F("Error: Invalid register address 0x%02X\n"), setting.reg);
    return;
  };

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

float BQ25798::getFloat(Setting setting) {
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
