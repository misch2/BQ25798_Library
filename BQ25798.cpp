#include "BQ25798.h"
#include <Wire.h>

BQ25798::BQ25798() {
  _address = DEFAULT_I2C_ADDRESS;
  clearRegs();
}

BQ25798::BQ25798(uint8_t address) {
  _address = address;
  clearRegs();
}

void BQ25798::clearRegs() {
  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = 0;
  }
}

bool BQ25798::begin() {
  return readAll();
}

bool BQ25798::readAll() {
  Wire.beginTransmission(_address);
  Wire.write(MIN_REGISTER_NUMBER);
  Wire.endTransmission();
  if (Wire.requestFrom(_address, registersCount) != registersCount) {
    Serial.println("Error reading BQ25798 registers");
    return false;
  }

  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = Wire.read();
  }

  return true;
}

bool BQ25798::writeReg8ToI2C(int reg) {
  Wire.beginTransmission(_address);
  if (Wire.write(reg) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  Wire.endTransmission();
  return true;
}

bool BQ25798::writeReg16ToI2C(int reg) {
  Wire.beginTransmission(_address);
  if (Wire.write(reg) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  if (Wire.write(_regs[reg + 1]) != 1) {
    Serial.println("Error writing to BQ25798 register");
    return false;
  }
  Wire.endTransmission();
  return true;
}

uint8_t BQ25798::getReg8(int reg, int bitMask, int bitShift) {
  return (_regs[reg] >> bitShift) & bitMask;
}

void BQ25798::setReg8(int reg, uint8_t value, int bitMask, int bitShift) {
  uint8_t shiftedMask = bitMask << bitShift;
  uint8_t shiftedValue = (value & bitMask) << bitShift;

  _regs[reg] &= shiftedMask;
  _regs[reg] |= (shiftedValue & shiftedMask);
}

uint16_t BQ25798::getReg16(int widereg, int bitMask, int bitShift) {
  return ((_regs[widereg] << 8) | _regs[widereg + 1]) >> bitShift & bitMask;
}

void BQ25798::setReg16(int widereg, uint16_t value, int bitMask, int bitShift) {
  uint16_t shiftedMask = bitMask << bitShift;
  uint16_t shiftedValue = (value & bitMask) << bitShift;

  _regs[widereg] &= shiftedMask >> 8;
  _regs[widereg + 1] &= shiftedMask & 0xFF;
  _regs[widereg] |= (shiftedValue & shiftedMask) >> 8;
  _regs[widereg + 1] |= (shiftedValue & shiftedMask) & 0xFF;
}

//
// REG00_Minimal_System_Voltage
// Determined from PROG pin at boot time
//
int BQ25798::getVSYSMIN() {
  return getReg8(REG00_Minimal_System_Voltage, 0x3F, 0);
};
void BQ25798::setVSYSMIN(int value) {
  setReg8(REG00_Minimal_System_Voltage, value, 0x3F, 0);
  writeReg8ToI2C(REG00_Minimal_System_Voltage);
}

//
// REG01_Charge_Voltage_Limit
// Determined from PROG pin at boot time
//
int BQ25798::getVREG() {
  return getReg16(REG01_Charge_Voltage_Limit, 0x7FF, 0);
}
void BQ25798::setVREG(int value) {
  setReg16(REG01_Charge_Voltage_Limit, value, 0x7FF, 0);
  writeReg16ToI2C(REG01_Charge_Voltage_Limit);
}

//
// REG03_Charge_Current_Limit
// Determined from PROG pin at boot time
//
int BQ25798::getICHG() {
  return getReg16(REG03_Charge_Current_Limit, 0x1FF, 0);
}
void BQ25798::setICHG(int value) {
  setReg16(REG03_Charge_Current_Limit, value, 0x1FF, 0);
  writeReg16ToI2C(REG03_Charge_Current_Limit);
}

//
// REG05_Input_Voltage_Limit
//
int BQ25798::getVINDPM() {
  return getReg8(REG05_Input_Voltage_Limit, 0xFF, 0);
}
void BQ25798::setVINDPM(int value) {
  setReg8(REG05_Input_Voltage_Limit, value, 0xFF, 0);
  writeReg8ToI2C(REG05_Input_Voltage_Limit);
}

//
// REG06_Input_Current_Limit
//
int BQ25798::getIINDPM() {
  return getReg16(REG06_Input_Current_Limit, 0x1FF, 0);
}
void BQ25798::setIINDPM(int value) {
  setReg16(REG06_Input_Current_Limit, value, 0x1FF, 0);
  writeReg16ToI2C(REG06_Input_Current_Limit);
}

//
// REG08_Precharge_Control
//
BQ25798::vbat_lowv BQ25798::getVBAT_LOWV() {
  return static_cast<BQ25798::vbat_lowv>(
      getReg8(REG08_Precharge_Control, 0x3, 6));
}
void BQ25798::setVBAT_LOWV(BQ25798::vbat_lowv value) {
  setReg8(REG08_Precharge_Control, value, 0x3, 6);
  writeReg8ToI2C(REG08_Precharge_Control);
}
const char* BQ25798::getVBAT_LOWVStr() {
  return vbat_lowv_str[getVBAT_LOWV()];
}

int BQ25798::getIPRECHG() {
  return getReg8(REG08_Precharge_Control, 0x3F, 0);
}
void BQ25798::setIPRECHG(int value) {
  setReg8(REG08_Precharge_Control, value, 0x3F, 0);
  writeReg8ToI2C(REG08_Precharge_Control);
}

//
// REG09_Termination_Control
//
int BQ25798::getREG_RST() {
  return getReg8(REG09_Termination_Control, 0x01, 6);
}
void BQ25798::setREG_RST(int value) {
  setReg8(REG09_Termination_Control, value, 0x01, 6);
  writeReg8ToI2C(REG09_Termination_Control);
}

int BQ25798::getSTOP_WD_CHG() {
  return getReg8(REG09_Termination_Control, 0x01, 5);
}
void BQ25798::setSTOP_WD_CHG(int value) {
  setReg8(REG09_Termination_Control, value, 0x01, 5);
  writeReg8ToI2C(REG09_Termination_Control);
}

int BQ25798::getITERM() {
  return getReg8(REG09_Termination_Control, 0x1F, 0);
}
void BQ25798::setITERM(int value) {
  setReg8(REG09_Termination_Control, value, 0x1F, 0);
  writeReg8ToI2C(REG09_Termination_Control);
}

//
// REG0A_Recharge_Control
//
BQ25798::cell BQ25798::getCELL() {
  return static_cast<BQ25798::cell>(getReg8(REG0A_Recharge_Control, 0x03, 6));
}
void BQ25798::setCELL(BQ25798::cell value) {
  setReg8(REG0A_Recharge_Control, value, 0x03, 6);
  writeReg8ToI2C(REG0A_Recharge_Control);
}
const char* BQ25798::getCELLStr() {
  return cell_str[getCELL()];
}

BQ25798::trechg BQ25798::getTRECHG() {
  return static_cast<BQ25798::trechg>(getReg8(REG0A_Recharge_Control, 0x03, 4));
}
void BQ25798::setTRECHG(BQ25798::trechg value) {
  setReg8(REG0A_Recharge_Control, value, 0x03, 4);
  writeReg8ToI2C(REG0A_Recharge_Control);
}
const char* BQ25798::getTRECHGStr() {
  return trechg_str[getTRECHG()];
}

int BQ25798::getVRECHG() {
  return getReg8(REG0A_Recharge_Control, 0x0F, 0);
}
void BQ25798::setVRECHG(int value) {
  setReg8(REG0A_Recharge_Control, value, 0x0F, 0);
  writeReg8ToI2C(REG0A_Recharge_Control);
}

//
// REG0B_VOTG_regulation
//
int BQ25798::getVOTG() {
  return getReg16(REG0B_VOTG_regulation, 0x7FF, 0);
}
void BQ25798::setVOTG(int value) {
  setReg16(REG0B_VOTG_regulation, value, 0x7FF, 0);
  writeReg16ToI2C(REG0B_VOTG_regulation);
}

//
// REG0D_IOTG_regulation
//
const char* BQ25798::getPRECHG_TMRStr() {
  return prechg_tmr_str[getPRECHG_TMR()];
}
BQ25798::precgh_tmr BQ25798::getPRECHG_TMR() {
  return static_cast<precgh_tmr>(getReg8(REG0D_IOTG_regulation, 0x01, 7));
}
void BQ25798::setPRECHG_TMR(precgh_tmr value) {
  setReg8(REG0D_IOTG_regulation, value, 0x01, 7);
  writeReg8ToI2C(REG0D_IOTG_regulation);
}
int BQ25798::getIOTG() {
  return getReg8(REG0D_IOTG_regulation, 0x7F, 0);
}
void BQ25798::setIOTG(int value) {
  setReg8(REG0D_IOTG_regulation, value, 0x7F, 0);
  writeReg8ToI2C(REG0D_IOTG_regulation);
}

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

//
// REG2E_ADC_Control
//
bool BQ25798::getADC_ENABLE() {
  return getReg8(REG2E_ADC_Control, 0x01, 7);
}
void BQ25798::setADC_ENABLE(bool value) {
  setReg8(REG2E_ADC_Control, value, 0x01, 7);
  writeReg8ToI2C(REG2E_ADC_Control);
}

const char* BQ25798::getADC_RATEStr() {
  return adc_rate_str[getADC_RATE()];
}
BQ25798::adc_rate BQ25798::getADC_RATE() {
  return static_cast<adc_rate>(getReg8(REG2E_ADC_Control, 0x01, 6));
}
void BQ25798::setADC_RATE(adc_rate value) {
  setReg8(REG2E_ADC_Control, value, 0x01, 6);
  writeReg8ToI2C(REG2E_ADC_Control);
}

const char* BQ25798::getADC_SAMPLEStr() {
  return adc_sample_str[getADC_SAMPLE()];
}
BQ25798::adc_sample BQ25798::getADC_SAMPLE() {
  return static_cast<adc_sample>(getReg8(REG2E_ADC_Control, 0x03, 4));
}
void BQ25798::setADC_SAMPLE(adc_sample value) {
  setReg8(REG2E_ADC_Control, value, 0x03, 4);
  writeReg8ToI2C(REG2E_ADC_Control);
}

bool BQ25798::ADC_AVG() {
  return getReg8(REG2E_ADC_Control, 0x01, 3);
}
void BQ25798::setADC_AVG(adc_sample value) {
  setReg8(REG2E_ADC_Control, value, 0x01, 3);
  writeReg8ToI2C(REG2E_ADC_Control);
}

bool BQ25798::ADC_AVG_INIT() {
  return getReg8(REG2E_ADC_Control, 0x01, 2);
}
void BQ25798::setADC_AVG_INIT(adc_sample value) {
  setReg8(REG2E_ADC_Control, value, 0x01, 2);
  writeReg8ToI2C(REG2E_ADC_Control);
}

// REG2F_ADC_Function_Disable_0
bool BQ25798::getIBUS_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 7);
}
void BQ25798::setIBUS_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 7);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getIBAT_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 6);
}
void BQ25798::setIBAT_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 6);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getVBUS_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 5);
}
void BQ25798::setVBUS_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 5);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getVBAT_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 4);
}
void BQ25798::setVBAT_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 4);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getVSYS_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 3);
}
void BQ25798::setVSYS_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 3);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getTS_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 2);
}
void BQ25798::setTS_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 2);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}

