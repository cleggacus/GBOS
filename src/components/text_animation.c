#include "text_animation.h"
#include "../core/print.h"

inline void set_frame_length(TextAnimation* text_animation) {
    if(text_animation->props & TEXT_ANIMATION_EASE) {
        uint8_t mid_point = text_animation->frames_count / 2;
        uint8_t speed = text_animation->props & TEXT_ANIMATION_SPEED;
        text_animation->frame_length = (text_animation->current_frame > mid_point) ? (text_animation->current_frame - mid_point) : (mid_point - text_animation->current_frame);
        if(text_animation->frame_length == 0) text_animation->frame_length += 1;
        text_animation->frame_length *= 16 - speed;
        text_animation->frame_length >>= 1;
        if(text_animation->frame_length == 0) text_animation->frame_length += 1;
    }
}


inline void render_text_animation(TextAnimation* text_animation) {
    print_xy(text_animation->frames[text_animation->current_frame], text_animation->x, text_animation->y);
}

inline void handle_last_frame_text_animation(TextAnimation* text_animation) {
    if(text_animation->props & TEXT_ANIMATION_MIRROR) {
        if(text_animation->current_frame >= text_animation->frames_count - 1) {
            text_animation->props |= TEXT_ANIMATION_REVERSE;
        } else if(text_animation->current_frame == 0) {
            text_animation->props &= ~TEXT_ANIMATION_REVERSE;
        }
    } else {
        if(text_animation->props & TEXT_ANIMATION_REVERSE) {
            if(text_animation->current_frame == 0) {
                text_animation->current_frame = text_animation->frames_count;
            }
        } else {
            if(text_animation->current_frame >= text_animation->frames_count - 1) {
                text_animation->current_frame = 0;
            }
        }
    }
}

void init_text_animation(TextAnimation* text_animation) {
    text_animation->current_frame = 0;
    text_animation->timer = 0;
    set_frame_length(text_animation);
}

void update_text_animation(TextAnimation* text_animation) {
    if(text_animation->timer == text_animation->frame_length) {
        render_text_animation(text_animation);
        handle_last_frame_text_animation(text_animation);

        if(text_animation->props & TEXT_ANIMATION_REVERSE) {
            text_animation->current_frame--;
        } else {
            text_animation->current_frame++;
        }

        set_frame_length(text_animation);
        text_animation->timer = 0;
    }

    text_animation->timer++;
}

