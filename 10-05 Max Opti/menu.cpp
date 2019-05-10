/*
* menu.cpp
*
* Created: 22/03/2019 13:14:24
*  Author: Anthony
*/

#include <LiquidMenu.h>
#include <util/delay.h>
#include "global.h"
#include "utility.h"
#include "menu.h"
#include "measure.h"



static void incrementNTC_nb(void);
static void decrementNTC_nb(void);
static void incrementTC_nb(void);
static void decrementTC_nb(void);
static void incrementI_nb(void);
static void decrementI_nb(void);
static void incrementV24_nb(void);
static void decrementV24_nb(void);
static void incrementV5_nb(void);
static void decrementV5_nb(void);

////Interval
static void incrementTempsSec(void);
static void decrementTempsSec(void);
static void incrementTempsMin(void);
static void decrementTempsMin(void);


//R_25
static void decrement10KR25(void);
static void increment10KR25(void);
static void decrement1KR25(void);
static void increment1KR25(void);
static void resetR25ToDefault(void);

//B_FACTOR
static void resetBFactorToDefault(void);
static void decrement100BFactor(void);
static void increment100BFactor(void);
static void decrement10BFactor(void);
static void increment10BFactor(void);
static void decrement1BFactor(void);
static void increment1BFactor(void);

//R_SHUNT
static void decrement1000Rshunt(void);
static void increment1000Rshunt(void);
static void decrement100Rshunt(void);
static void increment100Rshunt(void);
static void resetRshuntToDefault(void);

//TC_TYPE
static void incrementTcType(void);
static void decrementTcType(void);
const char* getTCType(void);

/////////ELAPSED TIME///////
static void getElapsedTime(void);
static void resetNumberOfMeasure(void);


//Duration
static void incrementDurationHour(void);
static void decrementDurationHour(void);
static void incrementDurationMinute(void);
static void decrementDurationMinute(void);
static void incrementDurationSec(void);
static void decrementDurationSec(void);



/////////////STRINGS//////////////////////
/////////////Welcome Screen////////////////////
const char welcome_text0[] PROGMEM = "DATALOGGER TAS-B";
const char welcome_text1[] PROGMEM = "TC AND NTC MEASURES";
const char welcome_text2[] PROGMEM = "Build on :";
const char welcome_text3[] PROGMEM =  __TIME__;
LiquidLine welcome_line0(1,0,welcome_text0);
LiquidLine welcome_line1(1,1,welcome_text1);
LiquidLine welcome_line2(1,2,welcome_text2);
LiquidLine welcome_line3(1,3,welcome_text3);
LiquidScreen welcome_Screen(welcome_line0, welcome_line1, welcome_line2, welcome_line3);

/////////////Main Screen////////////////////
const char main_text0[] PROGMEM = "    MAIN MENU";
const char main_text1[] PROGMEM = "1: SETTINGS";
const char main_text2[] PROGMEM = "2: LIVE MEASURE";
const char main_text3[] PROGMEM = "3: LAUNCH A TEST";
const char main_text4[] PROGMEM = "4: ";
const char main_text5[] PROGMEM = "5: ";
LiquidLine main_line0(1,0,main_text0);
LiquidLine main_line1(1,1,main_text1);
LiquidLine main_line2(1,2,main_text2);
LiquidLine main_line3(1,3,main_text3);
LiquidLine main_line4(1,4,main_text4);
LiquidLine main_line5(1,5,main_text5);
LiquidScreen main_Screen(main_line0,main_line1,main_line2,main_line3, main_line4,main_line5);


/////////////Settings Screen////////////////////
const char Settings_text0[] PROGMEM = " SETTINGS MENU";
const char Settings_text1[] PROGMEM = "1: NUMBER OF DEVICE";
const char Settings_text2[] PROGMEM = "2: TIME INTERVAL";
const char Settings_text3[] PROGMEM = "3: NTC CONFIG";
const char Settings_text4[] PROGMEM = "4: SHUNT VALUE";
const char Settings_text5[] PROGMEM = "5: TC TYPE";
const char Settings_text6[] PROGMEM = "6: SAVE CONFIG"; //Maybe plusieurs configuration
LiquidLine Settings_line0(1,0,Settings_text0);
LiquidLine Settings_line1(1,1,Settings_text1);
LiquidLine Settings_line2(1,2,Settings_text2);
LiquidLine Settings_line3(1,3,Settings_text3);
LiquidLine Settings_line4(1,4,Settings_text4);
LiquidLine Settings_line5(1,5,Settings_text5);
LiquidLine Settings_line6(1,6,Settings_text6);
LiquidScreen Settings_Screen(Settings_line0,Settings_line1,Settings_line2,Settings_line3,Settings_line4,Settings_line5,Settings_line6);



