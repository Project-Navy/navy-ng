#include "floatobject.h"

// FIXME
static float_format_type double_format = ieee_little_endian_format;
//

DoubleOption
PyFloat_Unpack8(const char *data, int le)
{
    double x;
    const char *p = data;

    if ((double_format == ieee_little_endian_format && !le)
        || (double_format == ieee_big_endian_format && le)) {
        char buf[8];
        char *d = &buf[7];
        int i;

        for (i = 0; i < 8; i++) {
            *d-- = *p++;
        }
        memcpy(&x, buf, 8);
    }
    else {
        memcpy(&x, p, 8);
    }

    return SOME(DoubleOption, x);
}