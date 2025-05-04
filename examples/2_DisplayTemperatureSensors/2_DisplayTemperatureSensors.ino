#include <BQ25798.h>
#include <Wire.h>

BQ25798 bq = BQ25798();

void setup() {
  Serial.begin(115200);
  Wire.begin();

  while (!bq.begin()) {
    Serial.println("BQ25798 not found!");
    delay(1000);
  }

  Serial.println("BQ25798 found, setting up ADC");
  bq.setAndWriteADC_EN(true);
  bq.setAndWriteADC_SAMPLE(BQ25798::ADC_SAMPLE_t::ADC_SAMPLE_15BIT);
  bq.setAndWriteADC_RATE(BQ25798::ADC_RATE_t::ADC_RATE_CONTINUOUS);

  bq.setAndWriteTS_IGNORE(false);
}

void loop() {
  if (!bq.readAll()) {
    Serial.println("BQ25798 readAll failed!");
    delay(1000);
    return;
  }

  Serial.printf("Chip die temperature = %.2f degC\n", bq.getTDIE_ADC());
  Serial.printf("External thermal sensor value = %.2f degC ", bq.getTS_ADC());
  if (static_cast<bool>(bq.getTS_COOL_STAT())) {
    Serial.printf("(COOL)");
  }
  if (static_cast<bool>(bq.getTS_COLD_STAT())) {
    Serial.printf("(COLD)");
  }
  if (static_cast<bool>(bq.getTS_WARM_STAT())) {
    Serial.printf("(WARM)");
  }
  if (static_cast<bool>(bq.getTS_HOT_STAT())) {
    Serial.printf("(HOT)");
  }
  Serial.println();

  Serial.println();

  delay(1000);
}
