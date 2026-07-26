#include "internal.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/proc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#if !defined(_WIN32)
#include <unistd.h>
#endif

// === PRIVATE HELPERS ===

static char* dal_c_Project__trimInPlace(char* text);
static char* dal_c_Project__findRoot(const char* start);
static char* dal_c_Project__findWorkspaceRoot(const char* start);
static char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd);
static char* dal_c_Project__normalizeDHPath(const char* path);
static bool dal_c_Project__isDHRoot(const char* path);
static bool dal_c_Project__parseProjectDH(const char* path, dal_c_Project* proj);
static bool dal_c_Project__configureAdHocUnit(dal_c_Project* proj, const dal_c_Cmd* cmd);
static bool dal_c_Project__parseUnitDH(const char* path, dal_c_Project* proj);
static char* dal_c_Project__primarySource(const dal_c_Cmd* cmd);
static char* dal_c_Project__pathWithoutExtension(const char* path);
static unsigned long long dal_c_Project__hashText(const char* text);
static bool dal_c_Project__applyFlatDHFile(dal_c_CompilerOpts* opts, dal_c_BuildDefaults* defaults, const char* path, const char* kind_name);
static bool dal_c_Project__isBuildDefaultsKey(const char* key);
static bool dal_c_Project__isPropertyKey(const char* key);
static bool dal_c_Project__isLibraryKey(const char* key);
static bool dal_c_Project__isTargetRootKey(const char* key);
static bool dal_c_Project__validateBuildDefaultsValue(const char* key, const char* value);
static bool dal_c_Project__validatePropertyValue(const char* key, const char* value);
static bool dal_c_Project__validateLibraryValue(const char* key, const char* value);
static bool dal_c_Project__validateTargetRootValue(const char* key, const char* value);
static bool dal_c_Project__isPropertyPathKey(const char* key);
static char* dal_c_Project__resolvePropertyValue(const char* base_dir, const char* key, const char* value);
static char* dal_c_Project__depsPreludeHeaderPath(const dal_c_Project* proj);
static char* dal_c_Project__findIncludeHeader(const dal_c_Project* proj, const char* header);
static char* dal_c_Project__findDHInstallHeader(const dal_c_Project* proj, const char* header);
static bool dal_c_Project__usesDHLibraryDefault(const dal_c_Project* proj);
static char* dal_c_Project__detectPCH(const dal_c_Project* proj);
static bool dal_c_Project__addLibrary(dal_c_Project* proj, dal_c_Lib* lib);
static void dal_c_Project__addSelfRoot(dal_c_Project* proj, const char* path);
static void dal_c_Project__addTargetRoot(dal_c_Project* proj, dal_c_TargetRoot* root);
static void dal_c_Project__addToArray(char*** arr, int* count, const char* value);
static void dal_c_Project__setString(char** slot, const char* value);
static void dal_c_Project__appendArgs(char** slot, const char* value);
static bool dal_c_Project__isAbsolutePath(const char* path);
static bool dal_c_Project__isTrue(const char* value);
static void dal_c_Project__freeLines(char** lines, int line_count);
static void dal_c_Project__applyBuildDefaultsLine(dal_c_BuildDefaults* defaults, const char* key, const char* value);
static void dal_c_Project__applyPropertyLine(dal_c_CompilerOpts* opts, const char* key, const char* value);
static bool dal_c_Project__parseUnsigned(const char* value, unsigned* out);
static bool dal_c_Project__versionBuildIsValid(const char* value);
static bool dal_c_Project__setVersionLabelPrefix(dal_c_VersionSpec* dst, int prefix_num, const char* prefix_str);
static void dal_c_Project__applyLibraryLine(dal_c_Lib* lib, const dal_c_Project* proj, const char* key, const char* value);
static void dal_c_Project__applyTargetRootLine(dal_c_TargetRoot* root, const dal_c_Project* proj, const char* key, const char* value);
static char* dal_c_Project__resolveProjectPath(const dal_c_Project* proj, const char* value);
static char* dal_c_Project__pathRelativeTo(const char* root, const char* path);
static bool dal_c_Project__pathHasPrefix(const char* root, const char* path);
static bool dal_c_Project__targetRootIsValid(const dal_c_TargetRoot* root);
static void dal_c_Project__resolveTargetRootExcludePaths(dal_c_TargetRoot* root);
static void dal_c_Project__ensureBuiltinTargetRoots(dal_c_Project* proj);
static bool dal_c_Project__resolveCategoryDirs(dal_c_Project* proj);
static bool dal_c_Project__resolveCategoryDir(dal_c_Project* proj, char** slot, const char* const* aliases);

// === PUBLIC API ===

dal_c_Project* dal_c_Project_detect(const dal_c_Cmd* cmd) {
    dal_c_Project* proj = calloc(1, sizeof(dal_c_Project));
    if (!proj) { return NULL; }

    char* cwd = env_getCWD();
    proj->root = dal_c_Project__findRoot(cwd);
    proj->workspace_root = dal_c_Project__findWorkspaceRoot(proj->root ? proj->root : cwd);
    free(cwd);
    proj->workspace_opts.profile = dal_c_Profile_invalid;

    if (proj->workspace_root) {
        proj->workspace_dh = path_join(proj->workspace_root, dal_c_file_detector_workspace);
        if (proj->workspace_dh && path_isFile(proj->workspace_dh)
            && !dal_c_Project__applyFlatDHFile(&proj->workspace_opts, &proj->workspace_defaults, proj->workspace_dh, dal_c_file_detector_workspace)) {
            dal_c_Project_cleanup(&proj);
            return NULL;
        }
    }

    if (proj->root) {
        proj->name = path_basename(proj->root);
        proj->project_dh = path_join(proj->root, dal_c_file_detector_project);
        proj->pch_enabled = true;
        if (path_isFile(proj->project_dh)) {
            if (!dal_c_Project__parseProjectDH(proj->project_dh, proj)) {
                dal_c_Project_cleanup(&proj);
                return NULL;
            }
        }
        if (!dal_c_Project__resolveCategoryDirs(proj)) {
            dal_c_Project_cleanup(&proj);
            return NULL;
        }
        dal_c_Project__ensureBuiltinTargetRoots(proj);
    } else if (!dal_c_Project__configureAdHocUnit(proj, cmd)) {
        dal_c_Project_cleanup(&proj);
        return NULL;
    }

    proj->dh_path = dal_c_Project__findDHInstallation(cmd);
    proj->pch_header = dal_c_Project__detectPCH(proj);
    return proj;
}

dal_c_Project* dal_c_Project_detectAt(const char* lib_path, const char* dh_path) {
    if (!lib_path || !path_isDir(lib_path)) { return NULL; }

    dal_c_Project* proj = calloc(1, sizeof(dal_c_Project));
    if (!proj) { return NULL; }

    proj->root = path_abs(lib_path);
    proj->workspace_root = dal_c_Project__findWorkspaceRoot(proj->root);
    proj->workspace_opts.profile = dal_c_Profile_invalid;
    if (proj->workspace_root) {
        proj->workspace_dh = path_join(proj->workspace_root, dal_c_file_detector_workspace);
        if (proj->workspace_dh && path_isFile(proj->workspace_dh)
            && !dal_c_Project__applyFlatDHFile(&proj->workspace_opts, &proj->workspace_defaults, proj->workspace_dh, dal_c_file_detector_workspace)) {
            dal_c_Project_cleanup(&proj);
            return NULL;
        }
    }
    proj->name = path_basename(proj->root);
    proj->dh_path = dal_c_Project__normalizeDHPath(dh_path);
    proj->project_dh = path_join(proj->root, dal_c_file_detector_project);
    proj->pch_enabled = true;
    if (proj->project_dh && path_isFile(proj->project_dh)) {
        if (!dal_c_Project__parseProjectDH(proj->project_dh, proj)) {
            dal_c_Project_cleanup(&proj);
            return NULL;
        }
    }
    if (!dal_c_Project__resolveCategoryDirs(proj)) {
        dal_c_Project_cleanup(&proj);
        return NULL;
    }
    dal_c_Project__ensureBuiltinTargetRoots(proj);
    proj->pch_header = dal_c_Project__detectPCH(proj);
    return proj;
}

void dal_c_Project_cleanup(dal_c_Project** self) {
    if (!self || !*self) { return; }
    dal_c_Project* proj = *self;

    dal_c_CompilerOpts_cleanup(&proj->workspace_opts);
    dal_c_BuildDefaults_cleanup(&proj->workspace_defaults);
    dal_c_CompilerOpts_cleanup(&proj->opts);
    for (int i = 0; i < proj->lib_count; ++i) {
        dal_c_Lib* lib = &proj->libraries[i];
        free(lib->name);
        free(lib->path);
        free(lib->source);
        free(lib->archive);
        free(lib->package_root);
        free(lib->revision);
        free(lib->provider);
        free(lib->build_command);
        free(lib->install_command);
        for (int j = 0; j < lib->runtime_file_count; ++j) free(lib->runtime_files[j]);
        free(lib->runtime_files);
        dal_c_CompilerOpts_cleanup(&lib->opts);
    }
    free(proj->libraries);
    free(proj->root);
    free(proj->name);
    free(proj->dh_path);
    free(proj->project_dh);
    free(proj->workspace_root);
    free(proj->workspace_dh);
    free(proj->unit_root);
    free(proj->unit_source);
    free(proj->unit_dh);
    free(proj->unit_lock_dh);
    free(proj->unit_id);
    free(proj->src_dir_name);
    free(proj->include_dir_name);
    free(proj->tests_dir_name);
    free(proj->samples_dir_name);
    free(proj->examples_dir_name);
    free(proj->pch_header_override);
    free(proj->pch_header);
    dal_c_BuildDefaults_cleanup(&proj->defaults);
    for (int i = 0; i < proj->self_root_count; ++i) {
        free(proj->self_roots[i].path);
    }
    free(proj->self_roots);
    for (int i = 0; i < proj->target_root_count; ++i) {
        free(proj->target_roots[i].name);
        free(proj->target_roots[i].path);
        for (int j = 0; j < proj->target_roots[i].exclude_count; ++j) {
            free(proj->target_roots[i].exclude_paths[j]);
        }
        free((void*)proj->target_roots[i].exclude_paths);
    }
    free(proj->target_roots);
    for (int i = 0; i < proj->exclude_count; ++i) {
        free(proj->exclude_paths[i]);
    }
    free((void*)proj->exclude_paths);
    for (int i = 0; i < proj->pch_exclude_count; ++i) {
        free(proj->pch_exclude_headers[i]);
    }
    free((void*)proj->pch_exclude_headers);
    free(proj);
    *self = NULL;
}

char* dal_c_Project_findDHInstallation(const dal_c_Cmd* cmd) {
    return dal_c_Project__findDHInstallation(cmd);
}

void dal_c_CompilerOpts_cleanup(dal_c_CompilerOpts* opts) {
    if (!opts) { return; }
    free(opts->compiler);
    free(opts->c_std);
    free(opts->arch_target);
    free(opts->target_arch);
    free(opts->target_tune);
    free(opts->target_abi);
    free(opts->sysroot);
    free(opts->entry_symbol);
    free(opts->compiler_args);
    free(opts->link_args);
    free(opts->output_ext);
    free(opts->linker_script);
    free(opts->objcopy);
    free(opts->objcopy_format);
    for (int i = 0; i < opts->define_count; ++i) {
        free(opts->define_macros[i]);
    }
    free((void*)opts->define_macros);
    for (int i = 0; i < opts->undef_count; ++i) {
        free(opts->undef_macros[i]);
    }
    free((void*)opts->undef_macros);
    for (int i = 0; i < opts->isystem_count; ++i) {
        free(opts->isystem_paths[i]);
    }
    free((void*)opts->isystem_paths);
    for (int i = 0; i < opts->include_count; ++i) {
        free(opts->include_paths[i]);
    }
    free((void*)opts->include_paths);
    for (int i = 0; i < opts->link_count; ++i) {
        free(opts->link_libs[i]);
    }
    free((void*)opts->link_libs);
    for (int i = 0; i < opts->link_dir_count; ++i) {
        free(opts->link_dirs[i]);
    }
    free((void*)opts->link_dirs);
    dal_c_VersionSpec_cleanup(&opts->version);
    memset(opts, 0, sizeof(*opts));
}

bool dal_c_MacroBacktraceLimit_parse(const char* value, int* out) {
    if (!value || !value[0] || !out) { return false; }
    if (str_eql(value, dal_c_macro_backtrace_limit_short)) {
        *out = dal_c_default_macro_backtrace_limit;
        return true;
    }
    if (str_eql(value, dal_c_macro_backtrace_limit_unlimited)) {
        *out = 0;
        return true;
    }

    int parsed = 0;
    for (const char* cursor = value; *cursor; ++cursor) {
        if (*cursor < '0' || *cursor > '9') { return false; }
        int digit = *cursor - '0';
        if (parsed > (INT_MAX - digit) / 10) { return false; }
        parsed = parsed * 10 + digit;
    }
    *out = parsed;
    return true;
}

