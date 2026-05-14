#include "dal-c.h"
#include "dal-c/internal.h"
#include "dal-c-ext/ArrStr.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_test_failures = 0;

#define TEST_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            (void)fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            g_test_failures++; \
            return; \
        } \
    } while (0)

#define TEST_ASSERT_MSG(cond, fmt, ...) \
    do { \
        if (!(cond)) { \
            (void)fprintf(stderr, "FAIL: %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
            g_test_failures++; \
            return; \
        } \
    } while (0)

#define RUN_TEST(fn) \
    do { \
        int before_failures = g_test_failures; \
        fn(); \
        if (g_test_failures == before_failures) { \
            printf("PASS %s\n", #fn); \
        } \
    } while (0)

static void test_str_helpers(void);
static void test_arrstr_helpers(void);
static void test_path_helpers(void);
static void test_file_and_dir_helpers(void);
static void test_env_helpers(void);
static void test_meta_tables(void);
static void test_cmd_parse(void);
static void test_compiler_mode_contracts(void);
static void test_makefile_mode_contracts(void);
static void test_project_detection(void);
static void test_target_request_resolution(void);
static void test_skip_source_filters(void);
static void test_test_source_classification(void);
static void test_source_collection_ignores_hidden_ancestors(void);

static char* test_repo_path(const char* relative_path);
static char* test_temp_root(void);
static void test_reset_temp_root(void);
static void test_free_str_array(char** items, int count);
static const dal_c_HelpCmd* test_find_help_cmd(const char* name, int* count_out);
static bool test_help_has_option(const dal_c_HelpCmd* cmd, const char* option_name);

int main(void) {
    RUN_TEST(test_str_helpers);
    RUN_TEST(test_arrstr_helpers);
    RUN_TEST(test_path_helpers);
    RUN_TEST(test_file_and_dir_helpers);
    RUN_TEST(test_env_helpers);
    RUN_TEST(test_meta_tables);
    RUN_TEST(test_cmd_parse);
    RUN_TEST(test_compiler_mode_contracts);
    RUN_TEST(test_makefile_mode_contracts);
    RUN_TEST(test_project_detection);
    RUN_TEST(test_target_request_resolution);
    RUN_TEST(test_skip_source_filters);
    RUN_TEST(test_test_source_classification);
    RUN_TEST(test_source_collection_ignores_hidden_ancestors);
    test_reset_temp_root();
    return g_test_failures == 0 ? 0 : 1;
}

static char* test_repo_path(const char* relative_path) {
    char* cwd = env_getCWD();
    if (!cwd) { return NULL; }
    char* path = path_join(cwd, relative_path);
    free(cwd);
    return path;
}

static char* test_temp_root(void) {
    return test_repo_path("dh-c/tests/.scratch/unit");
}

static void test_reset_temp_root(void) {
    char* temp_root = test_temp_root();
    if (temp_root && path_isDir(temp_root)) {
        (void)dir_removeRecur(temp_root);
    }
    free(temp_root);
}

static void test_free_str_array(char** items, int count) {
    if (!items) { return; }
    for (int i = 0; i < count; ++i) {
        free(items[i]);
    }
    free((void*)items);
}

static const dal_c_HelpCmd* test_find_help_cmd(const char* name, int* count_out) {
    const int count = dal_c_help_cmds_count;
    for (int i = 0; i < count; ++i) {
        if (str_eql(dal_c_help_cmds[i].name, name)) {
            if (count_out) {
                *count_out = count;
            }
            return &dal_c_help_cmds[i];
        }
    }
    if (count_out) {
        *count_out = count;
    }
    return NULL;
}

static bool test_help_has_option(const dal_c_HelpCmd* cmd, const char* option_name) {
    if (!cmd || !option_name) {
        return false;
    }

    for (int i = 0; i < cmd->option_count; ++i) {
        if (strstr(cmd->options[i].name, option_name) != NULL) {
            return true;
        }
    }
    return false;
}

static void test_str_helpers(void) {
    TEST_ASSERT(str_eql("abc", "abc"));
    TEST_ASSERT(!str_eql("abc", "abd"));
    TEST_ASSERT(str_startsWith("abcdef", "abc"));
    TEST_ASSERT(str_endsWith("abcdef", "def"));

    const char* parts[] = { "alpha", "beta", "gamma" };
    char* joined = str_join("/", parts, 3);
    TEST_ASSERT(str_eql(joined, "alpha/beta/gamma"));
    free(joined);

    int split_count = 0;
    char** split = str_split("a::b::", "::", &split_count);
    TEST_ASSERT(split_count == 3);
    TEST_ASSERT(str_eql(split[0], "a"));
    TEST_ASSERT(str_eql(split[1], "b"));
    TEST_ASSERT(str_eql(split[2], ""));
    test_free_str_array(split, split_count);

    char* trimmed = str_trim(" \t hello world \r\n");
    TEST_ASSERT(str_eql(trimmed, "hello world"));
    free(trimmed);

    char* upper = str_toUpper("Abc123!");
    TEST_ASSERT(str_eql(upper, "ABC123!"));
    free(upper);

    char* formatted = str_format("%s=%d", "value", 42);
    TEST_ASSERT(str_eql(formatted, "value=42"));
    free(formatted);
}

static void test_arrstr_helpers(void) {
    ArrStr* arr = ArrStr_init();
    TEST_ASSERT(arr != NULL);

    ArrStr_push(arr, "one");
    ArrStr_push(arr, "two");
    ArrStr_push(arr, "three");

    TEST_ASSERT(ArrStr_len(arr) == 3);
    TEST_ASSERT(str_eql(ArrStr_at(arr, 0), "one"));
    TEST_ASSERT(str_eql(ArrStr_atMut(arr, 2), "three"));

    char** raw = ArrStr_toRaw(arr);
    TEST_ASSERT(raw != NULL);
    TEST_ASSERT(str_eql(raw[0], "one"));
    TEST_ASSERT(str_eql(raw[1], "two"));
    TEST_ASSERT(str_eql(raw[2], "three"));
    TEST_ASSERT(raw[3] == NULL);
    for (int i = 0; raw[i] != NULL; ++i) {
        free(raw[i]);
    }
    free((void*)raw);

    ArrStr_fini(&arr);
    TEST_ASSERT(arr == NULL);
}

static void test_path_helpers(void) {
    char* joined = path_join("alpha/", "/beta");
    TEST_ASSERT(joined != NULL);
    TEST_ASSERT(str_endsWith(joined, "alpha\\beta") || str_endsWith(joined, "alpha/beta"));
    free(joined);

    char* parent = path_parent("alpha/beta/gamma.c");
    TEST_ASSERT(parent != NULL);
    TEST_ASSERT(str_endsWith(parent, "alpha/beta") || str_endsWith(parent, "alpha\\beta"));
    free(parent);

#ifdef _WIN32
    char* drive_parent = path_parent("C:\\alpha");
    TEST_ASSERT(drive_parent != NULL);
    TEST_ASSERT(str_eql(drive_parent, "C:\\"));
    free(drive_parent);
#endif

    char* base = path_basename("alpha/beta/gamma.c");
    TEST_ASSERT(str_eql(base, "gamma.c"));
    free(base);

    char* repo_root = test_repo_path(dal_c_tool_name);
    char* abs_root = path_abs(repo_root);
    TEST_ASSERT(abs_root != NULL);
    TEST_ASSERT(path_isDir(abs_root));
    TEST_ASSERT(path_exists(abs_root));

    char* header_path = path_join(abs_root, "include/dal-c.h");
    TEST_ASSERT(path_isFile(header_path));
    char* relative = path_relative(abs_root, header_path);
    TEST_ASSERT(relative != NULL);
    TEST_ASSERT(str_eql(relative, "include\\dal-c.h") || str_eql(relative, "include/dal-c.h"));

    free(relative);
    free(header_path);
    free(abs_root);
    free(repo_root);
}

static void test_file_and_dir_helpers(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dir_createRecur(temp_root));

    char* nested_dir = path_join(temp_root, "nested/deeper");
    TEST_ASSERT(nested_dir != NULL);
    TEST_ASSERT(dir_createRecur(nested_dir));
    TEST_ASSERT(path_isDir(nested_dir));

    char* file_path = path_join(nested_dir, "sample.txt");
    TEST_ASSERT(file_path != NULL);
    TEST_ASSERT(file_write(file_path, "line-1\r\nline-2\r\n"));

    char* file_copy_path = path_join(temp_root, "copied.txt");
    TEST_ASSERT(file_copy_path != NULL);
    TEST_ASSERT(file_copy(file_path, file_copy_path));

    char* file_content = file_read(file_copy_path);
    TEST_ASSERT(file_content != NULL);
    TEST_ASSERT(str_eql(file_content, "line-1\r\nline-2\r\n"));
    free(file_content);

    int line_count = 0;
    char** lines = file_readLines(file_path, &line_count);
    TEST_ASSERT(line_count == 2);
    TEST_ASSERT(str_eql(lines[0], "line-1"));
    TEST_ASSERT(str_eql(lines[1], "line-2"));
    test_free_str_array(lines, line_count);

    char* long_line_file = path_join(temp_root, "long-line.txt");
    TEST_ASSERT(long_line_file != NULL);
    {
        char* long_line = (char*)malloc(6004u);
        TEST_ASSERT(long_line != NULL);
        memset(long_line, 'x', 6000u);
        long_line[6000] = '\n';
        long_line[6001] = 'y';
        long_line[6002] = '\n';
        long_line[6003] = '\0';
        TEST_ASSERT(file_write(long_line_file, long_line));
        free(long_line);
    }

    lines = file_readLines(long_line_file, &line_count);
    TEST_ASSERT(line_count == 2);
    TEST_ASSERT((int)strlen(lines[0]) == 6000);
    TEST_ASSERT(str_eql(lines[1], "y"));
    test_free_str_array(lines, line_count);

    TEST_ASSERT(file_mtime(file_path) != 0);
    TEST_ASSERT(file_atime(file_path) != 0);

    int file_count = 0;
    char** listed = dir_listRecur(temp_root, &file_count);
    TEST_ASSERT(file_count >= 3);
    test_free_str_array(listed, file_count);

    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(long_line_file);
    free(file_copy_path);
    free(file_path);
    free(nested_dir);
    free(temp_root);
}

static void test_env_helpers(void) {
    char* cwd = env_getCWD();
    TEST_ASSERT(cwd != NULL);
    TEST_ASSERT(path_isDir(cwd));

    char* path_before = path_join(cwd, dal_c_tool_name);
    TEST_ASSERT(path_before != NULL);
    TEST_ASSERT(env_setCWD(path_before));

    char* changed = env_getCWD();
    TEST_ASSERT(changed != NULL);
    TEST_ASSERT(str_endsWith(changed, dal_c_tool_name));
    TEST_ASSERT(env_setCWD(cwd));

    char* executable_path = env_getExecutablePath();
    TEST_ASSERT(executable_path != NULL);
    TEST_ASSERT(path_isFile(executable_path));

    char* executable_dir = env_getExecutableDir();
    TEST_ASSERT(executable_dir != NULL);
    TEST_ASSERT(path_isDir(executable_dir));

    free(executable_dir);
    free(executable_path);
    free(changed);
    free(path_before);
    free(cwd);
}

static void test_meta_tables(void) {
    TEST_ASSERT(dal_c_boolean_parse("true"));
    TEST_ASSERT(!dal_c_boolean_parse("false"));
    TEST_ASSERT(str_eql(dal_c_boolean_format(true), "true"));
    TEST_ASSERT(dal_c_Target_parse("executable") == dal_c_Target_executable);
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_shared_lib), "shared-lib"));
    TEST_ASSERT(dal_c_Linking_fromFlag("-shared") == dal_c_Linking_shared);
    TEST_ASSERT(str_eql(dal_c_OptiLevel_toFlag(dal_c_OptiLevel_balanced), "-O2"));
    TEST_ASSERT(dal_c_DebugLevel_fromFlag("-g3") == dal_c_DebugLevel_extended);
    TEST_ASSERT(dal_c_Profile_parse("release") == dal_c_Profile_release);
    TEST_ASSERT(dal_c_CompileEnv_parse("hosted") == dal_c_CompileEnv_hosted);
    TEST_ASSERT(dal_c_CompileEnv_parse("freestanding") == dal_c_CompileEnv_freestanding);
    TEST_ASSERT(dal_c_CompileEnv_resolve(dal_c_CompileEnv_auto) == dal_c_CompileEnv_hosted);
    TEST_ASSERT(dal_c_LibcMode_parse("has-libc") == dal_c_LibcMode_has_libc);
    TEST_ASSERT(dal_c_LibcMode_parse("no-libc") == dal_c_LibcMode_no_libc);
    TEST_ASSERT(dal_c_LibcMode_resolve(dal_c_LibcMode_auto, dal_c_CompileEnv_freestanding) == dal_c_LibcMode_no_libc);
    TEST_ASSERT(dal_c_LibcMode_resolve(dal_c_LibcMode_auto, dal_c_CompileEnv_hosted) == dal_c_LibcMode_has_libc);
    TEST_ASSERT(dal_c_SampleDir_parse("examples") == dal_c_SampleDir_examples);
    TEST_ASSERT(dal_c_TargetSelection_parse("dir") == dal_c_TargetSelection_dir);

    const dal_c_ProfileSpec* release_spec = dal_c_ProfileSpec_by(dal_c_Profile_release);
    TEST_ASSERT(release_spec != NULL);
    TEST_ASSERT(str_eql(release_spec->name, dal_c_profile_release));
    TEST_ASSERT(release_spec->opti_level == dal_c_OptiLevel_aggressive);

    int cmd_count = 0;
    const dal_c_HelpCmd* build_cmd = test_find_help_cmd(dal_c_cmd_action_build, &cmd_count);
    TEST_ASSERT(build_cmd != NULL);
    TEST_ASSERT(build_cmd->implemented);
    TEST_ASSERT(!build_cmd->extends_build_options);
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_args));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_use_dsl));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_dsl));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_hosted));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_has_libc));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_libc));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_has_default_libs));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_default_libs));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_has_start_files));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_start_files));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_std_lib));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_std_lib));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_crt));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_no_crt));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_entry));

    const dal_c_HelpCmd* workspace_cmd = test_find_help_cmd(dal_c_cmd_action_workspace, NULL);
    TEST_ASSERT(workspace_cmd != NULL);
    TEST_ASSERT(!workspace_cmd->implemented);

    const int option_count = dal_c_help_global_options_count;
    TEST_ASSERT(option_count == 2);
    TEST_ASSERT(cmd_count >= 10);

    const int profile_count = dal_c_help_profiles_count;
    TEST_ASSERT(profile_count >= 8);

    const char* const* skip_segments = dal_c_sourceSkipSegments();
    TEST_ASSERT(skip_segments != NULL);
    TEST_ASSERT(str_eql(skip_segments[0], dal_c_source_skip_segment_build));
}

