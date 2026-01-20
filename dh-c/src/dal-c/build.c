#include "internal.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/ArrStr.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
/* FIXME: correct placing binaries path */

// === PRIVATE HELPERS (Core Layer - use asserts) ===

static bool dal_c__copyHeaderFile(const char* src, const char* dst_dir);
static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir);
static bool dal_c__isHeaderOnlyLibrary(const dal_c_Lib* lib);
static void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type);
static void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Project* proj, const char* build_dir);
static void dal_c__writeMakefileCompilationRules(FILE* fp, ArrStr* sources, bool has_pch);
static void dal_c__writeMakefileTargetVar(FILE* fp, dal_c_Target type, const char* target_name, bool is_windows);
static void dal_c__writeMakefileTargetRule(FILE* fp, dal_c_Target type, bool is_windows);
static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile);
static char* dal_c__buildParallelFlag(void);
static ArrStr* dal_c__collectHeaderFiles(const dal_c_Project* proj, const char* target_file);
static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib);
static const char* dal_c__getTargetFile(const dal_c_Cmd* cmd);
static const char* dal_c__getRunArgs(const dal_c_Cmd* cmd);

// === PLATFORM ===

bool dal_c__platformIsWindows(void) {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

// === SOURCE COLLECTION (Core Layer) ===

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_file) {
    ArrStr* sources = ArrStr_init();
    if (target_file) {
        ArrStr_push(sources, target_file);
    } else {
        assert(proj != NULL && proj->root != NULL);
        char* src_dir = path_join(proj->root, dal_c_dir_src);
        if (path_isDir(src_dir)) {
            int file_count = 0;
            char** files = dir_listRecur(src_dir, &file_count);
            for (int i = 0; i < file_count; ++i) {
                // Skip excluded directories (per .clangd PathExclude)
                if (strstr(files[i], "build")
                    || strstr(files[i], "tmp")
                    || strstr(files[i], "temp")
                    || strstr(files[i], "cache")
                    || strstr(files[i], "mock")
                    || strstr(files[i], "draft")
                    || strstr(files[i], "bak")
                    || strstr(files[i], "backup")
                    || strstr(files[i], "archive")) { continue; }
                if (str_endsWith(files[i], ".c")) {
                    ArrStr_push(sources, files[i]);
                }
            }
        }
        free(src_dir);

        // Include libs/BlocksRuntime/src/*.c if present (for dh library)
        char* blocks_src = path_join(proj->root, "libs/BlocksRuntime/src");
        if (path_isDir(blocks_src)) {
            int file_count = 0;
            char** files = dir_listRecur(blocks_src, &file_count);
            for (int i = 0; i < file_count; ++i) {
                if (str_endsWith(files[i], ".c")) {
                    ArrStr_push(sources, files[i]);
                }
            }
        }
        free(blocks_src);
    }
    return sources;
}

ArrStr* dal_c__collectDirectoryFiles(const dal_c_Project* proj, const char* dir_name) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    assert(dir_name != NULL);

    ArrStr* sources = ArrStr_init();
    char* dir = path_join(proj->root, dir_name);
    if (path_isDir(dir)) {
        int file_count = 0;
        char** files = dir_listRecur(dir, &file_count);
        if (files) {
            for (int i = 0; i < file_count; ++i) {
                if (str_endsWith(files[i], ".c")) {
                    ArrStr_push(sources, files[i]);
                }
            }
        }
    }
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
        char* inc_dir = path_join(proj->root, dal_c_dir_include);
        bool has_inc = path_isDir(inc_dir);
        free(inc_dir);
        return has_inc;
    }

    return false;
}

