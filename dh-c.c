/**
 * dh-c - Build tool for DH-C projects
 *
 * Compilation:
 * - clang -std=c17 -Wall -Wextra -O3 -o dh-c dh-c.c -static
 *
 * Features:
 * - Detects clang toolchain
 * - Supports workspace and project creation
 * - Supports build, test, and run commands
 * - Supports various build configurations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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

#define DH_C_VERSION \
    /** \
     * Version number \
     * major.minor.patch-prerelease.minor.patch+build.number \
     */ \
    "0.1.0-alpha.0.3.1"

// Debug level enum
typedef enum {
    DEBUG_NONE,     // No debug info
    DEBUG_MINIMAL,  // -g1: Line tables only
    DEBUG_STANDARD, // -g: Standard debug info
    DEBUG_EXTENDED  // -g3: Extra debug info
} DebugLevel;

// Optimization level enum
typedef enum {
    OPT_NONE,        // -O0: No optimization
    OPT_BASIC,       // -O1: Basic optimizations
    OPT_BALANCED,    // -O2: Good balance
    OPT_AGGRESSIVE,  // -O3: Maximum performance
    OPT_SIZE,        // -Os: Optimize for size
    OPT_SIZE_EXTREME // -Oz: Size at all costs
} OptimizationLevel;

// Extra flags array
typedef struct {
    char** flags;
    int    count;
} ExtraFlags;

// Library configuration for lib.dh
typedef struct {
    char library_name[256];
    char linking_type[16]; // "static" or "dynamic"
    char library_path[1024];
    char profile[32]; // Build profile: "dev", "release", "performance", etc.
    bool no_libdh;    // Flag to disable dh support for this library
} LibraryConfig;

// Array of libraries from lib.dh
typedef struct {
    LibraryConfig* libraries;
    int            count;
} LibraryConfigArray;

// Configuration
typedef struct {
    char               project_name[256];
    char               project_root[1024];
    char               compiler[64];
    char               c_standard[8];
    char               output_dir[1024];
    bool               is_test;
    DebugLevel         debug_level;
    OptimizationLevel  optimization_level;
    bool               assertions_enabled;
    ExtraFlags         extra_flags;
    char               linked_libraries[1024];
    char               run_args[1024];
    char               compiler_args[1024]; // Additional arguments for compiler
    bool               is_single_file;
    char               single_file[1024];
    char               dh_path[1024];
    bool               no_libdh;              // Flag to skip DH library
    char               build_config_name[32]; // Name of selected build configuration
    bool               show_commands;         // Flag to control printing of commands
    bool               verbose;               // Flag for verbose logging
    bool               use_output_suffix;     // Flag to enable output suffix (dev, release, etc.)
    LibraryConfigArray lib_configs;           // Libraries from lib.dh
} BuildConfig;

// Build configuration presets
typedef struct {
    char              name[32];
    DebugLevel        debug_level;
    OptimizationLevel optimization_level;
    bool              assertions_enabled;
    ExtraFlags        extra_flags;
} BuildConfigPreset;

// Function prototypes
bool        detect_clang();
void        detect_project_structure(BuildConfig* config);
void        collect_source_files(BuildConfig* config, char*** sources, int* source_count);
void        collect_include_paths(BuildConfig* config, char*** includes, int* include_count);
void        parse_libraries(BuildConfig* config, char* libs_file);
void        build_project(BuildConfig* config);
void        run_executable(BuildConfig* config);
void        print_usage();
void        free_string_array(char** array, int count);
bool        find_dh_path(BuildConfig* config);
bool        is_c_source_file(const char* filename);
void        detect_build_type(BuildConfig* config, int argc, const char* argv[]);
void        add_dh_includes(BuildConfig* config, char*** includes, int* include_count);
void        add_dh_sources(BuildConfig* config, char*** sources, int* source_count);
void        create_workspace(const char* name, const char* dh_path);
void        create_project(const char* name, const char* dh_path);
void        write_file(const char* path, const char* content);
bool        create_directory(const char* path);
bool        find_dh_path_from_env(BuildConfig* config);
void        write_example_files(const char* project_path);
void        create_project_clangd(const char* project_path, const char* dh_path);
void        apply_build_preset(BuildConfig* config, const char* preset_name);
const char* debug_level_to_flag(DebugLevel level);
const char* optimization_level_to_flag(OptimizationLevel level);
void        init_build_config(BuildConfig* config);
void        free_extra_flags(ExtraFlags* flags);
bool        get_executable_path(char* buffer, size_t size);
bool        check_blocksruntime_lib(const char* dh_path, char* out_lib_path, size_t path_size);
bool        compile_blocksruntime_lib(const char* dh_path, const char* out_lib_path, bool verbose);

// Library management functions
bool  parse_lib_dh_file(BuildConfig* config, const char* lib_dh_path);
bool  compile_libraries(BuildConfig* config);
bool  compile_single_library(BuildConfig* config, const LibraryConfig* lib_config);
bool  copy_library_includes(const char* lib_src_path, const char* project_lib_path, const char* lib_name, bool verbose);
void  add_lib_dh_includes(BuildConfig* config, char*** includes, int* include_count);
void  add_lib_dh_libraries(BuildConfig* config);
void  free_library_configs(LibraryConfigArray* lib_configs);
bool  create_library_command(BuildConfig* config);
char* optimization_level_to_lib_flag(const char* opt_level);

// Add forward declarations for functions
void init_build_presets(void);
void cleanup_build_presets(void);
bool load_template_file(const char* filename, char** template_content, bool verbose);
void initialize_templates(const char* dh_path, bool verbose);
void cleanup_templates(void);
void create_templates_directory(const char* dh_path, bool verbose);

// Predefined build configurations
BuildConfigPreset build_presets[] = {
    { .name               = "dev",
      .debug_level        = DEBUG_EXTENDED,
      .optimization_level = OPT_NONE,
      .assertions_enabled = true,
      .extra_flags        = { NULL, 0 } },
    { .name               = "test",
      .debug_level        = DEBUG_STANDARD,
      .optimization_level = OPT_BASIC,
      .assertions_enabled = true,
      .extra_flags        = { NULL, 0 } },
    { .name               = "profile",
      .debug_level        = DEBUG_STANDARD,
      .optimization_level = OPT_BALANCED,
      .assertions_enabled = true,
      .extra_flags        = { NULL, 0 } },
    { .name               = "release",
      .debug_level        = DEBUG_MINIMAL,
      .optimization_level = OPT_BALANCED,
      .assertions_enabled = false,
      .extra_flags        = { NULL, 0 } },
    { .name               = "performance",
      .debug_level        = DEBUG_NONE,
      .optimization_level = OPT_AGGRESSIVE,
      .assertions_enabled = false,
      .extra_flags        = { NULL, 0 } },
    { .name               = "embedded",
      .debug_level        = DEBUG_NONE,
      .optimization_level = OPT_SIZE,
      .assertions_enabled = false,
      .extra_flags        = { NULL, 0 } },
    { .name               = "micro",
      .debug_level        = DEBUG_NONE,
      .optimization_level = OPT_SIZE_EXTREME,
      .assertions_enabled = false,
      .extra_flags        = { NULL, 0 } }
};

const int NUM_BUILD_PRESETS = sizeof(build_presets) / sizeof(BuildConfigPreset);

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
    struct stat st = {};
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

// Helper function to check if a file exists
bool file_exists(const char* path) {
    struct stat st = {};
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

// Recursively find all C source files in a directory
void find_source_files(const char* dir_path, char*** sources, int* source_count) { /* NOLINT(misc-no-recursion) */
    DIR* dir = opendir(dir_path);
    if (!dir) {
        return;
    }

    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Skip hidden files and . / ..
        }

        char path[1024] = {};
        int  result     = snprintf(path, sizeof(path), "%s%s%s", dir_path, PATH_SEPARATOR, entry->d_name);
        if (result < 0 || (size_t)result >= sizeof(path)) {
            continue; // Path too long, skip
        }

        struct stat st = {};
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
                        (void)fprintf(stderr, "Memory allocation failed\n");
                        closedir(dir);
                        exit(1);
                    }
                    (*sources)[*source_count] = strdup(path);
                    if ((*sources)[*source_count] == NULL) {
                        (void)fprintf(stderr, "Memory allocation failed\n");
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

    char buffer[128] = {};
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        pclose(pipe);
        return true;
    }

    pclose(pipe);
    return false;
}

