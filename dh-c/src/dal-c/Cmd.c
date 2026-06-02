#include "internal.h"
#include "dal-c-ext/ArrStr.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/file.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
/* Supported: single file build/run/test; --sample/--example/--test with bare filename
 * (e.g. dh-c build --sample target.c resolves to samples/target.c); aggregate test builds
 * use a generated runner TU plus separate test translation units. */

// === PRIVATE HELPERS ===

static void dal_c_Cmd__addToArray(char*** arr, int* count, const char* value);
static void dal_c_Cmd__pushOwnedString(char*** arr, int* count, char* value);
static void dal_c_Cmd__setOwnedString(char** slot, const char* value);
static bool dal_c_Cmd__isSourceOrHeader(const char* name);
static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action);
static bool dal_c_Cmd__tryParseBoolValue(const char* value, bool* out);
static bool dal_c_Cmd__tryParseToggleValue(const char* value, dal_c_ToggleState* out);
static int dal_c_Cmd__applyAssignedBooleanOption(dal_c_Cmd* cmd, const char* opt, size_t opt_len, const char* value, bool* handled);
static dal_c_Target dal_c_Cmd__resolveBuildTargetType(const dal_c_Cmd* cmd, dal_c_Linking linking, bool builds_library);
static void dal_c_Cmd__setStdlibBundle(dal_c_CompilerOpts* opts, bool linked);
static void dal_c_Cmd__setCrtStartFiles(dal_c_CompilerOpts* opts, bool linked);
static int dal_c_Cmd__parseOptions(dal_c_Cmd* cmd, int argc, const char* argv[], int start_idx);
static char** dal_c_Cmd__targetPathSlot(dal_c_Cmd* cmd);
static char** dal_c_Cmd__outputPathSlot(dal_c_Cmd* cmd);
static char** dal_c_Cmd__runArgsSlot(dal_c_Cmd* cmd);
static bool dal_c_Cmd__buildsLibrary(const dal_c_Cmd* cmd);
static int dal_c_Cmd__validateCanonicalModifiers(const dal_c_Cmd* cmd);
static void dal_c_Cmd__setPrimaryTargetPath(dal_c_Cmd* cmd, const char* path);
static void dal_c_Cmd__setOutputPath(dal_c_Cmd* cmd, const char* path);
static const char* dal_c_Cmd__sampleDirCanonical(dal_c_SampleDir sample_dir);
static ArrStr* dal_c_Cmd__resolveInputs(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* dir_name, bool fallback_all);
static bool dal_c_Cmd__inputsNeedCategoryResolution(const dal_c_Cmd* cmd);
static ArrStr* dal_c_Cmd__collectPathSources(const char* path, bool resolved_is_dir, bool skip_auto_paths);
static ArrStr* dal_c_Cmd__collectExplicitSources(const dal_c_Cmd* cmd);
static ArrStr* dal_c_Cmd__collectTargetSources(const dal_c_TargetRequest* request);
static int dal_c_Cmd__pushExcludePath(dal_c_Cmd* cmd, const char* value);
static bool dal_c_Cmd__pathMatchesExclude(const char* path, const char* exclude_path);
static const char* dal_c_Cmd__findMatchingExclude(const char* path, const ArrStr* excludes);
static int dal_c_Cmd__rejectExcludedPath(const char* path, const ArrStr* excludes);
static ArrStr* dal_c_Cmd__collectActiveExcludes(const dal_c_Project* proj, const dal_c_TargetRequest* request, const dal_c_Cmd* cmd);
static int dal_c_Cmd__applyExcludeContract(ArrStr** sources, const ArrStr* excludes, bool explicit_selection);
static char* dal_c_Cmd__basenameNoExt(const char* path);
static void dal_c_Cmd__collectCompanionDhFiles(ArrStr* dh_files, ArrStr* sources);
static void dal_c_Cmd__mergeBuildProperties(dal_c_CompilerOpts* opts, dal_c_BuildDefaults* defaults, const dal_c_Project* proj, ArrStr* sources, const dal_c_Cmd* cmd);
static bool dal_c_Cmd__hasExplicitVersionFlags(const dal_c_Cmd* cmd);
static int dal_c_Cmd__recordVersionFlags(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);
static char* dal_c_Cmd__versionRecordPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);
static int dal_c_Cmd__writeVersionDhFile(const char* path, const dal_c_VersionSpec* version);
static int dal_c_Cmd__ensureProjectStaticLibrary(const dal_c_Cmd* self, const dal_c_Project* proj);
static int dal_c_Cmd__runBuildDefaultTests(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Profile profile);
static int dal_c_Cmd__buildFromSources(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* output_name,
    dal_c_Target target_type,
    const char* extra_compiler_args,
    bool allow_output_defaults,
    bool print_success
);
static int dal_c_Cmd__buildLibrarySetFromSources(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* output_name,
    bool allow_output_defaults
);
static char* dal_c_Cmd__mergeCompilerArgs(const char* base, const char* extra);
static bool dal_c_Cmd__writeFileIfChanged(const char* path, const char* content);
static char* dal_c_Cmd__generatedSourceDir(const dal_c_Project* proj, const dal_c_Cmd* cmd);
static char* dal_c_Cmd__writeTestMainSource(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* output_name);
static char* dal_c_Cmd__writeUnitySource(const dal_c_Project* proj, const dal_c_Cmd* cmd, ArrStr* inputs, const char* output_name);
static bool dal_c_Cmd__arrHasString(const ArrStr* arr, const char* value);
static void dal_c_Cmd__appendUniqueString(ArrStr* arr, const char* value);
static void dal_c_Cmd__appendProjectTestIncludeDirs(const dal_c_Project* proj, ArrStr* include_dirs);
static void dal_c_Cmd__collectRecursiveTestInputs(
    const dal_c_Project* proj,
    const dal_c_Cmd* cmd,
    bool recurse_dependencies,
    ArrStr* visited_roots,
    ArrStr* tests,
    ArrStr* include_dirs
);
static char* dal_c_Cmd__buildIncludeArgs(const ArrStr* include_dirs);
static int dal_c_Cmd__runRecursiveBuild(const dal_c_Cmd* self, const dal_c_Project* proj);
static int dal_c_Cmd__runRecursiveTest(const dal_c_Cmd* self, const dal_c_Project* proj);
static int dal_c_Cmd__runRecursiveClean(const dal_c_Cmd* self, const dal_c_Project* proj);

// === PUBLIC API ===

dal_c_Cmd* dal_c_Cmd_parse(int argc, const char* argv[]) {
    if (argc < 2) { return NULL; }
    dal_c_Cmd* cmd = calloc(1, sizeof(dal_c_Cmd));
    if (!cmd) { return NULL; }
    cmd->opts.profile = dal_c_default_profile;

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (str_eql(arg, dal_c_opt_prefix_long dal_c_opt_help)
            || (arg[0] == dal_c_opt_prefix_short[0] && arg[1] == dal_c_opt_help_short_char && arg[2] == '\0')) {
            cmd->is_help = true;
        }
        if (str_eql(arg, dal_c_opt_prefix_long dal_c_opt_version)
            || (arg[0] == dal_c_opt_prefix_short[0] && arg[1] == dal_c_opt_version_short_char && arg[2] == '\0')) {
            cmd->is_version = true;
        }
    }

    if (cmd->is_help || cmd->is_version) {
        return cmd;
    }

    cmd->action = dal_c_CmdAction_parse(argv[1]);
    if (cmd->action == dal_c_CmdAction_invalid) {
        free(cmd);
        return NULL;
    }

    int arg_idx = 2;
    if (argc > 2 && !str_startsWith(argv[2], dal_c_opt_prefix_short)) {
        dal_c_Profile profile = dal_c_Profile_parse(argv[2]);
        if (profile != dal_c_Profile_invalid) {
            cmd->opts.profile = profile;
            cmd->profile_explicit = true;
            arg_idx = 3;
        }
    }

    if (dal_c_Cmd__parseOptions(cmd, argc, argv, arg_idx) != 0) {
        dal_c_Cmd_cleanup(&cmd);
        return NULL;
    }
    if (dal_c_Cmd__validateCanonicalModifiers(cmd) != 0) {
        dal_c_Cmd_cleanup(&cmd);
        return NULL;
    }

    return cmd;
}

void dal_c_Cmd_cleanup(dal_c_Cmd** self) {
    if (!self || !*self) { return; }
    dal_c_Cmd* cmd = *self;

    dal_c_CompilerOpts_cleanup(&cmd->opts);
    for (int i = 0; i < cmd->input_count; ++i) {
        free(cmd->input_files[i]);
    }
    free((void*)cmd->input_files);
    for (int i = 0; i < cmd->explicit_dh_count; ++i) {
        free(cmd->explicit_dh_files[i]);
    }
    free((void*)cmd->explicit_dh_files);
    for (int i = 0; i < cmd->exclude_count; ++i) {
        free(cmd->exclude_paths[i]);
    }
    free((void*)cmd->exclude_paths);
    free(cmd->compiler_args);
    free(cmd->link_args);
    free(cmd->linker_script);
    free(cmd->objcopy);
    free(cmd->objcopy_format);
    free(cmd->dh_path_override);

    switch (cmd->action) {
    case dal_c_CmdAction_build:
        free(cmd->payload.build.target_path);
        free(cmd->payload.build.output_path);
        break;
    case dal_c_CmdAction_lib:
        free(cmd->payload.lib.target_path);
        free(cmd->payload.lib.output_path);
        break;
    case dal_c_CmdAction_run:
        free(cmd->payload.run.target_path);
        free(cmd->payload.run.output_path);
        free(cmd->payload.run.run_args);
        break;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl:
        free(cmd->payload.test.target_path);
        free(cmd->payload.test.output_path);
        free(cmd->payload.test.run_args);
        break;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        break;
    }

    free(cmd);
    *self = NULL;
}

static bool dal_c_Cmd__buildsLibrary(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    return cmd->action == dal_c_CmdAction_lib
        || (cmd->action == dal_c_CmdAction_build && cmd->payload.build.as_library);
}

static bool dal_c_Cmd__tryParseBoolValue(const char* value, bool* out) {
    assert(out != NULL);
    if (!value) {
        return false;
    }
    if (str_eql(value, "true") || str_eql(value, "yes") || str_eql(value, "on") || str_eql(value, "1")) {
        *out = true;
        return true;
    }
    if (str_eql(value, "false") || str_eql(value, "no") || str_eql(value, "off") || str_eql(value, "0")) {
        *out = false;
        return true;
    }
    return false;
}

static bool dal_c_Cmd__tryParseToggleValue(const char* value, dal_c_ToggleState* out) {
    assert(out != NULL);
    dal_c_ToggleState state = dal_c_ToggleState_parse(value);
    if (state == dal_c_ToggleState_invalid) {
        return false;
    }
    *out = state;
    return true;
}

static int dal_c_Cmd__applyAssignedBooleanOption(dal_c_Cmd* cmd, const char* opt, size_t opt_len, const char* value, bool* handled) {
    assert(cmd != NULL);
    assert(opt != NULL);
    assert(value != NULL);
    assert(handled != NULL);

#define dal_c_Cmd__OPT_IS(_name) (strlen(_name) == opt_len && strncmp(opt, (_name), opt_len) == 0)

    *handled = false;
    bool enabled = false;
    bool has_bool = dal_c_Cmd__tryParseBoolValue(value, &enabled);
    dal_c_ToggleState toggle = dal_c_ToggleState_auto;
    bool has_toggle = dal_c_Cmd__tryParseToggleValue(value, &toggle);

    *handled = true;
    if (dal_c_Cmd__OPT_IS(dal_c_opt_link_dsl)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.dsl_mode = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_libc)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.libc_linked = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_default_libs)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.default_libs_linked = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_start_files)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.start_files_linked = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_stdlib)) {
        if (!has_toggle) { *handled = false; }
        else {
            cmd->opts.default_libs_linked = toggle;
            cmd->opts.start_files_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_crt)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.start_files_linked = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_lto)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.lto_mode = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_omit_frame_pointer)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.omit_frame_pointer = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_function_sections)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.function_sections = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_data_sections)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.data_sections = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_gc_sections)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.gc_sections = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_whole_archive)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.whole_archive = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_unroll_loops)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.unroll_loops = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_unwind_tables)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.unwind_tables = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_async_unwind_tables)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.async_unwind_tables = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_strip)) {
        if (!has_toggle) { *handled = false; }
        else { cmd->opts.strip_mode = toggle; }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_freestanding)) {
        if (!has_bool) { *handled = false; }
        else {
        cmd->opts.compile_env = enabled ? dal_c_CompileEnv_freestanding : dal_c_CompileEnv_hosted;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_hosted)) {
        if (!has_bool) { *handled = false; }
        else {
        cmd->opts.compile_env = enabled ? dal_c_CompileEnv_hosted : dal_c_CompileEnv_freestanding;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_loose_errors)) {
        if (!has_bool) { *handled = false; }
        else {
        cmd->opts.loose_errors = enabled;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_image)) {
        if (!has_bool) { *handled = false; }
        else {
        if (cmd->action == dal_c_CmdAction_build) {
            cmd->payload.build.as_image = enabled;
        }
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_emit_preprocessed)) {
        if (!has_bool) { *handled = false; }
        else {
        if (cmd->action == dal_c_CmdAction_build) {
            cmd->payload.build.emit_preprocessed = enabled;
        }
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_emit_asm)) {
        if (!has_bool) { *handled = false; }
        else {
        if (cmd->action == dal_c_CmdAction_build) {
            cmd->payload.build.emit_asm = enabled;
        }
        }
    } else {
        *handled = false;
    }
