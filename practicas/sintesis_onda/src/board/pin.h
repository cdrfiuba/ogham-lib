#ifndef _PIN_H_
#define _PIN_H_

/**
 * Cabecera para el manejo de pines en Atmega8 y Atmega88.
 **/


#include "bitmanip.h"


/**
 * IOPIN_t
 * Estructura que define todo lo necesario para controlar un pin.
 * Ejemplo de uso:
 * #include <avr/io.h>
 * IOPIN_t led1 = {&PORTC, &DDRC, &PINC, PIN3);
 **/
typedef struct
{
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
    uint8_t bit;
} IOPIN_t;


// Funciones públicas

void configPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup);
static inline void setPin(const IOPIN_t *pin);
static inline void clearPin(const IOPIN_t *pin);
static inline void togglePin(const IOPIN_t *pin);
static inline uint8_t readPort(const IOPIN_t *pin);
static inline uint8_t readPin(const IOPIN_t *pin);

// Funciones públicas inline

/**
 * Enciende el bit del puerto.
 * pin: estructura que define al pin
 * Nota: función definida como static en el archivo de cabecera para que se
 * copie en cada módulo invoncante como inline. El compilador puede optimizar
 * mucho más de esta forma.
 **/
void setPin(const IOPIN_t *pin)
{
    setBit(*(pin->port), pin->bit);
}

/**
 * Apaga el bit del puerto.
 * pin: estructura que define al pin
 * Nota: función definida como static en el archivo de cabecera para que se
 * copie en cada módulo invoncante como inline. El compilador puede optimizar
 * mucho más de esta forma.
 **/
void clearPin(const IOPIN_t *pin)
{
    clearBit(*(pin->port), pin->bit);
}

/**
 * Invierte el bit del puerto.
 * pin: estructura que define al pin
 * Nota: función definida como static en el archivo de cabecera para que se
 * copie en cada módulo invoncante como inline. El compilador puede optimizar
 * mucho más de esta forma.
 **/
void togglePin(const IOPIN_t *pin)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
        setBit(*(pin->pin), pin->bit);
    #elif defined (__AVR_ATmega8__)
        *(pin->port) ^= (1 << pin->bit);
    #endif
}

/**
 * Lee el valor actual del port (buffer) para el pin correspondiente.
 * pin: estructura que describe el pin
 **/
uint8_t readPort(const IOPIN_t *pin)
{
    return (*(pin->port) >> pin->bit) & 0x01;
}

/**
 * Lee el valor actual del pin correspondiente (sin buffer).
 * pin: estructura que describe el pin
 **/
uint8_t readPin(const IOPIN_t *pin)
{
    return (*(pin->pin) >> pin->bit) & 0x01;
}


#endif
