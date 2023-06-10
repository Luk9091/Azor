#ifndef __SONIC__HPP
#define __SONIC__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.hpp"

#define SOUND_V 1.715F //* 1000 / 1000000UL  //34300/2

#define TRIG_DDR DDRB
#define TRIG_PORT PORTB
#define TRIG_PIN PB6

#define ECHO_DDR DDRD
#define ECHO_PIN PD3


void SONIC_Init();
uint16_t SONIC_measure();





#endif