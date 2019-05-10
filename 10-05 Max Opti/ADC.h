#ifndef ADC_H
#define ADC_H

void ADCinit(void);
void ADCselectChannel(uint8_t channel);
void ADCstartConversion(void);
uint16_t ADCread(uint8_t channel);



#endif