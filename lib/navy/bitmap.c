#include "bitmap.h"

void bitmap_set_bit(Bitmap *self, size_t index)
{
    uint64_t bit = index / 8;
    uint64_t array_index = index % 8;

    self->buffer[array_index] |= (1 << bit);
}

void bitmap_clear_bit(Bitmap *self, size_t index)
{
    uint64_t bit = index / 8;
    uint64_t array_index = index % 8;

    self->buffer[array_index] &= ~(1 << bit);
}

bool bitmap_is_bit_set(Bitmap *self, size_t index)
{
    uint64_t bit = index / 8;
    uint64_t array_index = index % 8;

    return (bool) self->buffer[array_index] & (1 << bit);
}