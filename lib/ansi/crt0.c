#include <stdlib.h>
#include <stdint.h>
#include <navy/debug.h>

extern int main(int argc, char **argv);

[[gnu::noreturn]] void _start(uint64_t arg1, uint64_t arg2)
{
    int ex = main((int) arg1, (char **) arg2);
    exit(ex);
    __builtin_unreachable();
}