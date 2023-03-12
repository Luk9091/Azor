#ifndef __TIMER__HPP
#define __TIMER__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void PWM_init(uint8_t run = 1);

void PWM_start();
void PWM_stop();

void PWM_setDuty(uint8_t duty);
void PWM_setPulse(uint8_t duty);


#endif