//number
const char SettingsNumber_text0[] PROGMEM = "NTC :      ";
const char SettingsNumber_text1[] PROGMEM = "TC :       ";
const char SettingsNumber_text2[] PROGMEM = "I :        ";
const char SettingsNumber_text3[] PROGMEM = "V (24V) :  ";
const char SettingsNumber_text4[] PROGMEM = "V (5V) :   ";
const char SettingsNumber_text5[] PROGMEM = "BACK";
LiquidLine SettingsNumbeR_LINE_NTC0(1,0,SettingsNumber_text0, GLYPH::leftArrowIndex, NTC_nb, GLYPH::rightArrowIndex);
LiquidLine SettingsNumbeR_LINE_NTC1(1,1,SettingsNumber_text1, GLYPH::leftArrowIndex, TC_nb, GLYPH::rightArrowIndex);
LiquidLine SettingsNumbeR_LINE_NTC2(1,2,SettingsNumber_text2, GLYPH::leftArrowIndex, I_nb, GLYPH::rightArrowIndex);
LiquidLine SettingsNumbeR_LINE_NTC3(1,3,SettingsNumber_text3, GLYPH::leftArrowIndex, V24_nb, GLYPH::rightArrowIndex);
LiquidLine SettingsNumbeR_LINE_NTC4(1,4,SettingsNumber_text4, GLYPH::leftArrowIndex, V5_nb, GLYPH::rightArrowIndex);
LiquidLine SettingsNumbeR_LINE_NTC5(1,5,SettingsNumber_text5);
LiquidScreen SettingsNumber_Screen(SettingsNumbeR_LINE_NTC0,SettingsNumbeR_LINE_NTC1,SettingsNumbeR_LINE_NTC2,SettingsNumbeR_LINE_NTC3,SettingsNumbeR_LINE_NTC4,SettingsNumbeR_LINE_NTC5);


///Time
//interval
const char SettingsInterval_text0[] PROGMEM = "Interval of capture ?";
const char SettingsInterval_text1[] PROGMEM = "seconds : ";
const char SettingsInterval_text2[] PROGMEM = "minutes : ";
const char SettingsInterval_text3[] PROGMEM = "BACK";
LiquidLine SettingsInterval_line0(1,0,SettingsInterval_text0);
LiquidLine SettingsInterval_line1(1,1,SettingsInterval_text1, GLYPH::leftArrowIndex, IntervalSeconds,GLYPH::rightArrowIndex);
LiquidLine SettingsInterval_line2(1,2,SettingsInterval_text2, GLYPH::leftArrowIndex, IntervalMinutes, GLYPH::rightArrowIndex);
LiquidLine SettingsInterval_line3(1,3,SettingsInterval_text3);
LiquidScreen SettingsInterval_Screen(SettingsInterval_line0,SettingsInterval_line1,SettingsInterval_line2,SettingsInterval_line3);

//Duration

const char Duration_text0[] PROGMEM = "Duration of test :";
const char Duration_text1[] PROGMEM = "hour";
const char Duration_text2[] PROGMEM = "min";
const char Duration_text3[] PROGMEM = "sec";
const char Duration_text4[] PROGMEM = "LAUNCH TEST";
LiquidLine Duration_line0(1,0, Duration_text0);
LiquidLine Duration_line1(1,1, Duration_text1, GLYPH::leftArrowIndex, DurationHour, GLYPH::rightArrowIndex);
LiquidLine Duration_line2(1,2, Duration_text2, GLYPH::leftArrowIndex, DurationMin, GLYPH::rightArrowIndex);
LiquidLine Duration_line3(1,3, Duration_text3, GLYPH::leftArrowIndex, DurationSec, GLYPH::rightArrowIndex);
LiquidLine Duration_line4(1,4, Duration_text4);
LiquidScreen Duration_Screen(Duration_line0,Duration_line1,Duration_line2,Duration_line3,Duration_line4);




//NTC
const char Settings_NTC_text0[] PROGMEM = "R at 25 degrees";
const char Settings_NTC_text1[] PROGMEM = "B Factor";
const char Settings_NTC_text2[] PROGMEM = "BACK";
LiquidLine Settings_NTC_line0(1,0,Settings_NTC_text0);
LiquidLine Settings_NTC_line1(1,1,Settings_NTC_text1);
LiquidLine Settings_NTC_line2(1,2,Settings_NTC_text2);
LiquidScreen Settings_NTC_Screen(Settings_NTC_line0,Settings_NTC_line1,Settings_NTC_line2);

//R_25
const char R_25_Selection_text0[] = "R@25=";
const char R_25_Selection_text1[] = " - 10K +";
const char R_25_Selection_text2[] = " - 1K  +";
const char R_25_Selection_text3[] = "Reset to default";
const char R_25_Selection_text4[] = "BACK";
LiquidLine R_25_Selection_line0(1,0,R_25_Selection_text0, R_25, " Ohm");
LiquidLine R_25_Selection_line1(1,1,GLYPH::leftArrowIndex, R_25_Selection_text1, GLYPH::rightArrowIndex);
LiquidLine R_25_Selection_line2(1,2,GLYPH::leftArrowIndex, R_25_Selection_text2, GLYPH::rightArrowIndex);
LiquidLine R_25_Selection_line3(1,3,R_25_Selection_text3);
LiquidLine R_25_Selection_line4(1,4,R_25_Selection_text4);
LiquidScreen R_25_Selection_Screen(R_25_Selection_line0,R_25_Selection_line1,R_25_Selection_line2,R_25_Selection_line3,R_25_Selection_line4);


