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
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// === PRIVATE HELPERS (Core Layer - use asserts) ===

static void dal_c__freeFileList(char** files, int file_count);
static const char** dal_c__makeConstArgvView(ArrStr* argv);
static ArrStr* dal_c__collectFilesWithExt(const char* dir, const char* ext, bool skip_source_paths);
static ArrStr* dal_c__collectBuildFiles(const char* dir, bool skip_source_paths);
static bool dal_c__copyHeaderToPathIfChanged(const char* src, const char* dst);
static bool dal_c__copyHeaderFile(const char* src, const char* out_dir);
static bool dal_c__copyHeaderRelativeTo(const char* src, const char* src_root, const char* dst_dir);
static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir);
static char* dal_c__resolveDepsTargetDir(const char* deps_dir, const char* lib_name);
static char* dal_c__makeMakePath(const char* path);
static void dal_c__fprintMakePath(FILE* fp, const char* path);
static dal_c__noinline void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type, const char* target_path, const char* link_contract_path);
static dal_c__noinline void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* build_dir, dal_c_Target target_type);
static dal_c__noinline int dal_c__writeEmitOnlyMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type,
    char* makefile_tmp,
    char* makefile_path,
    char* makefile_dir,
    char* link_contract_path
);
static dal_c__noinline int dal_c__writeLinkedMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type,
    char* makefile_tmp,
    char* makefile_path,
    char* makefile_dir,
    char* link_contract_path
);
static char* dal_c__sourceToObjStem(const char* base, const char* src);
static bool dal_c__commandUsesTestMode(const dal_c_Cmd* cmd);
static bool dal_c__sourceUsesTestMode(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* src);
static bool dal_c__sourceUsesPchExcludedHeader(const dal_c_Project* proj, const char* src);
static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode);
static char* dal_c__makeLinkContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeLinkContractPath(const char* build_dir, const char* target_path);
static bool dal_c__writeLinkContractFile(const char* path, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode);
static bool dal_c__sourceIsAssembly(const char* src);
static char* dal_c__makeImageLinkPath(const char* target_path);
static uint64_t dal_c__hashVersionSpec(uint64_t hash, const dal_c_VersionSpec* version);
static void dal_c__writeVersionDefines(FILE* fp, const dal_c_VersionSpec* version);
static bool dal_c__targetIsEmitOnly(dal_c_Target target_type);
static dal_c__noinline void dal_c__writeMakefileCompilationRules(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base, dal_c_Target target_type);
static void dal_c__writeMakefileTargetVar(FILE* fp, const char* target_path);
static dal_c__noinline void dal_c__writeMakefileTargetRule(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target type, bool is_windows, const char* target_path, const char* link_contract_path);
static char* dal_c__artifactPath(const char* target_path, const char* override_path, const char* ext);
static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile, const char* target_path);
static void dal_c__writeLinkModelFlags(FILE* fp, bool is_windows, const dal_c_CompilerOpts* opts, dal_c_Target target_type);
static char* dal_c__buildParallelFlag(void);
static bool dal_c__writeFileIfChanged(const char* path, const char* content);
static const char* dal_c__planContextDir(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_Target target_type);
static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib, const dal_c_Project* lib_proj);
static int dal_c__runDependencyTests(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
static bool dal_c__copyLibraryArtifacts(
    const dal_c_Project* consumer_proj,
    const dal_c_Project* lib_proj,
    const dal_c_Lib* lib,
    const char* lib_abs_path,
    const char* lib_build_dir,
    const dal_c_Cmd* build_cmd,
    const dal_c_ProfileSpec* build_profile,
    dal_c_Target build_target_type,
    bool is_windows
);
static char* dal_c__sanitizePathFragment(const char* value);
static bool dal_c__pathHasSeparator(const char* path);
static bool dal_c__pathIsAbsolute(const char* path);
static char* dal_c__makeTargetFileName(const char* name, dal_c_Target type, bool is_windows);
static char* dal_c__makePdbPath(const char* target_path);
static bool dal_c__usesAggregateTestTarget(const dal_c_Cmd* cmd);
static char* dal_c__buildBaseDir(const dal_c_Project* proj);
static bool dal_c__pathContainsSegment(const char* path, const char* segment);
static bool dal_c__pathHasHiddenSegment(const char* path);
static bool dal_c__pathMatchesExclude(const char* path, const char* exclude_path);
static bool dal_c__isExcludedByProject(const dal_c_Project* proj, const char* path);
static bool dal_c__isSelfProjectDir(const char* path);
static char* dal_c__findSelfProjectDirFrom(const char* start_dir);
static char* dal_c__resolveSelfProjectDir(void);
static int dal_c__runSelfMake(const dal_c_Cmd* cmd, const char* target);
static bool dal_c__usesDhLibrary(const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
static dal_c__noinline dal_c__optnone bool dal_c__validateLinkToolchain(const dal_c_CompilerOpts* opts, dal_c_Target target_type);
static bool dal_c__validateBuildArtifacts(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, ArrStr* sources, dal_c_Target target_type);
static bool dal_c__targetUsesImplicitCompilerRt(dal_c_Target target_type);
static bool dal_c__resolvedDefaultLibsLinked(const dal_c_CompilerOpts* opts);
static dal_c__noinline dal_c__optnone char* dal_c__queryCompilerRtPath(const dal_c_CompilerOpts* opts);
static ArrStr* dal_c__parseQuotedTokens(const char* line);
static ArrStr* dal_c__queryToolchainLinkTokens(const dal_c_CompilerOpts* opts);
static void dal_c__printToolchainCategory(const char* title, ArrStr* link_tokens, const char* compiler_rt_path, dal_c_ToolchainQuery query);
static void dal_c__appendCompileDbArguments(ArrStr* argv, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* src, dal_c_Target target_type);
static void dal_c__appendCompileDbDiagnostics(ArrStr* argv, const dal_c_CompilerOpts* opts);
static char* dal_c__jsonEscape(const char* text);
static void dal_c__fprintJsonString(FILE* fp, const char* text);

// === PLATFORM ===

bool dal_c__platformIsWindows(void) {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

static char* dal_c__makeMakePath(const char* path) {
    assert(path != NULL);
    char* result = strdup(path);
    if (!result) {
        return NULL;
    }
    for (char* it = result; *it != '\0'; ++it) {
        if (*it == '\\') {
            *it = '/';
        }
    }
    return result;
}

static void dal_c__fprintMakePath(FILE* fp, const char* path) {
    assert(fp != NULL);
    assert(path != NULL);
    char* make_path = dal_c__makeMakePath(path);
    (void)fprintf(fp, "%s", make_path ? make_path : path);
    free(make_path);
}

static char* dal_c__buildBaseDir(const dal_c_Project* proj) {
    if (proj && proj->root) {
        return dal_c_Project_getBuildDir(proj);
    }
    char* cwd = env_getCWD();
    char* build_dir = cwd ? path_join(cwd, dal_c_dir_build) : strdup(dal_c_dir_build);
    free(cwd);
    return build_dir;
}

char* dal_c__cacheBaseDir(const dal_c_Project* proj) {
    char* build_dir = dal_c__buildBaseDir(proj);
    char* cache_dir = path_join(build_dir, dal_c_dir_cache);
    free(build_dir);
    return cache_dir;
}

static bool dal_c__pathContainsSegment(const char* path, const char* segment) {
    assert(path != NULL);
    assert(segment != NULL);

    const size_t segment_len = strlen(segment);
    const char* cursor = path;
    while (*cursor != '\0') {
        while (*cursor == '/' || *cursor == '\\') {
            ++cursor;
        }
        if (*cursor == '\0') {
            break;
        }

        const char* end = cursor;
        while (*end != '\0' && *end != '/' && *end != '\\') {
            ++end;
        }

        const size_t len = (size_t)(end - cursor);
        if (len == segment_len && strncmp(cursor, segment, segment_len) == 0) {
            return true;
        }
        cursor = end;
    }
    return false;
}

static bool dal_c__pathHasHiddenSegment(const char* path) {
    assert(path != NULL);

    const char* cursor = path;
    while (*cursor != '\0') {
        while (*cursor == '/' || *cursor == '\\') {
            ++cursor;
        }
        if (*cursor == '\0') {
            break;
        }

        const char* end = cursor;
        while (*end != '\0' && *end != '/' && *end != '\\') {
            ++end;
        }

        if (*cursor == '.') {
            return true;
        }
        cursor = end;
    }
    return false;
}

static bool dal_c__pathMatchesExclude(const char* path, const char* exclude_path) {
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

static bool dal_c__isExcludedByProject(const dal_c_Project* proj, const char* path) {
    if (!proj || !path) {
        return false;
    }
    for (int i = 0; i < proj->exclude_count; ++i) {
        if (dal_c__pathMatchesExclude(path, proj->exclude_paths[i])) {
            return true;
        }
    }
    return false;
}

static bool dal_c__isSelfProjectDir(const char* path) {
    if (!path || !path_isDir(path)) {
        return false;
    }

    char* makefile = path_join(path, dal_c_file_makefile);
    char* generator = path_join(path, "gen-makefile.sh");
    char* main_src = path_join(path, "src/dal-c.c");
    char* cmd_dir = path_join(path, "src/dal-c");
    bool is_self_dir = path_isFile(makefile)
                    && path_isFile(generator)
                    && path_isFile(main_src)
                    && path_isDir(cmd_dir);
    free(cmd_dir);
    free(main_src);
    free(generator);
    free(makefile);
    return is_self_dir;
}

static char* dal_c__findSelfProjectDirFrom(const char* start_dir) {
    if (!start_dir) {
        return NULL;
    }

    char* current = path_abs(start_dir);
    if (!current) {
        current = strdup(start_dir);
    }

    while (current) {
        if (dal_c__isSelfProjectDir(current)) {
            return current;
        }

        char* child = path_join(current, dal_c_tool_name);
        if (dal_c__isSelfProjectDir(child)) {
            free(current);
            return child;
        }
        free(child);

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(parent);
            free(current);
            return NULL;
        }
        free(current);
        current = parent;
    }

    return NULL;
}

static char* dal_c__resolveSelfProjectDir(void) {
    char* cwd = env_getCWD();
    char* resolved = dal_c__findSelfProjectDirFrom(cwd);
    free(cwd);
    if (resolved) {
        return resolved;
    }

    char* exe_dir = env_getExecutableDir();
    resolved = dal_c__findSelfProjectDirFrom(exe_dir);
    free(exe_dir);
    return resolved;
}

static void dal_c__pushSelfMakeKeyValue(ArrStr* argv, const char* key, const char* value) {
    assert(argv != NULL);
    assert(key != NULL);
    if (!value || value[0] == '\0') {
        return;
    }
    char* arg = str_format("%s=%s", key, value);
    ArrStr_push(argv, arg);
    free(arg);
}

static void dal_c__pushSelfMakeToggle(ArrStr* argv, const char* key, dal_c_ToggleState state) {
    if (state == dal_c_ToggleState_auto) {
        return;
    }
    dal_c__pushSelfMakeKeyValue(argv, key, dal_c_ToggleState_format(state));
}

static int dal_c__runSelfMake(const dal_c_Cmd* cmd, const char* target) {
    assert(cmd != NULL);

    char* self_dir = dal_c__resolveSelfProjectDir();
    if (!self_dir) {
        (void)fprintf(stderr, "Error: Failed to locate `" dal_c_tool_name "` self-build directory\n");
        return 1;
    }

    if (cmd->verbose) {
        (void)printf("Using self-build Makefile at %s\n", self_dir);
    }

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c_tool_make);
    ArrStr_push(argv, "-C");
    ArrStr_push(argv, self_dir);
    const char* profile_name = dal_c_Profile_format(cmd->opts.profile);
    if (profile_name) {
        char* profile_arg = str_format("PROFILE=%s", profile_name);
        ArrStr_push(argv, profile_arg);
        free(profile_arg);
    }
    dal_c__pushSelfMakeKeyValue(argv, "CC", cmd->opts.compiler);
    dal_c__pushSelfMakeKeyValue(argv, "C_STD", cmd->opts.c_std);
    dal_c__pushSelfMakeKeyValue(argv, "ARCH_TARGET", cmd->opts.arch_target);
    dal_c__pushSelfMakeKeyValue(argv, "TARGET_ARCH", cmd->opts.target_arch);
    dal_c__pushSelfMakeKeyValue(argv, "TARGET_TUNE", cmd->opts.target_tune);
    dal_c__pushSelfMakeKeyValue(argv, "TARGET_ABI", cmd->opts.target_abi);
    dal_c__pushSelfMakeKeyValue(argv, "SYSROOT", cmd->opts.sysroot);
    if (cmd->opts.lto_mode != dal_c_LtoMode_auto) {
        dal_c__pushSelfMakeKeyValue(argv, "LTO", dal_c_LtoMode_format(cmd->opts.lto_mode));
    }
    dal_c__pushSelfMakeToggle(argv, "OMIT_FRAME_POINTER", cmd->opts.omit_frame_pointer);
    dal_c__pushSelfMakeToggle(argv, "FUNCTION_SECTIONS", cmd->opts.function_sections);
    dal_c__pushSelfMakeToggle(argv, "DATA_SECTIONS", cmd->opts.data_sections);
    dal_c__pushSelfMakeToggle(argv, "GC_SECTIONS", cmd->opts.gc_sections);
    dal_c__pushSelfMakeToggle(argv, "LINK_LIBC", cmd->opts.libc_linked);
    dal_c__pushSelfMakeToggle(argv, "LINK_DEFAULT_LIBS", cmd->opts.default_libs_linked);
    dal_c__pushSelfMakeToggle(argv, "LINK_START_FILES", cmd->opts.start_files_linked);
    dal_c__pushSelfMakeToggle(argv, "LINK_COMPILER_RT", cmd->opts.compiler_rt_linked);
    if (cmd->opts.link_mode != dal_c_LinkMode_auto) {
        dal_c__pushSelfMakeKeyValue(argv, "LINK_MODE", dal_c_LinkMode_format(cmd->opts.link_mode));
    }
    dal_c__pushSelfMakeToggle(argv, "WHOLE_ARCHIVE", cmd->opts.whole_archive);
    dal_c__pushSelfMakeToggle(argv, "UNROLL_LOOPS", cmd->opts.unroll_loops);
    dal_c__pushSelfMakeToggle(argv, "UNWIND_TABLES", cmd->opts.unwind_tables);
    dal_c__pushSelfMakeToggle(argv, "ASYNC_UNWIND_TABLES", cmd->opts.async_unwind_tables);
    dal_c__pushSelfMakeToggle(argv, "EXCEPTIONS", cmd->opts.exceptions);
    dal_c__pushSelfMakeToggle(argv, "STRIP", cmd->opts.strip_mode);
    if (cmd->opts.icf_mode != dal_c_IcfMode_auto) {
        dal_c__pushSelfMakeKeyValue(argv, "ICF", dal_c_IcfMode_format(cmd->opts.icf_mode));
    }
    if (target) {
        ArrStr_push(argv, target);
    }

    if (cmd->show_commands) {
        (void)printf("Running:");
        for (int i = 0; i < ArrStr_len(argv); ++i) {
            (void)printf(" %s", ArrStr_at(argv, i));
        }
        (void)printf("\n");
    }

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        free(self_dir);
        return -1;
    }
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    free(self_dir);
    return result;
}

static bool dal_c__usesDhLibrary(const dal_c_Project* proj, const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return proj
        && proj->dh_path
        && dal_c_ToggleState_resolve(opts->dsl_mode, true)
        && !(proj->root && str_eql(proj->root, proj->dh_path));
}

static FILE* dal_c__openGeneratedFileForWrite(const char* path) {
    assert(path != NULL);
    return fopen(path, "w");
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

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_path) {
    ArrStr* sources = ArrStr_init();
    if (target_path) {
        ArrStr_push(sources, target_path);
    } else {
        assert(proj != NULL && proj->root != NULL);
        for (int i = 0; i < proj->self_root_count; ++i) {
            ArrStr* project_sources = dal_c__collectBuildFiles(proj->self_roots[i].path, true);
            for (int j = 0; j < ArrStr_len(project_sources); ++j) {
                const char* src = ArrStr_at(project_sources, j);
                if (!dal_c__isExcludedByProject(proj, src)) {
                    ArrStr_push(sources, src);
                }
            }
            ArrStr_fini(&project_sources);
        }
    }
    return sources;
}

ArrStr* dal_c__collectDirectoryFiles(const dal_c_Project* proj, const char* dir_name) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    assert(dir_name != NULL);

    char* dir = dal_c_Project_getCategoryDir(proj, dir_name);
    ArrStr* collected = dal_c__collectBuildFiles(dir, true);
    ArrStr* sources = ArrStr_init();
    for (int i = 0; i < ArrStr_len(collected); ++i) {
        const char* src = ArrStr_at(collected, i);
        if (!dal_c__isExcludedByProject(proj, src)) {
            ArrStr_push(sources, src);
        }
    }
    ArrStr_fini(&collected);
    free(dir);
    return sources;
}

