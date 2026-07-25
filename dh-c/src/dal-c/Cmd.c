#include "internal.h"
#include "dal-c-ext/ArrStr.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/proc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#endif
/* Supported: single file build/run/test; --sample/--example/--test with bare filename
 * (e.g. dh-c build --sample target.c resolves to samples/target.c); aggregate test builds
 * use a generated runner TU plus separate test translation units. */

// === PRIVATE HELPERS ===

typedef struct dal_c_Cmd__ElapsedPhases {
    double project_lib_build;
    double dependency_build;
    double dh_build;
    double self_build;
    double test_build;
    double run_build;
    double test_run;
    double run_exec;
    double syntax;
    double tidy;
    double format;
    double compile_db;
    double clean;
} dal_c_Cmd__ElapsedPhases;

static void dal_c_Cmd__addToArray(char*** arr, int* count, const char* value);
static void dal_c_Cmd__pushOwnedString(char*** arr, int* count, char* value);
static void dal_c_Cmd__setOwnedString(char** slot, const char* value);
static bool dal_c_Cmd__isSourceOrHeader(const char* name);
static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action);
static bool dal_c_Cmd__usesBuildPayload(dal_c_CmdAction action);
static bool dal_c_Cmd__usesBuildArtifactPayload(dal_c_CmdAction action);
static bool dal_c_Cmd__tryParseBoolValue(const char* value, bool* out);
static bool dal_c_Cmd__tryParseToggleValue(const char* value, dal_c_ToggleState* out);
static bool dal_c_Cmd__tryParseElapsedPrecision(const char* value, int* out);
static int dal_c_Cmd__applyAssignedBooleanOption(dal_c_Cmd* cmd, const char* opt, size_t opt_len, const char* value, bool* handled);
static dal_c_Target dal_c_Cmd__resolveBuildTargetType(const dal_c_Cmd* cmd, dal_c_Linking linking, bool builds_library);
static void dal_c_Cmd__setStdlibBundle(dal_c_CompilerOpts* opts, bool linked);
static void dal_c_Cmd__setCrtStartFiles(dal_c_CompilerOpts* opts, bool linked);
static int dal_c_Cmd__parseOptions(dal_c_Cmd* cmd, int argc, const char* argv[], int start_idx);
static char** dal_c_Cmd__targetPathSlot(dal_c_Cmd* cmd);
static char** dal_c_Cmd__outputPathSlot(dal_c_Cmd* cmd);
static char** dal_c_Cmd__runArgsSlot(dal_c_Cmd* cmd);
static bool dal_c_Cmd__buildsLibrary(const dal_c_Cmd* cmd);
static bool dal_c_Cmd__hasExplicitFileInputs(const dal_c_Cmd* cmd);
static bool dal_c_Cmd__outputNamesLibraryArtifact(const char* path);
static double dal_c_Cmd__nowSeconds(void);
static bool dal_c_Cmd__reportsElapsed(const dal_c_Cmd* cmd);
static void dal_c_Cmd__reportElapsed(
    const dal_c_Cmd* cmd,
    int result,
    double elapsed_seconds,
    double lock_wait_seconds,
    int elapsed_precision,
    const dal_c_Cmd__ElapsedPhases* phases
);
static int dal_c_Cmd__validateCanonicalModifiers(const dal_c_Cmd* cmd);
static bool dal_c_Cmd__phasesHaveAny(const dal_c_Cmd__ElapsedPhases* phases);
static void dal_c_Cmd__setPrimaryTargetPath(dal_c_Cmd* cmd, const char* path);
static void dal_c_Cmd__setOutputPath(dal_c_Cmd* cmd, const char* path);
static const char* dal_c_Cmd__sampleDirCanonical(dal_c_SampleDir sample_dir);
static bool dal_c_Cmd__optionNameIs(const char* opt, const char* name);
static bool dal_c_Cmd__validOutputExt(const char* value);
static ArrStr* dal_c_Cmd__resolveInputs(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* dir_name, bool fallback_all);
static bool dal_c_Cmd__inputsNeedCategoryResolution(const dal_c_Cmd* cmd);
static ArrStr* dal_c_Cmd__collectPathSources(const char* path, bool resolved_is_dir, bool skip_auto_paths);
static ArrStr* dal_c_Cmd__collectExplicitSources(const dal_c_Cmd* cmd);
static ArrStr* dal_c_Cmd__collectTargetSources(const dal_c_Project* proj, const dal_c_TargetRequest* request);
static char* dal_c_Cmd__targetLocalSourceRoot(const dal_c_TargetRequest* request);
static char* dal_c_Cmd__targetLocalIncludeRoot(const dal_c_TargetRequest* request);
static char* dal_c_Cmd__targetLocalProjectDHPath(const dal_c_Project* proj, const dal_c_Cmd* cmd);
static int dal_c_Cmd__pushExcludePath(dal_c_Cmd* cmd, const char* value);
static bool dal_c_Cmd__pathMatchesExclude(const char* path, const char* exclude_path);
static const char* dal_c_Cmd__findMatchingExclude(const char* path, const ArrStr* excludes);
static int dal_c_Cmd__rejectExcludedPath(const char* path, const ArrStr* excludes);
static ArrStr* dal_c_Cmd__collectActiveExcludes(const dal_c_Project* proj, const dal_c_TargetRequest* request, const dal_c_Cmd* cmd);
static int dal_c_Cmd__applyExcludeContract(ArrStr** sources, const ArrStr* excludes, bool explicit_selection);
static char* dal_c_Cmd__basenameNoExt(const char* path);
static void dal_c_Cmd__collectCompanionDHFiles(ArrStr* dh_files, ArrStr* sources);
static void dal_c_Cmd__mergeBuildProperties(dal_c_CompilerOpts* opts, dal_c_BuildDefaults* defaults, const dal_c_Project* proj, ArrStr* sources, const dal_c_Cmd* cmd);
static bool dal_c_Cmd__hasExplicitVersionFlags(const dal_c_Cmd* cmd);
static int dal_c_Cmd__recordVersionFlags(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);
static char* dal_c_Cmd__versionRecordPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);
static int dal_c_Cmd__writeVersionDHFile(const char* path, const dal_c_VersionSpec* version);
static int dal_c_Cmd__ensureProjectStaticLibrary(const dal_c_Cmd* self, const dal_c_Project* proj);
static int dal_c_Cmd__runBuildDefaultTests(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Profile profile);
static char* dal_c_Cmd__resolveLibraryOutputName(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* fallback_output_name,
    bool allow_output_defaults
);

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
static int dal_c_Cmd__buildOneFromSources(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* output_name,
    dal_c_Target target_type,
    const char* extra_compiler_args,
    bool allow_output_defaults,
    bool print_success
);
static dal_c_LtoMode dal_c_Cmd__resolveLibraryLtoMode(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources
);
static void dal_c_Cmd__appendTargetLocalInclude(dal_c_CompilerOpts* opts, const dal_c_Project* proj, const dal_c_Cmd* cmd);
static char* dal_c_Cmd__compileDbOutputPath(const dal_c_Cmd* self, const dal_c_Project* proj);
typedef struct dal_c_Cmd__CheckPlan dal_c_Cmd__CheckPlan;
static int dal_c_Cmd__prepareCheckPlan(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Cmd__CheckPlan* plan);
static void dal_c_Cmd__cleanupCheckPlan(dal_c_Cmd__CheckPlan* plan);
static bool dal_c_Cmd__isAssemblySource(const char* path);
static int dal_c_Cmd__runSyntaxPlan(dal_c_Cmd__CheckPlan* plan);
static void dal_c_Cmd__freeRawArgv(char** raw);
static int dal_c_Cmd__runTidyPlan(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Cmd__CheckPlan* plan);
static int dal_c_Cmd__runFormatPlan(const dal_c_Cmd* self, dal_c_Cmd__CheckPlan* plan);
static bool dal_c_Cmd__toolAvailable(const char* tool);
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
static bool dal_c_Cmd__pathIsUnderRoot(const char* root, const char* path);
static int dal_c_Cmd__cleanDependencyExportsAt(const char* root, bool verbose, bool* cleaned);
static bool dal_c_Cmd__executeNeedsProjectLock(const dal_c_Cmd* self);
static int dal_c_Cmd__executeUnlocked(const dal_c_Cmd* self, const dal_c_Project* proj);
static int dal_c_Cmd__makeTargetUnlocked(const dal_c_Cmd* self, const dal_c_Project* proj);
static char* dal_c_Cmd__makeProjectLockPath(const char* root);

// === PUBLIC API ===

typedef struct dal_c_Cmd__HeldProjectLock {
    char* path;
    file_Lock lock;
    int ref_count;
} dal_c_Cmd__HeldProjectLock;

static dal_c_Cmd__HeldProjectLock* dal_c_Cmd__held_locks = NULL;
static int dal_c_Cmd__held_lock_count = 0;
static double dal_c_Cmd__elapsed_lock_wait_seconds = 0.0;
static dal_c_Cmd__ElapsedPhases dal_c_Cmd__elapsed_phases = { 0 };

static char* dal_c_Cmd__makeProjectLockPath(const char* root) {
    char* base_dir = NULL;
    if (root && root[0] != '\0') {
        base_dir = strdup(root);
    } else {
        char* cwd = env_getCWD();
        base_dir = cwd ? cwd : strdup(".");
    }
    if (!base_dir) {
        return NULL;
    }

    char* lock_path = path_join(base_dir, ".dh-c.lock");
    free(base_dir);
    return lock_path;
}

bool dal_c__projectLockAcquireAt(const char* root, dal_c_ProjectLock* lock) {
    assert(lock != NULL);
    memset(lock, 0, sizeof(*lock));

    char* lock_path = dal_c_Cmd__makeProjectLockPath(root);
    if (!lock_path) {
        return false;
    }

    for (int i = 0; i < dal_c_Cmd__held_lock_count; ++i) {
        if (str_eql(dal_c_Cmd__held_locks[i].path, lock_path)) {
            dal_c_Cmd__held_locks[i].ref_count++;
            lock->path = lock_path;
            lock->acquired = true;
            return true;
        }
    }

    char* lock_parent = path_parent(lock_path);
    if (lock_parent && !dir_createRecur(lock_parent)) {
        free(lock_parent);
        free(lock_path);
        return false;
    }
    free(lock_parent);

    file_Lock os_lock = { 0 };
#ifndef _WIN32
    os_lock.fd = -1;
#endif
    double lock_started_at = dal_c_Cmd__nowSeconds();
    if (!file_lockAcquire(&os_lock, lock_path)) {
        free(lock_path);
        return false;
    }
    double lock_elapsed = dal_c_Cmd__nowSeconds() - lock_started_at;
    if (os_lock.waited) {
        lock->waited = true;
        dal_c_Cmd__elapsed_lock_wait_seconds += lock_elapsed;
    }

    dal_c_Cmd__HeldProjectLock* grown = realloc(
        dal_c_Cmd__held_locks,
        (size_t)(dal_c_Cmd__held_lock_count + 1) * sizeof(*dal_c_Cmd__held_locks)
    );
    if (!grown) {
        file_lockRelease(&os_lock);
        free(lock_path);
        return false;
    }
    dal_c_Cmd__held_locks = grown;
    dal_c_Cmd__held_locks[dal_c_Cmd__held_lock_count] = (dal_c_Cmd__HeldProjectLock){
        .path = strdup(lock_path),
        .lock = os_lock,
        .ref_count = 1,
    };
    if (!dal_c_Cmd__held_locks[dal_c_Cmd__held_lock_count].path) {
        file_lockRelease(&os_lock);
        free(lock_path);
        return false;
    }
    dal_c_Cmd__held_lock_count++;

    lock->path = lock_path;
    lock->acquired = true;
    lock->waited = os_lock.waited;
    return true;
}

bool dal_c__projectLockAcquire(const dal_c_Project* proj, dal_c_ProjectLock* lock) {
    return dal_c__projectLockAcquireAt((proj && proj->root) ? proj->root : NULL, lock);
}

void dal_c__projectLockRelease(dal_c_ProjectLock* lock) {
    if (!lock || !lock->acquired || !lock->path) {
        return;
    }

    for (int i = 0; i < dal_c_Cmd__held_lock_count; ++i) {
        if (!str_eql(dal_c_Cmd__held_locks[i].path, lock->path)) {
            continue;
        }

        dal_c_Cmd__held_locks[i].ref_count--;
        if (dal_c_Cmd__held_locks[i].ref_count <= 0) {
            file_lockRelease(&dal_c_Cmd__held_locks[i].lock);
            free(dal_c_Cmd__held_locks[i].path);
            for (int j = i + 1; j < dal_c_Cmd__held_lock_count; ++j) {
                dal_c_Cmd__held_locks[j - 1] = dal_c_Cmd__held_locks[j];
            }
            dal_c_Cmd__held_lock_count--;
            if (dal_c_Cmd__held_lock_count == 0) {
                free(dal_c_Cmd__held_locks);
                dal_c_Cmd__held_locks = NULL;
            }
        }
        break;
    }

    free(lock->path);
    memset(lock, 0, sizeof(*lock));
}

