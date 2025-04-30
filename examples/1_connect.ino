#include <BQ25798.h>
#include <Wire.h>

BQ25798 bq25798 = BQ25798();

void setup() {
  Serial.begin(115200);
  Wire.begin();

  bq25798.begin();

  Serial.printf("Part number: %s\n", bq25798.getString(bq25798.PN));
  Serial.printf("Chip revision: %s\n", bq25798.getString(bq25798.DEV_REV));
}

void loop() {}
