#ifndef __ENGINE__HPP
#define __ENGINE__HPP

#include <avr/io.h>
#include "compass.hpp"

#define ENGINE_LEFT_UP_PIN     0
#define ENGINE_LEFT_DOWN_PIN   1

#define ENGINE_RIGHT_UP_PIN    4
#define ENGINE_RIGHT_DOWN_PIN  2

#define ENGINE_ENABLE_PIN 7

#define ENGINE_ENABLE() PORTD |= (1 << ENGINE_ENABLE_PIN)
#define ENGINE_DISABLE() PORTD &= ~(1 << ENGINE_ENABLE_PIN)


void ENGINE_Init();
// void ENGINE_enable(bool enable = true);

void move_forward(bool direction = true, bool enable = true);
void move_rotate(int16_t angle);

void move_stop();

void LEFT_forward (bool direction = true);
void RIGHT_forward(bool direction = true);

// void move_left(uint8_t angle : 0);
// void move_right(uint8_t angle : 0);




#endif