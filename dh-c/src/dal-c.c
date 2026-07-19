#include "dal-c.h"
#include "dal-c-ext/str.h"
#include "dal-c-ext/env.h"
#include <stdio.h>

static void dal_c__printUsage(void);
static void dal_c__printVersion(const dal_c_Cmd* cmd);
static bool dal_c__needsProject(const dal_c_Cmd* cmd);
static bool dal_c__allowsNoProject(const dal_c_Cmd* cmd);

int main(int argc, const char* argv[]) {
    if (argc < 2) return dal_c__printUsage(), 1;

    dal_c_Cmd* cmd = dal_c_Cmd_parse(argc, argv);
    if (!cmd) {
        (void)fprintf(stderr, "Error: Failed to parse command\n");
        return 1;
    }
    if (cmd->is_version || cmd->is_help) {
        if (cmd->is_version) dal_c__printVersion(cmd);
        if (cmd->is_help) dal_c__printUsage();
        return dal_c_Cmd_cleanup(&cmd), 0;
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

void dal_c__printUsage(void) {
    const int help_cmd_count = dal_c_help_cmds_count;
    const int global_option_count = dal_c_help_global_options_count;
    const int help_profile_count = dal_c_help_profiles_count;

    printf("Usage: %s <command> [profile] [file.c] [options]\n\n", dal_c_tool_name);
    printf("COMMANDS:\n\n");

    for (int i = 0; i < help_cmd_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name || !cmd->implemented) { continue; }
        printf("  %s %s\n", cmd->name, cmd->usage);
        printf("    %s\n\n", cmd->description);

        if (cmd->option_count > 0) {
            printf("    Options:\n");
            for (int j = 0; j < cmd->option_count; ++j) {
                printf("      %-*s %s\n", dal_c_help_opt_width, cmd->options[j].name, cmd->options[j].description);
            }
            if (cmd->extends_build_options) {
                printf("      [all %s options...]\n", dal_c_cmd_action_build);
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

    printf("TARGET RESOLUTION:\n");
    printf("  `" dal_c_tool_name " build` with no explicit path builds the project default output.\n");
    printf("  Explicit `file.c` inputs build that file directly, even outside a project.\n");
    printf("  Explicit paths under `[target-root ...]` use the declared target-root contract.\n");
    printf("  `--sample`, `--example`, and `--test` select the built-in target families.\n");
    printf("  `.` is a compatibility alias for `--all`.\n");
    printf("  `build --self` and `clean --self` operate on the " dal_c_tool_name " self boundary only.\n\n");

    printf("PROJECT.DH KEYS:\n");
    printf("  Project keys: `kind`, `output`, `build-runs-tests`, `link-dsl`, `pch`, `pch-exclude`, `self-root`, `version-core`, `version-prefix`, `version-suffix`, `version-build`\n");
    printf("  `[target-root <name>]`: `path`, `kind`, `selection`, `link-project`\n");
    printf("  Dependency blocks: `path`, `profile`, `linking`, `link-dsl`, `test`\n");
    printf("  See `BUILD.md` and `dh-c/docs/project-dh-contract.md` for the full explicit contract.\n\n");

    printf("GENERATED DIRECTORIES:\n");
    printf("  `build/` stores artifacts, object files, and generated plan makefiles.\n");
    printf("  `build/.cache/` stores generated unity/test helper sources.\n");
    printf("  `lib/deps/` stores copied dependency headers, static/shared libraries, and PCH files.\n\n");

    printf("PROFILES:\n");
    for (int i = 0; i < help_profile_count; ++i) {
        printf("  %-*s %s\n", 14, dal_c_help_profiles[i].name, dal_c_help_profiles[i].description);
    }
    printf("\n");
    printf("SUPPORT STATUS:\n");
    printf("  `workspace` and `project` are reserved scaffold commands and are not implemented.\n");
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
