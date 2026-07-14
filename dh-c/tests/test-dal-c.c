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
static void test_pch_dependency_invalidates_linked_plan(void);
static void test_project_detection(void);
static void test_clean_prefers_local_build_dir(void);
static void test_target_request_resolution(void);
static void test_explicit_file_build_uses_file_project(void);
static void test_target_root_directory_uses_local_include(void);
static void test_syntax_arguments_follow_build_compile_contract(void);
static void test_compile_db_command(void);
static void test_deps_prelude_tracks_dh_contract(void);
static void test_skip_source_filters(void);
static void test_test_source_classification(void);
static void test_test_mode_is_command_scoped(void);
static void test_source_collection_ignores_hidden_ancestors(void);

static char* test_repo_path(const char* relative_path);
static char* test_repo_root(void);
static char* test_temp_root(void);
static void test_reset_temp_root(void);
static void test_free_str_array(char** items, int count);
static char* test_makefile_var_first_value(const char* makefile_text, const char* var_name);
static bool test_arrstr_contains(ArrStr* items, const char* value);
static const dal_c_HelpProfile* test_find_help_profile(const char* name);
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
    RUN_TEST(test_pch_dependency_invalidates_linked_plan);
    RUN_TEST(test_project_detection);
    RUN_TEST(test_clean_prefers_local_build_dir);
    RUN_TEST(test_target_request_resolution);
    RUN_TEST(test_explicit_file_build_uses_file_project);
    RUN_TEST(test_target_root_directory_uses_local_include);
    RUN_TEST(test_syntax_arguments_follow_build_compile_contract);
    RUN_TEST(test_compile_db_command);
    RUN_TEST(test_deps_prelude_tracks_dh_contract);
    RUN_TEST(test_skip_source_filters);
    RUN_TEST(test_test_source_classification);
    RUN_TEST(test_test_mode_is_command_scoped);
    RUN_TEST(test_source_collection_ignores_hidden_ancestors);
    test_reset_temp_root();
    return g_test_failures == 0 ? 0 : 1;
}

static char* test_repo_path(const char* relative_path) {
    char* root = test_repo_root();
    if (!root) { return NULL; }
    char* path = path_join(root, relative_path);
    free(root);
    return path;
}

