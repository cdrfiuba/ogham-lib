/**
 * Biblioteca de utilidades generales para el Atmega8 y Atmega88.
 **/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "utils.h"
#include <stddef.h>

// Vector de punteros a la función de manejo de la interrupción externa.
static void (*extIntIsr[2])(void);


/**
 * Configura un pin como entrada o salida
 * pin: estructura que define al pin
 * dir: 1 salida, 0 entrada
 * pullup: 1 con pullup, 0 sin pullup
 **/
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

/**
 * Configura el PWM asociado al timer 2 (8 bits).
 * mode: 1 phase correct PWM
 *       3 fast PWM
 * prescaler: divisor para la frecuencia del micro
 *            1 fclk
 *            2 fclk/8
 *            3 fclk/32
 *            4 fclk/64
 *            5 fclk/128
 *            6 fclk/256
 *            7 fclk/1024
 * isr: puntero a la función de manejo de la interrupción que se produce cada
 *      vez que se alcanza el nivel de comparación. La función no debe recibir
 *      ningún parámetro y no debe devolver nada. Ej: void mifunc(void){}
 *      Si es NULL no se generará la interrupción.
 * output: 0 sin salida
 *         1 salida por OC2
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
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            setBit(TIMSK2, 0);  // TOIE2 = 1 timer 2 overflow interrupt enable
        #elif defined (__AVR_ATmega8__)
            setBit(TIMSK, 6);  // TOIE2 = 1 timer 2 overflow interrupt enable
        #endif
        sei();  // habilitar interrupciones globales
    }
    else
    {
        // Deshabilitar interrupción de overflow del timer
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            clearBit(TIMSK2, 0);  // TOIE2 = 0 timer 2 overflow interrupt disable
        #elif defined (__AVR_ATmega8__)
            clearBit(TIMSK, 6);  // TOIE2 = 0 timer 2 overflow interrupt disable
        #endif
    }
    
    switch(mode)
    {
        case 1:  // phase correct PWM
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2A, 0);  // WGM20 = 1
                clearBit(TCCR2A, 1);  // WGM21 = 0
                clearBit(TCCR2B, 3);  // WGM22 = 0
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 6);  // WGM20 = 1
                clearBit(TCCR2, 3);  // WGM21 = 0
            #endif
            break;
        case 3:  // fast PWM
        default:
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2A, 0);  // WGM20 = 1
                setBit(TCCR2A, 1);  // WGM21 = 1
                clearBit(TCCR2B, 3);  // WGM22 = 0
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 6);  // WGM20 = 1
                setBit(TCCR2, 3);  // WGM21 = 1
            #endif
            break;
    }
    
    if (output)  // configuramos el pin de salida
    {
        setBit(DDRB, PIN3);  // pin como salida
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            setBit(TCCR2A, 7);  // COM2A1 = 1
            clearBit(TCCR2A, 6);  // COM2A0 = 0
        #elif defined (__AVR_ATmega8__)
            setBit(TCCR2, 5);  // COM21 = 1
            clearBit(TCCR2, 4);  // COM20 = 0
        #endif
    }
    else  // sin salida por pin
    {
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            clearBit(TCCR2A, 7);  // COM2A1 = 0
            clearBit(TCCR2A, 6);  // COM2A0 = 0
        #elif defined (__AVR_ATmega8__)
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
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2B, 0);  // CS20 = 1
                clearBit(TCCR2B, 1);  // CS21 = 0
                clearBit(TCCR2B, 2);  // CS22 = 0
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);  // CS20 = 1
                clearBit(TCCR2, 1);  // CS21 = 0
                clearBit(TCCR2, 2);  // CS22 = 0
            #endif
            break;
        case 2:  // fclk/8
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                setBit(TCCR2B, 1);  // CS21 = 1
                clearBit(TCCR2B, 2);  // CS22 = 0
            #elif defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);  // CS20 = 0
                setBit(TCCR2, 1);  // CS21 = 1
                clearBit(TCCR2, 2);  // CS22 = 0
            #endif
            break;
        case 3:  // fclk/32
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2B, 0);  // CS20 = 1
                setBit(TCCR2B, 1);  // CS21 = 1
                clearBit(TCCR2B, 2);  // CS22 = 0
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);  // CS20 = 1
                setBit(TCCR2, 1);  // CS21 = 1
                clearBit(TCCR2, 2);  // CS22 = 0
            #endif
            break;
        case 4:  // fclk/64
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                clearBit(TCCR2B, 1);  // CS21 = 0
                setBit(TCCR2B, 2);  // CS22 = 1
            #elif defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);  // CS20 = 0
                clearBit(TCCR2, 1);  // CS21 = 0
                setBit(TCCR2, 2);  // CS22 = 1
            #endif
            break;
        case 5:  // fclk/128
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2B, 0);  // CS20 = 1
                clearBit(TCCR2B, 1);  // CS21 = 0
                setBit(TCCR2B, 2);  // CS22 = 1
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);  // CS20 = 1
                clearBit(TCCR2, 1);  // CS21 = 0
                setBit(TCCR2, 2);  // CS22 = 1
            #endif
            break;
        case 6:  // fclk/256
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                clearBit(TCCR2B, 0);  // CS20 = 0
                setBit(TCCR2B, 1);  // CS21 = 1
                setBit(TCCR2B, 2);  // CS22 = 1
            #elif defined (__AVR_ATmega8__)
                clearBit(TCCR2, 0);  // CS20 = 0
                setBit(TCCR2, 1);  // CS21 = 1
                setBit(TCCR2, 2);  // CS22 = 1
            #endif
            break;
        case 7:  // fclk/1024
            #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
                setBit(TCCR2B, 0);  // CS20 = 1
                setBit(TCCR2B, 1);  // CS21 = 1
                setBit(TCCR2B, 2);  // CS22 = 1
            #elif defined (__AVR_ATmega8__)
                setBit(TCCR2, 0);  // CS20 = 1
                setBit(TCCR2, 1);  // CS21 = 1
                setBit(TCCR2, 2);  // CS22 = 1
            #endif
            break;
    }    
}

/**
 * Detiene el PWM.
 **/
