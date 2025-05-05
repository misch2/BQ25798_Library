#include "I2CWire.h"

#include <Arduino.h>

#include "../src/utility/debug.h"

I2CWire::I2CWire(TwoWire& wire) : I2CGeneric() {
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

uint8_t I2CWire::i2c_readReg8(int reg) {
  _clearI2CError();

  // Send the register address to read from
  _wire->beginTransmission(_chip_address);
  _wire->write(reg);
  if (_wire->endTransmission() != 0) {
    DEBUG_PRINT("[i2c_readReg8] I2C endTransmission failed!\n");
    _setI2CError();
    return 0;
  }

  // Ask for 1 byte and read it into the buffer
  if (_wire->requestFrom(_chip_address, (size_t)1, true) != 1) {  // Read 1 byte and release the bus
    DEBUG_PRINT("[i2c_readReg8] I2C requestFrom failed!\n");
    _setI2CError();
    return 0;
  }

  // Get the results from the buffer
  uint8_t value = _wire->read();

  return value;
}

void I2CWire::i2c_writeReg8(int reg, uint8_t value) {
  _clearI2CError();

  _wire->beginTransmission(_chip_address);
  if (_wire->write(reg) != 1) {  // Send the register address
    DEBUG_PRINT("[i2c_writeReg8] I2C write 1 failed!\n");
    _setI2CError();
    return;
  }
  if (_wire->write(value) != 1) {  // Send the value to write
    DEBUG_PRINT("[i2c_writeReg8] I2C write 2 failed!\n");
    _setI2CError();
    return;
  }
  if (_wire->endTransmission(true) != 0) {  // Release the bus
    DEBUG_PRINT("[i2c_writeReg8] I2C endTransmission failed!\n");
    _setI2CError();
  }
}

uint16_t I2CWire::i2c_readReg16(int reg) {
  _clearI2CError();

  // Send the register address to read from
  _wire->beginTransmission(_chip_address);
  _wire->write(reg);
  if (_wire->endTransmission() != 0) {
    DEBUG_PRINT("[i2c_readReg16] I2C endTransmission failed!\n");
    _setI2CError();
    return 0;
  }

  // Ask for 2 bytes and read them into the buffer
  if (_wire->requestFrom(_chip_address, (size_t)2, true) != 2) {  // Read 2 bytes and release the bus
    DEBUG_PRINT("[i2c_readReg16] I2C requestFrom failed!\n");
    _setI2CError();
    return 0;
  }

  // Get the results from the buffer
  uint16_t value = (_wire->read() << 8) | _wire->read();  // Combine high and low bytes

  return value;
}

void I2CWire::i2c_writeReg16(int reg, uint16_t value) {
  _clearI2CError();

  _wire->beginTransmission(_chip_address);
  if (_wire->write(reg) != 1) {  // Send the register address
    DEBUG_PRINT("[i2c_writeReg16] I2C write 1 failed!\n");
    _setI2CError();
    return;
  }
  if (_wire->write((value >> 8) & 0xFF) != 1) {  // Send the high byte
    DEBUG_PRINT("[i2c_writeReg16] I2C write 2 failed!\n");
    _setI2CError();
    return;
  }
  if (_wire->write(value & 0xFF) != 1) {  // Send the low byte
    DEBUG_PRINT("[i2c_writeReg16] I2C write 3 failed!\n");
    _setI2CError();
    return;
  }
  if (_wire->endTransmission(true) != 0) {  // Release the bus
    DEBUG_PRINT("[i2c_writeReg16] I2C endTransmission failed!\n");
    _setI2CError();
  }
}