#undef dal_c_Cmd__OPT_IS
    return 0;
}

static dal_c_Target dal_c_Cmd__resolveBuildTargetType(const dal_c_Cmd* cmd, dal_c_Linking linking, bool builds_library) {
    assert(cmd != NULL);

    if (builds_library) {
        return (linking == dal_c_Linking_shared) ? dal_c_Target_shared_lib : dal_c_Target_static_lib;
    }
    if (cmd->action == dal_c_CmdAction_build) {
        if (cmd->payload.build.emit_preprocessed) {
            return dal_c_Target_preprocessed;
        }
        if (cmd->payload.build.emit_asm) {
            return dal_c_Target_assembly;
        }
        if (cmd->payload.build.as_image) {
            return dal_c_Target_image;
        }
    }
    return dal_c_Target_executable;
}

static int dal_c_Cmd__validateCanonicalModifiers(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.self_boundary) {
        if (cmd->input_count > 0
            || cmd->payload.build.output_path != NULL
            || cmd->payload.build.sample_dir != dal_c_SampleDir_none
            || cmd->payload.build.build_all
            || cmd->payload.build.recursive
            || cmd->payload.build.as_library
            || cmd->payload.build.as_image
            || cmd->payload.build.dsl_first
            || cmd->exclude_count > 0) {
            (void)fprintf(stderr, "Error: `build --self` does not accept target, traversal, library, or exclude modifiers\n");
            return 1;
        }
    }

    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.as_library && cmd->payload.build.as_image) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--lib` and `--image`\n");
        return 1;
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_preprocessed && cmd->payload.build.emit_asm) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--emit-preprocessed` and `--emit-asm`\n");
        return 1;
    }
    if (cmd->action == dal_c_CmdAction_build
        && cmd->payload.build.as_library
        && (cmd->payload.build.emit_preprocessed || cmd->payload.build.emit_asm)) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--lib` with emit-only outputs\n");
        return 1;
    }
    if (cmd->action == dal_c_CmdAction_build
        && cmd->payload.build.as_image
        && (cmd->payload.build.emit_preprocessed || cmd->payload.build.emit_asm)) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--image` with emit-only outputs\n");
        return 1;
    }

    if (cmd->action == dal_c_CmdAction_clean && cmd->payload.clean.self_boundary) {
        if (cmd->payload.clean.cache_only
            || cmd->payload.clean.recursive
            || cmd->payload.clean.dsl_first
            || cmd->exclude_count > 0) {
            (void)fprintf(stderr, "Error: `clean --self` does not accept cache, traversal, DSL, or exclude modifiers\n");
            return 1;
        }
    }

    return 0;
}

void dal_c_Cmd_normalizeIntent(const dal_c_Cmd* cmd, dal_c_CommandIntent* out) {
    assert(cmd != NULL);
    assert(out != NULL);
    memset(out, 0, sizeof(*out));
    out->action = cmd->action;
    out->linking = dal_c_Linking_static;

    switch (cmd->action) {
    case dal_c_CmdAction_build:
        out->target_path = cmd->payload.build.target_path;
        out->target_root_name_hint = dal_c_Cmd__sampleDirCanonical(cmd->payload.build.sample_dir);
        out->output_path = cmd->payload.build.output_path;
        out->linking = cmd->payload.build.linking;
        out->build_all = cmd->payload.build.build_all;
        out->as_library = cmd->payload.build.as_library;
        out->self_boundary = cmd->payload.build.self_boundary;
        out->recursive = cmd->payload.build.recursive;
        out->dsl_first = cmd->payload.build.dsl_first;
        break;
    case dal_c_CmdAction_lib:
        out->target_path = cmd->payload.lib.target_path;
        out->output_path = cmd->payload.lib.output_path;
        out->linking = cmd->payload.lib.linking;
        out->as_library = true;
        break;
    case dal_c_CmdAction_run:
        out->target_path = cmd->payload.run.target_path;
        out->target_root_name_hint = dal_c_Cmd__sampleDirCanonical(cmd->payload.run.sample_dir);
        out->output_path = cmd->payload.run.output_path;
        out->run_args = cmd->payload.run.run_args;
        out->debug = cmd->payload.run.debug;
        break;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl:
        out->target_path = cmd->payload.test.target_path;
        out->target_root_name_hint = dal_c_dir_tests;
        out->output_path = cmd->payload.test.output_path;
        out->run_args = cmd->payload.test.run_args;
        out->build_all = cmd->payload.test.build_all;
        out->recursive = cmd->payload.test.recursive;
        out->debug = cmd->payload.test.debug;
        out->dsl_first = cmd->payload.test.dsl_first;
        break;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
        out->cache_only = cmd->payload.clean.cache_only;
        out->self_boundary = cmd->payload.clean.self_boundary;
        out->recursive = cmd->payload.clean.recursive;
        out->dsl_first = cmd->payload.clean.dsl_first;
        break;
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_invalid:
    default:
        break;
    }
}

