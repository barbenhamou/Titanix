#include "../../include/lib/monitor.h"

int main(struct multiboot *mboot_ptr)
{
    init_monitor();
    monitor_put_hex(0x132);
    return 0;
}