# BQ25798 library for Arduino

This library offers a simple interface to communicate with the BQ25798 battery charger IC from Texas Instruments.

## Features

Easy to use with named registers and bitfields:

```
BQ25798 bq = BQ25798();

bq.setADC_EN(true);
bq.setTS_IGNORE(true);
bq.setEN_BACKUP(true);

bq.setADC_RATE(BQ25798::ADC_RATE_t::ONESHOT);

printf("VSYSMIN: %d millivolts\n", bq.getVSYSMIN());
printf("Charging status: %s\n", bq.getCHG_STAT_string());
```

## Example

Use the included "track_changes" example to see how the library works. It will read the registers and print them to the serial monitor like this:

```
[T+  1.541s]               VINDPM (int)   = 4300                                                   (was  3600)
[T+  1.541s]            EN_BACKUP (bool)  = TRUE                                                   (was false)
[T+  1.541s]             ICO_ILIM (int)   = 3000                                                   (was   500)
[T+  1.541s]              PG_STAT (enum)  = [1] "Power good"                                       (was [0] "Not in power good status")
[T+  1.541s]             CHG_STAT (enum)  = [1] "Trickle Charge"                                   (was [0] "Not Charging")
[T+  1.541s]            VBUS_STAT (enum)  = [5] "Unknown adaptor (3A)"                             (was [8] "Not qualified adaptor")
[T+  1.541s]        ADC_DONE_STAT (bool)  = TRUE                                                   (was false)
[T+  1.541s]            VSYS_STAT (enum)  = [1] "In VSYSMIN regulation (VBAT < VSYSMIN)"           (was [0] "Not in VSYSMIN regulation (VBAT > VSYSMIN)")
[T+  1.541s]          IINDPM_FLAG (bool)  = TRUE                                                   (was false)
[T+  1.541s]             CHG_FLAG (bool)  = TRUE                                                   (was false)
[T+  1.541s]       DPDM_DONE_FLAG (bool)  = TRUE                                                   (was false)
[T+  1.541s]               ADC_EN (bool)  = false                                                  (was TRUE)
[T+  1.541s]             VBUS_ADC (int)   = 5112                                                   (was  5068)
[T+  1.541s]             VAC2_ADC (int)   = 5096                                                   (was     0)
[T+  1.541s]               TS_ADC (float) = 56.445                                                 (was 0.000)
[T+  1.541s]             TDIE_ADC (float) = 7.000                                                  (was 0.000)
```

## Home Assistant metadata

Each setting in `chip_definitions.json` can provide `homeassistant` metadata. Read it by setting index or by a named setting:

```cpp
const auto& ha = bq.getHomeAssistantInfo(bq.TDIE_ADC);
// ha.device_class == "temperature", ha.entity_category == "diagnostic"
// ha.state_class == "measurement", ha.icon == nullptr (automatic icon)

for (int i = 0; i < BQ25798::SETTINGS_COUNT; ++i) {
  const auto& setting = bq.getSetting(i);
  const auto& info = bq.getHomeAssistantInfo(i);
  // Use setting.name/unit and info.device_class/entity_category/state_class/icon.
}
```

The four metadata fields are nullable `const char*` values. Omit null fields when building discovery messages. Missing JSON fields and JSON `null` become
`nullptr`; existing empty strings are preserved. The library provides the configured values without validating Home Assistant entity domains or publishing MQTT.
For example, legacy device classes such as `status` or `control` may need mapping in the client before publication.

Both accessors are const and return references to static, read-only data. Invalid indices and unknown settings return four null fields without changing
`lastError()`. Setting lookup uses the register, bit shift, bit length, register width and setting type, so it also works with copies and
`BQ25798_MEMORY_SAVER`. Index lookup is constant time; setting lookup scans the settings list.

Metadata adds no fields to `Setting` or to each charger instance and performs no dynamic allocation or I2C access. The generated constant table is suitable
for flash storage on ESP32. After editing metadata, run `py process_templates.py` and include the updated `gen_homeassistant.h` with its template.

## Testing

```bash
platformio test -e native
```
