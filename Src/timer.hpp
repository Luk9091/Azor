#ifndef __TIMER__HPP
#define __TIMER__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.hpp"
#define PRESCALER 1

#define TIMER_start()   TCCR1B |= (PRESCALER << CS10)
#define TIMER_stop()    TCCR1B &= ~(7 <<CS10)

void TIMER_Init();
void TIMER_set(uint8_t limit, uint8_t *overflow);

uint32_t TIMER_getValue();
// #define TIMER_getValue() _TIMER_counter * 65535 + TCNT1

#endif