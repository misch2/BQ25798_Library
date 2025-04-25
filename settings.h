enum class regsize_t : uint8_t {
  SHORT = 8,
  LONG = 16,
};

enum class settings_flags_t : uint8_t {
  NONE = 0,
  IS_2COMPLEMENT = 0x80,
};

typedef struct {
  uint8_t reg;     // Register address
  regsize_t size;  // 8bit or 16bit
  uint16_t mask;   // Bitmask for the setting
  uint8_t shift;   // Bit shift for the setting
  // optional:
  float range_low;
  float range_high;
  float fixed_offset;   // Fixed offset for the setting (e.g., for voltage settings)
  float bit_step_size;  // Bit step size for the setting (e.g., for current settings)
  // adjusted value = raw_value * bit_step_size + fixed_offset
  settings_flags_t flags;
} Setting;

typedef const std::vector<std::string> strings_vector_t;

// ===================================
// REG00_Minimal_System_Voltage
// ===================================
Setting VSYSMIN = {REG00_Minimal_System_Voltage, regsize_t::SHORT, 0x3F, 0, 2500, 16000, 2500, 250};

// ===================================
// REG01_Charge_Voltage_Limit (wide)
// ===================================
Setting VREG = {REG01_Charge_Voltage_Limit, regsize_t::LONG, 0x7FF, 0, 3000, 18800, 0, 10};

// ===================================
// REG03_Charge_Current_Limit (wide)
// ===================================
Setting ICHG = {REG03_Charge_Current_Limit, regsize_t::LONG, 0x1FF, 0, 50, 5000, 0, 10};

// ===================================
// REG05_Input_Voltage_Limit
// ===================================
Setting VINDPM = {REG05_Input_Voltage_Limit, regsize_t::SHORT, 0xFF, 0, 3600, 22000, 0, 100};

// ===================================
// REG06_Input_Current_Limit (wide)
// ===================================
Setting IINDPM = {REG06_Input_Current_Limit, regsize_t::LONG, 0x1FF, 0, 100, 3300, 0, 10};

// ===================================
// REG08_Precharge_Control
// ===================================
Setting VBAT_LOWV = {REG08_Precharge_Control, regsize_t::SHORT, 0x3, 6};
enum class vbat_lowv_t : uint8_t { PCT_15 = 0, PCT_62 = 1, PCT_67 = 2, PCT_71 = 3 };
strings_vector_t VBAT_LOWV_strings = {{"15%(VREG)"}, {"62.2%(VREG)"}, {"66.7%(VREG)"}, {"71.4%(VREG)"}};

Setting IPRECHG = {REG08_Precharge_Control, regsize_t::SHORT, 0x3F, 0, 40, 2000, 0, 40};

// ===================================
// REG09_Termination_Control
// ===================================
Setting REG_RST = {REG09_Termination_Control, regsize_t::SHORT, 0x01, 6};

Setting STOP_WD_CHG = {REG09_Termination_Control, regsize_t::SHORT, 0x01, 5};

Setting ITERM = {REG09_Termination_Control, regsize_t::SHORT, 0x1F, 0, 40, 1000, 0, 40};

// ===================================
// REG0A_Recharge_Control
// ===================================
Setting CELL = {REG0A_Recharge_Control, regsize_t::SHORT, 0x03, 6};
enum class cell_t : uint8_t { CELL_1S = 0, CELL_2S = 1, CELL_3S = 2, CELL_4S = 3 };
strings_vector_t CELL_strings = {{"1S"}, {"2S"}, {"3S"}, {"4S"}};

Setting TRECHG = {REG0A_Recharge_Control, regsize_t::SHORT, 0x03, 4};
enum class trechg_t : uint8_t { TRECHG_64MS = 0, TRECHG_256MS = 1, TRECHG_1024MS = 2, TRECHG_2048MS = 3 };
strings_vector_t TRECHG_strings = {{"64 ms"}, {"256 ms"}, {"1024 ms"}, {"2048 ms"}};

Setting VRECHG = {REG0A_Recharge_Control, regsize_t::SHORT, 0x0F, 0, 50, 800, 50, 50};

// ===================================
// REG0B_VOTG_regulation (wide)
// ===================================
Setting VOTG = {REG0B_VOTG_regulation, regsize_t::LONG, 0x7FF, 0, 2800, 22000, 2800, 10};

