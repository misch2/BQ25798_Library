#ifndef BQ25798_H
#define BQ25798_H

#include "Arduino.h"

#include <cstdint>
#include <string>
#include "registers.h"

#define I2C_ADDR 0x6B

class BQ25798 {
  uint8_t _regs[1 + MAX_REGISTER_NUMBER] = {};
  void writeByte(int reg);
  void writeWord(int reg);

  uint8_t getreg8(int reg, int bitMask = 0xFF, int bitShift = 0);
  void setreg8(int reg, uint8_t value, int bitMask = 0xFF, int bitShift = 0);
  uint16_t getreg16(int widereg, int bitMask = 0xFFFF, int bitShift = 0);
  void setreg16(int widereg,
                uint16_t value,
                int bitMask = 0xFFFF,
                int bitShift = 0);

 public:
  BQ25798();
  void readAll();

  // mV
  int getVSYSMIN();
  void setVSYSMIN(int value);

  // mV
  int getVREG();
  void setVREG(int value);

  // mA
  int getICHG();
  void setICHG(int value);

  // mV
  int getVINDPM();
  void setVINDPM(int value);

  // mA
  int getIINDPM();
  void setIINDPM(int value);

// 0 = 15%*VREG
// 1 = 62.2%*VREG
// 2 = 66.7%*VREG
// 3 = 71.4%*VREG
#define VBAT_LOWV_15PCT 0
#define VBAT_LOWV_62PCT 1
#define VBAT_LOWV_67PCT 2
#define VBAT_LOWV_71PCT 3
  int getVBAT_LOWV();
  void setVBAT_LOWV(int value);
  // mA
  int getIPRECHG();
  void setIPRECHG(int value);

  int getREG_RST();
  void setREG_RST(int value);
  int getSTOP_WD_CHG();
  void setSTOP_WD_CHG(int value);
  // mA
  int getITERM();
  void setITERM(int value);

// Cell configuration
#define CELL_1S 0
#define CELL_2S 1
#define CELL_3S 2
#define CELL_4S 3
  char* CELL_NAME[4] = {"1S", "2S", "3S", "4S"};
  int getCELL();
  char* getCELLStr();
  void setCELL(int value);

// Battery recharge deglitch time
#define TRECHG_64MS 0
#define TRECHG_256MS 1
#define TRECHG_1024MS 2
#define TRECHG_2048MS 3
  char* TRECHG_NAME[4] = {"64 ms", "256 ms", "1024 ms", "2048 ms"};
  int getTRECHG();
  char* getTRECHGStr();
  void setTRECHG(int value);
  // mV
  int getVRECHG();
  void setVRECHG(int value);

  // FIXME missing REG0B_VOTG_regulation ... REG30_ADC_Function_Disable_1

  uint16_t getIBUS_ADC();
  uint16_t getIBAT_ADC();
  uint16_t getVBUS_ADC();
  uint16_t getVAC1_ADC();
  uint16_t getVAC2_ADC();
  uint16_t getVBAT_ADC();
  uint16_t getVSYS_ADC();
  uint16_t getTS_ADC();
  uint16_t getTDIE_ADC();
  uint16_t getDPLUS_ADC();
  uint16_t getDMINUS_ADC();

  int getDPLUS_DAC();
  void setDPLUS_DAC(int value);
  int getDMINUS_DAC();
  void setDMINUS_DAC(int value);

  int getPN();
  char* PN_NAME[8] = {"?", "?", "?", "BQ25798", "?", "?", "?", "?"};
  char* getPNStr();
  int getDEV_REV();
  char* DEV_REV_NAME[8] = {"?", "BQ25798", "?", "?", "?", "?", "?", "?"};
  char* getDEV_REVStr();

  const int registersCount = 1 + MAX_REGISTER_NUMBER - MIN_REGISTER_NUMBER;
};

#endif