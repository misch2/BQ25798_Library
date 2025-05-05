#ifndef I2C_WIRE_H
#define I2C_WIRE_H

#include <Wire.h>

#include "I2CGeneric.h"

class I2CWire : public I2CGeneric {
 public:
  I2CWire(TwoWire& wire = Wire);

  bool i2c_begin(uint8_t chip_address) override;
  void i2c_end() override;
  bool i2c_isConnected() override;
  uint8_t i2c_readReg8(int reg) override;
  void i2c_writeReg8(int reg, uint8_t value) override;
  uint16_t i2c_readReg16(int reg) override;
  void i2c_writeReg16(int reg, uint16_t value) override;

 private:
  TwoWire* _wire;
  uint8_t _chip_address = 0;
};

#endif
