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

#include "stdlib.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#if defined(KERNEL) || defined(LOADER)
// #include "kernel/kmalloc.h"
#endif

static unsigned int random_next = 1;

int64_t pow10(int64_t power)
{
    int64_t return_value = 1;

    for (int64_t i = 0; i < power; i++)
    {
        return_value *= 10;
    }

    return return_value;
}

int atoi(char const *nptr)
{
    bool is_negative = false;
    int64_t return_value = 0;
    size_t index = 0;

    if (nptr[index] == '-')
    {
        is_negative = true;
        index++;
    }

    while (nptr[index] != '\0')
    {
        uint16_t power = strlen(nptr) - index - 1;

        return_value += (nptr[index] - '0') * (uint32_t) pow10(power);
        index++;
    }

    if (is_negative)
    {
        return_value *= -1;
    }

    return return_value;
}

void srandom(unsigned int seed)
{
    random_next = seed;
}

long random(void)
{
    random_next = random_next * 1103515245 + 12345;
    return (unsigned int) (random_next / 65536) % 32768;
}

void exit(int status)
{
#if defined(KERNEL) || defined(LOADER)
    (void) status;
#else
    syscall(SYS_EXIT, status);
#endif
}

/*
void *malloc(size_t size)
{
#ifdef KERNEL 
    return kmalloc(size);
#else 
    return (void *) syscall(SYS_ALLOC, size);
#endif
}

void free(void *ptr)
{
#ifdef KERNEL 
    kfree(ptr);
#else 
    syscall(SYS_FREE, (uintptr_t) ptr);
#endif
}

void *realloc(void *ptr, size_t size)
{
#ifdef KERNEL
    return krealloc(ptr, size);
#else 
    return (void *) syscall(SYS_REALLOC, (uintptr_t) ptr, size);
#endif
}

void *calloc(size_t nmemb, size_t size)
{
    void *ptr = malloc(nmemb * size);
    memset(ptr, 0, nmemb * size);
    return ptr;
}*/