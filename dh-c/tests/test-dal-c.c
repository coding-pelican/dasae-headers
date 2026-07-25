#include "dal-c.h"
#include "dal-c/internal.h"
#include "dal-c-ext/ArrStr.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/file.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/str.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

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
static void test_workspace_cache_scope(void);
static void test_dependency_lock_contract(void);
static void test_prebuilt_dependency_staging(void);
static void test_clean_prefers_local_build_dir(void);
static void test_clean_profile_removes_dependency_exports(void);
static void test_clean_cache_scope(void);
static void test_clean_unused_dependencies(void);
static void test_target_request_resolution(void);
static void test_output_override_generates_target_extensions(void);
static void test_output_ext_does_not_rewrite_dependency_artifacts(void);
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
static bool test_remove_recur(const char* path);
static void test_reset_temp_root(void);
static void test_free_str_array(char** items, int count);
static char* test_makefile_var_first_value(const char* makefile_text, const char* var_name);
static bool test_arrstr_contains(ArrStr* items, const char* value);
static bool test_path_text_eql(const char* lhs, const char* rhs);
static const dal_c_HelpProfile* test_find_help_profile(const char* name);
static const dal_c_HelpCmd* test_find_help_cmd(const char* name, int* count_out);
static bool test_help_has_option(const dal_c_HelpCmd* cmd, const char* option_name);
static bool test_help_has_exact_option(const dal_c_HelpCmd* cmd, const char* option_name);
static bool test_help_has_note(const dal_c_HelpCmd* cmd, const char* text);

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
    RUN_TEST(test_workspace_cache_scope);
    RUN_TEST(test_dependency_lock_contract);
    RUN_TEST(test_prebuilt_dependency_staging);
    RUN_TEST(test_clean_prefers_local_build_dir);
    RUN_TEST(test_clean_profile_removes_dependency_exports);
    RUN_TEST(test_clean_cache_scope);
    RUN_TEST(test_clean_unused_dependencies);
    RUN_TEST(test_target_request_resolution);
    RUN_TEST(test_output_override_generates_target_extensions);
    RUN_TEST(test_output_ext_does_not_rewrite_dependency_artifacts);
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

static void test_sleep_remove_retry(void) {
#ifdef _WIN32
    Sleep(20);
#else
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 20 * 1000 * 1000 };
    (void)nanosleep(&ts, NULL);
#endif
}

static bool test_remove_recur(const char* path) {
    if (!path || !path_exists(path)) { return true; }
    for (int i = 0; i < 20; ++i) {
        if ((!path_isDir(path) || dir_removeRecur(path)) && !path_exists(path)) {
            return true;
        }
        test_sleep_remove_retry();
    }
    return !path_exists(path);
}

static void test_reset_temp_root(void) {
    char* temp_root = test_temp_root();
    (void)test_remove_recur(temp_root);
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

static bool test_path_text_eql(const char* lhs, const char* rhs) {
    if (!lhs || !rhs) { return false; }
    while (*lhs && *rhs) {
        char a = (*lhs == '\\') ? '/' : *lhs;
        char b = (*rhs == '\\') ? '/' : *rhs;
        if (a != b) { return false; }
        ++lhs;
        ++rhs;
    }
    return *lhs == '\0' && *rhs == '\0';
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
        if (cmd->options[i].name && strstr(cmd->options[i].name, option_name) != NULL) {
            return true;
        }
    }
    return false;
}

static bool test_help_has_exact_option(const dal_c_HelpCmd* cmd, const char* option_name) {
    if (!cmd || !option_name) {
        return false;
    }

    const size_t option_len = strlen(option_name);
    for (int i = 0; i < cmd->option_count; ++i) {
        const char* name = cmd->options[i].name;
        for (const char* p = strstr(name, option_name); p; p = strstr(p + 1, option_name)) {
            const bool starts_name = p == name || p[-1] == '-';
            const char next = p[option_len];
            const bool ends_name = next == '\0' || next == '=' || next == ' ' || next == ',' || next == '[';
            if (starts_name && ends_name) {
                return true;
            }
        }
    }
    return false;
}

