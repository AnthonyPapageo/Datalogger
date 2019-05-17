/*
* menu.h
*
* Created: 22/03/2019 13:14:39
*  Author: Anthony
*/


#ifndef MENU_H_
#define MENU_H_


/////////////STRINGS//////////////////////
/////////////Welcome Screen////////////////////
extern LiquidLine welcome_line0;
extern LiquidLine welcome_line1;
extern LiquidLine welcome_line2;
extern LiquidLine welcome_line3;
extern LiquidScreen welcome_Screen;

/////////////Main Screen////////////////////
extern LiquidLine main_line0;
extern LiquidLine main_line1;
extern LiquidLine main_line2;
extern LiquidLine main_line3;
extern LiquidScreen main_Screen;

/////////////Settings Screen////////////////////


//Main Settings Screen
extern LiquidLine Settings_line0;
extern LiquidLine Settings_line1;
extern LiquidLine Settings_line2;
extern LiquidLine Settings_line3;
extern LiquidLine Settings_line4;
extern LiquidLine Settings_line5;
extern LiquidLine Settings_line6;
extern LiquidScreen Settings_Screen;


//Number
extern LiquidLine SettingsNumbeR_LINE_NTC0;
extern LiquidLine SettingsNumbeR_LINE_NTC1;
extern LiquidLine SettingsNumbeR_LINE_NTC2;
extern LiquidLine SettingsNumbeR_LINE_NTC3;
extern LiquidLine SettingsNumbeR_LINE_NTC4;
extern LiquidLine SettingsNumbeR_LINE_NTC5;
extern LiquidScreen SettingsNumber_Screen;

//////Time
//Interval
extern LiquidLine SettingsInterval_line0;
extern LiquidLine SettingsInterval_line1;
extern LiquidLine SettingsInterval_line2;
extern LiquidLine SettingsInterval_line3;
extern LiquidScreen SettingsInterval_Screen;

//Duration
extern LiquidLine Duration_line0;
extern LiquidLine Duration_line1;
extern LiquidLine Duration_line2;
extern LiquidLine Duration_line3;
extern LiquidLine Duration_line4;
extern LiquidScreen Duration_Screen;


//NTC

extern LiquidLine Settings_NTC_line0;
extern LiquidLine Settings_NTC_line1;
extern LiquidLine Settings_NTC_line2;
extern LiquidScreen Settings_NTC_Screen;


///R_25
extern LiquidLine R_25_Selection_line0;
extern LiquidLine R_25_Selection_line1;
extern LiquidLine R_25_Selection_line2;
extern LiquidLine R_25_Selection_line3;
extern LiquidLine R_25_Selection_line4;
extern LiquidScreen R_25_Selection_Screen;


///B_FACTOR
extern LiquidLine B_value_selection_line0;
extern LiquidLine B_value_selection_line1;
extern LiquidLine B_value_selection_line2;
extern LiquidLine B_value_selection_line3;
extern LiquidScreen B_value_selection_Screen;

//Rshunt
extern LiquidLine RshuntSelection_line0;
extern LiquidLine RshuntSelection_line1;
extern LiquidLine RshuntSelection_line2;
extern LiquidLine RshuntSelection_line3;
extern LiquidLine RshuntSelection_line4;
extern LiquidScreen RshuntSelection_Screen;

//TC TYPE
extern LiquidLine TcTypeSelection_line0;
extern LiquidLine TcTypeSelection_line1;
extern LiquidScreen TcTypeSelection_Screen;



//Save to EEPROM
extern LiquidLine SaveToEEPROM_line0;
extern LiquidLine SaveToEEPROM_line1;
extern LiquidLine SaveToEEPROM_line2;
extern LiquidLine SaveToEEPROM_line3;
extern LiquidScreen SaveToEEPROM_Screen;

//Saved to EEPROM
extern LiquidLine SavedToEEPROM_line0;
extern LiquidLine SavedToEEPROM_line1;
extern LiquidScreen SavedToEEPROM_Screen;

//Setup RTC
extern LiquidLine Setup_RTC_line0;
extern LiquidLine Setup_RTC_line1;
extern LiquidLine Setup_RTC_line2;
extern LiquidLine Setup_RTC_line3;
extern LiquidLine Setup_RTC_line4;
extern LiquidLine Setup_RTC_line5;
extern LiquidLine Setup_RTC_line6;
extern LiquidScreen Setup_RTC_Screen;


//////////////LAUNCH///////////////
extern LiquidLine Launch_line0;
extern LiquidLine Launch_line1;
extern LiquidLine Launch_line2;
extern LiquidLine Launch_line3;
extern LiquidLine Launch_line4;
extern LiquidLine Launch_line5;
extern LiquidScreen Launch_Screen;

//////////////MEASURING///////////////
extern LiquidLine Measuring_line0;
extern LiquidLine Measuring_line1;
extern LiquidLine Measuring_line2;
extern LiquidLine Measuring_line3;
extern LiquidScreen Measuring_Screen;

//End of measures
extern LiquidLine EndOfMeasures_line0;
extern LiquidLine EndOfMeasures_line1;
extern LiquidLine EndOfMeasures_line2;
extern LiquidLine EndOfMeasures_line3;
extern LiquidScreen EndOfMeasures_Screen;



//////////////Menu//////////////
extern LiquidMenu menu;


///////Goto for Menu navigation
void gotoMainScreen(void);
void gotoSettingsScreen(void);
void gotoSettingsNumberScreen(void);
void gotoSettingsIntervalScreen(void);
void gotoDurationScreen(void);
void gotoNTCScreen(void);
void gotoR25Screen(void);
void gotoBValueSelectionScreen(void);
void gotoRshuntSelectionScreen(void);
void gotoTcTypeSelectionScreen(void);
void gotoSaveToEEPROMScreen(void);
void gotoSavedToEEPROMScreen(void);
void gotoSetupRTCScreen(void);
void gotoINAScreen(void);
void gotoLaunchScreen(void);
void gotoMeasuringScreen(void);
void gotoEndOfMeasuresScreen(void);
/////Utility functions
void AddVariableToLine(void);
void putInProgmem(void);
void enableScrolling(void);
void attachFunctionToLine(void);
void addScreens(void);
void setGlyph(void);



void endMeasures(void);



#endif /* MENU_H_ */