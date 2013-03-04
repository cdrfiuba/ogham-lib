#ifndef _UTILS_H_
#define _UTILS_H_

#include <avr/io.h>
#include <util/delay.h>

// Macros

#define true                    1
#define false                   0
#define Nop()                   asm volatile("nop")

#define setBit(Byte,Bit)        ((Byte) |= (1<<(Bit)))
#define clearBit(Byte,Bit)      ((Byte) &= (~(1<<(Bit))))
#define isBitSet(Byte,Bit)      (((Byte) & (1<<(Bit))) ? true : false)
#define isBitClear(Byte,Bit)    (((Byte) & (1<<(Bit))) ? false : true)


// Variables

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

typedef struct
{
    uint8_t prescaler;
    uint8_t mode;
    void (*isr)(void);   // interrupt service routine
} PWM2_t;

PWM2_t PWM2;


// Funciones públicas

// Pines
void initPin(const IOPIN_t *pin, uint8_t dir, uint8_t pullup);
static void setPin(const IOPIN_t *pin) __attribute__((unused));
static void clearPin(const IOPIN_t *pin) __attribute__((unused));
static void togglePin(const IOPIN_t *pin) __attribute__((unused));

// PWM
void initPWM2(uint8_t mode, uint8_t prescaler, void (*isr)(void), uint8_t output);
void startPWM2(void);
void stopPWM2(void);
void setPWM2(uint8_t x);


/**
 * Enciende el bit del puerto.
 * pin: estructura que define al pin
 * Nota: función definida como static en el archivo de cabecera para que se
 * copie en cada módulo invoncante como inline. El compilador puede optimizar
 * mucho más de esta forma.
 **/
static void setPin(const IOPIN_t *pin)
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
static void clearPin(const IOPIN_t *pin)
{
    clearBit(*(pin->port), pin->bit);
}

/**
 * Invierte el bit del puerto.
 * pin: estructura que define al pin
 * Nota 1: utiliza la característica del AVR que si escribo un 1 en el registro
 * PIN produce un toggle.
 * Nota 2: función definida como static en el archivo de cabecera para que se
 * copie en cada módulo invoncante como inline. El compilador puede optimizar
 * mucho más de esta forma.
 **/
static void togglePin(const IOPIN_t *pin)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
        setBit(*(pin->pin), pin->bit);
    #elif defined (__AVR_ATmega8__)
        *(pin->pin) ^= (1 << pin->bit);
    #endif
}


#endif
