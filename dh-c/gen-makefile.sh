#!/bin/sh
cat > Makefile << 'EOF'
# Makefile for building dh-c
# Cross-platform build system

.SUFFIXES:
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --no-print-directory
COMMANDS ?= hide
PROGRESS ?= show
VERBOSE ?= off
# Compatibility with the old self-build knob. Prefer COMMANDS=show|hide.
ifeq ($(V),1)
COMMANDS := show
endif
ifeq ($(VERBOSE),on)
COMMANDS := show
endif
ifeq ($(COMMANDS),show)
Q :=
PROGRESS := hide
else
Q := @
endif
ifeq ($(PROGRESS),show)
P_CC = printf '[CC] %s\n' '$1';
P_LD = printf '[LD] %s\n' '$1';
else
P_CC =
P_LD =
endif


# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    EXE_EXT = .exe
    RM = del /Q
    MKDIR = mkdir -p
    RMDIR = rm -rf
else
    PLATFORM = unix
    EXE_EXT =
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
endif

# Self-build profile (matches `dh-c` runtime profile names)
PROFILE ?= dev

ifeq ($(origin CC),default)
CC = clang
endif
CC ?= clang
C_STD ?= gnu17
COMPILE_ENV ?= auto
ARCH_TARGET ?= auto
TARGET_ARCH ?= profile
TARGET_TUNE ?= profile
TARGET_ABI ?= auto
SYSROOT ?= auto
LTO ?= profile
OMIT_FRAME_POINTER ?= profile
FUNCTION_SECTIONS ?= profile
DATA_SECTIONS ?= profile
GC_SECTIONS ?= profile
HOST_IS_WINDOWS := $(if $(filter Windows_NT,$(OS)),on,off)
LINK_LIBC ?= auto
LINK_DEFAULT_LIBS ?= auto
LINK_START_FILES ?= auto
LINK_COMPILER_RT ?= auto
LINK_MODE ?= auto
WHOLE_ARCHIVE ?= profile
UNROLL_LOOPS ?= profile
UNWIND_TABLES ?= profile
ASYNC_UNWIND_TABLES ?= profile
EXCEPTIONS ?= profile
STRIP ?= profile
ICF ?= profile
MERGE_ALL_CONSTANTS ?= auto
STACK_PROTECTOR ?= auto
MACRO_BACKTRACE_LIMIT ?= 8
LOOSE_ERRORS ?= auto
SAVE_TEMPS ?= off
EMIT_MAP ?= off
MAP_PATH ?=
EMIT_PREPROCESSED ?= off
PREPROCESSED_PATH ?=
EMIT_ASM ?= off
ASM_PATH ?=
EMIT_IR ?= off
IR_PATH ?=
EMIT_LINKED_ASM ?= off
LINKED_ASM_PATH ?=
EMIT_DISASM ?= off
DISASM_PATH ?=
EMIT_DEBUG_INFO ?= off
DEBUG_INFO_PATH ?=
PRINT_LINK_GC ?= off
ANALYSIS_ARTIFACTS ?= off
LLVM_OBJDUMP ?= llvm-objdump
LLVM_DWARFDUMP ?= llvm-dwarfdump
OBJCOPY ?= llvm-objcopy
OBJCOPY_FORMAT ?= binary
DISASM_DEMANGLE ?= auto
DISASM_SOURCE ?= auto
DISASM_LINE_NUMBERS ?= auto
DISASM_SYMBOLIZE_OPERANDS ?= auto
DISASM_RAW_INSN ?= auto
DISASM_SECTION_CONTENTS ?= auto
DISASM_FLAGS ?=
COMP_ARGS ?=
LINK_ARGS ?=
LINKER_SCRIPT ?=
ENTRY ?=
EXTRA_INCLUDES ?=
EXTRA_ISYSTEMS ?=
EXTRA_DEFINES ?=
EXTRA_UNDEFS ?=
EXTRA_LIBS ?=
EXTRA_LIB_DIRS ?=

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
             $(DAL_C_DIR)/Project.c \
             $(DAL_C_DIR)/archive.c

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
              -fgnu-keywords -fms-extensions -Wno-microsoft-anon-tag \
              -fcolor-diagnostics \
              -fmacro-backtrace-limit=$(MACRO_BACKTRACE_LIMIT) \
              -funsigned-char \
              -mllvm -enable-dfa-jump-thread \
              $(VERSION_DEFINES) \
              -Ddal_c__STR__VER_BUILD=\"$(VERSION_BUILD_VALUE)\"

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
PROFILE_EXCEPTIONS = auto
PROFILE_STRIP = auto
PROFILE_ICF = auto
PROFILE_TARGET_ARCH = auto
PROFILE_TARGET_TUNE = auto
PROFILE_DEBUG_ASSERTIONS = on
PROFILE_DEBUG_INFO = off

