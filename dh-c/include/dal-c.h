#pragma once
#ifndef dal_c__included
#define dal_c__included 1

#include "dal-c-ext/base.h"
#include "dal-c-ext/str.h"
#include <assert.h>

/// === TOOL METADATA ===

#define dal_c_tool_target_dsl "dasae-headers"
#define dal_c_tool_name "dh-c"
#define dal_c_tool_description "Build tool for " dal_c_tool_target_dsl " projects"
#define dal_c_tool_copyright "Copyright (c) 2024-2026 Gyeongtae Kim"

/// === VERSION ===

/// --- Queries ---

#define dal_c_ver_core_sep dal_c__str__ver_core_sep
#define dal_c_ver_core_major dal_c__num__ver_core_major
#define dal_c_ver_core_minor dal_c__num__ver_core_minor
#define dal_c_ver_core_patch dal_c__num__ver_core_patch

#define dal_c_ver_core_num dal_c__val__ver_core_num
#define dal_c_ver_core_str dal_c__str__ver_core_str

#define dal_c_ver_label_delim dal_c__str__ver_label_delim
#define dal_c_ver_label_prefix_as_num dal_c__num__ver_label_prefix_as_num
#define dal_c_ver_label_prefix_as_str dal_c__str__ver_label_prefix_as_str
#define dal_c_ver_label_sep dal_c__str__ver_label_sep
#define dal_c_ver_label_suffix_as_num dal_c__num__ver_label_suffix_as_num
#define dal_c_ver_label_suffix_as_str dal_c__str__ver_label_suffix_as_str

#define dal_c_ver_build_delim dal_c__str__ver_build_delim
#define dal_c_ver_build_as_str dal_c__str__ver_build_as_str

#define dal_c_ver_num dal_c__val__ver_num
#define dal_c_ver_str dal_c__str__ver_str
#define dal_c_ver_str_with_build dal_c__str__ver_str_with_build

/// --- Defaults ---

#define dal_c_ver_core_calc(_major, _minor, _patch...) \
    dal_c__val__ver_core_calc(_major, _minor, _patch)
#define dal_c_ver_core_strfy(_major, _minor, _patch...) \
    dal_c__str__ver_core_strfy(_major, _minor, _patch)

#define dal_c_ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num...) \
    dal_c__val__ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num)
#define dal_c_ver_strfy(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str...) \
    dal_c__str__ver_strfy(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str)
#define dal_c_ver_strfyWithBuild(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str, _build_as_str...) \
    dal_c__str__ver_strfyWithBuild(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str, _build_as_str)

#define dal_c_ver_core_sep_default dal_c_ver_core_sep_some
#define dal_c_ver_core_sep_some "."
#define dal_c_ver_core_major_default dal_c_ver_core_major_none
#define dal_c_ver_core_major_none 0
#define dal_c_ver_core_minor_default dal_c_ver_core_minor_none
#define dal_c_ver_core_minor_none 0
#define dal_c_ver_core_patch_default dal_c_ver_core_patch_none
#define dal_c_ver_core_patch_none 0

#define dal_c_ver_label_delim_default dal_c_ver_label_delim_none
#define dal_c_ver_label_delim_none ""
#define dal_c_ver_label_delim_some "-"
#define dal_c_ver_label_prefix_as_num_default dal_c_ver_label_prefix_as_num_none
#define dal_c_ver_label_prefix_as_num_none dal_c_ver_label_prefix_as_num_release
#define dal_c_ver_label_prefix_as_num_alpha 0
#define dal_c_ver_label_prefix_as_num_beta 1
#define dal_c_ver_label_prefix_as_num_rc 2
#define dal_c_ver_label_prefix_as_num_release 3
#define dal_c_ver_label_prefix_as_str_default dal_c_ver_label_prefix_as_str_none
#define dal_c_ver_label_prefix_as_str_none dal_c_ver_label_prefix_as_str_release
#define dal_c_ver_label_prefix_as_str_alpha "alpha"
#define dal_c_ver_label_prefix_as_str_beta "beta"
#define dal_c_ver_label_prefix_as_str_rc "rc"
#define dal_c_ver_label_prefix_as_str_release ""
#define dal_c_ver_label_sep_default dal_c_ver_label_sep_none
#define dal_c_ver_label_sep_none ""
#define dal_c_ver_label_sep_some "."
#define dal_c_ver_label_suffix_as_num_default dal_c_ver_label_suffix_as_num_none
#define dal_c_ver_label_suffix_as_num_none 0
#define dal_c_ver_label_suffix_as_str_default dal_c_ver_label_suffix_as_str_none
#define dal_c_ver_label_suffix_as_str_none ""

#define dal_c_ver_build_delim_default dal_c_ver_build_delim_none
#define dal_c_ver_build_delim_none ""
#define dal_c_ver_build_delim_some "+"
#define dal_c_ver_build_as_str_default dal_c_ver_build_as_str_none
#define dal_c_ver_build_as_str_none ""

/// === BOOLEAN ===

#define dal_c_boolean_true "true"
#define dal_c_boolean_false "false"
#define dal_c_boolean_on "on"
#define dal_c_boolean_off "off"
static inline bool dal_c_boolean_parse(const char* str) {
    if (str_eql(str, dal_c_boolean_true) || str_eql(str, dal_c_boolean_on) || str_eql(str, "yes") || str_eql(str, "1")) { return true; }
    if (str_eql(str, dal_c_boolean_false) || str_eql(str, dal_c_boolean_off) || str_eql(str, "no") || str_eql(str, "0")) { return false; }
    return false;
}
static inline const char* dal_c_boolean_format(bool value) {
    return value ? dal_c_boolean_on : dal_c_boolean_off;
}

/// === TOGGLE STATE ===

typedef enum dal_c_ToggleState {
    dal_c_ToggleState_invalid = -1,
    dal_c_ToggleState_auto = 0,
    dal_c_ToggleState_enabled = 1,
    dal_c_ToggleState_disabled = 2,
} dal_c_ToggleState;
static inline dal_c_ToggleState dal_c_ToggleState_parse(const char* str) {
    if (str_eql(str, "auto")) { return dal_c_ToggleState_auto; }
    if (str_eql(str, dal_c_boolean_true) || str_eql(str, dal_c_boolean_on) || str_eql(str, "yes") || str_eql(str, "1")) {
        return dal_c_ToggleState_enabled;
    }
    if (str_eql(str, dal_c_boolean_false) || str_eql(str, dal_c_boolean_off) || str_eql(str, "no") || str_eql(str, "0")) {
        return dal_c_ToggleState_disabled;
    }
    return dal_c_ToggleState_invalid;
}
static inline const char* dal_c_ToggleState_format(dal_c_ToggleState state) {
    switch (state) {
    case dal_c_ToggleState_auto: return "auto";
    case dal_c_ToggleState_enabled: return dal_c_boolean_on;
    case dal_c_ToggleState_disabled: return dal_c_boolean_off;
    case dal_c_ToggleState_invalid:
    default: return NULL;
    }
}
static inline bool dal_c_ToggleState_resolve(dal_c_ToggleState state, bool default_enabled) {
    switch (state) {
    case dal_c_ToggleState_enabled: return true;
    case dal_c_ToggleState_disabled: return false;
    case dal_c_ToggleState_auto:
    default: return default_enabled;
    }
}

typedef enum dal_c_IcfMode {
    dal_c_IcfMode_auto = 0,
    dal_c_IcfMode_off = 1,
    dal_c_IcfMode_safe = 2,
    dal_c_IcfMode_all = 3,
} dal_c_IcfMode;
#define dal_c_icf_mode_auto "auto"
#define dal_c_icf_mode_off "off"
#define dal_c_icf_mode_safe "safe"
#define dal_c_icf_mode_all "all"
static inline dal_c_IcfMode dal_c_IcfMode_parse(const char* str) {
    if (str_eql(str, dal_c_icf_mode_auto)) { return dal_c_IcfMode_auto; }
    if (str_eql(str, dal_c_icf_mode_off) || str_eql(str, dal_c_boolean_false) || str_eql(str, "no") || str_eql(str, "0")) { return dal_c_IcfMode_off; }
    if (str_eql(str, dal_c_icf_mode_safe)) { return dal_c_IcfMode_safe; }
    if (str_eql(str, dal_c_icf_mode_all) || str_eql(str, dal_c_boolean_true) || str_eql(str, dal_c_boolean_on) || str_eql(str, "1")) { return dal_c_IcfMode_all; }
    return dal_c_IcfMode_auto;
}
static inline const char* dal_c_IcfMode_format(dal_c_IcfMode mode) {
    switch (mode) {
    case dal_c_IcfMode_auto: return dal_c_icf_mode_auto;
    case dal_c_IcfMode_off: return dal_c_icf_mode_off;
    case dal_c_IcfMode_safe: return dal_c_icf_mode_safe;
    case dal_c_IcfMode_all: return dal_c_icf_mode_all;
    default: return NULL;
    }
}

typedef enum dal_c_LtoMode {
    dal_c_LtoMode_invalid = -1,
    dal_c_LtoMode_auto = 0,
    dal_c_LtoMode_off = 1,
    dal_c_LtoMode_on = 2,
    dal_c_LtoMode_full = 3,
    dal_c_LtoMode_thin = 4,
} dal_c_LtoMode;
#define dal_c_lto_mode_auto "auto"
#define dal_c_lto_mode_off "off"
#define dal_c_lto_mode_on "on"
#define dal_c_lto_mode_full "full"
#define dal_c_lto_mode_thin "thin"
static inline dal_c_LtoMode dal_c_LtoMode_parse(const char* str) {
    if (str_eql(str, dal_c_lto_mode_auto)) { return dal_c_LtoMode_auto; }
    if (str_eql(str, dal_c_lto_mode_off) || str_eql(str, "no") || str_eql(str, "false") || str_eql(str, "0")) { return dal_c_LtoMode_off; }
    if (str_eql(str, dal_c_lto_mode_on) || str_eql(str, "yes") || str_eql(str, "true") || str_eql(str, "1")) { return dal_c_LtoMode_on; }
    if (str_eql(str, dal_c_lto_mode_full)) { return dal_c_LtoMode_full; }
    if (str_eql(str, dal_c_lto_mode_thin)) { return dal_c_LtoMode_thin; }
    return dal_c_LtoMode_invalid;
}
static inline const char* dal_c_LtoMode_format(dal_c_LtoMode mode) {
    switch (mode) {
    case dal_c_LtoMode_auto: return dal_c_lto_mode_auto;
    case dal_c_LtoMode_off: return dal_c_lto_mode_off;
    case dal_c_LtoMode_on: return dal_c_lto_mode_on;
    case dal_c_LtoMode_full: return dal_c_lto_mode_full;
    case dal_c_LtoMode_thin: return dal_c_lto_mode_thin;
    case dal_c_LtoMode_invalid:
    default: return NULL;
    }
}
static inline bool dal_c_LtoMode_isEnabled(dal_c_LtoMode mode) {
    return mode == dal_c_LtoMode_on || mode == dal_c_LtoMode_full || mode == dal_c_LtoMode_thin;
}
static inline const char* dal_c_LtoMode_toFlag(dal_c_LtoMode mode) {
    switch (mode) {
    case dal_c_LtoMode_off: return "-fno-lto";
    case dal_c_LtoMode_on: return "-flto";
    case dal_c_LtoMode_full: return "-flto=full";
    case dal_c_LtoMode_thin: return "-flto=thin";
    case dal_c_LtoMode_auto:
    case dal_c_LtoMode_invalid:
    default: return NULL;
    }
}

typedef enum dal_c_SaveTempsMode {
    dal_c_SaveTempsMode_off = 0,
    dal_c_SaveTempsMode_cwd = 1,
    dal_c_SaveTempsMode_obj = 2,
} dal_c_SaveTempsMode;
#define dal_c_save_temps_off "off"
#define dal_c_save_temps_cwd "cwd"
#define dal_c_save_temps_obj "obj"
static inline dal_c_SaveTempsMode dal_c_SaveTempsMode_parse(const char* str) {
    if (str_eql(str, dal_c_save_temps_off) || str_eql(str, dal_c_boolean_false) || str_eql(str, "no") || str_eql(str, "0")) { return dal_c_SaveTempsMode_off; }
    if (str_eql(str, dal_c_save_temps_cwd) || str_eql(str, dal_c_boolean_true) || str_eql(str, dal_c_boolean_on) || str_eql(str, "1")) { return dal_c_SaveTempsMode_cwd; }
    if (str_eql(str, dal_c_save_temps_obj)) { return dal_c_SaveTempsMode_obj; }
    return dal_c_SaveTempsMode_off;
}
static inline const char* dal_c_SaveTempsMode_format(dal_c_SaveTempsMode mode) {
    switch (mode) {
    case dal_c_SaveTempsMode_off: return dal_c_save_temps_off;
    case dal_c_SaveTempsMode_cwd: return dal_c_save_temps_cwd;
    case dal_c_SaveTempsMode_obj: return dal_c_save_temps_obj;
    default: return NULL;
    }
}

/// === COMPILE ENVIRONMENT ===

typedef enum dal_c_CompileEnv {
    dal_c_CompileEnv_auto = 0,
    dal_c_CompileEnv_hosted = 1,
    dal_c_CompileEnv_freestanding = 2,
} dal_c_CompileEnv;
#define dal_c_compile_env_auto "auto"
#define dal_c_compile_env_hosted "hosted"
#define dal_c_compile_env_freestanding "freestanding"
static inline dal_c_CompileEnv dal_c_CompileEnv_parse(const char* str) {
    if (str_eql(str, dal_c_compile_env_auto)) { return dal_c_CompileEnv_auto; }
    if (str_eql(str, dal_c_compile_env_hosted)) { return dal_c_CompileEnv_hosted; }
    if (str_eql(str, dal_c_compile_env_freestanding)) { return dal_c_CompileEnv_freestanding; }
    return dal_c_CompileEnv_auto;
}
static inline const char* dal_c_CompileEnv_format(dal_c_CompileEnv compile_env) {
    switch (compile_env) {
    case dal_c_CompileEnv_auto: return dal_c_compile_env_auto;
    case dal_c_CompileEnv_hosted: return dal_c_compile_env_hosted;
    case dal_c_CompileEnv_freestanding: return dal_c_compile_env_freestanding;
    default: return NULL;
    }
}
static inline dal_c_CompileEnv dal_c_CompileEnv_resolve(dal_c_CompileEnv compile_env) {
    return compile_env == dal_c_CompileEnv_auto ? dal_c_CompileEnv_hosted : compile_env;
}

/// === TARGET ===

typedef enum dal_c_Target {
    dal_c_Target_invalid = -1,
    dal_c_Target_executable = 0, // no-extension or `.out` (Unix), `.exe` (Windows) executable output
    dal_c_Target_static_lib = 1, // `.a` (Unix), `.lib` (Windows) static library output
    dal_c_Target_shared_lib = 2, // `.so` (Unix), `.dll` (Windows) shared library output
    dal_c_Target_lib = 3, // abstract library output: native/LTO static variants when enabled + shared
    dal_c_Target_image = 4, // raw image output (for example `.bin`)
    dal_c_Target_preprocessed = 5, // preprocessed translation unit output (`.i`)
    dal_c_Target_assembly = 6, // compiler-emitted assembly output (`.s`)
} dal_c_Target;
static inline const char* dal_c_Target_format(dal_c_Target target) {
    switch (target) {
    case dal_c_Target_executable: return "executable";
    case dal_c_Target_static_lib: return "static-lib";
    case dal_c_Target_shared_lib: return "shared-lib";
    case dal_c_Target_lib: return "lib";
    case dal_c_Target_image: return "image";
    case dal_c_Target_preprocessed: return "preprocessed";
    case dal_c_Target_assembly: return "assembly";
    case dal_c_Target_invalid:
    default: return NULL;
    }
}
static inline dal_c_Target dal_c_Target_parse(const char* str) {
    if (str_eql(str, "executable")) { return dal_c_Target_executable; }
    if (str_eql(str, "static-lib")) { return dal_c_Target_static_lib; }
    if (str_eql(str, "shared-lib")) { return dal_c_Target_shared_lib; }
    if (str_eql(str, "lib")) { return dal_c_Target_lib; }
    if (str_eql(str, "image")) { return dal_c_Target_image; }
    if (str_eql(str, "preprocessed")) { return dal_c_Target_preprocessed; }
    if (str_eql(str, "assembly")) { return dal_c_Target_assembly; }
    return dal_c_Target_invalid;
}

/// === LINKING ===

