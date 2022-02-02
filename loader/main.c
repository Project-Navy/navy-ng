#include "ProcessorBind.h"
#include "Uefi/UefiBaseType.h"
#include <navy/debug.h>
#include <navy/fmt.h>
#include <Uefi.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#define log$(FORMAT, ...) \
	log_impl(__FILENAME__, __LINE__, FORMAT, PRINT_ARGS(__VA_ARGS__));

#define __FILENAME__  \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define check_status$(status)                           \
    if (status != EFI_SUCCESS)                          \
    {                                                   \
        log$("Unsucessful EFI status: {x}", status);    \
        return status;                                  \
    }                                                   \

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
static EFI_BOOT_SERVICES *Bs;

static void cstr_stdout(char const *s)
{
    while (*s)
    {
        UINT16 out[2] = {0};
        out[0] = (UINT16) *s++;
        ConOut->OutputString(ConOut, out);
    }
}

static void log_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args)
{
    print_format(cstr_stdout, "{}:{} ", filename, line_nbr);
    PRINT_FORMAT(cstr_stdout, format, args);
    cstr_stdout("\r\n");
}

static VOID *alloc(UINTN size)
{
    VOID *ret;

    if (EFI_ERROR(Bs->AllocatePool(EfiBootServicesData, size, &ret)))
    {
        log$("Out of pool");
        return NULL;
    }

    return ret;
}

static void free(VOID *ptr)
{
    if (EFI_ERROR(Bs->FreePool(ptr)))
    {
        log$("Couldn't free pool");
        for(;;);
    }
}

static BOOLEAN GrowBuffer(EFI_STATUS *Status, VOID **Buffer, UINTN BufferSize)
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

static EFI_STATUS load_file (
    EFI_FILE_PROTOCOL *rootdir,
    UINT8 **buffer,
    CHAR16 *filename)
{
    EFI_FILE_PROTOCOL *file_handle;
    check_status$(rootdir->Open(
        rootdir, 
        &file_handle, 
        filename,
        EFI_FILE_MODE_READ, 
        EFI_FILE_READ_ONLY
    ));

    EFI_GUID info_guid = EFI_FILE_INFO_ID;
    EFI_STATUS info_status;
    EFI_FILE_INFO *file_info = NULL;
    UINTN size = SIZE_OF_EFI_FILE_INFO + 200;

    while (GrowBuffer(&info_status, (VOID **) &file_info, size)) 
    {
        info_status = file_handle->GetInfo(
            file_handle,
            &info_guid,
            &size,
            file_info
        );
    }

    *buffer = alloc(file_info->FileSize);
    file_handle->Read(
        file_handle,
        &file_info->FileSize,
        *buffer
    );

    free(file_info);
    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *SystemTable)
{
    ConOut = SystemTable->ConOut;
    Bs = SystemTable->BootServices;

    ConOut->ClearScreen(ConOut);
    check_status$(Bs->SetWatchdogTimer(0, 0, 0, NULL));
    log$("Navy Loader !");

    EFI_LOADED_IMAGE *image_loader;
    EFI_GUID img_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    check_status$(SystemTable->BootServices->OpenProtocol(
        handle,
        &img_guid,
        (void **) &image_loader,
        handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    ));

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *rootfs;
    EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    check_status$(SystemTable->BootServices->OpenProtocol(
        image_loader->DeviceHandle,
        &fs_guid,
        (void **) &rootfs,
        handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    ));

    EFI_FILE_PROTOCOL *rootdir;
    check_status$(rootfs->OpenVolume(rootfs, &rootdir));

    UINT8 *buffer = NULL;
    check_status$(load_file(rootdir, &buffer, L"\\boot\\kernel.elf"));

    if (buffer[0] != 0x7f || buffer[1] != 'E' || buffer[2] != 'L' || buffer[3] != 'F')
    {
        log$("Invalid ELF :/");
        for (;;);
    }

    for(;;);
    return EFI_SUCCESS;
}