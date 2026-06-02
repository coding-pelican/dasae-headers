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
    TEST_ASSERT(dal_c_boolean_parse("on"));
    TEST_ASSERT(!dal_c_boolean_parse("false"));
    TEST_ASSERT(!dal_c_boolean_parse("off"));
    TEST_ASSERT(str_eql(dal_c_boolean_format(true), "on"));
    TEST_ASSERT(dal_c_Target_parse("executable") == dal_c_Target_executable);
    TEST_ASSERT(dal_c_Target_parse("lib") == dal_c_Target_lib);
    TEST_ASSERT(dal_c_Target_parse("image") == dal_c_Target_image);
    TEST_ASSERT(dal_c_Target_parse("preprocessed") == dal_c_Target_preprocessed);
    TEST_ASSERT(dal_c_Target_parse("assembly") == dal_c_Target_assembly);
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_lib), "lib"));
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_shared_lib), "shared-lib"));
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_image), "image"));
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_preprocessed), "preprocessed"));
    TEST_ASSERT(str_eql(dal_c_Target_format(dal_c_Target_assembly), "assembly"));
    TEST_ASSERT(dal_c_Linking_fromFlag("-shared") == dal_c_Linking_shared);
    TEST_ASSERT(str_eql(dal_c_OptiLevel_toFlag(dal_c_OptiLevel_balanced), "-O2"));
    TEST_ASSERT(dal_c_DebugLevel_fromFlag("-g3") == dal_c_DebugLevel_extended);
    TEST_ASSERT(dal_c_Profile_parse("release") == dal_c_Profile_release);
    TEST_ASSERT(dal_c_CompileEnv_parse("hosted") == dal_c_CompileEnv_hosted);
    TEST_ASSERT(dal_c_CompileEnv_parse("freestanding") == dal_c_CompileEnv_freestanding);
    TEST_ASSERT(dal_c_CompileEnv_resolve(dal_c_CompileEnv_auto) == dal_c_CompileEnv_hosted);
    TEST_ASSERT(dal_c_ToggleState_resolve(dal_c_ToggleState_enabled, false) == true);
    TEST_ASSERT(dal_c_ToggleState_resolve(dal_c_ToggleState_disabled, true) == false);
    TEST_ASSERT(dal_c_ToggleState_resolve(dal_c_ToggleState_auto, true) == true);
    TEST_ASSERT(dal_c_ToggleState_resolve(dal_c_ToggleState_auto, false) == false);
    TEST_ASSERT(dal_c_SampleDir_parse("examples") == dal_c_SampleDir_examples);
    TEST_ASSERT(dal_c_TargetSelection_parse("dir") == dal_c_TargetSelection_dir);

    const dal_c_ProfileSpec* release_spec = dal_c_ProfileSpec_by(dal_c_Profile_release);
    TEST_ASSERT(release_spec != NULL);
    TEST_ASSERT(str_eql(release_spec->name, dal_c_profile_release));
    TEST_ASSERT(release_spec->opti_level == dal_c_OptiLevel_aggressive);

    const dal_c_ProfileSpec* optimize_spec = dal_c_ProfileSpec_by(dal_c_Profile_optimize);
    TEST_ASSERT(optimize_spec != NULL);
    TEST_ASSERT(optimize_spec->lto_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->function_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->data_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->gc_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->whole_archive == dal_c_ToggleState_auto);
    TEST_ASSERT(optimize_spec->omit_frame_pointer == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(optimize_spec->async_unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(optimize_spec->strip_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(optimize_spec->icf_mode == dal_c_IcfMode_all);
    TEST_ASSERT(str_eql(optimize_spec->target_arch, "native"));

    const dal_c_ProfileSpec* micro_spec = dal_c_ProfileSpec_by(dal_c_Profile_micro);
    TEST_ASSERT(micro_spec != NULL);
    TEST_ASSERT(micro_spec->lto_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->function_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->data_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->gc_sections == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->whole_archive == dal_c_ToggleState_auto);
    TEST_ASSERT(micro_spec->unroll_loops == dal_c_ToggleState_disabled);
    TEST_ASSERT(micro_spec->omit_frame_pointer == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(micro_spec->async_unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(micro_spec->strip_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(micro_spec->icf_mode == dal_c_IcfMode_all);

    int cmd_count = 0;
    const dal_c_HelpCmd* build_cmd = test_find_help_cmd(dal_c_cmd_action_build, &cmd_count);
    TEST_ASSERT(build_cmd != NULL);
    TEST_ASSERT(build_cmd->implemented);
    TEST_ASSERT(!build_cmd->extends_build_options);
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_args));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_dsl));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_hosted));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_libc));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_default_libs));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_start_files));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_stdlib));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_crt));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_lto));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_omit_frame_pointer));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_function_sections));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_data_sections));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_gc_sections));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_whole_archive));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_unroll_loops));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_unwind_tables));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_async_unwind_tables));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_strip));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_icf));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_merge_all_constants));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_stack_protector));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_entry));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_target_arch));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_target_abi));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_script));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_preprocessed));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_asm));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_map));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_linked_asm));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_disasm));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_ir));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_emit_debug_info));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_demangle));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_source));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_line_numbers));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_symbolize_operands));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_raw_insn));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_save_temps));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_print_link_gc));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_analysis_artifacts));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_core));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_prefix));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_suffix));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_build));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_record));

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
            "--link-dsl=off",
            "--freestanding",
            "--link-libc",
            "--link-default-libs",
            "--link-default-libs=off",
            "--link-start-files",
            "--link-start-files=off",
            "--link-stdlib",
            "--link-stdlib=off",
            "--link-crt",
            "--lto=no",
            "--omit-frame-pointer=off",
            "--function-sections=off",
            "--data-sections=off",
            "--gc-sections=off",
            "--whole-archive",
            "--unroll-loops=off",
            "--unwind-tables=off",
            "--async-unwind-tables=off",
            "--strip",
            "--icf=all",
            "--merge-all-constants",
            "--stack-protector=off",
            "--emit-map=build/app.map",
            "--emit-linked-asm",
            "--emit-disasm=build/app.disasm.s",
            "--emit-ir=build/app.ll",
            "--emit-debug-info=build/app.debug.txt",
            "--disasm-demangle=on",
            "--disasm-source=off",
            "--disasm-line-numbers=on",
            "--disasm-symbolize-operands=on",
            "--disasm-raw-insn=off",
            "--save-temps=obj",
            "--analysis-artifacts",
            "--link-crt=off",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(39, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.dsl_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.compile_env == dal_c_CompileEnv_freestanding);
        TEST_ASSERT(cmd->opts.libc_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.default_libs_linked == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.start_files_linked == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.lto_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.omit_frame_pointer == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.function_sections == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.data_sections == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.gc_sections == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.whole_archive == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.unroll_loops == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.unwind_tables == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.async_unwind_tables == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.strip_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.icf_mode == dal_c_IcfMode_all);
        TEST_ASSERT(cmd->opts.merge_all_constants == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.stack_protector == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->payload.build.emit_map);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_map_path, "build/app.map"));
        TEST_ASSERT(cmd->payload.build.emit_linked_asm);
        TEST_ASSERT(cmd->payload.build.emit_linked_asm_path == NULL);
        TEST_ASSERT(cmd->payload.build.emit_disasm);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_disasm_path, "build/app.disasm.s"));
        TEST_ASSERT(cmd->payload.build.emit_ir);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_ir_path, "build/app.ll"));
        TEST_ASSERT(cmd->payload.build.emit_debug_info);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_debug_info_path, "build/app.debug.txt"));
        TEST_ASSERT(cmd->payload.build.disasm_demangle == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->payload.build.disasm_source == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->payload.build.disasm_line_numbers == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->payload.build.disasm_symbolize_operands == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->payload.build.disasm_raw_insn == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->payload.build.save_temps == dal_c_SaveTempsMode_obj);
        TEST_ASSERT(cmd->payload.build.analysis_artifacts);
        TEST_ASSERT(str_eql(cmd->opts.entry_symbol, "custom_entry"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "run", "optimize", "sample.c", "--emit-asm", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "--link-dsl=on",
            "--hosted",
            "--link-libc=off",
            "--link-default-libs=on",
            "--link-start-files=on",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.dsl_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.compile_env == dal_c_CompileEnv_hosted);
        TEST_ASSERT(cmd->opts.libc_linked == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.default_libs_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.start_files_linked == dal_c_ToggleState_enabled);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "--target-arch=rv32im",
            "--target-abi=ilp32",
            "--version-core=1.2.3",
            "--version-prefix=beta",
            "--version-suffix=4",
            "--version-build=20260514.120000",
            "--version-record=project",
            "--emit-preprocessed",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(10, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(str_eql(cmd->opts.target_arch, "rv32im"));
        TEST_ASSERT(str_eql(cmd->opts.target_abi, "ilp32"));
        TEST_ASSERT(cmd->opts.version.core_set);
        TEST_ASSERT(cmd->opts.version.core_major == 1u);
        TEST_ASSERT(cmd->opts.version.core_minor == 2u);
        TEST_ASSERT(cmd->opts.version.core_patch == 3u);
        TEST_ASSERT(cmd->opts.version.label_prefix_set);
        TEST_ASSERT(str_eql(cmd->opts.version.label_prefix_str, "beta"));
        TEST_ASSERT(cmd->opts.version.label_suffix_set);
        TEST_ASSERT(cmd->opts.version.label_suffix_num == 4u);
        TEST_ASSERT(cmd->opts.version.build_set);
        TEST_ASSERT(str_eql(cmd->opts.version.build_str, "20260514.120000"));
        TEST_ASSERT(cmd->version_record_mode == dal_c_VersionRecordMode_project);
        TEST_ASSERT(cmd->payload.build.emit_preprocessed);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--linker-script=linker.ld", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
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
    merged.libc_linked = dal_c_ToggleState_disabled;
    merged.dsl_mode = dal_c_ToggleState_disabled;
    merged.default_libs_linked = dal_c_ToggleState_disabled;
    merged.start_files_linked = dal_c_ToggleState_disabled;
    override.compile_env = dal_c_CompileEnv_hosted;
    override.libc_linked = dal_c_ToggleState_enabled;
    override.dsl_mode = dal_c_ToggleState_enabled;
    override.default_libs_linked = dal_c_ToggleState_enabled;
    override.start_files_linked = dal_c_ToggleState_enabled;
    dal_c_CompilerOpts_merge(&merged, &override);
    TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
    TEST_ASSERT(merged.libc_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.default_libs_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.start_files_linked == dal_c_ToggleState_enabled);

    {
        dal_c_CompilerOpts auto_override = { 0 };
        dal_c_CompilerOpts_merge(&merged, &auto_override);
        TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
        TEST_ASSERT(merged.libc_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.default_libs_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.start_files_linked == dal_c_ToggleState_enabled);
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
        "freestanding=on\n"
        "link-libc=on\n"
        "link-dsl=off\n"
        "link-default-libs=off\n"
        "link-crt=off\n"
        "version-core=1.4.2\n"
        "version-prefix=rc\n"
        "version-suffix=7\n"
        "version-build=unit.1\n"
    ));
    TEST_ASSERT(dal_c_CompilerOpts_applyDhFile(&file_opts, opts_dh));
    TEST_ASSERT(file_opts.compile_env == dal_c_CompileEnv_freestanding);
    TEST_ASSERT(file_opts.libc_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(file_opts.dsl_mode == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.default_libs_linked == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.start_files_linked == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.version.core_set);
    TEST_ASSERT(file_opts.version.core_major == 1u);
    TEST_ASSERT(file_opts.version.core_minor == 4u);
    TEST_ASSERT(file_opts.version.core_patch == 2u);
    TEST_ASSERT(file_opts.version.label_prefix_set);
    TEST_ASSERT(str_eql(file_opts.version.label_prefix_str, "rc"));
    TEST_ASSERT(file_opts.version.label_suffix_set);
    TEST_ASSERT(file_opts.version.label_suffix_num == 7u);
    TEST_ASSERT(file_opts.version.build_set);
    TEST_ASSERT(str_eql(file_opts.version.build_str, "unit.1"));
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
    char* include_dir = path_join(project_root, "include");
    char* pch_header = path_join(include_dir, "dh.h");
    char* main_source = path_join(source_dir, "main.c");
    char* crt0_source = path_join(source_dir, "crt0.S");
    char* linker_script = path_join(project_root, "linker.ld");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(include_dir != NULL);
    TEST_ASSERT(pch_header != NULL);
    TEST_ASSERT(main_source != NULL);
    TEST_ASSERT(crt0_source != NULL);
    TEST_ASSERT(linker_script != NULL);
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(dir_createRecur(include_dir));
    TEST_ASSERT(file_write(project_dh, "output=mode-contract\nversion-core=0.9.1\nversion-prefix=alpha\nversion-suffix=3\nversion-build=proj.7\n"));
    TEST_ASSERT(file_write(pch_header, "#pragma once\n#define FIXTURE_PCH 1\n"));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(crt0_source, ".globl _start\n_start:\n  ret\n"));
    TEST_ASSERT(file_write(linker_script, "ENTRY(_start)\nSECTIONS { . = 0; .text : { *(.text*) } }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    ArrStr* sources = dal_c__collectSourceFiles(proj, NULL);
    TEST_ASSERT(sources != NULL);
    TEST_ASSERT(ArrStr_len(sources) == 2);

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--freestanding",
            "--link-libc",
            "--link-default-libs=off",
            "--function-sections",
            "--data-sections",
            "--gc-sections",
            "--whole-archive",
            "--unwind-tables=off",
            "--async-unwind-tables=off",
            "--strip",
            "--icf=all",
            "--merge-all-constants",
            "--stack-protector=off",
            "--emit-map=build/mode-contract.map",
            "--save-temps=obj",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(19, argv);
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
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_DEFAULT_LIBS") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_DEFAULT_LIBS") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_START_FILES") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_START_FILES") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_COMPILER_RT") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_COMPILER_RT") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_STDLIB") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_STDLIB") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_CRT") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_CRT") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wformat=2") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Werror=uninitialized") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wframe-larger-than=4096") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wno-switch-enum") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wswitch-enum") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -ffunction-sections") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fdata-sections") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fno-unwind-tables") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fno-asynchronous-unwind-tables") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fmerge-all-constants") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fno-stack-protector") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -save-temps=obj") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") != NULL);
        TEST_ASSERT(strstr(makefile_text, "$(CC) $(CFLAGS_PCH) -MMD -MP -MF $(PCH_DEP) -x c-header $< -o $@") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") != NULL);
        TEST_ASSERT(strstr(makefile_text, "--print-libgcc-file-name") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nolibc") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,--whole-archive") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,--no-whole-archive") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,--gc-sections") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,-Map=build/mode-contract.map") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,--strip-all") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wl,--icf=all") != NULL);
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
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--hosted", "--link-libc=off", "--link-stdlib=on", NULL };
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
        if (dal_c__platformIsWindows()) {
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") == NULL);
        } else {
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") == NULL);
        }
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_DEFAULT_LIBS") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_DEFAULT_LIBS") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_START_FILES") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_START_FILES") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_COMPILER_RT") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_COMPILER_RT") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_STDLIB") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_STDLIB") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_CRT") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_CRT") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wformat=2") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Werror=uninitialized") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wframe-larger-than=4096") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wno-switch-enum") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Wswitch-enum") == NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") == NULL);
        if (dal_c__platformIsWindows()) {
            TEST_ASSERT(strstr(makefile_text, " -nolibc") == NULL);
        } else {
            TEST_ASSERT(strstr(makefile_text, " -nolibc") != NULL);
        }
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostartfiles") == NULL);
        TEST_ASSERT(strstr(makefile_text, "--print-libgcc-file-name") == NULL);

        free(makefile_text);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
    }

    /* Cross-compilation: TARGET_FLAGS must propagate into the --print-libgcc-file-name
     * shell invocation so the linker receives the cross-target's compiler-rt, not the
     * host's. Without TARGET_FLAGS, --print-libgcc-file-name returns the host path,
     * which is wrong-architecture and causes a link error. */
    {
        char* fake_compiler_src = path_join(project_root, "fake-compiler-ok.c");
#ifdef _WIN32
        char* fake_compiler_exe = path_join(project_root, "fake-compiler-ok.exe");
#else
        char* fake_compiler_exe = path_join(project_root, "fake-compiler-ok");
#endif
        char* runtime_path = path_join(project_root, "present-builtins.a");
        char* runtime_literal = strdup(runtime_path);
        char* fake_compiler_source = NULL;
        char* compiler_opt = NULL;
        const char* compile_argv[5] = { dal_c_default_compiler, "-o", fake_compiler_exe, fake_compiler_src, NULL };
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--freestanding",
            "--link-stdlib=off",
            "--target=wasm32-unknown-wasi",
            NULL,
            NULL
        };
        dal_c_Cmd* cmd = NULL;
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        TEST_ASSERT(fake_compiler_src != NULL);
        TEST_ASSERT(fake_compiler_exe != NULL);
        TEST_ASSERT(runtime_path != NULL);
        TEST_ASSERT(runtime_literal != NULL);
        TEST_ASSERT(file_write(runtime_path, "present\n"));
        for (char* cursor = runtime_literal; *cursor != '\0'; ++cursor) {
            if (*cursor == '\\') { *cursor = '/'; }
        }
        fake_compiler_source = str_format(
            "#include <stdio.h>\n"
            "#include <string.h>\n"
            "int main(int argc, char** argv) {\n"
            "    for (int i = 1; i < argc; ++i) {\n"
            "        if (strcmp(argv[i], \"--print-libgcc-file-name\") == 0) {\n"
            "            puts(\"%s\");\n"
            "            return 0;\n"
            "        }\n"
            "    }\n"
            "    return 0;\n"
            "}\n",
            runtime_literal
        );
        TEST_ASSERT(fake_compiler_source != NULL);
        TEST_ASSERT(file_write(fake_compiler_src, fake_compiler_source));
        TEST_ASSERT(proc_run(compile_argv, false) == 0);
        TEST_ASSERT(path_isFile(fake_compiler_exe));
        compiler_opt = str_format("--compiler=%s", fake_compiler_exe);
        TEST_ASSERT(compiler_opt != NULL);
        argv[6] = compiler_opt;
        cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.arch_target != NULL);
        TEST_ASSERT(strcmp(cmd->opts.arch_target, "wasm32-unknown-wasi") == 0);
        dal_c_CompilerOpts_merge(&cmd->opts, &proj->opts);

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
        /* -target must not be embedded directly; must go through TARGET_FLAGS so the
         * same variable is available in the --print-libgcc-file-name shell call. */
        TEST_ASSERT(strstr(makefile_text, "TARGET_FLAGS = -target wasm32-unknown-wasi") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += $(TARGET_FLAGS)") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -target wasm32-unknown-wasi") == NULL);
        /* compiler-rt query must use $(TARGET_FLAGS) to get the cross-target library */
        TEST_ASSERT(strstr(makefile_text, "$(TARGET_FLAGS) --print-libgcc-file-name") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__NUM__VER_CORE_MAJOR=0") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__NUM__VER_CORE_MINOR=9") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__NUM__VER_CORE_PATCH=1") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__NUM__VER_LABEL_PREFIX=0") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__STR__VER_LABEL_PREFIX=\\\"alpha\\\"") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__NUM__VER_LABEL_SUFFIX=3") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__STR__VER_LABEL_SUFFIX=\\\"3\\\"") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Ddal_c__STR__VER_BUILD=\\\"proj.7\\\"") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nolibc") == NULL);

        free(makefile_text);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
        free(compiler_opt);
        free(fake_compiler_source);
        free(runtime_literal);
        free(runtime_path);
        free(fake_compiler_exe);
        free(fake_compiler_src);
    }

    {
        char* fake_compiler_src = path_join(project_root, "fake-compiler.c");
#ifdef _WIN32
        char* fake_compiler_exe = path_join(project_root, "fake-compiler.exe");
#else
        char* fake_compiler_exe = path_join(project_root, "fake-compiler");
#endif
        char* missing_runtime_path = path_join(project_root, "missing-builtins.a");
        char* runtime_literal = strdup(missing_runtime_path);
        char* fake_compiler_source = NULL;
        char* compiler_opt = NULL;
        const char* compile_argv[5] = { dal_c_default_compiler, "-o", fake_compiler_exe, fake_compiler_src, NULL };
        const char* argv[8] = { NULL };
        dal_c_Cmd* cmd = NULL;
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        int compile_result = 0;

        TEST_ASSERT(fake_compiler_src != NULL);
        TEST_ASSERT(fake_compiler_exe != NULL);
        TEST_ASSERT(missing_runtime_path != NULL);
        TEST_ASSERT(runtime_literal != NULL);
        for (char* cursor = runtime_literal; *cursor != '\0'; ++cursor) {
            if (*cursor == '\\') { *cursor = '/'; }
        }

        fake_compiler_source = str_format(
            "#include <stdio.h>\n"
            "#include <string.h>\n"
            "int main(int argc, char** argv) {\n"
            "    for (int i = 1; i < argc; ++i) {\n"
            "        if (strcmp(argv[i], \"--print-libgcc-file-name\") == 0) {\n"
            "            puts(\"%s\");\n"
            "            return 0;\n"
            "        }\n"
            "    }\n"
            "    return 0;\n"
            "}\n",
            runtime_literal
        );
        TEST_ASSERT(fake_compiler_source != NULL);
        TEST_ASSERT(file_write(fake_compiler_src, fake_compiler_source));

        compile_result = proc_run(compile_argv, false);
        TEST_ASSERT(compile_result == 0);
        TEST_ASSERT(path_isFile(fake_compiler_exe));

        compiler_opt = str_format("--compiler=%s", fake_compiler_exe);
        TEST_ASSERT(compiler_opt != NULL);
        argv[0] = dal_c_tool_name;
        argv[1] = "build";
        argv[2] = "dev";
        argv[3] = "--freestanding";
        argv[4] = "--link-stdlib=off";
        argv[5] = "--target=wasm32-unknown-wasi";
        argv[6] = compiler_opt;
        cmd = dal_c_Cmd_parse(7, argv);
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
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 1);

        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
        free(compiler_opt);
        free(fake_compiler_source);
        free(runtime_literal);
        free(missing_runtime_path);
        free(fake_compiler_exe);
        free(fake_compiler_src);
    }

    {
        char* fake_compiler_src = path_join(project_root, "fake-compiler-image.c");
#ifdef _WIN32
        char* fake_compiler_exe = path_join(project_root, "fake-compiler-image.exe");
#else
        char* fake_compiler_exe = path_join(project_root, "fake-compiler-image");
#endif
        char* runtime_path = path_join(project_root, "present-image-builtins.a");
        char* runtime_literal = strdup(runtime_path);
        char* fake_compiler_source = NULL;
        char* linker_opt = str_format("--link-script=%s", linker_script);
        char* compiler_opt = NULL;
        const char* argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--image",
            "--freestanding",
            "--link-stdlib=off",
            "--target=riscv32-unknown-elf",
            "--target-arch=rv32im",
            "--target-abi=ilp32",
            "--objcopy=llvm-objcopy",
            "--objcopy-format=binary",
            NULL,
            NULL
        };
        dal_c_Cmd* cmd = NULL;
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        ArrStr* image_sources = ArrStr_init();
        const char* compile_argv[5] = { dal_c_default_compiler, "-o", fake_compiler_exe, fake_compiler_src, NULL };
        TEST_ASSERT(fake_compiler_src != NULL);
        TEST_ASSERT(fake_compiler_exe != NULL);
        TEST_ASSERT(runtime_path != NULL);
        TEST_ASSERT(runtime_literal != NULL);
        TEST_ASSERT(linker_opt != NULL);
        TEST_ASSERT(image_sources != NULL);
        TEST_ASSERT(file_write(runtime_path, "present\n"));
        for (char* cursor = runtime_literal; *cursor != '\0'; ++cursor) {
            if (*cursor == '\\') { *cursor = '/'; }
        }
        fake_compiler_source = str_format(
            "#include <stdio.h>\n"
            "#include <string.h>\n"
            "int main(int argc, char** argv) {\n"
            "    for (int i = 1; i < argc; ++i) {\n"
            "        if (strcmp(argv[i], \"--print-libgcc-file-name\") == 0) {\n"
            "            puts(\"%s\");\n"
            "            return 0;\n"
            "        }\n"
            "    }\n"
            "    return 0;\n"
            "}\n",
            runtime_literal
        );
        TEST_ASSERT(fake_compiler_source != NULL);
        TEST_ASSERT(file_write(fake_compiler_src, fake_compiler_source));
        TEST_ASSERT(proc_run(compile_argv, false) == 0);
        compiler_opt = str_format("--compiler=%s", fake_compiler_exe);
        TEST_ASSERT(compiler_opt != NULL);
        ArrStr_push(image_sources, main_source);
        ArrStr_push(image_sources, crt0_source);
        argv[10] = linker_opt;
        argv[11] = compiler_opt;
        cmd = dal_c_Cmd_parse(12, argv);
        TEST_ASSERT(cmd != NULL);
        cmd->link_args = strdup("-Wl,--gc-sections -fuse-ld=lld");
        TEST_ASSERT(cmd->link_args != NULL);

        profile = dal_c_ProfileSpec_by(cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        build_dir = dal_c_Project_getBuildDir(proj);
        profile_dir = path_join(build_dir, profile->name);
        object_dir = path_join(profile_dir, "obj");
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));

        target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, "guest", dal_c_Target_image);
        TEST_ASSERT(target_path != NULL);
        TEST_ASSERT(strstr(target_path, ".bin") != NULL);
        makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_image);
        TEST_ASSERT(makefile_path != NULL);
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, image_sources, target_path, object_dir, dal_c_Target_image) == 0);
        makefile_text = file_read(makefile_path);
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "TARGET_ARCH_FLAGS = -march=rv32im") != NULL);
        TEST_ASSERT(strstr(makefile_text, "TARGET_ABI_FLAGS = -mabi=ilp32") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += $(TARGET_ARCH_FLAGS)") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += $(TARGET_ABI_FLAGS)") != NULL);
        TEST_ASSERT(strstr(makefile_text, "LDFLAGS = $(TARGET_FLAGS) $(TARGET_ARCH_FLAGS) $(TARGET_ABI_FLAGS) $(SYSROOT_FLAGS)") != NULL);
        TEST_ASSERT(strstr(makefile_text, "OBJCOPY = llvm-objcopy") != NULL);
        TEST_ASSERT(strstr(makefile_text, "OBJCOPY_FORMAT = binary") != NULL);
        TEST_ASSERT(strstr(makefile_text, "LINK_TARGET = ") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Xlinker -T -Xlinker ") != NULL);
        TEST_ASSERT(strstr(makefile_text, "--print-libgcc-file-name") == NULL);
        TEST_ASSERT(strstr(makefile_text, "[AS] ") != NULL);
        TEST_ASSERT(strstr(makefile_text, "[OBJCOPY] $@") != NULL);
        TEST_ASSERT(strstr(makefile_text, "$(TARGET): $(LINK_CONTRACT) $(LINK_TARGET)") != NULL);
        TEST_ASSERT(strstr(makefile_text, "$(LINK_TARGET): $(LINK_CONTRACT) $(OBJS)") != NULL);

        free(makefile_text);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        ArrStr_fini(&image_sources);
        dal_c_Cmd_cleanup(&cmd);
        free(compiler_opt);
        free(fake_compiler_source);
        free(runtime_literal);
        free(runtime_path);
        free(fake_compiler_exe);
        free(fake_compiler_src);
        free(linker_opt);
    }

    {
        const char* hosted_argv[] = { dal_c_tool_name, "build", "dev", "--link-dsl=off", NULL };
        const char* guest_argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--link-dsl=off",
            "--target=riscv32-unknown-elf",
            "--target-arch=rv32im",
            "--target-abi=ilp32",
            NULL
        };
        dal_c_Cmd* hosted_cmd = dal_c_Cmd_parse(4, hosted_argv);
        dal_c_Cmd* guest_cmd = dal_c_Cmd_parse(7, guest_argv);
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* hosted_makefile = NULL;
        char* guest_makefile = NULL;
        char* hosted_text = NULL;
        char* guest_text = NULL;
        char* hosted_pch_out = NULL;
        char* guest_pch_out = NULL;
        char* hosted_pch_line = NULL;
        char* guest_pch_line = NULL;
        char* hosted_pch_end = NULL;
        char* guest_pch_end = NULL;

        TEST_ASSERT(hosted_cmd != NULL);
        TEST_ASSERT(guest_cmd != NULL);

        profile = dal_c_ProfileSpec_by(hosted_cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        build_dir = dal_c_Project_getBuildDir(proj);
        profile_dir = path_join(build_dir, profile->name);
        object_dir = path_join(profile_dir, "obj");
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));

        target_path = dal_c__resolveOutputPath(proj, hosted_cmd, profile_dir, "probe", dal_c_Target_executable);
        TEST_ASSERT(target_path != NULL);
        hosted_makefile = dal_c__makePlanFilePath(proj, profile, hosted_cmd, target_path, dal_c_Target_executable);
        TEST_ASSERT(hosted_makefile != NULL);
        TEST_ASSERT(dal_c__generateMakefile(hosted_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        hosted_text = file_read(hosted_makefile);
        TEST_ASSERT(hosted_text != NULL);
        free(target_path);

        target_path = dal_c__resolveOutputPath(proj, guest_cmd, profile_dir, "probe-guest", dal_c_Target_executable);
        TEST_ASSERT(target_path != NULL);
        guest_makefile = dal_c__makePlanFilePath(proj, profile, guest_cmd, target_path, dal_c_Target_executable);
        TEST_ASSERT(guest_makefile != NULL);
        TEST_ASSERT(dal_c__generateMakefile(guest_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        guest_text = file_read(guest_makefile);
        TEST_ASSERT(guest_text != NULL);

        hosted_pch_line = strstr(hosted_text, "PCH_OUT = ");
        guest_pch_line = strstr(guest_text, "PCH_OUT = ");
        TEST_ASSERT(hosted_pch_line != NULL);
        TEST_ASSERT(guest_pch_line != NULL);
        hosted_pch_end = strchr(hosted_pch_line, '\n');
        guest_pch_end = strchr(guest_pch_line, '\n');
        TEST_ASSERT(hosted_pch_end != NULL);
        TEST_ASSERT(guest_pch_end != NULL);
        hosted_pch_out = strndup(
            hosted_pch_line + strlen("PCH_OUT = "),
            (size_t)(hosted_pch_end - (hosted_pch_line + strlen("PCH_OUT = ")))
        );
        guest_pch_out = strndup(
            guest_pch_line + strlen("PCH_OUT = "),
            (size_t)(guest_pch_end - (guest_pch_line + strlen("PCH_OUT = ")))
        );
        TEST_ASSERT(hosted_pch_out != NULL);
        TEST_ASSERT(guest_pch_out != NULL);
        TEST_ASSERT(strcmp(hosted_pch_out, guest_pch_out) != 0);

        free(guest_pch_out);
        free(hosted_pch_out);
        free(guest_text);
        free(hosted_text);
        free(guest_makefile);
        free(hosted_makefile);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&guest_cmd);
        dal_c_Cmd_cleanup(&hosted_cmd);
    }

    {
        const char* emit_argv[] = {
            dal_c_tool_name,
            "build",
            "dev",
            "--emit-preprocessed",
            "--target-arch=rv32im",
            "--target-abi=ilp32",
            NULL
        };
        const char* asm_argv[] = {
            dal_c_tool_name,
            "build",
            "release",
            "--emit-asm",
            NULL
        };
        const char* analysis_argv[] = {
            dal_c_tool_name,
            "build",
            "release",
            "--icf=safe",
            "--emit-map",
            "--emit-linked-asm",
            "--emit-disasm",
            "--emit-ir",
            "--emit-debug-info",
            "--disasm-demangle=on",
            "--disasm-source=off",
            "--disasm-line-numbers=on",
            "--disasm-symbolize-operands=on",
            "--disasm-raw-insn=off",
            NULL
        };
        dal_c_Cmd* emit_cmd = dal_c_Cmd_parse(6, emit_argv);
        dal_c_Cmd* asm_cmd = dal_c_Cmd_parse(4, asm_argv);
        dal_c_Cmd* analysis_cmd = dal_c_Cmd_parse(14, analysis_argv);
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        ArrStr* single_source = ArrStr_init();

        TEST_ASSERT(emit_cmd != NULL);
        TEST_ASSERT(asm_cmd != NULL);
        TEST_ASSERT(analysis_cmd != NULL);
        TEST_ASSERT(single_source != NULL);
        ArrStr_push(single_source, main_source);

        {
            const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(emit_cmd->opts.profile);
            TEST_ASSERT(profile != NULL);
            build_dir = dal_c_Project_getBuildDir(proj);
            profile_dir = path_join(build_dir, profile->name);
            object_dir = path_join(profile_dir, "obj");
            TEST_ASSERT(build_dir != NULL);
            TEST_ASSERT(profile_dir != NULL);
            TEST_ASSERT(object_dir != NULL);
            TEST_ASSERT(dir_createRecur(object_dir));

            target_path = dal_c__resolveOutputPath(proj, emit_cmd, profile_dir, "probe", dal_c_Target_preprocessed);
            TEST_ASSERT(target_path != NULL);
            TEST_ASSERT(strstr(target_path, ".i") != NULL);
            makefile_path = dal_c__makePlanFilePath(proj, profile, emit_cmd, target_path, dal_c_Target_preprocessed);
            TEST_ASSERT(makefile_path != NULL);
            TEST_ASSERT(dal_c__generateMakefile(emit_cmd, proj, profile, single_source, target_path, object_dir, dal_c_Target_preprocessed) == 0);
            makefile_text = file_read(makefile_path);
            TEST_ASSERT(makefile_text != NULL);
            TEST_ASSERT(strstr(makefile_text, "TARGET = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "SRC = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "[CPP] ") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -E $(SRC) -o $@") != NULL);
            TEST_ASSERT(strstr(makefile_text, "$(TARGET): $(SRC)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "TARGET_ARCH_FLAGS = -march=rv32im") != NULL);
            TEST_ASSERT(strstr(makefile_text, "TARGET_ABI_FLAGS = -mabi=ilp32") != NULL);

            free(makefile_text);
            free(makefile_path);
            free(target_path);
            free(object_dir);
            free(profile_dir);
            free(build_dir);
            makefile_text = NULL;
            makefile_path = NULL;
            target_path = NULL;
            object_dir = NULL;
            profile_dir = NULL;
            build_dir = NULL;
        }

        {
            const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(analysis_cmd->opts.profile);
            TEST_ASSERT(profile != NULL);
            build_dir = dal_c_Project_getBuildDir(proj);
            profile_dir = path_join(build_dir, profile->name);
            object_dir = path_join(profile_dir, "obj");
            TEST_ASSERT(build_dir != NULL);
            TEST_ASSERT(profile_dir != NULL);
            TEST_ASSERT(object_dir != NULL);
            TEST_ASSERT(dir_createRecur(object_dir));

            target_path = dal_c__resolveOutputPath(proj, analysis_cmd, profile_dir, "probe", dal_c_Target_executable);
            TEST_ASSERT(target_path != NULL);
            makefile_path = dal_c__makePlanFilePath(proj, profile, analysis_cmd, target_path, dal_c_Target_executable);
            TEST_ASSERT(makefile_path != NULL);
            TEST_ASSERT(dal_c__generateMakefile(analysis_cmd, proj, profile, single_source, target_path, object_dir, dal_c_Target_executable) == 0);
            makefile_text = file_read(makefile_path);
            TEST_ASSERT(makefile_text != NULL);
            TEST_ASSERT(strstr(makefile_text, " -Wl,-Map=") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LINKED_ASM = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "DISASM = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "DISASM_TARGET = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "DISASM_INPUT = $(DISASM_TARGET)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "OBJS_SHELL = $(subst \\,/,$(OBJS))") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_SHELL = $(subst \\,/,$(LDFLAGS))") != NULL);
            TEST_ASSERT(strstr(makefile_text, "COMMA = ,") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_DISASM = $(filter-out -Wl$(COMMA)--strip-all,$(LDFLAGS))") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_DISASM_SHELL = $(subst \\,/,$(LDFLAGS_DISASM))") != NULL);
            TEST_ASSERT(strstr(makefile_text, "IR = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "DEBUG_INFO = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(LINKED_ASM)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DISASM_TARGET)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DISASM)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(IR)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DEBUG_INFO)") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -Wl,--lto-emit-asm") != NULL);
            TEST_ASSERT(strstr(makefile_text, "$(CC) $(OBJS_SHELL) -o \"$(subst \\,/,$@)\" $(LDFLAGS_DISASM_SHELL)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "llvm-objdump -d --demangle --line-numbers --symbolize-operands --no-show-raw-insn \"$(subst \\,/,$(DISASM_INPUT))\" > \"$(subst \\,/,$@)\"") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -S -emit-llvm $(firstword $(SRCS)) -o \"$(subst \\,/,$@)\"") != NULL);
#ifdef _WIN32
            TEST_ASSERT(strstr(makefile_text, "PDB = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "llvm-pdbutil dump -symbols -globals -publics \"$(subst \\,/,$(PDB))\" > \"$(subst \\,/,$@)\"") != NULL);
#else
            TEST_ASSERT(strstr(makefile_text, "llvm-dwarfdump --debug-info --debug-line \"$(subst \\,/,$(TARGET))\" > \"$(subst \\,/,$@)\"") != NULL);
#endif

            free(makefile_text);
            free(makefile_path);
            free(target_path);
            free(object_dir);
            free(profile_dir);
            free(build_dir);
            makefile_text = NULL;
            makefile_path = NULL;
            target_path = NULL;
            object_dir = NULL;
            profile_dir = NULL;
            build_dir = NULL;
        }

        {
            const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(asm_cmd->opts.profile);
            TEST_ASSERT(profile != NULL);
            build_dir = dal_c_Project_getBuildDir(proj);
            profile_dir = path_join(build_dir, profile->name);
            object_dir = path_join(profile_dir, "obj");
            TEST_ASSERT(build_dir != NULL);
            TEST_ASSERT(profile_dir != NULL);
            TEST_ASSERT(object_dir != NULL);
            TEST_ASSERT(dir_createRecur(object_dir));

            target_path = dal_c__resolveOutputPath(proj, asm_cmd, profile_dir, "probe", dal_c_Target_assembly);
            TEST_ASSERT(target_path != NULL);
            TEST_ASSERT(strstr(target_path, ".s") != NULL);
            makefile_path = dal_c__makePlanFilePath(proj, profile, asm_cmd, target_path, dal_c_Target_assembly);
            TEST_ASSERT(makefile_path != NULL);
            TEST_ASSERT(dal_c__generateMakefile(asm_cmd, proj, profile, single_source, target_path, object_dir, dal_c_Target_assembly) == 0);
            makefile_text = file_read(makefile_path);
            TEST_ASSERT(makefile_text != NULL);
            TEST_ASSERT(strstr(makefile_text, "[ASM] ") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -S $(SRC) -o $@") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-flto") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-fno-lto") == NULL);

            free(makefile_text);
            free(makefile_path);
            free(target_path);
            free(object_dir);
            free(profile_dir);
            free(build_dir);
        }

        ArrStr_fini(&single_source);
        dal_c_Cmd_cleanup(&analysis_cmd);
        dal_c_Cmd_cleanup(&asm_cmd);
        dal_c_Cmd_cleanup(&emit_cmd);
    }

    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(main_source);
    free(crt0_source);
    free(linker_script);
    free(pch_header);
    free(include_dir);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_project_detection(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* lib_kind_root = path_join(temp_root, "lib-kind-project");
    char* lib_kind_project_dh = path_join(lib_kind_root, "project.dh");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(lib_kind_root != NULL);
    TEST_ASSERT(lib_kind_project_dh != NULL);
    TEST_ASSERT(dir_createRecur(lib_kind_root));
    TEST_ASSERT(file_write(lib_kind_project_dh, "kind=lib\noutput=core\n"));

    dal_c_Project* lib_kind_proj = dal_c_Project_detectAt(lib_kind_root, NULL);
    TEST_ASSERT(lib_kind_proj != NULL);
    TEST_ASSERT(lib_kind_proj->defaults.target_kind_set);
    TEST_ASSERT(lib_kind_proj->defaults.target_kind == dal_c_Target_lib);
    dal_c_Project_cleanup(&lib_kind_proj);
    free(lib_kind_project_dh);
    free(lib_kind_root);
    free(temp_root);

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
    TEST_ASSERT(request.link_project);
    TEST_ASSERT(request.resolved_is_dir);
    TEST_ASSERT(request.relative_path != NULL);
    TEST_ASSERT(str_eql(request.relative_path, "runner1"));
    dal_c_TargetRequest_cleanup(&request);

    intent.target_root_name_hint = "plugins";
    intent.target_path = "render";
    TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
    TEST_ASSERT(request.kind == dal_c_Target_shared_lib);
    TEST_ASSERT(request.selection == dal_c_TargetSelection_dir);
    TEST_ASSERT(request.link_project);
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
