#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

/*
    Power.
*/
uint32_t pow(uint32_t base, uint32_t power);

/*
    Number of digits
*/
uint32_t len(uint32_t n);

#endif