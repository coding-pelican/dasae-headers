#!/bin/sh
cat > Makefile << 'EOF'
# Makefile for building dh-c
# Cross-platform build system

# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    EXE_EXT = .exe
    RM = del /Q
    MKDIR = mkdir -p
    RMDIR = rm -rf
else
    PLATFORM = unix
    CC ?= clang
    EXE_EXT =
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
endif

# Self-build profile (matches `dh-c` runtime profile names)
PROFILE ?= dev

CC = clang
C_STD ?= gnu17
ARCH_TARGET ?= auto
TARGET_ARCH ?= profile
TARGET_ABI ?= auto
SYSROOT ?= auto
LTO ?= profile
OMIT_FRAME_POINTER ?= profile
FUNCTION_SECTIONS ?= profile
DATA_SECTIONS ?= profile
GC_SECTIONS ?= profile
WHOLE_ARCHIVE ?= profile
UNROLL_LOOPS ?= profile
UNWIND_TABLES ?= profile
ASYNC_UNWIND_TABLES ?= profile
STRIP ?= profile
ICF ?= profile
EXTRA_LIBS ?=

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_ROOT = build
BUILD_DIR = $(BUILD_ROOT)/$(PROFILE)
DAL_C_DIR = $(SRC_DIR)/dal-c
DAL_C_EXT_DIR = $(SRC_DIR)/dal-c-ext

# Source files
DAL_C_SRCS = $(DAL_C_DIR)/build.c \
             $(DAL_C_DIR)/Cmd.c \
             $(DAL_C_DIR)/Project.c

DAL_C_EXT_SRCS = $(DAL_C_EXT_DIR)/str.c \
                 $(DAL_C_EXT_DIR)/path.c \
                 $(DAL_C_EXT_DIR)/dir.c \
                 $(DAL_C_EXT_DIR)/file.c \
                 $(DAL_C_EXT_DIR)/env.c \
                 $(DAL_C_EXT_DIR)/proc.c \
                 $(DAL_C_EXT_DIR)/ArrStr.c

MAIN_SRC = $(SRC_DIR)/dal-c.c

ALL_SRCS = $(MAIN_SRC) $(DAL_C_SRCS) $(DAL_C_EXT_SRCS)