// Detect project structure and fill config
void detect_project_structure(BuildConfig* config) {
    char cwd[1024] = {};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        (void)fprintf(stderr, "Error: Could not get current working directory\n");
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
    char include_path[1024] = {};
    char src_path[1024]     = {};
    char libs_path[1024]    = {};

    (void)snprintf(include_path, sizeof(include_path), "%s%sinclude", config->project_root, PATH_SEPARATOR);
    (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);
    (void)snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

    if (!dir_exists(include_path)) {
        (void)fprintf(stderr, "Error: include/ directory not found\n");
        exit(1);
    }

    if (!dir_exists(src_path)) {
        (void)fprintf(stderr, "Error: src/ directory not found\n");
        exit(1);
    }

    // Check for optional libs directory and link.dhc
    char link_file[1024] = {};
    (void)snprintf(link_file, sizeof(link_file), "%s%slink.dhc", libs_path, PATH_SEPARATOR);

    if (dir_exists(libs_path) && file_exists(link_file)) {
        parse_libraries(config, link_file);
    }

    // Check for lib.dh file in lib directory
    char lib_dh_path[1024] = {};
    (void)snprintf(lib_dh_path, sizeof(lib_dh_path), "%s%slib.dh", config->project_root, PATH_SEPARATOR);

    if (file_exists(lib_dh_path)) {
        if (config->verbose) {
            printf("Found lib.dh file, parsing library configurations...\n");
        }
        parse_lib_dh_file(config, lib_dh_path);
    }

    // Create output directory
    char output_path[1024] = {};
    (void)snprintf(output_path, sizeof(output_path), "%s%sbuild", config->project_root, PATH_SEPARATOR);

    if (!dir_exists(output_path)) {
        if (mkdir(output_path, 0755) != 0) {
            (void)fprintf(stderr, "Warning: Could not create build directory\n");
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

    char line[1024] = {};
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

    (void)fclose(file);
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
    char cwd[1024] = {};
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
    char temp_path[1024] = {};
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
            char include_dir[1024] = {};
            (void)snprintf(include_dir, sizeof(include_dir), "%s%sinclude", standard_locations[i], PATH_SEPARATOR);

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
void detect_build_type(BuildConfig* config, int argc, const char* argv[]) {
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
            char basename[256] = {};
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
    // Skip if no_libdh flag is set
    if (config->no_libdh) {
        return;
    }

    // Check if dh_path is found
    if (strlen(config->dh_path) == 0) {
        if (!find_dh_path(config)) {
            (void)fprintf(stderr, "Warning: Could not find DH library path\n");
            return;
        }
    }

    // Add DH includes
    char dh_include[1024] = {};
    (void)snprintf(dh_include, sizeof(dh_include), "%s%sinclude", config->dh_path, PATH_SEPARATOR);

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
    char blocks_include[1024] = {};
    (void)snprintf(blocks_include, sizeof(blocks_include), "%s%slibs%sBlocksRuntime%sinclude", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (dir_exists(blocks_include)) {
        *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = strdup(blocks_include);
        (*include_count)++;
    }
}

// Add DH-specific source files
void add_dh_sources(BuildConfig* config, char*** sources, int* source_count) {
    // Skip if no_libdh flag is set
    if (config->no_libdh) {
        return;
    }

    // Check if dh_path is found
    if (strlen(config->dh_path) == 0) {
        if (!find_dh_path(config)) {
            (void)fprintf(stderr, "Warning: Could not find DH library path\n");
            return;
        }
    }

    // Add DH source files
    char dh_src_glob[1024] = {};
    (void)snprintf(dh_src_glob, sizeof(dh_src_glob), "%s%ssrc%s*.c", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR);

    // Find dh source files
    char dh_src_dir[1024] = {};
    (void)snprintf(dh_src_dir, sizeof(dh_src_dir), "%s%ssrc", config->dh_path, PATH_SEPARATOR);

    if (dir_exists(dh_src_dir)) {
        find_source_files(dh_src_dir, sources, source_count);
    }

    // For BlocksRuntime, we'll now check for the static library instead of directly adding source files
    // We'll handle linking the library separately in the build_project function
}

// Collect all include paths
void collect_include_paths(BuildConfig* config, char*** includes, int* include_count) {
    *includes      = NULL;
    *include_count = 0;

    // Always add DH includes
    add_dh_includes(config, includes, include_count);

    // Add library includes from lib.dh
    add_lib_dh_includes(config, includes, include_count);

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
        char include_dir[1024] = {};
        (void)snprintf(include_dir, sizeof(include_dir), "%sinclude", dir_path);

        if (dir_exists(include_dir)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_dir);
            (*include_count)++;
        }
    } else {
        // Add standard include paths for a project
        char include_path[1024] = {};
        (void)snprintf(include_path, sizeof(include_path), "%s%sinclude", config->project_root, PATH_SEPARATOR);

        if (dir_exists(include_path)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_path);
            (*include_count)++;
        }

        // Add src path for internal includes
        char src_path[1024] = {};
        (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

        if (dir_exists(src_path)) {
            *includes                   = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(src_path);
            (*include_count)++;
        }

        // Add libs paths if they exist
        char libs_path[1024] = {};
        (void)snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

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

                    char subdir_path[1024] = {};
                    (void)snprintf(subdir_path, sizeof(subdir_path), "%s%s%s", libs_path, PATH_SEPARATOR, entry->d_name);

                    struct stat st = {};
                    if (stat(subdir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                        // Check for include directory
                        char lib_include[1024] = {};
                        (void)snprintf(lib_include, sizeof(lib_include), "%s%sinclude", subdir_path, PATH_SEPARATOR);

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
        char src_path[1024] = {};
        (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

        if (dir_exists(src_path)) {
            find_source_files(src_path, sources, source_count);
        }

        // Also check for source files in libs
        char libs_path[1024] = {};
        (void)snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

        if (dir_exists(libs_path)) {
            DIR* dir = opendir(libs_path);
            if (dir) {
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] == '.') {
                        continue; // Skip hidden files and . / ..
                    }

                    char subdir_path[1024] = {};
                    (void)snprintf(subdir_path, sizeof(subdir_path), "%s%s%s", libs_path, PATH_SEPARATOR, entry->d_name);

                    struct stat st = {};
                    if (stat(subdir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                        // Check for src directory
                        char lib_src[1024] = {};
                        (void)snprintf(lib_src, sizeof(lib_src), "%s%ssrc", subdir_path, PATH_SEPARATOR);

                        if (dir_exists(lib_src)) {
                            find_source_files(lib_src, sources, source_count);
                        }
                    }
                }
                closedir(dir);
            }
        }
    }

    // Always add DH sources if no_libdh is false, regardless of libraries
    if (!config->no_libdh) {
        add_dh_sources(config, sources, source_count);
    }
}

// Convert debug level to compiler flag
const char* debug_level_to_flag(DebugLevel level) {
    switch (level) {
    case DEBUG_NONE:
        return "";
    case DEBUG_MINIMAL:
        return "-g1";
    case DEBUG_STANDARD:
        return "-g";
    case DEBUG_EXTENDED:
        return "-g3";
    default:
        return "-g";
    }
}

// Convert optimization level to compiler flag
const char* optimization_level_to_flag(OptimizationLevel level) {
    switch (level) {
    case OPT_NONE:
        return "-O0";
    case OPT_BASIC:
        return "-O1";
    case OPT_BALANCED:
        return "-O2";
    case OPT_AGGRESSIVE:
        return "-O3";
    case OPT_SIZE:
        return "-Os";
    case OPT_SIZE_EXTREME:
        return "-Oz";
    default:
        return "-O2";
    }
}

// Initialize default build configuration
void init_build_config(BuildConfig* config) {
    // Set defaults
    strcpy(config->compiler, "clang");
    strcpy(config->c_standard, "c17");
    config->debug_level        = DEBUG_STANDARD;
    config->optimization_level = OPT_BALANCED;
    config->assertions_enabled = true;
    config->extra_flags.flags  = NULL;
    config->extra_flags.count  = 0;
    config->is_test            = false;
    config->is_single_file     = false;
    config->no_libdh           = false;
    config->show_commands      = false;       // Default: don't show commands
    config->verbose            = false;       // Default: don't show verbose logs
    config->use_output_suffix  = false;       // Default: don't use output suffix
    strcpy(config->build_config_name, "dev"); // Default build config
    config->run_args[0]           = '\0';     // Initialize run args
    config->compiler_args[0]      = '\0';     // Initialize compiler args
    config->lib_configs.libraries = NULL;     // Initialize library configs
    config->lib_configs.count     = 0;        // Initialize library count
}

// Free the extra flags array
void free_extra_flags(ExtraFlags* flags) {
    if (flags->flags) {
        for (int i = 0; i < flags->count; ++i) {
            free(flags->flags[i]);
        }
        free(flags->flags);
        flags->flags = NULL;
        flags->count = 0;
    }
}

// Change the template strings to be pointers that will be loaded
// Replace constant definitions with declarations
extern char* CLANGD_TEMPLATE;
extern char* CLANG_FORMAT_TEMPLATE;
extern char* TASKS_JSON_TEMPLATE;

// Add fallback templates in case files can't be loaded
const char* DEFAULT_CLANGD_TEMPLATE
    = "CompileFlags:\n"
      "  Compiler: clang\n"
      "  Add: \n"
      "    - -xc\n"
      "    - -std=c17\n"
      "    - -Wall\n"
      "    - -Wextra\n"
      "    - -Werror\n"
      "    - -funsigned-char\n"
      "    - -fblocks\n"
      "Diagnostics:\n"
      "  UnusedIncludes: Strict\n"
      "  ClangTidy:\n"
      "    Add: \n"
      "      - modernize*\n"
      "      - performance*\n"
      "      - readability*\n"
      "      - bugprone*\n"
      "      - portability*\n"
      "    Remove: \n"
      "      - modernize-use-trailing-return-type\n"
      "      - readability-named-parameter\n"
      "InlayHints:\n"
      "  Enabled: true\n"
      "  TypeNameLimit: 24\n"
      "  DeducedTypes: true\n"
      "  ParameterNames: true\n"
      "  Designators: true\n"
      "  BlockEnd: false\n"
      "Hover:\n"
      "  ShowAKA: true\n"
      "---\n"
      "If:\n"
      "  PathMatch:\n"
      "    - .*\\.h\n"
      "    - .*\\.hh\n"
      "    - .*\\.hpp\n"
      "    - .*\\.hxx\n"
      "CompileFlags:\n"
      "  Add: \n"
      "    - -Wno-unused-function\n"
      "    - -Wno-unused-variable\n"
      "    - -Wno-unused-const-variable\n";

const char* DEFAULT_CLANG_FORMAT_TEMPLATE
    = "## clang-format configurator v2 https://clang-format-configurator.site/\n"
      "## see https://clang.llvm.org/docs/ClangFormatStyleOptions.html\n"
      "---\n"
      "BasedOnStyle: LLVM\n"
      "AccessModifierOffset: -4\n"
      "AlignAfterOpenBracket: BlockIndent\n"
      "AlignConsecutiveAssignments:\n"
      "  Enabled: true\n"
      "  AcrossComments: true\n"
      "AlignConsecutiveBitFields:\n"
      "  Enabled: true\n"
      "  AcrossComments: true\n"
      "AlignConsecutiveDeclarations:\n"
      "  Enabled: true\n"
      "  AcrossComments: true\n"
      "AlignConsecutiveMacros:\n"
      "  Enabled: true\n"
      "  AcrossComments: true\n"
      "AlignConsecutiveShortCaseStatements:\n"
      "  Enabled: true\n"
      "  AcrossComments: true\n"
      "AlignEscapedNewlines: Left\n"
      "AlignOperands: AlignAfterOperator\n"
      "AllowShortBlocksOnASingleLine: Always\n"
      "AllowShortIfStatementsOnASingleLine: Always\n"
      "AllowShortLoopsOnASingleLine: true\n"
      "BinPackArguments: false\n"
      "BinPackParameters: false\n"
      "BraceWrapping:\n"
      "  AfterCaseLabel: false\n"
      "  AfterClass: false\n"
      "  AfterControlStatement: Never\n"
      "  AfterEnum: false\n"
      "  AfterFunction: false\n"
      "  AfterNamespace: false\n"
      "  AfterObjCDeclaration: false\n"
      "  AfterStruct: false\n"
      "  AfterUnion: false\n"
      "  AfterExternBlock: false\n"
      "  BeforeCatch: false\n"
      "  BeforeElse: false\n"
      "  BeforeLambdaBody: false\n"
      "  BeforeWhile: false\n"
      "  IndentBraces: false\n"
      "  SplitEmptyFunction: false\n"
      "  SplitEmptyRecord: true\n"
      "  SplitEmptyNamespace: true\n"
      "BreakBeforeBinaryOperators: All\n"
      "BreakBeforeBraces: Custom\n"
      "BreakConstructorInitializers: BeforeComma\n"
      "ColumnLimit: 0\n"
      "Cpp11BracedListStyle: false\n"
      "IndentWidth: 4\n"
      "InsertBraces: true\n"
      "InsertNewlineAtEOF: true\n"
      "MaxEmptyLinesToKeep: 5\n"
      "ObjCBlockIndentWidth: 4\n"
      "PointerAlignment: Left\n"
      "SortIncludes: Never\n"
      "Standard: c++20\n"
      "TabWidth: 4\n"
      "ConstructorInitializerAllOnOneLineOrOnePerLine: false\n"
      "\n"
      "TypeNames:\n"
      "  # Inference\n"
      "  - let\n"
      "  - var\n"
      "  - let_\n"
      "  - var_\n"
      "  - TypeOf\n"
      "  - TypeUnqualOf\n"
      "  # primitive\n"
      "  - u8\n"
      "  - u16\n"
      "  - u32\n"
      "  - u64\n"
      "  - usize\n"
      "  - i8\n"
      "  - i16\n"
      "  - i32\n"
      "  - i64\n"
      "  - isize\n"
      "  - f32\n"
      "  - f64\n"
      "  - bool\n"
      "  - char\n"
      "  - rawptr_const$\n"
      "  - rawptr$\n"
      "  - anyptr_const\n"
      "  - anyptr\n"
      "  - AnyType\n"
      "  - Void\n"
      "  # Pointer\n"
      "  - Ptr_const$\n"
      "  - Ptr$\n"
      "  - Ptr_const$$\n"
      "  - Ptr$$\n"
      "  # Slice\n"
      "  - SliZ_const$\n"
      "  - SliZ$\n"
      "  - SliZ_const$$\n"
      "  - SliZ$$\n"
      "  - SliS_const$\n"
      "  - SliS$\n"
      "  - SliS_const$$\n"
      "  - SliS$$\n"
      "  - Sli_const$\n"
      "  - Sli$\n"
      "  - Sli_const$$\n"
      "  - Sli$$\n"
      "  - SliS$$\n"
      "  - Sli_const$\n"
      "  - Sli$\n"
      "  - Sli_const$$\n"
      "  - Sli$$\n"
      "  # Array\n"
      "  - Arr$ \n"
      "  - Arr$$ \n"
      "  # Optional value\n"
      "  - Opt$ \n"
      "  - Opt$$ \n"
      "  # Error result\n"
      "  - Err$ \n"
      "  - Err$$ \n"
      "  - Err$Void \n"
      "  - Err$void \n"
      "  # Atomic\n"
      "  - atomic_Value$ \n"
      "  - atomic_Value$$ \n"
      "Macros:\n"
      "  # Optional value\n"
      "  - if_some(_,_)=if (_)\n"
      "  - if_none(_)=if (_)\n"
      "  - else_none=else\n"
      "  - else_some(_)=else if (_)\n"
      "  # Error result\n"
      "  - if_err(_,_)=if (_)\n"
      "  - if_ok(_,_)=if (_)\n"
      "  - if_void(_)=if (_)\n"
      "  - else_ok(_)=else if (_)\n"
      "  - else_void=else\n"
      "  - else_err(_)=else if (_)\n"
      "  # Union enum\n"
      "  - tagUnion(tag,expr)= \n"
      "  - tagUnion$(T,tag,expr)= \n"
      "  - toTagUnion(addr,tag,expr)= \n"
      "  # Match\n"
      "  - match_(expr)=switch (expr)\n"
      "  - pattern_(tag,capture)=case tag:\n"
      "  - fallback_(_)=default:\n"
      "  - $must_check=/**/\n"
      "  # Eval\n"
      "  - eval_(_)=({ _; })\n"
      "  - eval$(T,_)=(T)({ _; })\n"
      "  - eval_return_(_)=return _\n"
      "  - asg_eval(_)=({ _; })\n"
      "  # Scope\n"
      "  - block_defer=/**/\n"
      "  - block_deferral=/**/\n"
      "  - defer_break=break\n"
      "  - fn_(name_with_params,return_type)=return_type name_with_params\n"
      "  - fn_scope(name_with_params,return_type)=return_type name_with_params\n"
      "  - $unscoped=/**/\n"
      "  - fn_scope_ext(name_with_params,return_type)=return_type name_with_params\n"
      "  - $unguarded=/**/\n"
      "  - fn_TEST_scope(name)=Err$void TEST_caseFn(void)\n"
      "  - TEST_unscoped=/**/\n"
      "  - fn_TEST_scope_ext(name)=Err$void TEST_caseFn_ext(void)\n"
      "  - TEST_unscoped_ext=/**/\n"
      "ForEachMacros:\n"
      "  - pp_foreach\n"
      "  - for_slice\n"
      "  - for_slice_indexed\n"
      "  - for_slice_rev\n"
      "  - for_slice_rev_indexed\n"
      "  - for_array\n"
      "  - for_array_indexed\n"
      "  - for_array_rev\n"
      "  - for_array_rev_indexed\n"
      "AttributeMacros:\n"
      "  - $on_load\n"
      "  - $on_exit\n"
      "  - $inline\n"
      "  - $inline_always\n"
      "  - $inline_never\n"
      "  - $deprecated\n"
      "  - $deprecated_msg\n"
      "  - $deprecated_instead\n"
      "  - $must_check\n"
      "  - $no_return\n"
      "  - $nullable\n"
      "  - $nonnull\n"
      "StatementAttributeLikeMacros:\n"
      "  - $ignore\n"
      "  - eval\n"
      "  - eval_return\n"
      "  - type$\n"
      "  - make$\n"
      "  - create$\n"
      "  - initial\n"
      "  - cleared\n"
      "  - unwrap\n"
      "  - orelse\n"
      "  - try_\n"
      "  - catch_\n"
      "  - catch_from\n"
      "WhitespaceSensitiveMacros:\n"
      "  - with_\n"
      "  - try_\n"
      "  - catch_\n"
      "  - catch_from\n"
      "  - return_err\n"
      "  - return_ok\n"
      "  - unwrap\n"
      "  - orelse\n"
      "  - return_some\n"
      "  - return_none\n"
      "  - config_ErrSet\n"
      "  - config_UnionEnum\n"
      "  - config_UnionEnumAsField\n"
      "  - meta_refPtr\n"
      "  - meta_refSli\n"
      "  - meta_refPtr_const\n"
      "  - meta_refSli_const\n"
      "  - meta_castPtr$\n"
      "  - meta_castSli$\n"
      "  - meta_cast$\n"
      "  - meta_ptrToAny\n"
      "  - meta_sliToAny\n";

const char* DEFAULT_TASKS_JSON_TEMPLATE
    = "{\n"
      "  \"version\": \"2.0.0\",\n"
      "  \"inputs\": [\n"
      "    { /* compiler */\n"
      "      \"id\": \"compiler\",\n"
      "      \"type\": \"pickString\",\n"
      "      \"description\": \"Select compiler\",\n"
      "      \"options\": [\n"
      "        \"clang\"\n"
      "      ],\n"
      "      \"default\": \"clang\"\n"
      "    },\n"
      "    { /* cStandard */\n"
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
      "    { /* buildConfig */\n"
      "      \"id\": \"buildConfig\",\n"
      "      \"type\": \"pickString\",\n"
      "      \"description\": \"Select build configuration\",\n"
      "      \"options\": [\n"
      "        \"dev\",\n"
      "        \"test\",\n"
      "        \"profile\",\n"
      "        \"release\",\n"
      "        \"performance\",\n"
      "        \"embedded\",\n"
      "        \"micro\"\n"
      "      ],\n"
      "      \"default\": \"dev\"\n"
      "    },\n"
      "    { /* noLibDh */\n"
      "      \"id\": \"noLibDh\",\n"
      "      \"type\": \"pickString\",\n"
      "      \"description\": \"Skip DH library usage?\",\n"
      "      \"options\": [\n"
      "        \"\",\n"
      "        \"--no-libdh\"\n"
      "      ],\n"
      "      \"default\": \"\"\n"
      "    },\n"
      "    { /* runArgs */\n"
      "      \"id\": \"runArgs\",\n"
      "      \"type\": \"promptString\",\n"
      "      \"description\": \"Arguments to pass to the program\",\n"
      "      \"default\": \"\"\n"
      "    }\n"
      "  ],\n"
      "  \"tasks\": [\n"
      "    { /* create workspace */\n"
      "      \"label\": \"dh>create workspace\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"workspace\",\n"
      "        \"${input:prompt:Enter workspace name:}\"\n"
      "      ],\n"
      "      \"problemMatcher\": []\n"
      "    },\n"
      "    { /* create project */\n"
      "      \"label\": \"dh>create project\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"project\",\n"
      "        \"${input:prompt:Enter project name:}\"\n"
      "      ],\n"
      "      \"problemMatcher\": []\n"
      "    },\n"
      "    { /* build current file */\n"
      "      \"label\": \"dh>build current file\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"build\",\n"
      "        \"${input:buildConfig}\",\n"
      "        \"${file}\",\n"
      "        \"--compiler=${input:compiler}\",\n"
      "        \"--std=${input:cStandard}\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${fileDirname}\"\n"
      "      },\n"
      "      \"group\": \"build\",\n"
      "      \"problemMatcher\": [\"$gcc\"]\n"
      "    },\n"
      "    { /* build project */\n"
      "      \"label\": \"dh>build project\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"build\",\n"
      "        \"${input:buildConfig}\",\n"
      "        \"--compiler=${input:compiler}\",\n"
      "        \"--std=${input:cStandard}\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${workspaceFolder}\"\n"
      "      },\n"
      "      \"group\": \"build\",\n"
      "      \"problemMatcher\": [\"$gcc\"]\n"
      "    },\n"
      "    { /* test current file */\n"
      "      \"label\": \"dh>test current file\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"test\",\n"
      "        \"${file}\",\n"
      "        \"--args=\\\"${input:runArgs}\\\"\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${fileDirname}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    },\n"
      "    { /* test project */\n"
      "      \"label\": \"dh>test project\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"test\",\n"
      "        \"${input:buildConfig}\",\n"
      "        \"--args=\\\"${input:runArgs}\\\"\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${workspaceFolder}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    },\n"
      "    { /* run current file */\n"
      "      \"label\": \"dh>run current file\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"run\",\n"
      "        \"${input:buildConfig}\",\n"
      "        \"${file}\",\n"
      "        \"--args=\\\"${input:runArgs}\\\"\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${fileDirname}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    },\n"
      "    { /* run project */\n"
      "      \"label\": \"dh>run project\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"dh-c\",\n"
      "      \"args\": [\n"
      "        \"run\",\n"
      "        \"${input:buildConfig}\",\n"
      "        \"--args=\\\"${input:runArgs}\\\"\",\n"
      "        \"${input:noLibDh}\"\n"
      "      ],\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${workspaceFolder}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    },\n"
      "    { /* execute current file - just runs existing executable */\n"
      "      \"label\": \"dh>execute current file\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"${fileDirname}/build/${fileBasenameNoExtension}-${input:buildConfig}.exe\",\n"
      "      \"args\": [\n"
      "        \"${input:runArgs}\"\n"
      "      ],\n"
      "      \"windows\": {\n"
      "        \"command\": \"${fileDirname}\\\\build\\\\${fileBasenameNoExtension}-${input:buildConfig}.exe\"\n"
      "      },\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${fileDirname}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    },\n"
      "    { /* execute project - just runs existing executable */\n"
      "      \"label\": \"dh>execute project\",\n"
      "      \"type\": \"shell\",\n"
      "      \"command\": \"${workspaceFolder}/build/${workspaceFolderBasename}-${input:buildConfig}.exe\",\n"
      "      \"args\": [\n"
      "        \"${input:runArgs}\"\n"
      "      ],\n"
      "      \"windows\": {\n"
      "        \"command\": \"${workspaceFolder}\\\\build\\\\${workspaceFolderBasename}-${input:buildConfig}.exe\"\n"
      "      },\n"
      "      \"options\": {\n"
      "        \"cwd\": \"${workspaceFolder}\"\n"
      "      },\n"
      "      \"group\": \"test\"\n"
      "    }\n"
      "  ]\n"
      "}\n";

// Initialize global pointers to NULL
char* CLANGD_TEMPLATE       = NULL;
char* CLANG_FORMAT_TEMPLATE = NULL;
char* TASKS_JSON_TEMPLATE   = NULL;

// Function to load a template file and store its contents - update to check verbose flag
bool load_template_file(const char* filename, char** template_content, bool verbose) {
    if (verbose) {
        printf("Attempting to load template from: %s\n", filename);
    }

    // Open in binary mode to avoid text mode translations
    FILE* file = fopen(filename, "rb");
    if (!file) {
        if (verbose) {
            printf("Failed to open template file: %s (error: %s)\n", filename, strerror(errno));
        }
        return false;
    }

    // Get file size
    if (fseek(file, 0, SEEK_END) != 0) {
        if (verbose) {
            printf("Failed to seek to end of file: %s\n", filename);
        }
        (void)fclose(file);
        return false;
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        if (verbose) {
            printf("Failed to determine file size: %s\n", filename);
        }
        (void)fclose(file);
        return false;
    }

    // Move back to the beginning of the file with proper error handling
    if (fseek(file, 0, SEEK_SET) != 0) {
        if (verbose) {
            printf("Failed to seek to beginning of file: %s\n", filename);
        }
        (void)fclose(file);
        return false;
    }

    // Allocate memory for content
    *template_content = (char*)malloc(file_size + 1);
    if (*template_content == NULL) {
        if (verbose) {
            printf("Memory allocation failed for template: %s\n", filename);
        }
        (void)fclose(file);
        return false;
    }

    // Read file content in chunks to handle potential partial reads
    size_t bytes_read = 0;
    size_t total_read = 0;
    char*  buffer_pos = *template_content;

    while (total_read < (size_t)file_size) {
        bytes_read = fread(buffer_pos, 1, file_size - total_read, file);

        if (bytes_read == 0) {
            if (feof(file)) {
                // End of file reached unexpectedly
                if (verbose) {
                    printf("Warning: EOF reached after reading %zu of %ld bytes from %s\n", total_read, file_size, filename);
                }
                break;
            }

            if (ferror(file)) {
                if (verbose) {
                    printf("Error reading from file %s: %s\n", filename, strerror(errno));
                }
                free(*template_content);
                *template_content = NULL;
                (void)fclose(file);
                return false;
            }
        }

        total_read += bytes_read;
        buffer_pos += bytes_read;
    }

    (void)fclose(file);

    // Null terminate
    (*template_content)[total_read] = '\0';
    if (verbose) {
        printf("Successfully loaded template: %s (%zu bytes)\n", filename, total_read);
    }
    return true;
}

// Function to initialize all templates
void initialize_templates(const char* dh_path, bool verbose) {
    char template_path[1024] = {};
    char templates_dir[1024] = {};

    // Ensure templates directory exists
    (void)snprintf(templates_dir, sizeof(templates_dir), "%s%stemplates", dh_path, PATH_SEPARATOR);
    if (!dir_exists(templates_dir)) {
        create_templates_directory(dh_path, verbose);
    }

    // Load .clangd template
    (void)snprintf(template_path, sizeof(template_path), "%s%s.clangd", templates_dir, PATH_SEPARATOR);
    if (!load_template_file(template_path, &CLANGD_TEMPLATE, verbose)) {
        // Fall back to default
        CLANGD_TEMPLATE = strdup(DEFAULT_CLANGD_TEMPLATE);
        if (verbose) {
            printf("Using default .clangd template (couldn't load from %s)\n", template_path);
        }

        // Try to create the file if it doesn't exist
        if (!file_exists(template_path)) {
            write_file(template_path, DEFAULT_CLANGD_TEMPLATE);
            if (verbose) {
                printf("Created default .clangd template file\n");
            }
        }
    } else {
        if (verbose) {
            printf("Loaded .clangd template from %s\n", template_path);
        }
    }

    // Load .clang-format template
    (void)snprintf(template_path, sizeof(template_path), "%s%s.clang-format", templates_dir, PATH_SEPARATOR);
    if (!load_template_file(template_path, &CLANG_FORMAT_TEMPLATE, verbose)) {
        // Fall back to default
        CLANG_FORMAT_TEMPLATE = strdup(DEFAULT_CLANG_FORMAT_TEMPLATE);
        if (verbose) {
            printf("Using default .clang-format template (couldn't load from %s)\n", template_path);
        }

        // Try to create the file if it doesn't exist
        if (!file_exists(template_path)) {
            write_file(template_path, DEFAULT_CLANG_FORMAT_TEMPLATE);
            if (verbose) {
                printf("Created default .clang-format template file\n");
            }
        }
    } else {
        if (verbose) {
            printf("Loaded .clang-format template from %s\n", template_path);
        }
    }

    // Load tasks.json template
    (void)snprintf(template_path, sizeof(template_path), "%s%stasks.json", templates_dir, PATH_SEPARATOR);
    if (!load_template_file(template_path, &TASKS_JSON_TEMPLATE, verbose)) {
        // Fall back to default
        TASKS_JSON_TEMPLATE = strdup(DEFAULT_TASKS_JSON_TEMPLATE);
        if (verbose) {
            printf("Using default tasks.json template (couldn't load from %s)\n", template_path);
        }

        // Try to create the file if it doesn't exist
        if (!file_exists(template_path)) {
            write_file(template_path, DEFAULT_TASKS_JSON_TEMPLATE);
            if (verbose) {
                printf("Created default tasks.json template file\n");
            }
        }
    } else {
        if (verbose) {
            printf("Loaded tasks.json template from %s\n", template_path);
        }
    }
}

// Function to free template memory
void cleanup_templates() {
    if (CLANGD_TEMPLATE) {
        free(CLANGD_TEMPLATE);
        CLANGD_TEMPLATE = NULL;
    }

    if (CLANG_FORMAT_TEMPLATE) {
        free(CLANG_FORMAT_TEMPLATE);
        CLANG_FORMAT_TEMPLATE = NULL;
    }

    if (TASKS_JSON_TEMPLATE) {
        free(TASKS_JSON_TEMPLATE);
        TASKS_JSON_TEMPLATE = NULL;
    }
}

// Function to create templates directory and write default templates
void create_templates_directory(const char* dh_path, bool verbose) {
    char templates_dir[1024] = {};
    (void)snprintf(templates_dir, sizeof(templates_dir), "%s%stemplates", dh_path, PATH_SEPARATOR);

    if (!dir_exists(templates_dir)) {
        if (!create_directory(templates_dir)) {
            (void)fprintf(stderr, "Warning: Could not create templates directory\n");
            return;
        }
        if (verbose) {
            printf("Created templates directory: %s\n", templates_dir);
        }
    }

    // Write default templates to files
    char template_path[1024] = {};

    // Write .clangd template
    (void)snprintf(template_path, sizeof(template_path), "%s%s.clangd", templates_dir, PATH_SEPARATOR);
    if (!file_exists(template_path)) {
        write_file(template_path, DEFAULT_CLANGD_TEMPLATE);
        if (verbose) {
            printf("Created default .clangd template file\n");
        }
    }

    // Write .clang-format template
    (void)snprintf(template_path, sizeof(template_path), "%s%s.clang-format", templates_dir, PATH_SEPARATOR);
    if (!file_exists(template_path)) {
        write_file(template_path, DEFAULT_CLANG_FORMAT_TEMPLATE);
        if (verbose) {
            printf("Created default .clang-format template file\n");
        }
    }

    // Write tasks.json template
    (void)snprintf(template_path, sizeof(template_path), "%s%stasks.json", templates_dir, PATH_SEPARATOR);
    if (!file_exists(template_path)) {
        write_file(template_path, DEFAULT_TASKS_JSON_TEMPLATE);
        if (verbose) {
            printf("Created default tasks.json template file\n");
        }
    }
}

// Build the project
void build_project(BuildConfig* config) {
    char** includes      = NULL;
    int    include_count = 0;

    char** sources      = NULL;
    int    source_count = 0;

    // Compile libraries from lib.dh first if any exist
    if (config->lib_configs.count > 0) {
        if (!compile_libraries(config)) {
            (void)fprintf(stderr, "Failed to compile libraries\n");
            exit(1);
        }
    }

    // Collect include paths and source files
    collect_include_paths(config, &includes, &include_count);
    collect_source_files(config, &sources, &source_count);

    // Handle BlocksRuntime library if needed
    char blocks_lib_path[1024] = {};
    bool needs_blocks_runtime  = false;
    bool has_blocks_lib        = false;
    bool blocks_sources_added  = false;

    // Only check for BlocksRuntime if not using --no-libdh
    if (!config->no_libdh) {
        needs_blocks_runtime = true;

        // Check if BlocksRuntime library exists
        has_blocks_lib = check_blocksruntime_lib(config->dh_path, blocks_lib_path, sizeof(blocks_lib_path));

        // If not, try to compile it
        if (!has_blocks_lib) {
            // Create a path for the new library
            char lib_dir[1024] = {};

            (void)snprintf(lib_dir, sizeof(lib_dir), "%s%slibs%sBlocksRuntime%slib", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

            if (!dir_exists(lib_dir)) {
                create_directory(lib_dir);
            }

#ifdef _WIN32
            (void)snprintf(blocks_lib_path, sizeof(blocks_lib_path), "%s%sBlocksRuntime.lib", lib_dir, PATH_SEPARATOR);
#else
            (void)snprintf(blocks_lib_path, sizeof(blocks_lib_path), "%s%slibBlocksRuntime.a", lib_dir, PATH_SEPARATOR);
#endif

            has_blocks_lib = compile_blocksruntime_lib(config->dh_path, blocks_lib_path, config->verbose);

            if (!has_blocks_lib) {
                if (config->verbose) {
                    printf("Warning: Could not compile BlocksRuntime library, will include source files directly\n");
                }

                // Add BlocksRuntime source files directly if we couldn't compile the library
                char blocks_src_dir[1024] = {};
                (void)snprintf(blocks_src_dir, sizeof(blocks_src_dir), "%s%slibs%sBlocksRuntime%ssrc", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

                if (dir_exists(blocks_src_dir)) {
                    // Add data.c and runtime.c explicitly
                    char data_c[1024]    = {};
                    char runtime_c[1024] = {};
                    (void)snprintf(data_c, sizeof(data_c), "%s%sdata.c", blocks_src_dir, PATH_SEPARATOR);
                    (void)snprintf(runtime_c, sizeof(runtime_c), "%s%sruntime.c", blocks_src_dir, PATH_SEPARATOR);

                    if (file_exists(data_c)) {
                        char** new_sources = realloc(sources, (source_count + 1) * sizeof(char*));
                        if (new_sources != NULL) {
                            sources               = new_sources;
                            sources[source_count] = strdup(data_c);
                            source_count++;
                            blocks_sources_added = true;
                        }
                    }

                    if (file_exists(runtime_c)) {
                        char** new_sources = realloc(sources, (source_count + 1) * sizeof(char*));
                        if (new_sources != NULL) {
                            sources               = new_sources;
                            sources[source_count] = strdup(runtime_c);
                            source_count++;
                            blocks_sources_added = true;
                        }
                    }
                }
            }
        } else if (config->verbose) {
            printf("Using existing BlocksRuntime library: %s\n", blocks_lib_path);
        }
    }

    // Prepare command
    char command[8192] = {};
    strcat(command, config->compiler);

    // Add standard
    strcat(command, " -std=");
    strcat(command, config->c_standard);

    // Add include paths
    for (int i = 0; i < include_count; ++i) {
        strcat(command, " -I");
        strcat(command, includes[i]);
    }

    // Add compile flags
    strcat(command, " -DCOMP");
    if (config->is_test) {
        strcat(command, " -DCOMP_TEST");
    }

    // Only add -funsigned-char -fblocks if not using --no-libdh
    if (!config->no_libdh) {
        strcat(command, " -funsigned-char -fblocks -DBlocksRuntime_STATIC");
    }

    // Add debug level flag
    const char* debug_flag = debug_level_to_flag(config->debug_level);
    if (strlen(debug_flag) > 0) {
        strcat(command, " ");
        strcat(command, debug_flag);
    }

    // Add optimization level flag
    const char* opt_flag = optimization_level_to_flag(config->optimization_level);
    if (strlen(opt_flag) > 0) {
        strcat(command, " ");
        strcat(command, opt_flag);
    }

    // Add NDEBUG if assertions are disabled
    if (!config->assertions_enabled) {
        strcat(command, " -DNDEBUG");
    }

    // Add extra flags
    for (int i = 0; i < config->extra_flags.count; ++i) {
        strcat(command, " ");
        strcat(command, config->extra_flags.flags[i]);
    }

    // Add compiler arguments (from --args for build command)
    if (config->compiler_args[0] != '\0') {
        strcat(command, " ");
        strcat(command, config->compiler_args);
    }

    // Add source files
    for (int i = 0; i < source_count; ++i) {
        strcat(command, " ");
        strcat(command, sources[i]);
    }

    // Add output file
    char output_file[1024] = {};
    (void)snprintf(output_file, sizeof(output_file), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->project_name);

    // Add build config name or test suffix to output file if enabled
    if (config->is_test) {
        strcat(output_file, "-TEST");
    } else if (config->use_output_suffix) {
        strcat(output_file, "-");
        strcat(output_file, config->build_config_name);
    }

#ifdef _WIN32
    strcat(output_file, ".exe");
#endif

    strcat(command, " -o ");
    strcat(command, output_file);

    // Add BlocksRuntime library if we found or compiled it AND we didn't add the source files directly
    if (needs_blocks_runtime && has_blocks_lib && !blocks_sources_added) {
        strcat(command, " \"");
        strcat(command, blocks_lib_path);
        strcat(command, "\"");
    }

    // Add linked libraries
    if (config->linked_libraries[0] != '\0') {
        strcat(command, " ");
        strcat(command, config->linked_libraries);
    }

    // Add library linking flags from lib.dh
    add_lib_dh_libraries(config);
    if (config->linked_libraries[0] != '\0') {
        strcat(command, " ");
        strcat(command, config->linked_libraries);
    }

    // Add pthread library (required for threading support)
    strcat(command, " -lpthread");
    strcat(command, " -static");

    // Execute build command
    printf("Building %s with config '%s'...\n", config->project_name, config->build_config_name);

    // Only print command if show_commands is true
    if (config->show_commands) {
        printf("Command: %s\n", command);
    }

    // Using system() is a linter warning but is required for this tool's functionality
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    int result = system(command);
    if (result != 0) {
        (void)fprintf(stderr, "Build failed with error code: %d\n", result);
        exit(1);
    }

    printf("Build successful!\n");

    // Clean up
    free_string_array(includes, include_count);
    free_string_array(sources, source_count);
}

// Run the compiled executable
void run_executable(BuildConfig* config) {
    char output_file[1024] = {};
    (void)snprintf(output_file, sizeof(output_file), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->project_name);

    // Add appropriate suffix based on build type
    if (config->is_test) {
        strcat(output_file, "-TEST");
    } else if (config->use_output_suffix) {
        strcat(output_file, "-");
        strcat(output_file, config->build_config_name);
    }

#ifdef _WIN32
    strcat(output_file, ".exe");
#endif

    if (!file_exists(output_file)) {
        (void)fprintf(stderr, "Error: Executable not found: %s\n", output_file);
        exit(1);
    }

    char command[2048] = {};
    (void)snprintf(command, sizeof(command), "\"%s\" %s", output_file, config->run_args);

    // Only print command if show_commands is true
    if (config->show_commands) {
        printf("Running: %s\n", command);
    } else {
        printf("Running %s...\n", config->project_name);
    }

    // Using system() is a linter warning but is required for this tool's functionality
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    system(command);

    // Delete test executable after running
    if (config->is_test) {
        // Only print command if show_commands is true
        if (config->show_commands) {
            printf("Removing test executable: %s\n", output_file);
        }

#ifdef _WIN32
        char rm_command[1024] = {};
        (void)snprintf(rm_command, sizeof(rm_command), "del \"%s\"", output_file);
        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        system(rm_command);
#else
        remove(output_file);
#endif
    }
}

// Free memory from string array
void free_string_array(char** array, int count) {
    if (!array) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        free(array[i]);
    }
    free(array);
}

// Create project-specific .clangd with absolute paths
void create_project_clangd(const char* project_path, const char* dh_path) {
    char clangd_path[1024] = {};
    int  result            = snprintf(clangd_path, sizeof(clangd_path), "%s%s.clangd", project_path, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(clangd_path)) {
        (void)fprintf(stderr, "Error: Path too long for .clangd\n");
        return;
    }

    char abs_include_path[1024]    = {};
    char abs_src_path[1024]        = {};
    char dh_include_path[1024]     = {};
    char blocks_include_path[1024] = {};
    char temp_path[1024]           = {};

    // Get absolute paths
    (void)snprintf(temp_path, sizeof(temp_path), "%s%sinclude", project_path, PATH_SEPARATOR);
    if (!get_absolute_path(temp_path, abs_include_path, sizeof(abs_include_path))) {
        (void)fprintf(stderr, "Warning: Could not resolve absolute path for include directory\n");
        strcpy(abs_include_path, temp_path);
    }

    (void)snprintf(temp_path, sizeof(temp_path), "%s%ssrc", project_path, PATH_SEPARATOR);
    if (!get_absolute_path(temp_path, abs_src_path, sizeof(abs_src_path))) {
        (void)fprintf(stderr, "Warning: Could not resolve absolute path for src directory\n");
        strcpy(abs_src_path, temp_path);
    }

    // Get DH paths
    result = snprintf(dh_include_path, sizeof(dh_include_path), "%s%sinclude", dh_path, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(dh_include_path)) {
        (void)fprintf(stderr, "Error: DH include path too long\n");
        return;
    }

    result = snprintf(blocks_include_path, sizeof(blocks_include_path), "%s%slibs%sBlocksRuntime%sinclude", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(blocks_include_path)) {
        (void)fprintf(stderr, "Error: BlocksRuntime include path too long\n");
        return;
    }

    char project_clangd[4096] = {};

    result = snprintf(
        project_clangd,
        sizeof(project_clangd),
        "CompileFlags:\n"
        "  Compiler: clang\n"
        "  CompilationDatabase: %s\n"
        "  Add:\n"
        "    - -xc\n"
        "    - -std=c17\n"
        "    - -Wall\n"
        "    - -Wextra\n"
        "    - -Werror\n"
        "    - -funsigned-char\n"
        "    - -fblocks\n"
        "    - -I%s\n"
        "    - -I%s\n"
        "    - -I%s\n"
        "    - -I%s\n"
        "Diagnostics:\n"
        "  UnusedIncludes: Strict\n"
        "  ClangTidy:\n"
        "    Add: \n"
        "      - modernize*\n"
        "      - performance*\n"
        "      - readability*\n"
        "      - bugprone*\n"
        "      - portability*\n"
        "    Remove: \n"
        "      - modernize-use-trailing-return-type\n"
        "      - readability-named-parameter\n"
        "InlayHints:\n"
        "  Enabled: true\n"
        "  TypeNameLimit: 24\n"
        "  DeducedTypes: true\n"
        "  ParameterNames: true\n"
        "  Designators: true\n"
        "  BlockEnd: false\n"
        "Hover:\n"
        "  ShowAKA: true\n"
        "---\n"
        "If:\n"
        "  PathMatch:\n"
        "    - .*\\.h\n"
        "    - .*\\.hh\n"
        "    - .*\\.hpp\n"
        "    - .*\\.hxx\n"
        "CompileFlags:\n"
        "  Add: \n"
        "    - -Wno-unused-function\n"
        "    - -Wno-unused-variable\n"
        "    - -Wno-unused-const-variable\n",
        project_path,
        abs_include_path,
        abs_src_path,
        dh_include_path,
        blocks_include_path
    );

    if (result < 0 || (size_t)result >= sizeof(project_clangd)) {
        (void)fprintf(stderr, "Error: Project clangd content too long\n");
        return;
    }

    write_file(clangd_path, project_clangd);
}

// Helper function to write example files
void write_example_files(const char* project_path) {
    char main_path[1024] = {};
    int  result          = snprintf(main_path, sizeof(main_path), "%s%ssrc%smain.c", project_path, PATH_SEPARATOR, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(main_path)) {
        (void)fprintf(stderr, "Error: Path too long for main.c\n");
        return;
    }

    const char* main_content
        = "/// @file  main.c\n"
          "/// @brief Example DH-C program\n"
          "\n"
          "#include <dh/main.h>\n"
          "#include <dh/Str.h>\n"
          "\n"
          "fn_TEST_scope(\"simple test\") {\n"
          "    try_(TEST_expect(1 + 1 == 2));\n"
          "} TEST_unscoped;\n"
          "\n"
          "fn_scope(dh_main(Sli$Str_const args), Err$void) {\n"
          "    $ignore args;\n"
          "    let message = Str_l(\"Hello, world!\");\n"
          "    Str_println(message);\n"
          "    return_ok({});\n"
          "} $unscoped;\n";

    write_file(main_path, main_content);
}

// Update create_project function
void create_project(const char* name, const char* dh_path) {
    char project_path[1024] = {};

    if (name == NULL || strcmp(name, ".") == 0) {
        if (getcwd(project_path, sizeof(project_path)) == NULL) {
            (void)fprintf(stderr, "Error: Could not get current working directory\n");
            return;
        }
    } else {
        if (strlen(name) >= sizeof(project_path)) {
            (void)fprintf(stderr, "Error: Project name too long\n");
            return;
        }
        strcpy(project_path, name);
        if (!create_directory(project_path)) {
            (void)fprintf(stderr, "Error: Could not create project directory '%s'\n", name);
            return;
        }
    }

    printf("Creating project in: %s\n", project_path);

    // Create standard project structure
    char include_path[1024] = {};
    char src_path[1024]     = {};
    char lib_path[1024]     = {}; // Changed from libs_path to lib_path

    int result = snprintf(include_path, sizeof(include_path), "%s%sinclude", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(include_path)) {
        create_directory(include_path);
    }

    result = snprintf(src_path, sizeof(src_path), "%s%ssrc", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(src_path)) {
        create_directory(src_path);
    }

    // Changed from libs to lib
    result = snprintf(lib_path, sizeof(lib_path), "%s%slib", project_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(lib_path)) {
        create_directory(lib_path);
    }

    // Removed tests directory creation

    // Create .clangd file
    create_project_clangd(project_path, dh_path);

    // Create example files
    write_example_files(project_path);

    printf("Project created successfully with example files!\n");
}

// Update create_workspace function to include DH_HOME in clangd config
void create_workspace(const char* name, const char* dh_path) {
    char workspace_path[1024] = {};

    if (name == NULL || strcmp(name, ".") == 0) {
        // Use current directory
        if (getcwd(workspace_path, sizeof(workspace_path)) == NULL) {
            (void)fprintf(stderr, "Error: Could not get current working directory\n");
            return;
        }
    } else {
        // Create a new directory with the given name
        strcpy(workspace_path, name);
        if (!create_directory(workspace_path)) {
            (void)fprintf(stderr, "Error: Could not create workspace directory '%s'\n", name);
            return;
        }
    }

    printf("Creating workspace in: %s\n", workspace_path);

    // Create custom .clangd file with DH_HOME paths
    char clangd_path[1024] = {};
    (void)snprintf(clangd_path, sizeof(clangd_path), "%s%s.clangd", workspace_path, PATH_SEPARATOR);

    // Create DH include paths for clangd
    char dh_include_path[1024] = {};
    (void)snprintf(dh_include_path, sizeof(dh_include_path), "%s%sinclude", dh_path, PATH_SEPARATOR);

    char blocks_include_path[1024] = {};
    (void)snprintf(blocks_include_path, sizeof(blocks_include_path), "%s%slibs%sBlocksRuntime%sinclude", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    char custom_clangd_content[4096] = {};

    (void)snprintf(
        custom_clangd_content,
        sizeof(custom_clangd_content),
        "CompileFlags:\n"
        "  Compiler: clang\n"
        "  Add: \n"
        "    - -xc\n"
        "    - -std=c17\n"
        "    - -Wall\n"
        "    - -Wextra\n"
        "    - -Werror\n"
        "    - -funsigned-char\n"
        "    - -fblocks\n"
        "    - -I%s\n"
        "    - -I%s\n"
        "Diagnostics:\n"
        "  UnusedIncludes: Strict\n"
        "  ClangTidy:\n"
        "    Add: \n"
        "      - modernize*\n"
        "      - performance*\n"
        "      - readability*\n"
        "      - bugprone*\n"
        "      - portability*\n"
        "    Remove: \n"
        "      - modernize-use-trailing-return-type\n"
        "      - readability-named-parameter\n"
        "InlayHints:\n"
        "  Enabled: true\n"
        "  TypeNameLimit: 24\n"
        "  DeducedTypes: true\n"
        "  ParameterNames: true\n"
        "  Designators: true\n"
        "  BlockEnd: false\n"
        "Hover:\n"
        "  ShowAKA: true\n"
        "---\n"
        "If:\n"
        "  PathMatch:\n"
        "    - .*\\.h\n"
        "    - .*\\.hh\n"
        "    - .*\\.hpp\n"
        "    - .*\\.hxx\n"
        "CompileFlags:\n"
        "  Add: \n"
        "    - -Wno-unused-function\n"
        "    - -Wno-unused-variable\n"
        "    - -Wno-unused-const-variable\n",
        dh_include_path,
        blocks_include_path
    );

    write_file(clangd_path, custom_clangd_content);

    // Create .clang-format file
    char clang_format_path[1024] = {};
    (void)snprintf(clang_format_path, sizeof(clang_format_path), "%s%s.clang-format", workspace_path, PATH_SEPARATOR);
    write_file(clang_format_path, CLANG_FORMAT_TEMPLATE);

    // Create .vscode directory
    char vscode_path[1024] = {};
    (void)snprintf(vscode_path, sizeof(vscode_path), "%s%s.vscode", workspace_path, PATH_SEPARATOR);
    if (!create_directory(vscode_path)) {
        (void)fprintf(stderr, "Warning: Could not create .vscode directory\n");
    } else {
        // Create tasks.json file
        char tasks_path[1024] = {};
        (void)snprintf(tasks_path, sizeof(tasks_path), "%s%stasks.json", vscode_path, PATH_SEPARATOR);
        write_file(tasks_path, TASKS_JSON_TEMPLATE);
    }

    // Create an example project in the workspace
    char example_project_path[1024] = {};
    int  result                     = snprintf(example_project_path, sizeof(example_project_path), "%s%shello-world", workspace_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(example_project_path)) {
        create_project(example_project_path, dh_path);
    } else {
        (void)fprintf(stderr, "Warning: Could not create example project (path too long)\n");
    }

    printf("Workspace created successfully!\n");
}

// Update print_usage function to include --show-commands
void print_usage() {
    printf("Usage: dh-c [command] [options]\n\n");
    printf("Commands:\n");
    printf("  --help, -h           - Display this help message\n");
    printf("  --version, -v        - Display version information\n");
    printf("  build                - Build the project or file\n");
    printf("  test                 - Build and run tests\n");
    printf("  run                  - Build and run the project or file\n");
    printf("  lib                  - Compile libraries specified in lib.dh file\n");
    printf("  workspace <.|name>   - Create workspace with config files (.clangd, .clang-format, .vscode)\n");
    printf("  project <.|name>     - Create a new project with DH-C structure\n\n");
    printf("Build configurations:\n");
    printf("  dev         - Extended debug info, no optimization, assertions enabled\n");
    printf("  test        - Standard debug info, basic optimization, assertions enabled\n");
    printf("  profile     - Standard debug info, balanced optimization, assertions enabled\n");
    printf("  release     - Minimal debug info, balanced optimization, assertions disabled\n");
    printf("  performance - No debug info, aggressive optimization, assertions disabled\n");
    printf("  embedded    - No debug info, size optimization, assertions disabled\n");
    printf("  micro       - No debug info, extreme size optimization, assertions disabled\n\n");
    printf("Options:\n");
    printf("  --compiler=<clang|gcc>  - Specify compiler (default: clang)\n");
    printf("  --std=<c99|c11|c17>     - Specify C standard (default: c17)\n");
    printf("  --args=\"args\"           - For 'build': compiler args; For 'run'/'test': 'compiler -- program'\n");
    printf("  --dh=<path>             - Path to DH library (auto-detected by default)\n");
    printf("  --no-libdh              - Skip DH library\n");
    printf("  --show-commands         - Show commands being executed\n");
    printf("  --use-output-suffix     - Enable output suffix for build configurations\n\n");
    printf("Environment Variables:\n");
    printf("  DH_HOME               - Path to DH library installation\n\n");
    printf("Examples:\n");
    printf("  dh-c --help                      - Display help information\n");
    printf("  dh-c --version                   - Show version information\n");
    printf("  dh-c build dev                   - Build project in dev mode\n");
    printf("  dh-c test sample.c               - Build and run tests for single file\n");
    printf("  dh-c run release sample.c        - Build and run single file in release mode\n");
    printf("  dh-c lib --verbose               - Compile libraries from lib.dh with verbose output\n");
    printf("  dh-c workspace .                 - Create new workspace from current directory\n");
    printf("  dh-c project myproject           - Create new project named 'myproject'\n");
    printf("  dh-c build embedded --no-libdh   - Build non-DH-C project with size optimization\n");
    printf("  dh-c build dev --args=\"-DDEBUG\"  - Build with additional compiler flags\n");
    printf("  dh-c run dev --args=\"arg1 arg2\"  - Build and run with program arguments\n");
    printf("  dh-c run dev --args=\"-lpthread -- --verbose --input file.txt\" - Compiler and program args\n");
}

// Update main function to handle --show-commands and show exe path with version
int main(int argc, const char* argv[]) {
    // Default configuration
    BuildConfig config = {};
    memset(&config, 0, sizeof(BuildConfig));

    // Initialize build presets
    init_build_presets();

    // Initialize default config
    init_build_config(&config);

    // Parse command-line arguments
    if (argc < 2) {
        print_usage();
        cleanup_build_presets();
        return 1;
    }

    // Handle help command first
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage();
        cleanup_build_presets();
        return 0;
    }

    // Handle version command
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        printf("DH-C Build Tool version %s\n", DH_C_VERSION);
        printf("Copyright (c) 2024-2025 Gyeongtae Kim\n");

        // Get and print executable path
        char exe_path[1024] = { 0 };
        if (get_executable_path(exe_path, sizeof(exe_path))) {
            printf("Installed at: %s\n", exe_path);
        }

        cleanup_build_presets();
        return 0;
    }

    // Check for --show-commands option
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--show-commands") == 0) {
            config.show_commands = true;
            break;
        }
    }

    // Check for --use-output-suffix option
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--use-output-suffix") == 0) {
            config.use_output_suffix = true;
            break;
        }
    }

    // Try to find DH path early for workspace and project commands
    if (!find_dh_path(&config)) {
        (void)fprintf(stderr, "Warning: Could not find DH library path\n");
        cleanup_build_presets();
        return 1;
    }

    // Create templates directory if needed and initialize templates
    create_templates_directory(config.dh_path, config.verbose);
    initialize_templates(config.dh_path, config.verbose);

    // Handle workspace and project commands
    if (strcmp(argv[1], "workspace") == 0) {
        const char* name = (argc > 2) ? argv[2] : ".";
        create_workspace(name, config.dh_path);
        cleanup_build_presets();
        cleanup_templates();
        return 0;
    }

    if (strcmp(argv[1], "project") == 0) {
        const char* name = (argc > 2) ? argv[2] : ".";
        create_project(name, config.dh_path);
        cleanup_build_presets();
        cleanup_templates();
        return 0;
    }

    // Handle lib command - compile libraries only
    if (strcmp(argv[1], "lib") == 0) {
        // Check for --verbose option
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--verbose") == 0) {
                config.verbose = true;
                break;
            }
        }

        // If not single file, detect project structure
        detect_project_structure(&config);

        // Create library command and compile libraries
        if (!create_library_command(&config)) {
            (void)fprintf(stderr, "Library compilation failed\n");
            free_library_configs(&config.lib_configs);
            cleanup_build_presets();
            cleanup_templates();
            return 1;
        }

        // Clean up
        free_library_configs(&config.lib_configs);
        cleanup_build_presets();
        cleanup_templates();
        return 0;
    }

    // For standard build commands, continue with existing logic
    // Check compiler availability
    if (!detect_clang()) {
        (void)fprintf(stderr, "Warning: clang compiler not found. Using gcc as fallback.\n");
        strcpy(config.compiler, "gcc");
    }

    // Process the command first
    if (strcmp(argv[1], "build") == 0) { // NOLINT(bugprone-branch-clone)
        // Just build
    } else if (strcmp(argv[1], "test") == 0) {
        // Apply test preset and run tests
        apply_build_preset(&config, "test");
        config.is_test = true;
    } else if (strcmp(argv[1], "run") == 0) {
        // Build and run the program
    } else if (!is_c_source_file(argv[1]) || !file_exists(argv[1])) {
        (void)fprintf(stderr, "Unknown command: %s\n", argv[1]);
        print_usage();
        cleanup_build_presets();
        cleanup_templates();
        return 1;
    }

    // Check if a build config has been specified
    if (argc > 2 && argv[2][0] != '-') {
        apply_build_preset(&config, argv[2]);
    }

    // Then check if we're building a single file or a project
    detect_build_type(&config, argc, argv);

    // Parse additional options
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--compiler=", 11) == 0) {
            strcpy(config.compiler, argv[i] + 11);
        } else if (strncmp(argv[i], "--std=", 6) == 0) {
            strcpy(config.c_standard, argv[i] + 6);
        } else if (strncmp(argv[i], "--args=", 7) == 0) {
            // For build command, args go to compiler; for run/test, args can be split
            if (strcmp(argv[1], "build") == 0) {
                strcpy(config.compiler_args, argv[i] + 7);
            } else {
                // For run/test: split on " -- " separator
                const char* args_str  = argv[i] + 7;
                const char* separator = strstr(args_str, " -- ");

                if (separator != NULL) {
                    // Split arguments: before " -- " goes to compiler, after goes to program
                    size_t compiler_len = separator - args_str;
                    if (compiler_len < sizeof(config.compiler_args)) {
                        strncpy(config.compiler_args, args_str, compiler_len);
                        config.compiler_args[compiler_len] = '\0';
                    }

                    // Skip the " -- " separator (4 characters)
                    const char* program_args = separator + 4;
                    strcpy(config.run_args, program_args);
                } else {
                    // No separator found, all args go to program
                    strcpy(config.run_args, args_str);
                }
            }
        } else if (strncmp(argv[i], "--dh=", 5) == 0) {
            // Allow user to specify DH path
            strcpy(config.dh_path, argv[i] + 5);
        } else if (strcmp(argv[i], "--no-libdh") == 0) {
            // Skip DH library
            config.no_libdh = true;
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

    // Clean up
    free_extra_flags(&config.extra_flags);
    free_library_configs(&config.lib_configs);
    cleanup_build_presets();
    cleanup_templates();

    return 0;
}