// ===================================
// REG0D_IOTG_regulation
// ===================================
Setting PRECHG_TMR = {REG0D_IOTG_regulation, regsize_t::SHORT, 0x01, 7};
enum class precgh_tmr_t {
  PRECHG_TMR_2HRS = 0,
  PRECHG_TMR_30MIN = 1,
};
strings_vector_t PRECHG_TMR_strings = {{"2 hours"}, {"30 minutes"}};

Setting IOTG = {REG0D_IOTG_regulation, regsize_t::SHORT, 0x7F, 0, 160, 3360, 0, 40};

// ===================================
// REG0E_Timer_Control
// ===================================
Setting TOPOFF_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x03, 6};
enum class tophff_tmr_t : uint8_t { TOPOFF_TMR_DISABLED = 0, TOPOFF_TMR_15MIN = 1, TOPOFF_TMR_30MIN = 2, TOPOFF_TMR_45MIN = 3 };
strings_vector_t TOPOFF_TMR_strings = {{"Disabled"}, {"15 minutes"}, {"30 minutes"}, {"45 minutes"}};

Setting EN_TRICHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 5};

Setting EN_PRECHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 4};

Setting EN_CHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 3};

Setting CHG_TMR = {REG0E_Timer_Control, regsize_t::SHORT, 0x03, 1};
enum class chg_tmr_t : uint8_t { CHG_TMR_5HRS = 0, CHG_TMR_8HRS = 1, CHG_TMR_12HRS = 2, CHG_TMR_24HRS = 3 };
strings_vector_t CHG_TMR_strings = {{"5 hours"}, {"8 hours"}, {"12 hours"}, {"24 hours"}};

Setting TMR2X_EN = {REG0E_Timer_Control, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG0F_Charger_Control_0
// ===================================
Setting EN_AUTO_IBATDIS = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 7};

Setting FORCE_IBATDIS = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 6};

Setting EN_CHG = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 5};

Setting EN_ICO = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 4};

Setting FORCE_ICO = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 3};

Setting EN_HIZ = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 2};

Setting EN_TERM = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 1};

Setting EN_BACKUP = {REG0F_Charger_Control_0, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG10_Charger_Control_1
// ===================================
Setting VBUS_BACKUP = {REG10_Charger_Control_1, regsize_t::SHORT, 0x03, 6};
enum class vbus_backup_t : uint8_t { PCT_VBUS_BACKUP_40 = 0, PCT_VBUS_BACKUP_60 = 1, PCT_VBUS_BACKUP_80 = 2, PCT_VBUS_BACKUP_100 = 3 };
strings_vector_t VBUS_BACKUP_strings = {{"<40%(VINDPM)"}, {"<60%(VINDPM)"}, {"<80%(VINDPM)"}, {"<100%(VINDPM)"}};

Setting VAC_OVP = {REG10_Charger_Control_1, regsize_t::SHORT, 0x03, 4};
enum class vac_ovp_t : uint8_t { VAC_OVP_26V = 0, VAC_OVP_22V = 1, VAC_OVP_12V = 2, VAC_OVP_7V = 3 };
strings_vector_t VAC_OVP_strings = {{">26V"}, {">22V"}, {">12V"}, {">7V"}};

Setting WD_RST = {REG10_Charger_Control_1, regsize_t::SHORT, 0x01, 3};

Setting WATCHDOG = {REG10_Charger_Control_1, regsize_t::SHORT, 0x07, 0};
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
strings_vector_t WATCHDOG_strings = {{"Disabled"}, {"0.5s"}, {"1s"}, {"2s"}, {"20s"}, {"40s"}, {"80s"}, {"160s"}};

// ===================================
// REG11_Charger_Control_2
// ===================================
Setting FORCE_INDET = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 7};

Setting AUTO_INDET_EN = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 6};

Setting EN_12V = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 5};

Setting EN_9V = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 4};

Setting HVDCP_EN = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 3};

Setting SDRV_CTRL = {REG11_Charger_Control_2, regsize_t::SHORT, 0x03, 1};
enum class sdrv_ctrl_t : uint8_t { SDRV_CTRL_IDLE = 0, SDRV_CTRL_SHUTDOWN = 1, SDRV_CTRL_SHIP = 2, SDRV_CTRL_SYS_PWR_RST = 3 };
strings_vector_t SDRV_CTRL_strings = {{"Idle"}, {"Shutdown"}, {"Ship"}, {"System Power Reset"}};

Setting SDRV_DLY = {REG11_Charger_Control_2, regsize_t::SHORT, 0x01, 0};
enum class sdrv_dly_t : uint8_t { SDRV_DLY_10S = 0, SDRV_DLY_0S = 1 };
strings_vector_t SDRV_DLY_strings = {{"10s"}, {"0s"}};

