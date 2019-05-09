/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//////GENERAL C++/////////
#include <stdint.h>

//////////////AVR//////////////
#include <avr/io.h>

//////////////ARDUINO/////////////
#include <LiquidCrystal.h>
#include <SPI.h>
#include <LiquidMenu.h>
#include <Pushbutton.h>
#include <INA.h>
#include <RTClib.h>


//////////////CUSTOM//////////////
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/utility.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/MAX31856.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/global.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/menu.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/measure.h"


//////////SCREEN UPDATE SPEED///////
uint32_t screenUpdatePeriod = 500;
uint32_t screenUpdateLastMs = 0;

/////////UPDATE MEASURE///////
uint32_t measureLastMs = 0;


///////OBJECT INSTANTATION///////
LiquidCrystal lcd(RS, RW, ENABLE_LCD, D4, D5, D6, D7);//4bit
INA_Class INA;
LiquidMenu menu(lcd);
RTC_DS3231 RTC;

Pushbutton butBack(40, PULL_UP_DISABLED); //Extern RC debounce + pull up so no need of internal pull up
Pushbutton butLeft(41, PULL_UP_DISABLED);
Pushbutton butDown(37, PULL_UP_DISABLED);
Pushbutton butUp(36, PULL_UP_DISABLED);
Pushbutton butRight(35, PULL_UP_DISABLED);
Pushbutton butOk(34, PULL_UP_DISABLED);




void setup()
{
	setVref(false);
	initRTC();
	menu.init();
	initMux();
	LoadFromEEPROM();
	
	lcd.begin(LCD_COL, LCD_ROWS);
	lcd.createChar(GLYPH::upArrowIndex, GLYPH::upArrow);
	lcd.createChar(GLYPH::downArrowIndex,GLYPH::downArrow);
	lcd.createChar(GLYPH::rightArrowIndex, GLYPH::rightArrow);
	lcd.createChar(GLYPH::leftArrowIndex, GLYPH::leftArrow);
	attachFunctionToLine();
	enableScrolling();
	putInProgmem();
	addScreens();
	setGlyph();
	
	
	menu.set_focusPosition(Position::LEFT);
	menu.set_focusSymbol(Position::LEFT, GLYPH::rightArrow);
	
	
	menu.update();
	menu.switch_focus(true);
	initSensor();
	
	while(1)
	{
		buttonsCheck(butBack, butLeft, butDown, butUp, butRight, butOk);
		if(millis()-screenUpdateLastMs > screenUpdatePeriod)
		{
			screenUpdateLastMs = millis();
			if(((millis()-measureLastMs) > IntervalMeasure) && (current_screen == &Measuring_Screen))
			{
				measureLastMs = millis();
				getAllMeasures();
				saveToSD();	
			}
			menu.softUpdate();
		}
	}
}

	
	
	
	
void loop(){} //already have while(1) in setup 