//B_Value
const char B_value_selection_text0[] = "B = ";
const char B_value_selection_text1[] = " - 100 + ";
const char B_value_selection_text2[] = " - 10  + ";
const char B_value_selection_text3[] = " - 1   + ";
const char B_value_selection_text4[] = "Reset to default";
const char B_value_selection_text5[] = "BACK";
LiquidLine B_value_selection_line0(1,0,B_value_selection_text0, B_FACTOR, " K"); //Set decimal
LiquidLine B_value_selection_line1(1,1,GLYPH::leftArrowIndex, B_value_selection_text1 , GLYPH::rightArrowIndex);
LiquidLine B_value_selection_line2(1,2,GLYPH::leftArrowIndex, B_value_selection_text2 , GLYPH::rightArrowIndex);
LiquidLine B_value_selection_line3(1,3,GLYPH::leftArrowIndex, B_value_selection_text3 , GLYPH::rightArrowIndex);
LiquidLine B_value_selection_line4(1,4, B_value_selection_text4);
LiquidLine B_value_selection_line5(1,5,B_value_selection_text5);
LiquidScreen B_value_selection_Screen(B_value_selection_line0,B_value_selection_line1,B_value_selection_line2,B_value_selection_line3,B_value_selection_line4 ,B_value_selection_line5);

//Rshunt
const char RshuntSelection_text0[] = "Rshunt = ";
const char RshuntSelection_text1[] = "- 1000 +";
const char RshuntSelection_text2[] = "- 100  +";
const char RshuntSelection_text3[] = "Reset to default";
const char RshuntSelection_text4[] = "BACK";
LiquidLine RshuntSelection_line0(1,0,RshuntSelection_text0, R_SHUNT, " uOhm");
LiquidLine RshuntSelection_line1(1,1,GLYPH::leftArrowIndex, RshuntSelection_text1, GLYPH::rightArrowIndex);
LiquidLine RshuntSelection_line2(1,2,GLYPH::leftArrowIndex, RshuntSelection_text2, GLYPH::rightArrowIndex);
LiquidLine RshuntSelection_line3(1,3,RshuntSelection_text3);
LiquidLine RshuntSelection_line4(1,4,RshuntSelection_text4);
LiquidScreen RshuntSelection_Screen(RshuntSelection_line0,RshuntSelection_line1,RshuntSelection_line2,RshuntSelection_line3,RshuntSelection_line4);

//TC TYPE
const char TcTypeSelection_text0[] PROGMEM = "TC TYPE = ";
const char TcTypeSelection_text1[] PROGMEM = "BACK";
LiquidLine TcTypeSelection_line0(1,0, TcTypeSelection_text0, GLYPH::leftArrowIndex, getTCType, GLYPH::rightArrowIndex);
LiquidLine TcTypeSelection_line1(1,1, TcTypeSelection_text1);
LiquidScreen TcTypeSelection_Screen(TcTypeSelection_line0,TcTypeSelection_line1);


//Save to EEPROM
const char SaveToEEPROM_text0[] PROGMEM = "Save config";
const char SaveToEEPROM_text1[] PROGMEM = "to EEPROM ?";
const char SaveToEEPROM_text2[] PROGMEM = "YES";
const char SaveToEEPROM_text3[] PROGMEM = "BACK";
LiquidLine SaveToEEPROM_line0(1,0,SaveToEEPROM_text0);
LiquidLine SaveToEEPROM_line1(1,1,SaveToEEPROM_text1);
LiquidLine SaveToEEPROM_line2(1,2,SaveToEEPROM_text2);
LiquidLine SaveToEEPROM_line3(1,3,SaveToEEPROM_text3);
LiquidScreen SaveToEEPROM_Screen(SaveToEEPROM_line0,SaveToEEPROM_line1,SaveToEEPROM_line2,SaveToEEPROM_line3);


//Saved to EEPROM
const char SavedToEEPROM_text0[] PROGMEM = "Config saved !";
const char SavedToEEPROM_text1[] PROGMEM = "Press OK";
LiquidLine SavedToEEPROM_line0(1,0,SavedToEEPROM_text0);
LiquidLine SavedToEEPROM_line1(1,1,SavedToEEPROM_text1);
LiquidScreen SavedToEEPROM_Screen(SavedToEEPROM_line0,SavedToEEPROM_line1);


//////////////INA///////////////
uint32_t I1Value = 0;
uint32_t VBusValue = 0;
const char INA_text0[] PROGMEM = "Current readings";
const char INA_text1[] PROGMEM = "I2 : ";
const char INA_text2[] PROGMEM = "I2 Bus :";
const char INA_text3[] PROGMEM = "BACK";
LiquidLine INA_line0 (1,0, INA_text0);
LiquidLine INA_line1 (1,1, INA_text1, I1Value, "uA");
LiquidLine INA_line2 (1,2, INA_text2, VBusValue, "mV");
LiquidLine INA_line3 (1,3, INA_text3);
LiquidScreen INA_Screen (INA_line0,INA_line1,INA_line2,INA_line3);

