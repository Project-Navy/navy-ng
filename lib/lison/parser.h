#pragma once

#include "object.h"
#include <brutal/str.h>
#include <navy/reader.h>

Lison lison_parse_cstr(char const *s);
Lison lison_parse_str(Str s);