void dal_c_CompilerOpts_merge(dal_c_CompilerOpts* dst, const dal_c_CompilerOpts* src) {
    assert(dst != NULL);
    if (!src) { return; }

    if (src->compiler) { dal_c_Project__setString(&dst->compiler, src->compiler); }
    if (src->c_std) { dal_c_Project__setString(&dst->c_std, src->c_std); }
    if (src->arch_target) { dal_c_Project__setString(&dst->arch_target, src->arch_target); }
    if (src->target_arch) { dal_c_Project__setString(&dst->target_arch, src->target_arch); }
    if (src->target_tune) { dal_c_Project__setString(&dst->target_tune, src->target_tune); }
    if (src->target_abi) { dal_c_Project__setString(&dst->target_abi, src->target_abi); }
    if (src->sysroot) { dal_c_Project__setString(&dst->sysroot, src->sysroot); }
    if (src->entry_symbol) { dal_c_Project__setString(&dst->entry_symbol, src->entry_symbol); }
    if (src->compiler_args) { dal_c_Project__appendArgs(&dst->compiler_args, src->compiler_args); }
    if (src->link_args) { dal_c_Project__appendArgs(&dst->link_args, src->link_args); }
    if (src->output_ext) { dal_c_Project__setString(&dst->output_ext, src->output_ext); }
    if (src->linker_script) { dal_c_Project__setString(&dst->linker_script, src->linker_script); }
    if (src->objcopy) { dal_c_Project__setString(&dst->objcopy, src->objcopy); }
    if (src->objcopy_format) { dal_c_Project__setString(&dst->objcopy_format, src->objcopy_format); }
    if (src->profile != dal_c_Profile_invalid) { dst->profile = src->profile; }
    if (src->compile_env != dal_c_CompileEnv_auto) { dst->compile_env = src->compile_env; }
    if (src->libc_linked != dal_c_ToggleState_auto) { dst->libc_linked = src->libc_linked; }
    if (src->dsl_mode != dal_c_ToggleState_auto) { dst->dsl_mode = src->dsl_mode; }
    if (src->default_libs_linked != dal_c_ToggleState_auto) { dst->default_libs_linked = src->default_libs_linked; }
    if (src->start_files_linked != dal_c_ToggleState_auto) { dst->start_files_linked = src->start_files_linked; }
    if (src->compiler_rt_linked != dal_c_ToggleState_auto) { dst->compiler_rt_linked = src->compiler_rt_linked; }
    if (src->link_mode != dal_c_LinkMode_auto) { dst->link_mode = src->link_mode; }
    if (src->lto_mode != dal_c_LtoMode_auto) { dst->lto_mode = src->lto_mode; }
    if (src->prebuilt_mode_set) {
        dst->prebuilt_mode = src->prebuilt_mode;
        dst->prebuilt_mode_set = true;
    }
    if (src->omit_frame_pointer != dal_c_ToggleState_auto) { dst->omit_frame_pointer = src->omit_frame_pointer; }
    if (src->function_sections != dal_c_ToggleState_auto) { dst->function_sections = src->function_sections; }
    if (src->data_sections != dal_c_ToggleState_auto) { dst->data_sections = src->data_sections; }
    if (src->gc_sections != dal_c_ToggleState_auto) { dst->gc_sections = src->gc_sections; }
    if (src->whole_archive != dal_c_ToggleState_auto) { dst->whole_archive = src->whole_archive; }
    if (src->unroll_loops != dal_c_ToggleState_auto) { dst->unroll_loops = src->unroll_loops; }
    if (src->unwind_tables != dal_c_ToggleState_auto) { dst->unwind_tables = src->unwind_tables; }
    if (src->async_unwind_tables != dal_c_ToggleState_auto) { dst->async_unwind_tables = src->async_unwind_tables; }
    if (src->exceptions != dal_c_ToggleState_auto) { dst->exceptions = src->exceptions; }
    if (src->strip_mode != dal_c_ToggleState_auto) { dst->strip_mode = src->strip_mode; }
    if (src->icf_mode != dal_c_IcfMode_auto) { dst->icf_mode = src->icf_mode; }
    if (src->merge_all_constants != dal_c_ToggleState_auto) { dst->merge_all_constants = src->merge_all_constants; }
    if (src->stack_protector != dal_c_ToggleState_auto) { dst->stack_protector = src->stack_protector; }
    if (src->macro_backtrace_limit_set) {
        dst->macro_backtrace_limit = src->macro_backtrace_limit;
        dst->macro_backtrace_limit_set = true;
    }
    if (src->loose_errors != dal_c_LooseErrorsMode_auto) { dst->loose_errors = src->loose_errors; }
    dal_c_VersionSpec_merge(&dst->version, &src->version);

    for (int i = 0; i < src->define_count; ++i) {
        dal_c_Project__addToArray(&dst->define_macros, &dst->define_count, src->define_macros[i]);
    }
    for (int i = 0; i < src->undef_count; ++i) {
        dal_c_Project__addToArray(&dst->undef_macros, &dst->undef_count, src->undef_macros[i]);
    }
    for (int i = 0; i < src->isystem_count; ++i) {
        dal_c_Project__addToArray(&dst->isystem_paths, &dst->isystem_count, src->isystem_paths[i]);
    }
    for (int i = 0; i < src->include_count; ++i) {
        dal_c_Project__addToArray(&dst->include_paths, &dst->include_count, src->include_paths[i]);
    }
    for (int i = 0; i < src->link_count; ++i) {
        dal_c_Project__addToArray(&dst->link_libs, &dst->link_count, src->link_libs[i]);
    }
    for (int i = 0; i < src->link_dir_count; ++i) {
        dal_c_Project__addToArray(&dst->link_dirs, &dst->link_dir_count, src->link_dirs[i]);
    }
}

void dal_c_VersionSpec_cleanup(dal_c_VersionSpec* version) {
    if (!version) { return; }
    free(version->namespace_name);
    free(version->label_prefix_str);
    free(version->label_suffix_str);
    free(version->build_str);
    memset(version, 0, sizeof(*version));
}

void dal_c_VersionSpec_merge(dal_c_VersionSpec* dst, const dal_c_VersionSpec* src) {
    assert(dst != NULL);
    if (!src) { return; }

    if (src->namespace_set) {
        dal_c_Project__setString(&dst->namespace_name, src->namespace_name ? src->namespace_name : "");
        dst->namespace_set = true;
    }
    if (src->core_set) {
        dst->core_major = src->core_major;
        dst->core_minor = src->core_minor;
        dst->core_patch = src->core_patch;
        dst->core_set = true;
    }
    if (src->label_prefix_set) {
        dst->label_prefix_num = src->label_prefix_num;
        dal_c_Project__setString(&dst->label_prefix_str, src->label_prefix_str ? src->label_prefix_str : "");
        dst->label_prefix_set = true;
    }
    if (src->label_suffix_set) {
        dst->label_suffix_num = src->label_suffix_num;
        dal_c_Project__setString(&dst->label_suffix_str, src->label_suffix_str ? src->label_suffix_str : "");
        dst->label_suffix_set = true;
    }
    if (src->build_set) {
        dal_c_Project__setString(&dst->build_str, src->build_str ? src->build_str : "");
        dst->build_set = true;
    }
}


bool dal_c_VersionSpec_parseNamespace(dal_c_VersionSpec* dst, const char* value) {
    assert(dst != NULL);
    if (!value || value[0] == '\0') { return false; }
    const unsigned char first = (unsigned char)value[0];
    if (!((first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') || first == '_')) {
        return false;
    }
    for (const unsigned char* it = (const unsigned char*)value + 1; *it; ++it) {
        if (!((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z')
            || (*it >= '0' && *it <= '9') || *it == '_')) {
            return false;
        }
    }
    dal_c_Project__setString(&dst->namespace_name, value);
    dst->namespace_set = true;
    return true;
}

bool dal_c_VersionSpec_parseCore(dal_c_VersionSpec* dst, const char* value) {
    assert(dst != NULL);
    if (!value || value[0] == '\0') { return false; }

    int count = 0;
    char** parts = str_split(value, ".", &count);
    bool ok = false;
    unsigned major = 0;
    unsigned minor = 0;
    unsigned patch = 0;
    if (parts && count == 3
        && dal_c_Project__parseUnsigned(parts[0], &major)
        && dal_c_Project__parseUnsigned(parts[1], &minor)
        && dal_c_Project__parseUnsigned(parts[2], &patch)) {
        dst->core_major = major;
        dst->core_minor = minor;
        dst->core_patch = patch;
        dst->core_set = true;
        ok = true;
    }
    dal_c_Project__freeLines(parts, count);
    return ok;
}

bool dal_c_VersionSpec_parsePrefix(dal_c_VersionSpec* dst, const char* value) {
    assert(dst != NULL);
    if (!value || value[0] == '\0' || str_eql(value, "none") || str_eql(value, "release")) {
        free(dst->label_prefix_str);
        dst->label_prefix_str = NULL;
        dst->label_prefix_num = 0;
        dst->label_prefix_set = false;
        return true;
    }
    if (str_eql(value, "alpha")) {
        return dal_c_Project__setVersionLabelPrefix(dst, dal_c_ver_label_prefix_as_num_alpha, "alpha");
    }
    if (str_eql(value, "beta")) {
        return dal_c_Project__setVersionLabelPrefix(dst, dal_c_ver_label_prefix_as_num_beta, "beta");
    }
    if (str_eql(value, "rc")) {
        return dal_c_Project__setVersionLabelPrefix(dst, dal_c_ver_label_prefix_as_num_rc, "rc");
    }
    return false;
}

bool dal_c_VersionSpec_parseSuffix(dal_c_VersionSpec* dst, const char* value) {
    assert(dst != NULL);
    if (!value || value[0] == '\0') { return false; }
    unsigned suffix = 0;
    if (!dal_c_Project__parseUnsigned(value, &suffix)) {
        return false;
    }
    if (suffix > 63u) {
        return false;
    }

    dst->label_suffix_num = suffix;
    char* suffix_str = str_format("%u", suffix);
    if (!suffix_str) {
        return false;
    }
    free(dst->label_suffix_str);
    dst->label_suffix_str = suffix_str;
    dst->label_suffix_set = true;
    return true;
}

bool dal_c_VersionSpec_parseBuild(dal_c_VersionSpec* dst, const char* value) {
    assert(dst != NULL);
    if (!value || value[0] == '\0') {
        free(dst->build_str);
        dst->build_str = NULL;
        dst->build_set = false;
        return true;
    }
    if (!dal_c_Project__versionBuildIsValid(value)) {
        return false;
    }
    dal_c_Project__setString(&dst->build_str, value);
    dst->build_set = true;
    return true;
}

void dal_c_BuildDefaults_cleanup(dal_c_BuildDefaults* defaults) {
    if (!defaults) { return; }
    free(defaults->output_name);
    memset(defaults, 0, sizeof(*defaults));
    defaults->target_kind = dal_c_Target_invalid;
}

void dal_c_BuildDefaults_merge(dal_c_BuildDefaults* dst, const dal_c_BuildDefaults* src) {
    assert(dst != NULL);
    if (!src) { return; }
    if (src->output_name) {
        dal_c_Project__setString(&dst->output_name, src->output_name);
    }
    if (src->target_kind_set) {
        dst->target_kind = src->target_kind;
        dst->target_kind_set = true;
    }
    if (src->build_runs_tests_set) {
        dst->build_runs_tests = src->build_runs_tests;
        dst->build_runs_tests_set = true;
    }
}

bool dal_c_DHFile_apply(dal_c_CompilerOpts* opts, dal_c_BuildDefaults* defaults, const char* path, const char* kind_name) {
    return dal_c_Project__applyFlatDHFile(opts, defaults, path, kind_name);
}

bool dal_c_BuildDefaults_applyDHFile(dal_c_BuildDefaults* dst, const char* path) {
    assert(dst != NULL);
    return dal_c_Project__applyFlatDHFile(NULL, dst, path, "build property file");
}

bool dal_c_CompilerOpts_applyDHFile(dal_c_CompilerOpts* dst, const char* path) {
    assert(dst != NULL);
    return dal_c_Project__applyFlatDHFile(dst, NULL, path, "compiler property file");
}

static bool dal_c_Project__applyFlatDHFile(
    dal_c_CompilerOpts* opts,
    dal_c_BuildDefaults* defaults,
    const char* path,
    const char* kind_name
) {
    if (!path || !path_isFile(path)) {
        (void)fprintf(stderr, "Error: %s not found: %s\n", kind_name ? kind_name : ".dh file", path ? path : "(null)");
        return false;
    }

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) {
        (void)fprintf(stderr, "Error: Unable to read %s: %s\n", kind_name ? kind_name : ".dh file", path);
        return false;
    }

    char* base_dir = path_parent(path);
    bool ok = true;
    for (int i = 0; i < line_count; ++i) {
        char* line = dal_c_Project__trimInPlace(lines[i]);
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';') { continue; }
        if (line[0] == '[') {
            (void)fprintf(stderr, "Error: %s:%d: sections are not allowed in %s; use root project.dh for dependencies and target roots\n",
                          path, i + 1, kind_name ? kind_name : "this .dh file");
            ok = false;
            break;
        }

        char* eq = strchr(line, '=');
        if (!eq) {
            (void)fprintf(stderr, "Error: %s:%d: expected key=value\n", path, i + 1);
            ok = false;
            break;
        }
        *eq = '\0';
        const char* key = dal_c_Project__trimInPlace(line);
        const char* value = dal_c_Project__trimInPlace(eq + 1);
        if (key[0] == '\0') {
            (void)fprintf(stderr, "Error: %s:%d: empty key\n", path, i + 1);
            ok = false;
            break;
        }

        bool recognized = false;
        bool value_valid = true;
        if (dal_c_Project__isBuildDefaultsKey(key)) {
            value_valid = dal_c_Project__validateBuildDefaultsValue(key, value);
        } else if (dal_c_Project__isPropertyKey(key)) {
            value_valid = dal_c_Project__validatePropertyValue(key, value);
        }
        if (!value_valid) {
            (void)fprintf(stderr, "Error: %s:%d: invalid value `%s` for `%s` in %s\n",
                          path, i + 1, value, key, kind_name ? kind_name : ".dh file");
            ok = false;
            break;
        }
        if (defaults && dal_c_Project__isBuildDefaultsKey(key)) {
            dal_c_Project__applyBuildDefaultsLine(defaults, key, value);
            recognized = true;
        }
        if (opts && dal_c_Project__isPropertyKey(key)) {
            char* resolved_value = dal_c_Project__resolvePropertyValue(base_dir, key, value);
            dal_c_Project__applyPropertyLine(opts, key, resolved_value ? resolved_value : value);
            free(resolved_value);
            recognized = true;
        }
        if (!recognized) {
            (void)fprintf(stderr, "Error: %s:%d: unsupported key `%s` in %s\n",
                          path, i + 1, key, kind_name ? kind_name : ".dh file");
            ok = false;
            break;
        }
    }

    free(base_dir);
    dal_c_Project__freeLines(lines, line_count);
    return ok;
}

ArrStr* dal_c__collectDescendantProjects(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);

    ArrStr* roots = ArrStr_init();
    int file_count = 0;
    char** files = dir_listRecur(proj->root, &file_count);
    if (!files) { return roots; }

    for (int i = 0; i < file_count; ++i) {
        const char* file = files[i];
        if (strstr(file, "\\build\\") || strstr(file, "/build/")
            || strstr(file, "\\.cache\\") || strstr(file, "/.cache/")
            || strstr(file, "\\lib\\deps\\") || strstr(file, "/lib/deps/")) {
            free(files[i]);
            continue;
        }

        char* base = path_basename(file);
        bool is_project_dh = base && str_eql(base, dal_c_file_detector_project);
        free(base);
        if (!is_project_dh) {
            free(files[i]);
            continue;
        }

        char* root = path_parent(file);
        if (root && !str_eql(root, proj->root)) {
            bool seen = false;
            for (int j = 0; j < ArrStr_len(roots); ++j) {
                if (str_eql(ArrStr_at(roots, j), root)) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                ArrStr_push(roots, root);
            }
        }
        free(root);
        free(files[i]);
    }
    free((void*)files);
    return roots;
}