void stopPWM2(void)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
        TCCR2B &= 0b11111000;  // CS20 = CS21 = CS22 = 0
        //clearBit(TCCR2B, 0);  // CS20 = 0
        //clearBit(TCCR2B, 1);  // CS21 = 0
        //clearBit(TCCR2B, 2);  // CS22 = 0
    #elif defined (__AVR_ATmega8__)
        TCCR2 &= 0b11111000;  // CS20 = CS21 = CS22 = 0
        //clearBit(TCCR2, 0);  // CS20 = 0
        //clearBit(TCCR2, 1);  // CS21 = 0
        //clearBit(TCCR2, 2);  // CS22 = 0
    #endif
}

/**
 * Establece el ancho de pulso.
 **/
void setPWM2(uint8_t x)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
        OCR2A = x;
    #elif defined (__AVR_ATmega8__)
        OCR2 = x;
    #endif
}

// Timer 2 overflow interrupt
ISR(TIMER2_OVF_vect)
{
    PWM2.isr();
}

/**
 * Configura una interrupción externa.
 * num: número de interupción 0 o 1
 * sense: tipo de activación para la interrupción
 *        0 nivel bajo
 *        1 cualquier cambio de nivel
 *        2 flanco descendente
 *        3 flanco ascendente
 * isr: puntero a la función de manejo de la interrupción. Si es NULL se
 *      desactiva la interrupción.
 **/
void configExtInt(uint8_t num, uint8_t sense, void (*isr)(void))
{
    num = (num & 0x01);
    uint8_t offset = num*2;
    
    extIntIsr[num] = isr;  // callback pointer
    
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
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
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            setBit(EIMSK, 0+num);  // INTx = 1
        #elif defined (__AVR_ATmega8__)
            setBit(GICR, 6+num);  // INTx = 1
        #endif
        sei(); // habilitar interrupciones globales
    }
    else
    {
        // deshabilitar la interrupción
        #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || (__AVR_ATmega88P__)
            clearBit(EIMSK, 0+num);  // INTx = 0
        #elif defined (__AVR_ATmega8__)
            clearBit(GICR, 6+num);  // INTx = 0
        #endif
    }
}

// External interrupt 0
ISR(INT0_vect)
{
    extIntIsr[0]();
}

// External interrupt 1
ISR(INT1_vect)
{
    extIntIsr[1]();
}
