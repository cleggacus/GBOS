#include "print.h"

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/hardware.h>
#include <stdint.h>
#include <string.h>

uint8_t print_tiles[DEVICE_SCREEN_WIDTH * DEVICE_SCREEN_HEIGHT] = {0};
uint8_t print_attrs[DEVICE_SCREEN_WIDTH * DEVICE_SCREEN_HEIGHT] = {0};

const palette_color_t palettes[8][4] = {
    {
        RGB8(255,255,255),
        RGB8(170,170,170),
        RGB8( 85, 85, 85),
        RGB8(  0,  0,  0) // black
    },
    {
        RGB8(255,255,255),
        RGB8( 205, 205, 205),
        RGB8( 155, 155, 155),
        RGB8(  105,  105,105) // red
    },
    {
        RGB8(  0,  0,  0),
        RGB8( 85, 85, 85),
        RGB8(170,170,170),
        RGB8(255,255,255),
    },
    {
        RGB8(255,255,255),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,  0),
        RGB8(255,255,  0) // yellow
    },
    {
        RGB8(255,255,255),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,  0),
        RGB8(  0,255,  0) // green
    },
    {
        RGB8(255,255,255),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,255) // Blue
    },
    {
        RGB8(255,255,255),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,  0),
        RGB8( 75,  0,130) // indigo
    },
    {
        RGB8(255,255,255),
        RGB8(  0,  0,  0),
        RGB8(  0,  0,  0),
        RGB8(148,  0,211) // vilot
    },
};

uint8_t current_palette = 0;

inline void init_print(void) {
    for(uint8_t i = 0; i < 8; i++) {
        set_bkg_palette(i, 1, palettes[i]);
    }

    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_data(FONT_REG_START, font_reg_TILE_COUNT, print_attrs); 
    VBK_REG = VBK_TILES;
    set_bkg_data(FONT_REG_START, font_reg_TILE_COUNT, font_reg_tiles); 
    set_bkg_tiles(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, print_tiles);
}

inline uint8_t* printc(char c, uint8_t* addr) {
    VBK_REG = VBK_ATTRIBUTES;
    set_vram_byte(addr, current_palette); // palette
    VBK_REG = VBK_TILES;
    set_vram_byte(addr, c + FONT_REG_START - FONT_CHAR_OFFSET);
    return addr+1;
}

inline uint8_t* printc_xy(char c, uint8_t x, uint8_t y) {
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tile_xy(x, y, current_palette); // palette
    VBK_REG = VBK_TILES;
    return set_bkg_tile_xy(x, y, c + FONT_REG_START - FONT_CHAR_OFFSET) + 1;
}

inline uint8_t* print(char* c, uint8_t* addr) {
    uint8_t length = strlen(c);

    while(*c != '\0') {
        printc(*c, addr);
        c++;
        addr++;
    }
    
    return addr;
}

inline uint8_t* print_xy(char* c, uint8_t x, uint8_t y) {
    uint8_t* addr = printc_xy(*c, x, y);
    return print(c+1, addr);
}
