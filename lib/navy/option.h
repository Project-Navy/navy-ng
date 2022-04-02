#pragma once 

#include <brutal/result.h>

#define Option(TOk) Result(TOk, int)
#define NONE(T) ERR(T, 1)
#define SOME(T, Res) OK(T, Res)