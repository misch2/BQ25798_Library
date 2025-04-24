#ifndef BQ25798_H
#define BQ25798_H

#include "Arduino.h"

#include <cstdint>
#include <string>
#include <vector>
#include "registers.h"

#define DEFAULT_I2C_ADDRESS 0x6B

class BQ25798 {
 private:
  int _address;
  uint8_t _regs[1 + MAX_REGISTER_NUMBER] = {};
  void clearRegs();
  bool writeReg8ToI2C(int reg);
  bool writeReg16ToI2C(int reg);

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg,
                uint16_t value,
                int bitMask = 0xFFFF,
                int bitShift = 0);

 public:
  BQ25798();
  BQ25798(uint8_t address);

  bool begin();
  bool readAll();
  const int registersCount = 1 + MAX_REGISTER_NUMBER - MIN_REGISTER_NUMBER;

  enum class regsize_t : uint8_t {
    SHORT = 8,
    LONG = 16,
  };

  typedef uint8_t regaddr_t;

  typedef struct {
    regaddr_t reg;   // Register address
    regsize_t size;  // 8bit or 16bit
    uint16_t mask;   // Bitmask for the setting
    uint8_t shift;   // Bit shift for the setting
  } Setting;

  typedef const std::vector<std::string> strings_vector_t;

  int getInt(Setting setting);
  void setInt(Setting setting, int value);
  const char* toString(int value, const std::vector<std::string> map);

  // ===================== registers =====================
  // 0
  Setting VSYSMIN = {REG00_Minimal_System_Voltage, regsize_t::SHORT, 0x3F, 0};

  // 1-2
  Setting VREG = {REG01_Charge_Voltage_Limit, regsize_t::LONG, 0x7FF, 0};

  // 3-4
  Setting ICHG = {REG03_Charge_Current_Limit, regsize_t::LONG, 0x1FF, 0};

  // 5
  Setting VINDPM = {REG05_Input_Voltage_Limit, regsize_t::SHORT, 0xFF, 0};

  // 6-7
  Setting IINDPM = {REG06_Input_Current_Limit, regsize_t::LONG, 0x1FF, 0};

  // 8
  Setting IPRECHG = {REG08_Precharge_Control, regsize_t::SHORT, 0x3F, 0};
  Setting VBAT_LOWV = {REG08_Precharge_Control, regsize_t::SHORT, 0x3, 6};
  enum class vbat_lowv_t : uint8_t {
    PCT_15 = 0,
    PCT_62 = 1,
    PCT_67 = 2,
    PCT_71 = 3
  };
  strings_vector_t VBAT_LOWV_strings = {{"15%(VREG)"},
                                        {"62.2%(VREG)"},
                                        {"66.7%(VREG)"},
                                        {"71.4%(VREG)"}};

  // 9
  Setting ITERM = {REG09_Termination_Control, regsize_t::SHORT, 0x1F, 0};
  Setting STOP_WD_CHG = {REG09_Termination_Control, regsize_t::SHORT, 0x01, 5};
  Setting REG_RST = {REG09_Termination_Control, regsize_t::SHORT, 0x01, 6};

  // 10
  Setting VRECHG = {REG0A_Recharge_Control, regsize_t::SHORT, 0x0F, 0};
  Setting TRECHG = {REG0A_Recharge_Control, regsize_t::SHORT, 0x03, 4};
  Setting CELL = {REG0A_Recharge_Control, regsize_t::SHORT, 0x03, 6};
  enum class trechg_t : uint8_t {
    TRECHG_64MS = 0,
    TRECHG_256MS = 1,
    TRECHG_1024MS = 2,
    TRECHG_2048MS = 3
  };
  strings_vector_t TRECHG_strings = {{"64 ms"},
                                     {"256 ms"},
                                     {"1024 ms"},
                                     {"2048 ms"}};
  enum class cell_t : uint8_t {
    CELL_1S = 0,
    CELL_2S = 1,
    CELL_3S = 2,
    CELL_4S = 3
  };
  strings_vector_t CELL_strings = {{"1S"}, {"2S"}, {"3S"}, {"4S"}};

  // 11-12
  Setting VOTG = {REG0B_VOTG_regulation, regsize_t::LONG, 0x7FF, 0};

  // 13
  Setting IOTG = {REG0D_IOTG_regulation, regsize_t::SHORT, 0x7F, 0};
  Setting PRECHG_TMR = {REG0D_IOTG_regulation, regsize_t::SHORT, 0x01, 7};
  enum class precgh_tmr_t {
    PRECHG_TMR_2HRS = 0,
    PRECHG_TMR_30MIN = 1,
  };
  strings_vector_t PRECHG_TMR_strings = {{"2 hours"}, {"30 minutes"}};

  // 14
  Setting TMR2X_EN = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 0};
  Setting CHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x03, 1};
  Setting EN_CHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 3};
  Setting EN_PRECHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 4};
  Setting EN_TRICHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 5};
  Setting TOPOFF_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x03, 6};
  enum class chg_tmr_t : uint8_t {
    CHG_TMR_5HRS = 0,
    CHG_TMR_8HRS = 1,
    CHG_TMR_12HRS = 2,
    CHG_TMR_24HRS = 3
  };
  strings_vector_t CHG_TMR_strings = {{"5 hours"},
                                      {"8 hours"},
                                      {"12 hours"},
                                      {"24 hours"}};
  enum class tophff_tmr_t : uint8_t {
    TOPOFF_TMR_DISABLED = 0,
    TOPOFF_TMR_15MIN = 1,
    TOPOFF_TMR_30MIN = 2,
    TOPOFF_TMR_45MIN = 3
  };
  strings_vector_t TOPOFF_TMR_strings = {{"Disabled"},
                                         {"15 minutes"},
                                         {"30 minutes"},
                                         {"45 minutes"}};

  // 15
  Setting EN_BACKUP = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 0};
  Setting EN_TERM = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 1};
  Setting EN_HIZ = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 2};
  Setting FORCE_ICO = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 3};
  Setting EN_ICO = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 4};
  Setting EN_CHG = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 5};
  Setting FORCE_IBATDIS = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 6};
  Setting EN_AUTO_IBATDIS = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01,
                             7};

  // 16
  Setting WATCHDOG = {REG10_Charger_Control_1, regsize_t::SHORT, 0x07, 0};
  Setting WD_RST = {REG10_Charger_Control_1, regsize_t::SHORT, 0x01, 3};
  Setting VAC_OVP = {REG10_Charger_Control_1, regsize_t::SHORT, 0x03, 4};
  Setting VBUS_BACKUP = {REG10_Charger_Control_1, regsize_t::SHORT, 0x03, 6};
  enum class vbus_backup_t : uint8_t {
    PCT_VBUS_BACKUP_40 = 0,
    PCT_VBUS_BACKUP_60 = 1,
    PCT_VBUS_BACKUP_80 = 2,
    PCT_VBUS_BACKUP_100 = 3
  };
  strings_vector_t VBUS_BACKUP_strings = {{"40%"}, {"60%"}, {"80%"}, {"100%"}};
  enum class vac_ovp_t : uint8_t {
    VAC_OVP_26V = 0,
    VAC_OVP_22V = 1,
    VAC_OVP_12V = 2,
    VAC_OVP_7V = 3
  };
  strings_vector_t VAC_OVP_strings = {{"26V"}, {"22V"}, {"12V"}, {"7V"}};
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
  strings_vector_t WATCHDOG_strings = {{"Disabled"}, {"0.5s"}, {"1s"},
                                       {"2s"},       {"20s"},  {"40s"},
                                       {"80s"},      {"160s"}};

  // 17
  Setting SDRV_DLY = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 0};
  Setting SDRV_CTRL = {REG11_Charger_Control_2, regsize_t::SHORT, 0x03, 1};
  Setting HVDCP_EN = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 3};
  Setting EN_9V = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 4};
  Setting EN_12V = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 5};
  Setting AUTO_INDET_EN = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 6};
  Setting FORCE_INDET = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 7};
  enum class sdrv_ctrl_t : uint8_t {
    SDRV_CTRL_IDLE = 0,
    SDRV_CTRL_SHUTDOWN = 1,
    SDRV_CTRL_SHIP = 2,
    SDRV_CTRL_SYS_PWR_RST = 3
  };
  strings_vector_t SDRV_CTRL_strings = {{"Idle"},
                                        {"Shutdown"},
                                        {"Ship"},
                                        {"System Power Reset"}};
  enum class sdrv_dly_t : uint8_t { SDRV_DLY_10S = 0, SDRV_DLY_0S = 1 };
  strings_vector_t SDRV_DLY_strings = {{"10s"}, {"0s"}};

  // 18 - 45 FIXME TODO

  // FIXME REG0E_Timer_Control
  // FIXME REG0F_Charger_Control_0
  // FIXME REG10_Charger_Control_1
  // FIXME REG11_Charger_Control_2
  // FIXME REG12_Charger_Control_3
  // FIXME REG13_Charger_Control_4
  // FIXME REG14_Charger_Control_5
  // FIXME REG15_MPPT_Control
  // FIXME REG16_Temperature_Control
  // FIXME REG17_NTC_Control_0
  // FIXME REG18_NTC_Control_1
  // FIXME REG19_ICO_Current_Limit
  // FIXME REG1B_Charger_Status_0
  // FIXME REG1C_Charger_Status_1
  // FIXME REG1D_Charger_Status_2
  // FIXME REG1E_Charger_Status_3
  // FIXME REG1F_Charger_Status_4
  // FIXME REG20_FAULT_Status_0
  // FIXME REG21_FAULT_Status_1
  // FIXME REG22_Charger_Flag_0
  // FIXME REG23_Charger_Flag_1
  // FIXME REG24_Charger_Flag_2
  // FIXME REG25_Charger_Flag_3
  // FIXME REG26_FAULT_Flag_0
  // FIXME REG27_FAULT_Flag_1
  // FIXME REG28_Charger_Mask_0
  // FIXME REG29_Charger_Mask_1
  // FIXME REG2A_Charger_Mask_2
  // FIXME REG2B_Charger_Mask_3
  // FIXME REG2C_FAULT_Mask_0
  // FIXME REG2D_FAULT_Mask_1

  // FIXME TODO:
  // 46
  Setting ADC_AVG_INIT = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 2};
  Setting ADC_AVG = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 3};
  Setting ADC_SAMPLE = {REG2E_ADC_Control, regsize_t::SHORT, 0x03, 4};
  Setting ADC_RATE = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 6};
  Setting ADC_ENABLE = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 7};
  enum class adc_rate_t : uint8_t {
    ADC_RATE_CONTINUOUS = 0,
    ADC_RATE_ONESHOT = 1
  };
  strings_vector_t ADC_RATE_strings = {{"Continuous"}, {"One-shot"}};
  enum class adc_sample_t : uint8_t {
    ADC_SAMPLE_15BIT = 0,
    ADC_SAMPLE_14BIT = 1,
    ADC_SAMPLE_13BIT = 2,
    ADC_SAMPLE_12BIT = 3
  };
  strings_vector_t ADC_SAMPLE_strings = {{"15-bit"},
                                         {"14-bit"},
                                         {"13-bit"},
                                         {"12-bit"}};

  // 47
  Setting TDIE_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          1};
  Setting TS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                        2};
  Setting VSYS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          3};
  Setting VBAT_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          4};
  Setting VBUS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          5};
  Setting IBAT_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          6};
  Setting IBUS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01,
                          7};

  // 48
  Setting VAC1_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01,
                          4};
  Setting VAC2_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01,
                          5};
  Setting DMINUS_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT,
                            0x01, 6};
  Setting DPLUS_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01,
                           7};

  // 49-50
  Setting IBUS_ADC = {REG31_IBUS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 51-52
  Setting IBAT_ADC = {REG33_IBAT_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 53-54
  Setting VBUS_ADC = {REG35_VBUS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 55-56
  Setting VAC1_ADC = {REG37_VAC1_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 57-58
  Setting VAC2_ADC = {REG39_VAC2_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 59-60
  Setting VBAT_ADC = {REG3B_VBAT_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 61-62
  Setting VSYS_ADC = {REG3D_VSYS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 63-64
  Setting TS_ADC = {REG3F_TS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 65-66
  Setting TDIE_ADC = {REG41_TDIE_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 67-68
  Setting DPLUS_ADC = {REG43_DPLUS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 69-70
  Setting DMINUS_ADC = {REG45_DMINUS_ADC, regsize_t::LONG, 0xFFFF, 0};

  // 71
  Setting DMINUS_DAC = {REG47_DPDM_Driver, regsize_t::SHORT, 0x07, 2};
  Setting DPLUS_DAC = {REG47_DPDM_Driver, regsize_t::SHORT, 0x07, 5};
  enum class dplus_dac_t : uint8_t {
    HIZ = 0,
    VOUT_0 = 1,
    VOUT_0_6 = 2,
    VOUT_1_2 = 3,
    VOUT_2_0 = 4,
    VOUT_2_7 = 5,
    VOUT_3_3 = 6,
    DPLUS_DMINUS_SHORT = 7
  };
  strings_vector_t DPLUS_DAC_strings = {{"HIZ"},  {"0V"},         {"0.6V"},
                                        {"1.2V"}, {"2.0V"},       {"2.7V"},
                                        {"3.3V"}, {"D+/D- Short"}};
  enum class dminus_dac_t : uint8_t {
    HIZ = 0,
    VOUT_0 = 1,
    VOUT_0_6 = 2,
    VOUT_1_2 = 3,
    VOUT_2_0 = 4,
    VOUT_2_7 = 5,
    VOUT_3_3 = 6,
    RESERVED = 7
  };
  strings_vector_t DMINUS_DAC_strings = {{"HIZ"},  {"0V"},      {"0.6V"},
                                         {"1.2V"}, {"2.0V"},    {"2.7V"},
                                         {"3.3V"}, {"Reserved"}};

  // 72
  Setting DEV_REV = {REG48_Part_Information, regsize_t::SHORT, 0x07, 0};
  Setting PN = {REG48_Part_Information, regsize_t::SHORT, 0x07, 3};
  enum class pn_t : uint8_t {
    RESERVED0 = 0x0,
    RESERVED1 = 0x1,
    RESERVED2 = 0x2,
    BQ25798 = 0x3,
    RESERVED4 = 0x4,
    RESERVED5 = 0x5,
    RESERVED6 = 0x6,
    RESERVED7 = 0x7,
  };
  strings_vector_t PN_strings = {{"?"}, {"?"}, {"?"}, {"BQ25798"},
                                 {"?"}, {"?"}, {"?"}, {"?"}};
  enum class dev_rev_t : uint8_t {
    RESERVED0 = 0x0,
    BQ25798 = 0x1,
    RESERVED2 = 0x2,
    RESERVED3 = 0x3,
    RESERVED4 = 0x4,
    RESERVED5 = 0x5,
    RESERVED6 = 0x6,
    RESERVED7 = 0x7,
  };
  strings_vector_t DEV_REV_strings = {{"?"}, {"BQ25798"}, {"?"}, {"?"},
                                      {"?"}, {"?"},       {"?"}, {"?"}};
};

#endif
