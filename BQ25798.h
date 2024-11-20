#ifndef BQ25798_H
#define BQ25798_H

#include "Arduino.h"

#include <cstdint>
#include "registers.h"

#define I2C_ADDRESS 0x6B

class BQ25798 {
  uint8_t _regs[1 + MAX_REGISTER_NUMBER] = {};
  void writeReg8ToI2C(int reg);
  void writeReg16ToI2C(int reg);

  uint8_t getReg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setReg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getReg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setReg16(int widereg,
                uint16_t value,
                int bitMask = 0xFFFF,
                int bitShift = 0);

 public:
  BQ25798();
  void readAll();

  // REG00_Minimal_System_Voltage
  int getVSYSMIN();
  void setVSYSMIN(int value);

  // REG01_Charge_Voltage_Limit
  int getVREG();
  void setVREG(int value);

  // REG03_Charge_Current_Limit
  int getICHG();
  void setICHG(int value);

  // REG05_Input_Voltage_Limit
  int getVINDPM();
  void setVINDPM(int value);

  // REG06_Input_Current_Limit
  int getIINDPM();
  void setIINDPM(int value);

  // REG08_Precharge_Control
  enum vbat_lowv {
    VBAT_LOWV_15PCT = 0,
    VBAT_LOWV_62PCT = 1,
    VBAT_LOWV_67PCT = 2,
    VBAT_LOWV_71PCT = 3
  };
  const char* const vbat_lowv_str[4] = {"15%*VREG", "62.2%*VREG", "66.7%*VREG",
                                        "71.4%*VREG"};
  const char* getVBAT_LOWVStr();
  vbat_lowv getVBAT_LOWV();
  void setVBAT_LOWV(vbat_lowv value);
  int getIPRECHG();
  void setIPRECHG(int value);

  // REG09_Termination_Control
  int getREG_RST();
  void setREG_RST(int value);
  int getSTOP_WD_CHG();
  void setSTOP_WD_CHG(int value);
  int getITERM();
  void setITERM(int value);

  // REG0A_Recharge_Control
  enum cell { CELL_1S = 0, CELL_2S = 1, CELL_3S = 2, CELL_4S = 3 };
  const char* const cell_str[4] = {"1S", "2S", "3S", "4S"};
  const char* getCELLStr();
  cell getCELL();
  void setCELL(cell value);
  enum trechg {
    TRECHG_64MS = 0,
    TRECHG_256MS = 1,
    TRECHG_1024MS = 2,
    TRECHG_2048MS = 3
  };
  const char* const trechg_str[4] = {"64 ms", "256 ms", "1024 ms", "2048 ms"};
  const char* getTRECHGStr();
  trechg getTRECHG();
  void setTRECHG(trechg value);
  int getVRECHG();
  void setVRECHG(int value);

  // FIXME missing REG0B_VOTG_regulation ... REG2D

  // REG2E_ADC_Control
  bool getADC_EN();
  void setADC_EN(bool value);
  enum adc_rate { ADC_RATE_CONTINUOUS = 0, ADC_RATE_ONESHOT = 1 };
  const char* const adc_rate_str[2] = {"Continuous", "One-shot"};
  const char* getADC_RATEStr();
  adc_rate getADC_RATE();
  void setADC_RATE(adc_rate value);
  enum adc_sample {
    ADC_SAMPLE_15BIT = 0,
    ADC_SAMPLE_14BIT = 1,
    ADC_SAMPLE_13BIT = 2,
    ADC_SAMPLE_12BIT = 3
  };
  const char* const adc_sample_str[4] = {"15-bit", "14-bit", "13-bit",
                                         "12-bit"};
  const char* getADC_SAMPLEStr();
  adc_sample getADC_SAMPLE();
  void setADC_SAMPLE(adc_sample value);
  bool ADC_AVG();
  void setADC_AVG(adc_sample value);
  bool ADC_AVG_INIT();
  void setADC_AVG_INIT(adc_sample value);

  // FIXME missing REG2F ... REG30_ADC_Function_Disable_1

  // REG31_IBUS_ADC
  uint16_t getIBUS_ADC();
  // REG33_IBAT_ADC
  uint16_t getIBAT_ADC();
  // REG35_VBUS_ADC
  uint16_t getVBUS_ADC();
  // REG37_VAC1_ADC
  uint16_t getVAC1_ADC();
  // REG39_VAC2_ADC
  uint16_t getVAC2_ADC();
  // REG3B_VBAT_ADC
  uint16_t getVBAT_ADC();
  // REG3D_VSYS_ADC
  uint16_t getVSYS_ADC();
  // REG3F_TS_ADC
  uint16_t getTS_ADC();
  // REG41_TDIE_ADC
  uint16_t getTDIE_ADC();
  // REG43_DPLUS_ADC
  uint16_t getDPLUS_ADC();
  // REG45_DMINUS_ADC
  uint16_t getDMINUS_ADC();

  // REG47_DPDM_Driver
  int getDPLUS_DAC();
  void setDPLUS_DAC(int value);
  int getDMINUS_DAC();
  void setDMINUS_DAC(int value);

  // REG48_Part_Information
  int getPN();
  const char* const pn_str[8] = {"?", "?", "?", "BQ25798", "?", "?", "?", "?"};
  const char* getPNStr();
  int getDEV_REV();
  const char* const dev_rev_str[8] = {"?", "BQ25798", "?", "?",
                                      "?", "?",       "?", "?"};
  const char* getDEV_REVStr();

  const int registersCount = 1 + MAX_REGISTER_NUMBER - MIN_REGISTER_NUMBER;
};

#endif