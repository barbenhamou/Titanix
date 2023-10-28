#include "../../include/lib/monitor.h"

int main(struct multiboot *mboot_ptr)
{
    init_monitor();
    monitor_write("hiw");
    monitor_put(0x08);
    return 0;
}