ifeq ($(PROFILE),dev)
    PROFILE_CFLAGS += -g3 -Og
    PROFILE_OMIT_FRAME_POINTER = off
    PROFILE_DEBUG_INFO = on
else ifeq ($(PROFILE),fast)
    PROFILE_CFLAGS += -O0
    PROFILE_LTO = off
    PROFILE_OMIT_FRAME_POINTER = off
    PROFILE_FUNCTION_SECTIONS = off
    PROFILE_DATA_SECTIONS = off
    PROFILE_GC_SECTIONS = off
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
else ifeq ($(PROFILE),test)
    PROFILE_CFLAGS += -g -O1
    PROFILE_OMIT_FRAME_POINTER = off
    PROFILE_DEBUG_INFO = on
else ifeq ($(PROFILE),profile)
    PROFILE_CFLAGS += -g -O2
    PROFILE_OMIT_FRAME_POINTER = off
    PROFILE_DEBUG_INFO = on
else ifeq ($(PROFILE),stable)
    PROFILE_CFLAGS += -g1 -O2
    PROFILE_DEBUG_ASSERTIONS = off
    PROFILE_DEBUG_INFO = on
    PROFILE_LTO = thin
else ifeq ($(PROFILE),release)
    PROFILE_CFLAGS += -g1 -O3
    PROFILE_DEBUG_ASSERTIONS = off
    PROFILE_DEBUG_INFO = on
    PROFILE_LTO = thin
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_EXCEPTIONS = off
    PROFILE_STRIP = on
    PROFILE_ICF = safe
else ifeq ($(PROFILE),optimize)
    PROFILE_CFLAGS += -O3
    PROFILE_DEBUG_ASSERTIONS = off
    PROFILE_LTO = full
    PROFILE_FUNCTION_SECTIONS = on
    PROFILE_DATA_SECTIONS = on
    PROFILE_GC_SECTIONS = on
    PROFILE_OMIT_FRAME_POINTER = on
    PROFILE_UNWIND_TABLES = off
    PROFILE_ASYNC_UNWIND_TABLES = off
    PROFILE_STRIP = on
    PROFILE_ICF = all
    PROFILE_TARGET_ARCH = native
    PROFILE_TARGET_TUNE = native
    PROFILE_EXCEPTIONS = off
else ifeq ($(PROFILE),compact)
    PROFILE_CFLAGS += -Os
    PROFILE_DEBUG_ASSERTIONS = off
    PROFILE_LTO = thin
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
    PROFILE_DEBUG_ASSERTIONS = off
    PROFILE_LTO = thin
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