static ArrStr* dal_c__collectBuildFiles(const char* dir, bool skip_source_paths) {
    ArrStr* files = ArrStr_init();
    ArrStr* c_files = dal_c__collectFilesWithExt(dir, ".c", skip_source_paths);
    ArrStr* upper_asm_files = dal_c__collectFilesWithExt(dir, ".S", skip_source_paths);
    ArrStr* lower_asm_files = dal_c__collectFilesWithExt(dir, ".s", skip_source_paths);
    for (int i = 0; i < ArrStr_len(c_files); ++i) {
        ArrStr_push(files, ArrStr_at(c_files, i));
    }
    for (int i = 0; i < ArrStr_len(upper_asm_files); ++i) {
        ArrStr_push(files, ArrStr_at(upper_asm_files, i));
    }
    for (int i = 0; i < ArrStr_len(lower_asm_files); ++i) {
        ArrStr_push(files, ArrStr_at(lower_asm_files, i));
    }
    ArrStr_fini(&lower_asm_files);
    ArrStr_fini(&upper_asm_files);
    ArrStr_fini(&c_files);
    return files;
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
    const char* target_path = intent.target_path;

    // Boundary: proj may be NULL for standalone header file
    if (!proj || !proj->root) {
        if (target_path && str_endsWith(target_path, ".h")) {
            if (cmd->verbose) {
                printf("Header-only library: %s (no project structure)\n", target_path);
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

    if (target_path && str_endsWith(target_path, ".h")) {
        if (!dal_c__copyHeaderRelativeTo(target_path, include_dir, deps_inc)) {
            (void)fprintf(stderr, "Error: Failed to copy header: %s\n", target_path);
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

static bool dal_c__projectHasTestSources(const dal_c_Project* proj);
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
        (void)fprintf(stderr, "Error: Dependency %s enables test=on but has no project.dh\n", lib->name);
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
    memset(&merged.payload, 0, sizeof(merged.payload));
    merged.action = dal_c_CmdAction_lib;
    merged.payload.lib.linking = lib->is_static ? dal_c_Linking_static : dal_c_Linking_shared;
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
        bool should_run_dependency_tests = lib_proj
                                        && (lib->test_enabled
                                            || (cmd->action == dal_c_CmdAction_test && cmd->payload.test.recursive));
        if (should_run_dependency_tests && (lib->test_enabled || dal_c__projectHasTestSources(lib_proj))) {
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
    char* saved_cwd = env_getCWD();
    if (!saved_cwd || !env_setCWD(build_proj.root)) {
        (void)fprintf(stderr, "Error: Failed to switch to dependency root: %s\n", build_proj.root);
        free(saved_cwd);
        ArrStr_fini(&lib_sources);
        free(lib_makefile_path);
        free(lib_object_dir);
        free(lib_target_path);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        free(lib_build_profile);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }

    if (dal_c__generateMakefile(&merged, &build_proj, lib_profile, lib_sources, lib_target_path, lib_object_dir, lib_target_type) != 0) {
        (void)env_setCWD(saved_cwd);
        free(saved_cwd);
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
    if (!env_setCWD(saved_cwd)) {
        (void)fprintf(stderr, "Error: Failed to restore working directory: %s\n", saved_cwd);
        result = 1;
    }
    free(saved_cwd);
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
    if (!dal_c__copyLibraryArtifacts(proj, lib_proj, lib, lib_abs_path, lib_build_profile, &merged, lib_profile, lib_target_type, is_windows)) {
        (void)fprintf(stderr, "Warning: Failed to copy some artifacts for %s\n", lib->name);
    }

    bool should_run_dependency_tests = lib_proj
                                    && (lib->test_enabled
                                        || (cmd->action == dal_c_CmdAction_test && cmd->payload.test.recursive));
    if (should_run_dependency_tests && (lib->test_enabled || dal_c__projectHasTestSources(lib_proj))) {
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

static bool dal_c__projectHasTestSources(const dal_c_Project* proj) {
    assert(proj != NULL);

    ArrStr* tests = dal_c__collectDirectoryFiles(proj, dal_c_dir_tests);
    bool has_tests = ArrStr_len(tests) > 0;
    ArrStr_fini(&tests);
    return has_tests;
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
    char* base_build_dir = dal_c__buildBaseDir(proj);
    char* build_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    assert(build_dir != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* target_path = dal_c__usesAggregateTestTarget(cmd) ? NULL : intent.target_path;

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_path) {
        char* basename = path_basename(target_path);
        char* dot = strrchr(basename, '.');
        if (dot && dot != basename) {
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
        dal_c__freeFileList(split, count);
    }

    if (cmd->show_commands) {
        printf("Running: ");
        for (int i = 0; i < ArrStr_len(argv); ++i) {
            printf("%s ", ArrStr_at(argv, i));
        }
        printf("\n");
    }

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        if (exe_name_alloc) { free(exe_name_alloc); }
        free(exe_path);
        return -1;
    }
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    if (exe_name_alloc) { free(exe_name_alloc); }
    free(exe_path);
    return result;
}

int dal_c__runDebugger(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    assert(proj != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* base_build_dir = dal_c__buildBaseDir(proj);
    char* build_dir = path_join(base_build_dir, profile->name);
    free(base_build_dir);
    assert(build_dir != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    const char* target_path = dal_c__usesAggregateTestTarget(cmd) ? NULL : intent.target_path;

    const char* exe_name = NULL;
    char* exe_name_alloc = NULL;
    if (target_path) {
        char* basename = path_basename(target_path);
        char* dot = strrchr(basename, '.');
        if (dot && dot != basename) {
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
        dal_c__freeFileList(split, count);
    }

    if (cmd->show_commands) {
        printf("Debugging: ");
        for (int i = 0; i < ArrStr_len(argv); ++i) {
            printf("%s ", ArrStr_at(argv, i));
        }
        printf("\n");
    }

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        if (exe_name_alloc) { free(exe_name_alloc); }
        free(exe_path);
        return -1;
    }
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    if (exe_name_alloc) { free(exe_name_alloc); }
    free(exe_path);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c__ensureLibDH(const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const dal_c_CompilerOpts* parent_opts) {
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
    char* saved_cwd = env_getCWD();
    if (!saved_cwd || !env_setCWD(dh_proj->root)) {
        (void)fprintf(stderr, "Error: Failed to switch to DH root: %s\n", dh_proj->root);
        free(saved_cwd);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        ArrStr_fini(&sources);
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }

    dal_c_Cmd cmd = { 0 };
    cmd.action = dal_c_CmdAction_lib;
    cmd.payload.lib.linking = dal_c_Linking_static;
    cmd.opts.profile = dal_c_Profile_invalid;
    dal_c_CompilerOpts_merge(&cmd.opts, &dh_proj->opts);
    if (parent_opts) {
        dal_c_CompilerOpts_merge(&cmd.opts, parent_opts);
    }
    cmd.opts.dsl_mode = dal_c_ToggleState_disabled;
    if (cmd.opts.profile == dal_c_Profile_invalid) {
        cmd.opts.profile = dal_c_Profile_parse(profile->name);
    }

    char* lib_target_path = dal_c__resolveOutputPath(dh_proj, &cmd, profile_dir, "dh", dal_c_Target_static_lib);
    char* makefile_path = dal_c__makePlanFilePath(dh_proj, profile, &cmd, lib_target_path, dal_c_Target_static_lib);
    int result = dal_c__generateMakefile(&cmd, dh_proj, profile, sources, lib_target_path, object_dir, dal_c_Target_static_lib);
    if (result == 0) {
        result = dal_c__executeMake(makefile_path);
    }
    if (!env_setCWD(saved_cwd)) {
        (void)fprintf(stderr, "Error: Failed to restore working directory: %s\n", saved_cwd);
        result = 1;
    }

    free(saved_cwd);
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
    return dal_c__ensureLibDH(&dh_proj, profile, &cmd->opts);
}

int dal_c__buildSelf(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    return dal_c__runSelfMake(cmd, NULL);
}

int dal_c__cleanDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(cmd != NULL);
    if (!proj || !proj->dh_path) {
        (void)fprintf(stderr, "Error: DH root not found\n");
        return 1;
    }

    bool cleaned = false;
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* dh_build = path_join(proj->dh_path, dal_c_dir_build);
    char* dh_build_target = NULL;
    if (cmd->profile_explicit && profile) {
        dh_build_target = path_join(dh_build, profile->name);
    } else {
        dh_build_target = strdup(dh_build);
    }
    if (path_isDir(dh_build_target)) {
        if (cmd->verbose) {
            printf("Removing: %s\n", dh_build_target);
        }
        dir_removeRecur(dh_build_target);
        printf("Cleaned: %s\n", dh_build_target);
        cleaned = true;
    }
    free(dh_build_target);

    char* dh_cache = path_join(dh_build, dal_c_dir_cache);
    char* dh_cache_target = NULL;
    if (cmd->profile_explicit && profile) {
        char* dh_cache_dh_c = path_join(dh_cache, dal_c_tool_name);
        dh_cache_target = path_join(dh_cache_dh_c, profile->name);
        free(dh_cache_dh_c);
    } else {
        dh_cache_target = strdup(dh_cache);
    }
    if (path_isDir(dh_cache_target)) {
        if (cmd->verbose) {
            printf("Removing: %s\n", dh_cache_target);
        }
        dir_removeRecur(dh_cache_target);
        printf("Cleaned: %s\n", dh_cache_target);
        cleaned = true;
    }
    free(dh_cache_target);
    free(dh_cache);
    free(dh_build);

    if (!cleaned) {
        printf("Nothing to clean\n");
    }
    return 0;
}

int dal_c__cleanSelf(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    return dal_c__runSelfMake(cmd, cmd->profile_explicit ? "clean-profile" : "clean");
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
    test_cmd.action = dal_c_CmdAction_test_dsl;
    test_cmd.payload.test.dsl_first = false;

    int result = dal_c_Cmd_makeTarget(&test_cmd, dh_proj);
    dal_c_Project_cleanup(&dh_proj);
    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
dal_c__noinline dal_c__optnone int dal_c__generateMakefile(
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
    if (dal_c__usesDhLibrary(proj, &cmd->opts)) {
        if (dal_c__ensureLibDH(proj, profile, &cmd->opts) != 0) {
            (void)fprintf(stderr, "Error: Failed to build libdh\n");
            return 1;
        }
    }
    if (!dal_c__validateLinkToolchain(&cmd->opts, target_type)) {
        return 1;
    }
    if (!dal_c__validateBuildArtifacts(cmd, profile, sources, target_type)) {
        return 1;
    }

    char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, target_type);
    char* makefile_dir = path_parent(makefile_path);
    dir_createRecur(makefile_dir);
    char* makefile_tmp = str_format("%s.tmp", makefile_path);
    char* link_contract_path = NULL;
    if (target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_image) {
        link_contract_path = dal_c__makeLinkContractPath(build_dir, target_path);
        if (!dal_c__writeLinkContractFile(link_contract_path, cmd, profile, target_type)) {
            (void)fprintf(stderr, "Error: Failed to write link contract: %s\n", link_contract_path);
            free(link_contract_path);
            free(makefile_tmp);
            free(makefile_dir);
            free(makefile_path);
            return 1;
        }
    }

    FILE* fp = dal_c__openGeneratedFileForWrite(makefile_tmp);
    if (!fp) {
        (void)fprintf(
            stderr,
            "Error: Failed to open Makefile for writing: %s (errno=%d: %s)\n",
            makefile_tmp,
            errno,
            strerror(errno)
        );
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }

    dal_c__writeMakefileVariables(fp, cmd, profile, proj, build_dir, target_type, target_path, link_contract_path);

    // Declare default goal before any rules to ensure 'all' is the default target
    (void)fprintf(fp, ".DEFAULT_GOAL := all\n\n");

    dal_c__writeMakefilePCH(fp, cmd, proj, profile, build_dir, target_type);

    if (dal_c__targetIsEmitOnly(target_type)) {
        return dal_c__writeEmitOnlyMakefile(
            fp, cmd, proj, profile, sources, target_path, build_dir, target_type,
            makefile_tmp, makefile_path, makefile_dir, link_contract_path
        );
    }

    return dal_c__writeLinkedMakefile(
        fp, cmd, proj, profile, sources, target_path, build_dir, target_type,
        makefile_tmp, makefile_path, makefile_dir, link_contract_path
    );
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

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        return -1;
    }
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
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

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        return -1;
    }
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
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
    case dal_c_Target_lib:
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
    case dal_c_Target_image:
        return str_endsWith(name, ".bin") ? strdup(name) : str_format("%s.bin", name);
    case dal_c_Target_preprocessed:
        return str_endsWith(name, ".i") ? strdup(name) : str_format("%s.i", name);
    case dal_c_Target_assembly:
        return str_endsWith(name, ".s") ? strdup(name) : str_format("%s.s", name);
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
    dal_c_TargetRequest request = { 0 };
    bool has_request = proj && dal_c_TargetRequest_resolve(proj, &intent, &request);
    if (!output_override || output_override[0] == '\0') {
        char* file_name = dal_c__makeTargetFileName(output_name, type, is_windows);
        char* output_root = strdup(build_dir);
        const char* context_dir = dal_c__planContextDir(proj, &intent, type);
        if (context_dir && !str_eql(context_dir, "targets")) {
            char* nested = path_join(output_root, context_dir);
            free(output_root);
            output_root = nested;
        }
        if (has_request && request.root && request.relative_path && request.relative_path[0] != '\0') {
            char* nested_dir = request.resolved_is_dir ? strdup(request.relative_path) : path_parent(request.relative_path);
            if (nested_dir && nested_dir[0] != '\0' && !str_eql(nested_dir, ".")) {
                char* nested = path_join(output_root, nested_dir);
                free(output_root);
                output_root = nested;
            }
            free(nested_dir);
        }
        char* output_path = path_join(output_root, file_name);
        dal_c_TargetRequest_cleanup(&request);
        free(output_root);
        free(file_name);
        return output_path;
    }

    if (!dal_c__pathHasSeparator(output_override)) {
        char* file_name = dal_c__makeTargetFileName(output_override, type, is_windows);
        char* output_root = strdup(build_dir);
        const char* context_dir = dal_c__planContextDir(proj, &intent, type);
        if (context_dir && !str_eql(context_dir, "targets")) {
            char* nested = path_join(output_root, context_dir);
            free(output_root);
            output_root = nested;
        }
        if (has_request && request.root && request.relative_path && request.relative_path[0] != '\0') {
            char* nested_dir = request.resolved_is_dir ? strdup(request.relative_path) : path_parent(request.relative_path);
            if (nested_dir && nested_dir[0] != '\0' && !str_eql(nested_dir, ".")) {
                char* nested = path_join(output_root, nested_dir);
                free(output_root);
                output_root = nested;
            }
            free(nested_dir);
        }
        char* output_path = path_join(output_root, file_name);
        dal_c_TargetRequest_cleanup(&request);
        free(output_root);
        free(file_name);
        return output_path;
    }

    if (dal_c__pathIsAbsolute(output_override)) {
        dal_c_TargetRequest_cleanup(&request);
        return strdup(output_override);
    }

    char* cwd = env_getCWD();
    char* output_path = cwd ? path_join(cwd, output_override) : strdup(output_override);
    dal_c_TargetRequest_cleanup(&request);
    free(cwd);
    return output_path;
}

static char* dal_c__makePdbPath(const char* target_path) {
    assert(target_path != NULL);
    char* basename = path_basename(target_path);
    char* stem = strdup(basename);
    char* dot = strrchr(stem, '.');
    if (dot && dot != stem) { *dot = '\0'; }
    char* pdb_name = str_format("%s.pdb", stem);
    char* parent = path_parent(target_path);
    char* pdb_path = parent ? path_join(parent, pdb_name) : strdup(pdb_name);
    free(parent);
    free(pdb_name);
    free(stem);
    free(basename);
    return pdb_path;
}

static char* dal_c__makeImageLinkPath(const char* target_path) {
    assert(target_path != NULL);
    char* parent = path_parent(target_path);
    char* basename = path_basename(target_path);
    char* stem = strdup(basename);
    char* dot = strrchr(stem, '.');
    if (dot && dot != stem) {
        *dot = '\0';
    }
    char* elf_name = str_format("%s.elf", stem);
    char* link_path = parent ? path_join(parent, elf_name) : strdup(elf_name);
    free(elf_name);
    free(stem);
    free(basename);
    free(parent);
    return link_path;
}

static bool dal_c__targetIsEmitOnly(dal_c_Target target_type) {
    return target_type == dal_c_Target_preprocessed || target_type == dal_c_Target_assembly;
}

static const char* dal_c__planContextDir(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_Target target_type) {
    assert(intent != NULL);
    if (proj) {
        dal_c_TargetRequest request = { 0 };
        if (dal_c_TargetRequest_resolve(proj, intent, &request) && request.root && request.root->name) {
            const char* context_name = request.root->name;
            dal_c_TargetRequest_cleanup(&request);
            return context_name;
        }
        dal_c_TargetRequest_cleanup(&request);
    }
    if (intent->action == dal_c_CmdAction_build_dsl) {
        return "dsl";
    }
    if (target_type == dal_c_Target_static_lib || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_lib) {
        return "libs";
    }
    if (target_type == dal_c_Target_image) {
        return "images";
    }
    if (target_type == dal_c_Target_preprocessed) {
        return "preprocessed";
    }
    if (target_type == dal_c_Target_assembly) {
        return "assembly";
    }
    if (intent->action == dal_c_CmdAction_test || intent->action == dal_c_CmdAction_test_dsl) {
        return proj ? dal_c_Project_getCategoryDirName(proj, dal_c_dir_tests) : dal_c_dir_tests;
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
    dal_c_TargetRequest request = { 0 };
    char* target_key = NULL;
    if (proj && dal_c_TargetRequest_resolve(proj, &intent, &request) && request.root && request.relative_path && request.relative_path[0] != '\0') {
        target_key = strdup(request.relative_path);
    }
    if (!target_key) {
        target_key = path_basename(target_path);
    }
    char* target_name = dal_c__sanitizePathFragment(target_key);
    char* plan_name = str_format("%s.mk", target_name);
    char* plan_path = path_join(context_dir, plan_name);
    free(plan_name);
    free(target_name);
    free(target_key);
    dal_c_TargetRequest_cleanup(&request);
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

static const char** dal_c__makeConstArgvView(ArrStr* argv) {
    assert(argv != NULL);

    int count = ArrStr_len(argv);
    const char** raw_argv = (const char**)calloc((size_t)count + 1, sizeof(const char*));
    if (!raw_argv) { return NULL; }

    for (int i = 0; i < count; ++i) {
        raw_argv[i] = ArrStr_at(argv, i);
    }
    raw_argv[count] = NULL;
    return raw_argv;
}

bool dal_c__shouldSkipAutoSourcePath(const char* path) {
    assert(path != NULL);
    if (dal_c__pathHasHiddenSegment(path)) {
        return true;
    }
    const char* const* skip_segments = dal_c_sourceSkipSegments();
    for (int i = 0; skip_segments[i] != NULL; ++i) {
        if (dal_c__pathContainsSegment(path, skip_segments[i])) {
            return true;
        }
    }
    return false;
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
        char* relative_path = NULL;
        if (skip_source_paths) {
            relative_path = path_relative(dir, files[i]);
            const char* candidate_path = relative_path ? relative_path : files[i];
            if (dal_c__shouldSkipAutoSourcePath(candidate_path)) {
                free(relative_path);
                continue;
            }
        }
        if (str_endsWith(files[i], ext)) {
            ArrStr_push(files_with_ext, files[i]);
        }
        free(relative_path);
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

    if (lib_proj && lib_proj->self_root_count > 0) {
        for (int i = 0; i < lib_proj->self_root_count; ++i) {
            ArrStr* lib_files = dal_c__collectBuildFiles(lib_proj->self_roots[i].path, false);
            for (int j = 0; j < ArrStr_len(lib_files); ++j) {
                ArrStr_push(sources, ArrStr_at(lib_files, j));
            }
            ArrStr_fini(&lib_files);
        }
    } else {
        char* lib_src = lib_proj ? dal_c_Project_getSrcDir(lib_proj) : path_join(lib_abs, dal_c_dir_src);
        ArrStr* lib_files = dal_c__collectBuildFiles(lib_src, false);
        for (int i = 0; i < ArrStr_len(lib_files); ++i) {
            ArrStr_push(sources, ArrStr_at(lib_files, i));
        }
        ArrStr_fini(&lib_files);
        free(lib_src);
    }
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

static bool dal_c__copyLibraryArtifacts(
    const dal_c_Project* consumer_proj,
    const dal_c_Project* lib_proj,
    const dal_c_Lib* lib,
    const char* lib_abs_path,
    const char* lib_build_dir,
    const dal_c_Cmd* build_cmd,
    const dal_c_ProfileSpec* build_profile,
    dal_c_Target build_target_type,
    bool is_windows
) {
    assert(consumer_proj != NULL);
    assert(lib != NULL);
    assert(lib_abs_path != NULL);
    assert(lib_build_dir != NULL);
    assert(build_cmd != NULL);
    assert(build_profile != NULL);

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

    char* lib_file = str_format("%s%s%s", lib_prefix, lib_basename, lib_ext);
    char* lib_name_with_ext = str_format("%s%s", lib->name, lib_ext);
    char* lib_src_path = NULL;
    const char* candidate_rel_paths[] = {
        lib_file,
        lib_name_with_ext,
        NULL,
        NULL,
    };
    char* libs_file = path_join("libs", lib_file);
    char* libs_name_with_ext = path_join("libs", lib_name_with_ext);
    candidate_rel_paths[2] = libs_file;
    candidate_rel_paths[3] = libs_name_with_ext;
    for (int i = 0; i < 4; ++i) {
        char* candidate = path_join(lib_build_dir, candidate_rel_paths[i]);
        if (path_isFile(candidate)) {
            lib_src_path = candidate;
            break;
        }
        free(candidate);
    }
    if (!lib_src_path) {
        lib_src_path = path_join(lib_build_dir, libs_name_with_ext);
    }

    char* lib_dst_path = path_join(target_dir, lib_file);

    if (path_isFile(lib_src_path)) {
        if (!file_copy(lib_src_path, lib_dst_path)) {
            success = false;
        }
    }
    free(libs_name_with_ext);
    free(libs_file);
    free(lib_name_with_ext);
    free(lib_file);
    free(lib_src_path);
    free(lib_dst_path);

    // 3. Copy current PCH artifact if present.
    if (lib_proj && lib_proj->pch_header) {
        char* pch_basename = path_basename(lib_proj->pch_header);
        char* pch_contract_key = dal_c__makeCompileContractKey(build_cmd, build_profile, build_target_type, true, false);
        char* pch_src_name = str_format("%s_%s.pch", pch_contract_key, pch_basename);
        char* pch_obj_dir = path_join(lib_build_dir, "obj");
        char* pch_src_path = path_join(pch_obj_dir, pch_src_name);
        char* pch_dst_name = str_format("%s.pch", pch_basename);
        char* pch_dst_path = path_join(target_dir, pch_dst_name);
        if (path_isFile(pch_src_path) && !dal_c__copyHeaderToPathIfChanged(pch_src_path, pch_dst_path)) {
            success = false;
        }
        free(pch_dst_path);
        free(pch_dst_name);
        free(pch_src_path);
        free(pch_obj_dir);
        free(pch_src_name);
        free(pch_contract_key);
        free(pch_basename);
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

static dal_c_CompileEnv dal_c__resolvedCompileEnv(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return dal_c_CompileEnv_resolve(opts->compile_env);
}

static bool dal_c__resolvedToggle(const dal_c_ToggleState state, const bool default_enabled) {
    return dal_c_ToggleState_resolve(state, default_enabled);
}

static bool dal_c__resolvedDslMode(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return dal_c__resolvedToggle(opts->dsl_mode, true);
}

static dal_c_ToggleState dal_c__resolvedProfileToggleState(dal_c_ToggleState override, dal_c_ToggleState profile_default) {
    return override != dal_c_ToggleState_auto ? override : profile_default;
}

static bool dal_c__resolvedProfileToggleEnabled(dal_c_ToggleState override, dal_c_ToggleState profile_default) {
    return dal_c__resolvedProfileToggleState(override, profile_default) == dal_c_ToggleState_enabled;
}

static dal_c_LtoMode dal_c__resolvedProfileLtoMode(dal_c_LtoMode override, dal_c_LtoMode profile_default) {
    return override != dal_c_LtoMode_auto ? override : profile_default;
}

static dal_c_LtoMode dal_c__resolvedLtoState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileLtoMode(opts->lto_mode, profile->lto_mode);
}

static bool dal_c__resolvedLtoMode(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    return dal_c_LtoMode_isEnabled(dal_c__resolvedLtoState(opts, profile));
}

static dal_c_ToggleState dal_c__resolvedOmitFramePointerState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleState(opts->omit_frame_pointer, profile->omit_frame_pointer);
}

static bool dal_c__resolvedFunctionSections(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleEnabled(opts->function_sections, profile->function_sections);
}

static bool dal_c__resolvedDataSections(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleEnabled(opts->data_sections, profile->data_sections);
}

static bool dal_c__resolvedGcSections(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleEnabled(opts->gc_sections, profile->gc_sections);
}

static bool dal_c__resolvedWholeArchive(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleEnabled(opts->whole_archive, profile->whole_archive);
}

static dal_c_ToggleState dal_c__resolvedUnrollLoopsState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleState(opts->unroll_loops, profile->unroll_loops);
}

static dal_c_ToggleState dal_c__resolvedUnwindTablesState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleState(opts->unwind_tables, profile->unwind_tables);
}

static dal_c_ToggleState dal_c__resolvedAsyncUnwindTablesState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleState(opts->async_unwind_tables, profile->async_unwind_tables);
}

static bool dal_c__resolvedStripMode(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleEnabled(opts->strip_mode, profile->strip_mode);
}

static dal_c_IcfMode dal_c__resolvedIcfMode(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return opts->icf_mode != dal_c_IcfMode_auto ? opts->icf_mode : profile->icf_mode;
}

static dal_c_ToggleState dal_c__resolvedMergeAllConstantsState(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return opts->merge_all_constants;
}

static dal_c_ToggleState dal_c__resolvedStackProtectorState(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return opts->stack_protector;
}

static const char* dal_c__resolvedTargetArch(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    if (opts->target_arch && opts->target_arch[0] != '\0') {
        return str_eql(opts->target_arch, "auto") ? NULL : opts->target_arch;
    }
    return profile->target_arch;
}

static const char* dal_c__resolvedTargetTune(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    if (opts->target_tune && opts->target_tune[0] != '\0') {
        return str_eql(opts->target_tune, "auto") ? NULL : opts->target_tune;
    }
    return profile->target_tune;
}

static dal_c_ToggleState dal_c__resolvedExceptionsState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(opts != NULL);
    assert(profile != NULL);
    return dal_c__resolvedProfileToggleState(opts->exceptions, profile->exceptions);
}

static bool dal_c__isLtoFlag(const char* flag) {
    return flag && (str_eql(flag, "-flto") || str_eql(flag, "-fno-lto") || str_startsWith(flag, "-flto="));
}

static bool dal_c__targetSupportsNoLibcFlag(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    const char* target = opts->arch_target;
    if (target && target[0] != '\0') {
        return strstr(target, "windows") == NULL
            && strstr(target, "mingw") == NULL
            && strstr(target, "msvc") == NULL;
    }
#ifdef _WIN32
    return false;
#else
    return true;
#endif
}

static bool dal_c__requestedLibcLinked(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    bool default_linked = dal_c__resolvedCompileEnv(opts) != dal_c_CompileEnv_freestanding;
    return dal_c__resolvedToggle(opts->libc_linked, default_linked);
}

static bool dal_c__resolvedLibcLinked(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    if (!default_libs_linked) {
        return false;
    }
    bool requested_linked = dal_c__requestedLibcLinked(opts);
    if (requested_linked) {
        return true;
    }
    return !dal_c__targetSupportsNoLibcFlag(opts);
}

static bool dal_c__resolvedDefaultLibsLinked(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return dal_c__resolvedToggle(opts->default_libs_linked, true);
}

static bool dal_c__resolvedStartFilesLinked(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return dal_c__resolvedToggle(opts->start_files_linked, true);
}

static bool dal_c__resolvedCompilerRtLinked(const dal_c_CompilerOpts* opts, dal_c_Target target_type) {
    assert(opts != NULL);
    if (!dal_c__targetUsesImplicitCompilerRt(target_type)) {
        return false;
    }
    if (dal_c__resolvedDefaultLibsLinked(opts)) {
        return true;
    }
    return dal_c__resolvedToggle(opts->compiler_rt_linked, true);
}

static bool dal_c__linkNeedsCompilerRt(const dal_c_CompilerOpts* opts, dal_c_Target target_type) {
    assert(opts != NULL);
    return !dal_c__resolvedDefaultLibsLinked(opts)
        && dal_c__resolvedCompilerRtLinked(opts, target_type);
}

static bool dal_c__targetUsesImplicitCompilerRt(dal_c_Target target_type) {
    return target_type != dal_c_Target_image;
}

static bool dal_c__targetLooksLikeWasi(const char* target) {
    return target != NULL
        && (strstr(target, "wasi") != NULL || strstr(target, "wasm") != NULL);
}

static dal_c__noinline dal_c__optnone char* dal_c__queryCompilerRtPath(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);

    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    char* sysroot_flag = NULL;
    char* target_flag = NULL;
    char* target_value = NULL;
    char* target_arch_flag = NULL;
    char* target_abi_flag = NULL;
    char* print_flag = NULL;
    char* compiler_copy = strdup(compiler);
    const char* argv[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    int argc = 1;
    argv[0] = compiler_copy;
    if (!compiler_copy) {
        return NULL;
    }

    if (opts->arch_target && opts->arch_target[0] != '\0') {
        target_flag = strdup("-target");
        target_value = strdup(opts->arch_target);
        if (!target_flag || !target_value) {
            free(compiler_copy);
            free(target_flag);
            free(target_value);
            return NULL;
        }
        argv[argc++] = target_flag;
        argv[argc++] = target_value;
    }
    if (opts->target_arch && opts->target_arch[0] != '\0') {
        target_arch_flag = str_format("-march=%s", opts->target_arch);
        if (!target_arch_flag) {
            free(compiler_copy);
            free(target_flag);
            free(target_value);
            return NULL;
        }
        argv[argc++] = target_arch_flag;
    }
    if (opts->target_abi && opts->target_abi[0] != '\0') {
        target_abi_flag = str_format("-mabi=%s", opts->target_abi);
        if (!target_abi_flag) {
            free(compiler_copy);
            free(target_flag);
            free(target_value);
            free(target_arch_flag);
            return NULL;
        }
        argv[argc++] = target_abi_flag;
    }
    if (opts->sysroot && opts->sysroot[0] != '\0') {
        sysroot_flag = str_format("--sysroot=%s", opts->sysroot);
        if (!sysroot_flag) {
            free(compiler_copy);
            free(target_flag);
            free(target_value);
            free(target_arch_flag);
            free(target_abi_flag);
            return NULL;
        }
        argv[argc++] = sysroot_flag;
    }
    print_flag = strdup("--print-libgcc-file-name");
    if (!print_flag) {
        free(compiler_copy);
        free(target_flag);
        free(target_value);
        free(target_arch_flag);
        free(target_abi_flag);
        free(sysroot_flag);
        return NULL;
    }
    argv[argc++] = print_flag;
    argv[argc] = NULL;

    char* output = proc_output(argv);
    free(print_flag);
    free(sysroot_flag);
    free(target_abi_flag);
    free(target_arch_flag);
    free(target_value);
    free(target_flag);
    free(compiler_copy);
    if (!output) {
        return NULL;
    }

    char* trimmed = str_trim(output);
    free(output);
    if (!trimmed) {
        return NULL;
    }

    const char* last_line = strrchr(trimmed, '\n');
    if (last_line) {
        char* last_trimmed = str_trim(last_line + 1);
        if (last_trimmed && last_trimmed[0] != '\0') {
            free(trimmed);
            return last_trimmed;
        }
        free(last_trimmed);
    }
    return trimmed;
}

static bool dal_c__tokenEndsWith(const char* token, const char* suffix) {
    assert(token != NULL);
    assert(suffix != NULL);
    size_t token_len = strlen(token);
    size_t suffix_len = strlen(suffix);
    return suffix_len <= token_len && strcmp(token + token_len - suffix_len, suffix) == 0;
}

static const char* dal_c__tokenBasename(const char* token) {
    assert(token != NULL);
    const char* slash = strrchr(token, '/');
    const char* backslash = strrchr(token, '\\');
    const char* sep = slash > backslash ? slash : backslash;
    return sep ? sep + 1 : token;
}

static bool dal_c__toolchainTokenIsStartFile(const char* token) {
    const char* base = dal_c__tokenBasename(token);
    return str_eql(base, "crt1.o")
        || str_eql(base, "crt2.o")
        || str_eql(base, "Scrt1.o")
        || str_eql(base, "gcrt1.o")
        || str_eql(base, "crti.o")
        || str_eql(base, "crtn.o")
        || str_startsWith(base, "crtbegin")
        || str_startsWith(base, "crtend");
}

static bool dal_c__toolchainTokenIsCompilerRt(const char* token, const char* compiler_rt_path) {
    return (compiler_rt_path && compiler_rt_path[0] != '\0' && str_eql(token, compiler_rt_path))
        || strstr(token, "libclang_rt") != NULL
        || strstr(token, "libgcc") != NULL;
}

static bool dal_c__toolchainTokenIsLib(const char* token) {
    return str_startsWith(token, "-l")
        || dal_c__tokenEndsWith(token, ".a")
        || dal_c__tokenEndsWith(token, ".lib");
}

static bool dal_c__toolchainTokenIsLinkInput(const char* token) {
    if (!token || token[0] == '\0') {
        return false;
    }
    if (str_startsWith(token, "-L")
        || str_eql(token, "-o")
        || str_eql(token, "-m")
        || str_eql(token, "-Bdynamic")
        || str_eql(token, "-Bstatic")
        || str_eql(token, "-shared")) {
        return false;
    }
    return dal_c__toolchainTokenIsStartFile(token)
        || dal_c__toolchainTokenIsLib(token);
}

static bool dal_c__toolchainTokenIsLibc(const char* token) {
    return str_eql(token, "-lc")
        || str_eql(token, "-lmsvcrt")
        || str_eql(token, "-lucrt")
        || str_eql(token, "-lcrtdll");
}

static bool dal_c__toolchainTokenIsCrt(const char* token) {
    return dal_c__toolchainTokenIsStartFile(token)
        || str_eql(token, "-lmingw32")
        || str_eql(token, "-lmingwex")
        || str_eql(token, "-lmoldname")
        || dal_c__toolchainTokenIsLibc(token);
}

static ArrStr* dal_c__parseQuotedTokens(const char* line) {
    assert(line != NULL);
    ArrStr* tokens = ArrStr_init();
    if (!tokens) {
        return NULL;
    }
    const char* cursor = line;
    while ((cursor = strchr(cursor, '"')) != NULL) {
        ++cursor;
        const char* end = strchr(cursor, '"');
        if (!end) {
            break;
        }
        size_t len = (size_t)(end - cursor);
        char* token = (char*)malloc(len + 1);
        if (!token) {
            ArrStr_fini(&tokens);
            return NULL;
        }
        memcpy(token, cursor, len);
        token[len] = '\0';
        ArrStr_push(tokens, token);
        free(token);
        cursor = end + 1;
    }
    return tokens;
}

static void dal_c__argvPushFormat(ArrStr* argv, const char* fmt, ...) dal_c__printf_format(2, 3);
static void dal_c__argvPushFormat(ArrStr* argv, const char* fmt, ...) {
    assert(argv != NULL);
    assert(fmt != NULL);
    va_list args;
    va_start(args, fmt);
    va_list copy;
    va_copy(copy, args);
    int size = vsnprintf(NULL, 0, fmt, copy);
    va_end(copy);
    if (size < 0) {
        va_end(args);
        return;
    }
    char* text = (char*)malloc((size_t)size + 1);
    if (!text) {
        va_end(args);
        return;
    }
    (void)vsnprintf(text, (size_t)size + 1, fmt, args);
    va_end(args);
    ArrStr_push(argv, text);
    free(text);
}

static void dal_c__appendCompilerArgsTokens(ArrStr* argv, const char* args) {
    assert(argv != NULL);
    if (!args || args[0] == '\0') {
        return;
    }

    const char* cursor = args;
    while (*cursor != '\0') {
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') {
            ++cursor;
        }
        if (*cursor == '\0') {
            break;
        }

        size_t cap = 32;
        size_t len = 0;
        char* token = (char*)malloc(cap);
        if (!token) {
            return;
        }

        char quote = '\0';
        while (*cursor != '\0') {
            char ch = *cursor;
            if (quote == '\0' && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')) {
                break;
            }
            if ((ch == '"' || ch == '\'') && (quote == '\0' || quote == ch)) {
                quote = quote == '\0' ? ch : '\0';
                ++cursor;
                continue;
            }
            if (ch == '\\' && cursor[1] != '\0') {
                ++cursor;
                ch = *cursor;
            }
            if (len + 2 > cap) {
                cap *= 2;
                char* grown = (char*)realloc(token, cap);
                if (!grown) {
                    free(token);
                    return;
                }
                token = grown;
            }
            token[len++] = ch;
            ++cursor;
        }
        token[len] = '\0';
        if (len > 0) {
            ArrStr_push(argv, token);
        }
        free(token);
    }
}

static void dal_c__appendCompileDbArguments(
    ArrStr* argv,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    const char* src,
    dal_c_Target target_type
) {
    assert(argv != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(src != NULL);
    (void)target_type;

    const dal_c_CompilerOpts* opts = &cmd->opts;
    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    ArrStr_push(argv, compiler);

    if (str_endsWith(src, ".h")) {
        ArrStr_push(argv, "-xc-header");
    } else if (str_endsWith(src, ".S")) {
        ArrStr_push(argv, "-x");
        ArrStr_push(argv, "assembler-with-cpp");
    } else if (str_endsWith(src, ".s")) {
        ArrStr_push(argv, "-x");
        ArrStr_push(argv, "assembler");
    } else {
        ArrStr_push(argv, "-xc");
    }
    if (!dal_c__sourceIsAssembly(src)) {
        const char* c_std = opts->c_std ? opts->c_std : dal_c_default_c_std;
        dal_c__argvPushFormat(argv, "-std=%s", c_std);
    }

    ArrStr_push(argv, "-fgnu-keywords");
    ArrStr_push(argv, "-fms-extensions");
    ArrStr_push(argv, "-Wno-microsoft-anon-tag");
    ArrStr_push(argv, "-funsigned-char");
    dal_c__appendCompileDbDiagnostics(argv, opts);

    if (opts->arch_target) {
        ArrStr_push(argv, "-target");
        ArrStr_push(argv, opts->arch_target);
    }
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    if (target_arch) {
        dal_c__argvPushFormat(argv, "-march=%s", target_arch);
    }
    const char* target_tune = dal_c__resolvedTargetTune(opts, profile);
    if (target_tune) {
        dal_c__argvPushFormat(argv, "-mtune=%s", target_tune);
    }
    if (opts->target_abi) {
        dal_c__argvPushFormat(argv, "-mabi=%s", opts->target_abi);
    }
    if (opts->sysroot) {
        dal_c__argvPushFormat(argv, "--sysroot=%s", opts->sysroot);
    }
    if (dal_c__resolvedCompileEnv(opts) == dal_c_CompileEnv_freestanding) {
        ArrStr_push(argv, "-ffreestanding");
    }
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    if (exceptions == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fexceptions");
    } else if (exceptions == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-exceptions");
    }

    /* Keep clangd away from build-only COMP branches. They make dh's preamble
     * large enough to trip malformed AST failures on clangd 22 for Windows. */
    if (!profile->debug_assertions) {
        ArrStr_push(argv, "-DNDEBUG");
    }

    if (proj && proj->root) {
        char* include_dir = path_join(proj->root, dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, include_dir);
        free(include_dir);
    }
    if (dal_c__usesDhLibrary(proj, opts)) {
        char* dh_include = path_join(proj->dh_path, dal_c_dir_include);
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, dh_include);
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, proj->dh_path);
        free(dh_include);
    }
    if (proj && proj->root && proj->lib_count > 0) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, deps_dir);
        free(deps_dir);
    }
    for (int i = 0; i < opts->include_count; ++i) {
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, opts->include_paths[i]);
    }
    for (int i = 0; i < opts->isystem_count; ++i) {
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, opts->isystem_paths[i]);
    }
    for (int i = 0; i < opts->define_count; ++i) {
        dal_c__argvPushFormat(argv, "-D%s", opts->define_macros[i]);
    }
    for (int i = 0; i < opts->undef_count; ++i) {
        dal_c__argvPushFormat(argv, "-U%s", opts->undef_macros[i]);
    }
    dal_c__appendCompilerArgsTokens(argv, cmd->compiler_args);
    ArrStr_push(argv, src);
}

static void dal_c__appendCompileDbDiagnostics(ArrStr* argv, const dal_c_CompilerOpts* opts) {
    assert(argv != NULL);
    assert(opts != NULL);

    if (opts->loose_errors == dal_c_LooseErrorsMode_suppress) {
        ArrStr_push(argv, "-w");
        return;
    }

    if (opts->loose_errors == dal_c_LooseErrorsMode_warn) {
        const char* flags[] = {
            "-Wall",
            "-Wextra",
            "-Wconversion",
            "-Wsign-conversion",
            "-Wfloat-conversion",
            "-Wformat=2",
            "-Wcast-qual",
            "-Wcast-align",
            "-Wpointer-arith",
            "-Wbad-function-cast",
            "-Wnull-dereference",
            "-Wwrite-strings",
            "-Wuninitialized",
            "-Wframe-larger-than=4096",
            "-Wno-switch-enum",
            "-Winfinite-recursion",
            "-Wno-microsoft-anon-tag",
            "-Wloop-analysis",
            "-Wstrict-prototypes",
            "-Wmissing-prototypes",
            "-Wmissing-variable-declarations",
            "-Wdiv-by-zero",
            "-Wthread-safety",
            NULL
        };
        for (int i = 0; flags[i] != NULL; ++i) {
            ArrStr_push(argv, flags[i]);
        }
        return;
    }

    const char* flags[] = {
        "-Werror=all",
        "-Werror=extra",
        "-Werror=conversion",
        "-Werror=sign-conversion",
        "-Wfloat-conversion",
        "-Wformat=2",
        "-Werror=cast-qual",
        "-Werror=cast-align",
        "-Wpointer-arith",
        "-Wbad-function-cast",
        "-Wnull-dereference",
        "-Wwrite-strings",
        "-Werror=uninitialized",
        "-Wframe-larger-than=4096",
        "-Wno-switch-enum",
        "-Winfinite-recursion",
        "-Wno-microsoft-anon-tag",
        "-Wloop-analysis",
        "-Werror=strict-prototypes",
        "-Werror=missing-prototypes",
        "-Wmissing-variable-declarations",
        "-Werror=div-by-zero",
        "-Wthread-safety",
        NULL
    };
    for (int i = 0; flags[i] != NULL; ++i) {
        ArrStr_push(argv, flags[i]);
    }
}

static char* dal_c__jsonEscape(const char* text) {
    if (!text) {
        return strdup("");
    }
    size_t cap = strlen(text) * 2 + 1;
    char* out = (char*)malloc(cap);
    if (!out) {
        return NULL;
    }
    size_t len = 0;
    for (const unsigned char* it = (const unsigned char*)text; *it != '\0'; ++it) {
        char escaped[8] = { 0 };
        const char* piece = NULL;
        if (*it == '\\') {
            piece = "\\\\";
        } else if (*it == '"') {
            piece = "\\\"";
        } else if (*it == '\n') {
            piece = "\\n";
        } else if (*it == '\r') {
            piece = "\\r";
        } else if (*it == '\t') {
            piece = "\\t";
        } else if (*it < 0x20) {
            (void)snprintf(escaped, sizeof(escaped), "\\u%04x", (unsigned)*it);
            piece = escaped;
        }
        if (piece) {
            size_t piece_len = strlen(piece);
            if (len + piece_len + 1 > cap) {
                cap = (len + piece_len + 1) * 2;
                char* grown = (char*)realloc(out, cap);
                if (!grown) {
                    free(out);
                    return NULL;
                }
                out = grown;
            }
            memcpy(out + len, piece, piece_len);
            len += piece_len;
        } else {
            if (len + 2 > cap) {
                cap = (len + 2) * 2;
                char* grown = (char*)realloc(out, cap);
                if (!grown) {
                    free(out);
                    return NULL;
                }
                out = grown;
            }
            out[len++] = (char)*it;
        }
    }
    out[len] = '\0';
    return out;
}

static void dal_c__fprintJsonString(FILE* fp, const char* text) {
    assert(fp != NULL);
    char* escaped = dal_c__jsonEscape(text);
    (void)fprintf(fp, "\"%s\"", escaped ? escaped : "");
    free(escaped);
}

int dal_c__writeCompileDb(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* output_path,
    dal_c_Target target_type
) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(output_path != NULL);

    char* output_parent = path_parent(output_path);
    if (output_parent) {
        dir_createRecur(output_parent);
    }
    free(output_parent);

    FILE* fp = fopen(output_path, "w");
    if (!fp) {
        (void)fprintf(stderr, "Error: Failed to open compile database: %s\n", output_path);
        return 1;
    }

    char* cwd = env_getCWD();
    const char* directory = (proj && proj->root) ? proj->root : (cwd ? cwd : ".");
    (void)fprintf(fp, "[\n");
    for (int i = 0; i < ArrStr_len(sources); ++i) {
        const char* src = ArrStr_at(sources, i);
        ArrStr* argv = ArrStr_init();
        dal_c__appendCompileDbArguments(argv, cmd, proj, profile, src, target_type);

        (void)fprintf(fp, "  {\n");
        (void)fprintf(fp, "    \"directory\": ");
        dal_c__fprintJsonString(fp, directory);
        (void)fprintf(fp, ",\n");
        (void)fprintf(fp, "    \"file\": ");
        dal_c__fprintJsonString(fp, src);
        (void)fprintf(fp, ",\n");
        (void)fprintf(fp, "    \"arguments\": [");
        for (int j = 0; j < ArrStr_len(argv); ++j) {
            if (j > 0) {
                (void)fprintf(fp, ", ");
            }
            dal_c__fprintJsonString(fp, ArrStr_at(argv, j));
        }
        (void)fprintf(fp, "]\n");
        (void)fprintf(fp, "  }%s\n", i + 1 < ArrStr_len(sources) ? "," : "");
        ArrStr_fini(&argv);
    }
    (void)fprintf(fp, "]\n");
    free(cwd);
    return fclose(fp) == 0 ? 0 : 1;
}

static bool dal_c__tokensLookLikeLinkerInvocation(const ArrStr* tokens) {
    if (!tokens || ArrStr_len(tokens) == 0) {
        return false;
    }
    const char* linker = dal_c__tokenBasename(ArrStr_at(tokens, 0));
    return str_startsWith(linker, "ld")
        || strstr(linker, "ld.") != NULL
        || strstr(linker, "lld") != NULL;
}

static ArrStr* dal_c__queryToolchainLinkTokens(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);

    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    const char* probe_input =
#ifdef _WIN32
        "NUL";
#else
        "/dev/null";
#endif
    char* sysroot_flag = NULL;
    char* target_flag = NULL;
    char* target_value = NULL;
    char* target_arch_flag = NULL;
    char* target_abi_flag = NULL;
    const char* argv[16] = { NULL };
    int argc = 0;
    argv[argc++] = compiler;
    if (opts->arch_target && opts->arch_target[0] != '\0') {
        target_flag = strdup("-target");
        target_value = strdup(opts->arch_target);
        if (!target_flag || !target_value) { goto fail; }
        argv[argc++] = target_flag;
        argv[argc++] = target_value;
    }
    if (opts->target_arch && opts->target_arch[0] != '\0') {
        target_arch_flag = str_format("-march=%s", opts->target_arch);
        if (!target_arch_flag) { goto fail; }
        argv[argc++] = target_arch_flag;
    }
    if (opts->target_abi && opts->target_abi[0] != '\0') {
        target_abi_flag = str_format("-mabi=%s", opts->target_abi);
        if (!target_abi_flag) { goto fail; }
        argv[argc++] = target_abi_flag;
    }
    if (opts->sysroot && opts->sysroot[0] != '\0') {
        sysroot_flag = str_format("--sysroot=%s", opts->sysroot);
        if (!sysroot_flag) { goto fail; }
        argv[argc++] = sysroot_flag;
    }
    argv[argc++] = "-###";
    argv[argc++] = "-x";
    argv[argc++] = "c";
    argv[argc++] = probe_input;
    argv[argc++] = "-o";
    argv[argc++] = "dh-c-toolchain-probe";
    argv[argc] = NULL;

    char* output = proc_output(argv);
    free(sysroot_flag);
    free(target_abi_flag);
    free(target_arch_flag);
    free(target_value);
    free(target_flag);
    if (!output) {
        return NULL;
    }

    ArrStr* link_tokens = NULL;
    char* cursor = output;
    while (cursor && *cursor != '\0') {
        char* next = strchr(cursor, '\n');
        if (next) {
            *next = '\0';
        }
        ArrStr* tokens = dal_c__parseQuotedTokens(cursor);
        if (tokens && dal_c__tokensLookLikeLinkerInvocation(tokens)) {
            ArrStr_fini(&link_tokens);
            link_tokens = tokens;
        } else {
            ArrStr_fini(&tokens);
        }
        cursor = next ? next + 1 : NULL;
    }
    free(output);
    return link_tokens ? link_tokens : ArrStr_init();

fail:
    free(sysroot_flag);
    free(target_abi_flag);
    free(target_arch_flag);
    free(target_value);
    free(target_flag);
    return NULL;
}

