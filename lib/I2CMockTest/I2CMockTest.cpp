#include "I2CMockTest.h"

I2CMockTest::I2CMockTest() : I2CGeneric() {}

bool I2CMockTest::i2c_begin(uint8_t chip_address) {
  // Initialize the mock I2C registers to zero
  for (int i = 0; i < MOCK_REGISTERS_COUNT; i++) {
    _reg8Values[i] = 0;
  }
  return i2c_isConnected();  // Always return true for the mock implementation
}

bool I2CMockTest::i2c_isConnected() { return true; }

void I2CMockTest::i2c_end() {
  // Cleanup if needed (not necessary for a mock implementation)
}

void I2CMockTest::i2c_readBytes(int startReg, int count, uint8_t *values) {
  // printf("i2c_readBytes(%d, %d, %p) -> reg8values are %02x %02x %02x %02x\n", startReg, count, values, _reg8Values[0], _reg8Values[1], _reg8Values[2],
  //        _reg8Values[3]);
  for (int i = 0; i < count; i++) {
    if (startReg + i < 0 || startReg + i >= MOCK_REGISTERS_COUNT) {
      values[i] = 0xFF;  // invalid register access
    } else {
      values[i] = _reg8Values[startReg + i];
    }
  }
}

void I2CMockTest::i2c_writeBytes(int startReg, int count, const uint8_t *values) {
  for (int i = 0; i < count; i++) {
    if (startReg + i >= 0 && startReg + i < MOCK_REGISTERS_COUNT) {
      _reg8Values[startReg + i] = values[i];
    }
  }
  // printf("i2c_writeBytes(%d, %d, %p) -> reg8values are %02x %02x %02x %02x\n", startReg, count, values, _reg8Values[0], _reg8Values[1], _reg8Values[2],
  //        _reg8Values[3]);
}
