#ifdef __x86_64__
#   include "hw/x86_64/com.h"
#else 
#   error "Unknown architecture"
#endif

int _start(void)
{
    serial_puts("Hello, World !");
    return 666;
}