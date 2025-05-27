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
  virtual void i2c_readBytes(int startReg, int count, uint8_t *values) = 0;
  virtual void i2c_writeBytes(int startReg, int count, const uint8_t *values) = 0;

  bool i2c_isError() const { return _isI2CError; }  // Check if an error occurred

 protected:
  bool _isI2CError = false;
  void _clearI2CError() { _isI2CError = false; }
  void _setI2CError() { _isI2CError = true; }
};

#endif