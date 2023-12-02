#include "../../include/lib/interrupts.h"

global exceptin_handler;

void exceptin_handler() {
    __asm__ __volatile__ ("cli;hlt");
}