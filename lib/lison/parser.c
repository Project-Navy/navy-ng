#include "parser.h"
#include <navy/reader.h>
#include <navy/debug.h>

static size_t line = 1;

Lison lison_parse_cstr(const char *s)
{
    return lison_parse_str(str$(s));
}

Lison lison_parse_str(Str s)
{
    char c;
    size_t i;
    str_foreach$(s, c, i)
    {
        if (c == '\n')
        {
            line++;
        }
    }

    return (Lison) {};
}