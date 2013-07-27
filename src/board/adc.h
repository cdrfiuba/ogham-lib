#ifndef _ADC_H_
#define _ADC_H_

/**
 * @file adc.h
 * Cabecera para el manejo del conversor analógico/digital en Atmega8 y Atmega88.
 **/


void configADC(uint8_t presc);
void setADCChannel(uint8_t chan);
uint8_t getADCValue(void);


#endif
