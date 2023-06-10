#ifndef __ENGINE__HPP
#define __ENGINE__HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "compass.hpp"
#include "accelerometer.hpp"

#define distancePerTic 64 //0.1mm

#define ENGINE_PORT PORTB
#define ENGINE_PIN PINB
#define ENGINE_LEFT_UP_PIN     1
#define ENGINE_LEFT_DOWN_PIN   0

#define ENGINE_RIGHT_UP_PIN    2
#define ENGINE_RIGHT_DOWN_PIN  4

#define ENGINE_PORT_EN PORTD
#define ENGINE_PIN_EN PIND
#define ENGINE_ENABLE_PIN 7

#define ENGINE_ENABLE() PORTD |= (1 << ENGINE_ENABLE_PIN)
#define ENGINE_DISABLE() PORTD &= ~(1 << ENGINE_ENABLE_PIN)


void ENGINE_Init();
// void ENGINE_enable(bool enable = true);

void ENGINE_forward(bool enable = true);
void ENGINE_backward(bool enable = true);
void move_rotate(int16_t angle);
void move_rotateTo(int16_t absAngle);

void ENGINE_stop();

void LEFT_forward (bool direction = true);
void RIGHT_forward(bool direction = true);


void move_forward(int16_t distance = 0);

void move_backward(int16_t distance = 0);



#endif