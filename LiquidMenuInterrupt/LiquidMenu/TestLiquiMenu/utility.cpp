
#include <stdint.h>
#include <string.h>

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Pushbutton.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "utility.h"
#include "ADS1241.h"





void setMux(uint8_t value) //Only for proto, need change for final version
{
	DDRH |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); //OE MUX and data as output
	PORTH &= ~(1 << 0); //OE MUX value = 0
	DDRL |= (1 << 0);//OE BUF as output
	PORTL &= ~(1 << 0);  //OE BUF = 0 for 3V3 MISO
	switch (value)
	{
	case 0:
	  PORTH &= ~(1 << 3); ///000
	  PORTH &= ~(1 << 2);
	  PORTH &= ~(1 << 1);
	  PORTL |= (1 << 0); //Enable 3v3 MISO
	  break;
	case 1:
	  PORTH |= (1 << 3); ///001
	  PORTH &= ~(1 << 2);
	  PORTH &= ~(1 << 1);
	  break;
	case 2:
	  PORTH &= ~(1 << 3); ///010
	  PORTH |= (1 << 2);
	  PORTH &= ~(1 << 1);
	  PORTL |= (1 << 0);//Enable 3v3 MISO
	  break;
	default:
	  PORTH |= (1 << 0); //disable mux
	  break;
	}
}



////////////END ISR////////////////


bool checkOk(void)
//Check bool for OK button, return bOK value and set it back to false//
{
	if (bOK)
	{
	bOK = false;
	return true;
	}
	else
	{
	return false;
	}
}


void enableIntButton(void)
{
	DDRD &= ~(1 << 2); //INT2 as input
	DDRE &= 0x0F; //INT 7:4 as input

	cli(); //Disable Global Interrupt
	EIMSK = 0; //Disable all external interrupt
	EICRA |= (1 << 5); //Falling edge on INT2
	EICRA &= ~(1 << 4);
	EICRB |= 0xAA; //0b1010 1010 Falling edge INT7:4
	EIMSK |= 0xF4; //0b1111 0100 Enable external interrupt
	sei(); //Enable Global Interrupt
	
}


void calibrateGainADC(LiquidCrystal& lcd, ADS1241& ADS1241) 
{
	lcd.clear();
	lcd.home();
	lcd.print("Set 24V on V1");
	lcd.setCursor(0,1);
	lcd.print("Press OK when ready");
	while(checkOk());
	ADS1241.calibrateGain(1);
	lcd.clear();
	lcd.print("Gain calibrated");
}



void calibrateOffsetADC(LiquidCrystal& lcd, ADS1241& ADS1241)
{
	lcd.clear();
	lcd.home();
	lcd.print("Set 0V on V1");
	lcd.setCursor(0,1);
	lcd.print("Press OK when ready");
	while(checkOk());
	ADS1241.calibrateOffset(1);
	lcd.clear();
	lcd.print("Offset calibrated");
	
}

void displayMenu(LiquidCrystal& lcd, const char tab[4][21])
{
	int i;
	for (i = 0; i < 4; i++)
	{
	lcd.print(tab[i][21]);
  }
}

void displayValue(LiquidCrystal& lcd,ADS1241& ADS1241, uint8_t channel)
{
	uint32_t value;
	value = ADS1241.getVoltage(channel);
	y++; //Move the cursor on the next line
	x = 0;
	lcd.setCursor(x,y);
	x +=lcd.print("Channel");
	x +=lcd.print(channel);
	x += lcd.print(":");
	 //move to the right to display the value
	lcd.setCursor(x,y);
	x += lcd.print(value, DEC);
	lcd.print("mV");
}


void centerText(char *src,char *dest)
{
	int8_t i,toPrint;
	uint8_t len;
	uint8_t compteur = 0;
	uint8_t toPrintBef;
	len = strlen(src);
	if(LCD_COL>20)
	{
		return;
	}
	toPrint = LCD_COL - len;//Compute how much to fill
	toPrintBef = (toPrint/2) -1; 
	for(i=0; i<toPrintBef; i++)
	{
		dest[compteur] = ' ';
		compteur++;
	}
	for(i=0; i<len; i++ )
	{
		dest[compteur] = src[i];
		compteur++;
	}
	for(i=0; compteur < (LCD_COL-1); i++)
	{
		dest[compteur] = ' ';
		compteur++;
	}

}

void buttonsCheck(Pushbutton& up, Pushbutton& down, Pushbutton& OK, LiquidMenu& menu)
{
	if(up.getSingleDebouncedPress())
	{
		menu.switch_focus(false); //Go up
	}
	else if(down.getSingleDebouncedPress())
	{
		menu.switch_focus(true); //Go down
	}
	else if(OK.getSingleDebouncedPress())
	{
		menu.call_function(1);//call the focused line function
	}	
	
}
	
	
	

	