/*
 * Biblioteca para el manejo del timer en Atmega88.
 */

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"
#include "bitmanip.h"

/**
 * Configura el timer0 de 8 bits. Cuenta de 0 a 255.
 * @param prescaler divisor para la frecuencia del micro
 *      @li 1: fclk
 *      @li 2: fclk/8
 *      @li 3: fclk/64
 *      @li 4: fclk/256
 *      @li 5: fclk/1024
 * @param isr puntero a la función de manejo de la interrupción que se produce
 *  cuando el timer llega a su cuenta máxima. La función no debe recibir
 *  ningún parámetro y no debe devolver nada. Ej: void mifunc(void){}. Si es
 *  NULL no se generará la interrupción.
 **/
void configTimer0(uint8_t prescaler, void (*isr)(void))
{
    stopTimer0();
    
    Timer0.mode = 0;
    Timer0.prescaler = prescaler;
    Timer0.isr = isr;

    if (isr != NULL)
    {
        // Habilitar interrupción de overflow del timer
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        setBit(TIMSK0, 0);  // TOIE0 = 1 timer 0 overflow interrupt enable
    #elif  defined (__AVR_ATmega8__)
        setBit(TIMSK, 0);  // TOIE0 = 1 timer 0 overflow interrupt enable
    #endif
        sei();  // habilitar interrupciones globales
    }
    else
    {
        // Deshabilitar interrupción de overflow del timer
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        clearBit(TIMSK0, 0);  // TOIE0 = 0 timer 0 overflow interrupt disable
    #elif  defined (__AVR_ATmega8__)
        clearBit(TIMSK, 0);  // TOIE2 = 0 timer 0 overflow interrupt disable
    #endif
    }
    
    // Configurar el modo del timer
    #if     defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
       defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        clearBit(TCCR0A, 0);    // WGM00 = 0
        clearBit(TCCR0A, 1);  // WGM01 = 0
        clearBit(TCCR0B, 3);  // WGM02 = 0
    #elif defined (__AVR_ATmega8__)
        // completar...
        //setBit(TCCR2, 6);     // WGM20 = 1
        //clearBit(TCCR2, 3);   // WGM21 = 0
    #endif
}

/**
 * Arranca el Timer 0 a la frecuencia que fue configurada.
 **/
void startTimer0(void)
{
    switch(Timer0.prescaler)
    {
        case 1:  // fclk
    #if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR0B, 0);    // CS00 = 1
                clearBit(TCCR0B, 1);  // CS01 = 0
                clearBit(TCCR0B, 2);  // CS02 = 0
    #elif  defined (__AVR_ATmega8__)
                /*setBit(TCCR2, 0);     // CS20 = 1
                clearBit(TCCR2, 1);   // CS21 = 0
                clearBit(TCCR2, 2);   // CS22 = 0*/
    #endif
                break;
        case 2:  // fclk/8
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                clearBit(TCCR0B, 0);  // CS00 = 0
                setBit(TCCR0B, 1);    // CS01 = 1
                clearBit(TCCR0B, 2);  // CS02 = 0
    #elif  defined (__AVR_ATmega8__)
                /*clearBit(TCCR2, 0);   // CS20 = 0
                setBit(TCCR2, 1);     // CS21 = 1
                clearBit(TCCR2, 2);   // CS22 = 0*/
    #endif
                break;
        case 3:  // fclk/64
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR0B, 0);    // CS00 = 1
                setBit(TCCR0B, 1);    // CS01 = 1
                clearBit(TCCR0B, 2);  // CS02 = 0
    #elif  defined (__AVR_ATmega8__)
                /*setBit(TCCR2, 0);     // CS20 = 1
                setBit(TCCR2, 1);     // CS21 = 1
                clearBit(TCCR2, 2);   // CS22 = 0*/
    #endif
                break;
        case 4:  // fclk/256
	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                clearBit(TCCR0B, 0);  // CS00 = 0
                clearBit(TCCR0B, 1);  // CS01 = 0
                setBit(TCCR0B, 2);    // CS02 = 1
    #elif  defined (__AVR_ATmega8__)
                /*clearBit(TCCR2, 0);   // CS20 = 0
                clearBit(TCCR2, 1);   // CS21 = 0
                setBit(TCCR2, 2);     // CS22 = 1*/
    #endif
                break;
        case 5:  // fclk/1024
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
                setBit(TCCR0B, 0);    // CS00 = 1
                clearBit(TCCR0B, 1);  // CS01 = 0
                setBit(TCCR0B, 2);    // CS02 = 1
    #elif  defined (__AVR_ATmega8__)
                /*setBit(TCCR2, 0);     // CS20 = 1
                clearBit(TCCR2, 1);   // CS21 = 0
                setBit(TCCR2, 2);     // CS22 = 1*/
    #endif
                break;
    }    
}

/**
 * Detiene el Timer0.
 **/
void stopTimer0(void)
{
   	#if    defined (__AVR_ATmega88__)  || defined (__AVR_ATmega88A__)  || defined (__AVR_ATmega88P__) ||\
	       defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) ||                               \
		   defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
        TCCR0B &= 0b11111000;   // CS00 = CS01 = CS02 = 0
        //clearBit(TCCR2B, 0);  // CS20 = 0
        //clearBit(TCCR2B, 1);  // CS21 = 0
        //clearBit(TCCR2B, 2);  // CS22 = 0
    #elif  defined (__AVR_ATmega8__)
        // completar...2
        //TCCR2 &= 0b11111000;  // CS20 = CS21 = CS22 = 0
        //clearBit(TCCR2, 0);  // CS20 = 0
        //clearBit(TCCR2, 1);  // CS21 = 0
        //clearBit(TCCR2, 2);  // CS22 = 0
    #endif
}

/**
 * @internal
 * Timer 0 overflow interrupt
 */
ISR(TIMER0_OVF_vect)
{
    Timer0.isr();
}
