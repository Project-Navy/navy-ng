#pragma once

#include <navy/option.h>

typedef enum {
    unknown_format, ieee_big_endian_format, ieee_little_endian_format
} float_format_type;

typedef Option(double) DoubleOption;

DoubleOption PyFloat_Unpack4(const char *data, int le);
DoubleOption PyFloat_Unpack8(const char *data, int le);