int dal_c__buildHeaderOnlyLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* output_name) {
    assert(cmd != NULL);
    assert(output_name != NULL);
    const char* target_file = dal_c__getTargetFile(cmd);

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
    char* deps_inc = path_join(lib_deps, dal_c_dir_include);
    free(lib_dir);
    free(deps_dir);

    ArrStr* headers = dal_c__collectHeaderFiles(proj, target_file);
    if (ArrStr_len(headers) == 0) {
        (void)fprintf(stderr, "Error: No header files found\n");
        ArrStr_fini(&headers);
        free(deps_inc);
        free(lib_deps);
        return 1;
    }

    for (int i = 0; i < ArrStr_len(headers); ++i) {
        const char* header = ArrStr_at(headers, i);
        if (target_file && str_eql(header, target_file)) {
            if (!dal_c__copyHeaderFile(header, deps_inc)) {
                (void)fprintf(stderr, "Error: Failed to copy header: %s\n", header);
                ArrStr_fini(&headers);
                free(deps_inc);
                free(lib_deps);
                return 1;
            }
        } else {
            char* inc_dir = path_join(proj->root, dal_c_dir_include);
            if (str_startsWith(header, inc_dir)) {
                const char* rel = header + strlen(inc_dir);
                if (*rel == '/' || *rel == '\\') { rel++; }
                char* rel_path = strdup(rel);

                char* dst_path = path_join(deps_inc, rel_path);
                char* dst_parent = path_parent(dst_path);
                dir_createRecur(dst_parent);
                if (!file_copy(header, dst_path)) {
                    (void)fprintf(stderr, "Error: Failed to copy header: %s\n", header);
                    free(rel_path);
                    free(dst_path);
                    free(dst_parent);
                    free(inc_dir);
                    ArrStr_fini(&headers);
                    free(deps_inc);
                    free(lib_deps);
                    return 1;
                }
                free(rel_path);
                free(dst_path);
                free(dst_parent);
            }
            free(inc_dir);
        }
    }

    ArrStr_fini(&headers);
    free(deps_inc);
    free(lib_deps);
    if (cmd->verbose) {
        printf("Header-only library built: %s\n", output_name);
    }
    return 0;
}