char* dal_c_Project_getBuildDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_dir_build);
}

char* dal_c_Project_getStateRoot(const dal_c_Project* proj) {
    assert(proj != NULL);
    /* Named projects own mutable dependency state directly. Ad-hoc units keep
       durable resolution beside the primary source while placing disposable
       source/build/package state in an isolated workspace/global unit scope. */
    if (proj->root) {
        return path_join(proj->root, ".dh-c");
    }
    if (!proj->is_adhoc || !proj->unit_id) { return NULL; }
    char* units_root = NULL;
    if (proj->workspace_root) {
        char* workspace_state = path_join(proj->workspace_root, ".dh-c");
        units_root = workspace_state ? path_join(workspace_state, "units") : NULL;
        free(workspace_state);
    } else {
        char* cache_root = dal_c__cacheBaseDir(proj);
        units_root = cache_root ? path_join(cache_root, "units") : NULL;
        free(cache_root);
    }
    char* result = units_root ? path_join(units_root, proj->unit_id) : NULL;
    free(units_root);
    return result;
}

char* dal_c_Project_getDependencyLockPath(const dal_c_Project* proj) {
    assert(proj != NULL);
    if (proj->root) { return path_join(proj->root, "lock.dh"); }
    return proj->unit_lock_dh ? strdup(proj->unit_lock_dh) : NULL;
}

const char* dal_c_Project_getCategoryDirName(const dal_c_Project* proj, const char* canonical_name) {
    assert(proj != NULL);
    assert(canonical_name != NULL);
    if (str_eql(canonical_name, dal_c_dir_include)) {
        return proj->include_dir_name ? proj->include_dir_name : dal_c_dir_include;
    }
    if (str_eql(canonical_name, dal_c_dir_src)) {
        return proj->src_dir_name ? proj->src_dir_name : dal_c_dir_src;
    }
    if (str_eql(canonical_name, dal_c_dir_tests)) {
        return proj->tests_dir_name ? proj->tests_dir_name : dal_c_dir_tests;
    }
    if (str_eql(canonical_name, dal_c_dir_samples)) {
        return proj->samples_dir_name ? proj->samples_dir_name : dal_c_dir_samples;
    }
    if (str_eql(canonical_name, dal_c_dir_examples)) {
        return proj->examples_dir_name ? proj->examples_dir_name : dal_c_dir_examples;
    }
    return canonical_name;
}

char* dal_c_Project_getCategoryDir(const dal_c_Project* proj, const char* canonical_name) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_Project_getCategoryDirName(proj, canonical_name));
}

char* dal_c_Project_getIncludeDir(const dal_c_Project* proj) {
    return dal_c_Project_getCategoryDir(proj, dal_c_dir_include);
}

char* dal_c_Project_getSrcDir(const dal_c_Project* proj) {
    return dal_c_Project_getCategoryDir(proj, dal_c_dir_src);
}

char* dal_c_Project_getTestsDir(const dal_c_Project* proj) {
    return dal_c_Project_getCategoryDir(proj, dal_c_dir_tests);
}

char* dal_c_Project_getSamplesDir(const dal_c_Project* proj) {
    return dal_c_Project_getCategoryDir(proj, dal_c_dir_samples);
}

char* dal_c_Project_getExamplesDir(const dal_c_Project* proj) {
    return dal_c_Project_getCategoryDir(proj, dal_c_dir_examples);
}

char* dal_c_Project_getLibDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    if (proj->root) { return path_join(proj->root, dal_c_dir_lib); }
    char* state_root = dal_c_Project_getStateRoot(proj);
    char* export_root = state_root ? path_join(state_root, "exports") : NULL;
    free(state_root);
    return export_root;
}

char* dal_c_Project_getDepsDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    char* lib_dir = dal_c_Project_getLibDir(proj);
    char* deps_dir = lib_dir ? path_join(lib_dir, dal_c_dir_deps) : NULL;
    free(lib_dir);
    return deps_dir;
}

const dal_c_TargetRoot* dal_c_Project_findTargetRootByName(const dal_c_Project* proj, const char* name) {
    assert(proj != NULL);
    if (!name) { return NULL; }
    for (int i = 0; i < proj->target_root_count; ++i) {
        const dal_c_TargetRoot* root = &proj->target_roots[i];
        if (root->name && str_eql(root->name, name)) {
            return root;
        }
    }
    return NULL;
}

const dal_c_TargetRoot* dal_c_Project_findTargetRootByPath(const dal_c_Project* proj, const char* path) {
    assert(proj != NULL);
    if (!path) { return NULL; }

    const dal_c_TargetRoot* best = NULL;
    size_t best_len = 0;
    for (int i = 0; i < proj->target_root_count; ++i) {
        const dal_c_TargetRoot* root = &proj->target_roots[i];
        if (!root->path || !dal_c_Project__pathHasPrefix(root->path, path)) {
            continue;
        }
        size_t root_len = strlen(root->path);
        if (!best || root_len > best_len) {
            best = root;
            best_len = root_len;
        }
    }
    return best;
}

void dal_c_TargetRequest_cleanup(dal_c_TargetRequest* request) {
    if (!request) { return; }
    free(request->resolved_path);
    free(request->relative_path);
    memset(request, 0, sizeof(*request));
}

bool dal_c_TargetRequest_resolve(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_TargetRequest* out) {
    assert(proj != NULL);
    assert(intent != NULL);
    assert(out != NULL);
    memset(out, 0, sizeof(*out));
    out->raw_target_path = intent->target_path;

    const dal_c_TargetRoot* root = NULL;
    if (intent->target_root_name_hint) {
        root = dal_c_Project_findTargetRootByName(proj, intent->target_root_name_hint);
        if (!root) {
            (void)fprintf(stderr, "Error: Unknown target root `%s`\n", intent->target_root_name_hint);
            return false;
        }
    }

    char* resolved_path = NULL;
    if (intent->target_path) {
        if (dal_c_Project__isAbsolutePath(intent->target_path)) {
            resolved_path = strdup(intent->target_path);
        } else if (root && root->path) {
            resolved_path = path_join(root->path, intent->target_path);
        } else {
            resolved_path = path_abs(intent->target_path);
        }
    } else if (root && root->path) {
        resolved_path = strdup(root->path);
    }

    if (!root && resolved_path && !intent->target_path_is_explicit_file) {
        root = dal_c_Project_findTargetRootByPath(proj, resolved_path);
    }

    out->root = root;
    out->kind = root ? root->kind : dal_c_Target_invalid;
    out->selection = root ? root->selection : dal_c_TargetSelection_invalid;
    out->link_project = root ? root->link_project : false;
    out->resolved_path = resolved_path;

    if (!resolved_path || !root) {
        if (resolved_path) {
            out->resolved_is_dir = path_isDir(resolved_path);
        }
        return true;
    }

    if (!dal_c_Project__pathHasPrefix(root->path, resolved_path)) {
        (void)fprintf(stderr, "Error: Target path `%s` is outside target root `%s`\n", resolved_path, root->name);
        dal_c_TargetRequest_cleanup(out);
        return false;
    }

    out->relative_path = dal_c_Project__pathRelativeTo(root->path, resolved_path);
    out->resolved_is_dir = path_isDir(resolved_path);
    if (!out->resolved_is_dir && !path_isFile(resolved_path)) {
        (void)fprintf(stderr, "Error: Target path not found: %s\n", resolved_path);
        dal_c_TargetRequest_cleanup(out);
        return false;
    }

    if (root->selection == dal_c_TargetSelection_file && out->resolved_is_dir) {
        (void)fprintf(stderr, "Error: Target root `%s` requires file targets\n", root->name);
        dal_c_TargetRequest_cleanup(out);
        return false;
    }
    if (root->selection == dal_c_TargetSelection_dir && !out->resolved_is_dir) {
        (void)fprintf(stderr, "Error: Target root `%s` requires directory targets\n", root->name);
        dal_c_TargetRequest_cleanup(out);
        return false;
    }

    return true;
}

// === PRIVATE IMPLEMENTATIONS ===

static char* dal_c_Project__trimInPlace(char* text) {
    if (!text) { return NULL; }
    char* start = text;
    while (*start && isspace((unsigned char)*start)) { ++start; }
    char* end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) { --end; }
    size_t len = (size_t)(end - start);
    if (start != text && len > 0) { memmove(text, start, len); }
    text[len] = '\0';
    return text;
}

static unsigned long long dal_c_Project__hashText(const char* text) {
    unsigned long long hash = 1469598103934665603ULL;
    for (const unsigned char* p = (const unsigned char*)(text ? text : ""); *p; ++p) {
        hash ^= (unsigned long long)*p;
        hash *= 1099511628211ULL;
    }
    return hash;
}

static char* dal_c_Project__pathWithoutExtension(const char* path) {
    if (!path) { return NULL; }
    char* result = strdup(path);
    if (!result) { return NULL; }
    char* slash = strrchr(result, '/');
    char* backslash = strrchr(result, '\\');
    char* base = slash > backslash ? slash : backslash;
    base = base ? base + 1 : result;
    char* dot = strrchr(base, '.');
    if (dot && dot != base) { *dot = '\0'; }
    return result;
}

static bool dal_c_Project__isUnitSourcePath(const char* path) {
    if (!path || !path_isFile(path)) { return false; }
    return str_endsWith(path, ".c") || str_endsWith(path, ".h")
        || str_endsWith(path, ".s") || str_endsWith(path, ".S");
}

static char* dal_c_Project__primarySource(const dal_c_Cmd* cmd) {
    if (!cmd) { return NULL; }
    for (int i = 0; i < cmd->input_count; ++i) {
        if (dal_c_Project__isUnitSourcePath(cmd->input_files[i])) {
            return path_abs(cmd->input_files[i]);
        }
    }
    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    if (dal_c_Project__isUnitSourcePath(intent.target_path)) {
        return path_abs(intent.target_path);
    }
    return NULL;
}

static void dal_c_Project__freeLibraryDraft(dal_c_Lib* lib) {
    if (!lib) { return; }
    free(lib->name);
    free(lib->path);
    free(lib->source);
    free(lib->archive);
    free(lib->package_root);
    free(lib->revision);
    free(lib->provider);
    free(lib->build_command);
    free(lib->install_command);
    for (int i = 0; i < lib->runtime_file_count; ++i) { free(lib->runtime_files[i]); }
    free(lib->runtime_files);
    dal_c_CompilerOpts_cleanup(&lib->opts);
    free(lib);
}