# Object files
OBJS = $(ALL_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/dh-c$(EXE_EXT)
PROJECT_DH = project.dh
ifeq ($(origin BUILD_STAMP), undefined)
BUILD_STAMP := $(shell date +%Y%m%d.%H%M%S)
endif
VER_CORE := $(strip $(shell sed -n 's/^[[:space:]]*version-core[[:space:]]*=[[:space:]]*//p' $(PROJECT_DH) 2>/dev/null | tail -n 1))
VER_PREFIX := $(strip $(shell sed -n 's/^[[:space:]]*version-prefix[[:space:]]*=[[:space:]]*//p' $(PROJECT_DH) 2>/dev/null | tail -n 1))
VER_SUFFIX := $(strip $(shell sed -n 's/^[[:space:]]*version-suffix[[:space:]]*=[[:space:]]*//p' $(PROJECT_DH) 2>/dev/null | tail -n 1))
VER_BUILD := $(strip $(shell sed -n 's/^[[:space:]]*version-build[[:space:]]*=[[:space:]]*//p' $(PROJECT_DH) 2>/dev/null | tail -n 1))
VER_MAJOR := $(word 1,$(subst ., ,$(VER_CORE)))
VER_MINOR := $(word 2,$(subst ., ,$(VER_CORE)))
VER_PATCH := $(word 3,$(subst ., ,$(VER_CORE)))
VERSION_DEFINES =
ifneq ($(strip $(VER_MAJOR)),)
    VERSION_DEFINES += -Ddal_c__NUM__VER_CORE_MAJOR=$(VER_MAJOR)
endif
ifneq ($(strip $(VER_MINOR)),)
    VERSION_DEFINES += -Ddal_c__NUM__VER_CORE_MINOR=$(VER_MINOR)
endif
ifneq ($(strip $(VER_PATCH)),)
    VERSION_DEFINES += -Ddal_c__NUM__VER_CORE_PATCH=$(VER_PATCH)
endif
ifeq ($(VER_PREFIX),alpha)
    VERSION_DEFINES += -Ddal_c__NUM__VER_LABEL_PREFIX=0 -Ddal_c__STR__VER_LABEL_PREFIX=\"alpha\"
else ifeq ($(VER_PREFIX),beta)
    VERSION_DEFINES += -Ddal_c__NUM__VER_LABEL_PREFIX=1 -Ddal_c__STR__VER_LABEL_PREFIX=\"beta\"
else ifeq ($(VER_PREFIX),rc)
    VERSION_DEFINES += -Ddal_c__NUM__VER_LABEL_PREFIX=2 -Ddal_c__STR__VER_LABEL_PREFIX=\"rc\"
endif
ifneq ($(strip $(VER_PREFIX)),)
ifneq ($(strip $(VER_SUFFIX)),)
    VERSION_DEFINES += -Ddal_c__NUM__VER_LABEL_SUFFIX=$(VER_SUFFIX) -Ddal_c__STR__VER_LABEL_SUFFIX=\"$(VER_SUFFIX)\"
endif
endif
VERSION_BUILD_VALUE = $(if $(strip $(VER_BUILD)),$(VER_BUILD),$(BUILD_STAMP))

# Compiler flags
BASE_CFLAGS = -std=$(C_STD) \
              -I$(INCLUDE_DIR) \
              -Wall -Wextra \
              -Werror=all -Werror=extra -Werror=conversion \
              -Werror=sign-conversion -Wfloat-conversion \
              -Wformat=2 \
              -Werror=cast-qual -Werror=cast-align \
              -Wpointer-arith -Wbad-function-cast \
              -Wnull-dereference -Wwrite-strings \
              -Werror=uninitialized \
              -Wframe-larger-than=4096 \
              -Wno-switch-enum -Winfinite-recursion \
              -Wloop-analysis -Werror=strict-prototypes \
              -Werror=missing-prototypes \
              -Wmissing-variable-declarations \
              -Werror=div-by-zero -Wthread-safety \
              -fgnu-keywords -fms-extensions -Wno-microsoft-anon-tag \
              -funsigned-char \
              -mllvm -enable-dfa-jump-thread \
              $(VERSION_DEFINES) \
              -Ddal_c__STR__VER_BUILD=\"$(VERSION_BUILD_VALUE)\" \
              -static

PROFILE_CFLAGS =
PROFILE_LDFLAGS =
PROFILE_LTO = auto
PROFILE_OMIT_FRAME_POINTER = auto
PROFILE_FUNCTION_SECTIONS = auto
PROFILE_DATA_SECTIONS = auto
PROFILE_GC_SECTIONS = auto
PROFILE_WHOLE_ARCHIVE = auto
PROFILE_UNROLL_LOOPS = auto
PROFILE_UNWIND_TABLES = auto
PROFILE_ASYNC_UNWIND_TABLES = auto
PROFILE_STRIP = auto
PROFILE_ICF = auto
PROFILE_TARGET_ARCH = auto

ifeq ($(PROFILE),dev)
    PROFILE_CFLAGS += -g3 -Og
    PROFILE_OMIT_FRAME_POINTER = off
else ifeq ($(PROFILE),test)
    PROFILE_CFLAGS += -g -O1
    PROFILE_OMIT_FRAME_POINTER = off
else ifeq ($(PROFILE),profile)
    PROFILE_CFLAGS += -g -O2
    PROFILE_OMIT_FRAME_POINTER = off
else ifeq ($(PROFILE),stable)
    PROFILE_CFLAGS += -g1 -O2
else ifeq ($(PROFILE),release)
    PROFILE_CFLAGS += -g1 -O3
    PROFILE_LTO = on
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_STRIP = on
    PROFILE_ICF = all
else ifeq ($(PROFILE),optimize)
    PROFILE_CFLAGS += -O3
    PROFILE_LTO = on
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_STRIP = on
    PROFILE_ICF = all
    PROFILE_TARGET_ARCH = native
else ifeq ($(PROFILE),compact)
    PROFILE_CFLAGS += -Os
    PROFILE_LTO = on
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_STRIP = on
    PROFILE_ICF = all
else ifeq ($(PROFILE),micro)
    PROFILE_CFLAGS += -Oz
    PROFILE_LTO = on
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNROLL_LOOPS = off
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_STRIP = on
    PROFILE_ICF = all
else
    $(error Unsupported PROFILE '$(PROFILE)')
endif

RESOLVED_LTO = $(if $(filter profile,$(LTO)),$(PROFILE_LTO),$(LTO))
RESOLVED_OMIT_FRAME_POINTER = $(if $(filter profile,$(OMIT_FRAME_POINTER)),$(PROFILE_OMIT_FRAME_POINTER),$(OMIT_FRAME_POINTER))
RESOLVED_FUNCTION_SECTIONS = $(if $(filter profile,$(FUNCTION_SECTIONS)),$(PROFILE_FUNCTION_SECTIONS),$(FUNCTION_SECTIONS))
RESOLVED_DATA_SECTIONS = $(if $(filter profile,$(DATA_SECTIONS)),$(PROFILE_DATA_SECTIONS),$(DATA_SECTIONS))
RESOLVED_GC_SECTIONS = $(if $(filter profile,$(GC_SECTIONS)),$(PROFILE_GC_SECTIONS),$(GC_SECTIONS))
RESOLVED_WHOLE_ARCHIVE = $(if $(filter profile,$(WHOLE_ARCHIVE)),$(PROFILE_WHOLE_ARCHIVE),$(WHOLE_ARCHIVE))
RESOLVED_UNROLL_LOOPS = $(if $(filter profile,$(UNROLL_LOOPS)),$(PROFILE_UNROLL_LOOPS),$(UNROLL_LOOPS))
RESOLVED_UNWIND_TABLES = $(if $(filter profile,$(UNWIND_TABLES)),$(PROFILE_UNWIND_TABLES),$(UNWIND_TABLES))
RESOLVED_ASYNC_UNWIND_TABLES = $(if $(filter profile,$(ASYNC_UNWIND_TABLES)),$(PROFILE_ASYNC_UNWIND_TABLES),$(ASYNC_UNWIND_TABLES))
RESOLVED_STRIP = $(if $(filter profile,$(STRIP)),$(PROFILE_STRIP),$(STRIP))
RESOLVED_ICF = $(if $(filter profile,$(ICF)),$(PROFILE_ICF),$(ICF))
RESOLVED_TARGET_ARCH = $(if $(filter profile,$(TARGET_ARCH)),$(PROFILE_TARGET_ARCH),$(TARGET_ARCH))

ifeq ($(RESOLVED_LTO),on)
    PROFILE_CFLAGS += -flto
    PROFILE_LDFLAGS += -flto
else ifeq ($(RESOLVED_LTO),off)
    PROFILE_CFLAGS += -fno-lto
    PROFILE_LDFLAGS += -fno-lto
else ifneq ($(RESOLVED_LTO),auto)
    $(error Unsupported LTO '$(LTO)')
endif

ifeq ($(RESOLVED_OMIT_FRAME_POINTER),on)
    PROFILE_CFLAGS += -fomit-frame-pointer
else ifeq ($(RESOLVED_OMIT_FRAME_POINTER),off)
    PROFILE_CFLAGS += -fno-omit-frame-pointer
else ifneq ($(RESOLVED_OMIT_FRAME_POINTER),auto)
    $(error Unsupported OMIT_FRAME_POINTER '$(OMIT_FRAME_POINTER)')
endif

ifeq ($(RESOLVED_FUNCTION_SECTIONS),on)
    PROFILE_CFLAGS += -ffunction-sections
else ifeq ($(RESOLVED_FUNCTION_SECTIONS),off)
else ifeq ($(RESOLVED_FUNCTION_SECTIONS),auto)
else
    $(error Unsupported FUNCTION_SECTIONS '$(FUNCTION_SECTIONS)')
endif

ifeq ($(RESOLVED_DATA_SECTIONS),on)
    PROFILE_CFLAGS += -fdata-sections
else ifeq ($(RESOLVED_DATA_SECTIONS),off)
else ifeq ($(RESOLVED_DATA_SECTIONS),auto)
else
    $(error Unsupported DATA_SECTIONS '$(DATA_SECTIONS)')
endif

ifeq ($(RESOLVED_GC_SECTIONS),on)
    PROFILE_LDFLAGS += -Wl,--gc-sections
else ifeq ($(RESOLVED_GC_SECTIONS),off)
else ifeq ($(RESOLVED_GC_SECTIONS),auto)
else
    $(error Unsupported GC_SECTIONS '$(GC_SECTIONS)')
endif

ifeq ($(RESOLVED_UNROLL_LOOPS),on)
    PROFILE_CFLAGS += -funroll-loops
else ifeq ($(RESOLVED_UNROLL_LOOPS),off)
    PROFILE_CFLAGS += -fno-unroll-loops
else ifneq ($(RESOLVED_UNROLL_LOOPS),auto)
    $(error Unsupported UNROLL_LOOPS '$(UNROLL_LOOPS)')
endif

TARGET_FLAGS =
ifneq ($(ARCH_TARGET),auto)
    TARGET_FLAGS += -target $(ARCH_TARGET)
endif
ifneq ($(RESOLVED_TARGET_ARCH),auto)
    TARGET_FLAGS += -march=$(RESOLVED_TARGET_ARCH)
endif
ifneq ($(TARGET_ABI),auto)
    TARGET_FLAGS += -mabi=$(TARGET_ABI)
endif
ifneq ($(SYSROOT),auto)
    TARGET_FLAGS += --sysroot=$(SYSROOT)
endif

ifeq ($(RESOLVED_UNWIND_TABLES),off)
    PROFILE_CFLAGS += -fno-unwind-tables
else ifeq ($(RESOLVED_UNWIND_TABLES),on)
    PROFILE_CFLAGS += -funwind-tables
else ifneq ($(RESOLVED_UNWIND_TABLES),auto)
    $(error Unsupported UNWIND_TABLES '$(UNWIND_TABLES)')
endif

ifeq ($(RESOLVED_ASYNC_UNWIND_TABLES),off)
    PROFILE_CFLAGS += -fno-asynchronous-unwind-tables
else ifeq ($(RESOLVED_ASYNC_UNWIND_TABLES),on)
    PROFILE_CFLAGS += -fasynchronous-unwind-tables
else ifneq ($(RESOLVED_ASYNC_UNWIND_TABLES),auto)
    $(error Unsupported ASYNC_UNWIND_TABLES '$(ASYNC_UNWIND_TABLES)')
endif

ifeq ($(RESOLVED_STRIP),on)
    PROFILE_LDFLAGS += -Wl,--strip-all
else ifeq ($(RESOLVED_STRIP),off)
else ifeq ($(RESOLVED_STRIP),auto)
else
    $(error Unsupported STRIP '$(STRIP)')
endif

ifeq ($(RESOLVED_ICF),safe)
    PROFILE_LDFLAGS += -Wl,--icf=safe
else ifeq ($(RESOLVED_ICF),all)
    PROFILE_LDFLAGS += -Wl,--icf=all
else ifeq ($(RESOLVED_ICF),off)
else ifeq ($(RESOLVED_ICF),auto)
else
    $(error Unsupported ICF '$(ICF)')
endif

LINK_LIBS = $(EXTRA_LIBS)
ifeq ($(RESOLVED_WHOLE_ARCHIVE),on)
ifneq ($(strip $(EXTRA_LIBS)),)
    LINK_LIBS = -Wl,--whole-archive $(EXTRA_LIBS) -Wl,--no-whole-archive
endif
else ifeq ($(RESOLVED_WHOLE_ARCHIVE),off)
else ifeq ($(RESOLVED_WHOLE_ARCHIVE),auto)
else
    $(error Unsupported WHOLE_ARCHIVE '$(WHOLE_ARCHIVE)')
endif

CFLAGS = $(BASE_CFLAGS) $(PROFILE_CFLAGS) $(TARGET_FLAGS)

# Platform-specific flags
ifeq ($(PLATFORM),windows)
    CFLAGS += -D_WIN32_WINNT=0x0600
    LDFLAGS = -fuse-ld=lld $(TARGET_FLAGS) $(PROFILE_LDFLAGS)
else
    CFLAGS += -D_POSIX_C_SOURCE=200809L
    LDFLAGS = $(TARGET_FLAGS) $(PROFILE_LDFLAGS)
endif

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	@$(MKDIR) $(dir $@)
	@echo "[LD] $@"
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LINK_LIBS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo "[CC] $<"
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(OBJS:.o=.d)

# Clean build artifacts
clean:
	@echo "Cleaning $(BUILD_ROOT)/ (ignoring PROFILE=$(PROFILE))..."
	@$(RMDIR) $(BUILD_ROOT) 2>/dev/null || true
	@echo "Clean complete"

clean-profile:
	@echo "Cleaning $(BUILD_DIR)/..."
	@$(RMDIR) $(BUILD_DIR) 2>/dev/null || true
	@echo "Clean complete"

# Install (copy to system path - optional)
ifeq ($(PLATFORM),windows)
install: $(TARGET)
	@echo "Installation not implemented for Windows"
else
install: $(TARGET)
	@echo "Installing dh-c..."
	@cp $(TARGET) /usr/local/bin/dh-c
	@chmod +x /usr/local/bin/dh-c
	@echo "Installation complete"
endif

# Phony targets
.PHONY: all clean clean-profile install
EOF
echo "Makefile generated"
