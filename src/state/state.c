#include "state.h"
#include "start_menu.h"
#include "settings.h"
#include "time_settings.h"
#include "wifi_settings.h"
#include "notepad.h"
#include "../core/input.h"
#include <gb/gb.h>

StateType next_state = STATE_START_MENU;
StateType current_state = STATE_BOOT;
StateType previous_state = STATE_BOOT;

const State states[] = {
    { // STATE_BOOT
        .init   = 0, 
        .update = 0 
    }, 
    { // STATE_START_MENU
        .init   = &init_start_menu, 
        .update = &update_start_menu
    }, 
    { // STATE_SETTINGS
        .init   = &init_settings, 
        .update = &update_settings 
    },
    { // STATE_SETTINGS
        .init   = &init_wifi_settings, 
        .update = &update_wifi_settings 
    },
    {
        .init   = &init_time_settings, 
        .update = &update_time_settings 
    },
    {
        .init   = &init_notepad, 
        .update = &update_notepad
    },
};

void update_state(void) {
    if(current_state != previous_state) {
        void (*init)(void) = states[current_state].init;
        if(init) (*init)();
    }

    void (*update)(void) = states[current_state].update;
    if(update) (*update)();

    if(input_down & J_START) {
        next_state = STATE_START_MENU;
    }

    previous_state = current_state;
    current_state = next_state;
}

inline StateType get_current_state(void) {
    return current_state;
}

inline StateType get_previous_state(void) {
    return previous_state;
}