static bool test_help_has_note(const dal_c_HelpCmd* cmd, const char* text) {
    if (!cmd || !text) {
        return false;
    }

    for (int i = 0; i < cmd->note_count; ++i) {
        if (cmd->notes[i] && strstr(cmd->notes[i], text) != NULL) {
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

    char* windows_lto = dal_c__makeLtoStaticLibraryPath("mylib.lib");
    TEST_ASSERT(str_eql(windows_lto, "mylib.lto.lib"));
    free(windows_lto);

    char* unix_lto = dal_c__makeLtoStaticLibraryPath("libmylib.a");
    TEST_ASSERT(str_eql(unix_lto, "libmylib.lto.a"));
    free(unix_lto);

    char* import_lib = dal_c__makeSharedImportLibraryPath("mylib.dll");
    TEST_ASSERT(str_eql(import_lib, "mylib.dll.lib"));
    free(import_lib);
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

    char* link_target = path_join(temp_root, "link target");
    char* link_target_file = path_join(link_target, "preserved.txt");
    char* link_alias = path_join(temp_root, "link alias");
    TEST_ASSERT(link_target != NULL && link_target_file != NULL && link_alias != NULL);
    TEST_ASSERT(dir_createRecur(link_target));
    TEST_ASSERT(file_write(link_target_file, "preserve"));
    TEST_ASSERT(dir_linkDir(link_alias, link_target));
    TEST_ASSERT(path_isDir(link_alias));
    TEST_ASSERT(dir_removeRecur(link_alias));
    TEST_ASSERT(!path_isDir(link_alias));
    TEST_ASSERT(path_isFile(link_target_file));

    TEST_ASSERT(test_remove_recur(temp_root));

    free(link_alias);
    free(link_target_file);
    free(link_target);
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
    TEST_ASSERT(dal_c_PrebuiltMode_parse("auto") == dal_c_PrebuiltMode_auto);
    TEST_ASSERT(dal_c_PrebuiltMode_parse("off") == dal_c_PrebuiltMode_off);
    TEST_ASSERT(dal_c_PrebuiltMode_parse("required") == dal_c_PrebuiltMode_required);
    TEST_ASSERT(str_eql(dal_c_PrebuiltMode_format(dal_c_PrebuiltMode_required), "required"));
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("warn") == dal_c_LooseErrorsMode_warn);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("suppress") == dal_c_LooseErrorsMode_suppress);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("auto") == dal_c_LooseErrorsMode_auto);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("never") == dal_c_LooseErrorsMode_never);
    TEST_ASSERT(dal_c_LooseErrorsMode_parse("off") == dal_c_LooseErrorsMode_strict);

    const dal_c_ProfileSpec* release_spec = dal_c_ProfileSpec_by(dal_c_Profile_release);
    TEST_ASSERT(release_spec != NULL);
    TEST_ASSERT(str_eql(release_spec->name, dal_c_profile_release));
    TEST_ASSERT(release_spec->opti_level == dal_c_OptiLevel_aggressive);
    TEST_ASSERT(release_spec->lto_mode == dal_c_LtoMode_thin);
    TEST_ASSERT(release_spec->exceptions == dal_c_ToggleState_disabled);
    TEST_ASSERT(release_spec->unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(release_spec->async_unwind_tables == dal_c_ToggleState_disabled);
    TEST_ASSERT(release_spec->icf_mode == dal_c_IcfMode_safe);

    const dal_c_ProfileSpec* stable_spec = dal_c_ProfileSpec_by(dal_c_Profile_stable);
    TEST_ASSERT(stable_spec != NULL);
    TEST_ASSERT(stable_spec->lto_mode == dal_c_LtoMode_thin);

    const dal_c_ProfileSpec* compact_spec = dal_c_ProfileSpec_by(dal_c_Profile_compact);
    TEST_ASSERT(compact_spec != NULL);
    TEST_ASSERT(compact_spec->lto_mode == dal_c_LtoMode_thin);

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
    TEST_ASSERT(optimize_spec->lto_mode == dal_c_LtoMode_full);
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
    TEST_ASSERT(micro_spec->lto_mode == dal_c_LtoMode_thin);
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
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_macro_backtrace_limit));
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
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_elapsed_precision));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_link_dir));
    TEST_ASSERT(test_help_has_option(build_cmd, dal_c_opt_output_ext));
    TEST_ASSERT(build_cmd->note_count > 0);
    TEST_ASSERT(test_help_has_note(build_cmd, "--output"));
    TEST_ASSERT(test_help_has_note(build_cmd, "--output-ext"));
    TEST_ASSERT(test_help_has_note(build_cmd, "--link-dir"));

    const dal_c_HelpCmd* workspace_cmd = test_find_help_cmd(dal_c_cmd_action_workspace, NULL);
    TEST_ASSERT(workspace_cmd != NULL);
    TEST_ASSERT(!workspace_cmd->implemented);

    const dal_c_HelpCmd* compile_db_cmd = test_find_help_cmd(dal_c_cmd_action_compile_db, NULL);
    TEST_ASSERT(compile_db_cmd != NULL);
    TEST_ASSERT(compile_db_cmd->implemented);
    TEST_ASSERT(test_help_has_option(compile_db_cmd, dal_c_opt_output));
    TEST_ASSERT(test_help_has_option(compile_db_cmd, dal_c_opt_macro_backtrace_limit));
    TEST_ASSERT(!test_help_has_option(compile_db_cmd, dal_c_opt_output_ext));
    TEST_ASSERT(test_help_has_note(compile_db_cmd, "does not compile"));

    const dal_c_HelpCmd* syntax_cmd = test_find_help_cmd(dal_c_cmd_action_syntax, NULL);
    TEST_ASSERT(syntax_cmd != NULL);
    TEST_ASSERT(syntax_cmd->implemented);
    TEST_ASSERT(test_help_has_option(syntax_cmd, dal_c_opt_progress));
    TEST_ASSERT(test_help_has_option(syntax_cmd, dal_c_opt_elapsed_precision));
    TEST_ASSERT(test_help_has_option(syntax_cmd, dal_c_opt_macro_backtrace_limit));
    TEST_ASSERT(!test_help_has_option(syntax_cmd, dal_c_opt_output));
    TEST_ASSERT(!test_help_has_exact_option(syntax_cmd, dal_c_opt_link));
    TEST_ASSERT(test_help_has_note(syntax_cmd, "never links"));

    const dal_c_HelpCmd* tidy_cmd = test_find_help_cmd(dal_c_cmd_action_tidy, NULL);
    TEST_ASSERT(tidy_cmd != NULL);
    TEST_ASSERT(tidy_cmd->implemented);
    TEST_ASSERT(test_help_has_option(tidy_cmd, dal_c_opt_commands));
    TEST_ASSERT(!test_help_has_exact_option(tidy_cmd, dal_c_opt_link));
    TEST_ASSERT(test_help_has_note(tidy_cmd, "never links"));

    const dal_c_HelpCmd* format_cmd = test_find_help_cmd(dal_c_cmd_action_format, NULL);
    TEST_ASSERT(format_cmd != NULL);
    TEST_ASSERT(format_cmd->implemented);
    TEST_ASSERT(test_help_has_option(format_cmd, dal_c_opt_verbose));
    TEST_ASSERT(!test_help_has_option(format_cmd, dal_c_opt_define));
    TEST_ASSERT(!test_help_has_option(format_cmd, dal_c_opt_output));
    TEST_ASSERT(test_help_has_note(format_cmd, "in place"));

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
        const char* argv[] = { dal_c_tool_name, "build", "stable", "--prebuilt=required", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.prebuilt_mode == dal_c_PrebuiltMode_required);
        TEST_ASSERT(cmd->opts.prebuilt_mode_set);
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
        const char* argv[] = { dal_c_tool_name, "clean", "--cache", "--older-than=30d", "--dry-run", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->payload.clean.cache_only);
        TEST_ASSERT(cmd->payload.clean.older_than_set);
        TEST_ASSERT(cmd->payload.clean.older_than_seconds == 30u * 24u * 60u * 60u);
        TEST_ASSERT(cmd->payload.clean.dry_run);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "--deps", "--unused", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->payload.clean.dependencies_only);
        TEST_ASSERT(cmd->payload.clean.unused_only);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "--unused", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "clean", "--older-than=30d", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--deps", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
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
        const char* argv[] = { dal_c_tool_name, "syntax", "dev", "--output=syntax.exe", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "syntax", "dev", "--output-ext=.pyd", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "tidy", "dev", "--link=python311", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "format", "dev", "--define=DEBUG", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd == NULL);
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
        const char* argv[] = { dal_c_tool_name, "build", "--comp-args=-Winvalid-offsetof", "--link-args=-pthread", "--output-ext=pyd", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(str_eql(cmd->compiler_args, "-Winvalid-offsetof"));
        TEST_ASSERT(str_eql(cmd->link_args, "-pthread"));
        TEST_ASSERT(str_eql(cmd->output_ext, "pyd"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        char* link_dir = test_repo_path("dh-c");
        TEST_ASSERT(link_dir != NULL);
        char* link_dir_opt = str_format("--link-dir=%s", link_dir);
        char* short_link_dir_opt = str_format("-L%s", link_dir);
        TEST_ASSERT(link_dir_opt != NULL);
        TEST_ASSERT(short_link_dir_opt != NULL);
        const char* argv[] = { dal_c_tool_name, "build", link_dir_opt, short_link_dir_opt, "--link=python311", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.link_dir_count == 2);
        TEST_ASSERT(str_eql(cmd->opts.link_dirs[0], link_dir));
        TEST_ASSERT(str_eql(cmd->opts.link_dirs[1], link_dir));
        TEST_ASSERT(cmd->opts.link_count == 1);
        TEST_ASSERT(str_eql(cmd->opts.link_libs[0], "python311"));
        dal_c_Cmd_cleanup(&cmd);
        free(short_link_dir_opt);
        free(link_dir_opt);
        free(link_dir);
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
        const char* argv[] = { dal_c_tool_name, "build", "--macro-backtrace-limit=short", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit_set);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit == dal_c_default_macro_backtrace_limit);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--macro-backtrace-limit=unlimited", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit_set);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit == 0);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--macro-backtrace-limit", "37", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit_set);
        TEST_ASSERT(cmd->opts.macro_backtrace_limit == 37);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--macro-backtrace-limit=-1", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--macro-backtrace-limit=invalid", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
    }


    {
        const char* argv[] = { dal_c_tool_name, "build", "--progress=hide", "--commands=show", "--verbose=off", "--jobs=3", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(6, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->action == dal_c_CmdAction_build);
        TEST_ASSERT(!cmd->show_progress);
        TEST_ASSERT(cmd->show_commands);
        TEST_ASSERT(!cmd->verbose);
        TEST_ASSERT(cmd->elapsed_precision == dal_c_default_elapsed_precision);
        TEST_ASSERT(str_eql(cmd->make_jobs, "3"));
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--elapsed-precision=0", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->elapsed_precision == 0);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "test", "--elapsed-precision", "4", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(cmd->elapsed_precision == 4);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "--elapsed-precision=10", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd == NULL);
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
    override.macro_backtrace_limit = 24;
    override.macro_backtrace_limit_set = true;
    dal_c_CompilerOpts_merge(&merged, &override);
    TEST_ASSERT(merged.compile_env == dal_c_CompileEnv_hosted);
    TEST_ASSERT(merged.libc_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.dsl_mode == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.default_libs_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.start_files_linked == dal_c_ToggleState_enabled);
    TEST_ASSERT(merged.loose_errors == dal_c_LooseErrorsMode_warn);
    TEST_ASSERT(merged.macro_backtrace_limit_set);
    TEST_ASSERT(merged.macro_backtrace_limit == 24);

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
        "macro-backtrace-limit=unlimited\n"
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
    TEST_ASSERT(file_opts.macro_backtrace_limit_set);
    TEST_ASSERT(file_opts.macro_backtrace_limit == 0);
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
    TEST_ASSERT(file_write(project_dh, "output=mode-contract\nversion-namespace=mode_contract\nversion-core=0.9.1\nversion-prefix=alpha\nversion-suffix=3\nversion-build=proj.7\n"));
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
            "--macro-backtrace-limit=37",
            "--emit-map=build/mode-contract.map",
            "--save-temps=obj",
            "--entry=custom_entry",
            NULL
        };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(22, argv);
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
        TEST_ASSERT(strstr(makefile_text, " -fmacro-backtrace-limit=37") != NULL);
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
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--freestanding", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
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
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_DEFAULT_LIBS") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_START_FILES") != NULL);
        TEST_ASSERT(strstr(makefile_text, "CFLAGS_BASE += -ffreestanding") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -nolibc") == NULL);
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
        int generate_result = dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable);
        if (dal_c__platformIsWindows()) {
            TEST_ASSERT(generate_result == 1);
        } else {
            TEST_ASSERT(generate_result == 0);
            makefile_text = file_read(makefile_path);
            TEST_ASSERT(makefile_text != NULL);
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_HOSTED") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") != NULL);
            TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") == NULL);
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
            TEST_ASSERT(strstr(makefile_text, " -nolibc") != NULL);
            TEST_ASSERT(strstr(makefile_text, " -nostdlib") == NULL);
            TEST_ASSERT(strstr(makefile_text, " -nodefaultlibs") == NULL);
            TEST_ASSERT(strstr(makefile_text, " -nostartfiles") == NULL);
            TEST_ASSERT(strstr(makefile_text, "--print-libgcc-file-name") == NULL);
        }

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
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__NUM__VER_CORE_MAJOR=0") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__NUM__VER_CORE_MINOR=9") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__NUM__VER_CORE_PATCH=1") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__NUM__VER_LABEL_PREFIX=0") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__STR__VER_LABEL_PREFIX=\\\"alpha\\\"") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__NUM__VER_LABEL_SUFFIX=3") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__STR__VER_LABEL_SUFFIX=\\\"3\\\"") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-Dmode_contract__STR__VER_BUILD=\\\"proj.7\\\"") != NULL);
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

    {
        const char* first_argv[] = {
            dal_c_tool_name,
            "lib",
            "dev",
            "--shared",
            "--link-stdlib=off",
            "--link-compiler-rt=off",
            "--link=customrt",
            "--link=user32",
            "--define=COMP_HAS_LIBC",
            "--define=COMP_HAS_STDLIB",
            NULL
        };
        const char* second_argv[] = {
            dal_c_tool_name,
            "lib",
            "dev",
            "--shared",
            "--link-stdlib=off",
            "--link-compiler-rt=off",
            "--link=customrt-v2",
            "--link=user32",
            "--define=COMP_HAS_LIBC",
            "--define=COMP_HAS_STDLIB",
            NULL
        };
        dal_c_Cmd* first_cmd = dal_c_Cmd_parse(10, first_argv);
        dal_c_Cmd* second_cmd = dal_c_Cmd_parse(10, second_argv);
        TEST_ASSERT(first_cmd != NULL);
        TEST_ASSERT(second_cmd != NULL);
        dal_c_CompilerOpts_merge(&first_cmd->opts, &proj->opts);
        dal_c_CompilerOpts_merge(&second_cmd->opts, &proj->opts);
        const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(first_cmd->opts.profile);
        char* build_dir = dal_c_Project_getBuildDir(proj);
        char* profile_dir = path_join(build_dir, profile->name);
        char* object_dir = path_join(profile_dir, "obj-runtime-contract");
        char* target_path = dal_c__resolveOutputPath(proj, first_cmd, profile_dir, "runtime-contract", dal_c_Target_shared_lib);
        TEST_ASSERT(profile != NULL);
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(target_path != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));
        TEST_ASSERT(dal_c__generateMakefile(first_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_shared_lib) == 0);
        char* link_dir = path_join(object_dir, ".link");
        int contract_count = 0;
        char** contracts = dir_listRecur(link_dir, &contract_count);
        TEST_ASSERT(contract_count == 1);
        char* first_contract = file_read(contracts[0]);
        TEST_ASSERT(first_contract != NULL);
        TEST_ASSERT(strstr(first_contract, "compile-env=hosted") != NULL);
        TEST_ASSERT(strstr(first_contract, "libc=off") != NULL);
        TEST_ASSERT(strstr(first_contract, "default-libs=off") != NULL);
        TEST_ASSERT(strstr(first_contract, "start-files=off") != NULL);
        TEST_ASSERT(strstr(first_contract, "link-lib.0=customrt") != NULL);
        TEST_ASSERT(strstr(first_contract, "link-lib.1=user32") != NULL);
        char* makefile_path = dal_c__makePlanFilePath(proj, profile, first_cmd, target_path, dal_c_Target_shared_lib);
        char* makefile_text = makefile_path ? file_read(makefile_path) : NULL;
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_LIBC") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_LIBC") == NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_HAS_STDLIB") != NULL);
        TEST_ASSERT(strstr(makefile_text, "-DCOMP_NO_STDLIB") == NULL);
        TEST_ASSERT(strstr(makefile_text, " -nostdlib") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -lcustomrt") != NULL);
        TEST_ASSERT(strstr(makefile_text, " -luser32") != NULL);
        free(makefile_text);
        free(makefile_path);

        TEST_ASSERT(dal_c__generateMakefile(second_cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_shared_lib) == 0);
        char* second_contract = file_read(contracts[0]);
        TEST_ASSERT(second_contract != NULL);
        TEST_ASSERT(strstr(second_contract, "link-lib.0=customrt-v2") != NULL);
        TEST_ASSERT(!str_eql(first_contract, second_contract));

        free(second_contract);
        free(first_contract);
        test_free_str_array(contracts, contract_count);
        free(link_dir);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&second_cmd);
        dal_c_Cmd_cleanup(&first_cmd);
    }

    ArrStr_fini(&sources);
    dal_c_Project_cleanup(&proj);
    TEST_ASSERT(test_remove_recur(temp_root));

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
    TEST_ASSERT(file_write(
        lib_kind_project_dh,
        "kind=lib\n"
        "output=core\n"
        "prebuilt=off\n"
        "[dep]\n"
        "path=../dep\n"
        "prebuilt=auto\n"
    ));

    dal_c_Project* lib_kind_proj = dal_c_Project_detectAt(lib_kind_root, NULL);
    TEST_ASSERT(lib_kind_proj != NULL);
    TEST_ASSERT(lib_kind_proj->defaults.target_kind_set);
    TEST_ASSERT(lib_kind_proj->defaults.target_kind == dal_c_Target_lib);
    TEST_ASSERT(lib_kind_proj->opts.prebuilt_mode_set);
    TEST_ASSERT(lib_kind_proj->opts.prebuilt_mode == dal_c_PrebuiltMode_off);
    TEST_ASSERT(lib_kind_proj->lib_count == 1);
    TEST_ASSERT(lib_kind_proj->libraries[0].opts.prebuilt_mode_set);
    TEST_ASSERT(lib_kind_proj->libraries[0].opts.prebuilt_mode == dal_c_PrebuiltMode_auto);
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

    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    temp_root = test_temp_root();
    char* workspace_root = path_join(temp_root, "workspace");
    char* dh_root = path_join(workspace_root, "dh");
    char* dh_include_dir = path_join(dh_root, "include");
    char* dh_include_child_dir = path_join(dh_include_dir, "dh");
    char* dh_src_dir = path_join(dh_root, "src/dh");
    char* dh_header = path_join(dh_include_dir, "dh.h");
    char* dh_main_header = path_join(dh_include_dir, "dh-main.h");
    char* child_project = path_join(workspace_root, "child-project");
    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(workspace_root != NULL);
    TEST_ASSERT(dh_root != NULL);
    TEST_ASSERT(dh_include_dir != NULL);
    TEST_ASSERT(dh_include_child_dir != NULL);
    TEST_ASSERT(dh_src_dir != NULL);
    TEST_ASSERT(dh_header != NULL);
    TEST_ASSERT(dh_main_header != NULL);
    TEST_ASSERT(child_project != NULL);
    TEST_ASSERT(dir_createRecur(workspace_root));
    TEST_ASSERT(dir_createRecur(dh_include_child_dir));
    TEST_ASSERT(dir_createRecur(dh_src_dir));
    TEST_ASSERT(file_write(dh_header, "\n"));
    TEST_ASSERT(file_write(dh_main_header, "\n"));
    TEST_ASSERT(env_setCWD(workspace_root));

    char* detected_dh = dal_c_Project_findDHInstallation(NULL);
    char* expected_dh = path_abs(dh_root);
    TEST_ASSERT(detected_dh != NULL);
    TEST_ASSERT(expected_dh != NULL);
    TEST_ASSERT(str_eql(detected_dh, expected_dh));

    const char* version_argv[] = { dal_c_tool_name, "--version", "--dh", dh_root, NULL };
    dal_c_Cmd* version_cmd = dal_c_Cmd_parse(4, version_argv);
    TEST_ASSERT(version_cmd != NULL);
    char* version_detected_dh = dal_c_Project_findDHInstallation(version_cmd);
    TEST_ASSERT(version_detected_dh != NULL);
    TEST_ASSERT(str_eql(version_detected_dh, expected_dh));
    free(version_detected_dh);
    dal_c_Cmd_cleanup(&version_cmd);

    {
        const char* relative_argv[] = { dal_c_tool_name, "--version", "--dh", "dh", NULL };
        dal_c_Cmd* relative_cmd = dal_c_Cmd_parse(4, relative_argv);
        TEST_ASSERT(relative_cmd != NULL);
        char* relative_detected_dh = dal_c_Project_findDHInstallation(relative_cmd);
        TEST_ASSERT(relative_detected_dh != NULL);
        TEST_ASSERT(str_eql(relative_detected_dh, expected_dh));
        free(relative_detected_dh);
        dal_c_Cmd_cleanup(&relative_cmd);
    }

    TEST_ASSERT(dir_createRecur(child_project));
    dal_c_Project* child_proj = dal_c_Project_detectAt(child_project, "dh");
    TEST_ASSERT(child_proj != NULL);
    TEST_ASSERT(child_proj->dh_path != NULL);
    TEST_ASSERT(str_eql(child_proj->dh_path, expected_dh));
    dal_c_Project_cleanup(&child_proj);

    TEST_ASSERT(env_setCWD(original_cwd));

    free(child_project);
    free(expected_dh);
    free(detected_dh);
    free(dh_main_header);
    free(dh_header);
    free(dh_src_dir);
    free(dh_include_child_dir);
    free(dh_include_dir);
    free(dh_root);
    free(workspace_root);
    free(temp_root);
    free(original_cwd);
}

