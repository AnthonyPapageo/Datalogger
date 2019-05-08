/*
* menu.cpp
*
* Created: 22/03/2019 13:14:24
*  Author: Anthony
*/

#include <LiquidMenu.h>
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/constant.h"
#include "C:/Users/Anthony/Documents/BA3 Electronique/Stages/Datalogger/Code/Library/Custom/utility.h"
#include "menu.h"



/////////////STRINGS//////////////////////
/////////////Welcome Screen////////////////////
const char welcome_text0[] PROGMEM = "    TEST PROGRAM";
const char welcome_text1[] PROGMEM = "ADC CALIBRATION";
const char welcome_text2[] PROGMEM = "Build on :";
const char welcome_text3[] PROGMEM =  __TIME__;
LiquidLine welcome_line0(0,0,welcome_text0);
LiquidLine welcome_line1(0,1,welcome_text1);
LiquidLine welcome_line2(0,2,welcome_text2);
LiquidLine welcome_line3(0,3,welcome_text3);
LiquidScreen welcome_Screen(welcome_line0, welcome_line1, welcome_line2, welcome_line3);

/////////////Main Screen////////////////////
const char main_text0[] PROGMEM = "   MAIN MENU   ";
const char main_text1[] PROGMEM = "1: CALIB ADC";
const char main_text2[] PROGMEM = "2: CONFI ADC";
const char main_text3[] PROGMEM = "3: READINGS";
LiquidLine main_line0(0,0,main_text0);
LiquidLine main_line1(0,1,main_text1);
LiquidLine main_line2(0,2,main_text2);
LiquidLine main_line3(0,3, main_text3);
LiquidScreen main_Screen(main_line0,main_line1,main_line2,main_line3);

/////////////Calibrate ADC Screen////////////////////
//CalibrateGain

const char calibrateGainText0[] PROGMEM = "Set 24V on V1";
const char calibrateGainText1[] PROGMEM = "Click here when ready";
LiquidLine calibrategain_line0(0,0, calibrateGainText0);
LiquidLine calibrategain_line1(0,1, calibrateGainText1);
LiquidScreen calibrateGain_Screen(calibrategain_line0,calibrategain_line1);

//CalibrateOffset

const char calibrateOffsetText0[] PROGMEM = "Set 0V on V1";
const char calibrateOffsetText1[] PROGMEM = "Click here when ready";
LiquidLine calibrateoffset_line0(0,0,calibrateOffsetText0);
LiquidLine calibrateoffset_line1(0,1,calibrateOffsetText1);
LiquidScreen calibrateOffset_Screen(calibrategain_line0,calibrategain_line1);

//CalibrateDone
const char calibrationDoneText0[] PROGMEM = "Calibration Done";
const char calibrationDoneText1[] PROGMEM = "Exit";
LiquidLine calibrationDone_line0(0,0, calibrationDoneText0);
LiquidLine calibrationDone_line1(0,1, calibrationDoneText1);
LiquidScreen calibrationDone_Screen(calibrategain_line0, calibrategain_line1);




/////////////ADC CONFIGURATION////////////////////
const char configureAdcText0[] PROGMEM = "Nothing yet sorry :(";
const char configureAdcText1[] PROGMEM = "Exit";
LiquidLine configureAdc_line0(0,0,configureAdcText0);
LiquidLine configureAdc_line1(0,1,configureAdcText1);
LiquidScreen configureAdc_Screen(configureAdc_line0, configureAdc_line1);

/////////////Live readings Screen////////////////////
uint32_t V1Value = 0;
uint32_t V2Value = 0;
const char liveReadText0[] PROGMEM = "V1 = ";
const char liveReadText1[] PROGMEM = "V2 = ";
const char liveReadText2[] PROGMEM = "EXIT";
//const char liveReadText3[] PROGMEM;
LiquidLine liveRead_line0(0,0,liveReadText0, V1Value, "mV");
LiquidLine liveRead_line1(0,1,liveReadText1, V2Value, "mV");
LiquidLine liveRead_line2(0,2, liveReadText2);
LiquidScreen liveRead_Screen(liveRead_line0,liveRead_line1,liveRead_line2);

