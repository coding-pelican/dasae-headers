#include "internal.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/ArrStr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// === PRIVATE HELPERS (Core Layer - use asserts) ===

static void dal_c__freeFileList(char** files, int file_count);
static bool dal_c__shouldSkipSourcePath(const char* path);
static ArrStr* dal_c__collectFilesWithExt(const char* dir, const char* ext, bool skip_source_paths);
static bool dal_c__copyHeaderToPathIfChanged(const char* src, const char* dst);
static bool dal_c__copyHeaderFile(const char* src, const char* out_dir);
static bool dal_c__copyHeaderRelativeTo(const char* src, const char* src_root, const char* dst_dir);
static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir);
static char* dal_c__resolveDepsTargetDir(const char* deps_dir, const char* lib_name);
static void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type, const char* target_path);
static void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Project* proj, const char* build_dir);
static char* dal_c__sourceToObjStem(const char* base, const char* src);
static bool dal_c__sourceNeedsTestMode(const dal_c_Project* proj, const char* src);
static bool dal_c__sourceUsesPchExcludedHeader(const dal_c_Project* proj, const char* src);
static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, bool use_pch, bool test_mode, bool is_third_party);
static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode, bool is_third_party);
static void dal_c__writeMakefileCompilationRules(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base);
static void dal_c__writeMakefileTargetVar(FILE* fp, const char* target_path);
static void dal_c__writeMakefileTargetRule(FILE* fp, dal_c_Target type, bool is_windows);
static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile, const char* target_path);
static char* dal_c__buildParallelFlag(void);
static bool dal_c__writeFileIfChanged(const char* path, const char* content);
static const char* dal_c__planContextDir(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_Target target_type);
static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib, const dal_c_Project* lib_proj);
static int dal_c__runDependencyTests(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
static bool dal_c__pathHasSeparator(const char* path);
static bool dal_c__pathIsAbsolute(const char* path);
static char* dal_c__makeTargetFileName(const char* name, dal_c_Target type, bool is_windows);
static char* dal_c__makePdbPath(const char* target_path);
static bool dal_c__usesAggregateTestTarget(const dal_c_Cmd* cmd);

// === PLATFORM ===

bool dal_c__platformIsWindows(void) {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

static bool dal_c__writeFileIfChanged(const char* path, const char* content) {
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

static uint64_t dal_c__hashBytes(uint64_t hash, const void* data, size_t len) {
    const unsigned char* bytes = (const unsigned char*)data;
    for (size_t i = 0; i < len; ++i) {
        hash ^= (uint64_t)bytes[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

static uint64_t dal_c__hashString(uint64_t hash, const char* value) {
    if (!value) {
        return dal_c__hashBytes(hash, "\0", 1);
    }
    return dal_c__hashBytes(dal_c__hashBytes(hash, value, strlen(value)), "\0", 1);
}

static uint64_t dal_c__hashBool(uint64_t hash, bool value) {
    const unsigned char byte = value ? 1U : 0U;
    return dal_c__hashBytes(hash, &byte, sizeof(byte));
}

// === SOURCE COLLECTION (Core Layer) ===

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_file) {
    ArrStr* sources = ArrStr_init();
    if (target_file) {
        ArrStr_push(sources, target_file);
    } else {
        assert(proj != NULL && proj->root != NULL);
        char* src_dir = dal_c_Project_getSrcDir(proj);
        ArrStr* project_sources = dal_c__collectFilesWithExt(src_dir, ".c", true);
        for (int i = 0; i < ArrStr_len(project_sources); ++i) {
            ArrStr_push(sources, ArrStr_at(project_sources, i));
        }
        ArrStr_fini(&project_sources);
        free(src_dir);

        // Include libs/BlocksRuntime/src/*.c if present (for dh library)
        char* blocks_src = path_join(proj->root, "libs/BlocksRuntime/src");
        ArrStr* block_sources = dal_c__collectFilesWithExt(blocks_src, ".c", false);
        for (int i = 0; i < ArrStr_len(block_sources); ++i) {
            ArrStr_push(sources, ArrStr_at(block_sources, i));
        }
        ArrStr_fini(&block_sources);
        free(blocks_src);
    }
    return sources;
}

ArrStr* dal_c__collectDirectoryFiles(const dal_c_Project* proj, const char* dir_name) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    assert(dir_name != NULL);

    char* dir = dal_c_Project_getCategoryDir(proj, dir_name);
    ArrStr* sources = dal_c__collectFilesWithExt(dir, ".c", false);
    free(dir);
    return sources;
}

// === BOUNDARY FUNCTIONS (Can return errors) ===

bool dal_c__isHeaderOnlyBuild(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources) {
    assert(cmd != NULL);
    assert(sources != NULL);
    if (cmd->action != dal_c_CmdAction_lib) { return false; }

    if (ArrStr_len(sources) > 0) {
        for (int i = 0; i < ArrStr_len(sources); ++i) {
            if (!str_endsWith(ArrStr_at(sources, i), ".h")) {
                return false;
            }
        }
        return true;
    }

    if (proj && proj->root) {
        char* inc_dir = dal_c_Project_getIncludeDir(proj);
        bool has_inc = path_isDir(inc_dir);
        free(inc_dir);
        return has_inc;
    }

    return false;
}

int dal_c__buildHeaderOnlyLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* output_name) {
    assert(cmd != NULL);
    assert(output_name != NULL);
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* target_file = intent.target_file;

    // Boundary: proj may be NULL for standalone header file
    if (!proj || !proj->root) {
        if (target_file && str_endsWith(target_file, ".h")) {
            if (cmd->verbose) {
                printf("Header-only library: %s (no project structure)\n", target_file);
            }
            return 0;
        }
        (void)fprintf(stderr, "Error: Cannot build header-only library without project root\n");
        return 1;
    }

    char* lib_dir = path_join(proj->root, dal_c_dir_lib);
    char* deps_dir = path_join(lib_dir, dal_c_dir_deps);
    char* lib_deps = path_join(deps_dir, output_name);
    char* deps_inc = path_join(lib_deps, dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
    char* include_dir = dal_c_Project_getIncludeDir(proj);
    free(lib_dir);
    free(deps_dir);

    if (target_file && str_endsWith(target_file, ".h")) {
        if (!dal_c__copyHeaderRelativeTo(target_file, include_dir, deps_inc)) {
            (void)fprintf(stderr, "Error: Failed to copy header: %s\n", target_file);
            free(include_dir);
            free(deps_inc);
            free(lib_deps);
            return 1;
        }
    } else {
        ArrStr* headers = dal_c__collectFilesWithExt(include_dir, ".h", false);
        if (ArrStr_len(headers) == 0) {
            (void)fprintf(stderr, "Error: No header files found\n");
            ArrStr_fini(&headers);
            free(include_dir);
            free(deps_inc);
            free(lib_deps);
            return 1;
        }
        for (int i = 0; i < ArrStr_len(headers); ++i) {
            const char* header = ArrStr_at(headers, i);
            if (!dal_c__copyHeaderRelativeTo(header, include_dir, deps_inc)) {
                (void)fprintf(stderr, "Error: Failed to copy header: %s\n", header);
                ArrStr_fini(&headers);
                free(include_dir);
                free(deps_inc);
                free(lib_deps);
                return 1;
            }
        }
        ArrStr_fini(&headers);
    }

    free(include_dir);
    free(deps_inc);
    free(lib_deps);
    if (cmd->verbose) {
        printf("Header-only library built: %s\n", output_name);
    }
    return 0;
}

static bool dal_c__copyLibraryArtifacts(const dal_c_Project* consumer_proj, const dal_c_Project* lib_proj, const dal_c_Lib* lib, const char* lib_abs_path, const char* lib_build_dir, bool is_windows);
/* NOLINTNEXTLINE(misc-no-recursion) */
int dal_c__buildSingleLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_Lib* lib) {
    assert(cmd != NULL);
    assert(proj != NULL);
    assert(lib != NULL);
    if (!lib->path) {
        (void)fprintf(stderr, "Error: Library %s has no path specified\n", lib->name);
        return 1;
    }

    if (cmd->verbose) {
        printf("Building library: %s\n", lib->name);
    }

    // 1. Recursively build the library's own dependencies first
    dal_c_Project* lib_proj = dal_c_Project_detectAt(lib->path, proj->dh_path);
    if (lib->test_enabled && !lib_proj) {
        (void)fprintf(stderr, "Error: Dependency %s enables test=true but has no project.dh\n", lib->name);
        return 1;
    }
    if (lib_proj && lib_proj->lib_count > 0) {
        if (cmd->verbose) {
            printf("Building %d dependencies for %s...\n", lib_proj->lib_count, lib->name);
        }
        for (int i = 0; i < lib_proj->lib_count; ++i) {
            if (dal_c__buildSingleLibrary(cmd, lib_proj, &lib_proj->libraries[i]) != 0) {
                dal_c_Project_cleanup(&lib_proj);
                return 1;
            }
        }
    }

    dal_c_Profile lib_profile_enum = (lib->opts.profile != dal_c_Profile_invalid)
                                       ? lib->opts.profile
                                       : cmd->opts.profile;
    dal_c_Cmd merged = *cmd;
    memset(&merged.opts, 0, sizeof(merged.opts));
    merged.opts.profile = dal_c_Profile_invalid;
    if (lib_proj) {
        dal_c_CompilerOpts_merge(&merged.opts, &lib_proj->opts);
    }
    dal_c_CompilerOpts_merge(&merged.opts, &cmd->opts);
    dal_c_CompilerOpts_merge(&merged.opts, &lib->opts);
    if (merged.opts.profile == dal_c_Profile_invalid) {
        merged.opts.profile = lib_profile_enum;
    }
    int result = 0;

    // 2. Handle header-only libraries
    ArrStr* lib_sources = dal_c__collectLibrarySources(lib, lib_proj);
    bool is_single_header = str_endsWith(lib->path, ".h") && path_isFile(lib->path);
    const char* lib_header_root = lib_proj ? lib_proj->root : lib->path;
    const char* include_dir_name = lib_proj ? dal_c_Project_getCategoryDirName(lib_proj, dal_c_dir_include) : dal_c_dir_include;
    char* lib_inc = path_join(lib_header_root, include_dir_name);
    bool has_include_dir = path_isDir(lib_inc);
    bool is_header_only = ArrStr_len(lib_sources) == 0 && (is_single_header || has_include_dir);
    if (is_header_only) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        dir_createRecur(deps_dir);
        char* target_dir = dal_c__resolveDepsTargetDir(deps_dir, lib->name);
        dir_createRecur(target_dir);

        bool copy_ok = true;
        if (is_single_header) {
            copy_ok = dal_c__copyHeaderRelativeTo(lib->path, NULL, target_dir);
            if (!copy_ok) {
                (void)fprintf(stderr, "Error: Failed to copy header file: %s\n", lib->path);
            }
        } else {
            ArrStr* headers = dal_c__collectFilesWithExt(lib_inc, ".h", false);
            if (ArrStr_len(headers) == 0) {
                copy_ok = false;
            } else {
                for (int i = 0; i < ArrStr_len(headers); ++i) {
                    if (!dal_c__copyHeaderRelativeTo(ArrStr_at(headers, i), lib_inc, target_dir)) {
                        copy_ok = false;
                        break;
                    }
                }
            }
            ArrStr_fini(&headers);
        }

        if (!copy_ok) {
            (void)fprintf(stderr, "Error: Failed to stage header-only library: %s\n", lib->name);
            free(target_dir);
            free(deps_dir);
            free(lib_inc);
            ArrStr_fini(&lib_sources);
            dal_c_CompilerOpts_cleanup(&merged.opts);
            dal_c_Project_cleanup(&lib_proj);
            return 1;
        }

        free(target_dir);
        free(deps_dir);
        free(lib_inc);
        ArrStr_fini(&lib_sources);
        if (lib->test_enabled && lib_proj) {
            result = dal_c__runDependencyTests(cmd, lib_proj, &merged.opts);
            dal_c_CompilerOpts_cleanup(&merged.opts);
            dal_c_Project_cleanup(&lib_proj);
            return result;
        }
        dal_c_CompilerOpts_cleanup(&merged.opts);
        dal_c_Project_cleanup(&lib_proj);
        if (cmd->verbose) {
            printf("Header-only library %s: headers copied\n", lib->name);
        }
        return 0;
    }

    free(lib_inc);

    // 3. Use sources collected from library's own src/ directory
    if (ArrStr_len(lib_sources) == 0) {
        (void)fprintf(stderr, "Error: Library %s has no source files\n", lib->name);
        ArrStr_fini(&lib_sources);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }

    // 4. Build in library's own build/<profile>/ directory
    const dal_c_ProfileSpec* lib_profile = dal_c_ProfileSpec_by(lib_profile_enum);
    assert(lib_profile != NULL);

    // Use absolute path from lib_proj (or convert lib->path if no lib_proj)
    char* lib_abs_path = lib_proj ? lib_proj->root : lib->path;
    char* lib_build_dir = path_join(lib_abs_path, dal_c_dir_build);
    char* lib_build_profile = path_join(lib_build_dir, lib_profile->name);
    free(lib_build_dir);
    dir_createRecur(lib_build_profile);

    // Use the detected lib_proj for Makefile generation (has absolute paths)
    dal_c_Project build_proj = {
        .root = lib_abs_path,
        .name = lib->name,
        .dh_path = proj->dh_path,
        .project_dh = lib_proj ? lib_proj->project_dh : NULL,
        .src_dir_name = lib_proj ? lib_proj->src_dir_name : NULL,
        .include_dir_name = lib_proj ? lib_proj->include_dir_name : NULL,
        .tests_dir_name = lib_proj ? lib_proj->tests_dir_name : NULL,
        .samples_dir_name = lib_proj ? lib_proj->samples_dir_name : NULL,
        .examples_dir_name = lib_proj ? lib_proj->examples_dir_name : NULL,
        .pch_enabled = lib_proj ? lib_proj->pch_enabled : true,
        .pch_header_override = lib_proj ? lib_proj->pch_header_override : NULL,
        .pch_header = lib_proj ? lib_proj->pch_header : NULL,
        .pch_exclude_headers = lib_proj ? lib_proj->pch_exclude_headers : NULL,
        .pch_exclude_count = lib_proj ? lib_proj->pch_exclude_count : 0,
        .opts = lib_proj ? lib_proj->opts : (dal_c_CompilerOpts){ 0 },
        .defaults = lib_proj ? lib_proj->defaults : (dal_c_BuildDefaults){ 0 },
        .libraries = lib_proj ? lib_proj->libraries : NULL,
        .lib_count = lib_proj ? lib_proj->lib_count : 0,
    };

    dal_c_Target lib_target_type = lib->is_static ? dal_c_Target_static_lib : dal_c_Target_shared_lib;
    char* lib_target_path = dal_c__resolveOutputPath(&build_proj, &merged, lib_build_profile, lib->name, lib_target_type);
    char* lib_object_dir = path_join(lib_build_profile, "obj");
    dir_createRecur(lib_object_dir);
    char* lib_makefile_path = dal_c__makePlanFilePath(&build_proj, lib_profile, &merged, lib_target_path, lib_target_type);
    if (dal_c__generateMakefile(&merged, &build_proj, lib_profile, lib_sources, lib_target_path, lib_object_dir, lib_target_type) != 0) {
        (void)fprintf(stderr, "Error: Failed to generate Makefile for library: %s\n", lib->name);
        ArrStr_fini(&lib_sources);
        free(lib_makefile_path);
        free(lib_object_dir);
        free(lib_target_path);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        free(lib_build_profile);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }
    free(lib_target_path);

    result = dal_c__executeMake(lib_makefile_path);
    free(lib_makefile_path);
    free(lib_object_dir);
    ArrStr_fini(&lib_sources);
    if (result != 0) {
        (void)fprintf(stderr, "Error: Failed to build library: %s\n", lib->name);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        free(lib_build_profile);
        dal_c_Project_cleanup(&lib_proj);
        return result;
    }

    // 5. Copy artifacts (headers, libs, PCH) to consumer's lib/deps/
    bool is_windows = dal_c__platformIsWindows();
    if (!dal_c__copyLibraryArtifacts(proj, lib_proj, lib, lib_abs_path, lib_build_profile, is_windows)) {
        (void)fprintf(stderr, "Warning: Failed to copy some artifacts for %s\n", lib->name);
    }

    if (lib->test_enabled && lib_proj) {
        result = dal_c__runDependencyTests(cmd, lib_proj, &merged.opts);
        if (result != 0) {
            dal_c_CompilerOpts_cleanup(&merged.opts);
            free(lib_build_profile);
            dal_c_Project_cleanup(&lib_proj);
            return result;
        }
    }

    dal_c_CompilerOpts_cleanup(&merged.opts);
    free(lib_build_profile);
    dal_c_Project_cleanup(&lib_proj);
    return 0;
}

static int dal_c__runDependencyTests(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_CompilerOpts* opts) {
    assert(cmd != NULL);
    assert(proj != NULL);

    dal_c_Cmd test_cmd = *cmd;
    test_cmd.action = dal_c_CmdAction_test;
    memset(&test_cmd.payload, 0, sizeof(test_cmd.payload));
    test_cmd.input_files = NULL;
    test_cmd.input_count = 0;
    if (opts) {
        test_cmd.opts = *opts;
    }
    return dal_c_Cmd_makeTarget(&test_cmd, proj);
}

int dal_c__runExecutable(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    assert(proj != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* base_build_dir = dal_c_Project_getBuildDir(proj);
    char* build_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    assert(build_dir != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* target_file = dal_c__usesAggregateTestTarget(cmd) ? NULL : intent.target_file;

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_file) {
        char* basename = path_basename(target_file);
        char* dot = strrchr(basename, '.');
        if (dot) {
            exe_name_alloc = strdup(basename);
            exe_name_alloc[dot - basename] = '\0';
            exe_name = exe_name_alloc;
        } else {
            exe_name_alloc = basename;
            exe_name = exe_name_alloc;
            basename = NULL;
        }
        free(basename);
    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
        exe_name = "test";
    } else {
        exe_name = proj->name;
    }
    assert(exe_name != NULL);

    char* exe_path = dal_c__resolveOutputPath(proj, cmd, build_dir, exe_name, dal_c_Target_executable);
    free(build_dir);
    if (!path_exists(exe_path)) {
        (void)fprintf(stderr, "Error: Executable not found: %s\n", exe_path);
        if (exe_name_alloc) { free(exe_name_alloc); }
        free(exe_path);
        return 1;
    }

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, exe_path);

    const char* run_args = intent.run_args;
    if (run_args) {
        int count = 0;
        char** split = str_split(run_args, " ", &count);
        for (int i = 0; i < count; ++i) {
            if (strlen(split[i]) > 0) {
                ArrStr_push(argv, split[i]);
            }
        }
    }

    if (cmd->show_commands) {
        printf("Running: ");
        for (int i = 0; i < ArrStr_len(argv); ++i) {
            printf("%s ", ArrStr_at(argv, i));
        }
        printf("\n");
    }

    int result = proc_run(ArrStr_toRaw(argv), true);
    ArrStr_fini(&argv);
    if (exe_name_alloc) { free(exe_name_alloc); }
    free(exe_path);
    return result;
}

int dal_c__runDebugger(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    assert(proj != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* base_build_dir = dal_c_Project_getBuildDir(proj);
    char* build_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    assert(build_dir != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* target_file = dal_c__usesAggregateTestTarget(cmd) ? NULL : intent.target_file;

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_file) {
        char* basename = path_basename(target_file);
        char* dot = strrchr(basename, '.');
        if (dot) {
            exe_name_alloc = strdup(basename);
            exe_name_alloc[dot - basename] = '\0';
            exe_name = exe_name_alloc;
        } else {
            exe_name_alloc = basename;
            exe_name = exe_name_alloc;
            basename = NULL;
        }
        free(basename);
    } else if (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl) {
        exe_name = "test";
    } else {
        exe_name = proj->name;
    }
    assert(exe_name != NULL);

    char* exe_path = dal_c__resolveOutputPath(proj, cmd, build_dir, exe_name, dal_c_Target_executable);
    free(build_dir);
    assert(exe_path != NULL);

    if (!path_exists(exe_path)) {
        (void)fprintf(stderr, "Error: Executable not found: %s\n", exe_path);
        if (exe_name_alloc) { free(exe_name_alloc); }
        free(exe_path);
        return 1;
    }

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c_tool_debugger);
    ArrStr_push(argv, exe_path);

    const char* run_args = intent.run_args;
    if (run_args) {
        int count = 0;
        char** split = str_split(run_args, " ", &count);
        for (int i = 0; i < count; ++i) {
            if (strlen(split[i]) > 0) {
                ArrStr_push(argv, split[i]);
            }
        }
    }

    if (cmd->show_commands) {
        printf("Debugging: ");
        for (int i = 0; i < ArrStr_len(argv); ++i) {
            printf("%s ", ArrStr_at(argv, i));
        }
        printf("\n");
    }

    int result = proc_run(ArrStr_toRaw(argv), true);
    ArrStr_fini(&argv);
    if (exe_name_alloc) { free(exe_name_alloc); }
    free(exe_path);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c__ensureLibDH(const dal_c_Project* proj, const dal_c_ProfileSpec* profile) {
    if (!proj || !proj->dh_path) { return 0; }
    dal_c_Project* dh_proj = dal_c_Project_detectAt(proj->dh_path, NULL);
    if (!dh_proj) {
        (void)fprintf(stderr, "Error: Failed to detect DH project at %s\n", proj->dh_path);
        return 1;
    }

    ArrStr* sources = dal_c__collectSourceFiles(dh_proj, NULL);
    if (ArrStr_len(sources) == 0) {
        (void)fprintf(stderr, "Error: No source files found for libdh\n");
        ArrStr_fini(&sources);
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }

    char* build_dir = dal_c_Project_getBuildDir(dh_proj);
    char* profile_dir = path_join(build_dir, profile->name);
    char* object_dir = path_join(profile_dir, "obj");
    dir_createRecur(object_dir);

    dal_c_Cmd cmd = { 0 };
    cmd.opts.profile = dal_c_Profile_invalid;
    dal_c_CompilerOpts_merge(&cmd.opts, &dh_proj->opts);
    cmd.opts.no_dsl = true;
    if (cmd.opts.profile == dal_c_Profile_invalid) {
        cmd.opts.profile = dal_c_Profile_parse(profile->name);
    }

    char* lib_target_path = dal_c__resolveOutputPath(dh_proj, &cmd, profile_dir, "dh", dal_c_Target_static_lib);
    char* makefile_path = dal_c__makePlanFilePath(dh_proj, profile, &cmd, lib_target_path, dal_c_Target_static_lib);
    int result = dal_c__generateMakefile(&cmd, dh_proj, profile, sources, lib_target_path, object_dir, dal_c_Target_static_lib);
    if (result == 0) {
        result = dal_c__executeMake(makefile_path);
    }

    free(makefile_path);
    free(lib_target_path);
    dal_c_CompilerOpts_cleanup(&cmd.opts);
    free(object_dir);
    free(profile_dir);
    free(build_dir);
    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&dh_proj);
    return result;
}

int dal_c__buildDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    if (!proj || !proj->dh_path) {
        (void)fprintf(stderr, "Error: DH root not found\n");
        return 1;
    }

    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    dal_c_Project dh_proj = {
        .dh_path = proj->dh_path,
    };
    if (cmd->verbose) {
        printf("Building DSL at %s\n", proj->dh_path);
    }
    return dal_c__ensureLibDH(&dh_proj, profile);
}

int dal_c__cleanDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    if (!proj || !proj->dh_path) {
        (void)fprintf(stderr, "Error: DH root not found\n");
        return 1;
    }

    bool cleaned = false;
    char* dh_build = path_join(proj->dh_path, dal_c_dir_build);
    if (path_isDir(dh_build)) {
        if (cmd->verbose) {
            printf("Removing: %s\n", dh_build);
        }
        dir_removeRecur(dh_build);
        printf("Cleaned: %s\n", dh_build);
        cleaned = true;
    }
    free(dh_build);

    char* dh_cache = path_join(proj->dh_path, dal_c_dir_cache);
    if (path_isDir(dh_cache)) {
        if (cmd->verbose) {
            printf("Removing: %s\n", dh_cache);
        }
        dir_removeRecur(dh_cache);
        printf("Cleaned: %s\n", dh_cache);
        cleaned = true;
    }
    free(dh_cache);

    if (!cleaned) {
        printf("Nothing to clean\n");
    }
    return 0;
}

