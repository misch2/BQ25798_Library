#include <unity.h>

#include "BQ25798MockTest.h"

BQ25798MockTest bq = BQ25798MockTest();

BQ25798MockTest::RegisterDefinition reg0 = {0x00, "TEST_REG00"};
BQ25798MockTest::RegisterDefinition reg1 = {0x01, "TEST_REG01"};

void setUp(void) {
  // Initialize the BQ25798MockTest object with a mock I2C address
  TEST_ASSERT_EQUAL(true, bq.begin());
  TEST_ASSERT_EQUAL(true, bq.i2c_isConnected());
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  bq._registerDefinitions[0] = reg0;
  bq._registerDefinitions[1] = reg1;

  uint8_t dataToRead[100];

  const uint8_t dataToWrite[] = {0x12, 0x34, 0xFF, 0xFF};
  bq.i2c_writeBytes(reg0.address, 1, &dataToWrite[0]);
  bq.i2c_writeBytes(reg1.address, 1, &dataToWrite[1]);

  bq.i2c_readBytes(reg0.address, 1, dataToRead);
  TEST_ASSERT_EQUAL(0x12, dataToRead[0]);
  bq.i2c_readBytes(reg1.address, 1, dataToRead);
  TEST_ASSERT_EQUAL(0x34, dataToRead[0]);

  const uint8_t dataToWrite2[] = {0x00, 0x00};
  bq.i2c_writeBytes(reg0.address, 1, &dataToWrite2[0]);
  bq.i2c_writeBytes(reg1.address, 1, &dataToWrite2[1]);

  bq.i2c_readBytes(reg0.address, 1, &dataToRead[0]);
  TEST_ASSERT_EQUAL(0x00, dataToRead[0]);
  bq.i2c_readBytes(reg1.address, 1, &dataToRead[1]);
  TEST_ASSERT_EQUAL(0x00, dataToRead[1]);

  bq.clearError();  // clear error for next test
}

void tearDown(void) {
  // Clean up after each test case
}

void test_reg8_fields(void) {
  BQ25798MockTest::Setting setting1 = {reg0.address, false, "TEST", "", BQ25798MockTest::settings_type_t::INT, /* len */ 3, /* shift */ 1, false};

  bq.writeSettingToReg8(0xFF, setting1);  // 0xFF = 0b11111111 -> mask len 3 = 0b00000111 -> shift 1 = 0b00001110
  uint8_t dataToRead[1];
  bq.i2c_readBytes(reg0.address, 1, dataToRead);

  TEST_ASSERT_EQUAL_MESSAGE(0b00001110, dataToRead[0], "bit shifting and masking works for writeSettingToReg8");
  TEST_ASSERT_EQUAL_MESSAGE(0b00000111, bq.readSettingFromReg8(setting1), "bit shifting and masking works for readSettingFromReg8");
}

void test_reg16_fields(void) {
  BQ25798MockTest::Setting setting1 = {reg0.address, true, "TEST", "", BQ25798MockTest::settings_type_t::INT, /* len */ 4, /* shift */ 7, false};
  bq.writeSettingToReg16(0xFFFF, setting1);  // 0xFFFF = 0b11111111_11111111 -> mask len 4 = 0b0000000_000001111 -> shift 7 = 0b00000111_10000000
  uint8_t dataToRead[2];
  bq.i2c_readBytes(reg0.address, 1, &dataToRead[0]);
  bq.i2c_readBytes(reg1.address, 1, &dataToRead[1]);

  TEST_ASSERT_EQUAL_MESSAGE(0b00000111, dataToRead[0], "bit shifting and masking works for writeSettingToReg16 big endian");
  TEST_ASSERT_EQUAL_MESSAGE(0b10000000, dataToRead[1], "bit shifting and masking works for writeSettingToReg16 big endian");
  TEST_ASSERT_EQUAL_MESSAGE(0x000F, bq.readSettingFromReg16(setting1), "bit shifting and masking works for readSettingFromReg16");
}

