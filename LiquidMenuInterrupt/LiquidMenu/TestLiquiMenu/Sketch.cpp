/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//////GENERAL C++/////////
#include <stdint.h>

//////////////AVR//////////////
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//////////////ARDUINO/////////////
#include <LiquidCrystal.h>
#include <SPI.h>
#include <LiquidMenu.h>
#include <Pushbutton.h>
#include <INA.h>


//////////////CUSTOM//////////////
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/utility.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/ADS1241.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/constant.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/menu.h"


//////////UPDATE SPEED///////
uint32_t updatePeriod = 500;
uint32_t updateLastMs = 0;


///////OBJECT INSTANTATION///////
LiquidCrystal lcd(RS, RW, ENABLE_LCD, D0, D1, D2, D3, D4, D5, D6, D7);//8bit
ADS1241 ads1241;
INA_Class INA;
LiquidMenu menu(lcd);


ISR(INT2_vect) //OK button
{
	bOk =  true;
}

ISR(INT4_vect) //Up button
{
	bUp = true;
}

ISR(INT5_vect) //Right button
{
	bRight = true;
}

ISR(INT6_vect) //Down button
{
	bDown = true;
}
ISR(INT7_vect) //Left button
{
	bLeft = true;
}

/* for debug
LiquidCrystal lcd(RS, RW, ENABLE_LCD, D4, D5, D6, D7);//4 bit
Pushbutton buttonOK(19);//Button OK is on pin 19 of Arduino Mega pinout
ADS1241 ads1241; //for debug
Pushbutton buttonHigh(2);
Pushbutton buttonDown(3);*/



void setup()
{
	lcd.begin(LCD_COL, LCD_ROWS);
	lcd.createChar(upArrowIndex, upArrow);
	lcd.createChar(downArrowIndex, downArrow);
	lcd.createChar(rightArrowIndex, rightArrow);
	ads1241.begin(true, true);
	
	enableIntButton();
	

	INA.setI2CSpeed(INA_I2C_STANDARD_MODE); // 100kHz I2C
	INA.begin(3, 2000); //MAX 3A, 2mOhm resistor
	INA.setBusConversion(8500); // Maximum conversion time 8.244ms
	INA.setShuntConversion(8500); // Maximum conversion time 8.244ms
	INA.setAveraging(128); //Average 128 readings
	INA.setMode(INA_MODE_CONTINUOUS_BOTH); //read bus and shunt

	attachFunctionToLine();
	enableScrolling();
	putInProgmem();
	
	menu.add_screen(welcome_Screen);
	menu.add_screen(main_Screen);
	menu.add_screen(calibrateGain_Screen);
	menu.add_screen(calibrateOffset_Screen);
	menu.add_screen(calibrationDone_Screen);
	menu.add_screen(configureAdc_Screen);
	menu.add_screen(liveRead_Screen);
	menu.add_screen(INA_Screen);
	menu.add_screen(K_value_selection_c_Screen);
	
	menu.set_focusPosition(Position::LEFT);
	menu.set_focusSymbol(Position::LEFT, rightArrow);
	

	menu.update();
	menu.switch_focus(true);
	
	while(1)
	{
		buttonsCheck();
		if(millis()-updateLastMs > updatePeriod)
		{
			updateLastMs = millis();
			if (menu.get_currentScreen() == &liveRead_Screen) //Maybe stock the pointer in a variable
															// so we don't cal it multiple times
			{
				V1Value = ads1241.getVoltage(1);
				V2Value = ads1241.getVoltage(2);
				menu.update();
			}
			else if (menu.get_currentScreen() == &INA_Screen)
			{
				I1Value = INA.getBusMicroAmps();
				VBusValue = INA.getBusMilliVolts();
				menu.update();
			}
			menu.softUpdate();
		}
	}
}

	
	
	
	
void loop(){} //already have while(1) in setup 