static bool dal_c_Project__hasLibraryNamed(const dal_c_Project* proj, const char* name) {
    if (!proj || !name) { return false; }
    for (int i = 0; i < proj->lib_count; ++i) {
        if (proj->libraries[i].name && str_eql(proj->libraries[i].name, name)) { return true; }
    }
    return false;
}

static bool dal_c_Project__parseUnitDH(const char* path, dal_c_Project* proj) {
    assert(path != NULL);
    assert(proj != NULL);

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    dal_c_Lib* current_lib = NULL;
    proj->opts.profile = dal_c_Profile_invalid;
    for (int i = 0; i < line_count; ++i) {
        char* line = dal_c_Project__trimInPlace(lines[i]);
        if (!line[0] || line[0] == '#' || line[0] == ';') { continue; }

        if (line[0] == '[') {
            if (current_lib) {
                if (dal_c_Project__hasLibraryNamed(proj, current_lib->name)) {
                    (void)fprintf(stderr, "Error: %s:%d: duplicate dependency section `%s`\n", path, i + 1, current_lib->name);
                    dal_c_Project__freeLibraryDraft(current_lib);
                    dal_c_Project__freeLines(lines, line_count);
                    return false;
                }
                if (!dal_c_Project__addLibrary(proj, current_lib)) {
                    dal_c_Project__freeLines(lines, line_count);
                    return false;
                }
                current_lib = NULL;
            }
            char* end = strchr(line, ']');
            if (!end || dal_c_Project__trimInPlace(end + 1)[0] != '\0') {
                (void)fprintf(stderr, "Error: %s:%d: malformed dependency section\n", path, i + 1);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            *end = '\0';
            char* section = dal_c_Project__trimInPlace(line + 1);
            if (!section[0]) {
                (void)fprintf(stderr, "Error: %s:%d: empty dependency section\n", path, i + 1);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            if (str_startsWith(section, dal_c_project_section_target_root)) {
                (void)fprintf(stderr, "Error: %s:%d: target-root sections require project.dh\n", path, i + 1);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            if (dal_c_Project__hasLibraryNamed(proj, section)) {
                (void)fprintf(stderr, "Error: %s:%d: duplicate dependency section `%s`\n", path, i + 1, section);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            current_lib = calloc(1, sizeof(*current_lib));
            if (!current_lib) {
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            current_lib->name = strdup(section);
            if (!current_lib->name) {
                dal_c_Project__freeLibraryDraft(current_lib);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            current_lib->provider = NULL;
            current_lib->is_static = true;
            current_lib->opts.profile = dal_c_Profile_invalid;
            continue;
        }

        char* eq = strchr(line, '=');
        if (!eq) {
            (void)fprintf(stderr, "Error: %s:%d: expected key=value\n", path, i + 1);
            dal_c_Project__freeLibraryDraft(current_lib);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        *eq = '\0';
        const char* key = dal_c_Project__trimInPlace(line);
        const char* value = dal_c_Project__trimInPlace(eq + 1);
        bool allowed = current_lib
                     ? dal_c_Project__isLibraryKey(key)
                     : (dal_c_Project__isBuildDefaultsKey(key) || dal_c_Project__isPropertyKey(key));
        bool valid = current_lib
                   ? dal_c_Project__validateLibraryValue(key, value)
                   : (dal_c_Project__isBuildDefaultsKey(key)
                        ? dal_c_Project__validateBuildDefaultsValue(key, value)
                        : dal_c_Project__validatePropertyValue(key, value));
        if (!allowed || !valid) {
            if (!allowed) {
                (void)fprintf(stderr, "Error: %s:%d: unsupported key `%s` in %s\n", path, i + 1, key,
                    current_lib ? "dependency section" : "ad-hoc unit scope");
            } else {
                (void)fprintf(stderr, "Error: %s:%d: invalid value `%s` for `%s` in %s\n", path, i + 1, value, key,
                    current_lib ? "dependency section" : "ad-hoc unit scope");
            }
            dal_c_Project__freeLibraryDraft(current_lib);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        if (current_lib) {
            dal_c_Project__applyLibraryLine(current_lib, proj, key, value);
        } else {
            dal_c_Project__applyBuildDefaultsLine(&proj->defaults, key, value);
            char* resolved = dal_c_Project__resolvePropertyValue(proj->unit_root, key, value);
            dal_c_Project__applyPropertyLine(&proj->opts, key, resolved ? resolved : value);
            free(resolved);
        }
    }

    if (current_lib) {
        if (dal_c_Project__hasLibraryNamed(proj, current_lib->name)) {
            (void)fprintf(stderr, "Error: %s: duplicate dependency section `%s`\n", path, current_lib->name);
            dal_c_Project__freeLibraryDraft(current_lib);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        if (!dal_c_Project__addLibrary(proj, current_lib)) {
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
    }
    dal_c_Project__freeLines(lines, line_count);
    return true;
}

static bool dal_c_Project__configureAdHocUnit(dal_c_Project* proj, const dal_c_Cmd* cmd) {
    assert(proj != NULL);
    char* primary = dal_c_Project__primarySource(cmd);
    if (!primary) { return true; }

    proj->is_adhoc = true;
    proj->unit_source = primary;
    proj->unit_root = path_parent(primary);
    char* stem = dal_c_Project__pathWithoutExtension(primary);
    proj->unit_dh = stem ? str_format("%s.dh", stem) : NULL;
    proj->unit_lock_dh = stem ? str_format("%s.lock.dh", stem) : NULL;
    char* base = stem ? path_basename(stem) : NULL;
    proj->name = base ? strdup(base) : strdup("adhoc");
    proj->unit_id = str_format("%s-%016llx", base ? base : "unit", dal_c_Project__hashText(primary));
    free(base);
    free(stem);
    if (!proj->unit_root || !proj->unit_dh || !proj->unit_lock_dh || !proj->unit_id || !proj->name) {
        return false;
    }
    if (path_isFile(proj->unit_dh) && !dal_c_Project__parseUnitDH(proj->unit_dh, proj)) {
        return false;
    }
    return true;
}

static char* dal_c_Project__findRoot(const char* start) {
    char* current = start ? path_abs(start) : NULL;
    while (current && strlen(current) > 0) {
        char* project_dh = path_join(current, dal_c_file_detector_project);
        bool has_project_dh = path_isFile(project_dh);
        free(project_dh);
        if (has_project_dh) {
            return current;
        }

        /* workspace.dh is a discovery boundary. An ad-hoc build inside a
           workspace must not inherit an unrelated project.dh above it. */
        char* workspace_dh = path_join(current, dal_c_file_detector_workspace);
        bool has_workspace_dh = workspace_dh && path_isFile(workspace_dh);
        free(workspace_dh);
        if (has_workspace_dh) {
            free(current);
            return NULL;
        }

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(parent);
            free(current);
            current = NULL;
            break;
        }
        free(current);
        current = parent;
    }
    free(current);
    return NULL;
}

static char* dal_c_Project__findWorkspaceRoot(const char* start) {
    char* current = start ? path_abs(start) : NULL;
    while (current && current[0] != '\0') {
        char* workspace_dh = path_join(current, dal_c_file_detector_workspace);
        bool has_workspace_dh = workspace_dh && path_isFile(workspace_dh);
        free(workspace_dh);
        if (has_workspace_dh) {
            return current;
        }

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(parent);
            free(current);
            current = NULL;
            break;
        }
        free(current);
        current = parent;
    }
    free(current);
    return NULL;
}

static bool dal_c_Project__isDHRoot(const char* path) {
    if (!path || !path_isDir(path)) { return false; }
    char* dh_header = path_join(path, "include/dh.h");
    char* dh_main_header = path_join(path, "include/dh-main.h");
    char* dh_include_dir = path_join(path, "include/dh");
    char* dh_src_dir = path_join(path, "src/dh");
    bool is_dh_root = path_isFile(dh_header)
                   && path_isFile(dh_main_header)
                   && path_isDir(dh_include_dir)
                   && path_isDir(dh_src_dir);
    free(dh_header);
    free(dh_main_header);
    free(dh_include_dir);
    free(dh_src_dir);
    return is_dh_root;
}

static char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd) {
    if (cmd && cmd->dh_path_override) {
        return dal_c_Project__normalizeDHPath(cmd->dh_path_override);
    }

    char* current = env_getCWD();
    while (current && strlen(current) > 0) {
        if (dal_c_Project__isDHRoot(current)) {
            return current;
        }

        char* child_dh = path_join(current, "dh");
        if (dal_c_Project__isDHRoot(child_dh)) {
            free(current);
            return child_dh;
        }
        free(child_dh);

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(parent);
            free(current);
            current = NULL;
            break;
        }
        free(current);
        current = parent;
    }
    free(current);

    char* dh_home = env_get("DH_HOME");
    if (dal_c_Project__isDHRoot(dh_home)) {
        return dh_home;
    }
    free(dh_home);

    char* exe_dir = env_getExecutableDir();
    if (dal_c_Project__isDHRoot(exe_dir)) {
        return exe_dir;
    }
    char* exe_child_dh = exe_dir ? path_join(exe_dir, "dh") : NULL;
    if (dal_c_Project__isDHRoot(exe_child_dh)) {
        free(exe_dir);
        return exe_child_dh;
    }
    free(exe_child_dh);
    free(exe_dir);
    return NULL;
}

static char* dal_c_Project__normalizeDHPath(const char* path) {
    if (!path || path[0] == '\0') { return NULL; }
    char* abs_path = path_abs(path);
    if (abs_path && dal_c_Project__isDHRoot(abs_path)) {
        return abs_path;
    }
    free(abs_path);
    return NULL;
}

static void dal_c_Project__addToArray(char*** arr, int* count, const char* value) {
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

static void dal_c_Project__setString(char** slot, const char* value) {
    assert(slot != NULL);
    if (*slot) {
        free(*slot);
    }
    *slot = value ? strdup(value) : NULL;
}

static void dal_c_Project__appendArgs(char** slot, const char* value) {
    assert(slot != NULL);
    if (!value || value[0] == '\0') { return; }
    if (!*slot || (*slot)[0] == '\0') {
        dal_c_Project__setString(slot, value);
        return;
    }
    char* merged = str_format("%s %s", *slot, value);
    if (!merged) { return; }
    free(*slot);
    *slot = merged;
}

static char* dal_c_Project__resolveProjectPath(const dal_c_Project* proj, const char* value) {
    assert(value != NULL);
    const char* scope_root = proj ? (proj->root ? proj->root : proj->unit_root) : NULL;
    if (dal_c_Project__isAbsolutePath(value) || !scope_root) {
        return strdup(value);
    }
    return path_join(scope_root, value);
}

static bool dal_c_Project__pathHasPrefix(const char* root, const char* path) {
    assert(root != NULL);
    assert(path != NULL);
    size_t root_len = strlen(root);
    if (strncmp(root, path, root_len) != 0) {
        return false;
    }
    char next = path[root_len];
    return next == '\0' || next == '/' || next == '\\';
}

static char* dal_c_Project__pathRelativeTo(const char* root, const char* path) {
    assert(root != NULL);
    assert(path != NULL);
    assert(dal_c_Project__pathHasPrefix(root, path));
    const char* rel = path + strlen(root);
    while (*rel == '/' || *rel == '\\') {
        ++rel;
    }
    return strdup(rel);
}

static bool dal_c_Project__isAbsolutePath(const char* path) {
    return path
        && (path[0] == '/' || path[0] == '\\'
            || (strlen(path) >= 2 && path[1] == ':'));
}

static bool dal_c_Project__isSafeRelativeSubpath(const char* path) {
    if (!path || !path[0] || dal_c_Project__isAbsolutePath(path)) {
        return false;
    }
    const char* segment = path;
    for (const char* cursor = path;; ++cursor) {
        if (*cursor != '/' && *cursor != '\\' && *cursor != '\0') {
            continue;
        }
        const size_t length = (size_t)(cursor - segment);
        if (length == 2u && segment[0] == '.' && segment[1] == '.') {
            return false;
        }
        if (*cursor == '\0') {
            return true;
        }
        segment = cursor + 1;
    }
}

static bool dal_c_Project__isTrue(const char* value) {
    return value && dal_c_boolean_parse(value);
}

static dal_c_ToggleState dal_c_Project__toggleStateFromPositiveBool(const char* value) {
    dal_c_ToggleState state = dal_c_ToggleState_parse(value);
    if (state != dal_c_ToggleState_invalid) {
        return state;
    }
    return dal_c_Project__isTrue(value)
             ? dal_c_ToggleState_enabled
             : dal_c_ToggleState_disabled;
}

static dal_c_CompileEnv dal_c_Project__compileEnvFromPositiveBool(const char* value) {
    return dal_c_Project__isTrue(value)
             ? dal_c_CompileEnv_hosted
             : dal_c_CompileEnv_freestanding;
}

static dal_c_CompileEnv dal_c_Project__compileEnvFromNegativeBool(const char* value) {
    return dal_c_Project__isTrue(value)
             ? dal_c_CompileEnv_freestanding
             : dal_c_CompileEnv_hosted;
}

static dal_c_ToggleState dal_c_Project__libcLinkedFromPositiveBool(const char* value) {
    return dal_c_Project__isTrue(value)
             ? dal_c_ToggleState_enabled
             : dal_c_ToggleState_disabled;
}

static bool dal_c_Project__isBuildDefaultsKey(const char* key) {
    return key && (str_eql(key, dal_c_opt_output)
        || str_eql(key, dal_c_project_prop_kind)
        || str_eql(key, dal_c_project_prop_build_runs_tests));
}

static bool dal_c_Project__isPropertyKey(const char* key) {
    if (!key) { return false; }
    static const char* const keys[] = {
        dal_c_opt_compiler, dal_c_opt_std, dal_c_opt_arch, dal_c_opt_target,
        dal_c_opt_target_arch, dal_c_opt_target_tune, dal_c_opt_target_abi,
        dal_c_opt_sysroot, dal_c_opt_entry, dal_c_opt_comp_args, dal_c_opt_link_args,
        dal_c_opt_output_ext, dal_c_opt_link_script, dal_c_opt_objcopy, dal_c_opt_objcopy_format,
        dal_c_opt_include, dal_c_opt_isystem, dal_c_opt_link, dal_c_opt_link_dir,
        dal_c_opt_define, dal_c_opt_undef, dal_c_opt_version_namespace,
        dal_c_opt_version_core, dal_c_opt_version_prefix, dal_c_opt_version_suffix,
        dal_c_opt_version_build, "profile", dal_c_opt_freestanding, dal_c_opt_hosted,
        dal_c_opt_link_libc, dal_c_opt_link_dsl, dal_c_opt_link_default_libs,
        dal_c_opt_link_start_files, dal_c_opt_link_compiler_rt, dal_c_opt_link_stdlib,
        dal_c_opt_link_crt, dal_c_opt_link_mode, dal_c_opt_lto, dal_c_opt_prebuilt,
        dal_c_opt_omit_frame_pointer, dal_c_opt_function_sections, dal_c_opt_data_sections,
        dal_c_opt_gc_sections, dal_c_opt_whole_archive, dal_c_opt_unroll_loops,
        dal_c_opt_unwind_tables, dal_c_opt_async_unwind_tables, dal_c_opt_exceptions,
        dal_c_opt_strip, dal_c_opt_icf, dal_c_opt_merge_all_constants,
        dal_c_opt_stack_protector, dal_c_opt_macro_backtrace_limit, dal_c_opt_loose_errors,
        NULL
    };
    for (int i = 0; keys[i] != NULL; ++i) {
        if (str_eql(key, keys[i])) { return true; }
    }
    return false;
}

static bool dal_c_Project__isLibraryKey(const char* key) {
    return key && (str_eql(key, "path")
        || str_eql(key, "source")
        || str_eql(key, "archive")
        || str_eql(key, "package-root")
        || str_eql(key, "revision")
        || str_eql(key, "provider")
        || str_eql(key, "build-command")
        || str_eql(key, "install-command")
        || str_eql(key, "runtime-file")
        || str_eql(key, "runtime-files")
        || str_eql(key, "profile")
        || str_eql(key, "linking")
        || str_eql(key, dal_c_opt_test)
        || dal_c_Project__isPropertyKey(key));
}

static bool dal_c_Project__isTargetRootKey(const char* key) {
    return key && (str_eql(key, "path")
        || str_eql(key, dal_c_project_prop_kind)
        || str_eql(key, dal_c_project_prop_selection)
        || str_eql(key, dal_c_project_prop_link_project)
        || str_eql(key, dal_c_project_prop_exclude));
}

static bool dal_c_Project__isBooleanValue(const char* value) {
    return value && (str_eql(value, "on") || str_eql(value, "off")
        || str_eql(value, "true") || str_eql(value, "false")
        || str_eql(value, "yes") || str_eql(value, "no")
        || str_eql(value, "1") || str_eql(value, "0"));
}

static bool dal_c_Project__isToggleValue(const char* value) {
    return dal_c_ToggleState_parse(value) != dal_c_ToggleState_invalid;
}

static bool dal_c_Project__validOutputExt(const char* value) {
    if (!value || value[0] != '.' || value[1] == '\0') { return false; }
    for (const char* p = value + 1; *p; ++p) {
        unsigned char c = (unsigned char)*p;
        if (!(isalnum(c) || c == '_' || c == '-' || c == '.')) { return false; }
    }
    return true;
}

static bool dal_c_Project__validateBuildDefaultsValue(const char* key, const char* value) {
    if (!key || !value || value[0] == '\0') { return false; }
    if (str_eql(key, dal_c_opt_output)) { return true; }
    if (str_eql(key, dal_c_project_prop_kind)) {
        dal_c_Target kind = dal_c_Target_parse(value);
        return kind != dal_c_Target_invalid
            && kind != dal_c_Target_preprocessed
            && kind != dal_c_Target_assembly
            && kind != dal_c_Target_image;
    }
    if (str_eql(key, dal_c_project_prop_build_runs_tests)) {
        return dal_c_Project__isBooleanValue(value);
    }
    return false;
}

static bool dal_c_Project__validatePropertyValue(const char* key, const char* value) {
    if (!key || !value || value[0] == '\0') { return false; }
    if (str_eql(key, dal_c_opt_output_ext)) { return dal_c_Project__validOutputExt(value); }
    if (str_eql(key, "profile")) { return dal_c_Profile_parse(value) != dal_c_Profile_invalid; }
    if (str_eql(key, dal_c_opt_hosted) || str_eql(key, dal_c_opt_freestanding)
        || str_eql(key, dal_c_opt_link_libc) || str_eql(key, dal_c_opt_link_dsl)
        || str_eql(key, dal_c_opt_link_default_libs) || str_eql(key, dal_c_opt_link_start_files)
        || str_eql(key, dal_c_opt_link_stdlib) || str_eql(key, dal_c_opt_link_crt)) {
        return dal_c_Project__isBooleanValue(value);
    }
    if (str_eql(key, dal_c_opt_link_compiler_rt)
        || str_eql(key, dal_c_opt_omit_frame_pointer)
        || str_eql(key, dal_c_opt_function_sections)
        || str_eql(key, dal_c_opt_data_sections)
        || str_eql(key, dal_c_opt_gc_sections)
        || str_eql(key, dal_c_opt_whole_archive)
        || str_eql(key, dal_c_opt_unroll_loops)
        || str_eql(key, dal_c_opt_unwind_tables)
        || str_eql(key, dal_c_opt_async_unwind_tables)
        || str_eql(key, dal_c_opt_exceptions)
        || str_eql(key, dal_c_opt_strip)
        || str_eql(key, dal_c_opt_merge_all_constants)
        || str_eql(key, dal_c_opt_stack_protector)) {
        return dal_c_Project__isToggleValue(value);
    }
    if (str_eql(key, dal_c_opt_link_mode)) { return dal_c_LinkMode_parse(value) != dal_c_LinkMode_invalid; }
    if (str_eql(key, dal_c_opt_lto)) { return dal_c_LtoMode_parse(value) != dal_c_LtoMode_invalid; }
    if (str_eql(key, dal_c_opt_prebuilt)) { return dal_c_PrebuiltMode_parse(value) != dal_c_PrebuiltMode_invalid; }
    if (str_eql(key, dal_c_opt_icf)) {
        return str_eql(value, "auto") || str_eql(value, "off") || str_eql(value, "safe")
            || str_eql(value, "all") || str_eql(value, "on") || str_eql(value, "true")
            || str_eql(value, "false") || str_eql(value, "yes") || str_eql(value, "no")
            || str_eql(value, "1") || str_eql(value, "0");
    }
    if (str_eql(key, dal_c_opt_macro_backtrace_limit)) {
        int parsed = 0;
        return dal_c_MacroBacktraceLimit_parse(value, &parsed);
    }
    if (str_eql(key, dal_c_opt_loose_errors)) {
        return dal_c_LooseErrorsMode_parse(value) != dal_c_LooseErrorsMode_invalid;
    }
    if (str_eql(key, dal_c_opt_version_namespace)) {
        dal_c_VersionSpec temp = { 0 };
        bool ok = dal_c_VersionSpec_parseNamespace(&temp, value);
        dal_c_VersionSpec_cleanup(&temp);
        return ok;
    }
    if (str_eql(key, dal_c_opt_version_core)) {
        dal_c_VersionSpec temp = { 0 };
        bool ok = dal_c_VersionSpec_parseCore(&temp, value);
        dal_c_VersionSpec_cleanup(&temp);
        return ok;
    }
    if (str_eql(key, dal_c_opt_version_prefix)) {
        dal_c_VersionSpec temp = { 0 };
        bool ok = dal_c_VersionSpec_parsePrefix(&temp, value);
        dal_c_VersionSpec_cleanup(&temp);
        return ok;
    }
    if (str_eql(key, dal_c_opt_version_suffix)) {
        dal_c_VersionSpec temp = { 0 };
        bool ok = dal_c_VersionSpec_parseSuffix(&temp, value);
        dal_c_VersionSpec_cleanup(&temp);
        return ok;
    }
    if (str_eql(key, dal_c_opt_version_build)) {
        dal_c_VersionSpec temp = { 0 };
        bool ok = dal_c_VersionSpec_parseBuild(&temp, value);
        dal_c_VersionSpec_cleanup(&temp);
        return ok;
    }
    return true;
}

static bool dal_c_Project__validateLibraryValue(const char* key, const char* value) {
    if (!key || !value || value[0] == '\0') { return false; }
    if (str_eql(key, "profile")) {
        return str_eql(value, "default") || dal_c_Profile_parse(value) != dal_c_Profile_invalid;
    }
    if (str_eql(key, "linking")) { return dal_c_Linking_parse(value) != dal_c_Linking_invalid; }
    if (str_eql(key, dal_c_opt_test)) { return dal_c_Project__isBooleanValue(value); }
    if (dal_c_Project__isPropertyKey(key)) { return dal_c_Project__validatePropertyValue(key, value); }
    return true;
}

static bool dal_c_Project__validateTargetRootValue(const char* key, const char* value) {
    if (!key || !value || value[0] == '\0') { return false; }
    if (str_eql(key, dal_c_project_prop_kind)) { return dal_c_Target_parse(value) != dal_c_Target_invalid; }
    if (str_eql(key, dal_c_project_prop_selection)) { return dal_c_TargetSelection_parse(value) != dal_c_TargetSelection_invalid; }
    if (str_eql(key, dal_c_project_prop_link_project)) { return dal_c_Project__isBooleanValue(value); }
    return true;
}

static bool dal_c_Project__isPropertyPathKey(const char* key) {
    return key && (str_eql(key, dal_c_opt_include)
        || str_eql(key, dal_c_opt_isystem)
        || str_eql(key, dal_c_opt_link_dir)
        || str_eql(key, dal_c_opt_sysroot)
        || str_eql(key, dal_c_opt_link_script));
}

static char* dal_c_Project__resolvePropertyValue(const char* base_dir, const char* key, const char* value) {
    if (!value) { return NULL; }
    if (!dal_c_Project__isPropertyPathKey(key)
        || !base_dir || !base_dir[0]
        || dal_c_Project__isAbsolutePath(value)) {
        return strdup(value);
    }
    return path_join(base_dir, value);
}

static void dal_c_Project__applyBuildDefaultsLine(dal_c_BuildDefaults* defaults, const char* key, const char* value) {
    assert(defaults != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, dal_c_opt_output)) {
        dal_c_Project__setString(&defaults->output_name, value);
    } else if (str_eql(key, dal_c_project_prop_kind)) {
        dal_c_Target kind = dal_c_Target_parse(value);
        if (kind == dal_c_Target_invalid) {
            (void)fprintf(stderr, "Error: Invalid project kind `%s`\n", value);
        } else if (kind == dal_c_Target_preprocessed || kind == dal_c_Target_assembly || kind == dal_c_Target_image) {
            (void)fprintf(stderr, "Error: Project kind `%s` is not allowed for plain project builds\n", value);
        } else {
            defaults->target_kind = kind;
            defaults->target_kind_set = true;
        }
    } else if (str_eql(key, dal_c_project_prop_build_runs_tests)) {
        defaults->build_runs_tests = dal_c_Project__isTrue(value);
        defaults->build_runs_tests_set = true;
    }
}

static bool dal_c_Project__parseUnsigned(const char* value, unsigned* out) {
    assert(out != NULL);
    if (!value || value[0] == '\0') {
        return false;
    }
    char* end = NULL;
    unsigned long parsed = strtoul(value, &end, 10);
    if (!end || *end != '\0') {
        return false;
    }
    *out = (unsigned)parsed;
    return true;
}

static bool dal_c_Project__versionBuildIsValid(const char* value) {
    assert(value != NULL);
    for (const unsigned char* p = (const unsigned char*)value; *p != '\0'; ++p) {
        unsigned char c = *p;
        bool ok = (c >= '0' && c <= '9')
               || (c >= 'A' && c <= 'Z')
               || (c >= 'a' && c <= 'z')
               || c == '.'
               || c == '_'
               || c == '-';
        if (!ok) {
            return false;
        }
    }
    return true;
}

static bool dal_c_Project__setVersionLabelPrefix(dal_c_VersionSpec* dst, int prefix_num, const char* prefix_str) {
    assert(dst != NULL);
    assert(prefix_str != NULL);
    char* dup = strdup(prefix_str);
    if (!dup) {
        return false;
    }
    free(dst->label_prefix_str);
    dst->label_prefix_str = dup;
    dst->label_prefix_num = prefix_num;
    dst->label_prefix_set = true;
    return true;
}

static void dal_c_Project__freeLines(char** lines, int line_count) {
    if (!lines) { return; }
    for (int i = 0; i < line_count; ++i) {
        free(lines[i]);
    }
    free((void*)lines);
}

static void dal_c_Project__applyPropertyLine(dal_c_CompilerOpts* opts, const char* key, const char* value) {
    assert(opts != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, dal_c_opt_compiler)) {
        dal_c_Project__setString(&opts->compiler, value);
    } else if (str_eql(key, dal_c_opt_std)) {
        dal_c_Project__setString(&opts->c_std, value);
    } else if (str_eql(key, dal_c_opt_arch) || str_eql(key, dal_c_opt_target)) {
        dal_c_Project__setString(&opts->arch_target, value);
    } else if (str_eql(key, dal_c_opt_target_arch)) {
        dal_c_Project__setString(&opts->target_arch, value);
    } else if (str_eql(key, dal_c_opt_target_tune)) {
        dal_c_Project__setString(&opts->target_tune, value);
    } else if (str_eql(key, dal_c_opt_target_abi)) {
        dal_c_Project__setString(&opts->target_abi, value);
    } else if (str_eql(key, dal_c_opt_sysroot)) {
        dal_c_Project__setString(&opts->sysroot, value);
    } else if (str_eql(key, dal_c_opt_entry)) {
        dal_c_Project__setString(&opts->entry_symbol, value);
    } else if (str_eql(key, dal_c_opt_comp_args)) {
        dal_c_Project__appendArgs(&opts->compiler_args, value);
    } else if (str_eql(key, dal_c_opt_link_args)) {
        dal_c_Project__appendArgs(&opts->link_args, value);
    } else if (str_eql(key, dal_c_opt_output_ext)) {
        dal_c_Project__setString(&opts->output_ext, value);
    } else if (str_eql(key, dal_c_opt_link_script)) {
        dal_c_Project__setString(&opts->linker_script, value);
    } else if (str_eql(key, dal_c_opt_objcopy)) {
        dal_c_Project__setString(&opts->objcopy, value);
    } else if (str_eql(key, dal_c_opt_objcopy_format)) {
        dal_c_Project__setString(&opts->objcopy_format, value);
    } else if (str_eql(key, dal_c_opt_include)) {
        dal_c_Project__addToArray(&opts->include_paths, &opts->include_count, value);
    } else if (str_eql(key, dal_c_opt_isystem)) {
        dal_c_Project__addToArray(&opts->isystem_paths, &opts->isystem_count, value);
    } else if (str_eql(key, dal_c_opt_link)) {
        dal_c_Project__addToArray(&opts->link_libs, &opts->link_count, value);
    } else if (str_eql(key, dal_c_opt_link_dir)) {
        dal_c_Project__addToArray(&opts->link_dirs, &opts->link_dir_count, value);
    } else if (str_eql(key, dal_c_opt_define)) {
        dal_c_Project__addToArray(&opts->define_macros, &opts->define_count, value);
    } else if (str_eql(key, dal_c_opt_undef)) {
        dal_c_Project__addToArray(&opts->undef_macros, &opts->undef_count, value);
    } else if (str_eql(key, dal_c_opt_version_namespace)) {
        if (!dal_c_VersionSpec_parseNamespace(&opts->version, value)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_version_namespace, value);
        }
    } else if (str_eql(key, dal_c_opt_version_core)) {
        if (!dal_c_VersionSpec_parseCore(&opts->version, value)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_version_core, value);
        }
    } else if (str_eql(key, dal_c_opt_version_prefix)) {
        if (!dal_c_VersionSpec_parsePrefix(&opts->version, value)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_version_prefix, value);
        }
    } else if (str_eql(key, dal_c_opt_version_suffix)) {
        if (!dal_c_VersionSpec_parseSuffix(&opts->version, value)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_version_suffix, value);
        }
    } else if (str_eql(key, dal_c_opt_version_build)) {
        if (!dal_c_VersionSpec_parseBuild(&opts->version, value)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_version_build, value);
        }
    } else if (str_eql(key, "profile")) {
        dal_c_Profile profile = dal_c_Profile_parse(value);
        if (profile != dal_c_Profile_invalid) {
            opts->profile = profile;
        }
    } else if (str_eql(key, dal_c_opt_freestanding)) {
        opts->compile_env = dal_c_Project__compileEnvFromNegativeBool(value);
    } else if (str_eql(key, dal_c_opt_hosted)) {
        opts->compile_env = dal_c_Project__compileEnvFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_libc)) {
        opts->libc_linked = dal_c_Project__libcLinkedFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_dsl)) {
        opts->dsl_mode = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_default_libs)) {
        opts->default_libs_linked = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_start_files)) {
        opts->start_files_linked = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_compiler_rt)) {
        opts->compiler_rt_linked = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_stdlib)) {
        dal_c_ToggleState s = dal_c_Project__toggleStateFromPositiveBool(value);
        opts->default_libs_linked = s;
        opts->start_files_linked = s;
    } else if (str_eql(key, dal_c_opt_link_crt)) {
        opts->start_files_linked = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_link_mode)) {
        dal_c_LinkMode mode = dal_c_LinkMode_parse(value);
        if (mode == dal_c_LinkMode_invalid) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_link_mode, value);
        } else {
            opts->link_mode = mode;
        }
    } else if (str_eql(key, dal_c_opt_lto)) {
        dal_c_LtoMode mode = dal_c_LtoMode_parse(value);
        if (mode == dal_c_LtoMode_invalid) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_lto, value);
        } else {
            opts->lto_mode = mode;
        }
    } else if (str_eql(key, dal_c_opt_prebuilt)) {
        dal_c_PrebuiltMode mode = dal_c_PrebuiltMode_parse(value);
        if (mode == dal_c_PrebuiltMode_invalid) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_prebuilt, value);
        } else {
            opts->prebuilt_mode = mode;
            opts->prebuilt_mode_set = true;
        }
    } else if (str_eql(key, dal_c_opt_omit_frame_pointer)) {
        opts->omit_frame_pointer = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_function_sections)) {
        opts->function_sections = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_data_sections)) {
        opts->data_sections = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_gc_sections)) {
        opts->gc_sections = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_whole_archive)) {
        opts->whole_archive = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_unroll_loops)) {
        opts->unroll_loops = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_unwind_tables)) {
        opts->unwind_tables = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_async_unwind_tables)) {
        opts->async_unwind_tables = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_exceptions)) {
        opts->exceptions = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_strip)) {
        opts->strip_mode = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_icf)) {
        opts->icf_mode = dal_c_IcfMode_parse(value);
    } else if (str_eql(key, dal_c_opt_merge_all_constants)) {
        opts->merge_all_constants = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_stack_protector)) {
        opts->stack_protector = dal_c_Project__toggleStateFromPositiveBool(value);
    } else if (str_eql(key, dal_c_opt_macro_backtrace_limit)) {
        int limit = 0;
        if (!dal_c_MacroBacktraceLimit_parse(value, &limit)) {
            (void)fprintf(stderr, "Error: Invalid `%s` value `%s`\n", dal_c_opt_macro_backtrace_limit, value);
        } else {
            opts->macro_backtrace_limit = limit;
            opts->macro_backtrace_limit_set = true;
        }
    } else if (str_eql(key, dal_c_opt_loose_errors)) {
        opts->loose_errors = dal_c_LooseErrorsMode_parse(value);
    }
}

