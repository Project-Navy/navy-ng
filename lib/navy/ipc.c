#include "ipc.h"

void ipc_send(Ipc *ipc)
{
    syscall(SYS_IPC_SEND, (uintptr_t) ipc);
}

void ipc_rcv_sync(Ipc *ipc)
{
    syscall(SYS_IPC_RCV_SYNC, (uintptr_t) ipc);
}