// Helper function to write a file
void write_file(const char* path, const char* content) {
    FILE* file = fopen(path, "w");
    if (!file) {
        (void)fprintf(stderr, "Error: Could not create file: %s\n", path);
        return;
    }

    size_t content_len = strlen(content);
    size_t written     = fwrite(content, 1, content_len, file);
    if (written != content_len) {
        (void)fprintf(stderr, "Error: Could not write complete content to file: %s\n", path);
        (void)fclose(file);
        return;
    }

    if (fclose(file) != 0) {
        (void)fprintf(stderr, "Warning: Error closing file: %s\n", path);
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
        (void)fprintf(stderr, "Error: Could not create directory: %s\n", path);
        return false;
    }
#else
    if (mkdir(path, 0755) != 0) {
        (void)fprintf(stderr, "Error: Could not create directory: %s\n", path);
        return false;
    }
#endif

    printf("Created directory: %s\n", path);
    return true;
}

// Fixed recursive call in apply_build_preset
void apply_build_preset(BuildConfig* config, const char* preset_name) {
    // Free any existing extra flags
    free_extra_flags(&config->extra_flags);

    bool preset_found = false;

    for (int i = 0; i < NUM_BUILD_PRESETS; ++i) {
        if (strcmp(build_presets[i].name, preset_name) == 0) {
            preset_found               = true;
            // Copy preset values
            config->debug_level        = build_presets[i].debug_level;
            config->optimization_level = build_presets[i].optimization_level;
            config->assertions_enabled = build_presets[i].assertions_enabled;
            strcpy(config->build_config_name, build_presets[i].name);

            // Copy extra flags
            if (build_presets[i].extra_flags.count > 0 && build_presets[i].extra_flags.flags) {
                config->extra_flags.count = build_presets[i].extra_flags.count;
                config->extra_flags.flags = malloc(config->extra_flags.count * sizeof(char*));

                for (int j = 0; j < config->extra_flags.count; ++j) {
                    config->extra_flags.flags[j] = strdup(build_presets[i].extra_flags.flags[j]);
                }
            }

            // Special handling for test mode
            if (strcmp(preset_name, "test") == 0) {
                config->is_test = true;
            }

            break;
        }
    }

    // If preset not found, use dev preset as default
    if (!preset_found) {
        // Set default values directly rather than recursively calling
        config->debug_level        = DEBUG_EXTENDED;
        config->optimization_level = OPT_NONE;
        config->assertions_enabled = true;
        strcpy(config->build_config_name, "dev");

        // Copy dev preset extra flags if they're set
        if (build_presets[0].extra_flags.count > 0 && build_presets[0].extra_flags.flags) {
            config->extra_flags.count = build_presets[0].extra_flags.count;
            config->extra_flags.flags = malloc(config->extra_flags.count * sizeof(char*));

            for (int j = 0; j < config->extra_flags.count; ++j) {
                config->extra_flags.flags[j] = strdup(build_presets[0].extra_flags.flags[j]);
            }
        }
    }
}

