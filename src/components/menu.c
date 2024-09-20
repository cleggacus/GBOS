#include "menu.h"
#include "../core/input.h"
#include "../core/print.h"
#include <stdint.h>

void render_menu(Menu* menu) {
    for(uint8_t i = 0; i < menu->option_count; i++) {
        if(i == menu->current_select) {
            current_palette = 0;
            uint8_t* addr = printc_xy(' ', menu->x, menu->y + i);
            print(menu->options[i], addr);
        } else {
            current_palette = 1;
            uint8_t* addr = print_xy(menu->options[i], menu->x, menu->y + i);
            printc(' ', addr);
        }
    }

    current_palette = 0;
}

void init_menu(Menu* menu) {
    menu->current_select = 0;
    render_menu(menu);
}

uint8_t update_menu(Menu* menu) {
    if((input_down & J_DOWN) && menu->current_select < menu->option_count - 1) {
        menu->current_select++;
        render_menu(menu);
    } else if((input_down & J_UP) && menu->current_select > 0) {
        menu->current_select--;
        render_menu(menu);
    }

    return input_down & J_A;
}
