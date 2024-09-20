#ifndef DATE_TIME
#define DATE_TIME

#include <stdint.h>

#define SECONDS_MAX 59
#define MINUTES_MAX 59
#define HOURS_MAX 23

typedef struct Date {
    uint8_t days;
    uint8_t months;
    uint8_t years;
} Date;

typedef struct Time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} Time;

typedef struct DateTime {
    Date date;
    Time time;
} DateTime;

extern DateTime date_time;
extern uint8_t date_time_changed;

void update_date_time(void);

#endif
