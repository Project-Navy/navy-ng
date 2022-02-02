/**
 * Copyright (C) 2021 Keyboard Slayer
 *
 * This file is part of navy.
 *
 * navy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * navy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with navy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <navy/debug.h>
#include <stdlib.h>
#include "string.h"

size_t strlen(const char *s)
{
    size_t i = 0;

    if (s == NULL)
    {
        return 0;
    }

    while (s[i++] != '\0');
    return i - 1;
}

char *strrchr(const char *s, int c)
{
    size_t length = strlen(s);
    size_t pos = length;

    while (s[pos] != c && pos-- > 0);

    if (pos == length)
    {
        return NULL;
    }

    return (char *) s + pos;
}

void *memset(void *s, int c, size_t n)
{
    if (n)
    {
        char *d = (char *) s;

        while (n--)
        {
            *d++ = c;
        }
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n)
{
    char *buffer = (char *) dest;
    const char *source = (const char *) src;

    for (size_t i = 0; i < n; i++)
    {
        buffer[i] = source[i];
    }

    return (void *) buffer;
}

int strcmp(const char *s1, const char *s2)
{
    if (strlen(s1) != strlen(s2))
    {
        return strlen(s1) - strlen(s2);
    }

    return strncmp(s1, s2, strlen(s1));
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
        {
            if (s1[1] < s2[1])
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }

        if (s1[i] == '\0')
        {
            return 0;
        }
    }

    return 0;
}

char *strtok(char *restrict str, const char *restrict delim)
{
    static char *p;

    if (!p && !str)
    {
        return NULL;
    }

    if (!str)
    {
        str = p;
    }
    else
    {
        p = str;
    }

    for (;;)
    {
        for (size_t i = 0; i < strlen(delim); i++)
        {
            if (*str == delim[i])
            {
                str++;
                break;
            }
        }

        if (*str == '\0')
        {
            return NULL;
        }

        break;
    }

    char *ret = str;
    for (;;)
    {
        if (*str == '\0')
        {
            p = str;
            return ret;
        }

        for (size_t i = 0; i < strlen(delim); i++)
        {
            if (*str == delim[i])
            {
                *str = '\0';
                p = str + 1;
                return ret;
            }
        }

        str++;
    }
}

void *memcpy(void *dest, const void *src, size_t n)
{
    if (src == NULL)
    {
        dest = NULL;
        return NULL;
    }

    size_t i;
    char *cdest = (char *) dest;
    char *csrc = (char *) src;

    for (i = 0; i < n; i++)
    {
        cdest[i] = csrc[i];
    }

    return (void *) cdest;
}

char *strdup(char const *s)
{
    /*
    if (s == NULL)
    {
        return NULL;
    }

    size_t len = strlen(s) + 1;
    void *new = malloc(len);
    if (new == NULL)
    {
        return NULL;
    }

    return (char *) memcpy(new, s, len);
    */

    (void) s;
    return NULL;
}

size_t strchrcount(char const *s, char c)
{
    size_t count = 0;
    size_t i = 0;

    while (i < strlen(s))
    {
        if (s[i++] == c) 
        {
            count++;
        }
    }

    return count;
}