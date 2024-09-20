#include "start_menu.h"
#include "state.h"
#include "../components/menu.h"
#include "../components/date_time_bar.h"
#include <gb/gb.h>
#include "../core/print.h"

Menu start_menu = {
    .x = 1,
    .y = 3,
    .option_count = 2,
    .options = {
        "Notepad",
        "Settings",
    }
};

void init_start_menu(void) {
    DISPLAY_ON;
    SHOW_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    init_print();
    init_menu(&start_menu);
    init_date_time_bar(&default_date_time_bar);

    print_xy("------------------", 1, 2);
}

void update_start_menu(void) {
    update_date_time_bar(&default_date_time_bar);

    if(update_menu(&start_menu)) {
        switch(start_menu.current_select) {
            case START_MENU_NOTEPAD: {
                next_state = STATE_NOTEPAD;
                break;
            }
            case START_MENU_SETTINGS: {
                next_state = STATE_SETTINGS;
                break;
            }
        }
    }
}

