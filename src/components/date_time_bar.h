#ifndef DATE_TIME_BAR_H
#define DATE_TIME_BAR_H

#include <stdint.h>

typedef struct DateTimeBar {
    uint8_t x;
    uint8_t y;
    uint8_t width;
} DateTimeBar;

extern const DateTimeBar default_date_time_bar;

inline void update_date_time_bar(DateTimeBar* date_time_bar); 
inline void init_date_time_bar(DateTimeBar* date_time_bar);

#endif
