/*
  ADS1241.h - Test library for ADS1241 - implementation
  Copyright (c) 2019 Anthony Papageorgiou.  All right reserved.
*/

#include <stdint.h>


// ensure this library description is only included once
#ifndef ADS1241_H
#define ADS1241_H


// library interface description
class ADS1241
{
    // user-accessible "public" interface
  public:
    /*ADS1241();
    ~ADS1241();*/
    void begin(bool isBufEnable, bool isBOCSenable);
    void reset(void);
    void write(uint8_t uiReg, uint8_t uiValue);
    uint8_t read(uint8_t uiReg);
    uint32_t read24bits(uint8_t uiReg);
    void selectChannel(uint8_t channel);
    uint32_t getVoltage(uint8_t channel);
    void calibrateGain (uint8_t channel);
    void calibrateOffset (uint8_t channel);
    void setupBOCS(bool boolBOCS);
	void setupBUF(bool boolBUF);
    void range(bool boolRange);

    // library-accessible "private" interface
  private:
    uint8_t transform(uint8_t uiValue1, uint8_t uiValue2);
};

#endif