int dal_c__testDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    if (!proj || !proj->dh_path) {
        (void)fprintf(stderr, "Error: DH root not found\n");
        return 1;
    }

    dal_c_Project* dh_proj = dal_c_Project_detectAt(proj->dh_path, proj->dh_path);
    if (!dh_proj) {
        (void)fprintf(stderr, "Error: Failed to detect DH project for `%s`\n", dal_c_cmd_action_test_dsl);
        return 1;
    }

    dal_c_Cmd test_cmd = *cmd;
    test_cmd.action = dal_c_CmdAction_test;
    test_cmd.payload.test.dsl_first = false;

    int result = dal_c_Cmd_makeTarget(&test_cmd, dh_proj);
    dal_c_Project_cleanup(&dh_proj);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
int dal_c__generateMakefile(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type
) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(target_path != NULL);
    assert(build_dir != NULL);

    // Auto-build libdh if needed and not disabled
    if (!cmd->opts.no_dsl && proj && proj->dh_path) {
        if (dal_c__ensureLibDH(proj, profile) != 0) {
            (void)fprintf(stderr, "Error: Failed to build libdh\n");
            return 1;
        }
    }

    char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, target_type);
    char* makefile_dir = path_parent(makefile_path);
    dir_createRecur(makefile_dir);
    char* makefile_tmp = str_format("%s.tmp", makefile_path);

    FILE* fp = fopen(makefile_tmp, "w");
    if (!fp) {
        (void)fprintf(stderr, "Error: Failed to open Makefile for writing: %s\n", makefile_tmp);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }

    bool is_windows = dal_c__platformIsWindows();
    bool has_pch = (proj && proj->pch_header != NULL);

    dal_c__writeMakefileVariables(fp, cmd, profile, proj, build_dir, target_type, target_path);

    // Declare default goal before any rules to ensure 'all' is the default target
    (void)fprintf(fp, ".DEFAULT_GOAL := all\n\n");

    dal_c__writeMakefilePCH(fp, proj, build_dir);

    (void)fprintf(fp, "SRCS =");
    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        (void)fprintf(fp, " %s", ArrStr_at(sources, i));
    }
    (void)fprintf(fp, "\n");

    const char* obj_base = (proj && proj->root) ? proj->root : NULL;
    (void)fprintf(fp, "OBJS =");
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        bool is_third_party = strstr(src, "libs/") || strstr(src, "libs\\");
        bool use_pch = has_pch && !is_third_party && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        bool test_mode = !is_third_party && dal_c__sourceNeedsTestMode(proj, src);
        char* obj_path = dal_c__makeObjectPath(cmd, profile, build_dir, obj_base, src, use_pch, test_mode, is_third_party);
        (void)fprintf(fp, " %s", obj_path);
        free(obj_path);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "DEPS = $(OBJS:.o=.d)\n\n");

    // Define TARGET before all: $(TARGET) so make knows the dependency
    dal_c__writeMakefileTargetVar(fp, target_path);
    (void)fprintf(fp, "all: $(TARGET)\n\n");

    dal_c__writeMakefileCompilationRules(fp, cmd, proj, profile, sources, has_pch, build_dir, obj_base);
    dal_c__writeMakefileTargetRule(fp, target_type, is_windows);

    (void)fprintf(fp, "clean:\n\trm -f $(TARGET)\n\n");
    (void)fprintf(fp, "-include $(DEPS)\n");

    (void)fclose(fp);
    char* generated = file_read(makefile_tmp);
    if (!generated || !dal_c__writeFileIfChanged(makefile_path, generated)) {
        free(generated);
        (void)remove(makefile_tmp);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }
    free(generated);
    (void)remove(makefile_tmp);
    free(makefile_tmp);
    free(makefile_dir);
    free(makefile_path);
    return 0;
}

