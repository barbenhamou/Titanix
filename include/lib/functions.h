#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

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

/*
    @param c - wanted char to the bash.

    Printing The char to thr screen.
*/
void putch(char_t c);

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



/*
    @param dest - wanted array.
    @param src - original array.
    @param counter - how many to copy.

    Copying memory.
*/
byte_t *memcpy(byte_t *dest, const byte_t *src, uint32_t counter);

void init_real_mode(void);

#endif