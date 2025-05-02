#include "BQ25798CoreMocked.h"

BQ25798CoreMocked::BQ25798CoreMocked() : BQ25798Core() {}

bool BQ25798CoreMocked::writeReg8ToI2C(int reg) {
  return true;  // Mocked function, always returns true
}

bool BQ25798CoreMocked::writeReg16ToI2C(int reg) {
  return true;  // Mocked function, always returns true;
}