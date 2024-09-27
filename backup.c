#include "input.h"
#include "keyboard.h"
#include "../utils/t9.h"
#include "print.h"
#include <stdint.h>
#include <string.h>
#include <gb/gb.h>

KeyboardState keyboard_state;

uint8_t key_x = 1;
uint8_t key_y = 0;

char keyboard_layout[3][21] = {
    " q w e r t y u i o p",
    "  a s d f g h j k l ",
    "   z x c v b n m    "
};

uint8_t keyboard_bounds[3][2] = {
    {1, 19},
    {2, 18},
    {3, 15},
};

void correct_key_pos(void) {
    if(key_x < keyboard_bounds[key_y][0]) {
        key_x = keyboard_bounds[key_y][0];
    } else if(key_x > keyboard_bounds[key_y][1]) {
        key_x = keyboard_bounds[key_y][1];
    }
}

void render_keyboard(void) {
    switch(keyboard_state.type) {
        case KEYBOARD_T9_PREDICTIVE:
        case KEYBOARD_NUM: {
            uint8_t letter_input = keyboard_state.type == KEYBOARD_T9_PREDICTIVE;

            current_palette = 1 - letter_input;
            print_xy("        abc     def ", 0, DEVICE_SCREEN_HEIGHT - 8);
            print_xy(" gih    jkl     mno ", 0, DEVICE_SCREEN_HEIGHT - 5);
            print_xy(" pqrs   tuv    wxyz ", 0, DEVICE_SCREEN_HEIGHT - 2);

            current_palette = letter_input;
            print_xy("  1      2       3  ", 0, DEVICE_SCREEN_HEIGHT - 9);
            print_xy("  4      5       6  ", 0, DEVICE_SCREEN_HEIGHT - 6);
            print_xy("  7      8       9  ", 0, DEVICE_SCREEN_HEIGHT - 3);

            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 4);
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 7);

            current_palette = 0;
            break;
        }
        case KEYBOARD_QWERTY: {
            current_palette = 1;
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 9);
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 8);
            print_xy(" q w e r t y u i o p", 0, DEVICE_SCREEN_HEIGHT - 7);
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 6);
            print_xy("  a s d f g h j k l ", 0, DEVICE_SCREEN_HEIGHT - 5);
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 4);
            print_xy("   z x c v b n m    ", 0, DEVICE_SCREEN_HEIGHT - 3);
            print_xy("                    ", 0, DEVICE_SCREEN_HEIGHT - 2);
            // set_palette_xy(0, key_x, DEVICE_SCREEN_HEIGHT - 7 + key_y * 2);

            printc_xy('>', key_x-1, DEVICE_SCREEN_HEIGHT - 7 + key_y * 2);
            break;
        }
    }
}

void cycle_keyboard_type(void) {
    keyboard_state.type++;

    if(keyboard_state.type >= KEYBOARD_TYPE_COUNT) {
        keyboard_state.type = 0;
    }

    render_keyboard();
}

void init_keyboard(void) {
    render_keyboard();
}

void update_keyboard_num(KeyboardOperation* operation) {
    switch(input_down) {
        case J_A: {
            operation->type = KEYBOARD_COMPLETE;
            break;
        }
        case J_B | J_LEFT: {
            operation->type = KEYBOARD_BACKSPACE_WORD;
            break;
        }
        case J_B | J_RIGHT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = ' ';
            break;
        }
        case J_LEFT | J_UP: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '1';
            break;
        }
        case J_UP: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '2';
            break;
        }
        case J_RIGHT | J_UP: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '3';
            break;
        }
        case J_LEFT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '4';
            break;
        }
        case J_B: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '5';
            break;
        }
        case J_RIGHT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '6';
            break;
        }
        case J_LEFT | J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '7';
            break;
        }
        case J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '8';
            break;
        }
        case J_RIGHT | J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '9';
            break;
        }
    }
}

