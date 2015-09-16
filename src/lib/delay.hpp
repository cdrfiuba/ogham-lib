#ifndef _DELAY_H_
#define _DELAY_H_

/**
 * @file delay.h
 * Cabecera para funciones de demoras temporales.
 */

#include <util/delay_basic.h>

extern "C"
{
static inline void delay_ms(uint16_t msec);
static inline void delay_us(uint16_t usec);
}

/**
 * Produce un retardo durante una cantidad entera de milisegundos.
 * @param msec tiempo en milisegundos. Rango de 1 a 65535.
 **/
void delay_ms(uint16_t msec)
{
    uint32_t tics = (F_CPU/4000) * msec;
    uint16_t highTics = (tics>>16);
    uint16_t lowTics = (uint16_t)tics;
    
    while (highTics--)
        _delay_loop_2(0xFFFF);

    _delay_loop_2(lowTics);
}

/**
 * Produce un retardo durante una cantidad entera de microsegundos.
 * @param usec tiempo en microsegundos. Rango de 1 a 2047.
 **/
void delay_us(uint16_t usec)
{
    // restringimos el rango a 11 bits (2047) para que no desborde
    usec = 0x07FF & usec;
    
    uint16_t tics = ((F_CPU/1000000) * usec)/3;
    uint8_t highTics = (tics>>8);
    uint8_t lowTics = (uint8_t)tics;
    
    while (highTics--)
        _delay_loop_1(0xFF);

    _delay_loop_1(lowTics);
}

#endif
