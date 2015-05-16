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
    // Establecemos el prescaler para la velocidad deseada
    uint16_t UBRR = (F_CPU/16/baud) - 1;
    
    UBRR0 = UBRR;
    
    // Mode: asynchronous, no parity, 1 stop bit, 8 data bits, ignore clock polarity
    UCSR0C = 0b00000110;
    
    // RXint enable, RX enable, TX enable, UCSZ02=0 for 8 data bits (see UCSR0C:UCSZ01,UCSZ00)
    UCSR0B = 0b10011000;
    
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
    while (isBitClear(UCSR0A, 5));  // esperar hasta que se vacíe el buffer de tx
    
    UDR0 = data;
}

/**
 * Retorna el último valor recibido en la UART.
 * @return Valor de 8 bits recibido.
 * @see configUart(), setUartRxIsr()
 */
uint8_t getUartData(void)
{
    return UDR0;
}

/**
 * @internal
 * Interrupción de recepción de la UART.
 */
ISR(USART_RX_vect)
{
    // el dato debe ser leído o la interrupción seguirá ocurriendo
    uint8_t data = getUartData();
    
    // si el puntero es válido llamar al callback de usuario
    if (isr_rx != NULL) isr_rx(data);
}
