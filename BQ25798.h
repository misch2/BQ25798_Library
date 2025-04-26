#ifndef BQ25798_H
#define BQ25798_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Arduino.h"
#include "debug.h"
#include "registers.h"

#define DEFAULT_I2C_ADDRESS 0x6B

// FIXME check
constexpr size_t BQ25798_PHYSICAL_REGISTERS_COUNT = 2;   // (1 + REG48_Part_Information);
// FIXME check
constexpr size_t BQ25798_SETTINGS_COUNT = 191;  // Number of settings

class BQ25798 {
 public:
  // Register definition structure
  enum class regsize_t : uint8_t {
    SHORT = 8,
    LONG = 16,
  };

  typedef int16_t regaddr_t;

  struct RegisterDefinition {
    regaddr_t address;  // Register address
    regsize_t size;     // 8bit or 16bit
    const char* name;   // Register name

    static RegisterDefinition invalid() { return RegisterDefinition{-1, regsize_t::SHORT, F("INVALID")}; };

    bool isValid() const { return address != -1; };
  };

  std::unordered_map<uint8_t, RegisterDefinition> _registerMap;

  RegisterDefinition getRegisterDefinition(regaddr_t address);

  // Register values
  std::array<uint8_t, BQ25798_PHYSICAL_REGISTERS_COUNT> _physicalReg8Values;  // Array to hold 8-bit register values

  // Settings
  enum class settings_flags_t : uint8_t {
    NONE = 0,
    IS_2COMPLEMENT = 0x80,
  };

  typedef const std::vector<std::string> strings_vector_t;

  struct Setting {
    regaddr_t reg;  // Register address
    uint16_t mask;  // Bitmask for the setting
    uint8_t shift;  // Bit shift for the setting
    // optional:
    float range_low;
    float range_high;
    float fixed_offset;   // Fixed offset for the setting (e.g., for voltage settings)
    float bit_step_size;  // Bit step size for the setting (e.g., for current settings)
    // adjusted value = raw_value * bit_step_size + fixed_offset
    settings_flags_t flags;  // Flags for the setting (e.g., 2's complement)
    const char* name;        // Register name

    Setting(regaddr_t reg, const char* name, uint16_t mask, uint8_t shift,
            // optional parameters:
            float range_low = 0, float range_high = 0, float fixed_offset = 0, float bit_step_size = 0, settings_flags_t flags = settings_flags_t::NONE)
        : reg(reg),
          name(name),
          mask(mask),
          shift(shift),
          range_low(range_low),
          range_high(range_high),
          fixed_offset(fixed_offset),
          bit_step_size(bit_step_size),
          flags(flags) {};

    static Setting invalid() { return Setting{RegisterDefinition::invalid().address, F("INVALID"), 0, 0, 0, 0, 0, 0}; };

    bool isValid() const { return mask != 0; };
  };

  std::array<Setting, BQ25798_SETTINGS_COUNT> _settingsList;

