#pragma once 

#include <unistd.h>
#include <brutal/str.h>
#include <navy/shared_memory.h>

enum IPC_TYPE 
{
    IPC_STR,
    IPC_SHARED_MEMORY
};

typedef struct 
{
    pid_t sender;
    pid_t receiver;
    enum IPC_TYPE type;

    union 
    {
        Str _str;
        uintptr_t _addr;
    };
} Ipc;

void ipc_send(Ipc *ipc);
void ipc_rcv_sync(Ipc *ipc);