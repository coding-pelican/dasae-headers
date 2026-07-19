#include "dal-c.h"
#include "dal-c-ext/str.h"
#include "dal-c-ext/env.h"
#include <stdio.h>

static int dal_c__printUsage(const char* topic);
static void dal_c__printVersion(const dal_c_Cmd* cmd);
static bool dal_c__needsProject(const dal_c_Cmd* cmd);
static bool dal_c__allowsNoProject(const dal_c_Cmd* cmd);
static const dal_c_HelpCmd* dal_c__findHelpCmd(const char* name);
static void dal_c__printHelpCmd(const dal_c_HelpCmd* cmd);

int main(int argc, const char* argv[]) {
    if (argc < 2) return dal_c__printUsage(NULL), 1;

    if (str_eql(argv[1], dal_c_cmd_action_help)) {
        return dal_c__printUsage(argc > 2 ? argv[2] : NULL);
    }

    dal_c_Cmd* cmd = dal_c_Cmd_parse(argc, argv);
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
    if (dal_c__needsProject(cmd)) {
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
    int result = dal_c_Cmd_execute(cmd, proj);
    if (proj) dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    return result;
}

static int dal_c__printUsage(const char* topic) {
    const int help_cmd_count = dal_c_help_cmds_count;
    const int global_option_count = dal_c_help_global_options_count;
    const int help_profile_count = dal_c_help_profiles_count;

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
    printf("\n");

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
    printf("  `lib/` stores dependency headers and libraries when project dependencies require them.\n");
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