static char* test_repo_root(void) {
    char* current = env_getCWD();
    if (!current) { return NULL; }

    for (;;) {
        char* dh_project = path_join(current, "dh-c/project.dh");
        char* test_source = path_join(current, "dh-c/tests/test-dal-c.c");
        bool is_root = path_isFile(dh_project) && path_isFile(test_source);
        free(dh_project);
        free(test_source);
        if (is_root) {
            return current;
        }

        char* parent = path_parent(current);
        if (!parent || str_eql(parent, current)) {
            free(parent);
            free(current);
            return NULL;
        }
        free(current);
        current = parent;
    }
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

static char* test_makefile_var_first_value(const char* makefile_text, const char* var_name) {
    if (!makefile_text || !var_name) { return NULL; }
    char* prefix = str_format("%s =", var_name);
    if (!prefix) { return NULL; }
    char* line = strstr(makefile_text, prefix);
    free(prefix);
    if (!line) { return NULL; }
    char* value = strchr(line, '=');
    if (!value) { return NULL; }
    ++value;
    while (*value == ' ' || *value == '\t') { ++value; }
    char* end = value;
    while (*end != '\0' && *end != ' ' && *end != '\t' && *end != '\r' && *end != '\n') { ++end; }
    size_t len = (size_t)(end - value);
    char* out = malloc(len + 1);
    if (!out) { return NULL; }
    memcpy(out, value, len);
    out[len] = '\0';
    return out;
}

static bool test_arrstr_contains(ArrStr* items, const char* value) {
    if (!items || !value) { return false; }
    for (int i = 0; i < ArrStr_len(items); ++i) {
        if (str_eql(ArrStr_at(items, i), value)) {
            return true;
        }
    }
    return false;
}

static const dal_c_HelpProfile* test_find_help_profile(const char* name) {
    if (!name) { return NULL; }
    for (int i = 0; i < dal_c_help_profiles_count; ++i) {
        if (str_eql(dal_c_help_profiles[i].name, name)) {
            return &dal_c_help_profiles[i];
        }
    }
    return NULL;
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

    char* path_before = test_repo_path("dh-c");
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
    TEST_ASSERT(dal_c_LinkMode_parse("auto") == dal_c_LinkMode_auto);
    TEST_ASSERT(dal_c_LinkMode_parse("static") == dal_c_LinkMode_static);
    TEST_ASSERT(dal_c_LinkMode_parse("shared") == dal_c_LinkMode_shared);
    TEST_ASSERT(dal_c_LinkMode_toLibraryLinking(dal_c_LinkMode_auto, dal_c_Linking_static) == dal_c_Linking_static);
    TEST_ASSERT(dal_c_LinkMode_toLibraryLinking(dal_c_LinkMode_shared, dal_c_Linking_static) == dal_c_Linking_shared);
    TEST_ASSERT(str_eql(dal_c_OptiLevel_toFlag(dal_c_OptiLevel_balanced), "-O2"));
    TEST_ASSERT(dal_c_DebugLevel_fromFlag("-g3") == dal_c_DebugLevel_extended);
    TEST_ASSERT(dal_c_Profile_parse("fast") == dal_c_Profile_fast);
    TEST_ASSERT(str_eql(dal_c_Profile_format(dal_c_Profile_fast), dal_c_profile_fast));
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
    TEST_ASSERT(dal_c_LtoMode_parse("on") == dal_c_LtoMode_on);
    TEST_ASSERT(dal_c_LtoMode_parse("full") == dal_c_LtoMode_full);
    TEST_ASSERT(dal_c_LtoMode_parse("thin") == dal_c_LtoMode_thin);
    TEST_ASSERT(str_eql(dal_c_LtoMode_toFlag(dal_c_LtoMode_full), "-flto=full"));
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("warn") == dal_c_LooseErrorsMode_warn);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("suppress") == dal_c_LooseErrorsMode_suppress);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("auto") == dal_c_LooseErrorsMode_auto);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("never") == dal_c_LooseErrorsMode_never);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("off") == dal_c_LooseErrorsMode_strict);

    const dal_c_ProfileSpec* release_spec = dal_c_ProfileSpec_by(dal_c_Profile_release);
    TEST_ASSERT(release_spec != NULL);
    TEST_ASSERT(str_eql(release_spec->name, dal_c_profile_release));
    TEST_ASSERT(release_spec->opti_level == dal_c_OptiLevel_aggressive);

    const dal_c_ProfileSpec* fast_spec = dal_c_ProfileSpec_by(dal_c_Profile_fast);
    TEST_ASSERT(fast_spec != NULL);
    TEST_ASSERT(str_eql(fast_spec->name, dal_c_profile_fast));
    TEST_ASSERT(fast_spec->opti_level == dal_c_OptiLevel_none);
    TEST_ASSERT(fast_spec->debug_level == dal_c_DebugLevel_none);
    TEST_ASSERT(fast_spec->debug_assertions);
    TEST_ASSERT(fast_spec->lto_mode == dal_c_LtoMode_off);
    TEST_ASSERT(fast_spec->function_sections == dal_c_ToggleState_disabled);
    TEST_ASSERT(fast_spec->data_sections == dal_c_ToggleState_disabled);
    TEST_ASSERT(fast_spec->gc_sections == dal_c_ToggleState_disabled);

    const dal_c_ProfileSpec* optimize_spec = dal_c_ProfileSpec_by(dal_c_Profile_optimize);
    TEST_ASSERT(optimize_spec != NULL);
    TEST_ASSERT(optimize_spec->lto_mode == dal_c_LtoMode_on);
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
    TEST_ASSERT(str_eql(optimize_spec->target_tune, "native"));
    TEST_ASSERT(optimize_spec->exceptions == dal_c_ToggleState_disabled);

    const dal_c_ProfileSpec* micro_spec = dal_c_ProfileSpec_by(dal_c_Profile_micro);
    TEST_ASSERT(micro_spec != NULL);
    TEST_ASSERT(micro_spec->lto_mode == dal_c_LtoMode_on);
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
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_exceptions));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_strip));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_icf));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_merge_all_constants));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_stack_protector));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_entry));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_target_arch));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_target_tune));
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
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_disasm_section_contents));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_save_temps));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_print_link_gc));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_analysis_artifacts));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_core));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_prefix));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_suffix));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_build));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_version_record));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_loose_errors));

    const dal_c_HelpCmd* workspace_cmd = test_find_help_cmd(dal_c_cmd_action_workspace, NULL);
    TEST_ASSERT(workspace_cmd != NULL);
    TEST_ASSERT(!workspace_cmd->implemented);

    const dal_c_HelpCmd* compile_db_cmd = test_find_help_cmd(dal_c_cmd_action_compile_db, NULL);
    TEST_ASSERT(compile_db_cmd != NULL);
    TEST_ASSERT(compile_db_cmd->implemented);
    TEST_ASSERT(test_help_has_option(compile_db_cmd, dal_c_opt_output));

    const dal_c_HelpCmd* syntax_cmd = test_find_help_cmd(dal_c_cmd_action_syntax, NULL);
    TEST_ASSERT(syntax_cmd != NULL);
    TEST_ASSERT(syntax_cmd->implemented);
    TEST_ASSERT(test_help_has_option(syntax_cmd, dal_c_opt_progress));

    const dal_c_HelpCmd* tidy_cmd = test_find_help_cmd(dal_c_cmd_action_tidy, NULL);
    TEST_ASSERT(tidy_cmd != NULL);
    TEST_ASSERT(tidy_cmd->implemented);
    TEST_ASSERT(test_help_has_option(tidy_cmd, dal_c_opt_commands));

    const dal_c_HelpCmd* format_cmd = test_find_help_cmd(dal_c_cmd_action_format, NULL);
    TEST_ASSERT(format_cmd != NULL);
    TEST_ASSERT(format_cmd->implemented);
    TEST_ASSERT(test_help_has_option(format_cmd, dal_c_opt_verbose));

    const int option_count = dal_c_help_global_options_count;
    TEST_ASSERT(option_count == 2);
    TEST_ASSERT(cmd_count >= 10);

    const int profile_count = dal_c_help_profiles_count;
    TEST_ASSERT(profile_count >= 9);
    TEST_ASSERT(test_find_help_profile(dal_c_profile_fast) != NULL);

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
        char* test_source = test_repo_path("dh-c/tests/test-dal-c.c");
        char* str_source = test_repo_path("dh-c/src/dal-c-ext/str.c");
        TEST_ASSERT(test_source != NULL);
        TEST_ASSERT(str_source != NULL);
        char* file_a = str_format("--file=%s", test_source);
        char* file_b = str_format("--file=%s", str_source);
        const char* argv[] = { dal_c_tool_name, "build", file_a, file_b, "-obundle", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->input_count == 2);
        TEST_ASSERT(str_eql(cmd->payload.build.output_path, "bundle"));
        dal_c_Cmd_cleanup(&cmd);
        free(file_b);
        free(file_a);
        free(str_source);
        free(test_source);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "--self", "--cache", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "syntax", "release", "--all", "--progress=hide", "--commands=show", "--verbose=off", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_syntax);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_release);
        TEST_ASSERT(cmd->payload.build.build_all);
        TEST_ASSERT(!cmd->show_progress);
        TEST_ASSERT(cmd->show_commands);
        TEST_ASSERT(!cmd->verbose);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        char* str_source = test_repo_path("dh-c/src/dal-c-ext/str.c");
        TEST_ASSERT(str_source != NULL);
        char* file_arg = str_format("--file=%s", str_source);
        TEST_ASSERT(file_arg != NULL);
        const char* argv[] = { dal_c_tool_name, "tidy", "dev", file_arg, NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_tidy);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_dev);
        TEST_ASSERT(cmd->input_count == 1);
        dal_c_Cmd_cleanup(&cmd);
        free(file_arg);
        free(str_source);
    }

    {
        char* str_source = test_repo_path("dh-c/src/dal-c-ext/str.c");
        TEST_ASSERT(str_source != NULL);
        char* file_arg = str_format("--file=%s", str_source);
        TEST_ASSERT(file_arg != NULL);
        const char* argv[] = { dal_c_tool_name, "format", file_arg, NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_format_code);
        TEST_ASSERT(cmd->input_count == 1);
        dal_c_Cmd_cleanup(&cmd);
        free(file_arg);
        free(str_source);
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
            "--disasm-section-contents=on",
            "--save-temps=obj",
            "--analysis-artifacts",
            "--link-crt=off",
            "--entry=custom_entry",
            "--loose-errors=suppress",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(41, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.dsl_mode == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.compile_env == dal_c_CompileEnv_freestanding);
        TEST_ASSERT(cmd->opts.libc_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->opts.default_libs_linked == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.start_files_linked == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->opts.lto_mode == dal_c_LtoMode_off);
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
        TEST_ASSERT(cmd->payload.build.disasm_section_contents == dal_c_ToggleState_enabled);
        TEST_ASSERT(cmd->payload.build.save_temps == dal_c_SaveTempsMode_obj);
        TEST_ASSERT(cmd->payload.build.analysis_artifacts);
        TEST_ASSERT(str_eql(cmd->opts.entry_symbol, "custom_entry"));
        TEST_ASSERT(cmd->opts.loose_errors == dal_c_LooseErrorsMode_suppress);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--loose-errors", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.loose_errors == dal_c_LooseErrorsMode_warn);
        dal_c_Cmd_cleanup(&cmd);
    }


    {
        const char* argv[] = { dal_c_tool_name, "build", "--progress=hide", "--commands=show", "--verbose=off", "--jobs=3", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(6, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build);
        TEST_ASSERT(!cmd->show_progress);
        TEST_ASSERT(cmd->show_commands);
        TEST_ASSERT(!cmd->verbose);
        TEST_ASSERT(str_eql(cmd->make_jobs, "3"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--progress=off", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
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
            "--target-tune=generic",
            "--target-abi=ilp32",
            "--exceptions=off",
            "--version-core=1.2.3",
            "--version-prefix=beta",
            "--version-suffix=4",
            "--version-build=20260514.120000",
            "--version-record=project",
            "--emit-preprocessed",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(12, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(str_eql(cmd->opts.target_arch, "rv32im"));
        TEST_ASSERT(str_eql(cmd->opts.target_tune, "generic"));
        TEST_ASSERT(str_eql(cmd->opts.target_abi, "ilp32"));
        TEST_ASSERT(cmd->opts.exceptions == dal_c_ToggleState_disabled);
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
        const char* argv[] = { dal_c_tool_name, "build-self", "optimize", "--commands=show", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build_self);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_optimize);
        TEST_ASSERT(cmd->profile_explicit);
        TEST_ASSERT(cmd->show_commands);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = {
            dal_c_tool_name,
            "build-self",
            "fast",
            "--commands=show",
            "--emit-ir=build/self.ll",
            "--emit-disasm=build/self.disasm.s",
            "--disasm-source=off",
            "--analysis-artifacts",
            "--save-temps=obj",
            "--loose-errors=warn",
            "--comp-args=-Wno-unused-function",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(11, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build_self);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_fast);
        TEST_ASSERT(cmd->show_commands);
        TEST_ASSERT(cmd->payload.build.emit_ir);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_ir_path, "build/self.ll"));
        TEST_ASSERT(cmd->payload.build.emit_disasm);
        TEST_ASSERT(str_eql(cmd->payload.build.emit_disasm_path, "build/self.disasm.s"));
        TEST_ASSERT(cmd->payload.build.disasm_source == dal_c_ToggleState_disabled);
        TEST_ASSERT(cmd->payload.build.analysis_artifacts);
        TEST_ASSERT(cmd->payload.build.emit_map);
        TEST_ASSERT(cmd->payload.build.emit_linked_asm);
        TEST_ASSERT(cmd->payload.build.save_temps == dal_c_SaveTempsMode_obj);
        TEST_ASSERT(cmd->opts.loose_errors == dal_c_LooseErrorsMode_warn);
        TEST_ASSERT(str_eql(cmd->compiler_args, "-Wno-unused-function"));
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
        const char* argv[] = {
            dal_c_tool_name,
            "compile-db",
            "dev",
            "--remove",
            "--output=compile_commands.json",
            "--all",
            "--define=APP=1",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_compile_db);
        TEST_ASSERT(cmd->opts.profile == dal_c_Profile_dev);
        TEST_ASSERT(cmd->profile_explicit);
        TEST_ASSERT(cmd->payload.build.remove_output);
        TEST_ASSERT(cmd->payload.build.build_all);
        TEST_ASSERT(str_eql(cmd->payload.build.output_path, "compile_commands.json"));
        TEST_ASSERT(cmd->opts.define_count == 1);
        TEST_ASSERT(str_eql(cmd->opts.define_macros[0], "APP=1"));
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
    override.loose_errors = dal_c_LooseErrorsMode_warn;
    dal_c_CompilerOpts_merge(&merged, &override);
    TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
    TEST_ASSERT(merged.libc_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.default_libs_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.start_files_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.loose_errors == dal_c_LooseErrorsMode_warn);

    {
        dal_c_CompilerOpts auto_override = { 0 };
        dal_c_CompilerOpts_merge(&merged, &auto_override);
        TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
        TEST_ASSERT(merged.libc_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.default_libs_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.start_files_linked == dal_c_ToggleState_enabled);
        TEST_ASSERT(merged.loose_errors == dal_c_LooseErrorsMode_warn);
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
        "target-tune=generic\n"
        "exceptions=off\n"
        "loose-errors=suppress\n"
    ));
    TEST_ASSERT(dal_c_CompilerOpts_applyDHFile(&file_opts, opts_dh));
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
    TEST_ASSERT(str_eql(file_opts.target_tune, "generic"));
    TEST_ASSERT(file_opts.exceptions == dal_c_ToggleState_disabled);
    TEST_ASSERT(file_opts.loose_errors == dal_c_LooseErrorsMode_suppress);
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
            "--target-tune=generic",
            "--exceptions=off",
            "--strip",
            "--icf=all",
            "--merge-all-constants",
            "--stack-protector=off",
            "--emit-map=build/mode-contract.map",
            "--save-temps=obj",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(21, argv);
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
        TEST_ASSERT(strstr(makefile_text, "TARGET_TUNE_FLAGS = -mtune=generic") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += $(TARGET_TUNE_FLAGS)") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fno-exceptions") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fmerge-all-constants") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -fno-stack-protector") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -save-temps=obj") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-include-pch") != NULL);
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
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--loose-errors=suppress", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        const dal_c_ProfileSpec* profile = NULL;
        char* build_dir = NULL;
        char* profile_dir = NULL;
        char* object_dir = NULL;
        char* target_path = NULL;
        char* makefile_path = NULL;
        char* makefile_text = NULL;
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.loose_errors == dal_c_LooseErrorsMode_suppress);

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
        TEST_ASSERT(strstr(makefile_text, " -w") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -Werror") == NULL);

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
        TEST_ASSERT(strstr(makefile_text, "P_AS = printf") != NULL);
        TEST_ASSERT(strstr(makefile_text, "P_OBJCOPY = printf") != NULL);
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

        hosted_pch_line = strstr(hosted_text, "-include-pch");
        guest_pch_line = strstr(guest_text, "-include-pch");
        hosted_pch_out = test_makefile_var_first_value(hosted_text, "PCH_OUT");
        guest_pch_out = test_makefile_var_first_value(guest_text, "PCH_OUT");
        TEST_ASSERT(hosted_pch_line != NULL);
        TEST_ASSERT(guest_pch_line != NULL);
        TEST_ASSERT(hosted_pch_out != NULL);
        TEST_ASSERT(guest_pch_out != NULL);
        TEST_ASSERT(!str_eql(hosted_pch_out, guest_pch_out));

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
            "--disasm-section-contents=on",
            NULL
        };
        dal_c_Cmd* emit_cmd = dal_c_Cmd_parse(6, emit_argv);
        dal_c_Cmd* asm_cmd = dal_c_Cmd_parse(4, asm_argv);
        dal_c_Cmd* analysis_cmd = dal_c_Cmd_parse(15, analysis_argv);
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
            TEST_ASSERT(strstr(makefile_text, "P_GEN = printf") != NULL);
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
            TEST_ASSERT(strstr(makefile_text, "OBJS_SHELL = ") == NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_SHELL = ") == NULL);
            TEST_ASSERT(strstr(makefile_text, "COMMA = ,") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_DISASM = $(filter-out -Wl$(COMMA)--strip-all,$(LDFLAGS))") != NULL);
            TEST_ASSERT(strstr(makefile_text, "LDFLAGS_DISASM_SHELL = ") == NULL);
            TEST_ASSERT(strstr(makefile_text, "IR = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "DEBUG_INFO = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(LINKED_ASM)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DISASM_TARGET)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DISASM)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(IR)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "EXTRA_TARGETS += $(DEBUG_INFO)") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -Wl,--lto-emit-asm") != NULL);
            TEST_ASSERT(strstr(makefile_text, "$(CC) $(OBJS) -o \"$@\" $(LDFLAGS) -Wl,--lto-emit-asm") != NULL);
            TEST_ASSERT(strstr(makefile_text, "$(CC) $(OBJS) -o \"$@\" $(LDFLAGS_DISASM)") != NULL);
            TEST_ASSERT(strstr(makefile_text, "llvm-objdump -d --demangle --line-numbers --symbolize-operands --no-show-raw-insn -s \"$(DISASM_INPUT)\" > \"$@\"") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -S -emit-llvm \"$(firstword $(SRCS))\" -o \"$@\"") != NULL);
