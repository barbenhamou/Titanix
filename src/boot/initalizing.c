#include "../../include/lib/functions.h"

extern byte_t* real_mode_start(void);
extern byte_t* real_mode_end(void);

void init_real_mode(void) {
    memcpy((byte_t*)0x4000, real_mode_start, real_mode_end - real_mode_start);
}