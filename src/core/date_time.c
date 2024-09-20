#include "date_time.h"
#include <stdint.h>

DateTime date_time;

inline uint8_t is_leap_year(uint8_t year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

inline uint8_t days_in_month(uint8_t month, uint8_t year) {
    switch (month) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return is_leap_year(year) ? 29 : 28;
        default:
            return 31;
    }
}

inline void add_second(DateTime *dt) {
    dt->time.seconds++;
    if (dt->time.seconds == 60) {
        dt->time.seconds = 0;
        dt->time.minutes++;
        if (dt->time.minutes == 60) {
            dt->time.minutes = 0;
            dt->time.hours++;
            if (dt->time.hours == 24) {
                dt->time.hours = 0;
                dt->date.days++;
                if (dt->date.days > days_in_month(dt->date.months, dt->date.years)) {
                    dt->date.days = 1;
                    dt->date.months++;
                    if (dt->date.months == 13) {
                        dt->date.months = 1;
                        dt->date.years++;
                    }
                }
            }
        }
    }
}

uint8_t frames_passed = 0;
uint8_t date_time_changed = 0;

void update_date_time(void) {
    date_time_changed = 0;
    frames_passed++;

    if(frames_passed == 60) {
        frames_passed = 0;
        add_second(&date_time);
        date_time_changed = 1;
    }
}

