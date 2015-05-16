#ifndef _BOARD_H_
#define _BOARD_H_
/**
 * @file board.h
 * Define cómo está conectado todo en la placa.
 **/
 
#include <avr/io.h>
#include "lib/utils.h"


// LEDs
#define NUM_LEDS 4
/*
 * Tener una copia de esto en cada módulo invocante permite que el compilador
 * optimice mejor.
 */
static const IOPIN_t LED[] = {definePin(C,3), definePin(C,2), definePin(D,7), definePin(D,4)};
static const IOPIN_t BUTTON = definePin(D,2);

// Funciones públicas
void initBoard(void);
void initLeds(void);
void initButton(void (*isr)(void), uint8_t debounce);
uint8_t readButton(void);
void initADC(void);

#endif