  Setting getSetting(uint16_t id);

#define DEFINE_SETTING3(setting, regaddr, mask, shift) Setting setting = {regaddr, #setting, mask, shift}
#define DEFINE_SETTING7(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size) \
  Setting setting = {regaddr, #setting, mask, shift, range_low, range_high, fixed_offset, bit_step_size}
#define DEFINE_SETTING8(setting, regaddr, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags) \
  Setting setting = {regaddr, #setting, mask, shift, range_low, range_high, fixed_offset, bit_step_size, flags}

  // ===================================
  // REG00_Minimal_System_Voltage
  // ===================================
  // DEFINE_SETTING3(VSYSMIN, REG00_Minimal_System_Voltage, 0x3F,  0, 2500, 16000, 2500, 250);
  DEFINE_SETTING7(VSYSMIN, REG00_Minimal_System_Voltage, 0x3F, 0, 2500, 16000, 2500, 250);

  // ===================================
  // REG01_Charge_Voltage_Limit (wide)
  // ===================================
  DEFINE_SETTING7(VREG, REG01_Charge_Voltage_Limit, 0x7FF, 0, 3000, 18800, 0, 10);

  // ===================================
  // REG03_Charge_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING7(ICHG, REG03_Charge_Current_Limit, 0x1FF, 0, 50, 5000, 0, 10);

  // ===================================
  // REG05_Input_Voltage_Limit
  // ===================================
  DEFINE_SETTING7(VINDPM, REG05_Input_Voltage_Limit, 0xFF, 0, 3600, 22000, 0, 100);

  // ===================================
  // REG06_Input_Current_Limit (wide)
  // ===================================
  DEFINE_SETTING7(IINDPM, REG06_Input_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ===================================
  // REG08_Precharge_Control
  // ===================================
  DEFINE_SETTING3(VBAT_LOWV, REG08_Precharge_Control, 0x3, 6);
  enum class vbat_lowv_t : uint8_t { PCT_15 = 0, PCT_62 = 1, PCT_67 = 2, PCT_71 = 3 };
  strings_vector_t VBAT_LOWV_strings = {{F("15%(VREG)")}, {F("62.2%(VREG)")}, {F("66.7%(VREG)")}, {F("71.4%(VREG)")}};

  DEFINE_SETTING7(IPRECHG, REG08_Precharge_Control, 0x3F, 0, 40, 2000, 0, 40);

  // ===================================
  // REG09_Termination_Control
  // ===================================
  DEFINE_SETTING3(REG_RST, REG09_Termination_Control, 0x01, 6);

  DEFINE_SETTING3(STOP_WD_CHG, REG09_Termination_Control, 0x01, 5);

  DEFINE_SETTING7(ITERM, REG09_Termination_Control, 0x1F, 0, 40, 1000, 0, 40);

  // ===================================
  // REG0A_Recharge_Control
  // ===================================
  DEFINE_SETTING3(CELL, REG0A_Recharge_Control, 0x03, 6);
  enum class cell_t : uint8_t { CELL_1S = 0, CELL_2S = 1, CELL_3S = 2, CELL_4S = 3 };
  strings_vector_t CELL_strings = {{F("1S")}, {F("2S")}, {F("3S")}, {F("4S")}};

  DEFINE_SETTING3(TRECHG, REG0A_Recharge_Control, 0x03, 4);
  enum class trechg_t : uint8_t { TRECHG_64MS = 0, TRECHG_256MS = 1, TRECHG_1024MS = 2, TRECHG_2048MS = 3 };
  strings_vector_t TRECHG_strings = {{F("64 ms")}, {F("256 ms")}, {F("1024 ms")}, {F("2048 ms")}};

  DEFINE_SETTING7(VRECHG, REG0A_Recharge_Control, 0x0F, 0, 50, 800, 50, 50);

  // ===================================
  // REG0B_VOTG_regulation (wide)
  // ===================================
  DEFINE_SETTING7(VOTG, REG0B_VOTG_regulation, 0x7FF, 0, 2800, 22000, 2800, 10);

  // ===================================
  // REG0D_IOTG_regulation
  // ===================================
  DEFINE_SETTING3(PRECHG_TMR, REG0D_IOTG_regulation, 0x01, 7);
  enum class precgh_tmr_t {
    PRECHG_TMR_2HRS = 0,
    PRECHG_TMR_30MIN = 1,
  };
  strings_vector_t PRECHG_TMR_strings = {{F("2 hours")}, {F("30 minutes")}};

  DEFINE_SETTING7(IOTG, REG0D_IOTG_regulation, 0x7F, 0, 160, 3360, 0, 40);

  // ===================================
  // REG0E_Timer_Control
  // ===================================
  DEFINE_SETTING3(TOPOFF_TMR, REG0E_Timer_Control, 0x03, 6);
  enum class tophff_tmr_t : uint8_t { TOPOFF_TMR_DISABLED = 0, TOPOFF_TMR_15MIN = 1, TOPOFF_TMR_30MIN = 2, TOPOFF_TMR_45MIN = 3 };
  strings_vector_t TOPOFF_TMR_strings = {{F("Disabled")}, {F("15 minutes")}, {F("30 minutes")}, {F("45 minutes")}};

  DEFINE_SETTING3(EN_TRICHG_TMR, REG0E_Timer_Control, 0x01, 5);

  DEFINE_SETTING3(EN_PRECHG_TMR, REG0E_Timer_Control, 0x01, 4);

  DEFINE_SETTING3(EN_CHG_TMR, REG0E_Timer_Control, 0x01, 3);

  DEFINE_SETTING3(CHG_TMR, REG0E_Timer_Control, 0x03, 1);
  enum class chg_tmr_t : uint8_t { CHG_TMR_5HRS = 0, CHG_TMR_8HRS = 1, CHG_TMR_12HRS = 2, CHG_TMR_24HRS = 3 };
  strings_vector_t CHG_TMR_strings = {{F("5 hours")}, {F("8 hours")}, {F("12 hours")}, {F("24 hours")}};

  DEFINE_SETTING3(TMR2X_EN, REG0E_Timer_Control, 0x01, 0);

  // ===================================
  // REG0F_Charger_Control_0
  // ===================================
  DEFINE_SETTING3(EN_AUTO_IBATDIS, REG0F_Charger_Control_0, 0x01, 7);

  DEFINE_SETTING3(FORCE_IBATDIS, REG0F_Charger_Control_0, 0x01, 6);

  DEFINE_SETTING3(EN_CHG, REG0F_Charger_Control_0, 0x01, 5);

  DEFINE_SETTING3(EN_ICO, REG0F_Charger_Control_0, 0x01, 4);

  DEFINE_SETTING3(FORCE_ICO, REG0F_Charger_Control_0, 0x01, 3);

  DEFINE_SETTING3(EN_HIZ, REG0F_Charger_Control_0, 0x01, 2);

  DEFINE_SETTING3(EN_TERM, REG0F_Charger_Control_0, 0x01, 1);

  DEFINE_SETTING3(EN_BACKUP, REG0F_Charger_Control_0, 0x01, 0);

  // ===================================
  // REG10_Charger_Control_1
  // ===================================
  DEFINE_SETTING3(VBUS_BACKUP, REG10_Charger_Control_1, 0x03, 6);
  enum class vbus_backup_t : uint8_t { PCT_VBUS_BACKUP_40 = 0, PCT_VBUS_BACKUP_60 = 1, PCT_VBUS_BACKUP_80 = 2, PCT_VBUS_BACKUP_100 = 3 };
  strings_vector_t VBUS_BACKUP_strings = {{F("<40%(VINDPM)")}, {F("<60%(VINDPM)")}, {F("<80%(VINDPM)")}, {F("<100%(VINDPM)")}};

  DEFINE_SETTING3(VAC_OVP, REG10_Charger_Control_1, 0x03, 4);
  enum class vac_ovp_t : uint8_t { VAC_OVP_26V = 0, VAC_OVP_22V = 1, VAC_OVP_12V = 2, VAC_OVP_7V = 3 };
  strings_vector_t VAC_OVP_strings = {{F(">26V")}, {F(">22V")}, {F(">12V")}, {F(">7V")}};

  DEFINE_SETTING3(WD_RST, REG10_Charger_Control_1, 0x01, 3);

  DEFINE_SETTING3(WATCHDOG, REG10_Charger_Control_1, 0x07, 0);
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

  // ===================================
  // REG11_Charger_Control_2
  // ===================================
  DEFINE_SETTING3(FORCE_INDET, REG11_Charger_Control_2, 0x01, 7);

  DEFINE_SETTING3(AUTO_INDET_EN, REG11_Charger_Control_2, 0x01, 6);

  DEFINE_SETTING3(EN_12V, REG11_Charger_Control_2, 0x01, 5);

  DEFINE_SETTING3(EN_9V, REG11_Charger_Control_2, 0x01, 4);

  DEFINE_SETTING3(HVDCP_EN, REG11_Charger_Control_2, 0x01, 3);

  DEFINE_SETTING3(SDRV_CTRL, REG11_Charger_Control_2, 0x03, 1);
  enum class sdrv_ctrl_t : uint8_t { SDRV_CTRL_IDLE = 0, SDRV_CTRL_SHUTDOWN = 1, SDRV_CTRL_SHIP = 2, SDRV_CTRL_SYS_PWR_RST = 3 };
  strings_vector_t SDRV_CTRL_strings = {{F("Idle")}, {F("Shutdown")}, {F("Ship")}, {F("System Power Reset")}};

  DEFINE_SETTING3(SDRV_DLY, REG11_Charger_Control_2, 0x01, 0);
  enum class sdrv_dly_t : uint8_t { SDRV_DLY_10S = 0, SDRV_DLY_0S = 1 };
  strings_vector_t SDRV_DLY_strings = {{F("10s")}, {F("0s")}};

  // ===================================
  // REG12_Charger_Control_3
  // ===================================
  DEFINE_SETTING3(DIS_ACDRV, REG12_Charger_Control_3, 0x01, 7);

  DEFINE_SETTING3(EN_OTG, REG12_Charger_Control_3, 0x01, 6);

  DEFINE_SETTING3(PFM_OTG_DIS, REG12_Charger_Control_3, 0x01, 5);

  DEFINE_SETTING3(PFM_FWD_DIS, REG12_Charger_Control_3, 0x01, 4);

  DEFINE_SETTING3(WKUP_DLY, REG12_Charger_Control_3, 0x01, 3);
  enum class wkup_dly_t : uint8_t { WKUP_DLY_1S = 0, WKUP_DLY_15MS = 1 };
  strings_vector_t WKUP_DLY_strings = {{F("1s")}, {F("15ms")}};

  DEFINE_SETTING3(DIS_LDO, REG12_Charger_Control_3, 0x01, 2);

  DEFINE_SETTING3(DIS_OTG_OOA, REG12_Charger_Control_3, 0x01, 1);

  DEFINE_SETTING3(DIS_FWD_OOA, REG12_Charger_Control_3, 0x01, 0);

  // ===================================
  // REG13_Charger_Control_4
  // ===================================
  DEFINE_SETTING3(EN_ACDRV2, REG13_Charger_Control_4, 0x01, 7);

  DEFINE_SETTING3(EN_ACDRV1, REG13_Charger_Control_4, 0x01, 6);

  DEFINE_SETTING3(PWM_FREQ, REG13_Charger_Control_4, 0x01, 5);
  enum class pwm_freq_t : uint8_t { PWM_FREQ_1_5MHZ = 0, PWM_FREQ_750KHZ = 1 };
  strings_vector_t PWM_FREQ_strings = {{F("1.5 MHz")}, {F("750 kHz")}};

  DEFINE_SETTING3(DIS_STAT, REG13_Charger_Control_4, 0x01, 4);

  DEFINE_SETTING3(DIS_VSYS_SHORT, REG13_Charger_Control_4, 0x01, 3);

  DEFINE_SETTING3(DIS_VOTG_UVP, REG13_Charger_Control_4, 0x01, 2);

  DEFINE_SETTING3(FORCE_VINDPM_DET, REG13_Charger_Control_4, 0x01, 1);

  DEFINE_SETTING3(EN_IBUS_OCP, REG13_Charger_Control_4, 0x01, 0);

  // ===================================
  // REG14_Charger_Control_5
  // ===================================
  DEFINE_SETTING3(SFET_PRESENT, REG14_Charger_Control_5, 0x01, 7);

  DEFINE_SETTING3(EN_IBAT, REG14_Charger_Control_5, 0x01, 5);

  DEFINE_SETTING3(IBAT_REG, REG14_Charger_Control_5, 0x03, 3);
  enum class ibat_reg_t : uint8_t { IBAT_REG_3A = 0, IBAT_REG_4A = 1, IBAT_REG_5A = 2, IBAT_REG_DISABLE = 3 };
  strings_vector_t IBAT_REG_strings = {{F("3A")}, {F("4A")}, {F("5A")}, {F("Disabled")}};

  DEFINE_SETTING3(EN_IINDPM, REG14_Charger_Control_5, 0x01, 2);

  DEFINE_SETTING3(EN_EXTILIM, REG14_Charger_Control_5, 0x01, 1);

  DEFINE_SETTING3(EN_BATOC, REG14_Charger_Control_5, 0x01, 0);

  // ===================================
  // REG15_MPPT_Control
  // ===================================
  DEFINE_SETTING3(VOC_PCT, REG15_MPPT_Control, 0x07, 5);
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

  DEFINE_SETTING3(VOC_DLY, REG15_MPPT_Control, 0x03, 4);
  enum class voc_dly_t : uint8_t { VOC_DLY_50MS = 0, VOC_DLY_300MS = 1, VOC_DLY_2S = 2, VOC_DLY_5S = 3 };
  strings_vector_t VOC_DLY_strings = {{F("50ms")}, {F("300ms")}, {F("2s")}, {F("5s")}};

  DEFINE_SETTING3(VOC_RATE, REG15_MPPT_Control, 0x03, 2);
  enum class voc_rate_t : uint8_t { VOC_RATE_30S = 0, VOC_RATE_2MIN = 1, VOC_RATE_10MIN = 2, VOC_RATE_30MIN = 3 };
  strings_vector_t VOC_RATE_strings = {{F("30s")}, {F("2min")}, {F("10min")}, {F("30min")}};

  DEFINE_SETTING3(EN_MPPT, REG15_MPPT_Control, 0x01, 0);

  // ===================================
  // REG16_Temperature_Control
  // ===================================
  DEFINE_SETTING3(TREG, REG16_Temperature_Control, 0x03, 6);
  enum class treg_t : uint8_t { TREG_60 = 0, TREG_80 = 1, TREG_100 = 2, TREG_120 = 3 };
  strings_vector_t TREG_strings = {{F("60°C")}, {F("80°C")}, {F("100°C")}, {F("120°C")}};

  DEFINE_SETTING3(TSHUT, REG16_Temperature_Control, 0x03, 4);
  enum class tshut_t : uint8_t { TSHUT_150 = 0, TSHUT_130 = 1, TSHUT_120 = 2, TSHUT_85 = 3 };
  strings_vector_t TSHUT_strings = {{F("150°C")}, {F("130°C")}, {F("120°C")}, {F("85°C")}};

  DEFINE_SETTING3(VBUS_PD_EN, REG16_Temperature_Control, 0x01, 3);

  DEFINE_SETTING3(VAC1_PD_EN, REG16_Temperature_Control, 0x01, 2);

  DEFINE_SETTING3(VAC2_PD_EN, REG16_Temperature_Control, 0x01, 1);

  DEFINE_SETTING3(BKUP_ACFET1_ON, REG16_Temperature_Control, 0x01, 0);
  enum class bkup_acfet1_on_t : uint8_t { IDLE = 0, TURN_ON = 1 };
  strings_vector_t BKUP_ACFET1_ON_strings = {{F("Idle")}, {F("Turn on ACFET1 in backup mode")}};

  // ==================================
  // REG17_NTC_Control_0
  // ==================================
  DEFINE_SETTING3(JEITA_VSET, REG17_NTC_Control_0, 0x07, 5);
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

  DEFINE_SETTING3(JEITA_ISETH, REG17_NTC_Control_0, 0x03, 3);
  enum class jeita_iseth_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETH_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};

  DEFINE_SETTING3(JEITA_ISETC, REG17_NTC_Control_0, 0x03, 1);
  enum class jeita_isetc_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
  strings_vector_t JEITA_ISETC_strings = {{F("Charge Suspend")}, {F("Set ICHG to 20%* ICHG (default)")}, {F("Set ICHG to 40%* ICHG")}, {F("ICHG unchanged")}};

  // ==================================
  // REG18_NTC_Control_1
  // ==================================
  DEFINE_SETTING3(TS_COOL, REG18_NTC_Control_1, 0x03, 6);
  enum class ts_cool_t : uint8_t { TS_5 = 0, TS_10 = 1, TS_15 = 2, TS_20 = 3 };
  strings_vector_t TS_COOL_strings = {{F("5°C")}, {F("10°C (default)")}, {F("15°C")}, {F("20°C")}};

  DEFINE_SETTING3(TS_WARM, REG18_NTC_Control_1, 0x03, 4);
  enum class ts_warm_t : uint8_t { TS_40 = 0, TS_45 = 1, TS_50 = 2, TS_55 = 3 };
  strings_vector_t TS_WARM_strings = {{F("40°C")}, {F("45°C (default)")}, {F("50°C")}, {F("55°C")}};

  DEFINE_SETTING3(BHOT, REG18_NTC_Control_1, 0x03, 2);
  enum class bhot_t : uint8_t { TS_55 = 0, TS_60 = 1, TS_65 = 2, DISABLE = 3 };
  strings_vector_t BHOT_strings = {{F("55°C")}, {F("60°C (default)")}, {F("65°C")}, {F("Disabled")}};

  DEFINE_SETTING3(BCOLD, REG18_NTC_Control_1, 0x01, 1);
  enum class bcold_t : uint8_t { MINUS_10 = 0, MINUS_20 = 1 };
  strings_vector_t BCOLD_strings = {{F("-10°C (default)")}, {F("-20°C")}};

  DEFINE_SETTING3(TS_IGNORE, REG18_NTC_Control_1, 0x01, 0);

  // ==================================
  // REG19_ICO_Current_Limit (wide)
  // ==================================
  DEFINE_SETTING7(ICO_ILIM, REG19_ICO_Current_Limit, 0x1FF, 0, 100, 3300, 0, 10);

  // ==================================
  // REG1B_Charger_Status_0
  // ==================================
  DEFINE_SETTING3(IINDPM_STAT, REG1B_Charger_Status_0, 0x01, 7);
  enum class iindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t IINDPM_STAT_strings = {{F("Normal")}, {F("In IINDPM regulation or IOTG regulation")}};

  DEFINE_SETTING3(VINDPM_STAT, REG1B_Charger_Status_0, 0x01, 6);
  enum class vindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
  strings_vector_t VINDPM_STAT_strings = {{F("Normal")}, {F("In VINDPM regulation or VOTG regulation")}};

  DEFINE_SETTING3(WD_STAT, REG1B_Charger_Status_0, 0x01, 5);
  enum class wd_stat_t : uint8_t { NORMAL = 0, EXPIRED = 1 };
  strings_vector_t WD_STAT_strings = {{F("Normal")}, {F("Watchdog timer expired")}};

  DEFINE_SETTING3(PG_STAT, REG1B_Charger_Status_0, 0x01, 3);
  enum class pg_stat_t : uint8_t { BAD = 0, GOOD = 1 };
  strings_vector_t PG_STAT_strings = {{F("Not in power good status")}, {F("Power good")}};

  DEFINE_SETTING3(AC2_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 2);
  enum class ac2_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC2_PRESENT_STAT_strings = {{F("VAC2 NOT present")}, {F("VAC2 present (above present threshold)")}};

  DEFINE_SETTING3(AC1_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 1);
  enum class ac1_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t AC1_PRESENT_STAT_strings = {{F("VAC1 NOT present")}, {F("VAC1 present (above present threshold)")}};

  DEFINE_SETTING3(VBUS_PRESENT_STAT, REG1B_Charger_Status_0, 0x01, 0);
  enum class vbus_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBUS_PRESENT_STAT_strings = {{F("VBUS NOT present")}, {F("VBUS present (above present threshold)")}};

  // ==================================
  // REG1C_Charger_Status_1
  // ==================================
  DEFINE_SETTING3(CHG_STAT, REG1C_Charger_Status_1, 0x7, 5);
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

  DEFINE_SETTING3(VBUS_STAT, REG1C_Charger_Status_1, 0x0F, 1);
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

  DEFINE_SETTING3(BC12_DONE_STAT, REG1C_Charger_Status_1, 0x01, 0);

  // ==================================
  // REG1D_Charger_Status_2
  // ==================================
  DEFINE_SETTING3(ICO_STAT, REG1D_Charger_Status_2, 0x03, 6);
  enum class ico_stat_t : uint8_t { ICO_DISABLED = 0, ICO_IN_PROGRESS = 1, ICO_MAX_CURRENT_DETECTED = 2, ICO_RESERVED = 3 };
  strings_vector_t ICO_STAT_strings = {{F("ICO disabled")}, {F("ICO optimization in progress")}, {F("Maximum input current detected")}, {F("Reserved")}};

  DEFINE_SETTING3(TREG_STAT, REG1D_Charger_Status_2, 0x01, 5);
  enum class treg_stat_t : uint8_t { NORMAL = 0, THERMAL_REGULATION = 1 };
  strings_vector_t TREG_STAT_strings = {{F("Normal")}, {F("Device in thermal regulation")}};

  DEFINE_SETTING3(DPDM_STAT, REG1D_Charger_Status_2, 0x01, 4);
  enum class dpdm_stat_t : uint8_t { NOT_STARTED = 0, IN_PROGRESS = 1 };
  strings_vector_t DPDM_STAT_strings = {{F("D+/D- detection NOT started yet or done")}, {F("D+/D- detection in progress")}};

  DEFINE_SETTING3(VBAT_PRESENT_STAT, REG1D_Charger_Status_2, 0x01, 0);
  enum class vbat_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
  strings_vector_t VBAT_PRESENT_STAT_strings = {{F("VBAT NOT present")}, {F("VBAT present")}};

  // ==================================
  // REG1E_Charger_Status_3
  // ==================================
  DEFINE_SETTING3(ACRB2_STAT, REG1E_Charger_Status_3, 0x01, 7);

  DEFINE_SETTING3(ACRB1_STAT, REG1E_Charger_Status_3, 0x01, 6);

  DEFINE_SETTING3(ADC_DONE_STAT, REG1E_Charger_Status_3, 0x01, 5);

  DEFINE_SETTING3(VSYS_STAT, REG1E_Charger_Status_3, 0x01, 4);
  enum class vsys_stat_t : uint8_t { NOT_IN_VSYSMIN_REGULATION = 0, IN_VSYSMIN_REGULATION = 1 };
  strings_vector_t VSYS_STAT_strings = {{F("Not in VSYSMIN regulation (VBAT > VSYSMIN)")}, {F("In VSYSMIN regulation (VBAT < VSYSMIN)")}};

  DEFINE_SETTING3(CHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 3);
  enum class chg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t CHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};

  DEFINE_SETTING3(TRICHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 2);
  enum class trichg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t TRICHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};