RESOLVED_COMPILE_ENV = $(if $(filter auto,$(COMPILE_ENV)),hosted,$(COMPILE_ENV))
RESOLVED_LTO = $(if $(filter profile,$(LTO)),$(PROFILE_LTO),$(LTO))
RESOLVED_OMIT_FRAME_POINTER = $(if $(filter profile,$(OMIT_FRAME_POINTER)),$(PROFILE_OMIT_FRAME_POINTER),$(OMIT_FRAME_POINTER))
RESOLVED_FUNCTION_SECTIONS = $(if $(filter profile,$(FUNCTION_SECTIONS)),$(PROFILE_FUNCTION_SECTIONS),$(FUNCTION_SECTIONS))
RESOLVED_DATA_SECTIONS = $(if $(filter profile,$(DATA_SECTIONS)),$(PROFILE_DATA_SECTIONS),$(DATA_SECTIONS))
RESOLVED_GC_SECTIONS = $(if $(filter profile,$(GC_SECTIONS)),$(PROFILE_GC_SECTIONS),$(GC_SECTIONS))
RESOLVED_LINK_LIBC = $(if $(filter auto,$(LINK_LIBC)),on,$(LINK_LIBC))
RESOLVED_LINK_DEFAULT_LIBS = $(if $(filter auto,$(LINK_DEFAULT_LIBS)),on,$(LINK_DEFAULT_LIBS))
RESOLVED_LINK_START_FILES = $(if $(filter auto,$(LINK_START_FILES)),on,$(LINK_START_FILES))
RESOLVED_LINK_COMPILER_RT = $(if $(filter auto,$(LINK_COMPILER_RT)),on,$(LINK_COMPILER_RT))
RESOLVED_LINK_MODE = $(LINK_MODE)
RESOLVED_WHOLE_ARCHIVE = $(if $(filter profile,$(WHOLE_ARCHIVE)),$(PROFILE_WHOLE_ARCHIVE),$(WHOLE_ARCHIVE))
RESOLVED_UNROLL_LOOPS = $(if $(filter profile,$(UNROLL_LOOPS)),$(PROFILE_UNROLL_LOOPS),$(UNROLL_LOOPS))
RESOLVED_UNWIND_TABLES = $(if $(filter profile,$(UNWIND_TABLES)),$(PROFILE_UNWIND_TABLES),$(UNWIND_TABLES))
RESOLVED_ASYNC_UNWIND_TABLES = $(if $(filter profile,$(ASYNC_UNWIND_TABLES)),$(PROFILE_ASYNC_UNWIND_TABLES),$(ASYNC_UNWIND_TABLES))
RESOLVED_EXCEPTIONS = $(if $(filter profile,$(EXCEPTIONS)),$(PROFILE_EXCEPTIONS),$(EXCEPTIONS))
RESOLVED_STRIP = $(if $(filter profile,$(STRIP)),$(PROFILE_STRIP),$(STRIP))
RESOLVED_ICF = $(if $(filter profile,$(ICF)),$(PROFILE_ICF),$(ICF))
RESOLVED_TARGET_ARCH = $(if $(filter profile,$(TARGET_ARCH)),$(PROFILE_TARGET_ARCH),$(TARGET_ARCH))
RESOLVED_TARGET_TUNE = $(if $(filter profile,$(TARGET_TUNE)),$(PROFILE_TARGET_TUNE),$(TARGET_TUNE))

ifeq ($(ANALYSIS_ARTIFACTS),on)
    EMIT_MAP := on
    EMIT_LINKED_ASM := on
    EMIT_DISASM := on
else ifeq ($(ANALYSIS_ARTIFACTS),off)
else ifeq ($(ANALYSIS_ARTIFACTS),auto)
else
    $(error Unsupported ANALYSIS_ARTIFACTS '$(ANALYSIS_ARTIFACTS)')
endif

ifeq ($(PROFILE_DEBUG_ASSERTIONS),off)
    PROFILE_CFLAGS += -DNDEBUG
endif

ifeq ($(RESOLVED_LTO),on)
    PROFILE_CFLAGS += -flto
    PROFILE_LDFLAGS += -flto
else ifeq ($(RESOLVED_LTO),full)
    PROFILE_CFLAGS += -flto=full
    PROFILE_LDFLAGS += -flto=full
else ifeq ($(RESOLVED_LTO),thin)
    PROFILE_CFLAGS += -flto=thin
    PROFILE_LDFLAGS += -flto=thin
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

