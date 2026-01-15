#include "internal.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/env.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// === PRIVATE HELPERS ===

static const char* dal_c_Project__findRoot(const char* start);
static const char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd);
static void dal_c_Project__parseLibDH(const char* path, dal_c_Project* proj);
static const char* dal_c_Project__detectPCH(const dal_c_Project* proj);
static void dal_c_Project__addLibrary(dal_c_Project* proj, dal_c_Lib* lib);
static void dal_c_Project__addToArray(char*** arr, int* count, const char* value);

// === PUBLIC API ===

dal_c_Project* dal_c_Project_detect(const dal_c_Cmd* cmd) {
    dal_c_Project* proj = calloc(1, sizeof(dal_c_Project));
    if (!proj) { return NULL; }

    char* cwd = env_getCWD();
    proj->root = dal_c_Project__findRoot(cwd);
    free(cwd);

    if (proj->root) {
        proj->name = path_basename(proj->root);
    }

    proj->dh_path = dal_c_Project__findDHInstallation(cmd);
    proj->pch_header = dal_c_Project__detectPCH(proj);

    if (proj->root) {
        char* lib_dh = path_join(proj->root, dal_c_file_lib_dh);
        if (path_isFile(lib_dh)) {
            dal_c_Project__parseLibDH(lib_dh, proj);
        }
        free(lib_dh);
    }

    return proj;
}

dal_c_Project* dal_c_Project_detectAt(const char* lib_path, const char* dh_path) {
    if (!lib_path || !path_isDir(lib_path)) { return NULL; }

    dal_c_Project* proj = calloc(1, sizeof(dal_c_Project));
    if (!proj) { return NULL; }

    // Convert to absolute path to avoid issues with relative paths in Makefiles
    proj->root = path_abs(lib_path);
    proj->name = path_basename(proj->root);
    proj->dh_path = dh_path;
    proj->pch_header = dal_c_Project__detectPCH(proj);

    char* lib_dh = path_join(proj->root, dal_c_file_lib_dh);
    if (path_isFile(lib_dh)) {
        dal_c_Project__parseLibDH(lib_dh, proj);
    }
    free(lib_dh);

    return proj;
}

void dal_c_Project_cleanup(dal_c_Project** self) {
    if (!self || !*self) { return; }
    dal_c_Project* proj = *self;

    for (int i = 0; i < proj->lib_count; ++i) {
        dal_c_Lib* lib = &proj->libraries[i];
        for (int j = 0; j < lib->opts.include_count; ++j) {
            free(lib->opts.include_paths[j]);
        }
        free((void*)lib->opts.include_paths);
        for (int j = 0; j < lib->opts.link_count; ++j) {
            free(lib->opts.link_libs[j]);
        }
        free((void*)lib->opts.link_libs);
        for (int j = 0; j < lib->opts.define_count; ++j) {
            free(lib->opts.define_macros[j]);
        }
        free((void*)lib->opts.define_macros);
        for (int j = 0; j < lib->opts.undef_count; ++j) {
            free(lib->opts.undef_macros[j]);
        }
        free((void*)lib->opts.undef_macros);
        for (int j = 0; j < lib->opts.isystem_count; ++j) {
            free(lib->opts.isystem_paths[j]);
        }
        free((void*)lib->opts.isystem_paths);
    }
    free(proj->libraries);

    free(proj);
    *self = NULL;
}

char* dal_c_Project_getBuildDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_dir_build);
}

char* dal_c_Project_getIncludeDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_dir_include);
}

char* dal_c_Project_getSrcDir(const dal_c_Project* proj) {
    assert(proj != NULL);
    assert(proj->root != NULL);
    return path_join(proj->root, dal_c_dir_src);
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

static const char* dal_c_Project__findRoot(const char* start) {
    const char* current = start;

    while (current && strlen(current) > 0) {
        char* inc = path_join(current, dal_c_dir_include);
        bool has_include = path_isDir(inc);
        free(inc);

        if (has_include) {
            return strdup(current);
        }

        const char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            break;
        }
        current = parent;
    }

    return NULL;
}

