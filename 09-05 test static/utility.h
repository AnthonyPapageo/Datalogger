#ifndef UTILITY_H
#define UTILITY_H


#include <Pushbutton.h>


//////////////PROTOTYPE//////////////
void initMux(void);
void setMux(const uint8_t value);
void initRTC(void);
void setVref(bool b);
bool IsSdInserted(void);
void saveToEEPROM(void);
void LoadFromEEPROM(void);
void correctEEPROM(void);
void buttonsCheck(Pushbutton& back, Pushbutton& left , Pushbutton& down, Pushbutton& up,Pushbutton& right, Pushbutton& Ok);
void setFileName(void);
void firstLineSD(void);
void saveToSD(void);
void SdWriteInt(int32_t data);
void SdWriteFloat(float data, uint8_t precision);
void led_SD(bool b);
void emptyfunction(void);



#endif