static bool dal_c__copyLibraryArtifacts(const dal_c_Project* consumer_proj, const dal_c_Lib* lib, const char* lib_abs_path, const char* lib_build_dir, bool is_windows);
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

    // 2. Handle header-only libraries
    if (dal_c__isHeaderOnlyLibrary(lib)) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        dir_createRecur(deps_dir);

        // Preserve path structure from lib->name
        char* target_subdir = path_parent(lib->name);
        char* target_dir = deps_dir;
        if (target_subdir && strlen(target_subdir) > 0) {
            target_dir = path_join(deps_dir, target_subdir);
            dir_createRecur(target_dir);
        }

        if (str_endsWith(lib->path, ".h") && path_isFile(lib->path)) {
            if (!dal_c__copyHeaderFile(lib->path, target_dir)) {
                (void)fprintf(stderr, "Error: Failed to copy header file: %s\n", lib->path);
                if (target_subdir && strlen(target_subdir) > 0) { free(target_dir); }
                free(target_subdir);
                free(deps_dir);
                dal_c_Project_cleanup(&lib_proj);
                return 1;
            }
        } else {
            const char* lib_abs_path = lib_proj ? lib_proj->root : lib->path;
            char* lib_inc = path_join(lib_abs_path, dal_c_dir_include);
            if (path_isDir(lib_inc)) {
                if (!dal_c__copyHeadersRecursive(lib_inc, target_dir)) {
                    (void)fprintf(stderr, "Error: Failed to copy headers from: %s\n", lib_inc);
                    free(lib_inc);
                    if (target_subdir && strlen(target_subdir) > 0) { free(target_dir); }
                    free(target_subdir);
                    free(deps_dir);
                    dal_c_Project_cleanup(&lib_proj);
                    return 1;
                }
            }
            free(lib_inc);
        }

        if (target_subdir && strlen(target_subdir) > 0) { free(target_dir); }
        free(target_subdir);
        free(deps_dir);
        dal_c_Project_cleanup(&lib_proj);
        if (cmd->verbose) {
            printf("Header-only library %s: headers copied\n", lib->name);
        }
        return 0;
    }

    // 3. Collect sources from library's own src/ directory
    ArrStr* lib_sources = dal_c__collectLibrarySources(lib);
    if (ArrStr_len(lib_sources) == 0) {
        (void)fprintf(stderr, "Error: Library %s has no source files\n", lib->name);
        ArrStr_fini(&lib_sources);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }

    // 4. Build in library's own build/<profile>/ directory
    dal_c_Profile lib_profile_enum = (lib->opts.profile != dal_c_Profile_invalid)
                                       ? lib->opts.profile
                                       : cmd->opts.profile;
    const dal_c_ProfileSpec* lib_profile = dal_c_ProfileSpec_by(lib_profile_enum);
    assert(lib_profile != NULL);

    // Use absolute path from lib_proj (or convert lib->path if no lib_proj)
    const char* lib_abs_path = lib_proj ? lib_proj->root : lib->path;
    char* lib_build_dir = path_join(lib_abs_path, dal_c_dir_build);
    char* lib_build_profile = path_join(lib_build_dir, lib_profile->name);
    free(lib_build_dir);
    dir_createRecur(lib_build_profile);

    dal_c_Cmd merged = *cmd;
    merged.opts = lib->opts;
    if (!merged.opts.compiler) { merged.opts.compiler = cmd->opts.compiler; }
    if (!merged.opts.c_std) { merged.opts.c_std = cmd->opts.c_std; }
    if (!merged.opts.arch_target) { merged.opts.arch_target = cmd->opts.arch_target; }
    if (!merged.opts.sysroot) { merged.opts.sysroot = cmd->opts.sysroot; }

    // Use the detected lib_proj for Makefile generation (has absolute paths)
    dal_c_Project build_proj = {
        .root = lib_abs_path,
        .name = lib->name,
        .dh_path = proj->dh_path,
        .pch_header = lib_proj ? lib_proj->pch_header : NULL,
        .libraries = lib_proj ? lib_proj->libraries : NULL,
        .lib_count = lib_proj ? lib_proj->lib_count : 0,
    };

    dal_c_Target lib_target_type = lib->is_static ? dal_c_Target_static_lib : dal_c_Target_shared_lib;
    if (dal_c__generateMakefile(&merged, &build_proj, lib_profile, lib_sources, lib->name, lib_build_profile, lib_target_type) != 0) {
        (void)fprintf(stderr, "Error: Failed to generate Makefile for library: %s\n", lib->name);
        ArrStr_fini(&lib_sources);
        free(lib_build_profile);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }

    int result = dal_c__executeMakeInDir(lib_build_profile);
    ArrStr_fini(&lib_sources);
    if (result != 0) {
        (void)fprintf(stderr, "Error: Failed to build library: %s\n", lib->name);
        free(lib_build_profile);
        dal_c_Project_cleanup(&lib_proj);
        return result;
    }

    // 5. Copy artifacts (headers, libs, PCH) to consumer's lib/deps/
    bool is_windows = dal_c__platformIsWindows();
    if (!dal_c__copyLibraryArtifacts(proj, lib, lib_abs_path, lib_build_profile, is_windows)) {
        (void)fprintf(stderr, "Warning: Failed to copy some artifacts for %s\n", lib->name);
    }

    free(lib_build_profile);
    dal_c_Project_cleanup(&lib_proj);
    return 0;
}

int dal_c__runExecutable(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    assert(proj != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* base_build_dir = dal_c_Project_getBuildDir(proj);
    char* build_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    assert(build_dir != NULL);

    const char* target_file = dal_c__getTargetFile(cmd);

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_file) {
        const char* basename = path_basename(target_file);
        char* dot = strrchr(basename, '.');
        if (dot) {
            exe_name_alloc = strdup(basename);
            exe_name_alloc[dot - basename] = '\0';
            exe_name = exe_name_alloc;
        } else {
            exe_name = basename;
        }
    } else {
        exe_name = proj->name;
    }
    assert(exe_name != NULL);

    bool is_windows = dal_c__platformIsWindows();
    char* exe_file = is_windows ? str_format("%s.exe", exe_name) : strdup(exe_name);
    char* exe_path = path_join(build_dir, exe_file);
    free(exe_file);
    free(build_dir);
    if (!path_exists(exe_path)) {
        (void)fprintf(stderr, "Error: Executable not found: %s\n", exe_path);
        if (exe_name_alloc) { free(exe_name_alloc); }
        free(exe_path);
        return 1;
    }

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, exe_path);

    const char* run_args = dal_c__getRunArgs(cmd);
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

    const char* target_file = dal_c__getTargetFile(cmd);

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_file) {
        const char* basename = path_basename(target_file);
        char* dot = strrchr(basename, '.');
        if (dot) {
            exe_name_alloc = strdup(basename);
            exe_name_alloc[dot - basename] = '\0';
            exe_name = exe_name_alloc;
        } else {
            exe_name = basename;
        }
    } else {
        exe_name = proj->name;
    }
    assert(exe_name != NULL);

    bool is_windows = dal_c__platformIsWindows();
    char* exe_path = is_windows
                       ? str_format("%s/%s.exe", build_dir, exe_name)
                       : path_join(build_dir, exe_name);
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

    const char* run_args = dal_c__getRunArgs(cmd);
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

