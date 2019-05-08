#ifndef UTILITY_H
#define UTILITY_H

#include <LiquidCrystal.h>
#include "ADS1241.h"

//////////////LCD//////////////

extern const uint8_t RS;
extern const uint8_t RW;
extern const uint8_t ENABLE_LCD;
extern const uint8_t D0;
extern const uint8_t D1;
extern const uint8_t D2;
extern const uint8_t D3;
extern const uint8_t D4;
extern const uint8_t D5;
extern const uint8_t D6;
extern const uint8_t D7;

//////////////LED//////////////

extern const uint8_t LED2;
extern const uint8_t LED3;
extern const uint8_t LED4;
extern const uint8_t LED5;
extern const uint8_t LED6;
extern const uint8_t LED_SD;

//////////////SPI//////////////
extern const uint32_t SPI_SPEED;

//////////////MUX//////////////
extern const uint8_t CS_MAX1;
extern const uint8_t CS_ADC;
extern const uint8_t CS_SD;
extern const uint8_t CS_DISABLE;

//////////////ISR//////////////
extern volatile bool bOK; //button OK
extern volatile uint8_t x; //LCD CURSOR LOCATION
extern volatile uint8_t y;


//////////////PROTOTYPE//////////////
void setMux(uint8_t value);
void enableButton(void);
bool checkOk(void);
void calibrateGainADC(LiquidCrystal& lcd, ADS1241& ADS1241); 
void calibrateOffsetADC(LiquidCrystal& lcd, ADS1241& ADS1241);
void displayMenu(LiquidCrystal& lcd, const char tab[4][21]);
void displayValue(LiquidCrystal& lcd,ADS1241& ADS1241, uint8_t channel);




#endif