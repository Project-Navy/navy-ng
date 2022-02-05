#include "utils.h"

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *conout = NULL;
static EFI_BOOT_SERVICES *bs = NULL;

VOID register_conout(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self)
{
    conout = self;
}

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut(void)
{
    return conout;
}

void register_bs(EFI_BOOT_SERVICES *self)
{
    bs = self;
}

EFI_BOOT_SERVICES *Bs(void)
{
    return bs;
}

VOID cstr_stdout(char const *s)
{
    while (*s)
    {
        UINT16 out[2] = {0};
        out[0] = (UINT16) *s++;
        ConOut()->OutputString(ConOut(), out);
    }
}

VOID log_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args)
{
    print_format(cstr_stdout, ">> {}: {} ", filename, line_nbr);
    PRINT_FORMAT(cstr_stdout, format, args);
    cstr_stdout("\r\n");
}