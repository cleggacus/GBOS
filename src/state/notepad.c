#include "notepad.h"
#include "../core/keyboard.h"
#include "../components/date_time_bar.h"
#include "state.h"
#include "../core/print.h"
#include <stdint.h>
#include <string.h>

#define NOTEPAD_INPUT_BUFFER_SIZE 18

char notepad_input_buffer[NOTEPAD_INPUT_BUFFER_SIZE] = {0};

void show_notepad_keyboard(void) {
    keyboard_state.buffer = notepad_input_buffer;
    keyboard_state.buffer_size = NOTEPAD_INPUT_BUFFER_SIZE;
    keyboard_state.type = 0;
    keyboard_state.cursor_pos = 0;

    init_keyboard();
}

void render_notepad(void) {
    uint8_t len = strlen(notepad_input_buffer);
    print_xy(notepad_input_buffer, 1, 6);

    for(uint8_t i = len; i < NOTEPAD_INPUT_BUFFER_SIZE; i++) {
        printc_xy(' ', 1 + i, 6);
    }
}

void init_notepad(void) {
    DISPLAY_ON;
    SHOW_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    init_print();
    init_date_time_bar(&default_date_time_bar);

    show_notepad_keyboard();

    print_xy("------------------", 1, 2);
    print_xy("Notepad", 1, 3);
    print_xy("------------------", 1, 4);

    render_notepad();
}

void update_notepad(void) {
    update_date_time_bar(&default_date_time_bar);

    uint8_t operation_type = update_keyboard();

    if(operation_type) {
        render_notepad();
    }

    if(operation_type == KEYBOARD_COMPLETE) {
        next_state = STATE_START_MENU;
    }
}
