#include "dal-c/internal.h"
#include "dal-c-ext/str.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dal_c__printUsage(const char* topic);
static void dal_c__printVersion(const dal_c_Cmd* cmd);
static bool dal_c__needsProject(const dal_c_Cmd* cmd);
static bool dal_c__allowsNoProject(const dal_c_Cmd* cmd);
static const dal_c_HelpCmd* dal_c__findHelpCmd(const char* name);
static void dal_c__printHelpCmd(const dal_c_HelpCmd* cmd);
static dal_c_Cmd* dal_c__parseAsBuild(int argc, const char* argv[], int skip_count);
static int dal_c__showTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj);
static int dal_c__doctor(const dal_c_Cmd* cmd, const dal_c_Project* proj);
static int dal_c__depsSourceCommand(const char* action, const dal_c_Project* proj);

int main(int argc, const char* argv[]) {
    if (argc < 2) return dal_c__printUsage(NULL), 1;

    if (str_eql(argv[1], dal_c_cmd_action_help)) {
        return dal_c__printUsage(argc > 2 ? argv[2] : NULL);
    }

    bool special_deps_source = argc > 2 && str_eql(argv[1], dal_c_cmd_action_deps)
                            && (str_eql(argv[2], "fetch") || str_eql(argv[2], "update") || str_eql(argv[2], "status"));
    if (special_deps_source) {
        const char* deps_argv[] = { argv[0], dal_c_cmd_action_deps };
        dal_c_Cmd* deps_cmd = dal_c_Cmd_parse(2, deps_argv);
        if (!deps_cmd) {
            (void)fprintf(stderr, "Error: Failed to initialize dependency command\n");
            return 1;
        }
        dal_c_Project* deps_proj = dal_c_Project_detect(deps_cmd);
        if (!deps_proj || !deps_proj->root) {
            (void)fprintf(stderr, "Error: Not in a dh-c project directory\n");
            if (deps_proj) dal_c_Project_cleanup(&deps_proj);
            dal_c_Cmd_cleanup(&deps_cmd);
            return 1;
        }
        int deps_result = dal_c__depsSourceCommand(argv[2], deps_proj);
        dal_c_Project_cleanup(&deps_proj);
        dal_c_Cmd_cleanup(&deps_cmd);
        return deps_result;
    }

    bool special_plan = str_eql(argv[1], "plan");
    bool special_explain = argc > 2 && str_eql(argv[1], "explain") && str_eql(argv[2], "rebuild");
    bool special_target = argc > 2 && str_eql(argv[1], "target") && str_eql(argv[2], "show");
    bool special_doctor = str_eql(argv[1], "doctor");
    dal_c_Cmd* cmd = (special_plan || special_doctor)
                   ? dal_c__parseAsBuild(argc, argv, 1)
                   : ((special_explain || special_target) ? dal_c__parseAsBuild(argc, argv, 2) : dal_c_Cmd_parse(argc, argv));
    if (!cmd) {
        (void)fprintf(stderr, "Error: Failed to parse command\n");
        return 1;
    }
    if (cmd->is_version || cmd->is_help) {
        int result = 0;
        if (cmd->is_version) dal_c__printVersion(cmd);
        if (cmd->is_help) {
            const char* topic = NULL;
            if (argc > 1 && argv[1][0] != '-') {
                topic = argv[1];
            }
            result = dal_c__printUsage(topic);
        }
        return dal_c_Cmd_cleanup(&cmd), result;
    }

    dal_c_Project* proj = NULL;
    if (special_target || special_doctor) {
        proj = dal_c_Project_detect(cmd);
        if (!proj) {
            proj = NULL;
        }
    } else if (dal_c__needsProject(cmd)) {
        proj = dal_c_Project_detect(cmd);
        if (!proj) {
            (void)fprintf(stderr, "Error: Failed to detect project\n");
            return dal_c_Cmd_cleanup(&cmd), 1;
        }
        if (!proj->root) {
            if (!dal_c__allowsNoProject(cmd)) {
                (void)fprintf(stderr, "Error: Not in a" dal_c_tool_name "project directory\n");
                (void)fprintf(stderr, "  (Looking for nearest ancestor with %s)\n", dal_c_file_detector_project);
                return dal_c_Project_cleanup(&proj), dal_c_Cmd_cleanup(&cmd), 1;
            }
        }
    }
    int result = 0;
    if (special_target) {
        result = dal_c__showTarget(cmd, proj);
    } else if (special_doctor) {
        result = dal_c__doctor(cmd, proj);
    } else {
        if (special_plan || special_explain) {
            cmd->dry_run = true;
            cmd->explain_rebuild = special_explain;
        }
        result = dal_c_Cmd_execute(cmd, proj);
    }
    if (proj) dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    return result;
}