int dal_c_Cmd_execute(const dal_c_Cmd* self, const dal_c_Project* proj) {
    switch (self->action) {
    case dal_c_CmdAction_build:
        if (self->payload.build.self_boundary) {
            return dal_c__buildSelf(self);
        }
        return dal_c_Cmd_makeTarget(self, proj);
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
        return dal_c_Cmd_makeTarget(self, proj);
    case dal_c_CmdAction_deps:
        return dal_c_Cmd_compileDeps(self, proj);
    case dal_c_CmdAction_clean:
        if (self->payload.clean.self_boundary) {
            return dal_c__cleanSelf(self);
        }
        return dal_c_Cmd_cleanTarget(self, proj);
    case dal_c_CmdAction_build_self:
        return dal_c__buildSelf(self);
    case dal_c_CmdAction_clean_self:
        return dal_c__cleanSelf(self);
    case dal_c_CmdAction_workspace:
        return dal_c_Cmd_createWorkspace(self, proj);
    case dal_c_CmdAction_project:
        return dal_c_Cmd_createProject(self, proj);
    case dal_c_CmdAction_build_dsl:
        return dal_c__buildDSL(self, proj);
    case dal_c_CmdAction_test_dsl:
        return dal_c__testDSL(self, proj);
    case dal_c_CmdAction_clean_dsl:
        return dal_c__cleanDSL(self, proj);
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        (void)fprintf(stderr, "Error: Unknown command\n");
        return 1;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
int dal_c_Cmd_makeTarget(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);
    assert(proj != NULL);
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    const bool builds_library = dal_c_Cmd__buildsLibrary(self);
    const bool build_all = intent.build_all;
    const bool aggregate_recursive_test_chain = self->action == dal_c_CmdAction_test
                                             && self->input_count == 0
                                             && !intent.target_path
                                             && (intent.recursive || intent.dsl_first);

    if (intent.dsl_first && self->action != dal_c_CmdAction_test) {
        int dsl_result = dal_c__buildDSL(self, proj);
        if (dsl_result != 0) { return dsl_result; }
    }

    int result = 0;
    const dal_c_Project* target_proj = proj;

    dal_c_TargetRequest target_request = { 0 };
    if (!dal_c_TargetRequest_resolve(target_proj, &intent, &target_request)) {
        return 1;
    }
    ArrStr* active_excludes = dal_c_Cmd__collectActiveExcludes(target_proj, &target_request, self);
    if (target_request.root && target_request.raw_target_path && target_request.resolved_path) {
        if (dal_c_Cmd__rejectExcludedPath(target_request.resolved_path, active_excludes) != 0) {
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }
    } else if (self->input_count > 0 && !dal_c_Cmd__inputsNeedCategoryResolution(self)) {
        for (int i = 0; i < self->input_count; ++i) {
            if (dal_c_Cmd__rejectExcludedPath(self->input_files[i], active_excludes) != 0) {
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        }
    }
    const bool is_test_mode = self->action == dal_c_CmdAction_test
                           || self->action == dal_c_CmdAction_test_dsl
                           || (target_request.root && target_request.root->name && str_eql(target_request.root->name, dal_c_dir_tests));

    if (target_proj->lib_count > 0) {
        if (self->verbose) {
            printf("Building %d libraries...\n", target_proj->lib_count);
        }
        for (int i = 0; i < target_proj->lib_count; ++i) {
            if (dal_c__buildSingleLibrary(self, target_proj, &target_proj->libraries[i]) != 0) {
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        }
    }

    bool needs_project_static_lib = (target_request.root && target_request.link_project) || is_test_mode;
    if (needs_project_static_lib) {
        int lib_result = dal_c_Cmd__ensureProjectStaticLibrary(self, target_proj);
        if (lib_result != 0) {
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return lib_result;
        }
    }

    if (is_test_mode) {
        ArrStr* tests = ArrStr_init();
        char* aggregate_compiler_args = NULL;
        if (aggregate_recursive_test_chain) {
            ArrStr* include_dirs = ArrStr_init();
            ArrStr* visited_roots = ArrStr_init();
            if (intent.dsl_first) {
                dal_c_Project* dh_proj = dal_c_Project_detectAt(proj->dh_path, proj->dh_path);
                if (!dh_proj) {
                    ArrStr_fini(&include_dirs);
                    ArrStr_fini(&visited_roots);
                    ArrStr_fini(&tests);
                    ArrStr_fini(&active_excludes);
                    dal_c_TargetRequest_cleanup(&target_request);
                    (void)fprintf(stderr, "Error: Failed to detect DH project for --dsl tests\n");
                    return 1;
                }
                dal_c_Cmd__collectRecursiveTestInputs(dh_proj, self, false, visited_roots, tests, include_dirs);
                dal_c_Project_cleanup(&dh_proj);
            }
            dal_c_Cmd__collectRecursiveTestInputs(target_proj, self, intent.recursive, visited_roots, tests, include_dirs);
            aggregate_compiler_args = dal_c_Cmd__buildIncludeArgs(include_dirs);
            ArrStr_fini(&include_dirs);
            ArrStr_fini(&visited_roots);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, false) != 0) {
                free(aggregate_compiler_args);
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        } else if (target_request.root) {
            ArrStr_fini(&tests);
            tests = dal_c_Cmd__collectTargetSources(&target_request);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, target_request.raw_target_path != NULL) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        } else if (self->input_count > 0 && !dal_c_Cmd__inputsNeedCategoryResolution(self)) {
            ArrStr_fini(&tests);
            tests = dal_c_Cmd__collectExplicitSources(self);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, true) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        } else {
            ArrStr_fini(&tests);
            tests = dal_c_Cmd__resolveInputs(target_proj, self, dal_c_dir_tests, true);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, self->input_count > 0) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        }
        if (ArrStr_len(tests) == 0) {
            bool allow_recursive_skip = intent.recursive && self->input_count == 0 && !target_request.raw_target_path;
            free(aggregate_compiler_args);
            ArrStr_fini(&tests);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            if (allow_recursive_skip) {
                if (self->action == dal_c_CmdAction_test) {
                    return dal_c_Cmd__runRecursiveTest(self, proj);
                }
                return 0;
            }
            (void)fprintf(stderr, "Error: No test source files found\n");
            return 1;
        }

        ArrStr* sources = ArrStr_init();
        char* output_name = NULL;
        const char* extra_compiler_args = aggregate_compiler_args;
        int registered_test_count = 0;
        for (int i = 0; i < ArrStr_len(tests); ++i) {
            if (dal_c__hasTestRegistration(ArrStr_at(tests, i))) {
                registered_test_count++;
            }
        }
        if (registered_test_count > 0) {
            const char* primary_test = ArrStr_at(tests, 0);
            if (ArrStr_len(tests) == 1) {
                output_name = dal_c_Cmd__basenameNoExt(primary_test);
            } else {
                output_name = strdup(self->action == dal_c_CmdAction_test ? "test" : "tests");
            }

            char* runner_source = dal_c_Cmd__writeTestMainSource(target_proj, self, output_name);
            if (!runner_source) {
                free(aggregate_compiler_args);
                ArrStr_fini(&sources);
                ArrStr_fini(&tests);
                free(output_name);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            ArrStr_push(sources, runner_source);
            free(runner_source);
            for (int i = 0; i < ArrStr_len(tests); ++i) {
                ArrStr_push(sources, ArrStr_at(tests, i));
            }
        } else if (ArrStr_len(tests) == 1) {
            ArrStr_push(sources, ArrStr_at(tests, 0));
            output_name = dal_c_Cmd__basenameNoExt(ArrStr_at(tests, 0));
        } else {
            (void)fprintf(
                stderr,
                "Error: Aggregate test build requires at least one registration-style source when multiple test files are selected\n"
            );
            free(aggregate_compiler_args);
            ArrStr_fini(&sources);
            ArrStr_fini(&tests);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }
        result = dal_c_Cmd__buildFromSources(
            self,
            target_proj,
            sources,
            output_name,
            dal_c_Target_executable,
            extra_compiler_args,
            true,
            true
        );
        free(aggregate_compiler_args);
        ArrStr_fini(&sources);
        ArrStr_fini(&tests);
        free(output_name);
    } else if (target_request.root) {
        ArrStr* target_sources = dal_c_Cmd__collectTargetSources(&target_request);
        if (dal_c_Cmd__applyExcludeContract(&target_sources, active_excludes, target_request.raw_target_path != NULL) != 0) {
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }
        if (ArrStr_len(target_sources) == 0) {
            (void)fprintf(stderr, "Error: No source files found in target root `%s`\n", target_request.root->name);
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }

        if (self->action == dal_c_CmdAction_run && target_request.kind != dal_c_Target_executable) {
            (void)fprintf(stderr, "Error: `run` requires an executable target root\n");
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }

        ArrStr* sources = ArrStr_init();
        for (int i = 0; i < ArrStr_len(target_sources); ++i) {
            ArrStr_push(sources, ArrStr_at(target_sources, i));
        }

        char* output_name = NULL;
        if (target_request.resolved_path && target_request.resolved_is_dir) {
            output_name = path_basename(target_request.resolved_path);
        } else if (target_request.resolved_path) {
            output_name = dal_c_Cmd__basenameNoExt(target_request.resolved_path);
        } else {
            output_name = strdup(target_request.root->name);
        }

        dal_c_Target target_type = target_request.kind;
        if (self->action == dal_c_CmdAction_build) {
            if (self->payload.build.emit_preprocessed) {
                target_type = dal_c_Target_preprocessed;
            } else if (self->payload.build.emit_asm) {
                target_type = dal_c_Target_assembly;
            } else if (builds_library) {
                dal_c_Linking linking = intent.linking;
                target_type = (linking == dal_c_Linking_shared) ? dal_c_Target_shared_lib : dal_c_Target_static_lib;
            } else if (self->payload.build.as_image) {
                target_type = dal_c_Target_image;
            }
        } else if (builds_library) {
            dal_c_Linking linking = intent.linking;
            target_type = (linking == dal_c_Linking_shared) ? dal_c_Target_shared_lib : dal_c_Target_static_lib;
        }
        if (target_type == dal_c_Target_lib) {
            result = dal_c_Cmd__buildLibrarySetFromSources(self, target_proj, sources, output_name, true);
        } else {
            result = dal_c_Cmd__buildFromSources(self, target_proj, sources, output_name, target_type, NULL, true, true);
        }
        ArrStr_fini(&sources);
        ArrStr_fini(&target_sources);
        free(output_name);
    } else {
        ArrStr* sources = ArrStr_init();
        if (self->input_count > 0) {
            ArrStr_fini(&sources);
            sources = dal_c_Cmd__collectExplicitSources(self);
            if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, true) != 0) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        } else if (build_all || !intent.target_path) {
            ArrStr_fini(&sources);
            sources = dal_c__collectSourceFiles(proj, NULL);
            if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, false) != 0) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        } else {
            ArrStr_push(sources, intent.target_path);
            if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, true) != 0) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        }

        if (builds_library && dal_c__isHeaderOnlyBuild(self, proj, sources)) {
            const char* output_name = proj->name;
            char* output_name_alloc = NULL;
            dal_c_CompilerOpts merged_opts = { 0 };
            dal_c_BuildDefaults defaults = { 0 };
            merged_opts.profile = dal_c_Profile_invalid;
            if (self->input_count > 0) {
                output_name_alloc = dal_c_Cmd__basenameNoExt(self->input_files[0]);
                output_name = output_name_alloc;
            }
            dal_c_Cmd__mergeBuildProperties(&merged_opts, &defaults, proj, sources, self);
            if (!intent.output_path && defaults.output_name) {
                output_name = defaults.output_name;
            }
            result = dal_c__buildHeaderOnlyLibrary(self, proj, output_name);
            dal_c_CompilerOpts_cleanup(&merged_opts);
            dal_c_BuildDefaults_cleanup(&defaults);
            free(output_name_alloc);
            ArrStr_fini(&sources);
            if (result != 0) {
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return result;
            }
        } else {
            if (ArrStr_len(sources) == 0) {
                (void)fprintf(stderr, "Error: No source files found\n");
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }

            const char* output_name = proj->name;
            char* output_name_alloc = NULL;
            if (self->input_count > 0) {
                output_name_alloc = dal_c_Cmd__basenameNoExt(self->input_files[0]);
                output_name = output_name_alloc;
            }

            dal_c_CompilerOpts effective_opts = { 0 };
            dal_c_BuildDefaults effective_defaults = { 0 };
            effective_opts.profile = dal_c_Profile_invalid;
            dal_c_Cmd__mergeBuildProperties(&effective_opts, &effective_defaults, proj, sources, self);

            dal_c_Target target_type = dal_c_Cmd__resolveBuildTargetType(self, intent.linking, builds_library);
            if (self->action == dal_c_CmdAction_build
                && !builds_library
                && !self->payload.build.emit_preprocessed
                && !self->payload.build.emit_asm
                && !self->payload.build.as_image
                && effective_defaults.target_kind_set) {
                target_type = effective_defaults.target_kind;
            }
            if (self->action == dal_c_CmdAction_run
                && effective_defaults.target_kind_set
                && effective_defaults.target_kind != dal_c_Target_executable) {
                (void)fprintf(
                    stderr,
                    "Error: `run` requires an executable plain project, but project kind is `%s`\n",
                    dal_c_Target_format(effective_defaults.target_kind)
                );
                dal_c_BuildDefaults_cleanup(&effective_defaults);
                dal_c_CompilerOpts_cleanup(&effective_opts);
                free(output_name_alloc);
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            bool use_unity = (self->action == dal_c_CmdAction_build || self->action == dal_c_CmdAction_run)
                          && self->input_count > 1;
            if (target_type == dal_c_Target_preprocessed || target_type == dal_c_Target_assembly) {
                use_unity = false;
            }
            if (use_unity) {
                for (int i = 0; i < ArrStr_len(sources); ++i) {
                    const char* src = ArrStr_at(sources, i);
                    if (!str_endsWith(src, ".c")) {
                        use_unity = false;
                        break;
                    }
                }
            }
            if (use_unity) {
                char* unity_source = dal_c_Cmd__writeUnitySource(proj, self, sources, output_name);
                if (!unity_source) {
                    dal_c_BuildDefaults_cleanup(&effective_defaults);
                    dal_c_CompilerOpts_cleanup(&effective_opts);
                    free(output_name_alloc);
                    ArrStr_fini(&sources);
                    ArrStr_fini(&active_excludes);
                    dal_c_TargetRequest_cleanup(&target_request);
                    return 1;
                }
                ArrStr* bundled = ArrStr_init();
                ArrStr_push(bundled, unity_source);
                free(unity_source);
                ArrStr_fini(&sources);
                sources = bundled;
            }

            if (target_type == dal_c_Target_lib) {
                result = dal_c_Cmd__buildLibrarySetFromSources(self, proj, sources, output_name, true);
            } else {
                result = dal_c_Cmd__buildFromSources(self, proj, sources, output_name, target_type, NULL, true, true);
            }
            dal_c_BuildDefaults_cleanup(&effective_defaults);
            dal_c_CompilerOpts_cleanup(&effective_opts);
            free(output_name_alloc);
            ArrStr_fini(&sources);
        }
    }

    ArrStr_fini(&active_excludes);
    dal_c_TargetRequest_cleanup(&target_request);
    if (result != 0) { return result; }
    if (intent.recursive) {
        if (self->action == dal_c_CmdAction_test) {
            if (aggregate_recursive_test_chain) {
                return 0;
            }
            return dal_c_Cmd__runRecursiveTest(self, proj);
        }
        return dal_c_Cmd__runRecursiveBuild(self, proj);
    }
    return 0;
}

int dal_c_Cmd_compileDeps(const dal_c_Cmd* self, const dal_c_Project* proj) {
    if (proj->lib_count == 0) { return 0; }
    if (self->verbose) {
        printf("Building %d libraries...\n", proj->lib_count);
    }
    for (int i = 0; i < proj->lib_count; ++i) {
        if (dal_c__buildSingleLibrary(self, proj, &proj->libraries[i]) != 0) {
            return 1;
        }
    }
    return 0;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
int dal_c_Cmd_cleanTarget(const dal_c_Cmd* self, const dal_c_Project* proj) {
    bool cleaned = false;
    bool cache_only = self->payload.clean.cache_only;
    bool recursive = self->payload.clean.recursive;
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(self->opts.profile);

    if (self->payload.clean.dsl_first) {
        int dsl_result = dal_c__cleanDSL(self, proj);
        if (dsl_result != 0) { return dsl_result; }
    }

    char* build_dir = dal_c_Project_getBuildDir(proj);
    char* build_target = NULL;
    if (self->profile_explicit && profile) {
        build_target = path_join(build_dir, profile->name);
    } else {
        build_target = strdup(build_dir);
    }
    if (build_target && path_isDir(build_target)) {
        if (self->verbose) {
            printf("Removing: %s\n", build_target);
        }
        dir_removeRecur(build_target);
        printf("Cleaned: %s\n", build_target);
        cleaned = true;
    }
    free(build_target);
    free(build_dir);

    if (!cache_only && !self->profile_explicit) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        if (deps_dir && path_isDir(deps_dir)) {
            if (self->verbose) {
                printf("Removing: %s\n", deps_dir);
            }
            dir_removeRecur(deps_dir);
            printf("Cleaned: %s\n", deps_dir);
            cleaned = true;
        }
        free(deps_dir);
    }

    char* cache_dir = dal_c__cacheBaseDir(proj);
    char* cache_target = NULL;
    if (self->profile_explicit && profile) {
        char* cache_dh_c = path_join(cache_dir, dal_c_tool_name);
        cache_target = path_join(cache_dh_c, profile->name);
        free(cache_dh_c);
    } else {
        cache_target = strdup(cache_dir);
    }
    if (cache_target && path_isDir(cache_target)) {
        if (self->verbose) {
            printf("Removing: %s\n", cache_target);
        }
        dir_removeRecur(cache_target);
        printf("Cleaned: %s\n", cache_target);
        cleaned = true;
    }
    free(cache_target);
    free(cache_dir);

    if (recursive) {
        if (proj->lib_count > 0) {
            for (int i = 0; i < proj->lib_count; ++i) {
                const dal_c_Lib* lib = &proj->libraries[i];
                if (!lib->path) { continue; }
                dal_c_Project* lib_proj = dal_c_Project_detectAt(lib->path, proj->dh_path);
                if (lib_proj) {
                    dal_c_Cmd child = *self;
                    child.payload.clean.recursive = false;
                    child.payload.clean.dsl_first = false;
                    dal_c_Cmd_cleanTarget(&child, lib_proj);
                    dal_c_Project_cleanup(&lib_proj);
                }
            }
        }
        int child_result = dal_c_Cmd__runRecursiveClean(self, proj);
        if (child_result != 0) { return child_result; }
    }

    if (!cleaned) {
        printf("Nothing to clean\n");
    }
    return 0;
}

int dal_c_Cmd_createWorkspace(const dal_c_Cmd* self, const dal_c_Project* proj) {
    (void)self;
    (void)proj;
    (void)fprintf(stderr, "Not implemented: workspace\n");
    return 1;
}

