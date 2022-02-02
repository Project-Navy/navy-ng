#include <navy/fmt.h>
#include <navy/elf.h>
#include <string.h>
#include <Uefi.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#ifdef __x86_64
#   define MMAP_KERNEL_BASE ((EFI_PHYSICAL_ADDRESS) 0xffffffff80000000)
#else
#   error "Unknown architecture"
#endif

#define align_up$(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))
#define log$(FORMAT, ...) \
	log_impl(__LINE__, FORMAT, PRINT_ARGS(__VA_ARGS__));

#define check_status$(status)                           \
    if (status != EFI_SUCCESS)                          \
    {                                                   \
        log$("Unsucessful EFI status: {x}", status);    \
        return status;                                  \
    }                                                   \

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
static EFI_BOOT_SERVICES *Bs;
static EFI_MEMORY_TYPE gKernelAndModulesMemoryType = 0x80000000;

static VOID cstr_stdout(char const *s)
{
    while (*s)
    {
        UINT16 out[2] = {0};
        out[0] = (UINT16) *s++;
        ConOut->OutputString(ConOut, out);
    }
}

static VOID log_impl(size_t line_nbr, char const *format, struct fmt_args args)
{
    print_format(cstr_stdout, "Navy Loader: {} ", line_nbr);
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

static VOID free(VOID *ptr)
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

static EFI_STATUS load_file (EFI_FILE_PROTOCOL *rootdir, UINT8 **buffer, CHAR16 *filename)
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

static EFI_STATUS load_kernel(UINT8 const *buffer)
{
    EFI_PHYSICAL_ADDRESS header_addr = (EFI_PHYSICAL_ADDRESS) buffer;
    Elf64_Ehdr *elf = (VOID *) buffer;
    Elf64_Phdr *phdr = (void *) (header_addr + elf->e_phoff);

    for (UINTN i = 0; i < elf->e_phnum; i++)
    {
        if (phdr->p_type == PT_LOAD)
        {
            UINTN npages = EFI_SIZE_TO_PAGES(align_up$(phdr->p_memsz, EFI_PAGE_SIZE));
            EFI_PHYSICAL_ADDRESS base = phdr->p_vaddr;
            log$("Mapping {a} -> {a} ({})", header_addr + phdr->p_offset, phdr->p_vaddr , phdr->p_type);
            check_status$(Bs->AllocatePages(AllocateAddress, gKernelAndModulesMemoryType, npages, &base));

            memcpy((VOID *) base, (VOID *)(header_addr + phdr->p_offset), phdr->p_filesz);
            memset(((VOID *) base) + phdr->p_filesz, 0, phdr->p_memsz - phdr->p_filesz);
        }

        phdr = (Elf64_Phdr *)((UINT8 *)phdr + elf->e_phentsize);
    }

    // TODO MAP THE KERNEL TO HIGHER HALF (Paging hell is back)

    log$("Entry poing at {a}", elf->e_entry);
    log$("Loading kernel");

    (*((int(*__attribute__((sysv_abi)))(void))(elf->e_entry)))();
    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *SystemTable)
{
    ConOut = SystemTable->ConOut;
    Bs = SystemTable->BootServices;

    ConOut->ClearScreen(ConOut);
    check_status$(Bs->SetWatchdogTimer(0, 0, 0, NULL));

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
    check_status$(load_kernel(buffer));

    for (;;);
}