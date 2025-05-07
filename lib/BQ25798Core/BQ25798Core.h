#ifndef BQ25798CORE_H
#define BQ25798CORE_H

#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "../../src/utility/debug.h"
#include "gen_others.h"
#include "gen_registers.h"

template <class C>
class BQ25798Core : public C {
 public:
  BQ25798Core() : C() {}

  bool begin(uint8_t chip_address);
  bool begin();

  enum class Error {  //
    ERROR_NONE = 0,
    ERROR_I2C_WRITE = 1,
    ERROR_I2C_READ = 2,
    ERROR_INVALID_REGISTER = 3,
    ERROR_INVALID_SETTING = 4,
    ERROR_INVALID_VALUE = 5
  };

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
#define DEFINE_REGISTER(propname) RegisterDefinition{propname, #propname}
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
    const char* unit;                 // Unit for the setting (e.g., "V", "A", "%")

    Setting(regaddr_t reg, bool long_reg, const char* name, const char* unit, settings_type_t type, uint8_t bitlength, uint8_t bitshift,
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
#ifdef BQ25798_MEMORY_SAVER
          unit(""),
#else
          unit(name),
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
      return Setting{RegisterDefinition::invalid().address, false, "INVALID", "", settings_type_t::INT, 0, 0, 0, 0, 0, 0, settings_flags_t::NONE, {}};
    };
    bool isValid() const { return bitlength != 0; };
  };

  // Read from I2C and return the raw or transformed value
  uint16_t getRaw(const Setting& setting);
  int getInt(const Setting& setting);
  bool getBool(const Setting& setting);
  float getFloat(const Setting& setting);
  const char* getString(const Setting& setting);

  #include "gen_settings.h"  // This file is generated by the template engine (Jinja2)
  
  const Setting& getSetting(int id);
  
  // memory-only operations
  void clearError();
  int lastError();
  
  int rawToInt(uint16_t raw, const Setting& setting);
  uint16_t intToRaw(int value, const Setting& setting);
  
  float rawToFloat(uint16_t raw, const Setting& setting);
  uint16_t floatToRaw(float value, const Setting& setting);

  bool rawToBool(uint16_t raw, const Setting& setting);
  uint16_t boolToRaw(bool value, const Setting& setting);

  const char* rawToString(uint16_t raw, const Setting& setting);
  
  void setRaw(const BQ25798Core::Setting& setting, uint16_t value);
  void setInt(const BQ25798Core::Setting& setting, int value);
  void setBool(const BQ25798Core::Setting& setting, bool value);
  void setFloat(const BQ25798Core::Setting& setting, float value);
  template <typename T_ENUM>
  void setEnum(const Setting& setting, T_ENUM value) {
    // the same as int, but with enum value
    setInt(setting, static_cast<int>(value));
  };

  uint8_t readSettingFromReg8(const BQ25798Core::Setting& setting);
  void writeSettingToReg8(uint8_t value, const BQ25798Core::Setting& setting);
  uint16_t readSettingFromReg16(const BQ25798Core::Setting& setting);
  void writeSettingToReg16(uint16_t value, const BQ25798Core::Setting& setting);

  const char* toString(int value, strings_vector_t map);

  protected:
  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);
  void _setError(Error errorCode);
  void _setError(int errorCode);

 private:
  Error _errorCode = Error::ERROR_NONE;
};

#include <assert.h>

template <class C>
bool BQ25798Core<C>::begin(uint8_t chip_address) {
  return this->i2c_begin(chip_address);
}

template <class C>
bool BQ25798Core<C>::begin() {
  return begin(BQ25798_DEFAULT_ADDRESS);
}

template <class C>
const typename BQ25798Core<C>::RegisterDefinition& BQ25798Core<C>::getRegisterDefinition(regaddr_t address) {
  static const RegisterDefinition& invalid_reg_def = RegisterDefinition::invalid();

  if (address >= PHYSICAL_REGISTERS_COUNT) {
    _setError(Error::ERROR_INVALID_REGISTER);
    return invalid_reg_def;
  }

  const RegisterDefinition& reg_def = _registerDefinitions[address];
  assert(reg_def.address == address);  // Ensure the definition is correct

  return reg_def;
};

