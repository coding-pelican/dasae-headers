#include "internal.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/env.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// === PRIVATE HELPERS ===

static char* dal_c_Project__findRoot(const char* start);
static char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd);
static bool dal_c_Project__isDHRoot(const char* path);
static void dal_c_Project__parseProjectDh(const char* path, dal_c_Project* proj);
static char* dal_c_Project__detectPCH(const dal_c_Project* proj);
static void dal_c_Project__addLibrary(dal_c_Project* proj, dal_c_Lib* lib);
static void dal_c_Project__addToArray(char*** arr, int* count, const char* value);
static void dal_c_Project__setString(char** slot, const char* value);
static bool dal_c_Project__isAbsolutePath(const char* path);
static bool dal_c_Project__isTrue(const char* value);
static void dal_c_Project__freeLines(char** lines, int line_count);
static void dal_c_Project__applyBuildDefaultsLine(dal_c_BuildDefaults* defaults, const char* key, const char* value);
static void dal_c_Project__applyPropertyLine(dal_c_CompilerOpts* opts, const char* key, const char* value);
static void dal_c_Project__applyLibraryLine(dal_c_Lib* lib, const dal_c_Project* proj, const char* key, const char* value);
static char* dal_c_Project__resolveProjectPath(const dal_c_Project* proj, const char* value);
static bool dal_c_Project__resolveCategoryDirs(dal_c_Project* proj);
static bool dal_c_Project__resolveCategoryDir(dal_c_Project* proj, char** slot, const char* const* aliases);

// === PUBLIC API ===

dal_c_Project* dal_c_Project_detect(const dal_c_Cmd* cmd) {
    dal_c_Project* proj = calloc(1, sizeof(dal_c_Project));
    if (!proj) { return NULL; }

    char* cwd = env_getCWD();
    proj->root = dal_c_Project__findRoot(cwd);
    free(cwd);

    if (proj->root) {
        proj->name = path_basename(proj->root);
        proj->project_dh = path_join(proj->root, dal_c_file_project_dh);
        proj->pch_enabled = true;
        if (path_isFile(proj->project_dh)) {
            dal_c_Project__parseProjectDh(proj->project_dh, proj);
        }
        if (!dal_c_Project__resolveCategoryDirs(proj)) {
            dal_c_Project_cleanup(&proj);
            return NULL;
        }
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
    proj->name = path_basename(proj->root);
    proj->dh_path = dh_path ? strdup(dh_path) : NULL;
    proj->project_dh = path_join(proj->root, dal_c_file_project_dh);
    proj->pch_enabled = true;
    if (proj->project_dh && path_isFile(proj->project_dh)) {
        dal_c_Project__parseProjectDh(proj->project_dh, proj);
    }
    if (!dal_c_Project__resolveCategoryDirs(proj)) {
        dal_c_Project_cleanup(&proj);
        return NULL;
    }
    proj->pch_header = dal_c_Project__detectPCH(proj);
    return proj;
}

void dal_c_Project_cleanup(dal_c_Project** self) {
    if (!self || !*self) { return; }
    dal_c_Project* proj = *self;

    dal_c_CompilerOpts_cleanup(&proj->opts);
    for (int i = 0; i < proj->lib_count; ++i) {
        dal_c_Lib* lib = &proj->libraries[i];
        free(lib->name);
        free(lib->path);
        dal_c_CompilerOpts_cleanup(&lib->opts);
    }
    free(proj->libraries);
    free(proj->root);
    free(proj->name);
    free(proj->dh_path);
    free(proj->project_dh);
    free(proj->src_dir_name);
    free(proj->include_dir_name);
    free(proj->tests_dir_name);
    free(proj->samples_dir_name);
    free(proj->examples_dir_name);
    free(proj->pch_header_override);
    free(proj->pch_header);
    dal_c_BuildDefaults_cleanup(&proj->defaults);
    for (int i = 0; i < proj->pch_exclude_count; ++i) {
        free(proj->pch_exclude_headers[i]);
    }
    free((void*)proj->pch_exclude_headers);
    free(proj);
    *self = NULL;
}

void dal_c_CompilerOpts_cleanup(dal_c_CompilerOpts* opts) {
    if (!opts) { return; }
    free(opts->compiler);
    free(opts->c_std);
    free(opts->arch_target);
    free(opts->sysroot);
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
    memset(opts, 0, sizeof(*opts));
}

void dal_c_CompilerOpts_merge(dal_c_CompilerOpts* dst, const dal_c_CompilerOpts* src) {
    assert(dst != NULL);
    if (!src) { return; }

    if (src->compiler) { dal_c_Project__setString(&dst->compiler, src->compiler); }
    if (src->c_std) { dal_c_Project__setString(&dst->c_std, src->c_std); }
    if (src->arch_target) { dal_c_Project__setString(&dst->arch_target, src->arch_target); }
    if (src->sysroot) { dal_c_Project__setString(&dst->sysroot, src->sysroot); }
    if (src->profile != dal_c_Profile_invalid) { dst->profile = src->profile; }
    dst->freestanding = dst->freestanding || src->freestanding;
    dst->loose_errors = dst->loose_errors || src->loose_errors;
    dst->no_dsl = dst->no_dsl || src->no_dsl;

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
}

void dal_c_BuildDefaults_cleanup(dal_c_BuildDefaults* defaults) {
    if (!defaults) { return; }
    free(defaults->output_name);
    memset(defaults, 0, sizeof(*defaults));
}

void dal_c_BuildDefaults_merge(dal_c_BuildDefaults* dst, const dal_c_BuildDefaults* src) {
    assert(dst != NULL);
    if (!src) { return; }
    if (src->output_name) {
        dal_c_Project__setString(&dst->output_name, src->output_name);
    }
    if (src->build_runs_tests_set) {
        dst->build_runs_tests = src->build_runs_tests;
        dst->build_runs_tests_set = true;
    }
}

bool dal_c_BuildDefaults_applyDhFile(dal_c_BuildDefaults* dst, const char* path) {
    assert(dst != NULL);
    if (!path || !path_isFile(path)) { return false; }

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    bool applied = false;
    for (int i = 0; i < line_count; ++i) {
        char* line = str_trim(lines[i]);
        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';' || line[0] == '[') {
            continue;
        }

        char* eq = strchr(line, '=');
        if (!eq) { continue; }
        *eq = '\0';
        dal_c_Project__applyBuildDefaultsLine(dst, str_trim(line), str_trim(eq + 1));
        applied = true;
    }

    dal_c_Project__freeLines(lines, line_count);
    return applied;
}

