#pragma once 

#include <stdint.h>
#include <brutal/str.h>
#include <navy/vec.h>

typedef enum 
{
    LISON_LIST,
    LISON_SYMBOL,
    LISON_TRUE,
    LISON_NIL,
    LISON_NUMBER,
    LISON_FLOAT,
    LISON_CHAR,
    LISON_STR,
    LISON_ERROR,
    LISON_QUOTE,
} LisonType;

typedef struct lison Lison;
typedef Vec(Lison) LisonList;

typedef struct 
{
    Str message;
    size_t line;
} LisonError;

struct lison
{
    LisonType type;

    union 
    {
        int64_t _nbr;
        double _float;
        char _char;
        Str _str;
        LisonList _list;
        LisonError _error;
    };
};

#define lison_nil               \
    (Lison)                     \
    {                           \
        .type = LISON_NIL,      \
    }

#define lison_raise(m, r)                                                           \
    ((Lison)                                                                        \
    {                                                                               \
        .type = LISON_ERROR,                                                        \
        ._error = (LisonError) {                                                    \
            .message = str$((m)),                                                   \
            .line = vec_sarray_count_before((r)->buf, str$("\n"), (r)->offset) + 1  \
        }                                                                           \
    })