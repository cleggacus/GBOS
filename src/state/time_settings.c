#include "time_settings.h"
#include "../components/date_time_bar.h"
#include "../core/date_time.h"
#include "../core/keyboard.h"
#include "state.h"
#include "../core/print.h"

#define TIME_INPUT_BUFFER_SIZE 6

char time_input_buffer[TIME_INPUT_BUFFER_SIZE] = { 
   '0',
   '0',
   '0',
   '0',
   '0',
   '0',
};

void set_time_to_buffer(void) {
    uint8_t hours = 10 * (time_input_buffer[0] - '0') + (time_input_buffer[1] - '0');
    uint8_t minutes = 10 * (time_input_buffer[2] - '0') + (time_input_buffer[3] - '0');
    uint8_t seconds = 10 * (time_input_buffer[4] - '0') + (time_input_buffer[5] - '0');
    date_time.time.hours = hours;
    date_time.time.minutes = minutes;
    date_time.time.seconds = seconds;
}

uint8_t validate_time_buffer(char* buffer) {
    uint8_t hours = 10 * (buffer[0] - '0') + (buffer[1] - '0');
    uint8_t minutes = 10 * (buffer[2] - '0') + (buffer[3] - '0');
    uint8_t seconds = 10 * (buffer[4] - '0') + (buffer[5] - '0');

    return hours < 24 && minutes < 60 && seconds < 60;
}

void show_keyboard(void) {
    keyboard.props = KEYBOARD_NUMBER;
    keyboard.buffer = time_input_buffer;
    keyboard.buffer_size = 6;
    keyboard.empty_char = '0';
    keyboard.validator = validate_time_buffer;
    keyboard.cursor = 0;
    init_keyboard();
}

void render_time_settings(void) {
    current_palette = (keyboard.cursor == 0) << 1;
    uint8_t* addr = printc_xy(time_input_buffer[0], 6, 6);
    current_palette = (keyboard.cursor == 1) << 1;
    addr = printc(time_input_buffer[1], addr);
    current_palette = 0;
    addr = printc(':', addr);
    current_palette = (keyboard.cursor == 2) << 1;
    addr = printc(time_input_buffer[2], addr);
    current_palette = (keyboard.cursor == 3) << 1;
    addr = printc(time_input_buffer[3], addr);
    current_palette = 0;
    addr = printc(':', addr);
    current_palette = (keyboard.cursor == 4) << 1;
    addr = printc(time_input_buffer[4], addr);
    current_palette = (keyboard.cursor == 5) << 1;
    addr = printc(time_input_buffer[5], addr);
    current_palette = 0;
}

void init_time_settings(void) {
    DISPLAY_ON;
    SHOW_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    init_print();
    init_date_time_bar(&default_date_time_bar);


    time_input_buffer[0] = (date_time.time.hours / 10) + '0';
    time_input_buffer[1] = (date_time.time.hours % 10) + '0';
    time_input_buffer[2] = (date_time.time.minutes / 10) + '0';
    time_input_buffer[3] = (date_time.time.minutes % 10) + '0';
    time_input_buffer[4] = (date_time.time.seconds / 10) + '0';
    time_input_buffer[5] = (date_time.time.seconds % 10) + '0';

    show_keyboard();

    print_xy("------------------", 1, 2);
    print_xy("Time Settings", 1, 3);
    print_xy("------------------", 1, 4);

    render_time_settings();
}

void update_time_settings(void) {
    update_date_time_bar(&default_date_time_bar);

    uint8_t update = update_keyboard();

    if(update) {
        render_time_settings();
    }

    if(update == KEYBOARD_UPDATE_COMPLETE) {
        set_time_to_buffer();
        next_state = STATE_SETTINGS;
    }

}
