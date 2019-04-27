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
extern LiquidLine main_line4;
extern LiquidLine main_line5;
extern LiquidScreen main_Screen;

/////////////Settings Screen////////////////////


//Main Settings Screen
extern LiquidLine Settings_line0;
extern LiquidLine Settings_line1;
extern LiquidLine Settings_line2;
extern LiquidLine Settings_line3;
extern LiquidLine Settings_line4;
extern LiquidLine Settings_line5;
extern LiquidScreen Settings_Screen;


//Number
extern LiquidLine SettingsNumber_line0;
extern LiquidLine SettingsNumber_line1;
extern LiquidLine SettingsNumber_line2;
extern LiquidLine SettingsNumber_line3;
extern LiquidLine SettingsNumber_line4;
extern LiquidLine SettingsNumber_line5;
extern LiquidScreen SettingsNumber_Screen;


//Interval
extern LiquidLine SettingsInterval_line0;
extern LiquidLine SettingsInterval_line1;
extern LiquidLine SettingsInterval_line2;
extern LiquidLine SettingsInterval_line3;
extern LiquidScreen SettingsInterval_Screen;


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

//////////////INA///////////////
extern uint32_t I1Value;
extern uint32_t VBusValue;
extern LiquidLine INA_line0;
extern LiquidLine INA_line1;
extern LiquidLine INA_line2;
extern LiquidLine INA_line3;
extern LiquidScreen INA_Screen;


//////////////LAUNCH///////////////
extern LiquidLine Launch_line0;
extern LiquidLine Launch_line1;
extern LiquidLine Launch_line2;
extern LiquidLine Launch_line3;
extern LiquidLine Launch_line4;
extern LiquidLine Launch_line5;
extern LiquidScreen Launch_Screen;



//////////////Menu//////////////
extern LiquidMenu menu;


///////Goto for Menu navigation
void gotoMainScreen(void);
void gotoSettingsScreen(void);
void gotoSettingsNumberScreen(void);
void gotoSettingsIntervalScreen(void);
void gotoNTCScreen(void);
void gotoR25Screen(void);
void gotoBValueSelectionScreen(void);
void gotoRshuntSelectionScreen(void);
void gotoSaveToEEPROMScreen(void);
void gotoSavedToEEPROMScreen(void);
void gotoINAScreen(void);
void gotoLaunchScreen(void);
/////Utility functions
void AddVariableToLine(void);
void putInProgmem(void);
void enableScrolling(void);
void attachFunctionToLine(void);
void addScreens(void);
void setGlyph(void);

//////////Increment and decrement Functions/////////////
//////Nb of measures
void incrementNTC_nb(void);
void decrementNTC_nb(void);
void incrementTC_nb(void);
void decrementTC_nb(void);
void incrementI_nb(void);
void decrementI_nb(void);
void incrementV24_nb(void);
void decrementV24_nb(void);
void incrementV5_nb(void);
void decrementV5_nb(void);

////Interval
void incrementTempsSec(void);
void decrementTempsSec(void);
void incrementTempsMin(void);
void decrementTempsMin(void);


//R_25
void decrement10KR25(void);
void increment10KR25(void);
void decrement1KR25(void);
void increment1KR25(void);
void resetR25ToDefault(void);

//B_FACTOR
void decrement100BFactor(void);
void increment100BFactor(void);
void decrement10BFactor(void);
void increment10BFactor(void);
void decrement1BFactor(void);
void increment1BFactor(void);

//R_SHUNT
void decrement1000Rshunt(void);
void increment1000Rshunt(void);
void decrement100Rshunt(void);
void increment100Rshunt(void);
void resetRshuntToDefault(void);


#endif /* MENU_H_ */