void update_keyboard_qwerty(KeyboardOperation* operation) {
    switch(input_down) {
        case J_A: {
            operation->type = KEYBOARD_COMPLETE;
            break;
        }
        case J_B | J_UP: {
            operation->type = KEYBOARD_NEXT_LETTER;
            break;
        }
        case J_B | J_DOWN: {
            operation->type = KEYBOARD_PREV_LETTER;
            break;
        }
        case J_B | J_LEFT: {
            operation->type = KEYBOARD_BACKSPACE_WORD;
            break;
        }
        case J_B | J_RIGHT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = ' ';
            break;
        }
        case J_B: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = keyboard_layout[key_y][key_x];
            break;
        }
        case J_UP: {
            operation->type = KEYBOARD_UP;
            break;
        }
        case J_LEFT: {
            operation->type = KEYBOARD_LEFT;
            break;
        }
        case J_RIGHT: {
            operation->type = KEYBOARD_RIGHT;
            break;
        }
        case J_DOWN: {
            operation->type = KEYBOARD_DOWN;
            break;
        }
    }
}

void update_keyboard_t9_predictive(KeyboardOperation* operation) {
    switch(input_down) {
        case J_A: {
            operation->type = KEYBOARD_COMPLETE;
            break;
        }
        case J_B | J_UP: {
            operation->type = KEYBOARD_PREV_T9_WORD;
            break;
        }
        case J_B | J_DOWN: {
            operation->type = KEYBOARD_NEXT_T9_WORD;
            break;
        }
        case J_B | J_LEFT: {
            operation->type = KEYBOARD_BACKSPACE_WORD;
            break;
        }
        case J_B | J_RIGHT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = ' ';
            break;
        }
        case J_UP: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '2';
            break;
        }
        case J_RIGHT | J_UP: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '3';
            break;
        }
        case J_LEFT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '4';
            break;
        }
        case J_B: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '5';
            break;
        }
        case J_RIGHT: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '6';
            break;
        }
        case J_LEFT | J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '7';
            break;
        }
        case J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '8';
            break;
        }
        case J_RIGHT | J_DOWN: {
            operation->type = KEYBOARD_INSERT_CHAR;
            operation->value = '9';
            break;
        }
    }
}

uint8_t find_prev_word(char* buffer, uint8_t start) {
    if(start == 0) return 0;

    uint8_t cursor = start;

    do {
        cursor--;
    } while(buffer[cursor-1] != ' ' && cursor > 0);

    return cursor;
}

T9Results results = {0};
uint8_t current_result = 0;

uint8_t cycle_timer = 0;

