#pragma once

#include <stdbool.h>
#include <navy/debug.h>

#define Result(TOk, TError)                                                    \
    struct                                                                     \
    {                                                                          \
        bool success;                                                          \
        union                                                                  \
        {                                                                      \
            TError _error;                                                     \
            TOk _ok;                                                           \
        };                                                                     \
    }

#define ERR(T, Err)                                                            \
    (T)                                                                        \
    {                                                                          \
        .success = false, ._error = Err                                        \
    }

#define OK(T, Res)                                                             \
    (T)                                                                        \
    {                                                                          \
        .success = true, ._ok = Res                                            \
    }

#define unwrap(r) (r)._ok
#define unwrap_or_panic(r)(                                                      \
    {                                                                            \
        if (!(r).success)                                                        \
        {                                                                        \
            panic$("Can't unwrap at {}", __LINE__);                              \
        }                                                                        \
        unwrap(r);                                                               \
    })                                                      