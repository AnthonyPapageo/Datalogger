/*
  ADS1241.h - Test library for ADS1241 - implementation
  Copyright (c) 2019 Anthony Papageorgiou.  All right reserved.
*/


// include this library's description file
#include "ADS1241.h"

// include description files for other libraries used (if any)
#include <SPI.h> 
#include <math.h>


//////AVR////////
#include <avr/io.h>
#include <avr/interrupt.h>

/////Personal/////
#include "utility.h"
#include "global.h"



//////////////ADC_COMMANDS//////////////
const uint8_t RDATA_ADC = 0x01;
const uint8_t RDATAC_ADC = 0x03;
const uint8_t STOPC_ADC = 0x0F;
const uint8_t RREG_ADC = 0x1;
const uint8_t WREG_ADC = 0x5;
const uint8_t SELFCA_ADCL = 0xF0;
const uint8_t SELFOCAL_ADC = 0xF1;
const uint8_t SELFGCAL_ADC = 0xF2;
const uint8_t SYSOCAL_ADC = 0xF3;
const uint8_t SYSGCAL_ADC = 0xF4;
const uint8_t WAKEUP_ADC = 0xFB;
const uint8_t DSYNC_ADC = 0xFC;
const uint8_t SLEEP_ADC = 0xFD;
const uint8_t RESET_ADC = 0xFE;

//////////////ADC_REGISTER//////////////
const uint8_t SETUP_ADC = 0x00;
const uint8_t MUX_ADC = 0x01;
const uint8_t ACR_ADC = 0x02;
const uint8_t ODAC_ADC = 0x03;
const uint8_t DIO_ADC = 0x04;
const uint8_t DIR_ADC = 0x05;
const uint8_t IOCON_ADC = 0x06;
const uint8_t OCR0_ADC = 0x07;
const uint8_t OCR1_ADC = 0x08;
const uint8_t OCR2_ADC = 0x09;
const uint8_t FSR0_ADC = 0x0A;
const uint8_t FSR1_ADC = 0x0B;
const uint8_t FSR2_ADC = 0x0C;
const uint8_t DOR2_ADC = 0x0D;
const uint8_t DOR1_ADC = 0x0E;
const uint8_t DOR0_ADC = 0x0F;








// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

ADS1241::ADS1241(){} //empty constructor



ADS1241::~ADS1241() //unused destructor
{
}

