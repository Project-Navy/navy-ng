CONFIG_ARCH ?= x86_64
LOADER ?= stivale

ifeq ($(CONFIG_ARCH), x86_64)
	AS := nasm
endif

CC := clang
LD := clang
AR := llvm-ar
ARFLAGS := rcs

CACHEDIR = cache
BINDIR = bin
BINDIR_HOST = bin/host
BINDIR_NAVY = bin/navy
SYSROOT = sysroot

MKCWD = @mkdir -p $(@D)

QEMU ?= qemu-system-$(CONFIG_ARCH)
QEMUFLAGS := \
	-m 4G \
	-enable-kvm \
	-no-shutdown

STD_CFLAGS = \
	-Wall \
	-ggdb \
	-Wextra \
	-Werror \
	-std=gnu2x \
	-Wvla \
	-MD \
	-Ilib/ \
	-D__$(CONFIG_ARCH)__

BINS :=

include kernel/.build.mk
include pkg/.build.mk
include loader/.build.mk

clean:
	rm -f $(BINS) $(SYSROOT)/EFI/BOOT/BOOTX64.EFI
	rm -rf $(BINDIR)
	rm -rf $(CACHEDIR)

all: $(KERNEL) $(NAVY_NAMES)

.PHONY: clean run all
.DEFAULT_GOAL := all
