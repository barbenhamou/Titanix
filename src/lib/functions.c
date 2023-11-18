#include "../../include/lib/functions.h"

///////////////////////////////__PORTS__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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

///////////////////////////////__CONSOLE__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void putch(char_t c) {
    outb(0x3f8, c);
}

///////////////////////////////__MATH__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

uint32_t len(uint32_t n) {
    if (n == 0) return 0;
    return 1 + len(n/10);
}

uint32_t pow(uint32_t base, uint32_t power) {
    if (base == 0) return 0;
    if (power == 0) return 1;
    return base * pow(base, power - 1);
}

///////////////////////////////__MEMORY__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

byte_t *memcpy(byte_t *dest, const byte_t *src, uint32_t bytes) {
    for (int i = 0; i < bytes; ++i) {
        dest[i] = src[i];
    }
    return dest;
}

bool_t memcmp(byte_t *p1, byte_t *p2, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; ++i) {
        if (*p1 != *p2) return FALSE;
        p1++; p2++;
    }
    return TRUE;
}

uint32_t strlen(char_t *p) {
    for (uint32_t i = 0; ; ++i) {
        if (p[i] == '\0') return i;
    }

    return -1;
}

byte_t *memset(byte_t *p, byte_t val, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; ++i) {
        p[i] = val;
    }
    return p;
}

word_t *memsetw(word_t *p, word_t val, uint32_t words) {
    for (uint32_t i = 0; i < words; ++i) {
        p[i] = val;
    }
    return p;
}