static bool dal_c_Project__addLibrary(dal_c_Project* proj, dal_c_Lib* lib) {
    assert(proj != NULL);
    assert(lib != NULL);

    if (!lib->provider) {
        lib->provider = strdup("dh");
    }
    if (lib->source && lib->source[0] && lib->archive && lib->archive[0]) {
        (void)fprintf(stderr, "Error: Dependency `%s` cannot declare both source= and archive=.\n",
            lib->name ? lib->name : "(unnamed)");
        dal_c_Project__freeLibraryDraft(lib);
        return false;
    }
    if (lib->archive && lib->archive[0] && lib->revision && lib->revision[0]) {
        (void)fprintf(stderr,
            "Error: Dependency `%s` cannot declare revision= with archive=; dh-c records the exact SHA-256 in lock.dh.\n",
            lib->name ? lib->name : "(unnamed)");
        dal_c_Project__freeLibraryDraft(lib);
        return false;
    }
    if (lib->package_root && lib->package_root[0]
        && !dal_c_Project__isSafeRelativeSubpath(lib->package_root)) {
        (void)fprintf(stderr,
            "Error: Dependency `%s` package-root must stay within its materialized source.\n",
            lib->name ? lib->name : "(unnamed)");
        dal_c_Project__freeLibraryDraft(lib);
        return false;
    }
    if (lib->package_root && lib->package_root[0]
        && !str_eql(lib->provider, "prebuilt")) {
        (void)fprintf(stderr,
            "Error: Dependency `%s` package-root requires provider=prebuilt.\n",
            lib->name ? lib->name : "(unnamed)");
        dal_c_Project__freeLibraryDraft(lib);
        return false;
    }
    if (lib->package_root && lib->package_root[0]
        && ((!lib->path || !lib->path[0])
            && (!lib->source || !lib->source[0])
            && (!lib->archive || !lib->archive[0]))) {
        (void)fprintf(stderr,
            "Error: Dependency `%s` package-root requires path=, source=, or archive=.\n",
            lib->name ? lib->name : "(unnamed)");
        dal_c_Project__freeLibraryDraft(lib);
        return false;
    }
    if (!lib->path && ((lib->source && lib->source[0] != '\0')
        || (lib->archive && lib->archive[0] != '\0')) && lib->name) {
        char* state_root = dal_c_Project_getStateRoot(proj);
        char* deps_root = state_root ? path_join(state_root, "deps") : NULL;
        char* src_root = deps_root ? path_join(deps_root, "src") : NULL;
        lib->path = src_root ? path_join(src_root, lib->name) : NULL;
        free(src_root);
        free(deps_root);
        free(state_root);
    }
    dal_c_Lib* new_libs = (dal_c_Lib*)realloc((void*)proj->libraries, ((size_t)proj->lib_count + 1) * sizeof(dal_c_Lib));
    assert(new_libs != NULL && "Out of memory");
    proj->libraries = new_libs;
    proj->libraries[proj->lib_count] = *lib;
    proj->lib_count++;
    free(lib);
    return true;
}