static bool dal_c__toolchainTokenMatchesQuery(const char* token, const char* compiler_rt_path, dal_c_ToolchainQuery query) {
    bool compiler_rt = dal_c__toolchainTokenIsCompilerRt(token, compiler_rt_path);
    bool start_file = dal_c__toolchainTokenIsStartFile(token);
    switch (query) {
    case dal_c_ToolchainQuery_start_files:
        return start_file;
    case dal_c_ToolchainQuery_compiler_rt:
        return compiler_rt;
    case dal_c_ToolchainQuery_default_libs:
        return !compiler_rt && !start_file && dal_c__toolchainTokenIsLib(token);
    case dal_c_ToolchainQuery_crt:
        return !compiler_rt && dal_c__toolchainTokenIsCrt(token);
    case dal_c_ToolchainQuery_stdlib:
        return !compiler_rt && (start_file || dal_c__toolchainTokenIsLib(token));
    case dal_c_ToolchainQuery_libc:
        return !compiler_rt && dal_c__toolchainTokenIsLibc(token);
    case dal_c_ToolchainQuery_raw_link:
        return true;
    case dal_c_ToolchainQuery_all:
    case dal_c_ToolchainQuery_invalid:
    default:
        return false;
    }
}

static bool dal_c__toolchainPrintedContains(ArrStr* printed, const char* token) {
    assert(printed != NULL);
    assert(token != NULL);
    for (int i = 0; i < ArrStr_len(printed); ++i) {
        if (str_eql(ArrStr_at(printed, i), token)) {
            return true;
        }
    }
    return false;
}