  DEFINE_SETTING3(PRECHG_TMR_STAT, REG1E_Charger_Status_3, 0x01, 1);
  enum class prechg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
  strings_vector_t PRECHG_TMR_STAT_strings = {{F("Normal")}, {F("Safety timer expired")}};

  // ==================================
  // REG1F_Charger_Status_4
  // ==================================
  DEFINE_SETTING3(VBATOTG_LOW_STAT, REG1F_Charger_Status_4, 0x01, 4);
  enum class vbatotg_low_stat_t : uint8_t { VBATOTG_LOW = 0, VBATOTG_OK = 1 };
  strings_vector_t VBATOTG_LOW_STAT_strings = {{F("VBAT is too low to enable OTG mode")}, {F("VBAT is high enough to enable OTG operation")}};

  DEFINE_SETTING3(TS_COLD_STAT, REG1F_Charger_Status_4, 0x01, 3);
  enum class ts_cold_stat_t : uint8_t { NOT_COLD = 0, COLD = 1 };
  strings_vector_t TS_COLD_STAT_strings = {{F("TS NOT in cold range")}, {F("TS in cold range")}};

  DEFINE_SETTING3(TS_COOL_STAT, REG1F_Charger_Status_4, 0x01, 2);
  enum class ts_cool_stat_t : uint8_t { NOT_COOL = 0, COOL = 1 };
  strings_vector_t TS_COOL_STAT_strings = {{F("TS NOT in cool range")}, {F("TS in cool range")}};