// ===================================
// REG12_Charger_Control_3
// ===================================
Setting DIS_ACDRV = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 7};

Setting EN_OTG = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 6};

Setting PFM_OTG_DIS = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 5};

Setting PFM_FWD_DIS = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 4};

Setting WKUP_DLY = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 3};
enum class wkup_dly_t : uint8_t { WKUP_DLY_1S = 0, WKUP_DLY_15MS = 1 };
strings_vector_t WKUP_DLY_strings = {{"1s"}, {"15ms"}};

Setting DIS_LDO = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 2};

Setting DIS_OTG_OOA = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 1};

Setting DIS_FWD_OOA = {REG12_Charger_Control_3, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG13_Charger_Control_4
// ===================================
Setting EN_ACDRV2 = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 7};

Setting EN_ACDRV1 = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 6};

Setting PWM_FREQ = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 5};
enum class pwm_freq_t : uint8_t { PWM_FREQ_1_5MHZ = 0, PWM_FREQ_750KHZ = 1 };
strings_vector_t PWM_FREQ_strings = {{"1.5 MHz"}, {"750 kHz"}};

Setting DIS_STAT = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 4};

Setting DIS_VSYS_SHORT = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 3};

Setting DIS_VOTG_UVP = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 2};

Setting FORCE_VINDPM_DET = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 1};

