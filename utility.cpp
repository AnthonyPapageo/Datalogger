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
String path = "/";


////////Static Prototype///////
static void SdWriteInt(File& dataFile,int32_t data);
static void SdWriteFloat(File& dataFile,float fdata, const uint8_t precision);
static void SdWriteTime(File& dataFile);
static void correctEEPROM(void);



void initMux(void)
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
	PORTE &= 0x07; //clear PE3 to PE7
	if (value < 20)
	{
		PORTB &= ~(1 << 0); //SS = 0
		PORTB &= ~(1 << 5); //WR_MUX = 0 so we can latch
		PORTE |= (value << 3); //Mux pins are on the 5 last pin of PORT E
		_delay_us(10);
		PORTB |= (1 << 5); //Latch address mux input
	}
	else
	{
		_delay_us(10);
		PORTB |=(1 << 0); //SS = 1
	}
}

void initRTC(void)
{
	RTC.begin();
	if(RTC.lostPower())
	{
		RTC.adjust(DateTime(F(__DATE__),F(__TIME__)));
	}
}

void enableVref(bool b)
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
	R_25 = 10000.0;
	B_FACTOR = eeprom_read_float(&NV_B_FACTOR);
	IntervalSeconds = eeprom_read_byte(&NV_IntervalSeconds);
	IntervalMinutes = eeprom_read_byte(&NV_IntervalMinutes);
	R_SHUNT = eeprom_read_dword(&NV_R_SHUNT);
	//TC_Type[0] = static_cast<char>(eeprom_read_byte(&NV_TC_Type));	
	if((NTC_nb > MAX_NTC_NB) ||( TC_nb > MAX_TC_NB) || (I_nb> MAX_I_NB) || (V5_nb > MAX_V5_NB) || (V24_nb > MAX_V24_NB) 
	|| (IntervalMinutes >30) || (IntervalSeconds>60) || R_SHUNT > 100000 ) 
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
	if(R_SHUNT > 100000){R_SHUNT = DEFAULT_R_SHUNT; }
	
	if(isnan(R_25)) //check if the value is a NaN
	{
		R_25 = 10000.0;
	}
	if(isnan(B_FACTOR))
	{
		B_FACTOR = 4000.0;
	}
	IntervalMinutes = 0;
	IntervalSeconds = 1;
	saveToEEPROM(); //Save the new value 
}

void buttonsCheck(void)//Pushbutton& back,Pushbutton& left ,Pushbutton& down,Pushbutton& up ,Pushbutton& right , Pushbutton& Ok)
{
	
	current_screen = menu.get_currentScreen();
	
	if(!(PINC & (1<<1)))
	{
		menu.switch_focus(false); //Go up
		_delay_ms(300);
	}
	else if(!(PINC & (1<<0)))
	{
		menu.switch_focus(true); //Go down
		_delay_ms(300);
	}
	else if(!(PINC & (1<<2)))
	{
		menu.call_function(7); //Increment function saved on the seventh
		_delay_ms(150);
	}
	else if(!(PING & (1<<0)))
	{
		menu.call_function(6); //Decrement function saved on the sixth
		_delay_ms(150);
	}
	else if(!(PINC & (1<<3)))//else if(Ok.isPressed())
	{
		uint8_t i = 1 ;
		while((menu.call_function(i)) && (i<= 5) && (menu.get_currentScreen() == current_screen))
		//Call the function and check what it returns
		//if the call_function returns false, there is no more function
		//so we get out. Verify that we don't go over 5 (reserved functions)
		// if the screen changed, we need to stop calling the functions
		{
			i++;
		}
		_delay_ms(300);
	}
	else if (!(PING & (1<<1)))
	{
		if((current_screen == &Settings_Screen) || (current_screen == &Launch_Screen) ) // We are are just after main screen
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
		_delay_ms(300);
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
	
	 B_FACTOR = 4000.0;
	 R_25 = 10000.0;
	 saveToEEPROM();
}



void setFileName(void)
{
	DateTime dt = RTC.now();
	uint16_t temp;
	temp = dt.year(); -2000; 
	//path = String(temp) + "-" + String(dt.month()) + "-" + String(dt.day()) ;
	path = String(dt.day()) + "-" + String(dt.month())  + "/"; //Folder is date + "-" + String(dt.year())
	SD.begin(CS_SD);
	SD.open("/");
	SD.mkdir(path);
	FileName = path + String(dt.hour()) + "h" + String(dt.minute()) + ".csv"; //File is time of beginning
}

void firstLineSD(void) //Write the header of the CSV file and get the beginning time
{
	uint8_t i;
	String temp;
	if(!Global_Is_multifile) //we don't need to calculate time when it's multifile
	{
		computeTime();
	}
	setFileName();
	SD.begin(CS_SD);
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
			temp = "I" + String(i+1) +"(uA);"; //I
			dataFile.print(temp);
			temp = "I" + String(i+1) +"_vBus(mV);"; //I
			dataFile.print(temp);
		}
		dataFile.print("Card temp;");
		temp = "Date;";
		dataFile.print(temp);
		
		temp = "Time;"; //last column
		dataFile.println(temp);
		
		dataFile.close();
	}
	
	else 
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
	}
	led_SD(false);
}

