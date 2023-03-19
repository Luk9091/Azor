#ifndef __TIMER__HPP
#define __TIMER__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.hpp"


void TIMER_Init();
void TIMER_set(uint8_t limit);

uint32_t TIMER_getValue();

#endif