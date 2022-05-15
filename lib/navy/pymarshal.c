#include "pymarshal.h"

#include <navy/debug.h>
#include <brutal/str.h>
#include <cpython/floatobject.h>

static double r_float_bin(MarshalReader *self)
{
    Str buffer = str_n$(8, (char const *) (self->buf + self->offset));
    return UNWRAP(PyFloat_Unpack8(buffer.buf, 1));
}

MarshalObjectOption marshal_r_object(MarshalReader *self)
{
    uint8_t code = reader_next(self);
    // uint8_t flag = code & FLAG_REF;
    uint8_t type = code & ~FLAG_REF;
    MarshalObject ret = {.type = type};

    if (reader_eof(self))
    {
        return NONE(MarshalObjectOption);
    }

    switch(type)
    {
        case TYPE_BINARY_FLOAT:
        {
            ret._float = r_float_bin(self);
            break;
        }
        default:
        {
            return NONE(MarshalObjectOption);
        }
    }

    return SOME(MarshalObjectOption, ret);
}