////////////LAUNCH////////////
const char Launch_text0[] PROGMEM = "Is this setup OK ?";
const char Launch_text4[] PROGMEM = "OK";
const char Launch_text5[] PROGMEM = "BACK";
LiquidLine Launch_line0(1,0, Launch_text0);
LiquidLine Launch_line1(1,1,"TC=", TC_nb, " NTC=", NTC_nb);
LiquidLine Launch_line2(1,2,"V5=", V5_nb, " V24=", V24_nb);
LiquidLine Launch_line3(1,3,"Interval = ", IntervalMinutes, "min", IntervalSeconds);
LiquidLine Launch_line4(1,4, Launch_text4);
LiquidLine Launch_line5(1,5, Launch_text5);
LiquidScreen Launch_Screen(Launch_line0,Launch_line1,Launch_line2,Launch_line3,Launch_line4, Launch_line5);


//////////////MEASURING///////////////
const char Measuring_text0[] PROGMEM = "Number of measure :";
const char Measuring_text2[] PROGMEM = "";
const char Measuring_text3[] PROGMEM = "STOP test";
LiquidLine Measuring_line0(1,0, Measuring_text0);
LiquidLine Measuring_line1(1,1, Nb_Of_Measure);
LiquidLine Measuring_line2(1,2, Measuring_text2);
LiquidLine Measuring_line3(1,3, Measuring_text3);
LiquidScreen Measuring_Screen(Measuring_line0,Measuring_line1,Measuring_line2,Measuring_line3);


//End of measures
const char EndOfMeasures_text0[] PROGMEM = "Test finished !";
const char EndOfMeasures_text1[] PROGMEM = "Nb of measures :";
const char EndOfMeasures_text3[] PROGMEM = "OK ";
LiquidLine EndOfMeasures_line0(1,0, EndOfMeasures_text0);
LiquidLine EndOfMeasures_line1(1,1, EndOfMeasures_text1, Nb_Of_Measure);
LiquidLine EndOfMeasures_line2(1,2, "Time :", ElapsedHours, "h ", ElapsedMinutes);
LiquidLine EndOfMeasures_line3(1,3, EndOfMeasures_text3);
LiquidScreen EndOfMeasures_Screen(EndOfMeasures_line0,EndOfMeasures_line1,EndOfMeasures_line2,EndOfMeasures_line3);


void AddVariableToLine(void)
{
	Launch_line1.add_variable(" I="); // Show the setup
	Launch_line1.add_variable( I_nb);
	Launch_line3.add_variable("s");
	EndOfMeasures_line2.add_variable("min"); // Show the elapsed time
	EndOfMeasures_line2.add_variable(ElapsedSeconds);
	EndOfMeasures_line2.add_variable("s");
}

void setGlyph(void)
{
	SettingsNumbeR_LINE_NTC0.set_asGlyph(2); //left and right arrow
	SettingsNumbeR_LINE_NTC0.set_asGlyph(4);
	SettingsNumbeR_LINE_NTC1.set_asGlyph(2);
	SettingsNumbeR_LINE_NTC1.set_asGlyph(4);
	SettingsNumbeR_LINE_NTC2.set_asGlyph(2);
	SettingsNumbeR_LINE_NTC2.set_asGlyph(4);
	SettingsNumbeR_LINE_NTC3.set_asGlyph(2);
	SettingsNumbeR_LINE_NTC3.set_asGlyph(4);
	SettingsNumbeR_LINE_NTC4.set_asGlyph(2);
	SettingsNumbeR_LINE_NTC4.set_asGlyph(4);
	
	SettingsInterval_line1.set_asGlyph(2); //left and right arrow
	SettingsInterval_line1.set_asGlyph(4);
	SettingsInterval_line2.set_asGlyph(2);
	SettingsInterval_line2.set_asGlyph(4);
	
	Duration_line1.set_asGlyph(2);
	Duration_line1.set_asGlyph(4);
	Duration_line2.set_asGlyph(2);
	Duration_line2.set_asGlyph(4);
	Duration_line3.set_asGlyph(2);
	Duration_line3.set_asGlyph(4);
	
	R_25_Selection_line1.set_asGlyph(1);
	R_25_Selection_line1.set_asGlyph(3);
	R_25_Selection_line2.set_asGlyph(1);
	R_25_Selection_line2.set_asGlyph(3);
	
	B_value_selection_line1.set_asGlyph(1);//left and right arrow
	B_value_selection_line1.set_asGlyph(3);
	B_value_selection_line2.set_asGlyph(1);
	B_value_selection_line2.set_asGlyph(3);
	B_value_selection_line3.set_asGlyph(1);
	B_value_selection_line3.set_asGlyph(3);
	
	RshuntSelection_line1.set_asGlyph(1);//left and right arrow
	RshuntSelection_line1.set_asGlyph(3);
	RshuntSelection_line2.set_asGlyph(1);
	RshuntSelection_line2.set_asGlyph(3);
	
	TcTypeSelection_line0.set_asGlyph(2);
	TcTypeSelection_line0.set_asGlyph(4);
}

