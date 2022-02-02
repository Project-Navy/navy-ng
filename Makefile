MKCWD = @mkdir -p $(@D)
CACHEDIR = cache
BINDIR = bin
CONFIG_ARCH ?= x86_64

CROSS_CC = clang
CROSS_LD = clang
STD_CFLAGS = \
	-Wall \
	-Wextra \
	-Werror \
	-std=gnu2x \
	-MD \
	-Ilib \
	-Ilib/ansi \
	-Ilib/hw \
	-I. \
	-D__$(CONFIG_ARCH)__

BINS :=

include kernel/.build.mk
include loader/.build.mk

$(CACHEDIR)/OVMF.fd:
	$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

clean:
	rm $(BINS)
	rm -r $(BINDIR)
	rm -r $(CACHEDIR)

all: $(BINS)

run: $(KERNEL) $(LOADER) $(CACHEDIR)/OVMF.fd
	mkdir -p $(BINDIR_LOADER)/image/EFI/BOOT/ $(BINDIR_LOADER)/image/boot
	cp $(LOADER) $(BINDIR_LOADER)/image/EFI/BOOT/BOOTX64.EFI
	cp $(KERNEL) $(BINDIR_LOADER)/image/boot/kernel.elf

	qemu-system-x86_64 \
		-serial stdio \
		-no-reboot \
		-no-shutdown \
		-bios $(CACHEDIR)/OVMF.fd \
		-drive file=fat:rw:$(BINDIR_LOADER)/image,media=disk,format=raw

.PHONY: clean run all 
.DEFAULT_GOAL := all