static void dal_c_Project__addSelfRoot(dal_c_Project* proj, const char* path) {
    assert(proj != NULL);
    assert(path != NULL);
    char* resolved = dal_c_Project__resolveProjectPath(proj, path);
    assert(resolved != NULL);

    for (int i = 0; i < proj->self_root_count; ++i) {
        if (str_eql(proj->self_roots[i].path, resolved)) {
            free(resolved);
            return;
        }
    }

    dal_c_SelfRoot* roots = (dal_c_SelfRoot*)realloc((void*)proj->self_roots, ((size_t)proj->self_root_count + 1) * sizeof(dal_c_SelfRoot));
    assert(roots != NULL && "Out of memory");
    proj->self_roots = roots;
    proj->self_roots[proj->self_root_count].path = resolved;
    proj->self_root_count++;
}

static bool dal_c_Project__targetRootIsValid(const dal_c_TargetRoot* root) {
    assert(root != NULL);
    if (!root->name || root->name[0] == '\0') {
        (void)fprintf(stderr, "Error: Target root name is required\n");
        return false;
    }
    if (!root->path || root->path[0] == '\0') {
        (void)fprintf(stderr, "Error: Target root `%s` is missing path=\n", root->name);
        return false;
    }
    if (root->kind == dal_c_Target_invalid) {
        (void)fprintf(stderr, "Error: Target root `%s` has invalid kind\n", root->name);
        return false;
    }
    if (root->selection == dal_c_TargetSelection_invalid) {
        (void)fprintf(stderr, "Error: Target root `%s` has invalid selection\n", root->name);
        return false;
    }
    return true;
}

static void dal_c_Project__resolveTargetRootExcludePaths(dal_c_TargetRoot* root) {
    assert(root != NULL);
    assert(root->path != NULL);

    for (int i = 0; i < root->exclude_count; ++i) {
        const char* exclude = root->exclude_paths[i];
        if (!exclude || dal_c_Project__isAbsolutePath(exclude)) {
            continue;
        }
        char* resolved = path_join(root->path, exclude);
        free(root->exclude_paths[i]);
        root->exclude_paths[i] = resolved;
    }
}

