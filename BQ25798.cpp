#include "BQ25798.h"
#include <Wire.h>

BQ25798::BQ25798() {
  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = 0;
  }
}

void BQ25798::readAll() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(MIN_REGISTER_NUMBER);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, registersCount);

  for (int i = 0; i < registersCount; i++) {
    _regs[i + MIN_REGISTER_NUMBER] = Wire.read();
  }
}

void BQ25798::writeReg8ToI2C(int reg) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(_regs[reg]);
  Wire.endTransmission();
}

void BQ25798::writeReg16ToI2C(int reg) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(_regs[reg]);
  Wire.write(_regs[reg + 1]);
  Wire.endTransmission();
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
// REG2E_ADC_Control
//
bool BQ25798::getADC_EN() {
  return getReg8(REG2E_ADC_Control, 0x01, 7);
}
void BQ25798::setADC_EN(bool value) {
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

// ADCs
uint16_t BQ25798::getIBUS_ADC() {
  return getReg16(REG31_IBUS_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getIBAT_ADC() {
  return getReg16(REG33_IBAT_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getVBUS_ADC() {
  return getReg16(REG35_VBUS_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getVAC1_ADC() {
  return getReg16(REG37_VAC1_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getVAC2_ADC() {
  return getReg16(REG39_VAC2_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getVBAT_ADC() {
  return getReg16(REG3B_VBAT_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getVSYS_ADC() {
  return getReg16(REG3D_VSYS_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getTS_ADC() {
  return getReg16(REG3F_TS_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getTDIE_ADC() {
  return getReg16(REG41_TDIE_ADC, 0xFFFF, 0);
}

uint16_t BQ25798::getDPLUS_ADC() {
  return getReg16(REG43_DPLUS_ADC, 0xFFFF, 0);
}

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