#ifdef _WIN32
            TEST_ASSERT(strstr(makefile_text, "PDB = ") != NULL);
            TEST_ASSERT(strstr(makefile_text, "llvm-pdbutil dump -symbols -globals -publics \"$(PDB)\" > \"$@\"") != NULL);
#else
            TEST_ASSERT(strstr(makefile_text, "llvm-dwarfdump --debug-info --debug-line \"$(TARGET)\" > \"$@\"") != NULL);
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
            TEST_ASSERT(strstr(makefile_text, "P_GEN = printf") != NULL);
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

static void test_pch_dependency_invalidates_linked_plan(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "pch-deps-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* include_dir = path_join(project_root, "include");
    char* source_dir = path_join(project_root, "source");
    char* pch_header = path_join(include_dir, "dh.h");
    char* pch_dep_header = path_join(include_dir, "dep.h");
    char* source = path_join(source_dir, "main.c");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(include_dir != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(pch_header != NULL);
    TEST_ASSERT(pch_dep_header != NULL);
    TEST_ASSERT(source != NULL);
    TEST_ASSERT(dir_createRecur(include_dir));
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(project_dh, "output=pch-deps\nlink-dsl=off\npch=dh.h\n"));
    TEST_ASSERT(file_write(pch_header, "#pragma once\n#include \"dep.h\"\n#define PCH_DEPS 1\n"));
    TEST_ASSERT(file_write(pch_dep_header, "#pragma once\n#define PCH_DEP_VALUE 1\n"));
    TEST_ASSERT(file_write(source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header != NULL);

    ArrStr* sources = dal_c__collectSourceFiles(proj, NULL);
    TEST_ASSERT(sources != NULL);
    TEST_ASSERT(ArrStr_len(sources) == 1);

    const char* argv[] = { dal_c_tool_name, "build", "release", "--link-dsl=off", NULL };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
    TEST_ASSERT(cmd != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    TEST_ASSERT(profile != NULL);

    char* build_dir = dal_c_Project_getBuildDir(proj);
    char* profile_dir = path_join(build_dir, profile->name);
    char* object_dir = path_join(profile_dir, "obj");
    char* target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, proj->defaults.output_name, dal_c_Target_executable);
    char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_executable);
    TEST_ASSERT(build_dir != NULL);
    TEST_ASSERT(profile_dir != NULL);
    TEST_ASSERT(object_dir != NULL);
    TEST_ASSERT(target_path != NULL);
    TEST_ASSERT(makefile_path != NULL);
    TEST_ASSERT(dir_createRecur(object_dir));

    TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == dal_c_generateMakefile_success);
    char* makefile_text = file_read(makefile_path);
    TEST_ASSERT(makefile_text != NULL);
    TEST_ASSERT(strstr(makefile_text, "PCH_OUT = ") != NULL);
    TEST_ASSERT(strstr(makefile_text, "PCH_DEP = ") != NULL);
    TEST_ASSERT(strstr(makefile_text, ".tmp.$$$$") != NULL);
    TEST_ASSERT(strstr(makefile_text, "-MQ \"$(PCH_OUT)\"") != NULL);
    TEST_ASSERT(strstr(makefile_text, "-MQ \"$@\"") != NULL);
    TEST_ASSERT(strstr(makefile_text, "-include $(PCH_DEP)") != NULL);

    char* obj_path = test_makefile_var_first_value(makefile_text, "OBJS");
    TEST_ASSERT(obj_path != NULL);
    char* obj_dep_path = strdup(obj_path);
    TEST_ASSERT(obj_dep_path != NULL);
    char* obj_ext = strrchr(obj_dep_path, '.');
    TEST_ASSERT(obj_ext != NULL && str_eql(obj_ext, ".o"));
    strcpy(obj_ext, ".d");

    char* target_parent = path_parent(target_path);
    char* obj_parent = path_parent(obj_path);
    TEST_ASSERT(target_parent != NULL);
    TEST_ASSERT(obj_parent != NULL);
    TEST_ASSERT(dir_createRecur(target_parent));
    TEST_ASSERT(dir_createRecur(obj_parent));
    TEST_ASSERT(file_write(obj_path, "fake object\n"));
    char* obj_dep_text = str_format("%s: %s\n", obj_path, source);
    TEST_ASSERT(obj_dep_text != NULL);
    TEST_ASSERT(file_write(obj_dep_path, obj_dep_text));
    TEST_ASSERT(file_write(target_path, "fake executable\n"));

    int plan_result = dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable);
    TEST_ASSERT(plan_result == dal_c_generateMakefile_success);

    free(obj_dep_text);
    free(obj_parent);
    free(target_parent);
    free(obj_dep_path);
    free(obj_path);
    free(makefile_text);
    free(makefile_path);
    free(target_path);
    free(object_dir);
    free(profile_dir);
    free(build_dir);
    dal_c_Cmd_cleanup(&cmd);
    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&proj);
    free(source);
    free(pch_dep_header);
    free(pch_header);
    free(source_dir);
    free(include_dir);
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

