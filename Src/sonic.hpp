#ifndef __SONIC__HPP
#define __SONIC__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.hpp"

#define SOUND_V 1.715F //* 1000 / 1000000UL  //34300/2

#define TRIG_PIN PD5
#define ECHO_PIN PD3

extern bool SONIC_run;

void SONIC_Init(bool run = true);
uint16_t SONIC_measure();





#endif