void putInProgmem(void)
{
	welcome_line0.set_asProgmem(1);
	welcome_line1.set_asProgmem(1);
	welcome_line2.set_asProgmem(1);
	welcome_line3.set_asProgmem(1);
	
	main_line0.set_asProgmem(1);
	main_line1.set_asProgmem(1);
	main_line2.set_asProgmem(1);
	main_line3.set_asProgmem(1);
	main_line4.set_asProgmem(1);
	main_line5.set_asProgmem(1);
	
	Settings_line0.set_asProgmem(1);
	Settings_line1.set_asProgmem(1);
	Settings_line2.set_asProgmem(1);
	Settings_line3.set_asProgmem(1);
	Settings_line4.set_asProgmem(1);
	Settings_line5.set_asProgmem(1);
	Settings_line6.set_asProgmem(1);
	
	SettingsNumbeR_LINE_NTC0.set_asProgmem(1);
	SettingsNumbeR_LINE_NTC1.set_asProgmem(1);
	SettingsNumbeR_LINE_NTC2.set_asProgmem(1);
	SettingsNumbeR_LINE_NTC3.set_asProgmem(1);
	SettingsNumbeR_LINE_NTC4.set_asProgmem(1);
	SettingsNumbeR_LINE_NTC5.set_asProgmem(1);
	
	SettingsInterval_line0.set_asProgmem(1);
	SettingsInterval_line1.set_asProgmem(1);
	SettingsInterval_line2.set_asProgmem(1);
	SettingsInterval_line3.set_asProgmem(1);
	
	Duration_line0.set_asProgmem(1);
	Duration_line1.set_asProgmem(1);
	Duration_line2.set_asProgmem(1);
	Duration_line3.set_asProgmem(1);
	Duration_line4.set_asProgmem(1);
	
	Settings_NTC_line0.set_asProgmem(1);
	Settings_NTC_line1.set_asProgmem(1);
	Settings_NTC_line2.set_asProgmem(1);
	
	TcTypeSelection_line0.set_asProgmem(1);
	TcTypeSelection_line1.set_asProgmem(1);
	
	SaveToEEPROM_line0.set_asProgmem(1);
	SaveToEEPROM_line1.set_asProgmem(1);
	SaveToEEPROM_line2.set_asProgmem(1);
	SaveToEEPROM_line3.set_asProgmem(1);
	
	SavedToEEPROM_line0.set_asProgmem(1);
	SavedToEEPROM_line1.set_asProgmem(1);
	
	INA_line0.set_asProgmem(1);
	INA_line1.set_asProgmem(1);
	INA_line2.set_asProgmem(1);
	INA_line3.set_asProgmem(1);
	
	Launch_line0.set_asProgmem(1);
	Launch_line4.set_asProgmem(1);
	Launch_line5.set_asProgmem(1);
	
	Measuring_line0.set_asProgmem(1);
	Measuring_line2.set_asProgmem(1);
	Measuring_line3.set_asProgmem(1);
	
	EndOfMeasures_line0.set_asProgmem(1);
	EndOfMeasures_line1.set_asProgmem(1);
	//EndOfMeasures_line2.set_asProgmem(1);
	EndOfMeasures_line3.set_asProgmem(1);
}

void enableScrolling(void)
{
	welcome_Screen.set_displayLineCount(LCD_ROWS);
	main_Screen.set_displayLineCount(LCD_ROWS);
	Settings_Screen.set_displayLineCount(LCD_ROWS);
	SettingsNumber_Screen.set_displayLineCount(LCD_ROWS);
	SettingsInterval_Screen.set_displayLineCount(LCD_ROWS);
	Duration_Screen.set_displayLineCount(LCD_ROWS);
	Settings_NTC_Screen.set_displayLineCount(LCD_ROWS);
	R_25_Selection_Screen.set_displayLineCount(LCD_ROWS);
	B_value_selection_Screen.set_displayLineCount(LCD_ROWS);
	RshuntSelection_Screen.set_displayLineCount(LCD_ROWS);
	TcTypeSelection_Screen.set_displayLineCount(LCD_ROWS);
	SaveToEEPROM_Screen.set_displayLineCount(LCD_ROWS);
	SavedToEEPROM_Screen.set_displayLineCount(LCD_ROWS);
	INA_Screen.set_displayLineCount(LCD_ROWS);
	Launch_Screen.set_displayLineCount(LCD_ROWS);
	Measuring_Screen.set_displayLineCount(LCD_ROWS);
	EndOfMeasures_Screen.set_displayLineCount(LCD_ROWS);
}

