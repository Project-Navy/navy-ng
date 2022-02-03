#include "parser.h"
#include "lexer.h"
#include "object.h"
#include "utils.h"
#include <navy/reader.h>
#include <navy/debug.h>

static Lison read_form(ReaderStr *r);

static Lison read_list(ReaderStr *r)
{
    Lison tokens;
    tokens.type = LISON_LIST;
    vec_init(&tokens._list);
    reader_next(r);

    while (!str_eq(reader_current(r), str$(")")))
    {
        if (str_eq(reader_current(r), str$("\n")))
        {
            reader_next(r);
            continue;
        }

        if (reader_eof(r))
        {
            vec_free(&tokens._list);
            return lison_raise("Unexpected EOF", r);
        }

        Lison tmp = read_form(r);
        if (tmp.type == LISON_ERROR)
        {
            lison_free(&tokens);
            return tmp;
        }
        else
        {
            vec_push(&tokens._list, tmp);
        }
    }

    reader_next(r);
    return tokens;
}

static Lison read_atom(ReaderStr *r)
{
    Str token = reader_next(r);
    Lison tok = lison_nil;

    if (isnumber(token))
    {
        tok.type = LISON_NUMBER;
        tok._nbr = atoll(token.buf);
    }
    else if (isfloat(token))
    {
    #if !defined(KERNEL) && !defined(LOADER)
        tok.type = LISON_FLOAT;
        tok._float = strtod(token.buf, NULL);
    #else
        return lison_raise("Unsported floating point numbers", r);
    #endif
    }
    else if (token.buf[0] == '\"')
    {
        if (isstr(token))
        {
            tok.type = LISON_STR;
            tok._str = str_sub(token, 1, token.len - 1);
        }
    }
    else if(str_eq(token, str$("nil")))
    {
        return lison_nil;
    }
    else if(str_eq(token, str$("#t")))
    {
        return (Lison) {
            .type = LISON_TRUE
        };
    }
    else  
    {
        tok.type = LISON_SYMBOL;
        tok._str = token;
    }

    return tok;
}

static Lison read_form(ReaderStr *r)
{
    Str token = reader_current(r);

    if (str_eq(token, str$("(")))
    {
        return read_list(r);
    }
    else if (str_eq(token, str$(")")))
    {
        return lison_raise("Unexpected EOF", r);
    }
    else if (str_eq(token, str$("\'")))
    {
        reader_next(r);
        return (Lison) {
            .type = LISON_QUOTE
        };
    }
    else if (str_eq(token, str$("\n")))
    {
        reader_next(r);
        return read_form(r);
    }

    else  
    {
        return read_atom(r);
    }

    return lison_nil;
}

Lison lison_parse_str(Str s)
{
    VecStr tokens = lison_lex_str(s);
    ReaderStr reader;

    reader_init(&reader, tokens.data, tokens.length);

    Lison quote = read_form(&reader);
    if (quote.type != LISON_QUOTE && quote.type != LISON_ERROR)
    {
        return lison_raise("Invalid LISON code: No quote found", &reader);
    }

    Lison root_node = read_form(&reader);
    if (root_node.type != LISON_LIST && root_node.type != LISON_ERROR)
    {
        return lison_raise("Invalid LISON code: No list found", &reader);
    }

    vec_free(&tokens);

    Lison error = lison_has_error(root_node);
    if (error.type != LISON_NIL)
    {
        lison_free(&root_node);
        return error;
    }

    return root_node;
}

Lison lison_parse_cstr(const char *s)
{
    return lison_parse_str(str$(s));
}