bool dal_c_CompilerOpts_applyDhFile(dal_c_CompilerOpts* dst, const char* path) {
    assert(dst != NULL);
    if (!path || !path_isFile(path)) { return false; }

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    bool applied = false;
    for (int i = 0; i < line_count; ++i) {
        char* line = str_trim(lines[i]);
        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';' || line[0] == '[') {
            continue;
        }

        char* eq = strchr(line, '=');
        if (!eq) { continue; }
        *eq = '\0';
        dal_c_Project__applyPropertyLine(dst, str_trim(line), str_trim(eq + 1));
        applied = true;
    }

    dal_c_Project__freeLines(lines, line_count);
    return applied;
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
        bool is_project_dh = base && str_eql(base, dal_c_file_project_dh);
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
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_dir_lib);
}

char* dal_c_Project_getDepsDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    char* lib_dir = path_join(proj->root, dal_c_dir_lib);
    char* deps_dir = path_join(lib_dir, dal_c_dir_deps);
    free(lib_dir);
    return deps_dir;
}

// === PRIVATE IMPLEMENTATIONS ===

static char* dal_c_Project__findRoot(const char* start) {
    char* current = start ? strdup(start) : NULL;
    while (current && strlen(current) > 0) {
        char* project_dh = path_join(current, dal_c_file_project_dh);
        bool has_project_dh = path_isFile(project_dh);
        free(project_dh);
        if (has_project_dh) {
            return current;
        }

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(current);
            free(parent);
            break;
        }
        free(current);
        current = parent;
    }

    return NULL;
}

static bool dal_c_Project__isDHRoot(const char* path) {
    if (!path || !path_isDir(path)) { return false; }
    char* dh_header = path_join(path, "include/dh.h");
    char* dh_main_header = path_join(path, "include/dh-main.h");
    char* dh_include_dir = path_join(path, "include/dh");
    char* blocks_src_dir = path_join(path, "libs/BlocksRuntime/src");
    bool is_dh_root = path_isFile(dh_header)
                   && path_isFile(dh_main_header)
                   && path_isDir(dh_include_dir)
                   && path_isDir(blocks_src_dir);
    free(dh_header);
    free(dh_main_header);
    free(dh_include_dir);
    free(blocks_src_dir);
    return is_dh_root;
}