int dal_c_Cmd_createProject(const dal_c_Cmd* self, const dal_c_Project* proj) {
    (void)self;
    (void)proj;
    (void)fprintf(stderr, "Not implemented: project\n");
    return 1;
}

// === PRIVATE IMPLEMENTATIONS ===

static void dal_c_Cmd__addToArray(char*** arr, int* count, const char* value) {
    assert(arr != NULL);
    assert(count != NULL);
    assert(value != NULL);
    int new_count = *count + 1;
    char** new_arr = (char**)realloc((void*)*arr, (size_t)new_count * sizeof(char*));
    assert(new_arr != NULL && "Out of memory");
    new_arr[*count] = strdup(value);
    *arr = new_arr;
    *count = new_count;
}

static void dal_c_Cmd__pushOwnedString(char*** arr, int* count, char* value) {
    assert(arr != NULL);
    assert(count != NULL);
    assert(value != NULL);
    int new_count = *count + 1;
    char** new_arr = (char**)realloc((void*)*arr, (size_t)new_count * sizeof(char*));
    assert(new_arr != NULL && "Out of memory");
    new_arr[*count] = value;
    *arr = new_arr;
    *count = new_count;
}

static void dal_c_Cmd__setOwnedString(char** slot, const char* value) {
    assert(slot != NULL);
    free(*slot);
    *slot = value ? strdup(value) : NULL;
}

static bool dal_c_Cmd__isSourceOrHeader(const char* name) {
    assert(name != NULL);
    return str_endsWith(name, ".c")
        || str_endsWith(name, ".h")
        || str_endsWith(name, ".S")
        || str_endsWith(name, ".s");
}

static bool dal_c_Cmd__isBuildSource(const char* name) {
    assert(name != NULL);
    return str_endsWith(name, ".c")
        || str_endsWith(name, ".S")
        || str_endsWith(name, ".s");
}

static char** dal_c_Cmd__targetPathSlot(dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return &cmd->payload.build.target_path;
    case dal_c_CmdAction_lib: return &cmd->payload.lib.target_path;
    case dal_c_CmdAction_run: return &cmd->payload.run.target_path;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl: return &cmd->payload.test.target_path;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default: return NULL;
    }
}

static char** dal_c_Cmd__outputPathSlot(dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return &cmd->payload.build.output_path;
    case dal_c_CmdAction_lib: return &cmd->payload.lib.output_path;
    case dal_c_CmdAction_run: return &cmd->payload.run.output_path;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl: return &cmd->payload.test.output_path;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default: return NULL;
    }
}

static char** dal_c_Cmd__runArgsSlot(dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_run: return &cmd->payload.run.run_args;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl: return &cmd->payload.test.run_args;
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default: return NULL;
    }
}

static void dal_c_Cmd__setPrimaryTargetPath(dal_c_Cmd* cmd, const char* path) {
    assert(cmd != NULL);
    if (!path) { return; }
    char** target_path_slot = dal_c_Cmd__targetPathSlot(cmd);
    if (target_path_slot && !*target_path_slot) {
        *target_path_slot = strdup(path);
    }
}

static void dal_c_Cmd__setOutputPath(dal_c_Cmd* cmd, const char* path) {
    assert(cmd != NULL);
    char** output_path_slot = dal_c_Cmd__outputPathSlot(cmd);
    if (output_path_slot) {
        dal_c_Cmd__setOwnedString(output_path_slot, path);
    }
}

static void dal_c_Cmd__setStdlibBundle(dal_c_CompilerOpts* opts, bool linked) {
    assert(opts != NULL);
    opts->default_libs_linked = linked ? dal_c_ToggleState_enabled : dal_c_ToggleState_disabled;
    opts->start_files_linked = linked ? dal_c_ToggleState_enabled : dal_c_ToggleState_disabled;
}

static void dal_c_Cmd__setCrtStartFiles(dal_c_CompilerOpts* opts, bool linked) {
    assert(opts != NULL);
    opts->start_files_linked = linked ? dal_c_ToggleState_enabled : dal_c_ToggleState_disabled;
}

static const char* dal_c_Cmd__sampleDirCanonical(dal_c_SampleDir sample_dir) {
    switch (sample_dir) {
    case dal_c_SampleDir_samples:
        return dal_c_dir_samples;
    case dal_c_SampleDir_examples:
        return dal_c_dir_examples;
    case dal_c_SampleDir_tests:
        return dal_c_dir_tests;
    case dal_c_SampleDir_none:
    default:
        return NULL;
    }
}

static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action) {
    if (str_startsWith(arg, dal_c_opt_prefix_long)) {
        const char* opt = arg + 2;
        if ((str_eql(opt, dal_c_opt_output) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl))
            || str_eql(opt, dal_c_opt_show_commands)
            || str_eql(opt, dal_c_opt_verbose)
            || str_eql(opt, dal_c_opt_link_dsl)
            || (str_eql(opt, dal_c_opt_self) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_clean))
            || (str_eql(opt, dal_c_opt_lib) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib))
            || ((str_eql(opt, dal_c_opt_static) || str_eql(opt, dal_c_opt_shared))
                && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib))
            || str_eql(opt, dal_c_opt_cache)
            || str_eql(opt, dal_c_opt_recur)
            || str_eql(opt, dal_c_opt_debug)
            || str_eql(opt, dal_c_opt_freestanding)
            || str_eql(opt, dal_c_opt_hosted)
            || str_eql(opt, dal_c_opt_link_libc)
            || str_eql(opt, dal_c_opt_link_default_libs)
            || str_eql(opt, dal_c_opt_link_start_files)
            || str_eql(opt, dal_c_opt_link_stdlib)
            || str_eql(opt, dal_c_opt_link_crt)
            || str_eql(opt, dal_c_opt_lto)
            || str_eql(opt, dal_c_opt_omit_frame_pointer)
            || str_eql(opt, dal_c_opt_function_sections)
            || str_eql(opt, dal_c_opt_data_sections)
            || str_eql(opt, dal_c_opt_gc_sections)
            || str_eql(opt, dal_c_opt_whole_archive)
            || str_eql(opt, dal_c_opt_unroll_loops)
            || str_eql(opt, dal_c_opt_unwind_tables)
            || str_eql(opt, dal_c_opt_async_unwind_tables)
            || str_eql(opt, dal_c_opt_strip)
            || str_eql(opt, dal_c_opt_icf)
            || str_eql(opt, dal_c_opt_loose_errors)
            || (str_eql(opt, dal_c_opt_image) && action == dal_c_CmdAction_build)
            || (str_eql(opt, dal_c_opt_emit_preprocessed) && action == dal_c_CmdAction_build)
            || (str_eql(opt, dal_c_opt_emit_asm) && action == dal_c_CmdAction_build)
            || str_eql(opt, dal_c_opt_version_core)
            || str_eql(opt, dal_c_opt_version_prefix)
            || str_eql(opt, dal_c_opt_version_suffix)
            || str_eql(opt, dal_c_opt_version_build)
            || str_eql(opt, dal_c_opt_version_record)
            || str_eql(opt, dal_c_opt_sample)
            || str_eql(opt, dal_c_opt_example)
            || str_eql(opt, dal_c_opt_test)
            || str_eql(opt, dal_c_opt_all)
            || str_eql(opt, dal_c_opt_exclude)
            || str_eql(opt, dal_c_opt_dsl)) {
            return true;
        }
        if (((str_startsWith(opt, dal_c_opt_output)) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl))
            || str_startsWith(opt, dal_c_opt_link_dsl)
            || str_startsWith(opt, dal_c_opt_freestanding)
            || str_startsWith(opt, dal_c_opt_hosted)
            || str_startsWith(opt, dal_c_opt_link_libc)
            || str_startsWith(opt, dal_c_opt_link_default_libs)
            || str_startsWith(opt, dal_c_opt_link_start_files)
            || str_startsWith(opt, dal_c_opt_link_stdlib)
            || str_startsWith(opt, dal_c_opt_link_crt)
            || str_startsWith(opt, dal_c_opt_lto)
            || str_startsWith(opt, dal_c_opt_omit_frame_pointer)
            || str_startsWith(opt, dal_c_opt_function_sections)
            || str_startsWith(opt, dal_c_opt_data_sections)
            || str_startsWith(opt, dal_c_opt_gc_sections)
            || str_startsWith(opt, dal_c_opt_whole_archive)
            || str_startsWith(opt, dal_c_opt_unroll_loops)
            || str_startsWith(opt, dal_c_opt_unwind_tables)
            || str_startsWith(opt, dal_c_opt_async_unwind_tables)
            || str_startsWith(opt, dal_c_opt_strip)
            || str_startsWith(opt, dal_c_opt_icf)
            || str_startsWith(opt, dal_c_opt_loose_errors)
            || str_startsWith(opt, dal_c_opt_image)
            || str_startsWith(opt, dal_c_opt_emit_preprocessed)
            || str_startsWith(opt, dal_c_opt_emit_asm)
            || str_startsWith(opt, dal_c_opt_version_core)
            || str_startsWith(opt, dal_c_opt_version_prefix)
            || str_startsWith(opt, dal_c_opt_version_suffix)
            || str_startsWith(opt, dal_c_opt_version_build)
            || str_startsWith(opt, dal_c_opt_version_record)
            || str_startsWith(opt, dal_c_opt_compiler)
            || str_startsWith(opt, dal_c_opt_std)
            || str_startsWith(opt, dal_c_opt_args)
            || str_startsWith(opt, dal_c_opt_comp_args)
            || str_startsWith(opt, dal_c_opt_link_args)
            || str_startsWith(opt, dal_c_opt_target_arch)
            || str_startsWith(opt, dal_c_opt_target_abi)
            || str_startsWith(opt, dal_c_opt_link_script)
            || str_startsWith(opt, dal_c_opt_objcopy)
            || str_startsWith(opt, dal_c_opt_objcopy_format)
            || str_startsWith(opt, dal_c_opt_exec_args)
            || str_startsWith(opt, dal_c_opt_dh)
            || str_startsWith(opt, dal_c_opt_exclude)
            || str_startsWith(opt, dal_c_opt_arch)
            || str_startsWith(opt, dal_c_opt_target)
            || str_startsWith(opt, dal_c_opt_sysroot)
            || str_startsWith(opt, dal_c_opt_entry)
            || str_startsWith(opt, dal_c_opt_include)
            || str_startsWith(opt, dal_c_opt_link)
            || str_startsWith(opt, dal_c_opt_define)
            || str_startsWith(opt, dal_c_opt_undef)
            || str_startsWith(opt, dal_c_opt_isystem)
            || str_startsWith(opt, dal_c_opt_file)
            || str_startsWith(opt, dal_c_opt_dh_file)) {
            return true;
        }
        return false;
    }

    if (arg[0] == dal_c_opt_prefix_short[0] && arg[1] != dal_c_opt_prefix_short[0]) {
        char c = arg[1];
        if (c == dal_c_opt_include_short_char || c == dal_c_opt_link_short_char
            || c == dal_c_opt_define_short_char || c == dal_c_opt_undef_short_char) {
            return true;
        }
        if (c == dal_c_opt_output_short_char) {
            return action == dal_c_CmdAction_build || action == dal_c_CmdAction_run
                || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl;
        }
    }

    if (dal_c_Cmd__isSourceOrHeader(arg)) {
        return action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib
            || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test
            || action == dal_c_CmdAction_test_dsl;
    }

    if (str_eql(arg, dal_c_opt_all_alias) || str_eql(arg, dal_c_opt_all)) {
        return action == dal_c_CmdAction_build || action == dal_c_CmdAction_test
            || action == dal_c_CmdAction_test_dsl;
    }

    return false;
}

