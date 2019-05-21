/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//////GENERAL C++/////////
#include <stdint.h>

//////////////AVR//////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

//////////////ARDUINO/////////////
#include <LiquidCrystal.h>
#include <SPI.h>
#include <LiquidMenu.h>
#include <Pushbutton.h>
#include <INA.h>
#include <Wire.h>


//////////////CUSTOM//////////////
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/utility.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/MAX31856.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/global.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/menu.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/measure.h"


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void enableIntButton(void);
void buttonsCheck(void);



///////OBJECT INSTANTATION///////
LiquidCrystal lcd(RS, RW, ENABLE_LCD, D4, D5, D6, D7);//4bit
INA_Class INA;
LiquidMenu menu(lcd);

/*Pushbutton butBack(40,PULL_UP_DISABLED); //Extern RC debounce+ pull up so no need of internal pull up
Pushbutton butLeft(41, PULL_UP_DISABLED);
Pushbutton butDown(37, PULL_UP_DISABLED);
Pushbutton butUp(36, PULL_UP_DISABLED);
Pushbutton butRight(35, PULL_UP_DISABLED);
Pushbutton butOk(34, PULL_UP_DISABLED);*/

volatile bool bOk = false;
volatile bool bUp = false;
volatile bool bRight = false;
volatile bool bLeft = false;
volatile bool bDown = false;
volatile uint32_t counterInterrupt = 0;

ISR(TIMER1_COMPA_vect)
{
	if(Global_test_launched) //first time we get in the interrupt 
	{
		Global_test_launched = false;
		counterInterrupt = 0;
	}
	counterInterrupt++;
	PORTA ^=(1<<3);
}

ISR(INT2_vect) //OK button
{
	bOk =  true;
	_delay_us(2000);
}

ISR(INT4_vect) //Up button
{
	bUp = true;
	_delay_us(20);
}

ISR(INT5_vect) //Right button
{
	bRight = true;
	_delay_us(20);
}

ISR(INT6_vect) //Down button
{
	bDown = true;
	_delay_us(20);
}
ISR(INT7_vect) //Left button
{
	bLeft = true;
	_delay_us(20);
}




void setup()
{
	Wire.begin();
	enableIntButton();
	
	enableVref(false); //disable LT1461
	initRTC(); //set RTC time of compilation if it has lost power
	menu.init();
	//initMux(); //Set mux pins as output and enable multiplexer
	
	
	lcd.begin(LCD_COL, LCD_ROWS);
	lcd.createChar(GLYPH::upArrowIndex, GLYPH::upArrow);
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
	
	LoadFromEEPROM(); //Load last config
	
	menu.update();
	menu.switch_focus(true);
	
	DDRA = 0xFF;
	
	while(1) 
	{
		buttonsCheck();
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
		PORTA ^= (1<<0);
	}
}

	
void loop(){} //already have while(1) in setup 
	
void wdt_init(void)
{
	MCUSR = 0;
	wdt_disable();
	return;
}

void enableIntButton(void)
{
	DDRD &= ~(1 << 2); //INT2 as input
	DDRE &= 0x0F; //INT 7:4 as input

	cli(); //Disable Global Interrupt
	EIMSK = 0; //Disable all external interrupt
	EICRA |= (1 << 5); //Falling edge on INT2
	EICRA &= ~(1 << 4);
	EICRB = 0xAA; //0b1010 1010 Falling edge INT7:4
	EIMSK |= 0xF4; //0b1111 0100 Enable external interrupt
	sei(); //Enable Global Interrupt
}


void buttonsCheck(void)
{
	current_screen = menu.get_currentScreen();
	
	if(bUp)
	{
		bUp = false;
		menu.switch_focus(false); //Go up
		return;
	}
	else if(bDown)
	{
		bDown = false;
		menu.switch_focus(true); //Go down
		return;
	}
	else if(bRight)
	{
		bRight = false;
		menu.call_function(7); //Increment function saved on the seventh
		return;
	}
	else if(bLeft)
	{
		bLeft = false;
		menu.call_function(6); //Decrement function saved on the sixth
		return;
	}
	else if(bOk)
	{
		bOk=false;
		uint8_t i = 1;
		while((menu.call_function(i)) && (i<= 5) && (menu.get_currentScreen() == current_screen))
		//Call the function and check what it returns
		//if the call_function returns false, there is no more function
		//so we get out. Verify that we don't go over 5 (reserved functions)
		//if we changed screen, it's we don't need to call an other function 
		{
			i++;
		}
		return;
	}
	_delay_ms(100);
}

