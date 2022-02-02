LOADER_CFLAGS = \
	$(STD_CFLAGS) \
	-ffreestanding \
	-target x86_64-unknown-windows \
	-fno-stack-protector \
	-fshort-wchar \
	-mno-red-zone \
	-Iloader/efi \
	-DLOADER

LOADER_LDFLAGS = \
	-target x86_64-unknown-windows \
	-nostdlib \
	-Wl,-entry:efi_main \
	-Wl,-subsystem:efi_application \
	-fuse-ld=lld-link

BINDIR_LOADER=$(BINDIR)/loader
LOADER_SRC = \
	loader/main.c \
	lib/navy/fmt.c \
	lib/navy/itoa.c \
	lib/ansi/string.c \
	lib/ansi/stdlib.c

LOADER_OBJS = $(patsubst %, $(BINDIR_LOADER)/%.o, $(LOADER_SRC))
LOADER=$(BINDIR_LOADER)/BOOTX64.EFI
BINS += $(LOADER)

$(LOADER): $(LOADER_OBJS)
	@$(MKCWD)
	$(CROSS_LD) -o $@ $^ $(LOADER_LDFLAGS)

$(BINDIR_LOADER)/%.c.o: %.c
	@$(MKCWD)
	$(CROSS_CC) -c -o $@ $< $(LOADER_CFLAGS)