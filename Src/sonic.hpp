#ifndef __SONIC__HPP
#define __SONIC__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.hpp"

#define SOUND_V 1715/(F_CPU/1000) //34300/2

#define TRIG_PIN PD4
#define ECHO_PIN PD3


void SONIC_Init();
uint16_t SONIC_measure();





#endif