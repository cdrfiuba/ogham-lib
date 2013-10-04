/*
 * Biblioteca para el manejo de interrupciones en Atmega8 y Atmega88.
 */

#include <stddef.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "bitmanip.h"


// Vector de punteros a la función de manejo de la interrupción externa.
static void (*extIntIsr[2])(void);


/**
 * Configura una interrupción externa.
 * @param num número de interupción 0 o 1
 * @param sense tipo de activación para la interrupción
 *        @li 0: nivel bajo
 *        @li 1: cualquier cambio de nivel
 *        @li 2: flanco descendente
 *        @li 3: flanco ascendente
 * @param isr puntero a la función de manejo de la interrupción. Si es NULL se
 *        desactiva la interrupción.
 **/
void configExtInt(uint8_t num, uint8_t sense, void (*isr)(void))
{
    num = (num & 0x01);
    uint8_t offset = num*2;
    
    extIntIsr[num] = isr;  // callback pointer
    
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)  || (__AVR_ATmega168P__) || (__AVR_ATmega168P__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
        switch(sense)
        {
            case 0:  // nivel bajo
                clearBit(EICRA, 0+offset); // ISCx0 = 0
                clearBit(EICRA, 1+offset); // ISCx1 = 0
                break;
            case 1:  // cualquier cambio de nivel
                setBit(EICRA, 0+offset); // ISCx0 = 1
                clearBit(EICRA, 1+offset); // ISCx1 = 0
                break;
            case 2:  // flanco descendente
                clearBit(EICRA, 0+offset); // ISCx0 = 0
                setBit(EICRA, 1+offset); // ISCx1 = 1
                break;
            case 3:  // flanco ascendente
                setBit(EICRA, 0+offset); // ISCx0 = 1
                setBit(EICRA, 1+offset); // ISCx1 = 1
                break;
        }
    #elif defined (__AVR_ATmega8__)
        switch(sense)
        {
            case 0:  // nivel bajo
                clearBit(MCUCR, 0+offset); // ISCx0 = 0
                clearBit(MCUCR, 1+offset); // ISCx1 = 0
                break;
            case 1:  // cualquier cambio de nivel
                setBit(MCUCR, 0+offset); // ISCx0 = 1
                clearBit(MCUCR, 1+offset); // ISCx1 = 0
                break;
            case 2:  // flanco descendente
                clearBit(MCUCR, 0+offset); // ISCx0 = 0
                setBit(MCUCR, 1+offset); // ISCx1 = 1
                break;
            case 3:  // flanco ascendente
                setBit(MCUCR, 0+offset); // ISCx0 = 1
                setBit(MCUCR, 1+offset); // ISCx1 = 1
                break;
        }
    #endif
    
    if (isr != NULL)
    {
        // habilitar la interrupción
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__) || (__AVR_ATmega168P__) || (__AVR_ATmega168P__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
            setBit(EIMSK, 0+num);  // INTx = 1
        #elif defined (__AVR_ATmega8__)
            setBit(GICR, 6+num);  // INTx = 1
        #endif
        sei(); // habilitar interrupciones globales
    }
    else
    {
        // deshabilitar la interrupción
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__) || (__AVR_ATmega168P__) || (__AVR_ATmega168P__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
            clearBit(EIMSK, 0+num);  // INTx = 0
        #elif defined (__AVR_ATmega8__)
            clearBit(GICR, 6+num);  // INTx = 0
        #endif
    }
}

/**
 * @internal
 * External interrupt 0
 */
ISR(INT0_vect)
{
    extIntIsr[0]();
}

/**
 * @internal
 * External interrupt 1
 */
ISR(INT1_vect)
{
    extIntIsr[1]();
}