static void test_workspace_cache_scope(void) {
    test_reset_temp_root();
    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* workspace_root = path_join(temp_root, "cache-workspace");
    char* workspace_dh = path_join(workspace_root, "workspace.dh");
    char* project_root = path_join(workspace_root, "project-a");
    char* project_dh = path_join(project_root, "project.dh");
    char* adhoc_root = path_join(workspace_root, "scratch");
    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(dir_createRecur(adhoc_root));
    TEST_ASSERT(file_write(workspace_dh, "# workspace boundary\n"));
    TEST_ASSERT(file_write(project_dh, "output=project-a\n"));

    TEST_ASSERT(env_setCWD(project_root));
    dal_c_Cmd project_cmd = { 0 };
    dal_c_Project* project = dal_c_Project_detect(&project_cmd);
    TEST_ASSERT(project != NULL);
    TEST_ASSERT(project->root != NULL);
    TEST_ASSERT(project->workspace_root != NULL);
    TEST_ASSERT(test_path_text_eql(project->workspace_root, workspace_root));
    char* project_state = dal_c_Project_getStateRoot(project);
    char* expected_state = path_join(project_root, ".dh-c");
    char* project_cache = dal_c__cacheBaseDir(project);
    char* workspace_state_for_cache = path_join(workspace_root, ".dh-c");
    char* expected_cache = path_join(workspace_state_for_cache, "cache");
    TEST_ASSERT(test_path_text_eql(project_state, expected_state));
    TEST_ASSERT(test_path_text_eql(project_cache, expected_cache));
    free(expected_cache);
    free(workspace_state_for_cache);
    free(project_cache);
    free(expected_state);
    free(project_state);
    dal_c_Project_cleanup(&project);

    TEST_ASSERT(env_setCWD(adhoc_root));
    dal_c_Project* adhoc = dal_c_Project_detect(&project_cmd);
    TEST_ASSERT(adhoc != NULL);
    TEST_ASSERT(adhoc->root == NULL);
    TEST_ASSERT(adhoc->workspace_root != NULL);
    TEST_ASSERT(test_path_text_eql(adhoc->workspace_root, workspace_root));
    char* adhoc_cache = dal_c__cacheBaseDir(adhoc);
    char* workspace_state = path_join(workspace_root, ".dh-c");
    char* workspace_cache = path_join(workspace_state, "cache");
    TEST_ASSERT(test_path_text_eql(adhoc_cache, workspace_cache));
    free(workspace_cache);
    free(workspace_state);
    free(adhoc_cache);
    dal_c_Project_cleanup(&adhoc);

    TEST_ASSERT(env_setCWD(original_cwd));
    free(adhoc_root);
    free(project_dh);
    free(project_root);
    free(workspace_dh);
    free(workspace_root);
    free(temp_root);
    free(original_cwd);
}

