#pragma once 

#include <stdbool.h>
#include <brutal/str.h>
#include "object.h"

bool isnumber(Str s);
bool isfloat(Str s);
bool isstr(Str s);

size_t vec_sarray_count_before(Str *arrs, Str element, size_t limit);

void lison_free(Lison *obj);
Lison lison_has_error(Lison node);