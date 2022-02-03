#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <lison/parser.h>

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

    lison_parse_cstr(buffer);
    free(buffer);
    return 0;
}