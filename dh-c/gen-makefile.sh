#!/bin/sh
cat > Makefile << 'EOF'
# Makefile for building dh-c
# Cross-platform build system

# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    CC = clang
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
BASE_CFLAGS = -std=gnu17 \
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

ifeq ($(PROFILE),dev)
    PROFILE_CFLAGS += -g3 -Og -fno-omit-frame-pointer
else ifeq ($(PROFILE),test)
    PROFILE_CFLAGS += -g -O1 -fno-omit-frame-pointer
else ifeq ($(PROFILE),profile)
    PROFILE_CFLAGS += -g -O2 -fno-omit-frame-pointer
else ifeq ($(PROFILE),stable)
    PROFILE_CFLAGS += -g1 -O2
else ifeq ($(PROFILE),release)
    PROFILE_CFLAGS += -g1 -O3 -flto -ffunction-sections -fdata-sections
    PROFILE_LDFLAGS += -flto -Wl,--gc-sections
else ifeq ($(PROFILE),optimize)
    PROFILE_CFLAGS += -O3 -march=native -flto -ffunction-sections -fdata-sections
    PROFILE_LDFLAGS += -flto
else ifeq ($(PROFILE),compact)
    PROFILE_CFLAGS += -Os -flto -ffunction-sections -fdata-sections
    PROFILE_LDFLAGS += -flto -Wl,--gc-sections
else ifeq ($(PROFILE),micro)
    PROFILE_CFLAGS += -Oz -fno-unroll-loops -flto
    PROFILE_LDFLAGS += -flto
else
    $(error Unsupported PROFILE '$(PROFILE)')
endif

CFLAGS = $(BASE_CFLAGS) $(PROFILE_CFLAGS)

# Platform-specific flags
ifeq ($(PLATFORM),windows)
    CFLAGS += -D_WIN32_WINNT=0x0600
    LDFLAGS = -fuse-ld=lld $(PROFILE_LDFLAGS)
else
    CFLAGS += -D_POSIX_C_SOURCE=200809L
    LDFLAGS = $(PROFILE_LDFLAGS)
endif

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	@$(MKDIR) $(dir $@)
	@echo "[LD] $@"
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

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