// Initialize the extra flags for predefined configurations
void init_build_presets() {
    // dev preset extra flags (warnings are NOT errors)
    BuildConfigPreset* dev    = &build_presets[0];
    dev->extra_flags.count    = 1;
    dev->extra_flags.flags    = malloc(dev->extra_flags.count * sizeof(char*));
    dev->extra_flags.flags[0] = strdup("-Wall");

    // test preset extra flags (warnings are NOT errors)
    BuildConfigPreset* test    = &build_presets[1];
    test->extra_flags.count    = 2;
    test->extra_flags.flags    = malloc(test->extra_flags.count * sizeof(char*));
    test->extra_flags.flags[0] = strdup("-Wall");
    test->extra_flags.flags[1] = strdup("-fno-omit-frame-pointer");

    // profile preset extra flags (warnings ARE errors)
    BuildConfigPreset* profile    = &build_presets[2];
    profile->extra_flags.count    = 3;
    profile->extra_flags.flags    = malloc(profile->extra_flags.count * sizeof(char*));
    profile->extra_flags.flags[0] = strdup("-Wall");
    profile->extra_flags.flags[1] = strdup("-Werror");
    profile->extra_flags.flags[2] = strdup("-fno-omit-frame-pointer");

    // release preset extra flags (warnings ARE errors)
    BuildConfigPreset* release    = &build_presets[3];
    release->extra_flags.count    = 2;
    release->extra_flags.flags    = malloc(release->extra_flags.count * sizeof(char*));
    release->extra_flags.flags[0] = strdup("-Wall");
    release->extra_flags.flags[1] = strdup("-Werror");

    // performance preset extra flags (warnings ARE errors)
    BuildConfigPreset* performance    = &build_presets[4];
    performance->extra_flags.count    = 2;
    performance->extra_flags.flags    = malloc(performance->extra_flags.count * sizeof(char*));
    performance->extra_flags.flags[0] = strdup("-Wall");
    performance->extra_flags.flags[1] = strdup("-Werror");

    // embedded preset extra flags (warnings ARE errors)
    BuildConfigPreset* embedded    = &build_presets[5];
    embedded->extra_flags.count    = 2;
    embedded->extra_flags.flags    = malloc(embedded->extra_flags.count * sizeof(char*));
    embedded->extra_flags.flags[0] = strdup("-Wall");
    embedded->extra_flags.flags[1] = strdup("-Werror");

    // micro preset extra flags (warnings ARE errors)
    BuildConfigPreset* micro    = &build_presets[6];
    micro->extra_flags.count    = 4;
    micro->extra_flags.flags    = malloc(micro->extra_flags.count * sizeof(char*));
    micro->extra_flags.flags[0] = strdup("-Wall");
    micro->extra_flags.flags[1] = strdup("-Werror");
    micro->extra_flags.flags[2] = strdup("-ffunction-sections");
    micro->extra_flags.flags[3] = strdup("-fdata-sections");
}

