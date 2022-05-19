#pragma once 

#include <stdbool.h>
#include <stddef.h>

#include <navy/vec.h>

#define Reader(T)               \
    struct                      \
    {                           \
        T buf;                  \
        size_t length;          \
        size_t offset;          \
    }

#define reader_init(r, t, l)    \
    (memset((r), 0, sizeof(*(r))), (r)->buf = (t), (r)->length = (l))

#define reader_peek(r, i) (r)->buf[(i)]
#define reader_current(r) reader_peek(r, r->offset)
#define reader_next(r) reader_peek(r, r->offset++)
#define reader_eof(r) ((r)->offset >= (r)->length)
#define reader_move(r, n) (r)->offset += n

typedef Reader(Str) ReaderStr;
typedef Reader(Str *) ReaderStrArray;
typedef Reader(char const *) ReaderCStr;