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
  uint8_t i2c_readReg8(int reg) override { return 0; };
  void i2c_writeReg8(int reg, uint8_t value) override {};
  uint16_t i2c_readReg16(int reg) override { return 0; };
  void i2c_writeReg16(int reg, uint16_t value) {};
};

#endif