// Free resources used by build presets
void cleanup_build_presets() {
    for (int i = 0; i < NUM_BUILD_PRESETS; ++i) {
        free_extra_flags(&build_presets[i].extra_flags);
    }
}

// Add function to get the executable path
bool get_executable_path(char* buffer, size_t size) {
#ifdef _WIN32
    return GetModuleFileName(NULL, buffer, (DWORD)size) > 0;
#else
    ssize_t len = readlink("/proc/self/exe", buffer, size - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return true;
    }
    return false;
#endif
}

// Add function to check if BlocksRuntime static library exists
bool check_blocksruntime_lib(const char* dh_path, char* out_lib_path, size_t path_size) {
    if (!dh_path || !out_lib_path || path_size == 0) {
        return false;
    }

    char lib_dir[1024] = {};
    (void)snprintf(lib_dir, sizeof(lib_dir), "%s%slibs%sBlocksRuntime%slib", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (!dir_exists(lib_dir)) {
        return false;
    }

#ifdef _WIN32
    (void)snprintf(out_lib_path, path_size, "%s%sBlocksRuntime.lib", lib_dir, PATH_SEPARATOR);
#else
    (void)snprintf(out_lib_path, path_size, "%s%slibBlocksRuntime.a", lib_dir, PATH_SEPARATOR);
#endif

    return file_exists(out_lib_path);
}

// Add function to compile BlocksRuntime as a static library
bool compile_blocksruntime_lib(const char* dh_path, const char* out_lib_path, bool verbose) {
    if (!dh_path || !out_lib_path) {
        return false;
    }

    char blocks_src_dir[1024] = {};
    (void)snprintf(blocks_src_dir, sizeof(blocks_src_dir), "%s%slibs%sBlocksRuntime%ssrc", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    char data_c[1024]    = {};
    char runtime_c[1024] = {};
    (void)snprintf(data_c, sizeof(data_c), "%s%sdata.c", blocks_src_dir, PATH_SEPARATOR);
    (void)snprintf(runtime_c, sizeof(runtime_c), "%s%sruntime.c", blocks_src_dir, PATH_SEPARATOR);

    if (!file_exists(data_c) || !file_exists(runtime_c)) {
        if (verbose) {
            printf("BlocksRuntime source files not found\n");
        }
        return false;
    }

    // Create object files directory
    char obj_dir[1024] = {};
    (void)snprintf(obj_dir, sizeof(obj_dir), "%s%slibs%sBlocksRuntime%sobj", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (!dir_exists(obj_dir)) {
        create_directory(obj_dir);
    }

    // Compile data.c
    char data_o[1024] = {};
    (void)snprintf(data_o, sizeof(data_o), "%s%sdata.o", obj_dir, PATH_SEPARATOR);

    char compile_data_cmd[2048] = {};
    (void)snprintf(compile_data_cmd, sizeof(compile_data_cmd), "clang -I%s%slibs%sBlocksRuntime%sinclude -DBlocksRuntime_STATIC -fPIC -c %s -o %s", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR, data_c, data_o);

    if (verbose) {
        printf("Compiling BlocksRuntime data.c: %s\n", compile_data_cmd);
    }

    // Using system() is a linter warning but is required for this tool's functionality
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(compile_data_cmd) != 0) {
        if (verbose) {
            printf("Failed to compile BlocksRuntime data.c\n");
        }
        return false;
    }

    // Compile runtime.c
    char runtime_o[1024] = {};
    (void)snprintf(runtime_o, sizeof(runtime_o), "%s%sruntime.o", obj_dir, PATH_SEPARATOR);

    char compile_runtime_cmd[2048] = {};
    (void)snprintf(compile_runtime_cmd, sizeof(compile_runtime_cmd), "clang -I%s%slibs%sBlocksRuntime%sinclude -DBlocksRuntime_STATIC -fPIC -c %s -o %s", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR, runtime_c, runtime_o);

    if (verbose) {
        printf("Compiling BlocksRuntime runtime.c: %s\n", compile_runtime_cmd);
    }

    // Using system() is a linter warning but is required for this tool's functionality
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(compile_runtime_cmd) != 0) {
        if (verbose) {
            printf("Failed to compile BlocksRuntime runtime.c\n");
        }
        return false;
    }

    // Create static library
#ifdef _WIN32
    // Windows: Use ar with clang instead of lib.exe since that's more widely available
    char ar_cmd[2048] = {};
    (void)snprintf(ar_cmd, sizeof(ar_cmd), "ar rcs %s %s %s", out_lib_path, data_o, runtime_o);
#else
    // Unix systems use ar
    char ar_cmd[2048] = {};
    (void)snprintf(ar_cmd, sizeof(ar_cmd), "ar rcs %s %s %s", out_lib_path, data_o, runtime_o);
#endif

    if (verbose) {
        printf("Creating BlocksRuntime static library: %s\n", ar_cmd);
    }

    // Using system() is a linter warning but is required for this tool's functionality
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(ar_cmd) != 0) {
        if (verbose) {
            printf("Failed to create BlocksRuntime static library\n");
        }
        return false;
    }

    if (verbose) {
        printf("Successfully created BlocksRuntime static library: %s\n", out_lib_path);
    }

    return true;
}

