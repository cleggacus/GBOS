#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include "../../res/font_reg.h"

#define FONT_REG_START 0
#define FONT_CHAR_OFFSET 32

extern uint8_t current_palette;

inline void init_print(void);

inline uint8_t* printc(char c, uint8_t* addr);
inline uint8_t* printc_xy(char c, uint8_t x, uint8_t y);

inline uint8_t* print(char* c, uint8_t* addr);
inline uint8_t* print_xy(char* c, uint8_t x, uint8_t y);

inline void set_palette_xy(uint8_t palette, uint8_t x, uint8_t y);

#endif
