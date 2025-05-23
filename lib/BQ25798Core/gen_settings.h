

// ================================
// REG00_Minimal_System_Voltage
// ================================


Setting VSYSMIN = { REG00_Minimal_System_Voltage, false, "VSYSMIN", "mV", settings_type_t::INT, 6, 0, false, 2500, 16000, 2500, 250, settings_flags_t::NONE };
/// Returns VSYSMIN in mV
int getVSYSMIN() { return getInt(VSYSMIN); }
/// Sets VSYSMIN in mV
void setVSYSMIN(int val) { setInt(VSYSMIN, val); }



// ================================
// REG01_Charge_Voltage_Limit
// ================================


Setting VREG = { REG01_Charge_Voltage_Limit, true, "VREG", "mV", settings_type_t::INT, 11, 0, false, 3000, 18800, 0, 10, settings_flags_t::NONE };
/// Returns VREG in mV
int getVREG() { return getInt(VREG); }
/// Sets VREG in mV
void setVREG(int val) { setInt(VREG, val); }



// ================================
// REG03_Charge_Current_Limit
// ================================


Setting ICHG = { REG03_Charge_Current_Limit, true, "ICHG", "mA", settings_type_t::INT, 9, 0, false, 50, 5000, 0, 10, settings_flags_t::NONE };
/// Returns ICHG in mA
int getICHG() { return getInt(ICHG); }
/// Sets ICHG in mA
void setICHG(int val) { setInt(ICHG, val); }



// ================================
// REG05_Input_Voltage_Limit
// ================================


Setting VINDPM = { REG05_Input_Voltage_Limit, false, "VINDPM", "mV", settings_type_t::INT, 8, 0, false, 3600, 22000, 0, 100, settings_flags_t::NONE };
/// Returns VINDPM in mV
int getVINDPM() { return getInt(VINDPM); }
/// Sets VINDPM in mV
void setVINDPM(int val) { setInt(VINDPM, val); }



// ================================
// REG06_Input_Current_Limit
// ================================


Setting IINDPM = { REG06_Input_Current_Limit, true, "IINDPM", "mA", settings_type_t::INT, 9, 0, false, 100, 3300, 0, 10, settings_flags_t::NONE };
/// Returns IINDPM in mA
int getIINDPM() { return getInt(IINDPM); }
/// Sets IINDPM in mA
void setIINDPM(int val) { setInt(IINDPM, val); }



// ================================
// REG08_Precharge_Control
// ================================


enum class VBAT_LOWV_t : uint8_t {

    VBAT_LOWV_PCT_15 = 0, 
    VBAT_LOWV_PCT_62 = 1, 
    VBAT_LOWV_PCT_67 = 2, 
    VBAT_LOWV_PCT_71 = 3
};

strings_vector_t VBAT_LOWV_strings = {

    "15%(VREG)", 
    "62.2%(VREG)", 
    "66.7%(VREG)", 
    "71.4%(VREG)"
};

Setting VBAT_LOWV = { REG08_Precharge_Control, false, "VBAT_LOWV", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, VBAT_LOWV_strings };
VBAT_LOWV_t getVBAT_LOWV() { return static_cast<VBAT_LOWV_t>(getInt(VBAT_LOWV)); }
const char * getVBAT_LOWV_enum_string() { return getString(VBAT_LOWV); }
void setVBAT_LOWV(VBAT_LOWV_t val) { setEnum<VBAT_LOWV_t>(VBAT_LOWV, val); }




// ================================
// REG08_Precharge_Control
// ================================


Setting IPRECHG = { REG08_Precharge_Control, false, "IPRECHG", "mA", settings_type_t::INT, 6, 0, false, 40, 2000, 0, 40, settings_flags_t::NONE };
/// Returns IPRECHG in mA
int getIPRECHG() { return getInt(IPRECHG); }
/// Sets IPRECHG in mA
void setIPRECHG(int val) { setInt(IPRECHG, val); }



// ================================
// REG09_Termination_Control
// ================================


Setting REG_RST = { REG09_Termination_Control, false, "REG_RST", "", settings_type_t::BOOL, 1, 6, false };
bool getREG_RST() { return getBool(REG_RST); }
void setREG_RST(bool val) { setBool(REG_RST, val); }



// ================================
// REG09_Termination_Control
// ================================


Setting STOP_WD_CHG = { REG09_Termination_Control, false, "STOP_WD_CHG", "", settings_type_t::BOOL, 1, 5, false };
bool getSTOP_WD_CHG() { return getBool(STOP_WD_CHG); }
void setSTOP_WD_CHG(bool val) { setBool(STOP_WD_CHG, val); }



// ================================
// REG09_Termination_Control
// ================================


Setting ITERM = { REG09_Termination_Control, false, "ITERM", "mA", settings_type_t::INT, 5, 0, false, 40, 1000, 0, 40, settings_flags_t::NONE };
/// Returns ITERM in mA
int getITERM() { return getInt(ITERM); }
/// Sets ITERM in mA
void setITERM(int val) { setInt(ITERM, val); }



// ================================
// REG0A_Recharge_Control
// ================================


enum class CELL_t : uint8_t {

    CELL_1S = 0, 
    CELL_2S = 1, 
    CELL_3S = 2, 
    CELL_4S = 3
};

strings_vector_t CELL_strings = {

    "1 cell", 
    "2 cell in series", 
    "3 cell in series", 
    "4 cell in series"
};

Setting CELL = { REG0A_Recharge_Control, false, "CELL", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, CELL_strings };
CELL_t getCELL() { return static_cast<CELL_t>(getInt(CELL)); }
const char * getCELL_enum_string() { return getString(CELL); }
void setCELL(CELL_t val) { setEnum<CELL_t>(CELL, val); }




// ================================
// REG0A_Recharge_Control
// ================================


enum class TRECHG_t : uint8_t {

    TRECHG_64MS = 0, 
    TRECHG_256MS = 1, 
    TRECHG_1024MS = 2, 
    TRECHG_2048MS = 3
};

strings_vector_t TRECHG_strings = {

    "64 ms", 
    "256 ms", 
    "1024 ms", 
    "2048 ms"
};

Setting TRECHG = { REG0A_Recharge_Control, false, "TRECHG", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, TRECHG_strings };
TRECHG_t getTRECHG() { return static_cast<TRECHG_t>(getInt(TRECHG)); }
const char * getTRECHG_enum_string() { return getString(TRECHG); }
void setTRECHG(TRECHG_t val) { setEnum<TRECHG_t>(TRECHG, val); }




// ================================
// REG0A_Recharge_Control
// ================================


Setting VRECHG = { REG0A_Recharge_Control, false, "VRECHG", "mV", settings_type_t::INT, 4, 0, false, 50, 800, 50, 50, settings_flags_t::NONE };
/// Returns VRECHG in mV
int getVRECHG() { return getInt(VRECHG); }
/// Sets VRECHG in mV
void setVRECHG(int val) { setInt(VRECHG, val); }



// ================================
// REG0B_VOTG_regulation
// ================================


Setting VOTG = { REG0B_VOTG_regulation, true, "VOTG", "mV", settings_type_t::INT, 11, 0, false, 2800, 22000, 2800, 10, settings_flags_t::NONE };
/// Returns VOTG in mV
int getVOTG() { return getInt(VOTG); }
/// Sets VOTG in mV
void setVOTG(int val) { setInt(VOTG, val); }



// ================================
// REG0D_IOTG_regulation
// ================================


enum class PRECHG_TMR_t : uint8_t {

    PRECHG_TMR_2HRS = 0, 
    PRECHG_TMR_30MIN = 1
};

strings_vector_t PRECHG_TMR_strings = {

    "2 hours", 
    "30 minutes"
};

Setting PRECHG_TMR = { REG0D_IOTG_regulation, false, "PRECHG_TMR", "", settings_type_t::ENUM, 1, 7, false, 0, 1, 0, 1, settings_flags_t::NONE, PRECHG_TMR_strings };
PRECHG_TMR_t getPRECHG_TMR() { return static_cast<PRECHG_TMR_t>(getInt(PRECHG_TMR)); }
const char * getPRECHG_TMR_enum_string() { return getString(PRECHG_TMR); }
void setPRECHG_TMR(PRECHG_TMR_t val) { setEnum<PRECHG_TMR_t>(PRECHG_TMR, val); }




// ================================
// REG0D_IOTG_regulation
// ================================


Setting IOTG = { REG0D_IOTG_regulation, false, "IOTG", "mA", settings_type_t::INT, 7, 0, false, 160, 3360, 0, 40, settings_flags_t::NONE };
/// Returns IOTG in mA
int getIOTG() { return getInt(IOTG); }
/// Sets IOTG in mA
void setIOTG(int val) { setInt(IOTG, val); }



// ================================
// REG0E_Timer_Control
// ================================


enum class TOPOFF_TMR_t : uint8_t {

    TOPOFF_TMR_DISABLED = 0, 
    TOPOFF_TMR_15MIN = 1, 
    TOPOFF_TMR_30MIN = 2, 
    TOPOFF_TMR_45MIN = 3
};

strings_vector_t TOPOFF_TMR_strings = {

    "Disabled", 
    "15 minutes", 
    "30 minutes", 
    "45 minutes"
};

Setting TOPOFF_TMR = { REG0E_Timer_Control, false, "TOPOFF_TMR", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, TOPOFF_TMR_strings };
TOPOFF_TMR_t getTOPOFF_TMR() { return static_cast<TOPOFF_TMR_t>(getInt(TOPOFF_TMR)); }
const char * getTOPOFF_TMR_enum_string() { return getString(TOPOFF_TMR); }
void setTOPOFF_TMR(TOPOFF_TMR_t val) { setEnum<TOPOFF_TMR_t>(TOPOFF_TMR, val); }




// ================================
// REG0E_Timer_Control
// ================================


Setting EN_TRICHG_TMR = { REG0E_Timer_Control, false, "EN_TRICHG_TMR", "", settings_type_t::BOOL, 1, 5, false };
bool getEN_TRICHG_TMR() { return getBool(EN_TRICHG_TMR); }
void setEN_TRICHG_TMR(bool val) { setBool(EN_TRICHG_TMR, val); }



// ================================
// REG0E_Timer_Control
// ================================


Setting EN_PRECHG_TMR = { REG0E_Timer_Control, false, "EN_PRECHG_TMR", "", settings_type_t::BOOL, 1, 4, false };
bool getEN_PRECHG_TMR() { return getBool(EN_PRECHG_TMR); }
void setEN_PRECHG_TMR(bool val) { setBool(EN_PRECHG_TMR, val); }



// ================================
// REG0E_Timer_Control
// ================================


Setting EN_CHG_TMR = { REG0E_Timer_Control, false, "EN_CHG_TMR", "", settings_type_t::BOOL, 1, 3, false };
bool getEN_CHG_TMR() { return getBool(EN_CHG_TMR); }
void setEN_CHG_TMR(bool val) { setBool(EN_CHG_TMR, val); }



// ================================
// REG0E_Timer_Control
// ================================


enum class CHG_TMR_t : uint8_t {

    CHG_TMR_5HRS = 0, 
    CHG_TMR_8HRS = 1, 
    CHG_TMR_12HRS = 2, 
    CHG_TMR_24HRS = 3
};

strings_vector_t CHG_TMR_strings = {

    "5 hours", 
    "8 hours", 
    "12 hours", 
    "24 hours"
};

Setting CHG_TMR = { REG0E_Timer_Control, false, "CHG_TMR", "", settings_type_t::ENUM, 2, 1, false, 0, 3, 0, 1, settings_flags_t::NONE, CHG_TMR_strings };
CHG_TMR_t getCHG_TMR() { return static_cast<CHG_TMR_t>(getInt(CHG_TMR)); }
const char * getCHG_TMR_enum_string() { return getString(CHG_TMR); }
void setCHG_TMR(CHG_TMR_t val) { setEnum<CHG_TMR_t>(CHG_TMR, val); }




// ================================
// REG0E_Timer_Control
// ================================


