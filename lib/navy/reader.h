#pragma once 

#include <stdbool.h>
#include <stddef.h>

#define Reader(T)               \
    struct                      \
    {                           \
        T buf;                  \
        size_t length;          \
        size_t offset;          \
    }

#define reader_init(r, t, l)    \
    (memset((r), 0, sizeof(*(r))), (r)->buf = (t), (r)->length = (l))

#define reader_peek(r, i) (r)->content[i]
#define reader_next(r) reader_peek(r, r->offset++)
#define reader_eof(r) ((r)->offset >= (r)->length)