#pragma once

#include <stdbool.h>
#include <navy/debug.h>
#include <brutal/str.h>

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

#define Err(T, Err)                                                            \
    (T)                                                                        \
    {                                                                          \
        .success = false, ._error = Err                                        \
    }

#define Ok(T, Res)                                                             \
    (T)                                                                        \
    {                                                                          \
        .success = true, ._ok = Res                                            \
    }


#define Option(TOk) Result(TOk, int)
#define None(T) Err(T, 1)
#define Some(T, Res) Ok(T, Res)

#define unwrap(r) (r)._ok
#define unwrap_or_panic(r)(                                                      \
    {                                                                            \
        if (!(r).success)                                                        \
        {                                                                        \
            panic$("Can't unwrap important value");                              \
        }                                                                        \
        unwrap(r);                                                               \
    })                                                      

#define unwrap_or(r, c)(                                                         \
    {                                                                            \
        typeof(r._ok) __result;                                                  \
        if (!(r).success)                                                        \
        {                                                                        \
            __result = (c);                                                      \
        }                                                                        \
        else                                                                     \
        {                                                                        \
            __result = unwrap(r);                                                \
        }                                                                        \
                                                                                 \
        __result;                                                                \
    }                                                                            \
)