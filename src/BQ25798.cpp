#include "BQ25798.h"

#include <Wire.h>

#include "BQ25798Core.h"

BQ25798::BQ25798(uint8_t chip_address) : BQ25798Core() {
  _chip_address = chip_address;
  _clearRegs();
}

BQ25798::BQ25798() : BQ25798(DEFAULT_I2C_ADDRESS) {}

void BQ25798::begin() { readAll(); }

void BQ25798::readAll() {
  DEBUG_PRINT(F("[readAll] Reading all BQ25798 registers\n"));

  Wire.beginTransmission(_chip_address);
  Wire.write(REG00_Minimal_System_Voltage);  // Start reading from the first register
  Wire.endTransmission();
  if (Wire.requestFrom(_chip_address, PHYSICAL_REGISTERS_COUNT) != PHYSICAL_REGISTERS_COUNT) {
    _setErrorMessage(F("Error batch reading all (%d) BQ25798 registers\n"), PHYSICAL_REGISTERS_COUNT);
    return;
  }

  for (int i = 0; i < PHYSICAL_REGISTERS_COUNT; i++) {
    _physicalReg8Values[i] = Wire.read();
  }

  DEBUG_PRINT(F("[readAll] -> success\n"));
  return;
}

void BQ25798::writeReg8ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  DEBUG_PRINT(F("[writeReg8ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X\n", reg, reg_def.name, _physicalReg8Values[reg]));
#endif

  Wire.beginTransmission(_chip_address);
  if (Wire.write(reg) != 1) {
    _setErrorMessage(F("Error writing to BQ25798 register %02X\n"), reg);
    return;
  }
  if (Wire.write(_physicalReg8Values[reg]) != 1) {
    _setErrorMessage(F("Error writing to BQ25798 register %02X\n"), reg);
    return;
  }
  Wire.endTransmission();

  DEBUG_PRINT(F("[writeReg8ToI2C] -> success\n"));
  return;
}

void BQ25798::writeReg16ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  DEBUG_PRINT(F("[writeReg16ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X 0x%02X\n"), reg, reg_def.name, _physicalReg8Values[reg],
              _physicalReg8Values[reg + 1]);
#endif

  Wire.beginTransmission(_chip_address);
  if (Wire.write(reg) != 1) {
    _setErrorMessage(F("Error writing to BQ25798 register %02X\n"), reg);
    return;
  }
  if (Wire.write(_physicalReg8Values[reg]) != 1) {
    _setErrorMessage(F("Error writing to BQ25798 register %02X\n"), reg);
    return;
  }
  if (Wire.write(_physicalReg8Values[reg + 1]) != 1) {
    _setErrorMessage(F("Error writing to BQ25798 register %02X\n"), reg);
    return;
  }
  Wire.endTransmission();

  DEBUG_PRINT(F("[writeReg16ToI2C] -> success\n"));
  return;
}

void BQ25798::setAndWriteRaw(const Setting& setting, uint16_t value) {
  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

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
}

void BQ25798::setAndWriteInt(const Setting& setting, int value) {
  uint16_t raw_value = intToRaw(value, setting);
  setAndWriteRaw(setting, raw_value);
}

void BQ25798::setAndWriteBool(const Setting& setting, bool value) {
  uint16_t raw_value = boolToRaw(value, setting);
  setAndWriteRaw(setting, raw_value);
}

void BQ25798::setAndWriteFloat(const Setting& setting, float value) {
  uint16_t raw_value = floatToRaw(value, setting);
  setAndWriteRaw(setting, raw_value);
}
