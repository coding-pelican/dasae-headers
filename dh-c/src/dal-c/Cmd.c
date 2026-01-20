#include "internal.h"
#include "dal-c-ext/ArrStr.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/dir.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* FIXME: single file, test, sample, example command does not work as expected
 *
 * dh-c build single_file.c
 *
 * dh-c test
 * dh-c test target_source.c
 * dh-c test tests/test-something.c
 * dh-c build --test
 * dh-c build --test test-something.c
 *
 * dh-c build samples/sample-something.c --lib=my-lib
 * dh-c build --sample
 * dh-c build --sample sample-something.c
 * dh-c build examples/example-something.c --lib=my-lib
 * dh-c build --example
 * dh-c build --example example-something.c
 */

// === PRIVATE HELPERS ===

static void dal_c_Cmd__addToArray(char*** arr, int* count, const char* value);
static bool dal_c_Cmd__isSourceOrHeader(const char* name);
static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action);
static int dal_c_Cmd__parseOptions(dal_c_Cmd* cmd, int argc, const char* argv[], int start_idx);
static const char* dal_c_Cmd__getTargetFile(const dal_c_Cmd* cmd);
static bool dal_c_Cmd__getBuildAll(const dal_c_Cmd* cmd);
static dal_c_SampleDir dal_c_Cmd__getSampleDir(const dal_c_Cmd* cmd);
static bool dal_c_Cmd__getDebugMode(const dal_c_Cmd* cmd);

// === PUBLIC API ===

dal_c_Cmd* dal_c_Cmd_parse(int argc, const char* argv[]) {
    if (argc < 2) { return NULL; }
    dal_c_Cmd* cmd = calloc(1, sizeof(dal_c_Cmd));
    if (!cmd) { return NULL; }
    cmd->opts.profile = dal_c_default_profile;

    // Pre-scan for global options (can appear anywhere)
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

    // If global options found, return early (no need to parse command)
    if (cmd->is_help || cmd->is_version) {
        return cmd;
    }

    const char* action_str = argv[1];
    cmd->action = dal_c_CmdAction_parse(action_str);
    if (cmd->action == dal_c_CmdAction_invalid) {
        free(cmd);
        return NULL;
    }

    int arg_idx = 2;
    if (argc > 2 && !str_startsWith(argv[2], dal_c_opt_prefix_short) && !dal_c_Cmd__isSourceOrHeader(argv[2])) {
        dal_c_Profile profile = dal_c_Profile_parse(argv[2]);
        if (profile != dal_c_Profile_invalid) {
            cmd->opts.profile = profile;
            arg_idx = 3;
        }
    }

    if (dal_c_Cmd__parseOptions(cmd, argc, argv, arg_idx) != 0) {
        dal_c_Cmd_cleanup(&cmd);
        return NULL;
    }

    return cmd;
}

void dal_c_Cmd_cleanup(dal_c_Cmd** self) {
    if (!self || !*self) { return; }
    dal_c_Cmd* cmd = *self;

    if (cmd->opts.include_paths) {
        for (int i = 0; i < cmd->opts.include_count; ++i) {
            free(cmd->opts.include_paths[i]);
        }
        free((void*)cmd->opts.include_paths);
    }
    if (cmd->opts.link_libs) {
        for (int i = 0; i < cmd->opts.link_count; ++i) {
            free(cmd->opts.link_libs[i]);
        }
        free((void*)cmd->opts.link_libs);
    }
    if (cmd->opts.define_macros) {
        for (int i = 0; i < cmd->opts.define_count; ++i) {
            free(cmd->opts.define_macros[i]);
        }
        free((void*)cmd->opts.define_macros);
    }
    if (cmd->opts.undef_macros) {
        for (int i = 0; i < cmd->opts.undef_count; ++i) {
            free(cmd->opts.undef_macros[i]);
        }
        free((void*)cmd->opts.undef_macros);
    }
    if (cmd->opts.isystem_paths) {
        for (int i = 0; i < cmd->opts.isystem_count; ++i) {
            free(cmd->opts.isystem_paths[i]);
        }
        free((void*)cmd->opts.isystem_paths);
    }

    switch (cmd->action) {
    case dal_c_CmdAction_build:
        free(cmd->payload.build.target_file);
        break;
    case dal_c_CmdAction_lib:
        free(cmd->payload.lib.target_file);
        break;
    case dal_c_CmdAction_run:
        free(cmd->payload.run.target_file);
        free(cmd->payload.run.run_args);
        break;
    case dal_c_CmdAction_test:
        free(cmd->payload.test.target_file);
        free(cmd->payload.test.run_args);
        break;
    default:
        break;
    }

    free(cmd);
    *self = NULL;
}

