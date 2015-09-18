#ifndef _UART_H_
#define _UART_H_

/**
 * @file uart.h
 * Cabecera para el manejo de la UART.
 **/
#ifdef __cplusplus
extern "C"
{
#endif

void configUart(uint32_t baud);
void setUartTx(uint8_t data);
uint8_t getUartData(void);
void setUartRxIsr(void (*isr)(uint8_t data));

#ifdef __cplusplus
}
#endif

#endif
