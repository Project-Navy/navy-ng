#include "debug.h"

#ifdef KERNEL
#   include <arch.h>
#   define out serial_puts
#elif HOST
#   include <stdio.h>
#   include <stdlib.h>
void out(char const *s)
{
    fputs(s, stdout);
}
void raise_debug(void)
{
    exit(1);
}
#else 
#   include <unistd.h>
#   define out sys_log
#endif

void log_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args)
{
    print_format(out, "\033[33m{}:{}\033[0m ", filename, line_nbr);
    PRINT_FORMAT(out, format, args);
    out("\n");
}

void panic_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args)
{
    print_format(out, "\033[31m{}:{}\033[0m ", filename, line_nbr);
    PRINT_FORMAT(out, format, args);
    out("\n");
#ifdef KERNEL
    raise_debug();
#else 
#   include <stdlib.h> 
    exit(1);
#endif
}