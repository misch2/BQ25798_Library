#ifndef BQ25798CORE_H
#define BQ25798CORE_H

#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

// To be able to run both natively and on Arduino:
#ifndef F
  #define F(x) (x)
#endif

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

    RegisterDefinition(regaddr_t address, const char* name) : address(address), name(name) {};

    static RegisterDefinition invalid() { return RegisterDefinition{-1, F("INVALID")}; };
    bool isValid() const { return address != -1; };
  };

  static constexpr uint8_t PHYSICAL_REGISTERS_COUNT = 1 + REG48_Part_Information;  // Number of physical registers (0x00 to 0x48)
#define DEFINE_REGISTER(propname) \
  RegisterDefinition { propname, F(#propname) }
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
    uint16_t mask;         // Bitmask for the setting
    uint8_t shift;         // Bit shift for the setting
    // optional:
    float range_low;
    float range_high;
    float fixed_offset;   // Fixed offset for the setting (e.g., for voltage settings)
    float bit_step_size;  // Bit step size for the setting (e.g., for current settings)
    // adjusted value = raw_value * bit_step_size + fixed_offset
    settings_flags_t flags;           // Flags for the setting (e.g., 2's complement)
    strings_vector_t strings_vector;  // Vector of strings for enum settings
    const char* name;                 // Register name

    Setting(regaddr_t reg, bool long_reg, const char* name, settings_type_t type, uint16_t mask, uint8_t shift,
            // optional parameters:
            float range_low = 0, float range_high = 0, float fixed_offset = 0, float bit_step_size = 0, settings_flags_t flags = settings_flags_t::NONE,
            strings_vector_t strings_vector = {})
        : reg(reg),
          long_reg(long_reg),
          name(name),
          type(type),
          mask(mask),
          shift(shift),
          range_low(range_low),
          range_high(range_high),
          fixed_offset(fixed_offset),
          bit_step_size(bit_step_size),
          flags(flags),
          strings_vector(strings_vector) {};

    static Setting invalid() {
      return Setting{RegisterDefinition::invalid().address, false, F("INVALID"), settings_type_t::INT, 0, 0, 0, 0, 0, 0, settings_flags_t::NONE, {}};
    };
    bool isValid() const { return mask != 0; };
  };

  uint16_t getRaw(const Setting& setting);
  int getInt(const Setting& setting);
  bool getBool(const Setting& setting);
  float getFloat(const Setting& setting);
  const char* getString(const Setting& setting);
  const char* toString(int value, strings_vector_t map);

#define _DEFINE_RO_BOOL(setting, long_reg, regaddr, mask, shift)                       \
  Setting setting = {regaddr, long_reg, #setting, settings_type_t::BOOL, mask, shift}; \
  bool get##setting() { return getBool(setting); }

#define _DEFINE_RW_BOOL(setting, long_reg, regaddr, mask, shift) \
  _DEFINE_RO_BOOL(setting, long_reg, regaddr, mask, shift)       \
  bool setAndWrite##setting(bool val) { return setAndWriteBool(setting, val); }

#define _DEFINE_RO_INT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  Setting setting = {regaddr,    long_reg,     #setting,      settings_type_t::INT,   mask, shift, range_low,              \
                     range_high, fixed_offset, bit_step_size, settings_flags_t::flags};                                    \
  int get##setting() { return getInt(setting); }
#define _DEFINE_RW_INT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  _DEFINE_RO_INT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags)       \
  bool setAndWrite##setting(int val) { return setAndWriteInt(setting, val); }

#define _DEFINE_RO_FLOAT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  Setting setting = {regaddr,    long_reg,     #setting,      settings_type_t::FLOAT, mask, shift, range_low,                \
                     range_high, fixed_offset, bit_step_size, settings_flags_t::flags};                                      \
  float get##setting() { return getFloat(setting); }
#define _DEFINE_RW_FLOAT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  _DEFINE_RO_FLOAT(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags)       \
  bool setAndWrite##setting(float val) { return setAndWriteFloat(setting, val); }