void attachFunctionToLine(void)
{
	welcome_line0.attach_function(1, gotoMainScreen);
	welcome_line1.attach_function(1, gotoMainScreen);
	welcome_line2.attach_function(1, gotoMainScreen);
	welcome_line3.attach_function(1, gotoMainScreen);
	
	main_line1.attach_function(1, gotoSettingsScreen); //Start the settings screen
	main_line2.attach_function(1, emptyfunction); //Live readings
	main_line3.attach_function(1, gotoLaunchScreen); //Launch test
	main_line4.attach_function(1, gotoINAScreen);
	main_line5.attach_function(1, gotoLaunchScreen);
	

	Settings_line0.attach_function(1, gotoMainScreen);
	Settings_line1.attach_function(1, gotoSettingsNumberScreen);
	Settings_line2.attach_function(1, gotoSettingsIntervalScreen);
	Settings_line3.attach_function(1, gotoNTCScreen);
	Settings_line4.attach_function(1, gotoRshuntSelectionScreen);
	Settings_line5.attach_function(1, gotoTcTypeSelectionScreen);
	Settings_line6.attach_function(1, gotoSaveToEEPROMScreen);
	
	SettingsNumbeR_LINE_NTC0.attach_function(6, decrementNTC_nb); //left we decrement, right we increment
	SettingsNumbeR_LINE_NTC0.attach_function(7, incrementNTC_nb);
	SettingsNumbeR_LINE_NTC1.attach_function(6, decrementTC_nb);
	SettingsNumbeR_LINE_NTC1.attach_function(7, incrementTC_nb);
	SettingsNumbeR_LINE_NTC2.attach_function(6, decrementI_nb);
	SettingsNumbeR_LINE_NTC2.attach_function(7, incrementI_nb);
	SettingsNumbeR_LINE_NTC3.attach_function(6, decrementV24_nb);
	SettingsNumbeR_LINE_NTC3.attach_function(7, incrementV24_nb);
	SettingsNumbeR_LINE_NTC4.attach_function(6, decrementV5_nb);
	SettingsNumbeR_LINE_NTC4.attach_function(7, incrementV5_nb);
	SettingsNumbeR_LINE_NTC5.attach_function(1, gotoSettingsScreen);
	
	Settings_NTC_line0.attach_function(1, gotoR25Screen );
	Settings_NTC_line1.attach_function(1, gotoBValueSelectionScreen);
	Settings_NTC_line2.attach_function(1, gotoSettingsScreen );
	
	//R_25_Selection_line0.attach_function(1, emptyfunction );
	R_25_Selection_line1.attach_function(6, decrement10KR25 );
	R_25_Selection_line1.attach_function(7, increment10KR25 );
	R_25_Selection_line2.attach_function(6, decrement1KR25 );
	R_25_Selection_line2.attach_function(7, increment1KR25 );
	R_25_Selection_line3.attach_function(1, resetR25ToDefault);
	R_25_Selection_line4.attach_function(1, gotoNTCScreen );
	
	//B_value_selection_line0.attach_function(1, emptyfunction);
	B_value_selection_line1.attach_function(6, decrement100BFactor);
	B_value_selection_line1.attach_function(7, increment100BFactor);
	B_value_selection_line2.attach_function(6, decrement10BFactor);
	B_value_selection_line2.attach_function(7, increment10BFactor);
	B_value_selection_line3.attach_function(6, decrement1BFactor);
	B_value_selection_line3.attach_function(7, increment1BFactor);
	B_value_selection_line4.attach_function(1, resetBFactorToDefault);
	B_value_selection_line5.attach_function(1, gotoNTCScreen);
	
	//RshuntSelection_line0.attach_function(1, emptyfunction );
	RshuntSelection_line1.attach_function(6, decrement1000Rshunt );
	RshuntSelection_line1.attach_function(7, increment1000Rshunt );
	RshuntSelection_line2.attach_function(6, decrement100Rshunt );
	RshuntSelection_line2.attach_function(7, increment100Rshunt );
	RshuntSelection_line3.attach_function(1, resetRshuntToDefault );
	RshuntSelection_line4.attach_function(1, gotoSettingsScreen);
	
	
	TcTypeSelection_line0.attach_function(1, emptyfunction );
	TcTypeSelection_line0.attach_function(6, decrementTcType );
	TcTypeSelection_line0.attach_function(7, incrementTcType );
	TcTypeSelection_line1.attach_function(1, gotoSettingsScreen );
	
	SettingsInterval_line1.attach_function(6, decrementTempsSec);
	SettingsInterval_line1.attach_function(7, incrementTempsSec);
	SettingsInterval_line2.attach_function(6, decrementTempsMin);
	SettingsInterval_line2.attach_function(7, incrementTempsMin);
	SettingsInterval_line3.attach_function(1, gotoSettingsScreen);




	SaveToEEPROM_line2.attach_function(1, gotoSavedToEEPROMScreen);
	SaveToEEPROM_line2.attach_function(2 ,saveToEEPROM);
	SaveToEEPROM_line3.attach_function(1, gotoSettingsScreen);
	
	SavedToEEPROM_line0.attach_function(1, gotoSettingsScreen);
	SavedToEEPROM_line1.attach_function(1, gotoSettingsScreen);
	
	
	INA_line0.attach_function(1, emptyfunction);
	INA_line1.attach_function(1, emptyfunction);
	INA_line2.attach_function(1, emptyfunction);
	INA_line3.attach_function(1, gotoMainScreen);//return to main menu
	
	//todo change first line
	Launch_line1.attach_function(1,emptyfunction);
	/*Launch_line2.attach_function(1,emptyfunction);
	Launch_line3.attach_function(1,emptyfunction);*/
	Launch_line4.attach_function(1, initSensors);
	Launch_line4.attach_function(2, gotoDurationScreen);
	Launch_line5.attach_function(1, gotoMainScreen);
	
	//Duration_line0.attach_function(1, emptyfunction );
	Duration_line1.attach_function(6, decrementDurationHour);
	Duration_line1.attach_function(7, incrementDurationHour);
	Duration_line2.attach_function(6, decrementDurationMinute);
	Duration_line2.attach_function(7, incrementDurationMinute);
	Duration_line3.attach_function(6, decrementDurationSec);
	Duration_line3.attach_function(7, incrementDurationSec);
	Duration_line4.attach_function(1, firstLineSD);
	Duration_line4.attach_function(2, gotoMeasuringScreen);//Launch the measures
	
	Measuring_line3.attach_function(1, endMeasures);
	
	EndOfMeasures_line3.attach_function(1,resetNumberOfMeasure );
	EndOfMeasures_line3.attach_function(2,gotoMainScreen );
}

