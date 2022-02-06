#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct 
{
    uint8_t *buffer;
    size_t length;
} Bitmap;

void bitmap_set_bit(Bitmap *self, size_t index);
void bitmap_clear_bit(Bitmap *self, size_t index);
bool bitmap_is_bit_set(Bitmap *self, size_t index);