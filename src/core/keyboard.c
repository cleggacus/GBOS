#include "keyboard.h"
#include "input.h"
#include "../utils/t9.h"
#include "print.h"

Keyboard keyboard;

void render_keyboard(void) {
    uint8_t alphabet_palette = keyboard.mode;
    uint8_t number_palette = 1 - keyboard.mode;

    if(keyboard.props & KEYBOARD_NUMBER) {
        current_palette = number_palette;
        uint8_t* addr = printc_xy('1', 2, 9);
        addr = printc('2', addr+6);
        addr = printc('3', addr+7);
        addr = printc('4', addr+80);
        addr = printc('5', addr+6);
        addr = printc('6', addr+7);
        addr = printc('7', addr+80);
        addr = printc('8', addr+6);
        addr = printc('9', addr+7);
    }

    if(keyboard.props & KEYBOARD_ALPHABET) {
        current_palette = alphabet_palette;
        uint8_t* addr = print_xy("+++", 1, 10);
        addr = print("abc", addr+4);
        addr = print("def", addr+5);
        addr = print("ghi", addr+78);
        addr = print("jkl", addr+4);
        addr = print("mno", addr+5);
        addr = print("pqrs", addr+78);
        addr = print("tuv", addr+3);
        addr = print("wxyz", addr+4);
    }

    current_palette = 0;
}

void correct_mode(void) {
    if(keyboard.mode > PUNCTUATION_MODE) {
        keyboard.mode = ALPHABET_MODE;
    }

    if(!(keyboard.props & KEYBOARD_ALPHABET) && keyboard.mode == ALPHABET_MODE) {
        keyboard.mode = NUMBER_MODE;
    }

    if(!(keyboard.props & KEYBOARD_NUMBER) && keyboard.mode == NUMBER_MODE) {
        keyboard.mode = PUNCTUATION_MODE;
    }

    if(!(keyboard.props & KEYBOARD_PUNCTUATION) && keyboard.mode == PUNCTUATION_MODE) {
        keyboard.mode = ALPHABET_MODE;
    }
}

void init_keyboard(void) {
    render_keyboard();
}

#define KEYBOARD_COMBO_DELAY 5

uint8_t combo_timer = 8;
uint8_t combo_input = 0;

uint8_t update_keyboard(void) {
    KeyboardUpdate update = KEYBOARD_UPDATE_NONE;

    if(keyboard.props == 0) return update;

    correct_mode();

    combo_timer--;

    if(input_down) {
        combo_timer = KEYBOARD_COMBO_DELAY;
        combo_input |= input_down;
    }

    if(combo_timer != 0) return update;

    char val = 0;

    if(combo_input & J_SELECT) {
        update = KEYBOARD_UPDATE_CYCLE_MODE;
    }

    switch(keyboard.mode) {
        case ALPHABET_MODE:
        case NUMBER_MODE: {
            {
                switch(combo_input) {
                    case J_A: {
                        update = KEYBOARD_UPDATE_COMPLETE;
                        break;
                    }
                    case J_A | J_LEFT: {
                        update = KEYBOARD_UPDATE_BACKWARD;
                        break;
                    }
                    case J_A | J_RIGHT: {
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_B | J_LEFT: {
                        val = keyboard.empty_char;
                        update = KEYBOARD_UPDATE_BACKWARD;
                        break;
                    }
                    case J_B | J_RIGHT: {
                        val = keyboard.empty_char;
                        update = KEYBOARD_UPDATE_NEXT;
                        break;
                    }
                    case J_LEFT | J_UP: {
                        val = '1';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_UP: {
                        val = '2';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_RIGHT | J_UP: {
                        val = '3';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_LEFT: {
                        val = '4';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_B: {
                        val = '5';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_RIGHT: {
                        val = '6';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_LEFT | J_DOWN: {
                        val = '7';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_DOWN: {
                        val = '8';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                    case J_RIGHT | J_DOWN: {
                        val = '9';
                        update = KEYBOARD_UPDATE_FORWARD;
                        break;
                    }
                }
            }
            break;
        }
    }

    if(update) {
        if(update == KEYBOARD_UPDATE_NEXT && keyboard.mode == ALPHABET_MODE) {
            uint8_t size = 0;

            while(size < keyboard.cursor && keyboard.buffer[keyboard.cursor - size - 1] != keyboard.empty_char) {
                size++;
            }

            t9_algorithm(keyboard.buffer + keyboard.cursor - size, size);
        }

        if(update == KEYBOARD_UPDATE_FORWARD || update == KEYBOARD_UPDATE_NEXT) {
            if(keyboard.cursor < keyboard.buffer_size) {
                char old_val = keyboard.buffer[keyboard.cursor];

                if(val) {
                    keyboard.buffer[keyboard.cursor] = val;
                }

                if(keyboard.validator == 0 || (*keyboard.validator)(keyboard.buffer)) {
                    keyboard.cursor++;
                } else {
                    keyboard.buffer[keyboard.cursor] = old_val;
                }
            } else {
                keyboard.cursor = keyboard.buffer_size;
            }
        } else if(update == KEYBOARD_UPDATE_BACKWARD) {
            if(keyboard.cursor > 0) {
                keyboard.cursor--;

                if(val) {
                    keyboard.buffer[keyboard.cursor] = val;
                }
            } else {
                keyboard.cursor = 0;
            }
        } else if(update == KEYBOARD_UPDATE_CYCLE_MODE) {
            keyboard.mode++;
            correct_mode();
        } 

        render_keyboard();
    }

    combo_input = 0;

    return update;
}