typedef enum dal_c_Linking {
    dal_c_Linking_invalid = -1,
    dal_c_Linking_static = 0, // static library artifact
    dal_c_Linking_shared = 1, // shared library artifact
} dal_c_Linking;
#define dal_c_linking_static "static"
#define dal_c_linking_shared "shared"
static inline dal_c_Linking dal_c_Linking_parse(const char* str) {
    if (str_eql(str, dal_c_linking_static)) { return dal_c_Linking_static; }
    if (str_eql(str, dal_c_linking_shared)) { return dal_c_Linking_shared; }
    return dal_c_Linking_invalid;
}
static inline const char* dal_c_Linking_format(dal_c_Linking linking) {
    switch (linking) {
    case dal_c_Linking_static: return dal_c_linking_static;
    case dal_c_Linking_shared: return dal_c_linking_shared;
    case dal_c_Linking_invalid:
    default: return NULL;
    }
}
static inline dal_c_Linking dal_c_Linking_fromFlag(const char* flag) {
    if (str_eql(flag, "-static")) { return dal_c_Linking_static; }
    if (str_eql(flag, "-shared")) { return dal_c_Linking_shared; }
    return dal_c_Linking_invalid;
}
static inline const char* dal_c_Linking_toFlag(dal_c_Linking linking) {
    switch (linking) {
    case dal_c_Linking_static: return "-static";
    case dal_c_Linking_shared: return "-shared";
    case dal_c_Linking_invalid:
    default: return NULL;
    }
}

typedef enum dal_c_LinkMode {
    dal_c_LinkMode_invalid = -1,
    dal_c_LinkMode_auto = 0, // use the compiler/linker default
    dal_c_LinkMode_static = 1, // static library artifact or `-static` executable link
    dal_c_LinkMode_shared = 2, // shared library artifact or default/shared executable link
} dal_c_LinkMode;
#define dal_c_link_mode_auto "auto"
#define dal_c_link_mode_static dal_c_linking_static
#define dal_c_link_mode_shared dal_c_linking_shared
static inline dal_c_LinkMode dal_c_LinkMode_parse(const char* str) {
    if (str_eql(str, dal_c_link_mode_auto)) { return dal_c_LinkMode_auto; }
    if (str_eql(str, dal_c_link_mode_static)) { return dal_c_LinkMode_static; }
    if (str_eql(str, dal_c_link_mode_shared)) { return dal_c_LinkMode_shared; }
    return dal_c_LinkMode_invalid;
}
static inline const char* dal_c_LinkMode_format(dal_c_LinkMode mode) {
    switch (mode) {
    case dal_c_LinkMode_auto: return dal_c_link_mode_auto;
    case dal_c_LinkMode_static: return dal_c_link_mode_static;
    case dal_c_LinkMode_shared: return dal_c_link_mode_shared;
    case dal_c_LinkMode_invalid:
    default: return NULL;
    }
}
static inline dal_c_Linking dal_c_LinkMode_toLibraryLinking(dal_c_LinkMode mode, dal_c_Linking default_linking) {
    switch (mode) {
    case dal_c_LinkMode_static: return dal_c_Linking_static;
    case dal_c_LinkMode_shared: return dal_c_Linking_shared;
    case dal_c_LinkMode_auto: return default_linking;
    case dal_c_LinkMode_invalid:
    default: return default_linking;
    }
}

/// === OPTIMIZATION LEVEL ===

typedef enum dal_c_OptiLevel {
    dal_c_OptiLevel_invalid = -1,
    dal_c_OptiLevel_none = 0, // -O0: No optimization
    dal_c_OptiLevel_debug = 1, // -Og: Smallest for debugging
    dal_c_OptiLevel_basic = 2, // -O1: Basic optimizations
    dal_c_OptiLevel_balanced = 3, // -O2: Good balance
    dal_c_OptiLevel_aggressive = 4, // -O3: Maximum optimize
    dal_c_OptiLevel_compact = 5, // -Os: Optimize for size
    dal_c_OptiLevel_minimal = 6, // -Oz: Size at all costs
} dal_c_OptiLevel;
#define dal_c_opti_level_none "none"
#define dal_c_opti_level_debug "debug"
#define dal_c_opti_level_basic "basic"
#define dal_c_opti_level_balanced "balanced"
#define dal_c_opti_level_aggressive "aggressive"
#define dal_c_opti_level_compact "compact"
#define dal_c_opti_level_minimal "minimal"
static inline dal_c_OptiLevel dal_c_OptiLevel_parse(const char* str) {
    if (str_eql(str, dal_c_opti_level_none)) { return dal_c_OptiLevel_none; }
    if (str_eql(str, dal_c_opti_level_debug)) { return dal_c_OptiLevel_debug; }
    if (str_eql(str, dal_c_opti_level_basic)) { return dal_c_OptiLevel_basic; }
    if (str_eql(str, dal_c_opti_level_balanced)) { return dal_c_OptiLevel_balanced; }
    if (str_eql(str, dal_c_opti_level_aggressive)) { return dal_c_OptiLevel_aggressive; }
    if (str_eql(str, dal_c_opti_level_compact)) { return dal_c_OptiLevel_compact; }
    if (str_eql(str, dal_c_opti_level_minimal)) { return dal_c_OptiLevel_minimal; }
    return dal_c_OptiLevel_invalid;
}
static inline const char* dal_c_OptiLevel_format(dal_c_OptiLevel opt_level) {
    switch (opt_level) {
    case dal_c_OptiLevel_none: return dal_c_opti_level_none;
    case dal_c_OptiLevel_debug: return dal_c_opti_level_debug;
    case dal_c_OptiLevel_basic: return dal_c_opti_level_basic;
    case dal_c_OptiLevel_balanced: return dal_c_opti_level_balanced;
    case dal_c_OptiLevel_aggressive: return dal_c_opti_level_aggressive;
    case dal_c_OptiLevel_compact: return dal_c_opti_level_compact;
    case dal_c_OptiLevel_minimal: return dal_c_opti_level_minimal;
    case dal_c_OptiLevel_invalid:
    default: return NULL;
    }
}
static inline dal_c_OptiLevel dal_c_OptiLevel_fromFlag(const char* flag) {
    if (str_eql(flag, "-O0")) { return dal_c_OptiLevel_none; }
    if (str_eql(flag, "-Og")) { return dal_c_OptiLevel_debug; }
    if (str_eql(flag, "-O1")) { return dal_c_OptiLevel_basic; }
    if (str_eql(flag, "-O2")) { return dal_c_OptiLevel_balanced; }
    if (str_eql(flag, "-O3")) { return dal_c_OptiLevel_aggressive; }
    if (str_eql(flag, "-Os")) { return dal_c_OptiLevel_compact; }
    if (str_eql(flag, "-Oz")) { return dal_c_OptiLevel_minimal; }
    return dal_c_OptiLevel_invalid;
}
static inline const char* dal_c_OptiLevel_toFlag(dal_c_OptiLevel opt_level) {
    switch (opt_level) {
    case dal_c_OptiLevel_none: return "-O0";
    case dal_c_OptiLevel_debug: return "-Og";
    case dal_c_OptiLevel_basic: return "-O1";
    case dal_c_OptiLevel_balanced: return "-O2";
    case dal_c_OptiLevel_aggressive: return "-O3";
    case dal_c_OptiLevel_compact: return "-Os";
    case dal_c_OptiLevel_minimal: return "-Oz";
    case dal_c_OptiLevel_invalid:
    default: return NULL;
    }
}

/// === DEBUG LEVEL ===

typedef enum dal_c_DebugLevel {
    dal_c_DebugLevel_invalid = -1,
    dal_c_DebugLevel_none = 0,
    dal_c_DebugLevel_minimal = 1, // -g1: Line tables only
    dal_c_DebugLevel_standard = 2, // -g: Standard debug info
    dal_c_DebugLevel_extended = 3, // -g3: Extra debug info
} dal_c_DebugLevel;
#define dal_c_debug_level_none "none"
#define dal_c_debug_level_minimal "minimal"
#define dal_c_debug_level_standard "standard"
#define dal_c_debug_level_extended "extended"
static inline dal_c_DebugLevel dal_c_DebugLevel_parse(const char* str) {
    if (str_eql(str, dal_c_debug_level_none)) { return dal_c_DebugLevel_none; }
    if (str_eql(str, dal_c_debug_level_minimal)) { return dal_c_DebugLevel_minimal; }
    if (str_eql(str, dal_c_debug_level_standard)) { return dal_c_DebugLevel_standard; }
    if (str_eql(str, dal_c_debug_level_extended)) { return dal_c_DebugLevel_extended; }
    return dal_c_DebugLevel_invalid;
}
static inline const char* dal_c_DebugLevel_format(dal_c_DebugLevel debug_level) {
    switch (debug_level) {
    case dal_c_DebugLevel_none: return dal_c_debug_level_none;
    case dal_c_DebugLevel_minimal: return dal_c_debug_level_minimal;
    case dal_c_DebugLevel_standard: return dal_c_debug_level_standard;
    case dal_c_DebugLevel_extended: return dal_c_debug_level_extended;
    case dal_c_DebugLevel_invalid:
    default: return NULL;
    }
}
static inline dal_c_DebugLevel dal_c_DebugLevel_fromFlag(const char* flag) {
    if (str_eql(flag, "")) { return dal_c_DebugLevel_none; }
    if (str_eql(flag, "-g1")) { return dal_c_DebugLevel_minimal; }
    if (str_eql(flag, "-g")) { return dal_c_DebugLevel_standard; }
    if (str_eql(flag, "-g3")) { return dal_c_DebugLevel_extended; }
    return dal_c_DebugLevel_invalid;
}
static inline const char* dal_c_DebugLevel_toFlag(dal_c_DebugLevel debug_level) {
    switch (debug_level) {
    case dal_c_DebugLevel_none: return "";
    case dal_c_DebugLevel_minimal: return "-g1";
    case dal_c_DebugLevel_standard: return "-g";
    case dal_c_DebugLevel_extended: return "-g3";
    case dal_c_DebugLevel_invalid:
    default: return NULL;
    }
}

/// === PROFILE ===

typedef enum dal_c_Profile {
    dal_c_Profile_invalid = -1,
    dal_c_Profile_dev = 0,
    dal_c_Profile_test = 1,
    dal_c_Profile_profile = 2,
    dal_c_Profile_stable = 3,
    dal_c_Profile_release = 4,
    dal_c_Profile_optimize = 5,
    dal_c_Profile_compact = 6,
    dal_c_Profile_micro = 7,
    dal_c_Profile_fast = 8
} dal_c_Profile;
#define dal_c_profile_dev "dev"
#define dal_c_profile_test "test"
#define dal_c_profile_profile "profile"
#define dal_c_profile_stable "stable"
#define dal_c_profile_release "release"
#define dal_c_profile_optimize "optimize"
#define dal_c_profile_compact "compact"
#define dal_c_profile_micro "micro"
#define dal_c_profile_fast "fast"
static inline dal_c_Profile dal_c_Profile_parse(const char* str) {
    if (str_eql(str, dal_c_profile_dev)) { return dal_c_Profile_dev; }
    if (str_eql(str, dal_c_profile_test)) { return dal_c_Profile_test; }
    if (str_eql(str, dal_c_profile_profile)) { return dal_c_Profile_profile; }
    if (str_eql(str, dal_c_profile_stable)) { return dal_c_Profile_stable; }
    if (str_eql(str, dal_c_profile_release)) { return dal_c_Profile_release; }
    if (str_eql(str, dal_c_profile_optimize)) { return dal_c_Profile_optimize; }
    if (str_eql(str, dal_c_profile_compact)) { return dal_c_Profile_compact; }
    if (str_eql(str, dal_c_profile_micro)) { return dal_c_Profile_micro; }
    if (str_eql(str, dal_c_profile_fast)) { return dal_c_Profile_fast; }
    return dal_c_Profile_invalid;
}
static inline const char* dal_c_Profile_format(dal_c_Profile profile) {
    switch (profile) {
    case dal_c_Profile_dev: return dal_c_profile_dev;
    case dal_c_Profile_test: return dal_c_profile_test;
    case dal_c_Profile_profile: return dal_c_profile_profile;
    case dal_c_Profile_stable: return dal_c_profile_stable;
    case dal_c_Profile_release: return dal_c_profile_release;
    case dal_c_Profile_optimize: return dal_c_profile_optimize;
    case dal_c_Profile_compact: return dal_c_profile_compact;
    case dal_c_Profile_micro: return dal_c_profile_micro;
    case dal_c_Profile_fast: return dal_c_profile_fast;
    case dal_c_Profile_invalid:
    default: return NULL;
    }
}

/// === SAMPLE DIRECTORY ===

typedef enum dal_c_SampleDir {
    dal_c_SampleDir_none = 0,
    dal_c_SampleDir_samples = 1,
    dal_c_SampleDir_examples = 2,
    dal_c_SampleDir_tests = 3,
} dal_c_SampleDir;
#define dal_c_sample_dir_samples "samples"
#define dal_c_sample_dir_examples "examples"
#define dal_c_sample_dir_tests "tests"
static inline dal_c_SampleDir dal_c_SampleDir_parse(const char* str) {
    if (str_eql(str, dal_c_sample_dir_samples)) { return dal_c_SampleDir_samples; }
    if (str_eql(str, dal_c_sample_dir_examples)) { return dal_c_SampleDir_examples; }
    if (str_eql(str, dal_c_sample_dir_tests)) { return dal_c_SampleDir_tests; }
    return dal_c_SampleDir_none;
}
static inline const char* dal_c_SampleDir_format(dal_c_SampleDir dir) {
    switch (dir) {
    case dal_c_SampleDir_samples: return dal_c_sample_dir_samples;
    case dal_c_SampleDir_examples: return dal_c_sample_dir_examples;
    case dal_c_SampleDir_tests: return dal_c_sample_dir_tests;
    case dal_c_SampleDir_none:
    default: return NULL;
    }
}

typedef enum dal_c_TargetSelection {
    dal_c_TargetSelection_invalid = -1,
    dal_c_TargetSelection_path = 0,
    dal_c_TargetSelection_file = 1,
    dal_c_TargetSelection_dir = 2,
} dal_c_TargetSelection;
#define dal_c_target_selection_path "path"
#define dal_c_target_selection_file "file"
#define dal_c_target_selection_dir "dir"
static inline dal_c_TargetSelection dal_c_TargetSelection_parse(const char* str) {
    if (str_eql(str, dal_c_target_selection_path)) { return dal_c_TargetSelection_path; }
    if (str_eql(str, dal_c_target_selection_file)) { return dal_c_TargetSelection_file; }
    if (str_eql(str, dal_c_target_selection_dir)) { return dal_c_TargetSelection_dir; }
    return dal_c_TargetSelection_invalid;
}
static inline const char* dal_c_TargetSelection_format(dal_c_TargetSelection selection) {
    switch (selection) {
    case dal_c_TargetSelection_path: return dal_c_target_selection_path;
    case dal_c_TargetSelection_file: return dal_c_target_selection_file;
    case dal_c_TargetSelection_dir: return dal_c_target_selection_dir;
    case dal_c_TargetSelection_invalid:
    default: return NULL;
    }
}

/// === PROFILE SPECIFICATION ===

