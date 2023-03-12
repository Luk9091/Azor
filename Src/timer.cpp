#include "timer.hpp"

#define PWM_prescaler 3


// void PWM_init(){
//     TCCR1A 
// }


void PWM_init(uint8_t run){
    TCCR2 |= (1 << WGM21 | 1 << WGM20) // FAST PWM
        | (1 << COM21 | 0 << COM20) 
        | (PWM_prescaler << CS20)      // Prescaler 32
    ;


    OCR2 = 128;
}

void PWM_start(){
    TCCR2 |= (PWM_prescaler << CS20);
}

void PWM_stop(){
    TCCR2 &= ~(7 << CS20);
}


void PWM_setDuty(uint8_t duty){
    // duty = duty*255/100;
    OCR2 = duty;
}


void PWM_setPulse(uint8_t duty){
    OCR2 = duty;
    PWM_start();
    _delay_ms(10);
    PWM_stop();

}