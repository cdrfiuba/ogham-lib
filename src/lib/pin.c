/*
 * Biblioteca para el manejo de pines en Atmega8 y Atmega88.
 */

#include "pin.h"


/**
 * Configura un pin como entrada o salida
 * @param pin puntero a la estructura que describe al pin
 * @param dir
 *      @li PIN_DIR_OUT: salida
 *      @li PIN_DIR_IN: entrada
 * @param pullup
 *      @li PIN_PULLUP: con pullup
 *      @li PIN_NO_PULLUP: sin pullup
 * @see setPin(), clearPin(), togglePin(), readPort()
 */
void configPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup)
{
    if (dir == PIN_DIR_OUT)  // output mode
        setBit(*(pin->ddr), pin->bit);
    else  // input mode
    {
        clearBit(*(pin->ddr), pin->bit);
        if (pullup == PIN_PULLUP)
            setBit(*(pin->port), pin->bit);  // con pullup
        else
            clearBit(*(pin->port), pin->bit);  // sin pullup
    }
}
