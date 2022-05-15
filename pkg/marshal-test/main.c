#include "navy/reader.h"
#include <navy/debug.h>
#include <navy/pymarshal.h>

#include <stdio.h>

uint8_t bytes[9] = {0xe7, 0x18, 0x2d, 0x44, 0x54, 0xfb, 0x21, 0x9, 0x40};

int main(void)
{
    MarshalReader r;

    reader_init(&r, bytes, 9);
    MarshalObject obj = UNWRAP(marshal_r_object(&r));

    printf("%lf\n", obj._float);

    return 0;
}