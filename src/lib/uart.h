#ifndef _UART_H_
#define _UART_H_

/**
 * @file uart.h
 * Cabecera para el manejo de la UART.
 **/

void configUART(uint16_t baud, void (*isr_rx)(uint8_t data));
void uartTx(uint8_t data);
uint8_t getUARTData(void);

#endif
