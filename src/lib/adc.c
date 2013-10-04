/*
 * Biblioteca para el manejo del conversor analógico/digital en Atmega8 y Atmega88.
 */

#include <avr/io.h>
#include "lib/adc.h"
#include "lib/bitmanip.h"


/**
 * Configura el conversor analógico/digital.
 * @param presc Prescaler para el conversor: 0 a 7
 * @see setADChannel(), getADCValue()
 */
void configADC(uint8_t presc)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)  || (__AVR_ATmega168P__) || (__AVR_ATmega168P__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
    // Encendemos el ADC
    clearBit(PRR, 0);

    // No auto trigger
    ADCSRB = 0x00;
#endif
        
    /* REFS = 01 (AVcc)
       ADLAR = 1 (left adjusted for 8 bit read)
       MUX = 1111 (GND)
    */
    ADMUX = 0b01101111;
    
    // Habilitar el ADC (ADEN = 1); ADPS2:0=presc
    ADCSRA = (0b00000111 & presc);
    setBit(ADCSRA, 7);
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
