#include "BQ25798.h"

#include "utility/BQ25798Core.h"

BQ25798::BQ25798(uint8_t chip_address, TwoWire& wire, Stream& serialPort) : BQ25798Core() {
  _chip_address = chip_address;
  _i2cPort = &wire;
  _serialPort = &serialPort;

  _clearRegs();
}

bool BQ25798::begin() { return readAll(); }

bool BQ25798::readAll() {
  DEBUG_PRINT(F("[readAll] Reading all BQ25798 registers\n"));

  _i2cPort->beginTransmission(_chip_address);
  _i2cPort->write(REG00_Minimal_System_Voltage);  // Start reading from the first register
  _i2cPort->endTransmission();
  if (_i2cPort->requestFrom(_chip_address, PHYSICAL_REGISTERS_COUNT) != PHYSICAL_REGISTERS_COUNT) {
    _setError(ERROR_I2C_READ);
    return false;
  }

  for (int i = 0; i < PHYSICAL_REGISTERS_COUNT; i++) {
    _physicalReg8Values[i] = _i2cPort->read();
  }

  DEBUG_PRINT(F("[readAll] -> success\n"));
  return true;
}

bool BQ25798::writeReg8ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  DEBUG_PRINT(F("[writeReg8ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X\n", reg, reg_def.name, _physicalReg8Values[reg]));
#endif

  _i2cPort->beginTransmission(_chip_address);
  if (_i2cPort->write(reg) != 1) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }
  if (_i2cPort->write(_physicalReg8Values[reg]) != 1) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }
  if (!_i2cPort->endTransmission()) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }

  DEBUG_PRINT(F("[writeReg8ToI2C] -> success\n"));
  return true;
}

bool BQ25798::writeReg16ToI2C(int reg) {
#ifdef DEBUG
  RegisterDefinition reg_def = getRegisterDefinition(reg);
  DEBUG_PRINT(F("[writeReg16ToI2C] Writing to BQ25798 register 0x%02X (%s): 0x%02X 0x%02X\n"), reg, reg_def.name, _physicalReg8Values[reg],
              _physicalReg8Values[reg + 1]);
#endif

  _i2cPort->beginTransmission(_chip_address);
  if (_i2cPort->write(reg) != 1) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }
  if (_i2cPort->write(_physicalReg8Values[reg]) != 1) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }
  if (_i2cPort->write(_physicalReg8Values[reg + 1]) != 1) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }

  if (!_i2cPort->endTransmission()) {
    _setError(ERROR_I2C_WRITE);
    return false;
  }

  DEBUG_PRINT(F("[writeReg16ToI2C] -> success\n"));
  return true;
}

