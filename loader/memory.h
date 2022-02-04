#pragma once 

#include <Uefi.h>
#include "utils.h"

VOID *alloc(UINTN size);
VOID free(VOID *ptr);

BOOLEAN GrowBuffer(EFI_STATUS *Status, VOID **Buffer, UINTN BufferSize);