static char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd) {
    if (cmd && cmd->dh_path_override) {
        return dal_c_Project__isDHRoot(cmd->dh_path_override) ? strdup(cmd->dh_path_override) : NULL;
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
            free(current);
            free(parent);
            break;
        }
        free(current);
        current = parent;
    }

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

static char* dal_c_Project__resolveProjectPath(const dal_c_Project* proj, const char* value) {
    assert(value != NULL);
    if (dal_c_Project__isAbsolutePath(value) || !proj || !proj->root) {
        return strdup(value);
    }
    return path_join(proj->root, value);
}

static bool dal_c_Project__isAbsolutePath(const char* path) {
    return path
        && (path[0] == '/' || path[0] == '\\'
            || (strlen(path) >= 2 && path[1] == ':'));
}

static bool dal_c_Project__isTrue(const char* value) {
    return value && dal_c_boolean_parse(value);
}

static void dal_c_Project__applyBuildDefaultsLine(dal_c_BuildDefaults* defaults, const char* key, const char* value) {
    assert(defaults != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, dal_c_opt_output)) {
        dal_c_Project__setString(&defaults->output_name, value);
    } else if (str_eql(key, dal_c_project_prop_build_runs_tests)) {
        defaults->build_runs_tests = dal_c_Project__isTrue(value);
        defaults->build_runs_tests_set = true;
    }
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
    } else if (str_eql(key, dal_c_opt_sysroot)) {
        dal_c_Project__setString(&opts->sysroot, value);
    } else if (str_eql(key, dal_c_opt_include)) {
        dal_c_Project__addToArray(&opts->include_paths, &opts->include_count, value);
    } else if (str_eql(key, dal_c_opt_isystem)) {
        dal_c_Project__addToArray(&opts->isystem_paths, &opts->isystem_count, value);
    } else if (str_eql(key, dal_c_opt_link)) {
        dal_c_Project__addToArray(&opts->link_libs, &opts->link_count, value);
    } else if (str_eql(key, dal_c_opt_define)) {
        dal_c_Project__addToArray(&opts->define_macros, &opts->define_count, value);
    } else if (str_eql(key, dal_c_opt_undef)) {
        dal_c_Project__addToArray(&opts->undef_macros, &opts->undef_count, value);
    } else if (str_eql(key, "profile")) {
        dal_c_Profile profile = dal_c_Profile_parse(value);
        if (profile != dal_c_Profile_invalid) {
            opts->profile = profile;
        }
    } else if (str_eql(key, dal_c_opt_freestanding)) {
        opts->freestanding = dal_c_Project__isTrue(value);
    } else if (str_eql(key, dal_c_opt_loose_errors)) {
        opts->loose_errors = dal_c_Project__isTrue(value);
    } else if (str_eql(key, dal_c_opt_no_dsl)) {
        opts->no_dsl = dal_c_Project__isTrue(value);
    }
}

static void dal_c_Project__addLibrary(dal_c_Project* proj, dal_c_Lib* lib) {
    assert(proj != NULL);
    assert(lib != NULL);

    dal_c_Lib* new_libs = (dal_c_Lib*)realloc((void*)proj->libraries, ((size_t)proj->lib_count + 1) * sizeof(dal_c_Lib));
    assert(new_libs != NULL && "Out of memory");
    proj->libraries = new_libs;
    proj->libraries[proj->lib_count] = *lib;
    proj->lib_count++;
    free(lib);
}

static void dal_c_Project__applyLibraryLine(dal_c_Lib* lib, const dal_c_Project* proj, const char* key, const char* value) {
    assert(lib != NULL);
    if (!key || !value) { return; }

    if (str_eql(key, "path")) {
        if (!dal_c_Project__isAbsolutePath(value) && proj && proj->root) {
            lib->path = path_join(proj->root, value);
        } else {
            lib->path = strdup(value);
        }
    } else if (str_eql(key, "profile")) {
        dal_c_Profile profile = dal_c_Profile_parse(value);
        if (profile != dal_c_Profile_invalid) {
            lib->opts.profile = profile;
        }
    } else if (str_eql(key, "linking")) {
        lib->is_static = !str_eql(value, dal_c_linking_dynamic);
    } else if (str_eql(key, dal_c_opt_test)) {
        lib->test_enabled = dal_c_Project__isTrue(value);
        lib->test_enabled_set = true;
    } else {
        dal_c_Project__applyPropertyLine(&lib->opts, key, value);
    }
}

