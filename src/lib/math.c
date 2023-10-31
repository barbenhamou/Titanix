#include "../../include/lib/math.h"

uint32_t pow(uint32_t base, uint32_t power) {
    int n = 1;
    for (int i = 0; i < power; ++i) {
        n *= base;
    }

    return n;
}

uint32_t len(uint32_t n) {
    uint32_t c = 0;
    while (n > 0) {
        ++c;
        n /= 10;
    }

    return c;
}