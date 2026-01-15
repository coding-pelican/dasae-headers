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
    RMDIR = rmdir /S /Q
else
    PLATFORM = unix
    CC ?= clang
    EXE_EXT =
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
endif

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DAL_C_DIR = $(SRC_DIR)/dal-c
DAL_C_EXT_DIR = $(SRC_DIR)/dal-c-ext

# Source files
DAL_C_SRCS = $(DAL_C_DIR)/build.c \
             $(DAL_C_DIR)/Cmd.c \
             $(DAL_C_DIR)/Project.c

DAL_C_EXT_SRCS = $(DAL_C_EXT_DIR)/str.c \
                 $(DAL_C_EXT_DIR)/path.c \
                 $(DAL_C_EXT_DIR)/file.c \
                 $(DAL_C_EXT_DIR)/dir.c \
                 $(DAL_C_EXT_DIR)/env.c \
                 $(DAL_C_EXT_DIR)/proc.c \
                 $(DAL_C_EXT_DIR)/ArrStr.c

MAIN_SRC = $(SRC_DIR)/dal-c.c

ALL_SRCS = $(MAIN_SRC) $(DAL_C_SRCS) $(DAL_C_EXT_SRCS)

# Object files
OBJS = $(ALL_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/dh-c$(EXE_EXT)

# Compiler flags
CFLAGS = -std=gnu17 \
         -I$(INCLUDE_DIR) \
         -Wall -Wextra \
         -Werror=all -Werror=extra -Werror=conversion \
         -Werror=sign-conversion -Wfloat-conversion \
         -Werror=cast-qual -Werror=cast-align \
         -Wpointer-arith -Wbad-function-cast \
         -Wnull-dereference -Wwrite-strings \
         -Wswitch-enum -Winfinite-recursion \
         -Wloop-analysis -Werror=strict-prototypes \
         -Werror=missing-prototypes \
         -Wmissing-variable-declarations \
         -Werror=div-by-zero -Wthread-safety \
         -fgnu-keywords -fms-extensions \
         -funsigned-char -fblocks \
         -g

# Platform-specific flags
ifeq ($(PLATFORM),windows)
    CFLAGS += -D_WIN32_WINNT=0x0600
    LDFLAGS = -fuse-ld=lld
else
    CFLAGS += -D_POSIX_C_SOURCE=200809L
    LDFLAGS =
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
	@echo "Cleaning..."
ifeq ($(PLATFORM),windows)
	@if exist $(BUILD_DIR) $(RMDIR) $(BUILD_DIR)
else
	@$(RM) -r $(BUILD_DIR)
endif
	@echo "Clean complete"

# Install (copy to system path - optional)
install: $(TARGET)
ifeq ($(PLATFORM),windows)
	@echo "Installation not implemented for Windows"
else
	@echo "Installing dh-c..."
	@cp $(TARGET) /usr/local/bin/dh-c
	@chmod +x /usr/local/bin/dh-c
	@echo "Installation complete"
endif

# Phony targets
.PHONY: all clean install
EOF
echo "Makefile generated"
