#include "pymarshal.h"

#include <navy/debug.h>
#include <brutal/str.h>
#include <cpython/floatobject.h>

#define r_string(N, self) (                               \
    {                                                     \
        StrFix(N) dst_str = {};                           \
        memcpy(dst_str.buf, self->buf + self->offset, N); \
        dst_str.len = N;                                  \
        dst_str;                                          \
    })

static double r_float_bin(MarshalReader *self)
{
    __auto_type n = r_string(8, self);
    return UNWRAP(PyFloat_Unpack8(n.buf, 1));
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

        // case TYPE_INT:
        // {
        //     ret._int = r_long()
        // }

        default:
        {
            log$("Unknown opcode {x}", type);
            return NONE(MarshalObjectOption);
        }
    }

    return SOME(MarshalObjectOption, ret);
}