ifeq ($(RESOLVED_EXCEPTIONS),on)
    PROFILE_CFLAGS += -fexceptions
else ifeq ($(RESOLVED_EXCEPTIONS),off)
    PROFILE_CFLAGS += -fno-exceptions
else ifneq ($(RESOLVED_EXCEPTIONS),auto)
    $(error Unsupported EXCEPTIONS '$(EXCEPTIONS)')
endif

TARGET_FLAGS =
ifneq ($(ARCH_TARGET),auto)
    TARGET_FLAGS += -target $(ARCH_TARGET)
endif
ifneq ($(RESOLVED_TARGET_ARCH),auto)
    TARGET_FLAGS += -march=$(RESOLVED_TARGET_ARCH)
endif
ifneq ($(RESOLVED_TARGET_TUNE),auto)
    PROFILE_CFLAGS += -mtune=$(RESOLVED_TARGET_TUNE)
endif
ifneq ($(TARGET_ABI),auto)
    TARGET_FLAGS += -mabi=$(TARGET_ABI)
endif
ifneq ($(SYSROOT),auto)
    TARGET_FLAGS += --sysroot=$(SYSROOT)
endif

ifeq ($(RESOLVED_COMPILE_ENV),freestanding)
    PROFILE_CFLAGS += -ffreestanding
else ifeq ($(RESOLVED_COMPILE_ENV),hosted)
else
    $(error Unsupported COMPILE_ENV '$(COMPILE_ENV)')
endif

CONTRACT_DEFINES = -DCOMP
ifeq ($(RESOLVED_COMPILE_ENV),freestanding)
    CONTRACT_DEFINES += -DCOMP_FREESTANDING
else
    CONTRACT_DEFINES += -DCOMP_HOSTED
endif

ifeq ($(RESOLVED_LINK_MODE),static)
    PROFILE_LDFLAGS += -static
else ifeq ($(RESOLVED_LINK_MODE),shared)
else ifeq ($(RESOLVED_LINK_MODE),auto)
else
    $(error Unsupported LINK_MODE '$(LINK_MODE)')
endif

ifeq ($(RESOLVED_LINK_DEFAULT_LIBS),on)
else ifeq ($(RESOLVED_LINK_DEFAULT_LIBS),off)
else
    $(error Unsupported LINK_DEFAULT_LIBS '$(LINK_DEFAULT_LIBS)')
endif

ifeq ($(RESOLVED_LINK_START_FILES),off)
    ifeq ($(RESOLVED_LINK_DEFAULT_LIBS),off)
        PROFILE_LDFLAGS += -nostdlib
        ifneq ($(RESOLVED_LINK_COMPILER_RT),off)
            PROFILE_LDFLAGS += $(shell $(CC) $(TARGET_FLAGS) --print-libgcc-file-name)
        endif
    else
        PROFILE_LDFLAGS += -nostartfiles
    endif
else ifeq ($(RESOLVED_LINK_START_FILES),on)
    ifeq ($(RESOLVED_LINK_DEFAULT_LIBS),off)
        PROFILE_LDFLAGS += -nodefaultlibs
        ifneq ($(RESOLVED_LINK_COMPILER_RT),off)
            PROFILE_LDFLAGS += $(shell $(CC) $(TARGET_FLAGS) --print-libgcc-file-name)
        endif
    else ifeq ($(RESOLVED_LINK_LIBC),off)
        ifeq ($(HOST_IS_WINDOWS),on)
            $(error LINK_LIBC=off cannot be represented on this host while LINK_DEFAULT_LIBS remains enabled; disable default libs and provide explicit runtime libraries)
        else
            PROFILE_LDFLAGS += -nolibc
        endif
    else ifeq ($(RESOLVED_LINK_LIBC),on)
    else
        $(error Unsupported LINK_LIBC '$(LINK_LIBC)')
    endif
else
    $(error Unsupported LINK_START_FILES '$(LINK_START_FILES)')
endif

