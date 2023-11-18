#include "../../include/lib/functions.h"

__attribute__((always_inline)) void inline outb(word_t port, byte_t data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

__attribute__((always_inline)) uint8_t inline inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

__attribute__((always_inline)) uint16_t inline inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


void putch(char_t c) {
    outb(0x3f8, c);
}

uint32_t len(uint32_t n) {
    if (n == 0) return 0;
    return 1 + len(n/10);
}

uint32_t pow(uint32_t base, uint32_t power) {
    if (base == 0) return 0;
    if (power == 0) return 1;
    return base * pow(base, power - 1);
}

byte_t *memcpy(byte_t *dest, const byte_t *src, uint32_t counter) {
    for (int i = 0; i < counter; ++i) {
        dest[i] = src[i];
    }
    return dest;
}