int dal_c__executeMake(const char* makefile_path) {
    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c_tool_make);

    char* j_flag = dal_c__buildParallelFlag();
    ArrStr_push(argv, j_flag);
    free(j_flag);

    if (makefile_path) {
        ArrStr_push(argv, "-f");
        ArrStr_push(argv, makefile_path);
    }

    int result = proc_run(ArrStr_toRaw(argv), true);
    ArrStr_fini(&argv);
    return result;
}

int dal_c__executeMakeInDir(const char* directory) {
    assert(directory != NULL);

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c_tool_make);

    char* j_flag = dal_c__buildParallelFlag();
    ArrStr_push(argv, j_flag);
    free(j_flag);

    ArrStr_push(argv, "-C");
    ArrStr_push(argv, directory);

    int result = proc_run(ArrStr_toRaw(argv), true);
    ArrStr_fini(&argv);
    return result;
}

void dal_c__printVerbose(const dal_c_Cmd* cmd, const char* fmt, ...) {
    assert(cmd != NULL);
    assert(fmt != NULL);

    if (!cmd->verbose) { return; }
    va_list args = dsl_l0$((va_list));
    va_start(args, fmt);
    (void)vprintf(fmt, args);
    va_end(args);
}

void dal_c__printError(const char* fmt, ...) {
    assert(fmt != NULL);

    va_list args = dsl_l0$((va_list));
    va_start(args, fmt);
    (void)vfprintf(stderr, fmt, args);
    va_end(args);
}