  DEFINE_SETTING3(TS_WARM_STAT, REG1F_Charger_Status_4, 0x01, 1);
  enum class ts_warm_stat_t : uint8_t { NOT_WARM = 0, WARM = 1 };
  strings_vector_t TS_WARM_STAT_strings = {{F("TS NOT in warm range")}, {F("TS in warm range")}};

  DEFINE_SETTING3(TS_HOT_STAT, REG1F_Charger_Status_4, 0x01, 0);
  enum class ts_hot_stat_t : uint8_t { NOT_HOT = 0, HOT = 1 };
  strings_vector_t TS_HOT_STAT_strings = {{F("TS NOT in hot range")}, {F("TS in hot range")}};

  // ==================================
  // REG20_FAULT_Status_0
  // ==================================
  DEFINE_SETTING3(IBAT_REG_STAT, REG20_FAULT_Status_0, 0x01, 7);

  DEFINE_SETTING3(VBUS_OVP_STAT, REG20_FAULT_Status_0, 0x01, 6);

  DEFINE_SETTING3(VBAT_OVP_STAT, REG20_FAULT_Status_0, 0x01, 5);

  DEFINE_SETTING3(IBUS_OCP_STAT, REG20_FAULT_Status_0, 0x01, 4);

  DEFINE_SETTING3(IBAT_OCP_STAT, REG20_FAULT_Status_0, 0x01, 3);