static void test_dependency_lock_contract(void) {
    test_reset_temp_root();
    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "lock-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* lock_path = path_join(project_root, "lock.dh");
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh,
        "[dep]\n"
        "source=https://example.invalid/dep.git\n"
        "revision=main\n"
        "provider=dh\n"));
    TEST_ASSERT(file_write(lock_path,
        "# Generated by dh-c fetch/update. Do not edit by hand.\n"
        "[dep]\n"
        "provider=dh\n"
        "source=https://example.invalid/dep.git\n"
        "revision=0123456789abcdef\n"));

    dal_c_Project* project = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(project != NULL);
    TEST_ASSERT(project->lib_count == 1);
    char* resolved_lock_path = dal_c_Project_getDependencyLockPath(project);
    TEST_ASSERT(test_path_text_eql(resolved_lock_path, lock_path));
    char* revision = NULL;
    char* reason = NULL;
    TEST_ASSERT(dal_c_Project_readDependencyLock(project, &project->libraries[0], &revision, &reason));
    TEST_ASSERT(reason == NULL);
    TEST_ASSERT(str_eql(revision, "0123456789abcdef"));
    free(reason);
    free(revision);
    free(resolved_lock_path);
    dal_c_Project_cleanup(&project);

    free(lock_path);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_prebuilt_dependency_staging(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* consumer_root = path_join(temp_root, "prebuilt-consumer");
    char* dependency_root = path_join(temp_root, "prebuilt-dependency");
    char* dependency_include = path_join(dependency_root, "include");
    char* dependency_header = path_join(dependency_include, "foo.h");
    char* dependency_project = path_join(dependency_root, "project.dh");
    dal_c_CompilerOpts prebuilt_opts = { 0 };
    prebuilt_opts.profile = dal_c_Profile_stable;
    char* target_name = dal_c__resolveTargetDirName(&prebuilt_opts);
    char* prebuilt_target = path_join(dependency_root, "prebuilt");
    char* prebuilt_target_dir = path_join(prebuilt_target, target_name);
    char* prebuilt_profile = path_join(prebuilt_target_dir, "stable");
    char* prebuilt_libs = path_join(prebuilt_profile, "libs");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(consumer_root != NULL);
    TEST_ASSERT(dependency_root != NULL);
    TEST_ASSERT(dependency_include != NULL);
    TEST_ASSERT(dependency_header != NULL);
    TEST_ASSERT(dependency_project != NULL);
    TEST_ASSERT(prebuilt_libs != NULL);
    TEST_ASSERT(dir_createRecur(consumer_root));
    TEST_ASSERT(dir_createRecur(dependency_include));
    TEST_ASSERT(dir_createRecur(prebuilt_libs));
    TEST_ASSERT(file_write(dependency_header, "#pragma once\n"));
    TEST_ASSERT(file_write(dependency_project, "output=foo\nkind=lib\n"));

#ifdef _WIN32
    const char* native_name = "foo.lib";
    const char* lto_name = "foo.lto.lib";
#else
    const char* native_name = "libfoo.a";
    const char* lto_name = "libfoo.lto.a";
#endif
    char* native_path = path_join(prebuilt_libs, native_name);
    char* lto_path = path_join(prebuilt_libs, lto_name);
    TEST_ASSERT(native_path != NULL);
    TEST_ASSERT(lto_path != NULL);
    TEST_ASSERT(file_write(native_path, "native-prebuilt"));
    TEST_ASSERT(file_write(lto_path, "lto-prebuilt"));
    char* manifest_path = path_join(prebuilt_profile, "manifest.dh");
    TEST_ASSERT(manifest_path != NULL);
    char* manifest_reason = NULL;
    const dal_c_ProfileSpec* stable_profile = dal_c_ProfileSpec_by(dal_c_Profile_stable);
    dal_c_Cmd manifest_cmd = { 0 };
    manifest_cmd.action = dal_c_CmdAction_lib;
    manifest_cmd.opts.profile = dal_c_Profile_stable;
    char* producer_link_libs[] = { "customrt", "user32" };
    manifest_cmd.opts.link_libs = producer_link_libs;
    manifest_cmd.opts.link_count = 2;
    manifest_cmd.opts.lto_mode = dal_c_LtoMode_off;
    TEST_ASSERT(dal_c__writePrebuiltManifest(prebuilt_profile, &manifest_cmd, stable_profile, native_path, dal_c_Target_static_lib));
    manifest_cmd.opts.lto_mode = dal_c_LtoMode_auto;
    TEST_ASSERT(dal_c__writePrebuiltManifest(prebuilt_profile, &manifest_cmd, stable_profile, lto_path, dal_c_Target_static_lib));
    char* manifest_text = file_read(manifest_path);
    TEST_ASSERT(manifest_text != NULL);
    TEST_ASSERT(strstr(manifest_text, "compiler=") == NULL);
    TEST_ASSERT(strstr(manifest_text, "artifact=static|") != NULL);
    TEST_ASSERT(strstr(manifest_text, "artifact=static-lto|") != NULL);

    /* Producer link inputs are provenance, not a consumer requirement. A native
     * archive built while the producer also linked custom runtime libraries must
     * remain consumable by a caller with a different top-level link surface when
     * the C ABI contract is unchanged. */
    char* consumer_link_libs[] = { "different-top-level-runtime" };
    prebuilt_opts.link_libs = consumer_link_libs;
    prebuilt_opts.link_count = 1;
    prebuilt_opts.lto_mode = dal_c_LtoMode_off;
    TEST_ASSERT(dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, false, native_path, &manifest_reason));
    TEST_ASSERT(manifest_reason == NULL);

    prebuilt_opts.lto_mode = dal_c_LtoMode_auto;
    TEST_ASSERT(dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason == NULL);
    TEST_ASSERT(remove(manifest_path) == 0);
    TEST_ASSERT(!dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason != NULL && strstr(manifest_reason, "missing manifest.dh") != NULL);
    free(manifest_reason);
    manifest_reason = NULL;
    TEST_ASSERT(file_write(manifest_path, manifest_text));

    char* wrong_manifest_text = strdup(manifest_text);
    TEST_ASSERT(wrong_manifest_text != NULL);
    char* target_line = strstr(wrong_manifest_text, "target=");
    TEST_ASSERT(target_line != NULL);
    char* target_end = strchr(target_line, '\n');
    TEST_ASSERT(target_end != NULL);
    size_t suffix_len = strlen(target_end);
    memmove(target_line + strlen("target=wrong-target"), target_end, suffix_len + 1);
    memcpy(target_line, "target=wrong-target", strlen("target=wrong-target"));
    TEST_ASSERT(file_write(manifest_path, wrong_manifest_text));
    free(wrong_manifest_text);
    TEST_ASSERT(!dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason != NULL && strstr(manifest_reason, "target mismatch") != NULL);
    free(manifest_reason);
    manifest_reason = NULL;

    char* wrong_abi_text = strdup(manifest_text);
    TEST_ASSERT(wrong_abi_text != NULL);
    char* lto_entry = strstr(wrong_abi_text, "artifact=static-lto|");
    TEST_ASSERT(lto_entry != NULL);
    char* field = strchr(lto_entry, '|');
    field = field ? strchr(field + 1, '|') : NULL;
    TEST_ASSERT(field != NULL && isxdigit((unsigned char)field[1]));
    field[1] = field[1] == '0' ? '1' : '0';
    TEST_ASSERT(file_write(manifest_path, wrong_abi_text));
    free(wrong_abi_text);
    TEST_ASSERT(!dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason != NULL && strstr(manifest_reason, "ABI contract mismatch") != NULL);
    free(manifest_reason);
    manifest_reason = NULL;

    char* wrong_toolchain_text = strdup(manifest_text);
    TEST_ASSERT(wrong_toolchain_text != NULL);
    lto_entry = strstr(wrong_toolchain_text, "artifact=static-lto|");
    TEST_ASSERT(lto_entry != NULL);
    char* line_end = strchr(lto_entry, '\n');
    TEST_ASSERT(line_end != NULL);
    char* last_separator = line_end;
    while (last_separator > lto_entry && *last_separator != '|') { --last_separator; }
    TEST_ASSERT(last_separator > lto_entry && isxdigit((unsigned char)last_separator[1]));
    last_separator[1] = last_separator[1] == '0' ? '1' : '0';
    TEST_ASSERT(file_write(manifest_path, wrong_toolchain_text));
    free(wrong_toolchain_text);
    TEST_ASSERT(!dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason != NULL && strstr(manifest_reason, "LTO toolchain mismatch") != NULL);
    free(manifest_reason);
    manifest_reason = NULL;

    char* obsolete_manifest_text = str_format("manifest-version=1\n%s", manifest_text);
    TEST_ASSERT(obsolete_manifest_text != NULL);
    TEST_ASSERT(file_write(manifest_path, obsolete_manifest_text));
    TEST_ASSERT(!dal_c__prebuiltManifestCompatible(prebuilt_profile, &prebuilt_opts, stable_profile, dal_c_Target_static_lib, true, lto_path, &manifest_reason));
    TEST_ASSERT(manifest_reason != NULL && strstr(manifest_reason, "unsupported manifest key") != NULL);
    free(obsolete_manifest_text);
    free(manifest_reason);
    manifest_reason = NULL;
    TEST_ASSERT(file_write(manifest_path, manifest_text));

    dal_c_Project consumer = { 0 };
    consumer.root = consumer_root;
    consumer.name = "consumer";
    consumer.opts.prebuilt_mode = dal_c_PrebuiltMode_off;
    consumer.opts.prebuilt_mode_set = true;

    dal_c_Lib lib = { 0 };
    lib.name = "foo";
    lib.path = dependency_root;
    lib.is_static = true;
    lib.opts.profile = dal_c_Profile_stable;
    // A dependency-local explicit auto overrides the consumer-wide off mode.
    lib.opts.prebuilt_mode = dal_c_PrebuiltMode_auto;
    lib.opts.prebuilt_mode_set = true;

    dal_c_Cmd cmd = { 0 };
    cmd.action = dal_c_CmdAction_deps;
    cmd.opts.profile = dal_c_Profile_stable;
    TEST_ASSERT(dal_c__buildSingleLibrary(&cmd, &consumer, &lib) == 0);

    char* staged_root = path_join(consumer_root, "lib/deps");
    char* staged_header = path_join(staged_root, "foo.h");
    char* staged_native = path_join(staged_root, native_name);
    char* staged_lto = path_join(staged_root, lto_name);
    TEST_ASSERT(path_isFile(staged_header));
    TEST_ASSERT(path_isFile(staged_native));
    TEST_ASSERT(path_isFile(staged_lto));

    dal_c_Lib missing = lib;
    missing.name = "missing";
    missing.opts.prebuilt_mode = dal_c_PrebuiltMode_required;
    missing.opts.prebuilt_mode_set = true;
    TEST_ASSERT(dal_c__buildSingleLibrary(&cmd, &consumer, &missing) != 0);

    free(staged_lto);
    free(staged_native);
    free(staged_header);
    free(staged_root);
    free(manifest_text);
    free(manifest_path);
    free(lto_path);
    free(native_path);
    free(prebuilt_libs);
    free(prebuilt_profile);
    free(prebuilt_target_dir);
    free(prebuilt_target);
    free(target_name);
    free(dependency_project);
    free(dependency_header);
    free(dependency_include);
    free(dependency_root);
    free(consumer_root);
    free(temp_root);
    test_reset_temp_root();
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
    TEST_ASSERT(test_remove_recur(temp_root));

    free(local_build_dev);
    free(project_build_dev);
    free(local_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
    free(original_cwd);
}