static int dal_c__ensureLibDH(const dal_c_Project* proj, const dal_c_ProfileSpec* profile) {
    if (!proj || !proj->dh_path) { return 0; }
    bool is_windows = dal_c__platformIsWindows();
    const char* lib_name = is_windows ? "dh.lib" : "libdh.a";

    // Use absolute path to avoid mixed slash issues
    char* dh_abs_path = path_abs(proj->dh_path);
    if (!dh_abs_path) { dh_abs_path = strdup(proj->dh_path); }

    char* build_dir = path_join(dh_abs_path, dal_c_dir_build);
    char* profile_dir = path_join(build_dir, profile->name);
    char* lib_path = path_join(profile_dir, lib_name);
    char* makefile_path = path_join(profile_dir, dal_c_file_makefile);

    // Only generate Makefile if it doesn't exist; make handles staleness
    bool needs_generate = !path_exists(makefile_path);

    free(lib_path);
    free(makefile_path);
    free(dh_abs_path);
    free(build_dir);

    if (needs_generate) {
        // Create a temporary project for dh
        dal_c_Project dh_proj = {
            .root = proj->dh_path,
            .name = "dh",
            .dh_path = NULL, // dh doesn't depend on itself
            .pch_header = NULL,
        };

        // Collect dh sources (includes BlocksRuntime automatically)
        ArrStr* sources = dal_c__collectSourceFiles(&dh_proj, NULL);
        if (ArrStr_len(sources) == 0) {
            (void)fprintf(stderr, "Error: No source files found for libdh\n");
            ArrStr_fini(&sources);
            free(profile_dir);
            return 1;
        }

        // Create build directory
        dir_createRecur(profile_dir);

        // Create a minimal cmd with no_libdh set
        dal_c_Cmd cmd = { 0 };
        cmd.opts.no_libdh = true;
        cmd.opts.profile = dal_c_Profile_parse(profile->name);

        // Generate makefile
        int result = dal_c__generateMakefile(&cmd, &dh_proj, profile, sources, "dh", profile_dir, dal_c_Target_static_lib);
        ArrStr_fini(&sources);
        if (result != 0) {
            free(profile_dir);
            return result;
        }
    }

    // Always run make - it handles staleness detection via .d files
    int result = dal_c__executeMakeInDir(profile_dir);
    free(profile_dir);
    return result;
}

int dal_c__generateMakefile(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* output_name,
    const char* build_dir,
    dal_c_Target target_type
) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(output_name != NULL);
    assert(build_dir != NULL);

    // Auto-build libdh if needed and not disabled
    if (!cmd->opts.no_libdh && proj && proj->dh_path) {
        if (dal_c__ensureLibDH(proj, profile) != 0) {
            (void)fprintf(stderr, "Error: Failed to build libdh\n");
            return 1;
        }
    }

    // All Makefiles go in build_dir for consistency
    char* makefile_path = path_join(build_dir, dal_c_file_makefile);

    FILE* fp = fopen(makefile_path, "w");
    if (!fp) {
        (void)fprintf(stderr, "Error: Failed to open Makefile for writing: %s\n", makefile_path);
        return 1;
    }

    bool is_windows = dal_c__platformIsWindows();
    bool has_pch = (proj && proj->pch_header != NULL);

    dal_c__writeMakefileVariables(fp, cmd, profile, proj, build_dir, target_type);

    // Declare default goal before any rules to ensure 'all' is the default target
    (void)fprintf(fp, ".DEFAULT_GOAL := all\n\n");

    dal_c__writeMakefilePCH(fp, proj, build_dir);

    (void)fprintf(fp, "SRCS =");
    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        (void)fprintf(fp, " %s", ArrStr_at(sources, i));
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "OBJS =");
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        const char* basename = path_basename(src);
        char* obj_name = strdup(basename);
        char* dot = strrchr(obj_name, '.');
        if (dot) { *dot = '\0'; }
        (void)fprintf(fp, " $(BUILD_DIR)/%s.o", obj_name);
        free(obj_name);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "DEPS = $(OBJS:.o=.d)\n\n");

    // Define TARGET before all: $(TARGET) so make knows the dependency
    dal_c__writeMakefileTargetVar(fp, target_type, output_name, is_windows);
    (void)fprintf(fp, "all: $(TARGET)\n\n");

    dal_c__writeMakefileCompilationRules(fp, sources, has_pch);
    dal_c__writeMakefileTargetRule(fp, target_type, is_windows);

    (void)fprintf(fp, "clean:\n\trm -rf $(BUILD_DIR)\n\n");
    (void)fprintf(fp, "-include $(DEPS)\n");

    (void)fclose(fp);
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
    va_list args = NULL;
    va_start(args, fmt);
    (void)vprintf(fmt, args);
    va_end(args);
}

