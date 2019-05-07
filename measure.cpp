/*
 * measure.cpp
 *
 * Created: 29/04/2019 08:46:27
 *  Author: Anthony
 */ 
#include <math.h>

#include "measure.h"
#include "MAX31856.h"
#include "ADC.h"
#include "utility.h"
#include "global.h"

float R_INF;




void initSensor(void)
{
	uint8_t i; 
	ADCinit(); // initialize and progress a dummy conversion
	for (i = 0; i<TC_nb; i++) //Setup T_type TC and filter 50Hz
	{
		max_array[i].begin();
		max_array[i].oneShotTemperature(); //dummy conversion for init
	}
}

void launchMeasures(void)
{
	uint8_t i;
	INA.setI2CSpeed(I2C_SPEED);
	INA.begin(3, R_SHUNT,1); //Init INA device for I1a or I1b with Shunt from user
	INA.begin(3, DEFAULT_R_SHUNT,2); //I2 device has a fixed shunt
	INA.setBusConversion(8500); // Maximum conversion time 8.244ms
	INA.setShuntConversion(8500); // Maximum conversion time 8.244ms
	INA.setAveraging(128); //Average 128 readings
	INA.setMode(INA_MODE_CONTINUOUS_BOTH);
	
	if(TC_Type != 'T') //By default it's T type so we don't need to change the type
	{
		if (TC_Type == 'K')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_K);
			}
		}
		else if(TC_Type == 'J')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_T);
			}
		}
		else if(TC_Type == 'N')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_N);
			}
		}
		else if(TC_Type == 'R')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_R);
			}
		}
		else if(TC_Type == 'S')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_S);
			}
		}
		else if(TC_Type == 'B')
		{
			for (i = 0; i<TC_nb; i++) //Setup
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_B);
			}
		}
		else if(TC_Type == 'E')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_E);
			}
		}
	}//end if
	
		
	R_INF = R_25*exp((-1*B_FACTOR)/298.15); //compute the term once so we avoid calculing it everytime
	IntervalMeasure = (60 * IntervalMinutes) + IntervalSeconds;
	IntervalMeasure *= 1000; //transform in milliseconds
	
}

void getAllMeasures(void)
{
	getTCMeasures();
	getNTCMeasures();
	getV24Measures();
	getV5Measures();
	getIMeasures();
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
		NTC_Measure_Array[i] = B_FACTOR / log(res/R_INF); //Steinhart Equation
	}
}

void getV24Measures(void)
{
	uint8_t i;
	float voltage;
	for (i = 0; i< V24_nb;i++)
	{
		V24_Measure_Array[i] = static_cast<float>(ADCread(V24Channel[i])); ///value from 0-1023 = ADC register
		V24_Measure_Array[i] = (V24_Measure_Array[i] * V_SUPPLY) / 1024.0; //Voltage before bridge
		V24_Measure_Array[i] = V24_Measure_Array[i] * ((R_UP+R_DOWN)/R_UP); //Compute pre divided voltage
	}
}
void getV5Measures(void)
{
	uint8_t i;
	float voltage;
	for (i = 0; i < V5_nb; i++)
	{
		V5_Measure_Array[i] = static_cast<float>(ADCread(V5Channel[i])); //value from 0-1023 = ADC register
		V5_Measure_Array[i] = (V5_Measure_Array[i]*V_SUPPLY) / 1024.0; //
	}
}

void getIMeasures(void)
{
	uint8_t i;
	uint8_t j;
	int32_t temp;
	for (i = 1; i<=I_nb;i++)
	{
		for(j=0; j<2;j++) //2 measures we get only Current and BUS voltage
		{
			I_Measure_Array[j] = INA.getBusMicroAmps(i);
			I_Measure_Array[j] = static_cast<int32_t>(INA.getBusMilliVolts(i));
		}		
	}
}

