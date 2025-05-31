#ifndef BQ25798_H
#define BQ25798_H

#include <Wire.h>

#include <cstdint>
#include <string>
#include <vector>

#include "../lib/BQ25798Core/BQ25798Core.h"
#include "Arduino.h"
#include "I2CWire.h"
#include "utility/debug.h"

class BQ25798 : public BQ25798Core<I2CWire> {
 public:
  BQ25798(TwoWire& wire = Wire, Stream& serialPort = Serial);

 private:
  Stream* _serialPort;
};

#endif