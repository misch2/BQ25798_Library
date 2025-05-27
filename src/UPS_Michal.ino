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

void patWatchdog() {
  // To keep the device in host mode, the host has  to reset the watchdog timer by writing 1 to WD_RST bit before the watchdog timer expires
  bq25798.setWD_RST(true);  // reset the watchdog timer
}

bool checkForError() {
  if (bq25798.lastError()) {
    Serial.printf("Error: %d\n", bq25798.lastError());
    bq25798.clearError();
    return true;
  }
  return false;
}

void printMostImportantStats() {
  Serial.printf("Main stats: ");

  if (bq25798.getIINDPM_STAT() == BQ25798::IINDPM_STAT_t::IINDPM_STAT_REGULATION ||
      bq25798.getVINDPM_STAT() == BQ25798::VINDPM_STAT_t::VINDPM_STAT_REGULATION) {
    Serial.printf("IN_DPM! ");
  };
  if (bq25798.getPG_STAT() == BQ25798::PG_STAT_t::PG_STAT_BAD) {
    Serial.printf("NOT_PG! ");
  };
  if (bq25798.getEN_HIZ()) {
    Serial.printf("HIZ_EN! ");
  };
  if (bq25798.getVSYS_STAT() == BQ25798::VSYS_STAT_t::VSYS_STAT_IN_VSYSMIN_REGULATION) {
    Serial.printf("VSYS_REG! ");
  };
  if (bq25798.getVBUS_PRESENT_STAT() == BQ25798::VBUS_PRESENT_STAT_t::VBUS_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("NO_VBUS! ");
  };
  if (bq25798.getAC1_PRESENT_STAT() == BQ25798::AC1_PRESENT_STAT_t::AC1_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("NO_AC1! ");
  };
  if (bq25798.getVBAT_PRESENT_STAT() == BQ25798::VBAT_PRESENT_STAT_t::VBAT_PRESENT_STAT_NOT_PRESENT) {
    Serial.printf("NO_VBAT! ");
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
    if (checkForError()) {
      Serial.printf("Error reading setting %d (%s)\n", i, setting.name);
      return;  // stop tracking changes if there is an error
    }
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
      if (setting.is_flag && newRawValues[i] == 0) {
        continue;
      }

      char type_info[100];

      changed = true;
      Serial.printf("[T+%7.3fs] ", elapsedMillis / 1000.0f);
      if (setting.type == BQ25798::settings_type_t::FLOAT) {
        snprintf(type_info, sizeof(type_info), "%s (%s, %s)", setting.name, "float", setting.unit);
        Serial.printf("%25s = %-20.3f    ",  //
                      type_info, bq25798.rawToFloat(newRawValues[i], setting));
        if (!justStarted) {
          Serial.printf("(was %.3f)",  //
                        bq25798.rawToFloat(oldRawValues[i], setting));
        }
      } else if (setting.type == BQ25798::settings_type_t::BOOL) {
        snprintf(type_info, sizeof(type_info), "%s (%s)", setting.name, "bool");
        Serial.printf("%25s = %-50s     ",  //
                      type_info, bq25798.rawToBool(newRawValues[i], setting) ? "TRUE" : "false");
        if (!justStarted) {
          Serial.printf("(was %s)",  //
                        bq25798.rawToBool(oldRawValues[i], setting) ? "TRUE" : "false");
        }
      } else if (setting.type == BQ25798::settings_type_t::ENUM) {
        snprintf(type_info, sizeof(type_info), "%s (%s)", setting.name, "enum");
        Serial.printf("%25s = [%d] \"%s\"%*s",  //
                      type_info, newRawValues[i], bq25798.rawToString(newRawValues[i], setting), 50 - 1 - strlen(bq25798.rawToString(newRawValues[i], setting)),
                      "");
        if (!justStarted) {
          Serial.printf("(was [%d] \"%s\")",  //
                        oldRawValues[i], bq25798.rawToString(oldRawValues[i], setting));
        };
      } else if (setting.type == BQ25798::settings_type_t::INT) {
        snprintf(type_info, sizeof(type_info), "%s (%s, %s)", setting.name, "int", setting.unit);
        Serial.printf("%25s = %-20d     ",  //
                      type_info, bq25798.rawToInt(newRawValues[i], setting));
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

void toggleCharger() {
  // Toggle the charger state
  if (bq25798.getEN_CHG()) {
    Serial.println("Disabling charger...");
    bq25798.setEN_CHG(false);
  } else {
    Serial.println("Enabling charger...");
    bq25798.setEN_CHG(true);
  }
}

void onetimeSetup() {
  // Disable HIZ mode (high impedance mode):
  bq25798.setEN_HIZ(false);

  // Enable BACKUP mode:
  // bq25798.setEN_OTG(1);  // no, this is not the same as EN_BACKUP. EN_OTG would provide power back to the input without disabling the ACFET/RBFET
  bq25798.setEN_BACKUP(true);

  bq25798.setVOTG(12000);  // set output voltage to 12 V
  bq25798.setIOTG(3300);   // set output current to 3.3 A (max)

  // FIXME to prevent chip reset when the host controller is disconnected temporarily
  bq25798.setWATCHDOG(BQ25798::WATCHDOG_t::WATCHDOG_DISABLE);  // disable watchdog timer
}

void rearmBackupMode() {
  // Re-arm the backup mode by setting EN_BACKUP to false and then true again
  Serial.println("Re-arming backup mode...");

  // See page 37 of the BQ25798 datasheet:
  // If there is an adapter reconnected while the charger is in backup mode, the user may transition the source which
  // powers the PMID load from the battery back to the adapter. The following sequence is used to switch from the
  // battery power back to ACIN1 while simultaneously re-arming the backup mode:
  // 1. Write BKUP_ACFET1_ON (REG0x16[0]) register bit to 1. Setting BKUP_ACFET1 _ON = 1 will cause the
  // device to set DIS_ACDRV = 0 and EN_ACDRV1 = 1. After that, backup mode is disabled, however, the
  // charger remains in the normal OTG mode. The ACFET1-RBFET1 is turned on to connect the adapter to
  // VBUS. The user must ensure the adapter voltage is equal to or higher than the charger VOTG voltage
  // setting, otherwise, the charger OTG output might back drive the adapter connected to VBUS.
  // 2. Determine the source at ACIN1 is valid (is not in overvoltage and did not fail poor source detection) by
  // reading back EN_ACDRV1 as 1.
  // 3. Set EN_OTG = 0, in order to exit OTG mode and enter the forward charging mode without PMID voltage
  // crash. Setting BKUP_ACFET1_ON = 1, also clears BKUP_ACFET1_ON to 0 and sets EN_BACKUP to 1.

  if (bq25798.getAC1_PRESENT_STAT() != BQ25798::AC1_PRESENT_STAT_t::AC1_PRESENT_STAT_PRESENT) {
    Serial.println("Error: AC1 is not present, cannot re-arm backup mode.");
    return;
  }

  bq25798.setBKUP_ACFET1_ON(true);  // turn on the ACFET1-RBFET1 to connect the adapter to VBUS
  delay(500);                       // wait for the ACFET1-RBFET1 to turn on
  // FIXME read all registers?
  if (bq25798.getEN_ACDRV1() == false) {
    Serial.println("Error: EN_ACDRV1 is not set, cannot re-arm backup mode.");
    return;
  }

  bq25798.setEN_OTG(false);  // exit OTG mode and enter the forward charging mode without PMID voltage crash
  delay(500);                // wait for the OTG mode to exit

  bq25798.setBKUP_ACFET1_ON(false);
  delay(500);  // wait for the OTG mode to exit

  bq25798.setEN_BACKUP(true);
  delay(500);  // wait for the OTG mode to exit

  Serial.println("Backup mode re-armed (hopefully?).");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial port initialized");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Serial.printf("I2C initialized on SDA=GPIO%d, SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.print("Looking for BQ25798 on I2C bus...");
  while (!bq25798.begin()) {
    delay(100);
  }
  bq25798.clearError();

  Serial.println("Connected.");

  Serial.print("Resetting the IC completely...");
  bq25798.setREG_RST(true);  // reset the IC
  while (bq25798.getREG_RST()) {
    delay(10);
  }
  Serial.println("Reset successful.");

  Serial.print("Setting up BQ25798...");

  // Disable watchdog timer (it would otherwise reset the chip if not cleared in time):
  // bq25798.setWATCHDOG(BQ25798::WATCHDOG_t::WATCHDOG_DISABLE);
  // ^we are reading values in loop() so it should not be needed

  // // Disable thermal sensor (not connected):
  // bq25798.setTS_IGNORE(true);

  // Enable ADC one shot mode. ADC_EN will be set to 0 after the readout is done.
  // A continuous ADC would otherwise produce too much visual noise (a lot of changes).
  bq25798.setADC_RATE(BQ25798::ADC_RATE_t::ADC_RATE_ONESHOT);
  bq25798.setADC_SAMPLE(BQ25798::ADC_SAMPLE_t::ADC_SAMPLE_15BIT);
  bq25798.setADC_EN(true);         // trigger ADC one-shot mode
  bq25798.setIBUS_ADC_DIS(false);  // enable IBUS ADC
  bq25798.setIBAT_ADC_DIS(false);  // enable IBAT ADC
  delay(100);

  onetimeSetup();  // for the first time
  rearmBackupMode();

  Serial.println("BQ25798 setup complete.");
  Serial.println("Waiting for changes...");
}

void loop() {
  patWatchdog();  // reset the watchdog timer
  trackChanges();
  checkForError();

  // If user pressed the 'p' (poke) key, try to re-arm backup mode again
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'r' || c == 'R') {
      rearmBackupMode();
    } else if (c == 'c' || c == 'C') {
      toggleCharger();  // toggle the charger state
    } else {
      Serial.printf("Unknown command: '%c'\n", c);
      Serial.println("Press 'r' to re-arm backup mode or 'c' to toggle charger state.");
    }
  }

  delay(100);
}
