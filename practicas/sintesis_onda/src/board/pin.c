/*
 * Biblioteca para el manejo de pines en Atmega8 y Atmega88.
 */

#include <avr/io.h>
#include "pin.h"


/**
 * Configura un pin como entrada o salida
 * @param pin puntero a la estructura que describe al pin
 * @param dir
 *      @li 1: salida
 *      @li 0: entrada
 * @param pullup
 *      @li 1: con pullup
 *      @li 0: sin pullup
 * @see setPin(), clearPin(), togglePin(), readPort()
 */
void configPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup)
{
    if (dir)  // output mode
        setBit(*(pin->ddr), pin->bit);
    else  // input mode
    {
        clearBit(*(pin->ddr), pin->bit);
        if (pullup)
            setBit(*(pin->port), pin->bit);  // con pullup
        else
            clearBit(*(pin->port), pin->bit);  // sin pullup
    }
}
