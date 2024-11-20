enum {
  REG00_Minimal_System_Voltage = 0x00,
  REG01_Charge_Voltage_Limit = 0x01,
  REG03_Charge_Current_Limit = 0x03,
  REG05_Input_Voltage_Limit = 0x05,
  REG06_Input_Current_Limit = 0x06,
  REG08_Precharge_Control = 0x08,
  REG09_Termination_Control = 0x09,
  REG0A_Recharge_Control = 0x0A,
  REG0B_VOTG_regulation = 0x0B,
  REG0D_IOTG_regulation = 0x0D,
  REG0E_Timer_Control = 0x0E,
  REG0F_Charger_Control_0 = 0x0F,
  REG10_Charger_Control_1 = 0x10,
  REG11_Charger_Control_2 = 0x11,
  REG12_Charger_Control_3 = 0x12,
  REG13_Charger_Control_4 = 0x13,
  REG14_Charger_Control_5 = 0x14,
  REG15_MPPT_Control = 0x15,
  REG16_Temperature_Control = 0x16,
  REG17_NTC_Control_0 = 0x17,
  REG18_NTC_Control_1 = 0x18,
  REG19_ICO_Current_Limit = 0x19,
  REG1B_Charger_Status_0 = 0x1B,
  REG1C_Charger_Status_1 = 0x1C,
  REG1D_Charger_Status_2 = 0x1D,
  REG1E_Charger_Status_3 = 0x1E,
  REG1F_Charger_Status_4 = 0x1F,
  REG20_FAULT_Status_0 = 0x20,
  REG21_FAULT_Status_1 = 0x21,
  REG22_Charger_Flag_0 = 0x22,
  REG23_Charger_Flag_1 = 0x23,
  REG24_Charger_Flag_2 = 0x24,
  REG25_Charger_Flag_3 = 0x25,
  REG26_FAULT_Flag_0 = 0x26,
  REG27_FAULT_Flag_1 = 0x27,
  REG28_Charger_Mask_0 = 0x28,
  REG29_Charger_Mask_1 = 0x29,
  REG2A_Charger_Mask_2 = 0x2A,
  REG2B_Charger_Mask_3 = 0x2B,
  REG2C_FAULT_Mask_0 = 0x2C,
  REG2D_FAULT_Mask_1 = 0x2D,
  REG2E_ADC_Control = 0x2E,
  REG2F_ADC_Function_Disable_0 = 0x2F,
  REG30_ADC_Function_Disable_1 = 0x30,
  REG31_IBUS_ADC = 0x31,
  REG33_IBAT_ADC = 0x33,
  REG35_VBUS_ADC = 0x35,
  REG37_VAC1_ADC = 0x37,
  REG39_VAC2_ADC = 0x39,
  REG3B_VBAT_ADC = 0x3B,
  REG3D_VSYS_ADC = 0x3D,
  REG3F_TS_ADC = 0x3F,
  REG41_TDIE_ADC = 0x41,
  REG43_DPLUS_ADC = 0x43,
  REG45_DMINUS_ADC = 0x45,
  REG47_DPDM_Driver = 0x47,
  REG48_Part_Information = 0x48
};

#define MIN_REGISTER_NUMBER REG00_Minimal_System_Voltage
#define MAX_REGISTER_NUMBER REG48_Part_Information
