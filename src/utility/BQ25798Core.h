#ifndef BQ25798CORE_H
#define BQ25798CORE_H

#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "debug.h"
#include "errors.h"
#include "registers.h"

class BQ25798Core {
 public:
  BQ25798Core();

  // =================================
  // Registers
  // =================================
  typedef int16_t regaddr_t;

  struct RegisterDefinition {
    regaddr_t address;  // Register address
    const char* name;   // Register name

    RegisterDefinition(regaddr_t address, const char* name)
        : address(address),
#ifdef BQ25798_MEMORY_SAVER
          name("")
#else
          name(name)
#endif
    {};

    static RegisterDefinition invalid() { return RegisterDefinition{-1, "INVALID"}; };
    bool isValid() const { return address != -1; };
  };

  static constexpr uint8_t PHYSICAL_REGISTERS_COUNT = 1 + REG48_Part_Information;  // Number of physical registers (0x00 to 0x48)
#define DEFINE_REGISTER(propname) \
  RegisterDefinition { propname, #propname }
  std::array<RegisterDefinition, PHYSICAL_REGISTERS_COUNT> _registerDefinitions = {
      //  There must be all 8-bit registers in the list (no skipping for 16-bit registers, no skipping for unused registers)!
      DEFINE_REGISTER(REG00_Minimal_System_Voltage),
      DEFINE_REGISTER(REG01_Charge_Voltage_Limit),
      DEFINE_REGISTER(REG02_dummy_fill),
      DEFINE_REGISTER(REG03_Charge_Current_Limit),
      DEFINE_REGISTER(REG04_dummy_fill),
      DEFINE_REGISTER(REG05_Input_Voltage_Limit),
      DEFINE_REGISTER(REG06_Input_Current_Limit),
      DEFINE_REGISTER(REG07_dummy_fill),
      DEFINE_REGISTER(REG08_Precharge_Control),
      DEFINE_REGISTER(REG09_Termination_Control),
      DEFINE_REGISTER(REG0A_Recharge_Control),
      DEFINE_REGISTER(REG0B_VOTG_regulation),
      DEFINE_REGISTER(REG0C_dummy_fill),
      DEFINE_REGISTER(REG0D_IOTG_regulation),
      DEFINE_REGISTER(REG0E_Timer_Control),
      DEFINE_REGISTER(REG0F_Charger_Control_0),
      DEFINE_REGISTER(REG10_Charger_Control_1),
      DEFINE_REGISTER(REG11_Charger_Control_2),
      DEFINE_REGISTER(REG12_Charger_Control_3),
      DEFINE_REGISTER(REG13_Charger_Control_4),
      DEFINE_REGISTER(REG14_Charger_Control_5),
      DEFINE_REGISTER(REG15_MPPT_Control),
      DEFINE_REGISTER(REG16_Temperature_Control),
      DEFINE_REGISTER(REG17_NTC_Control_0),
      DEFINE_REGISTER(REG18_NTC_Control_1),
      DEFINE_REGISTER(REG19_ICO_Current_Limit),
      DEFINE_REGISTER(REG1A_dummy_fill),
      DEFINE_REGISTER(REG1B_Charger_Status_0),
      DEFINE_REGISTER(REG1C_Charger_Status_1),
      DEFINE_REGISTER(REG1D_Charger_Status_2),
      DEFINE_REGISTER(REG1E_Charger_Status_3),
      DEFINE_REGISTER(REG1F_Charger_Status_4),
      DEFINE_REGISTER(REG20_FAULT_Status_0),
      DEFINE_REGISTER(REG21_FAULT_Status_1),
      DEFINE_REGISTER(REG22_Charger_Flag_0),
      DEFINE_REGISTER(REG23_Charger_Flag_1),
      DEFINE_REGISTER(REG24_Charger_Flag_2),
      DEFINE_REGISTER(REG25_Charger_Flag_3),
      DEFINE_REGISTER(REG26_FAULT_Flag_0),
      DEFINE_REGISTER(REG27_FAULT_Flag_1),
      DEFINE_REGISTER(REG28_Charger_Mask_0),
      DEFINE_REGISTER(REG29_Charger_Mask_1),
      DEFINE_REGISTER(REG2A_Charger_Mask_2),
      DEFINE_REGISTER(REG2B_Charger_Mask_3),
      DEFINE_REGISTER(REG2C_FAULT_Mask_0),
      DEFINE_REGISTER(REG2D_FAULT_Mask_1),
      DEFINE_REGISTER(REG2E_ADC_Control),
      DEFINE_REGISTER(REG2F_ADC_Function_Disable_0),
      DEFINE_REGISTER(REG30_ADC_Function_Disable_1),
      DEFINE_REGISTER(REG31_IBUS_ADC),
      DEFINE_REGISTER(REG32_dummy_fill),
      DEFINE_REGISTER(REG33_IBAT_ADC),
      DEFINE_REGISTER(REG34_dummy_fill),
      DEFINE_REGISTER(REG35_VBUS_ADC),
      DEFINE_REGISTER(REG36_dummy_fill),
      DEFINE_REGISTER(REG37_VAC1_ADC),
      DEFINE_REGISTER(REG38_dummy_fill),
      DEFINE_REGISTER(REG39_VAC2_ADC),
      DEFINE_REGISTER(REG3A_dummy_fill),
      DEFINE_REGISTER(REG3B_VBAT_ADC),
      DEFINE_REGISTER(REG3C_dummy_fill),
      DEFINE_REGISTER(REG3D_VSYS_ADC),
      DEFINE_REGISTER(REG3E_dummy_fill),
      DEFINE_REGISTER(REG3F_TS_ADC),
      DEFINE_REGISTER(REG40_dummy_fill),
      DEFINE_REGISTER(REG41_TDIE_ADC),
      DEFINE_REGISTER(REG42_dummy_fill),
      DEFINE_REGISTER(REG43_DPLUS_ADC),
      DEFINE_REGISTER(REG44_dummy_fill),
      DEFINE_REGISTER(REG45_DMINUS_ADC),
      DEFINE_REGISTER(REG46_dummy_fill),
      DEFINE_REGISTER(REG47_DPDM_Driver),
      DEFINE_REGISTER(REG48_Part_Information)
      //
  };

  const RegisterDefinition& getRegisterDefinition(regaddr_t address);

  // =================================
  // Settings
  // =================================
  enum class settings_flags_t : uint8_t {
    NONE = 0,
    IS_2COMPLEMENT = 0x80,
  };

  enum class settings_type_t : uint8_t {
    INT = 0,
    FLOAT = 1,
    BOOL = 2,
    ENUM = 3,
  };

  typedef const std::vector<const char*> strings_vector_t;

  struct Setting {
    settings_type_t type;  // Setting type (e.g., raw, int, float, bool, enum)
    bool long_reg;         // Long register (16-bit) or short register (8-bit)
    regaddr_t reg;         // Base register address
    uint8_t bitlength;     // Bit length for the setting
    uint8_t bitshift;      // Bit shift for the setting
    // optional:
    float range_low;
    float range_high;
    float fixed_offset;   // Fixed offset for the setting (e.g., for voltage settings)
    float bit_step_size;  // Bit step size for the setting (e.g., for current settings)
    // adjusted value = raw_value * bit_step_size + fixed_offset
    settings_flags_t flags;           // Flags for the setting (e.g., 2's complement)
    strings_vector_t strings_vector;  // Vector of strings for enum settings
    const char* name;                 // Setting name

    Setting(regaddr_t reg, bool long_reg, const char* name, settings_type_t type, uint8_t bitlength, uint8_t bitshift,
            // optional parameters:
            float range_low = 0, float range_high = 0, float fixed_offset = 0, float bit_step_size = 0, settings_flags_t flags = settings_flags_t::NONE,
            strings_vector_t strings_vector = {})
        : reg(reg),
          long_reg(long_reg),
#ifdef BQ25798_MEMORY_SAVER
          name(""),
#else
          name(name),
#endif
          type(type),
          bitlength(bitlength),
          bitshift(bitshift),
          range_low(range_low),
          range_high(range_high),
          fixed_offset(fixed_offset),
          bit_step_size(bit_step_size),
          flags(flags),
          strings_vector(strings_vector) {};
    static Setting invalid() {
      return Setting{RegisterDefinition::invalid().address, false, "INVALID", settings_type_t::INT, 0, 0, 0, 0, 0, 0, settings_flags_t::NONE, {}};
    };
    bool isValid() const { return bitlength != 0; };
  };

  uint16_t getRaw(const Setting& setting);
  int getInt(const Setting& setting);
  bool getBool(const Setting& setting);
  float getFloat(const Setting& setting);
  const char* getString(const Setting& setting);
  const char* toString(int value, strings_vector_t map);

#include "_settings.h"  // This file is generated by the template engine (Jinja2)

  const Setting& getSetting(int id);

  // memory-only operations
  void clearError();
  int lastError();
  bool faultFlagRaised();

  int rawToInt(uint16_t raw, const Setting& setting);
  uint16_t intToRaw(int value, const Setting& setting);

  float rawToFloat(uint16_t raw, const Setting& setting);
  uint16_t floatToRaw(float value, const Setting& setting);

  bool rawToBool(uint16_t raw, const Setting& setting);
  uint16_t boolToRaw(bool value, const Setting& setting);

  const char* rawToString(uint16_t raw, const Setting& setting);

  uint8_t getPhysicalReg(int reg);
  void setPhysicalReg(int reg, uint8_t value);
  uint8_t getReg8(int reg, int bitLength, int bitShift);
  void setReg8(int reg, uint8_t value, int bitLength, int bitShift);
  uint16_t getReg16(int widereg, int bitLength, int bitShift);
  void setReg16(int widereg, uint16_t value, int bitLength, int bitShift);

  virtual bool writeReg8ToI2C(int reg);
  virtual bool writeReg16ToI2C(int reg);

  bool setAndWriteRaw(const BQ25798Core::Setting& setting, uint16_t value);
  bool setAndWriteInt(const BQ25798Core::Setting& setting, int value);
  bool setAndWriteBool(const BQ25798Core::Setting& setting, bool value);
  bool setAndWriteFloat(const BQ25798Core::Setting& setting, float value);

  template <typename T>
  bool setAndWriteEnum(const Setting& setting, T value) {
    // the same as int, but with enum value
    return setAndWriteInt(setting, static_cast<int>(value));
  };

 protected:
  std::array<uint8_t, PHYSICAL_REGISTERS_COUNT> _physicalReg8Values;  // Array to hold 8-bit register values
  void _clearRegs();
  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);
  void _setError(int errorCode);

 private:
  int _errorCode = ERROR_NONE;
};

#endif