// === PRIVATE IMPLEMENTATIONS (Core Layer - use asserts) ===

static bool dal_c__usesAggregateTestTarget(const dal_c_Cmd* cmd) {
    return cmd != NULL
        && (cmd->action == dal_c_CmdAction_test || cmd->action == dal_c_CmdAction_test_dsl)
        && cmd->input_count > 1;
}

static bool dal_c__pathHasSeparator(const char* path) {
    return path != NULL && (strchr(path, '/') != NULL || strchr(path, '\\') != NULL);
}

static bool dal_c__pathIsAbsolute(const char* path) {
    if (!path || path[0] == '\0') { return false; }
#ifdef _WIN32
    return (strlen(path) >= 2 && path[1] == ':')
        || (path[0] == '\\' && path[1] == '\\')
        || path[0] == '/'
        || path[0] == '\\';
#else
    return path[0] == '/';
#endif
}

static char* dal_c__makeTargetFileName(const char* name, dal_c_Target type, bool is_windows) {
    assert(name != NULL);
    switch (type) {
    case dal_c_Target_invalid:
        break;
    case dal_c_Target_executable:
        if (is_windows && !str_endsWith(name, ".exe")) {
            return str_format("%s.exe", name);
        }
        return strdup(name);
    case dal_c_Target_static_lib:
        if (is_windows) {
            return str_endsWith(name, ".lib") ? strdup(name) : str_format("%s.lib", name);
        }
        if (str_endsWith(name, ".a")) { return strdup(name); }
        return str_startsWith(name, "lib") ? str_format("%s.a", name) : str_format("lib%s.a", name);
    case dal_c_Target_shared_lib:
        if (is_windows) {
            return str_endsWith(name, ".dll") ? strdup(name) : str_format("%s.dll", name);
        }
        if (str_endsWith(name, ".so")) { return strdup(name); }
        return str_startsWith(name, "lib") ? str_format("%s.so", name) : str_format("lib%s.so", name);
    }
    assert(false && "invalid target type");
    return strdup(name);
}