static bool dal_c__depsGitRun(const char* cwd, const char* a, const char* b, const char* c, const char* d) {
    const char* argv[10] = { "git", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    int i = 1;
    if (cwd) { argv[i++] = "-C"; argv[i++] = cwd; }
    if (a) argv[i++] = a;
    if (b) argv[i++] = b;
    if (c) argv[i++] = c;
    if (d) argv[i++] = d;
    argv[i] = NULL;
    return proc_run(argv, true) == 0;
}

static char* dal_c__depsGitOutput(const char* cwd, const char* a, const char* b) {
    const char* argv[7] = { "git", "-C", cwd, a, b, NULL, NULL };
    return proc_output(argv);
}

static int dal_c__depsSourceCommand(const char* action, const dal_c_Project* proj) {
    assert(action != NULL);
    assert(proj != NULL && proj->root != NULL);

    char* state_root = path_join(proj->root, ".dh-c");
    char* deps_root = path_join(state_root, "deps");
    char* src_root = path_join(deps_root, "src");
    char* build_root = path_join(deps_root, "build");
    char* package_root = path_join(deps_root, "packages");
    char* lock_path = path_join(deps_root, "lock.dh");
    if (!dir_createRecur(src_root) || !dir_createRecur(build_root) || !dir_createRecur(package_root)) {
        (void)fprintf(stderr, "Error: Failed to create dependency cache directories under %s\n", deps_root);
        free(lock_path); free(package_root); free(build_root); free(src_root); free(deps_root); free(state_root);
        return 1;
    }

    char* lock_text = strdup("# Generated by dh-c deps fetch/update. Do not edit by hand.\n");
    size_t lock_len = lock_text ? strlen(lock_text) : 0;
    int failures = 0;
    int external_count = 0;

    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        if (!lib->source || lib->source[0] == '\0') continue;
        external_count++;
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        char* source_dir = path_join(src_root, lib->name);
        bool exists = path_isDir(source_dir);

        if (str_eql(action, "fetch")) {
            if (!exists) {
                const char* argv[] = { "git", "clone", "--", lib->source, source_dir, NULL };
                printf("[FETCH] %s <- %s\n", lib->name, lib->source);
                if (proc_run(argv, true) != 0) { failures++; free(source_dir); continue; }
            } else {
                printf("[HAVE]  %s\n", lib->name);
            }
            if (lib->revision && lib->revision[0]) {
                if (!dal_c__depsGitRun(source_dir, "fetch", "--tags", "--prune", NULL)
                    || !dal_c__depsGitRun(source_dir, "checkout", "--detach", lib->revision, NULL)) {
                    (void)fprintf(stderr, "Error: Failed to lock %s to revision %s\n", lib->name, lib->revision);
                    failures++; free(source_dir); continue;
                }
            }
        } else if (str_eql(action, "update")) {
            if (!exists) {
                const char* argv[] = { "git", "clone", "--", lib->source, source_dir, NULL };
                printf("[FETCH] %s <- %s\n", lib->name, lib->source);
                if (proc_run(argv, true) != 0) { failures++; free(source_dir); continue; }
            }
            printf("[UPDATE] %s\n", lib->name);
            if (!dal_c__depsGitRun(source_dir, "fetch", "--tags", "--prune", NULL)) {
                failures++; free(source_dir); continue;
            }
            if (lib->revision && lib->revision[0]) {
                if (!dal_c__depsGitRun(source_dir, "checkout", "--detach", lib->revision, NULL)) {
                    failures++; free(source_dir); continue;
                }
            } else if (!dal_c__depsGitRun(source_dir, "pull", "--ff-only", NULL, NULL)) {
                failures++; free(source_dir); continue;
            }
        } else {
            if (!exists) {
                printf("[MISSING] %-16s provider=%s source=%s\n", lib->name, provider, lib->source);
                free(source_dir);
                continue;
            }
        }

        if (path_isDir(source_dir)) {
            char* head = dal_c__depsGitOutput(source_dir, "rev-parse", "HEAD");
            char* dirty = dal_c__depsGitOutput(source_dir, "status", "--porcelain");
            if (head) {
                char* trimmed_head = str_trim(head);
                printf("[%s] %-16s provider=%s revision=%s%s\n",
                    str_eql(action, "status") ? "READY" : "LOCK",
                    lib->name, provider, trimmed_head,
                    (dirty && str_trim(dirty)[0]) ? " dirty" : "");
                size_t add = strlen(lib->name) + strlen(trimmed_head) + strlen(provider) + 64;
                char* grown = realloc(lock_text, lock_len + add);
                if (grown) {
                    lock_text = grown;
                    int n = snprintf(lock_text + lock_len, add, "[%s]\nprovider=%s\nsource=%s\nrevision=%s\n\n",
                        lib->name, provider, lib->source, trimmed_head);
                    if (n > 0) lock_len += (size_t)n;
                }
            }
            free(head);
            free(dirty);
        }
        free(source_dir);
    }

    if (external_count == 0) {
        printf("No external dependencies declare source=.\n");
    } else if (!str_eql(action, "status") && failures == 0) {
        if (!file_writeAtomic(lock_path, lock_text ? lock_text : "")) {
            (void)fprintf(stderr, "Error: Failed to write dependency lock: %s\n", lock_path);
            failures++;
        } else {
            printf("[LOCK] %s\n", lock_path);
        }
    }

    free(lock_text);
    free(lock_path); free(package_root); free(build_root); free(src_root); free(deps_root); free(state_root);
    return failures == 0 ? 0 : 1;
}

