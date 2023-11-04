#include "../../include/lib/functions.h"

__attribute__((always_inline)) void inline outb(word_t port, byte_t data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
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