  DEFINE_SETTING3(CONV_OCP_STAT, REG20_FAULT_Status_0, 0x01, 2);

  DEFINE_SETTING3(VAC2_OVP_STAT, REG20_FAULT_Status_0, 0x01, 1);

  DEFINE_SETTING3(VAC1_OVP_STAT, REG20_FAULT_Status_0, 0x01, 0);

  // ==================================
  // REG21_FAULT_Status_1
  // ==================================
  DEFINE_SETTING3(VSYS_SHORT_STAT, REG21_FAULT_Status_1, 0x01, 7);

  DEFINE_SETTING3(VSYS_OVP_STAT, REG21_FAULT_Status_1, 0x01, 6);

  DEFINE_SETTING3(OTG_OVP_STAT, REG21_FAULT_Status_1, 0x01, 5);

  DEFINE_SETTING3(OTG_UVP_STAT, REG21_FAULT_Status_1, 0x01, 4);

  DEFINE_SETTING3(TSHUT_STAT, REG21_FAULT_Status_1, 0x01, 2);

  // ==================================
  // REG22_Charger_Flag_0
  // ==================================
  DEFINE_SETTING3(IINDPM_FLAG, REG22_Charger_Flag_0, 0x01, 7);

  DEFINE_SETTING3(VINDPM_FLAG, REG22_Charger_Flag_0, 0x01, 6);

