#include "uart.hpp"

char string[16];
char temp_str[8];
uint8_t readSize = 0;

ISR(USART_RXC_vect){
    cli();
    readSize = UART_read(string, 16);
    // UART_print("read\n");
    sei();
}

void UART_Init(uint16_t baud, bool enableEchoInterrupt){
    baud = F_CPU/8/baud-1;
    UCSRA |= (1 << U2X);

    UBRRH = baud >> 8;
    UBRRL = baud;

    UCSRB |= (1<<RXEN) | (1<<TXEN);

    if(enableEchoInterrupt)
        UART_ENABLE_INTERRUPT_RX;

    // Frame: 8bin 0parity 1stop
}


void UART_print_char(uint8_t c){
    while(!(UCSRA & (1 << UDRE)));
    UDR = c;
}

void UART_print(const char *str){
    while(*str != '\0'){
        UART_print_char(*str);
        str++;
    }
}

void UART_println(const char *str){
    UART_print(str);
    UART_print_char('\n');
}

void UART_print(uint16_t value, uint8_t base){
    itoa(value, temp_str, base);
    if(base == 16){
        UART_print("0x");
        if(value < 16)
            UART_print_char('0');
    }
    UART_print(temp_str);
}

void UART_println(uint16_t value, uint8_t base){
    UART_print(value, base);
    UART_print_char('\n');
}


uint8_t UART_read_char(){
    while(!(UCSRA & (1 << RXC)));
    return UDR;
}

uint8_t UART_read(char *buf, uint8_t buf_size, char terminator){
    uint8_t count = 0;
    char c = 0;

    if(buf_size <= 0 || buf == NULL)
        return 0;
    
    do{
        c = UART_read_char();
        UART_print_char(c); // XDDDD przeniesienie z lini 81 tu zaoszczędza 6 bajtów!
        if(c >= 'A' && c <= 'Z'){
            c += 32;
        }
        *(buf+count) = c;
        ++count;
    }while(c != terminator && count < buf_size);


    return count;
}