static int dal_c_Cmd__parseOptions(dal_c_Cmd* cmd, int argc, const char* argv[], int start_idx) {
    for (int i = start_idx; i < argc; ++i) {
        const char* arg = argv[i];

        if (str_startsWith(arg, dal_c_opt_prefix_short) && !dal_c_Cmd__isValidOption(arg, cmd->action)) {
            (void)fprintf(stderr, "Error: Unknown option: %s\n", arg);
            return 1;
        }

        if (str_startsWith(arg, dal_c_opt_prefix_long)) {
            const char* opt = arg + 2;
            const char* eq = strchr(opt, dal_c_opt_value_sep[0]);

            if (eq) {
                size_t opt_len = (size_t)(eq - opt);
                const char* value = eq + 1;
                bool handled_bool = false;
                if (dal_c_Cmd__applyAssignedBooleanOption(cmd, opt, opt_len, value, &handled_bool) != 0) {
                    return 1;
                }
                if (handled_bool) {
                    continue;
                }

                if (strncmp(opt, dal_c_opt_compiler, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.compiler, value);
                } else if (strncmp(opt, dal_c_opt_std, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.c_std, value);
                } else if (strncmp(opt, dal_c_opt_arch, opt_len) == 0 || strncmp(opt, dal_c_opt_target, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.arch_target, value);
                } else if (strncmp(opt, dal_c_opt_target_arch, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_arch, value);
                } else if (strncmp(opt, dal_c_opt_target_abi, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_abi, value);
                } else if (strncmp(opt, dal_c_opt_sysroot, opt_len) == 0) {
                    if (!path_exists(value)) {
                        (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.sysroot, value);
                } else if (strncmp(opt, dal_c_opt_entry, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.entry_symbol, value);
                } else if (strncmp(opt, dal_c_opt_include, opt_len) == 0) {
                    if (!path_exists(value)) {
                        (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__addToArray(&cmd->opts.include_paths, &cmd->opts.include_count, value);
                } else if (strncmp(opt, dal_c_opt_isystem, opt_len) == 0) {
                    dal_c_Cmd__addToArray(&cmd->opts.isystem_paths, &cmd->opts.isystem_count, value);
                } else if (strncmp(opt, dal_c_opt_link, opt_len) == 0) {
                    dal_c_Cmd__addToArray(&cmd->opts.link_libs, &cmd->opts.link_count, value);
                } else if (strncmp(opt, dal_c_opt_define, opt_len) == 0) {
                    dal_c_Cmd__addToArray(&cmd->opts.define_macros, &cmd->opts.define_count, value);
                } else if (strncmp(opt, dal_c_opt_undef, opt_len) == 0) {
                    dal_c_Cmd__addToArray(&cmd->opts.undef_macros, &cmd->opts.undef_count, value);
                } else if (strncmp(opt, dal_c_opt_dh, opt_len) == 0) {
                    if (!path_isDir(value)) {
                        (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->dh_path_override, value);
                } else if (strncmp(opt, dal_c_opt_comp_args, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->compiler_args, value);
                } else if (strncmp(opt, dal_c_opt_link_args, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->link_args, value);
                } else if (strncmp(opt, dal_c_opt_link_script, opt_len) == 0) {
                    char* abs_path = path_abs(value);
                    if (!abs_path || !path_isFile(abs_path)) {
                        free(abs_path);
                        (void)fprintf(stderr, "Error: Not a file: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->linker_script, abs_path);
                    free(abs_path);
                } else if (strncmp(opt, dal_c_opt_objcopy, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->objcopy, value);
                } else if (strncmp(opt, dal_c_opt_objcopy_format, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->objcopy_format, value);
                } else if (strncmp(opt, dal_c_opt_version_core, opt_len) == 0) {
                    if (!dal_c_VersionSpec_parseCore(&cmd->opts.version, value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_core, value);
                        return 1;
                    }
                } else if (strncmp(opt, dal_c_opt_version_prefix, opt_len) == 0) {
                    if (!dal_c_VersionSpec_parsePrefix(&cmd->opts.version, value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_prefix, value);
                        return 1;
                    }
                } else if (strncmp(opt, dal_c_opt_version_suffix, opt_len) == 0) {
                    if (!dal_c_VersionSpec_parseSuffix(&cmd->opts.version, value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_suffix, value);
                        return 1;
                    }
                } else if (strncmp(opt, dal_c_opt_version_build, opt_len) == 0) {
                    if (!dal_c_VersionSpec_parseBuild(&cmd->opts.version, value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_build, value);
                        return 1;
                    }
                } else if (strncmp(opt, dal_c_opt_version_record, opt_len) == 0) {
                    dal_c_VersionRecordMode mode = dal_c_VersionRecordMode_parse(value);
                    if (mode == dal_c_VersionRecordMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_record, value);
                        return 1;
                    }
                    cmd->version_record_mode = mode;
                } else if (strncmp(opt, dal_c_opt_icf, opt_len) == 0) {
                    dal_c_IcfMode mode = dal_c_IcfMode_parse(value);
                    if (mode == dal_c_IcfMode_auto && !str_eql(value, dal_c_icf_mode_auto)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_icf, value);
                        return 1;
                    }
                    cmd->opts.icf_mode = mode;
                } else if (strncmp(opt, dal_c_opt_args, opt_len) == 0) {
                    char** run_args_slot = dal_c_Cmd__runArgsSlot(cmd);
                    if (run_args_slot) {
                        dal_c_Cmd__setOwnedString(run_args_slot, value);
                    } else {
                        dal_c_Cmd__setOwnedString(&cmd->compiler_args, value);
                    }
                } else if (strncmp(opt, dal_c_opt_exec_args, opt_len) == 0) {
                    char** run_args_slot = dal_c_Cmd__runArgsSlot(cmd);
                    if (run_args_slot) {
                        dal_c_Cmd__setOwnedString(run_args_slot, value);
                    }
                } else if (strncmp(opt, dal_c_opt_file, opt_len) == 0) {
                    char* abs_path = path_abs(value);
                    if (!abs_path || !path_isFile(abs_path)) {
                        free(abs_path);
                        (void)fprintf(stderr, "Error: Not a file: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__pushOwnedString(&cmd->input_files, &cmd->input_count, abs_path);
                    dal_c_Cmd__setPrimaryTargetPath(cmd, abs_path);
                } else if (strncmp(opt, dal_c_opt_output, opt_len) == 0) {
                    dal_c_Cmd__setOutputPath(cmd, value);
                } else if (strncmp(opt, dal_c_opt_dh_file, opt_len) == 0) {
                    char* abs_path = path_abs(value);
                    if (!abs_path || !path_isFile(abs_path)) {
                        free(abs_path);
                        (void)fprintf(stderr, "Error: Not a file: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__pushOwnedString(&cmd->explicit_dh_files, &cmd->explicit_dh_count, abs_path);
                } else if (strncmp(opt, dal_c_opt_exclude, opt_len) == 0) {
                    if (dal_c_Cmd__pushExcludePath(cmd, value) != 0) {
                        return 1;
                    }
                } else {
                    (void)fprintf(stderr, "Error: Unknown option: %s\n", arg);
                    return 1;
                }
            } else {
                if (str_eql(opt, dal_c_opt_show_commands)) {
                    cmd->show_commands = true;
                } else if (str_eql(opt, dal_c_opt_output)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOutputPath(cmd, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_comp_args)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->compiler_args, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_link_args)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->link_args, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_target_arch)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_arch, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_target_abi)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_abi, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_link_script)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    char* abs_path = path_abs(argv[i + 1]);
                    if (!abs_path || !path_isFile(abs_path)) {
                        free(abs_path);
                        (void)fprintf(stderr, "Error: Not a file: %s\n", argv[i + 1]);
                        return 1;
                    }
                    ++i;
                    dal_c_Cmd__setOwnedString(&cmd->linker_script, abs_path);
                    free(abs_path);
                } else if (str_eql(opt, dal_c_opt_objcopy)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->objcopy, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_objcopy_format)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->objcopy_format, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_version_core)) {
                    if (i + 1 >= argc || !dal_c_VersionSpec_parseCore(&cmd->opts.version, argv[i + 1])) {
                        (void)fprintf(stderr, "Error: Invalid or missing value for option: %s\n", arg);
                        return 1;
                    }
                    ++i;
                } else if (str_eql(opt, dal_c_opt_version_prefix)) {
                    if (i + 1 >= argc || !dal_c_VersionSpec_parsePrefix(&cmd->opts.version, argv[i + 1])) {
                        (void)fprintf(stderr, "Error: Invalid or missing value for option: %s\n", arg);
                        return 1;
                    }
                    ++i;
                } else if (str_eql(opt, dal_c_opt_version_suffix)) {
                    if (i + 1 >= argc || !dal_c_VersionSpec_parseSuffix(&cmd->opts.version, argv[i + 1])) {
                        (void)fprintf(stderr, "Error: Invalid or missing value for option: %s\n", arg);
                        return 1;
                    }
                    ++i;
                } else if (str_eql(opt, dal_c_opt_version_build)) {
                    if (i + 1 >= argc || !dal_c_VersionSpec_parseBuild(&cmd->opts.version, argv[i + 1])) {
                        (void)fprintf(stderr, "Error: Invalid or missing value for option: %s\n", arg);
                        return 1;
                    }
                    ++i;
                } else if (str_eql(opt, dal_c_opt_version_record)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_VersionRecordMode mode = dal_c_VersionRecordMode_parse(argv[i + 1]);
                    if (mode == dal_c_VersionRecordMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_version_record, argv[i + 1]);
                        return 1;
                    }
                    cmd->version_record_mode = mode;
                    ++i;
                } else if (str_eql(opt, dal_c_opt_verbose)) {
                    cmd->verbose = true;
                } else if (str_eql(opt, dal_c_opt_self)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.self_boundary = true;
                    } else if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.self_boundary = true;
                    }
                } else if (str_eql(opt, dal_c_opt_lib)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.as_library = true;
                    }
                } else if (str_eql(opt, dal_c_opt_image)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.as_image = true;
                    }
                } else if (str_eql(opt, dal_c_opt_emit_preprocessed)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.emit_preprocessed = true;
                    }
                } else if (str_eql(opt, dal_c_opt_emit_asm)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.emit_asm = true;
                    }
                } else if (str_eql(opt, dal_c_opt_exclude)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    if (dal_c_Cmd__pushExcludePath(cmd, argv[++i]) != 0) {
                        return 1;
                    }
                } else if (str_eql(opt, dal_c_opt_link_dsl)) {
                    cmd->opts.dsl_mode = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_freestanding)) {
                    cmd->opts.compile_env = dal_c_CompileEnv_freestanding;
                } else if (str_eql(opt, dal_c_opt_hosted)) {
                    cmd->opts.compile_env = dal_c_CompileEnv_hosted;
                } else if (str_eql(opt, dal_c_opt_link_libc)) {
                    cmd->opts.libc_linked = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_link_default_libs)) {
                    cmd->opts.default_libs_linked = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_link_start_files)) {
                    cmd->opts.start_files_linked = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_link_stdlib)) {
                    dal_c_Cmd__setStdlibBundle(&cmd->opts, true);
                } else if (str_eql(opt, dal_c_opt_link_crt)) {
                    dal_c_Cmd__setCrtStartFiles(&cmd->opts, true);
                } else if (str_eql(opt, dal_c_opt_lto)) {
                    cmd->opts.lto_mode = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_omit_frame_pointer)) {
                    cmd->opts.omit_frame_pointer = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_function_sections)) {
                    cmd->opts.function_sections = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_data_sections)) {
                    cmd->opts.data_sections = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_gc_sections)) {
                    cmd->opts.gc_sections = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_whole_archive)) {
                    cmd->opts.whole_archive = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_unroll_loops)) {
                    cmd->opts.unroll_loops = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_unwind_tables)) {
                    cmd->opts.unwind_tables = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_async_unwind_tables)) {
                    cmd->opts.async_unwind_tables = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_strip)) {
                    cmd->opts.strip_mode = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_icf)) {
                    cmd->opts.icf_mode = dal_c_IcfMode_all;
                } else if (str_eql(opt, dal_c_opt_loose_errors)) {
                    cmd->opts.loose_errors = true;
                } else if (str_eql(opt, dal_c_opt_entry)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.entry_symbol, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_static)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_static;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.linking = dal_c_Linking_static;
                    }
                } else if (str_eql(opt, dal_c_opt_shared)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_shared;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.linking = dal_c_Linking_shared;
                    }
                } else if (str_eql(opt, dal_c_opt_debug)) {
                    if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.debug = true;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.debug = true;
                    }
                } else if (str_eql(opt, dal_c_opt_cache)) {
                    if (cmd->action == dal_c_CmdAction_clean || cmd->action == dal_c_CmdAction_clean_dsl) {
                        cmd->payload.clean.cache_only = true;
                    }
                } else if (str_eql(opt, dal_c_opt_recur)) {
                    if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.recursive = true;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.recursive = true;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.recursive = true;
                    }
                } else if (str_eql(opt, dal_c_opt_dsl)) {
                    if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.dsl_first = true;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.dsl_first = true;
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.dsl_first = true;
                    }
                } else if (str_eql(opt, dal_c_opt_sample)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_samples;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_samples;
                    }
                } else if (str_eql(opt, dal_c_opt_example)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_examples;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_examples;
                    }
                } else if (str_eql(opt, dal_c_opt_test)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_tests;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_tests;
                    }
                } else if (str_eql(opt, dal_c_opt_all)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.build_all = true;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.build_all = true;
                    }
                }
            }
        } else if (arg[0] == dal_c_opt_prefix_short[0] && arg[1] != dal_c_opt_prefix_short[0]) {
            char c = arg[1];
            const char* value = arg + 2;

            if (c == dal_c_opt_include_short_char) {
                if (!path_exists(value)) {
                    (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                    return 1;
                }
                dal_c_Cmd__addToArray(&cmd->opts.include_paths, &cmd->opts.include_count, value);
            } else if (c == dal_c_opt_link_short_char) {
                dal_c_Cmd__addToArray(&cmd->opts.link_libs, &cmd->opts.link_count, value);
            } else if (c == dal_c_opt_define_short_char) {
                dal_c_Cmd__addToArray(&cmd->opts.define_macros, &cmd->opts.define_count, value);
            } else if (c == dal_c_opt_undef_short_char) {
                dal_c_Cmd__addToArray(&cmd->opts.undef_macros, &cmd->opts.undef_count, value);
            } else if (c == dal_c_opt_output_short_char) {
                if (!value || value[0] == '\0') {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    value = argv[++i];
                }
                dal_c_Cmd__setOutputPath(cmd, value);
            }
        } else if (str_eql(arg, dal_c_opt_all_alias) || str_eql(arg, dal_c_opt_all)) {
            if (cmd->action == dal_c_CmdAction_build) {
                cmd->payload.build.build_all = true;
            } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                cmd->payload.test.build_all = true;
            }
        } else if (cmd->action == dal_c_CmdAction_build || cmd->action == dal_c_CmdAction_lib || cmd->action == dal_c_CmdAction_run || cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
            bool allow_bare = true;
            bool bare_name = (strchr(arg, '/') == NULL && strchr(arg, '\\') == NULL);
            char* abs_path = path_abs(arg);
            if (!abs_path || (!path_isFile(abs_path) && !path_isDir(abs_path))) {
                if (allow_bare && bare_name) {
                    char* bare = strdup(arg);
                    dal_c_Cmd__pushOwnedString(&cmd->input_files, &cmd->input_count, bare);
                    dal_c_Cmd__setPrimaryTargetPath(cmd, bare);
                    free(abs_path);
                } else {
                    free(abs_path);
                    (void)fprintf(stderr, "Error: Path not found: %s\n", arg);
                    return 1;
                }
            } else {
                dal_c_Cmd__pushOwnedString(&cmd->input_files, &cmd->input_count, abs_path);
                dal_c_Cmd__setPrimaryTargetPath(cmd, abs_path);
            }
        } else if (cmd->action == dal_c_CmdAction_workspace || cmd->action == dal_c_CmdAction_project) {
            if (cmd->action == dal_c_CmdAction_workspace) {
                cmd->payload.workspace.name = arg;
            } else {
                cmd->payload.project.name = arg;
            }
        }
    }

    return 0;
}