  DEFINE_SETTING3(WD_FLAG, REG22_Charger_Flag_0, 0x01, 5);

  DEFINE_SETTING3(POORSRC_FLAG, REG22_Charger_Flag_0, 0x01, 4);

  DEFINE_SETTING3(PG_FLAG, REG22_Charger_Flag_0, 0x01, 3);

  DEFINE_SETTING3(AC2_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 2);

  DEFINE_SETTING3(AC1_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 1);

  DEFINE_SETTING3(VBUS_PRESENT_FLAG, REG22_Charger_Flag_0, 0x01, 0);

  // ===================================
  // REG23_Charger_Flag_1
  // ===================================
  DEFINE_SETTING3(CHG_FLAG, REG23_Charger_Flag_1, 0x01, 7);

  DEFINE_SETTING3(ICO_FLAG, REG23_Charger_Flag_1, 0x01, 6);

  DEFINE_SETTING3(VBUS_FLAG, REG23_Charger_Flag_1, 0x01, 4);

  DEFINE_SETTING3(TREG_FLAG, REG23_Charger_Flag_1, 0x01, 2);

  DEFINE_SETTING3(VBAT_PRESENT_FLAG, REG23_Charger_Flag_1, 0x01, 1);

  DEFINE_SETTING3(BC1_2_DONE_FLAG, REG23_Charger_Flag_1, 0x01, 0);