static void dal_c__printToolchainToken(ArrStr* printed, const char* token, int* count) {
    assert(printed != NULL);
    assert(token != NULL);
    assert(count != NULL);
    if (dal_c__toolchainPrintedContains(printed, token)) {
        return;
    }
    ArrStr_push(printed, token);
    printf("  %s\n", token);
    *count += 1;
}

static void dal_c__printToolchainCategory(const char* title, ArrStr* link_tokens, const char* compiler_rt_path, dal_c_ToolchainQuery query) {
    assert(title != NULL);
    assert(link_tokens != NULL);
    printf("%s:\n", title);
    int count = 0;
    ArrStr* printed = ArrStr_init();
    if (!printed) {
        printf("  (error: out of memory)\n");
        return;
    }
    if (query == dal_c_ToolchainQuery_compiler_rt && compiler_rt_path && compiler_rt_path[0] != '\0') {
        dal_c__printToolchainToken(printed, compiler_rt_path, &count);
    }
    for (int i = 0; i < ArrStr_len(link_tokens); ++i) {
        const char* token = ArrStr_at(link_tokens, i);
        if (query != dal_c_ToolchainQuery_raw_link && !dal_c__toolchainTokenIsLinkInput(token)) {
            continue;
        }
        if (!dal_c__toolchainTokenMatchesQuery(token, compiler_rt_path, query)) {
            continue;
        }
        if (query == dal_c_ToolchainQuery_compiler_rt && compiler_rt_path && str_eql(token, compiler_rt_path)) {
            continue;
        }
        dal_c__printToolchainToken(printed, token, &count);
    }
    if (count == 0) {
        printf("  (none)\n");
    }
    ArrStr_fini(&printed);
}

