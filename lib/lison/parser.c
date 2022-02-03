#include "parser.h"
#include "lison/lexer.h"
#include <navy/reader.h>
#include <navy/debug.h>

Lison lison_parse_str(Str s)
{
    Str token;
    size_t i;
    VecStr tokens = lison_lex_str(s);

    vec_foreach(&tokens, token, i) 
    {
        log$("{}", token);
    }

    vec_free(&tokens);
    return (Lison) {};
}

Lison lison_parse_cstr(const char *s)
{
    return lison_parse_str(str$(s));
}
