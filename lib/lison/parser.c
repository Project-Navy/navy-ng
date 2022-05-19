#include "parser.h"
#include "brutal/str.h"
#include "lexer.h"
#include "object.h"
#include "utils.h"
#include <navy/reader.h>
#include <navy/debug.h>

static size_t line_count;
static Lison read_form(ReaderStrArray *r);

static Lison read_list(ReaderStrArray *r)
{
    Lison tokens;
    bool is_list = false;
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
        else if(tmp.type == LISON_QUOTE)
        {
            is_list = true;
        }
        else
        {
            vec_push(&tokens._list, tmp);
        }
    }

    if (tokens._list.length != 2 && \
        !is_list && \
        vec_sarray_count_before((r)->buf, str$("\n"), (r)->offset) + 1 != line_count)
    {
        lison_free(&tokens);
        return lison_raise("A pair was expected", r);
    }

    /*
    if (tokens._list.data[0].type != LISON_SYMBOL && \
        vec_sarray_count_before((r)->buf, str$("\n"), (r)->offset) + 1 != line_count)
    {
        lison_free(&tokens);
        return lison_raise("The first element of the pair has to be a symbol", r);
    }*/
   

    reader_next(r);
    return tokens;
}

static Lison read_atom(ReaderStrArray *r)
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

static Lison read_form(ReaderStrArray *r)
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
    line_count = str_count_chr(s, '\n') + 1;
    VecStr tokens = lison_lex_str(s);
    ReaderStrArray reader;

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

Lison lison_get(Lison *self, Str key)
{
    if (self->type != LISON_LIST)
    {
        return lison_nil;
    }

    Lison obj;
    size_t i;

    vec_foreach(&self->_list, obj, i)
    {
        if (obj.type != LISON_LIST && obj.type != LISON_QUOTE)
        {
            return lison_nil;
        }

        if (obj.type == LISON_QUOTE)
        {
            continue;
        }
        
        if (obj._list.data[0].type != LISON_SYMBOL)
        {
            return lison_nil;
        }

        if (str_eq(obj._list.data[0]._str, key))
        {
            return obj._list.data[1];
        }
    }

    return lison_nil;
}