#ifndef BQ25798_H
#define BQ25798_H

#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"
#include "debug.h"
#include "registers.h"

#define DEFAULT_I2C_ADDRESS 0x6B

class BQ25798 {
 public:
  // =================================
  // Registers
  // =================================
  typedef int16_t regaddr_t;

  enum class regsize_t : uint8_t {
    SHORT = 8,
    LONG = 16,
  };

  struct RegisterDefinition {
    regaddr_t address;  // Register address
    regsize_t size;     // 8bit or 16bit
    const char* name;   // Register name

    RegisterDefinition(regaddr_t address, regsize_t size, const char* name) : address(address), size(size), name(name) {};

    static RegisterDefinition invalid() { return RegisterDefinition{-1, regsize_t::SHORT, F("INVALID")}; };
    bool isValid() const { return address != -1; };
  };

  static constexpr size_t PHYSICAL_REGISTERS_COUNT = 1 + REG48_Part_Information;  // Number of physical registers (0x00 to 0x48)
#define DEFINE_REGISTER(propname, size) \
  RegisterDefinition { propname, regsize_t::size, F(#propname) }
  std::array<RegisterDefinition, PHYSICAL_REGISTERS_COUNT> _registerDefinitions = {
      //  There must be all 8-bit registers in the list (no skipping for 16-bit registers, no skipping for unused registers)!
      DEFINE_REGISTER(REG00_Minimal_System_Voltage, SHORT),
      DEFINE_REGISTER(REG01_Charge_Voltage_Limit, LONG),
      DEFINE_REGISTER(REG02_dummy_fill, SHORT),
      DEFINE_REGISTER(REG03_Charge_Current_Limit, LONG),
      DEFINE_REGISTER(REG04_dummy_fill, SHORT),
      DEFINE_REGISTER(REG05_Input_Voltage_Limit, SHORT),
      DEFINE_REGISTER(REG06_Input_Current_Limit, LONG),
      DEFINE_REGISTER(REG07_dummy_fill, SHORT),
      DEFINE_REGISTER(REG08_Precharge_Control, SHORT),
      DEFINE_REGISTER(REG09_Termination_Control, SHORT),
      DEFINE_REGISTER(REG0A_Recharge_Control, SHORT),
      DEFINE_REGISTER(REG0B_VOTG_regulation, LONG),
      DEFINE_REGISTER(REG0C_dummy_fill, SHORT),
      DEFINE_REGISTER(REG0D_IOTG_regulation, SHORT),
      DEFINE_REGISTER(REG0E_Timer_Control, SHORT),
      DEFINE_REGISTER(REG0F_Charger_Control_0, SHORT),
      DEFINE_REGISTER(REG10_Charger_Control_1, SHORT),
      DEFINE_REGISTER(REG11_Charger_Control_2, SHORT),
      DEFINE_REGISTER(REG12_Charger_Control_3, SHORT),
      DEFINE_REGISTER(REG13_Charger_Control_4, SHORT),
      DEFINE_REGISTER(REG14_Charger_Control_5, SHORT),
      DEFINE_REGISTER(REG15_MPPT_Control, SHORT),
      DEFINE_REGISTER(REG16_Temperature_Control, SHORT),
      DEFINE_REGISTER(REG17_NTC_Control_0, SHORT),
      DEFINE_REGISTER(REG18_NTC_Control_1, SHORT),
      DEFINE_REGISTER(REG19_ICO_Current_Limit, LONG),
      DEFINE_REGISTER(REG1A_dummy_fill, SHORT),
      DEFINE_REGISTER(REG1B_Charger_Status_0, SHORT),
      DEFINE_REGISTER(REG1C_Charger_Status_1, SHORT),
      DEFINE_REGISTER(REG1D_Charger_Status_2, SHORT),
      DEFINE_REGISTER(REG1E_Charger_Status_3, SHORT),
      DEFINE_REGISTER(REG1F_Charger_Status_4, SHORT),
      DEFINE_REGISTER(REG20_FAULT_Status_0, SHORT),
      DEFINE_REGISTER(REG21_FAULT_Status_1, SHORT),
      DEFINE_REGISTER(REG22_Charger_Flag_0, SHORT),
      DEFINE_REGISTER(REG23_Charger_Flag_1, SHORT),
      DEFINE_REGISTER(REG24_Charger_Flag_2, SHORT),
      DEFINE_REGISTER(REG25_Charger_Flag_3, SHORT),
      DEFINE_REGISTER(REG26_FAULT_Flag_0, SHORT),
      DEFINE_REGISTER(REG27_FAULT_Flag_1, SHORT),
      DEFINE_REGISTER(REG28_Charger_Mask_0, SHORT),
      DEFINE_REGISTER(REG29_Charger_Mask_1, SHORT),
      DEFINE_REGISTER(REG2A_Charger_Mask_2, SHORT),
      DEFINE_REGISTER(REG2B_Charger_Mask_3, SHORT),
      DEFINE_REGISTER(REG2C_FAULT_Mask_0, SHORT),
      DEFINE_REGISTER(REG2D_FAULT_Mask_1, SHORT),
      DEFINE_REGISTER(REG2E_ADC_Control, SHORT),
      DEFINE_REGISTER(REG2F_ADC_Function_Disable_0, SHORT),
      DEFINE_REGISTER(REG30_ADC_Function_Disable_1, SHORT),
      DEFINE_REGISTER(REG31_IBUS_ADC, LONG),
      DEFINE_REGISTER(REG32_dummy_fill, SHORT),
      DEFINE_REGISTER(REG33_IBAT_ADC, LONG),
      DEFINE_REGISTER(REG34_dummy_fill, SHORT),
      DEFINE_REGISTER(REG35_VBUS_ADC, LONG),
      DEFINE_REGISTER(REG36_dummy_fill, SHORT),
      DEFINE_REGISTER(REG37_VAC1_ADC, LONG),
      DEFINE_REGISTER(REG38_dummy_fill, SHORT),
      DEFINE_REGISTER(REG39_VAC2_ADC, LONG),
      DEFINE_REGISTER(REG3A_dummy_fill, SHORT),
      DEFINE_REGISTER(REG3B_VBAT_ADC, LONG),
      DEFINE_REGISTER(REG3C_dummy_fill, SHORT),
      DEFINE_REGISTER(REG3D_VSYS_ADC, LONG),
      DEFINE_REGISTER(REG3E_dummy_fill, SHORT),
      DEFINE_REGISTER(REG3F_TS_ADC, LONG),
      DEFINE_REGISTER(REG40_dummy_fill, SHORT),
      DEFINE_REGISTER(REG41_TDIE_ADC, LONG),
      DEFINE_REGISTER(REG42_dummy_fill, SHORT),
      DEFINE_REGISTER(REG43_DPLUS_ADC, LONG),
      DEFINE_REGISTER(REG44_dummy_fill, SHORT),
      DEFINE_REGISTER(REG45_DMINUS_ADC, LONG),
      DEFINE_REGISTER(REG46_dummy_fill, SHORT),
      DEFINE_REGISTER(REG47_DPDM_Driver, SHORT),
      DEFINE_REGISTER(REG48_Part_Information, SHORT)
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
    regaddr_t reg;         // Register address
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

    Setting(regaddr_t reg, const char* name, settings_type_t type, uint16_t mask, uint8_t shift,
            // optional parameters:
            float range_low = 0, float range_high = 0, float fixed_offset = 0, float bit_step_size = 0, settings_flags_t flags = settings_flags_t::NONE,
            strings_vector_t strings_vector = {})
        : reg(reg),
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
      return Setting{RegisterDefinition::invalid().address, F("INVALID"), settings_type_t::INT, 0, 0, 0, 0, 0, 0, settings_flags_t::NONE, {}};
    };
    bool isValid() const { return mask != 0; };
  };

#define DEFINE_SETTING3(setting, type, regaddr, mask, shift) Setting setting = {regaddr, #setting, settings_type_t::type, mask, shift}
#define DEFINE_ENUM(setting, type, regaddr, mask, shift) \
  Setting setting = {regaddr, #setting, settings_type_t::type, mask, shift, 0, 0, 0, 0, settings_flags_t::NONE, setting##_strings}
#define DEFINE_SETTING7(setting, type, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  Setting setting = {regaddr, #setting, settings_type_t::type, mask, shift, range_low, range_high, fixed_offset, bit_step_size}
#define DEFINE_SETTING8(setting, type, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  Setting setting = {regaddr, #setting, settings_type_t::type, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags}

  // ===================================
  // REG00_Minimal_System_Voltage
  // ===================================
  // DEFINE_SETTING3(VSYSMIN, REG00_Minimal_System_Voltage, 0x3F,  0, 2500, 16000, 2500, 250);
  DEFINE_SETTING7(VSYSMIN, INT, REG00_Minimal_System_Voltage, 0x3F, 0, 2500, 16000, 2500, 250);

  // ===================================
  // REG01_Charge_Voltage_Limit (wide)
  // ===================================
  DEFINE_SETTING7(VREG, INT, REG01_Charge_Voltage_Limit, 0x7FF, 0, 3000, 18800, 0, 10);

  // ===================================
  // REG03_Charge_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING7(ICHG, INT, REG03_Charge_Current_Limit, 0x1FF, 0, 50, 5000, 0, 10);

  // ===================================
  // REG05_Input_Voltage_Limit
  // ===================================
  DEFINE_SETTING7(VINDPM, INT, REG05_Input_Voltage_Limit, 0xFF, 0, 3600, 22000, 0, 100);

  // ===================================
  // REG06_Input_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING7(IINDPM, INT, REG06_Input_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ===================================
  // REG08_Precharge_Control
  // ===================================
  enum class vbat_lowv_t : uint8_t { PCT_15 = 0, PCT_62 = 1, PCT_67 = 2, PCT_71 = 3 };
  strings_vector_t VBAT_LOWV_strings = {{F("15%(VREG)")}, {F("62.2%(VREG)")}, {F("66.7%(VREG)")}, {F("71.4%(VREG)")}};
  DEFINE_ENUM(VBAT_LOWV, ENUM, REG08_Precharge_Control, 0x3, 6);

  DEFINE_SETTING7(IPRECHG, INT, REG08_Precharge_Control, 0x3F, 0, 40, 2000, 0, 40);

  // ===================================
  // REG09_Termination_Control
  // ===================================
  DEFINE_SETTING3(REG_RST, BOOL, REG09_Termination_Control, 0x01, 6);

  DEFINE_SETTING3(STOP_WD_CHG, BOOL, REG09_Termination_Control, 0x01, 5);

  DEFINE_SETTING7(ITERM, INT, REG09_Termination_Control, 0x1F, 0, 40, 1000, 0, 40);

  // ===================================
  // REG0A_Recharge_Control
  // ===================================
  enum class cell_t : uint8_t { CELL_1S = 0, CELL_2S = 1, CELL_3S = 2, CELL_4S = 3 };
  strings_vector_t CELL_strings = {{F("1S")}, {F("2S")}, {F("3S")}, {F("4S")}};
  DEFINE_ENUM(CELL, ENUM, REG0A_Recharge_Control, 0x03, 6);

  enum class trechg_t : uint8_t { TRECHG_64MS = 0, TRECHG_256MS = 1, TRECHG_1024MS = 2, TRECHG_2048MS = 3 };
  strings_vector_t TRECHG_strings = {{F("64 ms")}, {F("256 ms")}, {F("1024 ms")}, {F("2048 ms")}};
  DEFINE_ENUM(TRECHG, ENUM, REG0A_Recharge_Control, 0x03, 4);

  DEFINE_SETTING7(VRECHG, INT, REG0A_Recharge_Control, 0x0F, 0, 50, 800, 50, 50);

  // ===================================
  // REG0B_VOTG_regulation (wide)
  // ===================================
  DEFINE_SETTING7(VOTG, INT, REG0B_VOTG_regulation, 0x7FF, 0, 2800, 22000, 2800, 10);

  // ===================================
  // REG0D_IOTG_regulation
  // ===================================
  enum class precgh_tmr_t {
    PRECHG_TMR_2HRS = 0,
    PRECHG_TMR_30MIN = 1,
  };
  strings_vector_t PRECHG_TMR_strings = {{F("2 hours")}, {F("30 minutes")}};
  DEFINE_ENUM(PRECHG_TMR, ENUM, REG0D_IOTG_regulation, 0x01, 7);

  DEFINE_SETTING7(IOTG, INT, REG0D_IOTG_regulation, 0x7F, 0, 160, 3360, 0, 40);

  // ===================================
  // REG0E_Timer_Control
  // ===================================
  enum class tophff_tmr_t : uint8_t { TOPOFF_TMR_DISABLED = 0, TOPOFF_TMR_15MIN = 1, TOPOFF_TMR_30MIN = 2, TOPOFF_TMR_45MIN = 3 };
  strings_vector_t TOPOFF_TMR_strings = {{F("Disabled")}, {F("15 minutes")}, {F("30 minutes")}, {F("45 minutes")}};
  DEFINE_ENUM(TOPOFF_TMR, ENUM, REG0E_Timer_Control, 0x03, 6);

  DEFINE_SETTING3(EN_TRICHG_TMR, BOOL, REG0E_Timer_Control, 0x01, 5);

  DEFINE_SETTING3(EN_PRECHG_TMR, BOOL, REG0E_Timer_Control, 0x01, 4);

  DEFINE_SETTING3(EN_CHG_TMR, BOOL, REG0E_Timer_Control, 0x01, 3);

  enum class chg_tmr_t : uint8_t { CHG_TMR_5HRS = 0, CHG_TMR_8HRS = 1, CHG_TMR_12HRS = 2, CHG_TMR_24HRS = 3 };
  strings_vector_t CHG_TMR_strings = {{F("5 hours")}, {F("8 hours")}, {F("12 hours")}, {F("24 hours")}};
  DEFINE_ENUM(CHG_TMR, ENUM, REG0E_Timer_Control, 0x03, 1);

  DEFINE_SETTING3(TMR2X_EN, BOOL, REG0E_Timer_Control, 0x01, 0);

  // ===================================
  // REG0F_Charger_Control_0
  // ===================================
  DEFINE_SETTING3(EN_AUTO_IBATDIS, BOOL, REG0F_Charger_Control_0, 0x01, 7);

  DEFINE_SETTING3(FORCE_IBATDIS, BOOL, REG0F_Charger_Control_0, 0x01, 6);

  DEFINE_SETTING3(EN_CHG, BOOL, REG0F_Charger_Control_0, 0x01, 5);

  DEFINE_SETTING3(EN_ICO, BOOL, REG0F_Charger_Control_0, 0x01, 4);

  DEFINE_SETTING3(FORCE_ICO, BOOL, REG0F_Charger_Control_0, 0x01, 3);

  DEFINE_SETTING3(EN_HIZ, BOOL, REG0F_Charger_Control_0, 0x01, 2);

  DEFINE_SETTING3(EN_TERM, BOOL, REG0F_Charger_Control_0, 0x01, 1);

  DEFINE_SETTING3(EN_BACKUP, BOOL, REG0F_Charger_Control_0, 0x01, 0);

  // ===================================
  // REG10_Charger_Control_1
  // ===================================
  enum class vbus_backup_t : uint8_t { PCT_VBUS_BACKUP_40 = 0, PCT_VBUS_BACKUP_60 = 1, PCT_VBUS_BACKUP_80 = 2, PCT_VBUS_BACKUP_100 = 3 };
  strings_vector_t VBUS_BACKUP_strings = {{F("<40%(VINDPM)")}, {F("<60%(VINDPM)")}, {F("<80%(VINDPM)")}, {F("<100%(VINDPM)")}};
  DEFINE_ENUM(VBUS_BACKUP, ENUM, REG10_Charger_Control_1, 0x03, 6);

  enum class vac_ovp_t : uint8_t { VAC_OVP_26V = 0, VAC_OVP_22V = 1, VAC_OVP_12V = 2, VAC_OVP_7V = 3 };
  strings_vector_t VAC_OVP_strings = {{F(">26V")}, {F(">22V")}, {F(">12V")}, {F(">7V")}};
  DEFINE_ENUM(VAC_OVP, ENUM, REG10_Charger_Control_1, 0x03, 4);

  DEFINE_SETTING3(WD_RST, BOOL, REG10_Charger_Control_1, 0x01, 3);

  enum class watchdog_t : uint8_t {
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
  DEFINE_ENUM(WATCHDOG, ENUM, REG10_Charger_Control_1, 0x07, 0);

  // ===================================
  // REG11_Charger_Control_2
  // ===================================
  DEFINE_SETTING3(FORCE_INDET, BOOL, REG11_Charger_Control_2, 0x01, 7);

  DEFINE_SETTING3(AUTO_INDET_EN, BOOL, REG11_Charger_Control_2, 0x01, 6);

  DEFINE_SETTING3(EN_12V, BOOL, REG11_Charger_Control_2, 0x01, 5);

  DEFINE_SETTING3(EN_9V, BOOL, REG11_Charger_Control_2, 0x01, 4);

  DEFINE_SETTING3(HVDCP_EN, BOOL, REG11_Charger_Control_2, 0x01, 3);

  enum class sdrv_ctrl_t : uint8_t { SDRV_CTRL_IDLE = 0, SDRV_CTRL_SHUTDOWN = 1, SDRV_CTRL_SHIP = 2, SDRV_CTRL_SYS_PWR_RST = 3 };
  strings_vector_t SDRV_CTRL_strings = {{F("Idle")}, {F("Shutdown")}, {F("Ship")}, {F("System Power Reset")}};
  DEFINE_ENUM(SDRV_CTRL, ENUM, REG11_Charger_Control_2, 0x03, 1);

  enum class sdrv_dly_t : uint8_t { SDRV_DLY_10S = 0, SDRV_DLY_0S = 1 };
  strings_vector_t SDRV_DLY_strings = {{F("10s")}, {F("0s")}};
  DEFINE_ENUM(SDRV_DLY, ENUM, REG11_Charger_Control_2, 0x01, 0);

  // ===================================
  // REG12_Charger_Control_3
  // ===================================
  DEFINE_SETTING3(DIS_ACDRV, BOOL, REG12_Charger_Control_3, 0x01, 7);

  DEFINE_SETTING3(EN_OTG, BOOL, REG12_Charger_Control_3, 0x01, 6);

  DEFINE_SETTING3(PFM_OTG_DIS, BOOL, REG12_Charger_Control_3, 0x01, 5);

  DEFINE_SETTING3(PFM_FWD_DIS, BOOL, REG12_Charger_Control_3, 0x01, 4);

  enum class wkup_dly_t : uint8_t { WKUP_DLY_1S = 0, WKUP_DLY_15MS = 1 };
  strings_vector_t WKUP_DLY_strings = {{F("1s")}, {F("15ms")}};
  DEFINE_ENUM(WKUP_DLY, ENUM, REG12_Charger_Control_3, 0x01, 3);

  DEFINE_SETTING3(DIS_LDO, BOOL, REG12_Charger_Control_3, 0x01, 2);

  DEFINE_SETTING3(DIS_OTG_OOA, BOOL, REG12_Charger_Control_3, 0x01, 1);

  DEFINE_SETTING3(DIS_FWD_OOA, BOOL, REG12_Charger_Control_3, 0x01, 0);

  // ===================================
  // REG13_Charger_Control_4
  // ===================================
  DEFINE_SETTING3(EN_ACDRV2, BOOL, REG13_Charger_Control_4, 0x01, 7);

  DEFINE_SETTING3(EN_ACDRV1, BOOL, REG13_Charger_Control_4, 0x01, 6);

  enum class pwm_freq_t : uint8_t { PWM_FREQ_1_5MHZ = 0, PWM_FREQ_750KHZ = 1 };
  strings_vector_t PWM_FREQ_strings = {{F("1.5 MHz")}, {F("750 kHz")}};
  DEFINE_ENUM(PWM_FREQ, ENUM, REG13_Charger_Control_4, 0x01, 5);

  DEFINE_SETTING3(DIS_STAT, BOOL, REG13_Charger_Control_4, 0x01, 4);

  DEFINE_SETTING3(DIS_VSYS_SHORT, BOOL, REG13_Charger_Control_4, 0x01, 3);

  DEFINE_SETTING3(DIS_VOTG_UVP, BOOL, REG13_Charger_Control_4, 0x01, 2);

  DEFINE_SETTING3(FORCE_VINDPM_DET, BOOL, REG13_Charger_Control_4, 0x01, 1);

  DEFINE_SETTING3(EN_IBUS_OCP, BOOL, REG13_Charger_Control_4, 0x01, 0);

  // ===================================
  // REG14_Charger_Control_5
  // ===================================
  DEFINE_SETTING3(SFET_PRESENT, BOOL, REG14_Charger_Control_5, 0x01, 7);

  DEFINE_SETTING3(EN_IBAT, BOOL, REG14_Charger_Control_5, 0x01, 5);

  enum class ibat_reg_t : uint8_t { IBAT_REG_3A = 0, IBAT_REG_4A = 1, IBAT_REG_5A = 2, IBAT_REG_DISABLE = 3 };
  strings_vector_t IBAT_REG_strings = {{F("3A")}, {F("4A")}, {F("5A")}, {F("Disabled")}};
  DEFINE_ENUM(IBAT_REG, ENUM, REG14_Charger_Control_5, 0x03, 3);

  DEFINE_SETTING3(EN_IINDPM, BOOL, REG14_Charger_Control_5, 0x01, 2);

  DEFINE_SETTING3(EN_EXTILIM, BOOL, REG14_Charger_Control_5, 0x01, 1);

  DEFINE_SETTING3(EN_BATOC, BOOL, REG14_Charger_Control_5, 0x01, 0);

  // ===================================
  // REG15_MPPT_Control
  // ===================================
  enum class voc_pct_t : uint8_t {
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
  DEFINE_ENUM(VOC_PCT, ENUM, REG15_MPPT_Control, 0x07, 5);

  enum class voc_dly_t : uint8_t { VOC_DLY_50MS = 0, VOC_DLY_300MS = 1, VOC_DLY_2S = 2, VOC_DLY_5S = 3 };
  strings_vector_t VOC_DLY_strings = {{F("50ms")}, {F("300ms")}, {F("2s")}, {F("5s")}};
  DEFINE_ENUM(VOC_DLY, ENUM, REG15_MPPT_Control, 0x03, 4);

  enum class voc_rate_t : uint8_t { VOC_RATE_30S = 0, VOC_RATE_2MIN = 1, VOC_RATE_10MIN = 2, VOC_RATE_30MIN = 3 };
  strings_vector_t VOC_RATE_strings = {{F("30s")}, {F("2min")}, {F("10min")}, {F("30min")}};
  DEFINE_ENUM(VOC_RATE, ENUM, REG15_MPPT_Control, 0x03, 2);

  DEFINE_SETTING3(EN_MPPT, BOOL, REG15_MPPT_Control, 0x01, 0);

  // ===================================
  // REG16_Temperature_Control
  // ===================================
  enum class treg_t : uint8_t { TREG_60 = 0, TREG_80 = 1, TREG_100 = 2, TREG_120 = 3 };
  strings_vector_t TREG_strings = {{F("60°C")}, {F("80°C")}, {F("100°C")}, {F("120°C")}};
  DEFINE_ENUM(TREG, ENUM, REG16_Temperature_Control, 0x03, 6);

  enum class tshut_t : uint8_t { TSHUT_150 = 0, TSHUT_130 = 1, TSHUT_120 = 2, TSHUT_85 = 3 };
  strings_vector_t TSHUT_strings = {{F("150°C")}, {F("130°C")}, {F("120°C")}, {F("85°C")}};
  DEFINE_ENUM(TSHUT, ENUM, REG16_Temperature_Control, 0x03, 4);

  DEFINE_SETTING3(VBUS_PD_EN, BOOL, REG16_Temperature_Control, 0x01, 3);

  DEFINE_SETTING3(VAC1_PD_EN, BOOL, REG16_Temperature_Control, 0x01, 2);

  DEFINE_SETTING3(VAC2_PD_EN, BOOL, REG16_Temperature_Control, 0x01, 1);

  enum class bkup_acfet1_on_t : uint8_t { IDLE = 0, TURN_ON = 1 };
  strings_vector_t BKUP_ACFET1_ON_strings = {{F("Idle")}, {F("Turn on ACFET1 in backup mode")}};
  DEFINE_ENUM(BKUP_ACFET1_ON, ENUM, REG16_Temperature_Control, 0x01, 0);

  // ==================================
  // REG17_NTC_Control_0
  // ==================================
  enum class jeita_vset_t : uint8_t {
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
  DEFINE_ENUM(JEITA_VSET, ENUM, REG17_NTC_Control_0, 0x07, 5);

  enum class jeita_iseth_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETH_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};
  DEFINE_ENUM(JEITA_ISETH, ENUM, REG17_NTC_Control_0, 0x03, 3);

  enum class jeita_isetc_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETC_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG (default)")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};
  DEFINE_ENUM(JEITA_ISETC, ENUM, REG17_NTC_Control_0, 0x03, 1);

  // ==================================
  // REG18_NTC_Control_1
  // ==================================
  enum class ts_cool_t : uint8_t { TS_5 = 0, TS_10 = 1, TS_15 = 2, TS_20 = 3 };
  strings_vector_t TS_COOL_strings = {{F("5°C")}, {F("10°C (default)")}, {F("15°C")}, {F("20°C")}};
  DEFINE_ENUM(TS_COOL, ENUM, REG18_NTC_Control_1, 0x03, 6);

  enum class ts_warm_t : uint8_t { TS_40 = 0, TS_45 = 1, TS_50 = 2, TS_55 = 3 };
  strings_vector_t TS_WARM_strings = {{F("40°C")}, {F("45°C (default)")}, {F("50°C")}, {F("55°C")}};
  DEFINE_ENUM(TS_WARM, ENUM, REG18_NTC_Control_1, 0x03, 4);

  enum class bhot_t : uint8_t { TS_55 = 0, TS_60 = 1, TS_65 = 2, DISABLE = 3 };
  strings_vector_t BHOT_strings = {{F("55°C")}, {F("60°C (default)")}, {F("65°C")}, {F("Disabled")}};
  DEFINE_ENUM(BHOT, ENUM, REG18_NTC_Control_1, 0x03, 2);

  enum class bcold_t : uint8_t { MINUS_10 = 0, MINUS_20 = 1 };
  strings_vector_t BCOLD_strings = {{F("-10°C (default)")}, {F("-20°C")}};
  DEFINE_ENUM(BCOLD, ENUM, REG18_NTC_Control_1, 0x01, 1);

  DEFINE_SETTING3(TS_IGNORE, BOOL, REG18_NTC_Control_1, 0x01, 0);

  // ==================================
  // REG19_ICO_Current_Limit (wide)
  // ==================================
  DEFINE_SETTING7(ICO_ILIM, INT, REG19_ICO_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ==================================
  // REG1B_Charger_Status_0
  // ==================================
  enum class iindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t IINDPM_STAT_strings = {{F("Normal")}, {F("In IINDPM regulation or IOTG regulation")}};
  DEFINE_ENUM(IINDPM_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 7);

  enum class vindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t VINDPM_STAT_strings = {{F("Normal")}, {F("In VINDPM regulation or VOTG regulation")}};
  DEFINE_ENUM(VINDPM_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 6);

  enum class wd_stat_t : uint8_t { NORMAL = 0, EXPIRED = 1 };
  strings_vector_t WD_STAT_strings = {{F("Normal")}, {F("Watchdog timer expired")}};
  DEFINE_ENUM(WD_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 5);

  enum class pg_stat_t : uint8_t { BAD = 0, GOOD = 1 };
  strings_vector_t PG_STAT_strings = {{F("Not in power good status")}, {F("Power good")}};
  DEFINE_ENUM(PG_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 3);

  enum class ac2_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC2_PRESENT_STAT_strings = {{F("VAC2 NOT present")}, {F("VAC2 present (above present threshold)")}};
  DEFINE_ENUM(AC2_PRESENT_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 2);

  enum class ac1_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC1_PRESENT_STAT_strings = {{F("VAC1 NOT present")}, {F("VAC1 present (above present threshold)")}};
  DEFINE_ENUM(AC1_PRESENT_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 1);

  enum class vbus_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBUS_PRESENT_STAT_strings = {{F("VBUS NOT present")}, {F("VBUS present (above present threshold)")}};
  DEFINE_ENUM(VBUS_PRESENT_STAT, ENUM, REG1B_Charger_Status_0, 0x01, 0);

  // ==================================
  // REG1C_Charger_Status_1
  // ==================================
  enum class chg_stat_t : uint8_t {
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
  DEFINE_ENUM(CHG_STAT, ENUM, REG1C_Charger_Status_1, 0x7, 5);

  enum class vbus_stat_t : uint8_t {
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
  DEFINE_ENUM(VBUS_STAT, ENUM, REG1C_Charger_Status_1, 0x0F, 1);

  DEFINE_SETTING3(BC12_DONE_STAT, BOOL, REG1C_Charger_Status_1, 0x01, 0);

  // ==================================
  // REG1D_Charger_Status_2
  // ==================================
  enum class ico_stat_t : uint8_t { ICO_DISABLED = 0, ICO_IN_PROGRESS = 1, ICO_MAX_CURRENT_DETECTED = 2, ICO_RESERVED = 3 };
  strings_vector_t ICO_STAT_strings = {{F("ICO disabled")}, {F("ICO optimization in progress")}, {F("Maximum input current detected")}, {F("Reserved")}};
  DEFINE_ENUM(ICO_STAT, ENUM, REG1D_Charger_Status_2, 0x03, 6);

  enum class treg_stat_t : uint8_t { NORMAL = 0, THERMAL_REGULATION = 1 };
  strings_vector_t TREG_STAT_strings = {{F("Normal")}, {F("Device in thermal regulation")}};
  DEFINE_ENUM(TREG_STAT, ENUM, REG1D_Charger_Status_2, 0x01, 5);

  enum class dpdm_stat_t : uint8_t { NOT_STARTED = 0, IN_PROGRESS = 1 };
  strings_vector_t DPDM_STAT_strings = {{F("D+/D- detection NOT started yet or done")}, {F("D+/D- detection in progress")}};
  DEFINE_ENUM(DPDM_STAT, ENUM, REG1D_Charger_Status_2, 0x01, 4);

  enum class vbat_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBAT_PRESENT_STAT_strings = {{F("VBAT NOT present")}, {F("VBAT present")}};
  DEFINE_ENUM(VBAT_PRESENT_STAT, ENUM, REG1D_Charger_Status_2, 0x01, 0);

  // ==================================
  // REG1E_Charger_Status_3
  // ==================================
  DEFINE_SETTING3(ACRB2_STAT, BOOL, REG1E_Charger_Status_3, 0x01, 7);

  DEFINE_SETTING3(ACRB1_STAT, BOOL, REG1E_Charger_Status_3, 0x01, 6);

  DEFINE_SETTING3(ADC_DONE_STAT, BOOL, REG1E_Charger_Status_3, 0x01, 5);

  enum class vsys_stat_t : uint8_t { NOT_IN_VSYSMIN_REGULATION = 0, IN_VSYSMIN_REGULATION = 1 };
  strings_vector_t VSYS_STAT_strings = {{F("Not in VSYSMIN regulation (VBAT > VSYSMIN)")}, {F("In VSYSMIN regulation (VBAT < VSYSMIN)")}};
  DEFINE_ENUM(VSYS_STAT, ENUM, REG1E_Charger_Status_3, 0x01, 4);

  enum class chg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t CHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_ENUM(CHG_TMR_STAT, ENUM, REG1E_Charger_Status_3, 0x01, 3);

  enum class trichg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t TRICHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_ENUM(TRICHG_TMR_STAT, ENUM, REG1E_Charger_Status_3, 0x01, 2);

  enum class prechg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t PRECHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};
  DEFINE_ENUM(PRECHG_TMR_STAT, ENUM, REG1E_Charger_Status_3, 0x01, 1);

  // ==================================
  // REG1F_Charger_Status_4
  // ==================================
  enum class vbatotg_low_stat_t : uint8_t { VBATOTG_LOW = 0, VBATOTG_OK = 1 };
  strings_vector_t VBATOTG_LOW_STAT_strings = {{F("VBAT is too low to enable OTG mode")}, {F("VBAT is high enough to enable OTG operation")}};
  DEFINE_ENUM(VBATOTG_LOW_STAT, ENUM, REG1F_Charger_Status_4, 0x01, 4);

  enum class ts_cold_stat_t : uint8_t { NOT_COLD = 0, COLD = 1 };
  strings_vector_t TS_COLD_STAT_strings = {{F("TS NOT in cold range")}, {F("TS in cold range")}};
  DEFINE_ENUM(TS_COLD_STAT, ENUM, REG1F_Charger_Status_4, 0x01, 3);

  enum class ts_cool_stat_t : uint8_t { NOT_COOL = 0, COOL = 1 };
  strings_vector_t TS_COOL_STAT_strings = {{F("TS NOT in cool range")}, {F("TS in cool range")}};
  DEFINE_ENUM(TS_COOL_STAT, ENUM, REG1F_Charger_Status_4, 0x01, 2);

  enum class ts_warm_stat_t : uint8_t { NOT_WARM = 0, WARM = 1 };
  strings_vector_t TS_WARM_STAT_strings = {{F("TS NOT in warm range")}, {F("TS in warm range")}};
  DEFINE_ENUM(TS_WARM_STAT, ENUM, REG1F_Charger_Status_4, 0x01, 1);

  enum class ts_hot_stat_t : uint8_t { NOT_HOT = 0, HOT = 1 };
  strings_vector_t TS_HOT_STAT_strings = {{F("TS NOT in hot range")}, {F("TS in hot range")}};
  DEFINE_ENUM(TS_HOT_STAT, ENUM, REG1F_Charger_Status_4, 0x01, 0);

  // ==================================
  // REG20_FAULT_Status_0
  // ==================================
  DEFINE_SETTING3(IBAT_REG_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 7);

  DEFINE_SETTING3(VBUS_OVP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 6);

  DEFINE_SETTING3(VBAT_OVP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 5);

  DEFINE_SETTING3(IBUS_OCP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 4);

  DEFINE_SETTING3(IBAT_OCP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 3);

  DEFINE_SETTING3(CONV_OCP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 2);

  DEFINE_SETTING3(VAC2_OVP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 1);

  DEFINE_SETTING3(VAC1_OVP_STAT, BOOL, REG20_FAULT_Status_0, 0x01, 0);

  // ==================================
  // REG21_FAULT_Status_1
  // ==================================
  DEFINE_SETTING3(VSYS_SHORT_STAT, BOOL, REG21_FAULT_Status_1, 0x01, 7);

  DEFINE_SETTING3(VSYS_OVP_STAT, BOOL, REG21_FAULT_Status_1, 0x01, 6);

  DEFINE_SETTING3(OTG_OVP_STAT, BOOL, REG21_FAULT_Status_1, 0x01, 5);

  DEFINE_SETTING3(OTG_UVP_STAT, BOOL, REG21_FAULT_Status_1, 0x01, 4);

  DEFINE_SETTING3(TSHUT_STAT, BOOL, REG21_FAULT_Status_1, 0x01, 2);

  // ==================================
  // REG22_Charger_Flag_0
  // ==================================
  DEFINE_SETTING3(IINDPM_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 7);

  DEFINE_SETTING3(VINDPM_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 6);

  DEFINE_SETTING3(WD_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 5);

  DEFINE_SETTING3(POORSRC_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 4);

  DEFINE_SETTING3(PG_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 3);

  DEFINE_SETTING3(AC2_PRESENT_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 2);

  DEFINE_SETTING3(AC1_PRESENT_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 1);

  DEFINE_SETTING3(VBUS_PRESENT_FLAG, BOOL, REG22_Charger_Flag_0, 0x01, 0);

  // ===================================
  // REG23_Charger_Flag_1
  // ===================================
  DEFINE_SETTING3(CHG_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 7);

  DEFINE_SETTING3(ICO_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 6);

  DEFINE_SETTING3(VBUS_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 4);

  DEFINE_SETTING3(TREG_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 2);

  DEFINE_SETTING3(VBAT_PRESENT_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 1);

  DEFINE_SETTING3(BC1_2_DONE_FLAG, BOOL, REG23_Charger_Flag_1, 0x01, 0);

  // ===================================
  // REG24_Charger_Flag_2
  // ===================================
  DEFINE_SETTING3(DPDM_DONE_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 6);

  DEFINE_SETTING3(ADC_DONE_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 5);

  DEFINE_SETTING3(VSYS_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 4);

  DEFINE_SETTING3(CHG_TMR_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 3);

  DEFINE_SETTING3(TRICHG_TMR_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 2);

  DEFINE_SETTING3(PRECHG_TMR_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 1);

  DEFINE_SETTING3(TOPOFF_TMR_FLAG, BOOL, REG24_Charger_Flag_2, 0x01, 0);

  // ==================================
  // REG25_Charger_Flag_3
  // ==================================
  DEFINE_SETTING3(VBATOTG_LOW_FLAG, BOOL, REG25_Charger_Flag_3, 0x01, 4);

  DEFINE_SETTING3(TS_COLD_FLAG, BOOL, REG25_Charger_Flag_3, 0x01, 3);

  DEFINE_SETTING3(TS_COOL_FLAG, BOOL, REG25_Charger_Flag_3, 0x01, 2);

  DEFINE_SETTING3(TS_WARM_FLAG, BOOL, REG25_Charger_Flag_3, 0x01, 1);

  DEFINE_SETTING3(TS_HOT_FLAG, BOOL, REG25_Charger_Flag_3, 0x01, 0);

  // ==================================
  // REG26_FAULT_Flag_0
  // ==================================
  DEFINE_SETTING3(IBAT_REG_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 7);

  DEFINE_SETTING3(VBUS_OVP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 6);

  DEFINE_SETTING3(VBAT_OVP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 5);

  DEFINE_SETTING3(IBUS_OCP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 4);

  DEFINE_SETTING3(IBAT_OCP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 3);

  DEFINE_SETTING3(CONV_OCP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 2);

  DEFINE_SETTING3(VAC2_OVP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 1);

  DEFINE_SETTING3(VAC1_OVP_FLAG, BOOL, REG26_FAULT_Flag_0, 0x01, 0);

  // ==================================
  // REG27_FAULT_Flag_1
  // ==================================
  DEFINE_SETTING3(VSYS_SHORT_FLAG, BOOL, REG27_FAULT_Flag_1, 0x01, 7);

  DEFINE_SETTING3(VSYS_OVP_FLAG, BOOL, REG27_FAULT_Flag_1, 0x01, 6);

  DEFINE_SETTING3(OTG_OVP_FLAG, BOOL, REG27_FAULT_Flag_1, 0x01, 5);

  DEFINE_SETTING3(OTG_UVP_FLAG, BOOL, REG27_FAULT_Flag_1, 0x01, 4);

  DEFINE_SETTING3(TSHUT_FLAG, BOOL, REG27_FAULT_Flag_1, 0x01, 2);

  // FIXME REG28_Charger_Mask_0
  // FIXME REG29_Charger_Mask_1
  // FIXME REG2A_Charger_Mask_2
  // FIXME REG2B_Charger_Mask_3
  // FIXME REG2C_FAULT_Mask_0
  // FIXME REG2D_FAULT_Mask_1

  // ==================================
  // REG2E_ADC_Control
  // ==================================
  DEFINE_SETTING3(ADC_EN, BOOL, REG2E_ADC_Control, 0x01, 7);

  enum class adc_rate_t : uint8_t { ADC_RATE_CONTINUOUS = 0, ADC_RATE_ONESHOT = 1 };
  strings_vector_t ADC_RATE_strings = {{F("Continuous")}, {F("One-shot")}};
  DEFINE_ENUM(ADC_RATE, ENUM, REG2E_ADC_Control, 0x01, 6);

  enum class adc_sample_t : uint8_t { ADC_SAMPLE_15BIT = 0, ADC_SAMPLE_14BIT = 1, ADC_SAMPLE_13BIT = 2, ADC_SAMPLE_12BIT = 3 };
  strings_vector_t ADC_SAMPLE_strings = {{F("15-bit")}, {F("14-bit")}, {F("13-bit")}, {F("12-bit")}};
  DEFINE_ENUM(ADC_SAMPLE, ENUM, REG2E_ADC_Control, 0x03, 4);

  enum class adc_avg_t : uint8_t { NO_AVERAGING = 0, RUNNING_AVERAGE = 1 };
  strings_vector_t ADC_AVG_strings = {{F("No averaging")}, {F("Running average")}};
  DEFINE_ENUM(ADC_AVG, ENUM, REG2E_ADC_Control, 0x01, 3);

  DEFINE_SETTING3(ADC_AVG_INIT, BOOL, REG2E_ADC_Control, 0x01, 2);

  // ==================================
  // REG2F_ADC_Function_Disable_0
  // ==================================
  DEFINE_SETTING3(IBUS_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 7);

  DEFINE_SETTING3(IBAT_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 6);

  DEFINE_SETTING3(VBUS_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 5);

  DEFINE_SETTING3(VBAT_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 4);

  DEFINE_SETTING3(VSYS_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 3);

  DEFINE_SETTING3(TS_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 2);

  DEFINE_SETTING3(TDIE_ADC_DIS, BOOL, REG2F_ADC_Function_Disable_0, 0x01, 1);

  // ==================================
  // REG30_ADC_Function_Disable_1
  // ==================================
  DEFINE_SETTING3(DPLUS_ADC_DIS, BOOL, REG30_ADC_Function_Disable_1, 0x01, 7);

  DEFINE_SETTING3(DMINUS_ADC_DIS, BOOL, REG30_ADC_Function_Disable_1, 0x01, 6);

  DEFINE_SETTING3(VAC2_ADC_DIS, BOOL, REG30_ADC_Function_Disable_1, 0x01, 5);

  DEFINE_SETTING3(VAC1_ADC_DIS, BOOL, REG30_ADC_Function_Disable_1, 0x01, 4);

  // ===================================
  // REG31_ADC_Function_Disable_2 (wide)
  // ===================================
  DEFINE_SETTING8(IBUS_ADC, INT, REG31_IBUS_ADC, 0xFFFF, 0, 0, 5000, 0, 1, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG33_ADC_Function_Disable_3 (wide)
  // ===================================
  DEFINE_SETTING8(IBAT_ADC, INT, REG33_IBAT_ADC, 0xFFFF, 0, 0, 8000, 0, 1, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG35_ADC_Function_Disable_4 (wide)
  // ===================================
  DEFINE_SETTING7(VBUS_ADC, INT, REG35_VBUS_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG37_ADC_Function_Disable_5 (wide)
  // ===================================
  DEFINE_SETTING7(VAC1_ADC, INT, REG37_VAC1_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG39_ADC_Function_Disable_6 (wide)
  // ===================================
  DEFINE_SETTING7(VAC2_ADC, INT, REG39_VAC2_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG3B_ADC_Function_Disable_7 (wide)
  // ===================================
  DEFINE_SETTING7(VBAT_ADC, INT, REG3B_VBAT_ADC, 0xFFFF, 0, 0, 20000, 0, 1);

  // ===================================
  // REG3D_ADC_Function_Disable_8 (wide)
  // ===================================
  DEFINE_SETTING7(VSYS_ADC, INT, REG3D_VSYS_ADC, 0xFFFF, 0, 0, 24000, 0, 1);

  // ===================================
  // REG3F_ADC_Function_Disable_9 (wide)
  // ===================================
  DEFINE_SETTING7(TS_ADC, FLOAT, REG3F_TS_ADC, 0xFFFF, 0, 0, -99.9023, 0, 0.0976563);

  // ===================================
  // REG41_ADC_Function_Disable_A (wide)
  // ===================================
  DEFINE_SETTING8(TDIE_ADC, FLOAT, REG41_TDIE_ADC, 0xFFFF, 0, -40, 150, 0, 0.5, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG43_ADC_Function_Disable_B (wide)
  // ===================================
  DEFINE_SETTING7(DPLUS_ADC, INT, REG43_DPLUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG45_ADC_Function_Disable_C (wide)
  // ===================================
  DEFINE_SETTING7(DMINUS_ADC, INT, REG45_DMINUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG47_DPDM_Driver
  // ==================================
  enum class dplus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, DPLUS_DMINUS_SHORT = 7 };
  strings_vector_t DPLUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("D+/D- Short")}};
  DEFINE_ENUM(DPLUS_DAC, ENUM, REG47_DPDM_Driver, 0x07, 5);

  enum class dminus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, RESERVED = 7 };
  strings_vector_t DMINUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("Reserved")}};
  DEFINE_ENUM(DMINUS_DAC, ENUM, REG47_DPDM_Driver, 0x07, 2);

  // ==================================
  // REG48_Part_Information
  // ==================================
  enum class pn_t : uint8_t {
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
  DEFINE_ENUM(PN, ENUM, REG48_Part_Information, 0x07, 3);

  enum class dev_rev_t : uint8_t {
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
  DEFINE_ENUM(DEV_REV, ENUM, REG48_Part_Information, 0x07, 0);

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

 private:
  int _chip_address;

  void _clearRegs();
  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);
  std::array<uint8_t, PHYSICAL_REGISTERS_COUNT> _physicalReg8Values;  // Array to hold 8-bit register values

 public:
  BQ25798();
  BQ25798(uint8_t address);

  bool begin();

  // memory-only operations
  uint16_t getRaw(const Setting& setting);
  int getInt(const Setting& setting);
  bool getBool(const Setting& setting);
  float getFloat(const Setting& setting);
  const char* getString(const Setting& setting);
  const char* toString(int value, strings_vector_t map);
  bool faultDetected();

  int rawToInt(uint16_t raw, const Setting& setting);
  uint16_t intToRaw(int value, const Setting& setting);
  float rawToFloat(uint16_t raw, const Setting& setting);
  bool rawToBool(uint16_t raw, const Setting& setting);
  const char* rawToString(uint16_t raw, const Setting& setting);

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg, uint16_t value, int bitMask = 0xFFFF, int bitShift = 0);

  // I2C operations
  bool readAll();
  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

  void setAndWriteRaw(const Setting& setting, uint16_t value);
  void setAndWriteInt(const Setting& setting, int value);
  void setAndWriteBool(const Setting& setting, bool value);
  void setAndWriteFloat(const Setting& setting, float value);
  void setAndWriteEnum(const Setting& setting, int value);
};

#endif