void test_bool(void) {
  BQ25798MockTest::Setting boolSetting = {reg0.address, false, "TEST_BOOL", "", BQ25798MockTest::settings_type_t::BOOL, 1, 0};

  bq.setREG_RST(true);
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  bq.setEN_BACKUP(false);
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(false, bq.getAC1_PRESENT_FLAG());
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(true, bq.rawToBool(0x01, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(true, bq.rawToBool(0xFF, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL_MESSAGE(true, bq.rawToBool(0x40, boolSetting), "any non-zero value should be true");
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(false, bq.rawToBool(0x00, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.boolToRaw(false, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.boolToRaw(true, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.boolToRaw(12345, boolSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
}

void test_int(void) {
  BQ25798MockTest::Setting intSetting = {reg0.address, false, "TEST_INT", "foo", BQ25798MockTest::settings_type_t::INT, 0xFF, 0};

  bq.setVINDPM(12345);
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(12300, bq.getVINDPM());
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0, bq.rawToInt(0x00, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(3, bq.rawToInt(0x03, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(256, bq.rawToInt(0x100, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x03, bq.intToRaw(3, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x100, bq.intToRaw(256, intSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  BQ25798MockTest::Setting signedIntSetting = {reg0.address,
                                               true,
                                               "TEST_SIGNED_INT",
                                               "bar",
                                               BQ25798MockTest::settings_type_t::INT,
                                               0xFF,
                                               0,
                                               0,
                                               false,
                                               0,
                                               0,
                                               0,
                                               BQ25798MockTest::settings_flags_t::IS_2COMPLEMENT};

  TEST_ASSERT_EQUAL(0, bq.rawToInt(0x00, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(5, bq.rawToInt(0x05, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(-5, bq.rawToInt(0xFFFB, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x05, bq.intToRaw(5, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0xFFFB, bq.intToRaw(-5, signedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  BQ25798MockTest::Setting calculatedIntSetting = {reg0.address,    true,        "TEST_INT", "baz", BQ25798MockTest::settings_type_t::INT, 0xFF, 0, false,
                                                   /* range */ 10,  100,
                                                   /* offset */ 50, /* step */ 2};
  TEST_ASSERT_EQUAL(50, bq.rawToInt(0x00, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(52, bq.rawToInt(0x01, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(50, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.intToRaw(52, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  // test for out of range
  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(1000, calculatedIntSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
}

void test_float(void) {
  BQ25798MockTest::Setting floatSetting = {reg0.address,      true,          "TEST_FLOAT", "quux", BQ25798MockTest::settings_type_t::FLOAT, 0xFF, 0, false,
                                           /* range */ 10.0,  100.0,
                                           /* offset */ 50.0, /* step */ 2.0};
  TEST_ASSERT_EQUAL(50.0, bq.rawToFloat(0x00, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(52.0, bq.rawToFloat(0x01, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.floatToRaw(50.0, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.0, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.01, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x00, bq.floatToRaw(-1, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();

  // test for rounding
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.5, floatSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
}

void test_tdie_adc_signed_readings(void) {
  const uint16_t rawValues[] = {0xFFB0, 0xFFEC, 0xFFFF, 0x0000, 0x0001, 0x0033, 0x012C};
  const float temperatures[] = {-40.0f, -10.0f, -0.5f, 0.0f, 0.5f, 25.5f, 150.0f};
  for (int i = 0; i < 7; ++i) {
    const uint8_t bytes[] = {static_cast<uint8_t>(rawValues[i] >> 8), static_cast<uint8_t>(rawValues[i])};
    bq.i2c_writeBytes(0x41, 2, bytes);
    bq.readAllRegisters();
    TEST_ASSERT_EQUAL_FLOAT(temperatures[i], bq.getTDIE_ADC());
    TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  }
}

void test_float_signed_scaling_and_unsigned_values(void) {
  auto setting = bq.TDIE_ADC;
  setting.fixed_offset = 10.0f;
  TEST_ASSERT_EQUAL_FLOAT(8.5f, bq.rawToFloat(0xFFFD, setting));

  setting.flags = BQ25798MockTest::settings_flags_t::NONE;
  setting.fixed_offset = 0.0f;
  TEST_ASSERT_EQUAL_FLOAT(32767.5f, bq.rawToFloat(0xFFFF, setting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
}

void test_float_rejects_short_signed_setting(void) {
  auto setting = bq.TDIE_ADC;
  setting.long_reg = false;  // Match the signed-setting restriction of rawToInt().
  TEST_ASSERT_EQUAL_FLOAT(0.0f, bq.rawToFloat(0xFF, setting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_SETTING, bq.lastError());
  bq.clearError();
}

void test_enum(void) {
  BQ25798MockTest::Setting stringSetting = {reg0.address,
                                            false,
                                            "TEST_ENUM",
                                            "",
                                            BQ25798MockTest::settings_type_t::ENUM,
                                            0xFF,
                                            0,
                                            false,
                                            0,
                                            0,
                                            0,
                                            0,
                                            BQ25798MockTest::settings_flags_t::NONE,
                                            {"Value A", "Value B", "Value C"}};
  TEST_ASSERT_EQUAL_STRING("Value A", bq.rawToString(0x00, stringSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL_STRING("Value B", bq.rawToString(0x01, stringSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL_STRING("Unknown", bq.rawToString(0xFF, stringSetting));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
}

void test_mppt_fields_preserve_neighbors(void) {
  bq.setVOC_PCT(BQ25798MockTest::VOC_PCT_t::VOC_PCT_0_875);
  bq.setEN_MPPT(true);
  bq.setVOC_DLY(BQ25798MockTest::VOC_DLY_t::VOC_DLY_2S);
  bq.setVOC_RATE(BQ25798MockTest::VOC_RATE_t::VOC_RATE_30MIN);

  uint8_t reg;
  bq.i2c_readBytes(0x15, 1, &reg);
  TEST_ASSERT_EQUAL_HEX8(0xB7, reg);  // 101 | 10 | 11 | 1, per datasheet REG15.
  TEST_ASSERT_EQUAL(5, static_cast<int>(bq.getVOC_PCT()));
  TEST_ASSERT_EQUAL(2, static_cast<int>(bq.getVOC_DLY()));
  TEST_ASSERT_EQUAL(3, static_cast<int>(bq.getVOC_RATE()));
  TEST_ASSERT_TRUE(bq.getEN_MPPT());

  bq.setVOC_DLY(BQ25798MockTest::VOC_DLY_t::VOC_DLY_50MS);
  bq.i2c_readBytes(0x15, 1, &reg);
  TEST_ASSERT_EQUAL_HEX8(0xA7, reg);
  bq.setVOC_RATE(BQ25798MockTest::VOC_RATE_t::VOC_RATE_30S);
  bq.i2c_readBytes(0x15, 1, &reg);
  TEST_ASSERT_EQUAL_HEX8(0xA1, reg);
}

void test_status_fields_ignore_reserved_bits(void) {
  const uint8_t patterns[] = {0x04, 0x02, 0x38};
  const int thermal[] = {1, 0, 0};
  const int detection[] = {0, 1, 0};
  for (int i = 0; i < 3; ++i) {
    bq.i2c_writeBytes(0x1D, 1, &patterns[i]);
    bq.readAllRegisters();
    TEST_ASSERT_EQUAL(thermal[i], static_cast<int>(bq.getTREG_STAT()));
    TEST_ASSERT_EQUAL(detection[i], static_cast<int>(bq.getDPDM_STAT()));
  }
}

void test_corrected_enums_and_ranges(void) {
  bq.setVAC_OVP(BQ25798MockTest::VAC_OVP_t::VAC_OVP_18V);
  uint8_t reg;
  bq.i2c_readBytes(0x10, 1, &reg);
  TEST_ASSERT_EQUAL_HEX8(0x10, reg & 0x30);
  TEST_ASSERT_EQUAL_STRING(">18V", bq.getVAC_OVP_enum_string());

  // Reject a code that cannot fit in the single BCOLD bit.
  bq.intToRaw(2, bq.BCOLD);
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();

  TEST_ASSERT_EQUAL_FLOAT(99.9023f, bq.TS_ADC.range_high);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 99.9023f, bq.rawToFloat(1023, bq.TS_ADC));
  TEST_ASSERT_EQUAL(7, bq.intToRaw(7, bq.CHG_STAT));
  TEST_ASSERT_EQUAL(12, bq.intToRaw(12, bq.VBUS_STAT));
  TEST_ASSERT_EQUAL(2, bq.intToRaw(2, bq.ICO_STAT));
  TEST_ASSERT_EQUAL(3, bq.intToRaw(3, bq.PN));
  TEST_ASSERT_EQUAL(BQ25798MockTest::Error::ERROR_NONE, bq.lastError());

  const uint8_t present = 0xC0;
  bq.i2c_writeBytes(0x1E, 1, &present);
  bq.readAllRegisters();
  TEST_ASSERT_EQUAL(static_cast<int>(BQ25798MockTest::ACRB1_STAT_t::ACRB1_STAT_PRESENT), static_cast<int>(bq.getACRB1_STAT()));
  TEST_ASSERT_EQUAL(static_cast<int>(BQ25798MockTest::ACRB2_STAT_t::ACRB2_STAT_PRESENT), static_cast<int>(bq.getACRB2_STAT()));
  TEST_ASSERT_EQUAL_STRING("ACFET1/RBFET1 present", bq.getACRB1_STAT_enum_string());
  TEST_ASSERT_EQUAL_STRING("ACFET2/RBFET2 present", bq.getACRB2_STAT_enum_string());
}

int main(int argc, char** argv) {
  UNITY_BEGIN();  // Start Unity test framework

  RUN_TEST(test_reg8_fields);
  RUN_TEST(test_reg16_fields);
  RUN_TEST(test_bool);
  RUN_TEST(test_int);
  RUN_TEST(test_float);
  RUN_TEST(test_tdie_adc_signed_readings);
  RUN_TEST(test_float_signed_scaling_and_unsigned_values);
  RUN_TEST(test_float_rejects_short_signed_setting);
  RUN_TEST(test_enum);
  RUN_TEST(test_mppt_fields_preserve_neighbors);
  RUN_TEST(test_status_fields_ignore_reserved_bits);
  RUN_TEST(test_corrected_enums_and_ranges);

  UNITY_END();  // Stop Unity test framework

  return 0;
}
