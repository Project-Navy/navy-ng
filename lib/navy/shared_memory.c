#include "shared_memory.h"
#include "ansi/unistd.h"
#include <unistd.h>

SharedMemory *shared_memory_create(size_t size)
{
    return (SharedMemory *) syscall(SYS_CREATE_SHARED_MEM, size);
}