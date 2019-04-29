/*
 * measure.cpp
 *
 * Created: 29/04/2019 08:46:27
 *  Author: Anthony
 */ 
#include <math.h>

#include "MAX31856.h"
#include "ADC.h"
#include "utility.h"
#include "global.h"

float R_INF;


void initMeasures(void)
{
	uint8_t i; 
	ADCinit(); // initialize and progress a dummy conversion
	for (i = 0; i<TC_nb; i++) //Setup T_type TC and filter 50Hz
	{
		max_array[i].begin();
	}
	
	INA.setI2CSpeed(I2C_SPEED);
	INA.begin(3, R_SHUNT,1); //Init INA device for I1a or I1b with Shunt from user
	INA.begin(3, DEFAULT_R_SHUNT,2); //I2 device has a fixed shunt
	INA.setBusConversion(8500); // Maximum conversion time 8.244ms
	INA.setShuntConversion(8500); // Maximum conversion time 8.244ms
	INA.setAveraging(128); //Average 128 readings
	INA.setMode(INA_MODE_CONTINUOUS_BOTH);
	
	R_INF = R_25*exp((-1*B_FACTOR)/298.15);
}

void getMeasures(void)
{
	
}

void getTCMeasures(void)
{
	uint8_t i;
	for (i = 0; i<TC_nb; i++)
	{
		TC_Measure_Array[i] = max_array[i].readThermocoupleTemperature();
	}
}

void getNTCMeasures(void)
{
	uint8_t i;
	float res;
	for (i = 0; i<NTC_nb; i++)
	{
		NTC_Measure_Array[i] = static_cast<float>(ADCread(NTCChannel[i])); //value from 0-1023 = ADC register
		NTC_Measure_Array[i] = ( NTC_Measure_Array[i]*V_SUPPLY) / 1024.0; //now the array contain the voltage from 0 to 5V
		res = ((R_LINE_NTC * NTC_Measure_Array[i]) / V_SUPPLY - NTC_Measure_Array[i]); //With the voltage we can compute the R value
		NTC_Measure_Array[i] = B_FACTOR / log(res/R_INF);
	}
}

void getV24Measures(void)
{
	uint8_t i;
	float voltage;
	for (i = 0; i< V24_nb;i++)
	{
		V24_Measure_Array[i] = static_cast<float>(ADCread(NTCChannel[i])); //Voltage after bridge
		V24_Measure_Array[i] = V24_Measure_Array[i] * ((R_UP+R_DOWN)/R_UP);
	}
}
