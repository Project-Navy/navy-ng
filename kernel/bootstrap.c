#ifdef __x86_64__
#   include "hw/x86_64/com.h"
#else 
#   error "Unknown architecture"
#endif

void entry_main(void)
{
    serial_puts("Hello, World !");
    for(;;);
}