#include <stdint.h>

#include <LiquidMenu.h>
#include <Pushbutton.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>
#include <SD.h>

#include "utility.h"
#include "global.h"
#include "menu.h"
#include "measure.h"


#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)




String FileName = "default.csv";


////////Static Prototype///////
static void SdWriteInt(int32_t data);
static void SdWriteFloat(float fdata, const uint8_t precision);
static void correctEEPROM(void);
static void SdWriteTime(void);


/*void initMux(void)
{
	DDRE |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7); //MUX address data as output
	DDRB |= (1 << 0) | (1 << 4) | (1 << 5) | (1 << 6);//SS , !CS_MUX, !EN_MUX, !WR_MUX as output
	PORTB |= (1 << 5); //!WR_MUX = 1
	PORTB &= ~(1 << 4); //!CS_MUX = 0
	PORTB &= ~(1 << 6); //!EN_MUX = 0 
	DDRL |= (1 << 4); //OEBUF as output
	PORTL |= (1 << 4); //OEBUF = 1, enable output of 3v3 to 5V buffer
	
}


void setMux(const uint8_t value)
{
	if (value < 20)
	{
		PORTB &= ~(1 << 0); //SS = 0
		PORTB &= ~(1 << 5); //WR_MUX = 0 so we can latch
		PORTE |= (value << 3); //Mux pins are on the 5 last pin of PORT E
		_delay_us(10);
		PORTB |= (1 << 5); //Latch address mux input
	}
	else if (value == CS_DISABLE)
	{
		PORTB |=(1 << 0); //SS = 1
	}
}*/

void setMux(const uint8_t value) //Only for proto, need change for final version

{
	DDRH |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); //OE MUX and data as output
	PORTH &= ~(1 << 0); //OE MUX value = 0
	DDRL |= (1 << 0);//OE BUF as output
	
	switch (value)
	{
		case 0:
		PORTH &= ~(1 << 3); ///000
		PORTH &= ~(1 << 2);
		PORTH &= ~(1 << 1);
		PORTL &= ~(1 << 0); //Enable 3v3 MISO by setting !OE_BUF at 0
		break;
		case 1:
		PORTH |= (1 << 3); ///001
		PORTH &= ~(1 << 2);
		PORTH &= ~(1 << 1);
		PORTL |= (1 << 0);//disable 3v3 MISO
		break;
		case 2:
		PORTH &= ~(1 << 3); ///010
		PORTH |= (1 << 2);
		PORTH &= ~(1 << 1);
		PORTL &= ~(1 << 0); //Enable 3v3 MISO by setting !OE_BUF at 0
		break;
		default:
		PORTH |= (1 << 3);
		PORTH |= (1 << 2);
		PORTH |= (1 << 1);
		PORTH |= (1 << 0); //disable mux
		break;
	}
	_delay_us(20);
}

