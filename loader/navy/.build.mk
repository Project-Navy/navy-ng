LOADER_CFLAGS = \
	$(STD_CFLAGS) \
	-ffreestanding \
	-target x86_64-unknown-windows \
	-fno-stack-protector \
	-fshort-wchar \
	-mno-red-zone \
	-Iloader/navy/efi \
	-Ilib/ansi \
	-DLOADER

LOADER_LDFLAGS = \
	-target x86_64-unknown-windows \
	-nostdlib \
	-Wl,-entry:efi_main \
	-Wl,-subsystem:efi_application \
	-fuse-ld=lld-link

BINDIR_LOADER=$(BINDIR)/loader
LOADER_SRC = \
	loader/navy/main.c \
	loader/navy/memory.c \
	loader/navy/utils.c \
	lib/navy/fmt.c \
	lib/navy/itoa.c \
	lib/ansi/string.c \
	lib/ansi/stdlib.c

LOADER_OBJS = $(patsubst %, $(BINDIR_LOADER)/%.o, $(LOADER_SRC))
LOADER=$(BINDIR_LOADER)/BOOTX64.EFI
BINS += $(LOADER)

$(LOADER): $(LOADER_OBJS)
	@$(MKCWD)
	$(LD) -o $@ $^ $(LOADER_LDFLAGS)

$(BINDIR_LOADER)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(LOADER_CFLAGS)

copy-efi:
	mkdir -p $(BINDIR_LOADER)/image/EFI/BOOT/ $(BINDIR_LOADER)/image/boot
	cp $(LOADER) $(BINDIR_LOADER)/image/EFI/BOOT/BOOTX64.EFI
	cp $(KERNEL) $(BINDIR_LOADER)/image/boot/kernel.elf

run: $(KERNEL) $(LOADER) $(CACHEDIR)/OVMF.fd copy-efi
	$(QEMU) $(QEMUFLAGS) \
		-bios $(CACHEDIR)/OVMF.fd \
		-drive file=fat:rw:$(BINDIR_LOADER)/image,media=disk,format=raw

run-dbg: $(KERNEL) $(LOADER) $(CACHEDIR)/OVMF.fd copy-efi
	$(QEMU) $(QEMUFLAGS) \
		-bios $(CACHEDIR)/OVMF.fd \
		-s -S \
		-d int \
		-drive file=fat:rw:$(BINDIR_LOADER)/image,media=disk,format=raw

run-nogui: $(KERNEL) $(LOADER) $(CACHEDIR)/OVMF.fd copy-efi
	$(QEMU) $(QEMUFLAGS) \
		-nographic \
		-bios $(CACHEDIR)/OVMF.fd \
		-drive file=fat:rw:$(BINDIR_LOADER)/image,media=disk,format=raw