#include "BQ25798.h"

BQ25798::BQ25798(TwoWire& wire, Stream& serialPort) : BQ25798Core<I2CWire>() { _serialPort = &serialPort; }