Setting EN_IBUS_OCP = {REG13_Charger_Control_4, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG14_Charger_Control_5
// ===================================
Setting SFET_PRESENT = {REG14_Charger_Control_5, regsize_t::SHORT, 0x01, 7};

Setting EN_IBAT = {REG14_Charger_Control_5, regsize_t::SHORT, 0x01, 5};

Setting IBAT_REG = {REG14_Charger_Control_5, regsize_t::SHORT, 0x03, 3};
enum class ibat_reg_t : uint8_t { IBAT_REG_3A = 0, IBAT_REG_4A = 1, IBAT_REG_5A = 2, IBAT_REG_DISABLE = 3 };
strings_vector_t IBAT_REG_strings = {{"3A"}, {"4A"}, {"5A"}, {"Disabled"}};

Setting EN_IINDPM = {REG14_Charger_Control_5, regsize_t::SHORT, 0x01, 2};

Setting EN_EXTILIM = {REG14_Charger_Control_5, regsize_t::SHORT, 0x01, 1};

Setting EN_BATOC = {REG14_Charger_Control_5, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG15_MPPT_Control
// ===================================
Setting VOC_PCT = {REG15_MPPT_Control, regsize_t::SHORT, 0x07, 5};
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
strings_vector_t VOC_PCT_strings = {{"0.5625"}, {"0.625"}, {"0.6875"}, {"0.75"}, {"0.8125"}, {"0.875"}, {"0.9375"}, {"1"}};

Setting VOC_DLY = {REG15_MPPT_Control, regsize_t::SHORT, 0x03, 4};
enum class voc_dly_t : uint8_t { VOC_DLY_50MS = 0, VOC_DLY_300MS = 1, VOC_DLY_2S = 2, VOC_DLY_5S = 3 };
strings_vector_t VOC_DLY_strings = {{"50ms"}, {"300ms"}, {"2s"}, {"5s"}};

Setting VOC_RATE = {REG15_MPPT_Control, regsize_t::SHORT, 0x03, 2};
enum class voc_rate_t : uint8_t { VOC_RATE_30S = 0, VOC_RATE_2MIN = 1, VOC_RATE_10MIN = 2, VOC_RATE_30MIN = 3 };
strings_vector_t VOC_RATE_strings = {{"30s"}, {"2min"}, {"10min"}, {"30min"}};

Setting EN_MPPT = {REG15_MPPT_Control, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG16_Temperature_Control
// ===================================
Setting TREG = {REG16_Temperature_Control, regsize_t::SHORT, 0x03, 6};
enum class treg_t : uint8_t { TREG_60 = 0, TREG_80 = 1, TREG_100 = 2, TREG_120 = 3 };
strings_vector_t TREG_strings = {{"60°C"}, {"80°C"}, {"100°C"}, {"120°C"}};

Setting TSHUT = {REG16_Temperature_Control, regsize_t::SHORT, 0x03, 4};
enum class tshut_t : uint8_t { TSHUT_150 = 0, TSHUT_130 = 1, TSHUT_120 = 2, TSHUT_85 = 3 };
strings_vector_t TSHUT_strings = {{"150°C"}, {"130°C"}, {"120°C"}, {"85°C"}};

Setting VBUS_PD_EN = {REG16_Temperature_Control, regsize_t::SHORT, 0x01, 3};

Setting VAC1_PD_EN = {REG16_Temperature_Control, regsize_t::SHORT, 0x01, 2};

Setting VAC2_PD_EN = {REG16_Temperature_Control, regsize_t::SHORT, 0x01, 1};

Setting BKUP_ACFET1_ON = {REG16_Temperature_Control, regsize_t::SHORT, 0x01, 0};
enum class bkup_acfet1_on_t : uint8_t { IDLE = 0, TURN_ON = 1 };
strings_vector_t BKUP_ACFET1_ON_strings = {{"Idle"}, {"Turn on ACFET1 in backup mode"}};

// ==================================
// REG17_NTC_Control_0
// ==================================
Setting JEITA_VSET = {REG17_NTC_Control_0, regsize_t::SHORT, 0x07, 5};
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
strings_vector_t JEITA_VSET_strings = {{"Charge Suspend"},         {"Set VREG to VREG-800mV"}, {"Set VREG to VREG-600mV"}, {"Set VREG to VREG-400mV (default)"},
                                       {"Set VREG to VREG-300mV"}, {"Set VREG to VREG-200mV"}, {"Set VREG to VREG-100mV"}, {"VREG unchanged"}};

Setting JEITA_ISETH = {REG17_NTC_Control_0, regsize_t::SHORT, 0x03, 3};
enum class jeita_iseth_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
strings_vector_t JEITA_ISETH_strings = {{"Charge Suspend"}, {"Set ICHG to 20%* ICHG"}, {"Set ICHG to 40%* ICHG"}, {"ICHG unchanged"}};

Setting JEITA_ISETC = {REG17_NTC_Control_0, regsize_t::SHORT, 0x03, 1};
enum class jeita_isetc_t : uint8_t { CHARGE_SUSPEND = 0, SET_ICHG_TO_20 = 1, SET_ICHG_TO_40 = 2, ICHG_UNCHANGED = 3 };
strings_vector_t JEITA_ISETC_strings = {{"Charge Suspend"}, {"Set ICHG to 20%* ICHG (default)"}, {"Set ICHG to 40%* ICHG"}, {"ICHG unchanged"}};

// ==================================
// REG18_NTC_Control_1
// ==================================
Setting TS_COOL = {REG18_NTC_Control_1, regsize_t::SHORT, 0x03, 6};
enum class ts_cool_t : uint8_t { TS_5 = 0, TS_10 = 1, TS_15 = 2, TS_20 = 3 };
strings_vector_t TS_COOL_strings = {{"5°C"}, {"10°C (default)"}, {"15°C"}, {"20°C"}};

Setting TS_WARM = {REG18_NTC_Control_1, regsize_t::SHORT, 0x03, 4};
enum class ts_warm_t : uint8_t { TS_40 = 0, TS_45 = 1, TS_50 = 2, TS_55 = 3 };
strings_vector_t TS_WARM_strings = {{"40°C"}, {"45°C (default)"}, {"50°C"}, {"55°C"}};

Setting BHOT = {REG18_NTC_Control_1, regsize_t::SHORT, 0x03, 2};
enum class bhot_t : uint8_t { TS_55 = 0, TS_60 = 1, TS_65 = 2, DISABLE = 3 };
strings_vector_t BHOT_strings = {{"55°C"}, {"60°C (default)"}, {"65°C"}, {"Disabled"}};

Setting BCOLD = {REG18_NTC_Control_1, regsize_t::SHORT, 0x01, 1};
enum class bcold_t : uint8_t { MINUS_10 = 0, MINUS_20 = 1 };
strings_vector_t BCOLD_strings = {{"-10°C (default)"}, {"-20°C"}};

Setting TS_IGNORE = {REG18_NTC_Control_1, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG19_ICO_Current_Limit (wide)
// ==================================
Setting ICO_ILIM = {REG19_ICO_Current_Limit, regsize_t::LONG, 0x1FF, 0, 100, 3300, 0, 10};

// ==================================
// REG1B_Charger_Status_0
// ==================================
Setting IINDPM_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 7};
enum class iindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
strings_vector_t IINDPM_STAT_strings = {{"Normal"}, {"In IINDPM regulation or IOTG regulation"}};

Setting VINDPM_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 6};
enum class vindpm_stat_t : uint8_t { NORMAL = 0, REGULATION = 1 };
strings_vector_t VINDPM_STAT_strings = {{"Normal"}, {"In VINDPM regulation or VOTG regulation"}};

Setting WD_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 5};
enum class wd_stat_t : uint8_t { NORMAL = 0, EXPIRED = 1 };
strings_vector_t WD_STAT_strings = {{"Normal"}, {"Watchdog timer expired"}};

Setting PG_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 3};
enum class pg_stat_t : uint8_t { BAD = 0, GOOD = 1 };
strings_vector_t PG_STAT_strings = {{"Not in power good status"}, {"Power good"}};

