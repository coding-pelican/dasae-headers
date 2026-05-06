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
static bool dal_c_Cmd__hasTestRegistration(const char* path);
static bool dal_c_Cmd__isStandaloneTestSource(const char* path);
static void dal_c_Cmd__classifyTestSources(ArrStr* aggregate_sources, ArrStr* standalone_sources, ArrStr* tests);
static void dal_c_Cmd__warnExcludedStandaloneTests(const dal_c_Cmd* cmd, ArrStr* standalone_sources);
static char* dal_c_Cmd__mergeCompilerArgs(const char* base, const char* extra);
static bool dal_c_Cmd__writeFileIfChanged(const char* path, const char* content);
static char* dal_c_Cmd__generatedSourceDir(const dal_c_Project* proj, const dal_c_Cmd* cmd);
static char* dal_c_Cmd__writeTestMainSource(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* output_name);
static char* dal_c_Cmd__writeUnitySource(const dal_c_Project* proj, const dal_c_Cmd* cmd, ArrStr* inputs, const char* output_name);
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

static int dal_c_Cmd__validateCanonicalModifiers(const dal_c_Cmd* cmd) {
    assert(cmd != NULL);

    if (cmd->action == dal_c_CmdAction_build && cmd->payload.build.self_boundary) {
        if (cmd->input_count > 0
            || cmd->payload.build.output_path != NULL
            || cmd->payload.build.sample_dir != dal_c_SampleDir_none
            || cmd->payload.build.build_all
            || cmd->payload.build.recursive
            || cmd->payload.build.as_library
            || cmd->payload.build.dsl_first
            || cmd->exclude_count > 0) {
            (void)fprintf(stderr, "Error: `build --self` does not accept target, traversal, library, or exclude modifiers\n");
            return 1;
        }
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
    const bool use_dsl_tests = self->action == dal_c_CmdAction_test && intent.dsl_first;

    if (intent.dsl_first) {
        int dsl_result = dal_c__buildDSL(self, proj);
        if (dsl_result != 0) { return dsl_result; }
    }

    int result = 0;
    dal_c_Project* dsl_proj = NULL;
    const dal_c_Project* target_proj = proj;
    if (use_dsl_tests) {
        dsl_proj = dal_c_Project_detectAt(proj->dh_path, proj->dh_path);
        if (!dsl_proj) {
            (void)fprintf(stderr, "Error: Failed to detect DH project for --dsl tests\n");
            return 1;
        }
        target_proj = dsl_proj;
    }

    dal_c_TargetRequest target_request = { 0 };
    if (!dal_c_TargetRequest_resolve(target_proj, &intent, &target_request)) {
        dal_c_Project_cleanup(&dsl_proj);
        return 1;
    }
    ArrStr* active_excludes = dal_c_Cmd__collectActiveExcludes(target_proj, &target_request, self);
    if (target_request.root && target_request.raw_target_path && target_request.resolved_path) {
        if (dal_c_Cmd__rejectExcludedPath(target_request.resolved_path, active_excludes) != 0) {
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
            return 1;
        }
    } else if (self->input_count > 0 && !dal_c_Cmd__inputsNeedCategoryResolution(self)) {
        for (int i = 0; i < self->input_count; ++i) {
            if (dal_c_Cmd__rejectExcludedPath(self->input_files[i], active_excludes) != 0) {
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
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
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        }
    }

    bool needs_project_static_lib = (target_request.root && target_request.link_self) || is_test_mode;
    if (needs_project_static_lib) {
        int lib_result = dal_c_Cmd__ensureProjectStaticLibrary(self, target_proj);
        if (lib_result != 0) {
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
            return lib_result;
        }
    }

    if (is_test_mode) {
        ArrStr* tests = NULL;
        if (target_request.root) {
            tests = dal_c_Cmd__collectTargetSources(&target_request);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, target_request.raw_target_path != NULL) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        } else if (self->input_count > 0 && !dal_c_Cmd__inputsNeedCategoryResolution(self)) {
            tests = dal_c_Cmd__collectExplicitSources(self);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, true) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        } else {
            tests = dal_c_Cmd__resolveInputs(target_proj, self, dal_c_dir_tests, true);
            if (dal_c_Cmd__applyExcludeContract(&tests, active_excludes, self->input_count > 0) != 0) {
                ArrStr_fini(&tests);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        }
        if (ArrStr_len(tests) == 0) {
            bool allow_recursive_skip = intent.recursive && self->input_count == 0 && !target_request.raw_target_path;
            ArrStr_fini(&tests);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
            if (allow_recursive_skip) {
                if (self->action == dal_c_CmdAction_test) {
                    return dal_c_Cmd__runRecursiveTest(self, proj);
                }
                return 0;
            }
            (void)fprintf(stderr, "Error: No test source files found\n");
            return 1;
        }

        ArrStr* aggregate_tests = ArrStr_init();
        ArrStr* standalone_tests = ArrStr_init();
        dal_c_Cmd__classifyTestSources(aggregate_tests, standalone_tests, tests);

        ArrStr* sources = ArrStr_init();
        char* output_name = NULL;
        const char* extra_compiler_args = NULL;
        const bool use_single_test_runner = self->action == dal_c_CmdAction_test
                                         && ArrStr_len(tests) == 1
                                         && ArrStr_len(standalone_tests) == 1
                                         && dal_c_Cmd__hasTestRegistration(ArrStr_at(standalone_tests, 0));
        if (ArrStr_len(aggregate_tests) > 0 || use_single_test_runner) {
            if (ArrStr_len(standalone_tests) > 0) {
                if (!use_single_test_runner) {
                    dal_c_Cmd__warnExcludedStandaloneTests(self, standalone_tests);
                }
            }

            const char* primary_test = use_single_test_runner
                                         ? ArrStr_at(standalone_tests, 0)
                                         : ArrStr_at(aggregate_tests, 0);
            if (((use_single_test_runner ? 1 : ArrStr_len(aggregate_tests)) == 1) && ArrStr_len(tests) == 1) {
                output_name = dal_c_Cmd__basenameNoExt(primary_test);
            } else {
                output_name = strdup(self->action == dal_c_CmdAction_test ? "test" : "tests");
            }

            char* runner_source = dal_c_Cmd__writeTestMainSource(target_proj, self, output_name);
            if (!runner_source) {
                ArrStr_fini(&sources);
                ArrStr_fini(&aggregate_tests);
                ArrStr_fini(&standalone_tests);
                ArrStr_fini(&tests);
                free(output_name);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
            ArrStr_push(sources, runner_source);
            free(runner_source);
            if (use_single_test_runner) {
                ArrStr_push(sources, primary_test);
            } else {
                for (int i = 0; i < ArrStr_len(aggregate_tests); ++i) {
                    ArrStr_push(sources, ArrStr_at(aggregate_tests, i));
                }
            }
        } else if (ArrStr_len(standalone_tests) == 1) {
            ArrStr_push(sources, ArrStr_at(standalone_tests, 0));
            output_name = dal_c_Cmd__basenameNoExt(ArrStr_at(standalone_tests, 0));
        } else {
            (void)fprintf(
                stderr,
                "Error: Aggregate test build requires registration-style sources; standalone tests must be built one at a time\n"
            );
            ArrStr_fini(&sources);
            ArrStr_fini(&aggregate_tests);
            ArrStr_fini(&standalone_tests);
            ArrStr_fini(&tests);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
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
        ArrStr_fini(&sources);
        ArrStr_fini(&aggregate_tests);
        ArrStr_fini(&standalone_tests);
        ArrStr_fini(&tests);
        free(output_name);
    } else if (target_request.root) {
        ArrStr* target_sources = dal_c_Cmd__collectTargetSources(&target_request);
        if (dal_c_Cmd__applyExcludeContract(&target_sources, active_excludes, target_request.raw_target_path != NULL) != 0) {
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
            return 1;
        }
        if (ArrStr_len(target_sources) == 0) {
            (void)fprintf(stderr, "Error: No source files found in target root `%s`\n", target_request.root->name);
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
            return 1;
        }

        if (self->action == dal_c_CmdAction_run && target_request.kind != dal_c_Target_executable) {
            (void)fprintf(stderr, "Error: `run` requires an executable target root\n");
            ArrStr_fini(&target_sources);
            ArrStr_fini(&active_excludes);
            dal_c_TargetRequest_cleanup(&target_request);
            dal_c_Project_cleanup(&dsl_proj);
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
        if (builds_library) {
            dal_c_Linking linking = intent.linking;
            target_type = (linking == dal_c_Linking_shared) ? dal_c_Target_shared_lib : dal_c_Target_static_lib;
        }
        result = dal_c_Cmd__buildFromSources(self, target_proj, sources, output_name, target_type, NULL, true, true);
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
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        } else if (build_all || !intent.target_path) {
            ArrStr_fini(&sources);
            sources = dal_c__collectSourceFiles(proj, NULL);
            if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, false) != 0) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }
        } else {
            ArrStr_push(sources, intent.target_path);
            if (dal_c_Cmd__applyExcludeContract(&sources, active_excludes, true) != 0) {
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
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
                dal_c_Project_cleanup(&dsl_proj);
                return result;
            }
        } else {
            if (ArrStr_len(sources) == 0) {
                (void)fprintf(stderr, "Error: No source files found\n");
                ArrStr_fini(&sources);
                ArrStr_fini(&active_excludes);
                dal_c_TargetRequest_cleanup(&target_request);
                dal_c_Project_cleanup(&dsl_proj);
                return 1;
            }

            const char* output_name = proj->name;
            char* output_name_alloc = NULL;
            if (self->input_count > 0) {
                output_name_alloc = dal_c_Cmd__basenameNoExt(self->input_files[0]);
                output_name = output_name_alloc;
            }

            bool use_unity = (self->action == dal_c_CmdAction_build || self->action == dal_c_CmdAction_run)
                          && self->input_count > 1;
            if (use_unity) {
                char* unity_source = dal_c_Cmd__writeUnitySource(proj, self, sources, output_name);
                if (!unity_source) {
                    free(output_name_alloc);
                    ArrStr_fini(&sources);
                    ArrStr_fini(&active_excludes);
                    dal_c_TargetRequest_cleanup(&target_request);
                    dal_c_Project_cleanup(&dsl_proj);
                    return 1;
                }
                ArrStr* bundled = ArrStr_init();
                ArrStr_push(bundled, unity_source);
                free(unity_source);
                ArrStr_fini(&sources);
                sources = bundled;
            }

            dal_c_Target target_type = dal_c_Target_executable;
            if (builds_library) {
                dal_c_Linking linking = intent.linking;
                target_type = (linking == dal_c_Linking_shared) ? dal_c_Target_shared_lib : dal_c_Target_static_lib;
            }
            result = dal_c_Cmd__buildFromSources(self, proj, sources, output_name, target_type, NULL, true, true);
            free(output_name_alloc);
            ArrStr_fini(&sources);
        }
    }

    ArrStr_fini(&active_excludes);
    dal_c_TargetRequest_cleanup(&target_request);
    dal_c_Project_cleanup(&dsl_proj);
    if (result != 0) { return result; }
    if (intent.recursive) {
        if (self->action == dal_c_CmdAction_test) {
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

    if (self->payload.clean.dsl_first) {
        int dsl_result = dal_c__cleanDSL(self, proj);
        if (dsl_result != 0) { return dsl_result; }
    }

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
    return str_endsWith(name, ".c") || str_endsWith(name, ".h");
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
            || str_eql(opt, dal_c_opt_no_dsl)
            || (str_eql(opt, dal_c_opt_self) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_clean))
            || (str_eql(opt, dal_c_opt_lib) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib))
            || ((str_eql(opt, dal_c_opt_static) || str_eql(opt, dal_c_opt_shared))
                && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib))
            || str_eql(opt, dal_c_opt_cache)
            || str_eql(opt, dal_c_opt_recur)
            || str_eql(opt, dal_c_opt_debug)
            || str_eql(opt, dal_c_opt_freestanding)
            || str_eql(opt, dal_c_opt_loose_errors)
            || str_eql(opt, dal_c_opt_sample)
            || str_eql(opt, dal_c_opt_example)
            || str_eql(opt, dal_c_opt_test)
            || str_eql(opt, dal_c_opt_all)
            || str_eql(opt, dal_c_opt_exclude)
            || str_eql(opt, dal_c_opt_dsl)) {
            return true;
        }
        if (((str_startsWith(opt, dal_c_opt_output)) && (action == dal_c_CmdAction_build || action == dal_c_CmdAction_lib || action == dal_c_CmdAction_run || action == dal_c_CmdAction_test || action == dal_c_CmdAction_test_dsl))
            || str_startsWith(opt, dal_c_opt_compiler)
            || str_startsWith(opt, dal_c_opt_std)
            || str_startsWith(opt, dal_c_opt_args)
            || str_startsWith(opt, dal_c_opt_compiler_args)
            || str_startsWith(opt, dal_c_opt_runtime_args)
            || str_startsWith(opt, dal_c_opt_dh)
            || str_startsWith(opt, dal_c_opt_exclude)
            || str_startsWith(opt, dal_c_opt_arch)
            || str_startsWith(opt, dal_c_opt_target)
            || str_startsWith(opt, dal_c_opt_sysroot)
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

                if (strncmp(opt, dal_c_opt_compiler, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.compiler, value);
                } else if (strncmp(opt, dal_c_opt_std, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.c_std, value);
                } else if (strncmp(opt, dal_c_opt_arch, opt_len) == 0 || strncmp(opt, dal_c_opt_target, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->opts.arch_target, value);
                } else if (strncmp(opt, dal_c_opt_sysroot, opt_len) == 0) {
                    if (!path_exists(value)) {
                        (void)fprintf(stderr, "Error: Path not found: %s\n", value);
                        return 1;
                    }
                    dal_c_Cmd__setOwnedString(&cmd->opts.sysroot, value);
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
                } else if (strncmp(opt, dal_c_opt_compiler_args, opt_len) == 0) {
                    dal_c_Cmd__setOwnedString(&cmd->compiler_args, value);
                } else if (strncmp(opt, dal_c_opt_args, opt_len) == 0) {
                    char** run_args_slot = dal_c_Cmd__runArgsSlot(cmd);
                    if (run_args_slot) {
                        dal_c_Cmd__setOwnedString(run_args_slot, value);
                    } else {
                        dal_c_Cmd__setOwnedString(&cmd->compiler_args, value);
                    }
                } else if (strncmp(opt, dal_c_opt_runtime_args, opt_len) == 0) {
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
                } else if (str_eql(opt, dal_c_opt_exclude)) {
                    if (i + 1 >= argc) {
                        (void)fprintf(stderr, "Error: Missing value for option: %s\n", arg);
                        return 1;
                    }
                    if (dal_c_Cmd__pushExcludePath(cmd, argv[++i]) != 0) {
                        return 1;
                    }
                } else if (str_eql(opt, dal_c_opt_no_dsl)) {
                    cmd->opts.no_dsl = true;
                } else if (str_eql(opt, dal_c_opt_freestanding)) {
                    cmd->opts.freestanding = true;
                } else if (str_eql(opt, dal_c_opt_loose_errors)) {
                    cmd->opts.loose_errors = true;
                } else if (str_eql(opt, dal_c_opt_static)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_static;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.as_library = true;
                        cmd->payload.build.linking = dal_c_Linking_static;
                    }
                } else if (str_eql(opt, dal_c_opt_shared)) {
                    if (cmd->action == dal_c_CmdAction_lib) {
                        cmd->payload.lib.linking = dal_c_Linking_shared;
                    } else if (cmd->action == dal_c_CmdAction_build) {
                        cmd->payload.build.as_library = true;
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
        if (skip_auto_paths && dal_c__shouldSkipAutoSourcePath(entries[i])) {
            free(entries[i]);
            continue;
        }
        if (str_endsWith(entries[i], ".c")) {
            ArrStr_push(files, entries[i]);
        }
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

static bool dal_c_Cmd__hasTestRegistration(const char* path) {
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

static bool dal_c_Cmd__isStandaloneTestSource(const char* path) {
    assert(path != NULL);

    int line_count = 0;
    char** lines = file_readLines(path, &line_count);
    if (!lines) { return false; }

    bool is_standalone = false;
    for (int i = 0; i < line_count; ++i) {
        const char* line = lines[i];
        while (*line == ' ' || *line == '\t') { ++line; }

        if (str_startsWith(line, "#define main_no_hijack")
            || strstr(line, "fn_((main(") != NULL
            || strstr(line, "fn_(main(") != NULL
            || strstr(line, "fn_((dh_main(") != NULL
            || strstr(line, "fn_(dh_main(") != NULL
            || strstr(line, "int main(") != NULL
            || strstr(line, "int dh_main(") != NULL) {
            is_standalone = true;
            break;
        }
    }

    for (int i = 0; i < line_count; ++i) {
        free(lines[i]);
    }
    free((void*)lines);
    return is_standalone;
}

static void dal_c_Cmd__classifyTestSources(ArrStr* aggregate_sources, ArrStr* standalone_sources, ArrStr* tests) {
    assert(aggregate_sources != NULL);
    assert(standalone_sources != NULL);
    assert(tests != NULL);

    for (int i = 0; i < ArrStr_len(tests); ++i) {
        const char* path = ArrStr_at(tests, i);
        if (dal_c_Cmd__isStandaloneTestSource(path)) {
            ArrStr_push(standalone_sources, path);
        } else {
            ArrStr_push(aggregate_sources, path);
        }
    }
}

static void dal_c_Cmd__warnExcludedStandaloneTests(const dal_c_Cmd* cmd, ArrStr* standalone_sources) {
    assert(cmd != NULL);
    assert(standalone_sources != NULL);
    if (ArrStr_len(standalone_sources) == 0) { return; }

    (void)fprintf(
        stderr,
        "Warning: Excluding %d standalone test source(s) from aggregate build\n",
        ArrStr_len(standalone_sources)
    );
    if (!cmd->verbose) { return; }

    for (int i = 0; i < ArrStr_len(standalone_sources); ++i) {
        (void)fprintf(stderr, "  - %s\n", ArrStr_at(standalone_sources, i));
    }
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

    char* cache_dir = NULL;
    if (proj && proj->root) {
        cache_dir = path_join(proj->root, dal_c_dir_cache);
    } else {
        char* cwd = env_getCWD();
        cache_dir = cwd ? path_join(cwd, dal_c_dir_cache) : strdup(dal_c_dir_cache);
        free(cwd);
    }

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

    char* compiler_args = dal_c_Cmd__mergeCompilerArgs(self->compiler_args, extra_compiler_args);
    dal_c_Cmd effective = *self;
    dal_c_BuildDefaults effective_defaults = { 0 };
    effective.compiler_args = compiler_args;
    memset(&effective.opts, 0, sizeof(effective.opts));
    effective.opts.profile = dal_c_Profile_invalid;
    dal_c_Cmd__mergeBuildProperties(&effective.opts, &effective_defaults, proj, sources, self);
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

    if (self->action == dal_c_CmdAction_run || self->action == dal_c_CmdAction_test) {
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
