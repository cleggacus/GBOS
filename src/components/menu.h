#ifndef MENU_H
#define MENU_H

#include <stdint.h>

typedef struct Menu {
    uint8_t x;
    uint8_t y;
    uint8_t current_select;
    uint8_t option_count;
    char* options[];
} Menu;

void init_menu(Menu* menu);
uint8_t update_menu(Menu* menu);

#endif
