#include "../../include/lib/functions.h"

///////////////////////////////__ASM_INSTR__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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

__attribute__((always_inline)) void inline hlt() {
    __asm__ __volatile__("hlt");
}

///////////////////////////////__MATH__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

uint64_t len(uint64_t q, uint64_t n, uint64_t base) {
    if (q == 0) return 1;
    if (n == 0) return 0;
    return 1 + len(q, n/base, base);
}

uint64_t pow(uint64_t base, uint64_t power) {
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

byte_t *memset(byte_t *p, byte_t val, uint64_t bytes) {
    for (uint64_t i = 0; i < bytes; ++i) {
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

///////////////////////////////__CONSOLE__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void putch(char_t c) {
    outb(0x3f8, c);
}

void puts(char_t* str,...) {

    va_list ptr;
    va_start(ptr, str);

    uint32_t length = strlen(str);

    for (uint32_t i = 0; i < length; ++i) {
        if (str[i] == '%') {
            switch (str[i+1]) {
                case 'd': {
                    uint32_t num = va_arg(ptr, uint32_t);
                    uint32_t digits = (uint32_t)len(num, num, 10);
                    uint32_t bound = (uint32_t)pow(10, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 10);
                        bound /= 10;
                    }
                    break; 
                }
                case 'c':{
                    putch(va_arg(ptr, uint32_t));
                    break; 
                }
                case 'q': {
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 10);
                    uint64_t bound = pow(10, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 10);
                        bound /= 10;
                    }
                    break; 
                }
                case 'x': {
                    puts("0x");
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 16);
                    uint64_t bound = pow(16, digits - 1);

                    while (bound) {
                        if ((num/bound)%16 > 9) {
                            putch('a' + (num/bound) % 16 - 10);
                        } else {
                            putch('0' + (num/bound) % 16);
                        }
                        bound /= 16;
                    }
                    break; 
                }
                case 'b': {
                    puts("0b");
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 2);
                    uint64_t bound = pow(2, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 2);
                        bound /= 2;
                    }
                    break; 
                }
                case 'm': {
                    byte_t length_ = str[i+2] - '0';

                    byte_t *st = va_arg(ptr, byte_t*);
                    for (byte_t j = 0; j < length_; ++j) {
                        putch(*st);
                        st++;
                    }

                    ++i;
                    break; 
                }
            }

            ++i;
        } else {
            putch(str[i]);
        }
    }
}


void puts_with_va(char_t* str, va_list ptr) {

    uint32_t length = strlen(str);

    for (uint32_t i = 0; i < length; ++i) {
        if (str[i] == '%') {
            switch (str[i+1]) {
                case 'd': {
                    uint32_t num = va_arg(ptr, uint32_t);
                    uint32_t digits = (uint32_t)len(num, num, 10);
                    uint32_t bound = (uint32_t)pow(10, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 10);
                        bound /= 10;
                    }
                    break; 
                }
                case 'c':{
                    putch(va_arg(ptr, uint32_t));
                    break; 
                }
                case 'q': {
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 10);
                    uint64_t bound = pow(10, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 10);
                        bound /= 10;
                    }
                    break; 
                }
                case 'x': {
                    puts("0x");
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 16);
                    uint64_t bound = pow(16, digits - 1);

                    while (bound) {
                        if ((num/bound)%16 > 9) {
                            putch('a' + (num/bound) % 16 - 10);
                        } else {
                            putch('0' + (num/bound) % 16);
                        }
                        bound /= 16;
                    }
                    break; 
                }
                case 'b': {
                    puts("0b");
                    uint64_t num = va_arg(ptr, uint64_t);
                    uint64_t digits = len(num, num, 2);
                    uint64_t bound = pow(2, digits - 1);

                    while (bound) {
                        putch('0' + (num/bound) % 2);
                        bound /= 2;
                    }
                    break; 
                }
                case 'm': {
                    byte_t length_ = str[i+2] - '0';

                    byte_t *st = va_arg(ptr, byte_t*);
                    for (byte_t j = 0; j < length_; ++j) {
                        putch(*st);
                        st++;
                    }

                    ++i;
                    break; 
                }
            }

            ++i;
        } else {
            putch(str[i]);
        }
    }
}
///////////////////////////////__LOG__\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void DEBUG(char_t *str,...) {
    puts("\033[35m[DEBUG]\t");
    va_list ptr;
    va_start(ptr, str);
    puts_with_va(str, ptr);
    va_end(ptr);
    puts("\x1b[0m");
}

void INFO(char_t *str,...) {
    puts("\e[36m[INFO]\t");
    va_list ptr;
    va_start(ptr, str);
    puts_with_va(str, ptr);
    va_end(ptr);
    puts("\e[0m");
}

void ERROR(char_t *str,...) {
    puts("\e[31m[ERROR]\t");
    va_list ptr;
    va_start(ptr, str);
    puts_with_va(str, ptr);
    va_end(ptr);
    puts("\e[0m");
    hlt();
}