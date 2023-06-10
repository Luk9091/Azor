#ifndef __PWM__HPP
#define __PWM__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PWM_prescaler 0b110
#define PWM_ANGLE_OFFSET 15

void PWM_Init(bool run = true, uint8_t startDuty = 40);

#define PWM_start() TCCR2B |= (PWM_prescaler << CS20)
#define PWM_stop()  TCCR2B &= ~(7 <<CS20)


// void PWM_setDuty(uint8_t duty);
#define PWM_setDuty(duty) OCR2A = duty
#define PWM_getDuty() OCR2A
#define PWM_getAngle() ((PWM_getDuty-PWM_ANGLE_OFFSET)*3)
// void PWM_setPulse(uint8_t duty);

void PWM_setAngle(uint8_t angle);


#endif