static ArrStr* dal_c_Cmd__resolveInputs(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* dir_name, bool fallback_all) {
    assert(proj != NULL);
    assert(dir_name != NULL);

    ArrStr* files = ArrStr_init();
    if (cmd->input_count == 0 && fallback_all) {
        ArrStr_fini(&files);
        return dal_c__collectDirectoryFiles(proj, dir_name);
    }

    char* dir_path = dal_c_Project_getCategoryDir(proj, dir_name);
    for (int i = 0; i < cmd->input_count; ++i) {
        const char* input = cmd->input_files[i];
        char* input_name = path_basename(input);
        char* candidate = path_join(dir_path, input_name);
        free(input_name);
        if (path_isFile(candidate)) {
            ArrStr_push(files, candidate);
        } else if (path_isFile(input)) {
            ArrStr_push(files, input);
        } else {
            (void)fprintf(stderr, "Error: File not found: %s\n", input);
        }
        free(candidate);
    }
    free(dir_path);
    return files;
}

static bool dal_c_Cmd__inputsNeedCategoryResolution(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    for (int i = 0; i < cmd->input_count; ++i) {
        const char* input = cmd->input_files[i];
        if (!path_isFile(input) && !path_isDir(input)) {
            return true;
        }
    }
    return false;
}

static ArrStr* dal_c_Cmd__collectPathSources(const char* path, bool resolved_is_dir, bool skip_auto_paths) {
    assert(path != NULL);

    ArrStr* files = ArrStr_init();
    if (!resolved_is_dir) {
        ArrStr_push(files, path);
        return files;
    }
    if (!path_isDir(path)) {
        return files;
    }

    int file_count = 0;
    char** entries = dir_listRecur(path, &file_count);
    if (!entries) {
        return files;
    }
    for (int i = 0; i < file_count; ++i) {
        char* relative_path = NULL;
        if (skip_auto_paths) {
            relative_path = path_relative(path, entries[i]);
            const char* candidate_path = relative_path ? relative_path : entries[i];
            if (dal_c__shouldSkipAutoSourcePath(candidate_path)) {
                free(relative_path);
                free(entries[i]);
                continue;
            }
        }
        if (dal_c_Cmd__isBuildSource(entries[i])) {
            ArrStr_push(files, entries[i]);
        }
        free(relative_path);
        free(entries[i]);
    }
    free((void*)entries);
    return files;
}

static ArrStr* dal_c_Cmd__collectExplicitSources(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    ArrStr* sources = ArrStr_init();
    for (int i = 0; i < cmd->input_count; ++i) {
        const char* input = cmd->input_files[i];
        if (path_isDir(input)) {
            ArrStr* dir_sources = dal_c_Cmd__collectPathSources(input, true, false);
            for (int j = 0; j < ArrStr_len(dir_sources); ++j) {
                ArrStr_push(sources, ArrStr_at(dir_sources, j));
            }
            ArrStr_fini(&dir_sources);
            continue;
        }
        ArrStr_push(sources, input);
    }
    return sources;
}

static ArrStr* dal_c_Cmd__collectTargetSources(const dal_c_TargetRequest* request) {
    assert(request != NULL);
    if (!request->resolved_path) {
        return ArrStr_init();
    }
    return dal_c_Cmd__collectPathSources(request->resolved_path, request->resolved_is_dir, request->raw_target_path == NULL);
}

static int dal_c_Cmd__pushExcludePath(dal_c_Cmd* cmd, const char* value) {
    assert(cmd != NULL);
    assert(value != NULL);

    char* abs_path = path_abs(value);
    if (!abs_path || (!path_isFile(abs_path) && !path_isDir(abs_path))) {
        free(abs_path);
        (void)fprintf(stderr, "Error: Exclude path not found: %s\n", value);
        return 1;
    }
    dal_c_Cmd__pushOwnedString(&cmd->exclude_paths, &cmd->exclude_count, abs_path);
    return 0;
}

static bool dal_c_Cmd__pathMatchesExclude(const char* path, const char* exclude_path) {
    assert(path != NULL);
    assert(exclude_path != NULL);

    if (str_eql(path, exclude_path)) {
        return true;
    }
    char* rel = path_relative(exclude_path, path);
    bool matches = rel != NULL;
    free(rel);
    return matches;
}

static const char* dal_c_Cmd__findMatchingExclude(const char* path, const ArrStr* excludes) {
    assert(path != NULL);
    assert(excludes != NULL);

    for (int i = 0; i < ArrStr_len(excludes); ++i) {
        const char* exclude_path = ArrStr_at(excludes, i);
        if (dal_c_Cmd__pathMatchesExclude(path, exclude_path)) {
            return exclude_path;
        }
    }
    return NULL;
}

static int dal_c_Cmd__rejectExcludedPath(const char* path, const ArrStr* excludes) {
    assert(path != NULL);
    assert(excludes != NULL);

    const char* matched_exclude = dal_c_Cmd__findMatchingExclude(path, excludes);
    if (!matched_exclude) {
        return 0;
    }
    (void)fprintf(
        stderr,
        "Error: Explicit input conflicts with exclude contract: %s (excluded by %s)\n",
        path,
        matched_exclude
    );
    return 1;
}

static ArrStr* dal_c_Cmd__collectActiveExcludes(const dal_c_Project* proj, const dal_c_TargetRequest* request, const dal_c_Cmd* cmd) {
    ArrStr* excludes = ArrStr_init();
    if (proj) {
        for (int i = 0; i < proj->exclude_count; ++i) {
            ArrStr_push(excludes, proj->exclude_paths[i]);
        }
    }
    if (request && request->root) {
        for (int i = 0; i < request->root->exclude_count; ++i) {
            ArrStr_push(excludes, request->root->exclude_paths[i]);
        }
    }
    if (cmd) {
        for (int i = 0; i < cmd->exclude_count; ++i) {
            ArrStr_push(excludes, cmd->exclude_paths[i]);
        }
    }
    return excludes;
}

static int dal_c_Cmd__applyExcludeContract(ArrStr** sources, const ArrStr* excludes, bool explicit_selection) {
    assert(sources != NULL);
    assert(*sources != NULL);
    assert(excludes != NULL);

    if (ArrStr_len(*sources) == 0 || ArrStr_len(excludes) == 0) {
        return 0;
    }

    ArrStr* filtered = ArrStr_init();
    for (int i = 0; i < ArrStr_len(*sources); ++i) {
        const char* src = ArrStr_at(*sources, i);
        const char* matched_exclude = dal_c_Cmd__findMatchingExclude(src, excludes);
        if (!matched_exclude) {
            ArrStr_push(filtered, src);
            continue;
        }
        if (explicit_selection) {
            ArrStr_fini(&filtered);
            (void)fprintf(
                stderr,
                "Error: Explicit input conflicts with exclude contract: %s (excluded by %s)\n",
                src,
                matched_exclude
            );
            return 1;
        }
    }

    ArrStr_fini(sources);
    *sources = filtered;
    return 0;
}

static char* dal_c_Cmd__basenameNoExt(const char* path) {
    char* base = path_basename(path);
    char* dot = strrchr(base, '.');
    if (dot && dot != base) { *dot = '\0'; }
    return base;
}

static void dal_c_Cmd__collectCompanionDhFiles(ArrStr* dh_files, ArrStr* sources) {
    assert(dh_files != NULL);
    assert(sources != NULL);

    for (int i = 0; i < ArrStr_len(sources); ++i) {
        const char* src = ArrStr_at(sources, i);
        if (!dal_c_Cmd__isSourceOrHeader(src)) { continue; }
        char* no_ext = strdup(src);
        char* dot = strrchr(no_ext, '.');
        if (dot) { *dot = '\0'; }
        char* dh_file = str_format("%s.dh", no_ext);
        free(no_ext);
        if (dh_file && path_isFile(dh_file)) {
            ArrStr_push(dh_files, dh_file);
        }
        free(dh_file);
    }
}

static void dal_c_Cmd__mergeBuildProperties(dal_c_CompilerOpts* opts, dal_c_BuildDefaults* defaults, const dal_c_Project* proj, ArrStr* sources, const dal_c_Cmd* cmd) {
    assert(opts != NULL);
    assert(cmd != NULL);

    if (proj) {
        dal_c_CompilerOpts_merge(opts, &proj->opts);
        if (defaults) {
            dal_c_BuildDefaults_merge(defaults, &proj->defaults);
        }
    }

    ArrStr* dh_files = ArrStr_init();
    dal_c_Cmd__collectCompanionDhFiles(dh_files, sources);
    for (int i = 0; i < ArrStr_len(dh_files); ++i) {
        (void)dal_c_CompilerOpts_applyDhFile(opts, ArrStr_at(dh_files, i));
        if (defaults) {
            (void)dal_c_BuildDefaults_applyDhFile(defaults, ArrStr_at(dh_files, i));
        }
    }
    ArrStr_fini(&dh_files);

    for (int i = 0; i < cmd->explicit_dh_count; ++i) {
        (void)dal_c_CompilerOpts_applyDhFile(opts, cmd->explicit_dh_files[i]);
        if (defaults) {
            (void)dal_c_BuildDefaults_applyDhFile(defaults, cmd->explicit_dh_files[i]);
        }
    }
    dal_c_CompilerOpts_merge(opts, &cmd->opts);
}

static bool dal_c_Cmd__hasExplicitVersionFlags(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    const dal_c_VersionSpec* version = &cmd->opts.version;
    return version->core_set
        || version->label_prefix_set
        || version->label_suffix_set
        || version->build_set;
}

