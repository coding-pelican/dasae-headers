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
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

// === PRIVATE HELPERS (Core Layer - use asserts) ===

static char* dal_c__last_contract_diff = NULL;
static bool dal_c__read_only_planning = false;

typedef struct dal_c__ExternalToolSpec {
    const char* override_env;
    const char* conventional_env;
    const char* fallback;
} dal_c__ExternalToolSpec;

static const dal_c__ExternalToolSpec dal_c__external_tool_specs[] = {
    [dal_c_ExternalTool_make] = { "DH_C_MAKE", "MAKE", "make" },
    [dal_c_ExternalTool_archiver] = { "DH_C_AR", "AR", "llvm-ar" },
    [dal_c_ExternalTool_debugger] = { "DH_C_DEBUGGER", "DEBUGGER", "lldb" },
    [dal_c_ExternalTool_cmake] = { "DH_C_CMAKE", "CMAKE", "cmake" },
    [dal_c_ExternalTool_git] = { "DH_C_GIT", "GIT", "git" },
    [dal_c_ExternalTool_curl] = { "DH_C_CURL", "CURL", "curl" },
    [dal_c_ExternalTool_wget] = { "DH_C_WGET", "WGET", "wget" },
    [dal_c_ExternalTool_tar] = { "DH_C_TAR", "TAR", "tar" },
    [dal_c_ExternalTool_unzip] = { "DH_C_UNZIP", "UNZIP", "unzip" },
    [dal_c_ExternalTool_clang_tidy] = { "DH_C_CLANG_TIDY", "CLANG_TIDY", "clang-tidy" },
    [dal_c_ExternalTool_clang_format] = { "DH_C_CLANG_FORMAT", "CLANG_FORMAT", "clang-format" },
    [dal_c_ExternalTool_llvm_objdump] = { "DH_C_LLVM_OBJDUMP", "LLVM_OBJDUMP", "llvm-objdump" },
    [dal_c_ExternalTool_llvm_dwarfdump] = { "DH_C_LLVM_DWARFDUMP", "LLVM_DWARFDUMP", "llvm-dwarfdump" },
    [dal_c_ExternalTool_llvm_pdbutil] = { "DH_C_LLVM_PDBUTIL", "LLVM_PDBUTIL", "llvm-pdbutil" },
    [dal_c_ExternalTool_posix_shell] = { "DH_C_SHELL", NULL, "/bin/sh" },
    [dal_c_ExternalTool_windows_command] = { "DH_C_CMD", "COMSPEC", "cmd.exe" },
};

const char* dal_c__externalToolPath(dal_c_ExternalTool tool) {
    if ((size_t)tool >= sizeof(dal_c__external_tool_specs) / sizeof(dal_c__external_tool_specs[0])) {
        return NULL;
    }
    const dal_c__ExternalToolSpec* spec = &dal_c__external_tool_specs[tool];
    const char* value = spec->override_env ? getenv(spec->override_env) : NULL;
    if (value && value[0]) { return value; }
    value = spec->conventional_env ? getenv(spec->conventional_env) : NULL;
    if (value && value[0]) { return value; }
    return spec->fallback;
}

const char* dal_c__externalToolOverrideEnv(dal_c_ExternalTool tool) {
    if ((size_t)tool >= sizeof(dal_c__external_tool_specs) / sizeof(dal_c__external_tool_specs[0])) {
        return NULL;
    }
    return dal_c__external_tool_specs[tool].override_env;
}


bool dal_c__setReadOnlyPlanning(bool enabled) {
    bool previous = dal_c__read_only_planning;
    dal_c__read_only_planning = enabled;
    return previous;
}

char* dal_c__takeLastContractDiff(void) {
    char* result = dal_c__last_contract_diff;
    dal_c__last_contract_diff = NULL;
    return result;
}

static void dal_c__recordContractDiff(const char* label, const char* previous, const char* current);

static void dal_c__contractAppend(char** dst, const char* key, const char* value) {
    const char* safe_value = value ? value : "";
    char* next = *dst ? str_format("%s%s=%s\n", *dst, key, safe_value) : str_format("%s=%s\n", key, safe_value);
    free(*dst);
    *dst = next;
}

static void dal_c__contractAppendBool(char** dst, const char* key, bool value) {
    dal_c__contractAppend(dst, key, value ? "on" : "off");
}

static char* dal_c__contractLookup(const char* content, const char* key) {
    if (!content || !key) { return NULL; }
    size_t key_len = strlen(key);
    const char* line = content;
    while (*line) {
        const char* end = strchr(line, '\n');
        size_t len = end ? (size_t)(end - line) : strlen(line);
        if (len > key_len && strncmp(line, key, key_len) == 0 && line[key_len] == '=') {
            return str_format("%.*s", (int)(len - key_len - 1), line + key_len + 1);
        }
        if (!end) { break; }
        line = end + 1;
    }
    return NULL;
}

static char* dal_c__contractDiff(const char* previous, const char* current) {
    if (!previous || !previous[0]) { return strdup("  - no previous structured contract was recorded\n"); }
    if (!strchr(previous, '=')) { return strdup("  - previous contract used the legacy hash-only format\n"); }
    char* diff = NULL;
    const char* line = current;
    while (line && *line) {
        const char* end = strchr(line, '\n');
        size_t len = end ? (size_t)(end - line) : strlen(line);
        const char* eq = memchr(line, '=', len);
        if (eq) {
            char* key = str_format("%.*s", (int)(eq - line), line);
            char* now = str_format("%.*s", (int)(len - (size_t)(eq - line) - 1), eq + 1);
            char* old = dal_c__contractLookup(previous, key);
            if (!str_eql(key, "hash") && (!old || !str_eql(old, now))) {
                char* next = diff ? str_format("%s  - %s: %s -> %s\n", diff, key, old ? old : "(unset)", now)
                                  : str_format("  - %s: %s -> %s\n", key, old ? old : "(unset)", now);
                free(diff);
                diff = next;
            }
            free(old);
            free(now);
            free(key);
        }
        if (!end) { break; }
        line = end + 1;
    }
    return diff ? diff : strdup("  - contract content changed, but no key-level difference was found\n");
}

static void dal_c__recordContractDiff(const char* label, const char* previous, const char* current) {
    if (previous && current && str_eql(previous, current)) { return; }
    char* details = dal_c__contractDiff(previous, current);
    char* section = str_format("  %s changes:\n%s", label, details);
    if (section && dal_c__last_contract_diff && strstr(dal_c__last_contract_diff, section)) {
        free(section);
        free(details);
        return;
    }
    char* next = dal_c__last_contract_diff
                   ? str_format("%s%s", dal_c__last_contract_diff, section ? section : "")
                   : strdup(section ? section : "");
    free(section);
    free(dal_c__last_contract_diff);
    dal_c__last_contract_diff = next;
    free(details);
}

