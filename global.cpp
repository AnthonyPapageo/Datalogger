#include <LiquidCrystal.h>
#include <INA.h>
#include "global.h"
#include "menu.h"


//DEBUG ZONE
LiquidScreen* current_screen = &main_Screen;
RTC_DS3231 RTC;
INA_Class INA;
LiquidCrystal lcd(RS, RW, ENABLE_LCD, D4, D5, D6, D7);//4bit
LiquidMenu menu(lcd);

/////////MAX///////////
MAX31856 max_array[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; //Call the 20 constructor

////////MEASURE///////////
float TC_Measure_Array[MAX_TC_NB];
float NTC_Measure_Array[MAX_NTC_NB];
float V24_Measure_Array[MAX_V24_NB];
float V5_Measure_Array[MAX_V5_NB];
int32_t I_Measure_Array[MAX_I_NB];
float CJT_Measure_Array[MAX_TC_NB];
uint32_t IntervalMeasure = 1;
char TC_Type_Array[8] = {'T','K','J','N','R','S','B','E'};
char TC_Type[2] = {'T',0}; // 0 = '\0' so it stops printing, thanks Arnaud !
uint8_t TC_Type_Counter = 0;
uint32_t Nb_Of_Measure = 0;
DateTime Global_Begin_Datetime;
DateTime Global_Current_DateTime;
DateTime Global_End_Datetime;
bool Global_test_launched = false;
bool Global_Is_multifile = false;

/////////EEPROM DATA///////////
uint8_t EEMEM NV_NTC_nb = 0;
uint8_t EEMEM NV_TC_nb = 0;
uint8_t EEMEM NV_I_nb = 0;
uint8_t EEMEM NV_V24_nb = 0;
uint8_t EEMEM NV_V5_nb = 1;
float EEMEM NV_B_FACTOR = 4000.0;
float EEMEM NV_R_25 = 10000.0;
uint8_t EEMEM NV_IntervalSeconds = 5;
uint8_t EEMEM NV_IntervalMinutes = 0;
uint32_t EEMEM NV_R_SHUNT = 2000;


/////////NUMBER OF DEVICES ///////////
uint8_t NTC_nb = 0;
uint8_t TC_nb = 0;
uint8_t I_nb = 0;
uint8_t V24_nb = 0;
uint8_t V5_nb = 0;

/////////INTERVAL///////////
uint8_t IntervalSeconds = 1;
uint8_t IntervalMinutes = 0;

/////////DURATION///////////
uint8_t DurationHour = 0;
uint8_t DurationMin = 1;
uint8_t DurationSec = 0;

/////////TIME ELAPSED///////
uint8_t ElapsedSeconds = 0;
uint8_t ElapsedMinutes = 0;
uint16_t ElapsedHours = 0;

//////////////LCD//////////////
const uint8_t RS = 7;
const uint8_t RW = 8;
const uint8_t ENABLE_LCD = 9;
const uint8_t D4 = 30;
const uint8_t D5 = 31;
const uint8_t D6 = 32;
const uint8_t D7 = 33;
const uint8_t LCD_COL = 20;
const uint8_t LCD_ROWS = 4;
/*const uint8_t RS = 7;
const uint8_t RW = 8;
const uint8_t ENABLE_LCD = 9;
const uint8_t D0 = 12;
const uint8_t D1 = 13;
const uint8_t D2 = 32;
const uint8_t D3 = 33;
const uint8_t D4 = 34;
const uint8_t D5 = 35;
const uint8_t D6 = 36;
const uint8_t D7 = 37;
const uint8_t LCD_COL = 20;
const uint8_t LCD_ROWS = 4;*/


//////////////SPI//////////////
const uint32_t SPI_SPEED = 400000;

//////////////I2C//////////////
const uint32_t I2C_SPEED = 100000;

//////////////MUX//////////////
const uint8_t CS_MAX1 = 0;
const uint8_t CS_MAX2 = 1;
const uint8_t CS_MAX3 = 2;
const uint8_t CS_MAX4 = 3;
const uint8_t CS_MAX5 = 4;
const uint8_t CS_MAX6 = 5;
const uint8_t CS_MAX7 = 6;
const uint8_t CS_MAX8 = 7;
const uint8_t CS_MAX9 = 8;
const uint8_t CS_MAX10 = 9;
const uint8_t CS_MAX11 = 10;
const uint8_t CS_MAX12 = 11;
const uint8_t CS_MAX13 = 12;
const uint8_t CS_MAX14 = 13;
const uint8_t CS_MAX15 = 14;
const uint8_t CS_MAX16 = 15;
const uint8_t CS_MAX17 = 16;
const uint8_t CS_MAX18 = 17;
const uint8_t CS_MAX19 = 18;
const uint8_t CS_MAX20 = 19;
const uint8_t CS_DISABLE = 31;
const uint8_t CS_SD = 43; //Arduino pin of PL6

//////////////NTC//////////////
float B_FACTOR = 4000.0;
float R_25 = 10000.0;
const float R_LINE_NTC = 10000.0;
const float V_SUPPLY = 5.0;

//////////////0-24V READINGS//////////////
float const R_UP = 39000.0;
float const R_DOWN = 10000.0;

//////////////CURRENT SENSING//////////////
uint32_t R_SHUNT = 2000;
const uint16_t DEFAULT_R_SHUNT = 2000;


/////////GLYPH///////////

namespace GLYPH
{
	uint8_t upArrow[8] = {
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00100,
		0b01110,
		0b11111
	};
	uint8_t downArrow[8] = {
		0b11111,
		0b01110,
		0b00100,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000
	};
	uint8_t rightArrow[8] = {
		0b01000,
		0b01100,
		0b01110,
		0b01111,
		0b01110,
		0b01100,
		0b01000,
		0b00000
	};
	
	uint8_t leftArrow[8] = {
		0b00010,
		0b00110,
		0b01110,
		0b11110,
		0b01110,
		0b00110,
		0b00010,
		0b00000
	};

	const uint8_t upArrowIndex = 1;
	const uint8_t downArrowIndex = 2;
	const uint8_t rightArrowIndex = 3;
	const uint8_t leftArrowIndex = 4;
}

/*namespace ADCChannel
{
	 const uint8_t V1 = 13;
	 const uint8_t V2 = 14;
	 const uint8_t V3 = 3;
	 const uint8_t V4 = 2;
	 const uint8_t NTC1 = 8;
	 const uint8_t NTC2 = 9;
	 const uint8_t NTC3 = 10;
	 const uint8_t NTC4 = 11;
	 const uint8_t NTC5 = 12;
}*/
const uint8_t NTCChannel[MAX_NTC_NB] = {8,9,10,11,12}; //NTC are on ADC8 to ADC12
const uint8_t V24Channel[MAX_V24_NB] = {13,14};
const uint8_t V5Channel[MAX_V5_NB] = {3,2};