static void test_clean_profile_removes_dependency_exports(void) {
    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "clean-deps-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* build_dev = path_join(project_root, "build/dev");
    char* build_release = path_join(project_root, "build/release");
    char* lib_dir = path_join(project_root, "lib");
    char* deps_dir = path_join(lib_dir, "deps");
    char* stale_archive = path_join(deps_dir, "mad.lib");
    char* deps_header = path_join(lib_dir, "deps.h");

    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(build_dev != NULL);
    TEST_ASSERT(build_release != NULL);
    TEST_ASSERT(lib_dir != NULL);
    TEST_ASSERT(deps_dir != NULL);
    TEST_ASSERT(stale_archive != NULL);
    TEST_ASSERT(deps_header != NULL);

    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh, "output=clean-deps-project\n"));
    TEST_ASSERT(dir_createRecur(build_dev));
    TEST_ASSERT(dir_createRecur(build_release));
    TEST_ASSERT(dir_createRecur(deps_dir));
    TEST_ASSERT(file_write(stale_archive, "stale archive\n"));
    TEST_ASSERT(file_write(deps_header, "#pragma once\n"));
    TEST_ASSERT(env_setCWD(project_root));

    const char* argv[] = { dal_c_tool_name, "clean", "dev", NULL };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
    TEST_ASSERT(cmd != NULL);
    dal_c_Project* proj = dal_c_Project_detect(cmd);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->root != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(cmd, proj) == 0);
    TEST_ASSERT(!path_exists(build_dev));
    TEST_ASSERT(path_isDir(build_release));
    TEST_ASSERT(!path_exists(deps_dir));
    TEST_ASSERT(!path_exists(deps_header));

    dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    TEST_ASSERT(env_setCWD(original_cwd));
    TEST_ASSERT(test_remove_recur(temp_root));

    free(deps_header);
    free(stale_archive);
    free(deps_dir);
    free(lib_dir);
    free(build_release);
    free(build_dev);
    free(project_dh);
    free(project_root);
    free(temp_root);
    free(original_cwd);
}

