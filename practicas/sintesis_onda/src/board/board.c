#include <avr/io.h>
#include "board.h"
#include "utils.h"


void initLeds(void);

/**
 * Inicializa todo lo vinculado a la placa. Debe ser llamada siempre al comienzo
 * antes de usar cualquier elemento de la placa.
 **/
void initBoard(void)
{
    initLeds();
}

/**
 * Inicializa los puertos de los leds.
 **/
void initLeds(void)
{
    uint8_t i;
    
    for (i=0; i<NUM_LEDS; i++)
    {
        initPin(&LED[i], 1, 0);  // pin como salida
        setPin(&LED[i]);  // iniciamos el LED apagado
    }
}

