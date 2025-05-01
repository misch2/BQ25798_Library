#include <unity.h>

#include "utility/BQ25798Core.h"

BQ25798Core bq = BQ25798Core();  // Create an instance of the BQ25798 class

BQ25798Core::RegisterDefinition reg0 = {0x00, "TEST_REG"};

void setUp(void) {
  // Set up before each test case
  bq._registerDefinitions[0] = reg0;
  bq.clearError();  // clear error for next test
}

void tearDown(void) {
  // Clean up after each test case
}

void test_bool(void) {
  BQ25798Core::Setting boolSetting = {reg0.address, false, "TEST_BOOL", BQ25798Core::settings_type_t::BOOL, 0xFF, 0};

  TEST_ASSERT_EQUAL(true, bq.rawToBool(0x01, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(true, bq.rawToBool(0xFF, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(false, bq.rawToBool(0x00, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.boolToRaw(false, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.boolToRaw(true, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.boolToRaw(12345, boolSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
}

void test_int(void) {
  BQ25798Core::Setting intSetting = {reg0.address, false, "TEST_INT", BQ25798Core::settings_type_t::INT, 0xFF, 0};
  TEST_ASSERT_EQUAL(0, bq.rawToInt(0x00, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(3, bq.rawToInt(0x03, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(256, bq.rawToInt(0x100, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x03, bq.intToRaw(3, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x100, bq.intToRaw(256, intSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  BQ25798Core::Setting signedIntSetting = {
      reg0.address, true, "TEST_SIGNED_INT", BQ25798Core::settings_type_t::INT, 0xFF, 0, 0, 0, 0, 0, BQ25798Core::settings_flags_t::IS_2COMPLEMENT};

  TEST_ASSERT_EQUAL(0, bq.rawToInt(0x00, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(5, bq.rawToInt(0x05, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(-5, bq.rawToInt(0xFFFB, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x05, bq.intToRaw(5, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0xFFFB, bq.intToRaw(-5, signedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  BQ25798Core::Setting calculatedIntSetting = {reg0.address,    true,        "TEST_INT", BQ25798Core::settings_type_t::INT, 0xFF, 0, /* range */ 10, 100,
                                               /* offset */ 50, /* step */ 2};
  TEST_ASSERT_EQUAL(50, bq.rawToInt(0x00, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(52, bq.rawToInt(0x01, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(50, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.intToRaw(52, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  // test for out of range
  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(0, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(1000, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
}

void test_float(void) {
  BQ25798Core::Setting floatSetting = {reg0.address,      true,          "TEST_FLOAT", BQ25798Core::settings_type_t::FLOAT, 0xFF, 0, /* range */ 10.0, 100.0,
                                       /* offset */ 50.0, /* step */ 2.0};
  TEST_ASSERT_EQUAL(50.0, bq.rawToFloat(0x00, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(52.0, bq.rawToFloat(0x01, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL(0x00, bq.floatToRaw(50.0, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.0, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.01, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL(0x00, bq.floatToRaw(-1, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();

  // test for rounding
  TEST_ASSERT_EQUAL(0x01, bq.floatToRaw(52.5, floatSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
}

void test_enum(void) {
  BQ25798Core::Setting stringSetting = {reg0.address,
                                        false,
                                        "TEST_ENUM",
                                        BQ25798Core::settings_type_t::ENUM,
                                        0xFF,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        BQ25798Core::settings_flags_t::NONE,
                                        {"Value A", "Value B", "Value C"}};
  TEST_ASSERT_EQUAL_STRING("Value A", bq.rawToString(0x00, stringSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
  TEST_ASSERT_EQUAL_STRING("Value B", bq.rawToString(0x01, stringSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());

  TEST_ASSERT_EQUAL_STRING("Unknown", bq.rawToString(0xFF, stringSetting));
  TEST_ASSERT_EQUAL(ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();
}

int main(int argc, char** argv) {
  UNITY_BEGIN();  // Start Unity test framework

  RUN_TEST(test_bool);
  RUN_TEST(test_int);
  RUN_TEST(test_float);
  RUN_TEST(test_enum);

  UNITY_END();  // Stop Unity test framework

  return 0;
}
