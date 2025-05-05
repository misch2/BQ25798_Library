#ifndef I2C_MOCK_TEST_H
#define I2C_MOCK_TEST_H

#include "I2CGeneric.h"

constexpr int MAX_REGISTERS_COUNT = 256;  // Maximum number of registers

class I2CMockTest : public I2CGeneric {
 public:
  I2CMockTest();

  bool i2c_begin(uint8_t chip_address) override;
  void i2c_end() override;
  bool i2c_isConnected() override;
  uint8_t i2c_readReg8(int reg) override;
  void i2c_writeReg8(int reg, uint8_t value) override;
  uint16_t i2c_readReg16(int reg) override;
  void i2c_writeReg16(int reg, uint16_t value) override;

 private:
  uint8_t _reg8Values[MAX_REGISTERS_COUNT];
};

#endif
