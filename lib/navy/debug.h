#pragma once 

#include <stddef.h>
#include <navy/fmt.h>

#define MAYBE_UNUSED [[maybe_unused]]

#define __FILENAME__                                                                               \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define log$(FORMAT, ...) \
	log_impl(__FILENAME__, __LINE__, FORMAT, PRINT_ARGS(__VA_ARGS__));

void log_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args);