typedef struct dal_c_ProfileSpec {
    const char* name;
    dal_c_OptiLevel opti_level;
    dal_c_DebugLevel debug_level;
    bool debug_assertions;
    dal_c_LtoMode lto_mode;
    dal_c_ToggleState omit_frame_pointer;
    dal_c_ToggleState function_sections;
    dal_c_ToggleState data_sections;
    dal_c_ToggleState gc_sections;
    dal_c_ToggleState whole_archive;
    dal_c_ToggleState unroll_loops;
    dal_c_ToggleState unwind_tables;
    dal_c_ToggleState async_unwind_tables;
    dal_c_ToggleState strip_mode;
    dal_c_IcfMode icf_mode;
    const char* target_arch;
    const char* target_tune;
    dal_c_ToggleState exceptions;
    const char* extra_flags[8];
} dal_c_ProfileSpec;
static const dal_c_ProfileSpec dal_c_profile_specs[] = {
    [dal_c_Profile_dev] = {
        .name = dal_c_profile_dev,
        .opti_level = dal_c_OptiLevel_debug,
        .debug_level = dal_c_DebugLevel_extended,
        .debug_assertions = true,
        .omit_frame_pointer = dal_c_ToggleState_disabled,
    },
    [dal_c_Profile_test] = {
        .name = dal_c_profile_test,
        .opti_level = dal_c_OptiLevel_basic,
        .debug_level = dal_c_DebugLevel_standard,
        .debug_assertions = true,
        .omit_frame_pointer = dal_c_ToggleState_disabled,
    },
    [dal_c_Profile_profile] = {
        .name = dal_c_profile_profile,
        .opti_level = dal_c_OptiLevel_balanced,
        .debug_level = dal_c_DebugLevel_standard,
        .debug_assertions = true,
        .omit_frame_pointer = dal_c_ToggleState_disabled,
    },
    [dal_c_Profile_stable] = {
        .name = dal_c_profile_stable,
        .opti_level = dal_c_OptiLevel_balanced,
        .debug_level = dal_c_DebugLevel_minimal,
        .debug_assertions = false,
        .lto_mode = dal_c_LtoMode_thin,
        .extra_flags = { NULL },
    },
    [dal_c_Profile_release] = {
        .name = dal_c_profile_release,
        .opti_level = dal_c_OptiLevel_aggressive,
        .debug_level = dal_c_DebugLevel_minimal,
        .debug_assertions = false,
        .lto_mode = dal_c_LtoMode_thin,
        .omit_frame_pointer = dal_c_ToggleState_enabled,
        .function_sections = dal_c_ToggleState_enabled,
        .data_sections = dal_c_ToggleState_enabled,
        .gc_sections = dal_c_ToggleState_enabled,
        .unwind_tables = dal_c_ToggleState_disabled,
        .async_unwind_tables = dal_c_ToggleState_disabled,
        .exceptions = dal_c_ToggleState_disabled,
        .strip_mode = dal_c_ToggleState_enabled,
        .icf_mode = dal_c_IcfMode_safe,
    },
    [dal_c_Profile_optimize] = {
        .name = dal_c_profile_optimize,
        .opti_level = dal_c_OptiLevel_aggressive,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .lto_mode = dal_c_LtoMode_full,
        .omit_frame_pointer = dal_c_ToggleState_enabled,
        .function_sections = dal_c_ToggleState_enabled,
        .data_sections = dal_c_ToggleState_enabled,
        .gc_sections = dal_c_ToggleState_enabled,
        .unwind_tables = dal_c_ToggleState_disabled,
        .async_unwind_tables = dal_c_ToggleState_disabled,
        .strip_mode = dal_c_ToggleState_enabled,
        .icf_mode = dal_c_IcfMode_all,
        .target_arch = "native",
        .target_tune = "native",
        .exceptions = dal_c_ToggleState_disabled,
    },
    [dal_c_Profile_compact] = {
        .name = dal_c_profile_compact,
        .opti_level = dal_c_OptiLevel_compact,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .lto_mode = dal_c_LtoMode_thin,
        .omit_frame_pointer = dal_c_ToggleState_enabled,
        .function_sections = dal_c_ToggleState_enabled,
        .data_sections = dal_c_ToggleState_enabled,
        .gc_sections = dal_c_ToggleState_enabled,
        .unwind_tables = dal_c_ToggleState_disabled,
        .async_unwind_tables = dal_c_ToggleState_disabled,
        .strip_mode = dal_c_ToggleState_enabled,
        .icf_mode = dal_c_IcfMode_all,
    },
    [dal_c_Profile_micro] = {
        .name = dal_c_profile_micro,
        .opti_level = dal_c_OptiLevel_minimal,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .lto_mode = dal_c_LtoMode_thin,
        .omit_frame_pointer = dal_c_ToggleState_enabled,
        .function_sections = dal_c_ToggleState_enabled,
        .data_sections = dal_c_ToggleState_enabled,
        .gc_sections = dal_c_ToggleState_enabled,
        .unroll_loops = dal_c_ToggleState_disabled,
        .unwind_tables = dal_c_ToggleState_disabled,
        .async_unwind_tables = dal_c_ToggleState_disabled,
        .strip_mode = dal_c_ToggleState_enabled,
        .icf_mode = dal_c_IcfMode_all,
    },
    [dal_c_Profile_fast] = {
        .name = dal_c_profile_fast,
        .opti_level = dal_c_OptiLevel_none,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = true,
        .lto_mode = dal_c_LtoMode_off,
        .omit_frame_pointer = dal_c_ToggleState_disabled,
        .function_sections = dal_c_ToggleState_disabled,
        .data_sections = dal_c_ToggleState_disabled,
        .gc_sections = dal_c_ToggleState_disabled,
        .unwind_tables = dal_c_ToggleState_disabled,
        .async_unwind_tables = dal_c_ToggleState_disabled,
    },
};
#define dal_c_profile_specs_count ((int)(sizeof(dal_c_profile_specs) / sizeof(dal_c_profile_specs[0])))
static inline const dal_c_ProfileSpec* dal_c_ProfileSpec_by(dal_c_Profile profile) {
    if (0 <= profile && profile < dal_c_profile_specs_count) {
        return &dal_c_profile_specs[profile];
    }
    return NULL;
}

/// === COMMAND ACTION ENUM ===

typedef enum dal_c_CmdAction {
    dal_c_CmdAction_invalid = -1,
    dal_c_CmdAction_version = 0,
    dal_c_CmdAction_help = 1,
    dal_c_CmdAction_build = 2,
    dal_c_CmdAction_lib = 3,
    dal_c_CmdAction_run = 4,
    dal_c_CmdAction_test = 5,
    dal_c_CmdAction_deps = 6,
    dal_c_CmdAction_clean = 7,
    dal_c_CmdAction_workspace = 8,
    dal_c_CmdAction_project = 9,
    dal_c_CmdAction_build_dsl = 10,
    dal_c_CmdAction_test_dsl = 11,
    dal_c_CmdAction_clean_dsl = 12,
    dal_c_CmdAction_build_self = 13,
    dal_c_CmdAction_clean_self = 14,
    dal_c_CmdAction_toolchain = 15,
    dal_c_CmdAction_compile_db = 16,
    dal_c_CmdAction_syntax = 17,
    dal_c_CmdAction_tidy = 18,
    dal_c_CmdAction_format_code = 19,
} dal_c_CmdAction;
#define dal_c_cmd_action_version "version"
#define dal_c_cmd_action_help "help"
#define dal_c_cmd_action_build "build"
#define dal_c_cmd_action_lib "lib"
#define dal_c_cmd_action_run "run"
#define dal_c_cmd_action_test "test"
#define dal_c_cmd_action_deps "deps"
#define dal_c_cmd_action_clean "clean"
#define dal_c_cmd_action_workspace "workspace"
#define dal_c_cmd_action_project "project"
#define dal_c_cmd_action_build_dsl "build-dsl"
#define dal_c_cmd_action_test_dsl "test-dsl"
#define dal_c_cmd_action_clean_dsl "clean-dsl"
#define dal_c_cmd_action_build_self "build-self"
#define dal_c_cmd_action_clean_self "clean-self"
#define dal_c_cmd_action_toolchain "toolchain"
#define dal_c_cmd_action_compile_db "compile-db"
#define dal_c_cmd_action_syntax "syntax"
#define dal_c_cmd_action_tidy "tidy"
#define dal_c_cmd_action_format "format"
static inline dal_c_CmdAction dal_c_CmdAction_parse(const char* str) {
    if (str_eql(str, dal_c_cmd_action_build)) { return dal_c_CmdAction_build; }
    if (str_eql(str, dal_c_cmd_action_lib)) { return dal_c_CmdAction_lib; }
    if (str_eql(str, dal_c_cmd_action_run)) { return dal_c_CmdAction_run; }
    if (str_eql(str, dal_c_cmd_action_test)) { return dal_c_CmdAction_test; }
    if (str_eql(str, dal_c_cmd_action_deps)) { return dal_c_CmdAction_deps; }
    if (str_eql(str, dal_c_cmd_action_clean)) { return dal_c_CmdAction_clean; }
    if (str_eql(str, dal_c_cmd_action_workspace)) { return dal_c_CmdAction_workspace; }
    if (str_eql(str, dal_c_cmd_action_project)) { return dal_c_CmdAction_project; }
    if (str_eql(str, dal_c_cmd_action_build_dsl)) { return dal_c_CmdAction_build_dsl; }
    if (str_eql(str, dal_c_cmd_action_test_dsl)) { return dal_c_CmdAction_test_dsl; }
    if (str_eql(str, dal_c_cmd_action_clean_dsl)) { return dal_c_CmdAction_clean_dsl; }
    if (str_eql(str, dal_c_cmd_action_build_self)) { return dal_c_CmdAction_build_self; }
    if (str_eql(str, dal_c_cmd_action_clean_self)) { return dal_c_CmdAction_clean_self; }
    if (str_eql(str, dal_c_cmd_action_toolchain)) { return dal_c_CmdAction_toolchain; }
    if (str_eql(str, dal_c_cmd_action_compile_db)) { return dal_c_CmdAction_compile_db; }
    if (str_eql(str, dal_c_cmd_action_syntax)) { return dal_c_CmdAction_syntax; }
    if (str_eql(str, dal_c_cmd_action_tidy)) { return dal_c_CmdAction_tidy; }
    if (str_eql(str, dal_c_cmd_action_format)) { return dal_c_CmdAction_format_code; }
    if (str_eql(str, dal_c_cmd_action_help)) { return dal_c_CmdAction_help; }
    if (str_eql(str, dal_c_cmd_action_version)) { return dal_c_CmdAction_version; }
    return dal_c_CmdAction_invalid;
}
static inline const char* dal_c_CmdAction_format(dal_c_CmdAction action) {
    switch (action) {
    case dal_c_CmdAction_build: return dal_c_cmd_action_build;
    case dal_c_CmdAction_lib: return dal_c_cmd_action_lib;
    case dal_c_CmdAction_run: return dal_c_cmd_action_run;
    case dal_c_CmdAction_test: return dal_c_cmd_action_test;
    case dal_c_CmdAction_deps: return dal_c_cmd_action_deps;
    case dal_c_CmdAction_clean: return dal_c_cmd_action_clean;
    case dal_c_CmdAction_workspace: return dal_c_cmd_action_workspace;
    case dal_c_CmdAction_project: return dal_c_cmd_action_project;
    case dal_c_CmdAction_build_dsl: return dal_c_cmd_action_build_dsl;
    case dal_c_CmdAction_test_dsl: return dal_c_cmd_action_test_dsl;
    case dal_c_CmdAction_clean_dsl: return dal_c_cmd_action_clean_dsl;
    case dal_c_CmdAction_build_self: return dal_c_cmd_action_build_self;
    case dal_c_CmdAction_clean_self: return dal_c_cmd_action_clean_self;
    case dal_c_CmdAction_toolchain: return dal_c_cmd_action_toolchain;
    case dal_c_CmdAction_compile_db: return dal_c_cmd_action_compile_db;
    case dal_c_CmdAction_syntax: return dal_c_cmd_action_syntax;
    case dal_c_CmdAction_tidy: return dal_c_cmd_action_tidy;
    case dal_c_CmdAction_format_code: return dal_c_cmd_action_format;
    case dal_c_CmdAction_help: return dal_c_cmd_action_help;
    case dal_c_CmdAction_version: return dal_c_cmd_action_version;
    case dal_c_CmdAction_invalid:
    default: return NULL;
    }
}

/// === COMMAND LINE OPTIONS ===

// Option syntax
#define dal_c_opt_prefix_short "-"
#define dal_c_opt_prefix_long "--"
#define dal_c_opt_value_sep "="
#define dal_c_opt_all_alias "."

// Option names (bare, without prefix)
#define dal_c_opt_compiler "compiler"
#define dal_c_opt_std "std"
#define dal_c_opt_arch "arch"
#define dal_c_opt_target "target"
#define dal_c_opt_target_arch "target-arch"
#define dal_c_opt_target_tune "target-tune"
#define dal_c_opt_target_abi "target-abi"
#define dal_c_opt_sysroot "sysroot"
#define dal_c_opt_include "include"
#define dal_c_opt_isystem "isystem"
#define dal_c_opt_link "link"
#define dal_c_opt_link_dir "link-dir"
#define dal_c_opt_define "define"
#define dal_c_opt_undef "undef"
#define dal_c_opt_lib "lib"
#define dal_c_opt_dh "dh"
#define dal_c_opt_args "args"
#define dal_c_opt_comp_args "comp-args"
#define dal_c_opt_link_args "link-args"
#define dal_c_opt_exec_args "exec-args"
#define dal_c_opt_remove "remove"

#define dal_c_opt_verbose "verbose"
#define dal_c_opt_progress "progress"
#define dal_c_opt_elapsed_precision "elapsed-precision"
#define dal_c_opt_jobs "jobs"
#define dal_c_opt_debug "debug"
#define dal_c_opt_commands "commands"
#define dal_c_opt_link_dsl "link-dsl"
#define dal_c_opt_hosted "hosted"
#define dal_c_opt_freestanding "freestanding"
#define dal_c_opt_link_libc "link-libc" // libc link fact when the target can represent it
#define dal_c_opt_link_default_libs "link-default-libs" // COMP_NO_DEFAULT_LIBS when false
#define dal_c_opt_link_start_files "link-start-files" // COMP_NO_START_FILES when false
#define dal_c_opt_link_compiler_rt "link-compiler-rt" // explicit compiler runtime restore when default libs are off
#define dal_c_opt_link_stdlib "link-stdlib" // start-files + default-libs bundle
#define dal_c_opt_link_crt "link-crt" // start-files bundle
#define dal_c_opt_link_mode "link-mode"
#define dal_c_opt_lto "lto"
#define dal_c_opt_prebuilt "prebuilt"
#define dal_c_opt_omit_frame_pointer "omit-frame-pointer"
#define dal_c_opt_function_sections "function-sections"
#define dal_c_opt_data_sections "data-sections"
#define dal_c_opt_gc_sections "gc-sections"
#define dal_c_opt_whole_archive "whole-archive"
#define dal_c_opt_unroll_loops "unroll-loops"
#define dal_c_opt_unwind_tables "unwind-tables"
#define dal_c_opt_async_unwind_tables "async-unwind-tables"
#define dal_c_opt_exceptions "exceptions"
#define dal_c_opt_strip "strip"
#define dal_c_opt_icf "icf"
#define dal_c_opt_merge_all_constants "merge-all-constants"
#define dal_c_opt_stack_protector "stack-protector"
#define dal_c_opt_emit_map "emit-map"
#define dal_c_opt_emit_linked_asm "emit-linked-asm"
#define dal_c_opt_emit_disasm "emit-disasm"
#define dal_c_opt_emit_ir "emit-ir"
#define dal_c_opt_emit_debug_info "emit-debug-info"
#define dal_c_opt_disasm_demangle "disasm-demangle"
#define dal_c_opt_disasm_source "disasm-source"
#define dal_c_opt_disasm_line_numbers "disasm-line-numbers"
#define dal_c_opt_disasm_symbolize_operands "disasm-symbolize-operands"
#define dal_c_opt_disasm_raw_insn "disasm-raw-insn"
#define dal_c_opt_disasm_section_contents "disasm-section-contents"
#define dal_c_opt_save_temps "save-temps"
#define dal_c_opt_print_link_gc "print-link-gc"
#define dal_c_opt_analysis_artifacts "analysis-artifacts"
#define dal_c_opt_entry "entry"
#define dal_c_opt_image "image"
#define dal_c_opt_emit_preprocessed "emit-preprocessed"
#define dal_c_opt_emit_asm "emit-asm"
#define dal_c_opt_link_script "link-script"
#define dal_c_opt_objcopy "objcopy"
#define dal_c_opt_objcopy_format "objcopy-format"
#define dal_c_opt_version_namespace "version-namespace"
#define dal_c_opt_version_core "version-core"
#define dal_c_opt_version_prefix "version-prefix"
#define dal_c_opt_version_suffix "version-suffix"
#define dal_c_opt_version_build "version-build"
#define dal_c_opt_version_record "version-record"
#define dal_c_opt_loose_errors "loose-errors"
#define dal_c_opt_self "self"
#define dal_c_opt_static "static"
#define dal_c_opt_shared "shared"
#define dal_c_opt_cache "cache"
#define dal_c_opt_recur "recur"
#define dal_c_opt_dsl "dsl"
#define dal_c_opt_all "all"
#define dal_c_opt_sample "sample"
#define dal_c_opt_example "example"
#define dal_c_opt_test "test"
#define dal_c_opt_output "output"
#define dal_c_opt_output_ext "output-ext"
#define dal_c_opt_file "file"
#define dal_c_opt_exclude "exclude"
#define dal_c_opt_dh_file "dh-file"
#define dal_c_opt_help "help"
#define dal_c_opt_version "version"

// Short option names (string form for concatenation)
#define dal_c_opt_include_short "I"
#define dal_c_opt_link_short "l"
#define dal_c_opt_link_dir_short "L"
#define dal_c_opt_define_short "D"
#define dal_c_opt_undef_short "U"
#define dal_c_opt_output_short "o"
#define dal_c_opt_help_short "h"
#define dal_c_opt_version_short "v"

// Short option characters (for parsing)
#define dal_c_opt_include_short_char 'I'
#define dal_c_opt_link_short_char 'l'
#define dal_c_opt_link_dir_short_char 'L'
#define dal_c_opt_define_short_char 'D'
#define dal_c_opt_undef_short_char 'U'
#define dal_c_opt_output_short_char 'o'
#define dal_c_opt_help_short_char 'h'
#define dal_c_opt_version_short_char 'v'

