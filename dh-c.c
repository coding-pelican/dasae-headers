// clang -std=c17 -Wall -Wextra -funsigned-char -fblocks -o dh-c dh-c.c

/**
 * dh-c.c - Build tool for DH-C projects
 *
 * Features:
 * - Detects clang toolchain
 * - Supports build, test, and run commands
 * - Handles debug/release/optimize modes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define PATH_SEPARATOR   "\\"
#define mkdir(dir, mode) _mkdir(dir)
#else
#include <sys/types.h>
#include <limits.h> // For PATH_MAX
#define PATH_SEPARATOR "/"
#endif

#define DH_VERSION "0.1.0"

// Configuration
typedef struct {
    char project_name[256];
    char project_root[1024];
    char compiler[64];
    char c_standard[8];
    char output_dir[1024];
    bool is_test;
    char build_mode[16]; // debug, release, optimize
    char optimization_level[8];
    char linked_libraries[1024];
    char run_args[1024];
    bool is_single_file;
    char single_file[1024];
    char dh_path[1024];
} BuildConfig;

// Function prototypes
bool detect_clang();
void detect_project_structure(BuildConfig* config);
void collect_source_files(BuildConfig* config, char*** sources, int* source_count);
void collect_include_paths(BuildConfig* config, char*** includes, int* include_count);
void parse_libraries(BuildConfig* config, char* libs_file);
void build_project(BuildConfig* config);
void run_executable(BuildConfig* config);
void print_usage();
void free_string_array(char** array, int count);
bool find_dh_path(BuildConfig* config);
bool is_c_source_file(const char* filename);
void detect_build_type(BuildConfig* config, int argc, char* argv[]);
void add_dh_includes(BuildConfig* config, char*** includes, int* include_count);
void add_dh_sources(BuildConfig* config, char*** sources, int* source_count);
void create_workspace(const char* name, const char* dh_path);
void create_project(const char* name, const char* dh_path);
void write_file(const char* path, const char* content);
bool create_directory(const char* path);
bool find_dh_path_from_env(BuildConfig* config);
void write_example_files(const char* project_path);
void create_project_clangd(const char* project_path, const char* dh_path);

// Add the template content for config files
const char* CLANGD_TEMPLATE = "CompileFlags:\n"
                              "  Add: [-xc, -std=c17, -Wall, -Wextra, -funsigned-char, -fblocks]\n"
                              "  Compiler: clang\n"
                              "\n"
                              "Diagnostics:\n"
                              "  UnusedIncludes: Strict\n"
                              "  ClangTidy:\n"
                              "    Add: [modernize*, performance*, readability*, bugprone*, portability*]\n"
                              "    Remove: [modernize-use-trailing-return-type, readability-named-parameter]\n"
                              "\n"
                              "InlayHints:\n"
                              "  Enabled: Yes\n"
                              "  ParameterNames: Yes\n"
                              "  DeducedTypes: Yes\n"
                              "\n"
                              "Hover:\n"
                              "  ShowAKA: Yes\n";

const char* CLANG_FORMAT_TEMPLATE = "Language: Cpp\n"
                                    "BasedOnStyle: LLVM\n"
                                    "IndentWidth: 4\n"
                                    "TabWidth: 4\n"
                                    "UseTab: Never\n"
                                    "ColumnLimit: 120\n"
                                    "AlignAfterOpenBracket: Align\n"
                                    "AlignConsecutiveAssignments: true\n"
                                    "AlignConsecutiveDeclarations: true\n"
                                    "AlignEscapedNewlines: Right\n"
                                    "AlignOperands: true\n"
                                    "AlignTrailingComments: true\n"
                                    "AllowAllArgumentsOnNextLine: false\n"
                                    "AllowAllParametersOfDeclarationOnNextLine: false\n"
                                    "AllowShortBlocksOnASingleLine: Empty\n"
                                    "AllowShortCaseLabelsOnASingleLine: false\n"
                                    "AllowShortFunctionsOnASingleLine: Empty\n"
                                    "AllowShortIfStatementsOnASingleLine: Never\n"
                                    "AllowShortLoopsOnASingleLine: false\n"
                                    "AlwaysBreakAfterReturnType: None\n"
                                    "AlwaysBreakBeforeMultilineStrings: false\n"
                                    "BinPackArguments: false\n"
                                    "BinPackParameters: false\n"
                                    "BreakBeforeBraces: Attach\n"
                                    "BreakStringLiterals: true\n"
                                    "SpaceBeforeParens: ControlStatements\n";

const char* TASKS_JSON_TEMPLATE = "{\n"
                                  "  \"version\": \"2.0.0\",\n"
                                  "  \"inputs\": [\n"
                                  "    {\n"
                                  "      \"id\": \"compiler\",\n"
                                  "      \"type\": \"pickString\",\n"
                                  "      \"description\": \"Select compiler\",\n"
                                  "      \"options\": [\n"
                                  "        \"clang\",\n"
                                  "        \"gcc\"\n"
                                  "      ],\n"
                                  "      \"default\": \"clang\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"id\": \"cStandard\",\n"
                                  "      \"type\": \"pickString\",\n"
                                  "      \"description\": \"Select C standard\",\n"
                                  "      \"options\": [\n"
                                  "        \"c17\",\n"
                                  "        \"c11\",\n"
                                  "        \"c99\"\n"
                                  "      ],\n"
                                  "      \"default\": \"c17\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"id\": \"testFlag\",\n"
                                  "      \"type\": \"pickString\",\n"
                                  "      \"description\": \"Build tests?\",\n"
                                  "      \"options\": [\n"
                                  "        \"\",\n"
                                  "        \"-DCOMP_TEST\"\n"
                                  "      ],\n"
                                  "      \"default\": \"\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"id\": \"optimization\",\n"
                                  "      \"type\": \"pickString\",\n"
                                  "      \"description\": \"Select optimization level\",\n"
                                  "      \"options\": [\n"
                                  "        \"-O0\",\n"
                                  "        \"-O1\",\n"
                                  "        \"-O2\",\n"
                                  "        \"-O3\",\n"
                                  "        \"-Os\",\n"
                                  "        \"-Ofast\"\n"
                                  "      ],\n"
                                  "      \"default\": \"-O2\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"id\": \"libraries\",\n"
                                  "      \"type\": \"pickString\",\n"
                                  "      \"description\": \"Select libraries to link\",\n"
                                  "      \"options\": [\n"
                                  "        \"\",\n"
                                  "        \"-lm\",\n"
                                  "        \"-lpthread\"\n"
                                  "      ],\n"
                                  "      \"default\": \"\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"id\": \"runArgs\",\n"
                                  "      \"type\": \"promptString\",\n"
                                  "      \"description\": \"Arguments to pass to the program\",\n"
                                  "      \"default\": \"\"\n"
                                  "    }\n"
                                  "  ],\n"
                                  "  \"tasks\": [\n"
                                  "    {\n"
                                  "      \"label\": \"dh>build C file (debug)\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"build\",\n"
                                  "        \"debug\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--compiler=${input:compiler}\",\n"
                                  "        \"--std=${input:cStandard}\"\n"
                                  "      ],\n"
                                  "      \"group\": \"build\",\n"
                                  "      \"problemMatcher\": [\"$gcc\"]\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"label\": \"dh>build C file (release)\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"build\",\n"
                                  "        \"release\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--compiler=${input:compiler}\",\n"
                                  "        \"--std=${input:cStandard}\"\n"
                                  "      ],\n"
                                  "      \"group\": \"build\",\n"
                                  "      \"problemMatcher\": [\"$gcc\"]\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"label\": \"dh>build C file (optimized)\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"build\",\n"
                                  "        \"optimize\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--compiler=${input:compiler}\",\n"
                                  "        \"--std=${input:cStandard}\",\n"
                                  "        \"--opt=${input:optimization}\"\n"
                                  "      ],\n"
                                  "      \"group\": \"build\",\n"
                                  "      \"problemMatcher\": [\"$gcc\"]\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"label\": \"dh>run debug build\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"run\",\n"
                                  "        \"debug\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--args=\\\"${input:runArgs}\\\"\"\n"
                                  "      ],\n"
                                  "      \"options\": {\n"
                                  "        \"cwd\": \"${fileDirname}\"\n"
                                  "      },\n"
                                  "      \"group\": \"test\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"label\": \"dh>run release build\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"run\",\n"
                                  "        \"release\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--args=\\\"${input:runArgs}\\\"\"\n"
                                  "      ],\n"
                                  "      \"options\": {\n"
                                  "        \"cwd\": \"${fileDirname}\"\n"
                                  "      },\n"
                                  "      \"group\": \"test\"\n"
                                  "    },\n"
                                  "    {\n"
                                  "      \"label\": \"dh>run test\",\n"
                                  "      \"type\": \"shell\",\n"
                                  "      \"command\": \"dh-c\",\n"
                                  "      \"args\": [\n"
                                  "        \"test\",\n"
                                  "        \"${file}\",\n"
                                  "        \"--args=\\\"${input:runArgs}\\\"\"\n"
                                  "      ],\n"
                                  "      \"options\": {\n"
                                  "        \"cwd\": \"${fileDirname}\"\n"
                                  "      },\n"
                                  "      \"group\": \"test\"\n"
                                  "    }\n"
                                  "  ]\n"
                                  "}\n";

// Cross-platform wrapper for getting absolute path
bool get_absolute_path(const char* path, char* resolved_path, size_t resolved_path_size) {
#ifdef _WIN32
    return _fullpath(resolved_path, path, resolved_path_size) != NULL;
#else
    return realpath(path, resolved_path) != NULL;
#endif
}

// Helper function to check if a directory exists
bool dir_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

// Helper function to check if a file exists
bool file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

// Recursively find all C source files in a directory
void find_source_files(const char* dir_path, char*** sources, int* source_count) {
    DIR* dir = opendir(dir_path);
    if (!dir) {
        return;
    }

    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Skip hidden files and . / ..
        }

        char path[1024];
        int  result = snprintf(path, sizeof(path), "%s%s%s", dir_path, PATH_SEPARATOR, entry->d_name);
        if (result < 0 || (size_t)result >= sizeof(path)) {
            continue; // Path too long, skip
        }

        struct stat st;
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recurse into subdirectory
                find_source_files(path, sources, source_count);
            } else if (S_ISREG(st.st_mode)) {
                // Check if it's a C source file
                const char* ext = strrchr(entry->d_name, '.');
                if (ext && strcmp(ext, ".c") == 0) {
                    // Add to sources array
                    *sources = realloc(*sources, (*source_count + 1) * sizeof(char*));
                    if (*sources == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        closedir(dir);
                        exit(1);
                    }
                    (*sources)[*source_count] = strdup(path);
                    if ((*sources)[*source_count] == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        closedir(dir);
                        exit(1);
                    }
                    (*source_count)++;
                }
            }
        }
    }
    closedir(dir);
}

// Detect if clang is available
bool detect_clang() {
    FILE* pipe = popen("clang --version", "r");
    if (!pipe) {
        return false;
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        pclose(pipe);
        return true;
    }

    pclose(pipe);
    return false;
}

// Detect project structure and fill config
void detect_project_structure(BuildConfig* config) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "Error: Could not get current working directory\n");
        exit(1);
    }

    // Find the project root by looking for include/ and src/ directories
    char* last_slash = strrchr(cwd, PATH_SEPARATOR[0]);
    if (last_slash) {
        strncpy(config->project_name, last_slash + 1, sizeof(config->project_name) - 1);
    } else {
        strncpy(config->project_name, cwd, sizeof(config->project_name) - 1);
    }

    strcpy(config->project_root, cwd);

    printf("Detected project: %s at %s\n", config->project_name, config->project_root);

    // Check for required directories
    char include_path[1024];
    char src_path[1024];
    char libs_path[1024];

    snprintf(include_path, sizeof(include_path), "%s%sinclude", config->project_root, PATH_SEPARATOR);
    snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);
    snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

    if (!dir_exists(include_path)) {
        fprintf(stderr, "Error: include/ directory not found\n");
        exit(1);
    }

    if (!dir_exists(src_path)) {
        fprintf(stderr, "Error: src/ directory not found\n");
        exit(1);
    }

    // Check for optional libs directory and link.dhc
    char link_file[1024];
    snprintf(link_file, sizeof(link_file), "%s%slink.dhc", libs_path, PATH_SEPARATOR);

    if (dir_exists(libs_path) && file_exists(link_file)) {
        parse_libraries(config, link_file);
    }

    // Create output directory
    char output_path[1024];
    snprintf(output_path, sizeof(output_path), "%s%sbuild", config->project_root, PATH_SEPARATOR);

    if (!dir_exists(output_path)) {
        if (mkdir(output_path, 0755) != 0) {
            fprintf(stderr, "Warning: Could not create build directory\n");
        }
    }

    strcpy(config->output_dir, output_path);
}

// Parse the libraries file
void parse_libraries(BuildConfig* config, char* libs_file) {
    FILE* file = fopen(libs_file, "r");
    if (!file) {
        return;
    }

    config->linked_libraries[0] = '\0';

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\r\n")] = 0;

        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\0') {
            continue;
        }

        // Add to linked libraries
        if (config->linked_libraries[0] != '\0') {
            strcat(config->linked_libraries, " ");
        }
        strcat(config->linked_libraries, "-l");
        strcat(config->linked_libraries, line);
    }

    fclose(file);
}

// Add function to find dh path via environment variable
bool find_dh_path_from_env(BuildConfig* config) {
    char* dh_home = getenv("DH_HOME");
    if (dh_home && strlen(dh_home) > 0) {
        // Verify the path has the necessary structure
        char include_dir[1024];
        int  result = snprintf(include_dir, sizeof(include_dir), "%s%sinclude", dh_home, PATH_SEPARATOR);
        if (result >= 0 && (size_t)result < sizeof(include_dir) && dir_exists(include_dir)) {
            strcpy(config->dh_path, dh_home);
            printf("Using DH library from environment variable: %s\n", config->dh_path);
            return true;
        }
    }
    return false;
}

// Update find_dh_path to also check environment variable
bool find_dh_path(BuildConfig* config) {
    // First try to find from environment variable
    if (find_dh_path_from_env(config)) {
        return true;
    }

    // Existing code to find via standard locations
    // Try current path first
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        return false;
    }

    // Check if current directory is 'dh'
    char* last_dir = strrchr(cwd, PATH_SEPARATOR[0]);
    if (last_dir && strcmp(last_dir + 1, "dh") == 0) {
        strcpy(config->dh_path, cwd);
        return true;
    }

    // Check if we're in a subdirectory of dh
    char temp_path[1024];
    strcpy(temp_path, cwd);

    while (strlen(temp_path) > 3) { // Minimum path length (e.g., C:\ on Windows)
        char* last_separator = strrchr(temp_path, PATH_SEPARATOR[0]);
        if (!last_separator) {
            break;
        }

        // Truncate to parent directory
        *last_separator = '\0';

        // Check if parent is 'dh'
        char* parent_dir = strrchr(temp_path, PATH_SEPARATOR[0]);
        if (parent_dir && strcmp(parent_dir + 1, "dh") == 0) {
            strcpy(config->dh_path, temp_path);
            return true;
        }
    }

    // Try to locate dh in standard locations
    const char* standard_locations[] = {
        "C:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh",
        "../dh",
        "../../dh",
        "../../../dh"
    };

    for (size_t i = 0; i < sizeof(standard_locations) / sizeof(standard_locations[0]); i++) {
        if (dir_exists(standard_locations[i])) {
            char include_dir[1024];
            snprintf(include_dir, sizeof(include_dir), "%s%sinclude", standard_locations[i], PATH_SEPARATOR);

            if (dir_exists(include_dir)) {
                strcpy(config->dh_path, standard_locations[i]);
                return true;
            }
        }
    }

    return false;
}

// Check if a file is a valid C source file
bool is_c_source_file(const char* filename) {
    const char* ext = strrchr(filename, '.');
    return ext && strcmp(ext, ".c") == 0;
}

// Detect if we're building a single file or a project
void detect_build_type(BuildConfig* config, int argc, char* argv[]) {
    config->is_single_file = false;

    // Check if a filename was provided as an argument
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && is_c_source_file(argv[i]) && file_exists(argv[i])) {
            config->is_single_file = true;

            // Use cross-platform wrapper instead of realpath
            if (!get_absolute_path(argv[i], config->single_file, sizeof(config->single_file))) {
                // Fallback if get_absolute_path fails
                strcpy(config->single_file, argv[i]);
            }

            // Extract the base name for the output
            char* filename = strrchr(config->single_file, PATH_SEPARATOR[0]);
            if (filename) {
                filename++; // Skip the separator
            } else {
                filename = config->single_file;
            }

            // Remove the .c extension to get the project name
            char basename[256];
            strcpy(basename, filename);
            char* dot = strrchr(basename, '.');
            if (dot) {
                *dot = '\0';
            }

            strcpy(config->project_name, basename);

            // Set the project root to the file's directory
            strcpy(config->project_root, config->single_file);
            char* last_separator = strrchr(config->project_root, PATH_SEPARATOR[0]);
            if (last_separator) {
                *(last_separator + 1) = '\0';
            }

            // Set output directory to the same as source
            strcpy(config->output_dir, config->project_root);
            return;
        }
    }

    // If no file found, assume project build
    // (detect_project_structure will be called separately)
}

// Add DH-specific includes and libraries
void add_dh_includes(BuildConfig* config, char*** includes, int* include_count) {
    // Check if dh_path is found
    if (strlen(config->dh_path) == 0) {
        if (!find_dh_path(config)) {
            fprintf(stderr, "Warning: Could not find DH library path\n");
            return;
        }
    }

    // Add DH includes
    char dh_include[1024];
    snprintf(dh_include, sizeof(dh_include), "%s%sinclude", config->dh_path, PATH_SEPARATOR);

    if (dir_exists(dh_include)) {
        *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = strdup(dh_include);
        (*include_count)++;
    }

    // Add DH root
    *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
    (*includes)[*include_count] = strdup(config->dh_path);
    (*include_count)++;

    // Add BlocksRuntime include if it exists
    char blocks_include[1024];
    snprintf(blocks_include, sizeof(blocks_include), "%s%slibs%sBlocksRuntime%sinclude", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (dir_exists(blocks_include)) {
        *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = strdup(blocks_include);
        (*include_count)++;
    }
}

// Add DH-specific source files
void add_dh_sources(BuildConfig* config, char*** sources, int* source_count) {
    // Check if dh_path is found
    if (strlen(config->dh_path) == 0) {
        if (!find_dh_path(config)) {
            fprintf(stderr, "Warning: Could not find DH library path\n");
            return;
        }
    }

    // Add DH source files
    char dh_src_glob[1024];
    snprintf(dh_src_glob, sizeof(dh_src_glob), "%s%ssrc%s*.c", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR);

    // Find dh source files
    char dh_src_dir[1024];
    snprintf(dh_src_dir, sizeof(dh_src_dir), "%s%ssrc", config->dh_path, PATH_SEPARATOR);

    if (dir_exists(dh_src_dir)) {
        find_source_files(dh_src_dir, sources, source_count);
    }

    // Add BlocksRuntime source files if they exist
    char blocks_src_dir[1024];
    snprintf(blocks_src_dir, sizeof(blocks_src_dir), "%s%slibs%sBlocksRuntime%ssrc", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (dir_exists(blocks_src_dir)) {
        // Add data.c and runtime.c explicitly
        char data_c[1024], runtime_c[1024];
        snprintf(data_c, sizeof(data_c), "%s%sdata.c", blocks_src_dir, PATH_SEPARATOR);
        snprintf(runtime_c, sizeof(runtime_c), "%s%sruntime.c", blocks_src_dir, PATH_SEPARATOR);

        if (file_exists(data_c)) {
            *sources                  = realloc(*sources, (*source_count + 1) * sizeof(char*));
            (*sources)[*source_count] = strdup(data_c);
            (*source_count)++;
        }

        if (file_exists(runtime_c)) {
            *sources                  = realloc(*sources, (*source_count + 1) * sizeof(char*));
            (*sources)[*source_count] = strdup(runtime_c);
            (*source_count)++;
        }
    }
}

// Collect all include paths
void collect_include_paths(BuildConfig* config, char*** includes, int* include_count) {
    *includes      = NULL;
    *include_count = 0;

    // Always add DH includes
    add_dh_includes(config, includes, include_count);

    if (config->is_single_file) {
        // Add the directory of the single file
        char* dir_path       = strdup(config->single_file);
        char* last_separator = strrchr(dir_path, PATH_SEPARATOR[0]);
        if (last_separator) {
            *(last_separator + 1) = '\0';
        }

        *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = dir_path;
        (*include_count)++;

        // Add ./include if it exists relative to the file
        char include_dir[1024];
        snprintf(include_dir, sizeof(include_dir), "%sinclude", dir_path);

        if (dir_exists(include_dir)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_dir);
            (*include_count)++;
        }
    } else {
        // Add standard include paths for a project
        char include_path[1024];
        snprintf(include_path, sizeof(include_path), "%s%sinclude", config->project_root, PATH_SEPARATOR);

        if (dir_exists(include_path)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_path);
            (*include_count)++;
        }

        // Add src path for internal includes
        char src_path[1024];
        snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

        if (dir_exists(src_path)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(src_path);
            (*include_count)++;
        }

        // Add libs paths if they exist
        char libs_path[1024];
        snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

        if (dir_exists(libs_path)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(libs_path);
            (*include_count)++;

            // Scan for subdirectories in libs
            DIR* dir = opendir(libs_path);
            if (dir) {
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] == '.') {
                        continue; // Skip hidden files and . / ..
                    }

                    char subdir_path[1024];
                    snprintf(subdir_path, sizeof(subdir_path), "%s%s%s", libs_path, PATH_SEPARATOR, entry->d_name);

                    struct stat st;
                    if (stat(subdir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                        // Check for include directory
                        char lib_include[1024];
                        snprintf(lib_include, sizeof(lib_include), "%s%sinclude", subdir_path, PATH_SEPARATOR);

                        if (dir_exists(lib_include)) {
                            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
                            (*includes)[*include_count] = strdup(lib_include);
                            (*include_count)++;
                        }
                    }
                }
                closedir(dir);
            }
        }
    }
}

// Collect all source files
void collect_source_files(BuildConfig* config, char*** sources, int* source_count) {
    *sources      = NULL;
    *source_count = 0;

    if (config->is_single_file) {
        // Add just the single file
        *sources                  = realloc(*sources, (*source_count + 1) * sizeof(char*));
        (*sources)[*source_count] = strdup(config->single_file);
        (*source_count)++;
    } else {
        // For project builds, collect all source files
        char src_path[1024];
        snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

        if (dir_exists(src_path)) {
            find_source_files(src_path, sources, source_count);
        }

        // Also check for source files in libs
        char libs_path[1024];
        snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

        if (dir_exists(libs_path)) {
            DIR* dir = opendir(libs_path);
            if (dir) {
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] == '.') {
                        continue; // Skip hidden files and . / ..
                    }

                    char subdir_path[1024];
                    snprintf(subdir_path, sizeof(subdir_path), "%s%s%s", libs_path, PATH_SEPARATOR, entry->d_name);

                    struct stat st;
                    if (stat(subdir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                        // Check for src directory
                        char lib_src[1024];
                        snprintf(lib_src, sizeof(lib_src), "%s%ssrc", subdir_path, PATH_SEPARATOR);

                        if (dir_exists(lib_src)) {
                            find_source_files(lib_src, sources, source_count);
                        }
                    }
                }
                closedir(dir);
            }
        }
    }

    // Always add DH sources for any build
    add_dh_sources(config, sources, source_count);
}

// Build the project
void build_project(BuildConfig* config) {
    char** includes      = NULL;
    int    include_count = 0;

    char** sources      = NULL;
    int    source_count = 0;

    // Collect include paths and source files
    collect_include_paths(config, &includes, &include_count);
    collect_source_files(config, &sources, &source_count);

    // Prepare command
    char command[8192] = { 0 };
    strcat(command, config->compiler);

    // Add standard
    strcat(command, " -std=");
    strcat(command, config->c_standard);

    // Add include paths
    for (int i = 0; i < include_count; i++) {
        strcat(command, " -I");
        strcat(command, includes[i]);
    }

    // Add compile flags
    strcat(command, " -DCOMP");
    if (config->is_test) {
        strcat(command, " -DCOMP_TEST");
    }

    strcat(command, " -Wall -Wextra -funsigned-char -fblocks");

    // Add mode-specific flags
    if (strcmp(config->build_mode, "debug") == 0) {
        strcat(command, " -g -O0");
    } else if (strcmp(config->build_mode, "release") == 0) {
        strcat(command, " -O2 -DNDEBUG");
    } else if (strcmp(config->build_mode, "optimize") == 0) {
        strcat(command, " ");
        strcat(command, config->optimization_level);
        strcat(command, " -DNDEBUG");
    }

    // Add source files
    for (int i = 0; i < source_count; i++) {
        strcat(command, " ");
        strcat(command, sources[i]);
    }

    // Add output file
    char output_file[1024];
    snprintf(output_file, sizeof(output_file), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->project_name);

    if (strcmp(config->build_mode, "debug") == 0) {
        strcat(output_file, "-debug");
    } else if (strcmp(config->build_mode, "optimize") == 0) {
        strcat(output_file, "-opt");
    }

#ifdef _WIN32
    strcat(output_file, ".exe");
#endif

    strcat(command, " -o ");
    strcat(command, output_file);

    // Add linked libraries
    if (config->linked_libraries[0] != '\0') {
        strcat(command, " ");
        strcat(command, config->linked_libraries);
    }

    strcat(command, " -static");

    // Execute build command
    printf("Building %s in %s mode...\n", config->project_name, config->build_mode);
    printf("Command: %s\n", command);

    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Build failed with error code: %d\n", result);
        exit(1);
    }

    printf("Build successful!\n");

    // Clean up
    free_string_array(includes, include_count);
    free_string_array(sources, source_count);
}

// Run the compiled executable
void run_executable(BuildConfig* config) {
    char output_file[1024];
    snprintf(output_file, sizeof(output_file), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->project_name);

    if (strcmp(config->build_mode, "debug") == 0) {
        strcat(output_file, "-debug");
    } else if (strcmp(config->build_mode, "optimize") == 0) {
        strcat(output_file, "-opt");
    }

#ifdef _WIN32
    strcat(output_file, ".exe");
#endif

    if (!file_exists(output_file)) {
        fprintf(stderr, "Error: Executable not found: %s\n", output_file);
        exit(1);
    }

    char command[2048];
    snprintf(command, sizeof(command), "\"%s\" %s", output_file, config->run_args);

    printf("Running: %s\n", command);
    system(command);
}

// Free memory from string array
void free_string_array(char** array, int count) {
    if (!array) {
        return;
    }

    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}

// Create project-specific .clangd with absolute paths
void create_project_clangd(const char* project_path, const char* dh_path) {
    char clangd_path[1024];
    int  result = snprintf(clangd_path, sizeof(clangd_path), "%s%s.clangd", project_path, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(clangd_path)) {
        fprintf(stderr, "Error: Path too long for .clangd\n");
        return;
    }

    char abs_include_path[1024], abs_src_path[1024], dh_include_path[1024], blocks_include_path[1024];
    char temp_path[1024];

    // Get absolute paths
    snprintf(temp_path, sizeof(temp_path), "%s%sinclude", project_path, PATH_SEPARATOR);
    if (!get_absolute_path(temp_path, abs_include_path, sizeof(abs_include_path))) {
        fprintf(stderr, "Warning: Could not resolve absolute path for include directory\n");
        strcpy(abs_include_path, temp_path);
    }

    snprintf(temp_path, sizeof(temp_path), "%s%ssrc", project_path, PATH_SEPARATOR);
    if (!get_absolute_path(temp_path, abs_src_path, sizeof(abs_src_path))) {
        fprintf(stderr, "Warning: Could not resolve absolute path for src directory\n");
        strcpy(abs_src_path, temp_path);
    }

    // Get DH paths
    result = snprintf(dh_include_path, sizeof(dh_include_path), "%s%sinclude", dh_path, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(dh_include_path)) {
        fprintf(stderr, "Error: DH include path too long\n");
        return;
    }

    result = snprintf(blocks_include_path, sizeof(blocks_include_path), "%s%slibs%sBlocksRuntime%sinclude", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(blocks_include_path)) {
        fprintf(stderr, "Error: BlocksRuntime include path too long\n");
        return;
    }

    char project_clangd[4096];
    result = snprintf(project_clangd, sizeof(project_clangd), "CompileFlags:\n"
                                                              "  Compiler: clang\n"
                                                              "  CompilationDatabase: %s\n"
                                                              "  Add:\n"
                                                              "    - -xc\n"
                                                              "    - -std=c17\n"
                                                              "    - -Wall\n"
                                                              "    - -Wextra\n"
                                                              "    - -funsigned-char\n"
                                                              "    - -fblocks\n"
                                                              "    - -I%s\n"
                                                              "    - -I%s\n"
                                                              "    - -I%s\n"
                                                              "    - -I%s\n"
                                                              "\n"
                                                              "Diagnostics:\n"
                                                              "  UnusedIncludes: Strict\n"
                                                              "  ClangTidy:\n"
                                                              "    Add: [modernize*, performance*, readability*, bugprone*, portability*]\n"
                                                              "    Remove: [modernize-use-trailing-return-type, readability-named-parameter]\n"
                                                              "\n"
                                                              "InlayHints:\n"
                                                              "  Enabled: Yes\n"
                                                              "  ParameterNames: Yes\n"
                                                              "  DeducedTypes: Yes\n"
                                                              "\n"
                                                              "Hover:\n"
                                                              "  ShowAKA: Yes\n",
                      project_path,
                      abs_include_path,
                      abs_src_path,
                      dh_include_path,
                      blocks_include_path);

    if (result < 0 || (size_t)result >= sizeof(project_clangd)) {
        fprintf(stderr, "Error: Project clangd content too long\n");
        return;
    }

    write_file(clangd_path, project_clangd);
}

// Helper function to write example files
void write_example_files(const char* project_path) {
    char main_path[1024];
    int  result = snprintf(main_path, sizeof(main_path), "%s%ssrc%smain.c", project_path, PATH_SEPARATOR, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(main_path)) {
        fprintf(stderr, "Error: Path too long for main.c\n");
        return;
    }

    const char* main_content = "/// @file  main.c\n"
                               "/// @brief Example DH-C program\n"
                               "\n"
                               "#include <dh/main.h>\n"
                               "#include <dh/Str.h>\n"
                               "\n"
                               "fn_TEST_scope(\"simple test\") {\n"
                               "    try_(TEST_expect(1 + 1 == 2));\n"
                               "} TEST_unscoped;\n"
                               "\n"
                               "fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {\n"
                               "    $ignore args;\n"
                               "    let message = Str_l(\"Hello, world!\");\n"
                               "    Str_println(message);\n"
                               "    return_ok({});\n"
                               "} ext_unscoped;\n";

    write_file(main_path, main_content);
}

// Update create_project function
void create_project(const char* name, const char* dh_path) {
    char project_path[1024] = { 0 };

    if (name == NULL || strcmp(name, ".") == 0) {
        if (getcwd(project_path, sizeof(project_path)) == NULL) {
            fprintf(stderr, "Error: Could not get current working directory\n");
            return;
        }
    } else {
        if (strlen(name) >= sizeof(project_path)) {
            fprintf(stderr, "Error: Project name too long\n");
            return;
        }
        strcpy(project_path, name);
        if (!create_directory(project_path)) {
            fprintf(stderr, "Error: Could not create project directory '%s'\n", name);
            return;
        }
    }

    printf("Creating project in: %s\n", project_path);

    // Create standard project structure
    char include_path[1024], src_path[1024], tests_path[1024], libs_path[1024];

    int result = snprintf(include_path, sizeof(include_path), "%s%sinclude", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(include_path)) {
        create_directory(include_path);
    }

    result = snprintf(src_path, sizeof(src_path), "%s%ssrc", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(src_path)) {
        create_directory(src_path);
    }

    result = snprintf(tests_path, sizeof(tests_path), "%s%stests", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(tests_path)) {
        create_directory(tests_path);
    }

    result = snprintf(libs_path, sizeof(libs_path), "%s%slibs", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(libs_path)) {
        create_directory(libs_path);
    }

    // Create .clangd file
    create_project_clangd(project_path, dh_path);

    // Create example files
    write_example_files(project_path);

    printf("Project created successfully with example files!\n");
}

// Update create_workspace function
void create_workspace(const char* name, const char* dh_path) {
    char workspace_path[1024] = { 0 };

    if (name == NULL || strcmp(name, ".") == 0) {
        // Use current directory
        if (getcwd(workspace_path, sizeof(workspace_path)) == NULL) {
            fprintf(stderr, "Error: Could not get current working directory\n");
            return;
        }
    } else {
        // Create a new directory with the given name
        strcpy(workspace_path, name);
        if (!create_directory(workspace_path)) {
            fprintf(stderr, "Error: Could not create workspace directory '%s'\n", name);
            return;
        }
    }

    printf("Creating workspace in: %s\n", workspace_path);

    // Create .clangd file
    char clangd_path[1024];
    snprintf(clangd_path, sizeof(clangd_path), "%s%s.clangd", workspace_path, PATH_SEPARATOR);
    write_file(clangd_path, CLANGD_TEMPLATE);

    // Create .clang-format file
    char clang_format_path[1024];
    snprintf(clang_format_path, sizeof(clang_format_path), "%s%s.clang-format", workspace_path, PATH_SEPARATOR);
    write_file(clang_format_path, CLANG_FORMAT_TEMPLATE);

    // Create .vscode directory
    char vscode_path[1024];
    snprintf(vscode_path, sizeof(vscode_path), "%s%s.vscode", workspace_path, PATH_SEPARATOR);
    if (!create_directory(vscode_path)) {
        fprintf(stderr, "Warning: Could not create .vscode directory\n");
    } else {
        // Create tasks.json file
        char tasks_path[1024];
        snprintf(tasks_path, sizeof(tasks_path), "%s%stasks.json", vscode_path, PATH_SEPARATOR);
        write_file(tasks_path, TASKS_JSON_TEMPLATE);
    }

    // Create an example project in the workspace
    char example_project_path[1024];
    int  result = snprintf(example_project_path, sizeof(example_project_path), "%s%shello-world", workspace_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(example_project_path)) {
        create_project(example_project_path, dh_path);
    } else {
        fprintf(stderr, "Warning: Could not create example project (path too long)\n");
    }

    printf("Workspace created successfully!\n");
}

// Update print_usage function to include version command
void print_usage() {
    printf("Usage: dh-c [command] [options]\n\n");
    printf("Commands:\n");
    printf("  --version            - Display version information\n");
    printf("  build               - Build the project or file\n");
    printf("  test                - Build and run tests\n");
    printf("  run                 - Build and run the project or file\n");
    printf("  workspace <.|name>  - Create workspace with config files (.clangd, .clang-format, .vscode)\n");
    printf("  project <.|name>    - Create a new project with DH-C structure\n\n");
    printf("Build modes:\n");
    printf("  debug    - Debug build with no optimization\n");
    printf("  release  - Release build with -O2 optimization\n");
    printf("  optimize - Optimized build (default: -O3)\n\n");
    printf("Options:\n");
    printf("  --compiler=<clang|gcc>  - Specify compiler (default: clang)\n");
    printf("  --std=<c99|c11|c17>     - Specify C standard (default: c17)\n");
    printf("  --opt=<O0|O1|O2|O3|Os>  - Specify optimization level for optimize mode\n");
    printf("  --args=\"args\"           - Arguments to pass when running\n");
    printf("  --dh=<path>             - Path to DH library (auto-detected by default)\n\n");
    printf("Environment Variables:\n");
    printf("  DH_HOME               - Path to DH library installation\n\n");
    printf("Examples:\n");
    printf("  dh-c --version                    - Show version information\n");
    printf("  dh-c build debug                  - Build project in debug mode\n");
    printf("  dh-c run release sample.c         - Build and run single file in release mode\n");
    printf("  dh-c test sample.c                - Build and run tests for single file\n");
    printf("  dh-c build optimize --opt=O2 file.c - Build file with O2 optimization\n");
    printf("  dh-c workspace myworkspace        - Create new workspace named 'myworkspace'\n");
    printf("  dh-c project myproject            - Create new project named 'myproject'\n");
}

// Update main function to handle version command
int main(int argc, char* argv[]) {
    // Default configuration
    BuildConfig config;
    memset(&config, 0, sizeof(BuildConfig));

    strcpy(config.compiler, "clang");
    strcpy(config.c_standard, "c17");
    strcpy(config.optimization_level, "-O3");
    strcpy(config.build_mode, "release"); // Default mode
    config.is_test        = false;
    config.is_single_file = false;

    // Parse command-line arguments
    if (argc < 2) {
        print_usage();
        return 1;
    }

    // Handle version command first (now using --version format)
    if (strcmp(argv[1], "--version") == 0) {
        printf("DH-C Build Tool version %s\n", DH_VERSION);
        printf("Copyright (c) 2024-2025 Gyeongtae Kim\n");
        printf("Licensed under the MIT License\n");
        return 0;
    }

    // Try to find DH path early for workspace and project commands
    if (!find_dh_path(&config)) {
        fprintf(stderr, "Warning: Could not find DH library path\n");
        return 1;
    }

    // Create bin directory if it doesn't exist
    char bin_path[1024];
    int  result = snprintf(bin_path, sizeof(bin_path), "%s%sbin", config.dh_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(bin_path)) {
        if (!dir_exists(bin_path)) {
            if (!create_directory(bin_path)) {
                fprintf(stderr, "Warning: Could not create bin directory\n");
            }
        }
    }

    // Handle workspace and project commands
    if (strcmp(argv[1], "workspace") == 0) {
        const char* name = (argc > 2) ? argv[2] : ".";
        create_workspace(name, config.dh_path);
        return 0;
    } else if (strcmp(argv[1], "project") == 0) {
        const char* name = (argc > 2) ? argv[2] : ".";
        create_project(name, config.dh_path);
        return 0;
    }

    // For standard build commands, continue with existing logic
    // Check compiler availability
    if (!detect_clang()) {
        fprintf(stderr, "Warning: clang compiler not found. Using gcc as fallback.\n");
        strcpy(config.compiler, "gcc");
    }

    // Process the command first
    if (strcmp(argv[1], "build") == 0) {
        // build only
    } else if (strcmp(argv[1], "test") == 0) {
        config.is_test = true;
    } else if (strcmp(argv[1], "run") == 0) {
        // build and run
    } else if (!is_c_source_file(argv[1]) || !file_exists(argv[1])) {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        print_usage();
        return 1;
    }

    // Then check if we're building a single file or a project
    detect_build_type(&config, argc, argv);

    // Parse additional options
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--compiler=", 11) == 0) {
            strcpy(config.compiler, argv[i] + 11);
        } else if (strncmp(argv[i], "--std=", 6) == 0) {
            strcpy(config.c_standard, argv[i] + 6);
        } else if (strncmp(argv[i], "--opt=", 6) == 0) {
            sprintf(config.optimization_level, "-%s", argv[i] + 6);
        } else if (strncmp(argv[i], "--args=", 7) == 0) {
            strcpy(config.run_args, argv[i] + 7);
        } else if (strncmp(argv[i], "--dh=", 5) == 0) {
            // Allow user to specify DH path
            strcpy(config.dh_path, argv[i] + 5);
        }
    }

    // If not single file, detect project structure
    if (!config.is_single_file) {
        detect_project_structure(&config);
    }

    // Try to find DH if not specified
    if (strlen(config.dh_path) == 0) {
        find_dh_path(&config);
    }

    // Build the project
    build_project(&config);

    // Run tests or executable if requested
    if (strcmp(argv[1], "test") == 0 || strcmp(argv[1], "run") == 0) {
        run_executable(&config);
    }

    return 0;
}

// Helper function to write a file
void write_file(const char* path, const char* content) {
    FILE* file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not create file: %s\n", path);
        return;
    }

    size_t content_len = strlen(content);
    size_t written     = fwrite(content, 1, content_len, file);
    if (written != content_len) {
        fprintf(stderr, "Error: Could not write complete content to file: %s\n", path);
        fclose(file);
        return;
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file: %s\n", path);
        return;
    }

    printf("Created file: %s\n", path);
}

// Helper function to create a directory
bool create_directory(const char* path) {
    if (dir_exists(path)) {
        return true; // Directory already exists
    }

#ifdef _WIN32
    if (_mkdir(path) != 0) {
        fprintf(stderr, "Error: Could not create directory: %s\n", path);
        return false;
    }
#else
    if (mkdir(path, 0755) != 0) {
        fprintf(stderr, "Error: Could not create directory: %s\n", path);
        return false;
    }
#endif

    printf("Created directory: %s\n", path);
    return true;
}
