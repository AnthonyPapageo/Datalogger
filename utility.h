#ifndef UTILITY_H
#define UTILITY_H


#include <Pushbutton.h>


//////////////PROTOTYPE//////////////
void initMux(void);
void setMux(const uint8_t value);
void setVref(bool b);
bool IsSdInserted(void);
void saveToEEPROM(void);
void LoadFromEEPROM(void);
void correctEEPROM(void);
//void centerText(char *src,char *dest);
void buttonsCheck(Pushbutton& back, Pushbutton& left , Pushbutton& down, Pushbutton& up,Pushbutton& right, Pushbutton& Ok);
void emptyfunction(void);



#endif