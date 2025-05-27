#ifndef BQ25798_NOI2C_H
#define BQ25798_NOI2C_H

#include "../BQ25798Core/BQ25798Core.h"
#include "../I2CGeneric/I2CGeneric.h"

class BQ25798NoI2C : public BQ25798Core<I2CGeneric> {
 public:
  BQ25798NoI2C() {};

  bool i2c_begin(uint8_t chip_address) override { return false; }
  void i2c_end() override {};

  bool i2c_isConnected() override { return false; }
  void i2c_readBytes(int startReg, int count, uint8_t *values) override { for (int i = 0; i < count; i++) { values[i] = 0; } };
  void i2c_writeBytes(int startReg, int count, const uint8_t *values) override {};
};

#endif