#ifndef _UART_H_
#define _UART_H_

/**
 * @file uart.h
 * Cabecera para el manejo de la UART.
 **/

void configUart(uint16_t baud);
void setUartTx(uint8_t data);
uint8_t getUartData(void);
void setUartRxIsr(void (*isr)(uint8_t data));

#endif
