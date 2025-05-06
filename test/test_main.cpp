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

  bq.i2c_writeReg8(reg0.address, 0x12);
  bq.i2c_writeReg8(reg1.address, 0x34);
  TEST_ASSERT_EQUAL(0x12, bq.i2c_readReg8(reg0.address));
  TEST_ASSERT_EQUAL(0x34, bq.i2c_readReg8(reg1.address));

  bq.i2c_writeReg8(reg0.address, 0x00);
  bq.i2c_writeReg8(reg1.address, 0x00);
  TEST_ASSERT_EQUAL(0x00, bq.i2c_readReg8(reg0.address));
  TEST_ASSERT_EQUAL(0x00, bq.i2c_readReg8(reg1.address));

  bq.clearError();  // clear error for next test
}

void tearDown(void) {
  // Clean up after each test case
}

void test_reg8_fields(void) {
  bq.writeSettingToReg8(reg0.address, 0xFF, /* len */ 3, /* shift */ 1);  // 0xFF = 0b11111111 -> mask len 3 = 0b00000111 -> shift 1 = 0b00001110
  TEST_ASSERT_EQUAL_MESSAGE(0b00001110, bq.i2c_readReg8(reg0.address), "bit shifting and masking works for writeSettingToReg8");
  TEST_ASSERT_EQUAL_MESSAGE(0b00000111, bq.readSettingFromReg8(reg0.address, /* len */ 3, /* shift */ 1),
                            "bit shifting and masking works for readSettingFromReg8");
}

void test_reg16_fields(void) {
  bq.writeSettingToReg16(reg0.address, 0xFFFF, /* len */ 4,
                         /* shift */ 7);  // 0xFFFF = 0b11111111_11111111 -> mask len 4 = 0b0000000_000001111 -> shift 7 = 0b00000111_10000000
  TEST_ASSERT_EQUAL_MESSAGE(0b00000111, bq.i2c_readReg8(reg0.address), "bit shifting and masking works for writeSettingToReg16 big endian");
  TEST_ASSERT_EQUAL_MESSAGE(0b10000000, bq.i2c_readReg8(reg1.address), "bit shifting and masking works for writeSettingToReg16 big endian");
  TEST_ASSERT_EQUAL_MESSAGE(0x000F, bq.readSettingFromReg16(reg0.address, /* len */ 4, /* shift */ 7),
                            "bit shifting and masking works for readSettingFromReg16");
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

  BQ25798MockTest::Setting calculatedIntSetting = {reg0.address,    true,        "TEST_INT", "baz", BQ25798MockTest::settings_type_t::INT, 0xFF, 0,
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
  BQ25798MockTest::Setting floatSetting = {reg0.address,      true,          "TEST_FLOAT", "quux", BQ25798MockTest::settings_type_t::FLOAT, 0xFF, 0,
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

void test_enum(void) {
  BQ25798MockTest::Setting stringSetting = {reg0.address,
                                            false,
                                            "TEST_ENUM",
                                            "",
                                            BQ25798MockTest::settings_type_t::ENUM,
                                            0xFF,
                                            0,
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

int main(int argc, char** argv) {
  UNITY_BEGIN();  // Start Unity test framework

  RUN_TEST(test_reg8_fields);
  RUN_TEST(test_reg16_fields);
  RUN_TEST(test_bool);
  RUN_TEST(test_int);
  RUN_TEST(test_float);
  RUN_TEST(test_enum);

  UNITY_END();  // Stop Unity test framework

  return 0;
}
