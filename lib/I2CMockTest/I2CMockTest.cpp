#include "I2CMockTest.h"

I2CMockTest::I2CMockTest() : I2CGeneric() {}

bool I2CMockTest::i2c_begin(uint8_t chip_address) {
  // Initialize the mock I2C registers to zero
  for (int i = 0; i < MAX_REGISTERS_COUNT; i++) {
    _reg8Values[i] = 0;
  }
  return i2c_isConnected();  // Always return true for the mock implementation
}

bool I2CMockTest::i2c_isConnected() { return true; }

void I2CMockTest::i2c_end() {
  // Cleanup if needed (not necessary for a mock implementation)
}

uint8_t I2CMockTest::i2c_readReg8(int reg) {
  if (reg < 0 || reg >= MAX_REGISTERS_COUNT) {
    return 0;  // Return 0 for invalid register access
  }
  return _reg8Values[reg];
}

void I2CMockTest::i2c_writeReg8(int reg, uint8_t value) {
  if (reg < 0 || reg >= MAX_REGISTERS_COUNT) {
    return;  // Ignore invalid register access
  }
  _reg8Values[reg] = value;
}

uint16_t I2CMockTest::i2c_readReg16(int reg) {
  if (reg < 0 || reg >= MAX_REGISTERS_COUNT - 1) {
    return 0;  // Return 0 for invalid register access
  }
  return (_reg8Values[reg] << 8) | _reg8Values[reg + 1];
}

void I2CMockTest::i2c_writeReg16(int reg, uint16_t value) {
  if (reg < 0 || reg >= MAX_REGISTERS_COUNT - 1) {
    return;  // Ignore invalid register access
  }
  _reg8Values[reg] = (value >> 8) & 0xFF;  // Set the high byte
  _reg8Values[reg + 1] = value & 0xFF;     // Set the low byte
}
