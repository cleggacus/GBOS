#include "notepad.h"
#include "../core/keyboard.h"
#include "../components/date_time_bar.h"
#include "state.h"
#include "../core/print.h"

#define NOTEPAD_INPUT_BUFFER_SIZE 19

char notepad_input_buffer[NOTEPAD_INPUT_BUFFER_SIZE] = "                  ";

void show_notepad_keyboard(void) {
    keyboard.props = KEYBOARD_NUMBER | KEYBOARD_ALPHABET;
    keyboard.buffer = notepad_input_buffer;
    keyboard.buffer_size = NOTEPAD_INPUT_BUFFER_SIZE;
    keyboard.empty_char = ' ';
    keyboard.validator = 0;
    keyboard.cursor = 0;
    init_keyboard();
}

void render_notepad(void) {
    print_xy(notepad_input_buffer, 1, 6);
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

    uint8_t update = update_keyboard();

    if(update) {
        render_notepad();
    }

    if(update == KEYBOARD_UPDATE_COMPLETE) {
        next_state = STATE_START_MENU;
    }
}
