#ifndef I2C_WIRE_H
#define I2C_WIRE_H

#include <Wire.h>

#include "../lib/I2CGeneric/I2CGeneric.h"

class I2CWire : public I2CGeneric {
 public:
  I2CWire(TwoWire& wire = Wire);

  bool i2c_begin(uint8_t chip_address) override;
  void i2c_end() override;
  bool i2c_isConnected() override;
  void i2c_readBytes(int startReg, int count, uint8_t *values) override;
  void i2c_writeBytes(int startReg, int count, const uint8_t *values) override;

 private:
  TwoWire* _wire;
  uint8_t _chip_address = 0;
};

#endif
