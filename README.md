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

## Testing

```bash
platformio test -e native
```