static int dal_c__printUsage(const char* topic) {
    bool list_only = topic && str_eql(topic, "--list");
    bool print_all = topic && str_eql(topic, "--all");
    if (list_only) {
        for (int i = 0; i < dal_c_help_cmds_count; ++i) {
            const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
            if (cmd->name && cmd->implemented) { printf("%s\n", cmd->name); }
        }
        printf("plan\nexplain\ntarget\ndoctor\n");
        return 0;
    }
    if (print_all) { topic = NULL; }

    const int help_cmd_count = dal_c_help_cmds_count;
    const int global_option_count = dal_c_help_global_options_count;
    const int help_profile_count = dal_c_help_profiles_count;

    if (topic && (str_eql(topic, "plan") || str_eql(topic, "explain") || str_eql(topic, "target") || str_eql(topic, "doctor"))) {
        printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
        if (str_eql(topic, "plan")) {
            printf("USAGE:\n  %s plan [profile] [path] [build options]\n\nGenerate the real build plan and Makefile without compiling or linking.\n", dal_c_tool_name);
        } else if (str_eql(topic, "explain")) {
            printf("USAGE:\n  %s explain rebuild [profile] [path] [build options]\n\nExplain whether the requested output is missing or its tracked plan is stale.\n", dal_c_tool_name);
        } else if (str_eql(topic, "target")) {
            printf("USAGE:\n  %s target show [profile] [build options]\n\nShow the requested and normalized target, compiler, profile, and target-scoped build directory.\n", dal_c_tool_name);
        } else {
            printf("USAGE:\n  %s doctor [profile] [build options]\n\nCheck compiler, make, archiver, DH installation, project detection, and effective target.\n", dal_c_tool_name);
        }
        return 0;
    }
    if (topic) {
        const dal_c_HelpCmd* cmd = dal_c__findHelpCmd(topic);
        if (!cmd) {
            (void)fprintf(stderr, "Error: Unknown help topic: %s\n", topic);
            return 1;
        }
        dal_c__printHelpCmd(cmd);
        return 0;
    }

    printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
    printf("USAGE:\n");
    printf("  %s <command> [profile] [path] [options]\n", dal_c_tool_name);
    printf("  %s help [command]\n", dal_c_tool_name);
    printf("  %s help --list | --all\n", dal_c_tool_name);
    printf("  %s <command> --help\n", dal_c_tool_name);
    printf("  %s -h | --help\n", dal_c_tool_name);
    printf("  %s -v | --version\n\n", dal_c_tool_name);

    printf("COMMAND MODEL:\n");
    printf("  A command decides the phase: build, run, test, check, format, clean, or query.\n");
    printf("  A profile decides the optimization/debug contract; default profile is `dev`.\n");
    printf("  A path decides the target source, target-root member, directory, or explicit file.\n");
    printf("  Options are command-scoped. Unknown or irrelevant options are rejected instead of ignored.\n\n");

    printf("COMMANDS:\n");
    for (int i = 0; i < help_cmd_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name || !cmd->implemented) { continue; }
        printf("  %-14s %s\n", cmd->name, cmd->description);
    }
    printf("  %-14s %s\n", "plan", "Generate the real build plan without executing it");
    printf("  %-14s %s\n", "explain", "Explain why a requested build requires work");
    printf("  %-14s %s\n", "target", "Inspect the effective target and output directory");
    printf("  %-14s %s\n", "doctor", "Check the local build environment and DH installation");
    printf("\n");

    if (!print_all) {
        printf("Use `%s help <command>` for command details, `%s help --list` for names only, or `%s help --all` for the complete reference.\n", dal_c_tool_name, dal_c_tool_name, dal_c_tool_name);
        return 0;
    }

    printf("COMMAND DETAILS:\n\n");

    for (int i = 0; i < help_cmd_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name || !cmd->implemented) { continue; }
        printf("  %s %s\n", cmd->name, cmd->usage);
        printf("    %s\n\n", cmd->description);

        if (cmd->note_count > 0) {
            printf("    Contract:\n");
            for (int j = 0; j < cmd->note_count; ++j) {
                printf("      - %s\n", cmd->notes[j]);
            }
            printf("\n");
        }

        if (cmd->option_count > 0) {
            printf("    Options:\n");
            for (int j = 0; j < cmd->option_count; ++j) {
                printf("      %-*s %s\n", dal_c_help_opt_width, cmd->options[j].name, cmd->options[j].description);
            }
            printf("\n");
        }

        if (cmd->example_count > 0) {
            printf("    Examples:\n");
            for (int j = 0; j < cmd->example_count; ++j) {
                printf("      %s %s\n", dal_c_tool_name, cmd->examples[j]);
            }
            printf("\n");
        }
    }

    printf("RESERVED COMMANDS:\n");
    for (int i = 0; i < help_cmd_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name || cmd->implemented) continue;
        printf("  %s %s\n", cmd->name, cmd->usage);
        printf("    %s\n\n", cmd->description);
    }

    printf("GLOBAL OPTIONS:\n");
    for (int i = 0; i < global_option_count; ++i) {
        printf("  %-*s %s\n", dal_c_help_opt_width, dal_c_help_global_options[i].name, dal_c_help_global_options[i].description);
    }
    printf("  Toggle options accept `=on|off`, `=yes|no`, `=true|false`, or `=1|0` forms such as `--link-dsl=off` and `--lto=off`.\n");
    printf("\n");

    printf("COMMAND OPTION BOUNDARIES:\n");
    printf("  `build`, `run`, and `test` accept compile inputs and link inputs because they produce executables or libraries.\n");
    printf("  `compile-db`, `syntax`, and `tidy` accept compile/source-selection options and do not accept link or artifact-output options.\n");
    printf("  `format` accepts only source-selection, recursion, DH discovery, and output-visibility options.\n");
    printf("  `clean` accepts cleanup scope options only; build, compile, link, and runtime options are invalid.\n\n");

    printf("TARGET RESOLUTION:\n");
    printf("  `" dal_c_tool_name " build` with no explicit path builds the project default output.\n");
    printf("  Explicit `file.c` inputs build that file directly, even outside a project.\n");
    printf("  Explicit paths under `[target-root ...]` use the declared target-root contract.\n");
    printf("  `--sample`, `--example`, and `--test` select the built-in target families.\n");
    printf("  `.` is a compatibility alias for `--all`.\n");
    printf("  `build --self` and `clean --self` operate on the " dal_c_tool_name " self boundary only.\n\n");

    printf("OUTPUT CONTRACT:\n");
    printf("  Default output names come from `project.dh` `output`, target-root `output`, or the source file stem.\n");
    printf("  `--output <path>` means an existing directory or an output stem; dh-c generates `.exe`, `.a`, `.lib`, `.so`, or `.dll` as needed.\n");
    printf("  `--output-ext <.ext>` selects an explicit extension for one artifact, such as `.pyd`; it is invalid for two-output `kind=lib` auto builds.\n");
    printf("  `compile-db --output <path>` is different: it names the JSON file path exactly.\n\n");

    printf("LINK INPUT CONTRACT:\n");
    printf("  `--link-dir <dir>` / `-L<dir>` adds a library search directory and becomes a linker `-L` flag.\n");
    printf("  `--link <name>` / `-l<name>` adds a logical library name and becomes a linker `-l` flag.\n");
    printf("  `--link-args \"...\"` is a raw escape hatch for linker flags that do not have structured options.\n");
    printf("  Link inputs are rejected by check/format commands because those commands do not link.\n\n");

    printf("PCH CONTRACT:\n");
    printf("  `pch=auto` uses the detected DH bundle when available; `pch=deps` uses generated `lib/deps.h`; `pch=off` disables PCH.\n");
    printf("  PCH files are generated inside the active profile/flag cache plan and are rebuilt when their headers or compile contract change.\n");
    printf("  `lib/deps.h` is generated only when a dependency prelude is needed; it includes top-level headers under `lib/deps/`.\n\n");

    printf("PROJECT.DH KEYS:\n");
    printf("  Top level: `output`, `kind=<executable|static-lib|shared-lib|lib>`, `build-runs-tests=<on|off>`, `self-root=<path>`, `exclude=<path>`.\n");
    printf("  Toolchain: `compiler`, `std`, `arch`/`target`, `target-arch`, `target-tune`, `target-abi`, `sysroot`, `entry`.\n");
    printf("  Compile inputs: `include`, `isystem`, `define`, `undef`, `profile`, `hosted`, `freestanding`, `loose-errors`.\n");
    printf("  Link inputs: `link`, `link-dir`, `link-dsl`, `link-libc`, `link-default-libs`, `link-start-files`, `link-compiler-rt`, `link-stdlib`, `link-crt`, `link-mode`.\n");
    printf("  Optimization/artifacts: `lto`, `omit-frame-pointer`, `function-sections`, `data-sections`, `gc-sections`, `whole-archive`, `unroll-loops`, `unwind-tables`, `async-unwind-tables`, `exceptions`, `strip`, `icf`, `merge-all-constants`, `stack-protector`.\n");
    printf("  Version: `version-core`, `version-prefix=<alpha|beta|rc>`, `version-suffix`, `version-build`.\n");
    printf("  PCH: `pch=<auto|off|deps|header>`, `pch-exclude=<header>`.\n");
    printf("  `[target-root <name>]`: `path`, `kind=<executable|static-lib|shared-lib|lib|image|preprocessed|assembly>`, `selection=<path|file|dir>`, `link-project=<on|off>`, `exclude=<path>`.\n");
    printf("  `[<dependency-name>]`: `path`, `profile`, `linking=<static|shared>`, `link-dsl`, `test`, and the compile/link keys above.\n");
    printf("  CLI options override project defaults for the current invocation.\n\n");

    printf("GENERATED DIRECTORIES:\n");
    printf("  `build/` stores artifacts, object files, and generated plan makefiles.\n");
    printf("  `build/.cache/` stores generated unity/test helper sources.\n");
    printf("  `lib/deps/` stores generated dependency headers and libraries when project dependencies require them.\n");
    printf("  `lib/deps.h` stores the generated dependency prelude when one is needed.\n");
    printf("  `clean` owns these generated paths; do not place durable source assets, checked-in resources, or manual files there.\n");
    printf("  PCH outputs live in the active build cache plan, not as a global project artifact.\n\n");

    printf("PROFILES:\n");
    for (int i = 0; i < help_profile_count; ++i) {
        printf("  %-*s %s\n", 14, dal_c_help_profiles[i].name, dal_c_help_profiles[i].description);
    }
    printf("\n");
    printf("SUPPORT STATUS:\n");
    printf("  `workspace` and `project` are reserved scaffold commands and are not implemented.\n");
    return 0;
}

