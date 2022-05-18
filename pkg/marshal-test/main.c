#include <assert.h>
#include <navy/debug.h>
#include <navy/pymarshal.h>

uint8_t bytes[] = {111, 13, 13, 10, 0, 0, 0, 0, 223, 103, 131, 98, 24, 0, 0, 0, 227, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 64, 0, 0, 0, 115, 12, 0, 0, 0, 101, 0, 100, 0, 131, 1, 1, 0, 100, 1, 83, 0, 41, 2, 122, 14, 72, 101, 108, 108, 111, 44, 32, 87, 111, 114, 108, 100, 32, 33, 78, 41, 1, 218, 5, 112, 114, 105, 110, 116, 169, 0, 114, 2, 0, 0, 0, 114, 2, 0, 0, 0, 250, 7, 116, 101, 115, 116, 46, 112, 121, 218, 8, 60, 109, 111, 100, 117, 108, 101, 62, 1, 0, 0, 0, 115, 2, 0, 0, 0, 12, 0};

int main(void)
{
    MarshalReader r;

    reader_init(&r, bytes, sizeof(bytes) / sizeof(bytes[0]));
    auto magic = r_string(4, &r);
    reader_move(&r, 12);
    
    assert(strcmp(magic.buf, "o\r\r\n") == 0); // Python 3.10 magic
    MAYBE_UNUSED MarshalObject obj = UNWRAP(marshal_r_object(&r));

    log$("{}", obj._float);

    return 0;
}