// ===== LIBRARY MANAGEMENT FUNCTIONS =====

// Free library configurations
void free_library_configs(LibraryConfigArray* lib_configs) {
    if (lib_configs->libraries) {
        free(lib_configs->libraries);
        lib_configs->libraries = NULL;
        lib_configs->count     = 0;
    }
}

// Convert optimization level string to compiler flag
char* optimization_level_to_lib_flag(const char* opt_level) {
    if (strcmp(opt_level, "O1") == 0) {
        return "-O1";
    }
    if (strcmp(opt_level, "O2") == 0) {
        return "-O2";
    }
    if (strcmp(opt_level, "O3") == 0) {
        return "-O3";
    }
    if (strcmp(opt_level, "Os") == 0) {
        return "-Os";
    }
    if (strcmp(opt_level, "Oz") == 0) {
        return "-Oz";
    }
    return "-O2"; // Default
}

// Parse lib.dh file and populate library configurations
bool parse_lib_dh_file(BuildConfig* config, const char* lib_dh_path) {
    FILE* file = fopen(lib_dh_path, "r");
    if (!file) {
        if (config->verbose) {
            printf("Could not open lib.dh file: %s\n", lib_dh_path);
        }
        return false;
    }

    char          line[1024]  = {};
    LibraryConfig current_lib = {};
    bool          in_section  = false;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline and carriage return
        line[strcspn(line, "\r\n")] = 0;

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        // Check for section header [library_name]
        if (line[0] == '[' && line[strlen(line) - 1] == ']') {
            // Save previous library if we were in a section
            if (in_section && strlen(current_lib.library_name) > 0) {
                LibraryConfig* temp_libs = realloc(config->lib_configs.libraries, (config->lib_configs.count + 1) * sizeof(LibraryConfig));
                if (temp_libs == NULL) {
                    (void)fprintf(stderr, "Memory allocation failed\n");
                    (void)fclose(file);
                    return false;
                }
                config->lib_configs.libraries                            = temp_libs;
                config->lib_configs.libraries[config->lib_configs.count] = current_lib;
                config->lib_configs.count++;
            }

            // Start new library
            memset(&current_lib, 0, sizeof(LibraryConfig));
            strncpy(current_lib.library_name, line + 1, strlen(line) - 2);
            current_lib.library_name[strlen(line) - 2] = '\0';
            current_lib.no_libdh                       = false; // Default to using dh
            strcpy(current_lib.profile, "dev");                 // Default profile
            in_section = true;
            continue;
        }

        // Parse key=value pairs
        if (in_section) {
            char* equals = strchr(line, '=');
            if (equals) {
                *equals     = '\0';
                char* key   = line;
                char* value = equals + 1;

                // Trim whitespace
                while (*key == ' ' || *key == '\t') {
                    key++;
                }
                while (*value == ' ' || *value == '\t') {
                    value++;
                }

                if (strcmp(key, "linking") == 0) {
                    strncpy(current_lib.linking_type, value, sizeof(current_lib.linking_type) - 1);
                } else if (strcmp(key, "path") == 0) {
                    strncpy(current_lib.library_path, value, sizeof(current_lib.library_path) - 1);
                } else if (strcmp(key, "profile") == 0) {
                    strncpy(current_lib.profile, value, sizeof(current_lib.profile) - 1);
                } else if (strcmp(key, "no-libdh") == 0) {
                    current_lib.no_libdh = (strcmp(value, "true") == 0);
                }
            }
        }
    }

    // Save last library
    if (in_section && strlen(current_lib.library_name) > 0) {
        LibraryConfig* temp_libs = realloc(config->lib_configs.libraries, (config->lib_configs.count + 1) * sizeof(LibraryConfig));
        if (temp_libs == NULL) {
            (void)fprintf(stderr, "Memory allocation failed\n");
            (void)fclose(file);
            return false;
        }
        config->lib_configs.libraries                            = temp_libs;
        config->lib_configs.libraries[config->lib_configs.count] = current_lib;
        config->lib_configs.count++;
    }

    (void)fclose(file);

    if (config->verbose) {
        printf("Parsed %d libraries from lib.dh\n", config->lib_configs.count);
    }

    return true;
}

