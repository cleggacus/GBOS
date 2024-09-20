#ifndef STATE_H
#define STATE_H

#define STATE_COUNT 3

typedef enum StateType {
    STATE_BOOT,
    STATE_START_MENU,
    STATE_SETTINGS,
    STATE_WIFI_SETTINGS,
    STATE_TIME_SETTINGS,
    STATE_NOTEPAD,
} StateType;

typedef struct State {
    void (*init)(void);
    void (*update)(void);
} State;

extern StateType next_state;

void update_state(void);
StateType get_current_state(void);
StateType get_previous_state(void);

#endif

