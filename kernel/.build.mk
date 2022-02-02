BINDIR_KERNEL = $(BINDIR)/kernel
KERNEL = $(BINDIR_KERNEL)/kernel.elf
BINS += $(KERNEL)

KERNEL_CFLAGS = \
	$(STD_CFLAGS) \
	-ffreestanding \
	-fno-stack-protector \
    -fno-pic \
    -m64 \
    -ggdb \
    -Os \
	-mno-80387 \
	-mno-mmx \
	-mno-3dnow \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-DKERNEL \
	-Ikernel/

KERNEL_LDFLAGS =  \
    -nostdlib \
    -static \
    -Tkernel/hw/$(CONFIG_ARCH)/link.ld \
    -z max-page-size=0x1000 \

KERNEL_SRC = \
	$(wildcard kernel/*.c) \
	$(wildcard kernel/hw/$(CONFIG_ARCH)/*.c) \
	lib/ansi/string.c \
	lib/navy/fmt.c \
	lib/navy/itoa.c \
	lib/navy/debug.c \


KERNEL_OBJ = $(patsubst %, $(BINDIR_KERNEL)/%.o, $(KERNEL_SRC))
DEPENDENCIES += $(KERNEL_OBJ:.o=.d)

$(BINDIR_KERNEL)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(KERNEL_CFLAGS)

$(KERNEL): $(KERNEL_OBJ)
	@$(MKCWD)
	$(LD) -o $@ $^ $(KERNEL_LDFLAGS)
