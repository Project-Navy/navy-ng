#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void vec_expand_(char **data, size_t *length, size_t *capacity, int memsz);

#define Vec(T)                                                                 \
    struct                                                                     \
    {                                                                          \
        T *data;                                                               \
        size_t length;                                                         \
        size_t capacity;                                                       \
        size_t iter;                                                           \
    }

#define vec_init(v) memset((v), 0, sizeof(*(v)))

#define vec_push(v, val)                                                       \
    vec_expand_((char **) &(v)->data, &(v)->length, &(v)->capacity,            \
                sizeof(*(v)->data));                                           \
    (v)->data[(v)->length++] = (val)

#define vec_foreach(v, t)                                                      \
    if ((v)->length > 0)                                                       \
        for ((v)->iter = 0;                                                    \
             (v)->iter < (v)->length && (((t) = (v)->data[(v)->iter]), 1);     \
             ++(v)->iter)

#define vec_free(v) free((v)->data)

typedef Vec(char) VecStr;