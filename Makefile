CONFIG_ARCH ?= x86_64
LOADER ?= limine

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

ifeq ($(LOADER), navy)
	include loader/navy/.build.mk
else ifeq ($(LOADER), stivale)
	include loader/limine/.build.mk
else ifeq ($(LOADER), limine)
	include loader/limine/.build.mk
endif

$(CACHEDIR)/OVMF.fd:
	$(MKCWD)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd
	mv DEBUGX64_OVMF.fd $@

clean:
	rm -f $(BINS)
	rm -rf $(BINDIR)
	rm -rf $(CACHEDIR)

all: $(KERNEL) $(NAVY_NAMES)

.PHONY: clean run all 
.DEFAULT_GOAL := all
