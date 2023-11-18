#include "../../include/lib/monitor.h"

int main()
{
    init_real_mode();
    init_monitor();
    monitor_put_dec(2131);
    return 0;
}