#include "date_time_bar.h"
#include "../core/date_time.h"
#include "../core/print.h"
#include <gb/gb.h>

const DateTimeBar default_date_time_bar = {
    .x = 1,
    .y = 1,
    .width = DEVICE_SCREEN_WIDTH - 2
};


inline void render_date(uint8_t x, uint8_t y, uint8_t show_years) {
    // Render date (dd/mm/yy)
    printc_xy((date_time.date.days / 10) + '0', x, y);        // Tens place of days
    printc_xy((date_time.date.days % 10) + '0', x+1, y);        // Units place of days
    printc_xy('/', x+2, y);                                // Slash separator
    printc_xy((date_time.date.months / 10) + '0', x+3, y);      // Tens place of months
    printc_xy((date_time.date.months % 10) + '0', x+4, y);      // Units place of months

    if(show_years) {
        printc_xy('/', x+5, y);                                // Slash separator
        printc_xy(((date_time.date.years / 10) % 10) + '0', x+6, y);// Tens place of years
        printc_xy((date_time.date.years % 10) + '0', x+7, y);       // Units place of years
    }
}

inline void render_time(uint8_t x, uint8_t y, uint8_t show_seconds) {
    // Render time (hh:mm)
    printc_xy((date_time.time.hours / 10) + '0', x, y);   // Tens place of hours
    printc_xy((date_time.time.hours % 10) + '0', x+1, y);   // Units place of hours
    printc_xy(':', x+2, y);                            // Colon separator
    printc_xy((date_time.time.minutes / 10) + '0', x+3, y); // Tens place of minutes
    printc_xy((date_time.time.minutes % 10) + '0', x+4, y); // Units place of minutes
    
    if(show_seconds) {
        printc_xy(':', x+5, y);                            // Colon separator
        printc_xy((date_time.time.seconds / 10) + '0', x+6, y); // Tens place of minutes
        printc_xy((date_time.time.seconds % 10) + '0', x+7, y); // Units place of minutes
    }
}

inline void render_date_time_bar(DateTimeBar* date_time_bar) {
    render_date(date_time_bar->x, date_time_bar->y, 1);
    render_time(date_time_bar->x + date_time_bar->width - 8, date_time_bar->y, 1);
}

inline void init_date_time_bar(DateTimeBar* date_time_bar) {
    render_date_time_bar(date_time_bar);
}

inline void update_date_time_bar(DateTimeBar* date_time_bar) {
    if(date_time_changed) {
        render_date_time_bar(date_time_bar);
    }
}
