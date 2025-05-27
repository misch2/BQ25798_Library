#ifndef I2C_MOCK_TEST_H
#define I2C_MOCK_TEST_H

#include "I2CGeneric.h"

constexpr int MOCK_REGISTERS_COUNT = 256;  // Maximum number of registers

class I2CMockTest : public I2CGeneric {
 public:
  I2CMockTest();

  bool i2c_begin(uint8_t chip_address) override;
  void i2c_end() override;
  bool i2c_isConnected() override;
  void i2c_readBytes(int startReg, int count, uint8_t *values) override;
  void i2c_writeBytes(int startReg, int count, const uint8_t *values) override;

 private:
  uint8_t _reg8Values[MOCK_REGISTERS_COUNT];
};

#endif
