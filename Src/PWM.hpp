#ifndef __PWM__HPP
#define __PWM__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PWM_prescaler 3

void PWM_Init(bool run = true, uint8_t startDuty = 40);

#define PWM_start() TCCR2 |= (PWM_prescaler << CS20)
#define PWM_stop()  TCCR2 &= ~(7 <<CS20)


// void PWM_setDuty(uint8_t duty);
#define PWM_setDuty(duty) OCR2 = duty
// void PWM_setPulse(uint8_t duty);


#endif