static void test_cmd_parse(void) {
    {
        const char* argv[] = { dal_c_tool_name, "build", "release", "--output=app", "--sample", "--define=DEBUG", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(6, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_release);
        TEST_ASSERT(cmd->payload.build.sample_dir == dal_c_SampleDir_samples);
        TEST_ASSERT(str_eql(cmd->payload.build.output_path, "app"));
        TEST_ASSERT(cmd->opts.define_count == 1);
        TEST_ASSERT(str_eql(cmd->opts.define_macros[0], "DEBUG"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        char* file_a = str_format("--file=%s", dal_c_tool_name "/tests/test-dal-c.c");
        char* file_b = str_format("--file=%s", dal_c_tool_name "/src/dal-c-ext/str.c");
        const char* argv[] = { dal_c_tool_name, "build", file_a, file_b, "-obundle", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->input_count == 2);
        TEST_ASSERT(str_eql(cmd->payload.build.output_path, "bundle"));
        dal_c_Cmd_cleanup(&cmd);
        free(file_b);
        free(file_a);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "--self", "--cache", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--comp-args=-Winvalid-offsetof", "--link-args=-pthread", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(str_eql(cmd->compiler_args, "-Winvalid-offsetof"));
        TEST_ASSERT(str_eql(cmd->link_args, "-pthread"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "--use-dsl",
            "--freestanding",
            "--has-libc",
            "--has-default-libs",
            "--no-default-libs",
            "--has-start-files",
            "--no-start-files",
            "--std-lib",
            "--no-std-lib",
            "--crt",
            "--no-crt",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(14, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.dsl_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.compile_env == dal_c_CompileEnv_freestanding);
        TEST_ASSERT(cmd->opts.libc_mode == dal_c_LibcMode_has_libc);
        TEST_ASSERT(cmd->opts.default_libs_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.start_files_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(str_eql(cmd->opts.entry_symbol, "custom_entry"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "--no-dsl",
            "--hosted",
            "--no-libc",
            "--has-default-libs",
            "--has-start-files",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.dsl_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.compile_env == dal_c_CompileEnv_hosted);
        TEST_ASSERT(cmd->opts.libc_mode == dal_c_LibcMode_no_libc);
        TEST_ASSERT(cmd->opts.default_libs_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.start_files_mode == dal_c_ToggleState_enabled);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--entry", "entry_main", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(str_eql(cmd->opts.entry_symbol, "entry_main"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "dev", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_clean);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_dev);
        TEST_ASSERT(cmd->profile_explicit);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean-self", "optimize", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_clean_self);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_optimize);
        TEST_ASSERT(cmd->profile_explicit);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "workspace", "demo", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_workspace);
        TEST_ASSERT(str_eql(cmd->payload.workspace.name, "demo"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build-self", "optimize", "--show-commands", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build_self);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_optimize);
        TEST_ASSERT(cmd->profile_explicit);
        TEST_ASSERT(cmd->show_commands);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "optimize", "--self", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build);
        TEST_ASSERT(cmd->payload.build.self_boundary);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_optimize);
        TEST_ASSERT(cmd->profile_explicit);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "--help", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(2, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->is_help);
        dal_c_Cmd_cleanup(&cmd);
    }
}

static void test_compiler_mode_contracts(void) {
    dal_c_CompilerOpts merged = { 0 };
    dal_c_CompilerOpts override = { 0 };
    merged.compile_env = dal_c_CompileEnv_freestanding;
    merged.libc_mode = dal_c_LibcMode_no_libc;
    merged.dsl_mode = dal_c_ToggleState_disabled;
    merged.default_libs_mode = dal_c_ToggleState_disabled;
    merged.start_files_mode = dal_c_ToggleState_disabled;
    override.compile_env = dal_c_CompileEnv_hosted;
    override.libc_mode = dal_c_LibcMode_has_libc;
    override.dsl_mode = dal_c_ToggleState_enabled;
    override.default_libs_mode = dal_c_ToggleState_enabled;
    override.start_files_mode = dal_c_ToggleState_enabled;
    dal_c_CompilerOpts_merge(&merged, &override);
    TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
    TEST_ASSERT(merged.libc_mode == dal_c_LibcMode_has_libc);
    TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.default_libs_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.start_files_mode == dal_c_ToggleState_enabled);

    {
        dal_c_CompilerOpts auto_override = { 0 };
        dal_c_CompilerOpts_merge(&merged, &auto_override);
        TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
        TEST_ASSERT(merged.libc_mode == dal_c_LibcMode_has_libc);
        TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.default_libs_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.start_files_mode == dal_c_ToggleState_enabled);
    }

    test_reset_temp_root();
    char* temp_root = test_temp_root();
    char* opts_dh = path_join(temp_root, "modes.dh");
    dal_c_CompilerOpts file_opts = { 0 };
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(opts_dh != NULL);
    TEST_ASSERT(dir_createRecur(temp_root));
    TEST_ASSERT(file_write(
        opts_dh,
        "freestanding=true\n"
        "has-libc=true\n"
        "hosted=false\n"
        "no-libc=false\n"
        "use-dsl=false\n"
        "has-default-libs=false\n"
        "has-start-files=false\n"
        "std-lib=true\n"
        "no-crt=true\n"
    ));
    TEST_ASSERT(dal_c_CompilerOpts_applyDhFile(&file_opts, opts_dh));
    TEST_ASSERT(file_opts.compile_env == dal_c_CompileEnv_freestanding);
    TEST_ASSERT(file_opts.libc_mode == dal_c_LibcMode_has_libc);
    TEST_ASSERT(file_opts.dsl_mode == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.default_libs_mode == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.start_files_mode == dal_c_ToggleState_disabled);
    dal_c_CompilerOpts_cleanup(&file_opts);
    free(opts_dh);
    free(temp_root);
}

static void test_makefile_mode_contracts(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "mode-contract-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* source_dir = path_join(project_root, "source");
    char* main_source = path_join(source_dir, "main.c");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(main_source != NULL);
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(project_dh, "output=mode-contract\n"));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    ArrStr* sources = dal_c__collectSourceFiles(proj, NULL);
    TEST_ASSERT(sources != NULL);
    TEST_ASSERT(ArrStr_len(sources) == 1);

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--freestanding",
            "--has-libc",
            "--no-default-libs",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        TEST_ASSERT(cmd != NULL);

        profile = dal_c_ProfileSpec_by(cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        build_dir = dal_c_Project_getBuildDir(proj);
        profile_dir = path_join(build_dir, profile->name);
        object_dir = path_join(profile_dir, "obj");
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));

        target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, proj->defaults.output_name, dal_c_Target_executable);
        TEST_ASSERT(target_path != NULL);
        makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_executable);
        TEST_ASSERT(makefile_path != NULL);
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        makefile_text = file_read(makefile_path);
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_FREESTANDING") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_DEFAULT_LIBS") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_DEFAULT_LIBS") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_START_FILES") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_START_FILES") == NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") == NULL);
#ifdef _WIN32
        TEST_ASSERT(strstr(makefile_text, " -Wl,/entry:custom_entry") != NULL);