bool BQ25798::getTDIE_ADC_DISABLE() {
  return getReg8(REG2F_ADC_Function_Disable_0, 0x01, 1);
}
void BQ25798::setTDIE_ADC_DISABLE(bool value) {
  setReg8(REG2F_ADC_Function_Disable_0, value, 0x01, 1);
  writeReg8ToI2C(REG2F_ADC_Function_Disable_0);
}




// REG30_ADC_Function_Disable_1
bool BQ25798::getDPLUS_ADC_DISABLE() {
  return getReg8(REG30_ADC_Function_Disable_1, 0x01, 7);
}
void BQ25798::setDPLUS_ADC_DISABLE(bool value) {
  setReg8(REG30_ADC_Function_Disable_1, value, 0x01, 7);
  writeReg8ToI2C(REG30_ADC_Function_Disable_1);
}

bool BQ25798::getDMINUS_ADC_DISABLE() {
  return getReg8(REG30_ADC_Function_Disable_1, 0x01, 6);
}
void BQ25798::setDMINUS_ADC_DISABLE(bool value) {
  setReg8(REG30_ADC_Function_Disable_1, value, 0x01, 6);
  writeReg8ToI2C(REG30_ADC_Function_Disable_1);
}

bool BQ25798::getVAC2_ADC_DISABLE() {
  return getReg8(REG30_ADC_Function_Disable_1, 0x01, 5);
}
void BQ25798::setVAC2_ADC_DISABLE(bool value) {
  setReg8(REG30_ADC_Function_Disable_1, value, 0x01, 5);
  writeReg8ToI2C(REG30_ADC_Function_Disable_1);
}

