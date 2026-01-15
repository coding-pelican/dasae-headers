#include "dal-c.h"
#include "dal-c-ext/str.h"
#include "dal-c-ext/env.h"
#include <stdio.h>

static void dal_c__printUsage(void);
static void dal_c__printVersion(void);
static bool dal_c__needsProject(const dal_c_Cmd* cmd);

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        dal_c__printUsage();
        return 1;
    }

    dal_c_Cmd* cmd = dal_c_Cmd_parse(argc, argv);
    if (!cmd) {
        (void)fprintf(stderr, "Error: Failed to parse command\n");
        return 1;
    }
    if (cmd->is_version) {
        dal_c__printVersion();
    }
    if (cmd->is_help) {
        dal_c__printUsage();
    }
    if (cmd->is_help || cmd->is_version) {
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
            // For lib command with target file, allow no project structure
            bool allow_no_project = (cmd->action == dal_c_CmdAction_lib && cmd->payload.lib.target_file != NULL);
            if (!allow_no_project) {
                (void)fprintf(stderr, "Error: Not in a dh-c project directory\n");
                (void)fprintf(stderr, "  (Looking for directories: include/ and src/, or include/ for header-only)\n");
                return dal_c_Project_cleanup(&proj), dal_c_Cmd_cleanup(&cmd), 1;
            }
        }
    }
    int result = dal_c_Cmd_execute(cmd, proj);
    if (proj) { dal_c_Project_cleanup(&proj); }
    dal_c_Cmd_cleanup(&cmd);
    return result;
}

void dal_c__printUsage(void) {
    printf("Usage: %s <command> [profile] [file.c] [options]\n\n", dal_c_tool_name);
    printf("COMMANDS:\n\n");

    for (int i = 0; i < dal_c_help_cmds_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name) { continue; }
        printf("  %s %s\n", cmd->name, cmd->usage);
        printf("    %s\n\n", cmd->description);

        if (cmd->option_count > 0) {
            printf("    Options:\n");
            for (int j = 0; j < cmd->option_count; ++j) {
                printf("      %-*s %s\n", dal_c_help_opt_width, cmd->options[j].name, cmd->options[j].description);
            }
            // For commands that share build options, indicate that
            if (str_eql(cmd->name, dal_c_cmd_action_lib)
                || str_eql(cmd->name, dal_c_cmd_action_run)
                || str_eql(cmd->name, dal_c_cmd_action_test)
                || str_eql(cmd->name, dal_c_cmd_action_deps)) {
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

    printf("GLOBAL OPTIONS:\n");
    for (int i = 0; i < dal_c_help_global_options_count; ++i) {
        printf("  %-*s %s\n", dal_c_help_opt_width, dal_c_help_global_options[i].name, dal_c_help_global_options[i].description);
    }
    printf("\n");

    printf("PROFILES:\n");
    for (int i = 0; i < dal_c_help_profiles_count; ++i) {
        printf("  %-*s %s\n", 14, dal_c_help_profiles[i].name, dal_c_help_profiles[i].description);
    }
}

void dal_c__printVersion(void) {
    printf("%s version %s\n", dal_c_tool_name, dal_c_ver_str);
    printf("%s\n", dal_c_tool_description);
    printf("%s\n", dal_c_tool_copyright);
    char* const exe_path = env_getExecutablePath();
    if (exe_path) {
        printf("Binary path: %s\n", exe_path);
        free(exe_path);
    }
}

bool dal_c__needsProject(const dal_c_Cmd* cmd) {
    switch (cmd->action) {
    case dal_c_CmdAction_build:
    case dal_c_CmdAction_lib:
    case dal_c_CmdAction_run:
    case dal_c_CmdAction_test:
    case dal_c_CmdAction_deps:
    case dal_c_CmdAction_clean:
        return true;
    case dal_c_CmdAction_version:
    case dal_c_CmdAction_help:
    case dal_c_CmdAction_workspace:
    case dal_c_CmdAction_project:
    case dal_c_CmdAction_invalid:
    default:
        return false;
    }
}
