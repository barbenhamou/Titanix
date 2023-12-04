#include "../../include/lib/interrupts.h"

void exception_handler() {
    __asm__ __volatile__ ("cli;hlt");
}