#define _DEFINE_RO_ENUM(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags)                                  \
  Setting setting = {                                                                                                                                        \
      regaddr,          long_reg, #setting, settings_type_t::ENUM, mask, shift, range_low, range_high, fixed_offset, bit_step_size, settings_flags_t::flags, \
      setting##_strings};                                                                                                                                    \
  int get##setting() { return getInt(setting); }                                                                                                             \
  const char* get##setting##_string() { return getString(setting); }
#define _DEFINE_RW_ENUM(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  _DEFINE_RO_ENUM(setting, long_reg, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags)       \
  bool setAndWrite##setting(setting##_t val) { return setAndWriteEnum<setting##_t>(setting, val); }

  // #define WRAP_COMPLEX(...) __VA_ARGS__
  // #define TODO_DEFINE_SETTING_SHORT_ENUM(setting, regaddr, mask, shift, enumarray, stringarray) \
//   enum class setting##_t : uint8_t enumarray;                                                 \
//   strings_vector_t setting##_strings = {stringarray};                                         \
//   Setting setting = {regaddr, false, #setting, settings_type_t::ENUM, mask, shift, 0, 0, 0, 0, settings_flags_t::NONE, setting##_strings}

#define DEFINE_SETTING_SHORT_BOOL(setting, regaddr, mask, shift) _DEFINE_RW_BOOL(setting, false, regaddr, mask, shift)
#define DEFINE_SETTING_SHORT_BOOL_READONLY(setting, regaddr, mask, shift) _DEFINE_RO_BOOL(setting, false, regaddr, mask, shift)

#define DEFINE_SETTING_SHORT_INT(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RW_INT(setting, false, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, NONE)
#define DEFINE_SETTING_LONG_INT(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RW_INT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, NONE)
#define DEFINE_SETTING_LONG_INT_READONLY(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RO_INT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, NONE)
#define DEFINE_SETTING_LONG_INT_2COMPLEMENT(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RW_INT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, IS_2COMPLEMENT)
#define DEFINE_SETTING_LONG_INT_2COMPLEMENT_READONLY(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RO_INT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, IS_2COMPLEMENT)

#define DEFINE_SETTING_LONG_FLOAT(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RW_FLOAT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, NONE)
#define DEFINE_SETTING_LONG_FLOAT_READONLY(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RO_FLOAT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, NONE)
#define DEFINE_SETTING_LONG_FLOAT_2COMPLEMENT(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RW_FLOAT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, IS_2COMPLEMENT)
#define DEFINE_SETTING_LONG_FLOAT_2COMPLEMENT_READONLY(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  _DEFINE_RO_FLOAT(setting, true, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, IS_2COMPLEMENT)

#define DEFINE_SETTING_SHORT_ENUM(setting, regaddr, mask, shift) _DEFINE_RW_ENUM(setting, true, regaddr, mask, shift, 0, 0, 0, 0, NONE)
#define DEFINE_SETTING_SHORT_ENUM_READONLY(setting, regaddr, mask, shift) _DEFINE_RO_ENUM(setting, true, regaddr, mask, shift, 0, 0, 0, 0, NONE)

  // ===================================
  // REG00_Minimal_System_Voltage
  // ===================================
  DEFINE_SETTING_SHORT_INT(VSYSMIN, REG00_Minimal_System_Voltage, 0x3F, 0, 2500, 16000, 2500, 250);

  // ===================================
  // REG01_Charge_Voltage_Limit (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT(VREG, REG01_Charge_Voltage_Limit, 0x7FF, 0, 3000, 18800, 0, 10);

  // ===================================
  // REG03_Charge_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT(ICHG, REG03_Charge_Current_Limit, 0x1FF, 0, 50, 5000, 0, 10);

  // ===================================
  // REG05_Input_Voltage_Limit
  // ===================================
  DEFINE_SETTING_SHORT_INT(VINDPM, REG05_Input_Voltage_Limit, 0xFF, 0, 3600, 22000, 0, 100);

  // ===================================
  // REG06_Input_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT(IINDPM, REG06_Input_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ===================================
  // REG08_Precharge_Control
  // ===================================
  enum class VBAT_LOWV_t : uint8_t { PCT_15 = 0, PCT_62 = 1, PCT_67 = 2, PCT_71 = 3 };
  strings_vector_t VBAT_LOWV_strings = {{F("15%(VREG)")}, {F("62.2%(VREG)")}, {F("66.7%(VREG)")}, {F("71.4%(VREG)")}};
  DEFINE_SETTING_SHORT_ENUM(VBAT_LOWV, REG08_Precharge_Control, 0x3, 6);

  DEFINE_SETTING_SHORT_INT(IPRECHG, REG08_Precharge_Control, 0x3F, 0, 40, 2000, 0, 40);

  // ===================================
  // REG09_Termination_Control
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(REG_RST, REG09_Termination_Control, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(STOP_WD_CHG, REG09_Termination_Control, 0x01, 5);

  DEFINE_SETTING_SHORT_INT(ITERM, REG09_Termination_Control, 0x1F, 0, 40, 1000, 0, 40);

  // ===================================
  // REG0A_Recharge_Control
  // ===================================
  enum class CELL_t : uint8_t { CELL_1S = 0, CELL_2S = 1, CELL_3S = 2, CELL_4S = 3 };
  strings_vector_t CELL_strings = {{F("1S")}, {F("2S")}, {F("3S")}, {F("4S")}};
  DEFINE_SETTING_SHORT_ENUM(CELL, REG0A_Recharge_Control, 0x03, 6);

  enum class TRECHG_t : uint8_t { TRECHG_64MS = 0, TRECHG_256MS = 1, TRECHG_1024MS = 2, TRECHG_2048MS = 3 };
  strings_vector_t TRECHG_strings = {{F("64 ms")}, {F("256 ms")}, {F("1024 ms")}, {F("2048 ms")}};
  DEFINE_SETTING_SHORT_ENUM(TRECHG, REG0A_Recharge_Control, 0x03, 4);

  DEFINE_SETTING_SHORT_INT(VRECHG, REG0A_Recharge_Control, 0x0F, 0, 50, 800, 50, 50);

  // ===================================
  // REG0B_VOTG_regulation (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT(VOTG, REG0B_VOTG_regulation, 0x7FF, 0, 2800, 22000, 2800, 10);

  // ===================================
  // REG0D_IOTG_regulation
  // ===================================
  enum class PRECHG_TMR_t : uint8_t {
    PRECHG_TMR_2HRS = 0,
    PRECHG_TMR_30MIN = 1,
  };
  strings_vector_t PRECHG_TMR_strings = {{F("2 hours")}, {F("30 minutes")}};
  DEFINE_SETTING_SHORT_ENUM(PRECHG_TMR, REG0D_IOTG_regulation, 0x01, 7);

  DEFINE_SETTING_SHORT_INT(IOTG, REG0D_IOTG_regulation, 0x7F, 0, 160, 3360, 0, 40);

  // ===================================
  // REG0E_Timer_Control
  // ===================================
  enum class TOPOFF_TMR_t : uint8_t { TOPOFF_TMR_DISABLED = 0, TOPOFF_TMR_15MIN = 1, TOPOFF_TMR_30MIN = 2, TOPOFF_TMR_45MIN = 3 };
  strings_vector_t TOPOFF_TMR_strings = {{F("Disabled")}, {F("15 minutes")}, {F("30 minutes")}, {F("45 minutes")}};
  DEFINE_SETTING_SHORT_ENUM(TOPOFF_TMR, REG0E_Timer_Control, 0x03, 6);

  DEFINE_SETTING_SHORT_BOOL(EN_TRICHG_TMR, REG0E_Timer_Control, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(EN_PRECHG_TMR, REG0E_Timer_Control, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL(EN_CHG_TMR, REG0E_Timer_Control, 0x01, 3);

  enum class CHG_TMR_t : uint8_t { CHG_TMR_5HRS = 0, CHG_TMR_8HRS = 1, CHG_TMR_12HRS = 2, CHG_TMR_24HRS = 3 };
  strings_vector_t CHG_TMR_strings = {{F("5 hours")}, {F("8 hours")}, {F("12 hours")}, {F("24 hours")}};
  DEFINE_SETTING_SHORT_ENUM(CHG_TMR, REG0E_Timer_Control, 0x03, 1);

  DEFINE_SETTING_SHORT_BOOL(TMR2X_EN, REG0E_Timer_Control, 0x01, 0);

  // ===================================
  // REG0F_Charger_Control_0
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(EN_AUTO_IBATDIS, REG0F_Charger_Control_0, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(FORCE_IBATDIS, REG0F_Charger_Control_0, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(EN_CHG, REG0F_Charger_Control_0, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(EN_ICO, REG0F_Charger_Control_0, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL(FORCE_ICO, REG0F_Charger_Control_0, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(EN_HIZ, REG0F_Charger_Control_0, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(EN_TERM, REG0F_Charger_Control_0, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL(EN_BACKUP, REG0F_Charger_Control_0, 0x01, 0);

  // ===================================
  // REG10_Charger_Control_1
  // ===================================
  enum class VBUS_BACKUP_t : uint8_t { PCT_VBUS_BACKUP_40 = 0, PCT_VBUS_BACKUP_60 = 1, PCT_VBUS_BACKUP_80 = 2, PCT_VBUS_BACKUP_100 = 3 };
  strings_vector_t VBUS_BACKUP_strings = {{F("<40%(VINDPM)")}, {F("<60%(VINDPM)")}, {F("<80%(VINDPM)")}, {F("<100%(VINDPM)")}};
  DEFINE_SETTING_SHORT_ENUM(VBUS_BACKUP, REG10_Charger_Control_1, 0x03, 6);

  enum class VAC_OVP_t : uint8_t { VAC_OVP_26V = 0, VAC_OVP_22V = 1, VAC_OVP_12V = 2, VAC_OVP_7V = 3 };
  strings_vector_t VAC_OVP_strings = {{F(">26V")}, {F(">22V")}, {F(">12V")}, {F(">7V")}};
  DEFINE_SETTING_SHORT_ENUM(VAC_OVP, REG10_Charger_Control_1, 0x03, 4);

  DEFINE_SETTING_SHORT_BOOL(WD_RST, REG10_Charger_Control_1, 0x01, 3);

  enum class WATCHDOG_t : uint8_t {
    WATCHDOG_DISABLE = 0,
    WATCHDOG_05S = 1,
    WATCHDOG_1S = 2,
    WATCHDOG_2S = 3,
    WATCHDOG_20S = 4,
    WATCHDOG_40S = 5,
    WATCHDOG_80S = 6,
    WATCHDOG_160S = 7
  };
  strings_vector_t WATCHDOG_strings = {{F("Disabled")}, {F("0.5s")}, {F("1s")}, {F("2s")}, {F("20s")}, {F("40s")}, {F("80s")}, {F("160s")}};
  DEFINE_SETTING_SHORT_ENUM(WATCHDOG, REG10_Charger_Control_1, 0x07, 0);

  // ===================================
  // REG11_Charger_Control_2
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(FORCE_INDET, REG11_Charger_Control_2, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(AUTO_INDET_EN, REG11_Charger_Control_2, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(EN_12V, REG11_Charger_Control_2, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(EN_9V, REG11_Charger_Control_2, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL(HVDCP_EN, REG11_Charger_Control_2, 0x01, 3);

  enum class SDRV_CTRL_t : uint8_t { IDLE = 0, SHUTDOWN = 1, SHIP = 2, SYS_PWR_RST = 3 };
  strings_vector_t SDRV_CTRL_strings = {{F("Idle")}, {F("Shutdown")}, {F("Ship")}, {F("System Power Reset")}};
  DEFINE_SETTING_SHORT_ENUM(SDRV_CTRL, REG11_Charger_Control_2, 0x03, 1);

  enum class SDRV_DLY_t : uint8_t { SDRV_DLY_10S = 0, SDRV_DLY_0S = 1 };
  strings_vector_t SDRV_DLY_strings = {{F("10s")}, {F("0s")}};
  DEFINE_SETTING_SHORT_ENUM(SDRV_DLY, REG11_Charger_Control_2, 0x01, 0);

  // ===================================
  // REG12_Charger_Control_3
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(DIS_ACDRV, REG12_Charger_Control_3, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(EN_OTG, REG12_Charger_Control_3, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(PFM_OTG_DIS, REG12_Charger_Control_3, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(PFM_FWD_DIS, REG12_Charger_Control_3, 0x01, 4);

  enum class WKUP_DLY_t : uint8_t { WKUP_DLY_1S = 0, WKUP_DLY_15MS = 1 };
  strings_vector_t WKUP_DLY_strings = {{F("1s")}, {F("15ms")}};
  DEFINE_SETTING_SHORT_ENUM(WKUP_DLY, REG12_Charger_Control_3, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(DIS_LDO, REG12_Charger_Control_3, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(DIS_OTG_OOA, REG12_Charger_Control_3, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL(DIS_FWD_OOA, REG12_Charger_Control_3, 0x01, 0);

  // ===================================
  // REG13_Charger_Control_4
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(EN_ACDRV2, REG13_Charger_Control_4, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(EN_ACDRV1, REG13_Charger_Control_4, 0x01, 6);

  enum class PWM_FREQ_t : uint8_t { PWM_FREQ_1_5MHZ = 0, PWM_FREQ_750KHZ = 1 };
  strings_vector_t PWM_FREQ_strings = {{F("1.5 MHz")}, {F("750 kHz")}};
  DEFINE_SETTING_SHORT_ENUM(PWM_FREQ, REG13_Charger_Control_4, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(DIS_STAT, REG13_Charger_Control_4, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL(DIS_VSYS_SHORT, REG13_Charger_Control_4, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(DIS_VOTG_UVP, REG13_Charger_Control_4, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(FORCE_VINDPM_DET, REG13_Charger_Control_4, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL(EN_IBUS_OCP, REG13_Charger_Control_4, 0x01, 0);

  // ===================================
  // REG14_Charger_Control_5
  // ===================================
  DEFINE_SETTING_SHORT_BOOL(SFET_PRESENT, REG14_Charger_Control_5, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(EN_IBAT, REG14_Charger_Control_5, 0x01, 5);

  enum class IBAT_REG_t : uint8_t { IBAT_REG_3A = 0, IBAT_REG_4A = 1, IBAT_REG_5A = 2, IBAT_REG_DISABLE = 3 };
  strings_vector_t IBAT_REG_strings = {{F("3A")}, {F("4A")}, {F("5A")}, {F("Disabled")}};
  DEFINE_SETTING_SHORT_ENUM(IBAT_REG, REG14_Charger_Control_5, 0x03, 3);

  DEFINE_SETTING_SHORT_BOOL(EN_IINDPM, REG14_Charger_Control_5, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(EN_EXTILIM, REG14_Charger_Control_5, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL(EN_BATOC, REG14_Charger_Control_5, 0x01, 0);

  // ===================================
  // REG15_MPPT_Control
  // ===================================
  enum class VOC_PCT_t : uint8_t {
    VOC_PCT_0_5625 = 0,
    VOC_PCT_0_625 = 1,
    VOC_PCT_0_6875 = 2,
    VOC_PCT_0_75 = 3,
    VOC_PCT_0_8125 = 4,
    VOC_PCT_0_875 = 5,
    VOC_PCT_0_9375 = 6,
    VOC_PCT_1 = 7
  };
  strings_vector_t VOC_PCT_strings = {{F("0.5625")}, {F("0.625")}, {F("0.6875")}, {F("0.75")}, {F("0.8125")}, {F("0.875")}, {F("0.9375")}, {F("1")}};
  DEFINE_SETTING_SHORT_ENUM(VOC_PCT, REG15_MPPT_Control, 0x07, 5);

  enum class VOC_DLY_t : uint8_t { VOC_DLY_50MS = 0, VOC_DLY_300MS = 1, VOC_DLY_2S = 2, VOC_DLY_5S = 3 };
  strings_vector_t VOC_DLY_strings = {{F("50ms")}, {F("300ms")}, {F("2s")}, {F("5s")}};
  DEFINE_SETTING_SHORT_ENUM(VOC_DLY, REG15_MPPT_Control, 0x03, 4);

  enum class VOC_RATE_t : uint8_t { VOC_RATE_30S = 0, VOC_RATE_2MIN = 1, VOC_RATE_10MIN = 2, VOC_RATE_30MIN = 3 };
  strings_vector_t VOC_RATE_strings = {{F("30s")}, {F("2min")}, {F("10min")}, {F("30min")}};
  DEFINE_SETTING_SHORT_ENUM(VOC_RATE, REG15_MPPT_Control, 0x03, 2);

  DEFINE_SETTING_SHORT_BOOL(EN_MPPT, REG15_MPPT_Control, 0x01, 0);

  // ===================================
  // REG16_Temperature_Control
  // ===================================
  enum class TREG_t : uint8_t { TREG_60 = 0, TREG_80 = 1, TREG_100 = 2, TREG_120 = 3 };
  strings_vector_t TREG_strings = {{F("60'C")}, {F("80'C")}, {F("100'C")}, {F("120'C")}};
  DEFINE_SETTING_SHORT_ENUM(TREG, REG16_Temperature_Control, 0x03, 6);

  enum class TSHUT_t : uint8_t { TSHUT_150 = 0, TSHUT_130 = 1, TSHUT_120 = 2, TSHUT_85 = 3 };
  strings_vector_t TSHUT_strings = {{F("150'C")}, {F("130'C")}, {F("120'C")}, {F("85'C")}};
  DEFINE_SETTING_SHORT_ENUM(TSHUT, REG16_Temperature_Control, 0x03, 4);

  DEFINE_SETTING_SHORT_BOOL(VBUS_PD_EN, REG16_Temperature_Control, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(VAC1_PD_EN, REG16_Temperature_Control, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(VAC2_PD_EN, REG16_Temperature_Control, 0x01, 1);

  enum class BKUP_ACFET1_ON_t : uint8_t { IDLE = 0, TURN_ON = 1 };
  strings_vector_t BKUP_ACFET1_ON_strings = {{F("Idle")}, {F("Turn on ACFET1 in backup mode")}};
  DEFINE_SETTING_SHORT_ENUM(BKUP_ACFET1_ON, REG16_Temperature_Control, 0x01, 0);

  // ==================================
  // REG17_NTC_Control_0
  // ==================================
  enum class JEITA_VSET_t : uint8_t {
    CHARGE_SUSPEND = 0,
    SET_VREG_TO_VREG_800MV = 1,
    SET_VREG_TO_VREG_600MV = 2,
    SET_VREG_TO_VREG_400MV = 3,
    SET_VREG_TO_VREG_300MV = 4,
    SET_VREG_TO_VREG_200MV = 5,
    SET_VREG_TO_VREG_100MV = 6,
    VREG_UNCHANGED = 7
  };
  strings_vector_t JEITA_VSET_strings = {
      {F("Charge Suspend")},         {F("Set VREG to VREG-800mV")}, {F("Set VREG to VREG-600mV")}, {F("Set VREG to VREG-400mV (default)")},
      {F("Set VREG to VREG-300mV")}, {F("Set VREG to VREG-200mV")}, {F("Set VREG to VREG-100mV")}, {F("VREG unchanged")}};
  DEFINE_SETTING_SHORT_ENUM(JEITA_VSET, REG17_NTC_Control_0, 0x07, 5);

  enum class JEITA_ISETH_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETH_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};
  DEFINE_SETTING_SHORT_ENUM(JEITA_ISETH, REG17_NTC_Control_0, 0x03, 3);

  enum class JEITA_ISETC_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETC_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG (default)")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};
  DEFINE_SETTING_SHORT_ENUM(JEITA_ISETC, REG17_NTC_Control_0, 0x03, 1);

  // ==================================
  // REG18_NTC_Control_1
  // ==================================
  enum class TS_COOL_t : uint8_t { TS_5 = 0, TS_10 = 1, TS_15 = 2, TS_20 = 3 };
  strings_vector_t TS_COOL_strings = {{F("5'C")}, {F("10'C (default)")}, {F("15'C")}, {F("20'C")}};
  DEFINE_SETTING_SHORT_ENUM(TS_COOL, REG18_NTC_Control_1, 0x03, 6);

  enum class TS_WARM_t : uint8_t { TS_40 = 0, TS_45 = 1, TS_50 = 2, TS_55 = 3 };
  strings_vector_t TS_WARM_strings = {{F("40'C")}, {F("45'C (default)")}, {F("50'C")}, {F("55'C")}};
  DEFINE_SETTING_SHORT_ENUM(TS_WARM, REG18_NTC_Control_1, 0x03, 4);

  enum class BHOT_t : uint8_t { TS_55 = 0, TS_60 = 1, TS_65 = 2, DISABLE = 3 };
  strings_vector_t BHOT_strings = {{F("55'C")}, {F("60'C (default)")}, {F("65'C")}, {F("Disabled")}};
  DEFINE_SETTING_SHORT_ENUM(BHOT, REG18_NTC_Control_1, 0x03, 2);

  enum class BCOLD_t : uint8_t { MINUS_10 = 0, MINUS_20 = 1 };
  strings_vector_t BCOLD_strings = {{F("-10'C (default)")}, {F("-20'C")}};
  DEFINE_SETTING_SHORT_ENUM(BCOLD, REG18_NTC_Control_1, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL(TS_IGNORE, REG18_NTC_Control_1, 0x01, 0);

  // ==================================
  // REG19_ICO_Current_Limit (wide)
  // ==================================
  DEFINE_SETTING_LONG_INT_READONLY(ICO_ILIM, REG19_ICO_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ==================================
  // REG1B_Charger_Status_0
  // ==================================
  enum class IINDPM_STAT_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t IINDPM_STAT_strings = {{F("Normal")}, {F("In IINDPM regulation or IOTG regulation")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(IINDPM_STAT, REG1B_Charger_Status_0, 0x01, 7);

  enum class VINDPM_STAT_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t VINDPM_STAT_strings = {{F("Normal")}, {F("In VINDPM regulation or VOTG regulation")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VINDPM_STAT, REG1B_Charger_Status_0, 0x01, 6);

  enum class WD_STAT_t : uint8_t { NORMAL = 0, EXPIRED = 1 };
  strings_vector_t WD_STAT_strings = {{F("Normal")}, {F("Watchdog timer expired")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(WD_STAT, REG1B_Charger_Status_0, 0x01, 5);

  enum class PG_STAT_t : uint8_t { BAD = 0, GOOD = 1 };
  strings_vector_t PG_STAT_strings = {{F("Not in power good status")}, {F("Power good")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(PG_STAT, REG1B_Charger_Status_0, 0x01, 3);

  enum class AC2_PRESENT_STAT_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC2_PRESENT_STAT_strings = {{F("VAC2 NOT present")}, {F("VAC2 present (above present threshold)")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(AC2_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 2);

  enum class AC1_PRESENT_STAT_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC1_PRESENT_STAT_strings = {{F("VAC1 NOT present")}, {F("VAC1 present (above present threshold)")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(AC1_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 1);

  enum class VBUS_PRESENT_STAT_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBUS_PRESENT_STAT_strings = {{F("VBUS NOT present")}, {F("VBUS present (above present threshold)")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VBUS_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 0);

  // ==================================
  // REG1C_Charger_Status_1
  // ==================================
  enum class CHG_STAT_t : uint8_t {
    NOT_CHARGING = 0,
    TRICKLECHARGE = 1,
    PRECHARGE = 2,
    FASTCHARGE_CC = 3,
    FASTCHARGE_CV = 4,
    RESERVED_5 = 5,
    TOPOFF = 6,
    TERMINATED = 7
  };
  strings_vector_t CHG_STAT_strings = {{F("Not Charging")},
                                       {F("Trickle Charge")},
                                       {F("Pre-charge")},
                                       {F("Fast charge (CC mode)")},
                                       {F("Taper Charge (CV mode)")},
                                       {F("Reserved")},
                                       {F("Top-off Timer Active Charging")},
                                       {F("Charge Termination Done")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(CHG_STAT, REG1C_Charger_Status_1, 0x7, 5);

  enum class VBUS_STAT_t : uint8_t {
    NO_INPUT = 0,
    USB_SDP = 1,
    USB_CDP = 2,
    USB_DCP = 3,
    ADJUSTABLE_HVDCP = 4,
    UNKNOWN_ADAPTOR = 5,
    NON_STANDARD_ADAPTER = 6,
    OTG_MODE = 7,
    NOT_QUALIFIED_ADAPTOR = 8,
    RESERVED_9 = 9,
    RESERVED_A = 10,
    DEVICE_POWERED_FROM_VBUS = 11,
    BACKUP_MODE = 12,
    RESERVED_D = 13,
    RESERVED_E = 14,
    RESERVED_F = 15
  };
  strings_vector_t VBUS_STAT_strings = {{F("No Input or BHOT or BCOLD in OTG mode")},
                                        {F("USB SDP (500mA)")},
                                        {F("USB CDP (1.5A)")},
                                        {F("USB DCP (3.25A)")},
                                        {F("Adjustable High Voltage DCP (HVDCP) (1.5A)")},
                                        {F("Unknown adaptor (3A)")},
                                        {F("Non-Standard Adapter (1A/2A/2.1A/2.4A)")},
                                        {F("In OTG mode")},
                                        {F("Not qualified adaptor")},
                                        {F("Reserved")},
                                        {F("Reserved")},
                                        {F("Device directly powered from VBUS")},
                                        {F("Backup Mode")},
                                        {F("Reserved")},
                                        {F("Reserved")},
                                        {F("Reserved")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VBUS_STAT, REG1C_Charger_Status_1, 0x0F, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(BC12_DONE_STAT, REG1C_Charger_Status_1, 0x01, 0);

  // ==================================
  // REG1D_Charger_Status_2
  // ==================================
  enum class ICO_STAT_t : uint8_t { ICO_DISABLED = 0, ICO_IN_PROGRESS = 1, ICO_MAX_CURRENT_DETECTED = 2, ICO_RESERVED = 3 };
  strings_vector_t ICO_STAT_strings = {{F("ICO disabled")}, {F("ICO optimization in progress")}, {F("Maximum input current detected")}, {F("Reserved")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(ICO_STAT, REG1D_Charger_Status_2, 0x03, 6);

  enum class TREG_STAT_t : uint8_t { NORMAL = 0, THERMAL_REGULATION = 1 };
  strings_vector_t TREG_STAT_strings = {{F("Normal")}, {F("Device in thermal regulation")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TREG_STAT, REG1D_Charger_Status_2, 0x01, 5);

  enum class DPDM_STAT_t : uint8_t { NOT_STARTED = 0, IN_PROGRESS = 1 };
  strings_vector_t DPDM_STAT_strings = {{F("D+/D- detection NOT started yet or done")}, {F("D+/D- detection in progress")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(DPDM_STAT, REG1D_Charger_Status_2, 0x01, 4);

  enum class VBAT_PRESENT_STAT_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBAT_PRESENT_STAT_strings = {{F("VBAT NOT present")}, {F("VBAT present")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VBAT_PRESENT_STAT, REG1D_Charger_Status_2, 0x01, 0);

  // ==================================
  // REG1E_Charger_Status_3
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(ACRB2_STAT, REG1E_Charger_Status_3, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(ACRB1_STAT, REG1E_Charger_Status_3, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(ADC_DONE_STAT, REG1E_Charger_Status_3, 0x01, 5);

  enum class VSYS_STAT_t : uint8_t { NOT_IN_VSYSMIN_REGULATION = 0, IN_VSYSMIN_REGULATION = 1 };
  strings_vector_t VSYS_STAT_strings = {{F("Not in VSYSMIN regulation (VBAT > VSYSMIN)")}, {F("In VSYSMIN regulation (VBAT < VSYSMIN)")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VSYS_STAT, REG1E_Charger_Status_3, 0x01, 4);

  enum class CHG_TMR_STAT_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t CHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(CHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 3);

  enum class TRICHG_TMR_STAT_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t TRICHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TRICHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 2);

  enum class PRECHG_TMR_STAT_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t PRECHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(PRECHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 1);

  // ==================================
  // REG1F_Charger_Status_4
  // ==================================
  enum class VBATOTG_LOW_STAT_t : uint8_t { VBATOTG_LOW = 0, VBATOTG_OK = 1 };
  strings_vector_t VBATOTG_LOW_STAT_strings = {{F("VBAT is too low to enable OTG mode")}, {F("VBAT is high enough to enable OTG operation")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(VBATOTG_LOW_STAT, REG1F_Charger_Status_4, 0x01, 4);

  enum class TS_COLD_STAT_t : uint8_t { NOT_COLD = 0, COLD = 1 };
  strings_vector_t TS_COLD_STAT_strings = {{F("TS NOT in cold range")}, {F("TS in cold range")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TS_COLD_STAT, REG1F_Charger_Status_4, 0x01, 3);

  enum class TS_COOL_STAT_t : uint8_t { NOT_COOL = 0, COOL = 1 };
  strings_vector_t TS_COOL_STAT_strings = {{F("TS NOT in cool range")}, {F("TS in cool range")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TS_COOL_STAT, REG1F_Charger_Status_4, 0x01, 2);

  enum class TS_WARM_STAT_t : uint8_t { NOT_WARM = 0, WARM = 1 };
  strings_vector_t TS_WARM_STAT_strings = {{F("TS NOT in warm range")}, {F("TS in warm range")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TS_WARM_STAT, REG1F_Charger_Status_4, 0x01, 1);

  enum class TS_HOT_STAT_t : uint8_t { NOT_HOT = 0, HOT = 1 };
  strings_vector_t TS_HOT_STAT_strings = {{F("TS NOT in hot range")}, {F("TS in hot range")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(TS_HOT_STAT, REG1F_Charger_Status_4, 0x01, 0);

  // ==================================
  // REG20_FAULT_Status_0
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(IBAT_REG_STAT, REG20_FAULT_Status_0, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBUS_OVP_STAT, REG20_FAULT_Status_0, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBAT_OVP_STAT, REG20_FAULT_Status_0, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(IBUS_OCP_STAT, REG20_FAULT_Status_0, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(IBAT_OCP_STAT, REG20_FAULT_Status_0, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL_READONLY(CONV_OCP_STAT, REG20_FAULT_Status_0, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VAC2_OVP_STAT, REG20_FAULT_Status_0, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VAC1_OVP_STAT, REG20_FAULT_Status_0, 0x01, 0);

  // ==================================
  // REG21_FAULT_Status_1
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(VSYS_SHORT_STAT, REG21_FAULT_Status_1, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VSYS_OVP_STAT, REG21_FAULT_Status_1, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(OTG_OVP_STAT, REG21_FAULT_Status_1, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(OTG_UVP_STAT, REG21_FAULT_Status_1, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TSHUT_STAT, REG21_FAULT_Status_1, 0x01, 2);

  // ==================================
  // REG22_Charger_Flag_0
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(IINDPM_FLAG, REG22_Charger_Flag_0, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VINDPM_FLAG, REG22_Charger_Flag_0, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(WD_FLAG, REG22_Charger_Flag_0, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(POORSRC_FLAG, REG22_Charger_Flag_0, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(PG_FLAG, REG22_Charger_Flag_0, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL_READONLY(AC2_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(AC1_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBUS_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 0);

  // ===================================
  // REG23_Charger_Flag_1
  // ===================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(CHG_FLAG, REG23_Charger_Flag_1, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(ICO_FLAG, REG23_Charger_Flag_1, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBUS_FLAG, REG23_Charger_Flag_1, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TREG_FLAG, REG23_Charger_Flag_1, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBAT_PRESENT_FLAG, REG23_Charger_Flag_1, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(BC1_2_DONE_FLAG, REG23_Charger_Flag_1, 0x01, 0);

  // ===================================
  // REG24_Charger_Flag_2
  // ===================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(DPDM_DONE_FLAG, REG24_Charger_Flag_2, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(ADC_DONE_FLAG, REG24_Charger_Flag_2, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VSYS_FLAG, REG24_Charger_Flag_2, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(CHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TRICHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(PRECHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TOPOFF_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 0);

  // ==================================
  // REG25_Charger_Flag_3
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(VBATOTG_LOW_FLAG, REG25_Charger_Flag_3, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TS_COLD_FLAG, REG25_Charger_Flag_3, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TS_COOL_FLAG, REG25_Charger_Flag_3, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TS_WARM_FLAG, REG25_Charger_Flag_3, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TS_HOT_FLAG, REG25_Charger_Flag_3, 0x01, 0);

  // ==================================
  // REG26_FAULT_Flag_0
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(IBAT_REG_FLAG, REG26_FAULT_Flag_0, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBUS_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VBAT_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(IBUS_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(IBAT_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL_READONLY(CONV_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VAC2_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 1);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VAC1_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 0);

  // ==================================
  // REG27_FAULT_Flag_1
  // ==================================
  DEFINE_SETTING_SHORT_BOOL_READONLY(VSYS_SHORT_FLAG, REG27_FAULT_Flag_1, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL_READONLY(VSYS_OVP_FLAG, REG27_FAULT_Flag_1, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL_READONLY(OTG_OVP_FLAG, REG27_FAULT_Flag_1, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL_READONLY(OTG_UVP_FLAG, REG27_FAULT_Flag_1, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL_READONLY(TSHUT_FLAG, REG27_FAULT_Flag_1, 0x01, 2);

  // FIXME REG28_Charger_Mask_0
  // FIXME REG29_Charger_Mask_1
  // FIXME REG2A_Charger_Mask_2
  // FIXME REG2B_Charger_Mask_3
  // FIXME REG2C_FAULT_Mask_0
  // FIXME REG2D_FAULT_Mask_1

  // ==================================
  // REG2E_ADC_Control
  // ==================================
  DEFINE_SETTING_SHORT_BOOL(ADC_EN, REG2E_ADC_Control, 0x01, 7);

  enum class ADC_RATE_t : uint8_t { CONTINUOUS = 0, ONESHOT = 1 };
  strings_vector_t ADC_RATE_strings = {{F("Continuous")}, {F("One-shot")}};
  DEFINE_SETTING_SHORT_ENUM(ADC_RATE, REG2E_ADC_Control, 0x01, 6);

  enum class ADC_SAMPLE_t : uint8_t { ADC_SAMPLE_15BIT = 0, ADC_SAMPLE_14BIT = 1, ADC_SAMPLE_13BIT = 2, ADC_SAMPLE_12BIT = 3 };
  strings_vector_t ADC_SAMPLE_strings = {{F("15-bit")}, {F("14-bit")}, {F("13-bit")}, {F("12-bit")}};
  DEFINE_SETTING_SHORT_ENUM(ADC_SAMPLE, REG2E_ADC_Control, 0x03, 4);

  enum class ADC_AVG_t : uint8_t { NO_AVERAGING = 0, RUNNING_AVERAGE = 1 };
  strings_vector_t ADC_AVG_strings = {{F("No averaging")}, {F("Running average")}};
  DEFINE_SETTING_SHORT_ENUM(ADC_AVG, REG2E_ADC_Control, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(ADC_AVG_INIT, REG2E_ADC_Control, 0x01, 2);

  // ==================================
  // REG2F_ADC_Function_Disable_0
  // ==================================
  DEFINE_SETTING_SHORT_BOOL(IBUS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(IBAT_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(VBUS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(VBAT_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 4);

  DEFINE_SETTING_SHORT_BOOL(VSYS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 3);

  DEFINE_SETTING_SHORT_BOOL(TS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 2);

  DEFINE_SETTING_SHORT_BOOL(TDIE_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 1);

  // ==================================
  // REG30_ADC_Function_Disable_1
  // ==================================
  DEFINE_SETTING_SHORT_BOOL(DPLUS_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 7);

  DEFINE_SETTING_SHORT_BOOL(DMINUS_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 6);

  DEFINE_SETTING_SHORT_BOOL(VAC2_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 5);

  DEFINE_SETTING_SHORT_BOOL(VAC1_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 4);

  // ===================================
  // REG31_ADC_Function_Disable_2 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_2COMPLEMENT_READONLY(IBUS_ADC, REG31_IBUS_ADC, 0xFFFF, 0, 0, 5000, 0, 1);

  // ===================================
  // REG33_ADC_Function_Disable_3 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_2COMPLEMENT_READONLY(IBAT_ADC, REG33_IBAT_ADC, 0xFFFF, 0, 0, 8000, 0, 1);

  // ===================================
  // REG35_ADC_Function_Disable_4 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(VBUS_ADC, REG35_VBUS_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG37_ADC_Function_Disable_5 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(VAC1_ADC, REG37_VAC1_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG39_ADC_Function_Disable_6 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(VAC2_ADC, REG39_VAC2_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG3B_ADC_Function_Disable_7 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(VBAT_ADC, REG3B_VBAT_ADC, 0xFFFF, 0, 0, 20000, 0, 1);

  // ===================================
  // REG3D_ADC_Function_Disable_8 (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(VSYS_ADC, REG3D_VSYS_ADC, 0xFFFF, 0, 0, 24000, 0, 1);

  // ===================================
  // REG3F_ADC_Function_Disable_9 (wide)
  // ===================================
  DEFINE_SETTING_LONG_FLOAT_READONLY(TS_ADC, REG3F_TS_ADC, 0xFFFF, 0, 0, -99.9023, 0, 0.0976563);

  // ===================================
  // REG41_ADC_Function_Disable_A (wide)
  // ===================================
  DEFINE_SETTING_LONG_FLOAT_2COMPLEMENT_READONLY(TDIE_ADC, REG41_TDIE_ADC, 0xFFFF, 0, -40, 150, 0, 0.5);

  // ===================================
  // REG43_ADC_Function_Disable_B (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(DPLUS_ADC, REG43_DPLUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG45_ADC_Function_Disable_C (wide)
  // ===================================
  DEFINE_SETTING_LONG_INT_READONLY(DMINUS_ADC, REG45_DMINUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG47_DPDM_Driver
  // ==================================
  enum class DPLUS_DAC_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, DPLUS_DMINUS_SHORT = 7 };
  strings_vector_t DPLUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("D+/D- Short")}};
  DEFINE_SETTING_SHORT_ENUM(DPLUS_DAC, REG47_DPDM_Driver, 0x07, 5);

  // TODO_DEFINE_SETTING_SHORT_ENUM(FIXME1, REG47_DPDM_Driver, 0x07, 5,
  //                                WRAP_COMPLEX({HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6,
  //                                              DPLUS_DMINUS_SHORT = 7}),
  //                                WRAP_COMPLEX({{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("D+/D- Short")}}));

  enum class DMINUS_DAC_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, RESERVED = 7 };
  strings_vector_t DMINUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("Reserved")}};
  DEFINE_SETTING_SHORT_ENUM(DMINUS_DAC, REG47_DPDM_Driver, 0x07, 2);

  // ==================================
  // REG48_Part_Information
  // ==================================
  enum class PN_t : uint8_t {
    RESERVED_0 = 0x0,
    RESERVED_1 = 0x1,
    RESERVED_2 = 0x2,
    BQ25798 = 0x3,
    RESERVED_4 = 0x4,
    RESERVED_5 = 0x5,
    RESERVED_6 = 0x6,
    RESERVED_7 = 0x7,
  };
  strings_vector_t PN_strings = {{F("?")}, {F("?")}, {F("?")}, {F("BQ25798")}, {F("?")}, {F("?")}, {F("?")}, {F("?")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(PN, REG48_Part_Information, 0x07, 3);

  enum class DEV_REV_t : uint8_t {
    RESERVED_0 = 0x0,
    BQ25798 = 0x1,
    RESERVED_2 = 0x2,
    RESERVED_3 = 0x3,
    RESERVED_4 = 0x4,
    RESERVED_5 = 0x5,
    RESERVED_6 = 0x6,
    RESERVED_7 = 0x7,
  };
  strings_vector_t DEV_REV_strings = {{F("?")}, {F("BQ25798")}, {F("?")}, {F("?")}, {F("?")}, {F("?")}, {F("?")}, {F("?")}};
  DEFINE_SETTING_SHORT_ENUM_READONLY(DEV_REV, REG48_Part_Information, 0x07, 0);

  static constexpr size_t SETTINGS_COUNT = 191;  // Number of settings
  std::array<Setting, SETTINGS_COUNT> _settingsList = {
      VSYSMIN,
      VREG,
      ICHG,
      VINDPM,
      IINDPM,
      VBAT_LOWV,
      IPRECHG,
      REG_RST,
      STOP_WD_CHG,
      ITERM,
      CELL,
      TRECHG,
      VRECHG,
      VOTG,
      PRECHG_TMR,
      IOTG,
      TOPOFF_TMR,
      EN_TRICHG_TMR,
      EN_PRECHG_TMR,
      EN_CHG_TMR,
      CHG_TMR,
      TMR2X_EN,
      EN_AUTO_IBATDIS,
      FORCE_IBATDIS,
      EN_CHG,
      EN_ICO,
      FORCE_ICO,
      EN_HIZ,
      EN_TERM,
      EN_BACKUP,
      VBUS_BACKUP,
      VAC_OVP,
      WD_RST,
      WATCHDOG,
      FORCE_INDET,
      AUTO_INDET_EN,
      EN_12V,
      EN_9V,
      HVDCP_EN,
      SDRV_CTRL,
      SDRV_DLY,
      DIS_ACDRV,
      EN_OTG,
      PFM_OTG_DIS,
      PFM_FWD_DIS,
      WKUP_DLY,
      DIS_LDO,
      DIS_OTG_OOA,
      DIS_FWD_OOA,
      EN_ACDRV2,
      EN_ACDRV1,
      PWM_FREQ,
      DIS_STAT,
      DIS_VSYS_SHORT,
      DIS_VOTG_UVP,
      FORCE_VINDPM_DET,
      EN_IBUS_OCP,
      SFET_PRESENT,
      EN_IBAT,
      IBAT_REG,
      EN_IINDPM,
      EN_EXTILIM,
      EN_BATOC,
      VOC_PCT,
      VOC_DLY,
      VOC_RATE,
      EN_MPPT,
      TREG,
      TSHUT,
      VBUS_PD_EN,
      VAC1_PD_EN,
      VAC2_PD_EN,
      BKUP_ACFET1_ON,
      JEITA_VSET,
      JEITA_ISETH,
      JEITA_ISETC,
      TS_COOL,
      TS_WARM,
      BHOT,
      BCOLD,
      TS_IGNORE,
      ICO_ILIM,
      IINDPM_STAT,
      VINDPM_STAT,
      WD_STAT,
      PG_STAT,
      AC2_PRESENT_STAT,
      AC1_PRESENT_STAT,
      VBUS_PRESENT_STAT,
      CHG_STAT,
      VBUS_STAT,
      BC12_DONE_STAT,
      ICO_STAT,
      TREG_STAT,
      DPDM_STAT,
      VBAT_PRESENT_STAT,
      ACRB2_STAT,
      ACRB1_STAT,
      ADC_DONE_STAT,
      VSYS_STAT,
      CHG_TMR_STAT,
      TRICHG_TMR_STAT,
      PRECHG_TMR_STAT,
      VBATOTG_LOW_STAT,
      TS_COLD_STAT,
      TS_COOL_STAT,
      TS_WARM_STAT,
      TS_HOT_STAT,
      IBAT_REG_STAT,
      VBUS_OVP_STAT,
      VBAT_OVP_STAT,
      IBUS_OCP_STAT,
      IBAT_OCP_STAT,
      CONV_OCP_STAT,
      VAC2_OVP_STAT,
      VAC1_OVP_STAT,
      VSYS_SHORT_STAT,
      VSYS_OVP_STAT,
      OTG_OVP_STAT,
      OTG_UVP_STAT,
      TSHUT_STAT,
      IINDPM_FLAG,
      VINDPM_FLAG,
      WD_FLAG,
      POORSRC_FLAG,
      PG_FLAG,
      AC2_PRESENT_FLAG,
      AC1_PRESENT_FLAG,
      VBUS_PRESENT_FLAG,
      CHG_FLAG,
      ICO_FLAG,
      VBUS_FLAG,
      TREG_FLAG,
      VBAT_PRESENT_FLAG,
      BC1_2_DONE_FLAG,
      DPDM_DONE_FLAG,
      ADC_DONE_FLAG,
      VSYS_FLAG,
      CHG_TMR_FLAG,
      TRICHG_TMR_FLAG,
      PRECHG_TMR_FLAG,
      TOPOFF_TMR_FLAG,
      VBATOTG_LOW_FLAG,
      TS_COLD_FLAG,
      TS_COOL_FLAG,
      TS_WARM_FLAG,
      TS_HOT_FLAG,
      IBAT_REG_FLAG,
      VBUS_OVP_FLAG,
      VBAT_OVP_FLAG,
      IBUS_OCP_FLAG,
      IBAT_OCP_FLAG,
      CONV_OCP_FLAG,
      VAC2_OVP_FLAG,
      VAC1_OVP_FLAG,
      VSYS_SHORT_FLAG,
      VSYS_OVP_FLAG,
      OTG_OVP_FLAG,
      OTG_UVP_FLAG,
      TSHUT_FLAG,
      ADC_EN,
      ADC_RATE,
      ADC_SAMPLE,
      ADC_AVG,
      ADC_AVG_INIT,
      IBUS_ADC_DIS,
      IBAT_ADC_DIS,
      VBUS_ADC_DIS,
      VBAT_ADC_DIS,
      VSYS_ADC_DIS,
      TS_ADC_DIS,
      TDIE_ADC_DIS,
      DPLUS_ADC_DIS,
      DMINUS_ADC_DIS,
      VAC2_ADC_DIS,
      VAC1_ADC_DIS,
      IBUS_ADC,
      IBAT_ADC,
      VBUS_ADC,
      VAC1_ADC,
      VAC2_ADC,
      VBAT_ADC,
      VSYS_ADC,
      TS_ADC,
      TDIE_ADC,
      DPLUS_ADC,
      DMINUS_ADC,
      DPLUS_DAC,
      DMINUS_DAC,
      PN,
      DEV_REV
      //

  };
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

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg, uint16_t value, int bitMask = 0xFFFF, int bitShift = 0);

  bool setAndWriteRaw(const BQ25798Core::Setting& setting, uint16_t value);
  bool setAndWriteInt(const BQ25798Core::Setting& setting, int value);
  bool setAndWriteBool(const BQ25798Core::Setting& setting, bool value);
  bool setAndWriteFloat(const BQ25798Core::Setting& setting, float value);

  template <typename T>
  bool setAndWriteEnum(const Setting& setting, T value) {
    // the same as int, but with enum value
    return setAndWriteInt(setting, static_cast<int>(value));
  };

  bool writeReg8ToI2C(int reg) { return false; }   // mock only
  bool writeReg16ToI2C(int reg) { return false; }  // mock only

 protected:
  std::array<uint8_t, PHYSICAL_REGISTERS_COUNT> _physicalReg8Values;  // Array to hold 8-bit register values
  void _clearRegs();
  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);
  void _setError(int errorCode);

 private:
  int _errorCode = ERROR_NONE;
};

#endif