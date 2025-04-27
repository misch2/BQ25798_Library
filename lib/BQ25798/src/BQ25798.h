#ifndef BQ25798_H
#define BQ25798_H

#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"
#include "BQ25798Core.h"
#include "debug.h"

#define DEFAULT_I2C_ADDRESS 0x6B

class BQ25798 : public BQ25798Core {
 public:
  BQ25798();
  BQ25798(uint8_t address);

  bool begin();
  bool readAll();
  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

  void setAndWriteRaw(const BQ25798Core::Setting& setting, uint16_t value);
  void setAndWriteInt(const BQ25798Core::Setting& setting, int value);
  void setAndWriteBool(const BQ25798Core::Setting& setting, bool value);
  void setAndWriteFloat(const BQ25798Core::Setting& setting, float value);

  template <typename T>
  void setAndWriteEnum(const Setting& setting, T value) {
    // the same as int, but with enum value
    setAndWriteInt(setting, static_cast<int>(value));
  };

 private:
  int _chip_address;
};

#endif