static const dal_c_HelpCmd* dal_c__findHelpCmd(const char* name) {
    if (!name) { return NULL; }
    for (int i = 0; i < dal_c_help_cmds_count; ++i) {
        if (dal_c_help_cmds[i].name && str_eql(dal_c_help_cmds[i].name, name)) {
            return &dal_c_help_cmds[i];
        }
    }
    return NULL;
}

static void dal_c__printHelpCmd(const dal_c_HelpCmd* cmd) {
    printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
    printf("USAGE:\n");
    printf("  %s %s %s\n", dal_c_tool_name, cmd->name, cmd->usage ? cmd->usage : "");
    printf("  %s help %s\n\n", dal_c_tool_name, cmd->name);

    printf("SUMMARY:\n");
    printf("  %s\n\n", cmd->description);

    if (cmd->implemented) {
        printf("CONTRACT:\n");
        if (cmd->note_count > 0) {
            for (int i = 0; i < cmd->note_count; ++i) {
                printf("  %s\n", cmd->notes[i]);
            }
        } else {
            printf("  This command uses only the usage and options shown here.\n");
        }
        if (cmd->extends_build_options) {
            printf("  This command also accepts build options where they affect the command's build phase.\n");
        }
        printf("\n");
    } else {
        printf("STATUS:\n");
        printf("  Reserved command. It is listed so scripts do not assume it is implemented.\n\n");
    }

    if (cmd->option_count > 0) {
        printf("OPTIONS:\n");
        for (int i = 0; i < cmd->option_count; ++i) {
            printf("  %-*s %s\n", dal_c_help_opt_width, cmd->options[i].name, cmd->options[i].description);
        }
        if (cmd->extends_build_options) {
            printf("  [build options]%-*s See `%s help build` for the build option contract\n", dal_c_help_opt_width - 15, "", dal_c_tool_name);
        }
        printf("\n");
    }

    if (cmd->example_count > 0) {
        printf("EXAMPLES:\n");
        for (int i = 0; i < cmd->example_count; ++i) {
            printf("  %s %s\n", dal_c_tool_name, cmd->examples[i]);
        }
        printf("\n");
    }
}

