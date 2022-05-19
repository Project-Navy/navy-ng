#include <assert.h>
#include <stdio.h>
#include <navy/debug.h>
#include <navy/pymarshal.h>
#include <navy/pyvm.h>

int main(void)
{
    MarshalReader r;

    FILE *fp = fopen("./pkg/marshal-test/test.pyc", "r");
    assert(fp != NULL);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *bytes = calloc(1, size + 1);
    assert(bytes != NULL);
    fread(bytes, size, 1, fp);
    
    reader_init(&r, bytes, size);
    auto magic = r_string(4, &r);
    reader_move(&r, 12);
    
    assert(strcmp(magic.buf, "o\r\r\n") == 0); // Python 3.10 magic
    MAYBE_UNUSED MarshalObject obj = UNWRAP(marshal_r_object(&r));
    py_run(*obj._code);

    marshal_free(&obj);
    fclose(fp);
    free(bytes);
    return 0;
}