KERNEL_LDSCRIPT = kernel/hw/$(CONFIG_ARCH)/link_stivale.ld

ifneq ($(CONFIG_ARCH), x86_64)
	$(error Bad architecture)
endif

BOOTX64:
	mkdir -p $(SYSROOT)/EFI/BOOT
	wget https://github.com/limine-bootloader/limine/raw/v3.0-branch-binary/BOOTX64.EFI -O $(SYSROOT)/EFI/BOOT/$@.EFI

OVMF:
	mkdir -p $(CACHEDIR)
	wget 'https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd' -O $(CACHEDIR)/OVMF.fd

rootfs: $(KERNEL) $(NAVY_NAMES)
	mkdir -p $(SYSROOT)/bin
	cp $(KERNEL) $(SYSROOT)/boot/
	cp $(BINDIR_NAVY)/* $(SYSROOT)/bin
	rm $(SYSROOT)/bin/*.d

run: BOOTX64 OVMF rootfs
	$(QEMU) $(QEMUFLAGS) \
		-drive file=fat:rw:$(SYSROOT),media=disk,format=raw \
		-bios $(CACHEDIR)/OVMF.fd \
		-no-reboot

run-nogui: BOOTX64 OVMF rootfs
	$(QEMU) $(QEMUFLAGS) \
		-drive file=fat:rw:$(SYSROOT),media=disk,format=raw \
		-bios $(CACHEDIR)/OVMF.fd \
		-nographic \
		-no-reboot \

run-dbg: BOOTX64 OVMF rootfs
	$(QEMU) $(QEMUFLAGS) \
		-drive file=fat:rw:$(SYSROOT),media=disk,format=raw \
		-bios $(CACHEDIR)/OVMF.fd \
		-s -S \
		-d int

run-dbg-nogui: BOOTX64 OVMF rootfs
	$(QEMU) $(QEMUFLAGS) \
		-nographic \
		-bios $(CACHEDIR)/OVMF.fd \
		-drive file=fat:rw:$(SYSROOT),media=disk,format=raw \
		-s -S \
		-d int