int dal_c__queryToolchain(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    dal_c_ToolchainQuery query = cmd->payload.toolchain.query;
    if (query == dal_c_ToolchainQuery_invalid) {
        query = dal_c_ToolchainQuery_all;
    }
    ArrStr* link_tokens = dal_c__queryToolchainLinkTokens(&cmd->opts);
    char* compiler_rt_path = dal_c__queryCompilerRtPath(&cmd->opts);
    if (!link_tokens) {
        free(compiler_rt_path);
        (void)fprintf(stderr, "Error: Failed to query compiler driver link line\n");
        return 1;
    }

    if (query == dal_c_ToolchainQuery_all) {
        dal_c__printToolchainCategory(dal_c_toolchain_query_start_files, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_start_files);
        dal_c__printToolchainCategory(dal_c_toolchain_query_compiler_rt, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_compiler_rt);
        dal_c__printToolchainCategory(dal_c_toolchain_query_default_libs, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_default_libs);
        dal_c__printToolchainCategory(dal_c_toolchain_query_crt, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_crt);
        dal_c__printToolchainCategory(dal_c_toolchain_query_stdlib, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_stdlib);
        dal_c__printToolchainCategory(dal_c_toolchain_query_libc, link_tokens, compiler_rt_path, dal_c_ToolchainQuery_libc);
    } else {
        const char* title = dal_c_ToolchainQuery_format(query);
        dal_c__printToolchainCategory(title ? title : "toolchain", link_tokens, compiler_rt_path, query);
    }

    free(compiler_rt_path);
    ArrStr_fini(&link_tokens);
    return 0;
}

static bool dal_c__validateBuildArtifacts(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, ArrStr* sources, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);

    if (cmd->action != dal_c_CmdAction_build) {
        return true;
    }

    const dal_c_BuildOpts* build = &cmd->payload.build;
    bool executable_target = target_type == dal_c_Target_executable;
    bool linked_target = target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_image;

    if ((build->emit_map || build->emit_linked_asm || build->emit_disasm || build->emit_debug_info) && !linked_target) {
        (void)fprintf(stderr, "Error: linked analysis artifacts require a linked output target\n");
        return false;
    }
    if ((build->emit_linked_asm || build->emit_disasm || build->emit_debug_info) && !executable_target) {
        (void)fprintf(stderr, "Error: linked assembly, disassembly, and debug-info artifacts currently require an executable target\n");
        return false;
    }
    if (build->emit_linked_asm && !dal_c__resolvedLtoMode(&cmd->opts, profile)) {
        (void)fprintf(stderr, "Error: `%s` requires effective LTO to be enabled\n", dal_c_opt_emit_linked_asm);
        return false;
    }
    if (build->print_link_gc && dal_c__platformIsWindows()) {
        (void)fprintf(stderr, "Error: `%s` is not supported by the COFF LLD linker used on Windows\n", dal_c_opt_print_link_gc);
        return false;
    }
    if (build->emit_ir && ArrStr_len(sources) != 1) {
        (void)fprintf(stderr, "Error: `%s` currently requires exactly one source file\n", dal_c_opt_emit_ir);
        return false;
    }
    if (build->emit_debug_info && profile->debug_level == dal_c_DebugLevel_none) {
        (void)fprintf(stderr, "Error: `%s` requires debug info to be enabled by the selected profile\n", dal_c_opt_emit_debug_info);
        return false;
    }

    if (build->emit_disasm && profile->debug_level == dal_c_DebugLevel_none) {
        (void)fprintf(stderr, "Warning: `%s` is requested without debug info; source and line attribution will be limited\n", dal_c_opt_emit_disasm);
    }
    if (build->emit_linked_asm && dal_c__resolvedIcfMode(&cmd->opts, profile) == dal_c_IcfMode_all) {
        (void)fprintf(stderr, "Warning: `%s` is requested while `--icf=all` is enabled; identical functions may be folded before analysis\n", dal_c_opt_emit_linked_asm);
    }
    if (dal_c__resolvedGcSections(&cmd->opts, profile)
        && (!dal_c__resolvedFunctionSections(&cmd->opts, profile) || !dal_c__resolvedDataSections(&cmd->opts, profile))) {
        (void)fprintf(stderr, "Warning: `--gc-sections` is enabled without both function and data sections; garbage collection granularity is limited\n");
    }
    if (dal_c__resolvedWholeArchive(&cmd->opts, profile) && dal_c__resolvedGcSections(&cmd->opts, profile)) {
        (void)fprintf(stderr, "Warning: `--whole-archive` forces archive members into the link; `--gc-sections` can only remove sections after that\n");
    }

    return true;
}

static dal_c__noinline dal_c__optnone bool dal_c__validateLinkToolchain(const dal_c_CompilerOpts* opts, dal_c_Target target_type) {
    assert(opts != NULL);

    if (target_type != dal_c_Target_executable && target_type != dal_c_Target_shared_lib && target_type != dal_c_Target_image) {
        return true;
    }

    bool requested_libc_linked = dal_c__requestedLibcLinked(opts);
    bool effective_libc_linked = dal_c__resolvedLibcLinked(opts);
    if (!requested_libc_linked && effective_libc_linked && dal_c__resolvedDefaultLibsLinked(opts)) {
        const char* target = (opts->arch_target && opts->arch_target[0] != '\0') ? opts->arch_target : "host";
        (void)fprintf(
            stderr,
            "Warning: `--%s=off` cannot be represented for `%s` while `--%s` remains enabled; libc is still treated as linked. Use `--%s=off` and provide the remaining libraries explicitly for a true no-libc link.\n",
            dal_c_opt_link_libc,
            target,
            dal_c_opt_link_default_libs,
            dal_c_opt_link_default_libs
        );
    }
    if (!dal_c__targetUsesImplicitCompilerRt(target_type) || !dal_c__linkNeedsCompilerRt(opts, target_type)) {
        return true;
    }

    char* runtime_path = dal_c__queryCompilerRtPath(opts);
    if (!runtime_path || runtime_path[0] == '\0') {
        const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
        (void)fprintf(
            stderr,
            "Error: Failed to query compiler runtime archive from `%s` using `--print-libgcc-file-name`\n",
            compiler
        );
        free(runtime_path);
        return false;
    }
    if (path_exists(runtime_path)) {
        free(runtime_path);
        return true;
    }

    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    const char* target = (opts->arch_target && opts->arch_target[0] != '\0') ? opts->arch_target : "host";
    (void)fprintf(stderr, "Error: Target runtime archive not found for `%s`\n", target);
    (void)fprintf(
        stderr,
        "Compiler `%s` reported `%s` via `--print-libgcc-file-name`, but that path does not exist.\n",
        compiler,
        runtime_path
    );
    (void)fprintf(
        stderr,
        "This build removes default libraries, so `dh-c` must restore the target compiler runtime explicitly.\n"
    );
    if (dal_c__targetLooksLikeWasi(target)) {
        (void)fprintf(
            stderr,
            "Hint: this usually means the active clang installation recognizes the WASI target triple but does not ship the WASI runtime or sysroot.\n"
        );
        (void)fprintf(
            stderr,
            "Use a WASI-capable toolchain such as `wasi-sdk`, or provide `--sysroot=<path>` together with a compiler installation that includes `libclang_rt.builtins.a` for `%s`.\n",
            target
        );
    } else {
        (void)fprintf(
            stderr,
            "Hint: use a toolchain/sysroot that actually ships the target runtime archive for `%s`.\n",
            target
        );
    }

    free(runtime_path);
    return false;
}