// Recursively copy directory structure
bool copy_directory_recursive(const char* src, const char* dest, bool verbose) { /* NOLINT(misc-no-recursion) */
    DIR* dir = opendir(src);
    if (!dir) {
        return false;
    }

    if (!dir_exists(dest)) {
        if (!create_directory(dest)) {
            closedir(dir);
            return false;
        }
    }

    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Skip . and ..
        }

        char src_path[1024];
        char dest_path[1024];
        (void)snprintf(src_path, sizeof(src_path), "%s%s%s", src, PATH_SEPARATOR, entry->d_name);
        (void)snprintf(dest_path, sizeof(dest_path), "%s%s%s", dest, PATH_SEPARATOR, entry->d_name);

        struct stat st = {};
        if (stat(src_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recurse into subdirectory
                if (!copy_directory_recursive(src_path, dest_path, verbose)) {
                    closedir(dir);
                    return false;
                }
            } else if (S_ISREG(st.st_mode)) {
                // Copy file
                FILE* src_file = fopen(src_path, "rb");
                if (!src_file) {
                    closedir(dir);
                    return false;
                }

                FILE* dest_file = fopen(dest_path, "wb");
                if (!dest_file) {
                    (void)fclose(src_file);
                    closedir(dir);
                    return false;
                }

                char   buffer[4096];
                size_t bytes = 0;
                while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
                    if (fwrite(buffer, 1, bytes, dest_file) != bytes) {
                        (void)fclose(src_file);
                        (void)fclose(dest_file);
                        closedir(dir);
                        return false;
                    }
                }

                (void)fclose(src_file);
                (void)fclose(dest_file);

                if (verbose) {
                    printf("Copied: %s -> %s\n", src_path, dest_path);
                }
            }
        }
    }

    closedir(dir);
    return true;
}

// Copy library includes to project lib directory
bool copy_library_includes(const char* lib_src_path, const char* project_lib_path, const char* lib_name, bool verbose) {
    char lib_include_src[1024];
    char lib_include_dest[1024];

    (void)snprintf(lib_include_src, sizeof(lib_include_src), "%s%sinclude", lib_src_path, PATH_SEPARATOR);
    (void)snprintf(lib_include_dest, sizeof(lib_include_dest), "%s%s%s", project_lib_path, PATH_SEPARATOR, lib_name);

    if (!dir_exists(lib_include_src)) {
        if (verbose) {
            printf("No include directory found for library %s at %s\n", lib_name, lib_include_src);
        }
        return true; // Not an error if no includes
    }

    if (verbose) {
        printf("Copying includes for library %s: %s -> %s\n", lib_name, lib_include_src, lib_include_dest);
    }

    return copy_directory_recursive(lib_include_src, lib_include_dest, verbose);
}

