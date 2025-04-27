#include <Arduino.h>
#include <BQ25798.h>
#include <Wire.h>

#include <array>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

const int ADC_readout_time_millis = 30000;

BQ25798 bq25798 = BQ25798();

std::array<int, BQ25798::SETTINGS_COUNT> oldRawValues;
std::array<int, BQ25798::SETTINGS_COUNT> newRawValues;
long startMillis = 0;
long lastADCReadMillis = 0;

void trackChanges() {
  bq25798.readAll();

  for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    BQ25798::Setting setting = bq25798.getSetting(i);
    newRawValues[i] = bq25798.getRaw(setting);
  }

  bool justStarted = false;
  if (startMillis == 0) {
    justStarted = true;
    startMillis = millis();
    lastADCReadMillis = startMillis;

    for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
      oldRawValues[i] = 0xFFFF;  // set to invalid value
    }

    // First time just copy the values
    // Serial.println("First time reading BQ25798 settings...");
    // for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    //   oldRawValues[i] = newRawValues[i];
    // }
    // Serial.println("Waiting for changes...");
    // return;
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
      Serial.printf("[T+%7.3fs] %20s ", elapsedMillis / 1000.0f, setting.name);
      if (setting.type == BQ25798::settings_type_t::FLOAT) {
        Serial.printf("(float) = %-50.3f     ",  //
                      bq25798.rawToFloat(newRawValues[i], setting));
        if (!justStarted) {
          Serial.printf("(was %.3f)",  //
                        bq25798.rawToFloat(oldRawValues[i], setting));
        }
      } else if (setting.type == BQ25798::settings_type_t::BOOL) {
        Serial.printf("(bool)  = %-50s     ",  //
                      bq25798.rawToBool(newRawValues[i], setting) ? "TRUE" : "false");
        if (!justStarted) {
          Serial.printf("(was %s)",  //
                        bq25798.rawToBool(oldRawValues[i], setting) ? "TRUE" : "false");
        }
      } else if (setting.type == BQ25798::settings_type_t::ENUM) {
        Serial.printf("(enum)  = [%d] \"%s\"%*s",  //
                      newRawValues[i], bq25798.rawToString(newRawValues[i], setting), 50 - 1 - strlen(bq25798.rawToString(newRawValues[i], setting)), "");
        if (!justStarted) {
          Serial.printf("(was [%d] \"%s\")\n",  //
                        oldRawValues[i], bq25798.rawToString(oldRawValues[i], setting));
        };
      } else if (setting.type == BQ25798::settings_type_t::INT) {
        Serial.printf("(int)   = %-50d     ",  //
                      bq25798.rawToInt(newRawValues[i], setting));
        if (!justStarted) {
          Serial.printf("(was %5d)",  //
                        bq25798.rawToInt(oldRawValues[i], setting));
        }
      }
      Serial.println();
    }
  }
  if (changed) {
    Serial.println();  // group the changes
  }

  // update the old values
  for (int i = 0; i < BQ25798::SETTINGS_COUNT; i++) {
    oldRawValues[i] = newRawValues[i];
  }

  // Every 1 second read the ADC values in one-shot mode
  long elapsedADCMillis = millis() - lastADCReadMillis;
  if (elapsedADCMillis >= ADC_readout_time_millis) {
    // Serial.printf("ADC readout elapsed time: %ld ms\n", elapsedADCMillis);
    lastADCReadMillis = millis();
    bq25798.setAndWriteBool(bq25798.ADC_EN, 1);  // trigger ADC one-shot mode
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial port initialized");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.printf("I2C initialized on SDA=GPIO%d, SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.println("Looking for BQ25798 on I2C bus...");
  while (!bq25798.begin()) {
    delay(1);
  }

  Serial.println("Connected. Setting up BQ25798...");

  // Disable watchdog timer (it would otherwise reset the chip if not cleared in time):
  bq25798.setAndWriteEnum<BQ25798::watchdog_t>(bq25798.WATCHDOG, BQ25798::watchdog_t::WATCHDOG_DISABLE);

  // Enable ADC one shot mode. ADC_EN will be set to 0 after the readout is done.
  // A continuous ADC would otherwise produce too much visual noise (a lot of changes).
  bq25798.setAndWriteEnum<BQ25798::adc_rate_t>(bq25798.ADC_RATE, BQ25798::adc_rate_t::ONESHOT);
  bq25798.setAndWriteBool(bq25798.ADC_EN, 1);  // trigger ADC one-shot mode
  delay(10);

  // Enable backup mode
  // bq25798.setAndWriteBool(bq25798.EN_BACKUP, 1);

  Serial.println("BQ25798 setup complete. Waiting for changes...");
}

void loop() {
  trackChanges();
  delay(100);
  // bq25798.setAndWriteBool(bq25798.EN_BACKUP, 1);  // enable backup mode again
  // bq25798.setAndWriteBool(bq25798.EN_OTG, 1);      // enable OTG mode
}
