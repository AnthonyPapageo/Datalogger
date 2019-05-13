/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//////GENERAL C++/////////
#include <stdint.h>

//////////////AVR//////////////
#include <avr/io.h>
#include <avr/interrupt.h>
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


void enableIntButton(void);
void buttonsCheck(void);
void initInterruptDelay(void);


//////////UPDATE SPEED///////
uint32_t screenUpdatePeriod = 500;
uint32_t screenUpdateLastMs = 0;
/////////UPDATE MEASURE///////
uint32_t measureLastMs = 0;


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
volatile bool bTime = false;

ISR(TIMER1_COMPA_vect)
{
	bTime = true;
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
	initInterruptDelay();
	enableIntButton();
	
	
	setVref(false); //disable LT1461
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
	
	while(1) //TODO, maybe to with TIMER ISR for measure
	{
		buttonsCheck();
		if(((millis()-measureLastMs) > IntervalMeasure) && (current_screen == &Measuring_Screen)) //taking measure
		{
			measureLastMs = millis();
			getAllMeasures();
			saveToSD();
			if(Global_Current_DateTime.secondstime() > Global_End_Datetime.secondstime()) //more time than needed has passed so we stop
			{
				endMeasures();
			}
			menu.softUpdate();
		}
		PORTA ^= (1<<0);
	}
}

	
	
	
	
void loop(){} //already have while(1) in setup 

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
		while((menu.call_function(i)) && (i<= 5))
		//Call the function and check what it returns
		//if the call_function returns false, there is no more function
		//so we get out. Verify that we don't go over 5 (reserved functions)
		{
			i++;
		}
		return;
	}
	_delay_ms(300);
}

void initInterruptDelay(void)
{
	// set up Timer 1
	TCCR1A = 0;          // normal operation
	TCCR1B = bit(WGM12) | bit(CS10) | bit (CS12);   // CTC, scale to clock / 1024
	OCR1A =  15625;       // compare A register value (1000 * clock speed / 1024)
	TIMSK1 = bit (OCIE1A);             // interrupt on Compare A Match
	sei();
}