// Compile a single library
bool compile_single_library(BuildConfig* config, const LibraryConfig* lib_config) {
    if (config->verbose) {
        printf("Compiling library: %s\n", lib_config->library_name);
        printf("  Path: %s\n", lib_config->library_path);
        printf("  Profile: %s\n", lib_config->profile);
        printf("  Linking: %s\n", lib_config->linking_type);
    }

    // Find source files in the library
    char lib_src_path[1024];
    (void)snprintf(lib_src_path, sizeof(lib_src_path), "%s%ssrc", lib_config->library_path, PATH_SEPARATOR);

    if (!dir_exists(lib_src_path)) {
        (void)fprintf(stderr, "Library source directory not found: %s\n", lib_src_path);
        return false;
    }

    char** sources      = NULL;
    int    source_count = 0;
    find_source_files(lib_src_path, &sources, &source_count);

    if (source_count == 0) {
        (void)fprintf(stderr, "No source files found for library: %s\n", lib_config->library_name);
        return false;
    }

    // Create project lib directory
    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    if (!dir_exists(project_lib_path)) {
        if (!create_directory(project_lib_path)) {
            (void)fprintf(stderr, "Could not create lib directory: %s\n", project_lib_path);
            free_string_array(sources, source_count);
            return false;
        }
    }

    // Copy library includes
    if (!copy_library_includes(lib_config->library_path, project_lib_path, lib_config->library_name, config->verbose)) {
        (void)fprintf(stderr, "Failed to copy includes for library: %s\n", lib_config->library_name);
        free_string_array(sources, source_count);
        return false;
    }

    // Prepare compilation command
    char command[8192] = {};
    strcat(command, config->compiler);
    strcat(command, " -std=");
    strcat(command, config->c_standard);

    // Add include paths
    char lib_include_path[1024];
    (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%sinclude", lib_config->library_path, PATH_SEPARATOR);
    if (dir_exists(lib_include_path)) {
        strcat(command, " -I");
        strcat(command, lib_include_path);
    }

    // Add DH includes if not disabled for this library
    if (!lib_config->no_libdh && strlen(config->dh_path) > 0) {
        char dh_include[1024];
        (void)snprintf(dh_include, sizeof(dh_include), "%s%sinclude", config->dh_path, PATH_SEPARATOR);
        strcat(command, " -I");
        strcat(command, dh_include);
        strcat(command, " -I");
        strcat(command, config->dh_path);
        strcat(command, " -funsigned-char -fblocks -DBlocksRuntime_STATIC");

        // Add BlocksRuntime include path
        char blocks_include_path[1024];
        (void)snprintf(blocks_include_path, sizeof(blocks_include_path), "%s%slibs%sBlocksRuntime%sinclude", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);
        if (dir_exists(blocks_include_path)) {
            strcat(command, " -I");
            strcat(command, blocks_include_path);
        }

        // Add DH compilation flags
        strcat(command, " -DCOMP");
        if (config->is_test) {
            strcat(command, " -DCOMP_TEST");
        }
    }

    // Apply build profile settings
    BuildConfigPreset* profile_preset = NULL;

    // Use main project's build profile if library profile is "default"
    const char* effective_profile = lib_config->profile;
    if (strcmp(lib_config->profile, "default") == 0) {
        effective_profile = config->build_config_name;
        if (config->verbose) {
            printf("Library profile is 'default', using main project profile: %s\n", effective_profile);
        }
    }

    for (int i = 0; i < NUM_BUILD_PRESETS; ++i) {
        if (strcmp(build_presets[i].name, effective_profile) == 0) {
            profile_preset = &build_presets[i];
            break;
        }
    }

    if (profile_preset) {
        // Add optimization level flag from profile
        const char* opt_flag = optimization_level_to_flag(profile_preset->optimization_level);
        if (strlen(opt_flag) > 0) {
            strcat(command, " ");
            strcat(command, opt_flag);
        }

        // Add debug level flag
        const char* debug_flag = debug_level_to_flag(profile_preset->debug_level);
        if (strlen(debug_flag) > 0) {
            strcat(command, " ");
            strcat(command, debug_flag);
        }

        // Add NDEBUG if assertions are disabled
        if (!profile_preset->assertions_enabled) {
            strcat(command, " -DNDEBUG");
        }

        // Add extra flags from the profile
        for (int i = 0; i < profile_preset->extra_flags.count; ++i) {
            strcat(command, " ");
            strcat(command, profile_preset->extra_flags.flags[i]);
        }

        if (config->verbose) {
            printf("Applied profile '%s' settings to library compilation\n", effective_profile);
        }
    } else if (config->verbose) {
        printf("Warning: Unknown profile '%s', using default settings\n", effective_profile);
    }

    // Add position independent code for shared libraries
    if (strcmp(lib_config->linking_type, "dynamic") == 0) {
        strcat(command, " -fPIC");

        // For dynamic libraries, add all source files to the command
        for (int i = 0; i < source_count; ++i) {
            strcat(command, " ");
            strcat(command, sources[i]);
        }
    }

    // Set output file
    char output_file[1024];
    if (strcmp(lib_config->linking_type, "static") == 0) {
#ifdef _WIN32
        (void)snprintf(output_file, sizeof(output_file), "%s%s%s.lib", project_lib_path, PATH_SEPARATOR, lib_config->library_name);
#else
        (void)snprintf(output_file, sizeof(output_file), "%s%slib%s.a", project_lib_path, PATH_SEPARATOR, lib_config->library_name);
#endif
    } else {
#ifdef _WIN32
        (void)snprintf(output_file, sizeof(output_file), "%s%s%s.dll", project_lib_path, PATH_SEPARATOR, lib_config->library_name);
#else
        (void)snprintf(output_file, sizeof(output_file), "%s%slib%s.so", project_lib_path, PATH_SEPARATOR, lib_config->library_name);
#endif
    }

    if (strcmp(lib_config->linking_type, "static") == 0) {
        // For static libraries, compile to object files first, then create archive
        char obj_dir[1024];
        (void)snprintf(obj_dir, sizeof(obj_dir), "%s%sobj_%s", project_lib_path, PATH_SEPARATOR, lib_config->library_name);

        if (!dir_exists(obj_dir)) {
            if (!create_directory(obj_dir)) {
                (void)fprintf(stderr, "Could not create object directory: %s\n", obj_dir);
                free_string_array(sources, source_count);
                return false;
            }
        }

        // Compile each source to object file
        char obj_files[16384] = {}; // Increased buffer size to prevent overflow
        for (int i = 0; i < source_count; ++i) {
            char  obj_file[1024];
            char* filename = strrchr(sources[i], PATH_SEPARATOR[0]);
            if (!filename) {
                filename = sources[i];
            } else {
                filename++; // Skip separator
            }

            char basename[256];
            strncpy(basename, filename, sizeof(basename) - 1);
            basename[sizeof(basename) - 1] = '\0'; // Ensure null termination
            char* dot                      = strrchr(basename, '.');
            if (dot) {
                *dot = '\0';
            }

            (void)snprintf(obj_file, sizeof(obj_file), "%s%s%s.o", obj_dir, PATH_SEPARATOR, basename);

            char compile_cmd[4096];
            (void)snprintf(compile_cmd, sizeof(compile_cmd), "%s -c %s -o %s", command, sources[i], obj_file);

            if (config->show_commands) {
                printf("Command: %s\n", compile_cmd);
            }

            // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
            if (system(compile_cmd) != 0) {
                (void)fprintf(stderr, "Failed to compile %s\n", sources[i]);
                free_string_array(sources, source_count);
                return false;
            }

            // Add object file to list with bounds checking
            size_t current_len  = strlen(obj_files);
            size_t obj_file_len = strlen(obj_file);
            size_t space_needed = (current_len > 0 ? 1 : 0) + obj_file_len + 1; // +1 for space, +1 for null terminator

            if (current_len + space_needed >= sizeof(obj_files)) {
                (void)fprintf(stderr, "Error: Too many object files for buffer\n");
                free_string_array(sources, source_count);
                return false;
            }

            if (current_len > 0) {
                strcat(obj_files, " ");
            }
            strcat(obj_files, obj_file);
        }

        // Create static library
        char ar_cmd[8192];
        (void)snprintf(ar_cmd, sizeof(ar_cmd), "ar rcs %s %s", output_file, obj_files);

        if (config->show_commands) {
            printf("Command: %s\n", ar_cmd);
        }

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(ar_cmd) != 0) {
            (void)fprintf(stderr, "Failed to create static library: %s\n", output_file);
            free_string_array(sources, source_count);
            return false;
        }
    } else {
        // For dynamic libraries, compile and link in one step
        strcat(command, " -shared -o ");
        strcat(command, output_file);

        if (config->show_commands) {
            printf("Command: %s\n", command);
        }

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(command) != 0) {
            (void)fprintf(stderr, "Failed to create dynamic library: %s\n", output_file);
            free_string_array(sources, source_count);
            return false;
        }
    }

    printf("Successfully compiled library: %s -> %s\n", lib_config->library_name, output_file);

    free_string_array(sources, source_count);
    return true;
}

// Compile all libraries specified in lib.dh
bool compile_libraries(BuildConfig* config) {
    if (config->lib_configs.count == 0) {
        if (config->verbose) {
            printf("No libraries to compile\n");
        }
        return true;
    }

    printf("Compiling %d libraries...\n", config->lib_configs.count);

    for (int i = 0; i < config->lib_configs.count; ++i) {
        if (!compile_single_library(config, &config->lib_configs.libraries[i])) {
            (void)fprintf(stderr, "Failed to compile library: %s\n", config->lib_configs.libraries[i].library_name);
            return false;
        }
    }

    printf("All libraries compiled successfully!\n");
    return true;
}

// Add library includes to the include path list
void add_lib_dh_includes(BuildConfig* config, char*** includes, int* include_count) {
    if (config->lib_configs.count == 0) {
        return;
    }

    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    for (int i = 0; i < config->lib_configs.count; ++i) {
        char lib_include_path[1024];
        (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%s%s", project_lib_path, PATH_SEPARATOR, config->lib_configs.libraries[i].library_name);

        if (dir_exists(lib_include_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            if (*includes == NULL) {
                (void)fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            (*includes)[*include_count] = strdup(lib_include_path);
            (*include_count)++;

            if (config->verbose) {
                printf("Added library include path: %s\n", lib_include_path);
            }
        }
    }
}

// Add library linking flags to the configuration
void add_lib_dh_libraries(BuildConfig* config) {
    if (config->lib_configs.count == 0) {
        return;
    }

    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    // Add library search path
    if (config->linked_libraries[0] != '\0') {
        strcat(config->linked_libraries, " ");
    }
    strcat(config->linked_libraries, "-L");
    strcat(config->linked_libraries, project_lib_path);

    // Add each library
    for (int i = 0; i < config->lib_configs.count; ++i) {
        const LibraryConfig* lib = &config->lib_configs.libraries[i];

        if (strcmp(lib->linking_type, "static") == 0) {
            // For static libraries, add full path to the .a/.lib file
            char lib_file[1024];
#ifdef _WIN32
            (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.lib", project_lib_path, PATH_SEPARATOR, lib->library_name);
#else
            (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.a", project_lib_path, PATH_SEPARATOR, lib->library_name);
#endif
            strcat(config->linked_libraries, " \"");
            strcat(config->linked_libraries, lib_file);
            strcat(config->linked_libraries, "\"");
        } else {
            // For dynamic libraries, add -l flag
            strcat(config->linked_libraries, " -l");
            strcat(config->linked_libraries, lib->library_name);
        }

        if (config->verbose) {
            printf("Added library link: %s (%s)\n", lib->library_name, lib->linking_type);
        }
    }
}

// Create the library compilation command (for the 'lib' command)
bool create_library_command(BuildConfig* config) {
    // Check for lib.dh file in the current project
    char lib_dh_path[1024];
    (void)snprintf(lib_dh_path, sizeof(lib_dh_path), "%s%slib.dh", config->project_root, PATH_SEPARATOR);

    if (!file_exists(lib_dh_path)) {
        (void)fprintf(stderr, "Error: lib.dh file not found at %s\n", lib_dh_path);
        (void)fprintf(stderr, "Create a lib.dh file in the root directory to specify libraries to compile.\n");
        return false;
    }

    // Library configurations should already be parsed by detect_project_structure
    if (config->lib_configs.count == 0) {
        (void)fprintf(stderr, "Error: No libraries found in lib.dh file\n");
        return false;
    }

    // Compile all libraries
    return compile_libraries(config);
}