  // ===================================
  // REG24_Charger_Flag_2
  // ===================================
  DEFINE_SETTING3(DPDM_DONE_FLAG, REG24_Charger_Flag_2, 0x01, 6);

  DEFINE_SETTING3(ADC_DONE_FLAG, REG24_Charger_Flag_2, 0x01, 5);

  DEFINE_SETTING3(VSYS_FLAG, REG24_Charger_Flag_2, 0x01, 4);

  DEFINE_SETTING3(CHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 3);

  DEFINE_SETTING3(TRICHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 2);

  DEFINE_SETTING3(PRECHG_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 1);

  DEFINE_SETTING3(TOPOFF_TMR_FLAG, REG24_Charger_Flag_2, 0x01, 0);

  // ==================================
  // REG25_Charger_Flag_3
  // ==================================
  DEFINE_SETTING3(VBATOTG_LOW_FLAG, REG25_Charger_Flag_3, 0x01, 4);

  DEFINE_SETTING3(TS_COLD_FLAG, REG25_Charger_Flag_3, 0x01, 3);

  DEFINE_SETTING3(TS_COOL_FLAG, REG25_Charger_Flag_3, 0x01, 2);

  DEFINE_SETTING3(TS_WARM_FLAG, REG25_Charger_Flag_3, 0x01, 1);

  DEFINE_SETTING3(TS_HOT_FLAG, REG25_Charger_Flag_3, 0x01, 0);

  // ==================================
  // REG26_FAULT_Flag_0
  // ==================================
  DEFINE_SETTING3(IBAT_REG_FLAG, REG26_FAULT_Flag_0, 0x01, 7);

  DEFINE_SETTING3(VBUS_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 6);

  DEFINE_SETTING3(VBAT_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 5);

  DEFINE_SETTING3(IBUS_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 4);

  DEFINE_SETTING3(IBAT_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 3);

  DEFINE_SETTING3(CONV_OCP_FLAG, REG26_FAULT_Flag_0, 0x01, 2);

  DEFINE_SETTING3(VAC2_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 1);

  DEFINE_SETTING3(VAC1_OVP_FLAG, REG26_FAULT_Flag_0, 0x01, 0);

  // ==================================
  // REG27_FAULT_Flag_1
  // ==================================
  DEFINE_SETTING3(VSYS_SHORT_FLAG, REG27_FAULT_Flag_1, 0x01, 7);

  DEFINE_SETTING3(VSYS_OVP_FLAG, REG27_FAULT_Flag_1, 0x01, 6);

  DEFINE_SETTING3(OTG_OVP_FLAG, REG27_FAULT_Flag_1, 0x01, 5);

  DEFINE_SETTING3(OTG_UVP_FLAG, REG27_FAULT_Flag_1, 0x01, 4);

  DEFINE_SETTING3(TSHUT_FLAG, REG27_FAULT_Flag_1, 0x01, 2);

  // FIXME REG28_Charger_Mask_0
  // FIXME REG29_Charger_Mask_1
  // FIXME REG2A_Charger_Mask_2
  // FIXME REG2B_Charger_Mask_3
  // FIXME REG2C_FAULT_Mask_0
  // FIXME REG2D_FAULT_Mask_1

  // ==================================
  // REG2E_ADC_Control
  // ==================================
  DEFINE_SETTING3(ADC_EN, REG2E_ADC_Control, 0x01, 7);

  DEFINE_SETTING3(ADC_RATE, REG2E_ADC_Control, 0x01, 6);
  enum class adc_rate_t : uint8_t { ADC_RATE_CONTINUOUS = 0, ADC_RATE_ONESHOT = 1 };
  strings_vector_t ADC_RATE_strings = {{F("Continuous")}, {F("One-shot")}};

  DEFINE_SETTING3(ADC_SAMPLE, REG2E_ADC_Control, 0x03, 4);
  enum class adc_sample_t : uint8_t { ADC_SAMPLE_15BIT = 0, ADC_SAMPLE_14BIT = 1, ADC_SAMPLE_13BIT = 2, ADC_SAMPLE_12BIT = 3 };
  strings_vector_t ADC_SAMPLE_strings = {{F("15-bit")}, {F("14-bit")}, {F("13-bit")}, {F("12-bit")}};

