/*
 * Biblioteca para el manejo de la UART
 * Compatibilidad: Atmega8, Atmega88
 */

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/uart.h"
#include "lib/bitmanip.h"

static void (*isr_rx)(uint8_t data) = NULL;

/**
 * Configura la UART.
 * @param baud Velocidad en bps
 */
void configUart(uint32_t baud)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)

      // Establecemos el prescaler para la velocidad deseada
      uint16_t rate = (F_CPU/16/baud) - 1;
      UBRR0 = rate;
      
      // Mode: asynchronous, no parity, 1 stop bit, 8 data bits, ignore clock polarity
      UCSR0C = 0b00000110;
      
      // RXint enable, RX enable, TX enable, UCSZ02=0 for 8 data bits (see UCSR0C:UCSZ01,UCSZ00)
      UCSR0B = 0b10011000;

    #elif  defined (__AVR_ATmega8__)

      // Establecemos el prescaler para la velocidad deseada
      uint16_t rate = (F_CPU/16/baud) - 1;
      uint8_t hr = highByte(rate);
      uint8_t lr = lowByte(rate);
      
      // En Atmega8 asegurarse de que el bit superior es 0 para escribir en el registro UBRRH
      clearBit(hr, 7);
      UBRRH = hr;
      UBRRL = lr;
      
      // Mode: asynchronous, no parity, 1 stop bit, 8 data bits, ignore clock polarity
      // Note: bit 7 must be 1 when writing the UCSRC on the Atmega8
      UCSRC = 0b10000110;
      
      // RXint enable, RX enable, TX enable, UCSZ02=0 for 8 data bits (see UCSR0C:UCSZ01,UCSZ00)
      UCSRB = 0b10011000;

    #endif
    
    sei();  // habilitar interrupciones globales
}

/**
 * Establece la función de interrupción para recepción.
 * @param isr Puntero a la función que será llamada cuando se reciba un
 *             dato en la UART. Debe admitir un parámetro de 8 bits que
 *             es el dato recibido.
 */
void setUartRxIsr(void (*isr)(uint8_t data))
{
    isr_rx = isr;
}

/**
 * Transmite un byte por la UART.
 * @param data: valor de 8 bits a transmitir
 * @see configUart()
 */
void setUartTx(uint8_t data)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)

      while (isBitClear(UCSR0A, 5));  // esperar hasta que se vacíe el buffer de tx
      
      UDR0 = data;

    #elif  defined (__AVR_ATmega8__)

      while (isBitClear(UCSRA, 5));  // esperar hasta que se vacíe el buffer de tx
      
      UDR = data;

    #endif
}

/**
 * Retorna el último valor recibido en la UART.
 * @return Valor de 8 bits recibido.
 * @see configUart(), setUartRxIsr()
 */
uint8_t getUartData(void)
{
    #if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)

      return UDR0;

    #elif  defined (__AVR_ATmega8__)
    
      return UDR;

    #endif
}

/**
 * @internal
 * Interrupción de recepción de la UART.
 */
#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)

ISR(USART_RX_vect)

#elif  defined (__AVR_ATmega8__)

ISR(USART_RXC_vect)

#endif
{
    // el dato debe ser leído o la interrupción seguirá ocurriendo
    uint8_t data = getUartData();
    
    // si el puntero es válido llamar al callback de usuario
    if (isr_rx != NULL) isr_rx(data);
}
