#include <stdint.h>
#include <avr/io.h>

#include "utility.h"

void ADCinit(void)
{
	enableVref(false);
	ADMUX = 0x02; //select channel 2 by default and set ref to extern and ADLAR = 0
	enableVref(true);
	ADCSRA = 0xC7; //Start ADC and begin a dummy conversion Prescale of 128 => Fadc = 125kHz
	DIDR0 = 0x0C; //Disable digital input on ADC2 & ADC3
	DIDR2 = 0xFF; //Disable all digital input on K port
	 _delay_us(100); //wait Vref to be stable
}

void ADCselectChannel(uint8_t channel)
{
	ADMUX &= 0xF8;//reset to channel 0 (clear last 3 bits)
	if(channel <= 7)
	{
		ADMUX |= channel; // Set first 3 bits of mux
		ADMUX &= ~(1 << MUX3); //Clear Mux 4 and mux 3
		ADMUX &= ~(1 << MUX4);
		ADCSRB &= ~(1 << MUX5); //Clear Mux 5
	}
	else if (channel <= 14)
	{
		ADCSRB |= (1<< MUX5); //Set mux 5 bit
		ADMUX &= ~(1 << MUX3); //Clear Mux 4 and mux 3
		ADMUX &= ~(1 << MUX4);// normally it's not needed
		ADMUX |= (channel - 8); //Set MUX0,MUX1,MUX2
	}
}

void ADCstartConversion(void)
{
	ADCSRA |= (1<<ADSC) | (1<<ADIF); //Start conversion and clear ADC INT FLAG
}

uint16_t ADCread(uint8_t channel)
{
	uint16_t result = 0;
	ADCselectChannel(channel);
	ADCstartConversion();
	while((ADCSRA & (1 << ADIF)) == 0); //wait ADIF to be set to one
	result = ADCL;
	result |= (ADCH << 8); //ADLAR = 0
	return result;
}
