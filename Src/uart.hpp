#ifndef __UART__HPP
#define __UART__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define UART_ENABLE_INTERRUPT_RX UCSRB |= (1 << RXCIE)
#define UART_DISABLE_INTERRUPT_RX UCSRB &= ~(1 << RXCIE)

#define UART_ENABLE_INTERRUPT_TX UCSRB |= (1 << TXCIE)
#define UART_DISABLE_INTERRUPT_TX UCSRB &= ~(1 << TXCIE)


extern char string[16];
extern uint8_t readSize;

void UART_Init(uint16_t baud);

void UART_write_char(uint8_t c);
void UART_write(const char *str);

uint8_t UART_read_char();
uint8_t UART_read(char *buf, uint8_t buf_size = 255, char terminator = '\n');


#endif