void ADS1241::begin(bool isBufEnable, bool isBOCSenable)
{
	//change PIN FOR FINAL PRODUCT
	//Polarity is hard coded 0
	//RANGE = 1 because we use 5V as VREF
	SPI.begin();
	//Those 2 lines are very important they prevent the ATMEGA to enter in slave mode
	DDRB |= (1 << 0); //SS as output
	PORTB |= (1 << 0); //Write a 1
	
	_isBufEnable = isBufEnable;
	_isBOCSenable = isBOCSenable;
	SPIPol(false);
	setupBUF(_isBufEnable);
	setupBOCS(_isBOCSenable);
	setDataFormat(true);
	range(true);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in ADS1241 sketches, this library, and other libraries

void ADS1241::reset(void)
{
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	setMux(CS_ADC);
	SPI.transfer(RESET_ADC);//Does a reset
	SPI.endTransaction();
	setMux(CS_DISABLE);
}


void ADS1241::write(uint8_t uiReg, uint8_t uiValue)
{
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	setMux(CS_ADC);
	SPI.transfer(transform(WREG_ADC, uiReg));
	SPI.transfer(0x00); //We write in only 1 register (#reg - 1 see datasheet)
	SPI.transfer(uiValue);
	setMux(CS_DISABLE);
	SPI.endTransaction();
	
}

uint8_t ADS1241::read(uint8_t uiReg)
{
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	uint8_t data;
	cli(); //Disable global interrupt
	setMux(CS_ADC);
	SPI.transfer(transform(RREG_ADC, uiReg));
	SPI.transfer(0x00); //We read 1 register
	delayMicroseconds(80);
	data = SPI.transfer(0x00); //read the register
	sei(); // Enable global interrupt
	setMux(CS_DISABLE);
	SPI.endTransaction();
	return data;
}

uint32_t ADS1241::read24bits(uint8_t uiReg)//TODO :check endianess
{
	//Give as argument the first register to be read
	//then the function read the next 2 registers
	uint8_t i;
	union 
	{
		uint32_t data;
		uint8_t uiValue[4];
	}MyUnion;
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
	cli();
	setMux(CS_ADC);
	SPI.transfer(transform(RREG_ADC, uiReg));
	SPI.transfer(0x02); //we read 3 registers
	delayMicroseconds(80); //Maybe not needed, check with that line commented
	
	for(i=0; i<=2; i++) 
	{
		MyUnion.uiValue[i] = SPI.transfer(0x00); 
	}
	sei();
	setMux(CS_DISABLE);
	SPI.endTransaction();
	return MyUnion.data;
	
}

void ADS1241::selectChannel(uint8_t channel)
{
	//Select AINCOM as negative channel and 
	//select  the arg as positive channel
	write(MUX_ADC, transform(channel, 0x8));
}

uint32_t ADS1241::getVoltage(uint8_t channel)//TODO : Use the tension used for calibration
{
	
	cli(); //Disable global interrupt
	uint8_t temp;
	uint32_t data;
	uint32_t FSR; //Full Scale Register
	uint8_t i=0;
	
	bool isDataReady = false;
	selectChannel(channel);
	while(!isDataReady || (i<100) ) //If we tried 100 times
	{
		temp = read(ACR_ADC);
		isDataReady = (temp & (1<<7)); //Check DRDY bit
		i++;		
	}
	if(isDataReady)
	{
		data = read24bits(RDATA_ADC);
	}
	else
	{
		data = 0xFFFFFFFF;
	}
	data = read24bits(RDATA_ADC);
	FSR = read24bits(FSR0_ADC);
	data = (static_cast <float>(data) / static_cast <float>(FSR))*24000.0; //If we used 24V for calibration, 
	sei(); // Enable global interrupt
	return data;
}



void ADS1241::calibrateGain(void)
{
	cli();
	range(0); //Range must be 0 for calibration
	setupBUF(0); //Disable Buffer
	selectChannel(0);
	SPI.transfer(SELFGCAL_ADC);
	delay(10); //wait 10ms for calibration
	getVoltage(1); //Discard the data in the buffer
	setupBUF(_isBufEnable); //If buffer was enabled, we re-enable it
	range(1);
	sei();
}

void ADS1241::calibrateOffset(void) //Calibration on channel 1 TODO SELECT CHANNEL ALREADY ON GND
{
	cli();
	range(0); //Range must be 0 for calibration
	setupBUF(0); //Disable Buffer
	selectChannel(0);
	SPI.transfer(SELFOCAL_ADC);
	delay(10);
	getVoltage(1); //Discard the data in the buffer
	setupBUF(_isBufEnable); //If buffer was enabled, we re-enable it
	range(1);
	sei();
}

void ADS1241::setupBOCS(bool boolBOCS) //activate or disable burnout current source
{
	uint8_t temp;
	temp = read(SETUP_ADC);
	if(boolBOCS)
	{
		temp |= (1<<3);  //set BOCS bit
	}
	else
	{
		temp &= ~(1<<3); //clear BOCS bit
	}
	write(SETUP_ADC, temp);
}

void ADS1241::setupBUF(bool boolBUF)
{
	uint8_t temp;
	DDRB |= (1<<4); //BUF pin as output
	temp = read(ACR_ADC);
	if(boolBUF)
	{
		PORTB |= (1<<4);//Write 1 to BUF pin
		temp |= (1<<4);//Write 1 to ACR Register
		write(ACR_ADC, temp); // Unipolar data, MSBFIRST, buf enable, range = 1, 30Hz 00 DR1 DR0
	}
	else
	{
		PORTB &=~(1<<4); //Write 0 to BUF pin
		temp &=~(1<<4);
		write(ACR_ADC, temp); //Same but with buf disable SEE DATASHEET
	}
}

void ADS1241::range(bool boolRange)
{
	uint8_t temp;
	temp = read(ACR_ADC);
	if(boolRange)
	{
		temp |= (1<<2);  //set Range bit
	}
	else
	{
		temp &= ~(1<<2); //clear Range bit
	}
	write(ACR_ADC, temp);
}

void ADS1241::SPIPol(bool boolMode)
{
	DDRB |= (1<<5); //POL PIN as output
	if(boolMode)
	{
		PORTB |=(1<<5);//Write a 1 to POL pin
	}
	else if(boolMode == false)
	{
		PORTB &=~(1<<5);//Write a 0 to POL pin
	}	
}

void ADS1241::setDataFormat(bool boolData) //if true, set data as unipolar
{
	uint8_t temp;
	temp = read(ACR_ADC);
	if(boolData)
	{
		temp |= (1<<6);  //set UB
	}
	else
	{
		temp &= ~(1<<6); //clear UB bit
	}
	write(ACR_ADC, temp);
}

float ADS1241::getTempNTC(uint8_t channel)
{
	float voltage;
	float temperature;
	float Rinf;
	float Rt;
	voltage = getVoltage(channel);
	Rinf = R_25 * exp(K_FACTOR/298.15);
	Rt = (voltage*R_LINE)/(5.0-voltage);
	temperature = K_FACTOR/(log(Rt/Rinf));
	return temperature;
}

//Give 2* 4 bits value and return a 8 bit value with uiValue1 first (MSB)
//Example uiValue1 = 0b0010 and uiValue2 = 0b1000 it return
// temp = 0b0010 1000
uint8_t ADS1241::transform(uint8_t uiValue1, uint8_t uiValue2)
{
	uint8_t temp;
	temp = uiValue1 << 4; //put the first 4 bits (MSB first)
	temp |= uiValue2;
	return temp;
}
// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library