static void test_clean_cache_scope(void) {
    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "clean-cache-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* build_dir = path_join(project_root, "build");
    char* build_marker = path_join(build_dir, "keep.txt");
    char* cache_dir = path_join(build_dir, ".cache");
    char* cache_marker = path_join(cache_dir, "stale.cache");

    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(dir_createRecur(cache_dir));
    TEST_ASSERT(file_write(project_dh, "output=clean-cache-project\n"));
    TEST_ASSERT(file_write(build_marker, "keep\n"));
    TEST_ASSERT(file_write(cache_marker, "cache\n"));
    TEST_ASSERT(env_setCWD(project_root));

    const char* dry_argv[] = { dal_c_tool_name, "clean", "--cache", "--dry-run", NULL };
    dal_c_Cmd* dry_cmd = dal_c_Cmd_parse(4, dry_argv);
    TEST_ASSERT(dry_cmd != NULL);
    dal_c_Project* proj = dal_c_Project_detect(dry_cmd);
    TEST_ASSERT(proj != NULL && proj->root != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(dry_cmd, proj) == 0);
    TEST_ASSERT(path_isFile(build_marker));
    TEST_ASSERT(path_isFile(cache_marker));
    dal_c_Cmd_cleanup(&dry_cmd);

    const char* clean_argv[] = { dal_c_tool_name, "clean", "--cache", "--older-than=0s", NULL };
    dal_c_Cmd* clean_cmd = dal_c_Cmd_parse(4, clean_argv);
    TEST_ASSERT(clean_cmd != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(clean_cmd, proj) == 0);
    TEST_ASSERT(path_isFile(build_marker));
    TEST_ASSERT(!path_exists(cache_marker));

    dal_c_Cmd_cleanup(&clean_cmd);
    dal_c_Project_cleanup(&proj);
    TEST_ASSERT(env_setCWD(original_cwd));
    TEST_ASSERT(test_remove_recur(temp_root));

    free(cache_marker);
    free(cache_dir);
    free(build_marker);
    free(build_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
    free(original_cwd);
}

static void test_clean_unused_dependencies(void) {
    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "clean-unused-deps-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* lock_dh = path_join(project_root, "lock.dh");
    char* used_src = path_join(project_root, ".dh-c/deps/src/used");
    char* unused_src = path_join(project_root, ".dh-c/deps/src/unused");
    char* used_build = path_join(project_root, ".dh-c/deps/build/native/dev/used");
    char* unused_build = path_join(project_root, ".dh-c/deps/build/native/dev/unused");
    char* used_package = path_join(project_root, ".dh-c/deps/packages/native/dev/used");
    char* unused_package = path_join(project_root, ".dh-c/deps/packages/native/dev/unused");
    char* usage_dir = path_join(project_root, ".dh-c/deps/usage");
    char* used_stamp = path_join(usage_dir, "used.stamp");
    char* unused_stamp = path_join(usage_dir, "unused.stamp");

    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh,
        "output=clean-unused-deps-project\n"
        "[used]\n"
        "source=https://example.invalid/used.git\n"
        "provider=cmake\n"));
    TEST_ASSERT(file_write(lock_dh, "[unused]\nrevision=deadbeef\n"));
    TEST_ASSERT(dir_createRecur(used_src));
    TEST_ASSERT(dir_createRecur(unused_src));
    TEST_ASSERT(dir_createRecur(used_build));
    TEST_ASSERT(dir_createRecur(unused_build));
    TEST_ASSERT(dir_createRecur(used_package));
    TEST_ASSERT(dir_createRecur(unused_package));
    TEST_ASSERT(dir_createRecur(usage_dir));
    TEST_ASSERT(file_write(used_stamp, "1\n"));
    TEST_ASSERT(file_write(unused_stamp, "1\n"));
    TEST_ASSERT(env_setCWD(project_root));

    const char* dry_argv[] = { dal_c_tool_name, "clean", "--deps", "--unused", "--dry-run", NULL };
    dal_c_Cmd* dry_cmd = dal_c_Cmd_parse(5, dry_argv);
    TEST_ASSERT(dry_cmd != NULL);
    dal_c_Project* proj = dal_c_Project_detect(dry_cmd);
    TEST_ASSERT(proj != NULL && proj->root != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(dry_cmd, proj) == 0);
    TEST_ASSERT(path_isDir(unused_src));
    TEST_ASSERT(path_isDir(unused_build));
    TEST_ASSERT(path_isDir(unused_package));
    dal_c_Cmd_cleanup(&dry_cmd);

    const char* clean_argv[] = { dal_c_tool_name, "clean", "--deps", "--unused", NULL };
    dal_c_Cmd* clean_cmd = dal_c_Cmd_parse(4, clean_argv);
    TEST_ASSERT(clean_cmd != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(clean_cmd, proj) == 0);
    TEST_ASSERT(path_isDir(used_src));
    TEST_ASSERT(path_isDir(used_build));
    TEST_ASSERT(path_isDir(used_package));
    TEST_ASSERT(path_isFile(used_stamp));
    TEST_ASSERT(!path_exists(unused_src));
    TEST_ASSERT(!path_exists(unused_build));
    TEST_ASSERT(!path_exists(unused_package));
    TEST_ASSERT(!path_exists(unused_stamp));
    TEST_ASSERT(path_isFile(lock_dh));

    const char* old_argv[] = { dal_c_tool_name, "clean", "--deps", "--older-than=0s", NULL };
    dal_c_Cmd* old_cmd = dal_c_Cmd_parse(4, old_argv);
    TEST_ASSERT(old_cmd != NULL);
    TEST_ASSERT(dal_c_Cmd_cleanTarget(old_cmd, proj) == 0);
    TEST_ASSERT(!path_exists(used_src));
    TEST_ASSERT(!path_exists(used_build));
    TEST_ASSERT(!path_exists(used_package));
    TEST_ASSERT(!path_exists(used_stamp));
    TEST_ASSERT(path_isFile(lock_dh));
    dal_c_Cmd_cleanup(&old_cmd);

    dal_c_Cmd_cleanup(&clean_cmd);
    dal_c_Project_cleanup(&proj);
    TEST_ASSERT(env_setCWD(original_cwd));
    TEST_ASSERT(test_remove_recur(temp_root));

    free(unused_stamp);
    free(used_stamp);
    free(usage_dir);
    free(unused_package);
    free(used_package);
    free(unused_build);
    free(used_build);
    free(unused_src);
    free(used_src);
    free(lock_dh);
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

static void test_output_override_generates_target_extensions(void) {
    test_reset_temp_root();

    char* temp_root = test_temp_root();
    char* project_root = path_join(temp_root, "output-contract");
    char* project_dh = path_join(project_root, dal_c_file_detector_project);
    char* build_dir = path_join(project_root, "build/dev");
    char* output_stem = path_join(project_root, "artifacts/widget");
    char* output_dir = path_join(project_root, "dist");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(build_dir != NULL);
    TEST_ASSERT(output_stem != NULL);
    TEST_ASSERT(output_dir != NULL);
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh, "output=widget\nkind=lib\n"));
    TEST_ASSERT(dir_createRecur(output_dir));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, NULL);
    TEST_ASSERT(proj != NULL);

    {
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--output", output_stem, NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        char* static_path = dal_c__resolveOutputPath(proj, cmd, build_dir, "widget", dal_c_Target_static_lib);
        char* shared_path = dal_c__resolveOutputPath(proj, cmd, build_dir, "widget", dal_c_Target_shared_lib);
#ifdef _WIN32
        char* expected_static = str_format("%s.lib", output_stem);
        char* expected_shared = str_format("%s.dll", output_stem);
#else
        char* stem_parent = path_parent(output_stem);
        char* expected_static = stem_parent ? path_join(stem_parent, "libwidget.a") : strdup("libwidget.a");
        char* expected_shared = stem_parent ? path_join(stem_parent, "libwidget.so") : strdup("libwidget.so");
        free(stem_parent);
#endif
        TEST_ASSERT(static_path != NULL);
        TEST_ASSERT(shared_path != NULL);
        TEST_ASSERT(expected_static != NULL);
        TEST_ASSERT(expected_shared != NULL);
        TEST_ASSERT(test_path_text_eql(static_path, expected_static));
        TEST_ASSERT(test_path_text_eql(shared_path, expected_shared));
        free(expected_shared);
        free(expected_static);
        free(shared_path);
        free(static_path);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--output", output_dir, NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(5, argv);
        TEST_ASSERT(cmd != NULL);
        char* static_path = dal_c__resolveOutputPath(proj, cmd, build_dir, "widget", dal_c_Target_static_lib);
        char* shared_path = dal_c__resolveOutputPath(proj, cmd, build_dir, "widget", dal_c_Target_shared_lib);
#ifdef _WIN32
        char* expected_static = path_join(output_dir, "widget.lib");
        char* expected_shared = path_join(output_dir, "widget.dll");
#else
        char* expected_static = path_join(output_dir, "libwidget.a");
        char* expected_shared = path_join(output_dir, "libwidget.so");
#endif
        TEST_ASSERT(static_path != NULL);
        TEST_ASSERT(shared_path != NULL);
        TEST_ASSERT(expected_static != NULL);
        TEST_ASSERT(expected_shared != NULL);
        TEST_ASSERT(test_path_text_eql(static_path, expected_static));
        TEST_ASSERT(test_path_text_eql(shared_path, expected_shared));
        free(expected_shared);
        free(expected_static);
        free(shared_path);
        free(static_path);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        char* pyd_stem = path_join(project_root, "artifacts/_widget");
        char* pyd_path = path_join(project_root, "artifacts/_widget.pyd");
        TEST_ASSERT(pyd_stem != NULL);
        TEST_ASSERT(pyd_path != NULL);
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--shared", "--output", pyd_stem, "--output-ext=.pyd", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(7, argv);
        TEST_ASSERT(cmd != NULL);
        char* shared_path = dal_c__resolveOutputPath(proj, cmd, build_dir, "widget", dal_c_Target_shared_lib);
        TEST_ASSERT(shared_path != NULL);
#ifdef _WIN32
        TEST_ASSERT(test_path_text_eql(shared_path, pyd_path));
#else
        TEST_ASSERT(strstr(shared_path, "_widget.pyd") != NULL);
#endif
        free(shared_path);
        dal_c_Cmd_cleanup(&cmd);
        free(pyd_path);
        free(pyd_stem);
    }

    dal_c_Project_cleanup(&proj);
    TEST_ASSERT(test_remove_recur(temp_root));
    free(output_dir);
    free(output_stem);
    free(build_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}