void dal_c__printVersion(const dal_c_Cmd* cmd) {
    printf("%s version %s\n", dal_c_tool_name, dal_c_ver_str_with_build);
    printf("%s\n", dal_c_tool_description);
    printf("%s\n", dal_c_tool_copyright);
    char* dh_path = dal_c_Project_findDHInstallation(cmd);
    printf("dasae-headers path: %s\n", dh_path ? dh_path : "(not found)");
    free(dh_path);
    char* const exe_path = env_getExecutablePath();
    if (exe_path) {
        printf("Binary path: %s\n", exe_path);
        free(exe_path);
    }
}

bool dal_c__needsProject(const dal_c_Cmd* cmd) {
    switch (cmd->action) {
    case dal_c_CmdAction_build:
        return !cmd->payload.build.self_boundary;
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_clean:
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
        if (cmd->action == dal_c_CmdAction_clean) {
            return !cmd->payload.clean.self_boundary;
        }
        return true;
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_test_dsl:
    case dal_c_CmdAction_clean_dsl:
        return true;
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}

static bool dal_c__allowsNoProject(const dal_c_Cmd* cmd) {
    switch (cmd->action) {
    case dal_c_CmdAction_build:
        return cmd->payload.build.target_path != NULL && cmd->payload.build.sample_dir == dal_c_SampleDir_none;
    case dal_c_CmdAction_lib:
        return cmd->payload.lib.target_path != NULL;
    case dal_c_CmdAction_run:
        return cmd->payload.run.target_path != NULL && cmd->payload.run.sample_dir == dal_c_SampleDir_none;
    case dal_c_CmdAction_test:
        return cmd->payload.test.target_path != NULL && cmd->payload.test.sample_dir == dal_c_SampleDir_none;
    case dal_c_CmdAction_build_dsl:
    case dal_c_CmdAction_test_dsl:
    case dal_c_CmdAction_clean_dsl:
        return true;
    case dal_c_CmdAction_build_self:
    case dal_c_CmdAction_clean_self:
    case dal_c_CmdAction_toolchain:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_clean:
        return cmd->action == dal_c_CmdAction_clean;
    case dal_c_CmdAction_compile_db:
    case dal_c_CmdAction_syntax:
    case dal_c_CmdAction_tidy:
    case dal_c_CmdAction_format_code:
        return cmd->payload.build.target_path != NULL && cmd->payload.build.sample_dir == dal_c_SampleDir_none;
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}


static dal_c_Cmd* dal_c__parseAsBuild(int argc, const char* argv[], int skip_count) {
    int out_argc = argc - skip_count + 1;
    const char** out_argv = (const char**)calloc((size_t)out_argc + 1, sizeof(*out_argv));
    if (!out_argv) { return NULL; }
    out_argv[0] = argv[0];
    out_argv[1] = dal_c_cmd_action_build;
    int out_i = 2;
    for (int i = 1 + skip_count; i < argc; ++i) { out_argv[out_i++] = argv[i]; }
    dal_c_Cmd* result = dal_c_Cmd_parse(out_i, out_argv);
    free(out_argv);
    return result;
}

static const char* dal_c__effectiveCompiler(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->opts.compiler && cmd->opts.compiler[0]) { return cmd->opts.compiler; }
    if (proj && proj->opts.compiler && proj->opts.compiler[0]) { return proj->opts.compiler; }
    return dal_c_default_compiler;
}

static const char* dal_c__effectiveRequestedTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->opts.arch_target && cmd->opts.arch_target[0]) { return cmd->opts.arch_target; }
    if (proj && proj->opts.arch_target && proj->opts.arch_target[0]) { return proj->opts.arch_target; }
    return NULL;
}

