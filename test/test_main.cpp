#include <unity.h>

#include "utility/BQ25798Core.h"

BQ25798Core bq = BQ25798Core();  // Create an instance of the BQ25798 class

BQ25798Core::RegisterDefinition reg8bit = {0x00, BQ25798Core::regsize_t::SHORT, "TEST_REG8"};
BQ25798Core::RegisterDefinition reg16bit = {0x01, BQ25798Core::regsize_t::LONG, "TEST_REG16"};

void setUp(void) {
  // Set up before each test case
  bq._registerDefinitions[0] = reg8bit;
  bq._registerDefinitions[1] = reg16bit;
}

void tearDown(void) {
  // Clean up after each test case
}

void test_bool(void) {
  BQ25798Core::Setting boolSetting = {reg8bit.address, "TEST_BOOL", BQ25798Core::settings_type_t::BOOL, 0xFF, 0};

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
  BQ25798Core::Setting intSetting = {reg8bit.address, "TEST_INT", BQ25798Core::settings_type_t::INT, 0xFF, 0};
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
      reg16bit.address, "TEST_SIGNED_INT", BQ25798Core::settings_type_t::INT, 0xFF, 0, 0, 0, 0, 0, BQ25798Core::settings_flags_t::IS_2COMPLEMENT};

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

  BQ25798Core::Setting calculatedIntSetting = {reg16bit.address, "TEST_INT",  BQ25798Core::settings_type_t::INT, 0xFF, 0, /* range */ 10, 100,
                                               /* offset */ 50,  /* step */ 2};
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
  TEST_ASSERT_EQUAL(0x00, bq.intToRaw(1000, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_INVALID_VALUE, bq.lastError());
  bq.clearError();  // clear error for next test

  // test for rounding
  TEST_ASSERT_EQUAL(0x01, bq.intToRaw(52.5, calculatedIntSetting));
  TEST_ASSERT_EQUAL(ERROR_NONE, bq.lastError());
}

int main(int argc, char** argv) {
  UNITY_BEGIN();  // Start Unity test framework

  RUN_TEST(test_bool);
  RUN_TEST(test_int);

  UNITY_END();  // Stop Unity test framework

  return 0;
}