static void dal_c_Project__addTargetRoot(dal_c_Project* proj, dal_c_TargetRoot* root) {
    assert(proj != NULL);
    assert(root != NULL);
    assert(dal_c_Project__targetRootIsValid(root));
    dal_c_Project__resolveTargetRootExcludePaths(root);

    for (int i = 0; i < proj->target_root_count; ++i) {
        const dal_c_TargetRoot* existing = &proj->target_roots[i];
        if (existing->name && str_eql(existing->name, root->name)) {
            (void)fprintf(stderr, "Error: Duplicate target root name `%s`\n", root->name);
            assert(false && "duplicate target root name");
        }
        if (existing->path && str_eql(existing->path, root->path)) {
            (void)fprintf(stderr, "Error: Duplicate target root path `%s`\n", root->path);
            assert(false && "duplicate target root path");
        }
    }

    dal_c_TargetRoot* roots = (dal_c_TargetRoot*)realloc((void*)proj->target_roots, ((size_t)proj->target_root_count + 1) * sizeof(dal_c_TargetRoot));
    assert(roots != NULL && "Out of memory");
    proj->target_roots = roots;
    proj->target_roots[proj->target_root_count] = *root;
    proj->target_root_count++;
    free(root);
}

static void dal_c_Project__applyLibraryLine(dal_c_Lib* lib, const dal_c_Project* proj, const char* key, const char* value) {
    assert(lib != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, "path")) {
        char* resolved = dal_c_Project__resolveProjectPath(proj, value);
        dal_c_Project__setString(&lib->path, resolved ? resolved : value);
        free(resolved);
    } else if (str_eql(key, "source")) {
        dal_c_Project__setString(&lib->source, value);
    } else if (str_eql(key, "archive")) {
        if (strstr(value, "://")) {
            dal_c_Project__setString(&lib->archive, value);
        } else {
            char* resolved = dal_c_Project__resolveProjectPath(proj, value);
            dal_c_Project__setString(&lib->archive, resolved ? resolved : value);
            free(resolved);
        }
    } else if (str_eql(key, "package-root")) {
        dal_c_Project__setString(&lib->package_root, value);
    } else if (str_eql(key, "revision")) {
        dal_c_Project__setString(&lib->revision, value);
    } else if (str_eql(key, "provider")) {
        dal_c_Project__setString(&lib->provider, value);
    } else if (str_eql(key, "build-command")) {
        dal_c_Project__setString(&lib->build_command, value);
    } else if (str_eql(key, "install-command")) {
        dal_c_Project__setString(&lib->install_command, value);
    } else if (str_eql(key, "runtime-file") || str_eql(key, "runtime-files")) {
        dal_c_Project__addToArray(&lib->runtime_files, &lib->runtime_file_count, value);
    } else if (str_eql(key, "profile")) {
        dal_c_Profile profile = dal_c_Profile_parse(value);
        if (profile != dal_c_Profile_invalid) {
            lib->opts.profile = profile;
        }
    } else if (str_eql(key, "linking")) {
        lib->is_static = !str_eql(value, dal_c_linking_shared);
    } else if (str_eql(key, dal_c_opt_test)) {
        lib->test_enabled = dal_c_Project__isTrue(value);
        lib->test_enabled_set = true;
    } else {
        const char* property_root = proj ? (proj->root ? proj->root : proj->unit_root) : NULL;
        char* resolved_value = dal_c_Project__resolvePropertyValue(property_root, key, value);
        dal_c_Project__applyPropertyLine(&lib->opts, key, resolved_value ? resolved_value : value);
        free(resolved_value);
    }
}

static void dal_c_Project__applyTargetRootLine(dal_c_TargetRoot* root, const dal_c_Project* proj, const char* key, const char* value) {
    assert(root != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, "path")) {
        char* resolved = dal_c_Project__resolveProjectPath(proj, value);
        dal_c_Project__setString(&root->path, resolved);
        free(resolved);
    } else if (str_eql(key, dal_c_project_prop_kind)) {
        dal_c_Target kind = dal_c_Target_parse(value);
        if (kind == dal_c_Target_invalid) {
            (void)fprintf(stderr, "Error: Invalid target-root kind `%s`\n", value);
            root->kind = dal_c_Target_invalid;
        } else {
            root->kind = kind;
        }
    } else if (str_eql(key, dal_c_project_prop_selection)) {
        dal_c_TargetSelection selection = dal_c_TargetSelection_parse(value);
        if (selection == dal_c_TargetSelection_invalid) {
            (void)fprintf(stderr, "Error: Invalid target-root selection `%s`\n", value);
            root->selection = dal_c_TargetSelection_invalid;
        } else {
            root->selection = selection;
        }
    } else if (str_eql(key, dal_c_project_prop_link_project)) {
        root->link_project = dal_c_Project__isTrue(value);
    } else if (str_eql(key, dal_c_project_prop_exclude)) {
        if (root->path && !dal_c_Project__isAbsolutePath(value)) {
            char* resolved = path_join(root->path, value);
            dal_c_Project__addToArray(&root->exclude_paths, &root->exclude_count, resolved);
            free(resolved);
        } else {
            dal_c_Project__addToArray(&root->exclude_paths, &root->exclude_count, value);
        }
    }
}


static void dal_c_Project__setLockReason(char** reason_out, const char* text) {
    if (!reason_out) { return; }
    free(*reason_out);
    *reason_out = text ? strdup(text) : NULL;
}

bool dal_c_Project_readDependencyLock(const dal_c_Project* proj, const dal_c_Lib* lib, char** revision_out, char** reason_out) {
    if (revision_out) { *revision_out = NULL; }
    if (reason_out) { *reason_out = NULL; }
    if (!proj || !lib || !lib->name || (!proj->root && !proj->unit_lock_dh)) {
        dal_c_Project__setLockReason(reason_out, "dependency lock scope is unavailable");
        return false;
    }

    char* lock_path = dal_c_Project_getDependencyLockPath(proj);
    if (!lock_path || !path_isFile(lock_path)) {
        char* reason = str_format("missing dependency lock: %s", lock_path ? lock_path : "lock.dh");
        dal_c_Project__setLockReason(reason_out, reason);
        free(reason);
        free(lock_path);
        return false;
    }

    int line_count = 0;
    char** lines = file_readLines(lock_path, &line_count);
    if (!lines) {
        char* reason = str_format("failed to read dependency lock: %s", lock_path);
        dal_c_Project__setLockReason(reason_out, reason);
        free(reason);
        free(lock_path);
        return false;
    }

    bool in_section = false;
    bool found = false;
    char* provider = NULL;
    char* source = NULL;
    char* archive = NULL;
    char* revision = NULL;
    for (int i = 0; i < line_count; ++i) {
        char* line = dal_c_Project__trimInPlace(lines[i]);
        if (!line[0] || line[0] == '#' || line[0] == ';') { continue; }
        if (line[0] == '[') {
            char* end = strchr(line, ']');
            if (!end) { continue; }
            *end = '\0';
            if (in_section) { break; }
            in_section = str_eql(dal_c_Project__trimInPlace(line + 1), lib->name);
            found = found || in_section;
            continue;
        }
        if (!in_section) { continue; }
        char* eq = strchr(line, '=');
        if (!eq) { continue; }
        *eq = '\0';
        const char* key = dal_c_Project__trimInPlace(line);
        const char* value = dal_c_Project__trimInPlace(eq + 1);
        if (str_eql(key, "provider")) {
            free(provider);
            provider = strdup(value);
        } else if (str_eql(key, "source")) {
            free(source);
            source = strdup(value);
        } else if (str_eql(key, "archive")) {
            free(archive);
            archive = strdup(value);
        } else if (str_eql(key, "revision")) {
            free(revision);
            revision = strdup(value);
        }
    }
    dal_c_Project__freeLines(lines, line_count);

    const char* expected_provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
    const bool expects_archive = lib->archive && lib->archive[0];
    const bool locator_matches = expects_archive
        ? archive && str_eql(archive, lib->archive)
        : source && lib->source && str_eql(source, lib->source);
    bool ok = found && provider && revision && revision[0]
           && str_eql(provider, expected_provider)
           && locator_matches;
    if (!ok) {
        char* reason = NULL;
        if (!found) {
            reason = str_format("dependency `%s` is not recorded in %s", lib->name, lock_path);
        } else if (!provider || !str_eql(provider, expected_provider)) {
            reason = str_format("dependency `%s` provider differs from %s", lib->name, lock_path);
        } else if (expects_archive && (!archive || !str_eql(archive, lib->archive))) {
            reason = str_format("dependency `%s` archive differs from %s", lib->name, lock_path);
        } else if (!expects_archive && (!source || !lib->source || !str_eql(source, lib->source))) {
            reason = str_format("dependency `%s` source differs from %s", lib->name, lock_path);
        } else {
            reason = str_format("dependency `%s` has no resolved revision in %s", lib->name, lock_path);
        }
        dal_c_Project__setLockReason(reason_out, reason);
        free(reason);
    } else if (revision_out) {
        *revision_out = strdup(revision);
    }

    free(revision);
    free(archive);
    free(source);
    free(provider);
    free(lock_path);
    return ok;
}

bool dal_c_Project_dependencySourceMatchesLock(const dal_c_Project* proj, const dal_c_Lib* lib, char** reason_out) {
    if (reason_out) { *reason_out = NULL; }
    if (!lib || ((!lib->source || !lib->source[0]) && (!lib->archive || !lib->archive[0]))) { return true; }

    char* locked_revision = NULL;
    if (!dal_c_Project_readDependencyLock(proj, lib, &locked_revision, reason_out)) {
        free(locked_revision);
        return false;
    }
    if (!lib->path || !path_isDir(lib->path)) {
        char* reason = str_format("dependency `%s` source is missing; run `dh-c fetch`", lib->name);
        dal_c_Project__setLockReason(reason_out, reason);
        free(reason);
        free(locked_revision);
        return false;
    }

    if (lib->archive && lib->archive[0]) {
        bool ok = dal_c__archiveMaterializationMatches(lib->path, locked_revision, reason_out);
        free(locked_revision);
        return ok;
    }

    const char* argv[] = { "git", "-C", lib->path, "rev-parse", "HEAD", NULL };
    char* head = proc_output(argv);
    char* trimmed = head ? dal_c_Project__trimInPlace(head) : NULL;
    bool ok = trimmed && str_eql(trimmed, locked_revision);
    if (!ok) {
        char* reason = str_format(
            "dependency `%s` checkout does not match lock.dh: checkout=%s locked=%s",
            lib->name, trimmed && trimmed[0] ? trimmed : "(unavailable)", locked_revision
        );
        dal_c_Project__setLockReason(reason_out, reason);
        free(reason);
    }
    free(head);
    free(locked_revision);
    return ok;
}

