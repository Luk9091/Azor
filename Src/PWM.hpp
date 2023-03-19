#ifndef __PWM__HPP
#define __PWM__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void PWM_Init(bool run = true, uint8_t startDuty = 40);

void PWM_start();
void PWM_stop();

void PWM_setDuty(uint8_t duty);
void PWM_setPulse(uint8_t duty);


#endif