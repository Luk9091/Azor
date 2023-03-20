#ifndef __ENGINE__HPP
#define __ENGINE__HPP

#include <avr/io.h>



void ENGINE_Init();
void ENGINE_enable(bool enable = true);
// uint16_t calculate_distance();

// Distance in cm
void move_forward(int8_t distance = 0);
void move_rotate(int8_t angle);

void move_stop();

void LEFT_forward(int8_t distance = 0);
void RIGHT_forward(int8_t distance = 0);

// void move_left(uint8_t angle = 0);
// void move_right(uint8_t angle = 0);




#endif