#ifndef BQ25798_NOI2C_H
#define BQ25798_NOI2C_H

#include "../BQ25798Core/BQ25798Core.h"
#include "../I2CGeneric/I2CGeneric.h"

class BQ25798NoI2C : public BQ25798Core<I2CGeneric> {
 public:
  BQ25798NoI2C() {};
};

#endif