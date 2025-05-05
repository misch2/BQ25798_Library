#include <BQ25798.h>
#include <Wire.h>

BQ25798 bq25798 = BQ25798();

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!bq25798.i2c_begin(BQ25798_DEFAULT_ADDRESS)) {
    Serial.println("BQ25798 not found!");
    return;
  }

  Serial.printf("Part number: %s\n", bq25798.getPN_string());
  Serial.printf("Chip revision: %s\n", bq25798.getDEV_REV_string());
}

void loop() {}