static void dal_c__writeLinkModelFlags(FILE* fp, bool is_windows, const dal_c_CompilerOpts* opts, dal_c_Target target_type) {
    assert(fp != NULL);
    assert(opts != NULL);

    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);

    bool restore_compiler_rt = !default_libs_linked && compiler_rt_linked;

    if (target_type == dal_c_Target_executable && opts->link_mode == dal_c_LinkMode_static) {
        (void)fprintf(fp, " -static");
    }

    if (!start_files_linked && !default_libs_linked) {
        /* -nostdlib removes startup files + all default libs (incl. compiler-rt).
         * Restore compiler-rt so arithmetic helpers and exception tables remain available.
         * $(TARGET_FLAGS) passes -target <triple> when cross-compiling, ensuring the
         * correct target compiler-rt is queried instead of the host's. */
        if (restore_compiler_rt) {
            (void)fprintf(fp, " -nostdlib $(shell $(CC) $(TARGET_FLAGS) --print-libgcc-file-name)");
        } else {
            (void)fprintf(fp, " -nostdlib");
        }
    } else {
        if (!start_files_linked) {
            (void)fprintf(fp, " -nostartfiles");
        }
        if (!default_libs_linked) {
            /* -nodefaultlibs removes compiler-rt along with libc/libm/OS libs.
             * Restore compiler-rt explicitly. $(TARGET_FLAGS) ensures the correct
             * cross-compilation target compiler-rt is queried. */
            if (restore_compiler_rt) {
                (void)fprintf(fp, " -nodefaultlibs $(shell $(CC) $(TARGET_FLAGS) --print-libgcc-file-name)");
            } else {
                (void)fprintf(fp, " -nodefaultlibs");
            }
        } else if (!libc_linked) {
            /* Remove only libc; compiler-rt and other default libs remain. */
            (void)fprintf(fp, " -nolibc");
        }
    }

    if (opts->entry_symbol && opts->entry_symbol[0] != '\0') {
        if (is_windows) {
            (void)fprintf(fp, " -Wl,/entry:%s", opts->entry_symbol);
        } else {
            (void)fprintf(fp, " -Wl,-e,%s", opts->entry_symbol);
        }
    }
}

static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile, const char* target_path) {
    assert(fp != NULL);
    assert(profile != NULL);
    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (is_windows && debug_flag && strlen(debug_flag) > 0 && target_path != NULL) {
        char* pdb_path = dal_c__makePdbPath(target_path);
        (void)fprintf(fp, " -fuse-ld=lld -Wl,--pdb=");
        dal_c__fprintMakePath(fp, pdb_path);
        free(pdb_path);
    }
}

static uint64_t dal_c__hashVersionSpec(uint64_t hash, const dal_c_VersionSpec* version) {
    assert(version != NULL);
    hash = dal_c__hashBool(hash, version->core_set);
    hash = dal_c__hashBytes(hash, &version->core_major, sizeof(version->core_major));
    hash = dal_c__hashBytes(hash, &version->core_minor, sizeof(version->core_minor));
    hash = dal_c__hashBytes(hash, &version->core_patch, sizeof(version->core_patch));
    hash = dal_c__hashBool(hash, version->label_prefix_set);
    hash = dal_c__hashBytes(hash, &version->label_prefix_num, sizeof(version->label_prefix_num));
    hash = dal_c__hashString(hash, version->label_prefix_str);
    hash = dal_c__hashBool(hash, version->label_suffix_set);
    hash = dal_c__hashBytes(hash, &version->label_suffix_num, sizeof(version->label_suffix_num));
    hash = dal_c__hashString(hash, version->label_suffix_str);
    hash = dal_c__hashBool(hash, version->build_set);
    hash = dal_c__hashString(hash, version->build_str);
    return hash;
}

static void dal_c__writeVersionDefines(FILE* fp, const dal_c_VersionSpec* version) {
    assert(fp != NULL);
    assert(version != NULL);

    if (version->core_set) {
        (void)fprintf(fp, " -Ddal_c__NUM__VER_CORE_MAJOR=%u", version->core_major);
        (void)fprintf(fp, " -Ddal_c__NUM__VER_CORE_MINOR=%u", version->core_minor);
        (void)fprintf(fp, " -Ddal_c__NUM__VER_CORE_PATCH=%u", version->core_patch);
    }
    if (version->label_prefix_set && version->label_prefix_str) {
        (void)fprintf(fp, " -Ddal_c__NUM__VER_LABEL_PREFIX=%d", version->label_prefix_num);
        (void)fprintf(fp, " -Ddal_c__STR__VER_LABEL_PREFIX=\\\"%s\\\"", version->label_prefix_str);
    }
    if (version->label_suffix_set && version->label_suffix_str) {
        (void)fprintf(fp, " -Ddal_c__NUM__VER_LABEL_SUFFIX=%u", version->label_suffix_num);
        (void)fprintf(fp, " -Ddal_c__STR__VER_LABEL_SUFFIX=\\\"%s\\\"", version->label_suffix_str);
    }
    if (version->build_set && version->build_str) {
        (void)fprintf(fp, " -Ddal_c__STR__VER_BUILD=\\\"%s\\\"", version->build_str);
    }
}

