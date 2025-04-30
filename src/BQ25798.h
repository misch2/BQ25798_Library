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
  BQ25798();

  bool begin(uint8_t chip_address = DEFAULT_I2C_ADDRESS, TwoWire& wire = Wire, Stream& serialPort = Serial);
  bool readAll();
  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

  bool setAndWriteRaw(const BQ25798Core::Setting& setting, uint16_t value);
  bool setAndWriteInt(const BQ25798Core::Setting& setting, int value);
  bool setAndWriteBool(const BQ25798Core::Setting& setting, bool value);
  bool setAndWriteFloat(const BQ25798Core::Setting& setting, float value);

  template <typename T>
  bool setAndWriteEnum(const Setting& setting, T value) {
    // the same as int, but with enum value
    return setAndWriteInt(setting, static_cast<int>(value));
  };

 private:
  TwoWire* _i2cPort;
  Stream* _serialPort;
  int _chip_address;
};

#endif