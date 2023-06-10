#include "PWM.hpp"
#include "timer.hpp"
#include "uart.hpp"


void PWM_Init(bool run, uint8_t startDuty){
    DDRB |= 1 << PB3;
    TCCR2A |= (1 << WGM21 | 1 << WGM20) //  PWM
        | (1 << COM2A1 | 0 << COM2A0)
    ;
    if (run)
        TCCR2B |= (PWM_prescaler << CS20);      // Prescaler

    PWM_setDuty(startDuty);
}

void PWM_setAngle(uint8_t angle){
    PWM_setDuty(uint8_t((angle + 1) * 0.3333F) + PWM_ANGLE_OFFSET);
}