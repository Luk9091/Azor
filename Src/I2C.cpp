#include "I2C.hpp"



void I2C_Init(){
    DDRC  &= ~(1 << PC5 | 1 <<PC4);
    PORTC &= ~(1 << PC5 | 1 <<PC4);

    TWBR = 0;
    TWSR |= (0 << TWPS1) | (0 << TWPS0);
    TWCR |= (1 << TWEN);
}


void I2C_beginTransition(uint8_t address){
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));

    I2C_write(address);
}

void I2C_endTransition(){
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while ((TWCR & (1 << TWSTO)));
}


void I2C_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 <<TWEN);
    while(!(TWCR & (1 << TWINT)));
}


#if MULTI_READ
uint8_t I2C_read(uint8_t count){
    if(count)
        return I2C_read_AK();
    else
        return I2C_read_NAK();

}

uint8_t I2C_read_AK(){
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_read_NAK() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}
#else

uint8_t I2C_read(){
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

#endif