#else
        TEST_ASSERT(strstr(makefile_text, " -Wl,-e,custom_entry") != NULL);
#endif

        free(makefile_text);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--hosted", "--no-libc", "--std-lib", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(6, argv);
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        TEST_ASSERT(cmd != NULL);

        profile = dal_c_ProfileSpec_by(cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        build_dir = dal_c_Project_getBuildDir(proj);
        profile_dir = path_join(build_dir, profile->name);
        object_dir = path_join(profile_dir, "obj");
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));

        target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, proj->defaults.output_name, dal_c_Target_executable);
        TEST_ASSERT(target_path != NULL);
        makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_executable);
        TEST_ASSERT(makefile_path != NULL);
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        makefile_text = file_read(makefile_path);
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HOSTED") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_DEFAULT_LIBS") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_DEFAULT_LIBS") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_START_FILES") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_START_FILES") == NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostartfiles") == NULL);

        free(makefile_text);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
    }

    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(main_source);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_project_detection(void) {
    char* pkg_no_dsl = test_repo_path(dal_c_tool_name "/lab/pkg-no_dsl_contract");
    dal_c_Project* proj = dal_c_Project_detectAt(pkg_no_dsl, NULL);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(str_eql(proj->defaults.output_name, "project-no-dsl"));
    TEST_ASSERT(proj->opts.dsl_mode == dal_c_ToggleState_disabled);
    dal_c_Project_cleanup(&proj);
    free(pkg_no_dsl);

    char* target_root_contract = test_repo_path(dal_c_tool_name "/lab/target-root-contract");
    proj = dal_c_Project_detectAt(target_root_contract, NULL);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->has_explicit_self_roots);
    TEST_ASSERT(proj->self_root_count == 3);
    TEST_ASSERT(proj->target_root_count >= 2);
    TEST_ASSERT(dal_c_Project_findTargetRootByName(proj, "cmd") != NULL);
    TEST_ASSERT(dal_c_Project_findTargetRootByName(proj, "plugins") != NULL);
    dal_c_Project_cleanup(&proj);
    free(target_root_contract);

    char* target_root_compat = test_repo_path(dal_c_tool_name "/lab/target-root-compat");
    proj = dal_c_Project_detectAt(target_root_compat, NULL);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(!proj->pch_enabled);
    dal_c_Project_cleanup(&proj);
    free(target_root_compat);
}

