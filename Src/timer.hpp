#ifndef __TIMER__HPP
#define __TIMER__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.hpp"
#define PRESCALER 1

#define TIMER_start()   TCCR1B |= (PRESCALER << CS10)
// #define TIMER_start_prescaler_8() TCCR1B |= (0b010 << CS10)
#define TIMER_stop()    TCCR1B &= ~(7 <<CS10)


extern bool TIMER_overflow;
extern uint16_t velocity;

void TIMER_Init();
void TIMER_set(uint8_t limit, bool *overflow = &TIMER_overflow);
void TIMER_clear();

void TIMER_wait_ms(uint16_t delay);

uint32_t TIMER_getValue();
// #define TIMER_get_ms() (TIMER_getValue() * (1000/F_CPU))
// #define TIMER_getValue() _TIMER_counter * 65535 + TCNT1


#define COUNTER_DDR DDRD
#define COUNTER_PORT PORTD
#define COUNTER_PIN PD4

// Prawdopodobnie na 1 jednostkę przypada około 0.25cm
void COUNTER_Init();
#define COUNTER_start() TCCR0B |= (6 << CS00) // count falling edge
#define COUNTER_stop()  TCCR0B &=~(7 << CS00)
// Prawdopodobnie do wymiany, ale na razie zostawmy zobaczę czy będzie się przepełniać?
#define COUNTER_clear() TCNT0 = 0
#define COUNTER_read() TCNT0
#define COUNTER_compValue OCR0A


#endif