void initRTC(void)
{
	RTC.begin();
	if(RTC.lostPower())
	{
		RTC.adjust(DateTime(F(__DATE__),F(__TIME__)));
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


bool IsSdInserted(void) //TODO CHANGE FOR FINAL
{
	DDRE &=~(1<<2); //SD_Detect as input
	if (PINE & (1<<2)) //The pin is up so there is no SD card
	{
		return false;
	}
	else			//The pin is set to ground, there is a SD card
	{
		return true;
	}
	/*DDRL &=~(1<<5); //SD_Detect as input
	if (PINL & (1<<5)) //The pin is up so there is no SD card
	{
		return false;
	}
	else			//The pin is set to ground, there is a SD card
	{
		return true;
	}*/
}

void saveToEEPROM(void)
{
	cli();//disable interrupt
	eeprom_busy_wait(); //wait until EEPROM is ready
	eeprom_update_byte(&NV_NTC_nb, NTC_nb);
	eeprom_update_byte(&NV_TC_nb, TC_nb);
	eeprom_update_byte(&NV_I_nb, I_nb);
	eeprom_update_byte(&NV_V24_nb, V24_nb);
	eeprom_update_byte(&NV_V5_nb, V5_nb);
	eeprom_update_float(&NV_R_25, R_25);
	eeprom_update_float(&NV_B_FACTOR, B_FACTOR);	
	eeprom_update_byte(&NV_IntervalSeconds, IntervalSeconds);
	eeprom_update_byte(&NV_IntervalMinutes, IntervalMinutes);
	eeprom_update_dword(&NV_R_SHUNT, R_SHUNT);
	//eeprom_update_byte(&NV_TC_Type, static_cast<uint8_t>(TC_Type[0])); //because eeprom function use uint8_t
	sei();//enable interrupt
}

void LoadFromEEPROM(void)
{
	cli(); //disable interrupt
	eeprom_busy_wait();
	NTC_nb = eeprom_read_byte(&NV_NTC_nb);
	TC_nb = eeprom_read_byte(&NV_TC_nb);
	I_nb = eeprom_read_byte(&NV_I_nb);
	V24_nb = eeprom_read_byte(&NV_V24_nb);
	V5_nb = eeprom_read_byte(&NV_V5_nb);
	R_25 = eeprom_read_float(&NV_R_25);
	B_FACTOR = eeprom_read_float(&NV_B_FACTOR);
	IntervalSeconds = eeprom_read_byte(&NV_IntervalSeconds);
	IntervalMinutes = eeprom_read_byte(&NV_IntervalMinutes);
	R_SHUNT = eeprom_read_dword(&NV_R_SHUNT);
	//TC_Type[0] = static_cast<char>(eeprom_read_byte(&NV_TC_Type));	
	if((NTC_nb > MAX_NTC_NB) ||( TC_nb > MAX_TC_NB) || (I_nb> MAX_I_NB) || (V5_nb > MAX_V5_NB) || (V24_nb > MAX_V24_NB) 
	|| (IntervalMinutes >30) || (IntervalSeconds>60)) 
	{
		correctEEPROM();
	} //Check if the EEPROM is not full of 1, if it is, correct to the default value
	sei(); //enable interrupt
}

static void correctEEPROM(void) 
{
	if(NTC_nb > MAX_NTC_NB){NTC_nb = 0;}
	if(TC_nb > MAX_TC_NB){TC_nb = 0 ; }
	if(I_nb > MAX_I_NB){I_nb = 0; }
	if(V24_nb > MAX_V24_NB){V24_nb = 0; }
	if(V5_nb > MAX_V5_NB){V5_nb = 0; }
	if(R_SHUNT == 65535){R_SHUNT = DEFAULT_R_SHUNT; }
	
	if(isnan(R_25)) //check if the value is a NaN
	{
		R_25 = 10000.0;
	}
	if(isnan(B_FACTOR))
	{
		B_FACTOR = 3977.0;
	}
	IntervalMinutes = 0;
	IntervalSeconds = 1;
	saveToEEPROM(); //Save the new value 
}

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
	else if(right.isPressed())
	{
		menu.call_function(7); //Increment function saved on the seventh
		_delay_ms(500);
	}
	else if(left.isPressed())
	{
		menu.call_function(6); //Decrement function saved on the sixth
		_delay_ms(500);
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
	else if (back.getSingleDebouncedPress())
	{
		if((current_screen == &Settings_Screen) || (current_screen == &Launch_Screen) ) // Were are just after main screen
		{
			gotoMainScreen();
		}
		else if((current_screen == &R_25_Selection_Screen) || (current_screen == &B_value_selection_Screen))
		{
			gotoNTCScreen();
		}
		else if(current_screen != &Measuring_Screen)
		{
			gotoSettingsScreen();
		}
	}
	current_screen = menu.get_currentScreen();//update variable
}

void initInterruptTimer(void)
{
	// set up Timer 1
	cli();
	TCCR1A = 0;          // normal operation
	TCCR1B = bit(WGM12) | bit(CS10) | bit (CS12);   // CTC, scale to clock / 1024
	OCR1A =  15625;       // compare A register value (1000 * clock speed / 1024), 1s
	TIMSK1 = bit (OCIE1A);             // interrupt on Compare A Match
	Global_test_launched = true;
	sei();
}

void stopTimer(void)
{
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10); //select no clock source
	TIMSK1 &= ~(OCIE1A); //disable interrupt
}




void resetToDefault(void)
{
	NTC_nb = 0;
	TC_nb = 0;
	I_nb = 0;
	V24_nb = 0;
	V5_nb = 0;
	/////////INTERVAL///////////
	IntervalSeconds = 1;
	IntervalMinutes = 0;
	/////////DURATION///////////
	DurationHour = 0;
	DurationMin = 1;
	DurationSec = 0;
	/////////TIME ELAPSED///////
	ElapsedSeconds = 0;
	ElapsedMinutes = 0;
	ElapsedHours = 0;
	
	 B_FACTOR = 3977.0;
	 R_25 = 10000.0;
	 saveToEEPROM();
}



void setFileName(void)
{
	String str = "";
	str = String(Global_Begin_Datetime.day()) + "-" + String(Global_Begin_Datetime.month()) + "-" + String(Global_Begin_Datetime.year()); //Folder is date
	str = "/" + str;
	SD.begin(CS_SD);
	SD.mkdir(str);
	FileName = str + "/" + String(Global_Begin_Datetime.hour()) + "-" + String(Global_Begin_Datetime.minute()) + ".csv"; //File is time of beginning
	
}

void firstLineSD(void) //Write the header of the CSV file and get the beginning time
{
	uint8_t i;
	String temp;
	computeTime();
	setFileName();
	File dataFile = SD.open(FileName, FILE_WRITE);
	led_SD(true);//Assert that we are writing to the SD card
	
	if(dataFile)
	{
		
		dataFile.print("N°;");
		for(i = 0; i<TC_nb;i++)
		{
			temp = "TC" + String(i+1) +";";
			dataFile.print(temp);
		}
		for(i=0; i<NTC_nb;i++)
		{
			temp = "NTC" + String(i+1) +";";
			dataFile.print(temp);
		}
		for(i=0;i<V24_nb;i++)
		{
			temp = "V" + String(i+1) +";";
			dataFile.print(temp);
		}
		for(i=0;i<V5_nb;i++)
		{
			temp = "V" + String(i+3) +";"; //V5 begin at V3
			dataFile.print(temp);
		}
		for(i=0;i<I_nb;i++)
		{
			temp = "I" + String(i+1) +";"; //I
			dataFile.print(temp);
		}
		temp = "Date;";
		dataFile.print(temp);
		
		temp = "Time;"; //last column
		dataFile.print(temp);
		
		dataFile.close();
	}
	
	/*else //uncomment on final product
	{
		lcd.clear();
		lcd.home();
		lcd.print("ERROR");
		lcd.setCursor(0,1);
		lcd.print("SD CARD UNREADABLE");
		lcd.setCursor(0,2);
		lcd.print("RESET IN 5sec");
		_delay_ms(5000);
		soft_reset();
	}*/
	
	led_SD(false);
}

void saveToSD(void)
{
	uint8_t i;
	String temp;
	SdWriteInt(static_cast<int32_t>(Nb_Of_Measure)); // first column
	for(i = 0; i<TC_nb;i++)
	{
		SdWriteFloat(TC_Measure_Array[i],1);
	}
	for(i=0; i<NTC_nb;i++)
	{
		SdWriteFloat(NTC_Measure_Array[i],1);
	}
	for(i=0;i<V24_nb;i++)
	{
		SdWriteFloat(V24_Measure_Array[i],3);
	}
	for(i=0;i<V5_nb;i++)
	{
		SdWriteFloat(V5_Measure_Array[i],3);
	}
	for(i=0;i<(2*I_nb);i++)
	{
		SdWriteInt(I_Measure_Array[i]);
	}
	
	SdWriteTime();
	
	led_SD(false);
	
}

static void SdWriteInt(int32_t data)
{
	String string_data = ""; 
	File dataFile = SD.open(FileName, FILE_WRITE);
	string_data = String(data) + ";";
	if(dataFile)
	{
		led_SD(true);//Assert that we are writing to the SD card
		dataFile.print(string_data);
		dataFile.close();
	}
	led_SD(false);
}

static void SdWriteFloat(float fdata, const uint8_t precision )
{
	String string_data = "";
	File dataFile = SD.open(FileName, FILE_WRITE);
	string_data = String(fdata,precision) + ";";
	if(dataFile)
	{
		led_SD(true);//Assert that we are writing to the SD card
		dataFile.print(string_data);
		dataFile.close();
	}
	led_SD(false);
}

static void SdWriteTime(void) //first the date then the time
{
	String temp;
	DateTime dt = RTC.now();
	Global_Current_DateTime = dt;
	File dataFile = SD.open(FileName, FILE_WRITE);
	if(dataFile)
	{
		led_SD(true);
		temp = String(dt.day()) + "-" + String(dt.month()) + ";";
		dataFile.print(temp);
		temp = String(dt.hour()) + "-"  + String(dt.minute()) + "-" + String(dt.second()) + ";" ;
	}
	led_SD(false);
}


void led_SD(bool b)
{
	DDRL |= (1 << 7);
	if(b)
	{
		PORTL |=(1 << 7);
	}
	else
	{
		PORTL &= ~(1 << 7);
	}
	
}

void emptyfunction(void) //To attach to line to enable scrolling
{
}







