#ifndef dal_c__included
#define dal_c__included 1

#include "dal-c-ext/base.h"
#include "dal-c-ext/str.h"
#include <assert.h>

/// === PREPROCESSOR UTILITIES ===

#define dal_c__comp_expand(...) __VA_ARGS__
#define dal_c_expand(...) dal_c__comp_expand(__VA_ARGS__)
#define dal_c__comp_strfy(x...) #x
#define dal_c_strfy(x...) dal_c__comp_strfy(x)

/// === TOOL METADATA ===

#define dal_c_tool_name "dh-c"
#define dal_c_tool_description "Build tool for dasae-headers projects"
#define dal_c_tool_copyright "Copyright (c) 2024-2026 Gyeongtae Kim"

/// === VERSION ===

#define dal_c_ver_major 0
#define dal_c_ver_minor 2
#define dal_c_ver_patch 3
#define dal_c_ver_sep "."
#define dal_c_ver_label ""

#define dal_c_ver_str dal_c_expand( \
    dal_c_strfy(dal_c_ver_major) dal_c_ver_sep \
        dal_c_strfy(dal_c_ver_minor) dal_c_ver_sep \
            dal_c_strfy(dal_c_ver_patch) dal_c_ver_label \
)

/// === BOOLEAN ===

#define dal_c_boolean_true "true"
#define dal_c_boolean_false "false"
static inline bool dal_c_boolean_parse(const char* str) {
    if (str_eql(str, dal_c_boolean_true) || str_eql(str, "1")) { return true; }
    if (str_eql(str, dal_c_boolean_false) || str_eql(str, "0")) { return false; }
    return false;
}
static inline const char* dal_c_boolean_format(bool value) {
    return value ? dal_c_boolean_true : dal_c_boolean_false;
}

/// === TARGET ===

typedef enum dal_c_Target {
    dal_c_Target_invalid = -1,
    dal_c_Target_executable = 0, // .exe output
    dal_c_Target_static_lib = 1, // .a (Unix) or .lib (Windows) output
    dal_c_Target_shared_lib = 2, // .so (Unix) or .dll (Windows) output
} dal_c_Target;

/// === LINKING ===

