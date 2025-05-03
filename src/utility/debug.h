#ifndef BQ25798_DEBUG_H
#define BQ25798_DEBUG_H

#ifdef BQ25798_DEBUG
  #ifdef PIO_UNIT_TESTING
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
  #else
    #define DEBUG_PRINT(...)            \
      Serial.print("[BQ25798_DEBUG] "); \
      Serial.printf(__VA_ARGS__)
  #endif
#else
  #define DEBUG_PRINT(...)
#endif

#endif