void saveToSD(void)
{
	uint8_t i;
	String temp;
	float average = 0.0;
	SD.begin(CS_SD);
	File dataFile = SD.open(FileName, FILE_WRITE);
	SdWriteInt(dataFile,static_cast<int32_t>(Nb_Of_Measure)); // first column
	for(i = 0; i<TC_nb;i++)
	{
		SdWriteFloat(dataFile,TC_Measure_Array[i],1);
		average += CJT_Measure_Array[i];
	}
	for(i=0; i<NTC_nb;i++)
	{
		SdWriteFloat(dataFile,NTC_Measure_Array[i],1);
	}
	for(i=0;i<V24_nb;i++)
	{
		SdWriteFloat(dataFile,V24_Measure_Array[i],3);
	}
	for(i=0;i<V5_nb;i++)
	{
		SdWriteFloat(dataFile,V5_Measure_Array[i],3);
	}
	for(i=0;i<(2*I_nb);i++)
	{
		SdWriteInt(dataFile,I_Measure_Array[i]);
	}
		
	average = average / static_cast<float>(TC_nb);
	SdWriteFloat(dataFile,average,1);	//Card temp
	SdWriteTime(dataFile);
	dataFile.println();
	dataFile.close();

	led_SD(false);
	
}

static void SdWriteInt(File& dataFile, int32_t data)
{
	String string_data = "";
	string_data = String(data) + ";";
	if(dataFile)
	{
		led_SD(true);//Assert that we are writing to the SD card
		dataFile.print(string_data);
	}
	led_SD(false);
}

static void SdWriteFloat(File& dataFile, float fdata, const uint8_t precision )
{
	String string_data = "";
	string_data = String(fdata,precision) + ";";
	if(dataFile)
	{
		led_SD(true);//Assert that we are writing to the SD card
		dataFile.print(string_data);
	}
	led_SD(false);
}

static void SdWriteTime(File& dataFile) //first the date then the time
{
	String temp;
	Global_Current_DateTime = RTC.now();
	if(dataFile)
	{
		led_SD(true);
		temp = String(Global_Current_DateTime.day()) + "-" + String(Global_Current_DateTime.month()) + ";";
		dataFile.print(temp);
		temp = String(Global_Current_DateTime.hour()) + ":"  + String(Global_Current_DateTime.minute()) + ":" + String(Global_Current_DateTime.second()) + ";" ;
		dataFile.print(temp);
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
	/*DDRG |= (1 << 5);
	if(b)
	{
		PORTG |= (1 << 5);
	}
	else
	{
		PORTG &= ~(1 << 5);
	}*/
	
	
}

void emptyfunction(void) //To attach to line to enable scrolling
{
}







