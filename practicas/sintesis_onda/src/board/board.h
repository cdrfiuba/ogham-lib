#ifndef _BOARD_H_
#define _BOARD_H_
/**
 * @file board.h
 * Define cómo está conectado todo en la placa.
 **/
 
#include <avr/io.h>
#include "utils.h"


// LEDs
#define NUM_LEDS 4
/*
 * Tener una copia de esto en cada módulo invocante permite que el compilador
 * optimice mejor.
 */
static const IOPIN_t LED[] = {{&PORTC, &DDRC, &PINC, PIN3}, {&PORTC, &DDRC, &PINC, PIN2}, {&PORTD, &DDRD, &PIND, PIN7}, {&PORTD, &DDRD, &PIND, PIN4}};
static const IOPIN_t BUTTON = {&PORTD, &DDRD, &PIND, PIN2};

// Funciones públicas
void initBoard(void);
void initLeds(void);
void initButton(void (*isr)(void), uint8_t debounce);
uint8_t readButton(void);

#endif
