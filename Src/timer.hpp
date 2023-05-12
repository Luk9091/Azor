#ifndef __TIMER__HPP
#define __TIMER__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.hpp"
#define PRESCALER 1

#define TIMER_start()   TCCR1B |= (PRESCALER << CS10)
#define TIMER_start_prescaler_8() TCCR1B |= (0b010 << CS10)
#define TIMER_stop()    TCCR1B &= ~(7 <<CS10)

extern bool TIMER_overflow;

void TIMER_Init();
void TIMER_set(uint8_t limit, bool *overflow = &TIMER_overflow);

void TIMER_wait_ms(uint8_t delay);

uint32_t TIMER_getValue();
uint16_t TIMER_get_us();
// #define TIMER_get_ms() (TIMER_getValue() * (1000/F_CPU))
// #define TIMER_getValue() _TIMER_counter * 65535 + TCNT1


// void COUNTER_Init();
// void COUNTER_clear();
// uint8_t COUNTER_read();


#endif