Setting AC2_PRESENT_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 2};
enum class ac2_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
strings_vector_t AC2_PRESENT_STAT_strings = {{"VAC2 NOT present"}, {"VAC2 present (above present threshold)"}};

Setting AC1_PRESENT_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 1};
enum class ac1_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
strings_vector_t AC1_PRESENT_STAT_strings = {{"VAC1 NOT present"}, {"VAC1 present (above present threshold)"}};

Setting VBUS_PRESENT_STAT = {REG1B_Charger_Status_0, regsize_t::SHORT, 0x01, 0};
enum class vbus_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
strings_vector_t VBUS_PRESENT_STAT_strings = {{"VBUS NOT present"}, {"VBUS present (above present threshold)"}};

// ==================================
// REG1C_Charger_Status_1
// ==================================
Setting CHG_STAT = {REG1C_Charger_Status_1, regsize_t::SHORT, 0x7, 5};
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
strings_vector_t CHG_STAT_strings = {{"Not Charging"},
                                     {"Trickle Charge"},
                                     {"Pre-charge"},
                                     {"Fast charge (CC mode)"},
                                     {"Taper Charge (CV mode)"},
                                     {"Reserved"},
                                     {"Top-off Timer Active Charging"},
                                     {"Charge Termination Done"}};

Setting VBUS_STAT = {REG1C_Charger_Status_1, regsize_t::SHORT, 0x0F, 1};
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
strings_vector_t VBUS_STAT_strings = {{"No Input or BHOT or BCOLD in OTG mode"},
                                      {"USB SDP (500mA)"},
                                      {"USB CDP (1.5A)"},
                                      {"USB DCP (3.25A)"},
                                      {"Adjustable High Voltage DCP (HVDCP) (1.5A)"},
                                      {"Unknown adaptor (3A)"},
                                      {"Non-Standard Adapter (1A/2A/2.1A/2.4A)"},
                                      {"In OTG mode"},
                                      {"Not qualified adaptor"},
                                      {"Reserved"},
                                      {"Reserved"},
                                      {"Device directly powered from VBUS"},
                                      {"Backup Mode"},
                                      {"Reserved"},
                                      {"Reserved"},
                                      {"Reserved"}};

Setting BC12_DONE_STAT = {REG1C_Charger_Status_1, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG1D_Charger_Status_2
// ==================================
Setting ICO_STAT = {REG1D_Charger_Status_2, regsize_t::SHORT, 0x03, 6};
enum class ico_stat_t : uint8_t { ICO_DISABLED = 0, ICO_IN_PROGRESS = 1, ICO_MAX_CURRENT_DETECTED = 2, ICO_RESERVED = 3 };
strings_vector_t ICO_STAT_strings = {{"ICO disabled"}, {"ICO optimization in progress"}, {"Maximum input current detected"}, {"Reserved"}};

Setting TREG_STAT = {REG1D_Charger_Status_2, regsize_t::SHORT, 0x01, 5};
enum class treg_stat_t : uint8_t { NORMAL = 0, THERMAL_REGULATION = 1 };
strings_vector_t TREG_STAT_strings = {{"Normal"}, {"Device in thermal regulation"}};

Setting DPDM_STAT = {REG1D_Charger_Status_2, regsize_t::SHORT, 0x01, 4};
enum class dpdm_stat_t : uint8_t { NOT_STARTED = 0, IN_PROGRESS = 1 };
strings_vector_t DPDM_STAT_strings = {{"D+/D- detection NOT started yet or done"}, {"D+/D- detection in progress"}};

Setting VBAT_PRESENT_STAT = {REG1D_Charger_Status_2, regsize_t::SHORT, 0x01, 0};
enum class vbat_present_stat_t : uint8_t { NOT_PRESENT = 0, PRESENT = 1 };
strings_vector_t VBAT_PRESENT_STAT_strings = {{"VBAT NOT present"}, {"VBAT present"}};

// ==================================
// REG1E_Charger_Status_3
// ==================================
Setting ACRB2_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 7};

