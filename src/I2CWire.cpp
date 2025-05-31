#include "I2CWire.h"

#include <Arduino.h>

#include "../src/utility/debug.h"

I2CWire::I2CWire(TwoWire &wire) : I2CGeneric() {
  _wire = &wire;
  _chip_address = 0;
}

bool I2CWire::i2c_begin(uint8_t chip_address) {
  _chip_address = chip_address;

  _clearI2CError();
  if (!_wire->begin()) {
    DEBUG_PRINT("[i2c_begin] I2C begin failed!\n");
    _setI2CError();
    return false;
  }
  if (!i2c_isConnected()) {
    DEBUG_PRINT("[i2c_begin] I2C device not found!\n");
    _setI2CError();
    return false;
  }

  return true;
}

void I2CWire::i2c_end() {}

bool I2CWire::i2c_isConnected() {
  _wire->beginTransmission(_chip_address);
  return (_wire->endTransmission(true) == 0);
}

void I2CWire::i2c_readBytes(int startReg, int count, uint8_t *values) {
  _clearI2CError();

  // Send the register address to read from
  _wire->beginTransmission(_chip_address);
  _wire->write(startReg);
  if (_wire->endTransmission() != 0) {
    DEBUG_PRINT("[i2c_readReg8] I2C endTransmission failed!\n");
    _setI2CError();
  }

  // Ask for count bytes and read it into the buffer
  if (_wire->requestFrom(_chip_address, (size_t)count, true) != count) {  // Read count bytes and release the bus
    DEBUG_PRINT("[i2c_readReg8] I2C requestFrom failed!\n");
    _setI2CError();
  }

  // Get the results from the buffer
  for (int i = 0; i < count; i++) {
    if (_wire->available()) {
      values[i] = _wire->read();
    } else {
      DEBUG_PRINT("[i2c_readReg8] Not enough data available!\n");
      _setI2CError();
      return;
    }
  }
}

void I2CWire::i2c_writeBytes(int startReg, int count, const uint8_t *values) {
  _clearI2CError();

  _wire->beginTransmission(_chip_address);
  if (_wire->write(startReg) != 1) {  // Send the register address
    DEBUG_PRINT("[i2c_writeReg8] I2C write 1 failed!\n");
    _setI2CError();
    return;
  }

  for (int i = 0; i < count; i++) {
    if (_wire->write(values[i]) != 1) {  // Send the value to write
      DEBUG_PRINT("[i2c_writeReg8] I2C write %d failed!\n", i + 2);
      _setI2CError();
      return;
    }
  }

  if (_wire->endTransmission(true) != 0) {  // Release the bus
    DEBUG_PRINT("[i2c_writeReg8] I2C endTransmission failed!\n");
    _setI2CError();
  }
}