dal_c_Cmd* dal_c_Cmd_parse(int argc, const char* argv[]) {
    if (argc < 2) { return NULL; }
    dal_c_Cmd* cmd = calloc(1, sizeof(dal_c_Cmd));
    if (!cmd) { return NULL; }
    cmd->opts.profile = dal_c_default_profile;
    cmd->show_progress = true;
    cmd->elapsed_precision = dal_c_default_elapsed_precision;

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
        const char* dh_assigned_prefix = dal_c_opt_prefix_long dal_c_opt_dh dal_c_opt_value_sep;
        size_t dh_assigned_prefix_len = strlen(dh_assigned_prefix);
        if (strncmp(arg, dh_assigned_prefix, dh_assigned_prefix_len) == 0) {
            const char* value = arg + dh_assigned_prefix_len;
            if (!path_isDir(value)) {
                (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                dal_c_Cmd_cleanup(&cmd);
                return NULL;
            }
            dal_c_Cmd__setOwnedString(&cmd->dh_path_override, value);
        } else if (str_eql(arg, dal_c_opt_prefix_long dal_c_opt_dh)) {
            if (i + 1 >= argc || !path_isDir(argv[i + 1])) {
                (void)fprintf(stderr, "Error: Path not found: %s\n", i + 1 < argc ? argv[i + 1] : "");
                dal_c_Cmd_cleanup(&cmd);
                return NULL;
            }
            dal_c_Cmd__setOwnedString(&cmd->dh_path_override, argv[++i]);
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
    free(cmd->output_ext);
    free(cmd->linker_script);
    free(cmd->objcopy);
    free(cmd->objcopy_format);
    free(cmd->dh_path_override);
    free(cmd->make_jobs);

    switch (cmd->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
        free(cmd->payload.build.target_path);
        free(cmd->payload.build.output_path);
        free(cmd->payload.build.emit_map_path);
        free(cmd->payload.build.emit_linked_asm_path);
        free(cmd->payload.build.emit_disasm_path);
        free(cmd->payload.build.emit_ir_path);
        free(cmd->payload.build.emit_debug_info_path);
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
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
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
        || (dal_c_Cmd__usesBuildPayload(cmd->action) && cmd->payload.build.as_library);
}

static bool dal_c_Cmd__usesBuildPayload(dal_c_CmdAction action) {
    return action == dal_c_CmdAction_build
        || action == dal_c_CmdAction_compile_db
        || action == dal_c_CmdAction_syntax
        || action == dal_c_CmdAction_tidy
        || action == dal_c_CmdAction_format_code;
}

static bool dal_c_Cmd__usesBuildArtifactPayload(dal_c_CmdAction action) {
    return action == dal_c_CmdAction_build
        || action == dal_c_CmdAction_build_self;
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

static bool dal_c_Cmd__tryParseVisibilityValue(const char* value, bool* out) {
    assert(out != NULL);
    if (!value) {
        return false;
    }
    if (str_eql(value, "show")) {
        *out = true;
        return true;
    }
    if (str_eql(value, "hide")) {
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

static bool dal_c_Cmd__tryParseElapsedPrecision(const char* value, int* out) {
    assert(out != NULL);
    if (!value || value[0] == '\0') {
        return false;
    }
    char* end = NULL;
    long parsed = strtol(value, &end, 10);
    if (parsed < 0 || parsed > dal_c_max_elapsed_precision || !end || *end != '\0') {
        return false;
    }
    *out = (int)parsed;
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
    bool shown = false;
    bool has_visibility = dal_c_Cmd__tryParseVisibilityValue(value, &shown);
    dal_c_ToggleState toggle = dal_c_ToggleState_auto;
    bool has_toggle = dal_c_Cmd__tryParseToggleValue(value, &toggle);

    *handled = true;
    if (dal_c_Cmd__OPT_IS(dal_c_opt_link_dsl)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.dsl_mode = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_libc)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.libc_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_default_libs)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.default_libs_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_start_files)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.start_files_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_compiler_rt)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.compiler_rt_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_stdlib)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.default_libs_linked = toggle;
            cmd->opts.start_files_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_link_crt)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.start_files_linked = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_lto)) {
        dal_c_LtoMode mode = dal_c_LtoMode_parse(value);
        if (mode == dal_c_LtoMode_invalid) {
            *handled = false;
        } else {
            cmd->opts.lto_mode = mode;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_prebuilt)) {
        dal_c_PrebuiltMode mode = dal_c_PrebuiltMode_parse(value);
        if (mode == dal_c_PrebuiltMode_invalid) {
            *handled = false;
        } else {
            cmd->opts.prebuilt_mode = mode;
            cmd->opts.prebuilt_mode_set = true;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_omit_frame_pointer)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.omit_frame_pointer = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_function_sections)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.function_sections = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_data_sections)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.data_sections = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_gc_sections)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.gc_sections = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_whole_archive)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.whole_archive = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_unroll_loops)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.unroll_loops = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_unwind_tables)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.unwind_tables = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_async_unwind_tables)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.async_unwind_tables = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_exceptions)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.exceptions = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_strip)) {
        if (!has_toggle) {
            *handled = false;
        } else {
            cmd->opts.strip_mode = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_demangle)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_demangle = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_source)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_source = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_line_numbers)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_line_numbers = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_symbolize_operands)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_symbolize_operands = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_raw_insn)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_raw_insn = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_disasm_section_contents)) {
        if (!has_toggle) {
            *handled = false;
        } else if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
            cmd->payload.build.disasm_section_contents = toggle;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_freestanding)) {
        if (!has_bool) {
            *handled = false;
        } else {
            cmd->opts.compile_env = enabled ? dal_c_CompileEnv_freestanding : dal_c_CompileEnv_hosted;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_hosted)) {
        if (!has_bool) {
            *handled = false;
        } else {
            cmd->opts.compile_env = enabled ? dal_c_CompileEnv_hosted : dal_c_CompileEnv_freestanding;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_loose_errors)) {
        dal_c_LooseErrorsMode mode = dal_c_LooseErrorsMode_parse(value);
        if (mode == dal_c_LooseErrorsMode_invalid) {
            (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_loose_errors, value);
            return 1;
        }
        cmd->opts.loose_errors = mode;
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_verbose)) {
        if (!has_bool) {
            *handled = false;
        } else {
            cmd->verbose = enabled;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_commands)) {
        if (!has_visibility) {
            *handled = false;
        } else {
            cmd->show_commands = shown;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_progress)) {
        if (!has_visibility) {
            *handled = false;
        } else {
            cmd->show_progress = shown;
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_elapsed_precision)) {
        int precision = 0;
        if (!dal_c_Cmd__tryParseElapsedPrecision(value, &precision)) {
            (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_elapsed_precision, value);
            return 1;
        }
        cmd->elapsed_precision = precision;
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_image)) {
        if (!has_bool) {
            *handled = false;
        } else {
            if (cmd->action == dal_c_CmdAction_build) {
                cmd->payload.build.as_image = enabled;
            }
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_emit_preprocessed)) {
        if (!has_bool) {
            *handled = false;
        } else {
            if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                cmd->payload.build.emit_preprocessed = enabled;
            }
        }
    } else if (dal_c_Cmd__OPT_IS(dal_c_opt_emit_asm)) {
        if (!has_bool) {
            *handled = false;
        } else {
            if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                cmd->payload.build.emit_asm = enabled;
            }
        }
    } else {
        *handled = false;
    }
#undef dal_c_Cmd__OPT_IS
    return 0;
}

static void dal_c_Cmd__setArtifactPath(char** slot, bool* enabled, const char* value) {
    assert(slot != NULL);
    assert(enabled != NULL);
    *enabled = true;
    if (value && value[0] != '\0') {
        dal_c_Cmd__setOwnedString(slot, value);
    }
}