static dal_c_Profile dal_c__effectiveProfile(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->profile_explicit && cmd->opts.profile != dal_c_Profile_invalid) { return cmd->opts.profile; }
    if (proj && proj->opts.profile != dal_c_Profile_invalid) { return proj->opts.profile; }
    if (cmd && cmd->opts.profile != dal_c_Profile_invalid) { return cmd->opts.profile; }
    return dal_c_default_profile;
}

static int dal_c__showTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    dal_c_CompilerOpts opts = {0};
    opts.compiler = strdup(dal_c__effectiveCompiler(cmd, proj));
    const char* requested = dal_c__effectiveRequestedTarget(cmd, proj);
    opts.arch_target = requested ? strdup(requested) : NULL;
    char* normalized = dal_c__resolveTargetDirName(&opts);
    dal_c_Profile profile_id = dal_c__effectiveProfile(cmd, proj);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(profile_id);
    char* profile_dir = (proj && proj->root && profile) ? dal_c__makeBuildProfileDir(proj, &opts, profile) : NULL;
    printf("TARGET:\n");
    printf("  requested: %s\n", requested ? requested : "(host compiler default)");
    printf("  normalized: %s\n", normalized ? normalized : "(unresolved)");
    printf("  compiler: %s\n", opts.compiler ? opts.compiler : "(none)");
    printf("  profile: %s\n", profile ? profile->name : "(unknown)");
    printf("  build-dir: %s\n", profile_dir ? profile_dir : "(project unavailable)");
    printf("  native-alias: %s\n", requested ? "not used for explicit target" : "build/native");
    free(profile_dir);
    free(normalized);
    dal_c_CompilerOpts_cleanup(&opts);
    return 0;
}

