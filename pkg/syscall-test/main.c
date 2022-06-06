#include <brutal/str.h>
#include <unistd.h>
#include <navy/debug.h>

int main(void)
{
    for (size_t i = 0; i < 10; i++)
    {
        log$("Hello, World !");
    }

    return 0;
}