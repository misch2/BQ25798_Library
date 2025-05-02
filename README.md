# BQ25798 library for Arduino

This library offers a simple interface to communicate with the BQ25798 battery charger IC from Texas Instruments. No bit manipulation is needed. No search for register addresses. No need to remember what different enum values mean. Just use the enum names and the library will take care of the rest.

## Features

Easy use with clearly named registers and bitfields.

```
BQ25798 bq = BQ25798();

bq.setAndWriteADC_EN(true);
bq.setAndWriteTS_IGNORE(rue);
bq.setAndWriteEN_BACKUP(true);

bq.setAndWriteADC_RATE(BQ25798::ADC_RATE_t::ONESHOT);

printf("Charging status: %s\n", bq.getCHG_STAT_string());
```

## Example

Use the included "track_changes" example to see how the library works. It will read the registers and print them to the serial monitor like this:

![example 1](extras/example1.png)

## Testing

```bash
platformio test -e native
```