static void test_target_request_resolution(void) {
    char* root_path = test_repo_path(dal_c_tool_name "/lab/target-root-contract");
    dal_c_Project* proj = dal_c_Project_detectAt(root_path, NULL);
    TEST_ASSERT(proj != NULL);

    dal_c_CommandIntent intent = { 0 };
    intent.action = dal_c_CmdAction_build;
    intent.target_root_name_hint = "cmd";
    intent.target_path = "runner1";

    dal_c_TargetRequest request = { 0 };
    TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
    TEST_ASSERT(request.root != NULL);
    TEST_ASSERT(str_eql(request.root->name, "cmd"));
    TEST_ASSERT(request.kind == dal_c_Target_executable);
    TEST_ASSERT(request.selection == dal_c_TargetSelection_dir);
    TEST_ASSERT(request.link_self);
    TEST_ASSERT(request.resolved_is_dir);
    TEST_ASSERT(request.relative_path != NULL);
    TEST_ASSERT(str_eql(request.relative_path, "runner1"));
    dal_c_TargetRequest_cleanup(&request);

    intent.target_root_name_hint = "plugins";
    intent.target_path = "render";
    TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
    TEST_ASSERT(request.kind == dal_c_Target_shared_lib);
    TEST_ASSERT(request.selection == dal_c_TargetSelection_dir);
    TEST_ASSERT(request.link_self);
    dal_c_TargetRequest_cleanup(&request);

    dal_c_Project_cleanup(&proj);
    free(root_path);
}