void proccess_operation(KeyboardOperation* operation) {
    switch(operation->type) {
        case KEYBOARD_NONE: {
            break;
        }
        case KEYBOARD_INSERT_CHAR: {
            if(keyboard_state.cursor_pos < keyboard_state.buffer_size) {
                keyboard_state.buffer[keyboard_state.cursor_pos] = operation->value;
                keyboard_state.cursor_pos++;
            }

            if(keyboard_state.type == KEYBOARD_T9_PREDICTIVE && operation->value != ' ') {
                uint8_t prev_word = find_prev_word(keyboard_state.buffer, keyboard_state.cursor_pos);
                uint8_t word_length = keyboard_state.cursor_pos - prev_word;

                results.result_count = 0;
                current_result = 0;
                t9_algorithm(keyboard_state.buffer + prev_word, word_length, &results);

                if(results.result_count > 0) {
                    memcpy(keyboard_state.buffer + prev_word, &results.results[current_result].value, word_length);
                }
            }

            break;
        }
        case KEYBOARD_BACKSPACE_WORD: {
            uint8_t buffer_length = strlen(keyboard_state.buffer);
            uint8_t prev_word = find_prev_word(keyboard_state.buffer, keyboard_state.cursor_pos);
            uint8_t backspace_count = keyboard_state.cursor_pos - prev_word;
            memcpy(keyboard_state.buffer + prev_word, keyboard_state.buffer + keyboard_state.cursor_pos, buffer_length - keyboard_state.cursor_pos);

            for(uint8_t i = buffer_length - backspace_count; i < keyboard_state.cursor_pos; i++) {
                keyboard_state.buffer[i] = '\0';
            }

            keyboard_state.cursor_pos = prev_word;
            break;
        }
        case KEYBOARD_NEXT_T9_WORD: {
            if(results.result_count > 0) {
                uint8_t prev_word = find_prev_word(keyboard_state.buffer, keyboard_state.cursor_pos);
                uint8_t word_length = keyboard_state.cursor_pos - prev_word;

                current_result++;

                if(current_result >= results.result_count) {
                    current_result = 0;
                }

                memcpy(keyboard_state.buffer + prev_word, &results.results[current_result].value, word_length);
            }
            break;
        }
        case KEYBOARD_PREV_T9_WORD: {
            if(results.result_count > 0) {
                uint8_t prev_word = find_prev_word(keyboard_state.buffer, keyboard_state.cursor_pos);
                uint8_t word_length = keyboard_state.cursor_pos - prev_word;

                if(current_result <= 0) {
                    current_result = results.result_count;
                }

                current_result--;

                memcpy(keyboard_state.buffer + prev_word, &results.results[current_result].value, word_length);
            }
            break;
        }
        case KEYBOARD_NEXT_LETTER: {
            if(keyboard_state.cursor_pos > 0) {
                char val = keyboard_state.buffer[keyboard_state.cursor_pos-1];

                uint8_t key = (val - 'a' - (val > 'p') - (val > 'w')) / 3;
                uint8_t letter = 'a' + key * 3 + (key > 5) + (key > 7);
                uint8_t letter_count = 3 + (key == 5 || key == 7);

                keyboard_state.buffer[keyboard_state.cursor_pos-1]++;

                if(keyboard_state.buffer[keyboard_state.cursor_pos-1] - letter >= letter_count) {
                    keyboard_state.buffer[keyboard_state.cursor_pos-1] = letter;
                }
            }
            break;
        }
        case KEYBOARD_PREV_LETTER: {
            if(keyboard_state.cursor_pos > 0) {
                char val = keyboard_state.buffer[keyboard_state.cursor_pos-1];

                uint8_t key = (val - 'a' - (val > 'p') - (val > 'w')) / 3;
                char letter = 'a' + key * 3 + (key > 5) + (key > 7);
                uint8_t letter_count = 3 + (key == 5 || key == 7);

                keyboard_state.buffer[keyboard_state.cursor_pos-1]--;

                if(keyboard_state.buffer[keyboard_state.cursor_pos-1] < letter) {
                    keyboard_state.buffer[keyboard_state.cursor_pos-1] = letter + letter_count - 1;
                }
            }
            break;
        }
        case KEYBOARD_UP: {
            if(key_y > 0) {
                key_y--;
                key_x--;
                correct_key_pos();
                render_keyboard();
            }
            break;
        }
        case KEYBOARD_DOWN: {
            if(key_y < 2) {
                key_y++;
                key_x++;
                correct_key_pos();
                render_keyboard();
            }
            break;
        }
        case KEYBOARD_LEFT: {
            if(key_x < 2) key_x = 2;
            key_x -= 2;
            correct_key_pos();
            render_keyboard();
            break;
        }
        case KEYBOARD_RIGHT: {
            key_x += 2;
            correct_key_pos();
            render_keyboard();
            break;
        }
        case KEYBOARD_COMPLETE: {
            break;
        }
    }
}

KeyboardOperationType update_keyboard(void) {
    KeyboardOperation operation = {0};

    if(input_down == J_SELECT) {
        cycle_keyboard_type();
        return operation.type;
    }

    switch(keyboard_state.type) {
        case KEYBOARD_T9_PREDICTIVE: {
            update_keyboard_t9_predictive(&operation);
            proccess_operation(&operation);
            break;
        }
        case KEYBOARD_QWERTY: {
            update_keyboard_qwerty(&operation);
            proccess_operation(&operation);
            break;
        }
        case KEYBOARD_NUM: {
            update_keyboard_num(&operation);
            proccess_operation(&operation);
            break;
        }
    }


    return operation.type;
}
