#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

extern uint8_t input_press;
extern uint8_t input_down;
extern uint8_t input_up;

void update_input(void);

#endif
