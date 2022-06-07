#include <navy/ipc.h>
#include <navy/debug.h>
#include <navy/shared_memory.h>

#include <string.h>

int main(void)
{
    log$("PID: {}", getpid());

    if (getpid() == 1)
    {
        char const *test = "Yes it works !";
        SharedMemory *mem = shared_memory_create(strlen(test) + 1);
        void *buf = (void *) mem->range.base;

        memset(buf, 0, mem->range.length);
        memcpy(buf, test, mem->range.length);
        
        Ipc msg = {
            .receiver = 2,
            .type = IPC_SHARED_MEMORY,
            ._addr = (uintptr_t) mem
        };

        ipc_send(&msg);
        return 0;
    }
    else  
    {
        Ipc msg;
        ipc_rcv_sync(&msg);

        SharedMemory *mem = (SharedMemory *) msg._addr;
        char const *buf = (char const *) mem->range.base;

        log$("From shared memory: {}", buf);
        return 0;
    }
}