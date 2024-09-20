#include "settings.h"
#include "state.h"
#include "../components/date_time_bar.h"
#include "../components/menu.h"
#include "../core/print.h"

Menu settings_menu = {
    .x = 1,
    .y = 5,
    .option_count = 3,
    .options = {
        "Wifi",
        "Time",
        "Back",
    }
};

void init_settings(void) {
    DISPLAY_ON;
    SHOW_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    init_print();
    init_menu(&settings_menu );
    init_date_time_bar(&default_date_time_bar);

    print_xy("------------------", 1, 2);
    print_xy("Settings", 1, 3);
    print_xy("------------------", 1, 4);
}

void update_settings(void) {
    update_date_time_bar(&default_date_time_bar);

    if(update_menu(&settings_menu)) {
        switch(settings_menu.current_select) {
            case SETTINGS_WIFI: {
                next_state = STATE_WIFI_SETTINGS;
                break;
            }
            case SETTINGS_TIME: {
                next_state = STATE_TIME_SETTINGS;
                break;
            }
            case SETTINGS_BACK: {
                next_state = STATE_START_MENU;
                break;
            }
        }
    }
}