Setting ACRB1_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 6};

Setting ADC_DONE_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 5};

Setting VSYS_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 4};
enum class vsys_stat_t : uint8_t { NOT_IN_VSYSMIN_REGULATION = 0, IN_VSYSMIN_REGULATION = 1 };
strings_vector_t VSYS_STAT_strings = {{"Not in VSYSMIN regulation (VBAT > VSYSMIN)"}, {"In VSYSMIN regulation (VBAT < VSYSMIN)"}};

Setting CHG_TMR_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 3};
enum class chg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
strings_vector_t CHG_TMR_STAT_strings = {{"Normal"}, {"Safety timer expired"}};

Setting TRICHG_TMR_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 2};
enum class trichg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
strings_vector_t TRICHG_TMR_STAT_strings = {{"Normal"}, {"Safety timer expired"}};

Setting PRECHG_TMR_STAT = {REG1E_Charger_Status_3, regsize_t::SHORT, 0x01, 1};
enum class prechg_tmr_stat_t : uint8_t { NORMAL = 0, SAFETY_TIMER_EXPIRED = 1 };
strings_vector_t PRECHG_TMR_STAT_strings = {{"Normal"}, {"Safety timer expired"}};

// ==================================
// REG1F_Charger_Status_4
// ==================================
Setting VBATOTG_LOW_STAT = {REG1F_Charger_Status_4, regsize_t::SHORT, 0x01, 4};
enum class vbatotg_low_stat_t : uint8_t { VBATOTG_LOW = 0, VBATOTG_OK = 1 };
strings_vector_t VBATOTG_LOW_STAT_strings = {{"VBAT is too low to enable OTG mode"}, {"VBAT is high enough to enable OTG operation"}};

Setting TS_COLD_STAT = {REG1F_Charger_Status_4, regsize_t::SHORT, 0x01, 3};
enum class ts_cold_stat_t : uint8_t { NOT_COLD = 0, COLD = 1 };
strings_vector_t TS_COLD_STAT_strings = {{"TS NOT in cold range"}, {"TS in cold range"}};

Setting TS_COOL_STAT = {REG1F_Charger_Status_4, regsize_t::SHORT, 0x01, 2};
enum class ts_cool_stat_t : uint8_t { NOT_COOL = 0, COOL = 1 };
strings_vector_t TS_COOL_STAT_strings = {{"TS NOT in cool range"}, {"TS in cool range"}};

Setting TS_WARM_STAT = {REG1F_Charger_Status_4, regsize_t::SHORT, 0x01, 1};
enum class ts_warm_stat_t : uint8_t { NOT_WARM = 0, WARM = 1 };
strings_vector_t TS_WARM_STAT_strings = {{"TS NOT in warm range"}, {"TS in warm range"}};

Setting TS_HOT_STAT = {REG1F_Charger_Status_4, regsize_t::SHORT, 0x01, 0};
enum class ts_hot_stat_t : uint8_t { NOT_HOT = 0, HOT = 1 };
strings_vector_t TS_HOT_STAT_strings = {{"TS NOT in hot range"}, {"TS in hot range"}};

// ==================================
// REG20_FAULT_Status_0
// ==================================
Setting IBAT_REG_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 7};

Setting VBUS_OVP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 6};

Setting VBAT_OVP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 5};

Setting IBUS_OCP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 4};

Setting IBAT_OCP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 3};

Setting CONV_OCP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 2};

Setting VAC2_OVP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 1};

Setting VAC1_OVP_STAT = {REG20_FAULT_Status_0, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG21_FAULT_Status_1
// ==================================
Setting VSYS_SHORT_STAT = {REG21_FAULT_Status_1, regsize_t::SHORT, 0x01, 7};

Setting VSYS_OVP_STAT = {REG21_FAULT_Status_1, regsize_t::SHORT, 0x01, 6};

Setting OTG_OVP_STAT = {REG21_FAULT_Status_1, regsize_t::SHORT, 0x01, 5};

Setting OTG_UVP_STAT = {REG21_FAULT_Status_1, regsize_t::SHORT, 0x01, 4};

Setting TSHUT_STAT = {REG21_FAULT_Status_1, regsize_t::SHORT, 0x01, 2};

// ==================================
// REG22_Charger_Flag_0
// ==================================
Setting IINDPM_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 7};

Setting VINDPM_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 6};

Setting WD_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 5};

Setting POORSRC_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 4};

Setting PG_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 3};