template <class C>
const typename BQ25798Core<C>::Setting& BQ25798Core<C>::getSetting(int id) {
  static const Setting& invalid_setting = Setting::invalid();

  if (id < 0 || id >= SETTINGS_COUNT) {
    _setError(Error::ERROR_INVALID_SETTING);
    return invalid_setting;
  }

  const Setting& setting = _settingsList[id];
  return setting;
};

#define BITLENGTH_TO_MASK(bitlength) ((1 << bitlength) - 1)

template <class C>
uint8_t BQ25798Core<C>::readSettingFromReg8(const BQ25798Core::Setting& setting) {
  DEBUG_PRINT("[readSettingFromReg8] [reg#=0x%02X, bitsength=%d, bitshift=%d], reg content = 0x%02X\n", setting.reg, setting.bitlength, setting.bitshift, regval);

  uint8_t regval = this->i2c_readReg8(setting.reg);
  DEBUG_PRINT("[readSettingFromReg8]  -> retval = %02X\n", (regval >> setting.bitshift) & BITLENGTH_TO_MASK(setting.bitlength));

  if (this->i2c_isError()) {
    DEBUG_PRINT("[readSettingFromReg8]  -> ERROR\n");
    _setError(Error::ERROR_I2C_READ);
    return 0;
  }

  return (regval >> setting.bitshift) & BITLENGTH_TO_MASK(setting.bitlength);
}

template <class C>
void BQ25798Core<C>::writeSettingToReg8(uint8_t value, const BQ25798Core::Setting& setting) {
  uint8_t shiftedMask = BITLENGTH_TO_MASK(setting.bitlength) << setting.bitshift;
  uint8_t shiftedValue = (value & BITLENGTH_TO_MASK(setting.bitlength)) << setting.bitshift;
  uint8_t negatedMask = 0xFF ^ shiftedMask;

  DEBUG_PRINT("[writeSettingToReg8] 0x%02X [reg#=0x%02X, bitlength=%d, bitshift=%d]\n", value, setting.reg, setting.bitlength, setting.bitshift);

  uint8_t regval = this->i2c_readReg8(setting.reg);
  if (this->i2c_isError()) {
    DEBUG_PRINT("[writeSettingToReg8]  -> ERROR\n");
    _setError(Error::ERROR_I2C_READ);
    return;
  }

  regval &= negatedMask;
  regval |= shiftedValue;
  this->i2c_writeReg8(setting.reg, regval);
  if (this->i2c_isError()) {
    DEBUG_PRINT("[writeSettingToReg8]  -> ERROR\n");
    _setError(Error::ERROR_I2C_WRITE);
    return;
  }

  DEBUG_PRINT("[writeSettingToReg8]  -> reg content = 0x%02X\n", regval);
}

template <class C>
uint16_t BQ25798Core<C>::readSettingFromReg16(const BQ25798Core::Setting& setting) {
  DEBUG_PRINT("[readSettingFromReg16] [reg#=0x%02X, bitlength=%d, bitshift=%d], reg content = 0x%04X\n", setting.reg, setting.bitlength, setting.bitshift, regval);

  uint16_t regval = this->i2c_readReg16(setting.reg);
  if (this->i2c_isError()) {
    DEBUG_PRINT("[readSettingFromReg16]  -> ERROR\n");
    _setError(Error::ERROR_I2C_READ);
    return 0;
  }

  DEBUG_PRINT("[readSettingFromReg16]  -> retval = %04X\n", (regval >> setting.bitshift) & BITLENGTH_TO_MASK(setting.bitlength));
  return (regval >> setting.bitshift) & BITLENGTH_TO_MASK(setting.bitlength);
}

