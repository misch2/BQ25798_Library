#ifndef BQ25798_H
#define BQ25798_H

#include <Wire.h>

#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"
#include "BQ25798Core.h"
#include "I2CWire.h"
#include "utility/debug.h"

class BQ25798 : public BQ25798Core<I2CWire> {
 public:
  BQ25798(TwoWire& wire = Wire, Stream& serialPort = Serial);

  // bool readAll();

 private:
  Stream* _serialPort;
};

#endif