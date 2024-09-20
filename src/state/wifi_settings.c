#include "wifi_settings.h"
#include "../components/text_animation.h"
#include "../components/date_time_bar.h"
#include "../core/print.h"
#include <stdint.h>


TextAnimation scanning_animation_1 = {
    .x = 1,
    .y = 5,
    .frames_count = 4,
    .frame_length = 20,
    .frames = {
        "Scanning      ",
        "Scanning .    ",
        "Scanning . .  ",
        "Scanning . . .",
    }
};

TextAnimation scanning_animation_2 = {
    .props = 
        TEXT_ANIMATION_MIRROR |
        TEXT_ANIMATION_EASE |
        (TEXT_ANIMATION_SPEED & 14), // max 15
    .x = 1,
    .y = 8,
    .frame_length = 0,
    .frames_count = 19,
    .current_frame = 0,
    .timer = 0,
    .frames = {
        "Scanning           ",
        "Scannin g          ",
        "Scanni n g         ",
        "Scann i n g        ",
        "Scan n i n g       ",
        "Sca n n i n g      ",
        "Sc a n n i n g     ",
        "S c a n n i n g    ",
        " S c a n n i n g   ",
        "  S c a n n i n g  ",
        "  S c a n n i n g ",
        "   S c a n n i n g",
        "    S c a n n i ng",
        "     S c a n n ing",
        "      S c a n ning",
        "       S c a nning",
        "        S c anning",
        "         S canning",
        "          Scanning",
    }
};

TextAnimation scanning_animation_3 = {
    .props = 
        TEXT_ANIMATION_EASE |
        (TEXT_ANIMATION_SPEED & 15), // max 15
    .x = 1,
    .y = 11,
    .frames_count = 29,
    .frames = {
        "Scanning           ",
        "Scanning           ",
        "Scanning           ",
        "Scannin g          ",
        "Scanni n g         ",
        "Scann i n g        ",
        "Scan n i n g       ",
        "Sca n n i n g.     ",
        "Sc a n n i n g     ",
        "S c a n n i n g    ",
        " S c a n n i n g   ",
        "  S c a n n i n g  ",
        "   S c a n n i n g ",
        "    S c a n n i n g",
        "g    S c a n n i n ",
        " g    S c a n n i n",
        "n g    S c a n n i ",
        " n g    S c a n n i",
        "i n g    S c a n n ",
        " i n g    S c a n n",
        "n i n g    S c a n ",
        " n i n g    S c a n",
        "n n i ng     S c a ",
        " n n ing      S c a",
        "a n ning       S c ",
        " a nning        S c",
        "c anning         S ",
        " canning          S",
        "Scanning           ",
    }
};


TextAnimation scanning_animation_4 = {
    .x = 1,
    .y = 14,
    .frames_count = 8,
    .frame_length = 5,
    .frames = {
        "Sca     ",
        " can    ",
        "  ann   ",
        "   nni  ",
        "    nin ",
        "     ing",
        "S     ng",
        "Sc     g",
    }
};

void init_wifi_settings(void) {
    DISPLAY_ON;
    SHOW_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    init_print();
    init_date_time_bar(&default_date_time_bar);
    init_text_animation(&scanning_animation_1);
    init_text_animation(&scanning_animation_2);
    init_text_animation(&scanning_animation_3);
    init_text_animation(&scanning_animation_4);

    print_xy("------------------", 1, 2);
    print_xy("Wifi Settings", 1, 3);
    print_xy("------------------", 1, 4);
}

WifiState wifi_state;

void update_wifi_settings(void) {
    update_date_time_bar(&default_date_time_bar);
    update_text_animation(&scanning_animation_1);
    update_text_animation(&scanning_animation_2);
    update_text_animation(&scanning_animation_3);
    update_text_animation(&scanning_animation_4);
}

