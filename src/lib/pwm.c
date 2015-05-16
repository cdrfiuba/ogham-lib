/*
 * Biblioteca para el manejo del PWM en Atmega8 y Atmega88.
 */

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "pwm.h"
#include "bitmanip.h"


/**
 * Configura el PWM asociado al timer 2 (8 bits).
 * @param mode @li 1: phase correct PWM
 *             @li 3: fast PWM
 * @param prescaler divisor para la frecuencia del micro
 *            @li 1: fclk
 *            @li 2: fclk/8
 *            @li 3: fclk/32
 *            @li 4: fclk/64
 *            @li 5: fclk/128
 *            @li 6: fclk/256
 *            @li 7: fclk/1024
 * @param isr puntero a la función de manejo de la interrupción que se produce
 *  cada vez que se alcanza el nivel de comparación. La función no debe recibir
 *  ningún parámetro y no debe devolver nada. Ej: void mifunc(void){}. Si es
 *  NULL no se generará la interrupción.
 * @param output @li 0 sin salida
 *               @li 1 salida por OC2
 **/
void configPWM2(uint8_t mode, uint8_t prescaler, void (*isr)(void), uint8_t output)
{
    stopPWM2();
    setPWM2(128);  // inicia en 50%
    
    PWM2.mode = mode;
    PWM2.prescaler = prescaler;
    PWM2.isr = isr;

    if (isr != NULL)
    {
        // Habilitar interrupción de overflow del timer
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        setBit(TIMSK2, 0);  // TOIE2 = 1 timer 2 overflow interrupt enable
    #elif  defined (__AVR_ATmega8__)
        setBit(TIMSK, 6);  // TOIE2 = 1 timer 2 overflow interrupt enable
    #endif
        sei();  // habilitar interrupciones globales
    }
    else
    {
        // Deshabilitar interrupción de overflow del timer
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        clearBit(TIMSK2, 0);  // TOIE2 = 0 timer 2 overflow interrupt disable
    #elif  defined (__AVR_ATmega8__)
        clearBit(TIMSK, 6);  // TOIE2 = 0 timer 2 overflow interrupt disable
    #endif
    }
    
    switch(mode)
    {
        case 1:  // phase correct PWM
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2A, 0);    // WGM20 = 1
                clearBit(TCCR2A, 1);  // WGM21 = 0
                clearBit(TCCR2B, 3);  // WGM22 = 0
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 6);     // WGM20 = 1
                clearBit(TCCR2, 3);   // WGM21 = 0
    #endif
                break;
        case 3:  // fast PWM
        default:
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2A, 0);    // WGM20 = 1
                setBit(TCCR2A, 1);    // WGM21 = 1
                clearBit(TCCR2B, 3);  // WGM22 = 0
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 6);     // WGM20 = 1
                setBit(TCCR2, 3);     // WGM21 = 1
    #endif
                break;
    }
    
    if (output)  // configuramos el pin de salida
    {
        setBit(DDRB, PIN3);   // pin como salida
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        setBit(TCCR2A, 7);    // COM2A1 = 1
        clearBit(TCCR2A, 6);  // COM2A0 = 0
    #elif  defined (__AVR_ATmega8__)
        setBit(TCCR2, 5);    // COM21 = 1
        clearBit(TCCR2, 4);  // COM20 = 0
    #endif
    }
    else  // sin salida por pin
    {
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        clearBit(TCCR2A, 7); // COM2A1 = 0
        clearBit(TCCR2A, 6); // COM2A0 = 0
    #elif  defined (__AVR_ATmega8__)
        clearBit(TCCR2, 5);  // COM21 = 0
        clearBit(TCCR2, 4);  // COM20 = 0
    #endif
    }
}

/**
 * Arranca el PWM a la frecuencia que fue configurada.
 **/
void startPWM2(void)
{
    switch(PWM2.prescaler)
    {
        case 1:  // fclk
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2B, 0);    // CS20 = 1
                clearBit(TCCR2B, 1);  // CS21 = 0
                clearBit(TCCR2B, 2);  // CS22 = 0
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);     // CS20 = 1
                clearBit(TCCR2, 1);   // CS21 = 0
                clearBit(TCCR2, 2);   // CS22 = 0
    #endif
                break;
        case 2:  // fclk/8
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                setBit(TCCR2B, 1);    // CS21 = 1
                clearBit(TCCR2B, 2);  // CS22 = 0
    #elif  defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);   // CS20 = 0
                setBit(TCCR2, 1);     // CS21 = 1
                clearBit(TCCR2, 2);   // CS22 = 0
    #endif
                break;
        case 3:  // fclk/32
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2B, 0);    // CS20 = 1
                setBit(TCCR2B, 1);    // CS21 = 1
                clearBit(TCCR2B, 2);  // CS22 = 0
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);     // CS20 = 1
                setBit(TCCR2, 1);     // CS21 = 1
                clearBit(TCCR2, 2);   // CS22 = 0
    #endif
                break;
        case 4:  // fclk/64
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                clearBit(TCCR2B, 1);  // CS21 = 0
                setBit(TCCR2B, 2);    // CS22 = 1
    #elif  defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);   // CS20 = 0
                clearBit(TCCR2, 1);   // CS21 = 0
                setBit(TCCR2, 2);     // CS22 = 1
    #endif
                break;
        case 5:  // fclk/128
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2B, 0);    // CS20 = 1
                clearBit(TCCR2B, 1);  // CS21 = 0
                setBit(TCCR2B, 2);    // CS22 = 1
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);     // CS20 = 1
                clearBit(TCCR2, 1);   // CS21 = 0
                setBit(TCCR2, 2);     // CS22 = 1
    #endif
                break;
        case 6:  // fclk/256
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                setBit(TCCR2B, 1);    // CS21 = 1
                setBit(TCCR2B, 2);    // CS22 = 1
    #elif  defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);   // CS20 = 0
                setBit(TCCR2, 1);     // CS21 = 1
                setBit(TCCR2, 2);     // CS22 = 1
    #endif
                break;
        case 7:  // fclk/1024
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR2B, 0);    // CS20 = 1
                setBit(TCCR2B, 1);    // CS21 = 1
                setBit(TCCR2B, 2);    // CS22 = 1
    #elif  defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);     // CS20 = 1
                setBit(TCCR2, 1);     // CS21 = 1
                setBit(TCCR2, 2);     // CS22 = 1
    #endif
                 break;
    }    
}

/**
 * Detiene el PWM.
 **/
void stopPWM2(void)
{
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        TCCR2B &= 0b11111000;   // CS20 = CS21 = CS22 = 0
        //clearBit(TCCR2B, 0);  // CS20 = 0
        //clearBit(TCCR2B, 1);  // CS21 = 0
        //clearBit(TCCR2B, 2);  // CS22 = 0
    #elif  defined (__AVR_ATmega8__)
        TCCR2 &= 0b11111000;  // CS20 = CS21 = CS22 = 0
        //clearBit(TCCR2, 0);  // CS20 = 0
        //clearBit(TCCR2, 1);  // CS21 = 0
        //clearBit(TCCR2, 2);  // CS22 = 0
    #endif
}

/**
 * Establece el ancho de pulso.
 * @param x ancho del pulso
 **/
void setPWM2(uint8_t x)
{
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        OCR2A = x;
    #elif  defined (__AVR_ATmega8__)
        OCR2 = x;
    #endif
}

/**
 * @internal
 * Timer 2 overflow interrupt
 */
ISR(TIMER2_OVF_vect)
{
    PWM2.isr();
}