void addScreens(void)
{
	menu.add_screen(welcome_Screen);
	menu.add_screen(main_Screen);
	menu.add_screen(Settings_Screen);
	menu.add_screen(SettingsNumber_Screen);
	menu.add_screen(SettingsInterval_Screen);
	menu.add_screen(Duration_Screen);
	menu.add_screen(Settings_NTC_Screen);
	menu.add_screen(R_25_Selection_Screen);
	menu.add_screen(B_value_selection_Screen);
	menu.add_screen(RshuntSelection_Screen);
	menu.add_screen(TcTypeSelection_Screen);
	menu.add_screen(SaveToEEPROM_Screen);
	menu.add_screen(SavedToEEPROM_Screen);
	menu.add_screen(INA_Screen);
	menu.add_screen(Launch_Screen);
	menu.add_screen(Measuring_Screen);
	menu.add_screen(EndOfMeasures_Screen);
}


void gotoMainScreen(void)
{
	menu.change_screen(main_Screen);
	menu.switch_focus(true);
}

void gotoSettingsScreen(void)
{
	menu.change_screen(Settings_Screen);
	menu.switch_focus(true);
}

void gotoSettingsNumberScreen(void)
{
	menu.change_screen(SettingsNumber_Screen);
	menu.switch_focus(true);
}

void gotoSettingsIntervalScreen(void)
{
	menu.change_screen(SettingsInterval_Screen);
	menu.switch_focus(true);
}

void gotoDurationScreen(void)
{
	menu.change_screen(Duration_Screen);
	menu.switch_focus(true);
}

void gotoNTCScreen(void)
{
	menu.change_screen(Settings_NTC_Screen);
	menu.switch_focus(true);
}

void gotoR25Screen(void)
{
	menu.change_screen(R_25_Selection_Screen);
	menu.switch_focus(true);
}

void gotoBValueSelectionScreen(void)
{
	menu.change_screen(B_value_selection_Screen);
	menu.switch_focus(true);
}

void gotoRshuntSelectionScreen(void)
{
	
	menu.change_screen(RshuntSelection_Screen);
	menu.switch_focus(true);
}

void gotoTcTypeSelectionScreen(void)
{
	menu.change_screen(TcTypeSelection_Screen);
	menu.switch_focus(true);
}

void gotoSaveToEEPROMScreen(void)
{
	menu.change_screen(SaveToEEPROM_Screen);
	menu.switch_focus(true);
}

void gotoSavedToEEPROMScreen(void)
{
	menu.change_screen(SavedToEEPROM_Screen);
	menu.switch_focus(true);
}

void gotoINAScreen(void)
{
	menu.change_screen(INA_Screen);
	menu.switch_focus(true);
}

void gotoLaunchScreen(void) //check if we can save things
{
	while(!IsSdInserted())
	{
		lcd.clear();
		lcd.home();
		lcd.print(F("ERROR SD CARD "));
		lcd.setCursor(0,1);
		lcd.print(F("NOT FOUND"));
		lcd.setCursor(0,2);
		lcd.print(F("Insert an SD card"));
		_delay_ms(500); //weird bug with delay here so using the avr one
	}
	menu.change_screen(Launch_Screen);
	menu.switch_focus(true);
}

void gotoMeasuringScreen(void)
{
	menu.change_screen(Measuring_Screen);
	menu.switch_focus(true);
}

void gotoEndOfMeasuresScreen(void)
{
	menu.change_screen(EndOfMeasures_Screen);
	menu.switch_focus(true);
}

static void incrementNTC_nb(void)
{
	if( NTC_nb != MAX_NTC_NB) //We can add an NTC because we are not yet full
	{
		NTC_nb++;
	}
}
static void decrementNTC_nb(void)
{
	if (NTC_nb != 0) //We are not at 0 and we want to remove one so we can
	{
		NTC_nb--;
	}
}

static void incrementTC_nb(void)
{
	if(TC_nb != MAX_TC_NB) //We can add an TC because we are not yet full
	{
		TC_nb++;
	}
}
static void decrementTC_nb(void)
{
	if (TC_nb != 0) //We are not at 0 and we want to remove one so we can
	{
		TC_nb--;
	}
}

static void incrementI_nb(void)
{
	if(I_nb != MAX_I_NB) //We can add an I because we are not yet full
	{
		I_nb++;
	}
}

static void decrementI_nb(void)
{
	if (I_nb != 0) //We are not at 0 and we want to remove one so we can
	{
		I_nb--;
	}
}