static void test_skip_source_filters(void) {
    TEST_ASSERT(dal_c__shouldSkipAutoSourcePath("build/output.c"));
    TEST_ASSERT(dal_c__shouldSkipAutoSourcePath("build/.cache/temp.c"));
    TEST_ASSERT(dal_c__shouldSkipAutoSourcePath("src/.cache/temp.c"));
    TEST_ASSERT(!dal_c__shouldSkipAutoSourcePath("src/core/main.c"));
}

static void test_test_source_classification(void) {
    char* registered_with_sample_main = test_repo_path("dh/tests/test-TEST.c");
    char* registered_with_sample_main_2 = test_repo_path("dh/tests/test-TEST-sample.c");
    char* unregistered_source = test_repo_path("dh-c/src/dal-c-ext/str.c");

    TEST_ASSERT(registered_with_sample_main != NULL);
    TEST_ASSERT(registered_with_sample_main_2 != NULL);
    TEST_ASSERT(unregistered_source != NULL);

    TEST_ASSERT(dal_c__hasTestRegistration(registered_with_sample_main));
    TEST_ASSERT(dal_c__hasTestRegistration(registered_with_sample_main_2));
    TEST_ASSERT(!dal_c__hasTestRegistration(unregistered_source));

    free(unregistered_source);
    free(registered_with_sample_main_2);
    free(registered_with_sample_main);
}

static void test_source_collection_ignores_hidden_ancestors(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dir_createRecur(temp_root));

    char* project_root = path_join(temp_root, "hidden-ancestor-project");
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(dir_createRecur(project_root));

    char* project_dh = path_join(project_root, dal_c_file_detector_project);
    char* source_dir = path_join(project_root, "source");
    char* main_source = path_join(source_dir, "main.c");
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(main_source != NULL);
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(project_dh, "output=hidden-ancestor-project\n"));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    ArrStr* sources = dal_c__collectSourceFiles(proj, NULL);
    char* source_relative = NULL;
    TEST_ASSERT(sources != NULL);
    TEST_ASSERT(ArrStr_len(sources) == 1);
    TEST_ASSERT(path_isFile(ArrStr_at(sources, 0)));
    source_relative = path_relative(project_root, ArrStr_at(sources, 0));
    TEST_ASSERT(source_relative != NULL);
    TEST_ASSERT(
        str_eql(source_relative, "source/main.c")
        || str_eql(source_relative, "source\\main.c")
    );

    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(source_relative);
    free(main_source);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}