static void test_output_ext_does_not_rewrite_dependency_artifacts(void) {
    test_reset_temp_root();

    char* original_cwd = env_getCWD();
    char* temp_root = test_temp_root();
    char* dep_root = path_join(temp_root, "dep");
    char* dep_project_dh = path_join(dep_root, "project.dh");
    char* dep_include_dir = path_join(dep_root, "include");
    char* dep_src_dir = path_join(dep_root, "src");
    char* dep_header = path_join(dep_include_dir, "dep.h");
    char* dep_source = path_join(dep_src_dir, "dep.c");
    char* app_root = path_join(temp_root, "app");
    char* app_project_dh = path_join(app_root, "project.dh");
    char* app_include_dir = path_join(app_root, "include");
    char* app_src_dir = path_join(app_root, "src");
    char* app_header = path_join(app_include_dir, "app.h");
    char* app_source = path_join(app_src_dir, "app.c");
    char* output_stem = path_join(app_root, "dist/app_module");
    dal_c_CompilerOpts layout_opts = { 0 };
    layout_opts.profile = dal_c_Profile_dev;
    char* layout_target = dal_c__resolveTargetDirName(&layout_opts);
    char* app_build_root = path_join(app_root, "build");
    char* app_target_root = path_join(app_build_root, layout_target);
    char* app_build_dir = path_join(app_target_root, "dev");
    char* dep_build_root = path_join(dep_root, "build");
    char* dep_target_root = path_join(dep_build_root, layout_target);
    char* dep_build_dir = path_join(dep_target_root, "dev");
    char* dep_libs_dir = path_join(dep_build_dir, "libs");
    char* app_deps_dir = path_join(app_root, "lib/deps");
#ifdef _WIN32
    char* expected_dep_archive = path_join(dep_libs_dir, "dep.lib");
    char* expected_staged_archive = path_join(app_deps_dir, "dep.lib");
    char* unexpected_dep_archive = path_join(dep_libs_dir, "dep.pyd");
    char* unexpected_staged_archive = path_join(app_deps_dir, "dep.pyd");
#else
    char* expected_dep_archive = path_join(dep_libs_dir, "libdep.a");
    char* expected_staged_archive = path_join(app_deps_dir, "libdep.a");
    char* unexpected_dep_archive = path_join(dep_libs_dir, "libdep.pyd");
    char* unexpected_staged_archive = path_join(app_deps_dir, "libdep.pyd");
#endif

    TEST_ASSERT(original_cwd != NULL);
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dep_root != NULL);
    TEST_ASSERT(dep_project_dh != NULL);
    TEST_ASSERT(dep_include_dir != NULL);
    TEST_ASSERT(dep_src_dir != NULL);
    TEST_ASSERT(dep_header != NULL);
    TEST_ASSERT(dep_source != NULL);
    TEST_ASSERT(app_root != NULL);
    TEST_ASSERT(app_project_dh != NULL);
    TEST_ASSERT(app_include_dir != NULL);
    TEST_ASSERT(app_src_dir != NULL);
    TEST_ASSERT(app_header != NULL);
    TEST_ASSERT(app_source != NULL);
    TEST_ASSERT(output_stem != NULL);
    TEST_ASSERT(app_build_dir != NULL);
    TEST_ASSERT(dep_build_dir != NULL);
    TEST_ASSERT(dep_libs_dir != NULL);
    TEST_ASSERT(app_deps_dir != NULL);
    TEST_ASSERT(expected_dep_archive != NULL);
    TEST_ASSERT(expected_staged_archive != NULL);
    TEST_ASSERT(unexpected_dep_archive != NULL);
    TEST_ASSERT(unexpected_staged_archive != NULL);

    TEST_ASSERT(dir_createRecur(dep_include_dir));
    TEST_ASSERT(dir_createRecur(dep_src_dir));
    TEST_ASSERT(file_write(dep_project_dh, "output=dep\npch=off\nlink-dsl=off\n"));
    TEST_ASSERT(file_write(dep_header, "#pragma once\nint dep_value(void);\n"));
    TEST_ASSERT(file_write(dep_source, "#include \"dep.h\"\nint dep_value(void) { return 7; }\n"));

    TEST_ASSERT(dir_createRecur(app_include_dir));
    TEST_ASSERT(dir_createRecur(app_src_dir));
    TEST_ASSERT(file_write(
        app_project_dh,
        "output=app\nkind=shared-lib\npch=off\nlink-dsl=off\n"
        "[dep]\n"
        "path=../dep\n"
        "linking=static\n"
        "link-dsl=off\n"
    ));
    TEST_ASSERT(file_write(app_header, "#pragma once\nint app_value(void);\n"));
    TEST_ASSERT(file_write(app_source, "#include \"app.h\"\n#include <dep.h>\nint app_value(void) { return dep_value(); }\n"));
    TEST_ASSERT(env_setCWD(app_root));

    const char* argv[] = {
        dal_c_tool_name,
        "build",
        "dev",
        "--shared",
        "--output",
        output_stem,
        "--output-ext=.pyd",
        "--link-dsl=off",
        NULL
    };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(8, argv);
    TEST_ASSERT(cmd != NULL);
    dal_c_Project* proj = dal_c_Project_detect(cmd);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->root != NULL);
    TEST_ASSERT(dal_c_Cmd_makeTarget(cmd, proj) == 0);

    char* final_output = dal_c__resolveOutputPath(proj, cmd, app_build_dir, "app", dal_c_Target_shared_lib);
    TEST_ASSERT(final_output != NULL);
    TEST_ASSERT(str_endsWith(final_output, ".pyd"));
    TEST_ASSERT(path_isFile(final_output));
    TEST_ASSERT(path_isFile(expected_dep_archive));
    TEST_ASSERT(path_isFile(expected_staged_archive));
    TEST_ASSERT(!path_exists(unexpected_dep_archive));
    TEST_ASSERT(!path_exists(unexpected_staged_archive));

    free(final_output);
    dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    TEST_ASSERT(env_setCWD(original_cwd));
    TEST_ASSERT(test_remove_recur(temp_root));

    free(unexpected_staged_archive);
    free(unexpected_dep_archive);
    free(expected_staged_archive);
    free(expected_dep_archive);
    free(app_deps_dir);
    free(dep_libs_dir);
    free(dep_build_dir);
    free(dep_target_root);
    free(dep_build_root);
    free(app_build_dir);
    free(app_target_root);
    free(app_build_root);
    free(layout_target);
    free(output_stem);
    free(app_source);
    free(app_header);
    free(app_src_dir);
    free(app_include_dir);
    free(app_project_dh);
    free(app_root);
    free(dep_source);
    free(dep_header);
    free(dep_src_dir);
    free(dep_include_dir);
    free(dep_project_dh);
    free(dep_root);
    free(temp_root);
    free(original_cwd);
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
        const char* multi_argv[] = { dal_c_tool_name, "build", "dev", source, sample_source, NULL };
        dal_c_Cmd* multi_cmd = dal_c_Cmd_parse(5, multi_argv);
        TEST_ASSERT(multi_cmd != NULL);
        dal_c_Cmd_normalizeIntent(multi_cmd, &intent);
        TEST_ASSERT(intent.target_path_is_explicit_file);
        TEST_ASSERT(dal_c_TargetRequest_resolve(proj, &intent, &request));
        TEST_ASSERT(request.root == NULL);
        dal_c_TargetRequest_cleanup(&request);
        dal_c_Cmd_cleanup(&multi_cmd);
    }

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
    dal_c_CompilerOpts demo_opts = { 0 };
    demo_opts.profile = dal_c_Profile_dev;
    char* demo_target_name = dal_c__resolveTargetDirName(&demo_opts);
    char* demo_build_root = path_join(project_root, "build");
    char* demo_target_root = path_join(demo_build_root, demo_target_name);
    char* demo_profile_root = path_join(demo_target_root, "dev");
    char* demo_output_dir = path_join(demo_profile_root, "examples/demo");
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
    TEST_ASSERT(test_remove_recur(temp_root));

    free(compile_db_path);
    free(demo_output_dir);
    free(demo_profile_root);
    free(demo_target_root);
    free(demo_build_root);
    free(demo_target_name);
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
        "--macro-backtrace-limit=unlimited",
        "--version-core=1.2.3",
        "--comp-args=-Wno-unused",
        "--jobs=3",
        NULL
    };
    dal_c_Cmd* cmd = dal_c_Cmd_parse(13, argv);
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
    TEST_ASSERT(test_arrstr_contains(args, "-fmacro-backtrace-limit=0"));
    TEST_ASSERT(test_arrstr_contains(args, "-Dsyntax_contract_project__NUM__VER_CORE_MAJOR=1"));
    TEST_ASSERT(test_arrstr_contains(args, "-Dsyntax_contract_project__NUM__VER_CORE_MINOR=2"));
    TEST_ASSERT(test_arrstr_contains(args, "-Dsyntax_contract_project__NUM__VER_CORE_PATCH=3"));
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
    TEST_ASSERT(test_remove_recur(temp_root));

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
    TEST_ASSERT(test_remove_recur(temp_root));

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
    char* dh_bundle = path_join(dh_root, "include/dh-bundle.h");
    char* project_root = path_join(temp_root, "dh-prelude-project");
    char* project_dh = path_join(project_root, "project.dh");
    char* source_dir = path_join(project_root, "src");
    char* main_source = path_join(source_dir, "main.c");
    char* deps_header = path_join(project_root, "lib/deps.h");
    char* deps_dir = path_join(project_root, "lib/deps");
    char* top_dep_header = path_join(deps_dir, "vendor.h");
    char* nested_dep_dir = path_join(deps_dir, "vendor");
    char* nested_dep_header = path_join(nested_dep_dir, "detail.h");
    TEST_ASSERT(temp_root != NULL);
    TEST_ASSERT(dh_root != NULL);
    TEST_ASSERT(dh_bundle != NULL);
    TEST_ASSERT(project_root != NULL);
    TEST_ASSERT(project_dh != NULL);
    TEST_ASSERT(source_dir != NULL);
    TEST_ASSERT(main_source != NULL);
    TEST_ASSERT(deps_header != NULL);
    TEST_ASSERT(deps_dir != NULL);
    TEST_ASSERT(top_dep_header != NULL);
    TEST_ASSERT(nested_dep_dir != NULL);
    TEST_ASSERT(nested_dep_header != NULL);
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(file_write(project_dh, "output=dh-prelude\n"));
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));

    dal_c_Project* proj = dal_c_Project_detectAt(project_root, dh_root);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header != NULL);
    TEST_ASSERT(test_path_text_eql(proj->pch_header, dh_bundle));
    TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &proj->opts));
    TEST_ASSERT(!path_exists(deps_header));

    {
        const char* argv[] = { dal_c_tool_name, "build", "dev", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(3, argv);
        TEST_ASSERT(cmd != NULL);

        const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        char* build_dir = dal_c_Project_getBuildDir(proj);
        char* profile_dir = path_join(build_dir, profile->name);
        char* object_dir = path_join(profile_dir, "obj");
        char* target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, "dh-prelude", dal_c_Target_executable);
        char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_executable);
        ArrStr* sources = ArrStr_init();
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(target_path != NULL);
        TEST_ASSERT(makefile_path != NULL);
        TEST_ASSERT(sources != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));
        ArrStr_push(sources, main_source);
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        char* makefile_text = file_read(makefile_path);
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "-include-pch") != NULL);
        TEST_ASSERT(strstr(makefile_text, "PCH_SRC = ") != NULL);
        TEST_ASSERT(strstr(makefile_text, "dh-bundle.h") != NULL);
        TEST_ASSERT(strstr(makefile_text, "lib/deps.h") == NULL);
        free(makefile_text);
        ArrStr_fini(&sources);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
    }

    {
        const char* argv[] = { dal_c_tool_name, "build", "dev", "--link-dsl=off", NULL };
        dal_c_Cmd* cmd = dal_c_Cmd_parse(4, argv);
        TEST_ASSERT(cmd != NULL);
        TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &cmd->opts));
        TEST_ASSERT(!path_exists(deps_header));

        const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
        TEST_ASSERT(profile != NULL);
        char* build_dir = dal_c_Project_getBuildDir(proj);
        char* profile_dir = path_join(build_dir, profile->name);
        char* object_dir = path_join(profile_dir, "obj");
        char* target_path = dal_c__resolveOutputPath(proj, cmd, profile_dir, "dh-prelude", dal_c_Target_executable);
        char* makefile_path = dal_c__makePlanFilePath(proj, profile, cmd, target_path, dal_c_Target_executable);
        ArrStr* sources = ArrStr_init();
        TEST_ASSERT(build_dir != NULL);
        TEST_ASSERT(profile_dir != NULL);
        TEST_ASSERT(object_dir != NULL);
        TEST_ASSERT(target_path != NULL);
        TEST_ASSERT(makefile_path != NULL);
        TEST_ASSERT(sources != NULL);
        TEST_ASSERT(dir_createRecur(object_dir));
        ArrStr_push(sources, main_source);
        TEST_ASSERT(dal_c__generateMakefile(cmd, proj, profile, sources, target_path, object_dir, dal_c_Target_executable) == 0);
        char* makefile_text = file_read(makefile_path);
        TEST_ASSERT(makefile_text != NULL);
        TEST_ASSERT(strstr(makefile_text, "-include-pch") == NULL);
        TEST_ASSERT(strstr(makefile_text, "PCH_SRC = ") == NULL);
        free(makefile_text);
        ArrStr_fini(&sources);
        free(makefile_path);
        free(target_path);
        free(object_dir);
        free(profile_dir);
        free(build_dir);
        dal_c_Cmd_cleanup(&cmd);
    }
    dal_c_Project_cleanup(&proj);

    TEST_ASSERT(test_remove_recur(project_root));
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(project_dh, "output=dh-prelude\nlink-dsl=off\n"));
    proj = dal_c_Project_detectAt(project_root, dh_root);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header == NULL);
    TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &proj->opts));
    TEST_ASSERT(!path_exists(deps_header));

    dal_c_Project_cleanup(&proj);

    TEST_ASSERT(test_remove_recur(project_root));
    TEST_ASSERT(dir_createRecur(project_root));
    TEST_ASSERT(dir_createRecur(source_dir));
    TEST_ASSERT(dir_createRecur(nested_dep_dir));
    TEST_ASSERT(file_write(main_source, "int main(void) { return 0; }\n"));
    TEST_ASSERT(file_write(project_dh, "output=dh-prelude\npch=deps\n"));
    TEST_ASSERT(file_write(top_dep_header, "#pragma once\n#define VENDOR_VALUE 1\n"));
    TEST_ASSERT(file_write(nested_dep_header, "#pragma once\n#define VENDOR_DETAIL 2\n"));
    proj = dal_c_Project_detectAt(project_root, dh_root);
    TEST_ASSERT(proj != NULL);
    TEST_ASSERT(proj->pch_header != NULL);
    TEST_ASSERT(strstr(proj->pch_header, "deps.h") != NULL);
    TEST_ASSERT(dal_c__writeDepsPreludeHeader(proj, &proj->opts));
    TEST_ASSERT(path_isFile(deps_header));
    {
        char* deps_text = file_read(deps_header);
        TEST_ASSERT(deps_text != NULL);
        TEST_ASSERT(strstr(deps_text, "#include <dh-bundle.h>") != NULL);
        TEST_ASSERT(strstr(deps_text, "#include <vendor.h>") != NULL);
        TEST_ASSERT(strstr(deps_text, "vendor/detail.h") == NULL);
        free(deps_text);
    }
    dal_c_Project_cleanup(&proj);
    TEST_ASSERT(test_remove_recur(temp_root));

    free(nested_dep_header);
    free(nested_dep_dir);
    free(top_dep_header);
    free(deps_dir);
    free(deps_header);
    free(main_source);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(dh_bundle);
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
    TEST_ASSERT(test_remove_recur(temp_root));

    free(source_relative);
    free(main_source);
    free(source_dir);
    free(project_dh);
    free(project_root);
    free(temp_root);
}
