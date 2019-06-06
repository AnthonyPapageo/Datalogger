/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//////GENERAL C++/////////
#include <stdint.h>

//////////////AVR//////////////
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

//////////////ARDUINO/////////////
#include <LiquidCrystal.h>
#include <SPI.h>
#include <LiquidMenu.h>
#include <Pushbutton.h>
#include <INA.h>
#include <RTClib.h>
#include <Wire.h>




//////////////CUSTOM//////////////
#include "utility.h"
#include "MAX31856.h"
#include "global.h"
#include "menu.h"
#include "measure.h"


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));


///////OBJECT INSTANTATION///////
volatile uint16_t counterInterrupt = 0;

Pushbutton butBack(40, PULL_UP_DISABLED); //Extern RC debounce + pull up so no need of internal pull up
Pushbutton butLeft(41, PULL_UP_DISABLED);
Pushbutton butDown(37, PULL_UP_DISABLED);
Pushbutton butUp(36, PULL_UP_DISABLED);
Pushbutton butRight(35, PULL_UP_DISABLED);
Pushbutton butOk(34, PULL_UP_DISABLED);


ISR(TIMER1_COMPA_vect)
{
	if(Global_test_launched) //first time we get in the interrupt 
	{
		Global_test_launched = false;
		counterInterrupt = 0;
	}
	counterInterrupt++;
	PORTJ ^=(1<<5); //blink LED
}


void setup()
{
	Wire.begin();
	Wire.setClock(INA_I2C_STANDARD_MODE);
	enableVref(false); //disable LT1461
	initRTC(); //set RTC time of compilation if it has lost power
	menu.init();
	initMux(); //Set mux pins as output and enable multiplexer
	
	lcd.begin(LCD_COL, LCD_ROWS);
	lcd.createChar(GLYPH::upArrowIndex, GLYPH::upArrow); //Add arrow glyphs
	lcd.createChar(GLYPH::downArrowIndex,GLYPH::downArrow);
	lcd.createChar(GLYPH::rightArrowIndex, GLYPH::rightArrow);
	lcd.createChar(GLYPH::leftArrowIndex, GLYPH::leftArrow);
	
	AddVariableToLine();
	attachFunctionToLine();
	enableScrolling();
	putInProgmem();
	addScreens();
	setGlyph();
	
	menu.set_focusPosition(Position::LEFT);
	menu.set_focusSymbol(Position::LEFT, GLYPH::rightArrow);
	

	LoadFromEEPROM(); //We do it here because we are sure that the power supply is already in a stable state
	menu.update();
	menu.switch_focus(true);
	DDRJ |= 0x60; //LED 2 and LED 3 as output
	DDRL |= (1<<6);
	PORTL |= (1<<6); //CS SD high
	SPI.beginTransaction(SPISettings(400000,MSBFIRST,SPI_MODE0));
	setMux(CS_DISABLE);
	
	while(1)
	{
		buttonsCheck(butBack, butLeft, butDown, butUp, butRight, butOk);
		if((counterInterrupt == IntervalMeasure) && (current_screen == &Measuring_Screen)) //Interval measures is the time interval we need in seconds
		{
			getAllMeasures();
			saveToSD();
			counterInterrupt = 0;
			menu.softUpdate();//update nb of measure
		}
		else if(Global_Current_DateTime.secondstime() > Global_End_Datetime.secondstime()) //the test is over
		{			
			endMeasures();
		}
	}
}

	
	
	
	
void loop(){} //already have while(1) in setup 

void wdt_init(void) //Disable watchdog after soft reset
{
    MCUSR = 0;
    wdt_disable();
    return;
}



