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

#include "unistd.h"

uint64_t syscall1(uint64_t syscall_id)
{
    return syscall5(syscall_id, 0, 0, 0, 0);
}

uint64_t syscall2(uint64_t syscall_id, uint64_t arg1)
{
    return syscall5(syscall_id, arg1, 0, 0, 0);
}

uint64_t syscall3(uint64_t syscall_id, uint64_t arg1, uint64_t arg2)
{
    return syscall5(syscall_id, arg1, arg2, 0, 0);
}

uint64_t syscall4(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3)
{
    return syscall5(syscall_id, arg1, arg2, arg3, 0);
}

uint64_t syscall5(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
    uint64_t syscall_return;

    asm volatile(
        "syscall \n"
        : "=a"(syscall_return)
        : "a"(syscall_id), "b"(arg1), "d"(arg2), "S"(arg3), "D"(arg4)
        : "memory");

    return syscall_return;
}

void sys_log(char const *s)
{
    syscall(SYS_LOG, (uintptr_t) s);
}

pid_t getpid(void)
{
    return syscall(SYS_GETPID);
}