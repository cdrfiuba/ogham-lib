#ifndef _DELAY_H_
#define _DELAY_H_


#include <util/delay_basic.h>

static inline void delay_ms(uint16_t msec);

/**
 * Produce un retardo durante una cantidad etera de milisegundos.
 * msec: tiempo en milisengundos. Rango de 1 a 65536.
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


#endif