static bool dal_c__toolResponds(const char* tool, const char* arg) {
    if (!tool || !tool[0]) { return false; }
    const char* argv[] = { tool, arg, NULL };
    char* output = proc_output(argv);
    bool ok = output && output[0];
    free(output);
    return ok;
}

static int dal_c__doctor(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    int failures = 0;
    const char* compiler = dal_c__effectiveCompiler(cmd, proj);
    bool compiler_ok = dal_c__toolResponds(compiler, "--version");
    bool make_ok = dal_c__toolResponds("make", "--version") || dal_c__toolResponds("gmake", "--version");
    bool ar_ok = dal_c__toolResponds(dal_c_tool_ar, "--version");
    char* dh_path = dal_c_Project_findDHInstallation(cmd);
    bool dh_ok = dh_path && path_isDir(dh_path);
    printf("DOCTOR:\n");
    printf("  compiler: %s [%s]\n", compiler, compiler_ok ? "ok" : "missing or unusable");
    printf("  make: %s\n", make_ok ? "ok" : "missing or unusable");
    printf("  archiver: %s [%s]\n", dal_c_tool_ar, ar_ok ? "ok" : "missing or unusable");
    printf("  dh: %s [%s]\n", dh_path ? dh_path : "(not found)", dh_ok ? "ok" : "missing");
    printf("  project: %s\n", proj && proj->root ? proj->root : "(not detected; explicit-file builds remain possible)");
    if (!compiler_ok) failures++;
    if (!make_ok) failures++;
    if (!ar_ok) failures++;
    if (!dh_ok) failures++;
    if (compiler_ok) {
        dal_c__showTarget(cmd, proj);
    }
    printf("RESULT: %s\n", failures == 0 ? "healthy" : "issues detected");
    free(dh_path);
    return failures == 0 ? 0 : 1;
}
