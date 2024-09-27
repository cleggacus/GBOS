#include "input.h"

#include <gb/gb.h>

// public
uint8_t input_press;
uint8_t input_down;
uint8_t input_up;

// private
uint8_t input_down_buffer;
uint8_t input_prev;
uint8_t input_down_timer = 0;

void update_input(void) {
    input_prev = input_press;
    input_press = joypad();
    input_up = input_prev & ~input_press;

    input_down = 0; // reset down

    // input down without delay
    uint8_t input_down_now = input_press & ~input_prev;

    // handle input down with delay for combos 
    input_down_timer--;

    // add current down to buffer
    if(input_down_now) {
        input_down_timer = INPUT_DOWN_DELAY;
        input_down_buffer |= input_down_now;
    }

    // Commit buffer
    if(input_down_timer == 0) {
        input_down = input_down_buffer;
        input_down_buffer = 0;
    }
}
