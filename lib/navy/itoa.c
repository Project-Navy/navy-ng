#include "itoa.h"

#include <stddef.h>
#include <string.h>

void strrev(char *s)
{
    char tmp;
    size_t end = strlen(s) - 1;
    size_t start = 0;

    while (start < end)
    {
        tmp = s[start];
        s[start++] = s[end];
        s[end--] = tmp;
    }
}

char *itoa(int64_t value, char *str, uint16_t base)
{
    size_t index = 0;
    int8_t digit;

    if (value < 0 && base != 10)
    {
        int64_t left;
        int64_t right;
        size_t i;

        char left_str[32];
        char right_str[32];

        right = value & 0xffffffff;
        left = (value & 0xffffffff00000000) >> 32;

        itoa(left, left_str, base);
        itoa(right, right_str, base);

        for (i = 0; i < strlen(left_str); i++)
        {
            str[i] = left_str[i];
        }

        for (i = 0; i < strlen(right_str); i++)
        {
            str[i + strlen(left_str)] = right_str[i];
        }

        return str;
    }

    else if (value < 0)
    {
        str[index++] = '-';
        value *= 1;
    }

    do
    {
        digit = value % base;

        if (digit < 0xa)
        {
            str[index++] = digit + '0';
        }
        else
        {
            str[index++] = (digit - 0xa) + 'a';
        }

        value /= base;
    } while (value);

    str[index] = 0;

    strrev(str);

    return str;
}