Setting AC2_PRESENT_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 2};

Setting AC1_PRESENT_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 1};

Setting VBUS_PRESENT_FLAG = {REG22_Charger_Flag_0, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG23_Charger_Flag_1
// ===================================
Setting CHG_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 7};

Setting ICO_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 6};

Setting VBUS_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 4};

Setting TREG_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 2};

Setting VBAT_PRESENT_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 1};

Setting BC1_2_DONE_FLAG = {REG23_Charger_Flag_1, regsize_t::SHORT, 0x01, 0};

// ===================================
// REG24_Charger_Flag_2
// ===================================
Setting DPDM_DONE_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 6};

Setting ADC_DONE_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 5};

Setting VSYS_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 4};

Setting CHG_TMR_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 3};

Setting TRICHG_TMR_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 2};

Setting PRECHG_TMR_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 1};

Setting TOPOFF_TMR_FLAG = {REG24_Charger_Flag_2, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG25_Charger_Flag_3
// ==================================
Setting VBATOTG_LOW_FLAG = {REG25_Charger_Flag_3, regsize_t::SHORT, 0x01, 4};

Setting TS_COLD_FLAG = {REG25_Charger_Flag_3, regsize_t::SHORT, 0x01, 3};

Setting TS_COOL_FLAG = {REG25_Charger_Flag_3, regsize_t::SHORT, 0x01, 2};

Setting TS_WARM_FLAG = {REG25_Charger_Flag_3, regsize_t::SHORT, 0x01, 1};

Setting TS_HOT_FLAG = {REG25_Charger_Flag_3, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG26_FAULT_Flag_0
// ==================================
Setting IBAT_REG_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 7};

Setting VBUS_OVP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 6};

Setting VBAT_OVP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 5};

Setting IBUS_OCP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 4};

Setting IBAT_OCP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 3};

Setting CONV_OCP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 2};

Setting VAC2_OVP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 1};

Setting VAC1_OVP_FLAG = {REG26_FAULT_Flag_0, regsize_t::SHORT, 0x01, 0};

// ==================================
// REG27_FAULT_Flag_1
// ==================================
Setting VSYS_SHORT_FLAG = {REG27_FAULT_Flag_1, regsize_t::SHORT, 0x01, 7};

Setting VSYS_OVP_FLAG = {REG27_FAULT_Flag_1, regsize_t::SHORT, 0x01, 6};

Setting OTG_OVP_FLAG = {REG27_FAULT_Flag_1, regsize_t::SHORT, 0x01, 5};

Setting OTG_UVP_FLAG = {REG27_FAULT_Flag_1, regsize_t::SHORT, 0x01, 4};

Setting TSHUT_FLAG = {REG27_FAULT_Flag_1, regsize_t::SHORT, 0x01, 2};

// FIXME REG28_Charger_Mask_0
// FIXME REG29_Charger_Mask_1
// FIXME REG2A_Charger_Mask_2
// FIXME REG2B_Charger_Mask_3
// FIXME REG2C_FAULT_Mask_0
// FIXME REG2D_FAULT_Mask_1

// ==================================
// REG2E_ADC_Control
// ==================================
Setting ADC_EN = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 7};

Setting ADC_RATE = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 6};
enum class adc_rate_t : uint8_t { ADC_RATE_CONTINUOUS = 0, ADC_RATE_ONESHOT = 1 };
strings_vector_t ADC_RATE_strings = {{"Continuous"}, {"One-shot"}};

Setting ADC_SAMPLE = {REG2E_ADC_Control, regsize_t::SHORT, 0x03, 4};
enum class adc_sample_t : uint8_t { ADC_SAMPLE_15BIT = 0, ADC_SAMPLE_14BIT = 1, ADC_SAMPLE_13BIT = 2, ADC_SAMPLE_12BIT = 3 };
strings_vector_t ADC_SAMPLE_strings = {{"15-bit"}, {"14-bit"}, {"13-bit"}, {"12-bit"}};

Setting ADC_AVG = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 3};
enum class adc_avg_t : uint8_t { NO_AVERAGING = 0, RUNNING_AVERAGE = 1 };
strings_vector_t ADC_AVG_strings = {{"No averaging"}, {"Running average"}};

Setting ADC_AVG_INIT = {REG2E_ADC_Control, regsize_t::SHORT, 0x01, 2};

// ==================================
// REG2F_ADC_Function_Disable_0
// ==================================
Setting IBUS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 7};

Setting IBAT_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 6};

Setting VBUS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 5};

