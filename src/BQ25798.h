#ifndef BQ25798_H
#define BQ25798_H

#include <Wire.h>

#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"
#include "utility/BQ25798Core.h"
#include "utility/debug.h"

#define DEFAULT_I2C_ADDRESS 0x6B

class BQ25798 : public BQ25798Core {
 public:
  BQ25798(uint8_t chip_address = DEFAULT_I2C_ADDRESS, TwoWire& wire = Wire, Stream& serialPort = Serial);

  bool begin();
  bool readAll();

  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

 private:
  TwoWire* _i2cPort;
  Stream* _serialPort;
  uint8_t _chip_address;
};

#endif