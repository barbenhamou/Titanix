#include "../../include/lib/monitor.h"

int main()
{
    init_real_mode();
    init_monitor();
    monitor_put_dec(2131);
    puts("hello %x bye bye %d\n", 16, 12);
    return 0;
}