Setting VBAT_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 4};

Setting VSYS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 3};

Setting TS_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 2};

Setting TDIE_ADC_DIS = {REG2F_ADC_Function_Disable_0, regsize_t::SHORT, 0x01, 1};

// ==================================
// REG30_ADC_Function_Disable_1
// ==================================
Setting DPLUS_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01, 7};

Setting DMINUS_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01, 6};

Setting VAC2_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01, 5};

Setting VAC1_ADC_DIS = {REG30_ADC_Function_Disable_1, regsize_t::SHORT, 0x01, 4};

// ===================================
// REG31_ADC_Function_Disable_2 (wide)
// ===================================
Setting IBUS_ADC = {REG31_IBUS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 5000, 0, 1, settings_flags_t::IS_2COMPLEMENT};

// ===================================
// REG33_ADC_Function_Disable_3 (wide)
// ===================================
Setting IBAT_ADC = {REG33_IBAT_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 8000, 0, 1, settings_flags_t::IS_2COMPLEMENT};

// ===================================
// REG35_ADC_Function_Disable_4 (wide)
// ===================================
Setting VBUS_ADC = {REG35_VBUS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 30000, 0, 1};

// ===================================
// REG37_ADC_Function_Disable_5 (wide)
// ===================================
Setting VAC1_ADC = {REG37_VAC1_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 30000, 0, 1};

// ===================================
// REG39_ADC_Function_Disable_6 (wide)
// ===================================
Setting VAC2_ADC = {REG39_VAC2_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 30000, 0, 1};

// ===================================
// REG3B_ADC_Function_Disable_7 (wide)
// ===================================
Setting VBAT_ADC = {REG3B_VBAT_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 20000, 0, 1};

// ===================================
// REG3D_ADC_Function_Disable_8 (wide)
// ===================================
Setting VSYS_ADC = {REG3D_VSYS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 24000, 0, 1};

// ===================================
// REG3F_ADC_Function_Disable_9 (wide)
// ===================================
Setting TS_ADC = {REG3F_TS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, -99.9023, 0, 0.0976563};

// ===================================
// REG41_ADC_Function_Disable_A (wide)
// ===================================
Setting TDIE_ADC = {REG41_TDIE_ADC, regsize_t::LONG, 0xFFFF, 0, -40, 150, 0, 0.5, settings_flags_t::IS_2COMPLEMENT};

// ===================================
// REG43_ADC_Function_Disable_B (wide)
// ===================================
Setting DPLUS_ADC = {REG43_DPLUS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 3600, 0, 1};

// ===================================
// REG45_ADC_Function_Disable_C (wide)
// ===================================
Setting DMINUS_ADC = {REG45_DMINUS_ADC, regsize_t::LONG, 0xFFFF, 0, 0, 3600, 0, 1};

// ===================================
// REG47_DPDM_Driver
// ==================================
Setting DPLUS_DAC = {REG47_DPDM_Driver, regsize_t::SHORT, 0x07, 5};
enum class dplus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, DPLUS_DMINUS_SHORT = 7 };
strings_vector_t DPLUS_DAC_strings = {{"HIZ"}, {"0V"}, {"0.6V"}, {"1.2V"}, {"2.0V"}, {"2.7V"}, {"3.3V"}, {"D+/D- Short"}};

Setting DMINUS_DAC = {REG47_DPDM_Driver, regsize_t::SHORT, 0x07, 2};
enum class dminus_dac_t : uint8_t { HIZ = 0, VOUT_0 = 1, VOUT_0_6 = 2, VOUT_1_2 = 3, VOUT_2_0 = 4, VOUT_2_7 = 5, VOUT_3_3 = 6, RESERVED = 7 };
strings_vector_t DMINUS_DAC_strings = {{"HIZ"}, {"0V"}, {"0.6V"}, {"1.2V"}, {"2.0V"}, {"2.7V"}, {"3.3V"}, {"Reserved"}};

// ==================================
// REG48_Part_Information
// ==================================
Setting PN = {REG48_Part_Information, regsize_t::SHORT, 0x07, 3};
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
strings_vector_t PN_strings = {{"?"}, {"?"}, {"?"}, {"BQ25798"}, {"?"}, {"?"}, {"?"}, {"?"}};

Setting DEV_REV = {REG48_Part_Information, regsize_t::SHORT, 0x07, 0};
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
strings_vector_t DEV_REV_strings = {{"?"}, {"BQ25798"}, {"?"}, {"?"}, {"?"}, {"?"}, {"?"}, {"?"}};