  DEFINE_SETTING3(ADC_AVG, REG2E_ADC_Control, 0x01, 3);
  enum class adc_avg_t : uint8_t { NO_AVERAGING = 0, RUNNING_AVERAGE = 1 };
  strings_vector_t ADC_AVG_strings = {{F("No averaging")}, {F("Running average")}};

  DEFINE_SETTING3(ADC_AVG_INIT, REG2E_ADC_Control, 0x01, 2);

  // ==================================
  // REG2F_ADC_Function_Disable_0
  // ==================================
  DEFINE_SETTING3(IBUS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 7);

  DEFINE_SETTING3(IBAT_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 6);

  DEFINE_SETTING3(VBUS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 5);

  DEFINE_SETTING3(VBAT_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 4);

  DEFINE_SETTING3(VSYS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 3);

  DEFINE_SETTING3(TS_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 2);

  DEFINE_SETTING3(TDIE_ADC_DIS, REG2F_ADC_Function_Disable_0, 0x01, 1);

  // ==================================
  // REG30_ADC_Function_Disable_1
  // ==================================
  DEFINE_SETTING3(DPLUS_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 7);

  DEFINE_SETTING3(DMINUS_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 6);

  DEFINE_SETTING3(VAC2_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 5);

  DEFINE_SETTING3(VAC1_ADC_DIS, REG30_ADC_Function_Disable_1, 0x01, 4);

  // ===================================
  // REG31_ADC_Function_Disable_2 (wide)
  // ===================================
  DEFINE_SETTING8(IBUS_ADC, REG31_IBUS_ADC, 0xFFFF, 0, 0, 5000, 0, 1, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG33_ADC_Function_Disable_3 (wide)
  // ===================================
  DEFINE_SETTING8(IBAT_ADC, REG33_IBAT_ADC, 0xFFFF, 0, 0, 8000, 0, 1, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG35_ADC_Function_Disable_4 (wide)
  // ===================================
  DEFINE_SETTING7(VBUS_ADC, REG35_VBUS_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG37_ADC_Function_Disable_5 (wide)
  // ===================================
  DEFINE_SETTING7(VAC1_ADC, REG37_VAC1_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG39_ADC_Function_Disable_6 (wide)
  // ===================================
  DEFINE_SETTING7(VAC2_ADC, REG39_VAC2_ADC, 0xFFFF, 0, 0, 30000, 0, 1);

  // ===================================
  // REG3B_ADC_Function_Disable_7 (wide)
  // ===================================
  DEFINE_SETTING7(VBAT_ADC, REG3B_VBAT_ADC, 0xFFFF, 0, 0, 20000, 0, 1);

  // ===================================
  // REG3D_ADC_Function_Disable_8 (wide)
  // ===================================
  DEFINE_SETTING7(VSYS_ADC, REG3D_VSYS_ADC, 0xFFFF, 0, 0, 24000, 0, 1);

  // ===================================
  // REG3F_ADC_Function_Disable_9 (wide)
  // ===================================
  DEFINE_SETTING7(TS_ADC, REG3F_TS_ADC, 0xFFFF, 0, 0, -99.9023, 0, 0.0976563);

  // ===================================
  // REG41_ADC_Function_Disable_A (wide)
  // ===================================
  DEFINE_SETTING8(TDIE_ADC, REG41_TDIE_ADC, 0xFFFF, 0, -40, 150, 0, 0.5, settings_flags_t::IS_2COMPLEMENT);

  // ===================================
  // REG43_ADC_Function_Disable_B (wide)
  // ===================================
  DEFINE_SETTING7(DPLUS_ADC, REG43_DPLUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG45_ADC_Function_Disable_C (wide)
  // ===================================
  DEFINE_SETTING7(DMINUS_ADC, REG45_DMINUS_ADC, 0xFFFF, 0, 0, 3600, 0, 1);

  // ===================================
  // REG47_DPDM_Driver
  // ==================================
  DEFINE_SETTING3(DPLUS_DAC, REG47_DPDM_Driver, 0x07, 5);
  enum class dplus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, DPLUS_DMINUS_SHORT = 7 };
  strings_vector_t DPLUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("D+/D- Short")}};

  DEFINE_SETTING3(DMINUS_DAC, REG47_DPDM_Driver, 0x07, 2);
  enum class dminus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, RESERVED = 7 };
  strings_vector_t DMINUS_DAC_strings = {{F("HIZ")}, {F("0V")}, {F("0.6V")}, {F("1.2V")}, {F("2.0V")}, {F("2.7V")}, {F("3.3V")}, {F("Reserved")}};

  // ==================================
  // REG48_Part_Information
  // ==================================
  DEFINE_SETTING3(PN, REG48_Part_Information, 0x07, 3);
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

  DEFINE_SETTING3(DEV_REV, REG48_Part_Information, 0x07, 0);
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

 private:
  int _chip_address;

  void _clearRegs();
  bool _flagIsSet(settings_flags_t flagset, settings_flags_t flag);

 public:
  BQ25798();
  BQ25798(uint8_t address);

  bool begin();
  bool readAll();

  int getInt(Setting setting);
  void setInt(Setting setting, int value);
  float getFloat(Setting setting);
  const char* toString(int value, const std::vector<std::string> map);

  bool faultDetected();

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg, uint16_t value, int bitMask = 0xFFFF, int bitShift = 0);

  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);
};

#endif