char* dal_c__resolveOutputPath(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* build_dir, const char* output_name, dal_c_Target type) {
    assert(proj != NULL || type != dal_c_Target_executable);
    assert(cmd != NULL);
    assert(build_dir != NULL);
    assert(output_name != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* output_override = intent.output_path;
    bool is_windows = dal_c__platformIsWindows();
    if (!output_override || output_override[0] == '\0') {
        char* file_name = dal_c__makeTargetFileName(output_name, type, is_windows);
        char* output_root = strdup(build_dir);
        if (type == dal_c_Target_executable) {
            const char* context_dir = dal_c__planContextDir(proj, &intent, type);
            if (context_dir && !str_eql(context_dir, "targets")) {
                char* nested = path_join(output_root, context_dir);
                free(output_root);
                output_root = nested;
            }
        }
        char* output_path = path_join(output_root, file_name);
        free(output_root);
        free(file_name);
        return output_path;
    }

    if (!dal_c__pathHasSeparator(output_override)) {
        char* file_name = dal_c__makeTargetFileName(output_override, type, is_windows);
        char* output_root = strdup(build_dir);
        if (type == dal_c_Target_executable) {
            const char* context_dir = dal_c__planContextDir(proj, &intent, type);
            if (context_dir && !str_eql(context_dir, "targets")) {
                char* nested = path_join(output_root, context_dir);
                free(output_root);
                output_root = nested;
            }
        }
        char* output_path = path_join(output_root, file_name);
        free(output_root);
        free(file_name);
        return output_path;
    }

    if (dal_c__pathIsAbsolute(output_override)) {
        return strdup(output_override);
    }

    char* cwd = env_getCWD();
    char* output_path = cwd ? path_join(cwd, output_override) : strdup(output_override);
    free(cwd);
    return output_path;
}

static char* dal_c__makePdbPath(const char* target_path) {
    assert(target_path != NULL);
    char* basename = path_basename(target_path);
    char* stem = strdup(basename);
    char* dot = strrchr(stem, '.');
    if (dot) { *dot = '\0'; }
    char* pdb_name = str_format("%s.pdb", stem);
    char* parent = path_parent(target_path);
    char* pdb_path = parent ? path_join(parent, pdb_name) : strdup(pdb_name);
    free(parent);
    free(pdb_name);
    free(stem);
    free(basename);
    return pdb_path;
}

static const char* dal_c__planContextDir(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_Target target_type) {
    assert(intent != NULL);
    if (target_type == dal_c_Target_static_lib || target_type == dal_c_Target_shared_lib) {
        return "libs";
    }
    if (intent->sample_dir == dal_c_SampleDir_samples) {
        return proj ? dal_c_Project_getCategoryDirName(proj, dal_c_dir_samples) : dal_c_dir_samples;
    }
    if (intent->sample_dir == dal_c_SampleDir_examples) {
        return proj ? dal_c_Project_getCategoryDirName(proj, dal_c_dir_examples) : dal_c_dir_examples;
    }
    if (intent->sample_dir == dal_c_SampleDir_tests
        || intent->action == dal_c_CmdAction_test
        || intent->action == dal_c_CmdAction_test_dsl) {
        return proj ? dal_c_Project_getCategoryDirName(proj, dal_c_dir_tests) : dal_c_dir_tests;
    }
    if (intent->action == dal_c_CmdAction_build_dsl) {
        return "dsl";
    }
    return "targets";
}

static char* dal_c__sanitizePathFragment(const char* value) {
    assert(value != NULL);
    char* copy = strdup(value);
    for (char* p = copy; *p; ++p) {
        if (*p == '/' || *p == '\\' || *p == ':' || *p == ' ') {
            *p = '_';
        }
    }
    return copy;
}

char* dal_c__makePlanFilePath(const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const dal_c_Cmd* cmd, const char* target_path, dal_c_Target target_type) {
    assert(profile != NULL);
    assert(cmd != NULL);
    assert(target_path != NULL);

    char* base_dir = NULL;
    if (proj && proj->root) {
        base_dir = dal_c_Project_getBuildDir(proj);
    } else {
        char* cwd = env_getCWD();
        base_dir = cwd ? path_join(cwd, dal_c_dir_build) : strdup(dal_c_dir_build);
        free(cwd);
    }
    char* profile_dir = path_join(base_dir, profile->name);
    char* plans_dir = path_join(profile_dir, ".plans");
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    char* context_dir = path_join(plans_dir, dal_c__planContextDir(proj, &intent, target_type));
    char* target_base = path_basename(target_path);
    char* target_name = dal_c__sanitizePathFragment(target_base);
    char* plan_name = str_format("%s.mk", target_name);
    char* plan_path = path_join(context_dir, plan_name);
    free(plan_name);
    free(target_name);
    free(target_base);
    free(context_dir);
    free(plans_dir);
    free(profile_dir);
    free(base_dir);
    return plan_path;
}

static void dal_c__freeFileList(char** files, int file_count) {
    if (!files) { return; }
    for (int i = 0; i < file_count; ++i) {
        free(files[i]);
    }
    free((void*)files);
}

static bool dal_c__shouldSkipSourcePath(const char* path) {
    assert(path != NULL);
    return strstr(path, "build") != NULL
        || strstr(path, "tmp") != NULL
        || strstr(path, "temp") != NULL
        || strstr(path, "cache") != NULL
        || strstr(path, "mock") != NULL
        || strstr(path, "draft") != NULL
        || strstr(path, "bak") != NULL
        || strstr(path, "backup") != NULL
        || strstr(path, "archive") != NULL;
}

static ArrStr* dal_c__collectFilesWithExt(const char* dir, const char* ext, bool skip_source_paths) {
    assert(dir != NULL);
    assert(ext != NULL);

    ArrStr* files_with_ext = ArrStr_init();
    if (!path_isDir(dir)) { return files_with_ext; }

    int file_count = 0;
    char** files = dir_listRecur(dir, &file_count);
    if (!files) { return files_with_ext; }

    for (int i = 0; i < file_count; ++i) {
        if (skip_source_paths && dal_c__shouldSkipSourcePath(files[i])) { continue; }
        if (str_endsWith(files[i], ext)) {
            ArrStr_push(files_with_ext, files[i]);
        }
    }
    dal_c__freeFileList(files, file_count);
    return files_with_ext;
}

static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib, const dal_c_Project* lib_proj) {
    assert(lib != NULL);
    ArrStr* sources = ArrStr_init();
    if (!lib->path) { return sources; }

    // Use absolute path to ensure Makefile can find sources from any directory
    char* lib_abs = path_abs(lib->path);
    if (!lib_abs) { lib_abs = strdup(lib->path); }

    char* lib_src = lib_proj ? dal_c_Project_getSrcDir(lib_proj) : path_join(lib_abs, dal_c_dir_src);
    ArrStr* lib_files = dal_c__collectFilesWithExt(lib_src, ".c", false);
    for (int i = 0; i < ArrStr_len(lib_files); ++i) {
        ArrStr_push(sources, ArrStr_at(lib_files, i));
    }
    ArrStr_fini(&lib_files);
    free(lib_src);
    free(lib_abs);
    return sources;
}

static bool dal_c__copyHeaderToPathIfChanged(const char* src, const char* dst) {
    assert(src != NULL);
    assert(dst != NULL);

    char* content = file_read(src);
    if (!content) { return false; }
    bool success = dal_c__writeFileIfChanged(dst, content);
    free(content);
    return success;
}

static bool dal_c__copyHeaderFile(const char* src, const char* out_dir) {
    assert(src != NULL);
    assert(out_dir != NULL);
    char* header_name = path_basename(src);
    char* dst_path = path_join(out_dir, header_name);
    dir_createRecur(out_dir);
    bool success = dal_c__copyHeaderToPathIfChanged(src, dst_path);
    free(header_name);
    free(dst_path);
    return success;
}

