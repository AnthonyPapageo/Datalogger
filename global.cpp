#include <LiquidCrystal.h>
#include <INA.h>
#include "global.h"
#include "menu.h"


//DEBUG ZONE
LiquidScreen* previous_screen = &main_Screen;
LiquidScreen* current_screen = &main_Screen;



/////////EEPROM DATA///////////
uint8_t EEMEM NV_NTC_nb = 0;
uint8_t EEMEM NV_TC_nb = 0;
uint8_t EEMEM NV_I_nb = 0;
uint8_t EEMEM NV_V24_nb = 0;
uint8_t EEMEM NV_V5_nb = 1;

float EEMEM NV_B_FACTOR = 3977.0;
float EEMEM NV_R_25 = 10000.0;

uint8_t EEMEM NV_IntervalSeconds = 5;
uint8_t EEMEM NV_IntervalMinutes = 0;

uint16_t EEMEM NV_R_SHUNT = 2000;

/////////NUMBER OF DEVICES ///////////
uint8_t NTC_nb = 0;
uint8_t TC_nb = 0;
uint8_t I_nb = 0;
uint8_t V24_nb = 0;
uint8_t V5_nb = 0;

/////////INTERVAL///////////
uint8_t IntervalSeconds = 5;
uint8_t IntervalMinutes = 0;


//////////////LCD//////////////
/*const uint8_t RS = 7;
const uint8_t RW = 8;
const uint8_t ENABLE_LCD = 9;
const uint8_t D4 = 30;
const uint8_t D5 = 31;
const uint8_t D6 = 32;
const uint8_t D7 = 33;
const uint8_t LCD_COL = 20;
const uint8_t LCD_ROWS = 4;*/
const uint8_t RS = 7;
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
const uint8_t LCD_ROWS = 4;


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
float B_FACTOR = 3977.0;
float R_25 = 10000.0;
float R_LINE = 10000.0;

//////////////CURRENT SENSING//////////////
uint16_t R_SHUNT = 2000;



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

namespace ADCChannel
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
}


