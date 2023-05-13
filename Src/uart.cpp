#include "uart.hpp"

char string[16];
char temp_str[8];
uint8_t readSize = 0;

#define DEC 10
#define BIN 2
#define HEX 16

ISR(USART_RXC_vect){
    readSize = UART_read(string, 16);
    // UART_print("read\n");
}

uint16_t find_int(uint8_t count){
    int16_t data = 0;
    uint8_t dataFormat = DEC;
    int8_t negative = 1;

    for(uint8_t i = 0; i < 16; i++){
        if(string[i] >= '0' && string[i] <= '9'){
            data *= dataFormat;
            data += string[i] - '0';
            if(string[i+1] == ' '){
                if(count == 0)
                    break;
                --count;
                data = 0;
                dataFormat = DEC;
            }
        } else if(dataFormat == HEX && (string[i] >= 'a' && string[i] <= 'f')) {
            data *= dataFormat;
            data += 10 + string[i] - 'a';
            if(string[i+1] == ' '){
                if(count == 0)
                    break;
                --count;
                data = 0;
                dataFormat = DEC;
            }
        } else if(string[i] == 'x' && string[i-1] == '0'){
            dataFormat = HEX;
        } else if(string[i] == 'b' && string[i-1] == '0'){
            dataFormat = BIN;
        } else if(string[i] == '-'){
            negative = -1;
        }
    }
    data = data * negative;
    return data;
}


void UART_Init(bool enableEchoInterrupt, bool run){
    // BT_DDR |= 1 << BT_PIN_num;
    // if(run)
    //     BT_PORT |= 1 << BT_PIN_num;

    UCSRA |= (1 << U2X);

    UBRRH = (F_CPU/8/BAUDRATE-1) >> 8;
    UBRRL = (F_CPU/8/BAUDRATE-1);

    UCSRB |= (1<<RXEN) | (1<<TXEN);

    if(enableEchoInterrupt)
        UART_ENABLE_INTERRUPT_RX;
}

// void BT_enable(bool run){
//     if(run){
//         BT_PORT |= 1 << BT_PIN_num;
//         UCSRB   |= (1<<RXEN) | (1<<TXEN);
//     }else{
//         BT_PORT &= ~(1 << BT_PIN_num);
//         UCSRB   &= ~((1<<RXEN) | (1<<TXEN));
//     }
// }


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

void UART_print(int32_t value, uint8_t base){
    itoa(value, temp_str, base);
    if(base == 16){
        UART_print("0x");
        if(value < 16)
            UART_print_char('0');
    }
    UART_print(temp_str);
}

void UART_println(int32_t value, uint8_t base){
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
        if(c == 0x08){
            UART_print_char(' ');
            UART_print_char(c);
            --count;
        } else {
            if(c >= 'A' && c <= 'Z'){
                c += 32;
            }
            *(buf+count) = c;
            ++count;
        }
    }while(c != terminator && count < buf_size);
    for(int8_t i = count; i < sizeof(string); i++){
        string[i] = 0;
    }

    return count;
}