static bool dal_c__copyHeaderRelativeTo(const char* src, const char* src_root, const char* dst_dir) {
    assert(src != NULL);
    assert(dst_dir != NULL);

    if (!src_root || !str_startsWith(src, src_root)) {
        return dal_c__copyHeaderFile(src, dst_dir);
    }

    const char* rel_path = src + strlen(src_root);
    if (*rel_path == '/' || *rel_path == '\\') { rel_path++; }
    if (*rel_path == '\0') {
        return dal_c__copyHeaderFile(src, dst_dir);
    }

    char* dst_path = path_join(dst_dir, rel_path);
    char* dst_parent = path_parent(dst_path);
    dir_createRecur(dst_parent);
    bool success = dal_c__copyHeaderToPathIfChanged(src, dst_path);
    free(dst_parent);
    free(dst_path);
    return success;
}

static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir) {
    assert(src_dir != NULL);
    assert(dst_dir != NULL);
    if (!path_isDir(src_dir)) { return true; }

    ArrStr* headers = dal_c__collectFilesWithExt(src_dir, ".h", false);
    bool success = true;
    for (int i = 0; i < ArrStr_len(headers); ++i) {
        if (!dal_c__copyHeaderRelativeTo(ArrStr_at(headers, i), src_dir, dst_dir)) {
            success = false;
        }
    }
    ArrStr_fini(&headers);
    return success;
}

static char* dal_c__resolveDepsTargetDir(const char* deps_dir, const char* lib_name) {
    assert(deps_dir != NULL);
    assert(lib_name != NULL);

    char* target_subdir = path_parent(lib_name);
    if (!target_subdir || strlen(target_subdir) == 0) {
        free(target_subdir);
        return strdup(deps_dir);
    }

    char* target_dir = path_join(deps_dir, target_subdir);
    free(target_subdir);
    return target_dir;
}

static bool dal_c__copyLibraryArtifacts(const dal_c_Project* consumer_proj, const dal_c_Project* lib_proj, const dal_c_Lib* lib, const char* lib_abs_path, const char* lib_build_dir, bool is_windows) {
    assert(consumer_proj != NULL);
    assert(lib != NULL);
    assert(lib_abs_path != NULL);
    assert(lib_build_dir != NULL);

    char* deps_dir = dal_c_Project_getDepsDir(consumer_proj);
    dir_createRecur(deps_dir);
    bool success = true;

    char* target_dir = dal_c__resolveDepsTargetDir(deps_dir, lib->name);
    dir_createRecur(target_dir);

    // 1. Copy headers: lib/include/* -> consumer/lib/deps/<subdir>/*
    char* lib_inc = lib_proj ? dal_c_Project_getIncludeDir(lib_proj) : path_join(lib_abs_path, dal_c_dir_include);
    if (path_isDir(lib_inc)) {
        if (!dal_c__copyHeadersRecursive(lib_inc, target_dir)) {
            success = false;
        }
    }
    free(lib_inc);

    // 2. Copy compiled library: lib/build/<profile>/<name>.lib -> consumer/lib/deps/<subdir>/
    // For libs with paths like "edition/bar", the .lib is at build/dev/edition/bar.lib
    const char* lib_ext = is_windows ? ".lib" : ".a";
    const char* lib_prefix = is_windows ? "" : "lib";
    const char* lib_basename_tmp = path_basename(lib->name);
    const char* lib_basename = (lib_basename_tmp && strlen(lib_basename_tmp) > 0) ? lib_basename_tmp : lib->name;

    // Try direct path first (e.g., build/dev/B.lib)
    char* lib_file = str_format("%s%s%s", lib_prefix, lib_basename, lib_ext);
    char* lib_src_path = path_join(lib_build_dir, lib_file);

    // If not found, try with lib->name path (e.g., build/dev/edition/bar.lib)
    if (!path_isFile(lib_src_path)) {
        free(lib_src_path);
        char* lib_name_with_ext = str_format("%s%s", lib->name, lib_ext);
        lib_src_path = path_join(lib_build_dir, lib_name_with_ext);
        free(lib_name_with_ext);
    }

    char* lib_dst_path = path_join(target_dir, lib_file);

    if (path_isFile(lib_src_path)) {
        if (!file_copy(lib_src_path, lib_dst_path)) {
            success = false;
        }
    }
    free(lib_file);
    free(lib_src_path);
    free(lib_dst_path);

    // 3. Copy PCH files if present: lib/build/<profile>/*.pch -> consumer/lib/deps/<subdir>/
    int file_count = 0;
    char** files = dir_listRecur(lib_build_dir, &file_count);
    if (files) {
        for (int i = 0; i < file_count; ++i) {
            if (str_endsWith(files[i], ".pch")) {
                const char* pch_name = path_basename(files[i]);
                char* pch_dst = path_join(target_dir, pch_name);
                if (!dal_c__copyHeaderToPathIfChanged(files[i], pch_dst)) {
                    success = false;
                }
                free(pch_dst);
            }
        }
        dal_c__freeFileList(files, file_count);
    }

    // 4. Copy transitive dependencies: lib/lib/deps/*.lib -> consumer/lib/deps/
    // This ensures all transitive dependencies are available to the consumer
    char* lib_deps_dir = path_join(lib_abs_path, "lib/deps");
    if (path_isDir(lib_deps_dir)) {
        int dep_count = 0;
        char** dep_files = dir_listRecur(lib_deps_dir, &dep_count);
        if (dep_files) {
            for (int i = 0; i < dep_count; ++i) {
                // Copy .lib/.a files and .h files
                if (str_endsWith(dep_files[i], lib_ext) || str_endsWith(dep_files[i], ".h") || str_endsWith(dep_files[i], ".pch")) {
                    // Preserve relative path structure
                    const char* rel_path = dep_files[i] + strlen(lib_deps_dir) + 1;
                    char* dst_path = path_join(deps_dir, rel_path);
                    char* dst_parent = path_parent(dst_path);
                    if (dst_parent && strlen(dst_parent) > 0) {
                        dir_createRecur(dst_parent);
                    }
                    bool copied = str_endsWith(dep_files[i], ".h") || str_endsWith(dep_files[i], ".pch")
                        ? dal_c__copyHeaderToPathIfChanged(dep_files[i], dst_path)
                        : file_copy(dep_files[i], dst_path);
                    if (!copied) {
                        success = false;
                    }
                    free(dst_parent);
                    free(dst_path);
                }
            }
            dal_c__freeFileList(dep_files, dep_count);
        }
    }
    free(lib_deps_dir);

    free(target_dir);
    free(deps_dir);
    return success;
}

static char* dal_c__buildParallelFlag(void) {
    bool is_windows = dal_c__platformIsWindows();
    if (is_windows) {
        const char* procs = env_get("NUMBER_OF_PROCESSORS");
        if (procs) {
            return str_format("-j%s", procs);
        }
    }
    return strdup("-j");
}

static void dal_c__writePlatformDebugFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile) {
    assert(fp != NULL);
    assert(profile != NULL);
    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (is_windows && debug_flag && strlen(debug_flag) > 0) {
        (void)fprintf(fp, " -gcodeview");
    }
}

static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile, const char* target_path) {
    assert(fp != NULL);
    assert(profile != NULL);
    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (is_windows && debug_flag && strlen(debug_flag) > 0 && target_path != NULL) {
        char* pdb_path = dal_c__makePdbPath(target_path);
        (void)fprintf(fp, " -fuse-ld=lld -Wl,--pdb=%s", pdb_path);
        free(pdb_path);
    }
}

