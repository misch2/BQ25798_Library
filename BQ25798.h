#ifndef BQ25798_H
#define BQ25798_H

#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"
#include "registers.h"

#define DEFAULT_I2C_ADDRESS 0x6B

class BQ25798 {
 public:
#include "settings.h"
  BQ25798();
  BQ25798(uint8_t address);

  bool begin();
  bool readAll();
  const int registersCount = 1 + MAX_REGISTER_NUMBER - MIN_REGISTER_NUMBER;

  int getInt(Setting setting);
  void setInt(Setting setting, int value);
  float getFloat(Setting setting);
  const char* toString(int value, const std::vector<std::string> map);

  bool faultDetected();

 private:
  int _address;
  uint8_t _regs[1 + MAX_REGISTER_NUMBER] = {};
  void clearRegs();
  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg, uint16_t value, int bitMask = 0xFFFF, int bitShift = 0);

  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);
};

#endif
