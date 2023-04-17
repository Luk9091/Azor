#include "PWM.hpp"
#include "timer.hpp"
#include "uart.hpp"


void PWM_Init(bool run, uint8_t startDuty){
    DDRB |= 1 << PB3;
    TCCR2 |= (1 << WGM21 | 1 << WGM20) // FAST PWM
        | (1 << COM21 | 0 << COM20) 
        | (PWM_prescaler * run << CS20)      // Prescaler 32
    ;
    
    OCR2 = startDuty;
    // UART_println("PWM run");
}

// void PWM_start(){
//     TCCR2 |= (PWM_prescaler << CS20);
// }

// void PWM_stop(){
//     TCCR2 &= ~(7 << CS20);
// }


// void PWM_setDuty(uint8_t duty){
//     // duty = duty*255/100;
//     OCR2 = duty;
// }


// void PWM_setPulse(uint8_t duty){
//     // OCR2 = duty;
//     PWM_setDuty(duty);
//     PWM_start();
//     _delay_ms(35);
//     PWM_stop();
// }