void dal_c__printError(const char* fmt, ...) {
    assert(fmt != NULL);

    va_list args = NULL;
    va_start(args, fmt);
    (void)vfprintf(stderr, fmt, args);
    va_end(args);
}

// === PRIVATE IMPLEMENTATIONS (Core Layer - use asserts) ===

static const char* dal_c__getTargetFile(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return cmd->payload.build.target_file;
    case dal_c_CmdAction_lib:   return cmd->payload.lib.target_file;
    case dal_c_CmdAction_run:   return cmd->payload.run.target_file;
    case dal_c_CmdAction_test:  return cmd->payload.test.target_file;
    default:                    return NULL;
    }
}

static const char* dal_c__getRunArgs(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_run:  return cmd->payload.run.run_args;
    case dal_c_CmdAction_test: return cmd->payload.test.run_args;
    default:                   return NULL;
    }
}

static ArrStr* dal_c__collectHeaderFiles(const dal_c_Project* proj, const char* target_file) {
    ArrStr* headers = ArrStr_init();
    if (target_file && str_endsWith(target_file, ".h")) {
        ArrStr_push(headers, target_file);
    } else {
        assert(proj != NULL && proj->root != NULL);
        char* inc_dir = path_join(proj->root, dal_c_dir_include);
        if (path_isDir(inc_dir)) {
            int file_count = 0;
            char** files = dir_listRecur(inc_dir, &file_count);
            if (files) {
                for (int i = 0; i < file_count; ++i) {
                    if (str_endsWith(files[i], ".h")) {
                        ArrStr_push(headers, files[i]);
                    }
                }
            }
        }
        free(inc_dir);
    }
    return headers;
}

static bool dal_c__isHeaderOnlyLibrary(const dal_c_Lib* lib) {
    assert(lib != NULL);
    if (!lib->path) { return false; }

    char* lib_src = path_join(lib->path, dal_c_dir_src);
    if (path_isDir(lib_src)) {
        int file_count = 0;
        char** files = dir_listRecur(lib_src, &file_count);
        if (files) {
            for (int i = 0; i < file_count; ++i) {
                if (str_endsWith(files[i], ".c")) {
                    free(lib_src);
                    return false;
                }
            }
        }
    }
    free(lib_src);

    char* lib_inc = path_join(lib->path, dal_c_dir_include);
    if (path_isDir(lib_inc)) {
        free(lib_inc);
        return true;
    }
    free(lib_inc);

    if (str_endsWith(lib->path, ".h") && path_isFile(lib->path)) {
        return true;
    }
    return false;
}

static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib) {
    assert(lib != NULL);
    ArrStr* sources = ArrStr_init();
    if (!lib->path) { return sources; }

    // Use absolute path to ensure Makefile can find sources from any directory
    char* lib_abs = path_abs(lib->path);
    if (!lib_abs) { lib_abs = strdup(lib->path); }

    char* lib_src = path_join(lib_abs, dal_c_dir_src);
    if (path_isDir(lib_src)) {
        int file_count = 0;
        char** files = dir_listRecur(lib_src, &file_count);
        if (files) {
            for (int i = 0; i < file_count; ++i) {
                if (str_endsWith(files[i], ".c")) {
                    ArrStr_push(sources, files[i]);
                }
            }
        }
    }
    free(lib_src);
    free(lib_abs);
    return sources;
}

