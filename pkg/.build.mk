LIBS_HOST_SRC = \
	$(shell find ./lib -name "*.c")

LIBS_HOST_OBJ = \
	$(patsubst %, $(BINDIR_HOST)/%.o, $(LIBS_HOST_SRC))

HOST_CFLAGS := \
	$(STD_CFLAGS) \
	-fsanitize=undefined \
	-fsanitize=address \
	-nostdlib

HOST_LDFLAGS := \
	-fsanitize=undefined \
	-fsanitize=address \
	-nostdlib

LIBS_HOST_BIN=$(BINDIR_HOST)/libnavy.a

$(BINDIR_HOST)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(HOST_CFLAGS)

$(LIBS_HOST_BIN): $(LIBS_HOST_OBJ)
	@$(MKCWD)
	$(HOST_AR) $(HOST_ARFLAGS) $@ $^

DEPENDENCIES += $(LIBS_HOST_OBJ:.o=.d)
include pkg/*/.build.mk

define HOST_TEMPLATE

HOST_NAMES+=$($(1)_NAME)

$(1)_HOST_SRC = \
	$(shell find ./pkg/$($(1)_NAME) -name "*.c")

$(1)_HOST_BIN = $(BINDIR_HOST)/$($(1)_NAME)
BINS += $($(1)_NAME)

$$($(1)_NAME): $$($(1)_HOST_BIN)

$$($(1)_HOST_BIN): $$($(1)_HOST_OBJ) $(LIBS_HOST_BIN)
	@$$(MKCWD)
	$(CC) -o $$@ $$^ $(HOST_LDFLAGS) $(HOST_CFLAGS)

endef

list-host:
	@echo $(HOST_NAMES)

$(foreach bin, $(HOST_PKGS), $(eval $(call HOST_TEMPLATE,$(bin))))