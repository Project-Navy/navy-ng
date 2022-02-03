#pragma once 

#define str_foreach$(str, c, iter)                                                              \
    if ((str).len > 0)                                                                          \
        for ((iter) = 0; (iter) < (str).len && (((c) = (str).buf[iter]), 1); ++(iter))          