static const char* dal_c_Project__findDHInstallation(const dal_c_Cmd* cmd) {
    if (cmd->dh_path_override) {
        if (path_isDir(cmd->dh_path_override)) {
            return cmd->dh_path_override;
        }
        return NULL;
    }

    const char* dh_home = env_get("DH_HOME");
    if (dh_home && path_isDir(dh_home)) {
        char* inc = path_join(dh_home, dal_c_dir_include);
        if (path_isDir(inc)) {
            free(inc);
            return dh_home;
        }
        free(inc);
    }

    char* exe_dir = env_getExecutableDir();
    if (exe_dir) {
        char* inc = path_join(exe_dir, dal_c_dir_include);
        if (path_isDir(inc)) {
            free(inc);
            return exe_dir;
        }
        free(inc);
        free(exe_dir);
    }

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

static void dal_c_Project__parseLibDH(const char* path, dal_c_Project* proj) {
    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return; }

    dal_c_Lib* current_lib = NULL;

    for (int i = 0; i < line_count; ++i) {
        char* line = str_trim(lines[i]);

        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // INI section header: [LibName]
        if (line[0] == '[') {
            if (current_lib) {
                dal_c_Project__addLibrary(proj, current_lib);
            }
            current_lib = calloc(1, sizeof(dal_c_Lib));
            current_lib->is_static = true;
            current_lib->opts.profile = dal_c_default_profile;

            // Extract name between [ and ]
            char* end = strchr(line, ']');
            if (end) {
                size_t len = (size_t)(end - line - 1);
                char* name = malloc(len + 1);
                strncpy(name, line + 1, len);
                name[len] = '\0';
                current_lib->name = name;
            } else {
                current_lib->name = strdup(line + 1);
            }
            continue;
        }

        // INI key=value pairs
        if (current_lib) {
            char* eq = strchr(line, '=');
            if (!eq) { continue; }

            size_t key_len = (size_t)(eq - line);
            const char* value = str_trim(eq + 1);

            if (strncmp(line, "path", key_len) == 0 && key_len == 4) {
                current_lib->path = strdup(value);
            } else if (strncmp(line, "profile", key_len) == 0 && key_len == 7) {
                current_lib->opts.profile = dal_c_Profile_parse(value);
            } else if (strncmp(line, dal_c_opt_link, key_len) == 0 && key_len == strlen(dal_c_opt_link)) {
                current_lib->is_static = !str_eql(value, dal_c_linking_dynamic);
            } else if (strncmp(line, dal_c_opt_no_libdh, key_len) == 0 && key_len == strlen(dal_c_opt_no_libdh)) {
                current_lib->opts.no_libdh = dal_c_boolean_parse(value);
            } else if (strncmp(line, dal_c_opt_compiler, key_len) == 0 && key_len == strlen(dal_c_opt_compiler)) {
                current_lib->opts.compiler = strdup(value);
            } else if (strncmp(line, dal_c_opt_std, key_len) == 0 && key_len == strlen(dal_c_opt_std)) {
                current_lib->opts.c_std = strdup(value);
            } else if ((strncmp(line, dal_c_opt_arch, key_len) == 0 && key_len == strlen(dal_c_opt_arch)) || (strncmp(line, dal_c_opt_target, key_len) == 0 && key_len == strlen(dal_c_opt_target))) {
                current_lib->opts.arch_target = strdup(value);
            } else if (strncmp(line, dal_c_opt_freestanding, key_len) == 0 && key_len == strlen(dal_c_opt_freestanding)) {
                current_lib->opts.freestanding = dal_c_boolean_parse(value);
            } else if (strncmp(line, dal_c_opt_sysroot, key_len) == 0 && key_len == strlen(dal_c_opt_sysroot)) {
                current_lib->opts.sysroot = strdup(value);
            } else if (strncmp(line, dal_c_opt_include, key_len) == 0 && key_len == strlen(dal_c_opt_include)) {
                dal_c_Project__addToArray(&current_lib->opts.include_paths, &current_lib->opts.include_count, value);
            } else if (strncmp(line, dal_c_opt_define, key_len) == 0 && key_len == strlen(dal_c_opt_define)) {
                dal_c_Project__addToArray(&current_lib->opts.define_macros, &current_lib->opts.define_count, value);
            } else if (strncmp(line, dal_c_opt_undef, key_len) == 0 && key_len == strlen(dal_c_opt_undef)) {
                dal_c_Project__addToArray(&current_lib->opts.undef_macros, &current_lib->opts.undef_count, value);
            } else if (strncmp(line, dal_c_opt_isystem, key_len) == 0 && key_len == strlen(dal_c_opt_isystem)) {
                dal_c_Project__addToArray(&current_lib->opts.isystem_paths, &current_lib->opts.isystem_count, value);
            } else if (strncmp(line, dal_c_opt_loose_errors, key_len) == 0 && key_len == strlen(dal_c_opt_loose_errors)) {
                current_lib->opts.loose_errors = dal_c_boolean_parse(value);
            }
        }
    }

    if (current_lib) {
        dal_c_Project__addLibrary(proj, current_lib);
    }
}

static const char* dal_c_Project__detectPCH(const dal_c_Project* proj) {
    if (!proj->root) { return NULL; }

    char* inc_dir = path_join(proj->root, dal_c_dir_include);
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
    if (files) {
        for (int i = 0; i < file_count; ++i) {
            if (str_endsWith(files[i], ".h")) {
                return files[i];
            }
        }
    }

    return NULL;
}