static void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type, const char* target_path) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(build_dir != NULL);

    bool is_windows = dal_c__platformIsWindows();
    const dal_c_CompilerOpts* opts = &cmd->opts;
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);

    if (proj && proj->root) {
        (void)fprintf(fp, "PROJECT_ROOT ?= %s\n", proj->root);
    }
    if (proj && proj->dh_path) {
        (void)fprintf(fp, "DH_PATH ?= %s\n", proj->dh_path);
    }
    (void)fprintf(fp, "\n");

    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    (void)fprintf(fp, "CC = %s\n", compiler);

    const char* c_std = opts->c_std ? opts->c_std : dal_c_default_c_std;
    (void)fprintf(fp, "STD = -std=%s\n", c_std);

    (void)fprintf(fp, "BUILD_DIR = %s\n", build_dir);
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "CFLAGS_BASE = $(STD)");
    (void)fprintf(fp, " -fgnu-keywords -fms-extensions");
    (void)fprintf(fp, " -funsigned-char -fblocks");
    (void)fprintf(fp, " -mllvm -enable-dfa-jump-thread");

    // Required macro definitions
    (void)fprintf(fp, " -DCOMP");
    if (proj && proj->dh_path && !opts->no_dsl) {
        (void)fprintf(fp, " -DBlocksRuntime_STATIC");
    }

    if (!profile->debug_assertions) {
        (void)fprintf(fp, " -DNDEBUG");
    }

    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (debug_flag && strlen(debug_flag) > 0) {
        (void)fprintf(fp, " %s", debug_flag);
    }

    dal_c__writePlatformDebugFlags(fp, is_windows, profile);

    const char* opt_flag = dal_c_OptiLevel_toFlag(profile->opti_level);
    if (opt_flag) {
        (void)fprintf(fp, " %s", opt_flag);
    }

    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (!str_startsWith(flag, "-Wl,") && !str_startsWith(flag, "-L") && !str_startsWith(flag, "-l")) {
            (void)fprintf(fp, " %s", flag);
        }
    }

    if (opts->loose_errors) {
        (void)fprintf(fp, " -Wall -Wextra -Wconversion");
        (void)fprintf(fp, " -Wsign-conversion -Wfloat-conversion");
        (void)fprintf(fp, " -Wcast-qual -Wcast-align");
        (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
        (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
        (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
        (void)fprintf(fp, " -Wno-microsoft-anon-tag");
        (void)fprintf(fp, " -Wloop-analysis -Wstrict-prototypes");
        (void)fprintf(fp, " -Wmissing-prototypes");
        (void)fprintf(fp, " -Wmissing-variable-declarations");
        (void)fprintf(fp, " -Wdiv-by-zero -Wthread-safety");
    } else {
        (void)fprintf(fp, " -Werror=all -Werror=extra -Werror=conversion");
        (void)fprintf(fp, " -Werror=sign-conversion -Wfloat-conversion");
        (void)fprintf(fp, " -Werror=cast-qual -Werror=cast-align");
        (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
        (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
        (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
        (void)fprintf(fp, " -Wno-microsoft-anon-tag");
        (void)fprintf(fp, " -Wloop-analysis -Werror=strict-prototypes");
        (void)fprintf(fp, " -Werror=missing-prototypes");
        (void)fprintf(fp, " -Wmissing-variable-declarations");
        (void)fprintf(fp, " -Werror=div-by-zero -Wthread-safety");
    }
    (void)fprintf(fp, "\n");

    if (opts->arch_target) {
        (void)fprintf(fp, "CFLAGS_BASE += -target %s\n", opts->arch_target);
    }
    if (opts->freestanding) {
        (void)fprintf(fp, "CFLAGS_BASE += -ffreestanding\n");
    }
    if (opts->sysroot) {
        (void)fprintf(fp, "CFLAGS_BASE += --sysroot=%s\n", opts->sysroot);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "INCLUDES =");
    if (proj && proj->root) {
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/%s", dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
    }
    if (!opts->no_dsl && proj && proj->dh_path) {
        (void)fprintf(fp, " -I$(DH_PATH)/include");
        (void)fprintf(fp, " -I$(DH_PATH)");
        (void)fprintf(fp, " -isystem $(DH_PATH)/libs/BlocksRuntime/include");
    } else if (opts->no_dsl && proj && proj->root) {
        // Building dh itself - use PROJECT_ROOT for BlocksRuntime
        (void)fprintf(fp, " -isystem $(PROJECT_ROOT)/libs/BlocksRuntime/include");
    }
    if (proj && proj->root && proj->lib_count > 0) {
        // Flat deps structure: all headers in lib/deps/
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/lib/deps");
    }
    for (int i = 0; i < opts->include_count; ++i) {
        (void)fprintf(fp, " -I%s", opts->include_paths[i]);
    }
    for (int i = 0; i < opts->isystem_count; ++i) {
        (void)fprintf(fp, " -isystem %s", opts->isystem_paths[i]);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "DEFINES =");
    for (int i = 0; i < opts->define_count; ++i) {
        (void)fprintf(fp, " -D%s", opts->define_macros[i]);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "UNDEFS =");
    for (int i = 0; i < opts->undef_count; ++i) {
        (void)fprintf(fp, " -U%s", opts->undef_macros[i]);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "CFLAGS_NO_PCH = $(CFLAGS_BASE) $(INCLUDES) $(DEFINES) $(UNDEFS)\n");
    if (cmd->compiler_args) {
        (void)fprintf(fp, "CFLAGS_NO_PCH += %s\n", cmd->compiler_args);
    }
    (void)fprintf(fp, "CFLAGS_PCH = $(CFLAGS_NO_PCH)\n");
    (void)fprintf(fp, "\n");

    if (target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib) {
        (void)fprintf(fp, "LDFLAGS =");
        char* project_lib_name = NULL;
        char* project_lib_path = NULL;
        bool link_project_static_lib = target_type == dal_c_Target_executable
                                    && proj
                                    && proj->root
                                    && proj->name
                                    && !(proj->dh_path && str_eql(proj->root, proj->dh_path) && !opts->no_dsl)
                                    && (intent.sample_dir != dal_c_SampleDir_none
                                        || intent.action == dal_c_CmdAction_test
                                        || intent.action == dal_c_CmdAction_test_dsl);
        if (link_project_static_lib) {
            char* project_build_dir = dal_c_Project_getBuildDir(proj);
            char* project_profile_dir = path_join(project_build_dir, profile->name);
            project_lib_name = dal_c__makeTargetFileName(proj->name, dal_c_Target_static_lib, is_windows);
            project_lib_path = path_join(project_profile_dir, project_lib_name);
            link_project_static_lib = path_isFile(project_lib_path);
            free(project_profile_dir);
            free(project_build_dir);
        }
        if (link_project_static_lib) {
            (void)fprintf(fp, " $(PROJECT_ROOT)/build/%s/%s", profile->name, project_lib_name);
        }
        free(project_lib_path);
        free(project_lib_name);
        if (proj && proj->root && proj->lib_count > 0) {
            (void)fprintf(fp, " -L$(PROJECT_ROOT)/lib/deps");
        }
        // Link libdh if using DH library (BlocksRuntime is already compiled into dh.lib)
        if (proj && proj->dh_path && !opts->no_dsl) {
            (void)fprintf(fp, " -L$(DH_PATH)/build/%s", profile->name);
            if (is_windows) {
                (void)fprintf(fp, " $(DH_PATH)/build/%s/dh.lib", profile->name);
            } else {
                (void)fprintf(fp, " $(DH_PATH)/build/%s/libdh.a", profile->name);
            }
        }
        // Link ALL .lib/.a files in lib/deps (includes transitive dependencies)
        if (proj && proj->root) {
            char* deps_dir = dal_c_Project_getDepsDir(proj);
            if (path_isDir(deps_dir)) {
                int lib_count = 0;
                char** lib_files = dir_listRecur(deps_dir, &lib_count);
                if (lib_files) {
                    const char* lib_ext = is_windows ? ".lib" : ".a";
                    for (int i = 0; i < lib_count; ++i) {
                        if (str_endsWith(lib_files[i], lib_ext)) {
                            (void)fprintf(fp, " %s", lib_files[i]);
                        }
                    }
                    dal_c__freeFileList(lib_files, lib_count);
                }
            }
            free(deps_dir);
        }
        for (int i = 0; i < opts->link_count; ++i) {
            (void)fprintf(fp, " -l%s", opts->link_libs[i]);
        }
        if (target_type == dal_c_Target_executable) {
            dal_c__writePlatformLinkerFlags(fp, is_windows, profile, target_path);
        }
        (void)fprintf(fp, "\n");
    } else {
        (void)fprintf(fp, "AR = %s\n", dal_c_tool_ar);
    }
    (void)fprintf(fp, "\n");
}

static char* dal_c__sourceToObjStem(const char* base, const char* src) {
    assert(src != NULL);
    char* work = (base && base[0] != '\0') ? path_relative(base, src) : NULL;
    if (!work) { work = strdup(src); }
    if (!work) { return NULL; }
    if (strncmp(work, "src/", 4) == 0 || strncmp(work, "src\\", 4) == 0) {
        memmove(work, work + 4, strlen(work + 4) + 1);
    }
    for (char* p = work; *p; p++) {
        if (*p == '/' || *p == '\\') { *p = '_'; }
    }
    char* dot = strrchr(work, '.');
    if (dot) { *dot = '\0'; }
    return work;
}

static bool dal_c__sourceNeedsTestMode(const dal_c_Project* proj, const char* src) {
    assert(src != NULL);

    if (proj && proj->root) {
        char* tests_dir = dal_c_Project_getTestsDir(proj);
        bool in_tests_dir = tests_dir && str_startsWith(src, tests_dir);
        free(tests_dir);
        if (in_tests_dir) {
            return true;
        }

        char* cache_dir = path_join(proj->root, dal_c_dir_cache);
        bool is_generated_runner = cache_dir && str_startsWith(src, cache_dir) && strstr(src, ".main.c") != NULL;
        free(cache_dir);
        if (is_generated_runner) {
            return true;
        }
    }

    FILE* fp = fopen(src, "r");
    if (!fp) {
        return false;
    }
    bool needs_test_mode = false;
    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "TEST_fn_(") != NULL || strstr(line, "TEST_Framework_bindCase(") != NULL) {
            needs_test_mode = true;
            break;
        }
    }
    (void)fclose(fp);
    return needs_test_mode;
}

static bool dal_c__sourceUsesPchExcludedHeader(const dal_c_Project* proj, const char* src) {
    assert(src != NULL);
    if (!proj || proj->pch_exclude_count == 0) {
        return false;
    }

    FILE* fp = fopen(src, "r");
    if (!fp) {
        return false;
    }

    bool excluded = false;
    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "#include") == NULL) {
            continue;
        }
        for (int i = 0; i < proj->pch_exclude_count; ++i) {
            const char* header = proj->pch_exclude_headers[i];
            if (header && strstr(line, header) != NULL) {
                excluded = true;
                break;
            }
        }
        if (excluded) {
            break;
        }
    }
    (void)fclose(fp);
    return excluded;
}