static void incrementV24_nb(void)
{
	if(V24_nb != MAX_V24_NB) //We can add an V because we are not yet full
	{
		V24_nb++;
	}
}

static void decrementV24_nb(void)
{
	if (V24_nb != 0) //We are not at 0 and we want to remove one so we can
	{
		V24_nb--;
	}
}

static void incrementV5_nb(void)
{
	if(V5_nb != MAX_V5_NB) //We can add an V because we are not yet full
	{
		V5_nb++;
	}
}

static void decrementV5_nb(void)
{
	if (V5_nb != 0) //We are not at 0 and we want to remove one so we can
	{
		V5_nb--;
	}
}

static void incrementTempsSec(void)
{
	IntervalSeconds++;
	if(IntervalSeconds == 60)
	{
		IntervalSeconds = 0;
		incrementTempsMin();
	}
}

static void decrementTempsSec(void)
{
	if((IntervalMinutes > 0) || (IntervalSeconds > 5))
	{
		IntervalSeconds--;
	}
}

static void incrementTempsMin(void)
{
	if (IntervalMinutes <= 29) //MAX 30minutes
	{
		IntervalMinutes++;
	}
}

static void decrementTempsMin(void)
{
	if (IntervalMinutes != 0)
	{
		IntervalMinutes--;
	}
	if(IntervalMinutes == 0) // we are at 0minutes interval but we have to make sure we are not under 5s which is the minimum
	{
		if(IntervalSeconds < 5)
		{
			IntervalSeconds = 5;
		}
	}
}

static void incrementDurationHour(void)
{
	DurationHour++;
}

static void decrementDurationHour(void)
{
	if(DurationHour>0)
	{
		DurationHour--;
	}
}
static void incrementDurationMinute(void)
{
	DurationMin++;
	if(DurationMin == 60)
	{
		DurationMin = 0; //60min => 1h
		incrementDurationHour();
	}
}
static void decrementDurationMinute(void)
{
	if(DurationMin > 1) //Minimum 1min of test
	{
		DurationMin--;
	}
}

static void incrementDurationSec(void)
{
	DurationSec++;
	if(DurationSec == 60)
	{
		DurationSec = 0;
		incrementDurationMinute();
	}
}

static void decrementDurationSec(void)
{
	if(DurationSec > 0)
	{
		DurationSec--;
	}
}






static void resetBFactorToDefault(void)
{
	B_FACTOR = 3977.0;
}

static void decrement100BFactor(void)
{
	if (B_FACTOR > 100)
	{
		B_FACTOR -=100;
	}
}

static void increment100BFactor(void)
{
	B_FACTOR+=100;
}

static void decrement10BFactor(void)
{
	if (B_FACTOR > 10)
	{
		B_FACTOR -=10;
	}
}

static void increment10BFactor(void)
{
	B_FACTOR+=10;
}


static void decrement1BFactor(void)
{
	if (B_FACTOR > 0)
	{
		B_FACTOR -= 1;
	}
}

static void increment1BFactor(void)
{
	B_FACTOR += 1;
}

static void decrement10KR25(void)
{
	if(R_25 > 9999.0)
	{
		R_25-=10000.0;
	}
}

static void increment10KR25(void)
{
	R_25 += 10000.0;
}

static void decrement1KR25(void)
{
	if(R_25 > 999.0)
	{
		R_25-=1000.0;
	}
}

static void increment1KR25(void)
{
	R_25 += 1000.0;
}

static void resetR25ToDefault(void)
{
	R_25 = 10000.0;
}


static void decrement1000Rshunt(void)
{
	if(R_SHUNT >999)
	{
		R_SHUNT-=1000;
	}
}

static void increment1000Rshunt(void)
{
	R_SHUNT+=1000;
}

static void decrement100Rshunt(void)
{
	if(R_SHUNT >99)
	{
		R_SHUNT-=100;
	}
}

static void increment100Rshunt(void)
{
	R_SHUNT+=100;
}

static void resetRshuntToDefault(void)
{
	R_SHUNT = 2000;
}

static void incrementTcType(void)
{
	if(TC_Type_Counter <=7) //if we are not at the end of the array we can go forward
	{
		TC_Type_Counter++;
	}
	else{TC_Type_Counter = 0;} //return at the beginning
	TC_Type[0] = TC_Type_Array[TC_Type_Counter];
}
static void decrementTcType(void)
{
	if(TC_Type_Counter > 0) //if we are not a the beginning we can go back
	{
		TC_Type_Counter--;
	}
	else{TC_Type_Counter = 7;}
	TC_Type[0] = TC_Type_Array[TC_Type_Counter];
}

const char* getTCType(void) //Todo check if it works
{
	const char* t= TC_Type; //Create a new char on the stack with the value pointed by TC_Type
	return t;
}

void endMeasures(void)
{
	getElapsedTime();
	gotoEndOfMeasuresScreen();
}

static void getElapsedTime(void)
{
	DateTime dt = RTC.now();
	TimeSpan ts = dt - Global_Begin_Datetime;
	ElapsedHours = ts.hours();
	ElapsedMinutes = ts.minutes();
	ElapsedSeconds = ts.seconds();
}

static void resetNumberOfMeasure(void)
{
	Nb_Of_Measure = 0;
}