static dal_c__noinline void dal_c__writeMakefileVariables(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_ProfileSpec* profile,
    const dal_c_Project* proj,
    const char* build_dir,
    dal_c_Target target_type,
    const char* target_path,
    const char* link_contract_path
) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(build_dir != NULL);

    bool is_windows = dal_c__platformIsWindows();
    const dal_c_CompilerOpts* opts = &cmd->opts;
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    bool stdlib_linked = start_files_linked && default_libs_linked;
    bool crt_linked = start_files_linked;
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);

    if (proj && proj->root) {
        (void)fprintf(fp, "PROJECT_ROOT ?= ");
        dal_c__fprintMakePath(fp, proj->root);
        (void)fprintf(fp, "\n");
    }
    if (proj && proj->dh_path) {
        (void)fprintf(fp, "DH_PATH ?= ");
        dal_c__fprintMakePath(fp, proj->dh_path);
        (void)fprintf(fp, "\n");
    }
    (void)fprintf(fp, "\n");

    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    (void)fprintf(fp, "CC = %s\n", compiler);

    const char* c_std = opts->c_std ? opts->c_std : dal_c_default_c_std;
    (void)fprintf(fp, "STD = -std=%s\n", c_std);

    (void)fprintf(fp, "BUILD_DIR = ");
    dal_c__fprintMakePath(fp, build_dir);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "CFLAGS_BASE = $(STD)");
    (void)fprintf(fp, " -fgnu-keywords -fms-extensions -Wno-microsoft-anon-tag");
    (void)fprintf(fp, " -funsigned-char");
    (void)fprintf(fp, " -mllvm -enable-dfa-jump-thread");

    // Required macro definitions
    (void)fprintf(fp, " -DCOMP");
    if (compile_env == dal_c_CompileEnv_freestanding) {
        (void)fprintf(fp, " -DCOMP_FREESTANDING");
    } else {
        (void)fprintf(fp, " -DCOMP_HOSTED");
    }
    (void)fprintf(fp, start_files_linked ? " -DCOMP_HAS_START_FILES" : " -DCOMP_NO_START_FILES");
    (void)fprintf(fp, crt_linked ? " -DCOMP_HAS_CRT" : " -DCOMP_NO_CRT");
    (void)fprintf(fp, default_libs_linked ? " -DCOMP_HAS_DEFAULT_LIBS" : " -DCOMP_NO_DEFAULT_LIBS");
    (void)fprintf(fp, compiler_rt_linked ? " -DCOMP_HAS_COMPILER_RT" : " -DCOMP_NO_COMPILER_RT");
    (void)fprintf(fp, libc_linked ? " -DCOMP_HAS_LIBC" : " -DCOMP_NO_LIBC");
    (void)fprintf(fp, stdlib_linked ? " -DCOMP_HAS_STDLIB" : " -DCOMP_NO_STDLIB");
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

    dal_c_LtoMode lto_state = dal_c__resolvedLtoState(opts, profile);
    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (dal_c__isLtoFlag(flag)) {
            continue;
        }
        if (!str_startsWith(flag, "-Wl,") && !str_startsWith(flag, "-L") && !str_startsWith(flag, "-l")) {
            (void)fprintf(fp, " %s", flag);
        }
    }
    const char* lto_flag = dal_c_LtoMode_toFlag(lto_state);
    if (lto_flag) {
        (void)fprintf(fp, " %s", lto_flag);
    }
    dal_c_ToggleState omit_frame_pointer = dal_c__resolvedOmitFramePointerState(opts, profile);
    if (omit_frame_pointer == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -fomit-frame-pointer");
    } else if (omit_frame_pointer == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-omit-frame-pointer");
    }
    if (dal_c__resolvedFunctionSections(opts, profile)) {
        (void)fprintf(fp, " -ffunction-sections");
    }
    if (dal_c__resolvedDataSections(opts, profile)) {
        (void)fprintf(fp, " -fdata-sections");
    }
    dal_c_ToggleState unroll_loops = dal_c__resolvedUnrollLoopsState(opts, profile);
    if (unroll_loops == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -funroll-loops");
    } else if (unroll_loops == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-unroll-loops");
    }
    dal_c_ToggleState unwind_tables = dal_c__resolvedUnwindTablesState(opts, profile);
    if (unwind_tables == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -funwind-tables");
    } else if (unwind_tables == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-unwind-tables");
    }
    dal_c_ToggleState async_unwind_tables = dal_c__resolvedAsyncUnwindTablesState(opts, profile);
    if (async_unwind_tables == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -fasynchronous-unwind-tables");
    } else if (async_unwind_tables == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-asynchronous-unwind-tables");
    }
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    if (exceptions == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -fexceptions");
    } else if (exceptions == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-exceptions");
    }
    dal_c_ToggleState merge_all_constants = dal_c__resolvedMergeAllConstantsState(opts);
    if (merge_all_constants == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -fmerge-all-constants");
    } else if (merge_all_constants == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-merge-all-constants");
    }
    dal_c_ToggleState stack_protector = dal_c__resolvedStackProtectorState(opts);
    if (stack_protector == dal_c_ToggleState_enabled) {
        (void)fprintf(fp, " -fstack-protector-strong");
    } else if (stack_protector == dal_c_ToggleState_disabled) {
        (void)fprintf(fp, " -fno-stack-protector");
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.save_temps != dal_c_SaveTempsMode_off) {
        (void)fprintf(fp, " -save-temps=%s", dal_c_SaveTempsMode_format(cmd->payload.build.save_temps));
    }

    if (opts->loose_errors == dal_c_LooseErrorsMode_suppress) {
        (void)fprintf(fp, " -w");
    } else if (opts->loose_errors == dal_c_LooseErrorsMode_warn) {
        (void)fprintf(fp, " -Wall -Wextra -Wconversion");
        (void)fprintf(fp, " -Wsign-conversion -Wfloat-conversion");
        (void)fprintf(fp, " -Wformat=2");
        (void)fprintf(fp, " -Wcast-qual -Wcast-align");
        (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
        (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
        (void)fprintf(fp, " -Wuninitialized");
        (void)fprintf(fp, " -Wframe-larger-than=4096");
        (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
        (void)fprintf(fp, " -Wno-microsoft-anon-tag");
        (void)fprintf(fp, " -Wloop-analysis -Wstrict-prototypes");
        (void)fprintf(fp, " -Wmissing-prototypes");
        (void)fprintf(fp, " -Wmissing-variable-declarations");
        (void)fprintf(fp, " -Wdiv-by-zero -Wthread-safety");
    } else {
        (void)fprintf(fp, " -Werror=all -Werror=extra -Werror=conversion");
        (void)fprintf(fp, " -Werror=sign-conversion -Wfloat-conversion");
        (void)fprintf(fp, " -Wformat=2");
        (void)fprintf(fp, " -Werror=cast-qual -Werror=cast-align");
        (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
        (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
        (void)fprintf(fp, " -Werror=uninitialized");
        (void)fprintf(fp, " -Wframe-larger-than=4096");
        (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
        (void)fprintf(fp, " -Wno-microsoft-anon-tag");
        (void)fprintf(fp, " -Wloop-analysis -Werror=strict-prototypes");
        (void)fprintf(fp, " -Werror=missing-prototypes");
        (void)fprintf(fp, " -Wmissing-variable-declarations");
        (void)fprintf(fp, " -Werror=div-by-zero -Wthread-safety");
    }
    (void)fprintf(fp, "\n");

    if (opts->arch_target) {
        /* TARGET_FLAGS is also referenced in LDFLAGS to pass the correct target
         * triple to --print-libgcc-file-name when restoring compiler-rt. */
        (void)fprintf(fp, "TARGET_FLAGS = -target %s\n", opts->arch_target);
        (void)fprintf(fp, "CFLAGS_BASE += $(TARGET_FLAGS)\n");
    }
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    if (target_arch) {
        (void)fprintf(fp, "TARGET_ARCH_FLAGS = -march=%s\n", target_arch);
        (void)fprintf(fp, "CFLAGS_BASE += $(TARGET_ARCH_FLAGS)\n");
    }
    const char* target_tune = dal_c__resolvedTargetTune(opts, profile);
    if (target_tune) {
        (void)fprintf(fp, "TARGET_TUNE_FLAGS = -mtune=%s\n", target_tune);
        (void)fprintf(fp, "CFLAGS_BASE += $(TARGET_TUNE_FLAGS)\n");
    }
    if (opts->target_abi) {
        (void)fprintf(fp, "TARGET_ABI_FLAGS = -mabi=%s\n", opts->target_abi);
        (void)fprintf(fp, "CFLAGS_BASE += $(TARGET_ABI_FLAGS)\n");
    }
    if (compile_env == dal_c_CompileEnv_freestanding) {
        (void)fprintf(fp, "CFLAGS_BASE += -ffreestanding\n");
    }
    if (opts->sysroot) {
        (void)fprintf(fp, "SYSROOT_FLAGS = --sysroot=");
        dal_c__fprintMakePath(fp, opts->sysroot);
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "CFLAGS_BASE += $(SYSROOT_FLAGS)\n");
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "INCLUDES =");
    if (proj && proj->root) {
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/%s", dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
    }
    if (dal_c__usesDhLibrary(proj, opts)) {
        (void)fprintf(fp, " -I$(DH_PATH)/include");
        (void)fprintf(fp, " -I$(DH_PATH)");
    }
    if (proj && proj->root && proj->lib_count > 0) {
        // Flat deps structure: all headers in lib/deps/
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/lib/deps");
    }
    for (int i = 0; i < opts->include_count; ++i) {
        (void)fprintf(fp, " -I");
        dal_c__fprintMakePath(fp, opts->include_paths[i]);
    }
    for (int i = 0; i < opts->isystem_count; ++i) {
        (void)fprintf(fp, " -isystem ");
        dal_c__fprintMakePath(fp, opts->isystem_paths[i]);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "DEFINES =");
    for (int i = 0; i < opts->define_count; ++i) {
        (void)fprintf(fp, " -D%s", opts->define_macros[i]);
    }
    dal_c__writeVersionDefines(fp, &opts->version);
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

    if (target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_image) {
        if (link_contract_path) {
            (void)fprintf(fp, "LINK_CONTRACT = ");
            dal_c__fprintMakePath(fp, link_contract_path);
            (void)fprintf(fp, "\n");
        }
        if (target_type == dal_c_Target_image) {
            const char* objcopy = cmd->objcopy ? cmd->objcopy : dal_c_default_objcopy;
            const char* objcopy_format = cmd->objcopy_format ? cmd->objcopy_format : "binary";
            char* link_target = dal_c__makeImageLinkPath(target_path);
            (void)fprintf(fp, "OBJCOPY = %s\n", objcopy);
            (void)fprintf(fp, "OBJCOPY_FORMAT = %s\n", objcopy_format);
            (void)fprintf(fp, "LINK_TARGET = ");
            dal_c__fprintMakePath(fp, link_target);
            (void)fprintf(fp, "\n");
            free(link_target);
        }
        (void)fprintf(fp, "LDFLAGS = $(TARGET_FLAGS) $(TARGET_ARCH_FLAGS) $(TARGET_ABI_FLAGS) $(SYSROOT_FLAGS)");
        dal_c__writeLinkModelFlags(fp, is_windows, opts, target_type);
        bool whole_archive_enabled = dal_c__resolvedWholeArchive(opts, profile);
        if (whole_archive_enabled) {
            (void)fprintf(fp, " -Wl,--whole-archive");
        }
        char* project_lib_name = NULL;
        char* project_lib_path = NULL;
        dal_c_TargetRequest request = { 0 };
        bool has_request = proj && dal_c_TargetRequest_resolve(proj, &intent, &request);
        bool link_project_static_lib = (target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_image)
                                    && proj
                                    && proj->root
                                    && proj->name
                                    && ((has_request && request.root && request.link_project)
                                        || intent.action == dal_c_CmdAction_test
                                        || intent.action == dal_c_CmdAction_test_dsl);
        if (link_project_static_lib) {
            char* project_build_dir = dal_c_Project_getBuildDir(proj);
            char* project_profile_dir = path_join(project_build_dir, profile->name);
            char* project_lib_dir = path_join(project_profile_dir, "libs");
            project_lib_name = dal_c__makeTargetFileName(proj->name, dal_c_Target_static_lib, is_windows);
            project_lib_path = path_join(project_lib_dir, project_lib_name);
            link_project_static_lib = path_isFile(project_lib_path);
            free(project_lib_dir);
            free(project_profile_dir);
            free(project_build_dir);
        }
        if (link_project_static_lib) {
            (void)fprintf(fp, " $(PROJECT_ROOT)/build/%s/libs/%s", profile->name, project_lib_name);
        }
        free(project_lib_path);
        free(project_lib_name);
        dal_c_TargetRequest_cleanup(&request);
        if (proj && proj->root && proj->lib_count > 0) {
            (void)fprintf(fp, " -L$(PROJECT_ROOT)/lib/deps");
        }
        if (dal_c__usesDhLibrary(proj, opts)) {
            (void)fprintf(fp, " -L$(DH_PATH)/build/%s/libs", profile->name);
            if (is_windows) {
                (void)fprintf(fp, " $(DH_PATH)/build/%s/libs/dh.lib", profile->name);
            } else {
                (void)fprintf(fp, " $(DH_PATH)/build/%s/libs/libdh.a", profile->name);
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
                            (void)fprintf(fp, " ");
                            dal_c__fprintMakePath(fp, lib_files[i]);
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
        if (whole_archive_enabled) {
            (void)fprintf(fp, " -Wl,--no-whole-archive");
        }
        for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
            const char* flag = profile->extra_flags[i];
            if (dal_c__isLtoFlag(flag)) {
                continue;
            }
            if (str_startsWith(flag, "-Wl,") || str_startsWith(flag, "-L") || str_startsWith(flag, "-l")) {
                (void)fprintf(fp, " %s", flag);
            }
        }
        if (lto_flag) {
            (void)fprintf(fp, " %s", lto_flag);
        }
        if (dal_c__resolvedGcSections(opts, profile)) {
            (void)fprintf(fp, " -Wl,--gc-sections");
        }
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.print_link_gc) {
            (void)fprintf(fp, " -Wl,--print-gc-sections");
        }
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_map) {
            char* map_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_map_path, ".map");
            (void)fprintf(fp, " -Wl,-Map=");
            dal_c__fprintMakePath(fp, map_path);
            free(map_path);
        }
        if (dal_c__resolvedStripMode(opts, profile)) {
            (void)fprintf(fp, " -Wl,--strip-all");
        }
        dal_c_IcfMode icf_mode = dal_c__resolvedIcfMode(opts, profile);
        if (icf_mode == dal_c_IcfMode_safe || icf_mode == dal_c_IcfMode_all) {
            (void)fprintf(fp, " -Wl,--icf=%s", dal_c_IcfMode_format(icf_mode));
        }
        if (cmd->linker_script) {
            (void)fprintf(fp, " -Xlinker -T -Xlinker ");
            dal_c__fprintMakePath(fp, cmd->linker_script);
        }
        if (cmd->link_args) {
            (void)fprintf(fp, " %s", cmd->link_args);
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
    char* sanitized = dal_c__sanitizePathFragment(work);
    free(work);
    work = sanitized;
    if (!work) { return NULL; }
    char* dot = strrchr(work, '.');
    if (dot) { *dot = '\0'; }
    return work;
}

static char* dal_c__artifactPath(const char* target_path, const char* override_path, const char* ext) {
    assert(target_path != NULL);
    assert(ext != NULL);
    if (override_path && override_path[0] != '\0') {
        return strdup(override_path);
    }
    char* parent = path_parent(target_path);
    char* basename = path_basename(target_path);
    char* stem = basename ? strdup(basename) : NULL;
    if (stem) {
        char* dot = strrchr(stem, '.');
        if (dot && dot != stem) {
            *dot = '\0';
        }
    }
    char* name = str_format("%s%s", stem ? stem : "artifact", ext);
    char* out = parent ? path_join(parent, name) : strdup(name);
    free(name);
    free(stem);
    free(basename);
    free(parent);
    return out;
}

static bool dal_c__sourceIsAssembly(const char* src) {
    assert(src != NULL);
    return str_endsWith(src, ".S") || str_endsWith(src, ".s");
}

static bool dal_c__commandUsesTestMode(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    switch (cmd->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_compile_db:
        return cmd->payload.build.sample_dir == dal_c_SampleDir_tests;
    case dal_c_CmdAction_run:
        return cmd->payload.run.sample_dir == dal_c_SampleDir_tests;
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_test_dsl:
        return true;
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_clean_dsl:
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}

static bool dal_c__sourceUsesTestMode(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* src) {
    assert(cmd != NULL);
    assert(src != NULL);

    if (dal_c__commandUsesTestMode(cmd)) {
        return true;
    }

    if (proj && proj->root) {
        char* tests_dir = dal_c_Project_getTestsDir(proj);
        bool in_tests_dir = tests_dir && str_startsWith(src, tests_dir);
        free(tests_dir);
        if (in_tests_dir) {
            return true;
        }

        char* cache_dir = dal_c__cacheBaseDir(proj);
        bool is_generated_runner = cache_dir && str_startsWith(src, cache_dir) && strstr(src, ".main.c") != NULL;
        free(cache_dir);
        if (is_generated_runner) {
            return true;
        }
    }
    return false;
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

static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode) {
    assert(cmd != NULL);
    assert(profile != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    bool stdlib_linked = start_files_linked && default_libs_linked;
    bool crt_linked = start_files_linked;
    bool lto_enabled = dal_c__resolvedLtoMode(opts, profile);
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    const char* target_tune = dal_c__resolvedTargetTune(opts, profile);
    dal_c_ToggleState unwind_tables = dal_c__resolvedUnwindTablesState(opts, profile);
    dal_c_ToggleState async_unwind_tables = dal_c__resolvedAsyncUnwindTablesState(opts, profile);
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashString(hash, opts->compiler);
    hash = dal_c__hashString(hash, opts->c_std);
    hash = dal_c__hashString(hash, opts->arch_target);
    hash = dal_c__hashString(hash, target_arch);
    hash = dal_c__hashString(hash, target_tune);
    hash = dal_c__hashString(hash, opts->target_abi);
    hash = dal_c__hashString(hash, opts->sysroot);
    hash = dal_c__hashString(hash, cmd->compiler_args);
    hash = dal_c__hashBytes(hash, &compile_env, sizeof(compile_env));
    hash = dal_c__hashBytes(hash, &target_type, sizeof(target_type));
    hash = dal_c__hashBool(hash, libc_linked);
    hash = dal_c__hashBool(hash, default_libs_linked);
    hash = dal_c__hashBool(hash, start_files_linked);
    hash = dal_c__hashBool(hash, compiler_rt_linked);
    hash = dal_c__hashBool(hash, stdlib_linked);
    hash = dal_c__hashBool(hash, crt_linked);
    hash = dal_c__hashBool(hash, dal_c__resolvedDslMode(opts));
    hash = dal_c__hashBytes(hash, &opts->lto_mode, sizeof(opts->lto_mode));
    hash = dal_c__hashBool(hash, lto_enabled);
    hash = dal_c__hashBytes(hash, &opts->omit_frame_pointer, sizeof(opts->omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &profile->omit_frame_pointer, sizeof(profile->omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &opts->unroll_loops, sizeof(opts->unroll_loops));
    hash = dal_c__hashBytes(hash, &profile->unroll_loops, sizeof(profile->unroll_loops));
    hash = dal_c__hashBytes(hash, &opts->function_sections, sizeof(opts->function_sections));
    hash = dal_c__hashBool(hash, dal_c__resolvedFunctionSections(opts, profile));
    hash = dal_c__hashBytes(hash, &opts->data_sections, sizeof(opts->data_sections));
    hash = dal_c__hashBool(hash, dal_c__resolvedDataSections(opts, profile));
    hash = dal_c__hashBytes(hash, &opts->unwind_tables, sizeof(opts->unwind_tables));
    hash = dal_c__hashBytes(hash, &profile->unwind_tables, sizeof(profile->unwind_tables));
    hash = dal_c__hashBytes(hash, &unwind_tables, sizeof(unwind_tables));
    hash = dal_c__hashBytes(hash, &opts->async_unwind_tables, sizeof(opts->async_unwind_tables));
    hash = dal_c__hashBytes(hash, &profile->async_unwind_tables, sizeof(profile->async_unwind_tables));
    hash = dal_c__hashBytes(hash, &async_unwind_tables, sizeof(async_unwind_tables));
    hash = dal_c__hashBytes(hash, &opts->exceptions, sizeof(opts->exceptions));
    hash = dal_c__hashBytes(hash, &profile->exceptions, sizeof(profile->exceptions));
    hash = dal_c__hashBytes(hash, &exceptions, sizeof(exceptions));
    hash = dal_c__hashBytes(hash, &opts->merge_all_constants, sizeof(opts->merge_all_constants));
    hash = dal_c__hashBytes(hash, &opts->stack_protector, sizeof(opts->stack_protector));
    if (cmd->action == dal_c_CmdAction_build) {
        hash = dal_c__hashBytes(hash, &cmd->payload.build.save_temps, sizeof(cmd->payload.build.save_temps));
    }
    hash = dal_c__hashBytes(hash, &opts->loose_errors, sizeof(opts->loose_errors));
    hash = dal_c__hashVersionSpec(hash, &opts->version);
    hash = dal_c__hashBool(hash, use_pch);
    hash = dal_c__hashBool(hash, test_mode);

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

static char* dal_c__makeLinkContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    bool lto_enabled = dal_c__resolvedLtoMode(opts, profile);
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    dal_c_IcfMode icf_mode = dal_c__resolvedIcfMode(opts, profile);
    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashString(hash, cmd->link_args);
    hash = dal_c__hashString(hash, cmd->linker_script);
    hash = dal_c__hashString(hash, cmd->objcopy);
    hash = dal_c__hashString(hash, cmd->objcopy_format);
    hash = dal_c__hashBytes(hash, &target_type, sizeof(target_type));
    hash = dal_c__hashString(hash, opts->entry_symbol);
    hash = dal_c__hashBytes(hash, &opts->link_mode, sizeof(opts->link_mode));
    hash = dal_c__hashString(hash, opts->arch_target);
    hash = dal_c__hashString(hash, target_arch);
    hash = dal_c__hashString(hash, opts->target_abi);
    hash = dal_c__hashString(hash, opts->sysroot);
    hash = dal_c__hashVersionSpec(hash, &opts->version);
    hash = dal_c__hashBool(hash, dal_c__commandUsesTestMode(cmd));
    hash = dal_c__hashBool(hash, default_libs_linked);
    hash = dal_c__hashBool(hash, start_files_linked);
    hash = dal_c__hashBool(hash, compiler_rt_linked);
    hash = dal_c__hashBytes(hash, &opts->lto_mode, sizeof(opts->lto_mode));
    hash = dal_c__hashBool(hash, lto_enabled);
    hash = dal_c__hashBytes(hash, &opts->gc_sections, sizeof(opts->gc_sections));
    hash = dal_c__hashBool(hash, dal_c__resolvedGcSections(opts, profile));
    hash = dal_c__hashBytes(hash, &opts->whole_archive, sizeof(opts->whole_archive));
    hash = dal_c__hashBool(hash, dal_c__resolvedWholeArchive(opts, profile));
    hash = dal_c__hashBytes(hash, &opts->strip_mode, sizeof(opts->strip_mode));
    hash = dal_c__hashBytes(hash, &profile->strip_mode, sizeof(profile->strip_mode));
    hash = dal_c__hashBool(hash, dal_c__resolvedStripMode(opts, profile));
    hash = dal_c__hashBytes(hash, &opts->icf_mode, sizeof(opts->icf_mode));
    hash = dal_c__hashBytes(hash, &profile->icf_mode, sizeof(profile->icf_mode));
    hash = dal_c__hashBytes(hash, &icf_mode, sizeof(icf_mode));
    if (cmd->action == dal_c_CmdAction_build) {
        hash = dal_c__hashBool(hash, cmd->payload.build.emit_map);
        hash = dal_c__hashString(hash, cmd->payload.build.emit_map_path);
        hash = dal_c__hashBool(hash, cmd->payload.build.emit_linked_asm);
        hash = dal_c__hashString(hash, cmd->payload.build.emit_linked_asm_path);
        hash = dal_c__hashBool(hash, cmd->payload.build.emit_disasm);
        hash = dal_c__hashString(hash, cmd->payload.build.emit_disasm_path);
        hash = dal_c__hashBool(hash, cmd->payload.build.emit_ir);
        hash = dal_c__hashString(hash, cmd->payload.build.emit_ir_path);
        hash = dal_c__hashBool(hash, cmd->payload.build.emit_debug_info);
        hash = dal_c__hashString(hash, cmd->payload.build.emit_debug_info_path);
        hash = dal_c__hashBytes(hash, &cmd->payload.build.disasm_demangle, sizeof(cmd->payload.build.disasm_demangle));
        hash = dal_c__hashBytes(hash, &cmd->payload.build.disasm_source, sizeof(cmd->payload.build.disasm_source));
        hash = dal_c__hashBytes(hash, &cmd->payload.build.disasm_line_numbers, sizeof(cmd->payload.build.disasm_line_numbers));
        hash = dal_c__hashBytes(
            hash,
            &cmd->payload.build.disasm_symbolize_operands,
            sizeof(cmd->payload.build.disasm_symbolize_operands)
        );
        hash = dal_c__hashBytes(hash, &cmd->payload.build.disasm_raw_insn, sizeof(cmd->payload.build.disasm_raw_insn));
        hash = dal_c__hashBytes(hash, &cmd->payload.build.disasm_section_contents, sizeof(cmd->payload.build.disasm_section_contents));
        hash = dal_c__hashBool(hash, cmd->payload.build.print_link_gc);
        hash = dal_c__hashBool(hash, cmd->payload.build.analysis_artifacts);
    }

    for (int i = 0; i < opts->link_count; ++i) {
        hash = dal_c__hashString(hash, opts->link_libs[i]);
    }
    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (dal_c__isLtoFlag(flag)) {
            continue;
        }
        if (str_startsWith(flag, "-Wl,") || str_startsWith(flag, "-L") || str_startsWith(flag, "-l")) {
            hash = dal_c__hashString(hash, flag);
        }
    }

    return str_format("%016llx", (unsigned long long)hash);
}

static char* dal_c__makeLinkContractPath(const char* build_dir, const char* target_path) {
    assert(build_dir != NULL);
    assert(target_path != NULL);

    char* contracts_dir = path_join(build_dir, ".link");
    char* target_key = dal_c__sanitizePathFragment(target_path);
    char* contract_name = str_format("%s.contract", target_key);
    char* contract_path = path_join(contracts_dir, contract_name);
    free(contract_name);
    free(target_key);
    free(contracts_dir);
    return contract_path;
}

static bool dal_c__writeLinkContractFile(const char* path, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(path != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);

    char* parent = path_parent(path);
    dir_createRecur(parent);
    free(parent);

    char* contract_key = dal_c__makeLinkContractKey(cmd, profile, target_type);
    bool success = dal_c__writeFileIfChanged(path, contract_key);
    free(contract_key);
    return success;
}

static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(object_dir != NULL);
    assert(src != NULL);

    char* contract_key = dal_c__makeCompileContractKey(cmd, profile, target_type, use_pch, test_mode);
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

static dal_c__noinline int dal_c__writeEmitOnlyMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type,
    char* makefile_tmp,
    char* makefile_path,
    char* makefile_dir,
    char* link_contract_path
) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(target_path != NULL);
    assert(build_dir != NULL);
    (void)cmd;
    (void)profile;
    (void)build_dir;

    int src_count = ArrStr_len(sources);
    bool has_pch = (proj && proj->pch_header != NULL);
    if (src_count != 1) {
        (void)fprintf(stderr, "Error: `%s` export requires exactly one source file\n", dal_c_Target_format(target_type));
        (void)fclose(fp);
        (void)remove(makefile_tmp);
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }

    const char* src = ArrStr_at(sources, 0);
    bool use_pch = has_pch && !dal_c__sourceIsAssembly(src) && !dal_c__sourceUsesPchExcludedHeader(proj, src);
    const char* cflags_base = use_pch ? "$(CFLAGS)" : "$(CFLAGS_NO_PCH)";
    char* emit_flags = strdup(cflags_base);
    if (!emit_flags) {
        (void)fclose(fp);
        (void)remove(makefile_tmp);
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }

    dal_c__writeMakefileTargetVar(fp, target_path);
    (void)fprintf(fp, "SRC = ");
    dal_c__fprintMakePath(fp, src);
    (void)fprintf(fp, "\n\n");
    (void)fprintf(fp, "all: $(TARGET)\n\n");
    (void)fprintf(fp, "$(TARGET): $(SRC)");
    if (use_pch) {
        (void)fprintf(fp, " $(PCH_OUT)");
    }
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
    (void)fprintf(fp, "\t@echo \"[%s] %s\"\n", target_type == dal_c_Target_preprocessed ? "CPP" : "ASM", src);
    (void)fprintf(
        fp,
        "\t$(CC) %s %s $(SRC) -o $@\n\n",
        emit_flags,
        target_type == dal_c_Target_preprocessed ? "-E" : "-S"
    );
    (void)fprintf(fp, "clean:\n\trm -f $(TARGET)\n\n");

    free(emit_flags);
    (void)fclose(fp);
    bool ok = file_copy(makefile_tmp, makefile_path);
    (void)remove(makefile_tmp);
    free(link_contract_path);
    free(makefile_tmp);
    free(makefile_dir);
    free(makefile_path);
    return ok ? 0 : 1;
}

static dal_c__noinline int dal_c__writeLinkedMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type,
    char* makefile_tmp,
    char* makefile_path,
    char* makefile_dir,
    char* link_contract_path
) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(target_path != NULL);
    assert(build_dir != NULL);

    int src_count = ArrStr_len(sources);
    bool has_pch = (proj && proj->pch_header != NULL);
    bool is_windows = dal_c__platformIsWindows();
    const char* obj_base = (proj && proj->root) ? proj->root : NULL;

    (void)fprintf(fp, "SRCS =");
    for (int i = 0; i < src_count; ++i) {
        (void)fprintf(fp, " ");
        dal_c__fprintMakePath(fp, ArrStr_at(sources, i));
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "OBJS =");
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        bool use_pch = has_pch && !dal_c__sourceIsAssembly(src) && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        bool test_mode = dal_c__sourceUsesTestMode(cmd, proj, src);
        char* obj_path = dal_c__makeObjectPath(cmd, profile, target_type, build_dir, obj_base, src, use_pch, test_mode);
        (void)fprintf(fp, " ");
        dal_c__fprintMakePath(fp, obj_path);
        free(obj_path);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "DEPS = $(OBJS:.o=.d)\n\n");
    dal_c__writeMakefileTargetVar(fp, target_path);
    (void)fprintf(fp, "EXTRA_TARGETS =\n");
    bool emits_disasm = cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_disasm;
    bool disasm_needs_unstripped_target = emits_disasm && dal_c__resolvedStripMode(&cmd->opts, profile);
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_linked_asm) {
        char* linked_asm_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_linked_asm_path, ".linked.s");
        (void)fprintf(fp, "LINKED_ASM = ");
        dal_c__fprintMakePath(fp, linked_asm_path);
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "EXTRA_TARGETS += $(LINKED_ASM)\n");
        free(linked_asm_path);
    }
    if (emits_disasm) {
        char* disasm_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_disasm_path, ".disasm.s");
        (void)fprintf(fp, "DISASM = ");
        dal_c__fprintMakePath(fp, disasm_path);
        (void)fprintf(fp, "\n");
        if (disasm_needs_unstripped_target) {
            char* disasm_target_path = dal_c__artifactPath(target_path, NULL, is_windows ? ".disasm.exe" : ".disasm.out");
            (void)fprintf(fp, "DISASM_TARGET = ");
            dal_c__fprintMakePath(fp, disasm_target_path);
            (void)fprintf(fp, "\n");
            if (is_windows && profile->debug_level != dal_c_DebugLevel_none) {
                char* disasm_pdb_path = dal_c__makePdbPath(disasm_target_path);
                (void)fprintf(fp, "DISASM_PDB = ");
                dal_c__fprintMakePath(fp, disasm_pdb_path);
                (void)fprintf(fp, "\n");
                free(disasm_pdb_path);
            }
            (void)fprintf(fp, "DISASM_INPUT = $(DISASM_TARGET)\n");
            (void)fprintf(fp, "COMMA = ,\n");
            (void)fprintf(fp, "LDFLAGS_DISASM = $(filter-out -Wl$(COMMA)--strip-all,$(LDFLAGS))\n");
            (void)fprintf(fp, "EXTRA_TARGETS += $(DISASM_TARGET)\n");
            free(disasm_target_path);
        } else {
            (void)fprintf(fp, "DISASM_INPUT = $(TARGET)\n");
        }
        (void)fprintf(fp, "EXTRA_TARGETS += $(DISASM)\n");
        free(disasm_path);
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_ir) {
        char* ir_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_ir_path, ".ll");
        (void)fprintf(fp, "IR = ");
        dal_c__fprintMakePath(fp, ir_path);
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "EXTRA_TARGETS += $(IR)\n");
        free(ir_path);
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_debug_info) {
        char* debug_info_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_debug_info_path, ".debug.txt");
        (void)fprintf(fp, "DEBUG_INFO = ");
        dal_c__fprintMakePath(fp, debug_info_path);
        (void)fprintf(fp, "\n");
        if (is_windows) {
            char* pdb_path = dal_c__makePdbPath(target_path);
            (void)fprintf(fp, "PDB = ");
            dal_c__fprintMakePath(fp, pdb_path);
            (void)fprintf(fp, "\n");
            free(pdb_path);
        }
        (void)fprintf(fp, "EXTRA_TARGETS += $(DEBUG_INFO)\n");
        free(debug_info_path);
    }
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "all: $(TARGET) $(EXTRA_TARGETS)\n\n");

    dal_c__writeMakefileCompilationRules(fp, cmd, proj, profile, sources, has_pch, build_dir, obj_base, target_type);
    dal_c__writeMakefileTargetRule(fp, cmd, profile, target_type, is_windows, target_path, link_contract_path);

    (void)fprintf(fp, "clean:\n\trm -f $(TARGET) $(EXTRA_TARGETS)\n\n");
    (void)fprintf(fp, "-include $(DEPS)\n");

    (void)fclose(fp);
    free(link_contract_path);

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

