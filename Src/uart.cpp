#include "uart.hpp"

ISR(USART_RX_vect){
    readSize = UART_read(string, 16);
}

void UART_Init(uint16_t baud){
    baud = F_CPU/8/baud-1;
    UCSRA |= (1 << U2X);

    UBRRH = baud >> 8;
    UBRRL = baud;

    UCSRB |= (1<<RXEN) | (1<<TXEN);

    // Frame: 8bin 0parity 1stop
}


void UART_write_char(uint8_t c){
    while(!(UCSRA & (1 << UDRE)));
    UDR = c;
}

void UART_write(const char *str){
    while(*str != '\0'){
        UART_write_char(*str);
        str++;
    }
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
        *(buf+count) = c;
        ++count;
    }while(c != terminator && count < buf_size);


    return count;
}