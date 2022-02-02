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

#include "assert.h"
#include <navy/fmt.h>
#ifdef KERNEL
#   include "arch/arch.h"
#else
#   include <unistd.h>
#   include <stdlib.h>
#endif

void ASSERT(char const *expr, char const *filename, size_t line)
{
    #ifdef KERNEL
        print_format(arch_serial_puts, "\033[31m{}:{}\033[0m assertion failed ({})\n", filename, line,
                     expr);
        raise_debug();
    #else 
        print_format(sys_log, "\033[31m{}:{}\033[0m assertion failed ({})\n", filename, line, expr);
        exit(1);
    #endif
}
