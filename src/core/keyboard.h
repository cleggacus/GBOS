#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../utils/dictionary.h"

typedef enum KeyboardType {
    KEYBOARD_T9_PREDICTIVE,
    KEYBOARD_QWERTY,
    KEYBOARD_NUM,
} KeyboardType;

#define KEYBOARD_TYPE_COUNT (KEYBOARD_NUM + 1)

typedef enum KeyboardOperationType {
    KEYBOARD_NONE = 0,
    KEYBOARD_INSERT_CHAR,
    KEYBOARD_BACKSPACE_WORD,
    KEYBOARD_NEXT_T9_WORD,
    KEYBOARD_PREV_T9_WORD,
    KEYBOARD_NEXT_LETTER,
    KEYBOARD_PREV_LETTER,
    KEYBOARD_UP,
    KEYBOARD_DOWN,
    KEYBOARD_LEFT,
    KEYBOARD_RIGHT,
    KEYBOARD_COMPLETE,
} KeyboardOperationType;

typedef struct KeyboardOperation {
    KeyboardOperationType type;
    char value; // for insert
} KeyboardOperation;

typedef struct KeyboardState {
    // general variables
    KeyboardType type;
    uint8_t cursor_pos;
    char *buffer;
    uint8_t buffer_size;
} KeyboardState;

extern KeyboardState keyboard_state;

void init_keyboard(void);
KeyboardOperationType update_keyboard(void);

#endif
