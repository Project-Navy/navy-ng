#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <lison/parser.h>
#include <navy/debug.h>

static void read_loader_entry(Lison entry)
{
    Lison name = lison_get(&entry, str$("name"));
    Lison kernel_location = lison_get(&entry, str$("kernel"));
    
    log$("{} @ {}", name._str, kernel_location._str);
}

int main(void)
{
    FILE *fp = fopen("./pkg/lison-test/test.lisp", "r");
    assert(fp != NULL);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = calloc(1, size + 1);
    assert(buffer != NULL);
    fread(buffer, size, 1, fp);

    Lison master = lison_parse_cstr(buffer);

    if (master.type == LISON_ERROR)
    {
        log$("{} at line {}", master._error.message, master._error.line);
    }

    Lison entries = lison_get(&master, str$("entries"));
    Lison entry;
    size_t i;

    vec_foreach(&entries._list, entry, i)
    {
        read_loader_entry(entry);
    }
    lison_free(&master);
    free(buffer);
    return 0;
}