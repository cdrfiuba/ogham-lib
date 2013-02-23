#include <avr/io.h>
#include "utils.h"

/**
 * Configura un pin como entrada o salida
 * pin: estructura que define al pin
 * dir: 1 salida, 0 entrada
 * pullup: 1 con pullup, 0 sin pullup
 **/
void initPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup)
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

