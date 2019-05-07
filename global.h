#ifndef GLOBAL_H
#define GLOBAL_H

#include <avr/eeprom.h>

#include <stdint.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <INA.h>
#include <RTClib.h>
#include "MAX31856.h"

#define MAX_TC_NB 20
#define MAX_NTC_NB 5
#define MAX_V24_NB 2
#define MAX_V5_NB 2
#define MAX_I_NB 4


//OBJECT
extern INA_Class INA;
extern RTC_DS3231 RTC;
extern LiquidScreen* current_screen;//Used so we have to call get_currentScreen only once
extern LiquidScreen* previous_screen;
extern MAX31856 max_array[20];

////////MEASURE///////////
extern float TC_Measure_Array[MAX_TC_NB];
extern float NTC_Measure_Array[MAX_NTC_NB];
extern float V24_Measure_Array[MAX_V24_NB];
extern float V5_Measure_Array[MAX_V5_NB];
extern int32_t I_Measure_Array[MAX_I_NB];//contain bus voltage and current
extern uint32_t IntervalMeasure;
extern char TC_Type_Array[8];
extern char TC_Type;
extern uint8_t TC_Type_Counter;


/////////EEPROM DATA///////////
extern uint8_t EEMEM NV_NTC_nb;
extern uint8_t EEMEM NV_TC_nb;
extern uint8_t EEMEM NV_I_nb;
extern uint8_t EEMEM NV_V24_nb;
extern uint8_t EEMEM NV_V5_nb;
extern float EEMEM  NV_B_FACTOR;
extern float EEMEM NV_R_25;
extern uint8_t EEMEM NV_IntervalSeconds;
extern uint8_t EEMEM NV_IntervalMinutes;
extern uint16_t EEMEM NV_R_SHUNT;

/////////NUMBER OF DEVICES ///////////
extern uint8_t NTC_nb;
extern uint8_t TC_nb;
extern uint8_t I_nb;
extern uint8_t V24_nb;
extern uint8_t V5_nb;


/////////INTERVAL///////////
extern uint8_t IntervalSeconds;
extern uint8_t IntervalMinutes;

//////////////LCD//////////////
extern const uint8_t RS;
extern const uint8_t RW;
extern const uint8_t ENABLE_LCD;
extern const uint8_t D4;
extern const uint8_t D5;
extern const uint8_t D6;
extern const uint8_t D7;
extern const uint8_t LCD_COL;
extern const uint8_t LCD_ROWS;


//////////////SPI//////////////
extern const uint32_t SPI_SPEED;

//////////////I2C//////////////
extern const uint32_t I2C_SPEED;

//////////////MUX//////////////
extern const uint8_t CS_MAX1;
extern const uint8_t CS_MAX2;
extern const uint8_t CS_MAX3;
extern const uint8_t CS_MAX4;
extern const uint8_t CS_MAX5;
extern const uint8_t CS_MAX6;
extern const uint8_t CS_MAX7;
extern const uint8_t CS_MAX8;
extern const uint8_t CS_MAX9;
extern const uint8_t CS_MAX10;
extern const uint8_t CS_MAX11;
extern const uint8_t CS_MAX12;
extern const uint8_t CS_MAX13;
extern const uint8_t CS_MAX14;
extern const uint8_t CS_MAX15;
extern const uint8_t CS_MAX16;
extern const uint8_t CS_MAX17;
extern const uint8_t CS_MAX18;
extern const uint8_t CS_MAX19;
extern const uint8_t CS_MAX20;
extern const uint8_t CS_SD;
extern const uint8_t CS_DISABLE;


//////////////NTC//////////////
extern float B_FACTOR;
extern float R_25;
extern const float R_LINE_NTC;
extern const float V_SUPPLY;

//////////////0-24V READINGS//////////////
extern const float R_UP;
extern const float R_DOWN;



//////////////CURRENT SENSING//////////////
extern uint16_t R_SHUNT;
extern const uint16_t DEFAULT_R_SHUNT;


/////////GLYPH///////////
namespace GLYPH
{
	extern uint8_t upArrow[8];
	extern uint8_t downArrow[8];
	extern uint8_t rightArrow[8];
	extern uint8_t leftArrow[8];
	extern const uint8_t upArrowIndex;
	extern const uint8_t downArrowIndex;
	extern const uint8_t rightArrowIndex;
	extern const uint8_t leftArrowIndex;
}

/*namespace ADCChannel
{
	extern const uint8_t V1;
	extern const uint8_t V2;
	extern const uint8_t V3;
	extern const uint8_t V4;
	extern const uint8_t NTC1;
	extern const uint8_t NTC2;
	extern const uint8_t NTC3;
	extern const uint8_t NTC4;
	extern const uint8_t NTC5;
}*/
extern const uint8_t NTCChannel[MAX_NTC_NB];
extern const uint8_t V24Channel[MAX_V24_NB];
extern const uint8_t V5Channel[MAX_V5_NB];

 
#endif