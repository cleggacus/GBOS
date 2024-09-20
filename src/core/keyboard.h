#ifndef KEYBAORD_H
#define KEYBAORD_H

#include <stdint.h>

#define KEYBOARD_ALPHABET       0b00000001
#define KEYBOARD_NUMBER         0b00000010
#define KEYBOARD_PUNCTUATION    0b00000100

typedef enum KeyboardUpdate {
    KEYBOARD_UPDATE_NONE = 0,
    KEYBOARD_UPDATE,
    KEYBOARD_UPDATE_NEXT,
    KEYBOARD_UPDATE_FORWARD,
    KEYBOARD_UPDATE_BACKWARD,
    KEYBOARD_UPDATE_COMPLETE,
    KEYBOARD_UPDATE_CYCLE_MODE,
} KeyboardUpdate;

typedef enum KeyboardMode {
    ALPHABET_MODE,
    NUMBER_MODE,
    PUNCTUATION_MODE,
} KeyboardMode;

typedef struct Keyboard {
    uint8_t props;
    KeyboardMode mode;
    uint8_t cursor;
    char empty_char;
    uint8_t buffer_size;
    uint8_t (*validator)(char*);
    char* buffer;
} Keyboard;

extern Keyboard keyboard;

void init_keyboard(void);
uint8_t update_keyboard(void);

#endif
