BINDIR_KERNEL = $(BINDIR)/kernel
KERNEL = $(BINDIR_KERNEL)/kernel.elf
BINS += $(KERNEL)

KERNEL_CFLAGS = \
	$(STD_CFLAGS) \
	--target=x86_64-none-elf \
	-ffreestanding \
	-fno-stack-protector \
    -fno-pic \
    -m64 \
    -ggdb \
    -O3 \
	-mno-80387 \
	-mno-mmx \
	-mno-3dnow \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-DKERNEL \
	-Ikernel/ \
	-I. \
	-Ilib/ansi

KERNEL_ASFLAGS = \
	-felf64 \

KERNEL_LDSCRIPT = kernel/hw/$(CONFIG_ARCH)/link_stivale.ld
KERNEL_LDFLAGS =  \
    -nostdlib \
    -static \
    -T$(KERNEL_LDSCRIPT) \
    -z max-page-size=0x1000 

KERNEL_SRC := \
	$(wildcard kernel/*.c) \
	$(wildcard kernel/hw/$(CONFIG_ARCH)/*.c) \
	$(wildcard kernel/hw/$(CONFIG_ARCH)/*.s) \
	$(wildcard lib/liballoc/*.c) \
	$(wildcard lib/brutal/*.c) \
	lib/ansi/string.c \
	lib/ansi/assert.c \
	lib/ansi/stdlib.c \
	lib/navy/fmt.c \
	lib/navy/itoa.c \
	lib/navy/debug.c \
	lib/navy/bitmap.c \
	lib/navy/handover.c \
	lib/navy/vec.c

ifeq ($(shell test -e loader/$(LOADER).c && echo -n yes),yes)
    KERNEL_SRC += loader/$(LOADER).c
endif

ifeq ($(shell test -e loader/$(LOADER).s && echo -n yes),yes)
    KERNEL_SRC += loader/$(LOADER).s
endif

KERNEL_OBJ := $(patsubst %, $(BINDIR_KERNEL)/%.o, $(KERNEL_SRC))
DEPENDENCIES += $(KERNEL_OBJ:.o=.d)

$(BINDIR_KERNEL)/%.s.o: %.s
	@$(MKCWD)
	$(AS) $(KERNEL_ASFLAGS) $< -o $@ 

$(BINDIR_KERNEL)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(KERNEL_CFLAGS)

$(KERNEL): $(KERNEL_OBJ)
	@$(MKCWD)
	$(LD) -o $@ $^ $(KERNEL_LDFLAGS)