typedef enum dal_c_Linking {
    dal_c_Linking_invalid = -1,
    dal_c_Linking_static = 0, // -static (default)
    dal_c_Linking_dynamic = 1, // -shared
} dal_c_Linking;
#define dal_c_linking_static "static"
#define dal_c_linking_dynamic "dynamic"
static inline dal_c_Linking dal_c_Linking_parse(const char* str) {
    if (str_eql(str, dal_c_linking_static)) { return dal_c_Linking_static; }
    if (str_eql(str, dal_c_linking_dynamic)) { return dal_c_Linking_dynamic; }
    return dal_c_Linking_invalid;
}
static inline const char* dal_c_Linking_format(dal_c_Linking linking) {
    switch (linking) {
    case dal_c_Linking_static:  return dal_c_linking_static;
    case dal_c_Linking_dynamic: return dal_c_linking_dynamic;
    case dal_c_Linking_invalid:
    default:                    return NULL;
    }
}
static inline dal_c_Linking dal_c_Linking_fromFlag(const char* flag) {
    if (str_eql(flag, "-static")) { return dal_c_Linking_static; }
    if (str_eql(flag, "-shared")) { return dal_c_Linking_dynamic; }
    return dal_c_Linking_invalid;
}
static inline const char* dal_c_Linking_toFlag(dal_c_Linking linking) {
    switch (linking) {
    case dal_c_Linking_static:  return "-static";
    case dal_c_Linking_dynamic: return "-shared";
    case dal_c_Linking_invalid:
    default:                    return NULL;
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
    case dal_c_OptiLevel_none:       return dal_c_opti_level_none;
    case dal_c_OptiLevel_debug:      return dal_c_opti_level_debug;
    case dal_c_OptiLevel_basic:      return dal_c_opti_level_basic;
    case dal_c_OptiLevel_balanced:   return dal_c_opti_level_balanced;
    case dal_c_OptiLevel_aggressive: return dal_c_opti_level_aggressive;
    case dal_c_OptiLevel_compact:    return dal_c_opti_level_compact;
    case dal_c_OptiLevel_minimal:    return dal_c_opti_level_minimal;
    case dal_c_OptiLevel_invalid:
    default:                         return NULL;
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
    case dal_c_OptiLevel_none:       return "-O0";
    case dal_c_OptiLevel_debug:      return "-Og";
    case dal_c_OptiLevel_basic:      return "-O1";
    case dal_c_OptiLevel_balanced:   return "-O2";
    case dal_c_OptiLevel_aggressive: return "-O3";
    case dal_c_OptiLevel_compact:    return "-Os";
    case dal_c_OptiLevel_minimal:    return "-Oz";
    case dal_c_OptiLevel_invalid:
    default:                         return NULL;
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
    case dal_c_DebugLevel_none:     return dal_c_debug_level_none;
    case dal_c_DebugLevel_minimal:  return dal_c_debug_level_minimal;
    case dal_c_DebugLevel_standard: return dal_c_debug_level_standard;
    case dal_c_DebugLevel_extended: return dal_c_debug_level_extended;
    case dal_c_DebugLevel_invalid:
    default:                        return NULL;
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
    case dal_c_DebugLevel_none:     return "";
    case dal_c_DebugLevel_minimal:  return "-g1";
    case dal_c_DebugLevel_standard: return "-g";
    case dal_c_DebugLevel_extended: return "-g3";
    case dal_c_DebugLevel_invalid:
    default:                        return NULL;
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
    dal_c_Profile_micro = 7
} dal_c_Profile;
#define dal_c_profile_dev "dev"
#define dal_c_profile_test "test"
#define dal_c_profile_profile "profile"
#define dal_c_profile_stable "stable"
#define dal_c_profile_release "release"
#define dal_c_profile_optimize "optimize"
#define dal_c_profile_compact "compact"
#define dal_c_profile_micro "micro"
static inline dal_c_Profile dal_c_Profile_parse(const char* str) {
    if (str_eql(str, dal_c_profile_dev)) { return dal_c_Profile_dev; }
    if (str_eql(str, dal_c_profile_test)) { return dal_c_Profile_test; }
    if (str_eql(str, dal_c_profile_profile)) { return dal_c_Profile_profile; }
    if (str_eql(str, dal_c_profile_stable)) { return dal_c_Profile_stable; }
    if (str_eql(str, dal_c_profile_release)) { return dal_c_Profile_release; }
    if (str_eql(str, dal_c_profile_optimize)) { return dal_c_Profile_optimize; }
    if (str_eql(str, dal_c_profile_compact)) { return dal_c_Profile_compact; }
    if (str_eql(str, dal_c_profile_micro)) { return dal_c_Profile_micro; }
    return dal_c_Profile_invalid;
}
static inline const char* dal_c_Profile_format(dal_c_Profile profile) {
    switch (profile) {
    case dal_c_Profile_dev:      return dal_c_profile_dev;
    case dal_c_Profile_test:     return dal_c_profile_test;
    case dal_c_Profile_profile:  return dal_c_profile_profile;
    case dal_c_Profile_stable:   return dal_c_profile_stable;
    case dal_c_Profile_release:  return dal_c_profile_release;
    case dal_c_Profile_optimize: return dal_c_profile_optimize;
    case dal_c_Profile_compact:  return dal_c_profile_compact;
    case dal_c_Profile_micro:    return dal_c_profile_micro;
    case dal_c_Profile_invalid:
    default:                     return NULL;
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
    case dal_c_SampleDir_samples:  return dal_c_sample_dir_samples;
    case dal_c_SampleDir_examples: return dal_c_sample_dir_examples;
    case dal_c_SampleDir_tests:    return dal_c_sample_dir_tests;
    case dal_c_SampleDir_none:
    default:                       return NULL;
    }
}

/// === PROFILE SPECIFICATION ===

typedef struct dal_c_ProfileSpec {
    const char* name;
    dal_c_OptiLevel opti_level;
    dal_c_DebugLevel debug_level;
    bool debug_assertions;
    const char* extra_flags[8];
} dal_c_ProfileSpec;
static const dal_c_ProfileSpec dal_c_profile_specs[] = {
    [dal_c_Profile_dev] = {
        .name = dal_c_profile_dev,
        .opti_level = dal_c_OptiLevel_debug,
        .debug_level = dal_c_DebugLevel_extended,
        .debug_assertions = true,
        .extra_flags = { "-fno-omit-frame-pointer", NULL },
    },
    [dal_c_Profile_test] = {
        .name = dal_c_profile_test,
        .opti_level = dal_c_OptiLevel_basic,
        .debug_level = dal_c_DebugLevel_standard,
        .debug_assertions = true,
        .extra_flags = { "-fno-omit-frame-pointer", NULL },
    },
    [dal_c_Profile_profile] = {
        .name = dal_c_profile_profile,
        .opti_level = dal_c_OptiLevel_balanced,
        .debug_level = dal_c_DebugLevel_standard,
        .debug_assertions = true,
        .extra_flags = { "-fno-omit-frame-pointer", NULL },
    },
    [dal_c_Profile_stable] = {
        .name = dal_c_profile_stable,
        .opti_level = dal_c_OptiLevel_balanced,
        .debug_level = dal_c_DebugLevel_minimal,
        .debug_assertions = false,
        .extra_flags = { NULL },
    },
    [dal_c_Profile_release] = {
        .name = dal_c_profile_release,
        .opti_level = dal_c_OptiLevel_aggressive,
        .debug_level = dal_c_DebugLevel_minimal,
        .debug_assertions = false,
        .extra_flags = { "-flto", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections", NULL },
    },
    [dal_c_Profile_optimize] = {
        .name = dal_c_profile_optimize,
        .opti_level = dal_c_OptiLevel_aggressive,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .extra_flags = { "-march=native", "-flto", "-fdata-sections", "-ffunction-sections", NULL },
    },
    [dal_c_Profile_compact] = {
        .name = dal_c_profile_compact,
        .opti_level = dal_c_OptiLevel_compact,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .extra_flags = { "-flto", "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections", NULL },
    },
    [dal_c_Profile_micro] = {
        .name = dal_c_profile_micro,
        .opti_level = dal_c_OptiLevel_minimal,
        .debug_level = dal_c_DebugLevel_none,
        .debug_assertions = false,
        .extra_flags = { "-fno-unroll-loops", "-flto", NULL },
    }
};
#define dal_c_profile_specs_count ((int)(sizeof(dal_c_profile_specs) / sizeof(dal_c_profile_specs[0])))
static inline const dal_c_ProfileSpec* dal_c_ProfileSpec_by(dal_c_Profile profile) {
    if (0 <= profile && profile < dal_c_profile_specs_count) { return &dal_c_profile_specs[profile]; }
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
static inline dal_c_CmdAction dal_c_CmdAction_parse(const char* str) {
    if (str_eql(str, dal_c_cmd_action_build)) { return dal_c_CmdAction_build; }
    if (str_eql(str, dal_c_cmd_action_lib)) { return dal_c_CmdAction_lib; }
    if (str_eql(str, dal_c_cmd_action_run)) { return dal_c_CmdAction_run; }
    if (str_eql(str, dal_c_cmd_action_test)) { return dal_c_CmdAction_test; }
    if (str_eql(str, dal_c_cmd_action_deps)) { return dal_c_CmdAction_deps; }
    if (str_eql(str, dal_c_cmd_action_clean)) { return dal_c_CmdAction_clean; }
    if (str_eql(str, dal_c_cmd_action_workspace)) { return dal_c_CmdAction_workspace; }
    if (str_eql(str, dal_c_cmd_action_project)) { return dal_c_CmdAction_project; }
    if (str_eql(str, dal_c_cmd_action_help)) { return dal_c_CmdAction_help; }
    if (str_eql(str, dal_c_cmd_action_version)) { return dal_c_CmdAction_version; }
    return dal_c_CmdAction_invalid;
}
static inline const char* dal_c_CmdAction_format(dal_c_CmdAction action) {
    switch (action) {
    case dal_c_CmdAction_build:     return dal_c_cmd_action_build;
    case dal_c_CmdAction_lib:       return dal_c_cmd_action_lib;
    case dal_c_CmdAction_run:       return dal_c_cmd_action_run;
    case dal_c_CmdAction_test:      return dal_c_cmd_action_test;
    case dal_c_CmdAction_deps:      return dal_c_cmd_action_deps;
    case dal_c_CmdAction_clean:     return dal_c_cmd_action_clean;
    case dal_c_CmdAction_workspace: return dal_c_cmd_action_workspace;
    case dal_c_CmdAction_project:   return dal_c_cmd_action_project;
    case dal_c_CmdAction_help:      return dal_c_cmd_action_help;
    case dal_c_CmdAction_version:   return dal_c_cmd_action_version;
    case dal_c_CmdAction_invalid:
    default:                        return NULL;
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
#define dal_c_opt_sysroot "sysroot"
#define dal_c_opt_include "include"
#define dal_c_opt_isystem "isystem"
#define dal_c_opt_link "link"
#define dal_c_opt_define "define"
#define dal_c_opt_undef "undef"
#define dal_c_opt_lib "lib"
#define dal_c_opt_dh "dh"
#define dal_c_opt_args "args"
#define dal_c_opt_compiler_args "compiler-args"
#define dal_c_opt_runtime_args "runtime-args"

#define dal_c_opt_verbose "verbose"
#define dal_c_opt_debug "debug"
#define dal_c_opt_show_commands "show-commands"
#define dal_c_opt_no_libdh "no-libdh"
#define dal_c_opt_freestanding "freestanding"
#define dal_c_opt_loose_errors "loose-errors"
#define dal_c_opt_static "static"
#define dal_c_opt_dynamic "dynamic"
#define dal_c_opt_cache "cache"
#define dal_c_opt_recur "recur"
#define dal_c_opt_all "all"
#define dal_c_opt_sample "sample"
#define dal_c_opt_example "example"
#define dal_c_opt_test "test"
#define dal_c_opt_help "help"
#define dal_c_opt_version "version"

// Short option names (string form for concatenation)
#define dal_c_opt_include_short "I"
#define dal_c_opt_link_short "l"
#define dal_c_opt_define_short "D"
#define dal_c_opt_undef_short "U"
#define dal_c_opt_help_short "h"
#define dal_c_opt_version_short "v"

// Short option characters (for parsing)
#define dal_c_opt_include_short_char 'I'
#define dal_c_opt_link_short_char 'l'
#define dal_c_opt_define_short_char 'D'
#define dal_c_opt_undef_short_char 'U'
#define dal_c_opt_help_short_char 'h'
#define dal_c_opt_version_short_char 'v'

/// === COMPILER OPTIONS (Shared) ===

typedef struct dal_c_CompilerOpts {
    const char* compiler; // --compiler=<name> (NULL = auto-detect)
    const char* c_std; // --std=<std> (NULL = gnu17)
    const char* arch_target; // --arch or --target
    const char* sysroot; // --sysroot=<path>
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
    dal_c_Profile profile;
    bool freestanding; // --freestanding
    bool loose_errors; // --loose-errors
    bool no_libdh; // --no-libdh
} dal_c_CompilerOpts;

/// === ACTION-SPECIFIC PAYLOADS ===

typedef struct dal_c_BuildOpts {
    char* target_file;
    dal_c_SampleDir sample_dir; // --sample, --example, or --test
    bool build_all; // --all or "." to build all files
} dal_c_BuildOpts;

typedef struct dal_c_LibOpts {
    char* target_file;
    dal_c_Linking linking; // --static or --dynamic (default for libraries)
    const char* explicit_lib; // --lib=<name> (not owned)
} dal_c_LibOpts;

typedef struct dal_c_RunOpts {
    char* target_file;
    char* run_args; // --runtime-args="..."
    dal_c_SampleDir sample_dir; // --sample, --example, or --test
    bool debug; // --debug
} dal_c_RunOpts;

typedef struct dal_c_TestOpts {
    char* target_file;
    char* run_args; // --runtime-args="..."
    bool debug;
    bool build_all; // --all or "." to build all files
} dal_c_TestOpts;

typedef struct dal_c_CleanOpts {
    bool cache_only; // --cache
    bool recursive; // --recur
} dal_c_CleanOpts;

typedef struct dal_c_ScaffoldOpts {
    const char* name;
} dal_c_ScaffoldOpts;

/// === COMMAND PAYLOAD (TaggedUnion) ===

typedef union dal_c_CmdPayload {
    dal_c_BuildOpts build;
    dal_c_LibOpts lib;
    dal_c_RunOpts run;
    dal_c_TestOpts test;
    dal_c_CleanOpts clean;
    dal_c_ScaffoldOpts workspace;
    dal_c_ScaffoldOpts project;
} dal_c_CmdPayload;

/// === COMMAND (User Intent) ===

typedef struct dal_c_Cmd {
    dal_c_CmdAction action;
    dal_c_CmdPayload payload;
    dal_c_CompilerOpts opts;

    const char* compiler_args; // --args="..."
    const char* dh_path_override; // --dh=<path>
    bool show_commands; // --show-commands
    bool verbose; // --verbose
    bool is_help; // --help, -h
    bool is_version; // --version, -v
} dal_c_Cmd;

dal_c_Cmd* dal_c_Cmd_parse(int argc, const char* argv[]);
void dal_c_Cmd_cleanup(dal_c_Cmd** self);

typedef struct dal_c_Project dal_c_Project;

int dal_c_Cmd_execute(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_makeTarget(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_cleanTarget(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_compileDeps(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_createWorkspace(const dal_c_Cmd* self, const dal_c_Project* proj);
int dal_c_Cmd_createProject(const dal_c_Cmd* self, const dal_c_Project* proj);

/// === LIBRARY (Dependency) ===

typedef struct dal_c_Lib {
    const char* name;
    const char* path;
    dal_c_CompilerOpts opts;
    bool is_static;
} dal_c_Lib;

/// === PROJECT (Detected State) ===

struct dal_c_Project {
    const char* root;
    const char* name;
    const char* dh_path;
    const char* pch_header;
    dal_c_Lib* libraries;
    int lib_count;
};

dal_c_Project* dal_c_Project_detect(const dal_c_Cmd* cmd);
dal_c_Project* dal_c_Project_detectAt(const char* lib_path, const char* dh_path);
void dal_c_Project_cleanup(dal_c_Project** self);
char* dal_c_Project_getBuildDir(const dal_c_Project* proj);
char* dal_c_Project_getLibDir(const dal_c_Project* proj);
char* dal_c_Project_getSrcDir(const dal_c_Project* proj);
char* dal_c_Project_getIncludeDir(const dal_c_Project* proj);
char* dal_c_Project_getDepsDir(const dal_c_Project* proj);

/// === DEFAULTS ===

#define dal_c_default_compiler "clang"
#define dal_c_default_c_std "gnu17"
#define dal_c_default_profile dal_c_Profile_dev

/// === PCH HEADER DETECTION NAMES ===

#define dal_c_pch_header_dasae_headers "dasae-headers.h"
#define dal_c_pch_header_dh "dh.h"
#define dal_c_pch_header_dal "dal.h"
#define dal_c_pch_header_da "da.h"

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

/// === FILE NAMES ===

#define dal_c_file_lib_dh "lib.dh"
#define dal_c_file_makefile "Makefile"

/// === EXTERNAL TOOLS ===

#define dal_c_tool_make "make"
#define dal_c_tool_ar "llvm-ar"
#define dal_c_tool_debugger "lldb"

/// === HELP SYSTEM ===

#define dal_c_help_opt_width 28

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
} dal_c_HelpCmd;

// Common build options (shared by build, lib, run, test, deps)
static const dal_c_HelpOption dal_c_help_build_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_compiler dal_c_opt_value_sep "<name>", "Compiler (default: " dal_c_default_compiler ")" },
    { dal_c_opt_prefix_long dal_c_opt_std dal_c_opt_value_sep "<std>", "C standard (default: " dal_c_default_c_std ")" },
    { dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "<target>", "Target architecture" },
    { dal_c_opt_prefix_long dal_c_opt_target dal_c_opt_value_sep "<triple>", "Target triple (alternative to " dal_c_opt_prefix_long dal_c_opt_arch ")" },
    { dal_c_opt_prefix_long dal_c_opt_freestanding, "Freestanding environment" },
    { dal_c_opt_prefix_long dal_c_opt_sysroot dal_c_opt_value_sep "<path>", "System root directory" },
    { dal_c_opt_prefix_long dal_c_opt_include dal_c_opt_value_sep "<path>", "Add include path (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_include_short "<path>", "Add include path (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_isystem dal_c_opt_value_sep "<path>", "Add system include path (can be repeated)" },
    { dal_c_opt_prefix_long dal_c_opt_link dal_c_opt_value_sep "<lib>", "Link library (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_link_short "<lib>", "Link library (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "<macro>", "Define macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_define_short "<macro>", "Define macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_undef dal_c_opt_value_sep "<macro>", "Undefine macro (can be repeated)" },
    { dal_c_opt_prefix_short dal_c_opt_undef_short "<macro>", "Undefine macro (alternative)" },
    { dal_c_opt_prefix_long dal_c_opt_compiler_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Additional compiler flags (context-aware)" },
    { dal_c_opt_prefix_long dal_c_opt_loose_errors, "Convert Werror to warnings" },
    { dal_c_opt_prefix_long dal_c_opt_sample, "Build sample code (includes project as library)" },
    { dal_c_opt_prefix_long dal_c_opt_example, "Build example code (includes project as library)" },
    { dal_c_opt_prefix_long dal_c_opt_test, "Build test code (includes project as library)" },
    { dal_c_opt_prefix_long dal_c_opt_all, "Build all source files in " dal_c_dir_src "/" },
    { dal_c_opt_all_alias, "Build all source files (alternative to " dal_c_opt_prefix_long dal_c_opt_all ")" },
    { dal_c_opt_prefix_long dal_c_opt_no_libdh, "Skip DH library" },
    { dal_c_opt_prefix_long dal_c_opt_show_commands, "Print commands" },
    { dal_c_opt_prefix_long dal_c_opt_verbose, "Verbose output" },
    { dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep "<path>", "Override DH path" },
};
#define dal_c_help_build_options_count ((int)(sizeof(dal_c_help_build_options) / sizeof(dal_c_help_build_options[0])))

static const char* const dal_c_help_build_examples[] = {
    dal_c_cmd_action_build " " dal_c_profile_dev,
    dal_c_cmd_action_build " " dal_c_profile_release " src/main.c",
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_arch dal_c_opt_value_sep "x86_64 " dal_c_opt_prefix_long dal_c_opt_define dal_c_opt_value_sep "DEBUG",
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_sample,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_example,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_test,
    dal_c_cmd_action_build " " dal_c_opt_all_alias,
    dal_c_cmd_action_build " " dal_c_opt_prefix_long dal_c_opt_all,
};
#define dal_c_help_build_examples_count ((int)(sizeof(dal_c_help_build_examples) / sizeof(dal_c_help_build_examples[0])))

// lib command options
static const dal_c_HelpOption dal_c_help_lib_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_static, "Build static library (default)" },
    { dal_c_opt_prefix_long dal_c_opt_dynamic, "Build shared library" },
};
#define dal_c_help_lib_options_count ((int)(sizeof(dal_c_help_lib_options) / sizeof(dal_c_help_lib_options[0])))

static const char* const dal_c_help_lib_examples[] = {
    dal_c_cmd_action_lib " " dal_c_opt_prefix_long dal_c_opt_static,
    dal_c_cmd_action_lib " " dal_c_profile_release " src/mylib.c " dal_c_opt_prefix_long dal_c_opt_dynamic,
    dal_c_cmd_action_lib " myheader.h",
};
#define dal_c_help_lib_examples_count ((int)(sizeof(dal_c_help_lib_examples) / sizeof(dal_c_help_lib_examples[0])))

// run command options
static const dal_c_HelpOption dal_c_help_run_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_debug, "Launch debugger" },
    { dal_c_opt_prefix_long dal_c_opt_runtime_args dal_c_opt_value_sep "\"...\"", "Runtime arguments" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Runtime arguments (context-aware)" },
};
#define dal_c_help_run_options_count ((int)(sizeof(dal_c_help_run_options) / sizeof(dal_c_help_run_options[0])))

static const char* const dal_c_help_run_examples[] = {
    dal_c_cmd_action_run " " dal_c_profile_dev,
    dal_c_cmd_action_run " " dal_c_opt_prefix_long dal_c_opt_debug " " dal_c_opt_prefix_long dal_c_opt_runtime_args dal_c_opt_value_sep "\"arg1 arg2\"",
};
#define dal_c_help_run_examples_count ((int)(sizeof(dal_c_help_run_examples) / sizeof(dal_c_help_run_examples[0])))

// test command options
static const dal_c_HelpOption dal_c_help_test_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_debug, "Launch debugger" },
    { dal_c_opt_prefix_long dal_c_opt_runtime_args dal_c_opt_value_sep "\"...\"", "Test arguments" },
    { dal_c_opt_prefix_long dal_c_opt_args dal_c_opt_value_sep "\"...\"", "Test arguments (context-aware)" },
};
#define dal_c_help_test_options_count ((int)(sizeof(dal_c_help_test_options) / sizeof(dal_c_help_test_options[0])))

static const char* const dal_c_help_test_examples[] = {
    dal_c_cmd_action_test " " dal_c_profile_dev,
    dal_c_cmd_action_test " " dal_c_opt_prefix_long dal_c_opt_debug " sample.c",
};
#define dal_c_help_test_examples_count ((int)(sizeof(dal_c_help_test_examples) / sizeof(dal_c_help_test_examples[0])))

// deps command options
static const dal_c_HelpOption dal_c_help_deps_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_verbose, "Verbose output" },
};
#define dal_c_help_deps_options_count ((int)(sizeof(dal_c_help_deps_options) / sizeof(dal_c_help_deps_options[0])))

static const char* const dal_c_help_deps_examples[] = {
    dal_c_cmd_action_deps,
    dal_c_cmd_action_deps " " dal_c_opt_prefix_long dal_c_opt_verbose,
};
#define dal_c_help_deps_examples_count ((int)(sizeof(dal_c_help_deps_examples) / sizeof(dal_c_help_deps_examples[0])))

// clean command options
static const dal_c_HelpOption dal_c_help_clean_options[] = {
    { dal_c_opt_prefix_long dal_c_opt_cache, "Clean only cache" },
    { dal_c_opt_prefix_long dal_c_opt_recur, "Recursive clean" },
};
#define dal_c_help_clean_options_count ((int)(sizeof(dal_c_help_clean_options) / sizeof(dal_c_help_clean_options[0])))

static const char* const dal_c_help_clean_examples[] = {
    dal_c_cmd_action_clean,
    dal_c_cmd_action_clean " " dal_c_opt_prefix_long dal_c_opt_cache " " dal_c_opt_prefix_long dal_c_opt_recur,
};
#define dal_c_help_clean_examples_count ((int)(sizeof(dal_c_help_clean_examples) / sizeof(dal_c_help_clean_examples[0])))

// Global options
static const dal_c_HelpOption dal_c_help_global_options[] = {
    { dal_c_opt_prefix_short dal_c_opt_help_short ", " dal_c_opt_prefix_long dal_c_opt_help, "Show this help message" },
    { dal_c_opt_prefix_short dal_c_opt_version_short ", " dal_c_opt_prefix_long dal_c_opt_version, "Show version information" },
};
#define dal_c_help_global_options_count ((int)(sizeof(dal_c_help_global_options) / sizeof(dal_c_help_global_options[0])))

// Profile descriptions
typedef struct dal_c_HelpProfile {
    const char* name;
    const char* description;
} dal_c_HelpProfile;

static const dal_c_HelpProfile dal_c_help_profiles[] = {
    [dal_c_Profile_dev] = { dal_c_profile_dev, "Debug build with assertions (-g3 -Og)" },
    [dal_c_Profile_test] = { dal_c_profile_test, "Test build with basic optimization (-g -O1)" },
    [dal_c_Profile_profile] = { dal_c_profile_profile, "Profile build with optimization (-g -O2)" },
    [dal_c_Profile_stable] = { dal_c_profile_stable, "Stable build without debug (-g1 -O2)" },
    [dal_c_Profile_release] = { dal_c_profile_release, "Release build with LTO (-g1 -O3)" },
    [dal_c_Profile_optimize] = { dal_c_profile_optimize, "Maximum optimization (-O3 -march=native)" },
    [dal_c_Profile_compact] = { dal_c_profile_compact, "Size-optimized build (-Os)" },
    [dal_c_Profile_micro] = { dal_c_profile_micro, "Extreme size optimization (-Oz)" },
};
#define dal_c_help_profiles_count ((int)(sizeof(dal_c_help_profiles) / sizeof(dal_c_help_profiles[0])))

// Command table
static const dal_c_HelpCmd dal_c_help_cmds[] = {
    { dal_c_cmd_action_build,
      "Build project or single file as executable",
      "[profile] [file.c] [options]",
      dal_c_help_build_options, dal_c_help_build_options_count,
      dal_c_help_build_examples, dal_c_help_build_examples_count },
    { dal_c_cmd_action_lib,
      "Build project or single file as library (supports header-only)",
      "[profile] [file.c|file.h] [options]",
      dal_c_help_lib_options, dal_c_help_lib_options_count,
      dal_c_help_lib_examples, dal_c_help_lib_examples_count },
    { dal_c_cmd_action_run,
      "Build and run executable",
      "[profile] [file.c] [options]",
      dal_c_help_run_options, dal_c_help_run_options_count,
      dal_c_help_run_examples, dal_c_help_run_examples_count },
    { dal_c_cmd_action_test,
      "Build and run tests",
      "[profile] [file.c] [options]",
      dal_c_help_test_options, dal_c_help_test_options_count,
      dal_c_help_test_examples, dal_c_help_test_examples_count },
    { dal_c_cmd_action_deps,
      "Build dependencies from " dal_c_file_lib_dh,
      "[profile] [options]",
      dal_c_help_deps_options, dal_c_help_deps_options_count,
      dal_c_help_deps_examples, dal_c_help_deps_examples_count },
    { dal_c_cmd_action_clean,
      "Clean build artifacts",
      "[options]",
      dal_c_help_clean_options, dal_c_help_clean_options_count,
      dal_c_help_clean_examples, dal_c_help_clean_examples_count },
    { dal_c_cmd_action_workspace,
      "Create workspace config",
      "[path]",
      NULL, 0,
      NULL, 0 },
    { dal_c_cmd_action_project,
      "Create new project",
      "[name]",
      NULL, 0,
      NULL, 0 },
};
#define dal_c_help_cmds_count ((int)(sizeof(dal_c_help_cmds) / sizeof(dal_c_help_cmds[0])))

#endif /* dal_c__included */