bool BQ25798::getVAC1_ADC_DISABLE() {
  return getReg8(REG30_ADC_Function_Disable_1, 0x01, 4);
}
void BQ25798::setVAC1_ADC_DISABLE(bool value) {
  setReg8(REG30_ADC_Function_Disable_1, value, 0x01, 4);
  writeReg8ToI2C(REG30_ADC_Function_Disable_1);
}


// REG31_IBUS_ADC
uint16_t BQ25798::getIBUS_ADC() {
  return getReg16(REG31_IBUS_ADC, 0xFFFF, 0);
}

// REG33_IBAT_ADC
uint16_t BQ25798::getIBAT_ADC() {
  return getReg16(REG33_IBAT_ADC, 0xFFFF, 0);
}

// REG35_VBUS_ADC
uint16_t BQ25798::getVBUS_ADC() {
  return getReg16(REG35_VBUS_ADC, 0xFFFF, 0);
}

// REG37_VAC1_ADC
uint16_t BQ25798::getVAC1_ADC() {
  return getReg16(REG37_VAC1_ADC, 0xFFFF, 0);
}

// REG39_VAC2_ADC
uint16_t BQ25798::getVAC2_ADC() {
  return getReg16(REG39_VAC2_ADC, 0xFFFF, 0);
}

// REG3B_VBAT_ADC
uint16_t BQ25798::getVBAT_ADC() {
  return getReg16(REG3B_VBAT_ADC, 0xFFFF, 0);
}

