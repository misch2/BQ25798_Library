#ifndef BQ25798_DEBUG_H
#define BQ25798_DEBUG_H

// FIXME remove?
// #include "Arduino.h"

#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print("[DEBUG] "); Serial.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#endif