static void test_clean_prefers_local_build_dir(void) {
    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "clean-local-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* local_dir = path_join(project_root, "lab/drafts");
    char* project_build_dev = path_join(project_root, "build/dev");
    char* local_build_dev = path_join(local_dir, "build/dev");

    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(local_dir != NULL);
    TEST_ASSERT(project_build_dev != NULL);
    TEST_ASSERT(local_build_dev != NULL);

    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(dir_createRecur(local_dir));
    TEST_ASSERT(file_write(project_dh, "output=clean-local-project\n"));
    TEST_ASSERT(dir_createRecur(project_build_dev));
    TEST_ASSERT(dir_createRecur(local_build_dev));
    TEST_ASSERT(env_setCWD(local_dir));

    const char* argv[] = { dal_c_tool_name, "clean", "dev", NULL };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
    TEST_ASSERT(cmd != NULL);
    dal_c_Project* proj = dal_c_Project_detect(cmd);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->root != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(cmd, proj) == 0);
    TEST_ASSERT(!path_exists(local_build_dev));
    TEST_ASSERT(path_isDir(project_build_dev));

    dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    TEST_ASSERT(env_setCWD(original_cwd));
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(local_build_dev);
    free(project_build_dev);
    free(local_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
    free(original_cwd);
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

static void test_explicit_file_build_uses_file_project(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "tic-tac-toe");
    char* project_dh = path_join(project_root, "project.dh");
    char* archive_dir = path_join(project_root, "archive");
    char* source = path_join(archive_dir, "ttt-origin.c");
    char* samples_dir = path_join(project_root, "samples");
    char* examples_dir = path_join(project_root, "examples");
    char* tests_dir = path_join(project_root, "tests");
    char* sample_source = path_join(samples_dir, "sample-main.c");
    char* example_source = path_join(examples_dir, "example-main.c");
    char* test_source = path_join(tests_dir, "test-main.c");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(archive_dir != NULL);
    TEST_ASSERT(source != NULL);
    TEST_ASSERT(samples_dir != NULL);
    TEST_ASSERT(examples_dir != NULL);
    TEST_ASSERT(tests_dir != NULL);
    TEST_ASSERT(sample_source != NULL);
    TEST_ASSERT(example_source != NULL);
    TEST_ASSERT(test_source != NULL);
    TEST_ASSERT(dir_createRecur(archive_dir));
    TEST_ASSERT(dir_createRecur(samples_dir));
    TEST_ASSERT(dir_createRecur(examples_dir));
    TEST_ASSERT(dir_createRecur(tests_dir));
    TEST_ASSERT(file_write(project_dh, "output=tic-tac-toe\n"));
    TEST_ASSERT(file_write(source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(sample_source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(example_source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(test_source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    const char* argv[] = { dal_c_tool_name, "build", "dev", source, NULL };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
    TEST_ASSERT(cmd != NULL);

    dal_c_CommandIntent intent = { 0 };
    dal_c_Cmd_normalizeIntent(cmd, &intent);
    TEST_ASSERT(intent.target_path_is_explicit_file);

    dal_c_TargetRequest request = { 0 };
    TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
    TEST_ASSERT(request.root == NULL);
    dal_c_TargetRequest_cleanup(&request);

    {
        const char* sample_argv[] = { dal_c_tool_name, "build", "--sample", sample_source, NULL };
        dal_c_Cmd* sample_cmd = dal_c_Cmd_parse(4, sample_argv);
        TEST_ASSERT(sample_cmd != NULL);
        dal_c_Cmd_normalizeIntent(sample_cmd, &intent);
        TEST_ASSERT(!intent.target_path_is_explicit_file);
        TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
        TEST_ASSERT(request.root != NULL);
        TEST_ASSERT(str_eql(request.root->name, dal_c_dir_samples));
        dal_c_TargetRequest_cleanup(&request);
        dal_c_Cmd_cleanup(&sample_cmd);
    }

    {
        const char* example_argv[] = { dal_c_tool_name, "build", "--example", example_source, NULL };
        dal_c_Cmd* example_cmd = dal_c_Cmd_parse(4, example_argv);
        TEST_ASSERT(example_cmd != NULL);
        dal_c_Cmd_normalizeIntent(example_cmd, &intent);
        TEST_ASSERT(!intent.target_path_is_explicit_file);
        TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
        TEST_ASSERT(request.root != NULL);
        TEST_ASSERT(str_eql(request.root->name, dal_c_dir_examples));
        dal_c_TargetRequest_cleanup(&request);
        dal_c_Cmd_cleanup(&example_cmd);
    }

    {
        const char* example_test_argv[] = { dal_c_tool_name, "test", "--example", "example-main.c", NULL };
        dal_c_Cmd* example_test_cmd = dal_c_Cmd_parse(4, example_test_argv);
        TEST_ASSERT(example_test_cmd != NULL);
        TEST_ASSERT(example_test_cmd->payload.test.sample_dir == dal_c_SampleDir_examples);
        dal_c_Cmd_normalizeIntent(example_test_cmd, &intent);
        TEST_ASSERT(!intent.target_path_is_explicit_file);
        TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
        TEST_ASSERT(request.root != NULL);
        TEST_ASSERT(str_eql(request.root->name, dal_c_dir_examples));
        TEST_ASSERT(str_eql(request.relative_path, "example-main.c"));
        TEST_ASSERT(path_isFile(request.resolved_path));
        dal_c_TargetRequest_cleanup(&request);
        dal_c_Cmd_cleanup(&example_test_cmd);
    }

    {
        const char* test_argv[] = { dal_c_tool_name, "build", "--test", test_source, NULL };
        dal_c_Cmd* test_cmd = dal_c_Cmd_parse(4, test_argv);
        TEST_ASSERT(test_cmd != NULL);
        dal_c_Cmd_normalizeIntent(test_cmd, &intent);
        TEST_ASSERT(!intent.target_path_is_explicit_file);
        TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
        TEST_ASSERT(request.root != NULL);
        TEST_ASSERT(str_eql(request.root->name, dal_c_dir_tests));
        dal_c_TargetRequest_cleanup(&request);
        dal_c_Cmd_cleanup(&test_cmd);
    }

    dal_c_Project* file_proj = dal_c_Project_detectAt(archive_dir, proj->dh_path);
    TEST_ASSERT(file_proj != NULL);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    TEST_ASSERT(profile != NULL);
    char* build_dir = dal_c_Project_getBuildDir(file_proj);
    char* profile_dir = path_join(build_dir, profile->name);
    char* target_path = dal_c__resolveOutputPath(file_proj, cmd, profile_dir, "ttt-origin", dal_c_Target_executable);
    char* target_parent = path_parent(target_path);
    char* target_name = path_basename(target_path);
    TEST_ASSERT(build_dir != NULL);
    TEST_ASSERT(profile_dir != NULL);
    TEST_ASSERT(target_path != NULL);
    TEST_ASSERT(target_parent != NULL);
    TEST_ASSERT(target_name != NULL);
    TEST_ASSERT(str_eql(target_parent, profile_dir));
#ifdef _WIN32
    TEST_ASSERT(str_eql(target_name, "ttt-origin.exe"));
#else
    TEST_ASSERT(str_eql(target_name, "ttt-origin"));
#endif

    free(target_name);
    free(target_parent);
    free(target_path);
    free(profile_dir);
    free(build_dir);
    dal_c_Project_cleanup(&file_proj);
    dal_c_Cmd_cleanup(&cmd);
    dal_c_Project_cleanup(&proj);
    free(test_source);
    free(example_source);
    free(sample_source);
    free(tests_dir);
    free(examples_dir);
    free(samples_dir);
    free(source);
    free(archive_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_target_root_directory_uses_local_include(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "target-local-include-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* demo_project_dh = path_join(project_root, "examples/demo/project.dh");
    char* demo_src_dir = path_join(project_root, "examples/demo/src");
    char* demo_include_dir = path_join(project_root, "examples/demo/include");
    char* demo_header = path_join(demo_include_dir, "demo.h");
    char* demo_source = path_join(demo_src_dir, "main.c");
    char* demo_output_dir = path_join(project_root, "build/dev/examples/demo");
    char* compile_db_path = path_join(project_root, "build/clangd/compile_commands.json");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(demo_project_dh != NULL);
    TEST_ASSERT(demo_src_dir != NULL);
    TEST_ASSERT(demo_include_dir != NULL);
    TEST_ASSERT(demo_header != NULL);
    TEST_ASSERT(demo_source != NULL);
    TEST_ASSERT(demo_output_dir != NULL);
    TEST_ASSERT(compile_db_path != NULL);

    TEST_ASSERT(dir_createRecur(demo_src_dir));
    TEST_ASSERT(dir_createRecur(demo_include_dir));
    TEST_ASSERT(file_write(project_dh, "output=parent-output\nlink-dsl=off\npch=off\n"));
    TEST_ASSERT(file_write(demo_project_dh, "output=demo-output\n"));
    TEST_ASSERT(file_write(demo_header, "#pragma once\n#define DEMO_VALUE 7\n"));
    TEST_ASSERT(file_write(demo_source, "#include \"demo.h\"\nint main(void) { return DEMO_VALUE == 7 ? 0 : 1; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    {
        const char* build_argv[] = { dal_c_tool_name, "build", "dev", "--example", "demo", NULL };
        dal_c_Cmd* build_cmd = dal_c_Cmd_parse(5, build_argv);
        TEST_ASSERT(build_cmd != NULL);
        TEST_ASSERT(dal_c_Cmd_makeTarget(build_cmd, proj) == 0);
        dal_c_Cmd_cleanup(&build_cmd);
    }
    char* demo_output = path_join(demo_output_dir,
#ifdef _WIN32
                                  "demo-output.exe"
#else
                                  "demo-output"
#endif
    );
    TEST_ASSERT(demo_output != NULL);
    TEST_ASSERT(path_isFile(demo_output));

    {
        const char* compile_db_argv[] = {
            dal_c_tool_name,
            "compile-db",
            "dev",
            "--example",
            "demo",
            "--output",
            compile_db_path,
            NULL
        };
        dal_c_Cmd* compile_db_cmd = dal_c_Cmd_parse(7, compile_db_argv);
        TEST_ASSERT(compile_db_cmd != NULL);
        TEST_ASSERT(dal_c_Cmd_writeCompileDb(compile_db_cmd, proj) == 0);
        dal_c_Cmd_cleanup(&compile_db_cmd);
    }

    char* compile_db = file_read(compile_db_path);
    TEST_ASSERT(compile_db != NULL);
    TEST_ASSERT(strstr(compile_db, "examples/demo/include") != NULL || strstr(compile_db, "examples\\\\demo\\\\include") != NULL || strstr(compile_db, "examples\\\\\\\\demo\\\\\\\\include") != NULL);

    free(compile_db);
    free(demo_output);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(compile_db_path);
    free(demo_output_dir);
    free(demo_source);
    free(demo_header);
    free(demo_include_dir);
    free(demo_src_dir);
    free(demo_project_dh);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_syntax_arguments_follow_build_compile_contract(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dir_createRecur(temp_root));

    char* project_root = path_join(temp_root, "syntax-contract-project");
    char* project_dh = path_join(project_root, dal_c_file_detector_project);
    char* source_dir = path_join(project_root, "src");
    char* source = path_join(source_dir, "main.c");
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(source != NULL);
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(project_dh, "output=syntax-contract-project\nlink-dsl=off\n"));
    TEST_ASSERT(file_write(source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    const char* argv[] = {
        dal_c_tool_name,
        "syntax",
        "release",
        "--freestanding",
        "--link-stdlib=off",
        "--target-tune=generic",
        "--exceptions=off",
        "--merge-all-constants=on",
        "--stack-protector=off",
        "--version-core=1.2.3",
        "--comp-args=-Wno-unused",
        "--jobs=3",
        NULL
    };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(12, argv);
    TEST_ASSERT(cmd != NULL);
    TEST_ASSERT(dal_c__parallelJobCount(cmd) == 3);
    dal_c_CompilerOpts_merge(&cmd->opts, &proj->opts);

    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    TEST_ASSERT(profile != NULL);

    ArrStr* args = ArrStr_init();
    dal_c__appendSyntaxArguments(args, cmd, proj, profile, source, dal_c_Target_executable);

    const char* opt_flag = dal_c_OptiLevel_toFlag(profile->opti_level);
    TEST_ASSERT(test_arrstr_contains(args, "-DCOMP"));
    TEST_ASSERT(test_arrstr_contains(args, "-DCOMP_FREESTANDING"));
    TEST_ASSERT(test_arrstr_contains(args, "-DCOMP_NO_STDLIB"));
    TEST_ASSERT(test_arrstr_contains(args, "-ffreestanding"));
    TEST_ASSERT(test_arrstr_contains(args, "-mtune=generic"));
    TEST_ASSERT(test_arrstr_contains(args, "-fno-exceptions"));
    TEST_ASSERT(test_arrstr_contains(args, "-fmerge-all-constants"));
    TEST_ASSERT(test_arrstr_contains(args, "-fno-stack-protector"));
    TEST_ASSERT(test_arrstr_contains(args, "-Ddal_c__NUM__VER_CORE_MAJOR=1"));
    TEST_ASSERT(test_arrstr_contains(args, "-Ddal_c__NUM__VER_CORE_MINOR=2"));
    TEST_ASSERT(test_arrstr_contains(args, "-Ddal_c__NUM__VER_CORE_PATCH=3"));
    TEST_ASSERT(test_arrstr_contains(args, "-Wno-unused"));
    TEST_ASSERT(test_arrstr_contains(args, source));
    if (opt_flag) {
        TEST_ASSERT(test_arrstr_contains(args, opt_flag));
    }
    if (!profile->debug_assertions) {
        TEST_ASSERT(test_arrstr_contains(args, "-DNDEBUG"));
    }
    TEST_ASSERT(!test_arrstr_contains(args, "-fsyntax-only"));

    ArrStr_fini(&args);
    dal_c_Cmd_cleanup(&cmd);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(source);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_compile_db_command(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "compile-db-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* source_dir = path_join(project_root, "src");
    char* include_dir = path_join(project_root, "include");
    char* samples_dir = path_join(project_root, "samples");
    char* header = path_join(include_dir, "app.h");
    char* source = path_join(source_dir, "main.c");
    char* sample_source = path_join(samples_dir, "sample-main.c");
    char* one_off_source = path_join(source_dir, "one-off.c");
    char* deps_header = path_join(project_root, "lib/deps.h");
    char* nested_deps_header = path_join(project_root, "lib/deps/deps.h");
    char* output_path = path_join(project_root, "build/clangd/compile_commands.json");
    char* auto_output_path = path_join(project_root, "compile_commands.json");

    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(include_dir != NULL);
    TEST_ASSERT(samples_dir != NULL);
    TEST_ASSERT(header != NULL);
    TEST_ASSERT(source != NULL);
    TEST_ASSERT(sample_source != NULL);
    TEST_ASSERT(one_off_source != NULL);
    TEST_ASSERT(deps_header != NULL);
    TEST_ASSERT(nested_deps_header != NULL);
    TEST_ASSERT(output_path != NULL);
    TEST_ASSERT(auto_output_path != NULL);

    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(dir_createRecur(include_dir));
    TEST_ASSERT(dir_createRecur(samples_dir));
    TEST_ASSERT(file_write(project_dh, "output=compile-db-project\nlink-dsl=off\n"));
    TEST_ASSERT(file_write(header, "#pragma once\n#define APP_HEADER 1\n"));
    TEST_ASSERT(file_write(source, "#include \"app.h\"\nint main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(sample_source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    const char* argv[] = {
        dal_c_tool_name,
        "compile-db",
        "dev",
        "--output",
        output_path,
        "--define=APP=1",
        "--target-tune=generic",
        "--exceptions=off",
        "--comp-args=-Wextra \"-DQUOTED=A B\"",
        NULL
    };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(9, argv);
    TEST_ASSERT(cmd != NULL);
    TEST_ASSERT(dal_c_Cmd_writeCompileDb(cmd, proj) == 0);
    TEST_ASSERT(path_isFile(output_path));

    char* json = file_read(output_path);
    TEST_ASSERT(json != NULL);
    TEST_ASSERT(strstr(json, "\"directory\"") != NULL);
    TEST_ASSERT(strstr(json, "\"file\"") != NULL);
    TEST_ASSERT(strstr(json, "\"arguments\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-fsyntax-only\"") == NULL);
    TEST_ASSERT(strstr(json, "\"-DCOMP\"") == NULL);
    TEST_ASSERT(strstr(json, "\"-Werror=all\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-Werror=strict-prototypes\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-mtune=generic\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-fno-exceptions\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-DAPP=1\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-Wextra\"") != NULL);
    TEST_ASSERT(strstr(json, "\"-DQUOTED=A B\"") != NULL);
    TEST_ASSERT(strstr(json, "main.c") != NULL);

    free(json);
    dal_c_Cmd_cleanup(&cmd);

    {
        const char* remove_argv[] = {
            dal_c_tool_name,
            "compile-db",
            "--remove",
            "--output",
            output_path,
            NULL
        };
        dal_c_Cmd* remove_cmd = dal_c_Cmd_parse(5, remove_argv);
        TEST_ASSERT(remove_cmd != NULL);
        TEST_ASSERT(dal_c_Cmd_writeCompileDb(remove_cmd, proj) == 0);
        TEST_ASSERT(!path_exists(output_path));
        dal_c_Cmd_cleanup(&remove_cmd);
    }

    const char* build_argv[] = { dal_c_tool_name, "build", "dev", NULL };
    dal_c_Cmd* build_cmd = dal_c_Cmd_parse(3, build_argv);
    TEST_ASSERT(build_cmd != NULL);
    TEST_ASSERT(dal_c_Cmd_makeTarget(build_cmd, proj) == 0);
    TEST_ASSERT(!path_exists(deps_header));
    TEST_ASSERT(!path_exists(nested_deps_header));
    TEST_ASSERT(!path_exists(auto_output_path));

    {
        const char* sample_argv[] = { dal_c_tool_name, "build", "dev", "--sample", sample_source, NULL };
        dal_c_Cmd* sample_cmd = dal_c_Cmd_parse(5, sample_argv);
        TEST_ASSERT(sample_cmd != NULL);
        TEST_ASSERT(dal_c_Cmd_makeTarget(sample_cmd, proj) == 0);
        dal_c_Cmd_cleanup(&sample_cmd);
        TEST_ASSERT(!path_exists(auto_output_path));
    }

    TEST_ASSERT(file_write(one_off_source, "int main(void) { return 0; }\n"));
    {
        const char* file_argv[] = { dal_c_tool_name, "build", "dev", one_off_source, NULL };
        dal_c_Cmd* file_cmd = dal_c_Cmd_parse(4, file_argv);
        TEST_ASSERT(file_cmd != NULL);
        TEST_ASSERT(dal_c_Cmd_makeTarget(file_cmd, proj) == 0);
        dal_c_Cmd_cleanup(&file_cmd);
        TEST_ASSERT(!path_exists(auto_output_path));
    }

    dal_c_Cmd_cleanup(&build_cmd);
    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(auto_output_path);
    free(output_path);
    free(nested_deps_header);
    free(deps_header);
    free(one_off_source);
    free(sample_source);
    free(source);
    free(header);
    free(samples_dir);
    free(include_dir);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_deps_prelude_tracks_dh_contract(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* dh_root = test_repo_path("dh");
    char* project_root = path_join(temp_root, "dh-prelude-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* deps_header = path_join(project_root, "lib/deps.h");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dh_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(deps_header != NULL);
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh, "output=dh-prelude\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, dh_root);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header != NULL);
    TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &proj->opts));
    TEST_ASSERT(path_isFile(deps_header));

    char* deps_text = file_read(deps_header);
    TEST_ASSERT(deps_text != NULL);
    TEST_ASSERT(strstr(deps_text, "#include <dh-bundle.h>") != NULL);
    free(deps_text);
    dal_c_Project_cleanup(&proj);

    TEST_ASSERT(dir_removeRecur(project_root));
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh, "output=dh-prelude\nlink-dsl=off\n"));
    proj = dal_c_Project_detectAt(project_root, dh_root);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header == NULL);
    TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &proj->opts));
    TEST_ASSERT(!path_exists(deps_header));

    dal_c_Project_cleanup(&proj);
    (void)dir_removeRecur(temp_root);
    TEST_ASSERT(!path_exists(temp_root));

    free(deps_header);
    free(project_dh);
    free(project_root);
    free(dh_root);
    free(temp_root);
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

static void test_test_mode_is_command_scoped(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "command-scoped-test-mode");
    char* project_dh = path_join(project_root, "project.dh");
    char* examples_dir = path_join(project_root, "examples");
    char* source = path_join(examples_dir, "example-tetris.c");
    char* source_fixture = test_repo_path("dh-examples/daterm/examples/example-tetris.c");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(examples_dir != NULL);
    TEST_ASSERT(source != NULL);
    TEST_ASSERT(source_fixture != NULL);
    TEST_ASSERT(dir_createRecur(examples_dir));
    TEST_ASSERT(file_write(project_dh, "pch=off\nlink-dsl=off\n"));
    TEST_ASSERT(file_copy(source_fixture, source));
    TEST_ASSERT(dal_c__hasTestRegistration(source));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    const char* test_argv[] = { dal_c_tool_name, "test", "--example", "example-tetris.c", NULL };
    const char* build_argv[] = { dal_c_tool_name, "build", "--example", "example-tetris.c", NULL };
    dal_c_Cmd* test_cmd = dal_c_Cmd_parse(4, test_argv);
    dal_c_Cmd* build_cmd = dal_c_Cmd_parse(4, build_argv);
    TEST_ASSERT(test_cmd != NULL);
    TEST_ASSERT(build_cmd != NULL);

    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(test_cmd->opts.profile);
    TEST_ASSERT(profile != NULL);
    char* build_dir = dal_c_Project_getBuildDir(proj);
    char* profile_dir = path_join(build_dir, profile->name);
    char* object_dir = path_join(profile_dir, "obj");
    char* target_path = dal_c__resolveOutputPath(proj, test_cmd, profile_dir, "example-tetris", dal_c_Target_executable);
    char* test_plan_path = dal_c__makePlanFilePath(proj, profile, test_cmd, target_path, dal_c_Target_executable);
    char* build_plan_path = dal_c__makePlanFilePath(proj, profile, build_cmd, target_path, dal_c_Target_executable);
    ArrStr* sources = ArrStr_init();
    ArrStr_push(sources, source);
    TEST_ASSERT(build_dir != NULL);
    TEST_ASSERT(profile_dir != NULL);
    TEST_ASSERT(object_dir != NULL);
    TEST_ASSERT(target_path != NULL);
    TEST_ASSERT(test_plan_path != NULL);
    TEST_ASSERT(build_plan_path != NULL);
    TEST_ASSERT(dir_createRecur(object_dir));

    TEST_ASSERT(dal_c__generateMakefile(test_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
    char* test_plan = file_read(test_plan_path);
    char* link_dir = path_join(object_dir, ".link");
    int contract_count = 0;
    char** contract_paths = dir_listRecur(link_dir, &contract_count);
    TEST_ASSERT(test_plan != NULL);
    TEST_ASSERT(strstr(test_plan, " -DCOMP_TEST") != NULL);
    TEST_ASSERT(contract_count == 1);
    char* test_contract = file_read(contract_paths[0]);
    TEST_ASSERT(test_contract != NULL);

    TEST_ASSERT(dal_c__generateMakefile(build_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
    char* build_plan = file_read(build_plan_path);
    char* build_contract = file_read(contract_paths[0]);
    TEST_ASSERT(build_plan != NULL);
    TEST_ASSERT(build_contract != NULL);
    TEST_ASSERT(strstr(build_plan, " -DCOMP_TEST") == NULL);
    TEST_ASSERT(!str_eql(test_contract, build_contract));

    free(build_contract);
    free(build_plan);
    free(test_contract);
    test_free_str_array(contract_paths, contract_count);
    free(link_dir);
    free(test_plan);
    ArrStr_fini(&sources);
    free(build_plan_path);
    free(test_plan_path);
    free(target_path);
    free(object_dir);
    free(profile_dir);
    free(build_dir);
    dal_c_Cmd_cleanup(&build_cmd);
    dal_c_Cmd_cleanup(&test_cmd);
    dal_c_Project_cleanup(&proj);
    free(source_fixture);
    free(source);
    free(examples_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
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