static char* dal_c_Cmd__versionRecordPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources) {
    assert(cmd != NULL);
    switch (cmd->version_record_mode) {
    case dal_c_VersionRecordMode_project:
        if (!proj || !proj->root) {
            (void)fprintf(stderr, "Error: `--%s=project` requires a detected project root\n", dal_c_opt_version_record);
            return NULL;
        }
        return proj->project_dh ? strdup(proj->project_dh) : path_join(proj->root, dal_c_file_detector_project);
    case dal_c_VersionRecordMode_companion: {
        if (!sources || ArrStr_len(sources) != 1) {
            (void)fprintf(stderr, "Error: `--%s=companion` requires exactly one source file\n", dal_c_opt_version_record);
            return NULL;
        }
        const char* src = ArrStr_at(sources, 0);
        char* no_ext = strdup(src);
        if (!no_ext) {
            return NULL;
        }
        char* dot = strrchr(no_ext, '.');
        if (dot) {
            *dot = '\0';
        }
        char* dh_path = str_format("%s.dh", no_ext);
        free(no_ext);
        return dh_path;
    }
    case dal_c_VersionRecordMode_none:
    case dal_c_VersionRecordMode_invalid:
    default:
        return NULL;
    }
}

static int dal_c_Cmd__writeVersionDhFile(const char* path, const dal_c_VersionSpec* version) {
    assert(path != NULL);
    assert(version != NULL);

    int line_count = 0;
    char** lines = path_isFile(path) ? file_readLines(path, &line_count) : NULL;
    bool wrote_core = false;
    bool wrote_prefix = false;
    bool wrote_suffix = false;
    bool wrote_build = false;
    char* core_value = version->core_set
                         ? str_format("%u.%u.%u", version->core_major, version->core_minor, version->core_patch)
                         : NULL;
    char* suffix_value = version->label_suffix_set
                           ? str_format("%u", version->label_suffix_num)
                           : NULL;
    char* content = strdup("");

    if (!content) {
        for (int i = 0; i < line_count; ++i) {
            free(lines[i]);
        }
        free((void*)lines);
        free(core_value);
        free(suffix_value);
        return 1;
    }

    for (int i = 0; i < line_count; ++i) {
        const char* preserved = lines[i] ? lines[i] : "";
        char* scratch = strdup(preserved);
        bool replaced = false;

        if (!scratch) {
            free(content);
            for (int j = 0; j < line_count; ++j) {
                free(lines[j]);
            }
            free((void*)lines);
            free(core_value);
            free(suffix_value);
            return 1;
        }

        char* trimmed = str_trim(scratch);
        if (trimmed[0] != '\0' && trimmed[0] != '#' && trimmed[0] != ';' && trimmed[0] != '[') {
            char* eq = strchr(trimmed, '=');
            if (eq) {
                *eq = '\0';
                char* key = str_trim(trimmed);
                const char* replacement_key = NULL;
                const char* replacement_value = NULL;

                if (version->core_set && str_eql(key, dal_c_project_prop_version_core) && !wrote_core) {
                    replacement_key = dal_c_project_prop_version_core;
                    replacement_value = core_value;
                    wrote_core = true;
                    replaced = true;
                } else if (version->label_prefix_set && str_eql(key, dal_c_project_prop_version_prefix) && !wrote_prefix) {
                    replacement_key = dal_c_project_prop_version_prefix;
                    replacement_value = version->label_prefix_str;
                    wrote_prefix = true;
                    replaced = true;
                } else if (version->label_suffix_set && str_eql(key, dal_c_project_prop_version_suffix) && !wrote_suffix) {
                    replacement_key = dal_c_project_prop_version_suffix;
                    replacement_value = suffix_value;
                    wrote_suffix = true;
                    replaced = true;
                } else if (version->build_set && str_eql(key, dal_c_project_prop_version_build) && !wrote_build) {
                    replacement_key = dal_c_project_prop_version_build;
                    replacement_value = version->build_str;
                    wrote_build = true;
                    replaced = true;
                }

                if (replaced) {
                    char* next = str_format("%s%s=%s\n", content, replacement_key, replacement_value ? replacement_value : "");
                    free(content);
                    content = next;
                    free(scratch);
                    continue;
                }
            }
        }

        char* next = str_format("%s%s\n", content, preserved);
        free(content);
        content = next;
        free(scratch);
    }

    if (version->core_set && !wrote_core) {
        char* next = str_format("%s%s=%s\n", content, dal_c_project_prop_version_core, core_value ? core_value : "");
        free(content);
        content = next;
    }
    if (version->label_prefix_set && !wrote_prefix) {
        char* next = str_format("%s%s=%s\n", content, dal_c_project_prop_version_prefix, version->label_prefix_str ? version->label_prefix_str : "");
        free(content);
        content = next;
    }
    if (version->label_suffix_set && !wrote_suffix) {
        char* next = str_format("%s%s=%s\n", content, dal_c_project_prop_version_suffix, suffix_value ? suffix_value : "");
        free(content);
        content = next;
    }
    if (version->build_set && !wrote_build) {
        char* next = str_format("%s%s=%s\n", content, dal_c_project_prop_version_build, version->build_str ? version->build_str : "");
        free(content);
        content = next;
    }

    bool ok = content != NULL && file_write(path, content);
    free(content);
    free(core_value);
    free(suffix_value);
    for (int i = 0; i < line_count; ++i) {
        free(lines[i]);
    }
    free((void*)lines);
    return ok ? 0 : 1;
}

