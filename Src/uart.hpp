#ifndef __UART__HPP
#define __UART__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <stddef.h>
#include <stdlib.h>

#define BAUDRATE 9600

// #define BT_PIN_num 1
// #define BT_DDR  DDRD
// #define BT_PORT PORTD
// #define BT_PIN  PIND

#define UART_ENABLE_INTERRUPT_RX UCSR0B |= (1 << RXCIE0)
#define UART_DISABLE_INTERRUPT_RX UCSR0B &= ~(1 << RXCIE0)

#define UART_ENABLE_INTERRUPT_TX UCSR0B |= (1 << TXCIE0)
#define UART_DISABLE_INTERRUPT_TX UCSR0B &= ~(1 << TXCIE0)


extern char string[16];
extern uint8_t readSize;

uint16_t find_int(uint8_t count = 0);

void UART_Init(bool enableEchoInterrupt = false, bool run = true);
// void BT_enable(bool run);

void UART_print_char(uint8_t c);
void UART_print(const char *str);
void UART_println(const char *str);

void UART_print(int16_t value, uint8_t base = 10);
void UART_println(int16_t value, uint8_t base = 10);


void UART_print_ulong(uint32_t value, uint8_t base = 16);
void UART_println_ulong(uint32_t value, uint8_t base = 16);

uint8_t UART_read_char();
uint8_t UART_read(char *buf, uint8_t buf_size = 255, char terminator = '\n');


#endif