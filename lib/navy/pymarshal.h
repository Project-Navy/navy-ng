#pragma once

#include <stdint.h>
#include <stddef.h>

#include <navy/reader.h>
#include <navy/option.h>
#include <navy/vec.h>
#include <brutal/str.h>
#include <rxi/map.h>

#define FLAG_REF '\x80'

#define r_string(N, self) (                                     \
    {                                                           \
        StrFix(N) dst_str = {};                                 \
        memcpy(dst_str.buf, (self)->buf + (self)->offset, N);   \
        dst_str.len = N;                                        \
        reader_move((self), N);                                 \
        dst_str;                                                \
    })


typedef enum 
{
    TYPE_NULL = '0',
    TYPE_NONE = 'N',
    TYPE_FALSE = 'F',
    TYPE_TRUE = 'T',
    TYPE_STOPITER = 'S',
    TYPE_ELLIPSIS = '.',
    TYPE_INT = 'i',
    TYPE_INT64 = 'I',
    TYPE_FLOAT = 'f',
    TYPE_BINARY_FLOAT = 'g',
    TYPE_COMPLEX = 'x',
    TYPE_BINARY_COMPLEX = 'y',
    TYPE_LONG = 'l',
    TYPE_STRING = 's',
    TYPE_INTERNED = 't',
    TYPE_REF = 'r',
    TYPE_TUPLE = '(',
    TYPE_LIST = '[',
    TYPE_DICT = '{',
    TYPE_CODE = 'c',
    TYPE_UNICODE = 'u',
    TYPE_UNKNOWN = '?',
    TYPE_SET = '<',
    TYPE_FROZENSET = '>',
    TYPE_ASCII = 'a',
    TYPE_ASCII_INTERNED = 'A',
    TYPE_SMALL_TUPLE = ')',
    TYPE_SHORT_ASCII = 'z',
    TYPE_SHORT_ASCII_INTERNED = 'Z',
    TYPE_BUILTIN = '%'
} MarshalType;

typedef Vec(struct marshal_object) MarshalVec;

typedef struct marshal_object
{
    MarshalType type;

    union 
    {
        double _float;
        int64_t _int;
        Str _str;
        MarshalVec _vec;
        struct marshal_code *_code;
        struct marshal_object (*_builtin)(MarshalVec);
    };
} MarshalObject;

typedef struct marshal_code
{
    int argcount;
    int posonlyargcount;
    int kwonlyargcount;
    int stacksize;
    int flags;
    MarshalObject code;
    MarshalObject consts;
    MarshalObject names;
    MarshalObject localsplusnames;
    MarshalObject localspluskinds;
    MarshalObject filename;
    MarshalObject name;
    MarshalObject qualname;
    int firstlineno;
    MarshalObject linetable;
} MarshalCode;


typedef MarshalObject (*PyBuiltin)(MarshalVec);
typedef Reader(uint8_t *) MarshalReader;
typedef Option(MarshalObject) MarshalObjectOption;
typedef Option(int64_t) IntOption;
typedef map_t(PyBuiltin) BuiltinMap;
typedef map_t(MarshalObject) MarshalMap;

MarshalObjectOption marshal_r_object(MarshalReader *self);
void marshal_free(MarshalObject *obj);