#ifndef _PIN_H_
#define _PIN_H_

/**
 * @file pin.h
 * Cabecera para el manejo de pines en Atmega8 y Atmega88.
 **/

/* Nota: las funciones definidas como static inline permiten al compilador tener
 * una copia local en el archivo invocante y efectivamente optimizarlas como
 * funciones inline.
 */

#include <avr/io.h>
#include "bitmanip.h"


/**
 * IOPIN_t
 * Estructura que define todo lo necesario para controlar un pin.
 * 
 * Ejemplo de uso:
 * @code
 * #include <pin.h>
 * IOPIN_t led1 = {&PORTC, &DDRC, &PINC, PIN3);
 * @endcode
 * @see configPin()
 **/
typedef struct
{
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
    uint8_t bit;
} IOPIN_t;

// Macros

// Constantes para dirección del pin
#define PIN_DIR_IN 0
#define PIN_DIR_OUT 1

// Constantes para resistencia de pullup
#define PIN_NO_PULLUP 0
#define PIN_PULLUP 1

/**
 * Crea la definición de un pin de manera conveniente.
 * Ejemplo de uso:
 * @code
 * #include <pin.h>
 * IOPIN_t led1 = definePin(C, 3);
 * @endcode
 * @param
 *  a letra que identifica al puerto. Debe estar en mayúscula.
 *  b número que identifica el pin, de 0 a 7
 **/
#define definePin(a, b) {&PORT##a, &DDR##a, &PIN##a, b}

// Funciones públicas

void configPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup);
static inline void setPin(const IOPIN_t *pin);
static inline void clearPin(const IOPIN_t *pin);
static inline void togglePin(const IOPIN_t *pin);
static inline uint8_t readPort(const IOPIN_t *pin);
static inline uint8_t readPin(const IOPIN_t *pin);
static inline void configPinOut(const IOPIN_t *pin);
static inline void configPinIn(const IOPIN_t *pin);


// Funciones públicas inline

/**
 * Enciende el bit del puerto.
 * @param pin puntero a la estructura que describe al pin
 **/
void setPin(const IOPIN_t *pin)
{
    setBit(*(pin->port), pin->bit);
}

/**
 * Apaga el bit del puerto.
 * @param pin puntero a la estructura que describe al pin
 **/
void clearPin(const IOPIN_t *pin)
{
    clearBit(*(pin->port), pin->bit);
}

/**
 * Invierte el bit del puerto.
 * @param pin puntero a la estructura que describe al pin
 **/
void togglePin(const IOPIN_t *pin)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)  || (__AVR_ATmega168P__) || (__AVR_ATmega168P__) || (__AVR_ATmega328__) || (__AVR_ATmega328P__)
        setBit(*(pin->pin), pin->bit);
    #elif defined (__AVR_ATmega8__)
        *(pin->port) ^= (1 << pin->bit);
    #endif
}

/**
 * Lee el valor actual del port (buffer) para el pin correspondiente.
 * @param pin puntero a la estructura que describe al pin
 **/
uint8_t readPort(const IOPIN_t *pin)
{
    return (*(pin->port) >> pin->bit) & 0x01;
}

/**
 * Lee el valor actual del pin correspondiente (sin buffer).
 * @param pin puntero a la estructura que describe al pin
 **/
uint8_t readPin(const IOPIN_t *pin)
{
    return (*(pin->pin) >> pin->bit) & 0x01;
}

/**
 * Configura el pin como salida.
 * @param pin puntero a la estructura que describe al pin
 **/
void configPinOut(const IOPIN_t *pin)
{
    configPin(pin, PIN_DIR_OUT, PIN_NO_PULLUP);
}

/**
 * Configura el pin como entrada (sin pullup).
 * @param pin puntero a la estructura que describe al pin
 **/
void configPinIn(const IOPIN_t *pin)
{
    configPin(pin, PIN_DIR_IN, PIN_NO_PULLUP);
}

#endif
