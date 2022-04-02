#include "hw/x86_64/com.h"

#include <stdint.h>

void brutal_entry(void *handover, uint64_t magic)
{
    (void) handover;
    (void) magic;
    serial_puts("\nHi from navy !\n\n");
}