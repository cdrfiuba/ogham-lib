/*
 * Biblioteca para el manejo del conversor analógico/digital en Atmega8 y Atmega88.
 */

#include <avr/io.h>
#include "adc.h"
#include "bitmanip.h"


/**
 * Configura el conversor analógico/digital.
 * @see setADChannel(), getADCValue()
 */
void configADC(void)
{
    // Encendemos el ADC
    clearBit(PRR, 0);
    
    /* REFS = 01 (AVcc)
       ADLAR = 1 (left adjusted for 8 bit read)
       MUX = 1111 (GND)
    */
    ADMUX = 0b01101111;
    
    // No auto trigger
    ADCSRB = 0x00;
    
    // Habilitar el ADC (ADEN = 1); fclk/128 (ADPS2:0=111)
    ADCSRA = 0b10000111;
}

/**
 * Establece el canal para la conversión digital/analógica.
 * @param chan Número de canal del multiplexor del A/D de 0 a 15
 * @see configADC(), getADCValue()
 */
void setADCChannel(uint8_t chan)
{
    ADMUX &= 0xF0;
    ADMUX |= (0x0F & chan);
}

/**
 * Realiza una conversión y devuelve el valor del conversor analógico/digital
 * para el canal seleccionado.
 * @return Valor de 8 bits resultante de la conversión.
 * @see configADC(), setADChannel()
 */
uint8_t getADCValue(void)
{
    // Disparar una conversión (ADSC=1)
    setBit(ADCSRA, 6);
    
    // Esperar hasta que la conversión termine
    while (isBitSet(ADCSRA, 6));
    
    // Retornar el registro de 8 bits
    return ADCH;
}