static bool dal_c_Project__parseProjectDH(const char* path, dal_c_Project* proj) {
    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    dal_c_Lib* current_lib = NULL;
    dal_c_TargetRoot* current_target_root = NULL;
    proj->opts.profile = dal_c_Profile_invalid;

    for (int i = 0; i < line_count; ++i) {
        char* line = dal_c_Project__trimInPlace(lines[i]);
        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';') {
            continue;
        }

        if (line[0] == '[') {
            if (current_lib) {
                if (!dal_c_Project__addLibrary(proj, current_lib)) {
                    dal_c_Project__freeLines(lines, line_count);
                    return false;
                }
                current_lib = NULL;
            }
            if (current_target_root) {
                if (!dal_c_Project__targetRootIsValid(current_target_root)) {
                    free(current_target_root->name);
                    free(current_target_root->path);
                    free(current_target_root);
                    dal_c_Project__freeLines(lines, line_count);
                    return false;
                }
                for (int j = 0; j < proj->target_root_count; ++j) {
                    const dal_c_TargetRoot* existing = &proj->target_roots[j];
                    if ((existing->name && str_eql(existing->name, current_target_root->name))
                        || (existing->path && current_target_root->path && str_eql(existing->path, current_target_root->path))) {
                        (void)fprintf(stderr, "Error: Duplicate target-root contract `%s`\n", current_target_root->name);
                        free(current_target_root->name);
                        free(current_target_root->path);
                        free(current_target_root);
                        dal_c_Project__freeLines(lines, line_count);
                        return false;
                    }
                }
                dal_c_Project__addTargetRoot(proj, current_target_root);
                current_target_root = NULL;
            }

            char* end = strchr(line, ']');
            if (!end || dal_c_Project__trimInPlace(end + 1)[0] != '\0') {
                (void)fprintf(stderr, "Error: %s:%d: malformed section header\n", path, i + 1);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            *end = '\0';
            char* section = dal_c_Project__trimInPlace(line + 1);
            if (section[0] == '\0') {
                (void)fprintf(stderr, "Error: %s:%d: empty section name\n", path, i + 1);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
            if (str_startsWith(section, dal_c_project_section_target_root " ")) {
                const char* name = dal_c_Project__trimInPlace(section + strlen(dal_c_project_section_target_root));
                if (name[0] == '\0') {
                    (void)fprintf(stderr, "Error: %s:%d: target-root requires a name\n", path, i + 1);
                    dal_c_Project__freeLines(lines, line_count);
                    return false;
                }
                current_target_root = calloc(1, sizeof(dal_c_TargetRoot));
                assert(current_target_root != NULL);
                current_target_root->name = strdup(name);
                current_target_root->kind = dal_c_Target_executable;
                current_target_root->selection = dal_c_TargetSelection_path;
                current_target_root->link_project = true;
                current_target_root->builtin = false;
            } else {
                current_lib = calloc(1, sizeof(dal_c_Lib));
                assert(current_lib != NULL);
                current_lib->is_static = true;
                current_lib->opts.profile = dal_c_Profile_invalid;
                current_lib->name = strdup(section);
            }
            continue;
        }

        char* eq = strchr(line, '=');
        if (!eq) {
            (void)fprintf(stderr, "Error: %s:%d: expected key=value\n", path, i + 1);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        *eq = '\0';
        const char* key = dal_c_Project__trimInPlace(line);
        const char* value = dal_c_Project__trimInPlace(eq + 1);
        if (key[0] == '\0') {
            (void)fprintf(stderr, "Error: %s:%d: empty key\n", path, i + 1);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        bool key_allowed = false;
        if (current_target_root) {
            key_allowed = dal_c_Project__isTargetRootKey(key);
        } else if (current_lib) {
            key_allowed = dal_c_Project__isLibraryKey(key);
        } else {
            key_allowed = str_eql(key, dal_c_project_prop_pch)
                       || str_eql(key, dal_c_project_prop_pch_exclude)
                       || str_eql(key, dal_c_project_prop_self_root)
                       || str_eql(key, dal_c_project_prop_exclude)
                       || dal_c_Project__isBuildDefaultsKey(key)
                       || dal_c_Project__isPropertyKey(key);
        }
        if (!key_allowed) {
            (void)fprintf(stderr, "Error: %s:%d: unsupported key `%s` in %s\n",
                          path, i + 1, key,
                          current_target_root ? "target-root section" : (current_lib ? "dependency section" : "project scope"));
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        bool value_valid = true;
        if (current_target_root) {
            value_valid = dal_c_Project__validateTargetRootValue(key, value);
        } else if (current_lib) {
            value_valid = dal_c_Project__validateLibraryValue(key, value);
        } else if (dal_c_Project__isBuildDefaultsKey(key)) {
            value_valid = dal_c_Project__validateBuildDefaultsValue(key, value);
        } else if (dal_c_Project__isPropertyKey(key)) {
            value_valid = dal_c_Project__validatePropertyValue(key, value);
        } else {
            value_valid = value[0] != '\0';
        }
        if (!value_valid) {
            (void)fprintf(stderr, "Error: %s:%d: invalid value `%s` for `%s` in %s\n",
                          path, i + 1, value, key,
                          current_target_root ? "target-root section" : (current_lib ? "dependency section" : "project scope"));
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        if (!current_lib && !current_target_root && str_eql(key, dal_c_project_prop_pch)) {
            if (str_eql(value, dal_c_pch_value_off)) {
                proj->pch_enabled = false;
                free(proj->pch_header_override);
                proj->pch_header_override = NULL;
            } else if (str_eql(value, dal_c_pch_value_auto)) {
                proj->pch_enabled = true;
                free(proj->pch_header_override);
                proj->pch_header_override = NULL;
            } else if (str_eql(value, dal_c_pch_value_deps)) {
                proj->pch_enabled = true;
                dal_c_Project__setString(&proj->pch_header_override, dal_c_pch_value_deps);
            } else {
                proj->pch_enabled = true;
                char* pch_path = dal_c_Project__resolveProjectPath(proj, value);
                dal_c_Project__setString(&proj->pch_header_override, pch_path);
                free(pch_path);
            }
        } else if (!current_lib && !current_target_root && str_eql(key, dal_c_project_prop_pch_exclude)) {
            dal_c_Project__addToArray(&proj->pch_exclude_headers, &proj->pch_exclude_count, value);
        } else if (!current_lib && !current_target_root && str_eql(key, dal_c_project_prop_self_root)) {
            proj->has_explicit_self_roots = true;
            dal_c_Project__addSelfRoot(proj, value);
        } else if (!current_lib && !current_target_root && str_eql(key, dal_c_project_prop_exclude)) {
            char* exclude_path = dal_c_Project__resolveProjectPath(proj, value);
            dal_c_Project__addToArray(&proj->exclude_paths, &proj->exclude_count, exclude_path);
            free(exclude_path);
        } else if (current_target_root) {
            dal_c_Project__applyTargetRootLine(current_target_root, proj, key, value);
        } else if (current_lib) {
            dal_c_Project__applyLibraryLine(current_lib, proj, key, value);
        } else {
            dal_c_Project__applyBuildDefaultsLine(&proj->defaults, key, value);
            char* resolved_value = dal_c_Project__resolvePropertyValue(proj->root, key, value);
            dal_c_Project__applyPropertyLine(&proj->opts, key, resolved_value ? resolved_value : value);
            free(resolved_value);
        }
    }

    if (current_lib) {
        if (!dal_c_Project__addLibrary(proj, current_lib)) {
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
    }
    if (current_target_root) {
        if (!dal_c_Project__targetRootIsValid(current_target_root)) {
            free(current_target_root->name);
            free(current_target_root->path);
            free(current_target_root);
            dal_c_Project__freeLines(lines, line_count);
            return false;
        }
        for (int i = 0; i < proj->target_root_count; ++i) {
            const dal_c_TargetRoot* existing = &proj->target_roots[i];
            if ((existing->name && str_eql(existing->name, current_target_root->name))
                || (existing->path && current_target_root->path && str_eql(existing->path, current_target_root->path))) {
                (void)fprintf(stderr, "Error: Duplicate target-root contract `%s`\n", current_target_root->name);
                free(current_target_root->name);
                free(current_target_root->path);
                free(current_target_root);
                dal_c_Project__freeLines(lines, line_count);
                return false;
            }
        }
        dal_c_Project__addTargetRoot(proj, current_target_root);
    }
    dal_c_Project__freeLines(lines, line_count);
    return true;
}

static char* dal_c_Project__depsPreludeHeaderPath(const dal_c_Project* proj) {
    if (!proj) { return NULL; }
    char* lib_dir = dal_c_Project_getLibDir(proj);
    if (!lib_dir) { return NULL; }
    char* deps_header = path_join(lib_dir, "deps.h");
    free(lib_dir);
    return deps_header;
}

static char* dal_c_Project__findIncludeHeader(const dal_c_Project* proj, const char* header) {
    if (!proj || !proj->root || !header) { return NULL; }
    char* inc_dir = dal_c_Project_getIncludeDir(proj);
    if (!inc_dir) { return NULL; }
    char* header_path = path_join(inc_dir, header);
    free(inc_dir);
    if (header_path && path_isFile(header_path)) { return header_path; }
    free(header_path);
    return NULL;
}

static char* dal_c_Project__findDHInstallHeader(const dal_c_Project* proj, const char* header) {
    if (!proj || !proj->dh_path || !header) { return NULL; }
    char* inc_dir = path_join(proj->dh_path, dal_c_dir_include);
    if (!inc_dir) { return NULL; }
    char* header_path = path_join(inc_dir, header);
    free(inc_dir);
    if (header_path && path_isFile(header_path)) { return header_path; }
    free(header_path);
    return NULL;
}

static bool dal_c_Project__usesDHLibraryDefault(const dal_c_Project* proj) {
    return proj
        && proj->dh_path
        && dal_c_ToggleState_resolve(proj->opts.dsl_mode, true)
        && !(proj->root && str_eql(proj->root, proj->dh_path));
}

static char* dal_c_Project__detectPCH(const dal_c_Project* proj) {
    if (!proj || !proj->root || !proj->pch_enabled) { return NULL; }

    if (proj->pch_header_override) {
        if (str_eql(proj->pch_header_override, dal_c_pch_value_deps)) {
            return dal_c_Project__depsPreludeHeaderPath(proj);
        }
        if (path_isFile(proj->pch_header_override)) {
            return strdup(proj->pch_header_override);
        }
        char* inc_dir_for_override = dal_c_Project_getIncludeDir(proj);
        char* override_name = path_basename(proj->pch_header_override);
        char* inc_relative = inc_dir_for_override && override_name ? path_join(inc_dir_for_override, override_name) : NULL;
        free(inc_dir_for_override);
        free(override_name);
        if (inc_relative && path_isFile(inc_relative)) { return inc_relative; }
        free(inc_relative);
        return NULL;
    }

    char* dh_pch_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh_bundle);
    if (dh_pch_header) { return dh_pch_header; }
    char* dh_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh);
    if (dh_header) { return dh_header; }
    char* sys_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh_sys);
    if (sys_header) { return sys_header; }
    char* prl_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh_prl);
    if (prl_header) { return prl_header; }
    char* core_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh_core);
    if (core_header) { return core_header; }
    char* builtin_header = dal_c_Project__findIncludeHeader(proj, dal_c_pch_header_dh_builtin);
    if (builtin_header) { return builtin_header; }

    if (!dal_c_Project__isDHRoot(proj->root)) {
        if (proj->lib_count > 0) {
            return dal_c_Project__depsPreludeHeaderPath(proj);
        }
        if (dal_c_Project__usesDHLibraryDefault(proj)) {
            char* installed_dh_bundle = dal_c_Project__findDHInstallHeader(proj, dal_c_pch_header_dh_bundle);
            if (installed_dh_bundle) { return installed_dh_bundle; }
            char* installed_dh_header = dal_c_Project__findDHInstallHeader(proj, dal_c_pch_header_dh);
            if (installed_dh_header) { return installed_dh_header; }
        }
    }

    return NULL;
}

static void dal_c_Project__ensureBuiltinTargetRoots(dal_c_Project* proj) {
    assert(proj != NULL);
    if (!proj->root) { return; }
    if (!proj->has_explicit_self_roots) {
        dal_c_Project__addSelfRoot(proj, dal_c_Project_getCategoryDirName(proj, dal_c_dir_src));
    }

    struct dal_c_BuiltinRootSpec {
        const char* name;
        const char* canonical_dir;
    };
    static const struct dal_c_BuiltinRootSpec builtin_roots[] = {
        { dal_c_dir_samples, dal_c_dir_samples },
        { dal_c_dir_examples, dal_c_dir_examples },
        { dal_c_dir_tests, dal_c_dir_tests },
    };

    for (int i = 0; i < (int)(sizeof(builtin_roots) / sizeof(builtin_roots[0])); ++i) {
        const char* name = builtin_roots[i].name;
        if (dal_c_Project_findTargetRootByName(proj, name)) {
            continue;
        }

        char* dir_path = dal_c_Project_getCategoryDir(proj, builtin_roots[i].canonical_dir);
        const dal_c_TargetRoot* existing_by_path = dal_c_Project_findTargetRootByPath(proj, dir_path);
        if (existing_by_path && existing_by_path->path && str_eql(existing_by_path->path, dir_path)) {
            free(dir_path);
            continue;
        }

        dal_c_TargetRoot* root = calloc(1, sizeof(dal_c_TargetRoot));
        assert(root != NULL);
        root->name = strdup(name);
        root->path = dir_path;
        root->kind = dal_c_Target_executable;
        root->selection = dal_c_TargetSelection_path;
        root->link_project = true;
        root->builtin = true;
        dal_c_Project__addTargetRoot(proj, root);
    }
}

static bool dal_c_Project__resolveCategoryDir(dal_c_Project* proj, char** slot, const char* const* aliases) {
    assert(proj != NULL);
    assert(slot != NULL);
    assert(aliases != NULL);

    const char* chosen = aliases[0];
    int match_count = 0;
    char* matches = NULL;

    for (int i = 0; aliases[i] != NULL; ++i) {
        char* candidate = path_join(proj->root, aliases[i]);
        bool exists = path_isDir(candidate);
        free(candidate);
        if (!exists) { continue; }

        chosen = aliases[i];
        match_count++;
        if (!matches) {
            matches = strdup(aliases[i]);
        } else {
            char* joined = str_format("%s, %s", matches, aliases[i]);
            free(matches);
            matches = joined;
        }
    }

    if (match_count > 1) {
        (void)fprintf(stderr, "Error: Multiple aliases for category `%s`: %s\n", aliases[0], matches ? matches : "");
        free(matches);
        return false;
    }

    free(matches);
    dal_c_Project__setString(slot, chosen);
    return true;
}

static bool dal_c_Project__resolveCategoryDirs(dal_c_Project* proj) {
    assert(proj != NULL);
    if (!proj->root) { return true; }

    static const char* include_aliases[] = {
        dal_c_dir_include, dal_c_dir_include_alias_includes, dal_c_dir_include_alias_inc, NULL
    };
    static const char* src_aliases[] = {
        dal_c_dir_src, dal_c_dir_src_alias_source, dal_c_dir_src_alias_sources, NULL
    };
    static const char* tests_aliases[] = { dal_c_dir_tests, dal_c_dir_tests_alias_test, NULL };
    static const char* samples_aliases[] = { dal_c_dir_samples, dal_c_dir_samples_alias_sample, NULL };
    static const char* examples_aliases[] = { dal_c_dir_examples, dal_c_dir_examples_alias_example, NULL };

    return dal_c_Project__resolveCategoryDir(proj, &proj->include_dir_name, include_aliases)
        && dal_c_Project__resolveCategoryDir(proj, &proj->src_dir_name, src_aliases)
        && dal_c_Project__resolveCategoryDir(proj, &proj->tests_dir_name, tests_aliases)
        && dal_c_Project__resolveCategoryDir(proj, &proj->samples_dir_name, samples_aliases)
        && dal_c_Project__resolveCategoryDir(proj, &proj->examples_dir_name, examples_aliases);
}
