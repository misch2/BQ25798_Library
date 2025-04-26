#include <Arduino.h>
#include <BQ25798.h>
#include <Wire.h>

#include <array>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

BQ25798 bq25798 = BQ25798();

std::array<int, BQ25798::SETTINGS_COUNT> oldRawValues;
std::array<int, BQ25798::SETTINGS_COUNT> newRawValues;
long startMillis = 0;
void trackChanges() {
  bq25798.readAll();

  for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    BQ25798::Setting setting = bq25798.getSetting(i);
    newRawValues[i] = bq25798.getRaw(setting);
  }

  // first time, just copy the values
  if (startMillis == 0) {
    Serial.println("First time reading BQ25798 settings...");
    for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
      oldRawValues[i] = newRawValues[i];
    }
    startMillis = millis();
    Serial.println("Waiting for changes...");
    return;
  }

  // every next time check if the values changed
  long elapsedMillis = millis() - startMillis;
  bool changed = false;
  for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    if (oldRawValues[i] != newRawValues[i]) {
      BQ25798::Setting setting = bq25798.getSetting(i);

      // exception: do not notify about flags set to FALSE because any read operation will reset them to FALSE
      if (setting.type == BQ25798::settings_type_t::BOOL && strstr(setting.name, "_FLAG") != nullptr) {
        if (newRawValues[i] == 0) {
          continue;
        }
      }

      changed = true;
      Serial.printf("[T+%-6.3f] %20s ", elapsedMillis / 1000.0f, setting.name);
      if (setting.type == BQ25798::settings_type_t::FLOAT) {
        Serial.printf("(float) = %-50.3f    (was %.3f)\n",           //
                      bq25798.rawToFloat(newRawValues[i], setting),  //
                      bq25798.rawToFloat(oldRawValues[i], setting));
      } else if (setting.type == BQ25798::settings_type_t::BOOL) {
        Serial.printf("(bool)  = %-50s     (was %s)\n",                                //
                      bq25798.rawToBool(newRawValues[i], setting) ? "TRUE" : "false",  //
                      bq25798.rawToBool(oldRawValues[i], setting) ? "TRUE" : "false");
      } else if (setting.type == BQ25798::settings_type_t::ENUM) {
        Serial.printf("(enum)  = [%d] \"%-48s\" (was [%d] \"%s\")\n",                  //
                      newRawValues[i], bq25798.rawToString(newRawValues[i], setting),  //
                      oldRawValues[i], bq25798.rawToString(oldRawValues[i], setting));
      } else if (setting.type == BQ25798::settings_type_t::INT) {
        Serial.printf("(int)   = %-50d     (was %5d)\n",           //
                      bq25798.rawToInt(newRawValues[i], setting),  //
                      bq25798.rawToInt(oldRawValues[i], setting));
      }
    }
  }
  if (changed) {
    Serial.println();  // group the changes
  }

  // update the old values
  for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    oldRawValues[i] = newRawValues[i];
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial port initialized");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.printf("I2C initialized on SDA=GPIO%d, SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

  if (!bq25798.begin()) {
    Serial.println("Failed to initialize BQ25798, stopping here.");
    while (1) {
      delay(1000);
    }
  }

  bq25798.setAndWriteEnum(bq25798.WATCHDOG, static_cast<int>(BQ25798::watchdog_t::WATCHDOG_DISABLE));
  // bq25798.setAndWriteBool(bq25798.ADC_EN, 1);
  // bq25798.setAndWriteEnum(bq25798.ADC_RATE, static_cast<int>(BQ25798::adc_rate_t::ADC_RATE_CONTINUOUS));
  // bq25798.setAndWriteEnum(bq25798.ADC_SAMPLE, static_cast<int>(BQ25798::adc_sample_t::ADC_SAMPLE_15BIT));
  // bq25798.setAndWriteEnum(bq25798.ADC_AVG, static_cast<int>(BQ25798::adc_avg_t::RUNNING_AVERAGE));
  bq25798.setAndWriteBool(bq25798.ADC_EN, 0);  // ADC produces too much noise (a lot of changes)
}

void loop() {
  trackChanges();
  delay(100);
}