int dal_c_Cmd_execute(const dal_c_Cmd* self, const dal_c_Project* proj) {
    switch (self->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
        return dal_c_Cmd_makeTarget(self, proj);
    case dal_c_CmdAction_deps:
        return dal_c_Cmd_compileDeps(self, proj);
    case dal_c_CmdAction_clean:
        return dal_c_Cmd_cleanTarget(self, proj);
    case dal_c_CmdAction_workspace:
        return dal_c_Cmd_createWorkspace(self, proj);
    case dal_c_CmdAction_project:
        return dal_c_Cmd_createProject(self, proj);
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_invalid:
    default:
        (void)fprintf(stderr, "Error: Unknown command\n");
        return 1;
    }
}

int dal_c_Cmd_makeTarget(const dal_c_Cmd* self, const dal_c_Project* proj) {
    const char* target_file = dal_c_Cmd__getTargetFile(self);
    bool build_all = dal_c_Cmd__getBuildAll(self);
    dal_c_SampleDir sample_dir = dal_c_Cmd__getSampleDir(self);

    // Handle test command with build_all
    if (self->action == dal_c_CmdAction_test && build_all) {
        ArrStr* test_sources = dal_c__collectDirectoryFiles(proj, dal_c_dir_tests);
        if (ArrStr_len(test_sources) == 0) {
            (void)fprintf(stderr, "Error: No source files found in %s/\n", dal_c_dir_tests);
            ArrStr_fini(&test_sources);
            return 1;
        }

        const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(self->opts.profile);
        char* build_dir = dal_c_Project_getBuildDir(proj);
        dir_createRecur(build_dir);

        char* output_name = strdup("test");
        if (dal_c__generateMakefile(self, proj, profile, test_sources, output_name, build_dir, dal_c_Target_executable) != 0) {
            (void)fprintf(stderr, "Error: Failed to generate Makefile\n");
            ArrStr_fini(&test_sources);
            free(output_name);
            free(build_dir);
            return 1;
        }

        int result = dal_c__executeMakeInDir(build_dir);
        ArrStr_fini(&test_sources);
        free(output_name);
        free(build_dir);
        if (result != 0) {
            (void)fprintf(stderr, "Error: Build failed\n");
            return result;
        }
        printf("Build successful!\n");
        return dal_c__runExecutable(self, proj);
    }

    // Build libraries first (before sample/example mode too)
    if (proj && proj->lib_count > 0) {
        if (self->verbose) {
            printf("Building %d libraries...\n", proj->lib_count);
        }
        for (int i = 0; i < proj->lib_count; ++i) {
            if (dal_c__buildSingleLibrary(self, proj, &proj->libraries[i]) != 0) {
                return 1;
            }
        }
    }

    // Handle sample/example/test mode
    if (sample_dir != dal_c_SampleDir_none) {
        const char* dir_name = dal_c_SampleDir_format(sample_dir);
        ArrStr* sample_sources = ArrStr_init();

        // If target_file specified, use only that file; otherwise collect all from directory
        if (target_file) {
            ArrStr_push(sample_sources, target_file);
        } else {
            ArrStr_fini(&sample_sources);
            sample_sources = dal_c__collectDirectoryFiles(proj, dir_name);
            if (ArrStr_len(sample_sources) == 0) {
                (void)fprintf(stderr, "Error: No source files found in %s/\n", dir_name);
                ArrStr_fini(&sample_sources);
                return 1;
            }
        }

        ArrStr* project_sources = NULL;
        if (proj && proj->root) {
            char* src_dir = path_join(proj->root, dal_c_dir_src);
            if (path_isDir(src_dir)) {
                project_sources = dal_c__collectSourceFiles(proj, NULL);
            }
            free(src_dir);
        }

        if (project_sources) {
            for (int i = 0; i < ArrStr_len(project_sources); ++i) {
                ArrStr_push(sample_sources, ArrStr_at(project_sources, i));
            }
            ArrStr_fini(&project_sources);
        }

        const char* first_file = ArrStr_at(sample_sources, 0);
        const char* basename = path_basename(first_file);
        char* dot = strrchr(basename, '.');
        char* output_name = NULL;
        if (dot) {
            output_name = strdup(basename);
            output_name[dot - basename] = '\0';
        } else {
            output_name = strdup(basename);
        }

        const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(self->opts.profile);
        char* base_build_dir = dal_c_Project_getBuildDir(proj);
        char* build_dir = path_join(base_build_dir, profile->name);
        free(base_build_dir);
        dir_createRecur(build_dir);
        if (dal_c__generateMakefile(self, proj, profile, sample_sources, output_name, build_dir, dal_c_Target_executable) != 0) {
            (void)fprintf(stderr, "Error: Failed to generate Makefile\n");
            ArrStr_fini(&sample_sources);
            free(output_name);
            free(build_dir);
            return 1;
        }

        int result = dal_c__executeMakeInDir(build_dir);
        ArrStr_fini(&sample_sources);
        free(output_name);
        free(build_dir);
        if (result != 0) {
            (void)fprintf(stderr, "Error: Build failed\n");
            return result;
        }
        printf("Build successful!\n");

        if (self->action == dal_c_CmdAction_run || self->action == dal_c_CmdAction_test) {
            if (dal_c_Cmd__getDebugMode(self)) {
                return dal_c__runDebugger(self, proj);
            }
            return dal_c__runExecutable(self, proj);
        }
        return 0;
    }

    // Build main project
    ArrStr* sources = dal_c__collectSourceFiles(proj, target_file);

    if (self->action == dal_c_CmdAction_lib && dal_c__isHeaderOnlyBuild(self, proj, sources)) {
        const char* output_name = NULL;
        if (target_file) {
            const char* basename = path_basename(target_file);
            char* dot = strrchr(basename, '.');
            if (dot) {
                char* name = strdup(basename);
                name[dot - basename] = '\0';
                output_name = name;
            } else {
                output_name = basename;
            }
        } else if (proj && proj->name) {
            output_name = proj->name;
        } else {
            (void)fprintf(stderr, "Error: Cannot determine output name for header-only library\n");
            ArrStr_fini(&sources);
            return 1;
        }

        ArrStr_fini(&sources);
        return dal_c__buildHeaderOnlyLibrary(self, proj, output_name);
    }

    if (ArrStr_len(sources) == 0) {
        (void)fprintf(stderr, "Error: No source files found\n");
        ArrStr_fini(&sources);
        return 1;
    }

    const char* output_name = NULL;
    char* output_name_allocated = NULL;
    if (target_file) {
        const char* basename = path_basename(target_file);
        char* dot = strrchr(basename, '.');
        if (dot) {
            output_name_allocated = strdup(basename);
            output_name_allocated[dot - basename] = '\0';
            output_name = output_name_allocated;
        } else {
            output_name = basename;
        }
    } else if (proj && proj->name) {
        output_name = proj->name;
    } else {
        (void)fprintf(stderr, "Error: Cannot determine output name\n");
        ArrStr_fini(&sources);
        return 1;
    }
    assert(output_name != NULL);

    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(self->opts.profile);

    dal_c_Target target_type = dal_c_Target_executable;
    if (self->action == dal_c_CmdAction_lib) {
        dal_c_Linking linking = self->payload.lib.linking;
        target_type = (linking == dal_c_Linking_dynamic)
                        ? dal_c_Target_shared_lib
                        : dal_c_Target_static_lib;
    }

    char* base_build_dir = dal_c_Project_getBuildDir(proj);
    char* build_dir = NULL;
    if (base_build_dir) {
        build_dir = path_join(base_build_dir, profile->name);
        free(base_build_dir);
    } else {
        build_dir = path_join(dal_c_dir_build, profile->name);
    }
    dir_createRecur(build_dir);
    if (dal_c__generateMakefile(self, proj, profile, sources, output_name, build_dir, target_type) != 0) {
        (void)fprintf(stderr, "Error: Failed to generate Makefile\n");
        ArrStr_fini(&sources);
        free(build_dir);
        if (output_name_allocated) { free(output_name_allocated); }
        return 1;
    }

    int result = dal_c__executeMakeInDir(build_dir);
    ArrStr_fini(&sources);
    free(build_dir);
    if (output_name_allocated) { free(output_name_allocated); }
    if (result != 0) {
        (void)fprintf(stderr, "Error: Build failed\n");
        return result;
    }
    printf("Build successful!\n");

    if (self->action == dal_c_CmdAction_run || self->action == dal_c_CmdAction_test) {
        if (dal_c_Cmd__getDebugMode(self)) {
            return dal_c__runDebugger(self, proj);
        }
        return dal_c__runExecutable(self, proj);
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

int dal_c_Cmd_cleanTarget(const dal_c_Cmd* self, const dal_c_Project* proj) {
    bool cleaned = false;
    bool cache_only = self->payload.clean.cache_only;
    bool recursive = self->payload.clean.recursive;

    char* build_dir = dal_c_Project_getBuildDir(proj);
    if (build_dir && path_isDir(build_dir)) {
        if (self->verbose) {
            printf("Removing: %s\n", build_dir);
        }
        dir_removeRecur(build_dir);
        printf("Cleaned: %s\n", build_dir);
        cleaned = true;
    }
    free(build_dir);

    if (!cache_only) {
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

    char* cache_dir = path_join(proj->root, dal_c_dir_cache);
    if (path_isDir(cache_dir)) {
        if (self->verbose) {
            printf("Removing: %s\n", cache_dir);
        }
        dir_removeRecur(cache_dir);
        printf("Cleaned: %s\n", cache_dir);
        cleaned = true;
    }
    free(cache_dir);

    // Recursively clean library dependencies (and their dependencies)
    if (recursive && proj->lib_count > 0) {
        for (int i = 0; i < proj->lib_count; ++i) {
            const dal_c_Lib* lib = &proj->libraries[i];
            if (!lib->path) { continue; }

            // Recursively clean this library's own dependencies first
            dal_c_Project* lib_proj = dal_c_Project_detectAt(lib->path, proj->dh_path);
            if (lib_proj) {
                dal_c_Cmd_cleanTarget(self, lib_proj);
                dal_c_Project_cleanup(&lib_proj);
            }
        }
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

// === PRIVATE IMPLEMENTATIONS (Core Layer - use asserts) ===

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

static bool dal_c_Cmd__isSourceOrHeader(const char* name) {
    assert(name != NULL);
    return str_endsWith(name, ".c") || str_endsWith(name, ".h");
}

static const char* dal_c_Cmd__getTargetFile(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return cmd->payload.build.target_file;
    case dal_c_CmdAction_lib:   return cmd->payload.lib.target_file;
    case dal_c_CmdAction_run:   return cmd->payload.run.target_file;
    case dal_c_CmdAction_test:  return cmd->payload.test.target_file;
    default:                    return NULL;
    }
}

static bool dal_c_Cmd__getBuildAll(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return cmd->payload.build.build_all;
    case dal_c_CmdAction_test:  return cmd->payload.test.build_all;
    default:                    return false;
    }
}

static dal_c_SampleDir dal_c_Cmd__getSampleDir(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_build: return cmd->payload.build.sample_dir;
    case dal_c_CmdAction_run:   return cmd->payload.run.sample_dir;
    default:                    return dal_c_SampleDir_none;
    }
}

static bool dal_c_Cmd__getDebugMode(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);
    switch (cmd->action) {
    case dal_c_CmdAction_run:  return cmd->payload.run.debug;
    case dal_c_CmdAction_test: return cmd->payload.test.debug;
    default:                   return false;
    }
}

static bool dal_c_Cmd__isValidOption(const char* arg, dal_c_CmdAction action) {
    // Boolean flags (--xxx form)
    if (str_startsWith(arg, dal_c_opt_prefix_long)) {
        const char* opt = arg + 2;
        if (str_eql(opt, dal_c_opt_show_commands)
            || str_eql(opt, dal_c_opt_verbose)
            || str_eql(opt, dal_c_opt_no_libdh)
            || str_eql(opt, dal_c_opt_static)
            || str_eql(opt, dal_c_opt_dynamic)
            || str_eql(opt, dal_c_opt_cache)
            || str_eql(opt, dal_c_opt_recur)
            || str_eql(opt, dal_c_opt_debug)
            || str_eql(opt, dal_c_opt_freestanding)
            || str_eql(opt, dal_c_opt_loose_errors)
            || str_eql(opt, dal_c_opt_sample)
            || str_eql(opt, dal_c_opt_example)
            || str_eql(opt, dal_c_opt_test)
            || str_eql(opt, dal_c_opt_all)) {
            return true;
        }
        // Value options (--xxx=yyy form)
        if (str_startsWith(opt, dal_c_opt_compiler)
            || str_startsWith(opt, dal_c_opt_std)
            || str_startsWith(opt, dal_c_opt_args)
            || str_startsWith(opt, dal_c_opt_compiler_args)
            || str_startsWith(opt, dal_c_opt_runtime_args)
            || str_startsWith(opt, dal_c_opt_lib)
            || str_startsWith(opt, dal_c_opt_dh)
            || str_startsWith(opt, dal_c_opt_arch)
            || str_startsWith(opt, dal_c_opt_target)
            || str_startsWith(opt, dal_c_opt_sysroot)
            || str_startsWith(opt, dal_c_opt_include)
            || str_startsWith(opt, dal_c_opt_link)
            || str_startsWith(opt, dal_c_opt_define)
            || str_startsWith(opt, dal_c_opt_undef)
            || str_startsWith(opt, dal_c_opt_isystem)) {
            return true;
        }
        return false;
    }

    // Short options
    if (arg[0] == dal_c_opt_prefix_short[0] && arg[1] != dal_c_opt_prefix_short[0]) {
        char c = arg[1];
        if (c == dal_c_opt_include_short_char || c == dal_c_opt_link_short_char
            || c == dal_c_opt_define_short_char || c == dal_c_opt_undef_short_char) {
            return true;
        }
    }

    // File arguments
    if (dal_c_Cmd__isSourceOrHeader(arg)) {
        return action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib
            || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test;
    }

    // "." and "all" for build commands
    if (str_eql(arg, dal_c_opt_all_alias) || str_eql(arg, dal_c_opt_all)) {
        return action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib
            || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test;
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

        // Handle --xxx options
        if (str_startsWith(arg, dal_c_opt_prefix_long)) {
            const char* opt = arg + 2;
            const char* eq = strchr(opt, dal_c_opt_value_sep[0]);

            if (eq) {
                // Value options
                size_t opt_len = (size_t)(eq - opt);
                const char* value = eq + 1;

                if (strncmp(opt, dal_c_opt_compiler, opt_len) == 0) {
                    cmd->opts.compiler = value;
                } else if (strncmp(opt, dal_c_opt_std, opt_len) == 0) {
                    cmd->opts.c_std = value;
                } else if (strncmp(opt, dal_c_opt_arch, opt_len) == 0 || strncmp(opt, dal_c_opt_target, opt_len) == 0) {
                    cmd->opts.arch_target = value;
                } else if (strncmp(opt, dal_c_opt_sysroot, opt_len) == 0) {
                    if (!path_exists(value)) {
                        (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                        return 1;
                    }
                    cmd->opts.sysroot = value;
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
                    cmd->dh_path_override = value;
                } else if (strncmp(opt, dal_c_opt_compiler_args, opt_len) == 0) {
                    cmd->compiler_args = str_trim(value);
                } else if (strncmp(opt, dal_c_opt_args, opt_len) == 0) {
                    if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.run_args = str_trim(value);
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.run_args = str_trim(value);
                    } else {
                        cmd->compiler_args = str_trim(value);
                    }
                } else if (strncmp(opt, dal_c_opt_runtime_args, opt_len) == 0) {
                    if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.run_args = str_trim(value);
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.run_args = str_trim(value);
                    }
                } else if (strncmp(opt, dal_c_opt_lib, opt_len) == 0) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.explicit_lib = value;
                    }
                }
            } else {
                // Boolean flags
                if (str_eql(opt, dal_c_opt_show_commands)) {
                    cmd->show_commands = true;
                } else if (str_eql(opt, dal_c_opt_verbose)) {
                    cmd->verbose = true;
                } else if (str_eql(opt, dal_c_opt_no_libdh)) {
                    cmd->opts.no_libdh = true;
                } else if (str_eql(opt, dal_c_opt_freestanding)) {
                    cmd->opts.freestanding = true;
                } else if (str_eql(opt, dal_c_opt_loose_errors)) {
                    cmd->opts.loose_errors = true;
                } else if (str_eql(opt, dal_c_opt_static)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_static;
                    }
                } else if (str_eql(opt, dal_c_opt_dynamic)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_dynamic;
                    }
                } else if (str_eql(opt, dal_c_opt_debug)) {
                    if (cmd->action == dal_c_CmdAction_run) {
                        cmd->payload.run.debug = true;
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.debug = true;
                    }
                } else if (str_eql(opt, dal_c_opt_cache)) {
                    if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.cache_only = true;
                    }
                } else if (str_eql(opt, dal_c_opt_recur)) {
                    if (cmd->action == dal_c_CmdAction_clean) {
                        cmd->payload.clean.recursive = true;
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
                    } else if (cmd->action == dal_c_CmdAction_test) {
                        cmd->payload.test.build_all = true;
                    }
                }
            }
        }
        // Handle short options
        else if (arg[0] == dal_c_opt_prefix_short[0] && arg[1] != dal_c_opt_prefix_short[0]) {
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
            }
        }
        // Handle positional arguments
        else if (str_eql(arg, dal_c_opt_all_alias) || str_eql(arg, dal_c_opt_all)) {
            if (cmd->action == dal_c_CmdAction_build) {
                cmd->payload.build.build_all = true;
            } else if (cmd->action == dal_c_CmdAction_test) {
                cmd->payload.test.build_all = true;
            }
        } else if (dal_c_Cmd__isSourceOrHeader(arg)) {
            char* abs_path = path_abs(arg);
            if (!abs_path) {
                (void)fprintf(stderr, "Error: File not found: %s\n", arg);
                return 1;
            }
            if (!path_isFile(abs_path)) {
                (void)fprintf(stderr, "Error: Not a file: %s\n", arg);
                free(abs_path);
                return 1;
            }

            switch (cmd->action) {
            case dal_c_CmdAction_build:
                cmd->payload.build.target_file = abs_path;
                break;
            case dal_c_CmdAction_lib:
                cmd->payload.lib.target_file = abs_path;
                break;
            case dal_c_CmdAction_run:
                cmd->payload.run.target_file = abs_path;
                break;
            case dal_c_CmdAction_test:
                cmd->payload.test.target_file = abs_path;
                break;
            default:
                free(abs_path);
                break;
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
