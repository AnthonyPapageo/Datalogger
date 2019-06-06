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

void initSensors(void) //Init ADC, INA and set correct TC type for each MAX31856
{
	uint8_t i;
	DateTime dt;
	ADCinit(); // initialize and progress a dummy conversion and enable VREF
	
	if(I_nb>0) //Init INA
	{
		INA.setI2CSpeed(INA_I2C_STANDARD_MODE);
		INA.begin(3, R_SHUNT,0); //Init INA device for I1a or I1b with Shunt from user
		if(I_nb == 2 )
		{
			INA.begin(3, DEFAULT_R_SHUNT,1); //I2 device has a fixed shunt.
		}
		INA.setBusConversion(8500); // Maximum conversion time 8.244ms
		INA.setShuntConversion(8500); // Maximum conversion time 8.244ms
		INA.setAveraging(128); //Average 128 readings
		INA.setMode(INA_MODE_CONTINUOUS_BOTH); //Continous conversion
	}
	
	for (i = 0; i<TC_nb; i++) //filter 50Hz
	{
		max_array[i].begin();
		max_array[i].oneShotTemperature(); //dummy conversion for init
		max_array[i].setAveraging(4); //Average 4 measures
	}
	
	if(TC_Type[0] != 'T') //By default it's T type, so we have to change for another type
	{
		if (TC_Type[0] == 'K')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_K);
			}
		}
		else if(TC_Type[0] == 'J')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_T);
			}
		}
		else if(TC_Type[0] == 'N')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_N);
			}
		}
		else if(TC_Type[0] == 'R')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_R);
			}
		}
		else if(TC_Type[0] == 'S')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_S);
			}
		}
		else if(TC_Type[0] == 'B')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_B);
			}
		}
		else if(TC_Type[0] == 'E')
		{
			for (i = 0; i<TC_nb; i++)
			{
				max_array[i].setThermocoupleType(MAX31856_TCTYPE_E);
			}
		}
	}//end if
			
	R_INF = R_25*exp((-1.0*B_FACTOR)/298.15); //compute the term once so we avoid computing it everytime
	
}

void computeTime(void)
{
	uint32_t TimeInSeconds;
	IntervalMeasure = (60 * IntervalMinutes) + IntervalSeconds; //nb of seconds for interval
	Global_Begin_Datetime = RTC.now();
	TimeInSeconds = (DurationHour*3600UL) + (DurationMin * 60UL) + DurationSec; //Duration in seconds
	Global_End_Datetime = Global_Begin_Datetime + TimeSpan(TimeInSeconds);
}


void getAllMeasures(void) //Launch for each MAX31856 a conversion, get other measure then get back to the max to get the result of the conversion
{
	uint8_t i;
	uint32_t temp;
	PORTJ ^= (1<<6);
	
	temp = millis(); //
	for (i = 0; i<TC_nb; i++)
	{
		max_array[i].oneShotTemperature(); //Tell each MAX to begin a conversion
	}
	getNTCMeasures();
	getV24Measures();
	getV5Measures();
	getIMeasures();
	if(TC_nb > 0)//if we are taking TC measure, we have to wait
	{
		while(millis() < (temp + 500)); //the other measures didn't take more than 500ms, we wait here
	}
	for (i = 0; i<TC_nb; i++) 
	{
		TC_Measure_Array[i] = max_array[i].readThermocoupleTemperature(); //read the temperature for each MAX
		CJT_Measure_Array[i] = max_array[i].readCJTemperature();
	}
	
	Nb_Of_Measure++;//increment counter of number of measure
	if((Nb_Of_Measure % 65534) == 0)  //max number of line for excel -1, file is full so we create a new file
	{
		firstLineSD(); //get new filename and create a new file
		Global_Is_multifile = true;
	}
	PORTJ ^= (1<<6);
}

void getNTCMeasures(void)
{
	uint8_t i;
	volatile float res;
	for (i = 0; i<NTC_nb; i++)
	{
		NTC_Measure_Array[i] = static_cast<float>(ADCread(NTCChannel[i])); //value from 0-1023 = ADC register
		NTC_Measure_Array[i] = ( NTC_Measure_Array[i]*V_SUPPLY) / 1024.0; //now the array contain the voltage from 0 to 5V
		res = (R_LINE_NTC / ((V_SUPPLY/NTC_Measure_Array[i]) - 1.0  ));
		res = ((R_LINE_NTC * NTC_Measure_Array[i]) / (V_SUPPLY - NTC_Measure_Array[i])); //With the voltage we can compute the R value
		NTC_Measure_Array[i] = (B_FACTOR / log(res/R_INF)) - 273.15; //Steinhart-Hart Equation and transformation in Celsius
	}
}

void getV24Measures(void)
{
	uint8_t i;
	for (i = 0; i< V24_nb;i++)
	{
		V24_Measure_Array[i] = static_cast<float>(ADCread(V24Channel[i])); ///value from 0-1023 = ADC register
		V24_Measure_Array[i] = (V24_Measure_Array[i] * V_SUPPLY) / 1024.0; //Voltage after bridge
		V24_Measure_Array[i] = V24_Measure_Array[i] * ((R_UP+R_DOWN)/R_DOWN); //Compute pre divided voltage
	}
}
void getV5Measures(void)
{
	uint8_t i;
	for (i = 0; i < V5_nb; i++)
	{
		V5_Measure_Array[i] = static_cast<float>(ADCread(V5Channel[i])); //value from 0-1023 = ADC register
		V5_Measure_Array[i] = (V5_Measure_Array[i]*V_SUPPLY) / 1024.0; //Voltage
	}
}

void getIMeasures(void)
{
	uint8_t i;
	for (i = 0; i<I_nb;i++)
	{
		I_Measure_Array[(2*i)] = INA.getBusMicroAmps(i); //sensor 0 is sensor 1 in INA lib
		I_Measure_Array[((2*i)+1)] = static_cast<int32_t>(INA.getBusMilliVolts(i));	
	}
}

