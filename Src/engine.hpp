#ifndef __ENGINE__HPP
#define __ENGINE__HPP

#include <avr/io.h>



void ENGINE_Init();
void ENGINE_enable(bool enable = true);
// uint16_t calculate_distance();

// Distance in cm
void move_forward(uint16_t distance = 0);

void move_stop();

void LEFT_move(uint16_t distance = 0);
void RIGHT_move(uint16_t distance = 0);

// void move_left(uint8_t angle = 0);
// void move_right(uint8_t angle = 0);




#endif