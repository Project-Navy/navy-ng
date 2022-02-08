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
SYSROOT = sysroot

MKCWD = @mkdir -p $(@D)

QEMU := qemu-system-$(CONFIG_ARCH)
QEMUFLAGS := \
	-serial mon:stdio \
	-m 4G \
	-enable-kvm \
	-no-shutdown

STD_CFLAGS = \
	-Wall \
	-O3 \
	-ggdb \
	-Wextra \
	-Werror \
	-std=gnu2x \
	-Wvla \
	-MD \
	-Ilib \
	-Ilib/hw \
	-I. \
	-D__$(CONFIG_ARCH)__

BINS :=

include kernel/.build.mk
include pkg/.build.mk

ifeq ($(LOADER), navy)
	include loader/navy/.build.mk
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

all: $(BINS)

.PHONY: clean run all 
.DEFAULT_GOAL := $(KERNEL)
