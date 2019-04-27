#include <stdint.h>

#include <LiquidMenu.h>
#include <Pushbutton.h>

#include <avr/io.h>
#include <avr/eeprom.h>

#include "utility.h"
#include "global.h"
#include "menu.h"

void initMux(void)
{
	DDRE |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7); //MUX address data as output
	DDRB |= (1 << 0) | (1 << 4) | (1 << 5) | (1 << 6);//SS , !CS_MUX, !EN_MUX, !WR_MUX as output
	PORTB |= (1 << 5); //WR_MUX = 1
	PORTB &= ~(1 << 4); //CS_MUX = 0
	PORTB &= ~(1 << 6); //EN_MUX = 0
	DDRL |= (1 << 4); //OEBUF as output
	PORTL |= (1 << 4); //OEBUF = 1, disable output
	
}


void setMux(const uint8_t value)
{
	if (value < 20)
	{
		PORTL &= ~(1 << 4); //OEBUF = 0, enable output of 3v3 to 5V buffer
		PORTB &= ~(1 << 6); //Enable_MUX = 0, enable the multiplexer
		PORTB &= ~(1 << 0); //SS = 0
		PORTB &= ~(1 << 5); //WR_MUX = 0 so we can latch
		PORTE |= (value << 3); //Mux pins are on the 5 last pin of PORT E
		delayMicroseconds(10);
		PORTB |= (1 << 5); //Latch address mux input
	}
	else if (value == CS_DISABLE)
	{
		PORTB |=(1 << 0); //SS = 1
	}
}

void setVref(bool b)
{
	DDRG |= (1 << 2); //SHDN pin as output
	if (b)
	{
		PORTG |=(1 << 2);
	}
	else
	{
		PORTG &= ~(1 << 2);
	}
}


bool IsSdInserted(void)
{
	DDRL &=~(1<<5); //SD_Detect as input
	if (PINL & (1<<5)) //The pin is up so there is no SD card
	{
		return false;
	}
	else			//The pin is set to ground, there is a SD card
	{
		return true;
	}
}

void saveToEEPROM(void)
{
	eeprom_update_byte(&NV_NTC_nb, NTC_nb);
	eeprom_update_byte(&NV_TC_nb, TC_nb);
	eeprom_update_byte(&NV_I_nb, I_nb);
	eeprom_update_byte(&NV_V24_nb, V24_nb);
	eeprom_update_byte(&NV_V5_nb, V5_nb);
	
	eeprom_update_float(&NV_R_25, R_25);
	eeprom_update_float(&NV_B_FACTOR, B_FACTOR);
	
	eeprom_update_byte(&NV_IntervalSeconds, IntervalSeconds);
	eeprom_update_byte(&NV_IntervalMinutes, IntervalMinutes);
	
	eeprom_update_word(&NV_R_SHUNT, R_SHUNT);
	
}

void LoadFromEEPROM(void)
{
	NTC_nb = eeprom_read_byte(&NV_NTC_nb);
	TC_nb = eeprom_read_byte(&NV_TC_nb);
	I_nb = eeprom_read_byte(&NV_I_nb);
	V24_nb = eeprom_read_byte(&NV_V24_nb);
	V5_nb = eeprom_read_byte(&NV_V5_nb);
	R_25 = eeprom_read_float(&NV_R_25);

	B_FACTOR = eeprom_read_float(&NV_B_FACTOR);
	
	IntervalSeconds = eeprom_read_byte(&NV_IntervalSeconds);
	IntervalMinutes = eeprom_read_byte(&NV_IntervalMinutes);
	
	R_SHUNT = eeprom_read_word(&NV_R_SHUNT);
	if(NTC_nb == 255){correctEEPROM();} //Check if the EEPROM is not full of 1, if it is correct to the default value

}

void correctEEPROM(void)
{
	if(NTC_nb == 255){NTC_nb = 0; } 
	if(TC_nb == 255){TC_nb = 0 ;}
	if(I_nb == 255){I_nb = 0; }
	if(V24_nb ==255){V24_nb = 0;}
	if(V5_nb ==255){V5_nb = 0;}
	if(R_SHUNT == 65535){R_SHUNT = 2000.0;}
	
	if(isnan(R_25))
	{
		R_25 = 10000.0;
	}
	if(isnan(B_FACTOR))
	{
		B_FACTOR = 3977.0;
	}
}


/*void centerText(char *src,char *dest)
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
}*/



void buttonsCheck(Pushbutton& back, Pushbutton& left , Pushbutton& down, Pushbutton& up,Pushbutton& right, Pushbutton& Ok)
{
	
	current_screen = menu.get_currentScreen();
	
	if(up.getSingleDebouncedPress())
	{
		menu.switch_focus(false); //Go up
	}
	else if(down.getSingleDebouncedPress())
	{
		menu.switch_focus(true); //Go down
	}
	else if(right.getSingleDebouncedPress())
	{
		if((current_screen == &SettingsNumber_Screen) || (current_screen == &SettingsInterval_Screen) ||
		(current_screen == &B_value_selection_Screen) || (current_screen ==&R_25_Selection_Screen) ||(current_screen ==&RshuntSelection_Screen) )
		{
			menu.call_function(7); //Increment function saved on the seventh
		}
	}
	else if(left.getSingleDebouncedPress())
	{
		if((current_screen == &SettingsNumber_Screen) || (current_screen == &SettingsInterval_Screen) ||
		(current_screen == &B_value_selection_Screen) || (current_screen ==&R_25_Selection_Screen) || (current_screen ==&RshuntSelection_Screen) )
		{
			menu.call_function(6); //Decrement function saved on the sixth
		}
		
	}
	else if(Ok.getSingleDebouncedPress())
	{
		uint8_t i = 1 ;
		while((menu.call_function(i)) && (i<= 5))
		//Call the function and check what it returns
		//if the call_function returns false, there is no more function
		//so we get out. Verify that we don't go over 5 (reserved functions)
		{
			i++;
		}
	}
	else if (back.getSingleDebouncedPress()) //TODO CHANGE TO GO BACK 1 LEVEL ABOVE
	{
		menu.previous_screen();
	}
	
	current_screen = menu.get_currentScreen();//update variable
}


void emptyfunction(void) //To attach to line to enable scrolling
{
}




