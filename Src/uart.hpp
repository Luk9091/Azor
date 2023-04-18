#ifndef __UART__HPP
#define __UART__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <stddef.h>
#include <stdlib.h>

#define BT_PIN_num 1
#define BT_DDR  DDRD
#define BT_PORT PORTD
#define BT_PIN  PIND

#define UART_ENABLE_INTERRUPT_RX UCSRB |= (1 << RXCIE)
#define UART_DISABLE_INTERRUPT_RX UCSRB &= ~(1 << RXCIE)

#define UART_ENABLE_INTERRUPT_TX UCSRB |= (1 << TXCIE)
#define UART_DISABLE_INTERRUPT_TX UCSRB &= ~(1 << TXCIE)


extern char string[16];
extern uint8_t readSize;

uint16_t find_int(uint8_t count = 0);

void UART_Init(uint16_t baud, bool enableEchoInterrupt = false, bool run = true);
void BT_enable(bool run);

void UART_print_char(uint8_t c);
void UART_print(const char *str);
void UART_println(const char *str);

void UART_print(uint16_t value, uint8_t base = 10);
void UART_println(uint16_t value, uint8_t base = 10);

uint8_t UART_read_char();
uint8_t UART_read(char *buf, uint8_t buf_size = 255, char terminator = '\n');


#endif