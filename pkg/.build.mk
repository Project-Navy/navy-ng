LIBS_HOST_SRC = \
	$(wildcard lib/navy/*.c) \
	$(wildcard lib/lison/*.c) \
	$(wildcard lib/brutal/*.c) \
	$(wildcard lib/cpython/*.c) \
	$(wildcard lib/rxi/*.c)

LIBS_HOST_OBJ = \
	$(patsubst %, $(BINDIR_HOST)/%.o, $(LIBS_HOST_SRC))

HOST_CFLAGS := \
	$(STD_CFLAGS_HOST) \
	-ggdb \
	-fsanitize=undefined \
	-fsanitize=address \
	-DHOST

HOST_LDFLAGS := \
	-fsanitize=undefined \
	-fsanitize=address \
	-lm

NAVY_CFLAGS := \
	$(STD_CFLAGS) \
	--target=x86_64-elf-none \
	-ffreestanding \
	-Ilib/ansi 

NAVY_LDFLAGS := \
	-nostdlib \
	-static \
	-Tkernel/hw/$(CONFIG_ARCH)/link.ld 

LIBS_HOST_BIN=$(BINDIR_HOST)/libnavy.a

$(BINDIR_HOST)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(HOST_CFLAGS)

$(LIBS_HOST_BIN): $(LIBS_HOST_OBJ)
	@$(MKCWD)
	$(AR) $(ARFLAGS) $@ $^

DEPENDENCIES += $(LIBS_HOST_OBJ:.o=.d)
include pkg/*/.build.mk

define HOST_TEMPLATE

HOST_NAMES+=$($(1)_NAME)

$(1)_HOST_SRC = \
	$(shell find ./pkg/$($(1)_NAME) -name "*.c")

$(1)_HOST_OBJ = $$(patsubst /%, $(BINDIR_HOST)/%.o, $$($(1)_HOST_SRC))
$(1)_HOST_BIN = $(BINDIR_HOST)/$($(1)_NAME)
BINS += $($(1)_HOST_BIN)

$$($(1)_NAME): $$($(1)_HOST_BIN)
	@$$($(1)_HOST_BIN)

$$($(1)_HOST_BIN): $$($(1)_HOST_OBJ) $(LIBS_HOST_BIN)
	@$$(MKCWD)
	$(CC) -o $$@ $$^ $(HOST_LDFLAGS) $(HOST_CFLAGS)

endef

define NAVY_TEMPLATE

NAVY_NAMES+=$($(1)_NAME)

$(1)_NAVY_SRC = \
	$(shell find ./pkg/$($(1)_NAME) -name "*.c") \
	$(shell find ./lib/ansi -name "*.c") \
	$(shell find ./lib/navy -name "*.c") \
	$(shell find ./lib/brutal -name "*.c") \
	$(wildcard lib/rxi/*.c) \
	$(wildcard lib/cpython/*.c)

$(1)_NAVY_OBJ = $$(patsubst /%, $(BINDIR_NAVY)/%.o, $$($(1)_NAVY_SRC))
$(1)_NAVY_BIN = $(BINDIR_NAVY)/$($(1)_NAME)
BINS += $($(1)_NAVY_BIN)

$$($(1)_NAME): $$($(1)_NAVY_BIN)

$$($(1)_NAVY_BIN): $$($(1)_NAVY_OBJ)
	@$$(MKCWD)
	$(CC) -o $$@ $$^ $(NAVY_LDFLAGS) $(NAVY_CFLAGS)

endef

list-host:
	@echo $(HOST_NAMES)

list-navy:
	@echo $(NAVY_NAMES)

$(foreach bin, $(HOST_PKGS), $(eval $(call HOST_TEMPLATE,$(bin))))
$(foreach bin, $(NAVY_PKGS), $(eval $(call NAVY_TEMPLATE,$(bin))))