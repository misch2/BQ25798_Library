#ifndef BQ25798_CORE_MOCKED_H
#define BQ25798_CORE_MOCKED_H

#include "BQ25798Core.h"

class BQ25798CoreMocked : public BQ25798Core {
 public:
  BQ25798CoreMocked();

  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);
};

#endif