#include "memory.h"
#include "utils.h"

VOID *alloc(UINTN size)
{
    VOID *ret;

    if (EFI_ERROR(Bs()->AllocatePool(EfiBootServicesData, size, &ret)))
    {
        log$("Out of pool");
        return NULL;
    }

    return ret;
}

VOID free(VOID *ptr)
{
    if (EFI_ERROR(Bs()->FreePool(ptr)))
    {
        log$("Couldn't free pool");
        for(;;);
    }
}

VOID *realloc(void *ptr, size_t size)
{
    free(ptr);
    return alloc(size);
}

BOOLEAN GrowBuffer(EFI_STATUS *Status, VOID **Buffer, UINTN BufferSize)
{
    BOOLEAN ret = FALSE;
    if (!*Buffer && BufferSize)
    {
        *Status = EFI_BUFFER_TOO_SMALL;
    }

    if (*Status == EFI_BUFFER_TOO_SMALL)
    {
        if (*Buffer)
        {
            free(*Buffer);
        }

        *Buffer = alloc(BufferSize);

        if (*Buffer)
        {
            ret = TRUE;
        }
        else  
        {
            *Status = EFI_OUT_OF_RESOURCES;
        }
    }

    if (!ret && EFI_ERROR(*Status) && *Buffer)
    {
        free(*Buffer);
        *Buffer = NULL;
    }

    return ret;
}
