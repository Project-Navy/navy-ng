#include "str.h"
#include <stdbool.h>

bool str_eq(Str const lhs, Str const rhs)
{
    if (lhs.len != rhs.len)
    {
        return false;
    }

    for (size_t i = 0; i < lhs.len; i++)
    {
        if (lhs.buf[i] != rhs.buf[i])
        {
            return false;
        }
    }

    return true;
}

int str_count(Str const haystack, Str const needle)
{
    if (haystack.len < needle.len)
    {
        return 0;
    }

    int count = 0;
    for (size_t i = 0; i < haystack.len - needle.len; i++)
    {
        Str slice = str_n$(needle.len, haystack.buf + i);

        if (str_eq(slice, needle))
        {
            count++;
        }
    }

    return count;
}

int str_count_chr(Str const str, char chr)
{
    int result = 0;

    for (size_t i = 0; i < str.len; i++)
    {
        if (str.buf[i] == chr)
        {
            result++;
        }
    }

    return result;
}

int str_last(Str const lStr, Str const rStr)
{
    if (lStr.len < rStr.len)
    {
        return 0;
    }

    int pos = -1;
    for (size_t i = 0; i < lStr.len - rStr.len; i++)
    {
        Str substr = str_n$(rStr.len, lStr.buf + i);

        if (str_eq(substr, rStr))
        {
            pos = i;
        }
    }

    return pos;
}

int str_last_chr(Str const str, char chr)
{
    int result = -1;

    for (size_t i = 0; i < str.len; i++)
    {
        if (str.buf[i] == chr)
        {
            result = i;
        }
    }

    return result;
}

int str_first(Str const lStr, Str const rStr)
{
    if (lStr.len < rStr.len)
    {
        return 0;
    }

    for (size_t i = 0; i < lStr.len - rStr.len; i++)
    {
        Str substr = str_n$(rStr.len, lStr.buf + i);

        if (str_eq(substr, rStr))
        {
            return i;
        }
    }

    return -1;
}

int str_first_chr(Str const str, char chr)
{
    for (size_t i = 0; i < str.len; i++)
    {
        if (str.buf[i] == chr)
        {
            return i;
        }
    }

    return -1;
}

bool str_any(Str const str)
{
    return str.len != 0;
}

Str str_sub(Str str, size_t start, size_t end)
{
    char *tmp = (char *) str.buf;
    tmp[end] = '\0';
    return str$(tmp + start);
}