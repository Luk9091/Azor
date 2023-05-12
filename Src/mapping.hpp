#ifndef __MAPPING__HPP
#define __MAPPING__HPP

#include <avr/io.h>
#include "uart.hpp"
#include "accelerometer.hpp"
#include "timer.hpp"
#include "engine.hpp"
#include "sonic.hpp"
#include "PWM.hpp"
#include "eeprom.hpp"

void runAndMeasure(int16_t destination = 0);

void map(int16_t addressToStore = -1);


#endif