/// === VERSION CONTRACT ===

typedef enum dal_c_VersionRecordMode {
    dal_c_VersionRecordMode_invalid = -1,
    dal_c_VersionRecordMode_none = 0,
    dal_c_VersionRecordMode_project = 1,
    dal_c_VersionRecordMode_companion = 2,
} dal_c_VersionRecordMode;
#define dal_c_version_record_none "none"
#define dal_c_version_record_project "project"
#define dal_c_version_record_companion "companion"
static inline dal_c_VersionRecordMode dal_c_VersionRecordMode_parse(const char* str) {
    if (str_eql(str, dal_c_version_record_none)) { return dal_c_VersionRecordMode_none; }
    if (str_eql(str, dal_c_version_record_project)) { return dal_c_VersionRecordMode_project; }
    if (str_eql(str, dal_c_version_record_companion)) { return dal_c_VersionRecordMode_companion; }
    return dal_c_VersionRecordMode_invalid;
}
static inline const char* dal_c_VersionRecordMode_format(dal_c_VersionRecordMode mode) {
    switch (mode) {
    case dal_c_VersionRecordMode_none: return dal_c_version_record_none;
    case dal_c_VersionRecordMode_project: return dal_c_version_record_project;
    case dal_c_VersionRecordMode_companion: return dal_c_version_record_companion;
    case dal_c_VersionRecordMode_invalid:
    default: return NULL;
    }
}

typedef enum dal_c_LooseErrorsMode {
    dal_c_LooseErrorsMode_invalid = -1,
    dal_c_LooseErrorsMode_auto = 0,
    dal_c_LooseErrorsMode_never = 1,
    dal_c_LooseErrorsMode_strict = dal_c_LooseErrorsMode_never,
    dal_c_LooseErrorsMode_warn = 2,
    dal_c_LooseErrorsMode_suppress = 3,
} dal_c_LooseErrorsMode;
#define dal_c_loose_errors_auto "auto"
#define dal_c_loose_errors_never "never"
#define dal_c_loose_errors_strict dal_c_loose_errors_never
#define dal_c_loose_errors_off "off"
#define dal_c_loose_errors_warn "warn"
#define dal_c_loose_errors_suppress "suppress"
static inline dal_c_LooseErrorsMode dal_c_LooseErrorsMode_parse(const char* str) {
    if (str_eql(str, dal_c_loose_errors_auto)) { return dal_c_LooseErrorsMode_auto; }
    if (str_eql(str, dal_c_loose_errors_never) || str_eql(str, dal_c_loose_errors_strict) || str_eql(str, dal_c_loose_errors_off)
        || str_eql(str, "no") || str_eql(str, "false")) {
        return dal_c_LooseErrorsMode_never;
    }
    if (str_eql(str, dal_c_loose_errors_warn) || str_eql(str, "on") || str_eql(str, "yes") || str_eql(str, "true")) {
        return dal_c_LooseErrorsMode_warn;
    }
    if (str_eql(str, dal_c_loose_errors_suppress)) { return dal_c_LooseErrorsMode_suppress; }
    return dal_c_LooseErrorsMode_invalid;
}
static inline const char* dal_c_LooseErrorsMode_format(dal_c_LooseErrorsMode mode) {
    switch (mode) {
    case dal_c_LooseErrorsMode_auto: return dal_c_loose_errors_auto;
    case dal_c_LooseErrorsMode_never: return dal_c_loose_errors_never;
    case dal_c_LooseErrorsMode_warn: return dal_c_loose_errors_warn;
    case dal_c_LooseErrorsMode_suppress: return dal_c_loose_errors_suppress;
    case dal_c_LooseErrorsMode_invalid:
    default: return NULL;
    }
}

typedef enum dal_c_PrebuiltMode {
    dal_c_PrebuiltMode_invalid = -1,
    dal_c_PrebuiltMode_auto = 0,
    dal_c_PrebuiltMode_off = 1,
    dal_c_PrebuiltMode_required = 2,
} dal_c_PrebuiltMode;
#define dal_c_prebuilt_auto "auto"
#define dal_c_prebuilt_off "off"
#define dal_c_prebuilt_required "required"
static inline dal_c_PrebuiltMode dal_c_PrebuiltMode_parse(const char* str) {
    if (str_eql(str, dal_c_prebuilt_auto)) { return dal_c_PrebuiltMode_auto; }
    if (str_eql(str, dal_c_prebuilt_off) || str_eql(str, "source") || str_eql(str, "false") || str_eql(str, "no")) {
        return dal_c_PrebuiltMode_off;
    }
    if (str_eql(str, dal_c_prebuilt_required) || str_eql(str, "on") || str_eql(str, "true") || str_eql(str, "yes")) {
        return dal_c_PrebuiltMode_required;
    }
    return dal_c_PrebuiltMode_invalid;
}
static inline const char* dal_c_PrebuiltMode_format(dal_c_PrebuiltMode mode) {
    switch (mode) {
    case dal_c_PrebuiltMode_auto: return dal_c_prebuilt_auto;
    case dal_c_PrebuiltMode_off: return dal_c_prebuilt_off;
    case dal_c_PrebuiltMode_required: return dal_c_prebuilt_required;
    case dal_c_PrebuiltMode_invalid:
    default: return NULL;
    }
}

typedef struct dal_c_VersionSpec {
    char* namespace_name;
    bool namespace_set;
    unsigned core_major;
    unsigned core_minor;
    unsigned core_patch;
    bool core_set;
    int label_prefix_num;
    char* label_prefix_str;
    bool label_prefix_set;
    unsigned label_suffix_num;
    char* label_suffix_str;
    bool label_suffix_set;
    char* build_str;
    bool build_set;
} dal_c_VersionSpec;

/// === COMPILER OPTIONS (Shared) ===

typedef struct dal_c_CompilerOpts {
    char* compiler; // --compiler=<name> (NULL = auto-detect)
    char* c_std; // --std=<std> (NULL = gnu17)
    char* arch_target; // --arch or --target
    char* target_arch; // --target-arch=<arch> (for example `rv32im`)
    char* target_tune; // --target-tune=<cpu> (for example `native`)
    char* target_abi; // --target-abi=<abi> (for example `ilp32`)
    char* sysroot; // --sysroot=<path>
    char** define_macros; // --define or -D (array)
    int define_count;
    char** undef_macros; // --undef or -U (array)
    int undef_count;
    char** isystem_paths; // --isystem (array)
    int isystem_count;
    char** include_paths; // --include or -I (array)
    int include_count;
    char** link_libs; // --link or -l (array)
    int link_count;
    char** link_dirs; // --link-dir or -L (array)
    int link_dir_count;
    char* entry_symbol; // --entry=<symbol>
    dal_c_Profile profile;
    dal_c_CompileEnv compile_env; // --hosted / --freestanding
    dal_c_ToggleState libc_linked; // --link-libc=<auto|on|off>
    dal_c_ToggleState dsl_mode; // --link-dsl=<auto|on|off>
    dal_c_ToggleState default_libs_linked; // --link-default-libs=<auto|on|off>
    dal_c_ToggleState start_files_linked; // --link-start-files=<auto|on|off> / --link-crt=<auto|on|off>
    dal_c_ToggleState compiler_rt_linked; // --link-compiler-rt=<auto|on|off>
    dal_c_LinkMode link_mode; // --link-mode=<auto|static|shared>
    dal_c_LtoMode lto_mode; // --lto=<auto|off|on|full|thin>
    dal_c_PrebuiltMode prebuilt_mode; // --prebuilt=<auto|off|required>
    bool prebuilt_mode_set; // distinguishes explicit auto from inherited/default auto
    dal_c_ToggleState omit_frame_pointer; // --omit-frame-pointer=<auto|on|off>
    dal_c_ToggleState function_sections; // --function-sections=<auto|on|off>
    dal_c_ToggleState data_sections; // --data-sections=<auto|on|off>
    dal_c_ToggleState gc_sections; // --gc-sections=<auto|on|off>
    dal_c_ToggleState whole_archive; // --whole-archive=<auto|on|off>
    dal_c_ToggleState unroll_loops; // --unroll-loops=<auto|on|off>
    dal_c_ToggleState unwind_tables; // --unwind-tables=<auto|on|off>
    dal_c_ToggleState async_unwind_tables; // --async-unwind-tables=<auto|on|off>
    dal_c_ToggleState exceptions; // --exceptions=<auto|on|off>
    dal_c_ToggleState strip_mode; // --strip=<auto|on|off>
    dal_c_IcfMode icf_mode; // --icf=<off|safe|all>
    dal_c_ToggleState merge_all_constants; // --merge-all-constants=<auto|on|off>
    dal_c_ToggleState stack_protector; // --stack-protector=<auto|on|off>
    dal_c_LooseErrorsMode loose_errors; // --loose-errors=<auto|never|warn|suppress>
    dal_c_VersionSpec version; // project/file/CLI version contract
} dal_c_CompilerOpts;

typedef struct dal_c_BuildDefaults {
    char* output_name; // default output name when CLI did not provide one
    dal_c_Target target_kind; // default artifact kind; `lib` builds native/LTO static variants + shared
    bool build_runs_tests;
    bool target_kind_set;
    bool build_runs_tests_set;
} dal_c_BuildDefaults;

typedef struct dal_c_SelfRoot {
    char* path;
} dal_c_SelfRoot;

typedef struct dal_c_TargetRoot {
    char* name;
    char* path;
    dal_c_Target kind;
    dal_c_TargetSelection selection;
    bool link_project;
    bool builtin;
    char** exclude_paths;
    int exclude_count;
} dal_c_TargetRoot;

/// === ACTION-SPECIFIC PAYLOADS ===

typedef struct dal_c_BuildOpts {
    char* target_path;
    char* output_path;
    dal_c_SampleDir sample_dir; // --sample, --example, or --test
    bool build_all; // --all or "." to build all files
    bool recursive; // --recur
    bool self_boundary; // --self
    bool remove_output; // compile-db --remove
    bool as_library; // --lib
    bool as_image; // --image
    bool emit_preprocessed; // --emit-preprocessed
    bool emit_asm; // --emit-asm
    bool emit_map; // --emit-map[=<path>]
    bool emit_linked_asm; // --emit-linked-asm[=<path>]
    bool emit_disasm; // --emit-disasm[=<path>]
    bool emit_ir; // --emit-ir[=<path>]
    bool emit_debug_info; // --emit-debug-info[=<path>]
    bool print_link_gc; // --print-link-gc
    bool analysis_artifacts; // --analysis-artifacts
    char* emit_map_path;
    char* emit_linked_asm_path;
    char* emit_disasm_path;
    char* emit_ir_path;
    char* emit_debug_info_path;
    dal_c_ToggleState disasm_demangle; // --disasm-demangle=<auto|on|off>
    dal_c_ToggleState disasm_source; // --disasm-source=<auto|on|off>
    dal_c_ToggleState disasm_line_numbers; // --disasm-line-numbers=<auto|on|off>
    dal_c_ToggleState disasm_symbolize_operands; // --disasm-symbolize-operands=<auto|on|off>
    dal_c_ToggleState disasm_raw_insn; // --disasm-raw-insn=<auto|on|off>
    dal_c_ToggleState disasm_section_contents; // --disasm-section-contents=<auto|on|off>
    dal_c_SaveTempsMode save_temps; // --save-temps=<off|cwd|obj>
    dal_c_Linking linking; // --static or --shared
    bool dsl_first; // --dsl
} dal_c_BuildOpts;

typedef struct dal_c_LibOpts {
    char* target_path;
    char* output_path;
    dal_c_Linking linking; // --static or --shared (default for libraries)
} dal_c_LibOpts;

typedef struct dal_c_RunOpts {
    char* target_path;
    char* output_path;
    char* run_args; // --exec-args="..."
    dal_c_SampleDir sample_dir; // --sample, --example, or --test
    bool debug; // --debug
} dal_c_RunOpts;

typedef struct dal_c_TestOpts {
    char* target_path;
    char* output_path;
    char* run_args; // --exec-args="..."
    dal_c_SampleDir sample_dir; // --sample, --example, or --test
    bool debug;
    bool build_all; // --all or "." to build all files
    bool recursive; // --recur
    bool dsl_first; // --dsl
} dal_c_TestOpts;

typedef struct dal_c_CleanOpts {
    bool cache_only; // --cache
    bool recursive; // --recur
    bool self_boundary; // --self
    bool dsl_first; // --dsl
} dal_c_CleanOpts;

typedef struct dal_c_ScaffoldOpts {
    const char* name;
} dal_c_ScaffoldOpts;

typedef enum dal_c_ToolchainQuery {
    dal_c_ToolchainQuery_invalid = -1,
    dal_c_ToolchainQuery_all = 0,
    dal_c_ToolchainQuery_start_files = 1,
    dal_c_ToolchainQuery_compiler_rt = 2,
    dal_c_ToolchainQuery_default_libs = 3,
    dal_c_ToolchainQuery_crt = 4,
    dal_c_ToolchainQuery_stdlib = 5,
    dal_c_ToolchainQuery_libc = 6,
    dal_c_ToolchainQuery_raw_link = 7,
} dal_c_ToolchainQuery;
#define dal_c_toolchain_query_all "all"
#define dal_c_toolchain_query_start_files "start-files"
#define dal_c_toolchain_query_compiler_rt "compiler-rt"
#define dal_c_toolchain_query_default_libs "default-libs"
#define dal_c_toolchain_query_crt "crt"
#define dal_c_toolchain_query_stdlib "stdlib"
#define dal_c_toolchain_query_libc "libc"
#define dal_c_toolchain_query_raw_link "raw-link"
static inline dal_c_ToolchainQuery dal_c_ToolchainQuery_parse(const char* str) {
    if (!str || str_eql(str, dal_c_toolchain_query_all)) { return dal_c_ToolchainQuery_all; }
    if (str_eql(str, dal_c_toolchain_query_start_files)) { return dal_c_ToolchainQuery_start_files; }
    if (str_eql(str, dal_c_toolchain_query_compiler_rt)) { return dal_c_ToolchainQuery_compiler_rt; }
    if (str_eql(str, dal_c_toolchain_query_default_libs)) { return dal_c_ToolchainQuery_default_libs; }
    if (str_eql(str, dal_c_toolchain_query_crt)) { return dal_c_ToolchainQuery_crt; }
    if (str_eql(str, dal_c_toolchain_query_stdlib)) { return dal_c_ToolchainQuery_stdlib; }
    if (str_eql(str, dal_c_toolchain_query_libc)) { return dal_c_ToolchainQuery_libc; }
    if (str_eql(str, dal_c_toolchain_query_raw_link)) { return dal_c_ToolchainQuery_raw_link; }
    return dal_c_ToolchainQuery_invalid;
}
static inline const char* dal_c_ToolchainQuery_format(dal_c_ToolchainQuery query) {
    switch (query) {
    case dal_c_ToolchainQuery_all: return dal_c_toolchain_query_all;
    case dal_c_ToolchainQuery_start_files: return dal_c_toolchain_query_start_files;
    case dal_c_ToolchainQuery_compiler_rt: return dal_c_toolchain_query_compiler_rt;
    case dal_c_ToolchainQuery_default_libs: return dal_c_toolchain_query_default_libs;
    case dal_c_ToolchainQuery_crt: return dal_c_toolchain_query_crt;
    case dal_c_ToolchainQuery_stdlib: return dal_c_toolchain_query_stdlib;
    case dal_c_ToolchainQuery_libc: return dal_c_toolchain_query_libc;
    case dal_c_ToolchainQuery_raw_link: return dal_c_toolchain_query_raw_link;
    case dal_c_ToolchainQuery_invalid:
    default: return NULL;
    }
}

typedef struct dal_c_ToolchainOpts {
    dal_c_ToolchainQuery query;
} dal_c_ToolchainOpts;

/// === COMMAND PAYLOAD (TaggedUnion) ===

typedef union dal_c_CmdPayload {
    dal_c_BuildOpts build;
    dal_c_LibOpts lib;
    dal_c_RunOpts run;
    dal_c_TestOpts test;
    dal_c_CleanOpts clean;
    dal_c_ScaffoldOpts workspace;
    dal_c_ScaffoldOpts project;
    dal_c_ToolchainOpts toolchain;
} dal_c_CmdPayload;

/// === COMMAND (User Intent) ===

