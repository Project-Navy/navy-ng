#pragma once 

#include <unistd.h>
#include <brutal/str.h>

enum IPC_TYPE 
{
    IPC_STR
};

typedef struct 
{
    pid_t sender;
    pid_t receiver;
    enum IPC_TYPE type;

    union 
    {
        Str _str;
    };
} Ipc;

void ipc_send(Ipc *ipc);
void ipc_rcv_sync(Ipc *ipc);