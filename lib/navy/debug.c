#include "debug.h"

#ifdef KERNEL
#   include <arch.h>
#   define out serial_puts
#else
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
    raise_debug();
}