static bool dal_c__copyHeaderFile(const char* src, const char* dst_dir) {
    assert(src != NULL);
    assert(dst_dir != NULL);
    const char* basename = path_basename(src);
    char* dst_path = path_join(dst_dir, basename);
    dir_createRecur(dst_dir);
    bool success = file_copy(src, dst_path);
    free(dst_path);
    return success;
}

static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir) {
    assert(src_dir != NULL);
    assert(dst_dir != NULL);
    if (!path_isDir(src_dir)) { return true; }

    dir_createRecur(dst_dir);
    int file_count = 0;
    char** files = dir_listRecur(src_dir, &file_count);
    if (!files) { return true; }

    bool success = true;
    for (int i = 0; i < file_count; ++i) {
        if (str_endsWith(files[i], ".h")) {
            size_t src_dir_len = strlen(src_dir);
            const char* rel_path = files[i] + src_dir_len;
            if (*rel_path == '/' || *rel_path == '\\') { rel_path++; }

            char* dst_path = path_join(dst_dir, rel_path);
            char* dst_parent = path_parent(dst_path);
            dir_createRecur(dst_parent);

            if (!file_copy(files[i], dst_path)) {
                success = false;
            }
            free(dst_path);
            free(dst_parent);
        }
    }

    return success;
}

static bool dal_c__copyLibraryArtifacts(const dal_c_Project* consumer_proj, const dal_c_Lib* lib, const char* lib_abs_path, const char* lib_build_dir, bool is_windows) {
    assert(consumer_proj != NULL);
    assert(lib != NULL);
    assert(lib_abs_path != NULL);
    assert(lib_build_dir != NULL);

    char* deps_dir = dal_c_Project_getDepsDir(consumer_proj);
    dir_createRecur(deps_dir);
    bool success = true;

    // Use lib->name to determine target directory structure
    // e.g., lib->name = "dh-extras/os/windows/wnd" -> target_subdir = "dh-extras/os/windows"
    char* target_subdir = path_parent(lib->name);
    char* target_dir = deps_dir;
    if (target_subdir && strlen(target_subdir) > 0) {
        target_dir = path_join(deps_dir, target_subdir);
        dir_createRecur(target_dir);
    }

    // 1. Copy headers: lib/include/* -> consumer/lib/deps/<subdir>/*
    char* lib_inc = path_join(lib_abs_path, dal_c_dir_include);
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
                if (!file_copy(files[i], pch_dst)) {
                    success = false;
                }
                free(pch_dst);
            }
        }
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
                    if (!file_copy(dep_files[i], dst_path)) {
                        success = false;
                    }
                    free(dst_parent);
                    free(dst_path);
                }
            }
            for (int i = 0; i < dep_count; ++i) {
                free(dep_files[i]);
            }
            free((void*)dep_files);
        }
    }
    free(lib_deps_dir);

    if (target_subdir && strlen(target_subdir) > 0) {
        free(target_dir);
    }
    free(target_subdir);
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

static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile) {
    assert(fp != NULL);
    assert(profile != NULL);
    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (is_windows && debug_flag && strlen(debug_flag) > 0) {
        (void)fprintf(fp, " -fuse-ld=lld -Wl,--pdb=$(BUILD_DIR)/app.pdb");
    }
}

