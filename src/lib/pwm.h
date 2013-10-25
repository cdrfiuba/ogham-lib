#ifndef _PWM_H_
#define _PWM_H_

/**
 * @file pwm.h
 * Cabecera para el manejo del PWM en Atmega8 y Atmega88.
 **/


// Tipos

typedef struct
{
    uint8_t prescaler;
    uint8_t mode;
    void (*isr)(void);   // interrupt service routine
} PWM2_t;

PWM2_t PWM2;


// Funciones públicas


// PWM
void configPWM2(uint8_t mode, uint8_t prescaler, void (*isr)(void), uint8_t output);
void startPWM2(void);
void stopPWM2(void);
void setPWM2(uint8_t x);


#endif