typedef struct dal_c_Cmd {
    dal_c_CmdAction action;
    dal_c_CmdPayload payload;
    dal_c_CompilerOpts opts;
    char** input_files; // positional or explicit source files
    int input_count;
    char** explicit_dh_files; // explicit .dh property files
    int explicit_dh_count;
    char** exclude_paths; // explicit exclude paths
    int exclude_count;

    char* compiler_args; // --args="..." or --comp-args="..."
    char* link_args; // --link-args="..."
    char* output_ext; // --output-ext=<.ext>
    char* linker_script; // --link-script=<path>
    char* objcopy; // --objcopy=<name>
    char* objcopy_format; // --objcopy-format=<fmt>
    char* dh_path_override; // --dh=<path>
    dal_c_VersionRecordMode version_record_mode; // --version-record=<mode>
    bool show_commands; // --commands=show|hide
    bool dry_run; // plan/explain: generate the real build plan without executing it
    bool explain_rebuild; // explain whether the generated plan was reused or refreshed
    bool verbose; // --verbose
    bool show_progress; // --progress=show|hide
    int elapsed_precision; // --elapsed-precision=<0..9>
    char* make_jobs; // --jobs=<n>
    bool profile_explicit; // profile token was provided explicitly by user
    bool is_help; // --help, -h
    bool is_version; // --version, -v
} dal_c_Cmd;

dal_c_Cmd* dal_c_Cmd_parse(int argc, const char* argv[]);
void dal_c_Cmd_cleanup(dal_c_Cmd** self);

typedef struct dal_c_Project dal_c_Project;

