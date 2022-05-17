#include "pymarshal.h"

#include <assert.h>
#include <navy/debug.h>
#include <cpython/floatobject.h>

static double r_float_bin(MarshalReader *self)
{
    auto n = r_string(8, self);
    return UNWRAP(PyFloat_Unpack8(n.buf, 1));
}

static IntOption r_long(MarshalReader *self)
{
    if (self->length - self->offset < 4)
    {
        return NONE(IntOption);
    }

    auto buf = r_string(4, self);
    int32_t x = buf.buf[0];


    for (size_t i = 8; i < 25; i += 8)
    {
        x |= buf.buf[i / 8] << i;
    }

    return SOME(IntOption, (int64_t) x);
}


static void r_code(MarshalReader *self)
{
    reader_move(self, 4);

    MAYBE_UNUSED int argcount = (int) UNWRAP(r_long(self));
    MAYBE_UNUSED int posonlyargcount = (int) UNWRAP(r_long(self));
    MAYBE_UNUSED int kwonlyargcount = (int) UNWRAP(r_long(self));
    MAYBE_UNUSED int stacksize = (int) UNWRAP(r_long(self));
    MAYBE_UNUSED int flags = (int) UNWRAP(r_long(self));

    MAYBE_UNUSED MarshalObject code = UNWRAP(marshal_r_object(self));
    MAYBE_UNUSED MarshalObject consts = UNWRAP(marshal_r_object(self)); 
    // MAYBE_UNUSED MarshalObject names = UNWRAP(marshal_r_object(self));
    // MAYBE_UNUSED MarshalObject localsplusnames = UNWRAP(marshal_r_object(self));
}

MarshalObjectOption marshal_r_object(MarshalReader *self)
{
    uint8_t code = reader_next(self);
    log$("{}", code);
    // uint8_t flag = code & FLAG_REF;
    uint8_t type = code & ~FLAG_REF;
    MarshalObject ret = {.type = type};

    if (reader_eof(self))
    {
        return NONE(MarshalObjectOption);
    }

    switch(type)
    {
        case TYPE_NONE:
        case TYPE_NULL:
        case TYPE_FALSE:
        case TYPE_TRUE:
        {
            break;
        }
        case TYPE_BINARY_FLOAT:
        {
            ret._float = r_float_bin(self);
            break;
        }

        case TYPE_INT:
        {
            ret._int = UNWRAP(r_long(self));
            break;
        }

        case TYPE_CODE:
        {
            r_code(self);
            break;
        }

        case TYPE_STRING:
        {
            size_t n = UNWRAP(r_long(self));
            Str buffer = str$(self->buf);
            ret._str = str_sub(buffer, self->offset, self->offset + n);
            reader_move(self, n);
            break;
        }

        case TYPE_SHORT_ASCII: 
        {
            uint8_t n = reader_next(self);
            Str buffer = str$(self->buf);
            ret._str = str_sub(buffer, self->offset, self->offset + n);
            reader_move(self, n);
            break;
        }

        default:
        {
            log$("Unknown opcode {x} ({})", type, (char) type);
            return NONE(MarshalObjectOption);
        }
    }

    return SOME(MarshalObjectOption, ret);
}