static void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(build_dir != NULL);

    bool is_windows = dal_c__platformIsWindows();
    const dal_c_CompilerOpts* opts = &cmd->opts;

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

    // Required macro definitions
    (void)fprintf(fp, " -DCOMP");
    if (cmd->action == dal_c_CmdAction_test) {
        (void)fprintf(fp, " -DCOMP_TEST");
    }
    if (proj && proj->dh_path && !opts->no_libdh) {
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
        (void)fprintf(fp, " -Wswitch-enum -Winfinite-recursion");
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
        (void)fprintf(fp, " -Wswitch-enum -Winfinite-recursion");
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
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/include");
    }
    if (!opts->no_libdh && proj && proj->dh_path) {
        (void)fprintf(fp, " -I$(DH_PATH)/include");
        (void)fprintf(fp, " -I$(DH_PATH)");
        (void)fprintf(fp, " -isystem $(DH_PATH)/libs/BlocksRuntime/include");
    } else if (opts->no_libdh && proj && proj->root) {
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

    (void)fprintf(fp, "CFLAGS = $(CFLAGS_BASE) $(INCLUDES) $(DEFINES) $(UNDEFS)\n");
    if (cmd->compiler_args) {
        (void)fprintf(fp, "CFLAGS += %s\n", cmd->compiler_args);
    }
    (void)fprintf(fp, "\n");

    if (target_type == dal_c_Target_executable) {
        (void)fprintf(fp, "LDFLAGS =");
        if (proj && proj->root && proj->lib_count > 0) {
            (void)fprintf(fp, " -L$(PROJECT_ROOT)/lib/deps");
        }
        // Link libdh if using DH library (BlocksRuntime is already compiled into dh.lib)
        if (proj && proj->dh_path && !opts->no_libdh) {
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
                        free(lib_files[i]);
                    }
                    free((void*)lib_files);
                }
            }
            free(deps_dir);
        }
        for (int i = 0; i < opts->link_count; ++i) {
            (void)fprintf(fp, " -l%s", opts->link_libs[i]);
        }
        dal_c__writePlatformLinkerFlags(fp, is_windows, profile);
        (void)fprintf(fp, "\n");
    } else {
        (void)fprintf(fp, "AR = %s\n", dal_c_tool_ar);
    }
    (void)fprintf(fp, "\n");
}

static void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Project* proj, const char* build_dir) {
    assert(fp != NULL);
    assert(build_dir != NULL);
    if (!proj || !proj->pch_header) {
        (void)fprintf(fp, "PCH_OUT =\n\n");
        return;
    }

    const char* pch_basename = path_basename(proj->pch_header);
    char* pch_out = str_format("%s/%s.pch", build_dir, pch_basename);

    (void)fprintf(fp, "PCH_SRC = %s\n", proj->pch_header);
    (void)fprintf(fp, "PCH_OUT = %s\n", pch_out);
    (void)fprintf(fp, "CFLAGS += -include-pch $(PCH_OUT)\n\n");

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

static void dal_c__writeMakefileCompilationRules(FILE* fp, ArrStr* sources, bool has_pch) {
    assert(fp != NULL);
    assert(sources != NULL);

    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        const char* basename = path_basename(src);
        char* obj_name = strdup(basename);
        char* dot = strrchr(obj_name, '.');
        if (dot) { *dot = '\0'; }

        // Third-party libs (BlocksRuntime) should compile silently with -w
        bool is_third_party = strstr(src, "libs/") || strstr(src, "libs\\");
        const char* cflags = is_third_party ? "$(CFLAGS_BASE) $(INCLUDES) -w" : "$(CFLAGS)";

        (void)fprintf(fp, "$(BUILD_DIR)/%s.o: %s", obj_name, src);
        if (has_pch && !is_third_party) {
            (void)fprintf(fp, " $(PCH_OUT)");
        }
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[CC] %s\"\n", src);
        (void)fprintf(fp, "\t$(CC) %s -MMD -MP -c %s -o $@\n\n", cflags, src);

        free(obj_name);
    }
}

static void dal_c__writeMakefileTargetVar(FILE* fp, dal_c_Target type, const char* target_name, bool is_windows) {
    assert(fp != NULL);
    assert(target_name != NULL);

    if (type == dal_c_Target_executable) {
        if (is_windows) {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/%s.exe\n\n", target_name);
        } else {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/%s\n\n", target_name);
        }
    } else if (type == dal_c_Target_static_lib) {
        if (is_windows) {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/%s.lib\n\n", target_name);
        } else {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/lib%s.a\n\n", target_name);
        }
    } else if (type == dal_c_Target_shared_lib) {
        if (is_windows) {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/%s.dll\n\n", target_name);
        } else {
            (void)fprintf(fp, "TARGET = $(BUILD_DIR)/lib%s.so\n\n", target_name);
        }
    }
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
