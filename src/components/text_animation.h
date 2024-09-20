#ifndef TEXT_ANIMATION_H
#define TEXT_ANIMATION_H

#include <stdint.h>


#define TEXT_ANIMATION_GRAVITY(name) 

#define TEXT_ANIMATION_SPEED        0b00001111
#define TEXT_ANIMATION_REVERSE      0b00010000
#define TEXT_ANIMATION_EASE         0b00100000
#define TEXT_ANIMATION_LOOP         0b01000000 // always on atm
#define TEXT_ANIMATION_MIRROR       0b10000000

typedef struct TextAnimation {
    uint8_t props;
    uint8_t x;
    uint8_t y;
    uint8_t current_frame;
    uint8_t timer;
    uint8_t frame_length;
    uint8_t frames_count;
    char* frames[];
} TextAnimation;

void init_text_animation(TextAnimation* text_animation);
void update_text_animation(TextAnimation* text_animation);

#endif
