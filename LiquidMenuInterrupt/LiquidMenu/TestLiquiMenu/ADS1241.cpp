/*
  ADS1241.h - Test library for ADS1241 - implementation
  Copyright (c) 2019 Anthony Papageorgiou.  All right reserved.
*/


// include this library's description file
#include "ADS1241.h"

// include description files for other libraries used (if any)
#include <SPI.h> //MSB FIRST but configurable


//////AVR////////
#include <avr/io.h>
#include <avr/interrupt.h>

/////Personal/////
#include "utility.h"



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

/*ADS1241::ADS1241() //unused constructor
{
}

ADS1241::~ADS1241() //unused destructor
{
}*/

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in ADS1241 sketches, this library, and other libraries

void ADS1241::begin(bool isBufEnable, bool isBOCSenable) 
//change PIN FOR FINAL PRODUCT
//Polarity is hard coded 0
//RANGE = 1 because we use 5V as VREF
{
	DDRB |= (1<<5); //POL PIN as output
	PORTB &=~(1<<5);//Write a 0 to POL pin
	setupBUF(isBOCSenable);
	setupBOCS(isBOCSenable);

	
	
	SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));

}

void ADS1241::reset(void)
{
	SPI.transfer(RESET_ADC);//Does a reset
}




void ADS1241::write(uint8_t uiReg, uint8_t uiValue)
{
	setMux(CS_ADC);
	SPI.transfer(transform(WREG_ADC, uiReg));
	SPI.transfer(0x00); //We write in only 1 register (#reg - 1 see datasheet)
	SPI.transfer(uiValue);
	setMux(CS_DISABLE);
	
}

uint8_t ADS1241::read(uint8_t uiReg)
{
	uint8_t data;
	cli(); //Disable global interrupt
	setMux(CS_ADC);
	SPI.transfer(transform(RREG_ADC, uiReg));
	SPI.transfer(0x00); //We read 1 register
	delayMicroseconds(20); //Maybe not needed, check with that line commented
	data = SPI.transfer(0x00); //read the register
	sei(); // Enable global interrupt
	setMux(CS_DISABLE);
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
	cli();
	setMux(CS_ADC);
	SPI.transfer(transform(RREG_ADC, uiReg));
	SPI.transfer(0x02); //we read 3 registers
	delayMicroseconds(20); //Maybe not needed, check with that line commented
	
	for(i=0; i<2; i++) 
	{
		MyUnion.uiValue[i] = SPI.transfer(0x00); 
	}
	sei();
	setMux(CS_DISABLE);
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
	bool isDataReady = false;
	uint32_t currentMillis = millis();
	uint32_t previousMillis = millis();
	selectChannel(channel);
	while(!isDataReady || (currentMillis - previousMillis) > 50 ) //If we waited 50ms we get out
	{
		temp = read(ACR_ADC);
		isDataReady = (temp & (1<<7)); //Check DRDY bit
		currentMillis = millis();		
	}
	if(isDataReady)
	{
		data = read24bits(RDATA_ADC);
	}
	else
	{
		data = 0xFFFFFFFF;
	}
	FSR = read24bits(FSR0_ADC);
	data = (static_cast <float>(data) / static_cast <float>(FSR))*24000; //If we used 24V for calibration, 
	sei(); // Enable global interrupt
	return data;
}


void ADS1241::calibrateGain(uint8_t channel)
{
	cli();
	range(0); //Range must be 0 for calibration
	selectChannel(channel);
	SPI.transfer(SELFGCAL_ADC);
	delay(10); //wait 10ms for calibration
	getVoltage(channel); //Discard the data in the buffer
	range(1);
	sei();
}

void ADS1241::calibrateOffset(uint8_t channel)
{
	cli();
	range(0); //Range must be 0 for calibration
	selectChannel(channel);
	SPI.transfer(SELFOCAL_ADC);
	delay(10);
	getVoltage(channel); //Discard the data in the buffer
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
	DDRB |= (1<<4); //BUF pin as output
	if(boolBUF)
	{
		PORTB |= (1<<4);//Write 1 to BUF pin
		write(ACR_ADC, 0x54); // Unipolar data, MSBFIRST, buf enable, range = 1, 30Hz 00 DR1 DR0
	}
	else
	{
		PORTB &=~(1<<4); //Write 0 to BUF pin
		write(ACR_ADC, 0x44); //Same but with buf disable SEE DATASHEET
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

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

//Give 2 4 bits value and return a 8 bit value with uiValue first (MSB)
//Exemple uiValue1 = 0b0010 and uiValue2 = 0b1000 it return
// temp = 0b0010 1000
uint8_t ADS1241::transform(uint8_t uiValue1, uint8_t uiValue2)
{
	uint8_t temp;
	temp = uiValue1 << 4; //put the first 4 bits (MSB first)
	temp |= uiValue2;
	return temp;
	
}





