#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

///////////////////////////////__PORTS__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*
    @param port - port to put data in.
    @param value - the data to put in.

    Putting data in port.
*/
void outb(uint16_t port, uint8_t value);

/*
    @param port - port to read data from.

    Reading data(bytes) from port.
*/
uint8_t inb(uint16_t port);

/*
    @param port - port to read data from.

    Reading data(words) from port.
*/
uint16_t inw(uint16_t port);

///////////////////////////////__CONSOLE__\\\\\\\\\\\\\\\\\\\\\\\\\\\\


/*
    @param c - wanted char to the bash.

    Printing The char to thr screen.
*/
void putch(char_t c);

///////////////////////////////__MATH__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*
    @param base - the number to raise.
    @param power - obvious

    Power.
*/
uint32_t pow(uint32_t base, uint32_t power);

/*
    @param n - the number to check.

    Number of digits
*/
uint32_t len(uint32_t n);

///////////////////////////////__MEMORY__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*
    @param dest - wanted array.
    @param src - original array.
    @param bytes - how many to copy.

    Copying memory.
*/
byte_t *memcpy(byte_t *dest, const byte_t *src, uint32_t bytes);

/*
    @param p1 - pointer to check.
    @param p2 - another pointer to check.
    @param bytes - how many bytes to check.

    Comparing two pointers, bytes amount.
*/
bool_t memcmp(byte_t *p1, byte_t *p2, uint32_t bytes);

/*
    @param p - wanted string.

    Length of a string.
*/
uint32_t strlen(char_t *p);

/*
    @param p - pointer to change.
    @param val - value to put.
    @param bytes - how many bytes to change.

    Changing the amount of bytes to value.
*/
byte_t *memset(byte_t *p, byte_t val, uint32_t bytes);

/*
    @param p - pointer to change.
    @param val - value to put.
    @param words - how many words to change.

    Changing the amount of words to change.
*/
word_t *memsetw(word_t *p, word_t val, uint32_t words);

///////////////////////////////__BOOT__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*
    Initializing real mode.
*/
void init_real_mode(void);

#endif