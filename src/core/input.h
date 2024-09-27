#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#define INPUT_DOWN_DELAY 5

extern uint8_t input_press;
extern uint8_t input_down;
extern uint8_t input_up;

typedef struct InputComboState {
    uint8_t input;
    uint8_t timer;
} InputComboState;

void update_input(void);

#endif