static char* dal_c__trimTargetOutput(char* text) {
    if (!text) { return NULL; }
    char* begin = text;
    while (*begin && (*begin == ' ' || *begin == '\t' || *begin == '\r' || *begin == '\n')) { ++begin; }
    char* end = begin + strlen(begin);
    while (end > begin && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n')) { --end; }
    *end = '\0';
    char* result = strdup(begin);
    free(text);
    return result;
}

static char* dal_c__sanitizeTargetDirName(const char* target) {
    if (!target || !target[0]) { return strdup("unknown-host"); }
    char* result = strdup(target);
    if (!result) { return NULL; }
    for (char* it = result; *it; ++it) {
        unsigned char c = (unsigned char)*it;
        if (!(isalnum(c) || c == '-' || c == '_' || c == '.')) { *it = '_'; }
    }
    return result;
}

static char* dal_c__canonicalTargetDirName(const char* target) {
    char* result = dal_c__sanitizeTargetDirName(target);
    if (!result) { return NULL; }

    static const char unknown_vendor[] = "-unknown-linux-gnu";
    static const char canonical_vendor[] = "-pc-linux-gnu";
    char* vendor = strstr(result, unknown_vendor);
    if (!vendor) { return result; }

    size_t prefix_len = (size_t)(vendor - result);
    char* canonical = str_format(
        "%.*s%s%s",
        (int)prefix_len,
        result,
        canonical_vendor,
        vendor + sizeof(unknown_vendor) - 1
    );
    free(result);
    return canonical;
}

typedef struct dal_c__TargetResolnCacheEntry {
    char* compiler;
    char* cwd;
    char* path_env;
    char* target;
} dal_c__TargetResolnCacheEntry;

#define dal_c__target_resoln_cache_capacity 16
static dal_c__TargetResolnCacheEntry dal_c__target_resoln_cache[dal_c__target_resoln_cache_capacity];
static int dal_c__target_resoln_cache_count = 0;

char* dal_c__resolveTargetDirName(const dal_c_CompilerOpts* opts) {
    if (opts && opts->arch_target && opts->arch_target[0]) {
        return dal_c__canonicalTargetDirName(opts->arch_target);
    }
    const char* compiler = (opts && opts->compiler && opts->compiler[0]) ? opts->compiler : dal_c_default_compiler;
    char* cwd = env_getCWD();
    const char* path_env = getenv("PATH");
    for (int i = 0; i < dal_c__target_resoln_cache_count; ++i) {
        if (str_eql(dal_c__target_resoln_cache[i].compiler, compiler)
            && str_eql(dal_c__target_resoln_cache[i].cwd, cwd)
            && str_eql(dal_c__target_resoln_cache[i].path_env, path_env)) {
            char* cached = strdup(dal_c__target_resoln_cache[i].target);
            free(cwd);
            return cached;
        }
    }

    const char* clang_argv[] = { compiler, "--print-target-triple", NULL };
    char* output = dal_c__trimTargetOutput(proc_output(clang_argv));
    if (!output || !output[0]) {
        free(output);
        const char* generic_argv[] = { compiler, "-dumpmachine", NULL };
        output = dal_c__trimTargetOutput(proc_output(generic_argv));
    }
    if (!output || !output[0]) {
        free(output);
#ifdef _WIN32
#if defined(_M_ARM64) || defined(__aarch64__)
        output = strdup("aarch64-w64-windows-gnu");
#else
        output = strdup("x86_64-w64-windows-gnu");
#endif
#elif defined(__APPLE__)
#if defined(__aarch64__)
        output = strdup("aarch64-apple-darwin");
#else
        output = strdup("x86_64-apple-darwin");
#endif
#else
#if defined(__aarch64__)
        output = strdup("aarch64-unknown-linux-gnu");
#else
        output = strdup("x86_64-unknown-linux-gnu");
#endif
#endif
    }
    char* result = dal_c__canonicalTargetDirName(output);
    free(output);
    if (result && dal_c__target_resoln_cache_count < dal_c__target_resoln_cache_capacity) {
        dal_c__TargetResolnCacheEntry* entry = &dal_c__target_resoln_cache[dal_c__target_resoln_cache_count++];
        entry->compiler = strdup(compiler);
        entry->cwd = cwd;
        cwd = NULL;
        entry->path_env = path_env ? strdup(path_env) : strdup("");
        entry->target = strdup(result);
        if (!entry->compiler || !entry->cwd || !entry->path_env || !entry->target) {
            free(entry->target);
            free(entry->path_env);
            free(entry->cwd);
            free(entry->compiler);
            entry->target = NULL;
            entry->path_env = NULL;
            entry->cwd = NULL;
            entry->compiler = NULL;
            dal_c__target_resoln_cache_count--;
        }
    }
    free(cwd);
    return result;
}

static char* dal_c__makeBuildProfileDirAtMode(
    const char* root,
    const dal_c_CompilerOpts* opts,
    const dal_c_ProfileSpec* profile,
    bool materialize
) {
    assert(root != NULL && profile != NULL);
    char* build_root = path_join(root, dal_c_dir_build);
    char* target_name = dal_c__resolveTargetDirName(opts);
    char* target_root = (build_root && target_name) ? path_join(build_root, target_name) : NULL;
    if (materialize && target_root) { dir_createRecur(target_root); }
    if (materialize && build_root && target_name && (!opts || !opts->arch_target || !opts->arch_target[0])) {
        char* native_link = path_join(build_root, "native");
        if (native_link && !dir_linkDir(native_link, target_name)) {
            (void)fprintf(stderr, "Warning: Failed to create native build alias: %s -> %s\n", native_link, target_name);
        }
        free(native_link);
    }
    char* result = target_root ? path_join(target_root, profile->name) : NULL;
    free(target_root);
    free(target_name);
    free(build_root);
    return result;
}

static char* dal_c__makeBuildProfileDirAt(const char* root, const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    return dal_c__makeBuildProfileDirAtMode(root, opts, profile, true);
}

char* dal_c__makeBuildProfileDir(const dal_c_Project* proj, const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(proj != NULL && profile != NULL);
    const char* root = proj->root ? proj->root : proj->unit_root;
    assert(root != NULL);
    return dal_c__makeBuildProfileDirAtMode(root, opts, profile, true);
}

char* dal_c__makeBuildProfileDirReadOnly(const dal_c_Project* proj, const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile) {
    assert(proj != NULL && profile != NULL);
    const char* root = proj->root ? proj->root : proj->unit_root;
    assert(root != NULL);
    return dal_c__makeBuildProfileDirAtMode(root, opts, profile, false);
}


static void dal_c__freeFileList(char** files, int file_count);
static const char** dal_c__makeConstArgvView(ArrStr* argv);
static ArrStr* dal_c__collectFilesWithExt(const char* dir, const char* ext, bool skip_source_paths);
static ArrStr* dal_c__collectBuildFiles(const char* dir, bool skip_source_paths);
static ArrStr* dal_c__collectBuildFilesCached(const dal_c_Project* proj, const char* dir, bool skip_source_paths);
static bool dal_c__copyHeaderToPathIfChanged(const char* src, const char* dst);
static bool dal_c__copyHeaderFile(const char* src, const char* out_dir);
static bool dal_c__copyHeaderRelativeTo(const char* src, const char* src_root, const char* dst_dir);
static bool dal_c__copyHeadersRecursive(const char* src_dir, const char* dst_dir);
static char* dal_c__resolveDepsTargetDir(const char* deps_dir, const char* lib_name);

static bool dal_c__writeFileIfChanged(const char* path, const char* content);
static bool dal_c__effectiveLtoEnabled(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeLinkContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
char* dal_c__makePrebuiltAbiContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeLtoToolchainContractKey(const dal_c_CompilerOpts* opts);
static char* dal_c__resolvePrebuiltArtifactPath(
    const char* prebuilt_profile_dir,
    const char* name,
    dal_c_Target target_type,
    bool is_windows,
    bool lto_enabled
);

static char* dal_c__manifestValue(const char* manifest_path, const char* key) {
    int line_count = 0;
    char** lines = file_readLines(manifest_path, &line_count);
    if (!lines) { return NULL; }
    char* result = NULL;
    size_t key_len = strlen(key);
    for (int i = 0; i < line_count; ++i) {
        char* line = lines[i];
        while (*line == ' ' || *line == '\t') { ++line; }
        if (strncmp(line, key, key_len) != 0 || line[key_len] != '=') { continue; }
        const char* value = line + key_len + 1;
        while (*value == ' ' || *value == '\t') { ++value; }
        result = strdup(value);
        if (result) {
            size_t n = strlen(result);
            while (n > 0 && (result[n - 1] == '\r' || result[n - 1] == '\n' || result[n - 1] == ' ' || result[n - 1] == '\t')) {
                result[--n] = '\0';
            }
        }
        break;
    }
    for (int i = 0; i < line_count; ++i) { free(lines[i]); }
    free(lines);
    return result;
}

static bool dal_c__manifestArtifactRoleValid(const char* role) {
    return str_eql(role, "static")
        || str_eql(role, "static-lto")
        || str_eql(role, "shared")
        || str_eql(role, "import");
}

static char* dal_c__manifestNormalizeRelative(const char* path) {
    if (!path) { return NULL; }
    char* normalized = strdup(path);
    if (!normalized) { return NULL; }
    for (char* p = normalized; *p; ++p) {
        if (*p == '\\') { *p = '/'; }
    }
    return normalized;
}

static bool dal_c__manifestHashValueValid(const char* value) {
    if (!value || strlen(value) != 16) { return false; }
    for (const char* p = value; *p; ++p) {
        if (!isxdigit((unsigned char)*p)) { return false; }
    }
    return true;
}

static void dal_c__manifestArtifactPartsCleanup(
    char** role,
    char** path,
    char** abi,
    char** link,
    char** toolchain
) {
    if (role) {
        free(*role);
        *role = NULL;
    }
    if (path) {
        free(*path);
        *path = NULL;
    }
    if (abi) {
        free(*abi);
        *abi = NULL;
    }
    if (link) {
        free(*link);
        *link = NULL;
    }
    if (toolchain) {
        free(*toolchain);
        *toolchain = NULL;
    }
}

static bool dal_c__manifestArtifactParse(
    const char* value,
    char** role_out,
    char** path_out,
    char** abi_out,
    char** link_out,
    char** toolchain_out,
    char** reason_out
) {
    if (role_out) { *role_out = NULL; }
    if (path_out) { *path_out = NULL; }
    if (abi_out) { *abi_out = NULL; }
    if (link_out) { *link_out = NULL; }
    if (toolchain_out) { *toolchain_out = NULL; }
    if (!value) {
        if (reason_out) { *reason_out = strdup("missing artifact entry"); }
        return false;
    }

    char* copy = strdup(value);
    if (!copy) {
        if (reason_out) { *reason_out = strdup("out of memory while parsing artifact entry"); }
        return false;
    }
    char* parts[5] = { NULL };
    char* cursor = copy;
    for (int i = 0; i < 5; ++i) {
        parts[i] = cursor;
        char* separator = strchr(cursor, '|');
        if (i == 4) {
            if (separator) {
                if (reason_out) { *reason_out = str_format("artifact entry has too many fields: %s", value); }
                free(copy);
                return false;
            }
        } else {
            if (!separator) {
                if (reason_out) { *reason_out = str_format("artifact entry has too few fields: %s", value); }
                free(copy);
                return false;
            }
            *separator = '\0';
            cursor = separator + 1;
        }
    }

    bool role_ok = dal_c__manifestArtifactRoleValid(parts[0]);
    bool path_ok = str_startsWith(parts[1], "libs/")
                && !strstr(parts[1], "../")
                && !strstr(parts[1], "/..")
                && !strchr(parts[1], '\\');
    bool abi_ok = dal_c__manifestHashValueValid(parts[2]);
    bool link_ok = str_eql(parts[3], "none") || dal_c__manifestHashValueValid(parts[3]);
    bool toolchain_ok = str_eql(parts[4], "native") || dal_c__manifestHashValueValid(parts[4]);
    if (str_eql(parts[0], "static-lto")) {
        toolchain_ok = dal_c__manifestHashValueValid(parts[4]);
    }
    if (!role_ok || !path_ok || !abi_ok || !link_ok || !toolchain_ok) {
        if (reason_out) { *reason_out = str_format("invalid artifact contract: %s", value); }
        free(copy);
        return false;
    }

    if (role_out) { *role_out = strdup(parts[0]); }
    if (path_out) { *path_out = strdup(parts[1]); }
    if (abi_out) { *abi_out = strdup(parts[2]); }
    if (link_out) { *link_out = strdup(parts[3]); }
    if (toolchain_out) { *toolchain_out = strdup(parts[4]); }
    bool copied = (!role_out || *role_out) && (!path_out || *path_out)
               && (!abi_out || *abi_out) && (!link_out || *link_out)
               && (!toolchain_out || *toolchain_out);
    free(copy);
    if (!copied) {
        dal_c__manifestArtifactPartsCleanup(role_out, path_out, abi_out, link_out, toolchain_out);
        if (reason_out) { *reason_out = strdup("out of memory while copying artifact contract"); }
        return false;
    }
    return true;
}

static bool dal_c__manifestArtifactValueValid(const char* value, char** reason_out) {
    return dal_c__manifestArtifactParse(value, NULL, NULL, NULL, NULL, NULL, reason_out);
}

static bool dal_c__validatePrebuiltManifestSchema(const char* manifest_path, char** reason_out) {
    enum { key_target,
           key_profile,
           key_count };
    bool seen[key_count] = { false };
    ArrStr* artifacts = ArrStr_init();
    int line_count = 0;
    char** lines = file_readLines(manifest_path, &line_count);
    if (!lines || !artifacts) {
        if (reason_out) { *reason_out = strdup("failed to read manifest.dh"); }
        ArrStr_fini(&artifacts);
        return false;
    }

    bool ok = true;
    for (int i = 0; i < line_count && ok; ++i) {
        char* line = lines[i];
        while (*line == ' ' || *line == '\t') { ++line; }
        size_t line_len = strlen(line);
        while (line_len > 0 && (line[line_len - 1] == '\r' || line[line_len - 1] == '\n' || line[line_len - 1] == ' ' || line[line_len - 1] == '\t')) {
            line[--line_len] = '\0';
        }
        if (line[0] == '\0' || line[0] == '#') { continue; }

        char* equals = strchr(line, '=');
        if (!equals || equals == line || equals[1] == '\0') {
            if (reason_out) { *reason_out = str_format("invalid manifest entry: %s", line); }
            ok = false;
            break;
        }
        *equals = '\0';
        char* key_end = equals;
        while (key_end > line && (key_end[-1] == ' ' || key_end[-1] == '\t')) { *--key_end = '\0'; }
        const char* value = equals + 1;
        while (*value == ' ' || *value == '\t') { ++value; }
        if (*value == '\0') {
            if (reason_out) { *reason_out = str_format("empty manifest value: %s", line); }
            ok = false;
            break;
        }

        int key_index = str_eql(line, "target") ? key_target
                                                : (str_eql(line, "profile") ? key_profile : -1);
        if (key_index >= 0) {
            if (seen[key_index]) {
                if (reason_out) { *reason_out = str_format("duplicate manifest key: %s", line); }
                ok = false;
                break;
            }
            seen[key_index] = true;
            continue;
        }
        if (!str_eql(line, "artifact")) {
            if (reason_out) { *reason_out = str_format("unsupported manifest key: %s", line); }
            ok = false;
            break;
        }
        if (!dal_c__manifestArtifactValueValid(value, reason_out)) {
            ok = false;
            break;
        }
        for (int j = 0; j < ArrStr_len(artifacts); ++j) {
            if (str_eql(ArrStr_at(artifacts, j), value)) {
                if (reason_out) { *reason_out = str_format("duplicate artifact entry: %s", value); }
                ok = false;
                break;
            }
        }
        if (ok) { ArrStr_push(artifacts, value); }
    }

    if (ok && (!seen[key_target] || !seen[key_profile])) {
        const char* missing = !seen[key_target] ? "target" : "profile";
        if (reason_out) { *reason_out = str_format("missing manifest key: %s", missing); }
        ok = false;
    }
    if (ok && ArrStr_len(artifacts) == 0) {
        if (reason_out) { *reason_out = strdup("manifest contains no library artifacts"); }
        ok = false;
    }

    ArrStr_fini(&artifacts);
    for (int i = 0; i < line_count; ++i) { free(lines[i]); }
    free(lines);
    return ok;
}

bool dal_c__copyPrebuiltManifestPackage(
    const char* source_profile_dir,
    const char* destination_profile_dir,
    char** reason_out
) {
    if (reason_out) { *reason_out = NULL; }
    if (!source_profile_dir || !destination_profile_dir) {
        if (reason_out) { *reason_out = strdup("missing prebuilt package path"); }
        return false;
    }

    char* manifest_src = path_join(source_profile_dir, "manifest.dh");
    char* manifest_dst = path_join(destination_profile_dir, "manifest.dh");
    if (!manifest_src || !manifest_dst || !dal_c__validatePrebuiltManifestSchema(manifest_src, reason_out)) {
        free(manifest_dst);
        free(manifest_src);
        return false;
    }

    int line_count = 0;
    char** lines = file_readLines(manifest_src, &line_count);
    bool ok = lines && dir_createRecur(destination_profile_dir) && file_copy(manifest_src, manifest_dst);
    for (int i = 0; lines && i < line_count && ok; ++i) {
        char* line = lines[i];
        while (*line == ' ' || *line == '\t') { ++line; }
        char* equals = strchr(line, '=');
        if (!equals) { continue; }
        char* key_end = equals;
        while (key_end > line && (key_end[-1] == ' ' || key_end[-1] == '\t')) { --key_end; }
        size_t key_len = (size_t)(key_end - line);
        if (key_len != strlen("artifact") || strncmp(line, "artifact", key_len) != 0) { continue; }

        char* value = equals + 1;
        while (*value == ' ' || *value == '\t') { ++value; }
        size_t value_len = strlen(value);
        while (value_len > 0 && (value[value_len - 1] == '\r' || value[value_len - 1] == '\n' || value[value_len - 1] == ' ' || value[value_len - 1] == '\t')) {
            value[--value_len] = '\0';
        }

        char* artifact_path = NULL;
        if (!dal_c__manifestArtifactParse(value, NULL, &artifact_path, NULL, NULL, NULL, reason_out)) {
            ok = false;
            break;
        }
        char* artifact_src = path_join(source_profile_dir, artifact_path);
        char* artifact_dst = path_join(destination_profile_dir, artifact_path);
        char* artifact_parent = artifact_dst ? path_parent(artifact_dst) : NULL;
        if (!artifact_src || !artifact_dst || !artifact_parent || !path_isFile(artifact_src)
            || !dir_createRecur(artifact_parent) || !file_copy(artifact_src, artifact_dst)) {
            if (reason_out && !*reason_out) {
                *reason_out = str_format("failed to copy manifest artifact: %s", artifact_path);
            }
            ok = false;
        }
        free(artifact_parent);
        free(artifact_dst);
        free(artifact_src);
        free(artifact_path);
    }

    if (!lines && reason_out && !*reason_out) {
        *reason_out = strdup("failed to read manifest artifacts");
    }
    for (int i = 0; i < line_count; ++i) { free(lines[i]); }
    free(lines);
    free(manifest_dst);
    free(manifest_src);
    return ok;
}

static const char* dal_c__manifestArtifactRole(dal_c_Target target_type, bool lto_enabled) {
    if (target_type == dal_c_Target_static_lib) {
        return lto_enabled ? "static-lto" : "static";
    }
    if (target_type == dal_c_Target_shared_lib) {
        return "shared";
    }
    return NULL;
}

static bool dal_c__manifestFindArtifact(
    const char* manifest_path,
    const char* expected_role,
    const char* expected_path,
    char** abi_out,
    char** link_out,
    char** toolchain_out
) {
    if (abi_out) { *abi_out = NULL; }
    if (link_out) { *link_out = NULL; }
    if (toolchain_out) { *toolchain_out = NULL; }
    if (!manifest_path || !expected_role || !expected_path) { return false; }

    int line_count = 0;
    char** lines = file_readLines(manifest_path, &line_count);
    bool found = false;
    for (int i = 0; lines && i < line_count && !found; ++i) {
        char* line = lines[i];
        while (*line == ' ' || *line == '\t') { ++line; }
        if (!str_startsWith(line, "artifact=")) { continue; }
        char* value = line + strlen("artifact=");
        size_t n = strlen(value);
        while (n > 0 && (value[n - 1] == '\r' || value[n - 1] == '\n' || value[n - 1] == ' ' || value[n - 1] == '\t')) {
            value[--n] = '\0';
        }
        char* role = NULL;
        char* path = NULL;
        char* abi = NULL;
        char* link = NULL;
        char* toolchain = NULL;
        if (dal_c__manifestArtifactParse(value, &role, &path, &abi, &link, &toolchain, NULL)
            && str_eql(role, expected_role) && str_eql(path, expected_path)) {
            if (abi_out) {
                *abi_out = abi;
                abi = NULL;
            }
            if (link_out) {
                *link_out = link;
                link = NULL;
            }
            if (toolchain_out) {
                *toolchain_out = toolchain;
                toolchain = NULL;
            }
            found = true;
        }
        dal_c__manifestArtifactPartsCleanup(&role, &path, &abi, &link, &toolchain);
    }
    for (int i = 0; lines && i < line_count; ++i) { free(lines[i]); }
    free(lines);
    return found;
}

bool dal_c__prebuiltManifestCompatible(
    const char* prebuilt_profile_dir,
    const dal_c_CompilerOpts* opts,
    const dal_c_ProfileSpec* profile,
    dal_c_Target target_type,
    bool lto_enabled,
    const char* selected_artifact,
    char** reason_out
) {
    if (reason_out) { *reason_out = NULL; }
    if (!prebuilt_profile_dir || !opts || !profile || !selected_artifact) { return false; }
    char* manifest_path = path_join(prebuilt_profile_dir, "manifest.dh");
    if (!manifest_path || !path_isFile(manifest_path)) {
        if (reason_out) { *reason_out = strdup("missing manifest.dh"); }
        free(manifest_path);
        return false;
    }
    if (!dal_c__validatePrebuiltManifestSchema(manifest_path, reason_out)) {
        free(manifest_path);
        return false;
    }

    dal_c_Cmd compatibility_cmd = { 0 };
    dal_c_CompilerOpts_merge(&compatibility_cmd.opts, opts);
    char* expected_target = dal_c__resolveTargetDirName(opts);
    char* actual_target = dal_c__manifestValue(manifest_path, "target");
    char* actual_profile = dal_c__manifestValue(manifest_path, "profile");
    const char* artifact_role = dal_c__manifestArtifactRole(target_type, lto_enabled);
    char* selected_relative_raw = path_relative(prebuilt_profile_dir, selected_artifact);
    char* selected_relative = dal_c__manifestNormalizeRelative(selected_relative_raw);
    char* actual_abi = NULL;
    char* actual_link = NULL;
    char* actual_toolchain = NULL;
    bool has_artifact = artifact_role && selected_relative
                     && dal_c__manifestFindArtifact(
                            manifest_path,
                            artifact_role,
                            selected_relative,
                            &actual_abi,
                            &actual_link,
                            &actual_toolchain
                     );
    char* expected_abi = dal_c__makePrebuiltAbiContractKey(&compatibility_cmd, profile, target_type);
    char* expected_toolchain = lto_enabled ? dal_c__makeLtoToolchainContractKey(opts) : strdup("native");
    bool ok = true;
    if (!actual_target || !expected_target || !str_eql(actual_target, expected_target)) {
        ok = false;
        if (reason_out) { *reason_out = str_format("target mismatch: package=%s requested=%s", actual_target ? actual_target : "(missing)", expected_target ? expected_target : "(unknown)"); }
    } else if (!actual_profile || !str_eql(actual_profile, profile->name)) {
        ok = false;
        if (reason_out) { *reason_out = str_format("profile mismatch: package=%s requested=%s", actual_profile ? actual_profile : "(missing)", profile->name); }
    } else if (!has_artifact) {
        ok = false;
        if (reason_out) { *reason_out = str_format("manifest does not provide requested artifact: %s|%s", artifact_role ? artifact_role : dal_c_Target_format(target_type), selected_relative ? selected_relative : "(unknown)"); }
    } else if (!actual_abi || !expected_abi || !str_eql(actual_abi, expected_abi)) {
        ok = false;
        if (reason_out) { *reason_out = str_format("ABI contract mismatch: package=%s requested=%s", actual_abi ? actual_abi : "(missing)", expected_abi ? expected_abi : "(unknown)"); }
    } else if (!actual_toolchain || !expected_toolchain || !str_eql(actual_toolchain, expected_toolchain)) {
        ok = false;
        if (reason_out) { *reason_out = str_format("LTO toolchain mismatch: package=%s requested=%s", actual_toolchain ? actual_toolchain : "(missing)", expected_toolchain ? expected_toolchain : "(unknown)"); }
    } else if (dal_c__platformIsWindows() && target_type == dal_c_Target_shared_lib) {
        char* import_path = dal_c__makeSharedImportLibraryPath(selected_artifact);
        char* import_relative_raw = import_path ? path_relative(prebuilt_profile_dir, import_path) : NULL;
        char* import_relative = dal_c__manifestNormalizeRelative(import_relative_raw);
        char* import_abi = NULL;
        char* import_link = NULL;
        char* import_toolchain = NULL;
        bool import_ok = import_path && path_isFile(import_path) && import_relative
                      && dal_c__manifestFindArtifact(
                             manifest_path,
                             "import",
                             import_relative,
                             &import_abi,
                             &import_link,
                             &import_toolchain
                      )
                      && str_eql(import_abi, actual_abi)
                      && str_eql(import_link, actual_link)
                      && str_eql(import_toolchain, actual_toolchain);
        if (!import_ok) {
            ok = false;
            if (reason_out) { *reason_out = strdup("shared artifact is missing its matching Windows import library contract"); }
        }
        free(import_toolchain);
        free(import_link);
        free(import_abi);
        free(import_relative);
        free(import_relative_raw);
        free(import_path);
    }

    free(expected_toolchain);
    free(expected_abi);
    free(actual_toolchain);
    free(actual_link);
    free(actual_abi);
    free(selected_relative);
    free(selected_relative_raw);
    free(actual_profile);
    free(actual_target);
    free(expected_target);
    dal_c_CompilerOpts_cleanup(&compatibility_cmd.opts);
    free(manifest_path);
    return ok;
}

bool dal_c__resolveCompatiblePrebuiltArtifact(
    const char* prebuilt_profile_dir,
    const char* name,
    dal_c_Target target_type,
    bool is_windows,
    bool lto_requested,
    const dal_c_CompilerOpts* opts,
    const dal_c_ProfileSpec* profile,
    char** artifact_out,
    bool* lto_selected_out,
    char** reason_out
) {
    if (artifact_out) { *artifact_out = NULL; }
    if (lto_selected_out) { *lto_selected_out = false; }
    if (reason_out) { *reason_out = NULL; }
    if (!prebuilt_profile_dir || !name || !opts || !profile || !artifact_out) {
        return false;
    }

    char* reason = NULL;
    char* artifact = dal_c__resolvePrebuiltArtifactPath(
        prebuilt_profile_dir, name, target_type, is_windows, lto_requested
    );
    bool compatible = artifact && dal_c__prebuiltManifestCompatible(prebuilt_profile_dir, opts, profile, target_type, lto_requested, artifact, &reason);
    if (compatible) {
        *artifact_out = artifact;
        if (lto_selected_out) { *lto_selected_out = lto_requested; }
        free(reason);
        return true;
    }

    if (lto_requested && target_type == dal_c_Target_static_lib) {
        free(reason);
        reason = NULL;
        free(artifact);
        artifact = dal_c__resolvePrebuiltArtifactPath(
            prebuilt_profile_dir, name, target_type, is_windows, false
        );
        dal_c_CompilerOpts native_opts = *opts;
        native_opts.lto_mode = dal_c_LtoMode_off;
        compatible = artifact && dal_c__prebuiltManifestCompatible(prebuilt_profile_dir, &native_opts, profile, target_type, false, artifact, &reason);
        if (compatible) {
            *artifact_out = artifact;
            free(reason);
            return true;
        }
    }

    free(artifact);
    if (reason_out) {
        *reason_out = reason;
    } else {
        free(reason);
    }
    return false;
}

static const char* dal_c__manifestRoleForFileName(const char* name, bool is_windows) {
    if (!name) { return NULL; }
    if (is_windows) {
        if (str_endsWith(name, ".lto.lib")) { return "static-lto"; }
        if (str_endsWith(name, ".dll.lib")) { return "import"; }
        if (str_endsWith(name, ".lib")) { return "static"; }
        if (str_endsWith(name, ".dll")) { return "shared"; }
        return NULL;
    }
    if (str_endsWith(name, ".lto.a")) { return "static-lto"; }
    if (str_endsWith(name, ".a")) { return "static"; }
    if (str_endsWith(name, ".so") || str_endsWith(name, ".dylib")) { return "shared"; }
    return NULL;
}

static int dal_c__compareCStringPointers(const void* lhs, const void* rhs) {
    const char* const* a = lhs;
    const char* const* b = rhs;
    if (!*a && !*b) { return 0; }
    if (!*a) { return -1; }
    if (!*b) { return 1; }
    return strcmp(*a, *b);
}

static bool dal_c__writeManifestArtifactSidecar(
    const char* profile_dir,
    const char* relative_path,
    const char* role,
    const char* abi_contract,
    const char* link_contract,
    const char* toolchain_contract
) {
    if (!profile_dir || !relative_path || !role || !abi_contract || !link_contract || !toolchain_contract) {
        return false;
    }
    char* sidecar_dir = path_join(profile_dir, ".manifest");
    char* basename = path_basename(relative_path);
    char* sidecar_name = basename ? str_format("%s.entry", basename) : NULL;
    char* sidecar_path = sidecar_dir && sidecar_name ? path_join(sidecar_dir, sidecar_name) : NULL;
    char* value = str_format(
        "%s|%s|%s|%s|%s",
        role,
        relative_path,
        abi_contract,
        link_contract,
        toolchain_contract
    );
    char* content = value ? str_format("artifact=%s\n", value) : NULL;
    bool ok = sidecar_dir && sidecar_path && content
           && dir_createRecur(sidecar_dir)
           && dal_c__writeFileIfChanged(sidecar_path, content);
    free(content);
    free(value);
    free(sidecar_path);
    free(sidecar_name);
    free(basename);
    free(sidecar_dir);
    return ok;
}

static bool dal_c__rebuildPrebuiltManifestFromSidecars(
    const char* profile_dir,
    const dal_c_CompilerOpts* opts,
    const dal_c_ProfileSpec* profile
) {
    char* sidecar_dir = path_join(profile_dir, ".manifest");
    int entry_count = 0;
    char** entries = sidecar_dir ? dir_listEntries(sidecar_dir, &entry_count) : NULL;
    if (entries && entry_count > 1) {
        qsort(entries, (size_t)entry_count, sizeof(*entries), dal_c__compareCStringPointers);
    }
    char* target_name = dal_c__resolveTargetDirName(opts);
    char* content = str_format(
        "target=%s\nprofile=%s\n",
        target_name ? target_name : "unknown",
        profile->name
    );
    int artifact_count = 0;
    for (int i = 0; entries && i < entry_count; ++i) {
        char* sidecar_text = path_isFile(entries[i]) ? file_read(entries[i]) : NULL;
        if (!sidecar_text || !str_startsWith(sidecar_text, "artifact=")) {
            free(sidecar_text);
            free(entries[i]);
            continue;
        }
        char* value = sidecar_text + strlen("artifact=");
        size_t value_len = strlen(value);
        while (value_len > 0 && (value[value_len - 1] == '\r' || value[value_len - 1] == '\n' || value[value_len - 1] == ' ' || value[value_len - 1] == '\t')) {
            value[--value_len] = '\0';
        }
        char* artifact_path = NULL;
        if (!dal_c__manifestArtifactParse(value, NULL, &artifact_path, NULL, NULL, NULL, NULL)) {
            free(artifact_path);
            free(sidecar_text);
            free(entries[i]);
            continue;
        }
        char* absolute_artifact = path_join(profile_dir, artifact_path);
        if (!absolute_artifact || !path_isFile(absolute_artifact)) {
            (void)remove(entries[i]);
        } else {
            char* next = content ? str_format("%sartifact=%s\n", content, value) : NULL;
            if (next) {
                free(content);
                content = next;
                artifact_count++;
            }
        }
        free(absolute_artifact);
        free(artifact_path);
        free(sidecar_text);
        free(entries[i]);
    }
    free(entries);

    char* manifest_path = path_join(profile_dir, "manifest.dh");
    bool ok = content && artifact_count > 0 && manifest_path
           && dal_c__writeFileIfChanged(manifest_path, content);
    free(manifest_path);
    free(content);
    free(target_name);
    free(sidecar_dir);
    return ok;
}

bool dal_c__writePrebuiltManifest(
    const char* profile_dir,
    const dal_c_Cmd* cmd,
    const dal_c_ProfileSpec* profile,
    const char* target_path,
    dal_c_Target target_type
) {
    if (!profile_dir || !cmd || !profile || !target_path) { return false; }
    char* artifact_dir = path_parent(target_path);
    char* relative_dir_raw = artifact_dir ? path_relative(profile_dir, artifact_dir) : NULL;
    char* relative_dir = dal_c__manifestNormalizeRelative(relative_dir_raw);
    if (!artifact_dir || !relative_dir || !str_eql(relative_dir, "libs")) {
        free(relative_dir);
        free(relative_dir_raw);
        free(artifact_dir);
        return true;
    }

    char* target_name = path_basename(target_path);
    const char* role = dal_c__manifestRoleForFileName(target_name, dal_c__platformIsWindows());
    char* target_relative_raw = path_relative(profile_dir, target_path);
    char* target_relative = dal_c__manifestNormalizeRelative(target_relative_raw);
    char* abi_contract = dal_c__makePrebuiltAbiContractKey(cmd, profile, target_type);
    bool is_lto = role && str_eql(role, "static-lto");
    char* toolchain_contract = is_lto
                                 ? dal_c__makeLtoToolchainContractKey(&cmd->opts)
                                 : strdup("native");
    char* link_contract = target_type == dal_c_Target_shared_lib
                            ? dal_c__makeLinkContractKey(cmd, profile, target_type)
                            : strdup("none");
    bool ok = role && target_relative && abi_contract && toolchain_contract && link_contract
           && dal_c__writeManifestArtifactSidecar(
                  profile_dir,
                  target_relative,
                  role,
                  abi_contract,
                  link_contract,
                  toolchain_contract
           );

    if (ok && dal_c__platformIsWindows() && target_type == dal_c_Target_shared_lib) {
        char* import_path = dal_c__makeSharedImportLibraryPath(target_path);
        char* import_relative_raw = import_path ? path_relative(profile_dir, import_path) : NULL;
        char* import_relative = dal_c__manifestNormalizeRelative(import_relative_raw);
        ok = import_path && path_isFile(import_path) && import_relative
          && dal_c__writeManifestArtifactSidecar(
                 profile_dir,
                 import_relative,
                 "import",
                 abi_contract,
                 link_contract,
                 toolchain_contract
          );
        free(import_relative);
        free(import_relative_raw);
        free(import_path);
    }
    if (ok) {
        ok = dal_c__rebuildPrebuiltManifestFromSidecars(profile_dir, &cmd->opts, profile);
    }

    free(link_contract);
    free(toolchain_contract);
    free(abi_contract);
    free(target_relative);
    free(target_relative_raw);
    free(target_name);
    free(relative_dir);
    free(relative_dir_raw);
    free(artifact_dir);
    return ok;
}

static char* dal_c__makePrebuiltProfileDir(const char* project_root, const dal_c_CompilerOpts* opts, const char* profile_name);
static char* dal_c__makeMakePath(const char* path);
static char* dal_c__makeTempPath(const char* path);
static void dal_c__fprintMakePath(FILE* fp, const char* path);
static void dal_c__ensureParentDir(const char* path);
static void dal_c__writeMakefilePrelude(FILE* fp, const dal_c_Cmd* cmd);
static dal_c__noinline void dal_c__writeMakefileVariables(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, const dal_c_Project* proj, const char* build_dir, dal_c_Target target_type, const char* target_path, const char* link_contract_path);
static dal_c__noinline void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* build_dir, dal_c_Target target_type);

static dal_c__noinline int dal_c__writeEmitOnlyMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* target_path,
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
static bool dal_c__shouldAddProjectInclude(const dal_c_Project* proj, const dal_c_Cmd* cmd);
static bool dal_c__shouldAddProjectPrivateInclude(const dal_c_Project* proj, const dal_c_Cmd* cmd);
static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode);
static char* dal_c__makeLinkContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
char* dal_c__makePrebuiltAbiContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeLtoToolchainContractKey(const dal_c_CompilerOpts* opts);
static char* dal_c__makeLinkContractPath(const char* build_dir, const char* target_path);
static bool dal_c__writeLinkContractFile(const char* path, const char* target_path, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode);
static char* dal_c__makePchPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* ext);
static bool dal_c__pchEnabled(const dal_c_Project* proj);
static bool dal_c__pchEnabledForOpts(const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
static bool dal_c__pchDepsAreUpToDate(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* object_dir, dal_c_Target target_type);
static bool dal_c__sourcesUsePch(const dal_c_Project* proj, ArrStr* sources, bool has_pch);
static bool dal_c__sourceIsAssembly(const char* src);
static char* dal_c__makeImageLinkPath(const char* target_path);
static uint64_t dal_c__hashVersionSpec(uint64_t hash, const dal_c_VersionSpec* version);
static char* dal_c__versionNamespace(const dal_c_Project* proj, const dal_c_VersionSpec* version);
static void dal_c__writeVersionDefines(FILE* fp, const dal_c_Project* proj, const dal_c_VersionSpec* version);
static void dal_c__appendVersionDefineArguments(ArrStr* argv, const dal_c_Project* proj, const dal_c_VersionSpec* version);
static bool dal_c__targetIsEmitOnly(dal_c_Target target_type);
static bool dal_c__linkedPlanIsUpToDate(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base, dal_c_Target target_type, const char* target_path, const char* link_contract_path);
static dal_c__noinline void dal_c__writeMakefileCompilationRules(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, ArrStr* sources, bool has_pch, const char* object_dir, const char* base, dal_c_Target target_type);
static void dal_c__writeMakefileTargetVar(FILE* fp, const char* target_path);
static dal_c__noinline void dal_c__writeMakefileTargetRule(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target type, bool is_windows, const char* link_contract_path);
static char* dal_c__artifactPath(const char* target_path, const char* override_path, const char* ext);
static void dal_c__writePlatformLinkerFlags(FILE* fp, bool is_windows, const dal_c_ProfileSpec* profile, const char* target_path);
static void dal_c__writeLinkModelFlags(FILE* fp, bool is_windows, const dal_c_CompilerOpts* opts, dal_c_Target target_type);
static char* dal_c__buildParallelFlag(const dal_c_Cmd* cmd);
static bool dal_c__shouldPrintLibDHSkip(const dal_c_Project* dh_proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static bool dal_c__writeFileIfChanged(const char* path, const char* content);
static const char* dal_c__planContextDir(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_Target target_type);
static ArrStr* dal_c__collectLibrarySources(const dal_c_Lib* lib, const dal_c_Project* lib_proj);
static bool dal_c__cmdAggregatesRecursiveTests(const dal_c_Cmd* cmd);
static int dal_c__runDependencyTests(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
static bool dal_c__compilerLooksLikeClang(const char* compiler);
static bool dal_c__copyLibraryArtifacts(
    const dal_c_Project* consumer_proj,
    const dal_c_Project* lib_proj,
    const dal_c_Lib* lib,
    const char* lib_abs_path,
    const char* lib_build_dir,
    const dal_c_Cmd* build_cmd,
    const dal_c_ProfileSpec* build_profile,
    dal_c_Target build_target_type,
    bool is_windows,
    bool from_prebuilt
);
static char* dal_c__sanitizePathFragment(const char* value);
static bool dal_c__pathHasSeparator(const char* path);
static bool dal_c__pathIsAbsolute(const char* path);
static char* dal_c__makeTargetFileName(const char* name, dal_c_Target type, bool is_windows, const char* output_ext);
static char* dal_c__makePdbPath(const char* target_path);
static bool dal_c__usesAggregateTestTarget(const dal_c_Cmd* cmd);
static bool dal_c__pathContainsSegment(const char* path, const char* segment);
static bool dal_c__pathHasHiddenSegment(const char* path);
static bool dal_c__pathMatchesExclude(const char* path, const char* exclude_path);
static bool dal_c__isExcludedByProject(const dal_c_Project* proj, const char* path);
static bool dal_c__isSelfProjectDir(const char* path);
static char* dal_c__findSelfProjectDirFrom(const char* start_dir);
static char* dal_c__resolveSelfProjectDir(void);
static int dal_c__runSelfMake(const dal_c_Cmd* cmd, const char* target);
static dal_c__noinline dal_c__optnone bool dal_c__validateLinkToolchain(const dal_c_CompilerOpts* opts, dal_c_Target target_type);
static bool dal_c__validateBuildArtifacts(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, ArrStr* sources, dal_c_Target target_type);
static bool dal_c__targetUsesImplicitCompilerRt(dal_c_Target target_type);
static bool dal_c__resolvedDefaultLibsLinked(const dal_c_CompilerOpts* opts);
static dal_c__noinline dal_c__optnone char* dal_c__queryCompilerRtPath(const dal_c_CompilerOpts* opts);
static ArrStr* dal_c__parseQuotedTokens(const char* line);
static ArrStr* dal_c__queryToolchainLinkTokens(const dal_c_CompilerOpts* opts);
static void dal_c__printToolchainCategory(const char* title, ArrStr* link_tokens, const char* compiler_rt_path, dal_c_ToolchainQuery query);
static dal_c_LtoMode dal_c__resolvedLtoState(const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile);
static dal_c_LtoMode dal_c__effectiveLtoState(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static bool dal_c__effectiveLtoEnabled(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static bool dal_c__shouldUseLldForClangLto(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
static void dal_c__warnIfProfileLtoDisabledByToolchain(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type);
void dal_c__appendCompileDbArguments(ArrStr* argv, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* src);
void dal_c__appendSyntaxArguments(ArrStr* argv, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* src, dal_c_Target target_type);
static void dal_c__appendCompileDbDiagnostics(
    ArrStr* argv,
    const dal_c_CompilerOpts* opts,
    bool compiler_is_clang,
    bool stack_frame_diagnostics
);
static char* dal_c__jsonEscape(const char* text);
static void dal_c__fprintJsonString(FILE* fp, const char* text);
static char* dal_c__mergeConfiguredArgs(const char* property_args, const char* cli_args);
static const char* dal_c__effectiveLinkerScript(const dal_c_Cmd* cmd);
static const char* dal_c__effectiveObjcopy(const dal_c_Cmd* cmd);
static const char* dal_c__effectiveObjcopyFormat(const dal_c_Cmd* cmd);

static char* dal_c__mergeConfiguredArgs(const char* property_args, const char* cli_args) {
    if (!property_args || property_args[0] == '\0') {
        return (!cli_args || cli_args[0] == '\0') ? NULL : strdup(cli_args);
    }
    if (!cli_args || cli_args[0] == '\0') { return strdup(property_args); }
    return str_format("%s %s", property_args, cli_args);
}

static const char* dal_c__effectiveLinkerScript(const dal_c_Cmd* cmd) {
    return cmd && cmd->linker_script ? cmd->linker_script : (cmd ? cmd->opts.linker_script : NULL);
}

static const char* dal_c__effectiveObjcopy(const dal_c_Cmd* cmd) {
    if (!cmd) { return dal_c_default_objcopy; }
    if (cmd->objcopy) { return cmd->objcopy; }
    if (cmd->opts.objcopy) { return cmd->opts.objcopy; }
    return dal_c_default_objcopy;
}

static const char* dal_c__effectiveObjcopyFormat(const dal_c_Cmd* cmd) {
    if (!cmd) { return "binary"; }
    if (cmd->objcopy_format) { return cmd->objcopy_format; }
    if (cmd->opts.objcopy_format) { return cmd->opts.objcopy_format; }
    return "binary";
}

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

static char* dal_c__makeTempPath(const char* path) {
    assert(path != NULL);
#ifdef _WIN32
    unsigned long pid = (unsigned long)GetCurrentProcessId();
#else
    unsigned long pid = (unsigned long)getpid();
#endif
    return str_format("%s.tmp.%lu.%p", path, pid, (const void*)path);
}

static void dal_c__fprintMakePath(FILE* fp, const char* path) {
    assert(fp != NULL);
    assert(path != NULL);
    char* make_path = dal_c__makeMakePath(path);
    (void)fprintf(fp, "%s", make_path ? make_path : path);
    free(make_path);
}

static void dal_c__ensureParentDir(const char* path) {
    if (!path || path[0] == '\0') {
        return;
    }
    char* parent = path_parent(path);
    if (parent && parent[0] != '\0') {
        dir_createRecur(parent);
    }
    free(parent);
}

static void dal_c__writeMakefilePrelude(FILE* fp, const dal_c_Cmd* cmd) {
    assert(fp != NULL);
    assert(cmd != NULL);
    (void)fprintf(fp, ".SUFFIXES:\n");
    (void)fprintf(fp, "MAKEFLAGS += --no-builtin-rules --no-builtin-variables --no-print-directory\n");
    (void)fprintf(fp, "COMMANDS ?= %s\n", (cmd->verbose || cmd->show_commands) ? "show" : "hide");
    (void)fprintf(fp, "PROGRESS ?= %s\n", cmd->show_progress ? "show" : "hide");
    (void)fprintf(fp, "VERBOSE ?= %s\n", cmd->verbose ? "on" : "off");
    (void)fprintf(fp, "ifeq ($(VERBOSE),on)\n");
    (void)fprintf(fp, "COMMANDS := show\n");
    (void)fprintf(fp, "endif\n");
    (void)fprintf(fp, "ifeq ($(COMMANDS),show)\n");
    (void)fprintf(fp, "Q :=\n");
    (void)fprintf(fp, "PROGRESS := hide\n");
    (void)fprintf(fp, "else\n");
    (void)fprintf(fp, "Q := @\n");
    (void)fprintf(fp, "endif\n");
    (void)fprintf(fp, "RM ?= rm -f\n");
    (void)fprintf(fp, "MV ?= mv -f\n");
    (void)fprintf(fp, "PRINTF ?= printf\n");
    (void)fprintf(fp, "ifeq ($(PROGRESS),show)\n");
    (void)fprintf(fp, "P_CC = $(PRINTF) '[%%s/%%s] CC %%s\\n' '$1' '$2' '$3'; \n");
    (void)fprintf(fp, "P_AS = $(PRINTF) '[%%s/%%s] AS %%s\\n' '$1' '$2' '$3'; \n");
    (void)fprintf(fp, "P_PCH = $(PRINTF) '[PCH] %%s\\n' '$1'; \n");
    (void)fprintf(fp, "P_LD = $(PRINTF) '[LD] %%s\\n' '$1'; \n");
    (void)fprintf(fp, "P_AR = $(PRINTF) '[AR] %%s\\n' '$1'; \n");
    (void)fprintf(fp, "P_OBJCOPY = $(PRINTF) '[OBJCOPY] %%s\\n' '$1'; \n");
    (void)fprintf(fp, "P_GEN = $(PRINTF) '[GEN] %%s\\n' '$1'; \n");
    (void)fprintf(fp, "else\n");
    (void)fprintf(fp, "P_CC =\n");
    (void)fprintf(fp, "P_AS =\n");
    (void)fprintf(fp, "P_PCH =\n");
    (void)fprintf(fp, "P_LD =\n");
    (void)fprintf(fp, "P_AR =\n");
    (void)fprintf(fp, "P_OBJCOPY =\n");
    (void)fprintf(fp, "P_GEN =\n");
    (void)fprintf(fp, "endif\n\n");
}

static char* dal_c__globalCacheBaseDir(void) {
    char* override = env_get("DH_C_CACHE_HOME");
    if (override && override[0]) { return override; }
    free(override);
#ifdef _WIN32
    char* local_app_data = env_get("LOCALAPPDATA");
    if (local_app_data && local_app_data[0]) {
        char* dh_c = path_join(local_app_data, dal_c_tool_name);
        char* cache = path_join(dh_c, "cache");
        free(dh_c);
        free(local_app_data);
        return cache;
    }
    free(local_app_data);
#else
    char* xdg_cache = env_get("XDG_CACHE_HOME");
    if (xdg_cache && xdg_cache[0]) {
        char* cache = path_join(xdg_cache, dal_c_tool_name);
        free(xdg_cache);
        return cache;
    }
    free(xdg_cache);
    char* home = env_get("HOME");
    if (home && home[0]) {
        char* dot_cache = path_join(home, ".cache");
        char* cache = path_join(dot_cache, dal_c_tool_name);
        free(dot_cache);
        free(home);
        return cache;
    }
    free(home);
#endif
    char* cwd = env_getCWD();
    char* fallback = cwd ? path_join(cwd, ".dh-c-cache") : strdup(".dh-c-cache");
    free(cwd);
    return fallback;
}

char* dal_c__cacheBaseDir(const dal_c_Project* proj) {
    if (proj && proj->workspace_root) {
        char* workspace_state = path_join(proj->workspace_root, ".dh-c");
        char* cache_dir = workspace_state ? path_join(workspace_state, "cache") : NULL;
        free(workspace_state);
        return cache_dir;
    }
    if (proj && proj->root) {
        char* build_dir = dal_c_Project_getBuildDir(proj);
        char* cache_dir = path_join(build_dir, dal_c_dir_cache);
        free(build_dir);
        return cache_dir;
    }
    return dal_c__globalCacheBaseDir();
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

static void dal_c__pushSelfMakeStringList(ArrStr* argv, const char* key, const char* prefix, char** items, int count) {
    assert(argv != NULL);
    assert(key != NULL);
    assert(prefix != NULL);
    if (!items || count <= 0) {
        return;
    }

    char* value = NULL;
    for (int i = 0; i < count; ++i) {
        if (!items[i] || items[i][0] == '\0') {
            continue;
        }
        char* next = value
                       ? str_format("%s %s%s", value, prefix, items[i])
                       : str_format("%s%s", prefix, items[i]);
        free(value);
        value = next;
    }
    dal_c__pushSelfMakeKeyValue(argv, key, value);
    free(value);
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

    dal_c_ProjectLock lock = { 0 };
    if (!dal_c__projectLockAcquireAt(self_dir, &lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire self-build lock\n");
        free(self_dir);
        return 1;
    }

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c__externalToolPath(dal_c_ExternalTool_make));
    ArrStr_push(argv, "-rR");
    ArrStr_push(argv, "--no-print-directory");
    char* j_flag = dal_c__buildParallelFlag(cmd);
    ArrStr_push(argv, j_flag);
    free(j_flag);
    ArrStr_push(argv, "-C");
    ArrStr_push(argv, self_dir);
    if (cmd->verbose) {
        ArrStr_push(argv, "VERBOSE=on");
    }
    if (cmd->show_commands) {
        ArrStr_push(argv, "COMMANDS=show");
    }
    if (!cmd->show_progress) {
        ArrStr_push(argv, "PROGRESS=hide");
    }
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
    if (cmd->opts.compile_env != dal_c_CompileEnv_auto) {
        dal_c__pushSelfMakeKeyValue(argv, "COMPILE_ENV", dal_c_CompileEnv_format(cmd->opts.compile_env));
    }
    dal_c__pushSelfMakeStringList(argv, "EXTRA_INCLUDES", "-I", cmd->opts.include_paths, cmd->opts.include_count);
    dal_c__pushSelfMakeStringList(argv, "EXTRA_ISYSTEMS", "-isystem ", cmd->opts.isystem_paths, cmd->opts.isystem_count);
    dal_c__pushSelfMakeStringList(argv, "EXTRA_DEFINES", "-D", cmd->opts.define_macros, cmd->opts.define_count);
    dal_c__pushSelfMakeStringList(argv, "EXTRA_UNDEFS", "-U", cmd->opts.undef_macros, cmd->opts.undef_count);
    dal_c__pushSelfMakeStringList(argv, "EXTRA_LIBS", "-l", cmd->opts.link_libs, cmd->opts.link_count);
    dal_c__pushSelfMakeStringList(argv, "EXTRA_LIB_DIRS", "-L", cmd->opts.link_dirs, cmd->opts.link_dir_count);
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
    dal_c__pushSelfMakeToggle(argv, "MERGE_ALL_CONSTANTS", cmd->opts.merge_all_constants);
    dal_c__pushSelfMakeToggle(argv, "STACK_PROTECTOR", cmd->opts.stack_protector);
    if (cmd->opts.macro_backtrace_limit_set) {
        char* limit = str_format("%d", cmd->opts.macro_backtrace_limit);
        dal_c__pushSelfMakeKeyValue(argv, "MACRO_BACKTRACE_LIMIT", limit);
        free(limit);
    }
    if (cmd->opts.loose_errors != dal_c_LooseErrorsMode_auto) {
        dal_c__pushSelfMakeKeyValue(argv, "LOOSE_ERRORS", dal_c_LooseErrorsMode_format(cmd->opts.loose_errors));
    }
    dal_c__pushSelfMakeKeyValue(argv, "ENTRY", cmd->opts.entry_symbol);
    char* configured_comp_args = dal_c__mergeConfiguredArgs(cmd->opts.compiler_args, cmd->compiler_args);
    char* configured_link_args = dal_c__mergeConfiguredArgs(cmd->opts.link_args, cmd->link_args);
    dal_c__pushSelfMakeKeyValue(argv, "COMP_ARGS", configured_comp_args);
    dal_c__pushSelfMakeKeyValue(argv, "LINK_ARGS", configured_link_args);
    dal_c__pushSelfMakeKeyValue(argv, "LINKER_SCRIPT", dal_c__effectiveLinkerScript(cmd));
    free(configured_comp_args);
    free(configured_link_args);
    bool build_artifact_like = cmd->action == dal_c_CmdAction_build || cmd->action == dal_c_CmdAction_build_self;
    if (build_artifact_like) {
        if (cmd->payload.build.save_temps != dal_c_SaveTempsMode_off) {
            dal_c__pushSelfMakeKeyValue(argv, "SAVE_TEMPS", dal_c_SaveTempsMode_format(cmd->payload.build.save_temps));
        }
        dal_c__pushSelfMakeToggle(argv, "EMIT_MAP", cmd->payload.build.emit_map ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeKeyValue(argv, "MAP_PATH", cmd->payload.build.emit_map_path);
        dal_c__pushSelfMakeToggle(argv, "EMIT_PREPROCESSED", cmd->payload.build.emit_preprocessed ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeToggle(argv, "EMIT_ASM", cmd->payload.build.emit_asm ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeToggle(argv, "EMIT_IR", cmd->payload.build.emit_ir ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeKeyValue(argv, "IR_PATH", cmd->payload.build.emit_ir_path);
        dal_c__pushSelfMakeToggle(argv, "EMIT_LINKED_ASM", cmd->payload.build.emit_linked_asm ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeKeyValue(argv, "LINKED_ASM_PATH", cmd->payload.build.emit_linked_asm_path);
        dal_c__pushSelfMakeToggle(argv, "EMIT_DISASM", cmd->payload.build.emit_disasm ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeKeyValue(argv, "DISASM_PATH", cmd->payload.build.emit_disasm_path);
        dal_c__pushSelfMakeToggle(argv, "EMIT_DEBUG_INFO", cmd->payload.build.emit_debug_info ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeKeyValue(argv, "DEBUG_INFO_PATH", cmd->payload.build.emit_debug_info_path);
        dal_c__pushSelfMakeToggle(argv, "PRINT_LINK_GC", cmd->payload.build.print_link_gc ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeToggle(argv, "ANALYSIS_ARTIFACTS", cmd->payload.build.analysis_artifacts ? dal_c_ToggleState_enabled : dal_c_ToggleState_auto);
        dal_c__pushSelfMakeToggle(argv, "DISASM_DEMANGLE", cmd->payload.build.disasm_demangle);
        dal_c__pushSelfMakeToggle(argv, "DISASM_SOURCE", cmd->payload.build.disasm_source);
        dal_c__pushSelfMakeToggle(argv, "DISASM_LINE_NUMBERS", cmd->payload.build.disasm_line_numbers);
        dal_c__pushSelfMakeToggle(argv, "DISASM_SYMBOLIZE_OPERANDS", cmd->payload.build.disasm_symbolize_operands);
        dal_c__pushSelfMakeToggle(argv, "DISASM_RAW_INSN", cmd->payload.build.disasm_raw_insn);
        dal_c__pushSelfMakeToggle(argv, "DISASM_SECTION_CONTENTS", cmd->payload.build.disasm_section_contents);
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
        dal_c__projectLockRelease(&lock);
        free(self_dir);
        return -1;
    }
    (void)fflush(stdout);
    (void)fflush(stderr);
    dal_c_CmdPhase phase = target ? dal_c_CmdPhase_clean : dal_c_CmdPhase_self_build;
    double self_started_at = dal_c__phaseNowSeconds();
    int result = proc_runMergedOutput(raw_argv, true);
    dal_c__phaseRecord(phase, dal_c__phaseNowSeconds() - self_started_at);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    dal_c__projectLockRelease(&lock);
    free(self_dir);
    return result;
}

bool dal_c__usesDHLibrary(const dal_c_Project* proj, const dal_c_CompilerOpts* opts) {
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
    return file_writeAtomic(path, content);
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

static bool dal_c__shouldPrintLibDHSkip(const dal_c_Project* dh_proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    static ArrStr* reported = NULL;
    if (!dh_proj || !profile) { return true; }

    char* key = str_format(
        "%s|%s|%s",
        dh_proj->root ? dh_proj->root : "",
        profile->name ? profile->name : "",
        dal_c_Target_format(target_type)
    );
    if (!key) { return true; }

    if (!reported) {
        reported = ArrStr_init();
        if (!reported) {
            free(key);
            return true;
        }
    }

    for (int i = 0; i < ArrStr_len(reported); ++i) {
        if (str_eql(ArrStr_at(reported, i), key)) {
            free(key);
            return false;
        }
    }

    ArrStr_push(reported, key);
    free(key);
    return true;
}


static bool dal_c__arrStrContains(const ArrStr* arr, const char* value) {
    assert(arr != NULL);
    assert(value != NULL);
    for (int i = 0; i < ArrStr_len(arr); ++i) {
        if (str_eql(ArrStr_at(arr, i), value)) { return true; }
    }
    return false;
}

static void dal_c__arrStrPushUnique(ArrStr* arr, const char* value) {
    assert(arr != NULL);
    if (!value || value[0] == '\0' || dal_c__arrStrContains(arr, value)) { return; }
    ArrStr_push(arr, value);
}

static bool dal_c__pathTextEquals(const char* lhs, const char* rhs) {
    if (!lhs || !rhs) { return false; }
    while (*lhs && *rhs) {
        char lhs_ch = *lhs == '\\' ? '/' : *lhs;
        char rhs_ch = *rhs == '\\' ? '/' : *rhs;
#ifdef _WIN32
        lhs_ch = (char)tolower((unsigned char)lhs_ch);
        rhs_ch = (char)tolower((unsigned char)rhs_ch);
#endif
        if (lhs_ch != rhs_ch) { return false; }
        lhs++;
        rhs++;
    }
    return *lhs == '\0' && *rhs == '\0';
}

static bool dal_c__arrStrContainsPath(const ArrStr* arr, const char* path) {
    assert(arr != NULL);
    assert(path != NULL);
    for (int i = 0; i < ArrStr_len(arr); ++i) {
        if (dal_c__pathTextEquals(ArrStr_at(arr, i), path)) { return true; }
    }
    return false;
}

static void dal_c__arrStrPushUniquePath(ArrStr* arr, const char* path) {
    assert(arr != NULL);
    if (!path || path[0] == '\0' || dal_c__arrStrContainsPath(arr, path)) { return; }
    ArrStr_push(arr, path);
}

static char* dal_c__sourceListCachePath(const dal_c_Project* proj, const char* dir, bool skip_source_paths) {
    if (!proj || !proj->root || !dir) { return NULL; }
    uint64_t h = 1469598103934665603ULL;
    h = dal_c__hashString(h, "source-list-v2");
    h = dal_c__hashString(h, dir);
    h = dal_c__hashBool(h, skip_source_paths);
    h = dal_c__hashString(h, proj->root);
    char* cache_base = dal_c__cacheBaseDir(proj);
    char* dh_c_cache = path_join(cache_base, dal_c_tool_name);
    char* source_cache = path_join(dh_c_cache, "source-lists");
    char* file_name = str_format("%016llx.list", (unsigned long long)h);
    char* path = path_join(source_cache, file_name);
    free(file_name);
    free(source_cache);
    free(dh_c_cache);
    free(cache_base);
    return path;
}

static bool dal_c__sourceListCacheLoad(const char* cache_path, const char* dir, bool skip_source_paths, ArrStr** out_files) {
    assert(out_files != NULL);
    *out_files = NULL;
    if (!cache_path || !path_isFile(cache_path)) { return false; }

    int line_count = 0;
    char** lines = file_readLines(cache_path, &line_count);
    if (!lines || line_count < 3) {
        if (lines) {
            for (int i = 0; i < line_count; ++i) { free(lines[i]); }
            free((void*)lines);
        }
        return false;
    }

    bool ok = str_eql(lines[0], "dal-c-source-list-v2");
    if (ok) {
        const char* root_prefix = "root\t";
        ok = str_startsWith(lines[1], root_prefix) && str_eql(lines[1] + strlen(root_prefix), dir);
    }
    if (ok) {
        const char* skip_prefix = "skip\t";
        ok = str_startsWith(lines[2], skip_prefix)
          && str_eql(lines[2] + strlen(skip_prefix), skip_source_paths ? "1" : "0");
    }

    ArrStr* files = ok ? ArrStr_init() : NULL;
    for (int i = 3; ok && i < line_count; ++i) {
        const char* line = lines[i];
        if (str_startsWith(line, "D\t")) {
            const char* mt_text = line + 2;
            const char* tab = strchr(mt_text, '\t');
            if (!tab) {
                ok = false;
                break;
            }
            char* end = NULL;
            long long expected = strtoll(mt_text, &end, 10);
            if (end != tab) {
                ok = false;
                break;
            }
            const char* path = tab + 1;
            time_t actual = file_mtime(path);
            if (actual == 0 || (long long)actual != expected || !path_isDir(path)) {
                ok = false;
                break;
            }
        } else if (str_startsWith(line, "F\t")) {
            const char* path = line + 2;
            if (!path_isFile(path)) {
                ok = false;
                break;
            }
            ArrStr_push(files, path);
        } else {
            ok = false;
            break;
        }
    }

    for (int i = 0; i < line_count; ++i) { free(lines[i]); }
    free((void*)lines);

    if (!ok) {
        ArrStr_fini(&files);
        return false;
    }
    *out_files = files;
    return true;
}

static bool dal_c__sourceListScanRecur(const char* root, const char* dir, bool skip_source_paths, ArrStr* dirs, ArrStr* files) {
    assert(root != NULL);
    assert(dir != NULL);
    assert(dirs != NULL);
    assert(files != NULL);
    if (!path_isDir(dir)) { return true; }

    dal_c__arrStrPushUnique(dirs, dir);
#ifdef _WIN32
    char* pattern = path_join(dir, "*");
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA(pattern, &data);
    free(pattern);
    if (hFind == INVALID_HANDLE_VALUE) { return true; }
    do {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0) { continue; }
        char* full = path_join(dir, data.cFileName);
        char* rel = path_relative(root, full);
        const char* candidate = rel ? rel : full;
        bool skip = skip_source_paths && dal_c__shouldSkipAutoSourcePath(candidate);
        if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            if (!skip && !dal_c__sourceListScanRecur(root, full, skip_source_paths, dirs, files)) {
                free(rel);
                free(full);
                FindClose(hFind);
                return false;
            }
        } else if (!skip && (str_endsWith(full, ".c") || str_endsWith(full, ".S") || str_endsWith(full, ".s"))) {
            ArrStr_push(files, full);
        }
        free(rel);
        free(full);
    } while (FindNextFileA(hFind, &data));
    FindClose(hFind);
#else
    DIR* dp = opendir(dir);
    if (!dp) { return true; }
    struct dirent* ent = NULL;
    while ((ent = readdir(dp)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) { continue; }
        char* full = path_join(dir, ent->d_name);
        char* rel = path_relative(root, full);
        const char* candidate = rel ? rel : full;
        bool skip = skip_source_paths && dal_c__shouldSkipAutoSourcePath(candidate);
        if (path_isDir(full)) {
            if (!skip && !dal_c__sourceListScanRecur(root, full, skip_source_paths, dirs, files)) {
                free(rel);
                free(full);
                (void)closedir(dp);
                return false;
            }
        } else if (!skip && (str_endsWith(full, ".c") || str_endsWith(full, ".S") || str_endsWith(full, ".s"))) {
            ArrStr_push(files, full);
        }
        free(rel);
        free(full);
    }
    (void)closedir(dp);
#endif
    return true;
}

static bool dal_c__sourceListCacheStore(const char* cache_path, const char* dir, bool skip_source_paths, const ArrStr* dirs, const ArrStr* files) {
    if (!cache_path || !dir || !dirs || !files) { return false; }
    if (dal_c__read_only_planning) { return true; }
    char* parent = path_parent(cache_path);
    bool parent_ready = parent && dir_createRecur(parent);
    free(parent);
    if (!parent_ready) { return false; }
    char* tmp = dal_c__makeTempPath(cache_path);
    FILE* fp = tmp ? fopen(tmp, "wb") : NULL;
    if (!fp) {
        free(tmp);
        return false;
    }
    (void)fprintf(fp, "dal-c-source-list-v2\n");
    (void)fprintf(fp, "root\t%s\n", dir);
    (void)fprintf(fp, "skip\t%d\n", skip_source_paths ? 1 : 0);
    for (int i = 0; i < ArrStr_len(dirs); ++i) {
        const char* path = ArrStr_at(dirs, i);
        (void)fprintf(fp, "D\t%lld\t%s\n", (long long)file_mtime(path), path);
    }
    for (int i = 0; i < ArrStr_len(files); ++i) {
        (void)fprintf(fp, "F\t%s\n", ArrStr_at(files, i));
    }
    bool ok = fclose(fp) == 0;
    if (ok) {
        char* content = file_read(tmp);
        ok = content != NULL && dal_c__writeFileIfChanged(cache_path, content);
        free(content);
    }
    (void)remove(tmp);
    free(tmp);
    return ok;
}

static ArrStr* dal_c__collectBuildFilesCached(const dal_c_Project* proj, const char* dir, bool skip_source_paths) {
    assert(dir != NULL);
    if (!path_isDir(dir)) { return ArrStr_init(); }

    char* cache_path = dal_c__sourceListCachePath(proj, dir, skip_source_paths);
    ArrStr* cached = NULL;
    if (cache_path && dal_c__sourceListCacheLoad(cache_path, dir, skip_source_paths, &cached)) {
        free(cache_path);
        return cached;
    }

    ArrStr* dirs = ArrStr_init();
    ArrStr* files = ArrStr_init();
    if (!dal_c__sourceListScanRecur(dir, dir, skip_source_paths, dirs, files)) {
        ArrStr_fini(&dirs);
        ArrStr_fini(&files);
        free(cache_path);
        return dal_c__collectBuildFiles(dir, skip_source_paths);
    }
    if (cache_path) {
        (void)dal_c__sourceListCacheStore(cache_path, dir, skip_source_paths, dirs, files);
    }
    ArrStr_fini(&dirs);
    free(cache_path);
    return files;
}

ArrStr* dal_c__collectPathSourcesCached(const dal_c_Project* proj, const char* path, bool resolved_is_dir, bool skip_auto_paths) {
    assert(path != NULL);
    if (!resolved_is_dir) {
        ArrStr* files = ArrStr_init();
        ArrStr_push(files, path);
        return files;
    }
    return dal_c__collectBuildFilesCached(proj, path, skip_auto_paths);
}

// === SOURCE COLLECTION (Core Layer) ===

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_path) {
    ArrStr* sources = ArrStr_init();
    if (target_path) {
        ArrStr_push(sources, target_path);
    } else {
        assert(proj != NULL && proj->root != NULL);
        for (int i = 0; i < proj->self_root_count; ++i) {
            ArrStr* project_sources = dal_c__collectBuildFilesCached(proj, proj->self_roots[i].path, true);
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
    ArrStr* collected = dal_c__collectBuildFilesCached(proj, dir, true);
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
    bool project_library = proj && proj->defaults.target_kind_set
                        && proj->defaults.target_kind == dal_c_Target_lib;
    bool library_build = cmd->action == dal_c_CmdAction_lib
                      || (cmd->action == dal_c_CmdAction_build
                          && (cmd->payload.build.as_library || project_library));
    if (!library_build) { return false; }

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
    dal_c_Cmd dependency_cmd = *cmd;
    dependency_cmd.output_ext = NULL;
    cmd = &dependency_cmd;

    if (!lib->path) {
        (void)fprintf(stderr, "Error: Library %s has no path specified\n", lib->name);
        return 1;
    }

    if (cmd->verbose) {
        printf("Building library: %s\n", lib->name);
    }

    dal_c_Project* lib_proj = dal_c_Project_detectAt(lib->path, proj->dh_path);
    if (lib->test_enabled && !lib_proj) {
        (void)fprintf(stderr, "Error: Dependency %s enables test=on but has no project.dh\n", lib->name);
        return 1;
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
        dal_c_CompilerOpts_merge(&merged.opts, &lib_proj->workspace_opts);
        dal_c_CompilerOpts_merge(&merged.opts, &lib_proj->opts);
    }
    if (proj->workspace_opts.prebuilt_mode_set) {
        merged.opts.prebuilt_mode = proj->workspace_opts.prebuilt_mode;
        merged.opts.prebuilt_mode_set = true;
    }
    if (proj->opts.prebuilt_mode_set) {
        merged.opts.prebuilt_mode = proj->opts.prebuilt_mode;
        merged.opts.prebuilt_mode_set = true;
    }
    dal_c_CompilerOpts_merge(&merged.opts, &cmd->opts);
    dal_c_PrebuiltMode source_prebuilt_mode = dal_c_PrebuiltMode_auto;
    bool source_prebuilt_mode_set = false;
    dal_c__resolveDependencySourcePrebuiltPolicy(
        lib_proj, cmd, &source_prebuilt_mode, &source_prebuilt_mode_set
    );
    dal_c_CompilerOpts_merge(&merged.opts, &lib->opts);
    if (merged.opts.profile == dal_c_Profile_invalid) {
        merged.opts.profile = lib_profile_enum;
    }
    lib_profile_enum = merged.opts.profile;
    const dal_c_ProfileSpec* lib_profile = dal_c_ProfileSpec_by(lib_profile_enum);
    assert(lib_profile != NULL);
    dal_c_Target lib_target_type = lib->is_static ? dal_c_Target_static_lib : dal_c_Target_shared_lib;
    char* lib_abs_path = lib_proj ? lib_proj->root : lib->path;
    bool should_run_dependency_tests = lib_proj
                                    && !dal_c__cmdAggregatesRecursiveTests(cmd)
                                    && (!lib->test_enabled_set || lib->test_enabled)
                                    && (lib->test_enabled
                                        || (cmd->action == dal_c_CmdAction_test && cmd->payload.test.recursive));
    int result = 0;

    // 1. Prefer packaged prebuilt artifacts when allowed. `prebuilt/` is separate
    // from `build/` so ordinary local caches never hide source changes.
    bool is_windows = dal_c__platformIsWindows();
    bool lto_enabled = lib->is_static
                    && dal_c_LtoMode_isEnabled(dal_c__resolvedLtoState(&merged.opts, lib_profile));
    char* prebuilt_profile_dir = dal_c__makePrebuiltProfileDir(lib_abs_path, &merged.opts, lib_profile->name);
    char* prebuilt_artifact = prebuilt_profile_dir
                                ? dal_c__resolvePrebuiltArtifactPath(
                                      prebuilt_profile_dir, lib->name, lib_target_type, is_windows, lto_enabled
                                  )
                                : NULL;
    bool may_use_prebuilt = merged.opts.prebuilt_mode != dal_c_PrebuiltMode_off && !should_run_dependency_tests;
    char* prebuilt_manifest_reason = NULL;
    bool prebuilt_manifest_ok = !prebuilt_artifact || dal_c__prebuiltManifestCompatible(prebuilt_profile_dir, &merged.opts, lib_profile, lib_target_type, lto_enabled, prebuilt_artifact, &prebuilt_manifest_reason);
    if (may_use_prebuilt && prebuilt_artifact && prebuilt_manifest_ok) {
        if (cmd->show_progress) {
            (void)printf("[PREBUILT] %s %s\n", lib->name, lib_profile->name);
            (void)fflush(stdout);
        }
        bool copied = dal_c__copyLibraryArtifacts(
            proj, lib_proj, lib, lib_abs_path, prebuilt_profile_dir,
            &merged, lib_profile, lib_target_type, is_windows, true
        );
        free(prebuilt_manifest_reason);
        free(prebuilt_artifact);
        free(prebuilt_profile_dir);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        dal_c_Project_cleanup(&lib_proj);
        return copied ? 0 : 1;
    }
    if (prebuilt_artifact && !prebuilt_manifest_ok && cmd->show_progress) {
        (void)fprintf(stderr, "[PREBUILT-SKIP] %s: %s\n", lib->name, prebuilt_manifest_reason ? prebuilt_manifest_reason : "manifest incompatible");
    }
    if (merged.opts.prebuilt_mode == dal_c_PrebuiltMode_required) {
        if (should_run_dependency_tests) {
            (void)fprintf(stderr, "Error: Dependency %s requires source because its tests were requested\n", lib->name);
        } else if (prebuilt_artifact && !prebuilt_manifest_ok) {
            (void)fprintf(stderr, "Error: Required prebuilt dependency %s is incompatible: %s\n", lib->name, prebuilt_manifest_reason ? prebuilt_manifest_reason : "invalid manifest.dh");
        } else {
            (void)fprintf(stderr, "Error: Required prebuilt dependency %s was not found under %s\n", lib->name, prebuilt_profile_dir ? prebuilt_profile_dir : "prebuilt/<target>/<profile>");
        }
        free(prebuilt_manifest_reason);
        free(prebuilt_artifact);
        free(prebuilt_profile_dir);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }
    free(prebuilt_manifest_reason);
    free(prebuilt_artifact);
    free(prebuilt_profile_dir);

    if ((lib->source && lib->source[0]) || (lib->archive && lib->archive[0])) {
        char* lock_reason = NULL;
        if (!dal_c_Project_dependencySourceMatchesLock(proj, lib, &lock_reason)) {
            (void)fprintf(stderr, "Error: Cannot source-build dependency %s: %s\n", lib->name, lock_reason ? lock_reason : "dependency lock mismatch");
            free(lock_reason);
            dal_c_CompilerOpts_cleanup(&merged.opts);
            dal_c_Project_cleanup(&lib_proj);
            return 1;
        }
        free(lock_reason);
    }

    // 2. Source mode recursively builds the dependency graph.
    if (lib_proj && lib_proj->lib_count > 0) {
        if (cmd->verbose) {
            printf("Building %d dependencies for %s...\n", lib_proj->lib_count, lib->name);
        }
        for (int i = 0; i < lib_proj->lib_count; ++i) {
            const dal_c_Lib* child = &lib_proj->libraries[i];
            const char* provider = child->provider && child->provider[0] ? child->provider : "dh";
            if (!str_eql(provider, "dh")) { continue; }
            if (dal_c__buildSingleLibrary(cmd, lib_proj, child) != 0) {
                dal_c_CompilerOpts_cleanup(&merged.opts);
                dal_c_Project_cleanup(&lib_proj);
                return 1;
            }
        }
    }

    // 3. Handle header-only libraries
    ArrStr* lib_sources = dal_c__collectLibrarySources(lib, lib_proj);
    bool is_single_header = str_endsWith(lib->path, ".h") && path_isFile(lib->path);
    const char* lib_header_root = lib_proj ? lib_proj->root : lib->path;
    const char* include_dir_name = lib_proj ? dal_c_Project_getCategoryDirName(lib_proj, dal_c_dir_include) : dal_c_dir_include;
    char* lib_inc = path_join(lib_header_root, include_dir_name);
    bool has_include_dir = path_isDir(lib_inc);
    bool is_header_only = ArrStr_len(lib_sources) == 0 && (is_single_header || has_include_dir);
    if (is_header_only) {
        double dependency_started_at = dal_c__phaseNowSeconds();
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
            dal_c__phaseRecord(dal_c_CmdPhase_dependency_build, dal_c__phaseNowSeconds() - dependency_started_at);
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
        if (should_run_dependency_tests && (lib->test_enabled || dal_c__projectHasTestSources(lib_proj))) {
            dal_c__phaseRecord(dal_c_CmdPhase_dependency_build, dal_c__phaseNowSeconds() - dependency_started_at);
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
        dal_c__phaseRecord(dal_c_CmdPhase_dependency_build, dal_c__phaseNowSeconds() - dependency_started_at);
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

    // 5. Build in library's own build/<profile>/ directory
    dal_c_ProjectLock lib_lock = { 0 };
    if (!dal_c__projectLockAcquireAt(lib_abs_path, &lib_lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire library build lock: %s\n", lib->name);
        ArrStr_fini(&lib_sources);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }
    char* lib_build_profile = dal_c__makeBuildProfileDir(lib_proj, &merged.opts, lib_profile);
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

    char* lib_object_dir = path_join(lib_build_profile, "obj");
    dir_createRecur(lib_object_dir);
    char* saved_cwd = env_getCWD();
    if (!saved_cwd || !env_setCWD(build_proj.root)) {
        (void)fprintf(stderr, "Error: Failed to switch to dependency root: %s\n", build_proj.root);
        free(saved_cwd);
        ArrStr_fini(&lib_sources);
        free(lib_object_dir);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        free(lib_build_profile);
        dal_c__projectLockRelease(&lib_lock);
        dal_c_Project_cleanup(&lib_proj);
        return 1;
    }

    double dependency_started_at = dal_c__phaseNowSeconds();
    dal_c_LtoMode static_lto_mode = dal_c__resolvedLtoState(&merged.opts, lib_profile);
    int artifact_count = lib->is_static && dal_c_LtoMode_isEnabled(static_lto_mode) ? 2 : 1;
    for (int artifact_idx = 0; artifact_idx < artifact_count && result == 0; ++artifact_idx) {
        bool lto_artifact = lib->is_static && artifact_idx == 1;
        dal_c_Cmd artifact_cmd = merged;
        artifact_cmd.opts.prebuilt_mode = source_prebuilt_mode;
        artifact_cmd.opts.prebuilt_mode_set = source_prebuilt_mode_set;
        if (lib->is_static) {
            artifact_cmd.opts.lto_mode = lto_artifact ? static_lto_mode : dal_c_LtoMode_off;
        }

        char* lib_target_path = dal_c__resolveOutputPath(
            &build_proj, &artifact_cmd, lib_build_profile, lib->name, lib_target_type
        );
        if (lto_artifact) {
            char* qualified = dal_c__makeLtoStaticLibraryPath(lib_target_path);
            free(lib_target_path);
            lib_target_path = qualified;
        }
        char* lib_makefile_path = dal_c__makePlanFilePath(
            &build_proj, lib_profile, &artifact_cmd, lib_target_path, lib_target_type
        );
        int plan_result = dal_c__generateMakefile(
            &artifact_cmd, &build_proj, lib_profile, lib_sources,
            lib_target_path, lib_object_dir, lib_target_type
        );
        if (plan_result != dal_c_generateMakefile_success && plan_result != dal_c_generateMakefile_upToDate) {
            (void)fprintf(stderr, "Error: Failed to generate Makefile for library: %s\n", lib->name);
            result = 1;
        } else if (plan_result == dal_c_generateMakefile_upToDate) {
            if (cmd->show_progress) {
                (void)printf(
                    "[SKIP] %s%s is up to date\n",
                    lib->name ? lib->name : "dependency",
                    lto_artifact ? " LTO" : ""
                );
                (void)fflush(stdout);
            }
        } else {
            if (cmd->show_progress) {
                (void)printf(
                    "[DEP] %s%s\n",
                    lib->name ? lib->name : "dependency",
                    lto_artifact ? " LTO" : ""
                );
                (void)fflush(stdout);
            }
            result = dal_c__executeMake(cmd, lib_makefile_path);
        }
        free(lib_makefile_path);
        free(lib_target_path);
    }
    if (!env_setCWD(saved_cwd)) {
        (void)fprintf(stderr, "Error: Failed to restore working directory: %s\n", saved_cwd);
        result = 1;
    }
    free(saved_cwd);
    free(lib_object_dir);
    ArrStr_fini(&lib_sources);
    if (result != 0) {
        (void)fprintf(stderr, "Error: Failed to build library: %s\n", lib->name);
        dal_c__phaseRecord(dal_c_CmdPhase_dependency_build, dal_c__phaseNowSeconds() - dependency_started_at);
        dal_c_CompilerOpts_cleanup(&merged.opts);
        free(lib_build_profile);
        dal_c__projectLockRelease(&lib_lock);
        dal_c_Project_cleanup(&lib_proj);
        return result;
    }

    // 6. Copy artifacts (headers, libs, PCH) to consumer's lib/deps/
    if (!dal_c__copyLibraryArtifacts(proj, lib_proj, lib, lib_abs_path, lib_build_profile, &merged, lib_profile, lib_target_type, is_windows, false)) {
        (void)fprintf(stderr, "Warning: Failed to copy some artifacts for %s\n", lib->name);
    }
    dal_c__phaseRecord(dal_c_CmdPhase_dependency_build, dal_c__phaseNowSeconds() - dependency_started_at);

    if (should_run_dependency_tests && (lib->test_enabled || dal_c__projectHasTestSources(lib_proj))) {
        result = dal_c__runDependencyTests(cmd, lib_proj, &merged.opts);
        if (result != 0) {
            dal_c_CompilerOpts_cleanup(&merged.opts);
            free(lib_build_profile);
            dal_c__projectLockRelease(&lib_lock);
            dal_c_Project_cleanup(&lib_proj);
            return result;
        }
    }

    dal_c_CompilerOpts_cleanup(&merged.opts);
    free(lib_build_profile);
    dal_c__projectLockRelease(&lib_lock);
    dal_c_Project_cleanup(&lib_proj);
    return 0;
}

void dal_c__resolveDependencySourcePrebuiltPolicy(
    const dal_c_Project* dependency,
    const dal_c_Cmd* cmd,
    dal_c_PrebuiltMode* mode,
    bool* mode_set
) {
    assert(mode != NULL);
    assert(mode_set != NULL);

    *mode = dal_c_PrebuiltMode_auto;
    *mode_set = false;
    if (dependency && dependency->workspace_opts.prebuilt_mode_set) {
        *mode = dependency->workspace_opts.prebuilt_mode;
        *mode_set = true;
    }
    if (dependency && dependency->opts.prebuilt_mode_set) {
        *mode = dependency->opts.prebuilt_mode;
        *mode_set = true;
    }
    if (cmd && cmd->opts.prebuilt_mode_set) {
        *mode = cmd->opts.prebuilt_mode;
        *mode_set = true;
    }
}

static bool dal_c__projectHasTestSources(const dal_c_Project* proj) {
    assert(proj != NULL);

    ArrStr* tests = dal_c__collectDirectoryFiles(proj, dal_c_dir_tests);
    bool has_tests = ArrStr_len(tests) > 0;
    ArrStr_fini(&tests);
    return has_tests;
}

static bool dal_c__cmdAggregatesRecursiveTests(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    if (cmd->action != dal_c_CmdAction_test) {
        return false;
    }
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    return cmd->input_count == 0
        && !intent.target_path
        && (intent.recursive || intent.dsl_first);
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
    char* build_dir = dal_c__makeBuildProfileDir(proj, &cmd->opts, profile);
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
    (void)fflush(stdout);
    (void)fflush(stderr);
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
    char* build_dir = dal_c__makeBuildProfileDir(proj, &cmd->opts, profile);
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
    ArrStr_push(argv, dal_c__externalToolPath(dal_c_ExternalTool_debugger));
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
    (void)fflush(stdout);
    (void)fflush(stderr);
    int result = proc_run(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    if (exe_name_alloc) { free(exe_name_alloc); }
    free(exe_path);
    return result;
}

void dal_c__inheritDependencyToolchainOpts(
    dal_c_CompilerOpts* dst,
    const dal_c_CompilerOpts* src
) {
    assert(dst != NULL);
    if (!src) { return; }

    dal_c_CompilerOpts toolchain = {
        .compiler = src->compiler,
        .arch_target = src->arch_target,
        .target_arch = src->target_arch,
        .target_tune = src->target_tune,
        .target_abi = src->target_abi,
        .sysroot = src->sysroot,
        .profile = src->profile,
        .lto_mode = src->lto_mode,
        .prebuilt_mode = src->prebuilt_mode,
        .prebuilt_mode_set = src->prebuilt_mode_set,
    };
    dal_c_CompilerOpts_merge(dst, &toolchain);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c__ensureLibDH(const dal_c_Cmd* parent_cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const dal_c_CompilerOpts* parent_opts, dal_c_Target consumer_target_type) {
    if (!proj || !proj->dh_path) { return 0; }
    dal_c_Project* dh_proj = dal_c_Project_detectAt(proj->dh_path, NULL);
    if (!dh_proj) {
        (void)fprintf(stderr, "Error: Failed to detect DH project at %s\n", proj->dh_path);
        return 1;
    }
    dal_c_ProjectLock lock = { 0 };
    if (!dal_c__projectLockAcquire(dh_proj, &lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire DH build lock\n");
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }
    char* build_dir = dal_c_Project_getBuildDir(dh_proj);
    dal_c_CompilerOpts target_opts = *parent_opts;
    char* profile_dir = dal_c__makeBuildProfileDir(dh_proj, &target_opts, profile);
    char* object_dir = path_join(profile_dir, "obj");
    dir_createRecur(object_dir);
    char* saved_cwd = env_getCWD();
    if (!saved_cwd || !env_setCWD(dh_proj->root)) {
        (void)fprintf(stderr, "Error: Failed to switch to DH root: %s\n", dh_proj->root);
        free(saved_cwd);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c__projectLockRelease(&lock);
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }

    dal_c_Target dh_target_type = consumer_target_type == dal_c_Target_shared_lib
                                    ? dal_c_Target_shared_lib
                                    : dal_c_Target_static_lib;

    dal_c_Cmd cmd = { 0 };
    cmd.action = dal_c_CmdAction_lib;
    cmd.payload.lib.linking = dh_target_type == dal_c_Target_shared_lib
                                ? dal_c_Linking_shared
                                : dal_c_Linking_static;
    if (parent_cmd) {
        cmd.show_commands = parent_cmd->show_commands;
        cmd.show_progress = parent_cmd->show_progress;
        cmd.verbose = parent_cmd->verbose;
        cmd.make_jobs = parent_cmd->make_jobs;
    }
    cmd.opts.profile = dal_c_Profile_invalid;
    dal_c_CompilerOpts_merge(&cmd.opts, &dh_proj->opts);
    dal_c__inheritDependencyToolchainOpts(&cmd.opts, parent_opts);
    cmd.opts.dsl_mode = dal_c_ToggleState_disabled;
    if (cmd.opts.profile == dal_c_Profile_invalid) {
        cmd.opts.profile = dal_c_Profile_parse(profile->name);
    }

    bool is_windows = dal_c__platformIsWindows();
    bool lto_enabled = dh_target_type == dal_c_Target_static_lib
                    && dal_c_LtoMode_isEnabled(dal_c__effectiveLtoState(&cmd, profile, dh_target_type));
    char* prebuilt_profile_dir = dal_c__makePrebuiltProfileDir(dh_proj->root, &cmd.opts, profile->name);
    char* prebuilt_artifact = NULL;
    char* dh_manifest_reason = NULL;
    bool selected_lto = false;
    bool dh_manifest_ok = prebuilt_profile_dir && dal_c__resolveCompatiblePrebuiltArtifact(prebuilt_profile_dir, "dh", dh_target_type, is_windows, lto_enabled, &cmd.opts, profile, &prebuilt_artifact, &selected_lto, &dh_manifest_reason);
    if (cmd.opts.prebuilt_mode != dal_c_PrebuiltMode_off && prebuilt_artifact && dh_manifest_ok) {
        char* output_libs_dir = path_join(profile_dir, "libs");
        dir_createRecur(output_libs_dir);
        char* artifact_name = path_basename(prebuilt_artifact);
        char* artifact_dst = path_join(output_libs_dir, artifact_name);
        bool copied = file_copy(prebuilt_artifact, artifact_dst);
        if (copied && dh_target_type == dal_c_Target_static_lib && !selected_lto) {
            char* stale_lto = dal_c__makeLtoStaticLibraryPath(artifact_dst);
            if (path_isFile(stale_lto) && remove(stale_lto) != 0) {
                copied = false;
            }
            free(stale_lto);
        }
        if (copied && is_windows && dh_target_type == dal_c_Target_shared_lib) {
            char* import_src = dal_c__makeSharedImportLibraryPath(prebuilt_artifact);
            char* import_name = path_basename(import_src);
            char* import_dst = path_join(output_libs_dir, import_name);
            copied = path_isFile(import_src) && file_copy(import_src, import_dst);
            free(import_dst);
            free(import_name);
            free(import_src);
        }
        if (copied && cmd.show_progress) {
            (void)printf(
                "[PREBUILT] dh %s %s%s\n",
                profile->name,
                dal_c_Target_format(dh_target_type),
                lto_enabled && !selected_lto ? " (native fallback)" : ""
            );
            (void)fflush(stdout);
        }
        free(artifact_dst);
        free(artifact_name);
        free(output_libs_dir);
        free(dh_manifest_reason);
        free(prebuilt_artifact);
        free(prebuilt_profile_dir);
        if (!env_setCWD(saved_cwd)) {
            (void)fprintf(stderr, "Error: Failed to restore working directory: %s\n", saved_cwd);
            copied = false;
        }
        free(saved_cwd);
        dal_c_CompilerOpts_cleanup(&cmd.opts);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c__projectLockRelease(&lock);
        dal_c_Project_cleanup(&dh_proj);
        return copied ? 0 : 1;
    }
    if (!dh_manifest_ok && dh_manifest_reason && cmd.show_progress) {
        (void)fprintf(stderr, "[PREBUILT-SKIP] dh: %s\n", dh_manifest_reason ? dh_manifest_reason : "manifest incompatible");
    }
    if (cmd.opts.prebuilt_mode == dal_c_PrebuiltMode_required) {
        if (dh_manifest_reason) {
            (void)fprintf(stderr, "Error: Required prebuilt dh artifact is incompatible: %s\n", dh_manifest_reason ? dh_manifest_reason : "invalid manifest.dh");
        } else {
            (void)fprintf(stderr, "Error: Required prebuilt dh artifact was not found under %s\n", prebuilt_profile_dir ? prebuilt_profile_dir : "prebuilt/<target>/<profile>");
        }
        free(dh_manifest_reason);
        free(prebuilt_artifact);
        free(prebuilt_profile_dir);
        (void)env_setCWD(saved_cwd);
        free(saved_cwd);
        dal_c_CompilerOpts_cleanup(&cmd.opts);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c__projectLockRelease(&lock);
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }
    free(dh_manifest_reason);
    free(prebuilt_artifact);
    free(prebuilt_profile_dir);

    ArrStr* sources = dal_c__collectSourceFiles(dh_proj, NULL);
    if (ArrStr_len(sources) == 0) {
        (void)fprintf(stderr, "Error: No source files found for libdh\n");
        ArrStr_fini(&sources);
        (void)env_setCWD(saved_cwd);
        free(saved_cwd);
        dal_c_CompilerOpts_cleanup(&cmd.opts);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c__projectLockRelease(&lock);
        dal_c_Project_cleanup(&dh_proj);
        return 1;
    }

    char* lib_target_path = dal_c__resolveOutputPath(dh_proj, &cmd, profile_dir, "dh", dh_target_type);
    if (dh_target_type == dal_c_Target_static_lib
        && dal_c_LtoMode_isEnabled(dal_c__effectiveLtoState(&cmd, profile, dh_target_type))) {
        char* qualified = dal_c__makeLtoStaticLibraryPath(lib_target_path);
        free(lib_target_path);
        lib_target_path = qualified;
    }
    char* makefile_path = dal_c__makePlanFilePath(dh_proj, profile, &cmd, lib_target_path, dh_target_type);
    double dh_started_at = dal_c__phaseNowSeconds();
    int result = dal_c__generateMakefile(&cmd, dh_proj, profile, sources, lib_target_path, object_dir, dh_target_type);
    if (result == dal_c_generateMakefile_success) {
        if (cmd.show_progress) {
            (void)printf("[DEP] dh %s\n", dal_c_Target_format(dh_target_type));
            (void)fflush(stdout);
        }
        result = dal_c__executeMake(&cmd, makefile_path);
    } else if (result == dal_c_generateMakefile_upToDate) {
        if (cmd.show_progress && dal_c__shouldPrintLibDHSkip(dh_proj, profile, dh_target_type)) {
            (void)printf("[SKIP] dh %s is up to date\n", dal_c_Target_format(dh_target_type));
            (void)fflush(stdout);
        }
        result = 0;
    }
    dal_c__phaseRecord(dal_c_CmdPhase_dh_build, dal_c__phaseNowSeconds() - dh_started_at);
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
    dal_c__projectLockRelease(&lock);
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
    return dal_c__ensureLibDH(cmd, &dh_proj, profile, &cmd->opts, dal_c_Target_static_lib);
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

    dal_c_ProjectLock lock = { 0 };
    if (!dal_c__projectLockAcquireAt(proj->dh_path, &lock)) {
        (void)fprintf(stderr, "Error: Failed to acquire DH clean lock\n");
        return 1;
    }

    bool cleaned = false;
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* dh_build = path_join(proj->dh_path, dal_c_dir_build);
    char* dh_build_target = NULL;
    if (cmd->profile_explicit && profile) {
        dh_build_target = dal_c__makeBuildProfileDirAt(proj->dh_path, &cmd->opts, profile);
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
    dal_c__projectLockRelease(&lock);
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
    if (!cmd->dry_run && dal_c__usesDHLibrary(proj, &cmd->opts)) {
        if (dal_c__ensureLibDH(cmd, proj, profile, &cmd->opts, target_type) != 0) {
            (void)fprintf(stderr, "Error: Failed to build libdh\n");
            return 1;
        }
        if (!dal_c__writeDepsPreludeHeader(proj, &cmd->opts)) {
            (void)fprintf(stderr, "Error: Failed to generate dependency prelude header\n");
            return 1;
        }
    }
    if (!dal_c__validateLinkToolchain(&cmd->opts, target_type)) {
        return 1;
    }
    if (!dal_c__validateBuildArtifacts(cmd, profile, sources, target_type)) {
        return 1;
    }
    dal_c__warnIfProfileLtoDisabledByToolchain(cmd, profile, target_type);

    char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, target_type);
    char* makefile_dir = path_parent(makefile_path);
    if (!cmd->dry_run) { dir_createRecur(makefile_dir); }
    char* makefile_tmp = NULL;
    char* link_contract_path = NULL;
    if (target_type == dal_c_Target_executable || target_type == dal_c_Target_shared_lib || target_type == dal_c_Target_image) {
        link_contract_path = dal_c__makeLinkContractPath(build_dir, target_path);
        if (!dal_c__writeLinkContractFile(link_contract_path, target_path, cmd, profile, target_type)) {
            (void)fprintf(stderr, "Error: Failed to write link contract: %s\n", link_contract_path);
            free(link_contract_path);
            free(makefile_dir);
            free(makefile_path);
            return 1;
        }
    }

    bool has_pch = dal_c__pchEnabledForOpts(proj, &cmd->opts);
    const char* obj_base = (proj && proj->root) ? proj->root : NULL;
    if (!dal_c__targetIsEmitOnly(target_type)
        && dal_c__linkedPlanIsUpToDate(cmd, proj, profile, sources, has_pch, build_dir, obj_base, target_type, target_path, link_contract_path)) {
        free(link_contract_path);
        free(makefile_dir);
        free(makefile_path);
        return dal_c_generateMakefile_upToDate;
    }
    if (cmd->dry_run) {
        free(link_contract_path);
        free(makefile_dir);
        free(makefile_path);
        return dal_c_generateMakefile_success;
    }

    makefile_tmp = dal_c__makeTempPath(makefile_path);
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

    dal_c__writeMakefilePrelude(fp, cmd);
    dal_c__writeMakefileVariables(fp, cmd, profile, proj, build_dir, target_type, target_path, link_contract_path);

    // Declare default goal before any rules to ensure 'all' is the default target
    (void)fprintf(fp, ".DEFAULT_GOAL := all\n\n");

    dal_c__writeMakefilePCH(fp, cmd, proj, profile, build_dir, target_type);

    if (dal_c__targetIsEmitOnly(target_type)) {
        return dal_c__writeEmitOnlyMakefile(
            fp, cmd, proj, sources, target_path, target_type,
            makefile_tmp, makefile_path, makefile_dir, link_contract_path
        );
    }

    return dal_c__writeLinkedMakefile(
        fp, cmd, proj, profile, sources, target_path, build_dir, target_type,
        makefile_tmp, makefile_path, makefile_dir, link_contract_path
    );
}

int dal_c__executeMake(const dal_c_Cmd* cmd, const char* makefile_path) {
    assert(cmd != NULL);
    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c__externalToolPath(dal_c_ExternalTool_make));
    ArrStr_push(argv, "-rR");
    ArrStr_push(argv, "--no-print-directory");

    char* j_flag = dal_c__buildParallelFlag(cmd);
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
    (void)fflush(stdout);
    (void)fflush(stderr);
    int result = proc_runMergedOutput(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    return result;
}

int dal_c__executeMakeInDir(const dal_c_Cmd* cmd, const char* directory) {
    assert(cmd != NULL);
    assert(directory != NULL);

    ArrStr* argv = ArrStr_init();
    ArrStr_push(argv, dal_c__externalToolPath(dal_c_ExternalTool_make));
    ArrStr_push(argv, "-rR");
    ArrStr_push(argv, "--no-print-directory");

    char* j_flag = dal_c__buildParallelFlag(cmd);
    ArrStr_push(argv, j_flag);
    free(j_flag);

    ArrStr_push(argv, "-C");
    ArrStr_push(argv, directory);

    const char** raw_argv = dal_c__makeConstArgvView(argv);
    if (!raw_argv) {
        ArrStr_fini(&argv);
        return -1;
    }
    (void)fflush(stdout);
    (void)fflush(stderr);
    int result = proc_runMergedOutput(raw_argv, true);
    free((void*)raw_argv);
    ArrStr_fini(&argv);
    return result;
}

void dal_c__printVerbose(const dal_c_Cmd* cmd, const char* fmt, ...) {
    assert(cmd != NULL);
    assert(fmt != NULL);

    if (!cmd->verbose) { return; }
    va_list args dsl_$undefined;
    va_start(args, fmt);
    (void)vprintf(fmt, args);
    va_end(args);
}

void dal_c__printError(const char* fmt, ...) {
    assert(fmt != NULL);

    va_list args dsl_$undefined;
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

static bool dal_c__pathEndsWithSeparator(const char* path) {
    if (!path || path[0] == '\0') { return false; }
    size_t len = strlen(path);
    char last = path[len - 1];
    return last == '/' || last == '\\';
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

static char* dal_c__makeTargetBaseFileName(const char* name, dal_c_Target type, bool is_windows) {
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

static char* dal_c__withOutputExtension(const char* name, const char* output_ext) {
    assert(name != NULL);
    if (!output_ext || output_ext[0] == '\0') { return strdup(name); }
    char* ext = output_ext[0] == '.' ? strdup(output_ext) : str_format(".%s", output_ext);
    if (!ext) { return NULL; }
    char* stem = strdup(name);
    if (!stem) {
        free(ext);
        return NULL;
    }
    char* dot = strrchr(stem, '.');
    if (dot && dot != stem) { *dot = '\0'; }
    char* result = str_format("%s%s", stem, ext);
    free(stem);
    free(ext);
    return result;
}

static char* dal_c__makeTargetFileName(const char* name, dal_c_Target type, bool is_windows, const char* output_ext) {
    assert(name != NULL);
    if (!dal_c__pathHasSeparator(name)) {
        char* base = dal_c__makeTargetBaseFileName(name, type, is_windows);
        char* result = output_ext ? dal_c__withOutputExtension(base, output_ext) : strdup(base);
        free(base);
        return result;
    }

    char* parent = path_parent(name);
    char* basename = path_basename(name);
    char* file_name = basename ? dal_c__makeTargetBaseFileName(basename, type, is_windows) : NULL;
    if (file_name && output_ext) {
        char* with_ext = dal_c__withOutputExtension(file_name, output_ext);
        free(file_name);
        file_name = with_ext;
    }
    char* result = NULL;
    if (parent && parent[0] != '\0' && !str_eql(parent, ".")) {
        result = file_name ? path_join(parent, file_name) : NULL;
    } else {
        result = file_name ? strdup(file_name) : NULL;
    }
    free(file_name);
    free(basename);
    free(parent);
    return result ? result : strdup(name);
}

char* dal_c__makeLtoStaticLibraryPath(const char* native_path) {
    assert(native_path != NULL);
    const char* dot = strrchr(native_path, '.');
    if (!dot || dot == native_path) {
        return str_format("%s.lto", native_path);
    }
    size_t stem_len = (size_t)(dot - native_path);
    return str_format("%.*s.lto%s", (int)stem_len, native_path, dot);
}

char* dal_c__makeSharedImportLibraryPath(const char* shared_path) {
    assert(shared_path != NULL);
    return str_format("%s.lib", shared_path);
}

static bool dal_c__isLtoStaticLibraryPath(const char* path, bool is_windows) {
    return path && str_endsWith(path, is_windows ? ".lto.lib" : ".lto.a");
}

static bool dal_c__isSharedLinkLibraryPath(const char* path, bool is_windows) {
    if (!path) { return false; }
    if (str_endsWith(path, ".dll.a")) { return true; }
    if (!is_windows) { return str_endsWith(path, ".so"); }
    return str_endsWith(path, ".dll.lib");
}

static bool dal_c__isNativeStaticLibraryPath(const char* path, bool is_windows) {
    if (!path || dal_c__isLtoStaticLibraryPath(path, is_windows)) { return false; }
    if (is_windows && dal_c__isSharedLinkLibraryPath(path, true)) { return false; }
    return str_endsWith(path, is_windows ? ".lib" : ".a");
}

bool dal_c__shouldLinkDependencyArtifact(const char* path, bool is_windows, bool lto_enabled) {
    if (!path) { return false; }
    if (dal_c__isSharedLinkLibraryPath(path, is_windows)) { return true; }
    if (dal_c__isLtoStaticLibraryPath(path, is_windows)) { return lto_enabled; }
    if (!dal_c__isNativeStaticLibraryPath(path, is_windows)) { return false; }
    if (!lto_enabled) { return true; }

    char* lto_path = dal_c__makeLtoStaticLibraryPath(path);
    bool has_lto_peer = lto_path && path_isFile(lto_path);
    free(lto_path);
    return !has_lto_peer;
}

static char* dal_c__selectStaticLibraryPath(const char* native_path, bool lto_enabled) {
    assert(native_path != NULL);
    if (lto_enabled) {
        char* lto_path = dal_c__makeLtoStaticLibraryPath(native_path);
        if (lto_path && path_isFile(lto_path)) {
            return lto_path;
        }
        free(lto_path);
    }
    return strdup(native_path);
}

static char* dal_c__makePrebuiltProfileDir(const char* project_root, const dal_c_CompilerOpts* opts, const char* profile_name) {
    assert(project_root != NULL);
    assert(profile_name != NULL);
    char* prebuilt_root = path_join(project_root, "prebuilt");
    char* target_name = dal_c__resolveTargetDirName(opts);
    char* target_root = (prebuilt_root && target_name) ? path_join(prebuilt_root, target_name) : NULL;
    char* profile_dir = target_root ? path_join(target_root, profile_name) : NULL;
    free(target_root);
    free(target_name);
    free(prebuilt_root);
    return profile_dir;
}

static char* dal_c__resolvePrebuiltArtifactPath(
    const char* prebuilt_profile_dir,
    const char* name,
    dal_c_Target target_type,
    bool is_windows,
    bool lto_enabled
) {
    assert(prebuilt_profile_dir != NULL);
    assert(name != NULL);

    char* libs_dir = path_join(prebuilt_profile_dir, "libs");
    char* file_name = dal_c__makeTargetFileName(name, target_type, is_windows, NULL);
    char* native_path = (libs_dir && file_name) ? path_join(libs_dir, file_name) : NULL;
    char* selected_path = NULL;
    bool available = native_path && path_isFile(native_path);
    if (native_path && target_type == dal_c_Target_static_lib) {
        selected_path = dal_c__selectStaticLibraryPath(native_path, lto_enabled);
        available = available && selected_path && path_isFile(selected_path);
    } else if (native_path) {
        selected_path = strdup(native_path);
    }

    if (available && is_windows && target_type == dal_c_Target_shared_lib) {
        char* import_path = dal_c__makeSharedImportLibraryPath(selected_path);
        available = import_path && path_isFile(import_path);
        free(import_path);
    }
    if (!available) {
        free(selected_path);
        selected_path = NULL;
    }

    free(native_path);
    free(file_name);
    free(libs_dir);
    return selected_path;
}


typedef struct dal_c__LinkGraphNode {
    char* key;
    ArrStr* artifacts;
    int* dependencies;
    int dependency_count;
    int dependency_capacity;
    int visit_state;
    bool emitted;
} dal_c__LinkGraphNode;

typedef struct dal_c__LinkGraph {
    dal_c__LinkGraphNode* nodes;
    int node_count;
    int node_capacity;
    const char* deps_dir;
    const char* dh_path;
    bool is_windows;
    bool lto_enabled;
    bool cycle;
    bool failed;
} dal_c__LinkGraph;

static void dal_c__linkGraphCleanup(dal_c__LinkGraph* graph) {
    if (!graph) { return; }
    for (int i = 0; i < graph->node_count; ++i) {
        dal_c__LinkGraphNode* node = &graph->nodes[i];
        free(node->key);
        ArrStr_fini(&node->artifacts);
        free(node->dependencies);
    }
    free(graph->nodes);
    memset(graph, 0, sizeof(*graph));
}

static int dal_c__linkGraphFindNode(const dal_c__LinkGraph* graph, const char* key) {
    if (!graph || !key) { return -1; }
    for (int i = 0; i < graph->node_count; ++i) {
        if (str_eql(graph->nodes[i].key, key)) { return i; }
    }
    return -1;
}

static int dal_c__linkGraphAddNode(dal_c__LinkGraph* graph, const char* key) {
    assert(graph != NULL);
    assert(key != NULL);
    int existing = dal_c__linkGraphFindNode(graph, key);
    if (existing >= 0) { return existing; }
    if (graph->node_count >= graph->node_capacity) {
        int capacity = graph->node_capacity > 0 ? graph->node_capacity * 2 : 16;
        dal_c__LinkGraphNode* nodes = realloc(graph->nodes, (size_t)capacity * sizeof(*nodes));
        if (!nodes) {
            graph->failed = true;
            return -1;
        }
        graph->nodes = nodes;
        graph->node_capacity = capacity;
    }
    dal_c__LinkGraphNode* node = &graph->nodes[graph->node_count];
    memset(node, 0, sizeof(*node));
    node->key = strdup(key);
    node->artifacts = ArrStr_init();
    if (!node->key || !node->artifacts) {
        free(node->key);
        ArrStr_fini(&node->artifacts);
        memset(node, 0, sizeof(*node));
        graph->failed = true;
        return -1;
    }
    return graph->node_count++;
}

static void dal_c__linkGraphAddEdge(dal_c__LinkGraph* graph, int consumer, int provider) {
    assert(graph != NULL);
    if (consumer < 0 || provider < 0) {
        graph->failed = true;
        return;
    }
    dal_c__LinkGraphNode* node = &graph->nodes[consumer];
    for (int i = 0; i < node->dependency_count; ++i) {
        if (node->dependencies[i] == provider) { return; }
    }
    if (node->dependency_count >= node->dependency_capacity) {
        int capacity = node->dependency_capacity > 0 ? node->dependency_capacity * 2 : 4;
        int* dependencies = realloc(node->dependencies, (size_t)capacity * sizeof(*dependencies));
        if (!dependencies) {
            graph->failed = true;
            return;
        }
        node->dependencies = dependencies;
        node->dependency_capacity = capacity;
    }
    node->dependencies[node->dependency_count++] = provider;
}

static char* dal_c__linkGraphNodeKey(const dal_c_Project* owner, const dal_c_Lib* lib) {
    assert(lib != NULL);
    const char* provider = lib->provider && lib->provider[0] ? lib->provider : "dh";
    const char* identity = NULL;
    char* canonical_path = NULL;
    if (lib->path && lib->path[0]) {
        canonical_path = path_abs(lib->path);
        identity = canonical_path ? canonical_path : lib->path;
    } else if (lib->source && lib->source[0]) {
        identity = lib->source;
    } else if (lib->archive && lib->archive[0]) {
        identity = lib->archive;
    } else {
        identity = owner && owner->root ? owner->root : "";
    }
    char* key = str_format(
        "%s:%s:%s:%s:%s",
        provider,
        identity,
        lib->package_root ? lib->package_root : "",
        lib->name,
        lib->is_static ? "static" : "shared"
    );
    free(canonical_path);
    return key;
}

static void dal_c__linkGraphPushExisting(dal_c__LinkGraphNode* node, char* path) {
    if (node && path && path_isFile(path)) {
        dal_c__arrStrPushUniquePath(node->artifacts, path);
    }
    free(path);
}

static void dal_c__linkGraphCollectNamedArtifact(
    dal_c__LinkGraphNode* node,
    const char* directory,
    const char* name,
    bool is_static,
    bool is_windows,
    bool lto_enabled
) {
    if (!node || !directory || !name || !name[0]) { return; }
    if (is_static) {
        char* native_name = is_windows ? str_format("%s.lib", name) : str_format("lib%s.a", name);
        char* native_path = native_name ? path_join(directory, native_name) : NULL;
        char* selected = native_path ? dal_c__selectStaticLibraryPath(native_path, lto_enabled) : NULL;
        dal_c__linkGraphPushExisting(node, selected);
        free(native_path);
        free(native_name);
        return;
    }
    if (is_windows) {
        char* dh_import_name = str_format("%s.dll.lib", name);
        char* gnu_import_name = str_format("lib%s.dll.a", name);
        char* generic_import_name = str_format("%s.lib", name);
        dal_c__linkGraphPushExisting(node, dh_import_name ? path_join(directory, dh_import_name) : NULL);
        dal_c__linkGraphPushExisting(node, gnu_import_name ? path_join(directory, gnu_import_name) : NULL);
        dal_c__linkGraphPushExisting(node, generic_import_name ? path_join(directory, generic_import_name) : NULL);
        free(generic_import_name);
        free(gnu_import_name);
        free(dh_import_name);
    } else {
        char* so_name = str_format("lib%s.so", name);
        char* dylib_name = str_format("lib%s.dylib", name);
        dal_c__linkGraphPushExisting(node, so_name ? path_join(directory, so_name) : NULL);
        dal_c__linkGraphPushExisting(node, dylib_name ? path_join(directory, dylib_name) : NULL);
        free(dylib_name);
        free(so_name);
    }
}

static void dal_c__linkGraphCollectArtifacts(
    dal_c__LinkGraph* graph,
    dal_c__LinkGraphNode* node,
    const dal_c_Lib* lib
) {
    assert(graph != NULL);
    assert(node != NULL);
    assert(lib != NULL);
    char* target_dir = dal_c__resolveDepsTargetDir(graph->deps_dir, lib->name);
    char* artifact_name = path_basename(lib->name);
    dal_c__linkGraphCollectNamedArtifact(
        node, target_dir, artifact_name, lib->is_static, graph->is_windows, graph->lto_enabled
    );
    free(artifact_name);
    free(target_dir);

    for (int i = 0; i < lib->opts.link_count; ++i) {
        dal_c__linkGraphCollectNamedArtifact(
            node,
            graph->deps_dir,
            lib->opts.link_libs[i],
            lib->is_static,
            graph->is_windows,
            graph->lto_enabled
        );
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
static int dal_c__linkGraphVisitLibrary(
    dal_c__LinkGraph* graph,
    const dal_c_Project* owner,
    const dal_c_Lib* lib
) {
    assert(graph != NULL);
    assert(lib != NULL);
    char* key = dal_c__linkGraphNodeKey(owner, lib);
    int node_index = key ? dal_c__linkGraphAddNode(graph, key) : -1;
    free(key);
    if (node_index < 0) {
        graph->failed = true;
        return -1;
    }

    dal_c__LinkGraphNode* node = &graph->nodes[node_index];
    dal_c__linkGraphCollectArtifacts(graph, node, lib);
    if (node->visit_state == 1) {
        graph->cycle = true;
        return node_index;
    }
    if (node->visit_state == 2) { return node_index; }

    node->visit_state = 1;
    const char* provider = lib->provider && lib->provider[0] ? lib->provider : "dh";
    if (str_eql(provider, "dh") && lib->path && lib->path[0]) {
        dal_c_Project* child = dal_c_Project_detectAt(lib->path, graph->dh_path);
        if (child) {
            for (int i = 0; i < child->lib_count; ++i) {
                int dependency = dal_c__linkGraphVisitLibrary(graph, child, &child->libraries[i]);
                dal_c__linkGraphAddEdge(graph, node_index, dependency);
            }
            dal_c_Project_cleanup(&child);
        }
    }
    node->visit_state = 2;
    return node_index;
}

static bool dal_c__linkGraphArtifactHasPendingOwner(
    const dal_c__LinkGraph* graph,
    int owner_index,
    const char* artifact
) {
    assert(graph != NULL);
    assert(artifact != NULL);
    for (int i = 0; i < graph->node_count; ++i) {
        if (i == owner_index || graph->nodes[i].emitted) { continue; }
        if (dal_c__arrStrContainsPath(graph->nodes[i].artifacts, artifact)) { return true; }
    }
    return false;
}

static bool dal_c__linkGraphAppendTopological(dal_c__LinkGraph* graph, ArrStr* paths) {
    assert(graph != NULL);
    assert(paths != NULL);
    if (graph->failed) { return false; }
    if (graph->cycle) {
        (void)fprintf(stderr, "Error: Dependency graph contains a cycle.\n");
        return false;
    }

    int* incoming = calloc((size_t)graph->node_count, sizeof(*incoming));
    if (graph->node_count > 0 && !incoming) { return false; }
    for (int i = 0; i < graph->node_count; ++i) {
        dal_c__LinkGraphNode* node = &graph->nodes[i];
        for (int j = 0; j < node->dependency_count; ++j) {
            incoming[node->dependencies[j]]++;
        }
    }

    int emitted = 0;
    while (emitted < graph->node_count) {
        int next = -1;
        for (int i = 0; i < graph->node_count; ++i) {
            if (!graph->nodes[i].emitted && incoming[i] == 0) {
                next = i;
                break;
            }
        }
        if (next < 0) {
            (void)fprintf(stderr, "Error: Dependency graph contains a cycle.\n");
            free(incoming);
            return false;
        }
        dal_c__LinkGraphNode* node = &graph->nodes[next];
        for (int i = 0; i < ArrStr_len(node->artifacts); ++i) {
            const char* artifact = ArrStr_at(node->artifacts, i);
            if (!dal_c__linkGraphArtifactHasPendingOwner(graph, next, artifact)) {
                dal_c__arrStrPushUniquePath(paths, artifact);
            }
        }
        node->emitted = true;
        emitted++;
        for (int i = 0; i < node->dependency_count; ++i) {
            incoming[node->dependencies[i]]--;
        }
    }
    free(incoming);
    return true;
}

ArrStr* dal_c__collectLinkDependencyPaths(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);

    ArrStr* deps = ArrStr_init();
    if (!deps) { return NULL; }
    if (target_type != dal_c_Target_executable && target_type != dal_c_Target_shared_lib && target_type != dal_c_Target_image) {
        return deps;
    }

    const bool is_windows = dal_c__platformIsWindows();
    const bool lto_enabled = dal_c__effectiveLtoEnabled(cmd, profile, target_type);
    const dal_c_CompilerOpts* opts = &cmd->opts;
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);

    char* project_lib_name = NULL;
    char* project_lib_path = NULL;
    dal_c_TargetRequest request = { 0 };
    bool has_request = proj && dal_c_TargetRequest_resolve(proj, &intent, &request);
    bool link_project_static_lib = proj
                                && proj->root
                                && proj->name
                                && ((has_request && request.root && request.link_project)
                                    || intent.action == dal_c_CmdAction_test
                                    || intent.action == dal_c_CmdAction_test_dsl);
    if (link_project_static_lib) {
        char* project_profile_dir = dal_c__makeBuildProfileDir(proj, opts, profile);
        char* project_lib_dir = project_profile_dir ? path_join(project_profile_dir, "libs") : NULL;
        const char* project_output_name = proj->defaults.output_name && proj->defaults.output_name[0]
                                            ? proj->defaults.output_name
                                            : proj->name;
        project_lib_name = dal_c__makeTargetFileName(project_output_name, dal_c_Target_static_lib, is_windows, NULL);
        char* project_native_path = (project_lib_dir && project_lib_name) ? path_join(project_lib_dir, project_lib_name) : NULL;
        project_lib_path = project_native_path ? dal_c__selectStaticLibraryPath(project_native_path, lto_enabled) : NULL;
        if (project_lib_path && path_isFile(project_lib_path)) {
            ArrStr_push(deps, project_lib_path);
        }
        free(project_native_path);
        free(project_lib_dir);
        free(project_profile_dir);
    }
    free(project_lib_path);
    free(project_lib_name);
    dal_c_TargetRequest_cleanup(&request);

    if (proj && proj->lib_count > 0) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        if (deps_dir && path_isDir(deps_dir)) {
            dal_c__LinkGraph graph = {
                .deps_dir = deps_dir,
                .dh_path = proj->dh_path,
                .is_windows = is_windows,
                .lto_enabled = lto_enabled,
            };
            for (int i = 0; i < proj->lib_count; ++i) {
                if (dal_c__linkGraphVisitLibrary(&graph, proj, &proj->libraries[i]) < 0) {
                    break;
                }
            }
            bool graph_ordered = dal_c__linkGraphAppendTopological(&graph, deps);

            int lib_count = 0;
            char** lib_files = graph_ordered ? dir_listRecur(deps_dir, &lib_count) : NULL;
            if (lib_files) {
                qsort(lib_files, (size_t)lib_count, sizeof(*lib_files), dal_c__compareCStringPointers);
                for (int i = 0; i < lib_count; ++i) {
                    if (dal_c__shouldLinkDependencyArtifact(lib_files[i], is_windows, lto_enabled)
                        && !dal_c__arrStrContainsPath(deps, lib_files[i])) {
                        ArrStr_push(deps, lib_files[i]);
                    }
                }
                dal_c__freeFileList(lib_files, lib_count);
            }
            dal_c__linkGraphCleanup(&graph);
            if (!graph_ordered) {
                free(deps_dir);
                ArrStr_fini(&deps);
                return NULL;
            }
        }
        free(deps_dir);
    }

    if (proj && proj->dh_path && dal_c__usesDHLibrary(proj, opts)) {
        char* dh_profile = dal_c__makeBuildProfileDirAt(proj->dh_path, opts, profile);
        char* dh_lib_dir = dh_profile ? path_join(dh_profile, "libs") : NULL;
        const char* dh_lib_name = target_type == dal_c_Target_shared_lib
                                    ? (is_windows ? "dh.dll.lib" : "libdh.so")
                                    : (is_windows ? "dh.lib" : "libdh.a");
        char* dh_native = dh_lib_dir ? path_join(dh_lib_dir, dh_lib_name) : NULL;
        char* dh_lib = (dh_native && target_type != dal_c_Target_shared_lib)
                         ? dal_c__selectStaticLibraryPath(dh_native, lto_enabled)
                         : (dh_native ? strdup(dh_native) : NULL);
        if (dh_lib && path_isFile(dh_lib)) {
            dal_c__arrStrPushUnique(deps, dh_lib);
        }
        free(dh_native);
        free(dh_lib);
        free(dh_lib_dir);
        free(dh_profile);
    }

    return deps;
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
        char* file_name = dal_c__makeTargetFileName(output_name, type, is_windows, intent.output_ext);
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

    bool output_override_is_dir = dal_c__pathEndsWithSeparator(output_override) || path_isDir(output_override);

    if (!dal_c__pathHasSeparator(output_override) && !output_override_is_dir) {
        char* file_name = dal_c__makeTargetFileName(output_override, type, is_windows, intent.output_ext);
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

    if (output_override_is_dir) {
        char* output_root = NULL;
        if (dal_c__pathIsAbsolute(output_override)) {
            output_root = strdup(output_override);
        } else {
            char* cwd = env_getCWD();
            output_root = cwd ? path_join(cwd, output_override) : strdup(output_override);
            free(cwd);
        }
        char* file_name = dal_c__makeTargetFileName(output_name, type, is_windows, intent.output_ext);
        char* output_path = output_root && file_name ? path_join(output_root, file_name) : NULL;
        dal_c_TargetRequest_cleanup(&request);
        free(file_name);
        free(output_root);
        return output_path;
    }

    char* output_stem = NULL;
    if (dal_c__pathIsAbsolute(output_override)) {
        output_stem = strdup(output_override);
    } else {
        char* cwd = env_getCWD();
        output_stem = cwd ? path_join(cwd, output_override) : strdup(output_override);
        free(cwd);
    }

    char* output_path = output_stem ? dal_c__makeTargetFileName(output_stem, type, is_windows, intent.output_ext) : NULL;
    dal_c_TargetRequest_cleanup(&request);
    free(output_stem);
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
    char* profile_dir = NULL;
    if (proj && proj->root) {
        profile_dir = cmd->dry_run
                        ? dal_c__makeBuildProfileDirReadOnly(proj, &cmd->opts, profile)
                        : dal_c__makeBuildProfileDir(proj, &cmd->opts, profile);
    } else {
        char* target_name = dal_c__resolveTargetDirName(&cmd->opts);
        char* target_dir = (base_dir && target_name) ? path_join(base_dir, target_name) : NULL;
        profile_dir = target_dir ? path_join(target_dir, profile->name) : NULL;
        free(target_dir);
        free(target_name);
    }
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
            ArrStr* lib_files = dal_c__collectBuildFilesCached(lib_proj, lib_proj->self_roots[i].path, false);
            for (int j = 0; j < ArrStr_len(lib_files); ++j) {
                ArrStr_push(sources, ArrStr_at(lib_files, j));
            }
            ArrStr_fini(&lib_files);
        }
    } else {
        char* lib_src = lib_proj ? dal_c_Project_getSrcDir(lib_proj) : path_join(lib_abs, dal_c_dir_src);
        ArrStr* lib_files = dal_c__collectBuildFilesCached(lib_proj, lib_src, false);
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
    if (!target_subdir || strlen(target_subdir) == 0 || str_eql(target_subdir, ".")) {
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
    bool is_windows,
    bool from_prebuilt
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

    // 2. Copy compiled artifacts from build/<profile>/libs/.
    char* libs_dir = path_join(lib_build_dir, "libs");
    char* artifact_rel = dal_c__makeTargetFileName(lib->name, build_target_type, is_windows, NULL);
    char* artifact_src = path_join(libs_dir, artifact_rel);
    if (!path_isFile(artifact_src)) {
        char* artifact_base = path_basename(artifact_rel);
        free(artifact_src);
        artifact_src = path_join(libs_dir, artifact_base);
        free(artifact_base);
    }

    if (build_target_type == dal_c_Target_static_lib) {
        char* artifact_name = path_basename(artifact_src);
        char* artifact_dst = path_join(target_dir, artifact_name);
        if (!path_isFile(artifact_src) || !file_copy(artifact_src, artifact_dst)) {
            success = false;
        }

        char* lto_src = dal_c__makeLtoStaticLibraryPath(artifact_src);
        if (path_isFile(lto_src)) {
            char* lto_name = path_basename(lto_src);
            char* lto_dst = path_join(target_dir, lto_name);
            if (!file_copy(lto_src, lto_dst)) {
                success = false;
            }
            free(lto_dst);
            free(lto_name);
        }
        free(lto_src);
        free(artifact_dst);
        free(artifact_name);
    } else if (build_target_type == dal_c_Target_shared_lib) {
        char* artifact_name = path_basename(artifact_src);
        char* artifact_dst = path_join(target_dir, artifact_name);
        if (!path_isFile(artifact_src) || !file_copy(artifact_src, artifact_dst)) {
            success = false;
        }
        if (is_windows) {
            char* import_src = dal_c__makeSharedImportLibraryPath(artifact_src);
            char* import_name = path_basename(import_src);
            char* import_dst = path_join(target_dir, import_name);
            if (!path_isFile(import_src) || !file_copy(import_src, import_dst)) {
                success = false;
            }
            free(import_dst);
            free(import_name);
            free(import_src);
        }
        free(artifact_dst);
        free(artifact_name);
    }
    free(artifact_src);
    free(artifact_rel);
    free(libs_dir);

    // 3. Copy the PCH matching the consumable artifact variant.
    if (!from_prebuilt && lib_proj && lib_proj->pch_header) {
        dal_c_Cmd pch_cmd = *build_cmd;
        if (build_target_type == dal_c_Target_static_lib) {
            dal_c_LtoMode artifact_lto_mode = dal_c__resolvedLtoState(&build_cmd->opts, build_profile);
            pch_cmd.opts.lto_mode = dal_c_LtoMode_isEnabled(artifact_lto_mode)
                                      ? artifact_lto_mode
                                      : dal_c_LtoMode_off;
        }
        char* pch_basename = path_basename(lib_proj->pch_header);
        char* pch_contract_key = dal_c__makeCompileContractKey(&pch_cmd, lib_proj, build_profile, build_target_type, true, false);
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
    char* lib_deps_dir = from_prebuilt ? path_join(lib_build_dir, "deps") : NULL;
    if (!lib_deps_dir || !path_isDir(lib_deps_dir)) {
        free(lib_deps_dir);
        lib_deps_dir = path_join(lib_abs_path, "lib/deps");
    }
    if (path_isDir(lib_deps_dir)) {
        int dep_count = 0;
        char** dep_files = dir_listRecur(lib_deps_dir, &dep_count);
        if (dep_files) {
            for (int i = 0; i < dep_count; ++i) {
                // Copy static/LTO archives, shared libraries/import libraries, headers, and PCH files.
                bool is_link_artifact = str_endsWith(dep_files[i], ".lib")
                                     || str_endsWith(dep_files[i], ".a")
                                     || str_endsWith(dep_files[i], ".dll")
                                     || str_endsWith(dep_files[i], ".so");
                if (is_link_artifact || str_endsWith(dep_files[i], ".h") || str_endsWith(dep_files[i], ".pch")) {
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

int dal_c__parallelJobCount(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    const char* jobs = cmd->make_jobs ? cmd->make_jobs : env_get("DAL_C_MAKE_JOBS");
    if (jobs && jobs[0] != '\0') {
        long parsed = strtol(jobs, NULL, 10);
        if (parsed < 1) { parsed = 1; }
        if (parsed > 32) { parsed = 32; }
        return (int)parsed;
    }
#if _WIN32
    const char* procs_env = env_get("NUMBER_OF_PROCESSORS");
    long procs = procs_env ? strtol(procs_env, NULL, 10) : 1;
    if (procs < 1) { procs = 1; }
    if (procs > 32) { procs = 32; }
    return (int)procs;
#else
    long procs = sysconf(_SC_NPROCESSORS_ONLN);
    if (procs < 1) { procs = 1; }
    long page_size = sysconf(_SC_PAGESIZE);
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    if (page_size > 0 && phys_pages > 0) {
        unsigned long long mem_bytes = (unsigned long long)page_size * (unsigned long long)phys_pages;
        unsigned long long bytes_per_job = 1024ull * 1024ull * 1024ull;
        const char* bytes_per_job_env = env_get("DAL_C_BYTES_PER_JOB");
        if (bytes_per_job_env && bytes_per_job_env[0] != '\0') {
            char* end = NULL;
            unsigned long long parsed = strtoull(bytes_per_job_env, &end, 10);
            if (parsed > 0 && end && *end == '\0') {
                bytes_per_job = parsed;
            }
        }
        long mem_jobs = (long)(mem_bytes / bytes_per_job);
        if (mem_jobs < 1) { mem_jobs = 1; }
        if (procs > mem_jobs) { procs = mem_jobs; }
    }
    if (procs > 32) { procs = 32; }
    return (int)procs;
#endif
}

static char* dal_c__buildParallelFlag(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    return str_format("-j%d", dal_c__parallelJobCount(cmd));
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

static int dal_c__resolvedMacroBacktraceLimit(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return opts->macro_backtrace_limit_set
             ? opts->macro_backtrace_limit
             : dal_c_default_macro_backtrace_limit;
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

static bool dal_c__isLinkedTarget(dal_c_Target target_type) {
    return target_type == dal_c_Target_executable
        || target_type == dal_c_Target_shared_lib
        || target_type == dal_c_Target_image;
}

static bool dal_c__hostClangLtoPrefersLld(void) {
#if defined(_WIN32) || defined(__APPLE__)
    return false;
#else
    return true;
#endif
}

static bool dal_c__textMentionsLinkerSelection(const char* text) {
    return text && (strstr(text, "-fuse-ld=") != NULL || strstr(text, "-fuse-ld ") != NULL);
}

static bool dal_c__cmdHasExplicitLinkerSelection(const dal_c_Cmd* cmd) {
    if (!cmd) { return false; }
    return dal_c__textMentionsLinkerSelection(cmd->opts.compiler_args)
        || dal_c__textMentionsLinkerSelection(cmd->compiler_args)
        || dal_c__textMentionsLinkerSelection(cmd->opts.link_args)
        || dal_c__textMentionsLinkerSelection(cmd->link_args);
}

static bool dal_c__toolExistsInDir(const char* dir, const char* tool) {
    if (!dir || !tool) { return false; }
    char* path = path_join(dir, tool);
    bool found = path_isFile(path);
#ifdef _WIN32
    if (!found && !str_endsWith(tool, ".exe")) {
        char* exe_name = str_format("%s.exe", tool);
        char* exe_path = exe_name ? path_join(dir, exe_name) : NULL;
        found = path_isFile(exe_path);
        free(exe_path);
        free(exe_name);
    }
#endif
    free(path);
    return found;
}

static bool dal_c__toolAvailableOnPath(const char* tool) {
    if (!tool || tool[0] == '\0') { return false; }
    char* path_env = env_get("PATH");
    if (!path_env) { return false; }
#ifdef _WIN32
    const char sep = ';';
#else
    const char sep = ':';
#endif
    bool found = false;
    char* cursor = path_env;
    while (!found && cursor && *cursor != '\0') {
        char* next = strchr(cursor, sep);
        if (next) { *next = '\0'; }
        found = dal_c__toolExistsInDir(cursor[0] != '\0' ? cursor : ".", tool);
        cursor = next ? next + 1 : NULL;
    }
    free(path_env);
    return found;
}

static bool dal_c__toolAvailableNearCompiler(const char* compiler, const char* tool) {
    if (!compiler || !tool || !dal_c__pathHasSeparator(compiler)) {
        return false;
    }
    char* dir = path_parent(compiler);
    bool found = dal_c__toolExistsInDir(dir, tool);
    free(dir);
    return found;
}

static bool dal_c__toolAvailableForCompiler(const char* compiler, const char* tool) {
    return dal_c__toolAvailableNearCompiler(compiler, tool)
        || dal_c__toolAvailableOnPath(tool);
}

static bool dal_c__clangCanUseLld(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    const char* compiler = cmd->opts.compiler ? cmd->opts.compiler : dal_c_default_compiler;
    return dal_c__toolAvailableForCompiler(compiler, "ld.lld")
        || dal_c__toolAvailableForCompiler(compiler, "lld");
}

static bool dal_c__clangLtoNeedsLldDecision(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);
    if (!dal_c__isLinkedTarget(target_type) || !dal_c__hostClangLtoPrefersLld()) {
        return false;
    }
    dal_c_LtoMode lto_state = dal_c__resolvedLtoState(&cmd->opts, profile);
    if (!dal_c_LtoMode_isEnabled(lto_state)) {
        return false;
    }
    const char* compiler = cmd->opts.compiler ? cmd->opts.compiler : dal_c_default_compiler;
    return dal_c__compilerLooksLikeClang(compiler)
        && !dal_c__cmdHasExplicitLinkerSelection(cmd);
}

static bool dal_c__profileDefaultLtoEnabled(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile) {
    assert(cmd != NULL);
    assert(profile != NULL);
    return cmd->opts.lto_mode == dal_c_LtoMode_auto
        && dal_c_LtoMode_isEnabled(profile->lto_mode);
}

static bool dal_c__profileLtoDisabledByToolchain(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    return dal_c__profileDefaultLtoEnabled(cmd, profile)
        && dal_c__clangLtoNeedsLldDecision(cmd, profile, target_type)
        && !dal_c__clangCanUseLld(cmd);
}

static dal_c_LtoMode dal_c__effectiveLtoState(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);
    if (dal_c__profileLtoDisabledByToolchain(cmd, profile, target_type)) {
        return dal_c_LtoMode_off;
    }
    return dal_c__resolvedLtoState(&cmd->opts, profile);
}

static bool dal_c__effectiveLtoEnabled(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    return dal_c_LtoMode_isEnabled(dal_c__effectiveLtoState(cmd, profile, target_type));
}

static bool dal_c__shouldUseLldForClangLto(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    return dal_c__clangLtoNeedsLldDecision(cmd, profile, target_type)
        && dal_c__effectiveLtoEnabled(cmd, profile, target_type)
        && dal_c__clangCanUseLld(cmd);
}

static void dal_c__warnIfProfileLtoDisabledByToolchain(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    static bool warned = false;
    if (warned || !dal_c__profileLtoDisabledByToolchain(cmd, profile, target_type)) {
        return;
    }
    warned = true;
    const char* compiler = cmd->opts.compiler ? cmd->opts.compiler : dal_c_default_compiler;
    (void)fprintf(
        stderr,
        "Warning: `%s` profile LTO was disabled because `%s` is Clang-like and `ld.lld` was not found. Install LLD/LLVMgold, pass `--%s=on` to require LTO, or pass `--%s=off` explicitly.\n",
        profile->name,
        compiler,
        dal_c_opt_lto,
        dal_c_opt_lto
    );
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
    /* Compilation environment and link model are orthogonal. A freestanding
     * translation unit may still be linked against libc, while a hosted
     * translation unit may deliberately omit it. */
    return dal_c__resolvedToggle(opts->libc_linked, true);
}

static bool dal_c__resolvedLibcLinked(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    if (!dal_c__resolvedDefaultLibsLinked(opts)) {
        return false;
    }
    return dal_c__requestedLibcLinked(opts);
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

static bool dal_c__macroSpecNames(const char* spec, const char* name) {
    if (!spec || !name) { return false; }
    size_t spec_len = strcspn(spec, "=");
    return strlen(name) == spec_len && strncmp(spec, name, spec_len) == 0;
}

static bool dal_c__optsDefinesMacro(const dal_c_CompilerOpts* opts, const char* name) {
    assert(opts != NULL);
    assert(name != NULL);
    for (int i = 0; i < opts->define_count; ++i) {
        if (dal_c__macroSpecNames(opts->define_macros[i], name)) { return true; }
    }
    return false;
}

static bool dal_c__optsDefinesEitherMacro(
    const dal_c_CompilerOpts* opts,
    const char* positive,
    const char* negative
) {
    return dal_c__optsDefinesMacro(opts, positive) || dal_c__optsDefinesMacro(opts, negative);
}

static bool dal_c__targetIsWindows(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    const char* target = opts->arch_target;
    if (target && target[0] != '\0') {
        return strstr(target, "windows") != NULL
            || strstr(target, "mingw") != NULL
            || strstr(target, "msvc") != NULL;
    }
    return dal_c__platformIsWindows();
}

static bool dal_c__targetHasComponentPrefix(const char* target, const char* prefix) {
    assert(target != NULL);
    assert(prefix != NULL);
    size_t prefix_len = strlen(prefix);
    for (const char* component = target; component[0] != '\0';) {
        const char* separator = strchr(component, '-');
        size_t component_len = separator ? (size_t)(separator - component) : strlen(component);
        if (component_len >= prefix_len && strncmp(component, prefix, prefix_len) == 0) {
            return true;
        }
        if (!separator) { break; }
        component = separator + 1;
    }
    return false;
}

static bool dal_c__targetHasGnuSourceLibcProfile(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    const char* target = opts->arch_target;
    if (target && target[0] != '\0') {
        if (dal_c__targetIsWindows(opts)) { return false; }
        return dal_c__targetHasComponentPrefix(target, "gnu")
            || dal_c__targetHasComponentPrefix(target, "musl")
            || dal_c__targetHasComponentPrefix(target, "uclibc")
            || dal_c__targetHasComponentPrefix(target, "android");
    }

    /* Native musl does not expose a reliable predefined libc macro before its
     * headers are included. Linux/Android hosts are therefore the native
     * fallback for the GNU-compatible feature-test profile. Explicit targets
     * are classified by their ABI/libc component above. */
#if defined(__linux__) || defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

static bool dal_c__targetUsesGnuSource(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    return dal_c__resolvedCompileEnv(opts) == dal_c_CompileEnv_hosted
        && dal_c__resolvedLibcLinked(opts)
        && dal_c__targetHasGnuSourceLibcProfile(opts);
}

static void dal_c__appendDefaultPlatformDefines(ArrStr* argv, const dal_c_CompilerOpts* opts) {
    assert(argv != NULL);
    assert(opts != NULL);
    if (dal_c__targetIsWindows(opts)) {
        if (!dal_c__optsDefinesMacro(opts, "UNICODE")) {
            ArrStr_push(argv, "-DUNICODE");
        }
        if (!dal_c__optsDefinesMacro(opts, "_UNICODE")) {
            ArrStr_push(argv, "-D_UNICODE");
        }
    } else if (dal_c__targetUsesGnuSource(opts)) {
        if (!dal_c__optsDefinesMacro(opts, "_GNU_SOURCE")) {
            ArrStr_push(argv, "-D_GNU_SOURCE");
        }
    }
}

static void dal_c__writeDefaultPlatformDefines(FILE* fp, const dal_c_CompilerOpts* opts) {
    assert(fp != NULL);
    assert(opts != NULL);
    if (dal_c__targetIsWindows(opts)) {
        if (!dal_c__optsDefinesMacro(opts, "UNICODE")) {
            (void)fprintf(fp, " -DUNICODE");
        }
        if (!dal_c__optsDefinesMacro(opts, "_UNICODE")) {
            (void)fprintf(fp, " -D_UNICODE");
        }
    } else if (dal_c__targetUsesGnuSource(opts)) {
        if (!dal_c__optsDefinesMacro(opts, "_GNU_SOURCE")) {
            (void)fprintf(fp, " -D_GNU_SOURCE");
        }
    }
}

static bool dal_c__resolvedCompileFact(
    const dal_c_CompilerOpts* opts,
    const char* positive,
    const char* negative,
    bool fallback
) {
    bool has_positive = dal_c__optsDefinesMacro(opts, positive);
    bool has_negative = dal_c__optsDefinesMacro(opts, negative);
    if (has_positive && !has_negative) { return true; }
    if (has_negative && !has_positive) { return false; }
    return fallback;
}

static bool dal_c__resolvedCompileStartFilesFact(const dal_c_CompilerOpts* opts) {
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_START_FILES",
        "COMP_NO_START_FILES",
        dal_c__resolvedStartFilesLinked(opts)
    );
}

static bool dal_c__resolvedCompileCrtFact(const dal_c_CompilerOpts* opts) {
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_CRT",
        "COMP_NO_CRT",
        dal_c__resolvedCompileStartFilesFact(opts)
    );
}

static bool dal_c__resolvedCompileDefaultLibsFact(const dal_c_CompilerOpts* opts) {
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_DEFAULT_LIBS",
        "COMP_NO_DEFAULT_LIBS",
        dal_c__resolvedDefaultLibsLinked(opts)
    );
}

static bool dal_c__resolvedCompileCompilerRtFact(const dal_c_CompilerOpts* opts, dal_c_Target target_type) {
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_COMPILER_RT",
        "COMP_NO_COMPILER_RT",
        dal_c__resolvedCompilerRtLinked(opts, target_type)
    );
}

static bool dal_c__resolvedCompileLibcFact(const dal_c_CompilerOpts* opts) {
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_LIBC",
        "COMP_NO_LIBC",
        dal_c__resolvedLibcLinked(opts)
    );
}

static bool dal_c__resolvedCompileStdlibFact(const dal_c_CompilerOpts* opts) {
    bool fallback = dal_c__resolvedCompileStartFilesFact(opts)
                 && dal_c__resolvedCompileDefaultLibsFact(opts);
    return dal_c__resolvedCompileFact(
        opts,
        "COMP_HAS_STDLIB",
        "COMP_NO_STDLIB",
        fallback
    );
}

static void dal_c__appendCompileFactArgument(
    ArrStr* argv,
    const dal_c_CompilerOpts* opts,
    const char* positive,
    const char* negative,
    bool enabled
) {
    if (!dal_c__optsDefinesEitherMacro(opts, positive, negative)) {
        char* flag = str_format("-D%s", enabled ? positive : negative);
        if (flag) {
            ArrStr_push(argv, flag);
            free(flag);
        }
    }
}

static void dal_c__writeCompileFactFlag(
    FILE* fp,
    const dal_c_CompilerOpts* opts,
    const char* positive,
    const char* negative,
    bool enabled
) {
    if (!dal_c__optsDefinesEitherMacro(opts, positive, negative)) {
        (void)fprintf(fp, " -D%s", enabled ? positive : negative);
    }
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

static bool dal_c__compilerLooksLikeClang(const char* compiler) {
    if (!compiler) { return false; }
    const char* base = dal_c__tokenBasename(compiler);
    return strstr(base, "clang") != NULL;
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

void dal_c__appendCompileDbArguments(
    ArrStr* argv,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    const char* src
) {
    assert(argv != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(src != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    bool compiler_is_clang = dal_c__compilerLooksLikeClang(compiler);
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

    if (compiler_is_clang) {
        ArrStr_push(argv, "-fgnu-keywords");
        ArrStr_push(argv, "-Wno-microsoft-anon-tag");
        dal_c__argvPushFormat(argv, "-fmacro-backtrace-limit=%d", dal_c__resolvedMacroBacktraceLimit(opts));
    }
    ArrStr_push(argv, "-fms-extensions");
    ArrStr_push(argv, "-funsigned-char");
    dal_c__appendDefaultPlatformDefines(argv, opts);
    dal_c__appendCompileDbDiagnostics(
        argv, opts, compiler_is_clang, !str_eql(profile->name, dal_c_profile_fast)
    );
    if (!compiler_is_clang) {
        ArrStr_push(argv, "-Wno-comment");
    }

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

    if (dal_c__shouldAddProjectInclude(proj, cmd)) {
        char* include_dir = path_join(proj->root, dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, include_dir);
        free(include_dir);
    }
    if (dal_c__shouldAddProjectPrivateInclude(proj, cmd)) {
        char* src_dir = path_join(proj->root, dal_c_Project_getCategoryDirName(proj, dal_c_dir_src));
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, src_dir);
        free(src_dir);
    }
    if (dal_c__usesDHLibrary(proj, opts)) {
        char* dh_include = path_join(proj->dh_path, dal_c_dir_include);
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, dh_include);
        ArrStr_push(argv, "-isystem");
        ArrStr_push(argv, proj->dh_path);
        free(dh_include);
    }
    if (proj && proj->lib_count > 0) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        if (deps_dir) {
            ArrStr_push(argv, "-isystem");
            ArrStr_push(argv, deps_dir);
        }
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
    dal_c__appendCompilerArgsTokens(argv, cmd->opts.compiler_args);
    dal_c__appendCompilerArgsTokens(argv, cmd->compiler_args);
    ArrStr_push(argv, src);
}

void dal_c__appendSyntaxArguments(
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

    const dal_c_CompilerOpts* opts = &cmd->opts;
    bool is_windows = dal_c__targetIsWindows(opts);
    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    bool compiler_is_clang = dal_c__compilerLooksLikeClang(compiler);
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_fact = dal_c__resolvedCompileLibcFact(opts);
    bool default_libs_fact = dal_c__resolvedCompileDefaultLibsFact(opts);
    bool start_files_fact = dal_c__resolvedCompileStartFilesFact(opts);
    bool crt_fact = dal_c__resolvedCompileCrtFact(opts);
    bool compiler_rt_fact = dal_c__resolvedCompileCompilerRtFact(opts, target_type);
    bool stdlib_fact = dal_c__resolvedCompileStdlibFact(opts);

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

    if (compiler_is_clang) {
        ArrStr_push(argv, "-fgnu-keywords");
        ArrStr_push(argv, "-Wno-microsoft-anon-tag");
        ArrStr_push(argv, "-fcolor-diagnostics");
        dal_c__argvPushFormat(argv, "-fmacro-backtrace-limit=%d", dal_c__resolvedMacroBacktraceLimit(opts));
        if (is_windows) {
            ArrStr_push(argv, "-fansi-escape-codes");
        }
    }
    ArrStr_push(argv, "-fms-extensions");
    ArrStr_push(argv, "-funsigned-char");
    dal_c__appendDefaultPlatformDefines(argv, opts);
    if (!is_windows && target_type == dal_c_Target_shared_lib) {
        ArrStr_push(argv, "-fPIC");
    }
    if (compiler_is_clang) {
        ArrStr_push(argv, "-mllvm");
        ArrStr_push(argv, "-enable-dfa-jump-thread");
    }

    ArrStr_push(argv, "-DCOMP");
    ArrStr_push(argv, compile_env == dal_c_CompileEnv_freestanding ? "-DCOMP_FREESTANDING" : "-DCOMP_HOSTED");
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_START_FILES", "COMP_NO_START_FILES", start_files_fact);
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_CRT", "COMP_NO_CRT", crt_fact);
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_DEFAULT_LIBS", "COMP_NO_DEFAULT_LIBS", default_libs_fact);
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_COMPILER_RT", "COMP_NO_COMPILER_RT", compiler_rt_fact);
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_LIBC", "COMP_NO_LIBC", libc_fact);
    dal_c__appendCompileFactArgument(argv, opts, "COMP_HAS_STDLIB", "COMP_NO_STDLIB", stdlib_fact);
    if (!profile->debug_assertions) {
        ArrStr_push(argv, "-DNDEBUG");
    }

    const char* debug_flag = dal_c_DebugLevel_toFlag(profile->debug_level);
    if (debug_flag && strlen(debug_flag) > 0) {
        ArrStr_push(argv, debug_flag);
        if (is_windows) {
            ArrStr_push(argv, "-gcodeview");
        }
    }

    const char* opt_flag = dal_c_OptiLevel_toFlag(profile->opti_level);
    if (opt_flag) {
        ArrStr_push(argv, opt_flag);
    }

    dal_c_LtoMode lto_state = dal_c__effectiveLtoState(cmd, profile, target_type);
    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (dal_c__isLtoFlag(flag)) {
            continue;
        }
        if (!str_startsWith(flag, "-Wl,") && !str_startsWith(flag, "-L") && !str_startsWith(flag, "-l")) {
            ArrStr_push(argv, flag);
        }
    }
    const char* lto_flag = dal_c_LtoMode_toFlag(lto_state);
    if (lto_flag) {
        ArrStr_push(argv, lto_flag);
    }

    dal_c_ToggleState omit_frame_pointer = dal_c__resolvedOmitFramePointerState(opts, profile);
    if (omit_frame_pointer == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fomit-frame-pointer");
    } else if (omit_frame_pointer == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-omit-frame-pointer");
    }
    if (dal_c__resolvedFunctionSections(opts, profile)) {
        ArrStr_push(argv, "-ffunction-sections");
    }
    if (dal_c__resolvedDataSections(opts, profile)) {
        ArrStr_push(argv, "-fdata-sections");
    }
    dal_c_ToggleState unroll_loops = dal_c__resolvedUnrollLoopsState(opts, profile);
    if (unroll_loops == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-funroll-loops");
    } else if (unroll_loops == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-unroll-loops");
    }
    dal_c_ToggleState unwind_tables = dal_c__resolvedUnwindTablesState(opts, profile);
    if (unwind_tables == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-funwind-tables");
    } else if (unwind_tables == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-unwind-tables");
    }
    dal_c_ToggleState async_unwind_tables = dal_c__resolvedAsyncUnwindTablesState(opts, profile);
    if (async_unwind_tables == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fasynchronous-unwind-tables");
    } else if (async_unwind_tables == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-asynchronous-unwind-tables");
    }
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    if (exceptions == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fexceptions");
    } else if (exceptions == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-exceptions");
    }
    dal_c_ToggleState merge_all_constants = dal_c__resolvedMergeAllConstantsState(opts);
    if (merge_all_constants == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fmerge-all-constants");
    } else if (merge_all_constants == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-merge-all-constants");
    }
    dal_c_ToggleState stack_protector = dal_c__resolvedStackProtectorState(opts);
    if (stack_protector == dal_c_ToggleState_enabled) {
        ArrStr_push(argv, "-fstack-protector-strong");
    } else if (stack_protector == dal_c_ToggleState_disabled) {
        ArrStr_push(argv, "-fno-stack-protector");
    }

    dal_c__appendCompileDbDiagnostics(
        argv, opts, compiler_is_clang, !str_eql(profile->name, dal_c_profile_fast)
    );

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
    if (compile_env == dal_c_CompileEnv_freestanding) {
        ArrStr_push(argv, "-ffreestanding");
    }
    if (opts->sysroot) {
        dal_c__argvPushFormat(argv, "--sysroot=%s", opts->sysroot);
    }

    if (dal_c__shouldAddProjectInclude(proj, cmd)) {
        char* include_dir = path_join(proj->root, dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, include_dir);
        free(include_dir);
    }
    if (dal_c__shouldAddProjectPrivateInclude(proj, cmd)) {
        char* src_dir = path_join(proj->root, dal_c_Project_getCategoryDirName(proj, dal_c_dir_src));
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, src_dir);
        free(src_dir);
    }
    if (dal_c__usesDHLibrary(proj, opts)) {
        char* dh_include = path_join(proj->dh_path, dal_c_dir_include);
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, dh_include);
        ArrStr_push(argv, "-I");
        ArrStr_push(argv, proj->dh_path);
        free(dh_include);
    }
    if (proj && proj->lib_count > 0) {
        char* deps_dir = dal_c_Project_getDepsDir(proj);
        if (deps_dir) {
            ArrStr_push(argv, "-I");
            ArrStr_push(argv, deps_dir);
        }
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
    dal_c__appendVersionDefineArguments(argv, proj, &opts->version);
    if (dal_c__sourceUsesTestMode(cmd, proj, src)) {
        ArrStr_push(argv, "-DCOMP_TEST");
    }
    for (int i = 0; i < opts->undef_count; ++i) {
        dal_c__argvPushFormat(argv, "-U%s", opts->undef_macros[i]);
    }
    dal_c__appendCompilerArgsTokens(argv, cmd->opts.compiler_args);
    dal_c__appendCompilerArgsTokens(argv, cmd->compiler_args);
    ArrStr_push(argv, src);
}

static void dal_c__appendCompileDbDiagnostics(
    ArrStr* argv,
    const dal_c_CompilerOpts* opts,
    bool compiler_is_clang,
    bool stack_frame_diagnostics
) {
    assert(argv != NULL);
    assert(opts != NULL);

    if (opts->loose_errors == dal_c_LooseErrorsMode_suppress) {
        ArrStr_push(argv, "-w");
        return;
    }

    if (!compiler_is_clang) {
        const char* gcc_flags[] = {
            "-Wall", "-Wextra", "-Wconversion", "-Wsign-conversion",
            "-Wfloat-conversion", "-Wformat=2", "-Wcast-qual", "-Wcast-align",
            "-Wpointer-arith", "-Wbad-function-cast", "-Wnull-dereference",
            "-Wwrite-strings", "-Wuninitialized",
            "-Wno-switch-enum", "-Wstrict-prototypes", "-Wmissing-prototypes",
            "-Wmissing-variable-declarations", "-Wmissing-field-initializers", "-Wdiv-by-zero", "-Wno-comment", NULL
        };
        for (int i = 0; gcc_flags[i] != NULL; ++i) {
            ArrStr_push(argv, gcc_flags[i]);
        }
        if (stack_frame_diagnostics) {
            ArrStr_push(argv, "-Wframe-larger-than=4096");
        }
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
            "-Wno-switch-enum",
            "-Winfinite-recursion",
            "-Wno-microsoft-anon-tag",
            "-Wloop-analysis",
            "-Wstrict-prototypes",
            "-Wmissing-prototypes",
            "-Wmissing-variable-declarations",
            "-Wmissing-field-initializers",
            "-Wdiv-by-zero",
            "-Wthread-safety",
            NULL
        };
        for (int i = 0; flags[i] != NULL; ++i) {
            ArrStr_push(argv, flags[i]);
        }
        if (stack_frame_diagnostics) {
            ArrStr_push(argv, "-Wframe-larger-than=4096");
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
        "-Wno-switch-enum",
        "-Winfinite-recursion",
        "-Wno-microsoft-anon-tag",
        "-Wloop-analysis",
        "-Werror=strict-prototypes",
        "-Werror=missing-prototypes",
        "-Werror=missing-variable-declarations",
        "-Werror=missing-field-initializers",
        "-Werror=div-by-zero",
        "-Wthread-safety",
        NULL
    };
    for (int i = 0; flags[i] != NULL; ++i) {
        ArrStr_push(argv, flags[i]);
    }
    if (stack_frame_diagnostics) {
        ArrStr_push(argv, "-Wframe-larger-than=4096");
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
    const char* output_path
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

    char* output_tmp = dal_c__makeTempPath(output_path);
    if (!output_tmp) {
        (void)fprintf(stderr, "Error: Failed to allocate compile database path: %s\n", output_path);
        return 1;
    }

    FILE* fp = fopen(output_tmp, "w");
    if (!fp) {
        (void)fprintf(stderr, "Error: Failed to open compile database: %s\n", output_tmp);
        free(output_tmp);
        return 1;
    }

    char* cwd = env_getCWD();
    const char* directory = (proj && proj->root) ? proj->root : (cwd ? cwd : ".");
    (void)fprintf(fp, "[\n");
    for (int i = 0; i < ArrStr_len(sources); ++i) {
        const char* src = ArrStr_at(sources, i);
        ArrStr* argv = ArrStr_init();
        dal_c__appendCompileDbArguments(argv, cmd, proj, profile, src);

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
    if (fclose(fp) != 0) {
        (void)remove(output_tmp);
        free(output_tmp);
        return 1;
    }

    char* generated = file_read(output_tmp);
    bool ok = generated != NULL && dal_c__writeFileIfChanged(output_path, generated);
    free(generated);
    (void)remove(output_tmp);
    free(output_tmp);
    return ok ? 0 : 1;
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
    if (build->emit_linked_asm && !dal_c__effectiveLtoEnabled(cmd, profile, target_type)) {
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
    if (!requested_libc_linked
        && dal_c__resolvedDefaultLibsLinked(opts)
        && !dal_c__targetSupportsNoLibcFlag(opts)) {
        const char* target = (opts->arch_target && opts->arch_target[0] != '\0') ? opts->arch_target : "host";
        (void)fprintf(
            stderr,
            "Error: `--%s=off` cannot be represented for `%s` while `--%s` remains enabled.\n"
            "Use `--%s=off` and provide the required non-libc runtime libraries explicitly.\n",
            dal_c_opt_link_libc,
            target,
            dal_c_opt_link_default_libs,
            dal_c_opt_link_default_libs
        );
        return false;
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
    hash = dal_c__hashBool(hash, version->namespace_set);
    hash = dal_c__hashString(hash, version->namespace_name);
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

static char* dal_c__versionNamespace(const dal_c_Project* proj, const dal_c_VersionSpec* version) {
    const char* raw = NULL;
    if (version && version->namespace_set && version->namespace_name && version->namespace_name[0]) {
        raw = version->namespace_name;
    } else if (proj && proj->name && proj->name[0]) {
        raw = proj->name;
    } else {
        raw = "dal_c";
    }

    size_t len = strlen(raw);
    char* result = malloc(len + 2u);
    assert(result != NULL);
    size_t out = 0;
    unsigned char first = (unsigned char)raw[0];
    if (!((first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') || first == '_')) {
        result[out++] = '_';
    }
    for (size_t i = 0; i < len; ++i) {
        unsigned char ch = (unsigned char)raw[i];
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')
            || (ch >= '0' && ch <= '9') || ch == '_') {
            result[out++] = (char)ch;
        } else {
            result[out++] = '_';
        }
    }
    result[out] = '\0';
    return result;
}

static void dal_c__writeVersionDefines(FILE* fp, const dal_c_Project* proj, const dal_c_VersionSpec* version) {
    assert(fp != NULL);
    assert(version != NULL);
    char* ns = dal_c__versionNamespace(proj, version);

    if (version->core_set) {
        (void)fprintf(fp, " -D%s__NUM__VER_CORE_MAJOR=%u", ns, version->core_major);
        (void)fprintf(fp, " -D%s__NUM__VER_CORE_MINOR=%u", ns, version->core_minor);
        (void)fprintf(fp, " -D%s__NUM__VER_CORE_PATCH=%u", ns, version->core_patch);
    }
    if (version->label_prefix_set && version->label_prefix_str) {
        (void)fprintf(fp, " -D%s__NUM__VER_LABEL_PREFIX=%d", ns, version->label_prefix_num);
        (void)fprintf(fp, " -D%s__STR__VER_LABEL_PREFIX=\\\"%s\\\"", ns, version->label_prefix_str);
    }
    if (version->label_suffix_set && version->label_suffix_str) {
        (void)fprintf(fp, " -D%s__NUM__VER_LABEL_SUFFIX=%u", ns, version->label_suffix_num);
        (void)fprintf(fp, " -D%s__STR__VER_LABEL_SUFFIX=\\\"%s\\\"", ns, version->label_suffix_str);
    }
    if (version->build_set && version->build_str) {
        (void)fprintf(fp, " -D%s__STR__VER_BUILD=\\\"%s\\\"", ns, version->build_str);
    }
    free(ns);
}

static void dal_c__appendVersionDefineArguments(ArrStr* argv, const dal_c_Project* proj, const dal_c_VersionSpec* version) {
    assert(argv != NULL);
    assert(version != NULL);
    char* ns = dal_c__versionNamespace(proj, version);

    if (version->core_set) {
        dal_c__argvPushFormat(argv, "-D%s__NUM__VER_CORE_MAJOR=%u", ns, version->core_major);
        dal_c__argvPushFormat(argv, "-D%s__NUM__VER_CORE_MINOR=%u", ns, version->core_minor);
        dal_c__argvPushFormat(argv, "-D%s__NUM__VER_CORE_PATCH=%u", ns, version->core_patch);
    }
    if (version->label_prefix_set && version->label_prefix_str) {
        dal_c__argvPushFormat(argv, "-D%s__NUM__VER_LABEL_PREFIX=%d", ns, version->label_prefix_num);
        dal_c__argvPushFormat(argv, "-D%s__STR__VER_LABEL_PREFIX=\\\"%s\\\"", ns, version->label_prefix_str);
    }
    if (version->label_suffix_set && version->label_suffix_str) {
        dal_c__argvPushFormat(argv, "-D%s__NUM__VER_LABEL_SUFFIX=%u", ns, version->label_suffix_num);
        dal_c__argvPushFormat(argv, "-D%s__STR__VER_LABEL_SUFFIX=\\\"%s\\\"", ns, version->label_suffix_str);
    }
    if (version->build_set && version->build_str) {
        dal_c__argvPushFormat(argv, "-D%s__STR__VER_BUILD=\\\"%s\\\"", ns, version->build_str);
    }
    free(ns);
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

    const dal_c_CompilerOpts* opts = &cmd->opts;
    bool is_windows = dal_c__targetIsWindows(opts);
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_fact = dal_c__resolvedCompileLibcFact(opts);
    bool default_libs_fact = dal_c__resolvedCompileDefaultLibsFact(opts);
    bool start_files_fact = dal_c__resolvedCompileStartFilesFact(opts);
    bool crt_fact = dal_c__resolvedCompileCrtFact(opts);
    bool compiler_rt_fact = dal_c__resolvedCompileCompilerRtFact(opts, target_type);
    bool stdlib_fact = dal_c__resolvedCompileStdlibFact(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
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
    bool compiler_is_clang = dal_c__compilerLooksLikeClang(compiler);
    bool stack_frame_diagnostics = !str_eql(profile->name, dal_c_profile_fast);
    (void)fprintf(fp, "CC = %s\n", compiler);

    const char* c_std = opts->c_std ? opts->c_std : dal_c_default_c_std;
    (void)fprintf(fp, "STD = -std=%s\n", c_std);

    (void)fprintf(fp, "BUILD_DIR = ");
    dal_c__fprintMakePath(fp, build_dir);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "CFLAGS_BASE = $(STD)");
    if (compiler_is_clang) {
        (void)fprintf(fp, " -fgnu-keywords -Wno-microsoft-anon-tag");
        (void)fprintf(fp, " -fcolor-diagnostics -fmacro-backtrace-limit=%d", dal_c__resolvedMacroBacktraceLimit(opts));
        if (is_windows) {
            (void)fprintf(fp, " -fansi-escape-codes");
        }
    }
    (void)fprintf(fp, " -fms-extensions");
    (void)fprintf(fp, " -funsigned-char");
    dal_c__writeDefaultPlatformDefines(fp, opts);
    if (!is_windows && target_type == dal_c_Target_shared_lib) {
        (void)fprintf(fp, " -fPIC");
    }
    if (compiler_is_clang) {
        (void)fprintf(fp, " -mllvm -enable-dfa-jump-thread");
    }

    // Required macro definitions
    (void)fprintf(fp, " -DCOMP");
    if (compile_env == dal_c_CompileEnv_freestanding) {
        (void)fprintf(fp, " -DCOMP_FREESTANDING");
    } else {
        (void)fprintf(fp, " -DCOMP_HOSTED");
    }
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_START_FILES", "COMP_NO_START_FILES", start_files_fact);
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_CRT", "COMP_NO_CRT", crt_fact);
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_DEFAULT_LIBS", "COMP_NO_DEFAULT_LIBS", default_libs_fact);
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_COMPILER_RT", "COMP_NO_COMPILER_RT", compiler_rt_fact);
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_LIBC", "COMP_NO_LIBC", libc_fact);
    dal_c__writeCompileFactFlag(fp, opts, "COMP_HAS_STDLIB", "COMP_NO_STDLIB", stdlib_fact);
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

    dal_c_LtoMode lto_state = dal_c__effectiveLtoState(cmd, profile, target_type);
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
        if (stack_frame_diagnostics) {
            (void)fprintf(fp, " -Wframe-larger-than=4096");
        }
        (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
        if (compiler_is_clang) {
            (void)fprintf(fp, " -Wno-microsoft-anon-tag");
            (void)fprintf(fp, " -Wloop-analysis");
        }
        (void)fprintf(fp, " -Wstrict-prototypes");
        (void)fprintf(fp, " -Wmissing-prototypes");
        (void)fprintf(fp, " -Wmissing-variable-declarations");
        (void)fprintf(fp, " -Wmissing-field-initializers");
        (void)fprintf(fp, " -Wdiv-by-zero");
        if (compiler_is_clang) {
            (void)fprintf(fp, " -Wthread-safety");
        }
    } else {
        if (!compiler_is_clang) {
            (void)fprintf(fp, " -Wall -Wextra -Wconversion");
            (void)fprintf(fp, " -Wsign-conversion -Wfloat-conversion");
            (void)fprintf(fp, " -Wformat=2 -Wcast-qual -Wcast-align");
            (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
            (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
            (void)fprintf(fp, " -Wuninitialized");
            if (stack_frame_diagnostics) {
                (void)fprintf(fp, " -Wframe-larger-than=4096");
            }
            (void)fprintf(fp, " -Wno-switch-enum -Wstrict-prototypes");
            (void)fprintf(fp, " -Wmissing-prototypes -Wmissing-variable-declarations -Wmissing-field-initializers");
            (void)fprintf(fp, " -Wdiv-by-zero -Wno-comment");
        } else {
            (void)fprintf(fp, " -Werror=all -Werror=extra -Werror=conversion");
            (void)fprintf(fp, " -Werror=sign-conversion -Wfloat-conversion");
            (void)fprintf(fp, " -Wformat=2");
            (void)fprintf(fp, " -Werror=cast-qual -Werror=cast-align");
            (void)fprintf(fp, " -Wpointer-arith -Wbad-function-cast");
            (void)fprintf(fp, " -Wnull-dereference -Wwrite-strings");
            (void)fprintf(fp, " -Werror=uninitialized");
            if (stack_frame_diagnostics) {
                (void)fprintf(fp, " -Wframe-larger-than=4096");
            }
            (void)fprintf(fp, " -Wno-switch-enum -Winfinite-recursion");
            (void)fprintf(fp, " -Wno-microsoft-anon-tag");
            (void)fprintf(fp, " -Wloop-analysis -Werror=strict-prototypes");
            (void)fprintf(fp, " -Werror=missing-prototypes");
            (void)fprintf(fp, " -Werror=missing-variable-declarations");
            (void)fprintf(fp, " -Werror=missing-field-initializers");
            (void)fprintf(fp, " -Werror=div-by-zero -Wthread-safety");
        }
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
    if (dal_c__shouldAddProjectInclude(proj, cmd)) {
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/%s", dal_c_Project_getCategoryDirName(proj, dal_c_dir_include));
    }
    if (dal_c__shouldAddProjectPrivateInclude(proj, cmd)) {
        (void)fprintf(fp, " -I$(PROJECT_ROOT)/%s", dal_c_Project_getCategoryDirName(proj, dal_c_dir_src));
    }
    if (dal_c__usesDHLibrary(proj, opts)) {
        (void)fprintf(fp, " -I$(DH_PATH)/include");
        (void)fprintf(fp, " -I$(DH_PATH)");
    }
    if (proj && proj->lib_count > 0) {
        // Flat deps structure: all staged headers share one generated include root.
        if (proj->root) {
            (void)fprintf(fp, " -I$(PROJECT_ROOT)/lib/deps");
        } else {
            char* deps_dir = dal_c_Project_getDepsDir(proj);
            if (deps_dir) {
                (void)fprintf(fp, " -I");
                dal_c__fprintMakePath(fp, deps_dir);
            }
            free(deps_dir);
        }
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
    dal_c__writeVersionDefines(fp, proj, &opts->version);
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "UNDEFS =");
    for (int i = 0; i < opts->undef_count; ++i) {
        (void)fprintf(fp, " -U%s", opts->undef_macros[i]);
    }
    (void)fprintf(fp, "\n");

    (void)fprintf(fp, "CFLAGS_NO_PCH = $(CFLAGS_BASE) $(INCLUDES) $(DEFINES) $(UNDEFS)\n");
    if (cmd->opts.compiler_args) {
        (void)fprintf(fp, "CFLAGS_NO_PCH += %s\n", cmd->opts.compiler_args);
    }
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
            const char* objcopy = dal_c__effectiveObjcopy(cmd);
            const char* objcopy_format = dal_c__effectiveObjcopyFormat(cmd);
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
        if (proj && proj->lib_count > 0) {
            if (proj->root) {
                (void)fprintf(fp, " -L$(PROJECT_ROOT)/lib/deps");
            } else {
                char* deps_dir = dal_c_Project_getDepsDir(proj);
                if (deps_dir) {
                    (void)fprintf(fp, " -L");
                    dal_c__fprintMakePath(fp, deps_dir);
                }
                free(deps_dir);
            }
        }
        if (dal_c__usesDHLibrary(proj, opts)) {
            char* dh_profile_dir = dal_c__makeBuildProfileDirAt(proj->dh_path, opts, profile);
            char* dh_libs_dir = dh_profile_dir ? path_join(dh_profile_dir, "libs") : NULL;
            if (dh_libs_dir) {
                (void)fprintf(fp, " -L");
                dal_c__fprintMakePath(fp, dh_libs_dir);
            }
            free(dh_libs_dir);
            free(dh_profile_dir);
        }
        ArrStr* link_dependencies = dal_c__collectLinkDependencyPaths(cmd, proj, profile, target_type);
        if (link_dependencies) {
            for (int i = 0; i < ArrStr_len(link_dependencies); ++i) {
                (void)fprintf(fp, " ");
                dal_c__fprintMakePath(fp, ArrStr_at(link_dependencies, i));
            }
            ArrStr_fini(&link_dependencies);
        }
        for (int i = 0; i < opts->link_dir_count; ++i) {
            (void)fprintf(fp, " -L");
            dal_c__fprintMakePath(fp, opts->link_dirs[i]);
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
        if (dal_c__shouldUseLldForClangLto(cmd, profile, target_type)) {
            (void)fprintf(fp, " -fuse-ld=lld");
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
        const char* linker_script = dal_c__effectiveLinkerScript(cmd);
        if (linker_script) {
            (void)fprintf(fp, " -Xlinker -T -Xlinker ");
            dal_c__fprintMakePath(fp, linker_script);
        }
        if (cmd->opts.link_args) {
            (void)fprintf(fp, " %s", cmd->opts.link_args);
        }
        if (cmd->link_args) {
            (void)fprintf(fp, " %s", cmd->link_args);
        }
        if (!is_windows && default_libs_linked) {
            (void)fprintf(fp, " -lm");
        }
        if (target_type == dal_c_Target_executable) {
            dal_c__writePlatformLinkerFlags(fp, is_windows, profile, target_path);
        }
        (void)fprintf(fp, "\n");
    } else {
        (void)fprintf(fp, "AR = %s\n", dal_c__externalToolPath(dal_c_ExternalTool_archiver));
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

static bool dal_c__linkUsesTestMode(const dal_c_Cmd* cmd, dal_c_Target target_type) {
    assert(cmd != NULL);
    return target_type == dal_c_Target_executable && dal_c__commandUsesTestMode(cmd);
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

static bool dal_c__shouldAddProjectInclude(const dal_c_Project* proj, const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    if (!proj || !proj->root) {
        return false;
    }

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    dal_c_TargetRequest request = { 0 };
    bool has_request = dal_c_TargetRequest_resolve(proj, &intent, &request);
    bool add_include = true;
    if (has_request && request.root) {
        add_include = request.link_project;
    }
    dal_c_TargetRequest_cleanup(&request);
    return add_include;
}

static bool dal_c__shouldAddProjectPrivateInclude(const dal_c_Project* proj, const dal_c_Cmd* cmd) {
    return proj
        && proj->root
        && dal_c__commandUsesTestMode(cmd)
        && dal_c__shouldAddProjectInclude(proj, cmd);
}

static char* dal_c__makeCompileContractKey(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode) {
    assert(cmd != NULL);
    assert(profile != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    bool is_windows = dal_c__platformIsWindows();
    const char* compiler = opts->compiler ? opts->compiler : dal_c_default_compiler;
    bool compiler_is_clang = dal_c__compilerLooksLikeClang(compiler);
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    bool stdlib_linked = start_files_linked && default_libs_linked;
    bool crt_linked = start_files_linked;
    dal_c_LtoMode lto_state = dal_c__effectiveLtoState(cmd, profile, target_type);
    bool lto_enabled = dal_c_LtoMode_isEnabled(lto_state);
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    const char* target_tune = dal_c__resolvedTargetTune(opts, profile);
    dal_c_ToggleState omit_frame_pointer = dal_c__resolvedOmitFramePointerState(opts, profile);
    bool function_sections = dal_c__resolvedFunctionSections(opts, profile);
    bool data_sections = dal_c__resolvedDataSections(opts, profile);
    dal_c_ToggleState unroll_loops = dal_c__resolvedUnrollLoopsState(opts, profile);
    dal_c_ToggleState unwind_tables = dal_c__resolvedUnwindTablesState(opts, profile);
    dal_c_ToggleState async_unwind_tables = dal_c__resolvedAsyncUnwindTablesState(opts, profile);
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    dal_c_ToggleState merge_all_constants = dal_c__resolvedMergeAllConstantsState(opts);
    dal_c_ToggleState stack_protector = dal_c__resolvedStackProtectorState(opts);
    bool pic_enabled = !is_windows && target_type == dal_c_Target_shared_lib;
    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashBool(hash, is_windows);
    hash = dal_c__hashBytes(hash, &profile->opti_level, sizeof(profile->opti_level));
    hash = dal_c__hashBytes(hash, &profile->debug_level, sizeof(profile->debug_level));
    hash = dal_c__hashBool(hash, profile->debug_assertions);
    hash = dal_c__hashString(hash, opts->compiler);
    hash = dal_c__hashBool(hash, compiler_is_clang);
    if (compiler_is_clang) {
        hash = dal_c__hashString(hash, "color-diagnostics-v1");
        int macro_backtrace_limit = dal_c__resolvedMacroBacktraceLimit(opts);
        hash = dal_c__hashBytes(hash, &macro_backtrace_limit, sizeof(macro_backtrace_limit));
    }
    hash = dal_c__hashString(hash, opts->c_std);
    hash = dal_c__hashString(hash, opts->arch_target);
    hash = dal_c__hashString(hash, target_arch);
    hash = dal_c__hashString(hash, target_tune);
    hash = dal_c__hashString(hash, opts->target_abi);
    hash = dal_c__hashString(hash, opts->sysroot);
    hash = dal_c__hashString(hash, cmd->opts.compiler_args);
    hash = dal_c__hashString(hash, cmd->compiler_args);
    hash = dal_c__hashBytes(hash, &compile_env, sizeof(compile_env));
    hash = dal_c__hashBool(hash, pic_enabled);
    hash = dal_c__hashBool(hash, libc_linked);
    hash = dal_c__hashBool(hash, default_libs_linked);
    hash = dal_c__hashBool(hash, start_files_linked);
    hash = dal_c__hashBool(hash, compiler_rt_linked);
    hash = dal_c__hashBool(hash, stdlib_linked);
    hash = dal_c__hashBool(hash, crt_linked);
    /* Hash only the effective compile surface, not the higher-level DSL/link mode itself.
     * A dependency project such as `dh` can be built both from a sample command and
     * from an external example command. Those commands may differ in whether a DH_PATH
     * helper variable is emitted, while the actual compile command for libdh is
     * byte-for-byte identical. Hashing the raw DSL mode caused separate object
     * directories and forced a second full libdh compile. */
    hash = dal_c__hashBool(hash, proj && dal_c__shouldAddProjectInclude(proj, cmd));
    hash = dal_c__hashBool(hash, proj && dal_c__shouldAddProjectPrivateInclude(proj, cmd));
    hash = dal_c__hashBytes(hash, &opts->lto_mode, sizeof(opts->lto_mode));
    hash = dal_c__hashBytes(hash, &profile->lto_mode, sizeof(profile->lto_mode));
    hash = dal_c__hashBytes(hash, &lto_state, sizeof(lto_state));
    hash = dal_c__hashBool(hash, lto_enabled);
    hash = dal_c__hashBytes(hash, &opts->omit_frame_pointer, sizeof(opts->omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &profile->omit_frame_pointer, sizeof(profile->omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &omit_frame_pointer, sizeof(omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &opts->unroll_loops, sizeof(opts->unroll_loops));
    hash = dal_c__hashBytes(hash, &profile->unroll_loops, sizeof(profile->unroll_loops));
    hash = dal_c__hashBytes(hash, &unroll_loops, sizeof(unroll_loops));
    hash = dal_c__hashBytes(hash, &opts->function_sections, sizeof(opts->function_sections));
    hash = dal_c__hashBytes(hash, &profile->function_sections, sizeof(profile->function_sections));
    hash = dal_c__hashBool(hash, function_sections);
    hash = dal_c__hashBytes(hash, &opts->data_sections, sizeof(opts->data_sections));
    hash = dal_c__hashBytes(hash, &profile->data_sections, sizeof(profile->data_sections));
    hash = dal_c__hashBool(hash, data_sections);
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
    hash = dal_c__hashBytes(hash, &merge_all_constants, sizeof(merge_all_constants));
    hash = dal_c__hashBytes(hash, &opts->stack_protector, sizeof(opts->stack_protector));
    hash = dal_c__hashBytes(hash, &stack_protector, sizeof(stack_protector));
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.save_temps != dal_c_SaveTempsMode_off) {
        hash = dal_c__hashBytes(hash, &cmd->payload.build.save_temps, sizeof(cmd->payload.build.save_temps));
    }
    hash = dal_c__hashBytes(hash, &opts->loose_errors, sizeof(opts->loose_errors));
    hash = dal_c__hashVersionSpec(hash, &opts->version);
    hash = dal_c__hashBool(hash, use_pch);
    if (use_pch && proj) {
        hash = dal_c__hashString(hash, proj->pch_header);
    }
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
    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (dal_c__isLtoFlag(flag)) {
            continue;
        }
        if (!str_startsWith(flag, "-Wl,") && !str_startsWith(flag, "-L") && !str_startsWith(flag, "-l")) {
            hash = dal_c__hashString(hash, flag);
        }
    }

    return str_format("%016llx", (unsigned long long)hash);
}

static char* dal_c__makeLinkContractKey(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);

    const dal_c_CompilerOpts* opts = &cmd->opts;
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    bool lto_enabled = dal_c__effectiveLtoEnabled(cmd, profile, target_type);
    bool use_lld_for_lto = dal_c__shouldUseLldForClangLto(cmd, profile, target_type);
    const char* target_arch = dal_c__resolvedTargetArch(opts, profile);
    dal_c_IcfMode icf_mode = dal_c__resolvedIcfMode(opts, profile);
    uint64_t hash = 1469598103934665603ULL;
    char* target_name = dal_c__resolveTargetDirName(opts);
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashString(hash, opts->compiler ? opts->compiler : dal_c_default_compiler);
    hash = dal_c__hashString(hash, target_name);
    hash = dal_c__hashBytes(hash, &compile_env, sizeof(compile_env));
    hash = dal_c__hashString(hash, cmd->opts.link_args);
    hash = dal_c__hashString(hash, cmd->link_args);
    hash = dal_c__hashString(hash, dal_c__effectiveLinkerScript(cmd));
    hash = dal_c__hashString(hash, dal_c__effectiveObjcopy(cmd));
    hash = dal_c__hashString(hash, dal_c__effectiveObjcopyFormat(cmd));
    hash = dal_c__hashBytes(hash, &target_type, sizeof(target_type));
    hash = dal_c__hashString(hash, opts->entry_symbol);
    hash = dal_c__hashBytes(hash, &opts->link_mode, sizeof(opts->link_mode));
    hash = dal_c__hashString(hash, opts->arch_target);
    hash = dal_c__hashString(hash, target_arch);
    hash = dal_c__hashString(hash, opts->target_abi);
    hash = dal_c__hashString(hash, opts->sysroot);
    hash = dal_c__hashVersionSpec(hash, &opts->version);
    hash = dal_c__hashBool(hash, dal_c__linkUsesTestMode(cmd, target_type));
    hash = dal_c__hashBool(hash, libc_linked);
    hash = dal_c__hashBool(hash, default_libs_linked);
    hash = dal_c__hashBool(hash, start_files_linked);
    hash = dal_c__hashBool(hash, compiler_rt_linked);
    hash = dal_c__hashBytes(hash, &opts->lto_mode, sizeof(opts->lto_mode));
    hash = dal_c__hashBool(hash, lto_enabled);
    hash = dal_c__hashBool(hash, use_lld_for_lto);
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
    bool emit_map = false;
    const char* emit_map_path = NULL;
    bool emit_linked_asm = false;
    const char* emit_linked_asm_path = NULL;
    bool emit_disasm = false;
    const char* emit_disasm_path = NULL;
    bool emit_ir = false;
    const char* emit_ir_path = NULL;
    bool emit_debug_info = false;
    const char* emit_debug_info_path = NULL;
    dal_c_ToggleState disasm_demangle = dal_c_ToggleState_auto;
    dal_c_ToggleState disasm_source = dal_c_ToggleState_auto;
    dal_c_ToggleState disasm_line_numbers = dal_c_ToggleState_auto;
    dal_c_ToggleState disasm_symbolize_operands = dal_c_ToggleState_auto;
    dal_c_ToggleState disasm_raw_insn = dal_c_ToggleState_auto;
    dal_c_ToggleState disasm_section_contents = dal_c_ToggleState_auto;
    bool print_link_gc = false;
    bool analysis_artifacts = false;
    if (cmd->action == dal_c_CmdAction_build) {
        emit_map = cmd->payload.build.emit_map;
        emit_map_path = cmd->payload.build.emit_map_path;
        emit_linked_asm = cmd->payload.build.emit_linked_asm;
        emit_linked_asm_path = cmd->payload.build.emit_linked_asm_path;
        emit_disasm = cmd->payload.build.emit_disasm;
        emit_disasm_path = cmd->payload.build.emit_disasm_path;
        emit_ir = cmd->payload.build.emit_ir;
        emit_ir_path = cmd->payload.build.emit_ir_path;
        emit_debug_info = cmd->payload.build.emit_debug_info;
        emit_debug_info_path = cmd->payload.build.emit_debug_info_path;
        disasm_demangle = cmd->payload.build.disasm_demangle;
        disasm_source = cmd->payload.build.disasm_source;
        disasm_line_numbers = cmd->payload.build.disasm_line_numbers;
        disasm_symbolize_operands = cmd->payload.build.disasm_symbolize_operands;
        disasm_raw_insn = cmd->payload.build.disasm_raw_insn;
        disasm_section_contents = cmd->payload.build.disasm_section_contents;
        print_link_gc = cmd->payload.build.print_link_gc;
        analysis_artifacts = cmd->payload.build.analysis_artifacts;
    }
    hash = dal_c__hashBool(hash, emit_map);
    hash = dal_c__hashString(hash, emit_map_path);
    hash = dal_c__hashBool(hash, emit_linked_asm);
    hash = dal_c__hashString(hash, emit_linked_asm_path);
    hash = dal_c__hashBool(hash, emit_disasm);
    hash = dal_c__hashString(hash, emit_disasm_path);
    hash = dal_c__hashBool(hash, emit_ir);
    hash = dal_c__hashString(hash, emit_ir_path);
    hash = dal_c__hashBool(hash, emit_debug_info);
    hash = dal_c__hashString(hash, emit_debug_info_path);
    hash = dal_c__hashBytes(hash, &disasm_demangle, sizeof(disasm_demangle));
    hash = dal_c__hashBytes(hash, &disasm_source, sizeof(disasm_source));
    hash = dal_c__hashBytes(hash, &disasm_line_numbers, sizeof(disasm_line_numbers));
    hash = dal_c__hashBytes(hash, &disasm_symbolize_operands, sizeof(disasm_symbolize_operands));
    hash = dal_c__hashBytes(hash, &disasm_raw_insn, sizeof(disasm_raw_insn));
    hash = dal_c__hashBytes(hash, &disasm_section_contents, sizeof(disasm_section_contents));
    hash = dal_c__hashBool(hash, print_link_gc);
    hash = dal_c__hashBool(hash, analysis_artifacts);

    for (int i = 0; i < opts->link_count; ++i) {
        hash = dal_c__hashString(hash, opts->link_libs[i]);
    }
    for (int i = 0; i < opts->link_dir_count; ++i) {
        hash = dal_c__hashString(hash, opts->link_dirs[i]);
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

    char* result = str_format("%016llx", (unsigned long long)hash);
    free(target_name);
    return result;
}

char* dal_c__makePrebuiltAbiContractKey(
    const dal_c_Cmd* cmd,
    const dal_c_ProfileSpec* profile,
    dal_c_Target target_type
) {
    assert(cmd != NULL);
    assert(profile != NULL);
    const dal_c_CompilerOpts* opts = &cmd->opts;
    char* target_name = dal_c__resolveTargetDirName(opts);
    dal_c_CompileEnv compile_env = dal_c__resolvedCompileEnv(opts);
    bool libc_linked = dal_c__resolvedLibcLinked(opts);
    bool default_libs_linked = dal_c__resolvedDefaultLibsLinked(opts);
    bool start_files_linked = dal_c__resolvedStartFilesLinked(opts);
    bool compiler_rt_linked = dal_c__resolvedCompilerRtLinked(opts, target_type);
    dal_c_LtoMode lto_state = dal_c__effectiveLtoState(cmd, profile, target_type);
    if (!dal_c_LtoMode_isEnabled(lto_state)) {
        lto_state = dal_c_LtoMode_off;
    }
    bool pic_enabled = !dal_c__platformIsWindows() && target_type == dal_c_Target_shared_lib;
    dal_c_ToggleState omit_frame_pointer = dal_c__resolvedOmitFramePointerState(opts, profile);
    dal_c_ToggleState unwind_tables = dal_c__resolvedUnwindTablesState(opts, profile);
    dal_c_ToggleState async_unwind_tables = dal_c__resolvedAsyncUnwindTablesState(opts, profile);
    dal_c_ToggleState exceptions = dal_c__resolvedExceptionsState(opts, profile);
    dal_c_ToggleState stack_protector = dal_c__resolvedStackProtectorState(opts);

    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, target_name);
    hash = dal_c__hashString(hash, profile->name);
    hash = dal_c__hashBytes(hash, &target_type, sizeof(target_type));
    hash = dal_c__hashString(hash, opts->c_std);
    hash = dal_c__hashString(hash, dal_c__resolvedTargetArch(opts, profile));
    hash = dal_c__hashString(hash, dal_c__resolvedTargetTune(opts, profile));
    hash = dal_c__hashString(hash, opts->target_abi);
    hash = dal_c__hashString(hash, cmd->opts.compiler_args);
    hash = dal_c__hashString(hash, cmd->compiler_args);
    hash = dal_c__hashBytes(hash, &compile_env, sizeof(compile_env));
    hash = dal_c__hashBool(hash, libc_linked);
    hash = dal_c__hashBool(hash, default_libs_linked);
    hash = dal_c__hashBool(hash, start_files_linked);
    hash = dal_c__hashBool(hash, compiler_rt_linked);
    hash = dal_c__hashBool(hash, pic_enabled);
    hash = dal_c__hashBytes(hash, &lto_state, sizeof(lto_state));
    hash = dal_c__hashBytes(hash, &omit_frame_pointer, sizeof(omit_frame_pointer));
    hash = dal_c__hashBytes(hash, &unwind_tables, sizeof(unwind_tables));
    hash = dal_c__hashBytes(hash, &async_unwind_tables, sizeof(async_unwind_tables));
    hash = dal_c__hashBytes(hash, &exceptions, sizeof(exceptions));
    hash = dal_c__hashBytes(hash, &stack_protector, sizeof(stack_protector));
    hash = dal_c__hashBool(hash, profile->debug_assertions);
    for (int i = 0; i < opts->define_count; ++i) {
        hash = dal_c__hashString(hash, opts->define_macros[i]);
    }
    for (int i = 0; i < opts->undef_count; ++i) {
        hash = dal_c__hashString(hash, opts->undef_macros[i]);
    }
    for (int i = 0; profile->extra_flags[i] != NULL; ++i) {
        const char* flag = profile->extra_flags[i];
        if (!dal_c__isLtoFlag(flag)
            && !str_startsWith(flag, "-Wl,")
            && !str_startsWith(flag, "-L")
            && !str_startsWith(flag, "-l")) {
            hash = dal_c__hashString(hash, flag);
        }
    }
    char* result = str_format("%016llx", (unsigned long long)hash);
    free(target_name);
    return result;
}

static char* dal_c__makeLtoToolchainContractKey(const dal_c_CompilerOpts* opts) {
    assert(opts != NULL);
    const char* compiler = opts->compiler && opts->compiler[0]
                             ? opts->compiler
                             : dal_c_default_compiler;
    const char* argv[] = { compiler, "--version", NULL };
    char* version = proc_output(argv);
    if (version) {
        char* newline = strpbrk(version, "\r\n");
        if (newline) { *newline = '\0'; }
    }
    char* target_name = dal_c__resolveTargetDirName(opts);
    char* compiler_name = path_basename(compiler);
    uint64_t hash = 1469598103934665603ULL;
    hash = dal_c__hashString(hash, compiler_name ? compiler_name : compiler);
    hash = dal_c__hashString(hash, version);
    hash = dal_c__hashString(hash, target_name);
    hash = dal_c__hashString(hash, opts->target_abi);
    char* result = str_format("%016llx", (unsigned long long)hash);
    free(compiler_name);
    free(target_name);
    free(version);
    return result;
}

static char* dal_c__makeCompileContractContent(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode, const char* source_path) {
    const dal_c_CompilerOpts* opts = &cmd->opts;
    char* content = NULL;
    char* target = dal_c__resolveTargetDirName(opts);
    char* hash = dal_c__makeCompileContractKey(cmd, proj, profile, target_type, use_pch, test_mode);
    char number[64];
    dal_c__contractAppend(&content, "contract-version", "2");
    dal_c__contractAppend(&content, "kind", "compile");
    dal_c__contractAppend(&content, "hash", hash);
    dal_c__contractAppend(&content, "profile", profile->name);
    dal_c__contractAppend(&content, "target", target);
    dal_c__contractAppend(&content, "compiler", opts->compiler ? opts->compiler : "auto");
    dal_c__contractAppend(&content, "source", source_path);
    dal_c__contractAppend(&content, "c-standard", opts->c_std);
    dal_c__contractAppend(&content, "target-arch", dal_c__resolvedTargetArch(opts, profile));
    dal_c__contractAppend(&content, "target-tune", dal_c__resolvedTargetTune(opts, profile));
    dal_c__contractAppend(&content, "target-abi", opts->target_abi);
    dal_c__contractAppend(&content, "sysroot", opts->sysroot);
    char* configured_comp_args = dal_c__mergeConfiguredArgs(cmd->opts.compiler_args, cmd->compiler_args);
    dal_c__contractAppend(&content, "compiler-args", configured_comp_args);
    free(configured_comp_args);
    (void)snprintf(number, sizeof(number), "%d", dal_c__resolvedMacroBacktraceLimit(opts));
    dal_c__contractAppend(&content, "macro-backtrace-limit", number);
    dal_c__contractAppend(&content, "compile-env", dal_c_CompileEnv_format(dal_c__resolvedCompileEnv(opts)));
    dal_c__contractAppendBool(&content, "libc-fact", dal_c__resolvedCompileLibcFact(opts));
    dal_c__contractAppendBool(&content, "default-libs-fact", dal_c__resolvedCompileDefaultLibsFact(opts));
    dal_c__contractAppendBool(&content, "start-files-fact", dal_c__resolvedCompileStartFilesFact(opts));
    dal_c__contractAppendBool(&content, "compiler-rt-fact", dal_c__resolvedCompileCompilerRtFact(opts, target_type));
    dal_c__contractAppendBool(&content, "crt-fact", dal_c__resolvedCompileCrtFact(opts));
    dal_c__contractAppendBool(&content, "stdlib-fact", dal_c__resolvedCompileStdlibFact(opts));
    (void)snprintf(number, sizeof(number), "%d", (int)profile->opti_level);
    dal_c__contractAppend(&content, "optimization", number);
    (void)snprintf(number, sizeof(number), "%d", (int)profile->debug_level);
    dal_c__contractAppend(&content, "debug-level", number);
    dal_c__contractAppendBool(&content, "debug-assertions", profile->debug_assertions);
    dal_c__contractAppendBool(&content, "lto", dal_c__effectiveLtoEnabled(cmd, profile, target_type));
    dal_c__contractAppendBool(&content, "use-pch", use_pch);
    dal_c__contractAppend(&content, "pch-header", use_pch && proj ? proj->pch_header : NULL);
    dal_c__contractAppendBool(&content, "test-mode", test_mode);
    dal_c__contractAppendBool(&content, "project-include", proj && dal_c__shouldAddProjectInclude(proj, cmd));
    dal_c__contractAppendBool(&content, "private-include", proj && dal_c__shouldAddProjectPrivateInclude(proj, cmd));
    for (int i = 0; i < opts->define_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "define.%d", i);
        dal_c__contractAppend(&content, key, opts->define_macros[i]);
    }
    for (int i = 0; i < opts->undef_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "undef.%d", i);
        dal_c__contractAppend(&content, key, opts->undef_macros[i]);
    }
    for (int i = 0; i < opts->include_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "include.%d", i);
        dal_c__contractAppend(&content, key, opts->include_paths[i]);
    }
    for (int i = 0; i < opts->isystem_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "isystem.%d", i);
        dal_c__contractAppend(&content, key, opts->isystem_paths[i]);
    }
    free(hash);
    free(target);
    return content;
}

static void dal_c__writeCompileContractFile(const char* object_dir, const char* base, const char* src, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, bool use_pch, bool test_mode) {
    char* contracts = path_join(object_dir, ".contracts");
    uint64_t source_hash = 1469598103934665603ULL;
    source_hash = dal_c__hashString(source_hash, base);
    source_hash = dal_c__hashString(source_hash, src);
    char* name = str_format("%016llx.contract", (unsigned long long)source_hash);
    char* path = path_join(contracts, name);
    char* previous = path_isFile(path) ? file_read(path) : NULL;
    char* content = dal_c__makeCompileContractContent(cmd, proj, profile, target_type, use_pch, test_mode, src);
    if (!previous || !content || !str_eql(previous, content)) { dal_c__recordContractDiff("compile", previous, content); }
    if (!cmd->dry_run) {
        dir_createRecur(contracts);
        if (content) { (void)dal_c__writeFileIfChanged(path, content); }
    }
    free(content);
    free(previous);
    free(path);
    free(name);
    free(contracts);
}

static char* dal_c__makeLinkContractContent(const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* target_path) {
    const dal_c_CompilerOpts* opts = &cmd->opts;
    char* content = NULL;
    char* target = dal_c__resolveTargetDirName(opts);
    char* hash = dal_c__makeLinkContractKey(cmd, profile, target_type);
    char number[64];
    dal_c__contractAppend(&content, "contract-version", "2");
    dal_c__contractAppend(&content, "kind", "link");
    dal_c__contractAppend(&content, "artifact", target_path);
    dal_c__contractAppend(&content, "hash", hash);
    dal_c__contractAppend(&content, "profile", profile->name);
    dal_c__contractAppend(&content, "target", target);
    dal_c__contractAppend(&content, "artifact-kind", dal_c_Target_format(target_type));
    dal_c__contractAppend(&content, "compiler", opts->compiler ? opts->compiler : "auto");
    dal_c__contractAppend(&content, "target-arch", dal_c__resolvedTargetArch(opts, profile));
    dal_c__contractAppend(&content, "target-abi", opts->target_abi);
    dal_c__contractAppend(&content, "sysroot", opts->sysroot);
    dal_c__contractAppend(&content, "entry", opts->entry_symbol);
    char* configured_link_args = dal_c__mergeConfiguredArgs(cmd->opts.link_args, cmd->link_args);
    dal_c__contractAppend(&content, "link-args", configured_link_args);
    dal_c__contractAppend(&content, "linker-script", dal_c__effectiveLinkerScript(cmd));
    free(configured_link_args);
    dal_c__contractAppend(&content, "compile-env", dal_c_CompileEnv_format(dal_c__resolvedCompileEnv(opts)));
    dal_c__contractAppendBool(&content, "test-mode", dal_c__linkUsesTestMode(cmd, target_type));
    dal_c__contractAppendBool(&content, "driver-libc", dal_c__resolvedLibcLinked(opts));
    dal_c__contractAppendBool(&content, "driver-default-libs", dal_c__resolvedDefaultLibsLinked(opts));
    dal_c__contractAppendBool(&content, "driver-start-files", dal_c__resolvedStartFilesLinked(opts));
    dal_c__contractAppendBool(&content, "driver-compiler-rt", dal_c__resolvedCompilerRtLinked(opts, target_type));
    dal_c__contractAppendBool(&content, "lto", dal_c__effectiveLtoEnabled(cmd, profile, target_type));
    dal_c__contractAppendBool(&content, "gc-sections", dal_c__resolvedGcSections(opts, profile));
    dal_c__contractAppendBool(&content, "whole-archive", dal_c__resolvedWholeArchive(opts, profile));
    dal_c__contractAppendBool(&content, "strip", dal_c__resolvedStripMode(opts, profile));
    (void)snprintf(number, sizeof(number), "%d", (int)(dal_c__resolvedIcfMode(opts, profile)));
    dal_c__contractAppend(&content, "icf-mode", number);
    for (int i = 0; i < opts->link_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "link-lib.%d", i);
        dal_c__contractAppend(&content, key, opts->link_libs[i]);
    }
    for (int i = 0; i < opts->link_dir_count; ++i) {
        char key[64];
        (void)snprintf(key, sizeof(key), "link-dir.%d", i);
        dal_c__contractAppend(&content, key, opts->link_dirs[i]);
    }
    free(hash);
    free(target);
    return content;
}

static char* dal_c__makeLinkContractPath(const char* build_dir, const char* target_path) {
    assert(build_dir != NULL);
    assert(target_path != NULL);

    char* contracts_dir = path_join(build_dir, ".link");
    uint64_t target_hash = 1469598103934665603ULL;
    target_hash = dal_c__hashString(target_hash, target_path);
    char* contract_name = str_format("%016llx.contract", (unsigned long long)target_hash);
    char* contract_path = path_join(contracts_dir, contract_name);
    free(contract_name);
    free(contracts_dir);
    return contract_path;
}

static bool dal_c__writeLinkContractFile(const char* path, const char* target_path, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type) {
    assert(path != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);

    char* parent = path_parent(path);
    if (!cmd->dry_run) { dir_createRecur(parent); }
    free(parent);

    char* previous = path_isFile(path) ? file_read(path) : NULL;
    char* content = dal_c__makeLinkContractContent(cmd, profile, target_type, target_path);
    bool changed = !previous || !content || !str_eql(previous, content);
    if (changed) { dal_c__recordContractDiff("link", previous, content); }
    bool success = content && (cmd->dry_run || dal_c__writeFileIfChanged(path, content));
    free(content);
    free(previous);
    return success;
}

static char* dal_c__makeObjectPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* base, const char* src, bool use_pch, bool test_mode) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(object_dir != NULL);
    assert(src != NULL);

    dal_c__writeCompileContractFile(object_dir, base, src, cmd, proj, profile, target_type, use_pch, test_mode);
    char* contract_key = dal_c__makeCompileContractKey(cmd, proj, profile, target_type, use_pch, test_mode);
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

static bool dal_c__archiveResponseReserve(char** content, size_t* capacity, size_t required) {
    if (!content || !capacity) { return false; }
    if (*capacity >= required) { return true; }
    size_t next_capacity = *capacity ? *capacity : 1024;
    while (next_capacity < required) {
        if (next_capacity > SIZE_MAX / 2) {
            next_capacity = required;
            break;
        }
        next_capacity *= 2;
    }
    char* grown = (char*)realloc(*content, next_capacity);
    if (!grown) { return false; }
    *content = grown;
    *capacity = next_capacity;
    return true;
}

static bool dal_c__objectResponseAppend(
    char** content,
    size_t* length,
    size_t* capacity,
    const char* path
) {
    if (!content || !length || !capacity || !path) { return false; }
    size_t path_length = strlen(path);
    if (*length > SIZE_MAX - 4 || path_length > (SIZE_MAX - *length - 4) / 2) { return false; }
    size_t required = *length + path_length * 2 + 4;
    if (!dal_c__archiveResponseReserve(content, capacity, required)) { return false; }

    (*content)[(*length)++] = '"';
    for (size_t i = 0; i < path_length; ++i) {
        char c = path[i];
        if (c == '\\') {
            (*content)[(*length)++] = '/';
        } else {
            if (c == '"') { (*content)[(*length)++] = '\\'; }
            (*content)[(*length)++] = c;
        }
    }
    (*content)[(*length)++] = '"';
    (*content)[(*length)++] = '\n';
    (*content)[*length] = '\0';
    return true;
}

static char* dal_c__makePchPath(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* object_dir, const char* ext) {
    assert(cmd != NULL);
    assert(proj != NULL);
    assert(profile != NULL);
    assert(object_dir != NULL);
    assert(ext != NULL);

    if (!proj->pch_header) { return NULL; }
    char* pch_basename = path_basename(proj->pch_header);
    char* pch_contract_key = dal_c__makeCompileContractKey(cmd, proj, profile, target_type, true, false);
    char* pch_path = str_format("%s/%s_%s%s", object_dir, pch_contract_key, pch_basename, ext);
    free(pch_contract_key);
    free(pch_basename);
    return pch_path;
}

static bool dal_c__pchHeaderIsDHInstallHeader(const dal_c_Project* proj) {
    if (!proj || !proj->dh_path || !proj->pch_header) { return false; }
    char* include_dir = path_join(proj->dh_path, dal_c_dir_include);
    char* rel = include_dir ? path_relative(include_dir, proj->pch_header) : NULL;
    bool is_dh_header = rel != NULL
                     && rel[0] != '\0'
                     && !str_startsWith(rel, "..")
                     && !dal_c__pathIsAbsolute(rel);
    free(rel);
    free(include_dir);
    return is_dh_header;
}

static bool dal_c__pchEnabled(const dal_c_Project* proj) {
    if (!proj || !proj->pch_header) { return false; }
    return path_isFile(proj->pch_header);
}

static bool dal_c__pchEnabledForOpts(const dal_c_Project* proj, const dal_c_CompilerOpts* opts) {
    if (!dal_c__pchEnabled(proj)) { return false; }
    if (dal_c__pchHeaderIsDHInstallHeader(proj) && !dal_c__usesDHLibrary(proj, opts)) {
        return false;
    }
    return true;
}


static char* dal_c__depPathForObject(const char* obj_path) {
    assert(obj_path != NULL);
    char* dep_path = strdup(obj_path);
    if (!dep_path) { return NULL; }
    char* dot = strrchr(dep_path, '.');
    if (dot && str_eql(dot, ".o")) {
        strcpy(dot, ".d");
    } else {
        free(dep_path);
        dep_path = str_format("%s.d", obj_path);
    }
    return dep_path;
}

static bool dal_c__pathMTimeAtLeast(const char* path, time_t minimum) {
    time_t mt = file_mtime(path);
    return mt != 0 && mt >= minimum;
}

static bool dal_c__depFileEntriesAreUpToDate(const char* dep_path, time_t output_mt) {
    assert(dep_path != NULL);

    char* deps = file_read(dep_path);
    if (!deps) {
        return false;
    }
    bool ok = true;
    bool in_deps = false;
    char* cursor = deps;
    while (*cursor != '\0') {
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') { ++cursor; }
        if (*cursor == '\0') { break; }
        if (*cursor == '\\') {
            ++cursor;
            if (*cursor == '\r') { ++cursor; }
            if (*cursor == '\n') { ++cursor; }
            continue;
        }
        char* start = cursor;
        while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && *cursor != '\r' && *cursor != '\n') { ++cursor; }
        char saved = *cursor;
        *cursor = '\0';

        if (!in_deps) {
            char* colon = strrchr(start, ':');
            if (colon) {
                in_deps = true;
                if (colon[1] != '\0') {
                    start = colon + 1;
                } else {
                    *cursor = saved;
                    continue;
                }
            } else {
                *cursor = saved;
                continue;
            }
        }

        size_t len = strlen(start);
        while (len > 0 && start[len - 1] == '\\') {
            start[--len] = '\0';
        }
        if (len > 0) {
            if (start[len - 1] == ':') {
                *cursor = saved;
                break;
            }
            time_t dep_mt = file_mtime(start);
            if (dep_mt == 0 || dep_mt > output_mt) {
                ok = false;
                *cursor = saved;
                break;
            }
        }
        *cursor = saved;
    }

    free(deps);
    return ok;
}

static bool dal_c__objectDepsAreUpToDate(const char* obj_path, const char* src_path) {
    assert(obj_path != NULL);
    assert(src_path != NULL);

    time_t obj_mt = file_mtime(obj_path);
    if (obj_mt == 0) { return false; }
    if (!dal_c__pathMTimeAtLeast(src_path, 0)) { return false; }
    if (file_mtime(src_path) > obj_mt) { return false; }

    char* dep_path = dal_c__depPathForObject(obj_path);
    if (!dep_path) { return false; }
    bool ok = dal_c__depFileEntriesAreUpToDate(dep_path, obj_mt);
    free(dep_path);
    return ok;
}

static bool dal_c__pchDepsAreUpToDate(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* object_dir, dal_c_Target target_type) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(object_dir != NULL);

    if (!proj || !proj->pch_header) { return true; }

    char* pch_out = dal_c__makePchPath(cmd, proj, profile, target_type, object_dir, ".pch");
    char* pch_dep = dal_c__makePchPath(cmd, proj, profile, target_type, object_dir, ".d");
    if (!pch_out || !pch_dep) {
        free(pch_dep);
        free(pch_out);
        return false;
    }

    time_t pch_mt = file_mtime(pch_out);
    bool ok = pch_mt != 0
           && dal_c__pathMTimeAtLeast(proj->pch_header, 0)
           && file_mtime(proj->pch_header) <= pch_mt
           && dal_c__depFileEntriesAreUpToDate(pch_dep, pch_mt);

    free(pch_dep);
    free(pch_out);
    return ok;
}

static bool dal_c__sourcesUsePch(const dal_c_Project* proj, ArrStr* sources, bool has_pch) {
    assert(sources != NULL);

    int src_count = ArrStr_len(sources);
    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        bool is_assembly = dal_c__sourceIsAssembly(src);
        bool use_pch = !is_assembly && has_pch && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        if (use_pch) { return true; }
    }
    return false;
}

static bool dal_c__linkedPlanHasExtraArtifacts(const dal_c_Cmd* cmd) {
    if (!cmd || cmd->action != dal_c_CmdAction_build) { return false; }
    return cmd->payload.build.emit_linked_asm
        || cmd->payload.build.emit_disasm
        || cmd->payload.build.emit_ir
        || cmd->payload.build.emit_debug_info
        || cmd->payload.build.emit_preprocessed
        || cmd->payload.build.emit_asm
        || cmd->payload.build.as_image;
}

static bool dal_c__linkedPlanIsUpToDate(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    bool has_pch,
    const char* object_dir,
    const char* base,
    dal_c_Target target_type,
    const char* target_path,
    const char* link_contract_path
) {
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(sources != NULL);
    assert(object_dir != NULL);
    assert(target_path != NULL);

    if (dal_c__targetIsEmitOnly(target_type) || dal_c__linkedPlanHasExtraArtifacts(cmd)) {
        return false;
    }

    time_t target_mt = file_mtime(target_path);
    if (target_mt == 0) { return false; }
    if (dal_c__platformIsWindows() && target_type == dal_c_Target_shared_lib) {
        char* import_lib_path = dal_c__makeSharedImportLibraryPath(target_path);
        bool import_ok = import_lib_path && path_isFile(import_lib_path);
        free(import_lib_path);
        if (!import_ok) { return false; }
    }
    if (link_contract_path) {
        time_t contract_mt = file_mtime(link_contract_path);
        if (contract_mt == 0 || contract_mt > target_mt) { return false; }
        char* existing_contract = file_read(link_contract_path);
        char* requested_contract = dal_c__makeLinkContractContent(cmd, profile, target_type, target_path);
        bool contract_matches = existing_contract && requested_contract && str_eql(existing_contract, requested_contract);
        free(requested_contract);
        free(existing_contract);
        if (!contract_matches) { return false; }
    }

    ArrStr* link_deps = dal_c__collectLinkDependencyPaths(cmd, proj, profile, target_type);
    if (!link_deps) { return false; }
    for (int i = 0; i < ArrStr_len(link_deps); ++i) {
        const char* dep = ArrStr_at(link_deps, i);
        time_t dep_mt = file_mtime(dep);
        if (dep_mt == 0 || dep_mt > target_mt) {
            ArrStr_fini(&link_deps);
            return false;
        }
    }
    ArrStr_fini(&link_deps);

    int src_count = ArrStr_len(sources);
    bool plan_uses_pch = dal_c__sourcesUsePch(proj, sources, has_pch);
    if (plan_uses_pch && !dal_c__pchDepsAreUpToDate(cmd, proj, profile, object_dir, target_type)) {
        return false;
    }

    for (int i = 0; i < src_count; ++i) {
        const char* src = ArrStr_at(sources, i);
        bool is_assembly = dal_c__sourceIsAssembly(src);
        bool use_pch = !is_assembly && has_pch && !dal_c__sourceUsesPchExcludedHeader(proj, src);
        bool test_mode = dal_c__sourceUsesTestMode(cmd, proj, src);
        char* obj_path = dal_c__makeObjectPath(cmd, proj, profile, target_type, object_dir, base, src, use_pch, test_mode);
        bool obj_ok = obj_path && dal_c__objectDepsAreUpToDate(obj_path, src) && file_mtime(obj_path) <= target_mt;
        free(obj_path);
        if (!obj_ok) { return false; }
    }
    return true;
}

static dal_c__noinline int dal_c__writeEmitOnlyMakefile(
    FILE* fp,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    ArrStr* sources,
    const char* target_path,
    dal_c_Target target_type,
    char* makefile_tmp,
    char* makefile_path,
    char* makefile_dir,
    char* link_contract_path
) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(sources != NULL);
    assert(target_path != NULL);

    int src_count = ArrStr_len(sources);
    bool has_pch = dal_c__pchEnabledForOpts(proj, &cmd->opts);
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

    dal_c__ensureParentDir(target_path);
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
    (void)fprintf(
        fp,
        "\t$(Q)$(CC) %s %s $(SRC) -o $@\n\n",
        emit_flags,
        target_type == dal_c_Target_preprocessed ? "-E" : "-S"
    );
    (void)fprintf(fp, "clean:\n\t$(RM) $(TARGET)\n\n");

    free(emit_flags);
    (void)fclose(fp);
    char* generated = file_read(makefile_tmp);
    bool ok = generated != NULL && dal_c__writeFileIfChanged(makefile_path, generated);
    free(generated);
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
    bool has_pch = dal_c__pchEnabledForOpts(proj, &cmd->opts);
    bool is_windows = dal_c__platformIsWindows();
    const char* obj_base = (proj && proj->root) ? proj->root : NULL;
    char* object_rsp_path = str_format("%s.rsp", target_path);
    char* object_rsp_content = NULL;
    size_t object_rsp_length = 0;
    size_t object_rsp_capacity = 0;
    if (!object_rsp_path) {
        (void)fclose(fp);
        (void)remove(makefile_tmp);
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }

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
        char* obj_path = dal_c__makeObjectPath(cmd, proj, profile, target_type, build_dir, obj_base, src, use_pch, test_mode);
        dal_c__ensureParentDir(obj_path);
        (void)fprintf(fp, " ");
        dal_c__fprintMakePath(fp, obj_path);
        if (!dal_c__objectResponseAppend(
                &object_rsp_content,
                &object_rsp_length,
                &object_rsp_capacity,
                obj_path
            )) {
            free(obj_path);
            free(object_rsp_content);
            free(object_rsp_path);
            (void)fclose(fp);
            (void)remove(makefile_tmp);
            free(link_contract_path);
            free(makefile_tmp);
            free(makefile_dir);
            free(makefile_path);
            return 1;
        }
        free(obj_path);
    }
    (void)fprintf(fp, "\n");

    dal_c__ensureParentDir(object_rsp_path);
    if (!object_rsp_content) {
        object_rsp_content = strdup("");
    }
    if (!object_rsp_content || !dal_c__writeFileIfChanged(object_rsp_path, object_rsp_content)) {
        free(object_rsp_content);
        free(object_rsp_path);
        (void)fclose(fp);
        (void)remove(makefile_tmp);
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        return 1;
    }
    (void)fprintf(fp, "%s = ", target_type == dal_c_Target_static_lib ? "AR_RSP" : "LINK_RSP");
    dal_c__fprintMakePath(fp, object_rsp_path);
    (void)fprintf(fp, "\n");
    free(object_rsp_content);

    (void)fprintf(fp, "DEPS = $(OBJS:.o=.d)\n");
    ArrStr* link_deps = dal_c__collectLinkDependencyPaths(cmd, proj, profile, target_type);
    (void)fprintf(fp, "LINK_DEPS =");
    if (link_deps) {
        for (int i = 0; i < ArrStr_len(link_deps); ++i) {
            (void)fprintf(fp, " ");
            dal_c__fprintMakePath(fp, ArrStr_at(link_deps, i));
        }
        ArrStr_fini(&link_deps);
    }
    (void)fprintf(fp, "\n\n");
    dal_c__ensureParentDir(target_path);
    dal_c__writeMakefileTargetVar(fp, target_path);
    if (is_windows && target_type == dal_c_Target_shared_lib) {
        char* import_lib_path = dal_c__makeSharedImportLibraryPath(target_path);
        dal_c__ensureParentDir(import_lib_path);
        (void)fprintf(fp, "IMPORT_LIB = ");
        dal_c__fprintMakePath(fp, import_lib_path);
        (void)fprintf(fp, "\n");
        free(import_lib_path);
    }
    (void)fprintf(fp, "EXTRA_TARGETS =\n");
    bool emits_disasm = cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_disasm;
    bool disasm_needs_unstripped_target = emits_disasm && dal_c__resolvedStripMode(&cmd->opts, profile);
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_linked_asm) {
        char* linked_asm_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_linked_asm_path, ".linked.s");
        dal_c__ensureParentDir(linked_asm_path);
        (void)fprintf(fp, "LINKED_ASM = ");
        dal_c__fprintMakePath(fp, linked_asm_path);
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "EXTRA_TARGETS += $(LINKED_ASM)\n");
        free(linked_asm_path);
    }
    if (emits_disasm) {
        char* disasm_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_disasm_path, ".disasm.s");
        dal_c__ensureParentDir(disasm_path);
        (void)fprintf(fp, "DISASM = ");
        dal_c__fprintMakePath(fp, disasm_path);
        (void)fprintf(fp, "\n");
        if (disasm_needs_unstripped_target) {
            char* disasm_target_path = dal_c__artifactPath(target_path, NULL, is_windows ? ".disasm.exe" : ".disasm.out");
            dal_c__ensureParentDir(disasm_target_path);
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
        dal_c__ensureParentDir(ir_path);
        (void)fprintf(fp, "IR = ");
        dal_c__fprintMakePath(fp, ir_path);
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "EXTRA_TARGETS += $(IR)\n");
        free(ir_path);
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_debug_info) {
        char* debug_info_path = dal_c__artifactPath(target_path, cmd->payload.build.emit_debug_info_path, ".debug.txt");
        dal_c__ensureParentDir(debug_info_path);
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
    dal_c__writeMakefileTargetRule(fp, cmd, profile, target_type, is_windows, link_contract_path);

    if (is_windows && target_type == dal_c_Target_shared_lib) {
        (void)fprintf(fp, "clean:\n\t$(RM) $(TARGET) $(IMPORT_LIB) $(EXTRA_TARGETS)\n\n");
    } else {
        (void)fprintf(fp, "clean:\n\t$(RM) $(TARGET) $(EXTRA_TARGETS)\n\n");
    }
    (void)fprintf(fp, "-include $(DEPS)\n");

    (void)fclose(fp);

    char* generated = file_read(makefile_tmp);
    if (!generated || !dal_c__writeFileIfChanged(makefile_path, generated)) {
        free(generated);
        (void)remove(makefile_tmp);
        free(link_contract_path);
        free(makefile_tmp);
        free(makefile_dir);
        free(makefile_path);
        free(object_rsp_path);
        return 1;
    }

    free(generated);
    (void)remove(makefile_tmp);
    int result = dal_c__linkedPlanIsUpToDate(cmd, proj, profile, sources, has_pch, build_dir, obj_base, target_type, target_path, link_contract_path)
                   ? dal_c_generateMakefile_upToDate
                   : dal_c_generateMakefile_success;
    free(link_contract_path);
    free(makefile_tmp);
    free(makefile_dir);
    free(makefile_path);
    free(object_rsp_path);
    return result;
}

static dal_c__noinline void dal_c__writeMakefilePCH(FILE* fp, const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const char* build_dir, dal_c_Target target_type) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);
    assert(build_dir != NULL);
    if (!dal_c__pchEnabledForOpts(proj, &cmd->opts)) {
        (void)fprintf(fp, "PCH_OUT =\n\n");
        (void)fprintf(fp, "CFLAGS = $(CFLAGS_NO_PCH)\n\n");
        return;
    }

    char* pch_out = dal_c__makePchPath(cmd, proj, profile, target_type, build_dir, ".pch");
    char* pch_dep = dal_c__makePchPath(cmd, proj, profile, target_type, build_dir, ".d");
    assert(pch_out != NULL);
    assert(pch_dep != NULL);
    dal_c__ensureParentDir(pch_out);

    (void)fprintf(fp, "PCH_SRC = ");
    dal_c__fprintMakePath(fp, proj->pch_header);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "PCH_OUT = ");
    dal_c__fprintMakePath(fp, pch_out);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "CFLAGS = $(CFLAGS_PCH) -include-pch $(PCH_OUT)\n\n");

    // PCH dependency file tracks all headers included by PCH_SRC
    (void)fprintf(fp, "PCH_DEP = ");
    dal_c__fprintMakePath(fp, pch_dep);
    (void)fprintf(fp, "\n");
    (void)fprintf(fp, "-include $(PCH_DEP)\n\n");

    (void)fprintf(fp, "$(PCH_OUT): $(PCH_SRC)\n");
    (void)fprintf(fp, "\t$(Q)$(call P_PCH,$<)tmp=\"$(PCH_OUT).tmp.$$$$\"; dep=\"$(PCH_DEP).tmp.$$$$\"; $(RM) \"$$tmp\" \"$$dep\"; if $(CC) $(CFLAGS_PCH) -MMD -MP -MQ \"$(PCH_OUT)\" -MF \"$$dep\" -x c-header $< -o \"$$tmp\"; then $(MV) \"$$tmp\" \"$@\" && $(MV) \"$$dep\" \"$(PCH_DEP)\" || { status=$$?; $(RM) \"$$tmp\" \"$$dep\"; exit $$status; }; else status=$$?; $(RM) \"$$tmp\" \"$$dep\"; exit $$status; fi\n\n");

    free(pch_dep);
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
        char* obj_path = dal_c__makeObjectPath(cmd, proj, profile, target_type, object_dir, base, src, use_pch, test_mode);
        dal_c__ensureParentDir(obj_path);

        const char* cflags_base = use_pch ? "$(CFLAGS)" : "$(CFLAGS_NO_PCH)";
        char* cflags = test_mode ? str_format("%s -DCOMP_TEST", cflags_base) : strdup(cflags_base);

        dal_c__fprintMakePath(fp, obj_path);
        (void)fprintf(fp, ": ");
        dal_c__fprintMakePath(fp, src);
        if (use_pch) {
            (void)fprintf(fp, " $(PCH_OUT)");
        }
        (void)fprintf(fp, "\n");
        (void)fprintf(fp, "\t$(Q)$(call %s,%d,%d,", is_assembly ? "P_AS" : "P_CC", i + 1, src_count);
        dal_c__fprintMakePath(fp, src);
        (void)fprintf(fp, ")$(CC) %s -MMD -MP -MQ \"$@\" -c ", cflags);
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

static dal_c__noinline void dal_c__writeMakefileTargetRule(FILE* fp, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target type, bool is_windows, const char* link_contract_path) {
    assert(fp != NULL);
    assert(cmd != NULL);
    assert(profile != NULL);

    if (type == dal_c_Target_executable) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        }
        (void)fprintf(fp, "\t$(Q)$(call P_LD,$@)$(CC) \"@$(LINK_RSP)\" -o $@ $(LDFLAGS)\n");
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_linked_asm) {
            if (link_contract_path) {
                (void)fprintf(fp, "\n$(LINKED_ASM): $(LINK_CONTRACT) $(OBJS) $(LINK_DEPS)\n");
            } else {
                (void)fprintf(fp, "\n$(LINKED_ASM): $(OBJS) $(LINK_DEPS)\n");
            }
            (void)fprintf(fp, "\t$(Q)$(call P_GEN,$@)$(CC) \"@$(LINK_RSP)\" -o \"$@\" $(LDFLAGS) -Wl,--lto-emit-asm\n");
        }
        if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_disasm) {
            if (dal_c__resolvedStripMode(&cmd->opts, profile)) {
                if (link_contract_path) {
                    (void)fprintf(fp, "\n$(DISASM_TARGET): $(LINK_CONTRACT) $(OBJS) $(LINK_DEPS)\n");
                } else {
                    (void)fprintf(fp, "\n$(DISASM_TARGET): $(OBJS) $(LINK_DEPS)\n");
                }
                (void)fprintf(fp, "\t$(Q)$(call P_LD,$@)$(CC) \"@$(LINK_RSP)\" -o \"$@\" $(LDFLAGS_DISASM)");
                if (is_windows && profile->debug_level != dal_c_DebugLevel_none) {
                    (void)fprintf(fp, " -Wl,--pdb=$(DISASM_PDB)");
                }
                (void)fprintf(fp, "\n");
            }
            (void)fprintf(fp, "\n$(DISASM): $(DISASM_INPUT)\n");
            (void)fprintf(fp, "\t$(Q)$(call P_GEN,$@)%s -d", dal_c__externalToolPath(dal_c_ExternalTool_llvm_objdump));
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
            if (is_windows) {
                (void)fprintf(fp, "\t$(Q)$(call P_GEN,$@)%s dump -symbols -globals -publics \"$(PDB)\" > \"$@\"\n", dal_c__externalToolPath(dal_c_ExternalTool_llvm_pdbutil));
            } else {
                (void)fprintf(fp, "\t$(Q)$(call P_GEN,$@)%s --debug-info --debug-line \"$(TARGET)\" > \"$@\"\n", dal_c__externalToolPath(dal_c_ExternalTool_llvm_dwarfdump));
            }
        }
    } else if (type == dal_c_Target_static_lib) {
        (void)fprintf(fp, "$(TARGET): $(OBJS) $(AR_RSP)\n");
        (void)fprintf(fp, "\t$(Q)$(call P_AR,$@)$(AR) rcs \"$@\" \"@$(AR_RSP)\"\n");
    } else if (type == dal_c_Target_shared_lib) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        }
        (void)fprintf(fp, "\t$(Q)$(call P_LD,$@)$(CC) -shared -fPIC \"@$(LINK_RSP)\" -o $@ $(LDFLAGS)");
        if (is_windows) {
            (void)fprintf(fp, " -Wl,--out-implib,$(IMPORT_LIB)");
        }
        (void)fprintf(fp, "\n");
    } else if (type == dal_c_Target_image) {
        if (link_contract_path) {
            (void)fprintf(fp, "$(LINK_TARGET): $(LINK_CONTRACT) $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        } else {
            (void)fprintf(fp, "$(LINK_TARGET): $(OBJS) $(LINK_DEPS) $(LINK_RSP)\n");
        }
        (void)fprintf(fp, "\t$(Q)$(call P_LD,$@)$(CC) \"@$(LINK_RSP)\" -o $@ $(LDFLAGS)\n\n");
        if (link_contract_path) {
            (void)fprintf(fp, "$(TARGET): $(LINK_CONTRACT) $(LINK_TARGET)\n");
        } else {
            (void)fprintf(fp, "$(TARGET): $(LINK_TARGET)\n");
        }
        (void)fprintf(fp, "\t$(Q)$(call P_OBJCOPY,$@)$(OBJCOPY) -O $(OBJCOPY_FORMAT) $(LINK_TARGET) $@\n");
    }
    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.emit_ir) {
        (void)fprintf(fp, "\n$(IR): $(SRCS)\n");
        (void)fprintf(fp, "\t$(Q)$(call P_GEN,$@)$(CC) $(CFLAGS_NO_PCH) -S -emit-llvm \"$(firstword $(SRCS))\" -o \"$@\"\n");
    }
    (void)fprintf(fp, "\n");
}