static int dal_c_Cmd__recordVersionFlags(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources) {
    assert(cmd != NULL);
    if (cmd->version_record_mode == dal_c_VersionRecordMode_none || !dal_c_Cmd__hasExplicitVersionFlags(cmd)) {
        return 0;
    }
    char* path = dal_c_Cmd__versionRecordPath(cmd, proj, sources);
    if (!path) {
        return 1;
    }
    int result = dal_c_Cmd__writeVersionDhFile(path, &cmd->opts.version);
    free(path);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__ensureProjectStaticLibrary(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);
    if (!proj || !proj->root || !proj->name) {
        return 0;
    }

    ArrStr* project_sources = dal_c__collectSourceFiles(proj, NULL);
    if (ArrStr_len(project_sources) == 0) {
        ArrStr_fini(&project_sources);
        return 0;
    }

    dal_c_Cmd lib_cmd = *self;
    memset(&lib_cmd.payload, 0, sizeof(lib_cmd.payload));
    lib_cmd.action = dal_c_CmdAction_lib;
    lib_cmd.payload.lib.linking = dal_c_Linking_static;
    int result = dal_c_Cmd__buildFromSources(
        &lib_cmd,
        proj,
        project_sources,
        proj->name,
        dal_c_Target_static_lib,
        NULL,
        false,
        false
    );
    ArrStr_fini(&project_sources);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__runBuildDefaultTests(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Profile profile) {
    assert(self != NULL);
    assert(proj != NULL);

    dal_c_Cmd test_cmd = *self;
    test_cmd.action = dal_c_CmdAction_test;
    memset(&test_cmd.payload, 0, sizeof(test_cmd.payload));
    test_cmd.opts.profile = profile;
    test_cmd.input_files = NULL;
    test_cmd.input_count = 0;
    test_cmd.payload.test.dsl_first = self->action == dal_c_CmdAction_build
                                        ? self->payload.build.dsl_first
                                        : false;
    return dal_c_Cmd_makeTarget(&test_cmd, proj);
}

static int dal_c_Cmd__buildLibrarySetFromSources(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* output_name,
    bool allow_output_defaults
) {
    assert(self != NULL);
    assert(proj != NULL);
    assert(sources != NULL);
    assert(output_name != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    if (intent.output_path) {
        (void)fprintf(stderr, "Error: `kind=lib` cannot use one explicit output path for both static and shared libraries\n");
        return 1;
    }

    int result = dal_c_Cmd__buildFromSources(
        self,
        proj,
        sources,
        output_name,
        dal_c_Target_static_lib,
        NULL,
        allow_output_defaults,
        false
    );
    if (result != 0) { return result; }

    result = dal_c_Cmd__buildFromSources(
        self,
        proj,
        sources,
        output_name,
        dal_c_Target_shared_lib,
        NULL,
        allow_output_defaults,
        false
    );
    if (result != 0) { return result; }

    printf("Build successful!\n");
    return 0;
}

bool dal_c__hasTestRegistration(const char* path) {
    assert(path != NULL);

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    bool has_registration = false;
    for (int i = 0; i < line_count; ++i) {
        if (strstr(lines[i], "TEST_fn_(") != NULL || strstr(lines[i], "TEST_Framework_bindCase(") != NULL) {
            has_registration = true;
            break;
        }
    }

    for (int i = 0; i < line_count; ++i) {
        free(lines[i]);
    }
    free((void*)lines);
    return has_registration;
}

static char* dal_c_Cmd__mergeCompilerArgs(const char* base, const char* extra) {
    if (!base || base[0] == '\0') {
        return (!extra || extra[0] == '\0') ? NULL : strdup(extra);
    }
    if (!extra || extra[0] == '\0') {
        return strdup(base);
    }
    return str_format("%s %s", base, extra);
}

static bool dal_c_Cmd__arrHasString(const ArrStr* arr, const char* value) {
    assert(arr != NULL);
    assert(value != NULL);

    for (int i = 0; i < ArrStr_len(arr); ++i) {
        if (str_eql(ArrStr_at(arr, i), value)) {
            return true;
        }
    }
    return false;
}

static void dal_c_Cmd__appendUniqueString(ArrStr* arr, const char* value) {
    assert(arr != NULL);
    if (!value || value[0] == '\0' || dal_c_Cmd__arrHasString(arr, value)) {
        return;
    }
    ArrStr_push(arr, value);
}

static void dal_c_Cmd__appendProjectTestIncludeDirs(const dal_c_Project* proj, ArrStr* include_dirs) {
    assert(proj != NULL);
    assert(include_dirs != NULL);

    if (proj->root && path_isDir(proj->root)) {
        dal_c_Cmd__appendUniqueString(include_dirs, proj->root);
    }

    char* include_dir = dal_c_Project_getIncludeDir(proj);
    if (include_dir && path_isDir(include_dir)) {
        dal_c_Cmd__appendUniqueString(include_dirs, include_dir);
    }
    free(include_dir);

    char* tests_dir = dal_c_Project_getTestsDir(proj);
    if (tests_dir && path_isDir(tests_dir)) {
        dal_c_Cmd__appendUniqueString(include_dirs, tests_dir);
    }
    free(tests_dir);
}

static void dal_c_Cmd__collectRecursiveTestInputs(
    const dal_c_Project* proj,
    const dal_c_Cmd* cmd,
    bool recurse_dependencies,
    ArrStr* visited_roots,
    ArrStr* tests,
    ArrStr* include_dirs
) {
    assert(proj != NULL);
    assert(cmd != NULL);
    assert(visited_roots != NULL);
    assert(tests != NULL);
    assert(include_dirs != NULL);

    if (!proj->root || dal_c_Cmd__arrHasString(visited_roots, proj->root)) {
        return;
    }
    ArrStr_push(visited_roots, proj->root);

    if (recurse_dependencies) {
        for (int i = 0; i < proj->lib_count; ++i) {
            const dal_c_Lib* lib = &proj->libraries[i];
            if (!lib->path) {
                continue;
            }
            dal_c_Project* lib_proj = dal_c_Project_detectAt(lib->path, proj->dh_path);
            if (!lib_proj) {
                continue;
            }
            dal_c_Cmd__collectRecursiveTestInputs(lib_proj, cmd, true, visited_roots, tests, include_dirs);
            dal_c_Project_cleanup(&lib_proj);
        }
    }

    dal_c_Cmd__appendProjectTestIncludeDirs(proj, include_dirs);
    ArrStr* project_tests = dal_c_Cmd__resolveInputs(proj, cmd, dal_c_dir_tests, true);
    for (int i = 0; i < ArrStr_len(project_tests); ++i) {
        dal_c_Cmd__appendUniqueString(tests, ArrStr_at(project_tests, i));
    }
    ArrStr_fini(&project_tests);
}

static char* dal_c_Cmd__buildIncludeArgs(const ArrStr* include_dirs) {
    assert(include_dirs != NULL);

    char* args = NULL;
    for (int i = 0; i < ArrStr_len(include_dirs); ++i) {
        const char* include_dir = ArrStr_at(include_dirs, i);
        char* next = args ? str_format("%s -I%s", args, include_dir) : str_format("-I%s", include_dir);
        free(args);
        args = next;
    }
    return args;
}

static bool dal_c_Cmd__writeFileIfChanged(const char* path, const char* content) {
    assert(path != NULL);
    assert(content != NULL);

    char* existing = file_read(path);
    if (existing && str_eql(existing, content)) {
        free(existing);
        return true;
    }
    free(existing);
    return file_write(path, content);
}

static char* dal_c_Cmd__generatedSourceDir(const dal_c_Project* proj, const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    char* cache_dir = dal_c__cacheBaseDir(proj);
    char* dsl_dir = path_join(cache_dir, dal_c_tool_name);
    free(cache_dir);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* profile_dir = path_join(dsl_dir, profile->name);
    free(dsl_dir);
    return profile_dir;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__buildFromSources(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* output_name,
    dal_c_Target target_type,
    const char* extra_compiler_args,
    bool allow_output_defaults,
    bool print_success
) {
    assert(self != NULL);
    assert(proj != NULL);
    assert(sources != NULL);
    assert(output_name != NULL);

    if ((target_type == dal_c_Target_preprocessed || target_type == dal_c_Target_assembly)
        && ArrStr_len(sources) != 1) {
        (void)fprintf(stderr, "Error: `%s` export requires exactly one source file\n", dal_c_Target_format(target_type));
        return 1;
    }

    char* compiler_args = dal_c_Cmd__mergeCompilerArgs(self->compiler_args, extra_compiler_args);
    dal_c_Cmd effective = *self;
    dal_c_BuildDefaults effective_defaults = { 0 };
    effective.compiler_args = compiler_args;
    memset(&effective.opts, 0, sizeof(effective.opts));
    effective.opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&effective.opts, &effective_defaults, proj, sources, self);
    if (effective.opts.version.label_suffix_set && !effective.opts.version.label_prefix_set) {
        (void)fprintf(stderr, "Error: `%s` requires `%s`\n", dal_c_opt_version_suffix, dal_c_opt_version_prefix);
        dal_c_CompilerOpts_cleanup(&effective.opts);
        dal_c_BuildDefaults_cleanup(&effective_defaults);
        free(compiler_args);
        return 1;
    }
    if (dal_c_Cmd__recordVersionFlags(self, proj, sources) != 0) {
        dal_c_CompilerOpts_cleanup(&effective.opts);
        dal_c_BuildDefaults_cleanup(&effective_defaults);
        free(compiler_args);
        return 1;
    }
    if (self->action == dal_c_CmdAction_test && self->payload.test.dsl_first) {
        effective.opts.dsl_mode = dal_c_ToggleState_enabled;
    }
    if (effective.opts.profile == dal_c_Profile_invalid) {
        effective.opts.profile = self->opts.profile;
    }

    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(effective.opts.profile);
    char* base_build_dir = NULL;
    if (proj && proj->root) {
        base_build_dir = dal_c_Project_getBuildDir(proj);
    } else {
        char* cwd = env_getCWD();
        base_build_dir = cwd ? path_join(cwd, dal_c_dir_build) : strdup(dal_c_dir_build);
        free(cwd);
    }
    char* profile_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    char* object_dir = path_join(profile_dir, "obj");
    dir_createRecur(object_dir);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    const char* resolved_output_name = output_name;
    if (allow_output_defaults && !intent.output_path && effective_defaults.output_name) {
        resolved_output_name = effective_defaults.output_name;
    }
    char* target_path = dal_c__resolveOutputPath(proj, self, profile_dir, resolved_output_name, target_type);
    char* makefile_path = dal_c__makePlanFilePath(proj, profile, &effective, target_path, target_type);
    if (dal_c__generateMakefile(&effective, proj, profile, sources, target_path, object_dir, target_type) != 0) {
        (void)fprintf(stderr, "Error: Failed to generate Makefile\n");
        dal_c_CompilerOpts_cleanup(&effective.opts);
        dal_c_BuildDefaults_cleanup(&effective_defaults);
        free(makefile_path);
        free(object_dir);
        free(compiler_args);
        free(target_path);
        free(profile_dir);
        return 1;
    }

    int result = dal_c__executeMake(makefile_path);
    free(makefile_path);
    free(object_dir);
    free(profile_dir);
    if (result != 0) {
        dal_c_CompilerOpts_cleanup(&effective.opts);
        dal_c_BuildDefaults_cleanup(&effective_defaults);
        free(compiler_args);
        free(target_path);
        (void)fprintf(stderr, "Error: Build failed\n");
        return result;
    }
    if (print_success) {
        printf("Build successful!\n");
    }

    if (self->action == dal_c_CmdAction_build
        && target_type == dal_c_Target_executable
        && !intent.target_root_name_hint
        && !intent.target_path
        && effective_defaults.build_runs_tests_set
        && effective_defaults.build_runs_tests) {
        result = dal_c_Cmd__runBuildDefaultTests(self, proj, effective.opts.profile);
        if (result != 0) {
            dal_c_CompilerOpts_cleanup(&effective.opts);
            dal_c_BuildDefaults_cleanup(&effective_defaults);
            free(compiler_args);
            free(target_path);
            return result;
        }
    }

    if (self->action == dal_c_CmdAction_run
        || self->action == dal_c_CmdAction_test
        || self->action == dal_c_CmdAction_test_dsl) {
        dal_c_Cmd runtime_cmd = *self;
        runtime_cmd.opts.profile = effective.opts.profile;
        switch (runtime_cmd.action) {
        case dal_c_CmdAction_build:
            runtime_cmd.payload.build.output_path = target_path;
            break;
        case dal_c_CmdAction_run:
            runtime_cmd.payload.run.output_path = target_path;
            break;
        case dal_c_CmdAction_test:
        case dal_c_CmdAction_test_dsl:
            runtime_cmd.payload.test.output_path = target_path;
            break;
        case dal_c_CmdAction_lib:
        case dal_c_CmdAction_deps:
        case dal_c_CmdAction_clean:
        case dal_c_CmdAction_build_dsl:
        case dal_c_CmdAction_clean_dsl:
        case dal_c_CmdAction_build_self:
        case dal_c_CmdAction_clean_self:
        case dal_c_CmdAction_workspace:
        case dal_c_CmdAction_project:
        case dal_c_CmdAction_help:
        case dal_c_CmdAction_version:
        case dal_c_CmdAction_invalid:
        default:
            break;
        }
        if (intent.debug) {
            result = dal_c__runDebugger(&runtime_cmd, proj);
        } else {
            result = dal_c__runExecutable(&runtime_cmd, proj);
        }
    }

    dal_c_CompilerOpts_cleanup(&effective.opts);
    dal_c_BuildDefaults_cleanup(&effective_defaults);
    free(compiler_args);
    free(target_path);
    return result;
}

static char* dal_c_Cmd__writeTestMainSource(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* output_name) {
    assert(cmd != NULL);
    assert(output_name != NULL);

    char* profile_dir = dal_c_Cmd__generatedSourceDir(proj, cmd);
    char* runner_name = str_format("%s.main.c", output_name);
    char* runner_path = path_join(profile_dir, runner_name);
    free(runner_name);

    if (!dir_createRecur(profile_dir)) {
        free(profile_dir);
        free(runner_path);
        return NULL;
    }
    free(profile_dir);

    const char* content = "/* generated by " dal_c_tool_name " */\n"
                          "#define TEST_main_enabled 1\n"
                          "#include \"dh-TEST-main.h\"\n";
    if (!dal_c_Cmd__writeFileIfChanged(runner_path, content)) {
        free(runner_path);
        return NULL;
    }
    return runner_path;
}

static char* dal_c_Cmd__writeUnitySource(const dal_c_Project* proj, const dal_c_Cmd* cmd, ArrStr* inputs, const char* output_name) {
    assert(cmd != NULL);
    assert(inputs != NULL);
    assert(output_name != NULL);

    char* profile_dir = dal_c_Cmd__generatedSourceDir(proj, cmd);
    char* unity_name = str_format("%s.unity.c", output_name);
    char* unity_path = path_join(profile_dir, unity_name);
    free(unity_name);

    if (!dir_createRecur(profile_dir)) {
        free(profile_dir);
        free(unity_path);
        return NULL;
    }
    free(profile_dir);

    ArrStr* lines = ArrStr_init();
    ArrStr_push(lines, "/* generated by " dal_c_tool_name " */");
    for (int i = 0; i < ArrStr_len(inputs); ++i) {
        char* normalized = strdup(ArrStr_at(inputs, i));
        for (char* p = normalized; *p; ++p) {
            if (*p == '\\') { *p = '/'; }
        }
        char* include_line = str_format("#include \"%s\"", normalized);
        free(normalized);
        ArrStr_push(lines, include_line);
        free(include_line);
    }

    char** raw_lines = ArrStr_toRaw(lines);
    const int raw_count = ArrStr_len(lines);
    const char** joined_lines = raw_count > 0 ? (const char**)malloc((size_t)raw_count * sizeof(char*)) : NULL;
    for (int i = 0; i < raw_count; ++i) {
        joined_lines[i] = raw_lines[i];
    }
    char* content = str_join("\n", joined_lines, raw_count);
    ArrStr_fini(&lines);
    if (raw_lines) {
        for (int i = 0; raw_lines[i] != NULL; ++i) {
            free(raw_lines[i]);
        }
        free((void*)raw_lines);
    }
    free((void*)joined_lines);
    bool ok = dal_c_Cmd__writeFileIfChanged(unity_path, content);
    free(content);
    if (!ok) {
        free(unity_path);
        return NULL;
    }
    return unity_path;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__runRecursiveBuild(const dal_c_Cmd* self, const dal_c_Project* proj) {
    ArrStr* child_roots = dal_c__collectDescendantProjects(proj);
    for (int i = 0; i < ArrStr_len(child_roots); ++i) {
        dal_c_Project* child_proj = dal_c_Project_detectAt(ArrStr_at(child_roots, i), proj->dh_path);
        if (!child_proj) { continue; }
        dal_c_Cmd child = *self;
        child.input_files = NULL;
        child.input_count = 0;
        child.explicit_dh_files = NULL;
        child.explicit_dh_count = 0;
        child.payload.build.recursive = false;
        child.payload.build.dsl_first = false;
        child.payload.build.target_path = NULL;
        child.payload.build.output_path = NULL;
        int result = dal_c_Cmd_makeTarget(&child, child_proj);
        dal_c_Project_cleanup(&child_proj);
        if (result != 0) {
            ArrStr_fini(&child_roots);
            return result;
        }
    }
    ArrStr_fini(&child_roots);
    return 0;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__runRecursiveTest(const dal_c_Cmd* self, const dal_c_Project* proj) {
    ArrStr* child_roots = dal_c__collectDescendantProjects(proj);
    for (int i = 0; i < ArrStr_len(child_roots); ++i) {
        dal_c_Project* child_proj = dal_c_Project_detectAt(ArrStr_at(child_roots, i), proj->dh_path);
        if (!child_proj) { continue; }
        if (self->payload.test.dsl_first && child_proj->dh_path && proj->dh_path
            && str_eql(child_proj->dh_path, proj->dh_path)) {
            dal_c_Project_cleanup(&child_proj);
            continue;
        }

        dal_c_Cmd child = *self;
        child.input_files = NULL;
        child.input_count = 0;
        child.explicit_dh_files = NULL;
        child.explicit_dh_count = 0;
        child.payload.test.recursive = false;
        child.payload.test.target_path = NULL;
        child.payload.test.output_path = NULL;
        int result = dal_c_Cmd_makeTarget(&child, child_proj);
        dal_c_Project_cleanup(&child_proj);
        if (result != 0) {
            ArrStr_fini(&child_roots);
            return result;
        }
    }
    ArrStr_fini(&child_roots);
    return 0;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__runRecursiveClean(const dal_c_Cmd* self, const dal_c_Project* proj) {
    ArrStr* child_roots = dal_c__collectDescendantProjects(proj);
    for (int i = 0; i < ArrStr_len(child_roots); ++i) {
        dal_c_Project* child_proj = dal_c_Project_detectAt(ArrStr_at(child_roots, i), proj->dh_path);
        if (!child_proj) { continue; }
        dal_c_Cmd child = *self;
        child.input_files = NULL;
        child.input_count = 0;
        child.explicit_dh_files = NULL;
        child.explicit_dh_count = 0;
        child.payload.clean.recursive = false;
        child.payload.clean.dsl_first = false;
        int result = dal_c_Cmd_cleanTarget(&child, child_proj);
        dal_c_Project_cleanup(&child_proj);
        if (result != 0) {
            ArrStr_fini(&child_roots);
            return result;
        }
    }
    ArrStr_fini(&child_roots);
    return 0;
}
