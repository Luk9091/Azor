#ifndef __SONIC__HPP
#define __SONIC__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SOUND_V 34300

#define TRIG_PIN PD4
#define ECHO_PIN PD3

// #define SONIC_PRESCALER 1


void SONIC_init();
uint16_t SONIC_measure();





#endif