static void dal_c_Cmd__enableAnalysisArtifacts(dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    if (!dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) { return; }
    cmd->payload.build.analysis_artifacts = true;
    cmd->payload.build.emit_map = true;
    cmd->payload.build.emit_linked_asm = true;
    cmd->payload.build.emit_disasm = true;
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

    if (cmd->opts.compiler_rt_linked == dal_c_ToggleState_disabled
        && cmd->opts.default_libs_linked != dal_c_ToggleState_disabled) {
        (void)fprintf(
            stderr,
            "Warning: `--%s=off` does not suppress compiler-rt while `--%s` remains enabled; clang may inject it through default libraries.\n",
            dal_c_opt_link_compiler_rt,
            dal_c_opt_link_default_libs
        );
    }

    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.self_boundary) {
        if (cmd->input_count > 0
            || cmd->payload.build.output_path != NULL
            || cmd->output_ext != NULL
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
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_preprocessed
        && (cmd->payload.build.emit_map || cmd->payload.build.emit_linked_asm || cmd->payload.build.emit_disasm)) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--emit-preprocessed` with linked artifact outputs\n");
        return 1;
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_asm
        && (cmd->payload.build.emit_map || cmd->payload.build.emit_linked_asm || cmd->payload.build.emit_disasm)) {
        (void)fprintf(stderr, "Error: `build` cannot combine `--emit-asm` with linked artifact outputs\n");
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
    out->output_ext = cmd->output_ext;
    out->linking = dal_c_LinkMode_toLibraryLinking(cmd->opts.link_mode, dal_c_Linking_static);

    switch (cmd->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
        out->target_path = cmd->payload.build.target_path;
        out->target_root_name_hint = dal_c_Cmd__sampleDirCanonical(cmd->payload.build.sample_dir);
        out->output_path = cmd->payload.build.output_path;
        out->linking = dal_c_LinkMode_toLibraryLinking(cmd->opts.link_mode, cmd->payload.build.linking);
        out->build_all = cmd->payload.build.build_all;
        out->as_library = cmd->payload.build.as_library;
        out->self_boundary = cmd->payload.build.self_boundary;
        out->recursive = cmd->payload.build.recursive;
        out->dsl_first = cmd->payload.build.dsl_first;
        break;
    case dal_c_CmdAction_lib:
        out->target_path = cmd->payload.lib.target_path;
        out->output_path = cmd->payload.lib.output_path;
        out->linking = dal_c_LinkMode_toLibraryLinking(cmd->opts.link_mode, cmd->payload.lib.linking);
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
        out->target_root_name_hint = cmd->payload.test.sample_dir == dal_c_SampleDir_none
                                       ? dal_c_dir_tests
                                       : dal_c_Cmd__sampleDirCanonical(cmd->payload.test.sample_dir);
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
    case dal_c_CmdAction_toolchain:
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
    out->target_path_is_explicit_file = out->target_root_name_hint == NULL
                                     && out->target_path != NULL
                                     && dal_c_Cmd__hasExplicitFileInputs(cmd);
}

static int dal_c_Cmd__compareDepsHeaderPath(const void* lhs, const void* rhs) {
    const char* a = *(const char* const*)lhs;
    const char* b = *(const char* const*)rhs;
    int depth_a = 0;
    int depth_b = 0;
    for (const char* p = a; p && *p; ++p) {
        if (*p == '/' || *p == '\\') { depth_a++; }
    }
    for (const char* p = b; p && *p; ++p) {
        if (*p == '/' || *p == '\\') { depth_b++; }
    }
    if (depth_a != depth_b) { return depth_a - depth_b; }
    return strcmp(a, b);
}

static void dal_c_Cmd__appendGeneratedLine(char** content, size_t* len, size_t* cap, const char* line) {
    assert(content != NULL);
    assert(len != NULL);
    assert(cap != NULL);
    assert(line != NULL);
    size_t line_len = strlen(line);
    if (*len + line_len + 1 > *cap) {
        size_t new_cap = (*cap == 0) ? 1024 : *cap;
        while (*len + line_len + 1 > new_cap) { new_cap *= 2; }
        char* grown = realloc(*content, new_cap);
        if (!grown) { return; }
        *content = grown;
        *cap = new_cap;
    }
    memcpy(*content + *len, line, line_len);
    *len += line_len;
    (*content)[*len] = '\0';
}

static void dal_c_Cmd__normalizeIncludePath(char* path) {
    if (!path) { return; }
    for (char* p = path; *p; ++p) {
        if (*p == '\\') { *p = '/'; }
    }
}

static bool dal_c_Cmd__writeTextIfChanged(const char* path, const char* content) {
    assert(path != NULL);
    assert(content != NULL);
    char* existing = file_read(path);
    if (existing && str_eql(existing, content)) {
        free(existing);
        return true;
    }
    free(existing);
    return file_writeAtomic(path, content);
}

bool dal_c__writeDepsPreludeHeader(const dal_c_Project* proj, const dal_c_CompilerOpts* opts) {
    if (!proj || !proj->root) { return true; }

    char* lib_dir = dal_c_Project_getLibDir(proj);
    char* deps_dir = dal_c_Project_getDepsDir(proj);
    if (!lib_dir || !deps_dir) {
        free(deps_dir);
        free(lib_dir);
        return false;
    }

    char* deps_header = path_join(lib_dir, "deps.h");
    if (!deps_header) {
        free(deps_dir);
        free(lib_dir);
        return false;
    }

    dal_c_CompilerOpts effective_opts = { 0 };
    effective_opts.profile = dal_c_Profile_invalid;
    dal_c_CompilerOpts_merge(&effective_opts, &proj->opts);
    dal_c_CompilerOpts_merge(&effective_opts, opts);
    bool include_dh_bundle = dal_c__usesDHLibrary(proj, &effective_opts);
    bool explicit_deps_pch = proj->pch_header_override && str_eql(proj->pch_header_override, dal_c_pch_value_deps);

    int raw_count = 0;
    char** raw_files = path_isDir(deps_dir) ? dir_list(deps_dir, &raw_count) : NULL;
    char** rel_headers = NULL;
    int rel_count = 0;
    int rel_cap = 0;
    for (int i = 0; i < raw_count; ++i) {
        const char* file = raw_files[i];
        if (!str_endsWith(file, ".h")) { continue; }
        char* rel = path_relative(deps_dir, file);
        if (!rel) { continue; }
        dal_c_Cmd__normalizeIncludePath(rel);
        if (strchr(rel, '/') != NULL) {
            free(rel);
            continue;
        }
        if (str_eql(rel, "deps.h")) {
            free(rel);
            continue;
        }
        if (rel_count >= rel_cap) {
            int new_cap = (rel_cap == 0) ? 16 : rel_cap * 2;
            char** grown = realloc(rel_headers, (size_t)new_cap * sizeof(char*));
            if (!grown) {
                free(rel);
                for (int j = 0; j < rel_count; ++j) { free(rel_headers[j]); }
                free(rel_headers);
                if (raw_files) {
                    for (int j = 0; j < raw_count; ++j) { free(raw_files[j]); }
                    free(raw_files);
                }
                free(deps_header);
                free(deps_dir);
                free(lib_dir);
                return false;
            }
            rel_headers = grown;
            rel_cap = new_cap;
        }
        rel_headers[rel_count++] = rel;
    }
    if (rel_count > 1) { qsort(rel_headers, (size_t)rel_count, sizeof(char*), dal_c_Cmd__compareDepsHeaderPath); }

    include_dh_bundle = include_dh_bundle && (explicit_deps_pch || rel_count > 0);

    if (!explicit_deps_pch && rel_count == 0) {
        (void)remove(deps_header);
        dal_c_CompilerOpts_cleanup(&effective_opts);
        for (int i = 0; i < rel_count; ++i) { free(rel_headers[i]); }
        free(rel_headers);
        if (raw_files) {
            for (int i = 0; i < raw_count; ++i) { free(raw_files[i]); }
            free(raw_files);
        }
        free(deps_header);
        free(deps_dir);
        free(lib_dir);
        return true;
    }

    dir_createRecur(lib_dir);
    dir_createRecur(deps_dir);

    char* content = NULL;
    size_t len = 0;
    size_t cap = 0;
    dal_c_Cmd__appendGeneratedLine(&content, &len, &cap, "#pragma once\n");
    dal_c_Cmd__appendGeneratedLine(&content, &len, &cap, "/* Generated by dh-c. Do not edit. */\n\n");
    if (include_dh_bundle) {
        dal_c_Cmd__appendGeneratedLine(&content, &len, &cap, "#include <dh-bundle.h>\n");
    }
    for (int i = 0; i < rel_count; ++i) {
        char* line = str_format("#include <%s>\n", rel_headers[i]);
        if (line) {
            dal_c_Cmd__appendGeneratedLine(&content, &len, &cap, line);
            free(line);
        }
    }
    bool ok = content && dal_c_Cmd__writeTextIfChanged(deps_header, content);
    dal_c_CompilerOpts_cleanup(&effective_opts);
    free(content);
    for (int i = 0; i < rel_count; ++i) { free(rel_headers[i]); }
    free(rel_headers);
    if (raw_files) {
        for (int i = 0; i < raw_count; ++i) { free(raw_files[i]); }
        free(raw_files);
    }
    free(deps_header);
    free(deps_dir);
    free(lib_dir);
    return ok;
}

static bool dal_c_Cmd__executeNeedsProjectLock(const dal_c_Cmd* self) {
    assert(self != NULL);
    switch (self->action) {
    case dal_c_CmdAction_build:
        return !self->payload.build.self_boundary;
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_test_dsl:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
        return true;
    case dal_c_CmdAction_clean:
        return !self->payload.clean.self_boundary;
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}

static double dal_c_Cmd__nowSeconds(void) {
#ifdef _WIN32
    static LARGE_INTEGER frequency = { 0 };
    LARGE_INTEGER counter = { 0 };
    if (frequency.QuadPart == 0) {
        (void)QueryPerformanceFrequency(&frequency);
    }
    if (frequency.QuadPart != 0 && QueryPerformanceCounter(&counter)) {
        return (double)counter.QuadPart / (double)frequency.QuadPart;
    }
#elif defined(CLOCK_MONOTONIC)
    struct timespec ts = { 0 };
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
    }
#endif
    time_t now = time(NULL);
    return (double)now;
}

double dal_c__phaseNowSeconds(void) {
    return dal_c_Cmd__nowSeconds();
}

void dal_c__phaseRecord(dal_c_CmdPhase phase, double elapsed_seconds) {
    if (elapsed_seconds <= 0.0) { return; }
    switch (phase) {
    case dal_c_CmdPhase_project_lib_build: dal_c_Cmd__elapsed_phases.project_lib_build += elapsed_seconds; break;
    case dal_c_CmdPhase_dependency_build: dal_c_Cmd__elapsed_phases.dependency_build += elapsed_seconds; break;
    case dal_c_CmdPhase_dh_build: dal_c_Cmd__elapsed_phases.dh_build += elapsed_seconds; break;
    case dal_c_CmdPhase_self_build: dal_c_Cmd__elapsed_phases.self_build += elapsed_seconds; break;
    case dal_c_CmdPhase_test_build: dal_c_Cmd__elapsed_phases.test_build += elapsed_seconds; break;
    case dal_c_CmdPhase_run_build: dal_c_Cmd__elapsed_phases.run_build += elapsed_seconds; break;
    case dal_c_CmdPhase_test_run: dal_c_Cmd__elapsed_phases.test_run += elapsed_seconds; break;
    case dal_c_CmdPhase_run_exec: dal_c_Cmd__elapsed_phases.run_exec += elapsed_seconds; break;
    case dal_c_CmdPhase_syntax: dal_c_Cmd__elapsed_phases.syntax += elapsed_seconds; break;
    case dal_c_CmdPhase_tidy: dal_c_Cmd__elapsed_phases.tidy += elapsed_seconds; break;
    case dal_c_CmdPhase_format: dal_c_Cmd__elapsed_phases.format += elapsed_seconds; break;
    case dal_c_CmdPhase_compile_db: dal_c_Cmd__elapsed_phases.compile_db += elapsed_seconds; break;
    case dal_c_CmdPhase_clean: dal_c_Cmd__elapsed_phases.clean += elapsed_seconds; break;
    default: break;
    }
}

static bool dal_c_Cmd__reportsElapsed(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    if (!cmd->show_progress) { return false; }
    switch (cmd->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_test_dsl:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
        return true;
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}

static void dal_c_Cmd__reportFinishedPhaseElapsed(FILE* out, const char* phase, double elapsed_seconds, int precision) {
    if (elapsed_seconds <= 0.0) { return; }
    (void)fprintf(out, "Finished %s in %.*fs\n", phase, precision, elapsed_seconds);
}

static bool dal_c_Cmd__phasesHaveAny(const dal_c_Cmd__ElapsedPhases* phases) {
    return phases
        && (phases->project_lib_build > 0.0
            || phases->dependency_build > 0.0
            || phases->dh_build > 0.0
            || phases->self_build > 0.0
            || phases->test_build > 0.0
            || phases->run_build > 0.0
            || phases->test_run > 0.0
            || phases->run_exec > 0.0
            || phases->syntax > 0.0
            || phases->tidy > 0.0
            || phases->format > 0.0
            || phases->compile_db > 0.0
            || phases->clean > 0.0);
}

static int dal_c_Cmd__phaseCount(const dal_c_Cmd__ElapsedPhases* phases) {
    if (!phases) { return 0; }
    int count = 0;
    count += phases->project_lib_build > 0.0 ? 1 : 0;
    count += phases->dependency_build > 0.0 ? 1 : 0;
    count += phases->dh_build > 0.0 ? 1 : 0;
    count += phases->self_build > 0.0 ? 1 : 0;
    count += phases->test_build > 0.0 ? 1 : 0;
    count += phases->run_build > 0.0 ? 1 : 0;
    count += phases->test_run > 0.0 ? 1 : 0;
    count += phases->run_exec > 0.0 ? 1 : 0;
    count += phases->syntax > 0.0 ? 1 : 0;
    count += phases->tidy > 0.0 ? 1 : 0;
    count += phases->format > 0.0 ? 1 : 0;
    count += phases->compile_db > 0.0 ? 1 : 0;
    count += phases->clean > 0.0 ? 1 : 0;
    return count;
}

static double dal_c_Cmd__executionPhaseSeconds(const dal_c_Cmd__ElapsedPhases* phases) {
    if (!phases) { return 0.0; }
    return phases->test_run + phases->run_exec;
}

static double dal_c_Cmd__setupWallSeconds(double active_seconds, const dal_c_Cmd__ElapsedPhases* phases) {
    double execution_seconds = dal_c_Cmd__executionPhaseSeconds(phases);
    double setup_seconds = active_seconds - execution_seconds;
    return setup_seconds > 0.0 ? setup_seconds : 0.0;
}

static void dal_c_Cmd__reportFinishedPhases(FILE* out, const dal_c_Cmd__ElapsedPhases* phases, int precision) {
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "deps", phases->dependency_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "dh build", phases->dh_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "self build", phases->self_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "project library build", phases->project_lib_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "executable build", phases->test_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "executable build", phases->run_build, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "execution", phases->test_run, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "execution", phases->run_exec, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "syntax", phases->syntax, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "tidy", phases->tidy, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "format", phases->format, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "compile-db", phases->compile_db, precision);
    dal_c_Cmd__reportFinishedPhaseElapsed(out, "clean", phases->clean, precision);
}

static void dal_c_Cmd__reportElapsedDetails(
    FILE* out,
    double active_seconds,
    double lock_wait_seconds,
    const dal_c_Cmd__ElapsedPhases* phases,
    int precision
) {
    double setup_seconds = dal_c_Cmd__setupWallSeconds(active_seconds, phases);
    double execution_seconds = dal_c_Cmd__executionPhaseSeconds(phases);
    if (execution_seconds > 0.0) {
        (void)fprintf(out, "Elapsed: setup %.*fs, execution %.*fs", precision, setup_seconds, precision, execution_seconds);
        if (lock_wait_seconds > 0.0) {
            (void)fprintf(out, ", lock %.*fs", precision, lock_wait_seconds);
        }
        (void)fprintf(out, "\n");
        return;
    }
    if (lock_wait_seconds > 0.0) {
        (void)fprintf(out, "Elapsed: active %.*fs, lock %.*fs\n", precision, active_seconds, precision, lock_wait_seconds);
    }
}

static void dal_c_Cmd__reportElapsed(
    const dal_c_Cmd* cmd,
    int result,
    double elapsed_seconds,
    double lock_wait_seconds,
    int elapsed_precision,
    const dal_c_Cmd__ElapsedPhases* phases
) {
    assert(cmd != NULL);
    const char* action = dal_c_CmdAction_format(cmd->action);
    if (!action) { action = "command"; }
    FILE* out = result == 0 ? stdout : stderr;
    const char* state = result == 0 ? "Finished" : "Failed";
    if (result == 0 && dal_c_Cmd__phaseCount(phases) > 1) {
        dal_c_Cmd__reportFinishedPhases(out, phases, elapsed_precision);
    }
    double active_seconds = elapsed_seconds - lock_wait_seconds;
    if (active_seconds < 0.0) { active_seconds = 0.0; }
    if (lock_wait_seconds > 0.0) {
        if (dal_c_Cmd__phasesHaveAny(phases)) {
            (void)fprintf(out, "%s `%s` in %.*fs\n", state, action, elapsed_precision, elapsed_seconds);
            dal_c_Cmd__reportElapsedDetails(out, active_seconds, lock_wait_seconds, phases, elapsed_precision);
            return;
        }
        (void)fprintf(out, "%s `%s` in %.*fs\n", state, action, elapsed_precision, elapsed_seconds);
        dal_c_Cmd__reportElapsedDetails(out, active_seconds, lock_wait_seconds, phases, elapsed_precision);
        return;
    }
    if (dal_c_Cmd__phasesHaveAny(phases)) {
        (void)fprintf(out, "%s `%s` in %.*fs\n", state, action, elapsed_precision, elapsed_seconds);
        dal_c_Cmd__reportElapsedDetails(out, active_seconds, 0.0, phases, elapsed_precision);
        return;
    }
    (void)fprintf(out, "%s `%s` in %.*fs\n", state, action, elapsed_precision, elapsed_seconds);
}

int dal_c_Cmd_execute(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);
    bool report_elapsed = dal_c_Cmd__reportsElapsed(self);
    double started_at = report_elapsed ? dal_c_Cmd__nowSeconds() : 0.0;
    double saved_lock_wait_seconds = dal_c_Cmd__elapsed_lock_wait_seconds;
    dal_c_Cmd__ElapsedPhases saved_phases = dal_c_Cmd__elapsed_phases;
    dal_c_Cmd__elapsed_lock_wait_seconds = 0.0;
    memset(&dal_c_Cmd__elapsed_phases, 0, sizeof(dal_c_Cmd__elapsed_phases));
    int result = 0;
    if (!dal_c_Cmd__executeNeedsProjectLock(self)) {
        result = dal_c_Cmd__executeUnlocked(self, proj);
        if (report_elapsed) {
            dal_c_Cmd__reportElapsed(
                self,
                result,
                dal_c_Cmd__nowSeconds() - started_at,
                dal_c_Cmd__elapsed_lock_wait_seconds,
                self->elapsed_precision,
                &dal_c_Cmd__elapsed_phases
            );
        }
        dal_c_Cmd__elapsed_lock_wait_seconds = saved_lock_wait_seconds;
        dal_c_Cmd__elapsed_phases = saved_phases;
        return result;
    }

    dal_c_ProjectLock lock = { 0 };
    if (!dal_c__projectLockAcquire(proj, &lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire build lock\n");
        if (report_elapsed) {
            dal_c_Cmd__reportElapsed(
                self,
                1,
                dal_c_Cmd__nowSeconds() - started_at,
                dal_c_Cmd__elapsed_lock_wait_seconds,
                self->elapsed_precision,
                &dal_c_Cmd__elapsed_phases
            );
        }
        dal_c_Cmd__elapsed_lock_wait_seconds = saved_lock_wait_seconds;
        dal_c_Cmd__elapsed_phases = saved_phases;
        return 1;
    }
    result = dal_c_Cmd__executeUnlocked(self, proj);
    dal_c__projectLockRelease(&lock);
    if (report_elapsed) {
        dal_c_Cmd__reportElapsed(
            self,
            result,
            dal_c_Cmd__nowSeconds() - started_at,
            dal_c_Cmd__elapsed_lock_wait_seconds,
            self->elapsed_precision,
            &dal_c_Cmd__elapsed_phases
        );
    }
    dal_c_Cmd__elapsed_lock_wait_seconds = saved_lock_wait_seconds;
    dal_c_Cmd__elapsed_phases = saved_phases;
    return result;
}

static int dal_c_Cmd__executeUnlocked(const dal_c_Cmd* self, const dal_c_Project* proj) {
    switch (self->action) {
    case dal_c_CmdAction_build:
        if (self->payload.build.self_boundary) {
            return dal_c__buildSelf(self);
        }
        return dal_c_Cmd_makeTarget(self, proj);
    case dal_c_CmdAction_compile_db:
    {
        double started_at = dal_c__phaseNowSeconds();
        int result = dal_c_Cmd_writeCompileDb(self, proj);
        dal_c__phaseRecord(dal_c_CmdPhase_compile_db, dal_c__phaseNowSeconds() - started_at);
        return result;
    }
    case dal_c_CmdAction_syntax:
        return dal_c_Cmd_runSyntax(self, proj);
    case dal_c_CmdAction_tidy:
        return dal_c_Cmd_runTidy(self, proj);
    case dal_c_CmdAction_format_code:
        return dal_c_Cmd_runFormat(self, proj);
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
        return dal_c_Cmd_makeTarget(self, proj);
    case dal_c_CmdAction_deps:
        return dal_c_Cmd_compileDeps(self, proj);
    case dal_c_CmdAction_toolchain:
        return dal_c_Cmd_queryToolchain(self);
    case dal_c_CmdAction_clean:
    {
        if (self->payload.clean.self_boundary) {
            return dal_c__cleanSelf(self);
        }
        double started_at = dal_c__phaseNowSeconds();
        int result = dal_c_Cmd_cleanTarget(self, proj);
        dal_c__phaseRecord(dal_c_CmdPhase_clean, dal_c__phaseNowSeconds() - started_at);
        return result;
    }
    case dal_c_CmdAction_build_self:
        return dal_c__buildSelf(self);
    case dal_c_CmdAction_clean_self:
        return dal_c__cleanSelf(self);
    case dal_c_CmdAction_workspace:
        return dal_c_Cmd_createWorkspace();
    case dal_c_CmdAction_project:
        return dal_c_Cmd_createProject();
    case dal_c_CmdAction_build_dsl:
        return dal_c__buildDSL(self, proj);
    case dal_c_CmdAction_test_dsl:
        return dal_c__testDSL(self, proj);
    case dal_c_CmdAction_clean_dsl:
    {
        double started_at = dal_c__phaseNowSeconds();
        int result = dal_c__cleanDSL(self, proj);
        dal_c__phaseRecord(dal_c_CmdPhase_clean, dal_c__phaseNowSeconds() - started_at);
        return result;
    }
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        (void)fprintf(stderr, "Error: Unknown command\n");
        return 1;
    }
}

int dal_c_Cmd_queryToolchain(const dal_c_Cmd* self) {
    assert(self != NULL);
    return dal_c__queryToolchain(self);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
int dal_c_Cmd_makeTarget(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);
    assert(proj != NULL);
    dal_c_ProjectLock lock = { 0 };
    if (!dal_c__projectLockAcquire(proj, &lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire build lock\n");
        return 1;
    }
    int result = dal_c_Cmd__makeTargetUnlocked(self, proj);
    dal_c__projectLockRelease(&lock);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c_Cmd__makeTargetUnlocked(const dal_c_Cmd* self, const dal_c_Project* proj) {
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
    ArrStr* active_excludes = intent.target_path_is_explicit_file
                                ? ArrStr_init()
                                : dal_c_Cmd__collectActiveExcludes(target_proj, &target_request, self);
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

    if (!intent.target_path_is_explicit_file && target_proj->lib_count > 0) {
        if (self->verbose) {
            printf("Building %d libraries...\n", target_proj->lib_count);
        }
        for (int i = 0; i < target_proj->lib_count; ++i) {
            const dal_c_Lib* lib = &target_proj->libraries[i];
            const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
            if (!str_eql(provider, "dh")) continue;
            if (dal_c__buildSingleLibrary(self, target_proj, lib) != 0) {
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
        }
    }

    if (!intent.target_path_is_explicit_file && !dal_c__writeDepsPreludeHeader(target_proj, &self->opts)) {
        ArrStr_fini(&active_excludes);
        dal_c_TargetRequest_cleanup(&target_request);
        (void)fprintf(stderr, "Error: Failed to generate dependency prelude header\n");
        return 1;
    }

    bool needs_project_static_lib = !intent.target_path_is_explicit_file
                                 && ((target_request.root && target_request.link_project) || is_test_mode);
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
            tests = dal_c_Cmd__collectTargetSources(target_proj, &target_request);
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
        ArrStr* target_sources = dal_c_Cmd__collectTargetSources(target_proj, &target_request);
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

            dal_c_Project* file_proj = NULL;
            const dal_c_Project* build_proj = proj;
            if (intent.target_path_is_explicit_file) {
                char* parent = path_parent(self->input_files[0]);
                if (!parent) {
                    ArrStr_fini(&sources);
                    ArrStr_fini(&active_excludes);
                    dal_c_TargetRequest_cleanup(&target_request);
                    return 1;
                }
                file_proj = dal_c_Project_detectAt(parent, proj->dh_path);
                free(parent);
                if (!file_proj) {
                    ArrStr_fini(&sources);
                    ArrStr_fini(&active_excludes);
                    dal_c_TargetRequest_cleanup(&target_request);
                    return 1;
                }
                file_proj->pch_enabled = false;
                free(file_proj->pch_header);
                file_proj->pch_header = NULL;
                build_proj = file_proj;
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
            dal_c_Cmd__mergeBuildProperties(&effective_opts, &effective_defaults, build_proj, sources, self);

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
                dal_c_Project_cleanup(&file_proj);
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
                    dal_c_Project_cleanup(&file_proj);
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
                result = dal_c_Cmd__buildLibrarySetFromSources(
                    self,
                    build_proj,
                    sources,
                    output_name,
                    !intent.target_path_is_explicit_file
                );
            } else {
                result = dal_c_Cmd__buildFromSources(
                    self,
                    build_proj,
                    sources,
                    output_name,
                    target_type,
                    NULL,
                    !intent.target_path_is_explicit_file,
                    true
                );
            }
            dal_c_BuildDefaults_cleanup(&effective_defaults);
            dal_c_CompilerOpts_cleanup(&effective_opts);
            dal_c_Project_cleanup(&file_proj);
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


typedef struct dal_c_Cmd__CheckPlan {
    dal_c_Cmd effective;
    dal_c_Project* file_proj;
    const dal_c_Project* build_proj;
    dal_c_BuildDefaults defaults;
    const dal_c_ProfileSpec* profile;
    ArrStr* sources;
    dal_c_Target target_type;
} dal_c_Cmd__CheckPlan;

static bool dal_c_Cmd__isAssemblySource(const char* path) {
    return path && (str_endsWith(path, ".S") || str_endsWith(path, ".s"));
}

static void dal_c_Cmd__cleanupCheckPlan(dal_c_Cmd__CheckPlan* plan) {
    if (!plan) { return; }
    dal_c_BuildDefaults_cleanup(&plan->defaults);
    dal_c_CompilerOpts_cleanup(&plan->effective.opts);
    dal_c_Project_cleanup(&plan->file_proj);
    ArrStr_fini(&plan->sources);
    memset(plan, 0, sizeof(*plan));
}

static int dal_c_Cmd__prepareCheckPlan(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Cmd__CheckPlan* plan) {
    assert(self != NULL);
    assert(proj != NULL);
    assert(plan != NULL);
    memset(plan, 0, sizeof(*plan));

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    const bool builds_library = dal_c_Cmd__buildsLibrary(self);
    const bool build_all = intent.build_all;

    const dal_c_Project* target_proj = proj;
    dal_c_TargetRequest target_request = { 0 };
    if (!dal_c_TargetRequest_resolve(target_proj, &intent, &target_request)) {
        return 1;
    }

    ArrStr* active_excludes = intent.target_path_is_explicit_file
                                ? ArrStr_init()
                                : dal_c_Cmd__collectActiveExcludes(target_proj, &target_request, self);
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

    ArrStr* sources = ArrStr_init();
    if (target_request.root) {
        ArrStr_fini(&sources);
        sources = dal_c_Cmd__collectTargetSources(target_proj, &target_request);
        if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, target_request.raw_target_path != NULL) != 0) {
            ArrStr_fini(&sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }
    } else if (self->input_count > 0) {
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

    if (ArrStr_len(sources) == 0) {
        (void)fprintf(stderr, "Error: No source files found\n");
        ArrStr_fini(&sources);
        ArrStr_fini(&active_excludes);
        dal_c_TargetRequest_cleanup(&target_request);
        return 1;
    }

    dal_c_Project* file_proj = NULL;
    const dal_c_Project* build_proj = proj;
    if (intent.target_path_is_explicit_file) {
        char* rel_to_project = (proj && proj->root) ? path_relative(proj->root, self->input_files[0]) : NULL;
        if (rel_to_project) {
            free(rel_to_project);
        } else {
            char* parent = path_parent(self->input_files[0]);
            if (!parent) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            file_proj = dal_c_Project_detectAt(parent, proj->dh_path);
            free(parent);
            if (!file_proj) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            file_proj->pch_enabled = false;
            free(file_proj->pch_header);
            file_proj->pch_header = NULL;
            build_proj = file_proj;
        }
    }

    plan->effective = *self;
    memset(&plan->effective.opts, 0, sizeof(plan->effective.opts));
    plan->effective.opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&plan->effective.opts, &plan->defaults, build_proj, sources, self);
    dal_c_Cmd__appendTargetLocalInclude(&plan->effective.opts, build_proj, self);
    if (plan->effective.opts.profile == dal_c_Profile_invalid) {
        plan->effective.opts.profile = self->opts.profile;
    }
    plan->profile = dal_c_ProfileSpec_by(plan->effective.opts.profile);
    if (!plan->profile) {
        (void)fprintf(stderr, "Error: Invalid profile\n");
        ArrStr_fini(&sources);
        ArrStr_fini(&active_excludes);
        dal_c_TargetRequest_cleanup(&target_request);
        plan->file_proj = file_proj;
        dal_c_Cmd__cleanupCheckPlan(plan);
        return 1;
    }

    dal_c_Target target_type = target_request.root ? target_request.kind : dal_c_Cmd__resolveBuildTargetType(self, intent.linking, builds_library);
    if (!target_request.root && !builds_library && plan->defaults.target_kind_set) {
        target_type = plan->defaults.target_kind;
    }
    if (target_type == dal_c_Target_lib) {
        target_type = dal_c_Target_static_lib;
    }

    plan->file_proj = file_proj;
    plan->build_proj = build_proj;
    plan->sources = sources;
    plan->target_type = target_type;
    ArrStr_fini(&active_excludes);
    dal_c_TargetRequest_cleanup(&target_request);
    return 0;
}

static int dal_c_Cmd__runSyntaxPlan(dal_c_Cmd__CheckPlan* plan) {
    assert(plan != NULL);
    int result = 0;
    const int total = ArrStr_len(plan->sources);
    int job_count = dal_c__parallelJobCount(&plan->effective);
    if (job_count < 1) { job_count = 1; }
    if (job_count > total) { job_count = total; }
    proc_Child** active = job_count > 0 ? (proc_Child**)calloc((size_t)job_count, sizeof(*active)) : NULL;
    if (job_count > 0 && !active) { return 1; }

    int next = 0;
    int active_count = 0;
    while (next < total || active_count > 0) {
        while (result == 0 && next < total && active_count < job_count) {
            const int source_index = next++;
            const char* src = ArrStr_at(plan->sources, source_index);
            if (dal_c_Cmd__isAssemblySource(src)) {
                if (plan->effective.show_progress) {
                    printf("[%d/%d] SKIP syntax %s\n", source_index + 1, total, src);
                }
                continue;
            }

            ArrStr* argv = ArrStr_init();
            dal_c__appendSyntaxArguments(argv, &plan->effective, plan->build_proj, plan->profile, src, plan->target_type);
            ArrStr_push(argv, "-fsyntax-only");
            if (plan->effective.show_progress) {
                printf("[%d/%d] SYNTAX %s\n", source_index + 1, total, src);
            }
            char** raw = ArrStr_toRaw(argv);
            int raw_count = 0;
            while (raw && raw[raw_count] != NULL) { ++raw_count; }
            const char** proc_argv = raw ? (const char**)calloc((size_t)raw_count + 1u, sizeof(char*)) : NULL;
            for (int j = 0; proc_argv && j < raw_count; ++j) { proc_argv[j] = raw[j]; }
            if (plan->effective.show_commands) {
                for (int j = 0; raw && raw[j] != NULL; ++j) {
                    if (j > 0) { printf(" "); }
                    printf("%s", raw[j]);
                }
                printf("\n");
            }
            proc_Child* child = proc_argv ? proc_spawn(proc_argv, true) : NULL;
            free((void*)proc_argv);
            dal_c_Cmd__freeRawArgv(raw);
            ArrStr_fini(&argv);
            if (!child) {
                result = 1;
                break;
            }
            active[active_count++] = child;
        }

        if (active_count == 0) {
            break;
        }

        int finished_index = -1;
        int code = proc_waitAny(active, active_count, &finished_index);
        if (finished_index >= 0 && finished_index < active_count) {
            active[finished_index] = active[active_count - 1];
            active[active_count - 1] = NULL;
            --active_count;
        } else {
            result = result == 0 ? 1 : result;
            break;
        }
        if (code != 0 && result == 0) {
            result = code;
        }
    }

    for (int i = 0; i < active_count; ++i) {
        int code = proc_wait(&active[i]);
        if (code != 0 && result == 0) {
            result = code;
        }
    }
    free(active);
    if (result == 0) {
        printf("Syntax check successful!\n");
    }
    return result;
}

static void dal_c_Cmd__freeRawArgv(char** raw) {
    if (!raw) { return; }
    for (int i = 0; raw[i] != NULL; ++i) {
        free(raw[i]);
    }
    free((void*)raw);
}

static bool dal_c_Cmd__toolAvailable(const char* tool) {
    if (!tool || tool[0] == '\0') { return false; }
    const char* argv[] = { tool, "--version", NULL };
    return proc_run(argv, false) == 0;
}

static int dal_c_Cmd__runTidyPlan(const dal_c_Cmd* self, const dal_c_Project* proj, dal_c_Cmd__CheckPlan* plan) {
    assert(self != NULL);
    assert(proj != NULL);
    assert(plan != NULL);
    if (!dal_c_Cmd__toolAvailable("clang-tidy")) {
        (void)fprintf(stderr, "Error: clang-tidy not found in PATH\n");
        return 1;
    }

    int db_result = dal_c_Cmd_writeCompileDb(self, proj);
    if (db_result != 0) { return db_result; }

    const char* project_root = (plan->build_proj && plan->build_proj->root) ? plan->build_proj->root : ".";
    int result = 0;
    const int total = ArrStr_len(plan->sources);
    for (int i = 0; i < total; ++i) {
        const char* src = ArrStr_at(plan->sources, i);
        if (dal_c_Cmd__isAssemblySource(src)) {
            if (self->show_progress) {
                printf("[%d/%d] SKIP tidy %s\n", i + 1, total, src);
            }
            continue;
        }
        if (self->show_progress) {
            printf("[%d/%d] TIDY %s\n", i + 1, total, src);
        }
        const char* argv[] = { "clang-tidy", src, "-p", project_root, NULL };
        if (self->show_commands) {
            printf("clang-tidy %s -p %s\n", src, project_root);
        }
        int code = proc_run(argv, true);
        if (code != 0 && result == 0) {
            result = code;
            break;
        }
    }
    if (result == 0) {
        printf("Tidy check successful!\n");
    }
    return result;
}

static int dal_c_Cmd__runFormatPlan(const dal_c_Cmd* self, dal_c_Cmd__CheckPlan* plan) {
    assert(self != NULL);
    assert(plan != NULL);
    if (!dal_c_Cmd__toolAvailable("clang-format")) {
        (void)fprintf(stderr, "Error: clang-format not found in PATH\n");
        return 1;
    }
    int result = 0;
    const int total = ArrStr_len(plan->sources);
    for (int i = 0; i < total; ++i) {
        const char* src = ArrStr_at(plan->sources, i);
        if (self->show_progress) {
            printf("[%d/%d] FORMAT %s\n", i + 1, total, src);
        }
        const char* argv[] = { "clang-format", "-i", src, NULL };
        if (self->show_commands) {
            printf("clang-format -i %s\n", src);
        }
        int code = proc_run(argv, true);
        if (code != 0 && result == 0) {
            result = code;
            break;
        }
    }
    if (result == 0) {
        printf("Format successful!\n");
    }
    return result;
}

int dal_c_Cmd_runSyntax(const dal_c_Cmd* self, const dal_c_Project* proj) {
    double started_at = dal_c__phaseNowSeconds();
    dal_c_Cmd__CheckPlan plan;
    if (dal_c_Cmd__prepareCheckPlan(self, proj, &plan) != 0) {
        dal_c__phaseRecord(dal_c_CmdPhase_syntax, dal_c__phaseNowSeconds() - started_at);
        return 1;
    }
    int result = dal_c_Cmd__runSyntaxPlan(&plan);
    dal_c_Cmd__cleanupCheckPlan(&plan);
    dal_c__phaseRecord(dal_c_CmdPhase_syntax, dal_c__phaseNowSeconds() - started_at);
    return result;
}

int dal_c_Cmd_runTidy(const dal_c_Cmd* self, const dal_c_Project* proj) {
    double started_at = dal_c__phaseNowSeconds();
    dal_c_Cmd__CheckPlan plan;
    if (dal_c_Cmd__prepareCheckPlan(self, proj, &plan) != 0) {
        dal_c__phaseRecord(dal_c_CmdPhase_tidy, dal_c__phaseNowSeconds() - started_at);
        return 1;
    }
    int result = dal_c_Cmd__runTidyPlan(self, proj, &plan);
    dal_c_Cmd__cleanupCheckPlan(&plan);
    dal_c__phaseRecord(dal_c_CmdPhase_tidy, dal_c__phaseNowSeconds() - started_at);
    return result;
}

int dal_c_Cmd_runFormat(const dal_c_Cmd* self, const dal_c_Project* proj) {
    double started_at = dal_c__phaseNowSeconds();
    dal_c_Cmd__CheckPlan plan;
    if (dal_c_Cmd__prepareCheckPlan(self, proj, &plan) != 0) {
        dal_c__phaseRecord(dal_c_CmdPhase_format, dal_c__phaseNowSeconds() - started_at);
        return 1;
    }
    int result = dal_c_Cmd__runFormatPlan(self, &plan);
    dal_c_Cmd__cleanupCheckPlan(&plan);
    dal_c__phaseRecord(dal_c_CmdPhase_format, dal_c__phaseNowSeconds() - started_at);
    return result;
}

int dal_c_Cmd_writeCompileDb(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);
    assert(proj != NULL);

    if (self->payload.build.remove_output) {
        char* output_path = dal_c_Cmd__compileDbOutputPath(self, proj);
        if (!output_path) {
            return 1;
        }
        if (!path_exists(output_path)) {
            printf("Compilation database not found: %s\n", output_path);
            free(output_path);
            return 0;
        }
        if (!path_isFile(output_path)) {
            (void)fprintf(stderr, "Error: Not a file: %s\n", output_path);
            free(output_path);
            return 1;
        }
        if (remove(output_path) != 0) {
            (void)fprintf(stderr, "Error: Failed to remove compilation database: %s\n", output_path);
            free(output_path);
            return 1;
        }
        printf("Removed compilation database: %s\n", output_path);
        free(output_path);
        return 0;
    }

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    const bool build_all = intent.build_all;

    const dal_c_Project* target_proj = proj;
    dal_c_TargetRequest target_request = { 0 };
    if (!dal_c_TargetRequest_resolve(target_proj, &intent, &target_request)) {
        return 1;
    }

    ArrStr* active_excludes = intent.target_path_is_explicit_file
                                ? ArrStr_init()
                                : dal_c_Cmd__collectActiveExcludes(target_proj, &target_request, self);
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

    ArrStr* sources = ArrStr_init();
    if (target_request.root) {
        ArrStr_fini(&sources);
        sources = dal_c_Cmd__collectTargetSources(target_proj, &target_request);
        if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, target_request.raw_target_path != NULL) != 0) {
            ArrStr_fini(&sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            return 1;
        }
    } else if (self->input_count > 0) {
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

    if (ArrStr_len(sources) == 0) {
        (void)fprintf(stderr, "Error: No source files found\n");
        ArrStr_fini(&sources);
        ArrStr_fini(&active_excludes);
        dal_c_TargetRequest_cleanup(&target_request);
        return 1;
    }

    dal_c_Project* file_proj = NULL;
    const dal_c_Project* build_proj = proj;
    if (intent.target_path_is_explicit_file) {
        char* rel_to_project = (proj && proj->root) ? path_relative(proj->root, self->input_files[0]) : NULL;
        if (rel_to_project) {
            free(rel_to_project);
        } else {
            char* parent = path_parent(self->input_files[0]);
            if (!parent) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            file_proj = dal_c_Project_detectAt(parent, proj->dh_path);
            free(parent);
            if (!file_proj) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                return 1;
            }
            file_proj->pch_enabled = false;
            free(file_proj->pch_header);
            file_proj->pch_header = NULL;
            build_proj = file_proj;
        }
    }

    dal_c_Cmd effective = *self;
    memset(&effective.opts, 0, sizeof(effective.opts));
    effective.opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&effective.opts, NULL, build_proj, sources, self);
    dal_c_Cmd__appendTargetLocalInclude(&effective.opts, build_proj, self);
    if (effective.opts.profile == dal_c_Profile_invalid) {
        effective.opts.profile = self->opts.profile;
    }
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(effective.opts.profile);
    if (!profile) {
        (void)fprintf(stderr, "Error: Invalid profile\n");
        dal_c_CompilerOpts_cleanup(&effective.opts);
        dal_c_Project_cleanup(&file_proj);
        ArrStr_fini(&sources);
        ArrStr_fini(&active_excludes);
        dal_c_TargetRequest_cleanup(&target_request);
        return 1;
    }

    char* output_path = dal_c_Cmd__compileDbOutputPath(&effective, build_proj);

    int result = dal_c__writeCompileDb(&effective, build_proj, profile, sources, output_path);
    if (result == 0) {
        printf("Wrote compilation database: %s\n", output_path);
    }

    free(output_path);
    dal_c_CompilerOpts_cleanup(&effective.opts);
    dal_c_Project_cleanup(&file_proj);
    ArrStr_fini(&sources);
    ArrStr_fini(&active_excludes);
    dal_c_TargetRequest_cleanup(&target_request);
    return result;
}

static char* dal_c_Cmd__compileDbOutputPath(const dal_c_Cmd* self, const dal_c_Project* proj) {
    assert(self != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(self, &intent);
    if (intent.output_path) {
        return strdup(intent.output_path);
    }
    if (proj && proj->root) {
        return path_join(proj->root, "compile_commands.json");
    }
    char* cwd = env_getCWD();
    char* output_path = cwd ? path_join(cwd, "compile_commands.json") : strdup("compile_commands.json");
    free(cwd);
    return output_path;
}

int dal_c_Cmd_compileDeps(const dal_c_Cmd* self, const dal_c_Project* proj) {
    if (proj->lib_count > 0 && self->verbose) {
        printf("Building %d libraries...\n", proj->lib_count);
    }
    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        if (!str_eql(provider, "dh")) continue;
        if (dal_c__buildSingleLibrary(self, proj, lib) != 0) {
            return 1;
        }
    }
    if (!dal_c__writeDepsPreludeHeader(proj, &self->opts)) {
        (void)fprintf(stderr, "Error: Failed to generate dependency prelude header\n");
        return 1;
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

    char* cwd = env_getCWD();
    char* cwd_build_dir = cwd ? path_join(cwd, dal_c_dir_build) : NULL;
    bool clean_local_build = !self->payload.clean.dsl_first
                          && !recursive
                          && cwd_build_dir
                          && path_isDir(cwd_build_dir)
                          && (!proj->root || !cwd || !str_eql(cwd, proj->root));

    char* build_dir = clean_local_build ? strdup(cwd_build_dir) : (proj->root ? dal_c_Project_getBuildDir(proj) : NULL);
    if (!build_dir) {
        build_dir = cwd ? path_join(cwd, dal_c_dir_build) : strdup(dal_c_dir_build);
    }
    char* build_target = NULL;
    if (self->profile_explicit && profile) {
        char* target_name = dal_c__resolveTargetDirName(&self->opts);
        char* target_dir = (build_dir && target_name) ? path_join(build_dir, target_name) : NULL;
        build_target = target_dir ? path_join(target_dir, profile->name) : NULL;
        free(target_dir);
        free(target_name);
    } else {
        build_target = strdup(build_dir);
    }
    if (!self->profile_explicit && build_dir) {
        char* native_link = path_join(build_dir, "native");
        if (native_link) { (void)remove(native_link); }
        free(native_link);
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
    if (self->profile_explicit && profile) {
        char* legacy_profile = path_join(build_dir, profile->name);
        if (legacy_profile && path_isDir(legacy_profile)) {
            if (self->verbose) { printf("Removing legacy profile: %s\n", legacy_profile); }
            dir_removeRecur(legacy_profile);
            printf("Cleaned: %s\n", legacy_profile);
            cleaned = true;
        }
        free(legacy_profile);
    }
    free(build_dir);

    if (clean_local_build) {
        if (!cache_only) {
            int deps_result = dal_c_Cmd__cleanDependencyExportsAt(cwd, self->verbose, &cleaned);
            if (deps_result != 0) {
                free(cwd_build_dir);
                free(cwd);
                return deps_result;
            }
        }
        free(cwd_build_dir);
        free(cwd);
        if (!cleaned) {
            printf("Nothing to clean\n");
        }
        return 0;
    }
    free(cwd_build_dir);
    free(cwd);

    if (proj->root && !cache_only) {
        int deps_result = dal_c_Cmd__cleanDependencyExportsAt(proj->root, self->verbose, &cleaned);
        if (deps_result != 0) { return deps_result; }
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

    if (proj->root && recursive) {
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

int dal_c_Cmd_createWorkspace(void) {
    (void)fprintf(stderr, "Not implemented: workspace\n");
    return 1;
}

int dal_c_Cmd_createProject(void) {
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
    case dal_c_CmdAction_compile_db: return &cmd->payload.build.target_path;
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code: return &cmd->payload.build.target_path;
    case dal_c_CmdAction_lib: return &cmd->payload.lib.target_path;
    case dal_c_CmdAction_run: return &cmd->payload.run.target_path;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl: return &cmd->payload.test.target_path;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_toolchain:
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
    case dal_c_CmdAction_compile_db: return &cmd->payload.build.output_path;
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code: return &cmd->payload.build.output_path;
    case dal_c_CmdAction_lib: return &cmd->payload.lib.output_path;
    case dal_c_CmdAction_run: return &cmd->payload.run.output_path;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl: return &cmd->payload.test.output_path;
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_toolchain:
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
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_toolchain:
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

static bool dal_c_Cmd__optionNameIs(const char* opt, const char* name) {
    assert(name != NULL);
    if (!opt) { return false; }
    size_t len = strlen(name);
    return strncmp(opt, name, len) == 0
        && (opt[len] == '\0' || opt[len] == dal_c_opt_value_sep[0]);
}

static bool dal_c_Cmd__validOutputExt(const char* value) {
    return value
        && value[0] != '\0'
        && !str_eql(value, ".")
        && strchr(value, '/') == NULL
        && strchr(value, '\\') == NULL;
}

static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action) {
    bool build_like = dal_c_Cmd__usesBuildPayload(action);
    bool build_artifact_like = dal_c_Cmd__usesBuildArtifactPayload(action);
    bool syntax_like = action == dal_c_CmdAction_syntax;
    bool tidy_like = action == dal_c_CmdAction_tidy;
    bool format_like = action == dal_c_CmdAction_format_code;
    bool check_like = syntax_like || tidy_like || format_like;
    bool output_like = action == dal_c_CmdAction_build
                    || action == dal_c_CmdAction_compile_db
                    || action == dal_c_CmdAction_lib
                    || action == dal_c_CmdAction_run
                    || action == dal_c_CmdAction_test
                    || action == dal_c_CmdAction_test_dsl;
    bool artifact_output_like = action == dal_c_CmdAction_build
                             || action == dal_c_CmdAction_lib
                             || action == dal_c_CmdAction_run
                             || action == dal_c_CmdAction_test
                             || action == dal_c_CmdAction_test_dsl;
    bool link_input_like = action == dal_c_CmdAction_build
                        || action == dal_c_CmdAction_lib
                        || action == dal_c_CmdAction_run
                        || action == dal_c_CmdAction_test
                        || action == dal_c_CmdAction_test_dsl;
    if (str_startsWith(arg, dal_c_opt_prefix_long)) {
        const char* opt = arg + 2;
        if (dal_c_Cmd__optionNameIs(opt, dal_c_opt_output)) { return output_like; }
        if (dal_c_Cmd__optionNameIs(opt, dal_c_opt_output_ext)) { return artifact_output_like; }
        if (dal_c_Cmd__optionNameIs(opt, dal_c_opt_link)
            || dal_c_Cmd__optionNameIs(opt, dal_c_opt_link_dir)
            || dal_c_Cmd__optionNameIs(opt, dal_c_opt_link_args)
            || dal_c_Cmd__optionNameIs(opt, dal_c_opt_link_script)) {
            return link_input_like;
        }
        if (check_like
            && (dal_c_Cmd__optionNameIs(opt, dal_c_opt_entry)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_objcopy)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_objcopy_format)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_image)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_lib)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_static)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_shared))) {
            return false;
        }
        if (format_like) {
            return dal_c_Cmd__optionNameIs(opt, dal_c_opt_commands)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_verbose)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_progress)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_elapsed_precision)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_sample)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_example)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_test)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_all)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_recur)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_exclude)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_dh)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_file)
                || dal_c_Cmd__optionNameIs(opt, dal_c_opt_dh_file);
        }
        if ((str_eql(opt, dal_c_opt_output) && (build_like || action == dal_c_CmdAction_lib || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl))
            || str_eql(opt, dal_c_opt_commands)
            || str_eql(opt, dal_c_opt_verbose)
            || str_eql(opt, dal_c_opt_progress)
            || str_eql(opt, dal_c_opt_elapsed_precision)
            || str_eql(opt, dal_c_opt_jobs)
            || (str_eql(opt, dal_c_opt_remove) && action == dal_c_CmdAction_compile_db)
            || str_eql(opt, dal_c_opt_link_dsl)
            || (str_eql(opt, dal_c_opt_self) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_clean))
            || (str_eql(opt, dal_c_opt_lib) && (build_like || action == dal_c_CmdAction_lib))
            || ((str_eql(opt, dal_c_opt_static) || str_eql(opt, dal_c_opt_shared))
                && (build_like || action == dal_c_CmdAction_lib
                    || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test
                    || action == dal_c_CmdAction_test_dsl))
            || str_eql(opt, dal_c_opt_cache)
            || str_eql(opt, dal_c_opt_recur)
            || str_eql(opt, dal_c_opt_debug)
            || str_eql(opt, dal_c_opt_freestanding)
            || str_eql(opt, dal_c_opt_hosted)
            || str_eql(opt, dal_c_opt_link_libc)
            || str_eql(opt, dal_c_opt_link_default_libs)
            || str_eql(opt, dal_c_opt_link_start_files)
            || str_eql(opt, dal_c_opt_link_compiler_rt)
            || str_eql(opt, dal_c_opt_link_stdlib)
            || str_eql(opt, dal_c_opt_link_crt)
            || str_eql(opt, dal_c_opt_link_mode)
            || str_eql(opt, dal_c_opt_lto)
            || str_eql(opt, dal_c_opt_prebuilt)
            || str_eql(opt, dal_c_opt_omit_frame_pointer)
            || str_eql(opt, dal_c_opt_function_sections)
            || str_eql(opt, dal_c_opt_data_sections)
            || str_eql(opt, dal_c_opt_gc_sections)
            || str_eql(opt, dal_c_opt_whole_archive)
            || str_eql(opt, dal_c_opt_unroll_loops)
            || str_eql(opt, dal_c_opt_unwind_tables)
            || str_eql(opt, dal_c_opt_async_unwind_tables)
            || str_eql(opt, dal_c_opt_exceptions)
            || str_eql(opt, dal_c_opt_strip)
            || str_eql(opt, dal_c_opt_icf)
            || str_eql(opt, dal_c_opt_merge_all_constants)
            || str_eql(opt, dal_c_opt_stack_protector)
            || str_eql(opt, dal_c_opt_loose_errors)
            || (str_eql(opt, dal_c_opt_image) && action == dal_c_CmdAction_build)
            || (str_eql(opt, dal_c_opt_emit_preprocessed) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_asm) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_map) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_linked_asm) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_disasm) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_ir) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_emit_debug_info) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_save_temps) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_print_link_gc) && build_artifact_like)
            || (str_eql(opt, dal_c_opt_analysis_artifacts) && build_artifact_like)
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
        if (((str_startsWith(opt, dal_c_opt_output)) && (build_like || action == dal_c_CmdAction_lib || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl))
            || str_startsWith(opt, dal_c_opt_link_dsl)
            || str_startsWith(opt, dal_c_opt_freestanding)
            || str_startsWith(opt, dal_c_opt_hosted)
            || str_startsWith(opt, dal_c_opt_link_libc)
            || str_startsWith(opt, dal_c_opt_link_default_libs)
            || str_startsWith(opt, dal_c_opt_link_start_files)
            || str_startsWith(opt, dal_c_opt_link_compiler_rt)
            || str_startsWith(opt, dal_c_opt_link_stdlib)
            || str_startsWith(opt, dal_c_opt_link_crt)
            || str_startsWith(opt, dal_c_opt_link_mode)
            || str_startsWith(opt, dal_c_opt_lto)
            || str_startsWith(opt, dal_c_opt_prebuilt)
            || str_startsWith(opt, dal_c_opt_omit_frame_pointer)
            || str_startsWith(opt, dal_c_opt_function_sections)
            || str_startsWith(opt, dal_c_opt_data_sections)
            || str_startsWith(opt, dal_c_opt_gc_sections)
            || str_startsWith(opt, dal_c_opt_whole_archive)
            || str_startsWith(opt, dal_c_opt_unroll_loops)
            || str_startsWith(opt, dal_c_opt_unwind_tables)
            || str_startsWith(opt, dal_c_opt_async_unwind_tables)
            || str_startsWith(opt, dal_c_opt_exceptions)
            || str_startsWith(opt, dal_c_opt_strip)
            || str_startsWith(opt, dal_c_opt_icf)
            || str_startsWith(opt, dal_c_opt_merge_all_constants)
            || str_startsWith(opt, dal_c_opt_stack_protector)
            || str_startsWith(opt, dal_c_opt_loose_errors)
            || (action == dal_c_CmdAction_build && str_startsWith(opt, dal_c_opt_image))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_preprocessed))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_asm))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_map))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_linked_asm))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_disasm))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_ir))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_emit_debug_info))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_demangle))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_source))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_line_numbers))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_symbolize_operands))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_raw_insn))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_disasm_section_contents))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_save_temps))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_print_link_gc))
            || (build_artifact_like && str_startsWith(opt, dal_c_opt_analysis_artifacts))
            || str_startsWith(opt, dal_c_opt_version_core)
            || str_startsWith(opt, dal_c_opt_version_prefix)
            || str_startsWith(opt, dal_c_opt_version_suffix)
            || str_startsWith(opt, dal_c_opt_version_build)
            || str_startsWith(opt, dal_c_opt_version_record)
            || str_startsWith(opt, dal_c_opt_compiler)
            || str_startsWith(opt, dal_c_opt_std)
            || str_startsWith(opt, dal_c_opt_args)
            || str_startsWith(opt, dal_c_opt_verbose)
            || str_startsWith(opt, dal_c_opt_commands)
            || str_startsWith(opt, dal_c_opt_progress)
            || str_startsWith(opt, dal_c_opt_elapsed_precision)
            || str_startsWith(opt, dal_c_opt_jobs)
            || str_startsWith(opt, dal_c_opt_comp_args)
            || str_startsWith(opt, dal_c_opt_link_args)
            || str_startsWith(opt, dal_c_opt_target_arch)
            || str_startsWith(opt, dal_c_opt_target_tune)
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
            || str_startsWith(opt, dal_c_opt_dh_file)
            || str_startsWith(opt, dal_c_opt_jobs)) {
            return true;
        }
        return false;
    }

    if (arg[0] == dal_c_opt_prefix_short[0] && arg[1] != dal_c_opt_prefix_short[0]) {
        char c = arg[1];
        if (c == dal_c_opt_include_short_char
            || c == dal_c_opt_define_short_char
            || c == dal_c_opt_undef_short_char) {
            return !format_like;
        }
        if (c == dal_c_opt_link_short_char || c == dal_c_opt_link_dir_short_char) {
            return link_input_like;
        }
        if (c == dal_c_opt_output_short_char) {
            return output_like;
        }
    }

    if (dal_c_Cmd__isSourceOrHeader(arg)) {
        return build_like || action == dal_c_CmdAction_lib
            || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test
            || action == dal_c_CmdAction_test_dsl;
    }

    if (str_eql(arg, dal_c_opt_all_alias) || str_eql(arg, dal_c_opt_all)) {
        return build_like || action == dal_c_CmdAction_test
            || action == dal_c_CmdAction_test_dsl;
    }

    return false;
}

static int dal_c_Cmd__parseOptions(dal_c_Cmd* cmd, int argc, const char* argv[], int start_idx) {
    for (int i = start_idx; i < argc; ++i) {
        const char* arg = argv[i];
        bool build_like = dal_c_Cmd__usesBuildPayload(cmd->action);

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
                } else if (strncmp(opt, dal_c_opt_target_tune, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_tune, value);
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
                } else if (strncmp(opt, dal_c_opt_link_dir, opt_len) == 0) {
                    if (!path_isDir(value)) {
                        (void)fprintf(stderr, "Error: Directory not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__addToArray(&cmd->opts.link_dirs, &cmd->opts.link_dir_count, value);
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
                } else if (strncmp(opt, dal_c_opt_link_mode, opt_len) == 0) {
                    dal_c_LinkMode mode = dal_c_LinkMode_parse(value);
                    if (mode == dal_c_LinkMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_link_mode, value);
                        return 1;
                    }
                    cmd->opts.link_mode = mode;
                } else if (strncmp(opt, dal_c_opt_lto, opt_len) == 0) {
                    dal_c_LtoMode mode = dal_c_LtoMode_parse(value);
                    if (mode == dal_c_LtoMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_lto, value);
                        return 1;
                    }
                    cmd->opts.lto_mode = mode;
                } else if (strncmp(opt, dal_c_opt_prebuilt, opt_len) == 0) {
                    dal_c_PrebuiltMode mode = dal_c_PrebuiltMode_parse(value);
                    if (mode == dal_c_PrebuiltMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_prebuilt, value);
                        return 1;
                    }
                    cmd->opts.prebuilt_mode = mode;
                    cmd->opts.prebuilt_mode_set = true;
                } else if (strncmp(opt, dal_c_opt_icf, opt_len) == 0) {
                    dal_c_IcfMode mode = dal_c_IcfMode_parse(value);
                    if (mode == dal_c_IcfMode_auto && !str_eql(value, dal_c_icf_mode_auto)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_icf, value);
                        return 1;
                    }
                    cmd->opts.icf_mode = mode;
                } else if (strncmp(opt, dal_c_opt_merge_all_constants, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_merge_all_constants, value);
                        return 1;
                    }
                    cmd->opts.merge_all_constants = toggle;
                } else if (strncmp(opt, dal_c_opt_stack_protector, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_stack_protector, value);
                        return 1;
                    }
                    cmd->opts.stack_protector = toggle;
                } else if (strncmp(opt, dal_c_opt_emit_map, opt_len) == 0) {
                    dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_map_path, &cmd->payload.build.emit_map, value);
                } else if (strncmp(opt, dal_c_opt_emit_linked_asm, opt_len) == 0) {
                    dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_linked_asm_path, &cmd->payload.build.emit_linked_asm, value);
                } else if (strncmp(opt, dal_c_opt_emit_disasm, opt_len) == 0) {
                    dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_disasm_path, &cmd->payload.build.emit_disasm, value);
                } else if (strncmp(opt, dal_c_opt_emit_ir, opt_len) == 0) {
                    dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_ir_path, &cmd->payload.build.emit_ir, value);
                } else if (strncmp(opt, dal_c_opt_emit_debug_info, opt_len) == 0) {
                    dal_c_Cmd__setArtifactPath(
                        &cmd->payload.build.emit_debug_info_path,
                        &cmd->payload.build.emit_debug_info,
                        value
                    );
                } else if (strncmp(opt, dal_c_opt_disasm_demangle, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_demangle, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_demangle = toggle;
                } else if (strncmp(opt, dal_c_opt_disasm_source, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_source, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_source = toggle;
                } else if (strncmp(opt, dal_c_opt_disasm_line_numbers, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_line_numbers, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_line_numbers = toggle;
                } else if (strncmp(opt, dal_c_opt_disasm_symbolize_operands, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_symbolize_operands, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_symbolize_operands = toggle;
                } else if (strncmp(opt, dal_c_opt_disasm_raw_insn, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_raw_insn, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_raw_insn = toggle;
                } else if (strncmp(opt, dal_c_opt_disasm_section_contents, opt_len) == 0) {
                    dal_c_ToggleState toggle = dal_c_ToggleState_parse(value);
                    if (toggle == dal_c_ToggleState_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_disasm_section_contents, value);
                        return 1;
                    }
                    cmd->payload.build.disasm_section_contents = toggle;
                } else if (strncmp(opt, dal_c_opt_save_temps, opt_len) == 0) {
                    dal_c_SaveTempsMode mode = dal_c_SaveTempsMode_parse(value);
                    if (!str_eql(value, dal_c_save_temps_off) && mode == dal_c_SaveTempsMode_off) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_save_temps, value);
                        return 1;
                    }
                    cmd->payload.build.save_temps = mode;
                } else if (strncmp(opt, dal_c_opt_print_link_gc, opt_len) == 0) {
                    bool enabled = false;
                    if (!dal_c_Cmd__tryParseBoolValue(value, &enabled)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_print_link_gc, value);
                        return 1;
                    }
                    cmd->payload.build.print_link_gc = enabled;
                } else if (strncmp(opt, dal_c_opt_analysis_artifacts, opt_len) == 0) {
                    bool enabled = false;
                    if (!dal_c_Cmd__tryParseBoolValue(value, &enabled)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_analysis_artifacts, value);
                        return 1;
                    }
                    if (enabled) {
                        dal_c_Cmd__enableAnalysisArtifacts(cmd);
                    } else {
                        cmd->payload.build.analysis_artifacts = false;
                    }
                } else if (strncmp(opt, dal_c_opt_elapsed_precision, opt_len) == 0) {
                    int precision = 0;
                    if (!dal_c_Cmd__tryParseElapsedPrecision(value, &precision)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_elapsed_precision, value);
                        return 1;
                    }
                    cmd->elapsed_precision = precision;
                } else if (strncmp(opt, dal_c_opt_jobs, opt_len) == 0) {
                    if (value[0] == '\0') {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_jobs, value);
                        return 1;
                    }
                    char* end = NULL;
                    long parsed = strtol(value, &end, 10);
                    if (parsed < 1 || !end || *end != '\0') {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_jobs, value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->make_jobs, value);
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
                } else if (strncmp(opt, dal_c_opt_output_ext, opt_len) == 0) {
                    if (!dal_c_Cmd__validOutputExt(value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_output_ext, value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->output_ext, value);
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
                if (str_eql(opt, dal_c_opt_commands)) {
                    cmd->show_commands = true;
                } else if (str_eql(opt, dal_c_opt_output)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOutputPath(cmd, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_output_ext)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    const char* value = argv[++i];
                    if (!dal_c_Cmd__validOutputExt(value)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_output_ext, value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->output_ext, value);
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
                } else if (str_eql(opt, dal_c_opt_link_dir)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    const char* value = argv[++i];
                    if (!path_isDir(value)) {
                        (void)fprintf(stderr, "Error: Directory not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__addToArray(&cmd->opts.link_dirs, &cmd->opts.link_dir_count, value);
                } else if (str_eql(opt, dal_c_opt_target_arch)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_arch, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_target_tune)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.target_tune, argv[++i]);
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
                } else if (str_eql(opt, dal_c_opt_progress)) {
                    cmd->show_progress = true;
                } else if (str_eql(opt, dal_c_opt_elapsed_precision)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    const char* value = argv[++i];
                    int precision = 0;
                    if (!dal_c_Cmd__tryParseElapsedPrecision(value, &precision)) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_elapsed_precision, value);
                        return 1;
                    }
                    cmd->elapsed_precision = precision;
                } else if (str_eql(opt, dal_c_opt_jobs)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    const char* value = argv[++i];
                    char* end = NULL;
                    long parsed = strtol(value, &end, 10);
                    if (parsed < 1 || !end || *end != '\0') {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_jobs, value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->make_jobs, value);
                } else if (str_eql(opt, dal_c_opt_remove)) {
                    if (cmd->action == dal_c_CmdAction_compile_db) {
                        cmd->payload.build.remove_output = true;
                    }
                } else if (str_eql(opt, dal_c_opt_self)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.self_boundary = true;
                    } else if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.self_boundary = true;
                    }
                } else if (str_eql(opt, dal_c_opt_lib)) {
                    if (build_like) {
                        cmd->payload.build.as_library = true;
                    }
                } else if (str_eql(opt, dal_c_opt_image)) {
                    if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.as_image = true;
                    }
                } else if (str_eql(opt, dal_c_opt_emit_preprocessed)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.emit_preprocessed = true;
                    }
                } else if (str_eql(opt, dal_c_opt_emit_asm)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.emit_asm = true;
                    }
                } else if (str_eql(opt, dal_c_opt_emit_map)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_map_path, &cmd->payload.build.emit_map, NULL);
                    }
                } else if (str_eql(opt, dal_c_opt_emit_linked_asm)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_linked_asm_path, &cmd->payload.build.emit_linked_asm, NULL);
                    }
                } else if (str_eql(opt, dal_c_opt_emit_disasm)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_disasm_path, &cmd->payload.build.emit_disasm, NULL);
                    }
                } else if (str_eql(opt, dal_c_opt_emit_ir)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        dal_c_Cmd__setArtifactPath(&cmd->payload.build.emit_ir_path, &cmd->payload.build.emit_ir, NULL);
                    }
                } else if (str_eql(opt, dal_c_opt_emit_debug_info)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        dal_c_Cmd__setArtifactPath(
                            &cmd->payload.build.emit_debug_info_path,
                            &cmd->payload.build.emit_debug_info,
                            NULL
                        );
                    }
                } else if (str_eql(opt, dal_c_opt_print_link_gc)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.print_link_gc = true;
                    }
                } else if (str_eql(opt, dal_c_opt_analysis_artifacts)) {
                    dal_c_Cmd__enableAnalysisArtifacts(cmd);
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
                } else if (str_eql(opt, dal_c_opt_link_compiler_rt)) {
                    cmd->opts.compiler_rt_linked = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_link_stdlib)) {
                    dal_c_Cmd__setStdlibBundle(&cmd->opts, true);
                } else if (str_eql(opt, dal_c_opt_link_crt)) {
                    dal_c_Cmd__setCrtStartFiles(&cmd->opts, true);
                } else if (str_eql(opt, dal_c_opt_link_mode)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_LinkMode mode = dal_c_LinkMode_parse(argv[i + 1]);
                    if (mode == dal_c_LinkMode_invalid) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_link_mode, argv[i + 1]);
                        return 1;
                    }
                    cmd->opts.link_mode = mode;
                    ++i;
                } else if (str_eql(opt, dal_c_opt_lto)) {
                    cmd->opts.lto_mode = dal_c_LtoMode_on;
                } else if (str_eql(opt, dal_c_opt_prebuilt)) {
                    cmd->opts.prebuilt_mode = dal_c_PrebuiltMode_required;
                    cmd->opts.prebuilt_mode_set = true;
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
                } else if (str_eql(opt, dal_c_opt_exceptions)) {
                    cmd->opts.exceptions = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_strip)) {
                    cmd->opts.strip_mode = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_icf)) {
                    cmd->opts.icf_mode = dal_c_IcfMode_all;
                } else if (str_eql(opt, dal_c_opt_merge_all_constants)) {
                    cmd->opts.merge_all_constants = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_stack_protector)) {
                    cmd->opts.stack_protector = dal_c_ToggleState_enabled;
                } else if (str_eql(opt, dal_c_opt_disasm_demangle)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_demangle = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_disasm_source)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_source = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_disasm_line_numbers)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_line_numbers = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_disasm_symbolize_operands)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_symbolize_operands = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_disasm_raw_insn)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_raw_insn = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_disasm_section_contents)) {
                    if (dal_c_Cmd__usesBuildArtifactPayload(cmd->action)) {
                        cmd->payload.build.disasm_section_contents = dal_c_ToggleState_enabled;
                    }
                } else if (str_eql(opt, dal_c_opt_save_temps)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_SaveTempsMode mode = dal_c_SaveTempsMode_parse(argv[i + 1]);
                    if (!str_eql(argv[i + 1], dal_c_save_temps_off) && mode == dal_c_SaveTempsMode_off) {
                        (void)fprintf(stderr, "Error: Invalid value for `%s`: %s\n", dal_c_opt_save_temps, argv[i + 1]);
                        return 1;
                    }
                    cmd->payload.build.save_temps = mode;
                    ++i;
                } else if (str_eql(opt, dal_c_opt_loose_errors)) {
                    cmd->opts.loose_errors = dal_c_LooseErrorsMode_warn;
                } else if (str_eql(opt, dal_c_opt_entry)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.entry_symbol, argv[++i]);
                } else if (str_eql(opt, dal_c_opt_static)) {
                    cmd->opts.link_mode = dal_c_LinkMode_static;
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_static;
                    } else if (build_like) {
                        cmd->payload.build.linking = dal_c_Linking_static;
                    }
                } else if (str_eql(opt, dal_c_opt_shared)) {
                    cmd->opts.link_mode = dal_c_LinkMode_shared;
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_shared;
                    } else if (build_like) {
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
                    } else if (build_like) {
                        cmd->payload.build.recursive = true;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.recursive = true;
                    }
                } else if (str_eql(opt, dal_c_opt_dsl)) {
                    if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.dsl_first = true;
                    } else if (build_like) {
                        cmd->payload.build.dsl_first = true;
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.dsl_first = true;
                    }
                } else if (str_eql(opt, dal_c_opt_sample)) {
                    if (build_like) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_samples;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_samples;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.sample_dir = dal_c_SampleDir_samples;
                    }
                } else if (str_eql(opt, dal_c_opt_example)) {
                    if (build_like) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_examples;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_examples;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.sample_dir = dal_c_SampleDir_examples;
                    }
                } else if (str_eql(opt, dal_c_opt_test)) {
                    if (build_like) {
                        cmd->payload.build.sample_dir = dal_c_SampleDir_tests;
                    } else if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.sample_dir = dal_c_SampleDir_tests;
                    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                        cmd->payload.test.sample_dir = dal_c_SampleDir_tests;
                    }
                } else if (str_eql(opt, dal_c_opt_all)) {
                    if (build_like) {
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
            } else if (c == dal_c_opt_link_dir_short_char) {
                if (!path_isDir(value)) {
                    (void)fprintf(stderr, "Error: Directory not found: %s\n", value);
                    return 1;
                }
                dal_c_Cmd__addToArray(&cmd->opts.link_dirs, &cmd->opts.link_dir_count, value);
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
            if (build_like) {
                cmd->payload.build.build_all = true;
            } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
                cmd->payload.test.build_all = true;
            }
        } else if (build_like || cmd->action == dal_c_CmdAction_lib || cmd->action == dal_c_CmdAction_run || cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
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
        } else if (cmd->action == dal_c_CmdAction_toolchain) {
            dal_c_ToolchainQuery query = dal_c_ToolchainQuery_parse(arg);
            if (query == dal_c_ToolchainQuery_invalid) {
                (void)fprintf(stderr, "Error: Unknown toolchain query: %s\n", arg);
                return 1;
            }
            cmd->payload.toolchain.query = query;
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

static bool dal_c_Cmd__hasExplicitFileInputs(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    if (cmd->input_count == 0 || dal_c_Cmd__inputsNeedCategoryResolution(cmd)) {
        return false;
    }
    for (int i = 0; i < cmd->input_count; ++i) {
        if (!path_isFile(cmd->input_files[i])) { return false; }
    }
    return true;
}

static const char* dal_c_Cmd__basenameView(const char* path) {
    assert(path != NULL);
    const char* sep = NULL;
    for (const char* p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') { sep = p; }
    }
    return sep ? sep + 1 : path;
}

static bool dal_c_Cmd__outputNamesLibraryArtifact(const char* path) {
    if (!path || path[0] == '\0') { return false; }
    const char* base = dal_c_Cmd__basenameView(path);
    return str_endsWith(base, ".lib")
        || str_endsWith(base, ".dll")
        || str_endsWith(base, ".pyd")
        || str_endsWith(base, ".a")
        || str_endsWith(base, ".so");
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

static ArrStr* dal_c_Cmd__collectTargetSources(const dal_c_Project* proj, const dal_c_TargetRequest* request) {
    assert(request != NULL);
    if (!request->resolved_path) {
        return ArrStr_init();
    }
    char* source_root = dal_c_Cmd__targetLocalSourceRoot(request);
    if (source_root) {
        ArrStr* sources = dal_c__collectPathSourcesCached(proj, source_root, true, request->raw_target_path == NULL);
        free(source_root);
        return sources;
    }
    return dal_c__collectPathSourcesCached(proj, request->resolved_path, request->resolved_is_dir, request->raw_target_path == NULL);
}

static char* dal_c_Cmd__firstExistingChildDir(const char* root, const char* const* names) {
    assert(root != NULL);
    assert(names != NULL);
    for (int i = 0; names[i] != NULL; ++i) {
        char* child = path_join(root, names[i]);
        if (child && path_isDir(child)) {
            return child;
        }
        free(child);
    }
    return NULL;
}

static char* dal_c_Cmd__targetLocalSourceRoot(const dal_c_TargetRequest* request) {
    assert(request != NULL);
    if (!request->resolved_is_dir || !request->resolved_path) {
        return NULL;
    }
    static const char* source_names[] = {
        dal_c_dir_src,
        dal_c_dir_src_alias_source,
        dal_c_dir_src_alias_sources,
        NULL
    };
    return dal_c_Cmd__firstExistingChildDir(request->resolved_path, source_names);
}

static char* dal_c_Cmd__targetLocalIncludeRoot(const dal_c_TargetRequest* request) {
    assert(request != NULL);
    if (!request->resolved_is_dir || !request->resolved_path) {
        return NULL;
    }
    static const char* include_names[] = {
        dal_c_dir_include,
        dal_c_dir_include_alias_includes,
        dal_c_dir_include_alias_inc,
        NULL
    };
    return dal_c_Cmd__firstExistingChildDir(request->resolved_path, include_names);
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

static bool dal_c_Cmd__pathIsUnderRoot(const char* root, const char* path) {
    assert(root != NULL);
    assert(path != NULL);

    char* root_abs = path_abs(root);
    char* path_abs_value = path_abs(path);
    if (!root_abs || !path_abs_value) {
        free(root_abs);
        free(path_abs_value);
        return false;
    }

    char* rel = path_relative(root_abs, path_abs_value);
    bool ok = rel != NULL && rel[0] != '\0';
    free(rel);
    free(path_abs_value);
    free(root_abs);
    return ok;
}

static int dal_c_Cmd__cleanDependencyExportsAt(const char* root, bool verbose, bool* cleaned) {
    assert(root != NULL);
    assert(cleaned != NULL);

    char* lib_dir = path_join(root, dal_c_dir_lib);
    char* deps_dir = lib_dir ? path_join(lib_dir, dal_c_dir_deps) : NULL;
    char* deps_header = lib_dir ? path_join(lib_dir, "deps.h") : NULL;
    int result = 0;

    if (deps_dir && path_isDir(deps_dir)) {
        if (!dal_c_Cmd__pathIsUnderRoot(root, deps_dir)) {
            (void)fprintf(stderr, "Error: Refusing to clean dependency export outside project root: %s\n", deps_dir);
            result = 1;
            goto done;
        }
        if (verbose) {
            printf("Removing: %s\n", deps_dir);
        }
        if (!dir_removeRecur(deps_dir)) {
            (void)fprintf(stderr, "Error: Failed to clean dependency export: %s\n", deps_dir);
            result = 1;
            goto done;
        }
        printf("Cleaned: %s\n", deps_dir);
        *cleaned = true;
    }

    if (deps_header && path_isFile(deps_header)) {
        if (!dal_c_Cmd__pathIsUnderRoot(root, deps_header)) {
            (void)fprintf(stderr, "Error: Refusing to clean dependency prelude outside project root: %s\n", deps_header);
            result = 1;
            goto done;
        }
        if (verbose) {
            printf("Removing: %s\n", deps_header);
        }
        if (remove(deps_header) != 0) {
            (void)fprintf(stderr, "Error: Failed to clean dependency prelude: %s\n", deps_header);
            result = 1;
            goto done;
        }
        printf("Cleaned: %s\n", deps_header);
        *cleaned = true;
    }

done:
    free(deps_header);
    free(deps_dir);
    free(lib_dir);
    return result;
}

static char* dal_c_Cmd__basenameNoExt(const char* path) {
    char* base = path_basename(path);
    char* dot = strrchr(base, '.');
    if (dot && dot != base) { *dot = '\0'; }
    return base;
}

static void dal_c_Cmd__collectCompanionDHFiles(ArrStr* dh_files, ArrStr* sources) {
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

    char* target_project_dh = dal_c_Cmd__targetLocalProjectDHPath(proj, cmd);
    if (target_project_dh) {
        (void)dal_c_CompilerOpts_applyDHFile(opts, target_project_dh);
        if (defaults) {
            (void)dal_c_BuildDefaults_applyDHFile(defaults, target_project_dh);
        }
        free(target_project_dh);
    }

    ArrStr* dh_files = ArrStr_init();
    dal_c_Cmd__collectCompanionDHFiles(dh_files, sources);
    for (int i = 0; i < ArrStr_len(dh_files); ++i) {
        (void)dal_c_CompilerOpts_applyDHFile(opts, ArrStr_at(dh_files, i));
        if (defaults) {
            (void)dal_c_BuildDefaults_applyDHFile(defaults, ArrStr_at(dh_files, i));
        }
    }
    ArrStr_fini(&dh_files);

    for (int i = 0; i < cmd->explicit_dh_count; ++i) {
        (void)dal_c_CompilerOpts_applyDHFile(opts, cmd->explicit_dh_files[i]);
        if (defaults) {
            (void)dal_c_BuildDefaults_applyDHFile(defaults, cmd->explicit_dh_files[i]);
        }
    }
    dal_c_CompilerOpts_merge(opts, &cmd->opts);
}

static char* dal_c_Cmd__targetLocalProjectDHPath(const dal_c_Project* proj, const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    if (!proj || !proj->root) {
        return NULL;
    }

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    if (intent.target_path_is_explicit_file
        || (intent.target_root_name_hint == NULL && intent.target_path != NULL && path_isFile(intent.target_path))) {
        return NULL;
    }

    dal_c_TargetRequest request = { 0 };
    if (!dal_c_TargetRequest_resolve(proj, &intent, &request)) {
        return NULL;
    }

    char* project_dh = NULL;
    if (request.root && request.resolved_is_dir && request.resolved_path) {
        char* candidate = path_join(request.resolved_path, dal_c_file_detector_project);
        if (candidate && path_isFile(candidate)) {
            project_dh = candidate;
            candidate = NULL;
        }
        free(candidate);
    }

    dal_c_TargetRequest_cleanup(&request);
    return project_dh;
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

static int dal_c_Cmd__writeVersionDHFile(const char* path, const dal_c_VersionSpec* version) {
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

    bool ok = content != NULL && file_writeAtomic(path, content);
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
    int result = dal_c_Cmd__writeVersionDHFile(path, &cmd->opts.version);
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
    if (intent.output_ext) {
        (void)fprintf(stderr, "Error: `kind=lib` cannot use one output extension for both static and shared libraries: %s\n", intent.output_ext);
        return 1;
    }
    if (dal_c_Cmd__outputNamesLibraryArtifact(intent.output_path)) {
        (void)fprintf(stderr, "Error: `kind=lib` --output must name an output stem or directory, not one library artifact: %s\n", intent.output_path);
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

    if (!self->dry_run) { printf("Build successful!\n"); }
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
    return file_writeAtomic(path, content);
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
static dal_c_LtoMode dal_c_Cmd__resolveLibraryLtoMode(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources
) {
    assert(self != NULL);
    assert(sources != NULL);

    dal_c_CompilerOpts opts = { 0 };
    dal_c_BuildDefaults defaults = { 0 };
    opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&opts, &defaults, proj, sources, self);
    if (opts.profile == dal_c_Profile_invalid) {
        opts.profile = self->opts.profile;
    }
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(opts.profile);
    dal_c_LtoMode mode = opts.lto_mode != dal_c_LtoMode_auto
                       ? opts.lto_mode
                       : (profile ? profile->lto_mode : dal_c_LtoMode_off);
    dal_c_CompilerOpts_cleanup(&opts);
    dal_c_BuildDefaults_cleanup(&defaults);
    return mode;
}

static char* dal_c_Cmd__resolveLibraryOutputName(
    const dal_c_Cmd* self,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* fallback_output_name,
    bool allow_output_defaults
) {
    assert(self != NULL);
    assert(sources != NULL);
    assert(fallback_output_name != NULL);

    if (!allow_output_defaults) {
        return strdup(fallback_output_name);
    }

    dal_c_CompilerOpts opts = { 0 };
    dal_c_BuildDefaults defaults = { 0 };
    opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&opts, &defaults, proj, sources, self);
    const char* resolved = defaults.output_name && defaults.output_name[0] != '\0'
                         ? defaults.output_name
                         : fallback_output_name;
    char* result = strdup(resolved);
    dal_c_CompilerOpts_cleanup(&opts);
    dal_c_BuildDefaults_cleanup(&defaults);
    return result;
}

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
    assert(sources != NULL);
    assert(output_name != NULL);

    if (target_type != dal_c_Target_static_lib) {
        return dal_c_Cmd__buildOneFromSources(
            self, proj, sources, output_name, target_type,
            extra_compiler_args, allow_output_defaults, print_success
        );
    }

    const dal_c_LtoMode lto_mode = dal_c_Cmd__resolveLibraryLtoMode(self, proj, sources);

    dal_c_Cmd native_cmd = *self;
    native_cmd.opts.lto_mode = dal_c_LtoMode_off;
    int result = dal_c_Cmd__buildOneFromSources(
        &native_cmd, proj, sources, output_name, target_type,
        extra_compiler_args, allow_output_defaults, false
    );
    if (result != 0) { return result; }

    if (dal_c_LtoMode_isEnabled(lto_mode)) {
        dal_c_Cmd lto_cmd = *self;
        lto_cmd.opts.lto_mode = lto_mode;
        char* resolved_output_name = dal_c_Cmd__resolveLibraryOutputName(
            self, proj, sources, output_name, allow_output_defaults
        );
        char* lto_output_name = dal_c__makeLtoStaticLibraryPath(resolved_output_name);
        free(resolved_output_name);
        char* lto_output_override = NULL;
        dal_c_CommandIntent intent = { 0 };
        dal_c_Cmd_normalizeIntent(self, &intent);
        if (intent.output_path && intent.output_path[0] != '\0') {
            lto_output_override = dal_c__makeLtoStaticLibraryPath(intent.output_path);
            char** output_slot = dal_c_Cmd__outputPathSlot(&lto_cmd);
            if (output_slot) {
                *output_slot = lto_output_override;
            }
        }
        result = dal_c_Cmd__buildOneFromSources(
            &lto_cmd, proj, sources, lto_output_name, target_type,
            extra_compiler_args, false, false
        );
        free(lto_output_override);
        free(lto_output_name);
        if (result != 0) { return result; }
    }

    if (print_success && !self->dry_run) {
        printf("Build successful!\n");
        (void)fflush(stdout);
    }
    return 0;
}

static int dal_c_Cmd__buildOneFromSources(
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
    dal_c_Cmd__appendTargetLocalInclude(&effective.opts, proj, self);
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
    char* profile_dir = NULL;
    if (proj && proj->root) {
        profile_dir = dal_c__makeBuildProfileDir(proj, &effective.opts, profile);
    } else {
        char* target_name = dal_c__resolveTargetDirName(&effective.opts);
        char* target_dir = (base_build_dir && target_name) ? path_join(base_build_dir, target_name) : NULL;
        profile_dir = target_dir ? path_join(target_dir, profile->name) : NULL;
        free(target_dir);
        free(target_name);
    }
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
    dal_c_CmdPhase build_phase = dal_c_CmdPhase_project_lib_build;
    if (self->action == dal_c_CmdAction_test || self->action == dal_c_CmdAction_test_dsl) {
        build_phase = dal_c_CmdPhase_test_build;
    } else if (self->action == dal_c_CmdAction_run) {
        build_phase = dal_c_CmdPhase_run_build;
    }
    bool target_existed_before_plan = path_isFile(target_path);
    bool makefile_existed_before_plan = path_isFile(makefile_path);
    double build_started_at = dal_c_Cmd__nowSeconds();
    int plan_result = dal_c__generateMakefile(&effective, proj, profile, sources, target_path, object_dir, target_type);
    if (plan_result != dal_c_generateMakefile_success && plan_result != dal_c_generateMakefile_upToDate) {
        dal_c__phaseRecord(build_phase, dal_c_Cmd__nowSeconds() - build_started_at);
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
    int result = 0;
    if (self->dry_run) {
        printf("PLAN:\n");
        printf("  target: %s\n", target_path);
        printf("  makefile: %s\n", makefile_path);
        printf("  profile: %s\n", profile ? profile->name : "(unknown)");
        printf("  target-kind: %s\n", dal_c_Target_format(target_type));
        printf("  sources: %d\n", ArrStr_len(sources));
        printf("  state: %s\n", plan_result == dal_c_generateMakefile_upToDate ? "up-to-date" : "rebuild-required");
        if (self->explain_rebuild) {
            if (plan_result == dal_c_generateMakefile_upToDate) {
                printf("REBUILD EXPLANATION:\n  The generated build contract and Makefile already match the requested build.\n");
            } else {
                printf("REBUILD EXPLANATION:\n");
                if (!target_existed_before_plan) {
                    printf("  - output artifact is missing: %s\n", target_path);
                }
                if (!makefile_existed_before_plan) {
                    printf("  - generated Makefile was missing\n");
                } else {
                    printf("  - generated plan or one of its tracked inputs is stale\n");
                }
                char* contract_diff = dal_c__takeLastContractDiff();
                if (contract_diff) {
                    printf("  Structured contract changes:\n%s", contract_diff);
                    free(contract_diff);
                } else {
                    printf("  - source, dependency, PCH, or generated-plan timestamp changed\n");
                }
            }
        }
    } else {
        result = (plan_result == dal_c_generateMakefile_upToDate) ? 0 : dal_c__executeMake(self, makefile_path);
    }
    dal_c__phaseRecord(build_phase, dal_c_Cmd__nowSeconds() - build_started_at);
    if (!self->dry_run && result == 0 && path_isFile(target_path)) {
        if (!dal_c__writeArtifactManifest(profile_dir, &effective, profile, target_type, target_path)) {
            (void)fprintf(stderr, "Warning: Failed to write artifact manifest for %s\n", target_path);
        }
    }
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
    if (print_success && !self->dry_run) {
        printf("Build successful!\n");
        (void)fflush(stdout);
    }

    if (!self->dry_run
        && self->action == dal_c_CmdAction_build
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

    if (!self->dry_run && (self->action == dal_c_CmdAction_run
        || self->action == dal_c_CmdAction_test
        || self->action == dal_c_CmdAction_test_dsl)) {
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
        case dal_c_CmdAction_toolchain:
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
            double runtime_started_at = dal_c_Cmd__nowSeconds();
            result = dal_c__runDebugger(&runtime_cmd, proj);
            double runtime_elapsed = dal_c_Cmd__nowSeconds() - runtime_started_at;
            if (self->action == dal_c_CmdAction_run) {
                dal_c__phaseRecord(dal_c_CmdPhase_run_exec, runtime_elapsed);
            } else {
                dal_c__phaseRecord(dal_c_CmdPhase_test_run, runtime_elapsed);
            }
        } else {
            double runtime_started_at = dal_c_Cmd__nowSeconds();
            result = dal_c__runExecutable(&runtime_cmd, proj);
            double runtime_elapsed = dal_c_Cmd__nowSeconds() - runtime_started_at;
            if (self->action == dal_c_CmdAction_run) {
                dal_c__phaseRecord(dal_c_CmdPhase_run_exec, runtime_elapsed);
            } else {
                dal_c__phaseRecord(dal_c_CmdPhase_test_run, runtime_elapsed);
                printf("[TEST REPORT] status=%s exit=%d executable=%s elapsed=%.3fs\n",
                    result == 0 ? "PASS" : "FAIL", result, target_path ? target_path : "(unknown)", runtime_elapsed);
            }
        }
    }

    dal_c_CompilerOpts_cleanup(&effective.opts);
    dal_c_BuildDefaults_cleanup(&effective_defaults);
    free(compiler_args);
    free(target_path);
    return result;
}

static void dal_c_Cmd__appendTargetLocalInclude(dal_c_CompilerOpts* opts, const dal_c_Project* proj, const dal_c_Cmd* cmd) {
    assert(opts != NULL);
    assert(cmd != NULL);
    if (!proj || !proj->root) {
        return;
    }

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    if (intent.target_path_is_explicit_file
        || (intent.target_root_name_hint == NULL && intent.target_path != NULL && path_isFile(intent.target_path))) {
        return;
    }
    dal_c_TargetRequest request = { 0 };
    if (!dal_c_TargetRequest_resolve(proj, &intent, &request)) {
        return;
    }

    if (request.root) {
        char* include_dir = dal_c_Cmd__targetLocalIncludeRoot(&request);
        if (include_dir) {
            dal_c_Cmd__addToArray(&opts->include_paths, &opts->include_count, include_dir);
            free(include_dir);
        }
    }

    dal_c_TargetRequest_cleanup(&request);
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