static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, bool use_pch, bool test_mode, bool is_third_party) {
    assert(cmd != NULL);
    assert(profile != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashString(hash, opts->compiler);
    hash = dal_c__hashString(hash, opts->c_std);
    hash = dal_c__hashString(hash, opts->arch_target);
    hash = dal_c__hashString(hash, opts->sysroot);
    hash = dal_c__hashString(hash, cmd->compiler_args);
    hash = dal_c__hashBool(hash, opts->freestanding);
    hash = dal_c__hashBool(hash, opts->loose_errors);
    hash = dal_c__hashBool(hash, opts->no_dsl);
    hash = dal_c__hashBool(hash, use_pch);
    hash = dal_c__hashBool(hash, test_mode);
    hash = dal_c__hashBool(hash, is_third_party);

    for (int i = 0; i < opts->define_count; ++i) {
        hash = dal_c__hashString(hash, opts->define_macros[i]);
    }
    for (int i = 0; i < opts->undef_count; ++i) {
        hash = dal_c__hashString(hash, opts->undef_macros[i]);
    }
    for (int i = 0; i < opts->include_count; ++i) {
        hash = dal_c__hashString(hash, opts->include_paths[i]);
    }
    for (int i = 0; i < opts->isystem_count; ++i) {
        hash = dal_c__hashString(hash, opts->isystem_paths[i]);
    }

    return str_format("%016llx", (unsigned long long)hash);
}

static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode, bool is_third_party) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(object_dir != NULL);
    assert(src != NULL);

    char* contract_key = dal_c__makeCompileContractKey(cmd, profile, use_pch, test_mode, is_third_party);
    char* contract_dir = path_join(object_dir, contract_key);
    char* obj_stem = dal_c__sourceToObjStem(base, src);
    char* obj_name = str_format("%s.o", obj_stem);
    char* obj_path = path_join(contract_dir, obj_name);
    free(obj_name);
    free(obj_stem);
    free(contract_dir);
    free(contract_key);
    return obj_path;
}

static void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Project* proj, const char* build_dir) {
    assert(fp != NULL);
    assert(build_dir != NULL);
    if (!proj || !proj->pch_header) {
        (void)fprintf(fp, "PCH_OUT =\n\n");
        (void)fprintf(fp, "CFLAGS = $(CFLAGS_NO_PCH)\n\n");
        return;
    }

    const char* pch_basename = path_basename(proj->pch_header);
    char* pch_out = str_format("%s/%s.pch", build_dir, pch_basename);

    (void)fprintf(fp, "PCH_SRC = %s\n", proj->pch_header);
    (void)fprintf(fp, "PCH_OUT = %s\n", pch_out);
    (void)fprintf(fp, "CFLAGS = $(CFLAGS_PCH) -include-pch $(PCH_OUT)\n\n");

    // PCH dependency file tracks all headers included by PCH_SRC
    char* pch_dep = str_format("%s/%s.d", build_dir, pch_basename);
    (void)fprintf(fp, "PCH_DEP = %s\n", pch_dep);
    (void)fprintf(fp, "-include $(PCH_DEP)\n\n");
    free(pch_dep);

    (void)fprintf(fp, "$(PCH_OUT): $(PCH_SRC)\n");
    (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
    (void)fprintf(fp, "\t@echo \"[PCH] Compiling $(PCH_SRC)...\"\n");
    (void)fprintf(fp, "\t$(CC) $(CFLAGS_BASE) $(INCLUDES) -MMD -MP -MF $(PCH_DEP) -x c-header $< -o $@\n\n");

    free(pch_out);
}

static void dal_c__writeMakefileCompilationRules(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(object_dir != NULL);

    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);

        // Third-party libs (BlocksRuntime) should compile silently with -w
        bool is_third_party = strstr(src, "libs/") || strstr(src, "libs\\");
        bool use_pch = has_pch && !is_third_party && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        bool test_mode = !is_third_party && dal_c__sourceNeedsTestMode(proj, src);
        char* obj_path = dal_c__makeObjectPath(cmd, profile, object_dir, base, src, use_pch, test_mode, is_third_party);

        const char* cflags_base = is_third_party ? "$(CFLAGS_BASE) $(INCLUDES) -w" : (use_pch ? "$(CFLAGS)" : "$(CFLAGS_NO_PCH)");
        char* cflags = test_mode ? str_format("%s -DCOMP_TEST", cflags_base) : strdup(cflags_base);

        (void)fprintf(fp, "%s: %s", obj_path, src);
        if (use_pch) {
            (void)fprintf(fp, " $(PCH_OUT)");
        }
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[CC] %s\"\n", src);
        (void)fprintf(fp, "\t$(CC) %s -MMD -MP -c %s -o $@\n\n", cflags, src);

        free(cflags);
        free(obj_path);
    }
}

static void dal_c__writeMakefileTargetVar(FILE* fp, const char* target_path) {
    assert(fp != NULL);
    assert(target_path != NULL);
    (void)fprintf(fp, "TARGET = %s\n\n", target_path);
}

static void dal_c__writeMakefileTargetRule(FILE* fp, dal_c_Target type, bool is_windows) {
    assert(fp != NULL);
    (void)is_windows;

    if (type == dal_c_Target_executable) {
        (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[LD] $@\"\n");
        (void)fprintf(fp, "\t$(CC) $(OBJS) -o $@ $(LDFLAGS)\n");
    } else if (type == dal_c_Target_static_lib) {
        (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[AR] $@\"\n");
        (void)fprintf(fp, "\t$(AR) rcs $@ $(OBJS)\n");
    } else if (type == dal_c_Target_shared_lib) {
        (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[LD] $@\"\n");
        (void)fprintf(fp, "\t$(CC) -shared -fPIC $(OBJS) -o $@ $(LDFLAGS)\n");
    }
    (void)fprintf(fp, "\n");
}