Setting TMR2X_EN = { REG0E_Timer_Control, false, "TMR2X_EN", "", settings_type_t::BOOL, 1, 0, false };
bool getTMR2X_EN() { return getBool(TMR2X_EN); }
void setTMR2X_EN(bool val) { setBool(TMR2X_EN, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_AUTO_IBATDIS = { REG0F_Charger_Control_0, false, "EN_AUTO_IBATDIS", "", settings_type_t::BOOL, 1, 7, false };
bool getEN_AUTO_IBATDIS() { return getBool(EN_AUTO_IBATDIS); }
void setEN_AUTO_IBATDIS(bool val) { setBool(EN_AUTO_IBATDIS, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting FORCE_IBATDIS = { REG0F_Charger_Control_0, false, "FORCE_IBATDIS", "", settings_type_t::BOOL, 1, 6, false };
bool getFORCE_IBATDIS() { return getBool(FORCE_IBATDIS); }
void setFORCE_IBATDIS(bool val) { setBool(FORCE_IBATDIS, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_CHG = { REG0F_Charger_Control_0, false, "EN_CHG", "", settings_type_t::BOOL, 1, 5, false };
bool getEN_CHG() { return getBool(EN_CHG); }
void setEN_CHG(bool val) { setBool(EN_CHG, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_ICO = { REG0F_Charger_Control_0, false, "EN_ICO", "", settings_type_t::BOOL, 1, 4, false };
bool getEN_ICO() { return getBool(EN_ICO); }
void setEN_ICO(bool val) { setBool(EN_ICO, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting FORCE_ICO = { REG0F_Charger_Control_0, false, "FORCE_ICO", "", settings_type_t::BOOL, 1, 3, false };
bool getFORCE_ICO() { return getBool(FORCE_ICO); }
void setFORCE_ICO(bool val) { setBool(FORCE_ICO, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_HIZ = { REG0F_Charger_Control_0, false, "EN_HIZ", "", settings_type_t::BOOL, 1, 2, false };
bool getEN_HIZ() { return getBool(EN_HIZ); }
void setEN_HIZ(bool val) { setBool(EN_HIZ, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_TERM = { REG0F_Charger_Control_0, false, "EN_TERM", "", settings_type_t::BOOL, 1, 1, false };
bool getEN_TERM() { return getBool(EN_TERM); }
void setEN_TERM(bool val) { setBool(EN_TERM, val); }



// ================================
// REG0F_Charger_Control_0
// ================================


Setting EN_BACKUP = { REG0F_Charger_Control_0, false, "EN_BACKUP", "", settings_type_t::BOOL, 1, 0, false };
bool getEN_BACKUP() { return getBool(EN_BACKUP); }
void setEN_BACKUP(bool val) { setBool(EN_BACKUP, val); }



// ================================
// REG10_Charger_Control_1
// ================================


enum class VBUS_BACKUP_t : uint8_t {

    PCT_VBUS_BACKUP_40 = 0, 
    PCT_VBUS_BACKUP_60 = 1, 
    PCT_VBUS_BACKUP_80 = 2, 
    PCT_VBUS_BACKUP_100 = 3
};

strings_vector_t VBUS_BACKUP_strings = {

    "<40%(VINDPM)", 
    "<60%(VINDPM)", 
    "<80%(VINDPM)", 
    "<100%(VINDPM)"
};

Setting VBUS_BACKUP = { REG10_Charger_Control_1, false, "VBUS_BACKUP", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, VBUS_BACKUP_strings };
VBUS_BACKUP_t getVBUS_BACKUP() { return static_cast<VBUS_BACKUP_t>(getInt(VBUS_BACKUP)); }
const char * getVBUS_BACKUP_enum_string() { return getString(VBUS_BACKUP); }
void setVBUS_BACKUP(VBUS_BACKUP_t val) { setEnum<VBUS_BACKUP_t>(VBUS_BACKUP, val); }




// ================================
// REG10_Charger_Control_1
// ================================


enum class VAC_OVP_t : uint8_t {

    VAC_OVP_26V = 0, 
    VAC_OVP_22V = 1, 
    VAC_OVP_12V = 2, 
    VAC_OVP_7V = 3
};

strings_vector_t VAC_OVP_strings = {

    ">26V", 
    ">22V", 
    ">12V", 
    ">7V"
};

Setting VAC_OVP = { REG10_Charger_Control_1, false, "VAC_OVP", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, VAC_OVP_strings };
VAC_OVP_t getVAC_OVP() { return static_cast<VAC_OVP_t>(getInt(VAC_OVP)); }
const char * getVAC_OVP_enum_string() { return getString(VAC_OVP); }
void setVAC_OVP(VAC_OVP_t val) { setEnum<VAC_OVP_t>(VAC_OVP, val); }




// ================================
// REG10_Charger_Control_1
// ================================


Setting WD_RST = { REG10_Charger_Control_1, false, "WD_RST", "", settings_type_t::BOOL, 1, 3, false };
bool getWD_RST() { return getBool(WD_RST); }
void setWD_RST(bool val) { setBool(WD_RST, val); }



// ================================
// REG10_Charger_Control_1
// ================================


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

strings_vector_t WATCHDOG_strings = {

    "Disabled", 
    "0.5s", 
    "1s", 
    "2s", 
    "20s", 
    "40s", 
    "80s", 
    "160s"
};

Setting WATCHDOG = { REG10_Charger_Control_1, false, "WATCHDOG", "", settings_type_t::ENUM, 3, 0, false, 0, 7, 0, 1, settings_flags_t::NONE, WATCHDOG_strings };
WATCHDOG_t getWATCHDOG() { return static_cast<WATCHDOG_t>(getInt(WATCHDOG)); }
const char * getWATCHDOG_enum_string() { return getString(WATCHDOG); }
void setWATCHDOG(WATCHDOG_t val) { setEnum<WATCHDOG_t>(WATCHDOG, val); }




// ================================
// REG11_Charger_Control_2
// ================================


Setting FORCE_INDET = { REG11_Charger_Control_2, false, "FORCE_INDET", "", settings_type_t::BOOL, 1, 7, false };
bool getFORCE_INDET() { return getBool(FORCE_INDET); }
void setFORCE_INDET(bool val) { setBool(FORCE_INDET, val); }



// ================================
// REG11_Charger_Control_2
// ================================


Setting AUTO_INDET_EN = { REG11_Charger_Control_2, false, "AUTO_INDET_EN", "", settings_type_t::BOOL, 1, 6, false };
bool getAUTO_INDET_EN() { return getBool(AUTO_INDET_EN); }
void setAUTO_INDET_EN(bool val) { setBool(AUTO_INDET_EN, val); }



// ================================
// REG11_Charger_Control_2
// ================================


Setting EN_12V = { REG11_Charger_Control_2, false, "EN_12V", "", settings_type_t::BOOL, 1, 5, false };
bool getEN_12V() { return getBool(EN_12V); }
void setEN_12V(bool val) { setBool(EN_12V, val); }



// ================================
// REG11_Charger_Control_2
// ================================


Setting EN_9V = { REG11_Charger_Control_2, false, "EN_9V", "", settings_type_t::BOOL, 1, 4, false };
bool getEN_9V() { return getBool(EN_9V); }
void setEN_9V(bool val) { setBool(EN_9V, val); }



// ================================
// REG11_Charger_Control_2
// ================================


Setting HVDCP_EN = { REG11_Charger_Control_2, false, "HVDCP_EN", "", settings_type_t::BOOL, 1, 3, false };
bool getHVDCP_EN() { return getBool(HVDCP_EN); }
void setHVDCP_EN(bool val) { setBool(HVDCP_EN, val); }



// ================================
// REG11_Charger_Control_2
// ================================


enum class SDRV_CTRL_t : uint8_t {

    SDRV_CTRL_IDLE = 0, 
    SDRV_CTRL_SHUTDOWN = 1, 
    SDRV_CTRL_SHIP = 2, 
    SDRV_CTRL_SYS_PWR_RST = 3
};

strings_vector_t SDRV_CTRL_strings = {

    "Idle", 
    "Shutdown", 
    "Ship", 
    "System Power Reset"
};

Setting SDRV_CTRL = { REG11_Charger_Control_2, false, "SDRV_CTRL", "", settings_type_t::ENUM, 2, 1, false, 0, 3, 0, 1, settings_flags_t::NONE, SDRV_CTRL_strings };
SDRV_CTRL_t getSDRV_CTRL() { return static_cast<SDRV_CTRL_t>(getInt(SDRV_CTRL)); }
const char * getSDRV_CTRL_enum_string() { return getString(SDRV_CTRL); }
void setSDRV_CTRL(SDRV_CTRL_t val) { setEnum<SDRV_CTRL_t>(SDRV_CTRL, val); }




// ================================
// REG11_Charger_Control_2
// ================================


enum class SDRV_DLY_t : uint8_t {

    SDRV_DLY_10S = 0, 
    SDRV_DLY_0S = 1
};

strings_vector_t SDRV_DLY_strings = {

    "10s", 
    "0s"
};

Setting SDRV_DLY = { REG11_Charger_Control_2, false, "SDRV_DLY", "", settings_type_t::ENUM, 1, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, SDRV_DLY_strings };
SDRV_DLY_t getSDRV_DLY() { return static_cast<SDRV_DLY_t>(getInt(SDRV_DLY)); }
const char * getSDRV_DLY_enum_string() { return getString(SDRV_DLY); }
void setSDRV_DLY(SDRV_DLY_t val) { setEnum<SDRV_DLY_t>(SDRV_DLY, val); }




// ================================
// REG12_Charger_Control_3
// ================================


Setting DIS_ACDRV = { REG12_Charger_Control_3, false, "DIS_ACDRV", "", settings_type_t::BOOL, 1, 7, false };
bool getDIS_ACDRV() { return getBool(DIS_ACDRV); }
void setDIS_ACDRV(bool val) { setBool(DIS_ACDRV, val); }



// ================================
// REG12_Charger_Control_3
// ================================


Setting EN_OTG = { REG12_Charger_Control_3, false, "EN_OTG", "", settings_type_t::BOOL, 1, 6, false };
bool getEN_OTG() { return getBool(EN_OTG); }
void setEN_OTG(bool val) { setBool(EN_OTG, val); }



// ================================
// REG12_Charger_Control_3
// ================================


Setting PFM_OTG_DIS = { REG12_Charger_Control_3, false, "PFM_OTG_DIS", "", settings_type_t::BOOL, 1, 5, false };
bool getPFM_OTG_DIS() { return getBool(PFM_OTG_DIS); }
void setPFM_OTG_DIS(bool val) { setBool(PFM_OTG_DIS, val); }



// ================================
// REG12_Charger_Control_3
// ================================


Setting PFM_FWD_DIS = { REG12_Charger_Control_3, false, "PFM_FWD_DIS", "", settings_type_t::BOOL, 1, 4, false };
bool getPFM_FWD_DIS() { return getBool(PFM_FWD_DIS); }
void setPFM_FWD_DIS(bool val) { setBool(PFM_FWD_DIS, val); }



// ================================
// REG12_Charger_Control_3
// ================================


enum class WKUP_DLY_t : uint8_t {

    WKUP_DLY_1S = 0, 
    WKUP_DLY_15MS = 1
};

strings_vector_t WKUP_DLY_strings = {

    "1s", 
    "15ms"
};

Setting WKUP_DLY = { REG12_Charger_Control_3, false, "WKUP_DLY", "", settings_type_t::ENUM, 1, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, WKUP_DLY_strings };
WKUP_DLY_t getWKUP_DLY() { return static_cast<WKUP_DLY_t>(getInt(WKUP_DLY)); }
const char * getWKUP_DLY_enum_string() { return getString(WKUP_DLY); }
void setWKUP_DLY(WKUP_DLY_t val) { setEnum<WKUP_DLY_t>(WKUP_DLY, val); }




// ================================
// REG12_Charger_Control_3
// ================================


Setting DIS_LDO = { REG12_Charger_Control_3, false, "DIS_LDO", "", settings_type_t::BOOL, 1, 2, false };
bool getDIS_LDO() { return getBool(DIS_LDO); }
void setDIS_LDO(bool val) { setBool(DIS_LDO, val); }



// ================================
// REG12_Charger_Control_3
// ================================


Setting DIS_OTG_OOA = { REG12_Charger_Control_3, false, "DIS_OTG_OOA", "", settings_type_t::BOOL, 1, 1, false };
bool getDIS_OTG_OOA() { return getBool(DIS_OTG_OOA); }
void setDIS_OTG_OOA(bool val) { setBool(DIS_OTG_OOA, val); }



// ================================
// REG12_Charger_Control_3
// ================================


Setting DIS_FWD_OOA = { REG12_Charger_Control_3, false, "DIS_FWD_OOA", "", settings_type_t::BOOL, 1, 0, false };
bool getDIS_FWD_OOA() { return getBool(DIS_FWD_OOA); }
void setDIS_FWD_OOA(bool val) { setBool(DIS_FWD_OOA, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting EN_ACDRV2 = { REG13_Charger_Control_4, false, "EN_ACDRV2", "", settings_type_t::BOOL, 1, 7, false };
bool getEN_ACDRV2() { return getBool(EN_ACDRV2); }
void setEN_ACDRV2(bool val) { setBool(EN_ACDRV2, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting EN_ACDRV1 = { REG13_Charger_Control_4, false, "EN_ACDRV1", "", settings_type_t::BOOL, 1, 6, false };
bool getEN_ACDRV1() { return getBool(EN_ACDRV1); }
void setEN_ACDRV1(bool val) { setBool(EN_ACDRV1, val); }



// ================================
// REG13_Charger_Control_4
// ================================


enum class PWM_FREQ_t : uint8_t {

    PWM_FREQ_1_5MHZ = 0, 
    PWM_FREQ_750KHZ = 1
};

strings_vector_t PWM_FREQ_strings = {

    "1.5 MHz", 
    "750 kHz"
};

Setting PWM_FREQ = { REG13_Charger_Control_4, false, "PWM_FREQ", "", settings_type_t::ENUM, 1, 5, false, 0, 1, 0, 1, settings_flags_t::NONE, PWM_FREQ_strings };
PWM_FREQ_t getPWM_FREQ() { return static_cast<PWM_FREQ_t>(getInt(PWM_FREQ)); }
const char * getPWM_FREQ_enum_string() { return getString(PWM_FREQ); }
void setPWM_FREQ(PWM_FREQ_t val) { setEnum<PWM_FREQ_t>(PWM_FREQ, val); }




// ================================
// REG13_Charger_Control_4
// ================================


Setting DIS_STAT = { REG13_Charger_Control_4, false, "DIS_STAT", "", settings_type_t::BOOL, 1, 4, false };
bool getDIS_STAT() { return getBool(DIS_STAT); }
void setDIS_STAT(bool val) { setBool(DIS_STAT, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting DIS_VSYS_SHORT = { REG13_Charger_Control_4, false, "DIS_VSYS_SHORT", "", settings_type_t::BOOL, 1, 3, false };
bool getDIS_VSYS_SHORT() { return getBool(DIS_VSYS_SHORT); }
void setDIS_VSYS_SHORT(bool val) { setBool(DIS_VSYS_SHORT, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting DIS_VOTG_UVP = { REG13_Charger_Control_4, false, "DIS_VOTG_UVP", "", settings_type_t::BOOL, 1, 2, false };
bool getDIS_VOTG_UVP() { return getBool(DIS_VOTG_UVP); }
void setDIS_VOTG_UVP(bool val) { setBool(DIS_VOTG_UVP, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting FORCE_VINDPM_DET = { REG13_Charger_Control_4, false, "FORCE_VINDPM_DET", "", settings_type_t::BOOL, 1, 1, false };
bool getFORCE_VINDPM_DET() { return getBool(FORCE_VINDPM_DET); }
void setFORCE_VINDPM_DET(bool val) { setBool(FORCE_VINDPM_DET, val); }



// ================================
// REG13_Charger_Control_4
// ================================


Setting EN_IBUS_OCP = { REG13_Charger_Control_4, false, "EN_IBUS_OCP", "", settings_type_t::BOOL, 1, 0, false };
bool getEN_IBUS_OCP() { return getBool(EN_IBUS_OCP); }
void setEN_IBUS_OCP(bool val) { setBool(EN_IBUS_OCP, val); }



// ================================
// REG14_Charger_Control_5
// ================================


Setting SFET_PRESENT = { REG14_Charger_Control_5, false, "SFET_PRESENT", "", settings_type_t::BOOL, 1, 7, false };
bool getSFET_PRESENT() { return getBool(SFET_PRESENT); }
void setSFET_PRESENT(bool val) { setBool(SFET_PRESENT, val); }



// ================================
// REG14_Charger_Control_5
// ================================


Setting EN_IBAT = { REG14_Charger_Control_5, false, "EN_IBAT", "", settings_type_t::BOOL, 1, 5, false };
bool getEN_IBAT() { return getBool(EN_IBAT); }
void setEN_IBAT(bool val) { setBool(EN_IBAT, val); }



// ================================
// REG14_Charger_Control_5
// ================================


enum class IBAT_REG_t : uint8_t {

    IBAT_REG_3A = 0, 
    IBAT_REG_4A = 1, 
    IBAT_REG_5A = 2, 
    IBAT_REG_DISABLE = 3
};

strings_vector_t IBAT_REG_strings = {

    "3A", 
    "4A", 
    "5A", 
    "Disabled"
};

Setting IBAT_REG = { REG14_Charger_Control_5, false, "IBAT_REG", "", settings_type_t::ENUM, 2, 3, false, 0, 3, 0, 1, settings_flags_t::NONE, IBAT_REG_strings };
IBAT_REG_t getIBAT_REG() { return static_cast<IBAT_REG_t>(getInt(IBAT_REG)); }
const char * getIBAT_REG_enum_string() { return getString(IBAT_REG); }
void setIBAT_REG(IBAT_REG_t val) { setEnum<IBAT_REG_t>(IBAT_REG, val); }




// ================================
// REG14_Charger_Control_5
// ================================


Setting EN_IINDPM = { REG14_Charger_Control_5, false, "EN_IINDPM", "", settings_type_t::BOOL, 1, 2, false };
bool getEN_IINDPM() { return getBool(EN_IINDPM); }
void setEN_IINDPM(bool val) { setBool(EN_IINDPM, val); }



// ================================
// REG14_Charger_Control_5
// ================================


Setting EN_EXTILIM = { REG14_Charger_Control_5, false, "EN_EXTILIM", "", settings_type_t::BOOL, 1, 1, false };
bool getEN_EXTILIM() { return getBool(EN_EXTILIM); }
void setEN_EXTILIM(bool val) { setBool(EN_EXTILIM, val); }



// ================================
// REG14_Charger_Control_5
// ================================


Setting EN_BATOC = { REG14_Charger_Control_5, false, "EN_BATOC", "", settings_type_t::BOOL, 1, 0, false };
bool getEN_BATOC() { return getBool(EN_BATOC); }
void setEN_BATOC(bool val) { setBool(EN_BATOC, val); }



// ================================
// REG15_MPPT_Control
// ================================


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

strings_vector_t VOC_PCT_strings = {

    "0.5625", 
    "0.625", 
    "0.6875", 
    "0.75", 
    "0.8125", 
    "0.875", 
    "0.9375", 
    "1"
};

Setting VOC_PCT = { REG15_MPPT_Control, false, "VOC_PCT", "", settings_type_t::ENUM, 3, 5, false, 0, 7, 0, 1, settings_flags_t::NONE, VOC_PCT_strings };
VOC_PCT_t getVOC_PCT() { return static_cast<VOC_PCT_t>(getInt(VOC_PCT)); }
const char * getVOC_PCT_enum_string() { return getString(VOC_PCT); }
void setVOC_PCT(VOC_PCT_t val) { setEnum<VOC_PCT_t>(VOC_PCT, val); }




// ================================
// REG15_MPPT_Control
// ================================


enum class VOC_DLY_t : uint8_t {

    VOC_DLY_50MS = 0, 
    VOC_DLY_300MS = 1, 
    VOC_DLY_2S = 2, 
    VOC_DLY_5S = 3
};

strings_vector_t VOC_DLY_strings = {

    "50ms", 
    "300ms", 
    "2s", 
    "5s"
};

Setting VOC_DLY = { REG15_MPPT_Control, false, "VOC_DLY", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, VOC_DLY_strings };
VOC_DLY_t getVOC_DLY() { return static_cast<VOC_DLY_t>(getInt(VOC_DLY)); }
const char * getVOC_DLY_enum_string() { return getString(VOC_DLY); }
void setVOC_DLY(VOC_DLY_t val) { setEnum<VOC_DLY_t>(VOC_DLY, val); }




// ================================
// REG15_MPPT_Control
// ================================


enum class VOC_RATE_t : uint8_t {

    VOC_RATE_30S = 0, 
    VOC_RATE_2MIN = 1, 
    VOC_RATE_10MIN = 2, 
    VOC_RATE_30MIN = 3
};

strings_vector_t VOC_RATE_strings = {

    "30s", 
    "2min", 
    "10min", 
    "30min"
};

Setting VOC_RATE = { REG15_MPPT_Control, false, "VOC_RATE", "", settings_type_t::ENUM, 2, 2, false, 0, 3, 0, 1, settings_flags_t::NONE, VOC_RATE_strings };
VOC_RATE_t getVOC_RATE() { return static_cast<VOC_RATE_t>(getInt(VOC_RATE)); }
const char * getVOC_RATE_enum_string() { return getString(VOC_RATE); }
void setVOC_RATE(VOC_RATE_t val) { setEnum<VOC_RATE_t>(VOC_RATE, val); }




// ================================
// REG15_MPPT_Control
// ================================


Setting EN_MPPT = { REG15_MPPT_Control, false, "EN_MPPT", "", settings_type_t::BOOL, 1, 0, false };
bool getEN_MPPT() { return getBool(EN_MPPT); }
void setEN_MPPT(bool val) { setBool(EN_MPPT, val); }



// ================================
// REG16_Temperature_Control
// ================================


enum class TREG_t : uint8_t {

    TREG_60 = 0, 
    TREG_80 = 1, 
    TREG_100 = 2, 
    TREG_120 = 3
};

strings_vector_t TREG_strings = {

    "60'C", 
    "80'C", 
    "100'C", 
    "120'C"
};

Setting TREG = { REG16_Temperature_Control, false, "TREG", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, TREG_strings };
TREG_t getTREG() { return static_cast<TREG_t>(getInt(TREG)); }
const char * getTREG_enum_string() { return getString(TREG); }
void setTREG(TREG_t val) { setEnum<TREG_t>(TREG, val); }




// ================================
// REG16_Temperature_Control
// ================================


enum class TSHUT_t : uint8_t {

    TSHUT_150 = 0, 
    TSHUT_130 = 1, 
    TSHUT_120 = 2, 
    TSHUT_85 = 3
};

strings_vector_t TSHUT_strings = {

    "150'C", 
    "130'C", 
    "120'C", 
    "85'C"
};

Setting TSHUT = { REG16_Temperature_Control, false, "TSHUT", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, TSHUT_strings };
TSHUT_t getTSHUT() { return static_cast<TSHUT_t>(getInt(TSHUT)); }
const char * getTSHUT_enum_string() { return getString(TSHUT); }
void setTSHUT(TSHUT_t val) { setEnum<TSHUT_t>(TSHUT, val); }




// ================================
// REG16_Temperature_Control
// ================================


Setting VBUS_PD_EN = { REG16_Temperature_Control, false, "VBUS_PD_EN", "", settings_type_t::BOOL, 1, 3, false };
bool getVBUS_PD_EN() { return getBool(VBUS_PD_EN); }
void setVBUS_PD_EN(bool val) { setBool(VBUS_PD_EN, val); }



// ================================
// REG16_Temperature_Control
// ================================


Setting VAC1_PD_EN = { REG16_Temperature_Control, false, "VAC1_PD_EN", "", settings_type_t::BOOL, 1, 2, false };
bool getVAC1_PD_EN() { return getBool(VAC1_PD_EN); }
void setVAC1_PD_EN(bool val) { setBool(VAC1_PD_EN, val); }



// ================================
// REG16_Temperature_Control
// ================================


Setting VAC2_PD_EN = { REG16_Temperature_Control, false, "VAC2_PD_EN", "", settings_type_t::BOOL, 1, 1, false };
bool getVAC2_PD_EN() { return getBool(VAC2_PD_EN); }
void setVAC2_PD_EN(bool val) { setBool(VAC2_PD_EN, val); }



// ================================
// REG16_Temperature_Control
// ================================


enum class BKUP_ACFET1_ON_t : uint8_t {

    BKUP_ACFET1_ON_IDLE = 0, 
    BKUP_ACFET1_ON_TURN_ON = 1
};

strings_vector_t BKUP_ACFET1_ON_strings = {

    "Idle", 
    "Turn on ACFET1 in backup mode"
};

Setting BKUP_ACFET1_ON = { REG16_Temperature_Control, false, "BKUP_ACFET1_ON", "", settings_type_t::ENUM, 1, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, BKUP_ACFET1_ON_strings };
BKUP_ACFET1_ON_t getBKUP_ACFET1_ON() { return static_cast<BKUP_ACFET1_ON_t>(getInt(BKUP_ACFET1_ON)); }
const char * getBKUP_ACFET1_ON_enum_string() { return getString(BKUP_ACFET1_ON); }
void setBKUP_ACFET1_ON(BKUP_ACFET1_ON_t val) { setEnum<BKUP_ACFET1_ON_t>(BKUP_ACFET1_ON, val); }




// ================================
// REG17_NTC_Control_0
// ================================


enum class JEITA_VSET_t : uint8_t {

    JEITA_VSET_CHARGE_SUSPEND = 0, 
    JEITA_VSET_SET_VREG_TO_VREG_800MV = 1, 
    JEITA_VSET_SET_VREG_TO_VREG_600MV = 2, 
    JEITA_VSET_SET_VREG_TO_VREG_400MV = 3, 
    JEITA_VSET_SET_VREG_TO_VREG_300MV = 4, 
    JEITA_VSET_SET_VREG_TO_VREG_200MV = 5, 
    JEITA_VSET_SET_VREG_TO_VREG_100MV = 6, 
    JEITA_VSET_VREG_UNCHANGED = 7
};

strings_vector_t JEITA_VSET_strings = {

    "Charge Suspend", 
    "Set VREG to VREG-800mV", 
    "Set VREG to VREG-600mV", 
    "Set VREG to VREG-400mV (default)", 
    "Set VREG to VREG-300mV", 
    "Set VREG to VREG-200mV", 
    "Set VREG to VREG-100mV", 
    "VREG unchanged"
};

Setting JEITA_VSET = { REG17_NTC_Control_0, false, "JEITA_VSET", "", settings_type_t::ENUM, 3, 5, false, 0, 7, 0, 1, settings_flags_t::NONE, JEITA_VSET_strings };
JEITA_VSET_t getJEITA_VSET() { return static_cast<JEITA_VSET_t>(getInt(JEITA_VSET)); }
const char * getJEITA_VSET_enum_string() { return getString(JEITA_VSET); }
void setJEITA_VSET(JEITA_VSET_t val) { setEnum<JEITA_VSET_t>(JEITA_VSET, val); }




// ================================
// REG17_NTC_Control_0
// ================================


enum class JEITA_ISETH_t : uint8_t {

    JEITA_ISETH_CHARGE_SUSPEND = 0, 
    JEITA_ISETH_SET_ICHG_TO_20 = 1, 
    JEITA_ISETH_SET_ICHG_TO_40 = 2, 
    JEITA_ISETH_ICHG_UNCHANGED = 3
};

strings_vector_t JEITA_ISETH_strings = {

    "Charge Suspend", 
    "Set ICHG to 20%* ICHG", 
    "Set ICHG to 40%* ICHG", 
    "ICHG unchanged"
};

Setting JEITA_ISETH = { REG17_NTC_Control_0, false, "JEITA_ISETH", "", settings_type_t::ENUM, 2, 3, false, 0, 3, 0, 1, settings_flags_t::NONE, JEITA_ISETH_strings };
JEITA_ISETH_t getJEITA_ISETH() { return static_cast<JEITA_ISETH_t>(getInt(JEITA_ISETH)); }
const char * getJEITA_ISETH_enum_string() { return getString(JEITA_ISETH); }
void setJEITA_ISETH(JEITA_ISETH_t val) { setEnum<JEITA_ISETH_t>(JEITA_ISETH, val); }




// ================================
// REG17_NTC_Control_0
// ================================


enum class JEITA_ISETC_t : uint8_t {

    JEITA_ISETC_CHARGE_SUSPEND = 0, 
    JEITA_ISETC_SET_ICHG_TO_20 = 1, 
    JEITA_ISETC_SET_ICHG_TO_40 = 2, 
    JEITA_ISETC_ICHG_UNCHANGED = 3
};

strings_vector_t JEITA_ISETC_strings = {

    "Charge Suspend", 
    "Set ICHG to 20%* ICHG (default)", 
    "Set ICHG to 40%* ICHG", 
    "ICHG unchanged"
};

Setting JEITA_ISETC = { REG17_NTC_Control_0, false, "JEITA_ISETC", "", settings_type_t::ENUM, 2, 1, false, 0, 3, 0, 1, settings_flags_t::NONE, JEITA_ISETC_strings };
JEITA_ISETC_t getJEITA_ISETC() { return static_cast<JEITA_ISETC_t>(getInt(JEITA_ISETC)); }
const char * getJEITA_ISETC_enum_string() { return getString(JEITA_ISETC); }
void setJEITA_ISETC(JEITA_ISETC_t val) { setEnum<JEITA_ISETC_t>(JEITA_ISETC, val); }




// ================================
// REG18_NTC_Control_1
// ================================


enum class TS_COOL_t : uint8_t {

    TS_COOL_5 = 0, 
    TS_COOL_10 = 1, 
    TS_COOL_15 = 2, 
    TS_COOL_20 = 3
};

strings_vector_t TS_COOL_strings = {

    "5'C", 
    "10'C (default)", 
    "15'C", 
    "20'C"
};

Setting TS_COOL = { REG18_NTC_Control_1, false, "TS_COOL", "", settings_type_t::ENUM, 2, 6, false, 0, 3, 0, 1, settings_flags_t::NONE, TS_COOL_strings };
TS_COOL_t getTS_COOL() { return static_cast<TS_COOL_t>(getInt(TS_COOL)); }
const char * getTS_COOL_enum_string() { return getString(TS_COOL); }
void setTS_COOL(TS_COOL_t val) { setEnum<TS_COOL_t>(TS_COOL, val); }




// ================================
// REG18_NTC_Control_1
// ================================


enum class TS_WARM_t : uint8_t {

    TS_WARM_40 = 0, 
    TS_WARM_45 = 1, 
    TS_WARM_50 = 2, 
    TS_WARM_55 = 3
};

strings_vector_t TS_WARM_strings = {

    "40'C", 
    "45'C (default)", 
    "50'C", 
    "55'C"
};

Setting TS_WARM = { REG18_NTC_Control_1, false, "TS_WARM", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, TS_WARM_strings };
TS_WARM_t getTS_WARM() { return static_cast<TS_WARM_t>(getInt(TS_WARM)); }
const char * getTS_WARM_enum_string() { return getString(TS_WARM); }
void setTS_WARM(TS_WARM_t val) { setEnum<TS_WARM_t>(TS_WARM, val); }




// ================================
// REG18_NTC_Control_1
// ================================


enum class BHOT_t : uint8_t {

    TS_BHOT_55 = 0, 
    TS_BHOT_60 = 1, 
    TS_BHOT_65 = 2, 
    TS_BHOT_DISABLE = 3
};

strings_vector_t BHOT_strings = {

    "55'C", 
    "60'C (default)", 
    "65'C", 
    "Disabled"
};

Setting BHOT = { REG18_NTC_Control_1, false, "BHOT", "", settings_type_t::ENUM, 2, 2, false, 0, 3, 0, 1, settings_flags_t::NONE, BHOT_strings };
BHOT_t getBHOT() { return static_cast<BHOT_t>(getInt(BHOT)); }
const char * getBHOT_enum_string() { return getString(BHOT); }
void setBHOT(BHOT_t val) { setEnum<BHOT_t>(BHOT, val); }




// ================================
// REG18_NTC_Control_1
// ================================


enum class BCOLD_t : uint8_t {

    TS_BCOLD_MINUS_10 = 0, 
    TS_BCOLD_MINUS_20 = 1
};

strings_vector_t BCOLD_strings = {

    "-10'C (default)", 
    "-20'C"
};

Setting BCOLD = { REG18_NTC_Control_1, false, "BCOLD", "", settings_type_t::ENUM, 1, 1, false, 0, 3, 0, 1, settings_flags_t::NONE, BCOLD_strings };
BCOLD_t getBCOLD() { return static_cast<BCOLD_t>(getInt(BCOLD)); }
const char * getBCOLD_enum_string() { return getString(BCOLD); }
void setBCOLD(BCOLD_t val) { setEnum<BCOLD_t>(BCOLD, val); }




// ================================
// REG18_NTC_Control_1
// ================================


Setting TS_IGNORE = { REG18_NTC_Control_1, false, "TS_IGNORE", "", settings_type_t::BOOL, 1, 0, false };
bool getTS_IGNORE() { return getBool(TS_IGNORE); }
void setTS_IGNORE(bool val) { setBool(TS_IGNORE, val); }



// ================================
// REG19_ICO_Current_Limit
// ================================


Setting ICO_ILIM = { REG19_ICO_Current_Limit, true, "ICO_ILIM", "mA", settings_type_t::INT, 9, 0, false, 100, 3300, 0, 10, settings_flags_t::NONE };
/// Returns ICO_ILIM in mA
int getICO_ILIM() { return getInt(ICO_ILIM); }



// ================================
// REG1B_Charger_Status_0
// ================================


enum class IINDPM_STAT_t : uint8_t {

    IINDPM_STAT_NORMAL = 0, 
    IINDPM_STAT_REGULATION = 1
};

strings_vector_t IINDPM_STAT_strings = {

    "Normal", 
    "In IINDPM regulation or IOTG regulation"
};

Setting IINDPM_STAT = { REG1B_Charger_Status_0, false, "IINDPM_STAT", "", settings_type_t::ENUM, 1, 7, false, 0, 1, 0, 1, settings_flags_t::NONE, IINDPM_STAT_strings };
IINDPM_STAT_t getIINDPM_STAT() { return static_cast<IINDPM_STAT_t>(getInt(IINDPM_STAT)); }
const char * getIINDPM_STAT_enum_string() { return getString(IINDPM_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class VINDPM_STAT_t : uint8_t {

    VINDPM_STAT_NORMAL = 0, 
    VINDPM_STAT_REGULATION = 1
};

strings_vector_t VINDPM_STAT_strings = {

    "Normal", 
    "In VINDPM regulation or VOTG regulation"
};

Setting VINDPM_STAT = { REG1B_Charger_Status_0, false, "VINDPM_STAT", "", settings_type_t::ENUM, 1, 6, false, 0, 1, 0, 1, settings_flags_t::NONE, VINDPM_STAT_strings };
VINDPM_STAT_t getVINDPM_STAT() { return static_cast<VINDPM_STAT_t>(getInt(VINDPM_STAT)); }
const char * getVINDPM_STAT_enum_string() { return getString(VINDPM_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class WD_STAT_t : uint8_t {

    WD_STAT_NORMAL = 0, 
    WD_STAT_EXPIRED = 1
};

strings_vector_t WD_STAT_strings = {

    "Normal", 
    "Watchdog timer expired"
};

Setting WD_STAT = { REG1B_Charger_Status_0, false, "WD_STAT", "", settings_type_t::ENUM, 1, 5, false, 0, 1, 0, 1, settings_flags_t::NONE, WD_STAT_strings };
WD_STAT_t getWD_STAT() { return static_cast<WD_STAT_t>(getInt(WD_STAT)); }
const char * getWD_STAT_enum_string() { return getString(WD_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class PG_STAT_t : uint8_t {

    PG_STAT_BAD = 0, 
    PG_STAT_GOOD = 1
};

strings_vector_t PG_STAT_strings = {

    "Not in power good status", 
    "Power good"
};

Setting PG_STAT = { REG1B_Charger_Status_0, false, "PG_STAT", "", settings_type_t::ENUM, 1, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, PG_STAT_strings };
PG_STAT_t getPG_STAT() { return static_cast<PG_STAT_t>(getInt(PG_STAT)); }
const char * getPG_STAT_enum_string() { return getString(PG_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class AC2_PRESENT_STAT_t : uint8_t {

    AC2_PRESENT_STAT_NOT_PRESENT = 0, 
    AC2_PRESENT_STAT_PRESENT = 1
};

strings_vector_t AC2_PRESENT_STAT_strings = {

    "VAC2 NOT present", 
    "VAC2 present (above present threshold)"
};

Setting AC2_PRESENT_STAT = { REG1B_Charger_Status_0, false, "AC2_PRESENT_STAT", "", settings_type_t::ENUM, 1, 2, false, 0, 1, 0, 1, settings_flags_t::NONE, AC2_PRESENT_STAT_strings };
AC2_PRESENT_STAT_t getAC2_PRESENT_STAT() { return static_cast<AC2_PRESENT_STAT_t>(getInt(AC2_PRESENT_STAT)); }
const char * getAC2_PRESENT_STAT_enum_string() { return getString(AC2_PRESENT_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class AC1_PRESENT_STAT_t : uint8_t {

    AC1_PRESENT_STAT_NOT_PRESENT = 0, 
    AC1_PRESENT_STAT_PRESENT = 1
};

strings_vector_t AC1_PRESENT_STAT_strings = {

    "VAC1 NOT present", 
    "VAC1 present (above present threshold)"
};

Setting AC1_PRESENT_STAT = { REG1B_Charger_Status_0, false, "AC1_PRESENT_STAT", "", settings_type_t::ENUM, 1, 1, false, 0, 1, 0, 1, settings_flags_t::NONE, AC1_PRESENT_STAT_strings };
AC1_PRESENT_STAT_t getAC1_PRESENT_STAT() { return static_cast<AC1_PRESENT_STAT_t>(getInt(AC1_PRESENT_STAT)); }
const char * getAC1_PRESENT_STAT_enum_string() { return getString(AC1_PRESENT_STAT); }




// ================================
// REG1B_Charger_Status_0
// ================================


enum class VBUS_PRESENT_STAT_t : uint8_t {

    VBUS_PRESENT_STAT_NOT_PRESENT = 0, 
    VBUS_PRESENT_STAT_PRESENT = 1
};

strings_vector_t VBUS_PRESENT_STAT_strings = {

    "VBUS NOT present", 
    "VBUS present (above present threshold)"
};

Setting VBUS_PRESENT_STAT = { REG1B_Charger_Status_0, false, "VBUS_PRESENT_STAT", "", settings_type_t::ENUM, 1, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, VBUS_PRESENT_STAT_strings };
VBUS_PRESENT_STAT_t getVBUS_PRESENT_STAT() { return static_cast<VBUS_PRESENT_STAT_t>(getInt(VBUS_PRESENT_STAT)); }
const char * getVBUS_PRESENT_STAT_enum_string() { return getString(VBUS_PRESENT_STAT); }




// ================================
// REG1C_Charger_Status_1
// ================================


enum class CHG_STAT_t : uint8_t {

    CHG_STAT_NOT_CHARGING = 0, 
    CHG_STAT_TRICKLECHARGE = 1, 
    CHG_STAT_PRECHARGE = 2, 
    CHG_STAT_FASTCHARGE_CC = 3, 
    CHG_STAT_FASTCHARGE_CV = 4, 
    CHG_STAT_RESERVED_5 = 5, 
    CHG_STAT_TOPOFF = 6, 
    CHG_STAT_TERMINATED = 7
};

strings_vector_t CHG_STAT_strings = {

    "Not Charging", 
    "Trickle Charge", 
    "Pre-charge", 
    "Fast Charge (CC mode)", 
    "Taper Charge (CV mode)", 
    "Reserved", 
    "Top-off Timer Active Charging", 
    "Charge Termination Done"
};

Setting CHG_STAT = { REG1C_Charger_Status_1, false, "CHG_STAT", "", settings_type_t::ENUM, 3, 5, false, 0, 1, 0, 1, settings_flags_t::NONE, CHG_STAT_strings };
CHG_STAT_t getCHG_STAT() { return static_cast<CHG_STAT_t>(getInt(CHG_STAT)); }
const char * getCHG_STAT_enum_string() { return getString(CHG_STAT); }




// ================================
// REG1C_Charger_Status_1
// ================================


enum class VBUS_STAT_t : uint8_t {

    VBUS_STAT_NO_INPUT = 0, 
    VBUS_STAT_USB_SDP = 1, 
    VBUS_STAT_USB_CDP = 2, 
    VBUS_STAT_USB_DCP = 3, 
    VBUS_STAT_ADJUSTABLE_HVDCP = 4, 
    VBUS_STAT_UNKNOWN_ADAPTOR = 5, 
    VBUS_STAT_NON_STANDARD_ADAPTER = 6, 
    VBUS_STAT_OTG_MODE = 7, 
    VBUS_STAT_NOT_QUALIFIED_ADAPTOR = 8, 
    VBUS_STAT_RESERVED_9 = 9, 
    VBUS_STAT_RESERVED_A = 10, 
    VBUS_STAT_DEVICE_POWERED_FROM_VBUS = 11, 
    VBUS_STAT_BACKUP_MODE = 12, 
    VBUS_STAT_RESERVED_D = 13, 
    VBUS_STAT_RESERVED_E = 14, 
    VBUS_STAT_RESERVED_F = 15
};

strings_vector_t VBUS_STAT_strings = {

    "No Input or BHOT or BCOLD in OTG mode", 
    "USB SDP (500mA)", 
    "USB CDP (1.5A)", 
    "USB DCP (3.25A)", 
    "Adjustable High Voltage DCP (HVDCP) (1.5A)", 
    "Unknown adaptor (3A)", 
    "Non-Standard Adapter (1A/2A/2.A/.4A)", 
    "In OTG mode", 
    "Not qualified adaptor", 
    "Reserved", 
    "Reserved", 
    "Device directly powered from VBUS", 
    "Backup Mode", 
    "Reserved", 
    "Reserved", 
    "Reserved"
};

Setting VBUS_STAT = { REG1C_Charger_Status_1, false, "VBUS_STAT", "", settings_type_t::ENUM, 4, 1, false, 0, 1, 0, 1, settings_flags_t::NONE, VBUS_STAT_strings };
VBUS_STAT_t getVBUS_STAT() { return static_cast<VBUS_STAT_t>(getInt(VBUS_STAT)); }
const char * getVBUS_STAT_enum_string() { return getString(VBUS_STAT); }




// ================================
// REG1C_Charger_Status_1
// ================================


Setting BC12_DONE_STAT = { REG1C_Charger_Status_1, false, "BC12_DONE_STAT", "", settings_type_t::BOOL, 1, 0, false };
bool getBC12_DONE_STAT() { return getBool(BC12_DONE_STAT); }



// ================================
// REG1D_Charger_Status_2
// ================================


enum class ICO_STAT_t : uint8_t {

    ICO_STAT_DISABLED = 0, 
    ICO_STAT_IN_PROGRESS = 1, 
    ICO_STAT_MAX_CURRENT_DETECTED = 2, 
    ICO_STAT_RESERVED = 3
};

strings_vector_t ICO_STAT_strings = {

    "ICO disabled", 
    "ICO optimization in progress", 
    "Maximum input current detected", 
    "Reserved"
};

Setting ICO_STAT = { REG1D_Charger_Status_2, false, "ICO_STAT", "", settings_type_t::ENUM, 2, 6, false, 0, 1, 0, 1, settings_flags_t::NONE, ICO_STAT_strings };
ICO_STAT_t getICO_STAT() { return static_cast<ICO_STAT_t>(getInt(ICO_STAT)); }
const char * getICO_STAT_enum_string() { return getString(ICO_STAT); }




// ================================
// REG1D_Charger_Status_2
// ================================


enum class TREG_STAT_t : uint8_t {

    TREG_STAT_NORMAL = 0, 
    TREG_STAT_THERMAL_REGULATION = 1
};

strings_vector_t TREG_STAT_strings = {

    "Normal", 
    "Device in thermal regulation"
};

Setting TREG_STAT = { REG1D_Charger_Status_2, false, "TREG_STAT", "", settings_type_t::ENUM, 1, 5, false, 0, 1, 0, 1, settings_flags_t::NONE, TREG_STAT_strings };
TREG_STAT_t getTREG_STAT() { return static_cast<TREG_STAT_t>(getInt(TREG_STAT)); }
const char * getTREG_STAT_enum_string() { return getString(TREG_STAT); }




// ================================
// REG1D_Charger_Status_2
// ================================


enum class DPDM_STAT_t : uint8_t {

    DPDM_STAT_NOT_STARTED = 0, 
    DPDM_STAT_IN_PROGRESS = 1
};

strings_vector_t DPDM_STAT_strings = {

    "D+/D- detection NOT started yet or done", 
    "D+/D- detection in progress"
};

Setting DPDM_STAT = { REG1D_Charger_Status_2, false, "DPDM_STAT", "", settings_type_t::ENUM, 1, 4, false, 0, 1, 0, 1, settings_flags_t::NONE, DPDM_STAT_strings };
DPDM_STAT_t getDPDM_STAT() { return static_cast<DPDM_STAT_t>(getInt(DPDM_STAT)); }
const char * getDPDM_STAT_enum_string() { return getString(DPDM_STAT); }




// ================================
// REG1D_Charger_Status_2
// ================================


enum class VBAT_PRESENT_STAT_t : uint8_t {

    VBAT_PRESENT_STAT_NOT_PRESENT = 0, 
    VBAT_PRESENT_STAT_PRESENT = 1
};

strings_vector_t VBAT_PRESENT_STAT_strings = {

    "VBAT NOT present", 
    "VBAT present"
};

Setting VBAT_PRESENT_STAT = { REG1D_Charger_Status_2, false, "VBAT_PRESENT_STAT", "", settings_type_t::ENUM, 1, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, VBAT_PRESENT_STAT_strings };
VBAT_PRESENT_STAT_t getVBAT_PRESENT_STAT() { return static_cast<VBAT_PRESENT_STAT_t>(getInt(VBAT_PRESENT_STAT)); }
const char * getVBAT_PRESENT_STAT_enum_string() { return getString(VBAT_PRESENT_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


enum class ACRB2_STAT_t : uint8_t {

    ACRB2_STAT_OFF = 0, 
    ACRB2_STAT_ON = 1
};

strings_vector_t ACRB2_STAT_strings = {

    "ACFET2/RBFET2 Open (Path Disabled)", 
    "ACFET2/RBFET2 Closed (Path Enabled)"
};

Setting ACRB2_STAT = { REG1E_Charger_Status_3, false, "ACRB2_STAT", "", settings_type_t::ENUM, 1, 7, false, 0, 1, 0, 1, settings_flags_t::NONE, ACRB2_STAT_strings };
ACRB2_STAT_t getACRB2_STAT() { return static_cast<ACRB2_STAT_t>(getInt(ACRB2_STAT)); }
const char * getACRB2_STAT_enum_string() { return getString(ACRB2_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


enum class ACRB1_STAT_t : uint8_t {

    ACRB1_STAT_OFF = 0, 
    ACRB1_STAT_ON = 1
};

strings_vector_t ACRB1_STAT_strings = {

    "ACFET1/RBFET1 Open (Path Disabled)", 
    "ACFET1/RBFET1 Closed (Path Enabled)"
};

Setting ACRB1_STAT = { REG1E_Charger_Status_3, false, "ACRB1_STAT", "", settings_type_t::ENUM, 1, 6, false, 0, 1, 0, 1, settings_flags_t::NONE, ACRB1_STAT_strings };
ACRB1_STAT_t getACRB1_STAT() { return static_cast<ACRB1_STAT_t>(getInt(ACRB1_STAT)); }
const char * getACRB1_STAT_enum_string() { return getString(ACRB1_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


Setting ADC_DONE_STAT = { REG1E_Charger_Status_3, false, "ADC_DONE_STAT", "", settings_type_t::BOOL, 1, 5, false };
bool getADC_DONE_STAT() { return getBool(ADC_DONE_STAT); }



// ================================
// REG1E_Charger_Status_3
// ================================


enum class VSYS_STAT_t : uint8_t {

    VSYS_STAT_NOT_IN_VSYSMIN_REGULATION = 0, 
    VSYS_STAT_IN_VSYSMIN_REGULATION = 1
};

strings_vector_t VSYS_STAT_strings = {

    "Not in VSYSMIN regulation (VBAT > VSYSMIN)", 
    "In VSYSMIN regulation (VBAT < VSYSMIN)"
};

Setting VSYS_STAT = { REG1E_Charger_Status_3, false, "VSYS_STAT", "", settings_type_t::ENUM, 1, 4, false, 0, 1, 0, 1, settings_flags_t::NONE, VSYS_STAT_strings };
VSYS_STAT_t getVSYS_STAT() { return static_cast<VSYS_STAT_t>(getInt(VSYS_STAT)); }
const char * getVSYS_STAT_enum_string() { return getString(VSYS_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


enum class CHG_TMR_STAT_t : uint8_t {

    CHG_TMR_STAT_NORMAL = 0, 
    CHG_TMR_STAT_SAFETY_TIMER_EXPIRED = 1
};

strings_vector_t CHG_TMR_STAT_strings = {

    "Normal", 
    "Safety timer expired"
};

Setting CHG_TMR_STAT = { REG1E_Charger_Status_3, false, "CHG_TMR_STAT", "", settings_type_t::ENUM, 1, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, CHG_TMR_STAT_strings };
CHG_TMR_STAT_t getCHG_TMR_STAT() { return static_cast<CHG_TMR_STAT_t>(getInt(CHG_TMR_STAT)); }
const char * getCHG_TMR_STAT_enum_string() { return getString(CHG_TMR_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


enum class TRICHG_TMR_STAT_t : uint8_t {

    TRICHG_TMR_STAT_NORMAL = 0, 
    TRICHG_TMR_STAT_SAFETY_TIMER_EXPIRED = 1
};

strings_vector_t TRICHG_TMR_STAT_strings = {

    "Normal", 
    "Safety timer expired"
};

Setting TRICHG_TMR_STAT = { REG1E_Charger_Status_3, false, "TRICHG_TMR_STAT", "", settings_type_t::ENUM, 1, 2, false, 0, 1, 0, 1, settings_flags_t::NONE, TRICHG_TMR_STAT_strings };
TRICHG_TMR_STAT_t getTRICHG_TMR_STAT() { return static_cast<TRICHG_TMR_STAT_t>(getInt(TRICHG_TMR_STAT)); }
const char * getTRICHG_TMR_STAT_enum_string() { return getString(TRICHG_TMR_STAT); }




// ================================
// REG1E_Charger_Status_3
// ================================


enum class PRECHG_TMR_STAT_t : uint8_t {

    PRECHG_TMR_STAT_NORMAL = 0, 
    PRECHG_TMR_STAT_SAFETY_TIMER_EXPIRED = 1
};

strings_vector_t PRECHG_TMR_STAT_strings = {

    "Normal", 
    "Safety timer expired"
};

Setting PRECHG_TMR_STAT = { REG1E_Charger_Status_3, false, "PRECHG_TMR_STAT", "", settings_type_t::ENUM, 1, 1, false, 0, 1, 0, 1, settings_flags_t::NONE, PRECHG_TMR_STAT_strings };
PRECHG_TMR_STAT_t getPRECHG_TMR_STAT() { return static_cast<PRECHG_TMR_STAT_t>(getInt(PRECHG_TMR_STAT)); }
const char * getPRECHG_TMR_STAT_enum_string() { return getString(PRECHG_TMR_STAT); }




// ================================
// REG1F_Charger_Status_4
// ================================


enum class VBATOTG_LOW_STAT_t : uint8_t {

    VBATOTG_LOW_STAT_VBATOTG_OK = 0, 
    VBATOTG_LOW_STAT_VBATOTG_LOW = 1
};

strings_vector_t VBATOTG_LOW_STAT_strings = {

    "VBAT is high enough to enable OTG operation", 
    "VBAT is too low to enable OTG mode"
};

Setting VBATOTG_LOW_STAT = { REG1F_Charger_Status_4, false, "VBATOTG_LOW_STAT", "", settings_type_t::ENUM, 1, 4, false, 0, 1, 0, 1, settings_flags_t::NONE, VBATOTG_LOW_STAT_strings };
VBATOTG_LOW_STAT_t getVBATOTG_LOW_STAT() { return static_cast<VBATOTG_LOW_STAT_t>(getInt(VBATOTG_LOW_STAT)); }
const char * getVBATOTG_LOW_STAT_enum_string() { return getString(VBATOTG_LOW_STAT); }




// ================================
// REG1F_Charger_Status_4
// ================================


enum class TS_COLD_STAT_t : uint8_t {

    TS_COLD_STAT_NOT_COLD = 0, 
    TS_COLD_STAT_COLD = 1
};

strings_vector_t TS_COLD_STAT_strings = {

    "TS NOT in cold range", 
    "TS in cold range"
};

Setting TS_COLD_STAT = { REG1F_Charger_Status_4, false, "TS_COLD_STAT", "", settings_type_t::ENUM, 1, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, TS_COLD_STAT_strings };
TS_COLD_STAT_t getTS_COLD_STAT() { return static_cast<TS_COLD_STAT_t>(getInt(TS_COLD_STAT)); }
const char * getTS_COLD_STAT_enum_string() { return getString(TS_COLD_STAT); }




// ================================
// REG1F_Charger_Status_4
// ================================


enum class TS_COOL_STAT_t : uint8_t {

    TS_COOL_STAT_NOT_COOL = 0, 
    TS_COOL_STAT_COOL = 1
};

strings_vector_t TS_COOL_STAT_strings = {

    "TS NOT in cool range", 
    "TS in cool range"
};

Setting TS_COOL_STAT = { REG1F_Charger_Status_4, false, "TS_COOL_STAT", "", settings_type_t::ENUM, 1, 2, false, 0, 1, 0, 1, settings_flags_t::NONE, TS_COOL_STAT_strings };
TS_COOL_STAT_t getTS_COOL_STAT() { return static_cast<TS_COOL_STAT_t>(getInt(TS_COOL_STAT)); }
const char * getTS_COOL_STAT_enum_string() { return getString(TS_COOL_STAT); }




// ================================
// REG1F_Charger_Status_4
// ================================


enum class TS_WARM_STAT_t : uint8_t {

    TS_WARM_STAT_NOT_WARM = 0, 
    TS_WARM_STAT_WARM = 1
};

strings_vector_t TS_WARM_STAT_strings = {

    "TS NOT in warm range", 
    "TS in warm range"
};

Setting TS_WARM_STAT = { REG1F_Charger_Status_4, false, "TS_WARM_STAT", "", settings_type_t::ENUM, 1, 1, false, 0, 1, 0, 1, settings_flags_t::NONE, TS_WARM_STAT_strings };
TS_WARM_STAT_t getTS_WARM_STAT() { return static_cast<TS_WARM_STAT_t>(getInt(TS_WARM_STAT)); }
const char * getTS_WARM_STAT_enum_string() { return getString(TS_WARM_STAT); }




// ================================
// REG1F_Charger_Status_4
// ================================


enum class TS_HOT_STAT_t : uint8_t {

    TS_HOT_STAT_NOT_HOT = 0, 
    TS_HOT_STAT_HOT = 1
};

strings_vector_t TS_HOT_STAT_strings = {

    "TS NOT in hot range", 
    "TS in hot range"
};

Setting TS_HOT_STAT = { REG1F_Charger_Status_4, false, "TS_HOT_STAT", "", settings_type_t::ENUM, 1, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, TS_HOT_STAT_strings };
TS_HOT_STAT_t getTS_HOT_STAT() { return static_cast<TS_HOT_STAT_t>(getInt(TS_HOT_STAT)); }
const char * getTS_HOT_STAT_enum_string() { return getString(TS_HOT_STAT); }




// ================================
// REG20_FAULT_Status_0
// ================================


Setting IBAT_REG_STAT = { REG20_FAULT_Status_0, false, "IBAT_REG_STAT", "", settings_type_t::BOOL, 1, 7, false };
bool getIBAT_REG_STAT() { return getBool(IBAT_REG_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting VBUS_OVP_STAT = { REG20_FAULT_Status_0, false, "VBUS_OVP_STAT", "", settings_type_t::BOOL, 1, 6, false };
bool getVBUS_OVP_STAT() { return getBool(VBUS_OVP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting VBAT_OVP_STAT = { REG20_FAULT_Status_0, false, "VBAT_OVP_STAT", "", settings_type_t::BOOL, 1, 5, false };
bool getVBAT_OVP_STAT() { return getBool(VBAT_OVP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting IBUS_OCP_STAT = { REG20_FAULT_Status_0, false, "IBUS_OCP_STAT", "", settings_type_t::BOOL, 1, 4, false };
bool getIBUS_OCP_STAT() { return getBool(IBUS_OCP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting IBAT_OCP_STAT = { REG20_FAULT_Status_0, false, "IBAT_OCP_STAT", "", settings_type_t::BOOL, 1, 3, false };
bool getIBAT_OCP_STAT() { return getBool(IBAT_OCP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting CONV_OCP_STAT = { REG20_FAULT_Status_0, false, "CONV_OCP_STAT", "", settings_type_t::BOOL, 1, 2, false };
bool getCONV_OCP_STAT() { return getBool(CONV_OCP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting VAC2_OVP_STAT = { REG20_FAULT_Status_0, false, "VAC2_OVP_STAT", "", settings_type_t::BOOL, 1, 1, false };
bool getVAC2_OVP_STAT() { return getBool(VAC2_OVP_STAT); }



// ================================
// REG20_FAULT_Status_0
// ================================


Setting VAC1_OVP_STAT = { REG20_FAULT_Status_0, false, "VAC1_OVP_STAT", "", settings_type_t::BOOL, 1, 0, false };
bool getVAC1_OVP_STAT() { return getBool(VAC1_OVP_STAT); }



// ================================
// REG21_FAULT_Status_1
// ================================


Setting VSYS_SHORT_STAT = { REG21_FAULT_Status_1, false, "VSYS_SHORT_STAT", "", settings_type_t::BOOL, 1, 7, false };
bool getVSYS_SHORT_STAT() { return getBool(VSYS_SHORT_STAT); }



// ================================
// REG21_FAULT_Status_1
// ================================


Setting VSYS_OVP_STAT = { REG21_FAULT_Status_1, false, "VSYS_OVP_STAT", "", settings_type_t::BOOL, 1, 6, false };
bool getVSYS_OVP_STAT() { return getBool(VSYS_OVP_STAT); }



// ================================
// REG21_FAULT_Status_1
// ================================


Setting OTG_OVP_STAT = { REG21_FAULT_Status_1, false, "OTG_OVP_STAT", "", settings_type_t::BOOL, 1, 5, false };
bool getOTG_OVP_STAT() { return getBool(OTG_OVP_STAT); }



// ================================
// REG21_FAULT_Status_1
// ================================


Setting OTG_UVP_STAT = { REG21_FAULT_Status_1, false, "OTG_UVP_STAT", "", settings_type_t::BOOL, 1, 4, false };
bool getOTG_UVP_STAT() { return getBool(OTG_UVP_STAT); }



// ================================
// REG21_FAULT_Status_1
// ================================


Setting TSHUT_STAT = { REG21_FAULT_Status_1, false, "TSHUT_STAT", "", settings_type_t::BOOL, 1, 2, false };
bool getTSHUT_STAT() { return getBool(TSHUT_STAT); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting IINDPM_FLAG = { REG22_Charger_Flag_0, false, "IINDPM_FLAG", "", settings_type_t::BOOL, 1, 7, true };
bool getIINDPM_FLAG() { return getBool(IINDPM_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting VINDPM_FLAG = { REG22_Charger_Flag_0, false, "VINDPM_FLAG", "", settings_type_t::BOOL, 1, 6, true };
bool getVINDPM_FLAG() { return getBool(VINDPM_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting WD_FLAG = { REG22_Charger_Flag_0, false, "WD_FLAG", "", settings_type_t::BOOL, 1, 5, true };
bool getWD_FLAG() { return getBool(WD_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting POORSRC_FLAG = { REG22_Charger_Flag_0, false, "POORSRC_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getPOORSRC_FLAG() { return getBool(POORSRC_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting PG_FLAG = { REG22_Charger_Flag_0, false, "PG_FLAG", "", settings_type_t::BOOL, 1, 3, true };
bool getPG_FLAG() { return getBool(PG_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting AC2_PRESENT_FLAG = { REG22_Charger_Flag_0, false, "AC2_PRESENT_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getAC2_PRESENT_FLAG() { return getBool(AC2_PRESENT_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting AC1_PRESENT_FLAG = { REG22_Charger_Flag_0, false, "AC1_PRESENT_FLAG", "", settings_type_t::BOOL, 1, 1, true };
bool getAC1_PRESENT_FLAG() { return getBool(AC1_PRESENT_FLAG); }



// ================================
// REG22_Charger_Flag_0
// ================================


Setting VBUS_PRESENT_FLAG = { REG22_Charger_Flag_0, false, "VBUS_PRESENT_FLAG", "", settings_type_t::BOOL, 1, 0, true };
bool getVBUS_PRESENT_FLAG() { return getBool(VBUS_PRESENT_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting CHG_FLAG = { REG23_Charger_Flag_1, false, "CHG_FLAG", "", settings_type_t::BOOL, 1, 7, true };
bool getCHG_FLAG() { return getBool(CHG_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting ICO_FLAG = { REG23_Charger_Flag_1, false, "ICO_FLAG", "", settings_type_t::BOOL, 1, 6, true };
bool getICO_FLAG() { return getBool(ICO_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting VBUS_FLAG = { REG23_Charger_Flag_1, false, "VBUS_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getVBUS_FLAG() { return getBool(VBUS_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting TREG_FLAG = { REG23_Charger_Flag_1, false, "TREG_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getTREG_FLAG() { return getBool(TREG_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting VBAT_PRESENT_FLAG = { REG23_Charger_Flag_1, false, "VBAT_PRESENT_FLAG", "", settings_type_t::BOOL, 1, 1, true };
bool getVBAT_PRESENT_FLAG() { return getBool(VBAT_PRESENT_FLAG); }



// ================================
// REG23_Charger_Flag_1
// ================================


Setting BC1_2_DONE_FLAG = { REG23_Charger_Flag_1, false, "BC1_2_DONE_FLAG", "", settings_type_t::BOOL, 1, 0, true };
bool getBC1_2_DONE_FLAG() { return getBool(BC1_2_DONE_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting DPDM_DONE_FLAG = { REG24_Charger_Flag_2, false, "DPDM_DONE_FLAG", "", settings_type_t::BOOL, 1, 6, true };
bool getDPDM_DONE_FLAG() { return getBool(DPDM_DONE_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting ADC_DONE_FLAG = { REG24_Charger_Flag_2, false, "ADC_DONE_FLAG", "", settings_type_t::BOOL, 1, 5, true };
bool getADC_DONE_FLAG() { return getBool(ADC_DONE_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting VSYS_FLAG = { REG24_Charger_Flag_2, false, "VSYS_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getVSYS_FLAG() { return getBool(VSYS_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting CHG_TMR_FLAG = { REG24_Charger_Flag_2, false, "CHG_TMR_FLAG", "", settings_type_t::BOOL, 1, 3, true };
bool getCHG_TMR_FLAG() { return getBool(CHG_TMR_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting TRICHG_TMR_FLAG = { REG24_Charger_Flag_2, false, "TRICHG_TMR_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getTRICHG_TMR_FLAG() { return getBool(TRICHG_TMR_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting PRECHG_TMR_FLAG = { REG24_Charger_Flag_2, false, "PRECHG_TMR_FLAG", "", settings_type_t::BOOL, 1, 1, true };
bool getPRECHG_TMR_FLAG() { return getBool(PRECHG_TMR_FLAG); }



// ================================
// REG24_Charger_Flag_2
// ================================


Setting TOPOFF_TMR_FLAG = { REG24_Charger_Flag_2, false, "TOPOFF_TMR_FLAG", "", settings_type_t::BOOL, 1, 0, true };
bool getTOPOFF_TMR_FLAG() { return getBool(TOPOFF_TMR_FLAG); }



// ================================
// REG25_Charger_Flag_3
// ================================


Setting VBATOTG_LOW_FLAG = { REG25_Charger_Flag_3, false, "VBATOTG_LOW_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getVBATOTG_LOW_FLAG() { return getBool(VBATOTG_LOW_FLAG); }



// ================================
// REG25_Charger_Flag_3
// ================================


Setting TS_COLD_FLAG = { REG25_Charger_Flag_3, false, "TS_COLD_FLAG", "", settings_type_t::BOOL, 1, 3, true };
bool getTS_COLD_FLAG() { return getBool(TS_COLD_FLAG); }



// ================================
// REG25_Charger_Flag_3
// ================================


Setting TS_COOL_FLAG = { REG25_Charger_Flag_3, false, "TS_COOL_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getTS_COOL_FLAG() { return getBool(TS_COOL_FLAG); }



// ================================
// REG25_Charger_Flag_3
// ================================


Setting TS_WARM_FLAG = { REG25_Charger_Flag_3, false, "TS_WARM_FLAG", "", settings_type_t::BOOL, 1, 1, true };
bool getTS_WARM_FLAG() { return getBool(TS_WARM_FLAG); }



// ================================
// REG25_Charger_Flag_3
// ================================


Setting TS_HOT_FLAG = { REG25_Charger_Flag_3, false, "TS_HOT_FLAG", "", settings_type_t::BOOL, 1, 0, true };
bool getTS_HOT_FLAG() { return getBool(TS_HOT_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting IBAT_REG_FLAG = { REG26_FAULT_Flag_0, false, "IBAT_REG_FLAG", "", settings_type_t::BOOL, 1, 7, true };
bool getIBAT_REG_FLAG() { return getBool(IBAT_REG_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting VBUS_OVP_FLAG = { REG26_FAULT_Flag_0, false, "VBUS_OVP_FLAG", "", settings_type_t::BOOL, 1, 6, true };
bool getVBUS_OVP_FLAG() { return getBool(VBUS_OVP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting VBAT_OVP_FLAG = { REG26_FAULT_Flag_0, false, "VBAT_OVP_FLAG", "", settings_type_t::BOOL, 1, 5, true };
bool getVBAT_OVP_FLAG() { return getBool(VBAT_OVP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting IBUS_OCP_FLAG = { REG26_FAULT_Flag_0, false, "IBUS_OCP_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getIBUS_OCP_FLAG() { return getBool(IBUS_OCP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting IBAT_OCP_FLAG = { REG26_FAULT_Flag_0, false, "IBAT_OCP_FLAG", "", settings_type_t::BOOL, 1, 3, true };
bool getIBAT_OCP_FLAG() { return getBool(IBAT_OCP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting CONV_OCP_FLAG = { REG26_FAULT_Flag_0, false, "CONV_OCP_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getCONV_OCP_FLAG() { return getBool(CONV_OCP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting VAC2_OVP_FLAG = { REG26_FAULT_Flag_0, false, "VAC2_OVP_FLAG", "", settings_type_t::BOOL, 1, 1, true };
bool getVAC2_OVP_FLAG() { return getBool(VAC2_OVP_FLAG); }



// ================================
// REG26_FAULT_Flag_0
// ================================


Setting VAC1_OVP_FLAG = { REG26_FAULT_Flag_0, false, "VAC1_OVP_FLAG", "", settings_type_t::BOOL, 1, 0, true };
bool getVAC1_OVP_FLAG() { return getBool(VAC1_OVP_FLAG); }



// ================================
// REG27_FAULT_Flag_1
// ================================


Setting VSYS_SHORT_FLAG = { REG27_FAULT_Flag_1, false, "VSYS_SHORT_FLAG", "", settings_type_t::BOOL, 1, 7, true };
bool getVSYS_SHORT_FLAG() { return getBool(VSYS_SHORT_FLAG); }



// ================================
// REG27_FAULT_Flag_1
// ================================


Setting VSYS_OVP_FLAG = { REG27_FAULT_Flag_1, false, "VSYS_OVP_FLAG", "", settings_type_t::BOOL, 1, 6, true };
bool getVSYS_OVP_FLAG() { return getBool(VSYS_OVP_FLAG); }



// ================================
// REG27_FAULT_Flag_1
// ================================


Setting OTG_OVP_FLAG = { REG27_FAULT_Flag_1, false, "OTG_OVP_FLAG", "", settings_type_t::BOOL, 1, 5, true };
bool getOTG_OVP_FLAG() { return getBool(OTG_OVP_FLAG); }



// ================================
// REG27_FAULT_Flag_1
// ================================


Setting OTG_UVP_FLAG = { REG27_FAULT_Flag_1, false, "OTG_UVP_FLAG", "", settings_type_t::BOOL, 1, 4, true };
bool getOTG_UVP_FLAG() { return getBool(OTG_UVP_FLAG); }



// ================================
// REG27_FAULT_Flag_1
// ================================


Setting TSHUT_FLAG = { REG27_FAULT_Flag_1, false, "TSHUT_FLAG", "", settings_type_t::BOOL, 1, 2, true };
bool getTSHUT_FLAG() { return getBool(TSHUT_FLAG); }



// ================================
// REG2E_ADC_Control
// ================================


Setting ADC_EN = { REG2E_ADC_Control, false, "ADC_EN", "", settings_type_t::BOOL, 1, 7, false };
bool getADC_EN() { return getBool(ADC_EN); }
void setADC_EN(bool val) { setBool(ADC_EN, val); }



// ================================
// REG2E_ADC_Control
// ================================


enum class ADC_RATE_t : uint8_t {

    ADC_RATE_CONTINUOUS = 0, 
    ADC_RATE_ONESHOT = 1
};

strings_vector_t ADC_RATE_strings = {

    "Continuous", 
    "One-shot"
};

Setting ADC_RATE = { REG2E_ADC_Control, false, "ADC_RATE", "", settings_type_t::ENUM, 1, 6, false, 0, 1, 0, 1, settings_flags_t::NONE, ADC_RATE_strings };
ADC_RATE_t getADC_RATE() { return static_cast<ADC_RATE_t>(getInt(ADC_RATE)); }
const char * getADC_RATE_enum_string() { return getString(ADC_RATE); }
void setADC_RATE(ADC_RATE_t val) { setEnum<ADC_RATE_t>(ADC_RATE, val); }




// ================================
// REG2E_ADC_Control
// ================================


enum class ADC_SAMPLE_t : uint8_t {

    ADC_SAMPLE_15BIT = 0, 
    ADC_SAMPLE_14BIT = 1, 
    ADC_SAMPLE_13BIT = 2, 
    ADC_SAMPLE_12BIT = 3
};

strings_vector_t ADC_SAMPLE_strings = {

    "15-bit", 
    "14-bit", 
    "13-bit", 
    "12-bit"
};

Setting ADC_SAMPLE = { REG2E_ADC_Control, false, "ADC_SAMPLE", "", settings_type_t::ENUM, 2, 4, false, 0, 3, 0, 1, settings_flags_t::NONE, ADC_SAMPLE_strings };
ADC_SAMPLE_t getADC_SAMPLE() { return static_cast<ADC_SAMPLE_t>(getInt(ADC_SAMPLE)); }
const char * getADC_SAMPLE_enum_string() { return getString(ADC_SAMPLE); }
void setADC_SAMPLE(ADC_SAMPLE_t val) { setEnum<ADC_SAMPLE_t>(ADC_SAMPLE, val); }




// ================================
// REG2E_ADC_Control
// ================================


enum class ADC_AVG_t : uint8_t {

    ADC_AVG_NO_AVERAGING = 0, 
    ADC_AVG_RUNNING_AVERAGE = 1
};

strings_vector_t ADC_AVG_strings = {

    "No averaging", 
    "Running average"
};

Setting ADC_AVG = { REG2E_ADC_Control, false, "ADC_AVG", "", settings_type_t::ENUM, 1, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, ADC_AVG_strings };
ADC_AVG_t getADC_AVG() { return static_cast<ADC_AVG_t>(getInt(ADC_AVG)); }
const char * getADC_AVG_enum_string() { return getString(ADC_AVG); }
void setADC_AVG(ADC_AVG_t val) { setEnum<ADC_AVG_t>(ADC_AVG, val); }




// ================================
// REG2E_ADC_Control
// ================================


Setting ADC_AVG_INIT = { REG2E_ADC_Control, false, "ADC_AVG_INIT", "", settings_type_t::BOOL, 1, 2, false };
bool getADC_AVG_INIT() { return getBool(ADC_AVG_INIT); }
void setADC_AVG_INIT(bool val) { setBool(ADC_AVG_INIT, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting IBUS_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "IBUS_ADC_DIS", "", settings_type_t::BOOL, 1, 7, false };
bool getIBUS_ADC_DIS() { return getBool(IBUS_ADC_DIS); }
void setIBUS_ADC_DIS(bool val) { setBool(IBUS_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting IBAT_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "IBAT_ADC_DIS", "", settings_type_t::BOOL, 1, 6, false };
bool getIBAT_ADC_DIS() { return getBool(IBAT_ADC_DIS); }
void setIBAT_ADC_DIS(bool val) { setBool(IBAT_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting VBUS_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "VBUS_ADC_DIS", "", settings_type_t::BOOL, 1, 5, false };
bool getVBUS_ADC_DIS() { return getBool(VBUS_ADC_DIS); }
void setVBUS_ADC_DIS(bool val) { setBool(VBUS_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting VBAT_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "VBAT_ADC_DIS", "", settings_type_t::BOOL, 1, 4, false };
bool getVBAT_ADC_DIS() { return getBool(VBAT_ADC_DIS); }
void setVBAT_ADC_DIS(bool val) { setBool(VBAT_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting VSYS_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "VSYS_ADC_DIS", "", settings_type_t::BOOL, 1, 3, false };
bool getVSYS_ADC_DIS() { return getBool(VSYS_ADC_DIS); }
void setVSYS_ADC_DIS(bool val) { setBool(VSYS_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting TS_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "TS_ADC_DIS", "", settings_type_t::BOOL, 1, 2, false };
bool getTS_ADC_DIS() { return getBool(TS_ADC_DIS); }
void setTS_ADC_DIS(bool val) { setBool(TS_ADC_DIS, val); }



// ================================
// REG2F_ADC_Function_Disable_0
// ================================


Setting TDIE_ADC_DIS = { REG2F_ADC_Function_Disable_0, false, "TDIE_ADC_DIS", "", settings_type_t::BOOL, 1, 1, false };
bool getTDIE_ADC_DIS() { return getBool(TDIE_ADC_DIS); }
void setTDIE_ADC_DIS(bool val) { setBool(TDIE_ADC_DIS, val); }



// ================================
// REG30_ADC_Function_Disable_1
// ================================


Setting DPLUS_ADC_DIS = { REG30_ADC_Function_Disable_1, false, "DPLUS_ADC_DIS", "", settings_type_t::BOOL, 1, 7, false };
bool getDPLUS_ADC_DIS() { return getBool(DPLUS_ADC_DIS); }
void setDPLUS_ADC_DIS(bool val) { setBool(DPLUS_ADC_DIS, val); }



// ================================
// REG30_ADC_Function_Disable_1
// ================================


Setting DMINUS_ADC_DIS = { REG30_ADC_Function_Disable_1, false, "DMINUS_ADC_DIS", "", settings_type_t::BOOL, 1, 6, false };
bool getDMINUS_ADC_DIS() { return getBool(DMINUS_ADC_DIS); }
void setDMINUS_ADC_DIS(bool val) { setBool(DMINUS_ADC_DIS, val); }



// ================================
// REG30_ADC_Function_Disable_1
// ================================


Setting VAC2_ADC_DIS = { REG30_ADC_Function_Disable_1, false, "VAC2_ADC_DIS", "", settings_type_t::BOOL, 1, 5, false };
bool getVAC2_ADC_DIS() { return getBool(VAC2_ADC_DIS); }
void setVAC2_ADC_DIS(bool val) { setBool(VAC2_ADC_DIS, val); }



// ================================
// REG30_ADC_Function_Disable_1
// ================================


Setting VAC1_ADC_DIS = { REG30_ADC_Function_Disable_1, false, "VAC1_ADC_DIS", "", settings_type_t::BOOL, 1, 4, false };
bool getVAC1_ADC_DIS() { return getBool(VAC1_ADC_DIS); }
void setVAC1_ADC_DIS(bool val) { setBool(VAC1_ADC_DIS, val); }



// ================================
// REG31_IBUS_ADC
// ================================


Setting IBUS_ADC = { REG31_IBUS_ADC, true, "IBUS_ADC", "mA", settings_type_t::INT, 16, 0, false, 0, 5000, 0, 1, settings_flags_t::IS_2COMPLEMENT };
/// Returns IBUS_ADC in mA
int getIBUS_ADC() { return getInt(IBUS_ADC); }



// ================================
// REG33_IBAT_ADC
// ================================


Setting IBAT_ADC = { REG33_IBAT_ADC, true, "IBAT_ADC", "mA", settings_type_t::INT, 16, 0, false, 0, 8000, 0, 1, settings_flags_t::IS_2COMPLEMENT };
/// Returns IBAT_ADC in mA
int getIBAT_ADC() { return getInt(IBAT_ADC); }



// ================================
// REG35_VBUS_ADC
// ================================


Setting VBUS_ADC = { REG35_VBUS_ADC, true, "VBUS_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 30000, 0, 1, settings_flags_t::NONE };
/// Returns VBUS_ADC in mV
int getVBUS_ADC() { return getInt(VBUS_ADC); }



// ================================
// REG37_VAC1_ADC
// ================================


Setting VAC1_ADC = { REG37_VAC1_ADC, true, "VAC1_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 30000, 0, 1, settings_flags_t::NONE };
/// Returns VAC1_ADC in mV
int getVAC1_ADC() { return getInt(VAC1_ADC); }



// ================================
// REG39_VAC2_ADC
// ================================


Setting VAC2_ADC = { REG39_VAC2_ADC, true, "VAC2_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 30000, 0, 1, settings_flags_t::NONE };
/// Returns VAC2_ADC in mV
int getVAC2_ADC() { return getInt(VAC2_ADC); }



// ================================
// REG3B_VBAT_ADC
// ================================


Setting VBAT_ADC = { REG3B_VBAT_ADC, true, "VBAT_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 20000, 0, 1, settings_flags_t::NONE };
/// Returns VBAT_ADC in mV
int getVBAT_ADC() { return getInt(VBAT_ADC); }



// ================================
// REG3D_VSYS_ADC
// ================================


Setting VSYS_ADC = { REG3D_VSYS_ADC, true, "VSYS_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 24000, 0, 1, settings_flags_t::NONE };
/// Returns VSYS_ADC in mV
int getVSYS_ADC() { return getInt(VSYS_ADC); }



// ================================
// REG3F_TS_ADC
// ================================


Setting TS_ADC = { REG3F_TS_ADC, true, "TS_ADC", "%", settings_type_t::FLOAT, 16, 0, false, 0, -99.9023, 0, 0.0976563, settings_flags_t::NONE };
/// Returns TS_ADC in %
float getTS_ADC() { return getFloat(TS_ADC); }



// ================================
// REG41_TDIE_ADC
// ================================


Setting TDIE_ADC = { REG41_TDIE_ADC, true, "TDIE_ADC", "degC", settings_type_t::FLOAT, 16, 0, false, -40, 150, 0, 0.5, settings_flags_t::IS_2COMPLEMENT };
/// Returns TDIE_ADC in degC
float getTDIE_ADC() { return getFloat(TDIE_ADC); }



// ================================
// REG43_DPLUS_ADC
// ================================


Setting DPLUS_ADC = { REG43_DPLUS_ADC, true, "DPLUS_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 3600, 0, 1, settings_flags_t::NONE };
/// Returns DPLUS_ADC in mV
int getDPLUS_ADC() { return getInt(DPLUS_ADC); }



// ================================
// REG45_DMINUS_ADC
// ================================


Setting DMINUS_ADC = { REG45_DMINUS_ADC, true, "DMINUS_ADC", "mV", settings_type_t::INT, 16, 0, false, 0, 3600, 0, 1, settings_flags_t::NONE };
/// Returns DMINUS_ADC in mV
int getDMINUS_ADC() { return getInt(DMINUS_ADC); }



// ================================
// REG47_DPDM_Driver
// ================================


enum class DPLUS_DAC_t : uint8_t {

    DPLUS_DAC_HIZ = 0, 
    DPLUS_DAC_VOUT_0 = 1, 
    DPLUS_DAC_VOUT_0_6 = 2, 
    DPLUS_DAC_VOUT_1_2 = 3, 
    DPLUS_DAC_VOUT_2_0 = 4, 
    DPLUS_DAC_VOUT_2_7 = 5, 
    DPLUS_DAC_VOUT_3_3 = 6, 
    DPLUS_DAC_DPLUS_DMINUS_SHORT = 7
};

strings_vector_t DPLUS_DAC_strings = {

    "HIZ", 
    "0V", 
    "0.6V", 
    "1.2V", 
    "2.0V", 
    "2.7V", 
    "3.3V", 
    "D+/D- Short"
};

Setting DPLUS_DAC = { REG47_DPDM_Driver, false, "DPLUS_DAC", "", settings_type_t::ENUM, 3, 5, false, 0, 7, 0, 1, settings_flags_t::NONE, DPLUS_DAC_strings };
DPLUS_DAC_t getDPLUS_DAC() { return static_cast<DPLUS_DAC_t>(getInt(DPLUS_DAC)); }
const char * getDPLUS_DAC_enum_string() { return getString(DPLUS_DAC); }
void setDPLUS_DAC(DPLUS_DAC_t val) { setEnum<DPLUS_DAC_t>(DPLUS_DAC, val); }




// ================================
// REG47_DPDM_Driver
// ================================


enum class DMINUS_DAC_t : uint8_t {

    DMINUS_DAC_HIZ = 0, 
    DMINUS_DAC_VOUT_0 = 1, 
    DMINUS_DAC_VOUT_0_6 = 2, 
    DMINUS_DAC_VOUT_1_2 = 3, 
    DMINUS_DAC_VOUT_2_0 = 4, 
    DMINUS_DAC_VOUT_2_7 = 5, 
    DMINUS_DAC_VOUT_3_3 = 6, 
    DMINUS_DAC_RESERVED = 7
};

strings_vector_t DMINUS_DAC_strings = {

    "HIZ", 
    "0V", 
    "0.6V", 
    "1.2V", 
    "2.0V", 
    "2.7V", 
    "3.3V", 
    "Reserved"
};

Setting DMINUS_DAC = { REG47_DPDM_Driver, false, "DMINUS_DAC", "", settings_type_t::ENUM, 3, 2, false, 0, 7, 0, 1, settings_flags_t::NONE, DMINUS_DAC_strings };
DMINUS_DAC_t getDMINUS_DAC() { return static_cast<DMINUS_DAC_t>(getInt(DMINUS_DAC)); }
const char * getDMINUS_DAC_enum_string() { return getString(DMINUS_DAC); }
void setDMINUS_DAC(DMINUS_DAC_t val) { setEnum<DMINUS_DAC_t>(DMINUS_DAC, val); }




// ================================
// REG48_Part_Information
// ================================


enum class PN_t : uint8_t {

    PN_RESERVED_0 = 0, 
    PN_RESERVED_1 = 1, 
    PN_RESERVED_2 = 2, 
    PN_BQ25798 = 3, 
    PN_RESERVED_4 = 4, 
    PN_RESERVED_5 = 5, 
    PN_RESERVED_6 = 6, 
    PN_RESERVED_7 = 7
};

strings_vector_t PN_strings = {

    "?", 
    "?", 
    "?", 
    "BQ25798", 
    "?", 
    "?", 
    "?", 
    "?"
};

Setting PN = { REG48_Part_Information, false, "PN", "", settings_type_t::ENUM, 3, 3, false, 0, 1, 0, 1, settings_flags_t::NONE, PN_strings };
PN_t getPN() { return static_cast<PN_t>(getInt(PN)); }
const char * getPN_enum_string() { return getString(PN); }




// ================================
// REG48_Part_Information
// ================================


enum class DEV_REV_t : uint8_t {

    DEV_REV_RESERVED_0 = 0, 
    DEV_REV_BQ25798 = 1, 
    DEV_REV_RESERVED_2 = 2, 
    DEV_REV_RESERVED_3 = 3, 
    DEV_REV_RESERVED_4 = 4, 
    DEV_REV_RESERVED_5 = 5, 
    DEV_REV_RESERVED_6 = 6, 
    DEV_REV_RESERVED_7 = 7
};

strings_vector_t DEV_REV_strings = {

    "?", 
    "BQ25798", 
    "?", 
    "?", 
    "?", 
    "?", 
    "?", 
    "?"
};

Setting DEV_REV = { REG48_Part_Information, false, "DEV_REV", "", settings_type_t::ENUM, 3, 0, false, 0, 1, 0, 1, settings_flags_t::NONE, DEV_REV_strings };
DEV_REV_t getDEV_REV() { return static_cast<DEV_REV_t>(getInt(DEV_REV)); }
const char * getDEV_REV_enum_string() { return getString(DEV_REV); }






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
    DEV_REV};