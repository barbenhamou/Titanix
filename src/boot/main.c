#include "../../include/lib/monitor.h"
#include "../../include/lib/mmap.h"

int main()
{
    init_monitor();
    monitor_put_dec(2131);
    init_mmap();
    put_mmap();
    byte_t* arr = alloc(1024);
    return 0;
}
