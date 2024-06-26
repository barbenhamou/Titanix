#pragma once

#ifndef MONITOR_H
#define MONITOR_H
#define VGA_BOARD_PORT 0X3D4
#define VGA_DATA_PORT 0X3D5
#define BUFF_LENGTH 2000

#include "functions.h"

typedef enum Color {
    black = (uint8_t)0,
    blue = (uint8_t)1,
    green = (uint8_t)2,
    cyan = (uint8_t)3,
    red = (uint8_t)4,
    magenta = (uint8_t)5,
    brown = (uint8_t)6,
    light_grey = (uint8_t)7,
    dark_grey = (uint8_t)8,
    light_blue = (uint8_t)9,
    light_green = (uint8_t)10,
    light_cyan = (uint8_t)11,
    light_red = (uint8_t)12,
    light_magenta = (uint8_t)13,
    light_brown = (uint8_t)14,
    white = (uint8_t)15
} Color_t;


/*
    @param c - character to write to the screen.

    Writes a single character out to the screen.
*/
void monitor_put(char_t c);

/*
    Clears the screen.
*/
static void monitor_clear();

/*
    @param c - string to write in the screen.

    Writes a string to the screen, ASCII and null terminated.
*/
void monitor_write(char_t *c);

/*
    Initializes the monitor.
*/
void init_monitor();

/*
    @param n - wanted hexadecimal number.

    Printing a hex to vga.
*/
void monitor_put_hex(uint64_t n);

/*
    @param n - wanted decimal.

    Printing a dec to vga.
*/
void monitor_put_dec(uint32_t n);

void handle_back_space();
#endif