ifeq ($(strip $(filter -DCOMP_HAS_START_FILES% -DCOMP_NO_START_FILES%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(filter on,$(RESOLVED_LINK_START_FILES)),-DCOMP_HAS_START_FILES,-DCOMP_NO_START_FILES)
endif
ifeq ($(strip $(filter -DCOMP_HAS_CRT% -DCOMP_NO_CRT%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(filter on,$(RESOLVED_LINK_START_FILES)),-DCOMP_HAS_CRT,-DCOMP_NO_CRT)
endif
ifeq ($(strip $(filter -DCOMP_HAS_DEFAULT_LIBS% -DCOMP_NO_DEFAULT_LIBS%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(filter on,$(RESOLVED_LINK_DEFAULT_LIBS)),-DCOMP_HAS_DEFAULT_LIBS,-DCOMP_NO_DEFAULT_LIBS)
endif
ifeq ($(strip $(filter -DCOMP_HAS_COMPILER_RT% -DCOMP_NO_COMPILER_RT%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(filter off,$(RESOLVED_LINK_COMPILER_RT)),-DCOMP_NO_COMPILER_RT,-DCOMP_HAS_COMPILER_RT)
endif
ifeq ($(strip $(filter -DCOMP_HAS_LIBC% -DCOMP_NO_LIBC%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(filter off,$(RESOLVED_LINK_LIBC)),-DCOMP_NO_LIBC,-DCOMP_HAS_LIBC)
endif
ifeq ($(strip $(filter -DCOMP_HAS_STDLIB% -DCOMP_NO_STDLIB%,$(EXTRA_DEFINES))),)
    CONTRACT_DEFINES += $(if $(and $(filter on,$(RESOLVED_LINK_START_FILES)),$(filter on,$(RESOLVED_LINK_DEFAULT_LIBS))),-DCOMP_HAS_STDLIB,-DCOMP_NO_STDLIB)
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

ifeq ($(MERGE_ALL_CONSTANTS),on)
    PROFILE_CFLAGS += -fmerge-all-constants
else ifeq ($(MERGE_ALL_CONSTANTS),off)
    PROFILE_CFLAGS += -fno-merge-all-constants
else ifeq ($(MERGE_ALL_CONSTANTS),auto)
else
    $(error Unsupported MERGE_ALL_CONSTANTS '$(MERGE_ALL_CONSTANTS)')
endif

ifeq ($(STACK_PROTECTOR),on)
    PROFILE_CFLAGS += -fstack-protector-strong
else ifeq ($(STACK_PROTECTOR),off)
    PROFILE_CFLAGS += -fno-stack-protector
else ifeq ($(STACK_PROTECTOR),auto)
else
    $(error Unsupported STACK_PROTECTOR '$(STACK_PROTECTOR)')
endif

ifeq ($(SAVE_TEMPS),cwd)
    PROFILE_CFLAGS += -save-temps=cwd
else ifeq ($(SAVE_TEMPS),obj)
    PROFILE_CFLAGS += -save-temps=obj
else ifeq ($(SAVE_TEMPS),off)
else
    $(error Unsupported SAVE_TEMPS '$(SAVE_TEMPS)')
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

ifeq ($(EMIT_MAP),on)
    PROFILE_LDFLAGS += -Wl,-Map=$(if $(strip $(MAP_PATH)),$(MAP_PATH),$(BUILD_DIR)/dh-c.map)
else ifeq ($(EMIT_MAP),off)
else
    $(error Unsupported EMIT_MAP '$(EMIT_MAP)')
endif

ifeq ($(PRINT_LINK_GC),on)
ifeq ($(PLATFORM),windows)
    $(error PRINT_LINK_GC is not supported by the COFF LLD linker used on Windows)
endif
    PROFILE_LDFLAGS += -Wl,--print-gc-sections
else ifeq ($(PRINT_LINK_GC),off)
else ifeq ($(PRINT_LINK_GC),auto)
else
    $(error Unsupported PRINT_LINK_GC '$(PRINT_LINK_GC)')
endif

RESOLVED_DISASM_FLAGS =
ifeq ($(DISASM_DEMANGLE),on)
    RESOLVED_DISASM_FLAGS += --demangle
else ifeq ($(DISASM_DEMANGLE),off)
else ifeq ($(DISASM_DEMANGLE),auto)
else
    $(error Unsupported DISASM_DEMANGLE '$(DISASM_DEMANGLE)')
endif
ifeq ($(DISASM_SOURCE),on)
    RESOLVED_DISASM_FLAGS += --source
else ifeq ($(DISASM_SOURCE),off)
else ifeq ($(DISASM_SOURCE),auto)
    RESOLVED_DISASM_FLAGS += --source
else
    $(error Unsupported DISASM_SOURCE '$(DISASM_SOURCE)')
endif
ifeq ($(DISASM_LINE_NUMBERS),on)
    RESOLVED_DISASM_FLAGS += --line-numbers
else ifeq ($(DISASM_LINE_NUMBERS),off)
else ifeq ($(DISASM_LINE_NUMBERS),auto)
    RESOLVED_DISASM_FLAGS += --line-numbers
else
    $(error Unsupported DISASM_LINE_NUMBERS '$(DISASM_LINE_NUMBERS)')
endif
ifeq ($(DISASM_SYMBOLIZE_OPERANDS),on)
    RESOLVED_DISASM_FLAGS += --symbolize-operands
else ifeq ($(DISASM_SYMBOLIZE_OPERANDS),off)
else ifeq ($(DISASM_SYMBOLIZE_OPERANDS),auto)
else
    $(error Unsupported DISASM_SYMBOLIZE_OPERANDS '$(DISASM_SYMBOLIZE_OPERANDS)')
endif
ifeq ($(DISASM_RAW_INSN),on)
else ifeq ($(DISASM_RAW_INSN),off)
    RESOLVED_DISASM_FLAGS += --no-show-raw-insn
else ifeq ($(DISASM_RAW_INSN),auto)
else
    $(error Unsupported DISASM_RAW_INSN '$(DISASM_RAW_INSN)')
endif
ifeq ($(DISASM_SECTION_CONTENTS),on)
    RESOLVED_DISASM_FLAGS += -s
else ifeq ($(DISASM_SECTION_CONTENTS),off)
else ifeq ($(DISASM_SECTION_CONTENTS),auto)
else
    $(error Unsupported DISASM_SECTION_CONTENTS '$(DISASM_SECTION_CONTENTS)')
endif
RESOLVED_DISASM_FLAGS += $(DISASM_FLAGS)

ifeq ($(LOOSE_ERRORS),suppress)
    DIAGNOSTIC_CFLAGS = -w
else ifeq ($(LOOSE_ERRORS),warn)
    DIAGNOSTIC_CFLAGS = -Wall -Wextra -Wconversion -Wsign-conversion -Wfloat-conversion -Wformat=2 -Wcast-qual -Wcast-align -Wpointer-arith -Wbad-function-cast -Wnull-dereference -Wwrite-strings -Wuninitialized -Wframe-larger-than=4096 -Wno-switch-enum -Winfinite-recursion -Wno-microsoft-anon-tag -Wloop-analysis -Wstrict-prototypes -Wmissing-prototypes -Wmissing-variable-declarations -Wdiv-by-zero -Wthread-safety
else ifeq ($(LOOSE_ERRORS),auto)
    DIAGNOSTIC_CFLAGS = -Werror=all -Werror=extra -Werror=conversion -Werror=sign-conversion -Wfloat-conversion -Wformat=2 -Werror=cast-qual -Werror=cast-align -Wpointer-arith -Wbad-function-cast -Wnull-dereference -Wwrite-strings -Werror=uninitialized -Wframe-larger-than=4096 -Wno-switch-enum -Winfinite-recursion -Wno-microsoft-anon-tag -Wloop-analysis -Werror=strict-prototypes -Werror=missing-prototypes -Wmissing-variable-declarations -Werror=div-by-zero -Wthread-safety
else ifeq ($(LOOSE_ERRORS),never)
    DIAGNOSTIC_CFLAGS = -Werror=all -Werror=extra -Werror=conversion -Werror=sign-conversion -Wfloat-conversion -Wformat=2 -Werror=cast-qual -Werror=cast-align -Wpointer-arith -Wbad-function-cast -Wnull-dereference -Wwrite-strings -Werror=uninitialized -Wframe-larger-than=4096 -Wno-switch-enum -Winfinite-recursion -Wno-microsoft-anon-tag -Wloop-analysis -Werror=strict-prototypes -Werror=missing-prototypes -Wmissing-variable-declarations -Werror=div-by-zero -Wthread-safety
else ifeq ($(LOOSE_ERRORS),off)
    DIAGNOSTIC_CFLAGS = -Werror=all -Werror=extra -Werror=conversion -Werror=sign-conversion -Wfloat-conversion -Wformat=2 -Werror=cast-qual -Werror=cast-align -Wpointer-arith -Wbad-function-cast -Wnull-dereference -Wwrite-strings -Werror=uninitialized -Wframe-larger-than=4096 -Wno-switch-enum -Winfinite-recursion -Wno-microsoft-anon-tag -Wloop-analysis -Werror=strict-prototypes -Werror=missing-prototypes -Wmissing-variable-declarations -Werror=div-by-zero -Wthread-safety
else
    $(error Unsupported LOOSE_ERRORS '$(LOOSE_ERRORS)')
endif

ifneq ($(strip $(ENTRY)),)
    PROFILE_LDFLAGS += -Wl,-e,$(ENTRY)
endif
ifneq ($(strip $(LINKER_SCRIPT)),)
    PROFILE_LDFLAGS += -Wl,-T,$(LINKER_SCRIPT)
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

CFLAGS = $(BASE_CFLAGS) $(PROFILE_CFLAGS) $(TARGET_FLAGS) $(CONTRACT_DEFINES) $(DIAGNOSTIC_CFLAGS) $(EXTRA_INCLUDES) $(EXTRA_ISYSTEMS) $(EXTRA_DEFINES) $(EXTRA_UNDEFS) $(COMP_ARGS)
LDFLAGS_EXTRA = $(EXTRA_LIB_DIRS) $(LINK_ARGS)

# Platform-specific flags
ifeq ($(PLATFORM),windows)
    CFLAGS += -D_WIN32_WINNT=0x0600
    CFLAGS += -fansi-escape-codes
    LDFLAGS = -fuse-ld=lld $(TARGET_FLAGS) $(PROFILE_LDFLAGS) $(LDFLAGS_EXTRA)
else
    CFLAGS += -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
    LDFLAGS = $(TARGET_FLAGS) $(PROFILE_LDFLAGS) $(LDFLAGS_EXTRA)
endif

EXTRA_TARGETS =
PREPROCESSED_TARGET = $(if $(strip $(PREPROCESSED_PATH)),$(PREPROCESSED_PATH),$(BUILD_DIR)/dh-c.i)
ASM_TARGET = $(if $(strip $(ASM_PATH)),$(ASM_PATH),$(BUILD_DIR)/dh-c.s)
IR_TARGET = $(if $(strip $(IR_PATH)),$(IR_PATH),$(BUILD_DIR)/dh-c.ll)
LINKED_ASM_TARGET = $(if $(strip $(LINKED_ASM_PATH)),$(LINKED_ASM_PATH),$(BUILD_DIR)/dh-c.linked.s)
DISASM_TARGET = $(if $(strip $(DISASM_PATH)),$(DISASM_PATH),$(BUILD_DIR)/dh-c.disasm.s)
DEBUG_INFO_TARGET = $(if $(strip $(DEBUG_INFO_PATH)),$(DEBUG_INFO_PATH),$(BUILD_DIR)/dh-c.debug.txt)

ifeq ($(EMIT_PREPROCESSED),on)
    EXTRA_TARGETS += $(PREPROCESSED_TARGET)
else ifeq ($(EMIT_PREPROCESSED),off)
else
    $(error Unsupported EMIT_PREPROCESSED '$(EMIT_PREPROCESSED)')
endif
ifeq ($(EMIT_ASM),on)
    EXTRA_TARGETS += $(ASM_TARGET)
else ifeq ($(EMIT_ASM),off)
else
    $(error Unsupported EMIT_ASM '$(EMIT_ASM)')
endif
ifeq ($(EMIT_IR),on)
    EXTRA_TARGETS += $(IR_TARGET)
else ifeq ($(EMIT_IR),off)
else
    $(error Unsupported EMIT_IR '$(EMIT_IR)')
endif
ifeq ($(EMIT_LINKED_ASM),on)
ifeq ($(filter on full thin,$(RESOLVED_LTO)),)
    $(error EMIT_LINKED_ASM requires effective LTO to be enabled)
endif
    EXTRA_TARGETS += $(LINKED_ASM_TARGET)
else ifeq ($(EMIT_LINKED_ASM),off)
else
    $(error Unsupported EMIT_LINKED_ASM '$(EMIT_LINKED_ASM)')
endif
ifeq ($(EMIT_DISASM),on)
    EXTRA_TARGETS += $(DISASM_TARGET)
else ifeq ($(EMIT_DISASM),off)
else
    $(error Unsupported EMIT_DISASM '$(EMIT_DISASM)')
endif
ifeq ($(EMIT_DEBUG_INFO),on)
ifneq ($(PROFILE_DEBUG_INFO),on)
    $(error EMIT_DEBUG_INFO requires debug info to be enabled by the selected profile)
endif
    EXTRA_TARGETS += $(DEBUG_INFO_TARGET)
else ifeq ($(EMIT_DEBUG_INFO),off)
else
    $(error Unsupported EMIT_DEBUG_INFO '$(EMIT_DEBUG_INFO)')
endif

# Default target
all: $(TARGET) $(EXTRA_TARGETS)

# Build executable
$(TARGET): $(OBJS)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_LD,$@)$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LINK_LIBS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_CC,$<)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(PREPROCESSED_TARGET): $(MAIN_SRC)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_CC,$<)$(CC) $(CFLAGS) -E $< -o $@

$(ASM_TARGET): $(MAIN_SRC)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_CC,$<)$(CC) $(CFLAGS) -S $< -o $@

$(IR_TARGET): $(MAIN_SRC)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_CC,$<)$(CC) $(CFLAGS) -S -emit-llvm $< -o $@

$(LINKED_ASM_TARGET): $(OBJS)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(call P_LD,$@)$(CC) $(OBJS) -o $@ $(LDFLAGS) -Wl,--lto-emit-asm $(LINK_LIBS)

$(DISASM_TARGET): $(TARGET)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(LLVM_OBJDUMP) -d $(RESOLVED_DISASM_FLAGS) $< > $@

$(DEBUG_INFO_TARGET): $(TARGET)
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(LLVM_DWARFDUMP) --debug-info --debug-line $< > $@

# Include dependency files
-include $(OBJS:.o=.d)

# Clean build artifacts
clean:
	$(Q)$(RMDIR) $(BUILD_ROOT) 2>/dev/null || true

clean-profile:
	$(Q)$(RMDIR) $(BUILD_DIR) 2>/dev/null || true

# Install (copy to system path - optional)
ifeq ($(PLATFORM),windows)
install: $(TARGET)
	$(Q)echo "Installation not implemented for Windows"
else
install: $(TARGET)
	$(Q)cp $(TARGET) /usr/local/bin/dh-c
	$(Q)chmod +x /usr/local/bin/dh-c
endif

# Phony targets
.PHONY: all clean clean-profile install
EOF
echo "Makefile generated"
