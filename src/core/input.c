#include "input.h"

#include <gb/gb.h>

uint8_t input_press;
uint8_t input_down;
uint8_t input_up;

uint8_t input_prev;

void update_input(void) {
    input_prev = input_press;
    input_press = joypad();

    input_down = input_press & ~input_prev;
    input_up = input_prev & ~input_press;
}

