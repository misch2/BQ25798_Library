#include <BQ25798.h>
#include <Wire.h>

#include <array>

const int ADC_readout_time_millis = 30000;

BQ25798 bq25798 = BQ25798();

std::array<int, BQ25798::SETTINGS_COUNT> oldRawValues;
std::array<int, BQ25798::SETTINGS_COUNT> newRawValues;
long startMillis = 0;
long lastADCReadMillis = 0;

void printMostImportantStats() {
  Serial.printf("Main stats: ");

  if (bq25798.getIINDPM_STAT() == BQ25798::IINDPM_STAT_t::IINDPM_STAT_REGULATION ||
      bq25798.getVINDPM_STAT() == BQ25798::VINDPM_STAT_t::VINDPM_STAT_REGULATION) {
    Serial.printf("DPM! ");
  };
  if (bq25798.getPG_STAT() == BQ25798::PG_STAT_t::PG_STAT_BAD) {
    Serial.printf("PG! ");
  };
  if (bq25798.getEN_HIZ()) {
    Serial.printf("HIZ! ");
  };
  if (bq25798.getVSYS_STAT() == BQ25798::VSYS_STAT_t::VSYS_STAT_IN_VSYSMIN_REGULATION) {
    Serial.printf("VSYS! ");
  };
  if (bq25798.getVBUS_PRESENT_STAT() == BQ25798::VBUS_PRESENT_STAT_t::VBUS_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("VBUS! ");
  };
  if (bq25798.getAC1_PRESENT_STAT() == BQ25798::AC1_PRESENT_STAT_t::AC1_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("AC1! ");
  };
  if (bq25798.getVBAT_PRESENT_STAT() == BQ25798::VBAT_PRESENT_STAT_t::VBAT_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("VBAT! ");
  };

  if (bq25798.getTS_IGNORE()) {
    Serial.printf("+TS_IGNORE ");
  };
  if (bq25798.getEN_BACKUP()) {
    Serial.printf("+EN_BACKUP ");
  };
  if (bq25798.getEN_OTG()) {
    Serial.printf("+EN_OTG ");
  };
  if (bq25798.getEN_CHG()) {
    Serial.printf("+EN_CHG ");
  };
  if (bq25798.getEN_ACDRV1()) {
    Serial.printf("+EN_ACDRV1 ");
  };
  Serial.printf("CHG=[%d]\"%s\" ", bq25798.getInt(bq25798.CHG_STAT), bq25798.getString(bq25798.CHG_STAT));

  Serial.println();
}

void trackChanges() {
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
          Serial.printf("(was [%d] \"%s\")",  //
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
    printMostImportantStats();
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
    bq25798.setADC_EN(true);  // trigger ADC one-shot mode
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial port initialized");

  Wire.begin();
  Serial.print("Looking for BQ25798 on I2C bus...");
  while (!bq25798.begin()) {
    delay(100);
  }
  bq25798.clearError();

  Serial.println("Connected.");

  Serial.print("Resetting the IC completely...");
  // Reset the chip and wait for it to finish:
  bq25798.setREG_RST(true);
  while (bq25798.getREG_RST()) {
    delay(10);
  }
  Serial.println("Reset successful.");

  Serial.print("Setting up BQ25798...");
  // Disable watchdog timer (it would otherwise reset the chip if not cleared in time):
  bq25798.setWATCHDOG(BQ25798::WATCHDOG_t::WATCHDOG_DISABLE);

  // Enable ADC one shot mode. ADC_EN will be set to 0 after the readout is done.
  // A continuous ADC would otherwise produce too much visual noise (a lot of changes).
  bq25798.setADC_RATE(BQ25798::ADC_RATE_t::ADC_RATE_ONESHOT);
  bq25798.setADC_EN(true);  // trigger ADC one-shot mode
  delay(10);

  Serial.println("BQ25798 setup complete.");
  Serial.println("Waiting for changes...");
}

void loop() {
  trackChanges();
  delay(100);
}