//////////////INA///////////////
uint32_t I1Value = 0;
uint32_t VBusValue = 0;
const char INA_text0[] PROGMEM = "Current readings";
const char INA_text1[] PROGMEM = "Current on I2 : ";
const char INA_text2[] PROGMEM = "I2 Bus voltage :";
const char INA_text3[] PROGMEM = "EXIT";
LiquidLine INA_line0 (0,0, INA_text0);
LiquidLine INA_line1 (0,1, INA_text1, I1Value, "uA");
LiquidLine INA_line2 (0,2, INA_text2, VBusValue, "mV");
LiquidLine INA_line3 (0,3, INA_text3);
LiquidScreen INA_Screen (INA_line0,INA_line1,INA_line2,INA_line3);



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
	calibrategain_line0.set_asProgmem(1);
	calibrategain_line1.set_asProgmem(1);
	calibrateoffset_line0.set_asProgmem(1);
	calibrateoffset_line1.set_asProgmem(1);
	calibrationDone_line0.set_asProgmem(1);
	calibrationDone_line1.set_asProgmem(1);
	configureAdc_line0.set_asProgmem(1);
	configureAdc_line1.set_asProgmem(1);
	liveRead_line0.set_asProgmem(1);
	liveRead_line1.set_asProgmem(1);
	liveRead_line2.set_asProgmem(1);
	INA_line0.set_asProgmem(1);
	INA_line1.set_asProgmem(1);
	INA_line2.set_asProgmem(1);
	INA_line3.set_asProgmem(1);
	
}
void enableScrolling(void)
{
	///////Enable scrolling/////
	welcome_Screen.set_displayLineCount(LCD_ROWS);
	main_Screen.set_displayLineCount(LCD_ROWS);
	calibrateGain_Screen.set_displayLineCount(LCD_ROWS);
	calibrateOffset_Screen.set_displayLineCount(LCD_ROWS);
	calibrationDone_Screen.set_displayLineCount(LCD_ROWS);
	liveRead_Screen.set_displayLineCount(LCD_ROWS);
	INA_Screen.set_displayLineCount(LCD_ROWS);
}

void attachFunctionToLine(void)
{
	welcome_line0.attach_function(1,gotoMainScreen);
	welcome_line1.attach_function(1,gotoMainScreen);
	welcome_line2.attach_function(1,gotoMainScreen);
	welcome_line3.attach_function(1,gotoMainScreen);
	
	main_line0.attach_function(1,emptyfunction);
	main_line1.attach_function(1,gotoCalibrationOffsetScreen); //go to calibration menu
	main_line2.attach_function(1,gotoConfigureAdcScreen);
	main_line3.attach_function(1,gotoLiveReadingsScreen);
	
	calibrateoffset_line0.attach_function(1,emptyfunction);
	calibrateoffset_line1.attach_function(1,gotoCalibrationGainScreen); //After calibrating offset, we go to gain screen
	calibrateoffset_line1.attach_function(2,calibrateOffsetADC);
	
	calibrategain_line0.attach_function(1,emptyfunction);
	calibrategain_line1.attach_function(1, gotoCalibrationDoneScreen); //Show the calibration done screen
	calibrategain_line1.attach_function(2, calibrateGainADC); //Does the calibration
	
	calibrationDone_line0.attach_function(1,emptyfunction);
	calibrationDone_line1.attach_function(1,gotoMainScreen); //return to main menu
	
	configureAdc_line0.attach_function(1,emptyfunction);
	configureAdc_line1.attach_function(1,gotoMainScreen);
	
	liveRead_line0.attach_function(1,emptyfunction);
	liveRead_line1.attach_function(1,emptyfunction);
	liveRead_line2.attach_function(1, gotoMainScreen); //Exit live readings, go back to main menu
	
	INA_line0.attach_function(1,emptyfunction);
	INA_line1.attach_function(1,emptyfunction);
	INA_line2.attach_function(1,emptyfunction);
	INA_line3.attach_function(1, gotoMainScreen);//return to main menu
	
}



void gotoMainScreen(void)
{
	menu.change_screen(main_Screen);
}

void gotoCalibrationOffsetScreen(void)
{
	menu.change_screen(calibrateOffset_Screen);
}
void gotoCalibrationGainScreen(void)
{
	menu.change_screen(calibrateGain_Screen);
}

void gotoConfigureAdcScreen(void)
{
	menu.change_screen(configureAdc_Screen);
}

void gotoLiveReadingsScreen(void)
{
	menu.change_screen(liveRead_Screen);
}

void gotoCalibrationDoneScreen(void)
{
	menu.change_screen(calibrationDone_Screen);
}
