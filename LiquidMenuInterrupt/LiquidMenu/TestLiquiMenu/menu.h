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

/////////////Calibrate ADC Screen////////////////////
//CalibrateGain
extern LiquidLine calibrategain_line0;
extern LiquidLine calibrategain_line1;
extern LiquidScreen calibrateGain_Screen;

//CalibrateOffset
extern LiquidLine calibrateoffset_line0;
extern LiquidLine calibrateoffset_line1;
extern LiquidScreen calibrateOffset_Screen;

//Calibrate done
extern LiquidLine calibrationDone_line0;
extern LiquidLine calibrationDone_line1;
extern LiquidScreen calibrationDone_Screen;



/////////////ADC CONFIGURATION////////////////////
extern LiquidLine configureAdc_line0;
extern LiquidLine configureAdc_line1;
extern LiquidScreen configureAdc_Screen;

/////////////Live readings Screen////////////////////
extern uint32_t V1Value;
extern uint32_t V2Value;
extern LiquidLine liveRead_line0;
extern LiquidLine liveRead_line1;
extern LiquidLine liveRead_line2;
extern LiquidScreen liveRead_Screen;


///////////////////END ADC//////////////////////
//////////////INA///////////////
extern uint32_t I1Value;
extern uint32_t VBusValue;
extern LiquidLine INA_line0;
extern LiquidLine INA_line1;
extern LiquidLine INA_line2;
extern LiquidLine INA_line3;
extern LiquidScreen INA_Screen;


//////////////Menu//////////////
extern LiquidMenu menu;


///////Goto for Menu navigation
void gotoMainScreen(void);
void gotoCalibrationOffsetScreen(void);
void gotoCalibrationGainScreen(void);
void gotoConfigureAdcScreen(void);
void gotoLiveReadingsScreen(void);
void gotoCalibrationDoneScreen(void);

/////Utility functions
void putInProgmem(void);
void enableScrolling(void);
void attachFunctionToLine(void);


#endif /* MENU_H_ */