static void dal_c_Project__parseProjectDh(const char* path, dal_c_Project* proj) {
    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return; }

    dal_c_Lib* current_lib = NULL;
    proj->opts.profile = dal_c_Profile_invalid;

    for (int i = 0; i < line_count; ++i) {
        char* line = str_trim(lines[i]);
        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';') {
            continue;
        }

        if (line[0] == '[') {
            if (current_lib) {
                dal_c_Project__addLibrary(proj, current_lib);
            }
            current_lib = calloc(1, sizeof(dal_c_Lib));
            current_lib->is_static = true;
            current_lib->opts.profile = dal_c_Profile_invalid;

            char* end = strchr(line, ']');
            if (end) {
                *end = '\0';
            }
            current_lib->name = strdup(line + 1);
            continue;
        }

        char* eq = strchr(line, '=');
        if (!eq) { continue; }
        *eq = '\0';
        const char* key = str_trim(line);
        const char* value = str_trim(eq + 1);
        if (!current_lib && str_eql(key, dal_c_project_prop_pch)) {
            if (str_eql(value, dal_c_pch_value_off)) {
                proj->pch_enabled = false;
                free(proj->pch_header_override);
                proj->pch_header_override = NULL;
            } else if (str_eql(value, dal_c_pch_value_auto)) {
                proj->pch_enabled = true;
                free(proj->pch_header_override);
                proj->pch_header_override = NULL;
            } else {
                proj->pch_enabled = true;
                char* pch_path = dal_c_Project__resolveProjectPath(proj, value);
                dal_c_Project__setString(&proj->pch_header_override, pch_path);
                free(pch_path);
            }
        } else if (!current_lib && str_eql(key, dal_c_project_prop_pch_exclude)) {
            dal_c_Project__addToArray(&proj->pch_exclude_headers, &proj->pch_exclude_count, value);
        } else if (current_lib) {
            dal_c_Project__applyLibraryLine(current_lib, proj, key, value);
        } else {
            dal_c_Project__applyBuildDefaultsLine(&proj->defaults, key, value);
            dal_c_Project__applyPropertyLine(&proj->opts, key, value);
        }
    }

    if (current_lib) {
        dal_c_Project__addLibrary(proj, current_lib);
    }
    dal_c_Project__freeLines(lines, line_count);
}

static char* dal_c_Project__detectPCH(const dal_c_Project* proj) {
    if (!proj->root || !proj->pch_enabled) { return NULL; }

    if (proj->pch_header_override) {
        return path_isFile(proj->pch_header_override) ? strdup(proj->pch_header_override) : NULL;
    }

    char* inc_dir = dal_c_Project_getIncludeDir(proj);
    if (!path_isDir(inc_dir)) {
        free(inc_dir);
        return NULL;
    }

    const char* common_names[] = {
        dal_c_pch_header_dasae_headers,
        dal_c_pch_header_dh,
        dal_c_pch_header_dal,
        dal_c_pch_header_da,
        NULL
    };

    for (int i = 0; common_names[i] != NULL; ++i) {
        char* header_path = path_join(inc_dir, common_names[i]);
        if (path_isFile(header_path)) {
            free(inc_dir);
            return header_path;
        }
        free(header_path);
    }

    if (proj->name) {
        char* proj_header = str_format("%s.h", proj->name);
        char* header_path = path_join(inc_dir, proj_header);
        free(proj_header);
        if (path_isFile(header_path)) {
            free(inc_dir);
            return header_path;
        }
        free(header_path);
    }

    int file_count = 0;
    char** files = dir_listRecur(inc_dir, &file_count);
    free(inc_dir);
    if (!files) { return NULL; }

    char* header_path = NULL;
    for (int i = 0; i < file_count; ++i) {
        if (!header_path && str_endsWith(files[i], ".h")) {
            header_path = strdup(files[i]);
        }
        free(files[i]);
    }
    free((void*)files);
    return header_path;
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