static dal_c__noinline void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* build_dir, dal_c_Target target_type) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(build_dir != NULL);
    if (!proj || !proj->pch_header) {
        (void)fprintf(fp, "PCH_OUT =\n\n");
        (void)fprintf(fp, "CFLAGS = $(CFLAGS_NO_PCH)\n\n");
        return;
    }

    const char* pch_basename = path_basename(proj->pch_header);
    char* pch_contract_key = dal_c__makeCompileContractKey(cmd, profile, target_type, true, false);
    char* pch_out = str_format("%s/%s_%s.pch", build_dir, pch_contract_key, pch_basename);

    (void)fprintf(fp, "PCH_SRC = ");
    dal_c__fprintMakePath(fp, proj->pch_header);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "PCH_OUT = ");
    dal_c__fprintMakePath(fp, pch_out);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "CFLAGS = $(CFLAGS_PCH) -include-pch $(PCH_OUT)\n\n");

    // PCH dependency file tracks all headers included by PCH_SRC
    char* pch_dep = str_format("%s/%s_%s.d", build_dir, pch_contract_key, pch_basename);
    (void)fprintf(fp, "PCH_DEP = ");
    dal_c__fprintMakePath(fp, pch_dep);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "-include $(PCH_DEP)\n\n");
    free(pch_dep);

    (void)fprintf(fp, "$(PCH_OUT): $(PCH_SRC)\n");
    (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
    (void)fprintf(fp, "\t@echo \"[PCH] Compiling $(PCH_SRC)...\"\n");
    (void)fprintf(fp, "\t$(CC) $(CFLAGS_PCH) -MMD -MP -MF $(PCH_DEP) -x c-header $< -o $@\n\n");

    free(pch_contract_key);
    free(pch_out);
}

static dal_c__noinline void dal_c__writeMakefileCompilationRules(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base, dal_c_Target target_type) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(object_dir != NULL);

    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);

        bool is_assembly = dal_c__sourceIsAssembly(src);
        bool use_pch = !is_assembly && has_pch && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        bool test_mode = dal_c__sourceUsesTestMode(cmd, proj, src);
        char* obj_path = dal_c__makeObjectPath(cmd, profile, target_type, object_dir, base, src, use_pch, test_mode);

        const char* cflags_base = use_pch ? "$(CFLAGS)" : "$(CFLAGS_NO_PCH)";
        char* cflags = test_mode ? str_format("%s -DCOMP_TEST", cflags_base) : strdup(cflags_base);

        dal_c__fprintMakePath(fp, obj_path);
        (void)fprintf(fp, ": ");
        dal_c__fprintMakePath(fp, src);
        if (use_pch) {
            (void)fprintf(fp, " $(PCH_OUT)");
        }
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[%s] %s\"\n", is_assembly ? "AS" : "CC", src);
        (void)fprintf(fp, "\t$(CC) %s -MMD -MP -c ", cflags);
        dal_c__fprintMakePath(fp, src);
        (void)fprintf(fp, " -o $@\n\n");

        free(cflags);
        free(obj_path);
    }
}

static void dal_c__writeMakefileTargetVar(FILE* fp, const char* target_path) {
    assert(fp != NULL);
    assert(target_path != NULL);
    (void)fprintf(fp, "TARGET = ");
    dal_c__fprintMakePath(fp, target_path);
    (void)fprintf(fp, "\n\n");
}

static dal_c__noinline void dal_c__writeMakefileTargetRule(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target type, bool is_windows, const char* target_path, const char* link_contract_path) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(target_path != NULL);
    (void)is_windows;
    (void)profile;

    if (type == dal_c_Target_executable) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(OBJS)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        }
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[LD] $@\"\n");
        (void)fprintf(fp, "\t$(CC) $(OBJS) -o $@ $(LDFLAGS)\n");
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_linked_asm) {
            if (link_contract_path) {
                (void)fprintf(fp, "\n$(LINKED_ASM): $(LINK_CONTRACT) $(OBJS)\n");
            } else {
                (void)fprintf(fp, "\n$(LINKED_ASM): $(OBJS)\n");
            }
            (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
            (void)fprintf(fp, "\t@echo \"[LTO-ASM] $@\"\n");
            (void)fprintf(fp, "\t$(CC) $(OBJS) -o \"$@\" $(LDFLAGS) -Wl,--lto-emit-asm\n");
        }
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_disasm) {
            if (dal_c__resolvedStripMode(&cmd->opts, profile)) {
                if (link_contract_path) {
                    (void)fprintf(fp, "\n$(DISASM_TARGET): $(LINK_CONTRACT) $(OBJS)\n");
                } else {
                    (void)fprintf(fp, "\n$(DISASM_TARGET): $(OBJS)\n");
                }
                (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
                (void)fprintf(fp, "\t@echo \"[LD-DISASM] $@\"\n");
                (void)fprintf(fp, "\t$(CC) $(OBJS) -o \"$@\" $(LDFLAGS_DISASM)");
                if (is_windows && profile->debug_level != dal_c_DebugLevel_none) {
                    (void)fprintf(fp, " -Wl,--pdb=$(DISASM_PDB)");
                }
                (void)fprintf(fp, "\n");
            }
            (void)fprintf(fp, "\n$(DISASM): $(DISASM_INPUT)\n");
            (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
            (void)fprintf(fp, "\t@echo \"[DISASM] $@\"\n");
            (void)fprintf(fp, "\tllvm-objdump -d");
            if (cmd->payload.build.disasm_demangle == dal_c_ToggleState_enabled) {
                (void)fprintf(fp, " --demangle");
            }
            if (cmd->payload.build.disasm_source != dal_c_ToggleState_disabled) {
                (void)fprintf(fp, " --source");
            }
            if (cmd->payload.build.disasm_line_numbers != dal_c_ToggleState_disabled) {
                (void)fprintf(fp, " --line-numbers");
            }
            if (cmd->payload.build.disasm_symbolize_operands == dal_c_ToggleState_enabled) {
                (void)fprintf(fp, " --symbolize-operands");
            }
            if (cmd->payload.build.disasm_raw_insn == dal_c_ToggleState_disabled) {
                (void)fprintf(fp, " --no-show-raw-insn");
            }
            if (cmd->payload.build.disasm_section_contents == dal_c_ToggleState_enabled) {
                (void)fprintf(fp, " -s");
            }
            (void)fprintf(fp, " \"$(DISASM_INPUT)\" > \"$@\"\n");
        }
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_debug_info) {
            (void)fprintf(fp, "\n$(DEBUG_INFO): $(TARGET)\n");
            (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
            if (is_windows) {
                (void)fprintf(fp, "\t@echo \"[PDB] $@\"\n");
                (void)fprintf(fp, "\tllvm-pdbutil dump -symbols -globals -publics \"$(PDB)\" > \"$@\"\n");
            } else {
                (void)fprintf(fp, "\t@echo \"[DWARF] $@\"\n");
                (void)fprintf(fp, "\tllvm-dwarfdump --debug-info --debug-line \"$(TARGET)\" > \"$@\"\n");
            }
        }
    } else if (type == dal_c_Target_static_lib) {
        (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[AR] $@\"\n");
        (void)fprintf(fp, "\t$(AR) rcs $@ $(OBJS)\n");
    } else if (type == dal_c_Target_shared_lib) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(OBJS)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(OBJS)\n");
        }
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[LD] $@\"\n");
        (void)fprintf(fp, "\t$(CC) -shared -fPIC $(OBJS) -o $@ $(LDFLAGS)\n");
    } else if (type == dal_c_Target_image) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(LINK_TARGET): $(LINK_CONTRACT) $(OBJS)\n");
        } else {
            (void)fprintf(fp, "$(LINK_TARGET): $(OBJS)\n");
        }
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[LD] $@\"\n");
        (void)fprintf(fp, "\t$(CC) $(OBJS) -o $@ $(LDFLAGS)\n\n");
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(LINK_TARGET)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(LINK_TARGET)\n");
        }
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[OBJCOPY] $@\"\n");
        (void)fprintf(fp, "\t$(OBJCOPY) -O $(OBJCOPY_FORMAT) $(LINK_TARGET) $@\n");
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_ir) {
        (void)fprintf(fp, "\n$(IR): $(SRCS)\n");
        (void)fprintf(fp, "\t@mkdir -p $(dir $@)\n");
        (void)fprintf(fp, "\t@echo \"[IR] $@\"\n");
        (void)fprintf(fp, "\t$(CC) $(CFLAGS_NO_PCH) -S -emit-llvm \"$(firstword $(SRCS))\" -o \"$@\"\n");
    }
    (void)fprintf(fp, "\n");
}