// REG3D_VSYS_ADC
uint16_t BQ25798::getVSYS_ADC() {
  return getReg16(REG3D_VSYS_ADC, 0xFFFF, 0);
}

// REG3F_TS_ADC
uint16_t BQ25798::getTS_ADC() {
  return getReg16(REG3F_TS_ADC, 0xFFFF, 0);
}

// REG41_TDIE_ADC
uint16_t BQ25798::getTDIE_ADC() {
  return getReg16(REG41_TDIE_ADC, 0xFFFF, 0);
}

// REG43_DPLUS_ADC
uint16_t BQ25798::getDPLUS_ADC() {
  return getReg16(REG43_DPLUS_ADC, 0xFFFF, 0);
}

// REG45_DMINUS_ADC
uint16_t BQ25798::getDMINUS_ADC() {
  return getReg16(REG45_DMINUS_ADC, 0xFFFF, 0);
}

//
// REG47_DPDM_Driver
//
int BQ25798::getDPLUS_DAC() {
  return getReg8(REG47_DPDM_Driver, 0x07, 5);
}
void BQ25798::setDPLUS_DAC(int value) {
  setReg8(REG47_DPDM_Driver, value, 0x07, 5);
  writeReg8ToI2C(REG47_DPDM_Driver);
}
int BQ25798::getDMINUS_DAC() {
  return getReg8(REG47_DPDM_Driver, 0x07, 2);
}
void BQ25798::setDMINUS_DAC(int value) {
  setReg8(REG47_DPDM_Driver, value, 0x07, 2);
  writeReg8ToI2C(REG47_DPDM_Driver);
}

//
// REG48_Part_Information
//
int BQ25798::getPN() {
  return getReg8(REG48_Part_Information, 0x07, 3);
}
const char* BQ25798::getPNStr() {
  return pn_str[getPN()];
}
int BQ25798::getDEV_REV() {
  return getReg8(REG48_Part_Information, 0x07, 0);
}
const char* BQ25798::getDEV_REVStr() {
  return dev_rev_str[getDEV_REV()];
}
