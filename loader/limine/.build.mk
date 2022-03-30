ISO = $(BINDIR)/navy.iso

check_submodule:
	git submodule update --init

KERNEL_LDSCRIPT = kernel/hw/$(CONFIG_ARCH)/link_stivale.ld

ifneq ($(CONFIG_ARCH), x86_64)
	$(error Bad architecture)
endif

$(ISO): check_submodule $(KERNEL)
	make -C ./loader/limine/limine
	mkdir -p $(CACHEDIR)/tmp/boot
	cp -r $(SYSROOT)/* $(CACHEDIR)/tmp/
	cp $(KERNEL) $(CACHEDIR)/tmp/boot/kernel.elf
	cp ./loader/limine/limine/{limine-cd.bin,limine-eltorito-efi.bin,limine.sys} $(CACHEDIR)/tmp/boot/

	xorriso -as mkisofs -b /boot/limine-cd.bin \
    	-no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot /boot/limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        $(CACHEDIR)/tmp/ -o $(ISO)

	./loader/limine/limine/limine-s2deploy $@
	rm -rf $(CACHEDIR)/tmp/

run: $(ISO)
	$(QEMU) $(QEMUFLAGS) \
		-cdrom $< \
		-no-reboot

run-nogui: $(ISO)
	$(QEMU) $(QEMUFLAGS) \
		-cdrom $< \
		-nographic \
		-no-reboot

run-dbg: $(ISO)
	$(QEMU) $(QEMUFLAGS) \
		-cdrom $< \
		-s -S \
		-d int

run-dbg-nogui: $(ISO)
	$(QEMU) $(QEMUFLAGS) \
		-cdrom $< \
		-nographic \
		-s -S \
		-d int