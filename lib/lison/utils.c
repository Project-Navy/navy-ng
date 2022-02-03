#include "utils.h"
#include "brutal/str.h"

#include <ctype.h>

bool isnumber(Str s)
{
    bool ret = true;

    for (size_t i = (s.buf[0] == '-' ? 1 : 0); i < s.len; i++)
    {
        if (isdigit(s.buf[i]) == 0)
        {
            ret = false;
        }
    }

    return (s.buf[0] == '-' && s.len == 1) ? false : ret;
}

bool isfloat(Str s)
{
    bool has_digit = false;
    bool found_dot = false;
    bool ret = true;

    for (size_t i = (s.buf[0] == '-' ? 1 : 0); i < s.len; i++)
    {
        if (found_dot && s.buf[i] == '.' && !isdigit(s.buf[i]))
        {
            ret = false;
        }

        if (isdigit(s.buf[i]))
        {
            has_digit = true;
        }

        if (s.buf[i] == '.')
        {
            found_dot = true;
        }
    }

    return found_dot && has_digit && ret;
}

bool isstr(Str s)
{
    if (s.buf[0] != '\"')
    {
        return 0;
    }

    size_t i;

    for (i = 1; i < s.len; i++)
    {
        if (s.buf[i] == '\\')
        {
            i++;
        }

        if (s.buf[i] == '\"' && s.buf[i + 1] != 0)
        {
            return false;
        }
    }

    if (s.buf[i - 1] != '\"')
    {
        return false;
    }

    return true;
}

void lison_free(Lison *lst)
{
    if (lst->type != LISON_LIST)
    {
        return;
    }

    Lison obj;
    size_t i;

    vec_foreach(&lst->_list, obj, i)
    {
        lison_free(&obj);
    }

    vec_free(&lst->_list);
}

Lison lison_has_error(Lison node)
{
    if (node.type == LISON_ERROR)
    {
        return node;
    }
    else if (node.type != LISON_LIST)
    {
        return lison_nil;
    }

    Lison obj;
    Lison tmp;
    size_t i;

    vec_foreach(&node._list, obj, i)
    {
        tmp = lison_has_error(obj);
        if (tmp.type != LISON_NIL)
        {
            return tmp;
        }

        lison_has_error(tmp);
    }

    return lison_nil;
}

size_t vec_sarray_count_before(Str *arrs, Str element, size_t limit)
{
    size_t count = 0;
    for (size_t i = 0; i < limit; i++)
    {
        if (str_eq(arrs[i], element))
        {
            count++;
        }
    }

    return count;
}