template <class C>
void BQ25798Core<C>::writeSettingToReg16(uint16_t value, const BQ25798Core::Setting& setting) {
  uint16_t shiftedMask = BITLENGTH_TO_MASK(setting.bitlength) << setting.bitshift;
  uint16_t shiftedValue = (value & BITLENGTH_TO_MASK(setting.bitlength)) << setting.bitshift;
  uint16_t negatedMask = 0xFFFF ^ shiftedMask;

  DEBUG_PRINT("[writeSettingToReg16] 0x%04X [widereg#=0x%02X 0x%02X, bitlength=%d, bitshift=%d]\n", value, setting.reg, setting.reg + 1, setting.bitlength, setting.bitshift);

  uint16_t regval = this->i2c_readReg16(setting.reg);
  if (this->i2c_isError()) {
    DEBUG_PRINT("[writeSettingToReg16]  -> ERROR\n");
    _setError(Error::ERROR_I2C_READ);
    return;
  }

  regval &= negatedMask;
  regval |= shiftedValue;
  this->i2c_writeReg16(setting.reg, regval);
  if (this->i2c_isError()) {
    DEBUG_PRINT("[writeSettingToReg16]  -> ERROR\n");
    _setError(Error::ERROR_I2C_WRITE);
    return;
  }

  DEBUG_PRINT("[writeSettingToReg16]  -> reg content = 0x%04X\n", regval);
}

template <class C>
bool BQ25798Core<C>::_flagIsSet(settings_flags_t flagset, settings_flags_t flag) {
  return (static_cast<uint8_t>(flagset) & static_cast<uint8_t>(flag)) != 0;
}

template <class C>
uint16_t BQ25798Core<C>::getRaw(const Setting& setting) {
  uint16_t raw_value = 0;  // getReg* always returns unsigned values, so we need to use unsigned type here

  RegisterDefinition reg_def = getRegisterDefinition(setting.reg);

  DEBUG_PRINT("[getRaw] [reg=0x%02X, bitlength=%d, bitshift=%d]\n", setting.reg, setting.bitlength, setting.bitshift);
  DEBUG_PRINT("[getRaw]   setting=%s [long=%d, reg=0x%02X (%s)]\n", setting.name, setting.long_reg, setting.reg, reg_def.name);

  if (setting.long_reg) {
    raw_value = readSettingFromReg16(setting);
  } else {
    raw_value = readSettingFromReg8(setting);
  }

  DEBUG_PRINT("[getRaw] -> raw 0x%04X\n", raw_value);

  return raw_value;
};

template <class C>
int BQ25798Core<C>::getInt(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToInt(raw_value, setting);
}

template <class C>
bool BQ25798Core<C>::getBool(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToBool(raw_value, setting);
}

template <class C>
float BQ25798Core<C>::getFloat(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToFloat(raw_value, setting);
}

template <class C>
const char* BQ25798Core<C>::getString(const Setting& setting) {
  uint16_t raw_value = getRaw(setting);
  return rawToString(raw_value, setting);
}

// Generic method to convert an integer to a string using a map
template <class C>
const char* BQ25798Core<C>::toString(int value, strings_vector_t map) {
  if (value >= 0 && value < map.size()) {
    return map[value];
  } else {
    _setError(Error::ERROR_INVALID_VALUE);
    return "Unknown";
  }
}

template <class C>
int BQ25798Core<C>::rawToInt(uint16_t raw, const Setting& setting) {
  // anything can be converted to int, no need to check the type

  int value;
  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    if (!setting.long_reg) {
      _setError(Error::ERROR_INVALID_SETTING);
      return 0;
    };

    if (raw & (1 << ((setting.long_reg ? 16 : 8) - 1))) {  // Check if the sign bit is set
      // DEBUG_PRINT("getInt() 2's complement detected, value=0x%04X\n", raw_value);
      int16_t adjusted_value = static_cast<int16_t>(raw);  // Cast to signed type
      // DEBUG_PRINT(" -> adjusted value: 0x%04X\n", adjusted_value);
      value = adjusted_value;
    } else {
      value = raw;  // No adjustment needed for positive values
    }
  } else {
    value = raw;  // No adjustment needed for unsigned values
  }
  DEBUG_PRINT("[rawToInt]  - intermediate 0x%04X\n", value);

  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    value *= setting.bit_step_size;
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  DEBUG_PRINT("[rawToInt] -> final %d\n", value);

  return value;
}