int dal_c_Cmd_execute(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_makeTarget(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_writeCompileDb(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_runSyntax(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_runTidy(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_runFormat(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_cleanTarget(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_compileDeps(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_createWorkspace(void);
int dal_c_Cmd_createProject(void);
int dal_c_Cmd_queryToolchain(const dal_c_Cmd* self);

/// === LIBRARY (Dependency) ===

typedef struct dal_c_Lib {
    char* name;
    char* path;
    char* source;      // external source URL, currently git URLs are fetchable
    char* revision;    // immutable tag/commit/branch requested by the project
    char* provider;    // dh|cmake|make|custom|prebuilt
    char* build_command;
    char* install_command;
    char** runtime_files; // package-relative runtime files exported by this dependency
    int runtime_file_count;
    dal_c_CompilerOpts opts;
    bool is_static;
    bool test_enabled;
    bool test_enabled_set;
} dal_c_Lib;

/// === PROJECT (Detected State) ===

struct dal_c_Project {
    char* root;
    char* name;
    char* dh_path;
    char* project_dh;
    char* src_dir_name;
    char* include_dir_name;
    char* tests_dir_name;
    char* samples_dir_name;
    char* examples_dir_name;
    bool pch_enabled;
    char* pch_header_override;
    char* pch_header;
    char** pch_exclude_headers;
    int pch_exclude_count;
    dal_c_SelfRoot* self_roots;
    int self_root_count;
    bool has_explicit_self_roots;
    dal_c_TargetRoot* target_roots;
    int target_root_count;
    char** exclude_paths;
    int exclude_count;
    dal_c_CompilerOpts opts;
    dal_c_BuildDefaults defaults;
    dal_c_Lib* libraries;
    int lib_count;
};

dal_c_Project* dal_c_Project_detect(const dal_c_Cmd* cmd);
dal_c_Project* dal_c_Project_detectAt(const char* lib_path, const char* dh_path);
void dal_c_Project_cleanup(dal_c_Project** self);
char* dal_c_Project_findDHInstallation(const dal_c_Cmd* cmd);
char* dal_c_Project_getBuildDir(const dal_c_Project* proj);
char* dal_c_Project_getLibDir(const dal_c_Project* proj);
char* dal_c_Project_getSrcDir(const dal_c_Project* proj);
char* dal_c_Project_getIncludeDir(const dal_c_Project* proj);
char* dal_c_Project_getTestsDir(const dal_c_Project* proj);
char* dal_c_Project_getSamplesDir(const dal_c_Project* proj);
char* dal_c_Project_getExamplesDir(const dal_c_Project* proj);
char* dal_c_Project_getCategoryDir(const dal_c_Project* proj, const char* canonical_name);
const char* dal_c_Project_getCategoryDirName(const dal_c_Project* proj, const char* canonical_name);
char* dal_c_Project_getDepsDir(const dal_c_Project* proj);

/// === DEFAULTS ===

#define dal_c_default_compiler "clang"
#define dal_c_default_objcopy "llvm-objcopy"
#define dal_c_default_c_std "gnu17"
#define dal_c_default_profile dal_c_Profile_dev
#define dal_c_default_elapsed_precision 2
#define dal_c_max_elapsed_precision 9

#define dal_c_default_compile_env "hosted"
// libc_linked default depends on compile_env: hosted -> linked, freestanding -> not linked
#define dal_c_default_libc_linked_hosted "linked"
#define dal_c_default_libc_linked_freestanding "not linked"
#define dal_c_default_libc_linked "auto (" dal_c_default_libc_linked_hosted " if hosted, " dal_c_default_libc_linked_freestanding " if freestanding)"
#define dal_c_default_default_libs_linked "linked"
#define dal_c_default_start_files_linked "linked"
#define dal_c_default_dsl_mode "auto (enabled if dh library detected)"
#define dal_c_default_linking dal_c_linking_static
#define dal_c_default_link_mode dal_c_link_mode_auto

/// === PCH HEADER DETECTION NAMES ===

#define dal_c_source_file_ext ".c"
#define dal_c_header_file_ext ".h"
#define dal_c_pch_header_target_dsl dal_c_tool_target_dsl dal_c_header_file_ext
#define dal_c_pch_header_dh "dh" dal_c_header_file_ext
#define dal_c_pch_header_dh_builtin "dh/builtin" dal_c_header_file_ext
#define dal_c_pch_header_dh_core "dh/core" dal_c_header_file_ext
#define dal_c_pch_header_dh_prl "dh/prl" dal_c_header_file_ext
#define dal_c_pch_header_dh_sys "dh/sys" dal_c_header_file_ext
#define dal_c_pch_header_dh_bundle "dh-bundle" dal_c_header_file_ext
#define dal_c_pch_header_dal "dal" dal_c_header_file_ext
#define dal_c_pch_header_da "da" dal_c_header_file_ext
#define dal_c_project_prop_pch "pch"
#define dal_c_project_prop_pch_exclude "pch-exclude"
#define dal_c_project_prop_build_runs_tests "build-runs-tests"
#define dal_c_project_prop_exclude "exclude"
#define dal_c_project_prop_self_root "self-root"
#define dal_c_project_prop_version_core dal_c_opt_version_core
#define dal_c_project_prop_version_prefix dal_c_opt_version_prefix
#define dal_c_project_prop_version_suffix dal_c_opt_version_suffix
#define dal_c_project_prop_version_build dal_c_opt_version_build
#define dal_c_project_prop_kind "kind"
#define dal_c_project_prop_selection "selection"
#define dal_c_project_prop_link_project "link-project"
#define dal_c_project_section_target_root "target-root"
#define dal_c_pch_value_auto "auto"
#define dal_c_pch_value_off "off"
#define dal_c_pch_value_deps "deps"

/// === DIRECTORY NAMES ===

#define dal_c_dir_src "src"
#define dal_c_dir_include "include"
#define dal_c_dir_lib "lib"
#define dal_c_dir_deps "deps"
#define dal_c_dir_samples "samples"
#define dal_c_dir_examples "examples"
#define dal_c_dir_tests "tests"
#define dal_c_dir_build "build"
#define dal_c_dir_cache ".cache"

#define dal_c_dir_include_alias_includes "includes"
#define dal_c_dir_include_alias_inc "inc"
#define dal_c_dir_src_alias_source "source"
#define dal_c_dir_src_alias_sources "sources"
#define dal_c_dir_tests_alias_test "test"
#define dal_c_dir_samples_alias_sample "sample"
#define dal_c_dir_examples_alias_example "example"

/// === SOURCE COLLECTION FILTERS ===

#define dal_c_source_skip_segment_build "build"
static const char* const dal_c_source_skip_segments[] = {
    dal_c_source_skip_segment_build,
    NULL,
};
#define dal_c_source_skip_segments_count ((int)(sizeof(dal_c_source_skip_segments) / sizeof(dal_c_source_skip_segments[0])) - 1)
static inline const char* const* dal_c_sourceSkipSegments(void) {
    return dal_c_source_skip_segments;
}

/// === FILE NAMES ===

#define dal_c_file_detector_ext ".dh"
#define dal_c_file_detector_project "project" dal_c_file_detector_ext
#define dal_c_file_detector_workspace "workspace" dal_c_file_detector_ext
#define dal_c_file_makefile "Makefile"

/// === EXTERNAL TOOLS ===

#define dal_c_tool_make "make"
#define dal_c_tool_ar "llvm-ar"
#define dal_c_tool_debugger "lldb"

/// === HELP SYSTEM ===

#define dal_c_help_opt_width 36

typedef struct dal_c_HelpOption {
    const char* name;
    const char* description;
} dal_c_HelpOption;

typedef struct dal_c_HelpCmd {
    const char* name;
    const char* description;
    const char* usage;
    const dal_c_HelpOption* options;
    int option_count;
    const char* const* examples;
    int example_count;
    const char* const* notes;
    int note_count;
    bool extends_build_options;
    bool implemented;
} dal_c_HelpCmd;

// Profile descriptions
typedef struct dal_c_HelpProfile {
    const char* name;
    const char* description;
} dal_c_HelpProfile;
static const dal_c_HelpOption dal_c_help_build_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_compiler dal_c_opt_value_sep "<name>", "Compiler (default: " dal_c_default_compiler ")" },
    { dal_c_opt_prefix_long dal_c_opt_std dal_c_opt_value_sep "<std>", "C standard (default: " dal_c_default_c_std ")" },
    { dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "<target>", "Target architecture" },
    { dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "<triple>", "Target triple (alternative to " dal_c_opt_prefix_long dal_c_opt_arch ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_dsl dal_c_opt_value_sep "<on|off>", "Enable or disable automatic DSL/DH library integration (default: " dal_c_default_dsl_mode ")" },
    { dal_c_opt_prefix_long dal_c_opt_hosted, "Use hosted compile semantics (default: " dal_c_default_compile_env ")" },
    { dal_c_opt_prefix_long dal_c_opt_freestanding, "Use freestanding compile semantics (`-ffreestanding`)" },
    { dal_c_opt_prefix_long dal_c_opt_link_libc dal_c_opt_value_sep "<on|off>", "Link libc, or omit it when the target can represent libc-only suppression (default: " dal_c_default_libc_linked ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_default_libs dal_c_opt_value_sep "<on|off>", "Link or omit compiler default libraries (default: " dal_c_default_default_libs_linked ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_start_files dal_c_opt_value_sep "<on|off>", "Link or omit startup files / CRT objects (default: " dal_c_default_start_files_linked ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_compiler_rt dal_c_opt_value_sep "<auto|on|off>", "Explicitly restore or omit compiler runtime when default libraries are disabled" },
    { dal_c_opt_prefix_long dal_c_opt_link_stdlib dal_c_opt_value_sep "<on|off>", "Toggle the `link-start-files` + `link-default-libs` bundle together" },
    { dal_c_opt_prefix_long dal_c_opt_link_crt dal_c_opt_value_sep "<on|off>", "Toggle the `link-start-files` bundle" },
    { dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep "<auto|static|shared>", "Select link mode for executable dependencies or library artifact kind (default: " dal_c_default_link_mode ")" },
    { dal_c_opt_prefix_long dal_c_opt_lto dal_c_opt_value_sep "<auto|off|on|full|thin>", "Override profile LTO policy for compile and link flags" },
    { dal_c_opt_prefix_long dal_c_opt_prebuilt dal_c_opt_value_sep "<auto|off|required>", "Use packaged `prebuilt/<profile>` artifacts, fall back to source, or require them" },
    { dal_c_opt_prefix_long dal_c_opt_omit_frame_pointer dal_c_opt_value_sep "<auto|on|off>", "Emit or omit frame-pointer omission flags" },
    { dal_c_opt_prefix_long dal_c_opt_function_sections dal_c_opt_value_sep "<auto|on|off>", "Override profile function section splitting (`-ffunction-sections`)" },
    { dal_c_opt_prefix_long dal_c_opt_data_sections dal_c_opt_value_sep "<auto|on|off>", "Override profile data section splitting (`-fdata-sections`)" },
    { dal_c_opt_prefix_long dal_c_opt_gc_sections dal_c_opt_value_sep "<auto|on|off>", "Override profile linker section garbage collection (`-Wl,--gc-sections`)" },
    { dal_c_opt_prefix_long dal_c_opt_whole_archive dal_c_opt_value_sep "<auto|on|off>", "Wrap explicit static archive inputs in linker whole-archive mode" },
    { dal_c_opt_prefix_long dal_c_opt_unroll_loops dal_c_opt_value_sep "<auto|on|off>", "Emit or omit loop unrolling flags" },
    { dal_c_opt_prefix_long dal_c_opt_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Override unwind table emission (`-fno-unwind-tables` when off)" },
    { dal_c_opt_prefix_long dal_c_opt_async_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Override asynchronous unwind table emission (`-fno-asynchronous-unwind-tables` when off)" },
    { dal_c_opt_prefix_long dal_c_opt_exceptions dal_c_opt_value_sep "<auto|on|off>", "Emit exception handling flags (`-fno-exceptions` when off)" },
    { dal_c_opt_prefix_long dal_c_opt_strip dal_c_opt_value_sep "<auto|on|off>", "Strip linked binary symbols (`-Wl,--strip-all` when on)" },
    { dal_c_opt_prefix_long dal_c_opt_icf dal_c_opt_value_sep "<auto|off|safe|all>", "Enable linker identical code folding (`-Wl,--icf=<mode>`)" },
    { dal_c_opt_prefix_long dal_c_opt_merge_all_constants dal_c_opt_value_sep "<auto|on|off>", "Emit or omit Clang constant merging flags" },
    { dal_c_opt_prefix_long dal_c_opt_stack_protector dal_c_opt_value_sep "<auto|on|off>", "Emit stack protector flags (`-fstack-protector-strong` or `-fno-stack-protector`)" },
    { dal_c_opt_prefix_long dal_c_opt_entry dal_c_opt_value_sep "<symbol>", "Override linker entry symbol" },
    { dal_c_opt_prefix_long dal_c_opt_target_arch dal_c_opt_value_sep "<arch>", "Target architecture sub-variant passed to compiler and linker (for example `rv32im`)" },
    { dal_c_opt_prefix_long dal_c_opt_target_tune dal_c_opt_value_sep "<cpu>", "Target CPU tuning passed to compiler (`-mtune=<cpu>`)" },
    { dal_c_opt_prefix_long dal_c_opt_target_abi dal_c_opt_value_sep "<abi>", "Target ABI passed to compiler and linker (for example `ilp32`)" },
    { dal_c_opt_prefix_long dal_c_opt_sysroot dal_c_opt_value_sep "<path>", "System root directory" },
    { dal_c_opt_prefix_long dal_c_opt_include dal_c_opt_value_sep "<path>", "Add include path (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_include_short "<path>", "Add include path (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_isystem dal_c_opt_value_sep "<path>", "Add system include path (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_link dal_c_opt_value_sep "<lib>", "Link library (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_link_short "<lib>", "Link library (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_link_dir dal_c_opt_value_sep "<path>", "Add library search directory (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_link_dir_short "<path>", "Add library search directory (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "<macro>", "Define macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_define_short "<macro>", "Define macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_undef dal_c_opt_value_sep "<macro>", "Undefine macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_undef_short "<macro>", "Undefine macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_comp_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags" },
    { dal_c_opt_prefix_long dal_c_opt_link_args dal_c_opt_value_sep "\"...\"", "Additional linker flags" },
    { dal_c_opt_prefix_long dal_c_opt_link_script dal_c_opt_value_sep "<path>", "Linker script file for freestanding links" },
    { dal_c_opt_prefix_long dal_c_opt_objcopy dal_c_opt_value_sep "<name>", "Objcopy tool for image outputs (default: " dal_c_default_objcopy ")" },
    { dal_c_opt_prefix_long dal_c_opt_objcopy_format dal_c_opt_value_sep "<fmt>", "Objcopy output format for image outputs (default: binary)" },
    { dal_c_opt_prefix_long dal_c_opt_version_core dal_c_opt_value_sep "<major.minor.patch>", "Export `<namespace>__NUM__VER_CORE_*` macros" },
    { dal_c_opt_prefix_long dal_c_opt_version_prefix dal_c_opt_value_sep "<alpha|beta|rc>", "Export prerelease label prefix macros" },
    { dal_c_opt_prefix_long dal_c_opt_version_suffix dal_c_opt_value_sep "<n>", "Export prerelease numeric suffix macros (requires prefix)" },
    { dal_c_opt_prefix_long dal_c_opt_version_build dal_c_opt_value_sep "<id>", "Export `dal_c__STR__VER_BUILD`" },
    { dal_c_opt_prefix_long dal_c_opt_version_record dal_c_opt_value_sep "<none|project|companion>", "Persist CLI version flags into `project.dh` or `{source}.dh`" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags (context-aware)" },
    { dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "<path>", "Add explicit source file (for example `.c` or `.S`) (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_output dal_c_opt_value_sep "<path>", "Override output stem or directory" },
    { dal_c_opt_prefix_short dal_c_opt_output_short "<path>", "Override output stem or directory (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_output_ext dal_c_opt_value_sep "<.ext>", "Override generated output extension for a single artifact" },
    { dal_c_opt_prefix_long dal_c_opt_exclude dal_c_opt_value_sep "<path>", "Exclude file or directory subtree (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_dh_file dal_c_opt_value_sep "<path>", "Add explicit `.dh` property file (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_loose_errors dal_c_opt_value_sep "<auto|never|warn|suppress>", "Relax warning policy: never preserves strict Werror diagnostics; warn downgrades them; suppress disables warnings" },
    { dal_c_opt_prefix_long dal_c_opt_self, "Apply `build` to the self boundary" },
    { dal_c_opt_prefix_long dal_c_opt_lib, "Build the target as a library" },
    { dal_c_opt_prefix_long dal_c_opt_image, "Build the target as a freestanding image and emit a raw binary via objcopy" },
    { dal_c_opt_prefix_long dal_c_opt_emit_preprocessed, "Emit the selected translation unit as a preprocessed `.i` file instead of linking" },
    { dal_c_opt_prefix_long dal_c_opt_emit_asm, "Emit the selected translation unit as `.s`; with `-flto`, clang emits LLVM IR instead of post-LTO machine assembly" },
    { dal_c_opt_prefix_long dal_c_opt_emit_map "[=<path>]", "Emit a linker map file for linked outputs" },
    { dal_c_opt_prefix_long dal_c_opt_emit_linked_asm "[=<path>]", "Emit post-LTO linked assembly via the linker" },
    { dal_c_opt_prefix_long dal_c_opt_emit_disasm "[=<path>]", "Emit an objdump disassembly file after linking" },
    { dal_c_opt_prefix_long dal_c_opt_emit_ir "[=<path>]", "Emit LLVM IR for the selected translation unit" },
    { dal_c_opt_prefix_long dal_c_opt_emit_debug_info "[=<path>]", "Emit debug-info metadata useful when reading disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_demangle dal_c_opt_value_sep "<auto|on|off>", "Control demangling for emitted disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_source dal_c_opt_value_sep "<auto|on|off>", "Control source interleaving for emitted disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_line_numbers dal_c_opt_value_sep "<auto|on|off>", "Control line numbers for emitted disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_symbolize_operands dal_c_opt_value_sep "<auto|on|off>", "Control operand symbolization for emitted disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_raw_insn dal_c_opt_value_sep "<auto|on|off>", "Control raw instruction bytes for emitted disassembly" },
    { dal_c_opt_prefix_long dal_c_opt_disasm_section_contents dal_c_opt_value_sep "<auto|on|off>", "Include section contents in emitted disassembly (`llvm-objdump -s` when on)" },
    { dal_c_opt_prefix_long dal_c_opt_save_temps dal_c_opt_value_sep "<off|cwd|obj>", "Ask Clang to preserve intermediate compilation files" },
    { dal_c_opt_prefix_long dal_c_opt_print_link_gc, "Ask the linker to print removed sections when supported" },
    { dal_c_opt_prefix_long dal_c_opt_analysis_artifacts, "Emit the standard linked analysis artifact bundle" },
    { dal_c_opt_prefix_long dal_c_opt_static, "Alias for `" dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep dal_c_link_mode_static "`" },
    { dal_c_opt_prefix_long dal_c_opt_shared, "Alias for `" dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep dal_c_link_mode_shared "`" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Build the project `samples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Build the project `examples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Build the project `tests` target family" },
    { dal_c_opt_prefix_long dal_c_opt_all, "Build all source files in " dal_c_dir_src "/" },
    { dal_c_opt_prefix_long dal_c_opt_dsl, "Include the DSL boundary in this command" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Apply command recursively to descendant `project.dh` projects" },
    { dal_c_opt_all_alias, "Build all source files (alternative to " dal_c_opt_prefix_long dal_c_opt_all ")" },
    { dal_c_opt_prefix_long dal_c_opt_commands "=show|hide", "Show or hide compiler/link commands" },
    { dal_c_opt_prefix_long dal_c_opt_progress "=show|hide", "Show or hide compact build progress lines" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_jobs dal_c_opt_value_sep "<n>", "Override make parallelism" },
    { dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep "<path>", "Override DH path" },
};
#define dal_c_help_build_options_count ((int)(sizeof(dal_c_help_build_options) / sizeof(dal_c_help_build_options[0])))

static const dal_c_HelpOption dal_c_help_compile_check_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_compiler dal_c_opt_value_sep "<name>", "Compiler (default: " dal_c_default_compiler ")" },
    { dal_c_opt_prefix_long dal_c_opt_std dal_c_opt_value_sep "<std>", "C standard (default: " dal_c_default_c_std ")" },
    { dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "<target>", "Target architecture" },
    { dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "<triple>", "Target triple (alternative to " dal_c_opt_prefix_long dal_c_opt_arch ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_dsl dal_c_opt_value_sep "<on|off>", "Enable or disable automatic DSL/DH library integration (default: " dal_c_default_dsl_mode ")" },
    { dal_c_opt_prefix_long dal_c_opt_hosted, "Use hosted compile semantics (default: " dal_c_default_compile_env ")" },
    { dal_c_opt_prefix_long dal_c_opt_freestanding, "Use freestanding compile semantics (`-ffreestanding`)" },
    { dal_c_opt_prefix_long dal_c_opt_link_libc dal_c_opt_value_sep "<on|off>", "Set libc compile contract macros" },
    { dal_c_opt_prefix_long dal_c_opt_link_default_libs dal_c_opt_value_sep "<on|off>", "Set default-library compile contract macros" },
    { dal_c_opt_prefix_long dal_c_opt_link_start_files dal_c_opt_value_sep "<on|off>", "Set startup-file compile contract macros" },
    { dal_c_opt_prefix_long dal_c_opt_link_compiler_rt dal_c_opt_value_sep "<auto|on|off>", "Set compiler-runtime compile contract macros" },
    { dal_c_opt_prefix_long dal_c_opt_link_stdlib dal_c_opt_value_sep "<on|off>", "Toggle the start-files + default-libs compile contract bundle" },
    { dal_c_opt_prefix_long dal_c_opt_link_crt dal_c_opt_value_sep "<on|off>", "Toggle the startup-file compile contract bundle" },
    { dal_c_opt_prefix_long dal_c_opt_lto dal_c_opt_value_sep "<auto|off|on|full|thin>", "Override profile LTO policy for compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_omit_frame_pointer dal_c_opt_value_sep "<auto|on|off>", "Emit or omit frame-pointer omission flags" },
    { dal_c_opt_prefix_long dal_c_opt_function_sections dal_c_opt_value_sep "<auto|on|off>", "Override function section splitting (`-ffunction-sections`)" },
    { dal_c_opt_prefix_long dal_c_opt_data_sections dal_c_opt_value_sep "<auto|on|off>", "Override data section splitting (`-fdata-sections`)" },
    { dal_c_opt_prefix_long dal_c_opt_unroll_loops dal_c_opt_value_sep "<auto|on|off>", "Emit or omit loop unrolling flags" },
    { dal_c_opt_prefix_long dal_c_opt_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Override unwind table emission" },
    { dal_c_opt_prefix_long dal_c_opt_async_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Override asynchronous unwind table emission" },
    { dal_c_opt_prefix_long dal_c_opt_exceptions dal_c_opt_value_sep "<auto|on|off>", "Emit exception handling flags" },
    { dal_c_opt_prefix_long dal_c_opt_merge_all_constants dal_c_opt_value_sep "<auto|on|off>", "Emit or omit Clang constant merging flags" },
    { dal_c_opt_prefix_long dal_c_opt_stack_protector dal_c_opt_value_sep "<auto|on|off>", "Emit stack protector flags" },
    { dal_c_opt_prefix_long dal_c_opt_target_arch dal_c_opt_value_sep "<arch>", "Target architecture sub-variant passed to compiler" },
    { dal_c_opt_prefix_long dal_c_opt_target_tune dal_c_opt_value_sep "<cpu>", "Target CPU tuning passed to compiler" },
    { dal_c_opt_prefix_long dal_c_opt_target_abi dal_c_opt_value_sep "<abi>", "Target ABI passed to compiler" },
    { dal_c_opt_prefix_long dal_c_opt_sysroot dal_c_opt_value_sep "<path>", "System root directory" },
    { dal_c_opt_prefix_long dal_c_opt_include dal_c_opt_value_sep "<path>", "Add include path (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_include_short "<path>", "Add include path (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_isystem dal_c_opt_value_sep "<path>", "Add system include path (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "<macro>", "Define macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_define_short "<macro>", "Define macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_undef dal_c_opt_value_sep "<macro>", "Undefine macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_undef_short "<macro>", "Undefine macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_comp_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags (context-aware)" },
    { dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "<path>", "Add explicit source file (for example `.c` or `.S`) (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_exclude dal_c_opt_value_sep "<path>", "Exclude file or directory subtree (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_dh_file dal_c_opt_value_sep "<path>", "Add explicit `.dh` property file (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_loose_errors dal_c_opt_value_sep "<auto|never|warn|suppress>", "Relax warning policy" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Select the project `samples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Select the project `examples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Select the project `tests` target family" },
    { dal_c_opt_prefix_long dal_c_opt_all, "Select all source files in " dal_c_dir_src "/" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Apply command recursively to descendant `project.dh` projects" },
    { dal_c_opt_all_alias, "Select all source files (alternative to " dal_c_opt_prefix_long dal_c_opt_all ")" },
    { dal_c_opt_prefix_long dal_c_opt_commands "=show|hide", "Show or hide tool commands" },
    { dal_c_opt_prefix_long dal_c_opt_progress "=show|hide", "Show or hide compact progress lines" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_jobs dal_c_opt_value_sep "<n>", "Override check parallelism" },
    { dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep "<path>", "Override DH path" },
};
#define dal_c_help_compile_check_options_count ((int)(sizeof(dal_c_help_compile_check_options) / sizeof(dal_c_help_compile_check_options[0])))

static const dal_c_HelpOption dal_c_help_format_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "<path>", "Add explicit source file (for example `.c` or `.h`) (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_exclude dal_c_opt_value_sep "<path>", "Exclude file or directory subtree (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_dh_file dal_c_opt_value_sep "<path>", "Add explicit `.dh` property file (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Select the project `samples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Select the project `examples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Select the project `tests` target family" },
    { dal_c_opt_prefix_long dal_c_opt_all, "Select all source files in " dal_c_dir_src "/" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Apply command recursively to descendant `project.dh` projects" },
    { dal_c_opt_all_alias, "Select all source files (alternative to " dal_c_opt_prefix_long dal_c_opt_all ")" },
    { dal_c_opt_prefix_long dal_c_opt_commands "=show|hide", "Show or hide clang-format commands" },
    { dal_c_opt_prefix_long dal_c_opt_progress "=show|hide", "Show or hide compact progress lines" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep "<path>", "Override DH path" },
};
#define dal_c_help_format_options_count ((int)(sizeof(dal_c_help_format_options) / sizeof(dal_c_help_format_options[0])))

static const char* const dal_c_help_build_examples[] = {
    dal_c_cmd_action_build " " dal_c_profile_dev,
    dal_c_cmd_action_build " " dal_c_profile_release " src/main.c",
    dal_c_cmd_action_build " cmd/runner1",
    dal_c_cmd_action_build " plugins/render",
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "x86_64 " dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "DEBUG",
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_sample,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_example,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_test,
    dal_c_cmd_action_build " " dal_c_opt_all_alias,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_all,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_self,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_lib " " dal_c_opt_prefix_long dal_c_opt_shared " src/mylib.c",
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "tests/test-a.c " dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "tests/test-b.c",
    dal_c_cmd_action_build " " dal_c_profile_release " " dal_c_opt_prefix_long dal_c_opt_image " " dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "riscv32-unknown-elf " dal_c_opt_prefix_long dal_c_opt_freestanding " " dal_c_opt_prefix_long dal_c_opt_link_stdlib dal_c_opt_value_sep "off " dal_c_opt_prefix_long dal_c_opt_link_script dal_c_opt_value_sep "linker.ld " dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "crt0.S " dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "guest.c",
    dal_c_cmd_action_build " " dal_c_opt_prefix_short dal_c_opt_output_short "bundle tests/test-a.c tests/test-b.c",
};
#define dal_c_help_build_examples_count ((int)(sizeof(dal_c_help_build_examples) / sizeof(dal_c_help_build_examples[0])))

static const char* const dal_c_help_build_notes[] = {
    "Default profile is `dev`; default compiler is `" dal_c_default_compiler "`; default C standard is `" dal_c_default_c_std "`.",
    "`--output` is interpreted as an existing directory or as an output stem; platform artifact extensions are generated.",
    "`--output-ext=<.ext>` overrides the generated extension for one artifact, for example `--shared --output _mad --output-ext=.pyd`.",
    "`kind=lib` / `--lib` with `link-mode=auto` emits both static and shared libraries, so it cannot use one `--output-ext`.",
    "Use structured `--link-dir`/`-L` plus `--link`/`-l` before falling back to raw `--link-args`.",
};
#define dal_c_help_build_notes_count ((int)(sizeof(dal_c_help_build_notes) / sizeof(dal_c_help_build_notes[0])))

static const dal_c_HelpOption dal_c_help_lib_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep "<auto|static|shared>", "Select library artifact kind (default: " dal_c_default_linking ")" },
    { dal_c_opt_prefix_long dal_c_opt_static, "Alias for `" dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep dal_c_link_mode_static "`" },
    { dal_c_opt_prefix_long dal_c_opt_shared, "Alias for `" dal_c_opt_prefix_long dal_c_opt_link_mode dal_c_opt_value_sep dal_c_link_mode_shared "`" },
};
#define dal_c_help_lib_options_count ((int)(sizeof(dal_c_help_lib_options) / sizeof(dal_c_help_lib_options[0])))

static const char* const dal_c_help_lib_examples[] = {
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_lib,
    dal_c_cmd_action_build " " dal_c_profile_release " " dal_c_opt_prefix_long dal_c_opt_lib " src/mylib.c " dal_c_opt_prefix_long dal_c_opt_shared,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_lib " myheader.h",
};
#define dal_c_help_lib_examples_count ((int)(sizeof(dal_c_help_lib_examples) / sizeof(dal_c_help_lib_examples[0])))

static const char* const dal_c_help_lib_notes[] = {
    "`lib` is an alias for `build --lib`; prefer `build --lib` in new scripts.",
    "It accepts the same compile, link, selection, output, profile, and diagnostic options as `build`.",
    "`--static` emits only the static library; `--shared` emits only the shared library; `auto` emits both.",
    "When both static and shared outputs are emitted, `--output` must be a directory or stem shared by both generated names.",
};
#define dal_c_help_lib_notes_count ((int)(sizeof(dal_c_help_lib_notes) / sizeof(dal_c_help_lib_notes[0])))

static const dal_c_HelpOption dal_c_help_run_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_debug, "Launch debugger" },
    { dal_c_opt_prefix_long dal_c_opt_exec_args dal_c_opt_value_sep "\"...\"", "Runtime arguments" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Runtime arguments (context-aware)" },
};
#define dal_c_help_run_options_count ((int)(sizeof(dal_c_help_run_options) / sizeof(dal_c_help_run_options[0])))

static const char* const dal_c_help_run_examples[] = {
    dal_c_cmd_action_run " " dal_c_profile_dev,
    dal_c_cmd_action_run " cmd/runner1",
    dal_c_cmd_action_run " " dal_c_opt_prefix_long dal_c_opt_debug " " dal_c_opt_prefix_long dal_c_opt_exec_args dal_c_opt_value_sep "\"arg1 arg2\"",
};
#define dal_c_help_run_examples_count ((int)(sizeof(dal_c_help_run_examples) / sizeof(dal_c_help_run_examples[0])))

static const char* const dal_c_help_run_notes[] = {
    "`run` first builds the executable target, then starts it once.",
    "Accepted build-compatible groups: compiler/target/profile flags, `-I`/`-D`/`-U`, `--link-dir`, `--link`, `--link-args`, source selection, and `--output`.",
    "`--args` means runtime arguments for `run`; use `--comp-args` when you need extra compiler flags.",
    "Library/image/analysis artifact options are build-only and are not accepted by `run`.",
};
#define dal_c_help_run_notes_count ((int)(sizeof(dal_c_help_run_notes) / sizeof(dal_c_help_run_notes[0])))

static const dal_c_HelpOption dal_c_help_test_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_debug, "Launch debugger" },
    { dal_c_opt_prefix_long dal_c_opt_exec_args dal_c_opt_value_sep "\"...\"", "Test arguments" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Test arguments (context-aware)" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Test the project `samples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Test the project `examples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Test the project `tests` target family" },
};
#define dal_c_help_test_options_count ((int)(sizeof(dal_c_help_test_options) / sizeof(dal_c_help_test_options[0])))

static const char* const dal_c_help_test_examples[] = {
    dal_c_cmd_action_test " " dal_c_profile_dev,
    dal_c_cmd_action_test " tests/test-parser.c",
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_example " example-color.c",
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_debug " sample.c",
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_dsl " " dal_c_opt_prefix_long dal_c_opt_recur,
};
#define dal_c_help_test_examples_count ((int)(sizeof(dal_c_help_test_examples) / sizeof(dal_c_help_test_examples[0])))

static const char* const dal_c_help_test_notes[] = {
    "`test` builds selected tests and runs the generated test executable once.",
    "Accepted build-compatible groups: compiler/target/profile flags, `-I`/`-D`/`-U`, `--link-dir`, `--link`, `--link-args`, source selection, and `--output`.",
    "`--sample`, `--example`, and `--test` select which target family is tested.",
    "`--args` means test runtime arguments for `test`; use `--comp-args` for compiler flags.",
};
#define dal_c_help_test_notes_count ((int)(sizeof(dal_c_help_test_notes) / sizeof(dal_c_help_test_notes[0])))

static const dal_c_HelpOption dal_c_help_deps_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_prebuilt dal_c_opt_value_sep "<auto|off|required>", "Use packaged dependency artifacts, force source builds, or require prebuilt artifacts" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
};
#define dal_c_help_deps_options_count ((int)(sizeof(dal_c_help_deps_options) / sizeof(dal_c_help_deps_options[0])))

static const char* const dal_c_help_deps_examples[] = {
    dal_c_cmd_action_deps,
    dal_c_cmd_action_deps " stable",
    dal_c_cmd_action_deps " " dal_c_opt_prefix_long dal_c_opt_verbose,
    "fetch",
    "update SDL",
    "status",
};
#define dal_c_help_deps_examples_count ((int)(sizeof(dal_c_help_deps_examples) / sizeof(dal_c_help_deps_examples[0])))

static const char* const dal_c_help_deps_notes[] = {
    "`deps` builds libraries declared in `project.dh`; it does not build the current project output.",
    "`fetch`, `update`, and `status` use the current `project.dh` as their implicit subject.",
    "`deps` builds declared dependencies; external providers are built and privately installed into target/profile package caches.",
    "It reads dependency blocks from `project.dh`; direct compile/link/source/output flags are not accepted by `deps`.",
    "Generated dependency headers and libraries live under `lib/`; PCH files live in the active cache plan.",
    "`--prebuilt=auto` reads `prebuilt/<profile>` packages when present; `required` fails instead of compiling source.",
};
#define dal_c_help_deps_notes_count ((int)(sizeof(dal_c_help_deps_notes) / sizeof(dal_c_help_deps_notes[0])))

static const dal_c_HelpOption dal_c_help_toolchain_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_compiler dal_c_opt_value_sep "<name>", "Compiler to query (default: " dal_c_default_compiler ")" },
    { dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "<triple>", "Target triple used for the query" },
    { dal_c_opt_prefix_long dal_c_opt_sysroot dal_c_opt_value_sep "<path>", "System root used for the query" },
    { dal_c_opt_prefix_long dal_c_opt_target_arch dal_c_opt_value_sep "<arch>", "Target architecture sub-variant used for the query" },
    { dal_c_opt_prefix_long dal_c_opt_target_abi dal_c_opt_value_sep "<abi>", "Target ABI used for the query" },
};
#define dal_c_help_toolchain_options_count ((int)(sizeof(dal_c_help_toolchain_options) / sizeof(dal_c_help_toolchain_options[0])))

static const char* const dal_c_help_toolchain_examples[] = {
    dal_c_cmd_action_toolchain,
    dal_c_cmd_action_toolchain " " dal_c_toolchain_query_start_files,
    dal_c_cmd_action_toolchain " " dal_c_toolchain_query_compiler_rt,
    dal_c_cmd_action_toolchain " " dal_c_toolchain_query_default_libs,
    dal_c_cmd_action_toolchain " " dal_c_toolchain_query_raw_link,
};
#define dal_c_help_toolchain_examples_count ((int)(sizeof(dal_c_help_toolchain_examples) / sizeof(dal_c_help_toolchain_examples[0])))

static const char* const dal_c_help_toolchain_notes[] = {
    "`toolchain` asks the compiler driver which implicit CRT/start/runtime/default libraries it would use.",
    "Use it when freestanding or cross-target link options need to match the compiler's own contract.",
};
#define dal_c_help_toolchain_notes_count ((int)(sizeof(dal_c_help_toolchain_notes) / sizeof(dal_c_help_toolchain_notes[0])))

static const dal_c_HelpOption dal_c_help_compile_db_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_compiler dal_c_opt_value_sep "<name>", "Compiler recorded in compile commands (default: " dal_c_default_compiler ")" },
    { dal_c_opt_prefix_long dal_c_opt_std dal_c_opt_value_sep "<std>", "C standard recorded in compile commands (default: " dal_c_default_c_std ")" },
    { dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "<target>", "Target architecture" },
    { dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "<triple>", "Target triple (alternative to " dal_c_opt_prefix_long dal_c_opt_arch ")" },
    { dal_c_opt_prefix_long dal_c_opt_link_dsl dal_c_opt_value_sep "<on|off>", "Enable or disable automatic DSL/DH include integration (default: " dal_c_default_dsl_mode ")" },
    { dal_c_opt_prefix_long dal_c_opt_hosted, "Use hosted compile semantics (default: " dal_c_default_compile_env ")" },
    { dal_c_opt_prefix_long dal_c_opt_freestanding, "Use freestanding compile semantics (`-ffreestanding`)" },
    { dal_c_opt_prefix_long dal_c_opt_lto dal_c_opt_value_sep "<auto|off|on|full|thin>", "Record profile LTO compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_omit_frame_pointer dal_c_opt_value_sep "<auto|on|off>", "Record frame-pointer compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_function_sections dal_c_opt_value_sep "<auto|on|off>", "Record function section compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_data_sections dal_c_opt_value_sep "<auto|on|off>", "Record data section compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_unroll_loops dal_c_opt_value_sep "<auto|on|off>", "Record loop unrolling compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Record unwind table compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_async_unwind_tables dal_c_opt_value_sep "<auto|on|off>", "Record asynchronous unwind table compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_exceptions dal_c_opt_value_sep "<auto|on|off>", "Record exception handling compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_merge_all_constants dal_c_opt_value_sep "<auto|on|off>", "Record Clang constant merging compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_stack_protector dal_c_opt_value_sep "<auto|on|off>", "Record stack protector compile flags" },
    { dal_c_opt_prefix_long dal_c_opt_target_arch dal_c_opt_value_sep "<arch>", "Target architecture sub-variant recorded in compile commands" },
    { dal_c_opt_prefix_long dal_c_opt_target_tune dal_c_opt_value_sep "<cpu>", "Target CPU tuning recorded in compile commands" },
    { dal_c_opt_prefix_long dal_c_opt_target_abi dal_c_opt_value_sep "<abi>", "Target ABI recorded in compile commands" },
    { dal_c_opt_prefix_long dal_c_opt_sysroot dal_c_opt_value_sep "<path>", "System root directory" },
    { dal_c_opt_prefix_long dal_c_opt_include dal_c_opt_value_sep "<path>", "Add include path (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_include_short "<path>", "Add include path (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_isystem dal_c_opt_value_sep "<path>", "Add system include path (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "<macro>", "Define macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_define_short "<macro>", "Define macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_undef dal_c_opt_value_sep "<macro>", "Undefine macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_undef_short "<macro>", "Undefine macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_comp_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags recorded verbatim" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags recorded verbatim (context-aware)" },
    { dal_c_opt_prefix_long dal_c_opt_file dal_c_opt_value_sep "<path>", "Add explicit source file (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_output dal_c_opt_value_sep "<path>", "Write compilation database to this JSON file path" },
    { dal_c_opt_prefix_short dal_c_opt_output_short "<path>", "Write compilation database to this JSON file path (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_remove, "Remove generated compilation database" },
    { dal_c_opt_prefix_long dal_c_opt_exclude dal_c_opt_value_sep "<path>", "Exclude file or directory subtree (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_dh_file dal_c_opt_value_sep "<path>", "Add explicit `.dh` property file (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_loose_errors dal_c_opt_value_sep "<auto|never|warn|suppress>", "Record relaxed warning policy" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Select the project `samples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Select the project `examples` target family" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Select the project `tests` target family" },
    { dal_c_opt_prefix_long dal_c_opt_all, "Select all source files in " dal_c_dir_src "/" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Apply command recursively to descendant `project.dh` projects" },
    { dal_c_opt_all_alias, "Select all source files (alternative to " dal_c_opt_prefix_long dal_c_opt_all ")" },
    { dal_c_opt_prefix_long dal_c_opt_commands "=show|hide", "Show or hide generated command lines" },
    { dal_c_opt_prefix_long dal_c_opt_progress "=show|hide", "Show or hide compact progress lines" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_jobs dal_c_opt_value_sep "<n>", "Override compile-db generation parallelism" },
    { dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep "<path>", "Override DH path" },
};
#define dal_c_help_compile_db_options_count ((int)(sizeof(dal_c_help_compile_db_options) / sizeof(dal_c_help_compile_db_options[0])))

static const char* const dal_c_help_compile_db_examples[] = {
    dal_c_cmd_action_compile_db,
    dal_c_cmd_action_compile_db " " dal_c_opt_prefix_long dal_c_opt_remove,
    dal_c_cmd_action_compile_db " " dal_c_profile_dev " " dal_c_opt_prefix_short dal_c_opt_output_short "compile_commands.json",
    dal_c_cmd_action_compile_db " " dal_c_opt_prefix_long dal_c_opt_all,
    dal_c_cmd_action_compile_db " src/main.c",
};
#define dal_c_help_compile_db_examples_count ((int)(sizeof(dal_c_help_compile_db_examples) / sizeof(dal_c_help_compile_db_examples[0])))

static const char* const dal_c_help_compile_db_notes[] = {
    "`compile-db` writes `compile_commands.json`; it does not compile, link, run, or emit artifacts.",
    "`--output` names the JSON file path. `--output-ext`, link inputs, linker scripts, image, and emit-artifact flags are not accepted.",
    "Compiler, preprocessor, target, DH, and source-selection options are accepted because they change compile commands.",
};
#define dal_c_help_compile_db_notes_count ((int)(sizeof(dal_c_help_compile_db_notes) / sizeof(dal_c_help_compile_db_notes[0])))

static const char* const dal_c_help_syntax_examples[] = {
    dal_c_cmd_action_syntax " " dal_c_profile_dev,
    dal_c_cmd_action_syntax " " dal_c_profile_dev " src/main.c",
    dal_c_cmd_action_syntax " " dal_c_profile_dev " " dal_c_opt_prefix_long dal_c_opt_all,
};
#define dal_c_help_syntax_examples_count ((int)(sizeof(dal_c_help_syntax_examples) / sizeof(dal_c_help_syntax_examples[0])))

static const char* const dal_c_help_syntax_notes[] = {
    "`syntax` runs compiler syntax-only checks and never links.",
    "Compiler, preprocessor, target, DH, and source-selection options are accepted.",
    "Output, link input, linker, library kind, image, and emit-artifact options are not accepted.",
};
#define dal_c_help_syntax_notes_count ((int)(sizeof(dal_c_help_syntax_notes) / sizeof(dal_c_help_syntax_notes[0])))

static const char* const dal_c_help_tidy_examples[] = {
    dal_c_cmd_action_tidy " " dal_c_profile_dev,
    dal_c_cmd_action_tidy " " dal_c_profile_dev " src/main.c",
    dal_c_cmd_action_tidy " " dal_c_profile_dev " " dal_c_opt_prefix_long dal_c_opt_all,
};
#define dal_c_help_tidy_examples_count ((int)(sizeof(dal_c_help_tidy_examples) / sizeof(dal_c_help_tidy_examples[0])))

static const char* const dal_c_help_tidy_notes[] = {
    "`tidy` runs clang-tidy using dh-c's compile command resolution and never links.",
    "Compiler, preprocessor, target, DH, and source-selection options are accepted.",
    "Output, link input, linker, library kind, image, and emit-artifact options are not accepted.",
};
#define dal_c_help_tidy_notes_count ((int)(sizeof(dal_c_help_tidy_notes) / sizeof(dal_c_help_tidy_notes[0])))

static const char* const dal_c_help_format_examples[] = {
    dal_c_cmd_action_format,
    dal_c_cmd_action_format " src/main.c",
    dal_c_cmd_action_format " " dal_c_opt_prefix_long dal_c_opt_all,
};
#define dal_c_help_format_examples_count ((int)(sizeof(dal_c_help_format_examples) / sizeof(dal_c_help_format_examples[0])))

static const char* const dal_c_help_format_notes[] = {
    "`format` rewrites selected source/header files in place through clang-format.",
    "Only source selection, recursion, DH discovery, and output-visibility options are accepted.",
    "Compiler, preprocessor, linker, output artifact, profile-tuning, and runtime options are not accepted.",
};
#define dal_c_help_format_notes_count ((int)(sizeof(dal_c_help_format_notes) / sizeof(dal_c_help_format_notes[0])))

static const dal_c_HelpOption dal_c_help_clean_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_cache, "Clean only cache" },
    { dal_c_opt_prefix_long dal_c_opt_self, "Apply `clean` to the self boundary" },
    { dal_c_opt_prefix_long dal_c_opt_dsl, "Include the DSL boundary in `clean`" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Recursive clean" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
};
#define dal_c_help_clean_options_count ((int)(sizeof(dal_c_help_clean_options) / sizeof(dal_c_help_clean_options[0])))

static const dal_c_HelpOption dal_c_help_clean_dsl_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_cache, "Clean only cache" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
};
#define dal_c_help_clean_dsl_options_count ((int)(sizeof(dal_c_help_clean_dsl_options) / sizeof(dal_c_help_clean_dsl_options[0])))

static const char* const dal_c_help_clean_examples[] = {
    dal_c_cmd_action_clean,
    dal_c_cmd_action_clean " " dal_c_profile_dev,
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_self,
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_cache " " dal_c_opt_prefix_long dal_c_opt_recur,
};
#define dal_c_help_clean_examples_count ((int)(sizeof(dal_c_help_clean_examples) / sizeof(dal_c_help_clean_examples[0])))

static const char* const dal_c_help_clean_notes[] = {
    "`clean` removes generated build products. With `--recur`, it also cleans descendant projects.",
    "Profile-specific clean such as `clean dev` also removes `lib/deps` and `lib/deps.h` because dependency exports are not profile-scoped.",
    "Do not store durable source assets, checked-in resources, or manual files under `build/`, `build/.cache/`, `lib/deps/`, or `lib/deps.h`; clean owns those generated paths.",
    "`--dsl` includes the DH/DSL dependency boundary; `--self` cleans only the dh-c self boundary.",
};
#define dal_c_help_clean_notes_count ((int)(sizeof(dal_c_help_clean_notes) / sizeof(dal_c_help_clean_notes[0])))

static const char* const dal_c_help_build_dsl_examples[] = {
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_build " " dal_c_profile_release " " dal_c_opt_prefix_long dal_c_opt_dsl,
};
#define dal_c_help_build_dsl_examples_count ((int)(sizeof(dal_c_help_build_dsl_examples) / sizeof(dal_c_help_build_dsl_examples[0])))

static const char* const dal_c_help_test_dsl_examples[] = {
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_dsl " test-ListSgl.c",
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_dsl " " dal_c_opt_prefix_long dal_c_opt_debug " test-meta.c",
};
#define dal_c_help_test_dsl_examples_count ((int)(sizeof(dal_c_help_test_dsl_examples) / sizeof(dal_c_help_test_dsl_examples[0])))

static const char* const dal_c_help_clean_dsl_examples[] = {
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_dsl,
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_dsl " " dal_c_opt_prefix_long dal_c_opt_cache,
};
#define dal_c_help_clean_dsl_examples_count ((int)(sizeof(dal_c_help_clean_dsl_examples) / sizeof(dal_c_help_clean_dsl_examples[0])))

static const char* const dal_c_help_dsl_notes[] = {
    "Compatibility alias. Prefer the canonical command form with `--dsl` in new scripts.",
    "`--dsl` includes the DH/DSL boundary before the requested project action.",
    "The canonical command owns the option contract: `build --dsl`, `test --dsl`, or `clean --dsl`.",
};
#define dal_c_help_dsl_notes_count ((int)(sizeof(dal_c_help_dsl_notes) / sizeof(dal_c_help_dsl_notes[0])))

static const dal_c_HelpOption dal_c_help_self_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_commands "=show|hide", "Show or hide compiler/link commands" },
    { dal_c_opt_prefix_long dal_c_opt_progress "=show|hide", "Show or hide compact build progress lines" },
    { dal_c_opt_prefix_long dal_c_opt_elapsed_precision dal_c_opt_value_sep "<0..9>", "Decimal places for elapsed-time output" },
    { dal_c_opt_prefix_long dal_c_opt_verbose "=on|off", "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_jobs dal_c_opt_value_sep "<n>", "Override make parallelism" },
};
#define dal_c_help_self_options_count ((int)(sizeof(dal_c_help_self_options) / sizeof(dal_c_help_self_options[0])))

static const char* const dal_c_help_build_self_examples[] = {
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_self,
    dal_c_cmd_action_build_self " " dal_c_profile_optimize,
};
#define dal_c_help_build_self_examples_count ((int)(sizeof(dal_c_help_build_self_examples) / sizeof(dal_c_help_build_self_examples[0])))

static const char* const dal_c_help_clean_self_examples[] = {
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_self,
};
#define dal_c_help_clean_self_examples_count ((int)(sizeof(dal_c_help_clean_self_examples) / sizeof(dal_c_help_clean_self_examples[0])))

static const char* const dal_c_help_self_notes[] = {
    "Compatibility alias. Prefer `build --self` or `clean --self` in new scripts.",
    "The self boundary is dh-c itself and does not accept project source, output, link, or DH options.",
};
#define dal_c_help_self_notes_count ((int)(sizeof(dal_c_help_self_notes) / sizeof(dal_c_help_self_notes[0])))

static const dal_c_HelpOption dal_c_help_global_options[] = {
    { dal_c_opt_prefix_short dal_c_opt_help_short ", " dal_c_opt_prefix_long dal_c_opt_help, "Show this help message" },
    { dal_c_opt_prefix_short dal_c_opt_version_short ", " dal_c_opt_prefix_long dal_c_opt_version, "Show version information" },
};
#define dal_c_help_global_options_count ((int)(sizeof(dal_c_help_global_options) / sizeof(dal_c_help_global_options[0])))

static const dal_c_HelpProfile dal_c_help_profiles[] = {
    [dal_c_Profile_dev] = { dal_c_profile_dev, "Debug build with assertions (-g3 -Og)" },
    [dal_c_Profile_fast] = { dal_c_profile_fast, "Fast compile build with assertions (-O0, no debug info)" },
    [dal_c_Profile_test] = { dal_c_profile_test, "Test build with basic optimization (-g -O1)" },
    [dal_c_Profile_profile] = { dal_c_profile_profile, "Profile build with optimization (-g -O2)" },
    [dal_c_Profile_stable] = { dal_c_profile_stable, "Stable build with ThinLTO (-g1 -O2)" },
    [dal_c_Profile_release] = { dal_c_profile_release, "Release build with ThinLTO, no exceptions/unwind, and safe ICF (-g1 -O3)" },
    [dal_c_Profile_optimize] = { dal_c_profile_optimize, "Maximum optimization with Full LTO (-O3 -march=native -mtune=native -fno-exceptions)" },
    [dal_c_Profile_compact] = { dal_c_profile_compact, "Size-optimized build with ThinLTO (-Os)" },
    [dal_c_Profile_micro] = { dal_c_profile_micro, "Extreme size optimization with ThinLTO (-Oz)" },
};
#define dal_c_help_profiles_count ((int)(sizeof(dal_c_help_profiles) / sizeof(dal_c_help_profiles[0])))

static const dal_c_HelpCmd dal_c_help_cmds[] = {
    {
        .name = dal_c_cmd_action_build,
        .description = "Build project, path, library target, or self boundary",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_build_options,
        .option_count = dal_c_help_build_options_count,
        .examples = dal_c_help_build_examples,
        .example_count = dal_c_help_build_examples_count,
        .notes = dal_c_help_build_notes,
        .note_count = dal_c_help_build_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_lib,
        .description = "Compatibility alias for `build --lib`",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_lib_options,
        .option_count = dal_c_help_lib_options_count,
        .examples = dal_c_help_lib_examples,
        .example_count = dal_c_help_lib_examples_count,
        .notes = dal_c_help_lib_notes,
        .note_count = dal_c_help_lib_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_run,
        .description = "Build and run project, file, or declared target-root path",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_run_options,
        .option_count = dal_c_help_run_options_count,
        .examples = dal_c_help_run_examples,
        .example_count = dal_c_help_run_examples_count,
        .notes = dal_c_help_run_notes,
        .note_count = dal_c_help_run_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_test,
        .description = "Build and run tests",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_test_options,
        .option_count = dal_c_help_test_options_count,
        .examples = dal_c_help_test_examples,
        .example_count = dal_c_help_test_examples_count,
        .notes = dal_c_help_test_notes,
        .note_count = dal_c_help_test_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_deps,
        .description = "Build dependencies from " dal_c_file_detector_project,
        .usage = "[profile] [options]",
        .options = dal_c_help_deps_options,
        .option_count = dal_c_help_deps_options_count,
        .examples = dal_c_help_deps_examples,
        .example_count = dal_c_help_deps_examples_count,
        .notes = dal_c_help_deps_notes,
        .note_count = dal_c_help_deps_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_toolchain,
        .description = "Query compiler driver start files, runtime archive, and default link libraries",
        .usage = "[" dal_c_toolchain_query_all "|" dal_c_toolchain_query_start_files "|" dal_c_toolchain_query_compiler_rt "|" dal_c_toolchain_query_default_libs "|" dal_c_toolchain_query_crt "|" dal_c_toolchain_query_stdlib "|" dal_c_toolchain_query_libc "|" dal_c_toolchain_query_raw_link "] [options]",
        .options = dal_c_help_toolchain_options,
        .option_count = dal_c_help_toolchain_options_count,
        .examples = dal_c_help_toolchain_examples,
        .example_count = dal_c_help_toolchain_examples_count,
        .notes = dal_c_help_toolchain_notes,
        .note_count = dal_c_help_toolchain_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_compile_db,
        .description = "Write a clang-compatible compilation database without building",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_compile_db_options,
        .option_count = dal_c_help_compile_db_options_count,
        .examples = dal_c_help_compile_db_examples,
        .example_count = dal_c_help_compile_db_examples_count,
        .notes = dal_c_help_compile_db_notes,
        .note_count = dal_c_help_compile_db_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_syntax,
        .description = "Run compiler syntax-only checks without linking",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_compile_check_options,
        .option_count = dal_c_help_compile_check_options_count,
        .examples = dal_c_help_syntax_examples,
        .example_count = dal_c_help_syntax_examples_count,
        .notes = dal_c_help_syntax_notes,
        .note_count = dal_c_help_syntax_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_tidy,
        .description = "Run clang-tidy using dh-c's compilation database",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_compile_check_options,
        .option_count = dal_c_help_compile_check_options_count,
        .examples = dal_c_help_tidy_examples,
        .example_count = dal_c_help_tidy_examples_count,
        .notes = dal_c_help_tidy_notes,
        .note_count = dal_c_help_tidy_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_format,
        .description = "Run clang-format in-place on selected source files",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_format_options,
        .option_count = dal_c_help_format_options_count,
        .examples = dal_c_help_format_examples,
        .example_count = dal_c_help_format_examples_count,
        .notes = dal_c_help_format_notes,
        .note_count = dal_c_help_format_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_clean,
        .description = "Clean build artifacts",
        .usage = "[options]",
        .options = dal_c_help_clean_options,
        .option_count = dal_c_help_clean_options_count,
        .examples = dal_c_help_clean_examples,
        .example_count = dal_c_help_clean_examples_count,
        .notes = dal_c_help_clean_notes,
        .note_count = dal_c_help_clean_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_build_dsl,
        .description = "Compatibility alias for `build --dsl`",
        .usage = "[profile] [options]",
        .options = dal_c_help_build_options,
        .option_count = dal_c_help_build_options_count,
        .examples = dal_c_help_build_dsl_examples,
        .example_count = dal_c_help_build_dsl_examples_count,
        .notes = dal_c_help_dsl_notes,
        .note_count = dal_c_help_dsl_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_test_dsl,
        .description = "Compatibility alias for `test --dsl`",
        .usage = "[profile] [path] [options]",
        .options = dal_c_help_test_options,
        .option_count = dal_c_help_test_options_count,
        .examples = dal_c_help_test_dsl_examples,
        .example_count = dal_c_help_test_dsl_examples_count,
        .notes = dal_c_help_dsl_notes,
        .note_count = dal_c_help_dsl_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_clean_dsl,
        .description = "Compatibility alias for `clean --dsl`",
        .usage = "[options]",
        .options = dal_c_help_clean_dsl_options,
        .option_count = dal_c_help_clean_dsl_options_count,
        .examples = dal_c_help_clean_dsl_examples,
        .example_count = dal_c_help_clean_dsl_examples_count,
        .notes = dal_c_help_dsl_notes,
        .note_count = dal_c_help_dsl_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_build_self,
        .description = "Compatibility alias for `build --self`",
        .usage = "[profile] [options]",
        .options = dal_c_help_self_options,
        .option_count = dal_c_help_self_options_count,
        .examples = dal_c_help_build_self_examples,
        .example_count = dal_c_help_build_self_examples_count,
        .notes = dal_c_help_self_notes,
        .note_count = dal_c_help_self_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_clean_self,
        .description = "Compatibility alias for `clean --self`",
        .usage = "[options]",
        .options = dal_c_help_self_options,
        .option_count = dal_c_help_self_options_count,
        .examples = dal_c_help_clean_self_examples,
        .example_count = dal_c_help_clean_self_examples_count,
        .notes = dal_c_help_self_notes,
        .note_count = dal_c_help_self_notes_count,
        .implemented = true,
    },
    {
        .name = dal_c_cmd_action_workspace,
        .description = "Reserved scaffold command",
        .usage = "[path]",
    },
    {
        .name = dal_c_cmd_action_project,
        .description = "Reserved scaffold command",
        .usage = "[name]",
    },
};
#define dal_c_help_cmds_count ((int)(sizeof(dal_c_help_cmds) / sizeof(dal_c_help_cmds[0])))

/*========== Macros and Definitions =========================================*/

/// --- Queries ---

#define dal_c__str__ver_core_sep dal_c_ver_core_sep_default
#define dal_c__num__ver_core_major dal_c_ver_core_major_default
#if defined(dal_c__NUM__VER_CORE_MAJOR)
#undef dal_c__num__ver_core_major
#define dal_c__num__ver_core_major dal_c__NUM__VER_CORE_MAJOR
#endif /* defined(dal_c__VER_MAJOR) */
#define dal_c__num__ver_core_minor dal_c_ver_core_minor_default
#if defined(dal_c__NUM__VER_CORE_MINOR)
#undef dal_c__num__ver_core_minor
#define dal_c__num__ver_core_minor dal_c__NUM__VER_CORE_MINOR
#endif /* defined(dal_c__VER_MINOR) */
#define dal_c__num__ver_core_patch dal_c_ver_core_patch_default
#if defined(dal_c__NUM__VER_CORE_PATCH)
#undef dal_c__num__ver_core_patch
#define dal_c__num__ver_core_patch dal_c__NUM__VER_CORE_PATCH
#endif /* defined(dal_c__VER_PATCH) */

#define dal_c__val__ver_core_num \
    dal_c_ver_core_calc( \
        dal_c_ver_core_major, dal_c_ver_core_minor, dal_c_ver_core_patch \
    )
#define dal_c__str__ver_core_str \
    dal_c_ver_core_strfy( \
        dal_c_ver_core_major, dal_c_ver_core_minor, dal_c_ver_core_patch \
    )

#define dal_c__str__ver_label_delim dal_c_ver_label_delim_default
#define dal_c__num__ver_label_prefix_as_num dal_c_ver_label_prefix_as_num_default
#define dal_c__str__ver_label_prefix_as_str dal_c_ver_label_prefix_as_str_default
#if defined(dal_c__NUM__VER_LABEL_PREFIX) && defined(dal_c__STR__VER_LABEL_PREFIX)
#undef dal_c__str__ver_label_delim
#define dal_c__str__ver_label_delim dal_c_ver_label_delim_some
#undef dal_c__num__ver_label_prefix_as_num
#define dal_c__num__ver_label_prefix_as_num dal_c__NUM__VER_LABEL_PREFIX
#undef dal_c__str__ver_label_prefix_as_str
#define dal_c__str__ver_label_prefix_as_str dal_c__STR__VER_LABEL_PREFIX
#endif /* defined(dal_c__NUM__VER_LABEL_PREFIX) && defined(dal_c__STR__VER_LABEL_PREFIX) */
#define dal_c__str__ver_label_sep dal_c_ver_label_sep_default
#define dal_c__num__ver_label_suffix_as_num dal_c_ver_label_suffix_as_num_default
#define dal_c__str__ver_label_suffix_as_str dal_c_ver_label_suffix_as_str_default
#if defined(dal_c__NUM__VER_LABEL_SUFFIX) && defined(dal_c__STR__VER_LABEL_SUFFIX)
#undef dal_c__str__ver_label_sep
#define dal_c__str__ver_label_sep dal_c_ver_label_sep_some
#undef dal_c__num__ver_label_suffix_as_num
#define dal_c__num__ver_label_suffix_as_num dal_c__NUM__VER_LABEL_SUFFIX
#undef dal_c__str__ver_label_suffix_as_str
#define dal_c__str__ver_label_suffix_as_str dal_c__STR__VER_LABEL_SUFFIX
#endif /* defined(dal_c__NUM__VER_LABEL_SUFFIX) && defined(dal_c__STR__VER_LABEL_SUFFIX) */

#define dal_c__str__ver_build_delim dal_c_ver_build_delim_default
#define dal_c__str__ver_build_as_str dal_c_ver_build_as_str_default
#if defined(dal_c__STR__VER_BUILD)
#undef dal_c__str__ver_build_delim
#define dal_c__str__ver_build_delim dal_c_ver_build_delim_some
#undef dal_c__str__ver_build_as_str
#define dal_c__str__ver_build_as_str dal_c__STR__VER_BUILD
#endif /* defined(dal_c__VER_BUILD) */

#define dal_c__val__ver_num \
    dal_c_ver_calc( \
        dal_c_ver_core_major, dal_c_ver_core_minor, dal_c_ver_core_patch, \
        dal_c_ver_label_prefix_as_num, dal_c_ver_label_suffix_as_num \
    )
#define dal_c__str__ver_str \
    dal_c_ver_strfy( \
        dal_c_ver_core_major, dal_c_ver_core_minor, dal_c_ver_core_patch, \
        dal_c_ver_label_prefix_as_str, dal_c_ver_label_suffix_as_str \
    )
#define dal_c__str__ver_str_with_build \
    dal_c_ver_strfyWithBuild( \
        dal_c_ver_core_major, dal_c_ver_core_minor, dal_c_ver_core_patch, \
        dal_c_ver_label_prefix_as_str, dal_c_ver_label_suffix_as_str, dal_c_ver_build_as_str \
    )

/// --- Defaults ---

#define dal_c__val__ver_core_calc(_major, _minor, _patch...) ( \
    (((_major) & 0xFFu) << 24u) \
    | (((_minor) & 0xFFu) << 16u) \
    | (((_patch) & 0xFFu) << 8u) \
)
#define dal_c__str__ver_core_strfy(_major, _minor, _patch...) pp_expand( \
    pp_strfy(_major) dal_c_ver_core_sep pp_strfy(_minor) dal_c_ver_core_sep pp_strfy(_patch) \
)
#define dal_c__val__ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num...) ( \
    dal_c__val__ver_core_calc(_major, _minor, _patch) \
    | (((_label_prefix_as_num) & 0x03u) << 6u) \
    | (((_label_suffix_as_num) & 0x3Fu) << 0u) \
)
#define dal_c__str__ver_strfy(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str...) pp_expand( \
    dal_c_ver_core_strfy(_major, _minor, _patch) \
        dal_c_ver_label_delim _label_prefix_as_str dal_c_ver_label_sep _label_suffix_as_str \
)
#define dal_c__str__ver_strfyWithBuild(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str, _build_as_str...) pp_expand( \
    dal_c_ver_strfy(_major, _minor, _patch, _label_prefix_as_str, _label_suffix_as_str) \
        dal_c_ver_build_delim _build_as_str \
)

#endif /* dal_c__included */
