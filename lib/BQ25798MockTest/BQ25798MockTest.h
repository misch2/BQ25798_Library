#ifndef BQ25798_MOCKTEST_H
#define BQ25798_MOCKTEST_H

#include "../BQ25798Core/BQ25798Core.h"
#include "../I2CMockTest/I2CMockTest.h"

class BQ25798MockTest : public BQ25798Core<I2CMockTest> {
 public:
  BQ25798MockTest() {};
};

#endif