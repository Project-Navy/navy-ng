#include <navy/ipc.h>
#include <navy/debug.h>

int main(void)
{
    log$("PID: {}", getpid());

    if (getpid() == 1)
    {
        Ipc msg = (Ipc) {
            .receiver = 2,
            .type = IPC_STR,
            ._str = str$("Hello from sender !")
        };

        ipc_send(&msg);
        log$("IPC SENT !");

        return 0;
    }
    else  
    {
        Ipc msg;
        ipc_rcv_sync(&msg);

        log$("IPC ! {}", msg._str);
    }

    return 0;
}