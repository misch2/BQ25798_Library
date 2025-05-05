#ifndef I2C_GENERIC_H
#define I2C_GENERIC_H

#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

class I2CGeneric {
 public:
  I2CGeneric() {};

  virtual bool i2c_begin(uint8_t chip_address) = 0;
  virtual void i2c_end() = 0;

  virtual bool i2c_isConnected() = 0;
  virtual uint8_t i2c_readReg8(int reg) = 0;
  virtual void i2c_writeReg8(int reg, uint8_t value) = 0;
  virtual uint16_t i2c_readReg16(int reg) = 0;
  virtual void i2c_writeReg16(int reg, uint16_t value) = 0;

  bool i2c_isError() const { return _isI2CError; }  // Check if an error occurred

 protected:
  bool _isI2CError = false;
  void _clearI2CError() { _isI2CError = false; }
  void _setI2CError() { _isI2CError = true; }
};

#endif