template <class C>
uint16_t BQ25798Core<C>::intToRaw(int value, const Setting& setting) {
  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      _setError(Error::ERROR_INVALID_VALUE);
      return 0;
    }
  }
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.fixed_offset != 0) {
    value -= setting.fixed_offset;
  };
  if (setting.bit_step_size != 0) {
    value /= setting.bit_step_size;
  };

  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    if (!setting.long_reg) {
      _setError(Error::ERROR_INVALID_SETTING);
      return 0;  // Handle error case
    };

    if (value < 0) {
      value = static_cast<uint16_t>(value);  // Cast to unsigned type
    }
  }

  return value;
}

template <class C>
float BQ25798Core<C>::rawToFloat(uint16_t raw, const Setting& setting) {
  // anything can be converted to float, no need to check the type

  float value = raw;
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.bit_step_size != 0) {
    // DEBUG_PRINT("getFloat() adjusting raw value %.3f by bit step size %.3f\n", value, setting.bit_step_size);
    value *= setting.bit_step_size;
    // DEBUG_PRINT(" -> adjusted to %.3f\n", value);
  };
  if (setting.fixed_offset != 0) {
    value += setting.fixed_offset;
  };

  return value;
}

template <class C>
uint16_t BQ25798Core<C>::floatToRaw(float value, const Setting& setting) {
  if (setting.type != settings_type_t::FLOAT) {
    _setError(Error::ERROR_INVALID_VALUE);
    return 0;
  }

  // Check range
  if (setting.range_low != 0 || setting.range_high != 0) {
    if (value < setting.range_low || value > setting.range_high) {
      _setError(Error::ERROR_INVALID_VALUE);
      return 0;
    }
  }
  // Adjust the value based on the fixed offset and bit step size if provided
  if (setting.fixed_offset != 0) {
    value -= setting.fixed_offset;
  };
  if (setting.bit_step_size != 0) {
    value /= setting.bit_step_size;
  };

  if (_flagIsSet(setting.flags, settings_flags_t::IS_2COMPLEMENT)) {
    _setError(Error::ERROR_INVALID_SETTING);
    return 0.0f;
  }

  return value;
}

template <class C>
bool BQ25798Core<C>::rawToBool(uint16_t raw, const Setting& setting) {
  // anything can be converted to bool, no need to check the type
  return raw != 0;
}

template <class C>
uint16_t BQ25798Core<C>::boolToRaw(bool value, const Setting& setting) {
  if (setting.type != settings_type_t::BOOL) {
    _setError(Error::ERROR_INVALID_VALUE);
    return 0;
  }

  return value ? 1 : 0;
}

template <class C>
const char* BQ25798Core<C>::rawToString(uint16_t raw, const Setting& setting) {
  // anything can be converted to string, no need to check the type
  return toString(raw, setting.strings_vector);
}

template <class C>
void BQ25798Core<C>::clearError() {
  _setError(Error::ERROR_NONE);
}
template <class C>
int BQ25798Core<C>::lastError() {
  return static_cast<int>(_errorCode);
}
template <class C>
void BQ25798Core<C>::_setError(int errorCode) {
  _errorCode = static_cast<Error>(errorCode);
}
template <class C>
void BQ25798Core<C>::_setError(Error errorCode) {
  _errorCode = errorCode;
}

template <class C>
void BQ25798Core<C>::setRaw(const Setting& setting, uint16_t value) {
  DEBUG_PRINT("[setRaw] [reg=0x%02X, bitlength=%d, bitshift=%d] := %d\n", setting.reg, setting.bitlength, setting.bitshift, value);

  if (setting.long_reg) {
    writeSettingToReg16(value, setting);
  } else {
    writeSettingToReg8(value, setting);
  }
}

template <class C>
void BQ25798Core<C>::setInt(const Setting& setting, int value) {
  uint16_t raw_value = intToRaw(value, setting);
  setRaw(setting, raw_value);
}

template <class C>
void BQ25798Core<C>::setBool(const Setting& setting, bool value) {
  uint16_t raw_value = boolToRaw(value, setting);
  setRaw(setting, raw_value);
}

template <class C>
void BQ25798Core<C>::setFloat(const Setting& setting, float value) {
  uint16_t raw_value = floatToRaw(value, setting);
  setRaw(setting, raw_value);
}

#endif