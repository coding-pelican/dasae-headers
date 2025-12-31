/**
 * dh-c - Build tool for DH-C projects
 *
 * Compilation:
 * - clang -std=c17 -Wall -Wextra -O3 -flto -o dh-c dh-c.c -static
 *
 * Features:
 * - Detects clang toolchain
 * - Supports workspace and project creation
 * - Supports build, test, and run commands
 * - Supports various build configurations
 *
 * Note:
 * - Major changes are coming. Please bear with this messy code ;)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define PATH_SEPARATOR "\\"
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
    "0.1.0-alpha.0.4.10"

// Debug level enum
typedef enum {
    DEBUG_NONE, // No debug info
    DEBUG_MINIMAL, // -g1: Line tables only
    DEBUG_STANDARD, // -g: Standard debug info
    DEBUG_EXTENDED // -g3: Extra debug info
} DebugLevel;

// Optimization level enum
typedef enum {
    OPT_NONE, // -O0: No optimization
    OPT_DEBUG, // -Og: Smallest for debugging
    OPT_BASIC, // -O1: Basic optimizations
    OPT_BALANCED, // -O2: Good balance
    OPT_AGGRESSIVE, // -O3: Maximum optimize
    OPT_SIZE, // -Os: Optimize for size
    OPT_SIZE_EXTREME // -Oz: Size at all costs
} OptimizationLevel;

// Extra flags array
typedef struct {
    char** flags;
    int count;
} ExtraFlags;

// Library configuration for lib.dh
typedef struct {
    char library_name[256];
    char linking_type[16]; // "static" or "dynamic"
    char library_path[1024];
    char profile[32]; // Build profile: "dev", "release", "optimize", etc.
    bool no_libdh; // Flag to disable dh support for this library
} LibraryConfig;

// Array of libraries from lib.dh
typedef struct {
    LibraryConfig* libraries;
    int count;
} LibraryConfigArray;

// Configuration
typedef struct {
    char project_name[256];
    char project_root[1024];
    char compiler[64];
    char c_standard[8];
    char output_dir[1024];
    bool is_test;
    DebugLevel debug_level;
    OptimizationLevel optimization_level;
    bool assertions_enabled;
    ExtraFlags extra_flags;
    char linked_libraries[1024];
    char run_args[1024];
    char compiler_args[1024]; // Additional arguments for compiler
    bool is_single_file;
    char single_file[1024];
    char dh_path[1024];
    bool no_libdh; // Flag to skip DH library
    char build_config_name[32]; // Name of selected build configuration
    bool show_commands; // Flag to control printing of commands
    bool verbose; // Flag for verbose logging
    bool use_output_suffix; // Flag to enable output suffix (dev, release, etc.)
    LibraryConfigArray lib_configs; // Libraries from lib.dh
    bool is_sample; // Flag to indicate building a sample file
    char sample_library[256]; // Library name if sample is in a library
    char sample_library_path[1024]; // Full path to library directory if sample is in a library
    char explicit_library[256]; // Explicitly specified library to link
    char original_cwd[1024]; // Original working directory before project root detection
    bool build_as_library; // Flag to build project as a library
    char library_type[16]; // "static" or "dynamic" for library builds
    bool clean_cache_only; // Flag to clean only .cache directories
    bool clean_recur; // Flag to recursively clean dependencies
} BuildConfig;

// Build configuration presets
typedef struct {
    char name[32];
    DebugLevel debug_level;
    OptimizationLevel optimization_level;
    bool assertions_enabled;
    ExtraFlags extra_flags;
} BuildConfigPreset;

// Function prototypes
bool detect_clang();
void detect_project_structure(BuildConfig* config);
void collect_source_files(BuildConfig* config, char*** sources, int* source_count);
void collect_include_paths(BuildConfig* config, char*** includes, int* include_count);
void parse_libraries(BuildConfig* config, char* libs_file);
void build_project(BuildConfig* config);
void build_project_as_library(BuildConfig* config);
void run_executable(BuildConfig* config);
void clean_project(BuildConfig* config, bool clean_build, bool clean_lib);
void print_usage();
void free_string_array(char** array, int count);
bool find_dh_path(BuildConfig* config);
bool is_c_source_file(const char* filename);
void detect_build_type(BuildConfig* config, int argc, const char* argv[]);
void add_dh_includes(BuildConfig* config, char*** includes, int* include_count);
void add_dh_sources(BuildConfig* config, char*** sources, int* source_count);
void create_workspace(const char* name, const char* dh_path);
void create_project(const char* name, const char* dh_path);
void write_file(const char* path, const char* content);
bool create_directory(const char* path);
bool find_dh_path_from_env(BuildConfig* config);
void write_example_files(const char* project_path);
void create_project_clangd(const char* project_path, const char* dh_path);
void apply_build_preset(BuildConfig* config, const char* preset_name);
const char* debug_level_to_flag(DebugLevel level);
const char* optimization_level_to_flag(OptimizationLevel level);
void init_build_config(BuildConfig* config);
void free_extra_flags(ExtraFlags* flags);
bool get_executable_path(char* buffer, size_t size);
bool check_blocksruntime_lib(const char* dh_path, char* out_lib_path, size_t path_size);
bool compile_blocksruntime_lib(const char* dh_path, const char* out_lib_path, bool verbose);

// Library management functions
bool parse_lib_dh_file(BuildConfig* config, const char* lib_dh_path);
bool compile_libraries(BuildConfig* config);
bool compile_single_library(BuildConfig* config, const LibraryConfig* lib_config);
bool copy_library_includes(const char* lib_src_path, const char* project_lib_path, const char* lib_name, bool verbose);
void add_lib_dh_includes(BuildConfig* config, char*** includes, int* include_count);
void add_lib_dh_libraries(BuildConfig* config);
void free_library_configs(LibraryConfigArray* lib_configs);
bool create_library_command(BuildConfig* config);
char* optimization_level_to_lib_flag(const char* opt_level);
void extract_library_dir_and_name(const char* lib_name, char* dir_part, char* name_part);
bool create_directory_recursive(const char* path);
bool copy_directory_recursive(const char* src, const char* dest, bool verbose);
bool move_directory_to_cache(const char* src_dir, const char* cache_base, const char* cache_subdir, bool verbose);
bool find_project_root(const char* start_path, char* project_root, size_t root_size);
bool detect_sample_library(const char* file_path, char* library_name, size_t name_size);
bool compile_library_for_sample(BuildConfig* config, const char* library_name);
bool compile_library_directory_as_library(BuildConfig* config, const char* library_path, const char* library_name);
void add_library_linking_for_sample(BuildConfig* config, const char* library_name);
void add_library_includes_for_sample(BuildConfig* config, const char* library_name, char*** includes, int* include_count);
bool detect_sample_library_path(const char* file_path, char* library_path, size_t path_size, char* library_name, size_t name_size);

// Add forward declarations for functions
void init_build_presets(void);
void cleanup_build_presets(void);
bool load_template_file(const char* filename, char** template_content, bool verbose);
void initialize_templates(const char* dh_path, bool verbose);
void cleanup_templates(void);
void create_templates_directory(const char* dh_path, bool verbose);

// Predefined build configurations
BuildConfigPreset build_presets[]
    = { { .name = "dev", .debug_level = DEBUG_EXTENDED, .optimization_level = OPT_DEBUG, .assertions_enabled = true, .extra_flags = { NULL, 0 } },
        { .name = "test", .debug_level = DEBUG_STANDARD, .optimization_level = OPT_BASIC, .assertions_enabled = true, .extra_flags = { NULL, 0 } },
        { .name = "profile", .debug_level = DEBUG_STANDARD, .optimization_level = OPT_BALANCED, .assertions_enabled = true, .extra_flags = { NULL, 0 } },
        { .name = "stable", .debug_level = DEBUG_MINIMAL, .optimization_level = OPT_BALANCED, .assertions_enabled = false, .extra_flags = { NULL, 0 } },
        { .name = "release", .debug_level = DEBUG_MINIMAL, .optimization_level = OPT_AGGRESSIVE, .assertions_enabled = false, .extra_flags = { NULL, 0 } },
        { .name = "optimize", .debug_level = DEBUG_NONE, .optimization_level = OPT_AGGRESSIVE, .assertions_enabled = false, .extra_flags = { NULL, 0 } },
        { .name = "compact", .debug_level = DEBUG_NONE, .optimization_level = OPT_SIZE, .assertions_enabled = false, .extra_flags = { NULL, 0 } },
        { .name = "micro", .debug_level = DEBUG_NONE, .optimization_level = OPT_SIZE_EXTREME, .assertions_enabled = false, .extra_flags = { NULL, 0 } } };

const int NUM_BUILD_PRESETS = sizeof(build_presets) / sizeof(BuildConfigPreset);

// Cross-platform wrapper for getting absolute path
bool get_absolute_path(const char* path, char* resolved_path, size_t resolved_path_size) {
#ifdef _WIN32
    DWORD result = GetFullPathNameA(path, (DWORD)resolved_path_size, resolved_path, NULL);
    return result > 0 && result < resolved_path_size;
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
        int result = snprintf(path, sizeof(path), "%s%s%s", dir_path, PATH_SEPARATOR, entry->d_name);
        if (result < 0 || (size_t)result >= sizeof(path)) {
            continue; // Path too long, skip
        }

        struct stat st = {};
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recurse into subdirectory
                find_source_files(path, sources, source_count);
            } else if (S_ISREG(st.st_mode)) {
                // Check if it's a C source file or object file (for BlocksRuntime)
                const char* ext = strrchr(entry->d_name, '.');
                if (ext && (strcmp(ext, ".c") == 0 || strcmp(ext, ".o") == 0)) {
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
    // NOLINTNEXTLINE(cert-env33-c)
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

// Find project root by walking up directory tree
bool find_project_root(const char* start_path, char* project_root, size_t root_size) {
    char temp_path[1024] = {};
    if (!get_absolute_path(start_path, temp_path, sizeof(temp_path))) {
        strncpy(temp_path, start_path, sizeof(temp_path) - 1);
        temp_path[sizeof(temp_path) - 1] = '\0';
    }

    while (strlen(temp_path) > 3) { // Minimum path length (e.g., C:\ on Windows)
        char include_path[1024] = {};
        char src_path[1024] = {};
        (void)snprintf(include_path, sizeof(include_path), "%s%sinclude", temp_path, PATH_SEPARATOR);
        (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", temp_path, PATH_SEPARATOR);

        if (dir_exists(include_path) && dir_exists(src_path)) {
            strncpy(project_root, temp_path, root_size - 1);
            project_root[root_size - 1] = '\0';
            return true;
        }

        // Move to parent directory
        char* last_separator = strrchr(temp_path, PATH_SEPARATOR[0]);
        if (!last_separator) { break; }
        *last_separator = '\0';
    }

    return false;
}

// Detect project structure and fill config
void detect_project_structure(BuildConfig* config) {
    // Save original working directory
    if (getcwd(config->original_cwd, sizeof(config->original_cwd)) == NULL) {
        (void)fprintf(stderr, "Error: Could not get current working directory\n");
        exit(1);
    }

    char cwd[1024] = {};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        (void)fprintf(stderr, "Error: Could not get current working directory\n");
        exit(1);
    }

    // Find the project root by walking up the directory tree
    if (!find_project_root(cwd, config->project_root, sizeof(config->project_root))) {
        (void)fprintf(stderr, "Error: Could not find project root (looking for include/ and src/ directories)\n");
        exit(1);
    }

    // Extract project name from root path
    char* last_slash = strrchr(config->project_root, PATH_SEPARATOR[0]);
    if (last_slash) {
        strncpy(config->project_name, last_slash + 1, sizeof(config->project_name) - 1);
    } else {
        strncpy(config->project_name, config->project_root, sizeof(config->project_name) - 1);
    }

    printf("Detected project: %s at %s\n", config->project_name, config->project_root);

    // Check for required directories
    char include_path[1024] = {};
    char src_path[1024] = {};
    char libs_path[1024] = {};

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
        int result = snprintf(include_dir, sizeof(include_dir), "%s%sinclude", dh_home, PATH_SEPARATOR);
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
        "C:/dev/dasae/dasae-headers/dh",
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

// Detect if a file is in a library directory and extract library path and name
// General approach: file is in SOME_FOLDER/foo.c, parent of SOME_FOLDER is the library directory
bool detect_sample_library_path(const char* file_path, char* library_path, size_t path_size, char* library_name, size_t name_size) {
    char abs_path[1024] = {};
    if (!get_absolute_path(file_path, abs_path, sizeof(abs_path))) {
        strncpy(abs_path, file_path, sizeof(abs_path) - 1);
        abs_path[sizeof(abs_path) - 1] = '\0';
    }

    // Remove filename to get the directory containing the file
    char file_dir[1024] = {};
    strncpy(file_dir, abs_path, sizeof(file_dir) - 1);
    file_dir[sizeof(file_dir) - 1] = '\0';

    char* last_sep = strrchr(file_dir, PATH_SEPARATOR[0]);
    if (!last_sep) {
        return false; // No path separator found
    }
    *last_sep = '\0'; // Remove filename, now file_dir is the folder containing the file

    // The parent of file_dir should be the library directory
    // Check if parent directory has src/ and include/ directories (library structure)
    char* parent_sep = strrchr(file_dir, PATH_SEPARATOR[0]);
    if (!parent_sep) {
        return false; // No parent directory
    }

    // Extract library directory path
    size_t lib_path_len = parent_sep - file_dir;
    if (lib_path_len > 0 && lib_path_len < path_size) {
        strncpy(library_path, file_dir, lib_path_len);
        library_path[lib_path_len] = '\0';

        // Extract library name (last component of library_path)
        char* lib_name_start = strrchr(library_path, PATH_SEPARATOR[0]);
        if (lib_name_start) {
            lib_name_start++; // Skip separator
        } else {
            lib_name_start = library_path;
        }

        size_t lib_name_len = strlen(lib_name_start);
        if (lib_name_len > 0 && lib_name_len < name_size) {
            strncpy(library_name, lib_name_start, lib_name_len);
            library_name[lib_name_len] = '\0';

            // Verify this is actually a library directory by checking for src/ or include/
            char lib_src_check[1024] = {};
            char lib_include_check[1024] = {};
            (void)snprintf(lib_src_check, sizeof(lib_src_check), "%s%ssrc", library_path, PATH_SEPARATOR);
            (void)snprintf(lib_include_check, sizeof(lib_include_check), "%s%sinclude", library_path, PATH_SEPARATOR);

            // It's a library if it has src/ or include/ directory
            if (dir_exists(lib_src_check) || dir_exists(lib_include_check)) { return true; }
        }
    }

    return false;
}

// Legacy function for backward compatibility
bool detect_sample_library(const char* file_path, char* library_name, size_t name_size) {
    char library_path[1024] = {};
    return detect_sample_library_path(file_path, library_path, sizeof(library_path), library_name, name_size);
}

// Detect if we're building a single file or a project
void detect_build_type(BuildConfig* config, int argc, const char* argv[]) {
    config->is_single_file = false;
    config->is_sample = false;

    // Check for --sample flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--sample") == 0) {
            config->is_sample = true;
            break;
        }
    }

    // Check if a filename was provided as an argument
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && is_c_source_file(argv[i]) && file_exists(argv[i])) {
            config->is_single_file = true;

            // Use cross-platform wrapper instead of realpath
            if (!get_absolute_path(argv[i], config->single_file, sizeof(config->single_file))) {
                // Fallback if get_absolute_path fails
                strcpy(config->single_file, argv[i]);
            }

            // If --sample flag is set, try to detect library context
            if (config->is_sample) {
                if (detect_sample_library_path(
                        config->single_file, config->sample_library_path, sizeof(config->sample_library_path), config->sample_library,
                        sizeof(config->sample_library)
                    )) {
                    if (config->verbose) { printf("Detected sample file in library: %s at %s\n", config->sample_library, config->sample_library_path); }
                }
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

            // Try to find project root from the file's location
            char file_dir[1024] = {};
            strcpy(file_dir, config->single_file);
            char* last_separator = strrchr(file_dir, PATH_SEPARATOR[0]);
            if (last_separator) {
                *(last_separator + 1) = '\0';
            }

            // Find project root by walking up from file location
            if (find_project_root(file_dir, config->project_root, sizeof(config->project_root))) {
                // Set output directory relative to project root
                (void)snprintf(config->output_dir, sizeof(config->output_dir), "%s%sbuild", config->project_root, PATH_SEPARATOR);
            } else {
                // Fallback: use file's directory
                strcpy(config->project_root, file_dir);
                strcpy(config->output_dir, config->project_root);
            }
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
        *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = strdup(dh_include);
        (*include_count)++;
    }

    // Add DH root
    *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
    (*includes)[*include_count] = strdup(config->dh_path);
    (*include_count)++;

    // Add BlocksRuntime include if it exists
    char blocks_include[1024] = {};
    (void)snprintf(blocks_include, sizeof(blocks_include), "%s%slibs%sBlocksRuntime%sinclude", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (dir_exists(blocks_include)) {
        *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
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

    // Find dh source files
    char dh_src_dir[1024] = {};
    (void)snprintf(dh_src_dir, sizeof(dh_src_dir), "%s%ssrc", config->dh_path, PATH_SEPARATOR);
    if (dir_exists(dh_src_dir)) {
        find_source_files(dh_src_dir, sources, source_count);
    }

    // For BlocksRuntime, we'll now check for the static library instead of directly adding source files
    // We'll handle linking the library separately in the build_project function
    char blocks_data_path[1024] = {};
    (void)snprintf(blocks_data_path, sizeof(blocks_data_path), "%s%slibs%sBlocksRuntime%ssrc%sdata.c", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);
    char blocks_runtime_path[1024] = {};
    (void)snprintf(blocks_runtime_path, sizeof(blocks_runtime_path), "%s%slibs%sBlocksRuntime%s.obj%sruntime.o", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

    if (file_exists(blocks_data_path)) {
        char** new_sources = realloc(*sources, (*source_count + 1) * sizeof(char*));
        if (new_sources != NULL) {
            *sources = new_sources;
            (*sources)[*source_count] = strdup(blocks_data_path);
            (*source_count)++;
        }
    }
    if (file_exists(blocks_runtime_path)) {
        char** new_sources = realloc(*sources, (*source_count + 1) * sizeof(char*));
        if (new_sources != NULL) {
            *sources = new_sources;
            (*sources)[*source_count] = strdup(blocks_runtime_path);
            (*source_count)++;
        }
    }
}

// Collect all include paths
void collect_include_paths(BuildConfig* config, char*** includes, int* include_count) {
    *includes = NULL;
    *include_count = 0;

    // Always add DH includes
    add_dh_includes(config, includes, include_count);

    // Add library includes from lib.dh
    add_lib_dh_includes(config, includes, include_count);

    // For sample files, add specific library include paths
    if (config->is_single_file && config->is_sample) {
        if (strlen(config->sample_library) > 0) { add_library_includes_for_sample(config, config->sample_library, includes, include_count); }
        if (strlen(config->explicit_library) > 0) { add_library_includes_for_sample(config, config->explicit_library, includes, include_count); }
    }

    if (config->is_single_file) {
        // Add the directory of the single file
        char* dir_path = strdup(config->single_file);
        char* last_separator = strrchr(dir_path, PATH_SEPARATOR[0]);
        if (last_separator) {
            *(last_separator + 1) = '\0';
        }

        *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
        (*includes)[*include_count] = dir_path;
        (*include_count)++;

        // Add ./include if it exists relative to the file
        char include_dir[1024] = {};
        (void)snprintf(include_dir, sizeof(include_dir), "%sinclude", dir_path);

        if (dir_exists(include_dir)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_dir);
            (*include_count)++;
        }
    } else {
        // Add standard include paths for a project
        char include_path[1024] = {};
        (void)snprintf(include_path, sizeof(include_path), "%s%sinclude", config->project_root, PATH_SEPARATOR);

        if (dir_exists(include_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(include_path);
            (*include_count)++;
        }

        // Add src path for internal includes
        char src_path[1024] = {};
        (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

        if (dir_exists(src_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(src_path);
            (*include_count)++;
        }

        // Add lib/deps path if it exists
        char project_lib_path[1024] = {};
        (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);
        char deps_path[1024] = {};
        (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

        if (dir_exists(deps_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            (*includes)[*include_count] = strdup(deps_path);
            (*include_count)++;
        }

        // Add libs paths if they exist
        char libs_path[1024] = {};
        (void)snprintf(libs_path, sizeof(libs_path), "%s%slibs", config->project_root, PATH_SEPARATOR);

        if (dir_exists(libs_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
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
                            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
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
    *sources = NULL;
    *source_count = 0;

    if (config->is_single_file) {
        // Add just the single file
        *sources = realloc(*sources, (*source_count + 1) * sizeof(char*));
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
    case OPT_DEBUG:
        return "-Og";
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
    config->debug_level = DEBUG_STANDARD;
    config->optimization_level = OPT_BALANCED;
    config->assertions_enabled = true;
    config->extra_flags.flags = NULL;
    config->extra_flags.count = 0;
    config->is_test = false;
    config->is_single_file = false;
    config->no_libdh = false;
    config->show_commands = false; // Default: don't show commands
    config->verbose = false; // Default: don't show verbose logs
    config->use_output_suffix = false; // Default: don't use output suffix
    strcpy(config->build_config_name, "dev"); // Default build config
    config->run_args[0] = '\0'; // Initialize run args
    config->compiler_args[0] = '\0'; // Initialize compiler args
    config->lib_configs.libraries = NULL; // Initialize library configs
    config->lib_configs.count = 0; // Initialize library count
    config->is_sample = false; // Initialize sample flag
    config->sample_library[0] = '\0'; // Initialize sample library
    config->sample_library_path[0] = '\0'; // Initialize sample library path
    config->explicit_library[0] = '\0'; // Initialize explicit library
    config->original_cwd[0] = '\0'; // Initialize original cwd
    config->build_as_library = false; // Initialize library build flag
    strcpy(config->library_type, "static"); // Default to static library
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
const char* DEFAULT_CLANGD_TEMPLATE = "CompileFlags:\n"
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
                                      "      - optimize*\n"
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
      "  - TypeOfUnqual\n"
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
      "  - S_const$\n"
      "  - S$\n"
      "  - S_const$$\n"
      "  - S$$\n"
      "  - SliS$$\n"
      "  - S_const$\n"
      "  - S$\n"
      "  - S_const$$\n"
      "  - S$$\n"
      "  # Array\n"
      "  - Arr$ \n"
      "  - Arr$$ \n"
      "  # Optional value\n"
      "  - Opt$ \n"
      "  - Opt$$ \n"
      "  # Error result\n"
      "  - E$ \n"
      "  - E$$ \n"
      "  - E$Void \n"
      "  - E$void \n"
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
      "  - eval_(T,_)=(T)({ _; })\n"
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
      "  - fn_TEST_scope(name)=E$void TEST_caseFn(void)\n"
      "  - TEST_unscoped=/**/\n"
      "  - fn_TEST_scope_ext(name)=E$void TEST_caseFn_ext(void)\n"
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

const char* DEFAULT_TASKS_JSON_TEMPLATE = "{\n"
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
                                          "        \"optimize\",\n"
                                          "        \"compact\",\n"
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
char* CLANGD_TEMPLATE = NULL;
char* CLANG_FORMAT_TEMPLATE = NULL;
char* TASKS_JSON_TEMPLATE = NULL;

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
    char* buffer_pos = *template_content;

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
    char** includes = NULL;
    int include_count = 0;

    char** sources = NULL;
    int source_count = 0;

    // For sample files, compile the associated library first
    if (config->is_single_file && config->is_sample) {
        // If sample library was detected, handle it
        if (strlen(config->sample_library) > 0 && strlen(config->sample_library_path) > 0) {
            // First, check if the library directory has its own lib.dh file
            char library_lib_dh_path[1024] = {};
            (void)snprintf(library_lib_dh_path, sizeof(library_lib_dh_path), "%s%slib.dh", config->sample_library_path, PATH_SEPARATOR);

            if (file_exists(library_lib_dh_path)) {
                // Save current lib_configs
                LibraryConfigArray saved_lib_configs = config->lib_configs;
                config->lib_configs.libraries = NULL;
                config->lib_configs.count = 0;

                // Parse the library's lib.dh
                if (parse_lib_dh_file(config, library_lib_dh_path)) {
                    // Compile dependencies from the library's lib.dh
                    if (config->lib_configs.count > 0) {
                        if (!compile_libraries(config)) {
                            (void)fprintf(stderr, "Failed to compile dependencies for library: %s\n", config->sample_library);
                            exit(1);
                        }

                        // Bring dependencies to sample library's lib/deps/ BEFORE compiling the library
                        char sample_lib_path[1024] = {};
                        (void)snprintf(sample_lib_path, sizeof(sample_lib_path), "%s%slib", config->sample_library_path, PATH_SEPARATOR);
                        char sample_deps_path[1024] = {};
                        (void)snprintf(sample_deps_path, sizeof(sample_deps_path), "%s%sdeps", sample_lib_path, PATH_SEPARATOR);

                        // Move existing lib/deps directory to cache if it exists
                        (void)move_directory_to_cache(sample_deps_path, sample_lib_path, "deps", config->verbose);

                        if (!dir_exists(sample_deps_path)) {
                            if (!create_directory_recursive(sample_deps_path)) {
                                (void)fprintf(stderr, "Could not create sample library deps directory: %s\n", sample_deps_path);
                                exit(1);
                            }
                        }

                        printf("Bring dependencies for '%s':\n", config->sample_library);

                        // Copy each library from its own build/profile/ to sample library's lib/deps/
                        for (int i = 0; i < config->lib_configs.count; ++i) {
                            const LibraryConfig* lib = &config->lib_configs.libraries[i];
                            char lib_dir_part[256] = {};
                            char lib_name_part[256] = {};
                            extract_library_dir_and_name(lib->library_name, lib_dir_part, lib_name_part);

                            // Source: library's own build/profile/ directory
                            char lib_build_path[1024] = {};
                            (void)snprintf(lib_build_path, sizeof(lib_build_path), "%s%sbuild", lib->library_path, PATH_SEPARATOR);

                            const char* effective_profile = lib->profile;
                            if (strcmp(lib->profile, "default") == 0) { effective_profile = config->build_config_name; }

                            char lib_profile_path[1024] = {};
                            (void)snprintf(lib_profile_path, sizeof(lib_profile_path), "%s%s%s", lib_build_path, PATH_SEPARATOR, effective_profile);

                            // Destination: sample library's lib/deps/dir_part/
                            char dest_dep_dir[1024] = {};
                            if (strlen(lib_dir_part) > 0) {
                                (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s%s%s", sample_deps_path, PATH_SEPARATOR, lib_dir_part);
                            } else {
                                (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s", sample_deps_path);
                            }

                            // Copy library binary from build/profile/ to lib/deps/
                            char src_lib_file[1024] = {};
                            char dest_lib_file[1024] = {};
#ifdef _WIN32
                            (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%s%s.lib", lib_profile_path, PATH_SEPARATOR, lib_name_part);
                            (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%s%s.lib", dest_dep_dir, PATH_SEPARATOR, lib_name_part);
#else
                            (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%slib%s.a", lib_profile_path, PATH_SEPARATOR, lib_name_part);
                            (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%slib%s.a", dest_dep_dir, PATH_SEPARATOR, lib_name_part);
#endif

                            if (file_exists(src_lib_file)) {
                                if (!dir_exists(dest_dep_dir)) {
                                    if (!create_directory_recursive(dest_dep_dir)) {
                                        (void)fprintf(stderr, "Failed to create dependency directory: %s\n", dest_dep_dir);
                                        exit(1);
                                    }
                                }

                                FILE* src_fp = fopen(src_lib_file, "rb");
                                if (src_fp) {
                                    FILE* dest_fp = fopen(dest_lib_file, "wb");
                                    if (dest_fp) {
                                        char buffer[4096] = {};
                                        size_t bytes = 0;
                                        while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
                                            if (fwrite(buffer, 1, bytes, dest_fp) != bytes) {
                                                (void)fprintf(stderr, "Failed to write to %s\n", dest_lib_file);
                                                (void)fclose(dest_fp);
                                                (void)fclose(src_fp);
                                                exit(1);
                                            }
                                        }
                                        (void)fclose(dest_fp);
                                    }
                                    (void)fclose(src_fp);
                                }
                            }

                            // Copy library's lib/deps/ (transitive dependencies)
                            char lib_lib_path_for_deps[1024] = {};
                            (void)snprintf(lib_lib_path_for_deps, sizeof(lib_lib_path_for_deps), "%s%slib", lib->library_path, PATH_SEPARATOR);
                            char lib_deps_path[1024] = {};
                            (void)snprintf(lib_deps_path, sizeof(lib_deps_path), "%s%sdeps", lib_lib_path_for_deps, PATH_SEPARATOR);

                            printf("- '%s' <- '%s'\n", lib->library_name, lib->library_path);

                            if (dir_exists(lib_deps_path)) {
                                (void)copy_directory_recursive(lib_deps_path, dest_dep_dir, false);
                            }

                            // Copy library includes to sample library's lib/deps/
                            if (!copy_library_includes(lib->library_path, sample_lib_path, lib->library_name, false)) {
                                (void)fprintf(stderr, "Failed to copy includes for library: %s\n", lib->library_name);
                                exit(1);
                            }
                        }
                    }
                }

                // Restore original lib_configs
                free_library_configs(&config->lib_configs);
                config->lib_configs = saved_lib_configs;
            }

            // Compile the library directory's src/ as a library
            if (!compile_library_directory_as_library(config, config->sample_library_path, config->sample_library)) {
                (void)fprintf(stderr, "Failed to compile library directory for sample: %s\n", config->sample_library);
                exit(1);
            }
        }
        // If explicit library was specified, compile it (from project's lib.dh)
        if (strlen(config->explicit_library) > 0) {
            if (!compile_library_for_sample(config, config->explicit_library)) {
                (void)fprintf(stderr, "Failed to compile library: %s\n", config->explicit_library);
                exit(1);
            }
        }
    }

    // Compile libraries from lib.dh first if any exist
    if (config->lib_configs.count > 0) {
        // Print dependency tree for project
        printf("Compiling dependencies '%s':\n", config->project_name);
        for (int i = 0; i < config->lib_configs.count; ++i) { printf("- %s\n", config->lib_configs.libraries[i].library_name); }

        if (!compile_libraries(config)) {
            (void)fprintf(stderr, "Failed to compile libraries\n");
            exit(1);
        }

        // After compiling libraries, bring them to project's lib/deps/
        char project_lib_path[1024] = {};
        (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);
        char deps_path[1024] = {};
        (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

        // Move existing lib/deps directory to cache if it exists
        (void)move_directory_to_cache(deps_path, project_lib_path, "deps", config->verbose);

        if (!dir_exists(deps_path)) {
            if (!create_directory_recursive(deps_path)) {
                (void)fprintf(stderr, "Could not create deps directory: %s\n", deps_path);
                exit(1);
            }
        }

        printf("Bring dependencies '%s':\n", config->project_name);

        // Copy each library from its own lib/ and lib/deps/ to project's lib/deps/
        for (int i = 0; i < config->lib_configs.count; ++i) {
            const LibraryConfig* lib = &config->lib_configs.libraries[i];
            char lib_dir_part[256] = {};
            char lib_name_part[256] = {};
            extract_library_dir_and_name(lib->library_name, lib_dir_part, lib_name_part);

            // Source: library's own build/profile/ directory
            char lib_build_path[1024] = {};
            (void)snprintf(lib_build_path, sizeof(lib_build_path), "%s%sbuild", lib->library_path, PATH_SEPARATOR);

            // Determine effective profile for this library
            const char* effective_profile = lib->profile;
            if (strcmp(lib->profile, "default") == 0) { effective_profile = config->build_config_name; }

            char lib_profile_path[1024] = {};
            (void)snprintf(lib_profile_path, sizeof(lib_profile_path), "%s%s%s", lib_build_path, PATH_SEPARATOR, effective_profile);

            // Destination: project's lib/deps/dir_part/ (not including lib_name_part as a directory)
            // Library binary goes to lib/deps/dir_part/lib_name_part.lib
            char dest_dep_dir[1024] = {};
            if (strlen(lib_dir_part) > 0) {
                (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s%s%s", deps_path, PATH_SEPARATOR, lib_dir_part);
            } else {
                (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s", deps_path);
            }

            // Copy library binary from build/profile/ to lib/deps/dir_part/lib_name_part.lib
            char src_lib_file[1024] = {};
            char dest_lib_file[1024] = {};
#ifdef _WIN32
            (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%s%s.lib", lib_profile_path, PATH_SEPARATOR, lib_name_part);
            (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%s%s.lib", dest_dep_dir, PATH_SEPARATOR, lib_name_part);
#else
            (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%slib%s.a", lib_profile_path, PATH_SEPARATOR, lib_name_part);
            (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%slib%s.a", dest_dep_dir, PATH_SEPARATOR, lib_name_part);
#endif

            if (file_exists(src_lib_file)) {
                if (!dir_exists(dest_dep_dir)) {
                    if (!create_directory_recursive(dest_dep_dir)) {
                        (void)fprintf(stderr, "Failed to create destination directory: %s\n", dest_dep_dir);
                        exit(1);
                    }
                }

                FILE* src_fp = fopen(src_lib_file, "rb");
                if (src_fp) {
                    FILE* dest_fp = fopen(dest_lib_file, "wb");
                    if (dest_fp) {
                        char buffer[4096] = {};
                        size_t bytes = 0;
                        while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
                            if (fwrite(buffer, 1, bytes, dest_fp) != bytes) {
                                (void)fprintf(stderr, "Failed to write to %s\n", dest_lib_file);
                                (void)fclose(dest_fp);
                                (void)fclose(src_fp);
                                exit(1);
                            }
                        }
                        (void)fclose(dest_fp);
                    }
                    (void)fclose(src_fp);
                }
            }

            // Copy library's lib/deps/ (transitive dependencies) to project's lib/deps/
            char lib_lib_path_for_deps[1024] = {};
            (void)snprintf(lib_lib_path_for_deps, sizeof(lib_lib_path_for_deps), "%s%slib", lib->library_path, PATH_SEPARATOR);
            char lib_deps_path[1024] = {};
            (void)snprintf(lib_deps_path, sizeof(lib_deps_path), "%s%sdeps", lib_lib_path_for_deps, PATH_SEPARATOR);

            // Print what's being copied
            char dest_path_str[1024] = {};
            if (strlen(lib_dir_part) > 0) {
                (void)snprintf(
                    dest_path_str, sizeof(dest_path_str), "%s%slib%sdeps%s%s%s%s", config->project_name, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR,
                    lib_dir_part, PATH_SEPARATOR, lib_name_part
                );
            } else {
                (void)snprintf(
                    dest_path_str, sizeof(dest_path_str), "%s%slib%sdeps%s%s", config->project_name, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR,
                    lib_name_part
                );
            }
            printf("- '%s' <- '%s'\n", dest_path_str, lib->library_name);

            if (dir_exists(lib_deps_path)) {
                // Copy entire deps directory recursively (transitive dependencies)
                (void)copy_directory_recursive(lib_deps_path, dest_dep_dir, false);
            }

            // Copy library includes to project's lib/deps/
            if (!copy_library_includes(lib->library_path, project_lib_path, lib->library_name, false)) {
                (void)fprintf(stderr, "Failed to copy includes for library: %s\n", lib->library_name);
                exit(1);
            }
        }
    }

    // Collect include paths and source files
    collect_include_paths(config, &includes, &include_count);
    collect_source_files(config, &sources, &source_count);

    // Handle BlocksRuntime library if needed
    char blocks_lib_path[1024] = {};
    bool needs_blocks_runtime = false;
    bool has_blocks_lib = false;
    bool blocks_sources_added = false;

    // Only check for BlocksRuntime if not using --no-libdh
    if (!config->no_libdh) {
        needs_blocks_runtime = true;

        // Check if BlocksRuntime library exists
        has_blocks_lib = check_blocksruntime_lib(config->dh_path, blocks_lib_path, sizeof(blocks_lib_path));

        // If not, try to compile it
        if (!has_blocks_lib) {
            // Create a path for the new library
            char lib_dir[1024] = {};

            (void)snprintf(lib_dir, sizeof(lib_dir), "%s%slibs%sBlocksRuntime%s.bin", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

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
                    char data_c[1024] = {};
                    char runtime_c[1024] = {};
                    (void)snprintf(data_c, sizeof(data_c), "%s%sdata.c", blocks_src_dir, PATH_SEPARATOR);
                    (void)snprintf(runtime_c, sizeof(runtime_c), "%s%sruntime.c", blocks_src_dir, PATH_SEPARATOR);

                    if (file_exists(data_c)) {
                        char** new_sources = realloc(sources, (source_count + 1) * sizeof(char*));
                        if (new_sources != NULL) {
                            sources = new_sources;
                            sources[source_count] = strdup(data_c);
                            source_count++;
                            blocks_sources_added = true;
                        }
                    }

                    if (file_exists(runtime_c)) {
                        char** new_sources = realloc(sources, (source_count + 1) * sizeof(char*));
                        if (new_sources != NULL) {
                            sources = new_sources;
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
#ifdef _WIN32
    strcat(command, " -fms-extensions");
#endif /* _WIN32 */

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

    // Output goes to build/profile/ directory
    char profile_output_dir[1024] = {};
    (void)snprintf(profile_output_dir, sizeof(profile_output_dir), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->build_config_name);

    if (!dir_exists(profile_output_dir)) {
        if (!create_directory_recursive(profile_output_dir)) {
            (void)fprintf(stderr, "Could not create profile output directory: %s\n", profile_output_dir);
            exit(1);
        }
    }

    // Add output file
    char output_file[1024] = {};
    (void)snprintf(output_file, sizeof(output_file), "%s%s%s", profile_output_dir, PATH_SEPARATOR, config->project_name);

    // Add test suffix to output file if enabled (profile is already in the path)
    if (config->is_test) { strcat(output_file, "-TEST"); }

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

    // For sample files, add library linking
    if (config->is_single_file && config->is_sample) {
        // Link the library directory itself (compiled from src/ to build/profile/)
        if (strlen(config->sample_library) > 0 && strlen(config->sample_library_path) > 0) {
            // Sample library is compiled to sample_library_path/build/profile/
            char library_build_path[1024] = {};
            (void)snprintf(library_build_path, sizeof(library_build_path), "%s%sbuild%s%s", config->sample_library_path, PATH_SEPARATOR, PATH_SEPARATOR, config->build_config_name);

            // Add library search path
            if (config->linked_libraries[0] != '\0') { strcat(config->linked_libraries, " "); }
            strcat(config->linked_libraries, "-L");
            strcat(config->linked_libraries, library_build_path);

            // Add the library file itself
            char lib_file[1024] = {};
#ifdef _WIN32
            (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.lib", library_build_path, PATH_SEPARATOR, config->sample_library);
#else
            (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.a", library_build_path, PATH_SEPARATOR, config->sample_library);
#endif
            strcat(config->linked_libraries, " \"");
            strcat(config->linked_libraries, lib_file);
            strcat(config->linked_libraries, "\"");
        }
        // Link explicit library (from project's lib.dh)
        if (strlen(config->explicit_library) > 0) { add_library_linking_for_sample(config, config->explicit_library); }
    }

    // Add library linking flags from lib.dh first (this also includes linked_libraries from link.dhc)
    add_lib_dh_libraries(config);

    // Add all linked libraries (includes both link.dhc and lib.dh libraries)
    if (config->linked_libraries[0] != '\0') {
        strcat(command, " ");
        strcat(command, config->linked_libraries);
    }

    // Add pthread library (required for threading support)
    strcat(command, " -lpthread");
#ifdef _WIN32
    // strcat(command, " -luser32");
    strcat(command, " -lws2_32");
    // strcat(command, " -lbcrypt");
    // strcat(command, " -ladvapi32");
    strcat(command, " -lsynchronization");
    strcat(command, " -lkernel32");
// strcat(command, " -lntdll");
#elif __linux__
    strcat(command, " -lrt");
    strcat(command, " -lm");
    strcat(command, " -ldl");
#elif __APPLE__
    strcat(command, " -framework CoreFoundation");
    strcat(command, " -lm");
#endif
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

// Build the project as a library
void build_project_as_library(BuildConfig* config) {
    char** includes = NULL;
    int include_count = 0;

    char** sources = NULL;
    int source_count = 0;

    // Compile libraries from lib.dh first if any exist
    if (config->lib_configs.count > 0) {
        if (!compile_libraries(config)) {
            (void)fprintf(stderr, "Failed to compile libraries\n");
            exit(1);
        }
    }

    // Collect include paths and source files (only from src/, not libs/)
    collect_include_paths(config, &includes, &include_count);

    // Only collect source files from src/ directory, not from libs/
    char src_path[1024] = {};
    (void)snprintf(src_path, sizeof(src_path), "%s%ssrc", config->project_root, PATH_SEPARATOR);

    if (dir_exists(src_path)) { find_source_files(src_path, &sources, &source_count); }

    if (source_count == 0) {
        (void)fprintf(stderr, "Error: No source files found in src/ directory\n");
        exit(1);
    }

    // Create build/profile directory
    char build_output_dir[1024] = {};
    (void)snprintf(build_output_dir, sizeof(build_output_dir), "%s%sbuild", config->project_root, PATH_SEPARATOR);

    char profile_output_dir[1024] = {};
    (void)snprintf(profile_output_dir, sizeof(profile_output_dir), "%s%s%s", build_output_dir, PATH_SEPARATOR, config->build_config_name);

    // Move existing build/profile directory to cache if it exists
    (void)move_directory_to_cache(profile_output_dir, build_output_dir, config->build_config_name, config->verbose);

    if (!dir_exists(profile_output_dir)) {
        if (!create_directory_recursive(profile_output_dir)) {
            (void)fprintf(stderr, "Error: Could not create build directory\n");
            exit(1);
        }
    }

    // Prepare base compilation command
    char base_command[8192] = {};
    strcat(base_command, config->compiler);
    strcat(base_command, " -std=");
    strcat(base_command, config->c_standard);

    // Add include paths
    for (int i = 0; i < include_count; ++i) {
        strcat(base_command, " -I");
        strcat(base_command, includes[i]);
    }

    // Add compile flags
    strcat(base_command, " -DCOMP");
    if (config->is_test) { strcat(base_command, " -DCOMP_TEST"); }

    // Only add -funsigned-char -fblocks if not using --no-libdh
    if (!config->no_libdh) { strcat(base_command, " -funsigned-char -fblocks -DBlocksRuntime_STATIC"); }
#ifdef _WIN32
    strcat(base_command, " -fms-extensions");
#endif /* _WIN32 */
    // Add debug level flag
    const char* debug_flag = debug_level_to_flag(config->debug_level);
    if (strlen(debug_flag) > 0) {
        strcat(base_command, " ");
        strcat(base_command, debug_flag);
    }

    // Add optimization level flag
    const char* opt_flag = optimization_level_to_flag(config->optimization_level);
    if (strlen(opt_flag) > 0) {
        strcat(base_command, " ");
        strcat(base_command, opt_flag);
    }

    // Add NDEBUG if assertions are disabled
    if (!config->assertions_enabled) { strcat(base_command, " -DNDEBUG"); }

    // Add extra flags
    for (int i = 0; i < config->extra_flags.count; ++i) {
        strcat(base_command, " ");
        strcat(base_command, config->extra_flags.flags[i]);
    }

    // Add compiler arguments
    if (config->compiler_args[0] != '\0') {
        strcat(base_command, " ");
        strcat(base_command, config->compiler_args);
    }

    // Create object files directory
    char obj_dir[1024] = {};
    (void)snprintf(obj_dir, sizeof(obj_dir), "%s%s.obj", profile_output_dir, PATH_SEPARATOR);

    if (!dir_exists(obj_dir)) {
        if (!create_directory(obj_dir)) {
            (void)fprintf(stderr, "Error: Could not create object directory\n");
            exit(1);
        }
    }

    // Compile each source file to object file
    char obj_files[16384] = {}; // Buffer for object file paths
    for (int i = 0; i < source_count; ++i) {
        char obj_file[1024] = {};
        char* filename = strrchr(sources[i], PATH_SEPARATOR[0]);
        if (!filename) {
            filename = sources[i];
        } else {
            filename++; // Skip separator
        }

        char basename[256] = {};
        strncpy(basename, filename, sizeof(basename) - 1);
        basename[sizeof(basename) - 1] = '\0';
        char* dot = strrchr(basename, '.');
        if (dot) { *dot = '\0'; }

        (void)snprintf(obj_file, sizeof(obj_file), "%s%s%s.o", obj_dir, PATH_SEPARATOR, basename);

        char compile_cmd[4096] = {};
        (void)snprintf(compile_cmd, sizeof(compile_cmd), "%s -c %s -o %s", base_command, sources[i], obj_file);

        if (config->show_commands) { printf("Command: %s\n", compile_cmd); }

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(compile_cmd) != 0) {
            (void)fprintf(stderr, "Failed to compile %s\n", sources[i]);
            free_string_array(includes, include_count);
            free_string_array(sources, source_count);
            exit(1);
        }

        // Add object file to list
        if (strlen(obj_files) > 0) { strcat(obj_files, " "); }
        strcat(obj_files, obj_file);
    }

    // Create library file
    char lib_file[1024] = {};
    if (strcmp(config->library_type, "static") == 0) {
#ifdef _WIN32
        (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.lib", profile_output_dir, PATH_SEPARATOR, config->project_name);
#else
        (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.a", profile_output_dir, PATH_SEPARATOR, config->project_name);
#endif
    } else {
#ifdef _WIN32
        (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.dll", profile_output_dir, PATH_SEPARATOR, config->project_name);
#else
        (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.so", profile_output_dir, PATH_SEPARATOR, config->project_name);
#endif
    }

    if (strcmp(config->library_type, "static") == 0) {
        // Create static library using ar
        char ar_cmd[8192] = {};
        (void)snprintf(ar_cmd, sizeof(ar_cmd), "ar rcs %s %s", lib_file, obj_files);

        if (config->show_commands) { printf("Command: %s\n", ar_cmd); }

        printf("Building static library %s with config '%s'...\n", config->project_name, config->build_config_name);

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(ar_cmd) != 0) {
            (void)fprintf(stderr, "Failed to create static library\n");
            free_string_array(includes, include_count);
            free_string_array(sources, source_count);
            exit(1);
        }
    } else {
        // Create dynamic library
        char link_cmd[8192] = {};
        (void)snprintf(link_cmd, sizeof(link_cmd), "%s -shared -o %s %s", base_command, lib_file, obj_files);

        // Add library linking flags from lib.dh
        add_lib_dh_libraries(config);
        if (config->linked_libraries[0] != '\0') {
            strcat(link_cmd, " ");
            strcat(link_cmd, config->linked_libraries);
        }

        if (config->show_commands) { printf("Command: %s\n", link_cmd); }

        printf("Building dynamic library %s with config '%s'...\n", config->project_name, config->build_config_name);

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(link_cmd) != 0) {
            (void)fprintf(stderr, "Failed to create dynamic library\n");
            free_string_array(includes, include_count);
            free_string_array(sources, source_count);
            exit(1);
        }
    }

    printf("Library built successfully: %s\n", lib_file);

    // Clean up
    free_string_array(includes, include_count);
    free_string_array(sources, source_count);
}

// Run the compiled executable
void run_executable(BuildConfig* config) {
    // Executable is in build/profile/ directory
    char profile_output_dir[1024] = {};
    (void)snprintf(profile_output_dir, sizeof(profile_output_dir), "%s%s%s", config->output_dir, PATH_SEPARATOR, config->build_config_name);

    char output_file[1024] = {};
    (void)snprintf(output_file, sizeof(output_file), "%s%s%s", profile_output_dir, PATH_SEPARATOR, config->project_name);

    // Add test suffix if enabled (profile is already in the path)
    if (config->is_test) { strcat(output_file, "-TEST"); }

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
    int result = snprintf(clangd_path, sizeof(clangd_path), "%s%s.clangd", project_path, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(clangd_path)) {
        (void)fprintf(stderr, "Error: Path too long for .clangd\n");
        return;
    }

    char abs_include_path[1024] = {};
    char abs_src_path[1024] = {};
    char dh_include_path[1024] = {};
    char blocks_include_path[1024] = {};
    char temp_path[1024] = {};

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
        project_clangd, sizeof(project_clangd),
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
        "      - optimize*\n"
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
        project_path, abs_include_path, abs_src_path, dh_include_path, blocks_include_path
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
    int result = snprintf(main_path, sizeof(main_path), "%s%ssrc%smain.c", project_path, PATH_SEPARATOR, PATH_SEPARATOR);
    if (result < 0 || (size_t)result >= sizeof(main_path)) {
        (void)fprintf(stderr, "Error: Path too long for main.c\n");
        return;
    }

    const char* main_content
        = "/// @file  main.c\n"
          "/// @brief Example DH-C program\n"
          "\n"
          "#include <dh/main.h>\n"
          "#include <dh/io/stream.h>\n"
          "\n"
          "TEST_fn_(\"simple test\" $scope) {\n"
          "    try_(TEST_expect(1 + 1 == 2));\n"
          "} $unscoped_(TEST_fn);\n"
          "\n"
          "fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {\n"
          "    let_ignore = args;\n"
          "    let message = u8_l(\"Hello\");\n"
          "    io_stream_println(u8_l(\"{:s}, world!\"), message);\n"
          "    return_ok({});\n"
          "} $unscoped_(fn);\n";

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
    char src_path[1024] = {};
    char lib_path[1024] = {}; // Changed from libs_path to lib_path

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
        custom_clangd_content, sizeof(custom_clangd_content),
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
        "      - optimize*\n"
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
        dh_include_path, blocks_include_path
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
    int result = snprintf(example_project_path, sizeof(example_project_path), "%s%shello-world", workspace_path, PATH_SEPARATOR);
    if (result >= 0 && (size_t)result < sizeof(example_project_path)) {
        create_project(example_project_path, dh_path);
    } else {
        (void)fprintf(stderr, "Warning: Could not create example project (path too long)\n");
    }

    printf("Workspace created successfully!\n");
}

// Update print_usage function to include --show-commands
void print_usage() {
    printf("Usage: dh-c [command] [options]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  --help, -h           - Display this help message\n");
    printf("  --version, -v        - Display version information\n");
    printf("  build                - Build the project or file\n");
    printf("  build-lib            - Build the project as a library (static or dynamic)\n");
    printf("  lib                  - Compile libraries specified in lib.dh file\n");
    printf("  test                 - Build and run tests\n");
    printf("  run                  - Build and run the project or file\n");
    printf("  clean                - Clean build and lib/deps directories\n");
    printf("  clean-build          - Clean build directories only\n");
    printf("  clean-lib            - Clean lib/deps directories only\n");
    printf("  workspace <.|name>   - Create workspace with config files (.clangd, .clang-format, .vscode)\n");
    printf("  project <.|name>     - Create a new project with DH-C structure\n");
    printf("\n");
    printf("Build configurations:\n");
    printf("  dev         - Extended debug info, debug optimization (-Og), assertions enabled, frame pointer kept\n");
    printf("  test        - Standard debug info, basic optimization (-O1), assertions enabled, frame pointer kept\n");
    printf("  profile     - Standard debug info, balanced optimization (-O2), assertions enabled, frame pointer kept\n");
    printf("  stable      - Minimal debug info, balanced optimization (-O2), assertions disabled, standard layout\n");
    printf("  release     - Minimal debug info, aggressive optimization (-O3), assertions disabled, dead code elimination\n");
    printf("  optimize    - No debug info, aggressive optimization (-O3), assertions disabled, max cache locality\n");
    printf("  compact     - No debug info, size optimization (-Os), assertions disabled, dead code elimination\n");
    printf("  micro       - No debug info, extreme size optimization (-Oz), assertions disabled, no loop unrolling\n");
    printf("\n");
    printf("Options:\n");
    printf("  --compiler=<clang|gcc>  - Specify compiler (default: clang)\n");
    printf("  --std=<c99|c11|c17>     - Specify C standard (default: c17)\n");
    printf("  --args=\"args\"           - For 'build': compiler args; For 'run'/'test': 'compiler -- program'\n");
    printf("  --dh=<path>             - Path to DH library (auto-detected by default)\n");
    printf("  --no-libdh              - Skip DH library\n");
    printf("  --show-commands         - Show commands being executed\n");
    printf("  --sample                - Build a sample file (auto-detects library context)\n");
    printf("  --lib=<name>            - Explicitly link a specific library with single file builds\n");
    printf("  --static                - Build as static library (for build-lib, default)\n");
    printf("  --dynamic               - Build as dynamic library (for build-lib)\n");
    printf("  --cache                 - Clean only .cache directories (for clean, clean-build, clean-lib)\n");
    printf("  --recur                 - Recursively clean dependencies (for clean, clean-build, clean-lib)\n");
    printf("\n");
    printf("Environment Variables:\n");
    printf("  DH_HOME               - Path to DH library installation\n");
    printf("\n");
    printf("Examples:\n");
    printf("  dh-c --help                      - Display help information\n");
    printf("  dh-c --version                   - Show version information\n");
    printf("  dh-c build dev                   - Build project in dev mode\n");
    printf("  dh-c build-lib release           - Build project as static library in release mode\n");
    printf("  dh-c build-lib dev --dynamic     - Build project as dynamic library in dev mode\n");
    printf("  dh-c test sample.c               - Build and run tests for single file\n");
    printf("  dh-c run release sample.c        - Build and run single file in release mode\n");
    printf("  dh-c build --sample sample.c     - Build sample file (auto-detects library)\n");
    printf("  dh-c build sample.c --lib=engine - Build single file with explicit library\n");
    printf("  dh-c lib --verbose               - Compile libraries from lib.dh with verbose output\n");
    printf("  dh-c clean                       - Clean build and lib/deps directories\n");
    printf("  dh-c clean-build                 - Clean build directories only\n");
    printf("  dh-c clean-lib                   - Clean lib/deps directories only\n");
    printf("  dh-c clean --cache               - Clean only .cache directories\n");
    printf("  dh-c clean --recur               - Recursively clean dependencies\n");
    printf("  dh-c clean dev --cache --recur   - Clean with profile and flags\n");
    printf("  dh-c workspace .                 - Create new workspace from current directory\n");
    printf("  dh-c project myproject           - Create new project named 'myproject'\n");
    printf("  dh-c build compact --no-libdh   - Build non-DH-C project with size optimization\n");
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
    } else if (strcmp(argv[1], "build-lib") == 0) {
        // Build as library
        config.build_as_library = true;
    } else if (strcmp(argv[1], "test") == 0) {
        // Apply test preset and run tests
        apply_build_preset(&config, "test");
        config.is_test = true;
    } else if (strcmp(argv[1], "run") == 0) {
        // Build and run the program
    } else if (strcmp(argv[1], "clean") == 0 || strcmp(argv[1], "clean-build") == 0 || strcmp(argv[1], "clean-lib") == 0) {
        // Handle clean commands - will be processed after flag parsing
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
                const char* args_str = argv[i] + 7;
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
        } else if (strncmp(argv[i], "--lib=", 6) == 0) {
            // Explicitly specify library to link
            strcpy(config.explicit_library, argv[i] + 6);
        } else if (strcmp(argv[i], "--static") == 0) {
            // Build as static library
            strcpy(config.library_type, "static");
        } else if (strcmp(argv[i], "--dynamic") == 0) {
            // Build as dynamic library
            strcpy(config.library_type, "dynamic");
        } else if (strcmp(argv[i], "--cache") == 0) {
            // Clean only .cache directories
            config.clean_cache_only = true;
        } else if (strcmp(argv[i], "--recur") == 0) {
            // Recursively clean dependencies
            config.clean_recur = true;
        }
    }

    // Handle clean commands (before detecting project structure to avoid duplicate detection)
    if (strcmp(argv[1], "clean") == 0 || strcmp(argv[1], "clean-build") == 0 || strcmp(argv[1], "clean-lib") == 0) {
        // Detect project structure for clean commands (only if not already detected)
        if (!config.is_single_file && strlen(config.project_root) == 0) { detect_project_structure(&config); }

        bool clean_build = (strcmp(argv[1], "clean") == 0 || strcmp(argv[1], "clean-build") == 0);
        bool clean_lib = (strcmp(argv[1], "clean") == 0 || strcmp(argv[1], "clean-lib") == 0);

        if (clean_build || clean_lib) {
            clean_project(&config, clean_build, clean_lib);
            printf("Clean completed successfully!\n");
        }

        // Clean up
        free_library_configs(&config.lib_configs);
        cleanup_build_presets();
        cleanup_templates();
        return 0;
    }

    // If not single file, detect project structure
    if (!config.is_single_file) {
        detect_project_structure(&config);
    } else if (config.is_single_file && strlen(config.project_root) > 0) {
        // For single files, ensure lib.dh is parsed if project root was found
        char lib_dh_path[1024] = {};
        (void)snprintf(lib_dh_path, sizeof(lib_dh_path), "%s%slib.dh", config.project_root, PATH_SEPARATOR);
        if (file_exists(lib_dh_path) && config.lib_configs.count == 0) { parse_lib_dh_file(&config, lib_dh_path); }
    }

    // Try to find DH if not specified
    if (strlen(config.dh_path) == 0) {
        find_dh_path(&config);
    }

    // Build the project or library
    if (config.build_as_library) {
        build_project_as_library(&config);
    } else {
        build_project(&config);
    }

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
    size_t written = fwrite(content, 1, content_len, file);
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
            preset_found = true;
            // Copy preset values
            config->debug_level = build_presets[i].debug_level;
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
        config->debug_level = DEBUG_EXTENDED;
        config->optimization_level = OPT_DEBUG;
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
    BuildConfigPreset* dev = &build_presets[0];
    dev->extra_flags.count = 3;
    dev->extra_flags.flags = malloc(dev->extra_flags.count * sizeof(char*));
    dev->extra_flags.flags[0] = strdup("-Wall");
    dev->extra_flags.flags[1] = strdup("-Wextra");
    dev->extra_flags.flags[2] = strdup("-fno-omit-frame-pointer");

    // test preset extra flags (warnings are NOT errors)
    BuildConfigPreset* test = &build_presets[1];
    test->extra_flags.count = 3;
    test->extra_flags.flags = malloc(test->extra_flags.count * sizeof(char*));
    test->extra_flags.flags[0] = strdup("-Wall");
    test->extra_flags.flags[1] = strdup("-Wextra");
    test->extra_flags.flags[2] = strdup("-fno-omit-frame-pointer");

    // profile preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* profile = &build_presets[2];
    profile->extra_flags.count = 5;
    profile->extra_flags.flags = malloc(profile->extra_flags.count * sizeof(char*));
    profile->extra_flags.flags[0] = strdup("-Wall");
    profile->extra_flags.flags[1] = strdup("-Wextra");
    profile->extra_flags.flags[2] = strdup("-Werror");
    profile->extra_flags.flags[4] = strdup("-fno-omit-frame-pointer");
    profile->extra_flags.flags[3] = strdup("-flto");

    // stable preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* stable = &build_presets[3];
    stable->extra_flags.count = 4;
    stable->extra_flags.flags = malloc(stable->extra_flags.count * sizeof(char*));
    stable->extra_flags.flags[0] = strdup("-Wall");
    stable->extra_flags.flags[1] = strdup("-Wextra");
    stable->extra_flags.flags[2] = strdup("-Werror");
    stable->extra_flags.flags[3] = strdup("-flto");

    // release preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* release = &build_presets[4];
    release->extra_flags.count = 7;
    release->extra_flags.flags = malloc(release->extra_flags.count * sizeof(char*));
    release->extra_flags.flags[0] = strdup("-Wall");
    release->extra_flags.flags[1] = strdup("-Wextra");
    release->extra_flags.flags[2] = strdup("-Werror");
    release->extra_flags.flags[3] = strdup("-flto");
    release->extra_flags.flags[4] = strdup("-Wl,--gc-sections");
    release->extra_flags.flags[5] = strdup("-fdata-sections");
    release->extra_flags.flags[6] = strdup("-ffunction-sections");

    // optimize preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* optimize = &build_presets[5];
    optimize->extra_flags.count = 4;
    optimize->extra_flags.flags = malloc(optimize->extra_flags.count * sizeof(char*));
    optimize->extra_flags.flags[0] = strdup("-Wall");
    optimize->extra_flags.flags[1] = strdup("-Wextra");
    optimize->extra_flags.flags[2] = strdup("-Werror");
    optimize->extra_flags.flags[3] = strdup("-flto");

    // compact preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* compact = &build_presets[6];
    compact->extra_flags.count = 7;
    compact->extra_flags.flags = malloc(compact->extra_flags.count * sizeof(char*));
    compact->extra_flags.flags[0] = strdup("-Wall");
    compact->extra_flags.flags[1] = strdup("-Wextra");
    compact->extra_flags.flags[2] = strdup("-Werror");
    compact->extra_flags.flags[3] = strdup("-flto");
    compact->extra_flags.flags[4] = strdup("-Wl,--gc-sections");
    compact->extra_flags.flags[5] = strdup("-fdata-sections");
    compact->extra_flags.flags[6] = strdup("-ffunction-sections");

    // micro preset extra flags (warnings ARE errors) + LTO
    BuildConfigPreset* micro = &build_presets[7];
    micro->extra_flags.count = 7;
    micro->extra_flags.flags = malloc(micro->extra_flags.count * sizeof(char*));
    micro->extra_flags.flags[0] = strdup("-Wall");
    micro->extra_flags.flags[1] = strdup("-Wextra");
    micro->extra_flags.flags[2] = strdup("-Werror");
    micro->extra_flags.flags[3] = strdup("-flto");
    micro->extra_flags.flags[4] = strdup("-Wl,--gc-sections");
    micro->extra_flags.flags[5] = strdup("-fdata-sections");
    micro->extra_flags.flags[6] = strdup("-ffunction-sections");
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
    (void)snprintf(lib_dir, sizeof(lib_dir), "%s%slibs%sBlocksRuntime%s.bin", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

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

    char data_c[1024] = {};
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
    (void)snprintf(obj_dir, sizeof(obj_dir), "%s%slibs%sBlocksRuntime%s.obj", dh_path, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR);

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
        lib_configs->count = 0;
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

    char line[1024] = {};
    LibraryConfig current_lib = {};
    bool in_section = false;

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
                config->lib_configs.libraries = temp_libs;
                config->lib_configs.libraries[config->lib_configs.count] = current_lib;
                config->lib_configs.count++;
            }

            // Start new library
            memset(&current_lib, 0, sizeof(LibraryConfig));
            strncpy(current_lib.library_name, line + 1, strlen(line) - 2);
            current_lib.library_name[strlen(line) - 2] = '\0';
            current_lib.no_libdh = false; // Default to using dh
            strcpy(current_lib.profile, "default"); // Default profile: follow main project
            in_section = true;
            continue;
        }

        // Parse key=value pairs
        if (in_section) {
            char* equals = strchr(line, '=');
            if (equals) {
                *equals = '\0';
                char* key = line;
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
                    // Resolve path relative to lib.dh file's directory
                    char lib_dh_dir[1024] = {};
                    strncpy(lib_dh_dir, lib_dh_path, sizeof(lib_dh_dir) - 1);
                    lib_dh_dir[sizeof(lib_dh_dir) - 1] = '\0';

                    // Get directory containing lib.dh file
                    char* last_sep = strrchr(lib_dh_dir, PATH_SEPARATOR[0]);
                    if (last_sep) {
                        *last_sep = '\0';
                    } else {
                        // If no separator, lib.dh is in current directory
                        strcpy(lib_dh_dir, ".");
                    }

                    // Construct full path: lib_dh_dir + PATH_SEPARATOR + value
                    char resolved_path[1024] = {};
                    if (strlen(lib_dh_dir) > 0) {
                        (void)snprintf(resolved_path, sizeof(resolved_path), "%s%s%s", lib_dh_dir, PATH_SEPARATOR, value);
                    } else {
                        strncpy(resolved_path, value, sizeof(resolved_path) - 1);
                    }
                    resolved_path[sizeof(resolved_path) - 1] = '\0';

                    // Convert to absolute path
                    if (!get_absolute_path(resolved_path, current_lib.library_path, sizeof(current_lib.library_path))) {
                        // Fallback: use resolved path as-is if get_absolute_path fails
                        strncpy(current_lib.library_path, resolved_path, sizeof(current_lib.library_path) - 1);
                        current_lib.library_path[sizeof(current_lib.library_path) - 1] = '\0';
                    }
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
        config->lib_configs.libraries = temp_libs;
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

                char buffer[4096];
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

// Get current timestamp as a string (format: YYYYMMDDHHMMSS)
static void get_timestamp_string(char* buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    (void)snprintf(
        buffer, buffer_size, "%04d%02d%02d%02d%02d%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,
        timeinfo->tm_sec
    );
}

// Move an existing directory to cache location
// Returns true if moved successfully or if directory didn't exist, false on error
bool move_directory_to_cache(const char* src_dir, const char* cache_base, const char* cache_subdir, bool verbose) {
    if (!dir_exists(src_dir)) {
        return true; // Directory doesn't exist, nothing to move
    }

    // Create cache base directory if it doesn't exist
    if (!dir_exists(cache_base)) {
        if (!create_directory_recursive(cache_base)) {
            (void)fprintf(stderr, "Warning: Could not create cache directory: %s\n", cache_base);
            return false;
        }
    }

    // Get timestamp
    char timestamp[32] = {};
    get_timestamp_string(timestamp, sizeof(timestamp));

    // Create cache subdirectory path: cache_base/.cache/timestamp/cache_subdir
    char cache_dir[1024] = {};
    (void)snprintf(cache_dir, sizeof(cache_dir), "%s%s.cache%s%s%s%s", cache_base, PATH_SEPARATOR, PATH_SEPARATOR, timestamp, PATH_SEPARATOR, cache_subdir);

    // Create parent directories for cache location
    char cache_parent[1024] = {};
    (void)snprintf(cache_parent, sizeof(cache_parent), "%s%s.cache%s%s", cache_base, PATH_SEPARATOR, PATH_SEPARATOR, timestamp);
    if (!dir_exists(cache_parent)) {
        if (!create_directory_recursive(cache_parent)) {
            (void)fprintf(stderr, "Warning: Could not create cache timestamp directory: %s\n", cache_parent);
            return false;
        }
    }

    // Copy directory to cache location
    if (!copy_directory_recursive(src_dir, cache_dir, false)) {
        (void)fprintf(stderr, "Warning: Could not copy directory to cache: %s -> %s\n", src_dir, cache_dir);
        return false;
    }

    if (verbose) { printf("Moved existing directory to cache: %s -> %s\n", src_dir, cache_dir); }

    // Remove original directory
    // On Windows, we need to use a different approach
#ifdef _WIN32
    // Use system command to remove directory recursively
    char rm_cmd[2048] = {};
    (void)snprintf(rm_cmd, sizeof(rm_cmd), "rmdir /s /q \"%s\"", src_dir);
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(rm_cmd) != 0) {
        (void)fprintf(stderr, "Warning: Could not remove original directory: %s\n", src_dir);
        return false;
    }
#else
    // Use system command to remove directory recursively
    char rm_cmd[2048] = {};
    (void)snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf \"%s\"", src_dir);
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(rm_cmd) != 0) {
        (void)fprintf(stderr, "Warning: Could not remove original directory: %s\n", src_dir);
        return false;
    }
#endif

    return true;
}

// Recursively remove a directory
static bool remove_directory_recursive(const char* dir_path) {
    if (!dir_exists(dir_path)) {
        return true; // Directory doesn't exist, nothing to remove
    }

#ifdef _WIN32
    char rm_cmd[2048] = {};
    (void)snprintf(rm_cmd, sizeof(rm_cmd), "rmdir /s /q \"%s\"", dir_path);
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    return system(rm_cmd) == 0;
#else
    char rm_cmd[2048] = {};
    (void)snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf \"%s\"", dir_path);
    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    return system(rm_cmd) == 0;
#endif
}

// Clean .cache directories in a given base directory
static void clean_cache_directories(const char* base_dir, bool verbose) {
    char cache_dir[1024] = {};
    (void)snprintf(cache_dir, sizeof(cache_dir), "%s%s.cache", base_dir, PATH_SEPARATOR);

    if (dir_exists(cache_dir)) {
        if (verbose) { printf("Cleaning cache directory: %s\n", cache_dir); }
        if (remove_directory_recursive(cache_dir)) {
            if (verbose) { printf("Removed cache directory: %s\n", cache_dir); }
        } else {
            (void)fprintf(stderr, "Warning: Failed to remove cache directory: %s\n", cache_dir);
        }
    }
}

// Clean a single library's build and deps directories
static void clean_single_library(BuildConfig* config, const LibraryConfig* lib_config, bool clean_build, bool clean_lib) {
    if (config->verbose) { printf("Cleaning library: %s\n", lib_config->library_name); }

    if (clean_build) {
        char library_build_path[1024] = {};
        (void)snprintf(library_build_path, sizeof(library_build_path), "%s%sbuild", lib_config->library_path, PATH_SEPARATOR);

        // Determine effective profile
        const char* lib_effective_profile = lib_config->profile;
        if (strcmp(lib_config->profile, "default") == 0) { lib_effective_profile = config->build_config_name; }

        char library_profile_path[1024] = {};
        (void)snprintf(library_profile_path, sizeof(library_profile_path), "%s%s%s", library_build_path, PATH_SEPARATOR, lib_effective_profile);

        if (!config->clean_cache_only) {
            // Move to cache first
            (void)move_directory_to_cache(library_profile_path, library_build_path, lib_effective_profile, config->verbose);
        }

        // Clean cache
        clean_cache_directories(library_build_path, config->verbose);

        // Remove empty build directory if it exists and is empty
        if (dir_exists(library_build_path)) {
            DIR* dir = opendir(library_build_path);
            if (dir) {
                bool is_empty = true;
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        is_empty = false;
                        break;
                    }
                }
                closedir(dir);

                if (is_empty) {
                    if (remove_directory_recursive(library_build_path)) {
                        if (config->verbose) { printf("Removed empty build directory: %s\n", library_build_path); }
                    }
                }
            }
        }
    }

    if (clean_lib) {
        char library_lib_path[1024] = {};
        (void)snprintf(library_lib_path, sizeof(library_lib_path), "%s%slib", lib_config->library_path, PATH_SEPARATOR);
        char library_deps_path[1024] = {};
        (void)snprintf(library_deps_path, sizeof(library_deps_path), "%s%sdeps", library_lib_path, PATH_SEPARATOR);

        if (!config->clean_cache_only) {
            // Move to cache first
            (void)move_directory_to_cache(library_deps_path, library_lib_path, "deps", config->verbose);
        }

        // Clean cache
        clean_cache_directories(library_lib_path, config->verbose);

        // Remove empty lib directory if it exists and is empty
        if (dir_exists(library_lib_path)) {
            DIR* dir = opendir(library_lib_path);
            if (dir) {
                bool is_empty = true;
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        is_empty = false;
                        break;
                    }
                }
                closedir(dir);

                if (is_empty) {
                    if (remove_directory_recursive(library_lib_path)) {
                        if (config->verbose) { printf("Removed empty lib directory: %s\n", library_lib_path); }
                    }
                }
            }
        }
    }

    // Recursively clean dependencies if requested
    if (config->clean_recur) {
        char library_lib_dh_path[1024] = {};
        (void)snprintf(library_lib_dh_path, sizeof(library_lib_dh_path), "%s%slib.dh", lib_config->library_path, PATH_SEPARATOR);

        if (file_exists(library_lib_dh_path)) {
            // Save current lib_configs
            LibraryConfigArray saved_lib_configs = config->lib_configs;
            config->lib_configs.libraries = NULL;
            config->lib_configs.count = 0;

            // Parse the library's lib.dh
            if (parse_lib_dh_file(config, library_lib_dh_path)) {
                // Recursively clean each dependency
                for (int i = 0; i < config->lib_configs.count; ++i) { clean_single_library(config, &config->lib_configs.libraries[i], clean_build, clean_lib); }
            }

            // Restore lib_configs
            free_library_configs(&config->lib_configs);
            config->lib_configs = saved_lib_configs;
        }
    }
}

// Clean project build and/or lib directories
void clean_project(BuildConfig* config, bool clean_build, bool clean_lib) {
    if (config->verbose) {
        printf("Cleaning project: %s\n", config->project_name);
        if (clean_build) { printf("  - Cleaning build directories\n"); }
        if (clean_lib) { printf("  - Cleaning lib/deps directories\n"); }
        if (config->clean_cache_only) { printf("  - Only cleaning .cache directories\n"); }
        if (config->clean_recur) { printf("  - Recursively cleaning dependencies\n"); }
    }

    if (clean_build) {
        char build_output_dir[1024] = {};
        (void)snprintf(build_output_dir, sizeof(build_output_dir), "%s%sbuild", config->project_root, PATH_SEPARATOR);

        // Get profile from build config or use "dev" as default
        const char* profile = config->build_config_name;
        if (strlen(profile) == 0) { profile = "dev"; }

        char profile_output_dir[1024] = {};
        (void)snprintf(profile_output_dir, sizeof(profile_output_dir), "%s%s%s", build_output_dir, PATH_SEPARATOR, profile);

        if (!config->clean_cache_only) {
            // Move existing build/profile to cache
            (void)move_directory_to_cache(profile_output_dir, build_output_dir, profile, config->verbose);
        }

        // Clean cache
        clean_cache_directories(build_output_dir, config->verbose);

        // Remove empty build directory if it exists and is empty
        if (dir_exists(build_output_dir)) {
            DIR* dir = opendir(build_output_dir);
            if (dir) {
                bool is_empty = true;
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        is_empty = false;
                        break;
                    }
                }
                closedir(dir);

                if (is_empty) {
                    if (remove_directory_recursive(build_output_dir)) {
                        if (config->verbose) { printf("Removed empty build directory: %s\n", build_output_dir); }
                    }
                }
            }
        }
    }

    if (clean_lib) {
        char project_lib_path[1024] = {};
        (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);
        char deps_path[1024] = {};
        (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

        if (!config->clean_cache_only) {
            // Move existing lib/deps to cache
            (void)move_directory_to_cache(deps_path, project_lib_path, "deps", config->verbose);
        }

        // Clean cache
        clean_cache_directories(project_lib_path, config->verbose);

        // Remove empty lib directory if it exists and is empty
        if (dir_exists(project_lib_path)) {
            DIR* dir = opendir(project_lib_path);
            if (dir) {
                bool is_empty = true;
                struct dirent* entry = NULL;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        is_empty = false;
                        break;
                    }
                }
                closedir(dir);

                if (is_empty) {
                    if (remove_directory_recursive(project_lib_path)) {
                        if (config->verbose) { printf("Removed empty lib directory: %s\n", project_lib_path); }
                    }
                }
            }
        }
    }

    // Recursively clean dependencies if requested
    if (config->clean_recur && config->lib_configs.count > 0) {
        if (config->verbose) {
            printf("Recursively cleaning dependencies:\n");
            for (int i = 0; i < config->lib_configs.count; ++i) { printf("  - %s\n", config->lib_configs.libraries[i].library_name); }
        }

        for (int i = 0; i < config->lib_configs.count; ++i) { clean_single_library(config, &config->lib_configs.libraries[i], clean_build, clean_lib); }
    }
}

// Copy library includes to project lib directory
bool copy_library_includes(const char* lib_src_path, const char* project_lib_path, const char* lib_name, bool verbose) {
    char lib_include_src[1024];
    char lib_include_dest[1024];
    char lib_dir_part[256];
    char lib_name_part[256];

    (void)snprintf(lib_include_src, sizeof(lib_include_src), "%s%sinclude", lib_src_path, PATH_SEPARATOR);

    // Extract directory and name parts from library name
    extract_library_dir_and_name(lib_name, lib_dir_part, lib_name_part);

    if (strlen(lib_dir_part) > 0) {
        // Copy contents of include/ to project/lib/deps/dir_part/ (not including name_part)
        // This way, when we use -I with this path, headers are at the root
        // The library name is not needed in the path since the contents are already the library's contents
        (void)snprintf(lib_include_dest, sizeof(lib_include_dest), "%s%sdeps%s%s", project_lib_path, PATH_SEPARATOR, PATH_SEPARATOR, lib_dir_part);
    } else {
        // For libraries without dir_part, copy directly to deps/ (not deps/name_part/)
        // This gives us deps/foo.h instead of deps/foo/foo.h
        (void)snprintf(lib_include_dest, sizeof(lib_include_dest), "%s%sdeps", project_lib_path, PATH_SEPARATOR);
    }

    if (!dir_exists(lib_include_src)) {
        if (verbose) {
            printf("No include directory found for library %s at %s\n", lib_name, lib_include_src);
        }
        return true; // Not an error if no includes
    }

    // Create the target directory structure recursively
    if (!create_directory_recursive(lib_include_dest)) {
        (void)fprintf(stderr, "Could not create include directory: %s\n", lib_include_dest);
        return false;
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

    // Check if this library has its own lib.dh file and compile its dependencies first
    char library_lib_dh_path[1024] = {};
    (void)snprintf(library_lib_dh_path, sizeof(library_lib_dh_path), "%s%slib.dh", lib_config->library_path, PATH_SEPARATOR);

    if (file_exists(library_lib_dh_path)) {
        if (config->verbose) { printf("Library %s has its own lib.dh, compiling dependencies first...\n", lib_config->library_name); }

        // Save current lib_configs
        LibraryConfigArray saved_lib_configs = config->lib_configs;
        config->lib_configs.libraries = NULL;
        config->lib_configs.count = 0;

        // Parse the library's lib.dh
        if (parse_lib_dh_file(config, library_lib_dh_path)) {
            // Print dependency tree
            if (config->lib_configs.count > 0) {
                printf("Compiling dependencies '%s':\n", lib_config->library_name);
                for (int i = 0; i < config->lib_configs.count; ++i) { printf("- %s\n", config->lib_configs.libraries[i].library_name); }
            }
            // Recursively compile dependencies from the library's lib.dh
            if (config->lib_configs.count > 0) {
                if (!compile_libraries(config)) {
                    (void)fprintf(stderr, "Failed to compile dependencies for library: %s\n", lib_config->library_name);
                    // Restore original lib_configs before returning
                    free_library_configs(&config->lib_configs);
                    config->lib_configs = saved_lib_configs;
                    return false;
                }

                // After compiling dependencies, copy them to the library's own lib/deps/ directory
                // This makes the library self-contained with all its dependencies
                char library_lib_path[1024] = {};
                (void)snprintf(library_lib_path, sizeof(library_lib_path), "%s%slib", lib_config->library_path, PATH_SEPARATOR);

                char library_deps_path[1024] = {};
                (void)snprintf(library_deps_path, sizeof(library_deps_path), "%s%sdeps", library_lib_path, PATH_SEPARATOR);

                // Create library's lib/deps/ directory if it doesn't exist
                if (!dir_exists(library_lib_path)) {
                    if (!create_directory_recursive(library_lib_path)) {
                        if (config->verbose) { printf("Warning: Could not create library lib directory: %s\n", library_lib_path); }
                    }
                }

                // Move existing library lib/deps directory to cache if it exists
                (void)move_directory_to_cache(library_deps_path, library_lib_path, "deps", config->verbose);

                if (!dir_exists(library_deps_path)) {
                    if (!create_directory_recursive(library_deps_path)) {
                        if (config->verbose) { printf("Warning: Could not create library deps directory: %s\n", library_deps_path); }
                    }
                }

                // Print "Bring dependencies" message
                printf("Bring dependencies '%s':\n", lib_config->library_name);

                // Copy each dependency from its own lib/ and lib/deps/ to library's lib/deps/
                for (int i = 0; i < config->lib_configs.count; ++i) {
                    const LibraryConfig* dep_lib = &config->lib_configs.libraries[i];
                    char dep_lib_dir_part[256] = {};
                    char dep_lib_name_part[256] = {};
                    extract_library_dir_and_name(dep_lib->library_name, dep_lib_dir_part, dep_lib_name_part);

                    // Source: dependency library's own build/profile/ directory
                    char dep_lib_build_path[1024] = {};
                    (void)snprintf(dep_lib_build_path, sizeof(dep_lib_build_path), "%s%sbuild", dep_lib->library_path, PATH_SEPARATOR);

                    // Determine effective profile for this dependency
                    const char* dep_effective_profile = dep_lib->profile;
                    if (strcmp(dep_lib->profile, "default") == 0) { dep_effective_profile = config->build_config_name; }

                    char dep_lib_profile_path[1024] = {};
                    (void)snprintf(dep_lib_profile_path, sizeof(dep_lib_profile_path), "%s%s%s", dep_lib_build_path, PATH_SEPARATOR, dep_effective_profile);

                    // Destination: library's lib/deps/dir_part/ (not including lib_name_part as a directory)
                    // Library binary goes to lib/deps/dir_part/lib_name_part.lib
                    char dest_dep_dir[1024] = {};
                    if (strlen(dep_lib_dir_part) > 0) {
                        (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s%s%s", library_deps_path, PATH_SEPARATOR, dep_lib_dir_part);
                    } else {
                        (void)snprintf(dest_dep_dir, sizeof(dest_dep_dir), "%s", library_deps_path);
                    }

                    // Copy library binary from dependency's build/profile/ to library's lib/deps/dir_part/lib_name_part.lib
                    char src_lib_file[1024] = {};
                    char dest_lib_file[1024] = {};
#ifdef _WIN32
                    (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%s%s.lib", dep_lib_profile_path, PATH_SEPARATOR, dep_lib_name_part);
                    (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%s%s.lib", dest_dep_dir, PATH_SEPARATOR, dep_lib_name_part);
#else
                    (void)snprintf(src_lib_file, sizeof(src_lib_file), "%s%slib%s.a", dep_lib_profile_path, PATH_SEPARATOR, dep_lib_name_part);
                    (void)snprintf(dest_lib_file, sizeof(dest_lib_file), "%s%slib%s.a", dest_dep_dir, PATH_SEPARATOR, dep_lib_name_part);
#endif

                    if (file_exists(src_lib_file)) {
                        // Create destination directory if needed
                        if (!dir_exists(dest_dep_dir)) {
                            if (!create_directory_recursive(dest_dep_dir)) {
                                (void)fprintf(stderr, "Failed to create destination directory: %s\n", dest_dep_dir);
                                free_library_configs(&config->lib_configs);
                                config->lib_configs = saved_lib_configs;
                                return false;
                            }
                        }

                        // Copy the library file
                        FILE* src_fp = fopen(src_lib_file, "rb");
                        if (src_fp) {
                            FILE* dest_fp = fopen(dest_lib_file, "wb");
                            if (dest_fp) {
                                char buffer[4096] = {};
                                size_t bytes = 0;
                                while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
                                    if (fwrite(buffer, 1, bytes, dest_fp) != bytes) {
                                        (void)fprintf(stderr, "Failed to write to %s\n", dest_lib_file);
                                        (void)fclose(dest_fp);
                                        (void)fclose(src_fp);
                                        free_library_configs(&config->lib_configs);
                                        config->lib_configs = saved_lib_configs;
                                        return false;
                                    }
                                }
                                (void)fclose(dest_fp);
                            }
                            (void)fclose(src_fp);
                        }
                    }

                    // Copy dependency's lib/deps/ (transitive dependencies) to library's lib/deps/
                    char dep_lib_lib_path_for_deps[1024] = {};
                    (void)snprintf(dep_lib_lib_path_for_deps, sizeof(dep_lib_lib_path_for_deps), "%s%slib", dep_lib->library_path, PATH_SEPARATOR);
                    char dep_lib_deps_path[1024] = {};
                    (void)snprintf(dep_lib_deps_path, sizeof(dep_lib_deps_path), "%s%sdeps", dep_lib_lib_path_for_deps, PATH_SEPARATOR);

                    // Print what's being copied
                    char dest_path_str[1024] = {};
                    if (strlen(dep_lib_dir_part) > 0) {
                        (void)snprintf(
                            dest_path_str, sizeof(dest_path_str), "%s%slib%sdeps%s%s%s%s", lib_config->library_name, PATH_SEPARATOR, PATH_SEPARATOR,
                            PATH_SEPARATOR, dep_lib_dir_part, PATH_SEPARATOR, dep_lib_name_part
                        );
                    } else {
                        (void)snprintf(
                            dest_path_str, sizeof(dest_path_str), "%s%slib%sdeps%s%s", lib_config->library_name, PATH_SEPARATOR, PATH_SEPARATOR, PATH_SEPARATOR,
                            dep_lib_name_part
                        );
                    }
                    printf("- '%s' <- '%s'\n", dest_path_str, dep_lib->library_name);

                    if (dir_exists(dep_lib_deps_path)) {
                        // Copy entire deps directory recursively (transitive dependencies)
                        (void)copy_directory_recursive(dep_lib_deps_path, dest_dep_dir, false);
                    }

                    // Copy dependency's includes directly from the dependency's include/ directory
                    // Use the same structure: for libraries with dir_part, copy to dest_dep_dir (which is library_deps_path/dir_part/)
                    // For libraries without dir_part, copy directly to library_deps_path (not library_deps_path/name_part/)
                    char dep_include_src[1024] = {};
                    (void)snprintf(dep_include_src, sizeof(dep_include_src), "%s%sinclude", dep_lib->library_path, PATH_SEPARATOR);

                    if (dir_exists(dep_include_src)) {
                        // Determine destination: same logic as copy_library_includes
                        char dep_include_dest[1024] = {};
                        if (strlen(dep_lib_dir_part) > 0) {
                            // Copy to library_deps_path/dir_part/
                            (void)snprintf(dep_include_dest, sizeof(dep_include_dest), "%s%s%s", library_deps_path, PATH_SEPARATOR, dep_lib_dir_part);
                        } else {
                            // Copy directly to library_deps_path/ (not library_deps_path/name_part/)
                            (void)snprintf(dep_include_dest, sizeof(dep_include_dest), "%s", library_deps_path);
                        }

                        if (!dir_exists(dep_include_dest)) {
                            if (!create_directory_recursive(dep_include_dest)) {
                                (void)fprintf(stderr, "Failed to create destination directory: %s\n", dep_include_dest);
                                free_library_configs(&config->lib_configs);
                                config->lib_configs = saved_lib_configs;
                                return false;
                            }
                        }
                        // Copy contents of the include directory
                        (void)copy_directory_recursive(dep_include_src, dep_include_dest, false);
                    }
                }
            }
        }

        // Restore original lib_configs
        free_library_configs(&config->lib_configs);
        config->lib_configs = saved_lib_configs;
    }

    // Find source files in the library
    char lib_src_path[1024];
    (void)snprintf(lib_src_path, sizeof(lib_src_path), "%s%ssrc", lib_config->library_path, PATH_SEPARATOR);

    if (!dir_exists(lib_src_path)) {
        (void)fprintf(stderr, "Library source directory not found: %s\n", lib_src_path);
        return false;
    }

    char** sources = NULL;
    int source_count = 0;
    find_source_files(lib_src_path, &sources, &source_count);

    if (source_count == 0) {
        (void)fprintf(stderr, "No source files found for library: %s\n", lib_config->library_name);
        return false;
    }

    // Create library's own build/profile directory (not project's lib/deps/)
    char library_build_path[1024];
    (void)snprintf(library_build_path, sizeof(library_build_path), "%s%sbuild", lib_config->library_path, PATH_SEPARATOR);

    // Determine effective profile for this library
    const char* lib_effective_profile = lib_config->profile;
    if (strcmp(lib_config->profile, "default") == 0) { lib_effective_profile = config->build_config_name; }

    char library_profile_path[1024];
    (void)snprintf(library_profile_path, sizeof(library_profile_path), "%s%s%s", library_build_path, PATH_SEPARATOR, lib_effective_profile);

    // Move existing library build/profile directory to cache if it exists
    (void)move_directory_to_cache(library_profile_path, library_build_path, lib_effective_profile, config->verbose);

    if (!dir_exists(library_profile_path)) {
        if (!create_directory_recursive(library_profile_path)) {
            (void)fprintf(stderr, "Could not create library build directory: %s\n", library_profile_path);
            free_string_array(sources, source_count);
            return false;
        }
    }

    // Keep reference to library's lib/deps/ path (may or may not exist depending on dependencies)
    char library_lib_path[1024];
    (void)snprintf(library_lib_path, sizeof(library_lib_path), "%s%slib", lib_config->library_path, PATH_SEPARATOR);
    char library_deps_path[1024];
    (void)snprintf(library_deps_path, sizeof(library_deps_path), "%s%sdeps", library_lib_path, PATH_SEPARATOR);

    // Also keep reference to project's lib/deps/ for copying dependencies later
    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);
    char deps_path[1024];
    (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

    // Extract directory and name parts from library name
    char lib_dir_part[256];
    char lib_name_part[256];
    extract_library_dir_and_name(lib_config->library_name, lib_dir_part, lib_name_part);

    // Prepare compilation command
    char command[8192] = {};
    strcat(command, config->compiler);
    strcat(command, " -std=");
    strcat(command, config->c_standard);

    // Add include paths: src, include, and lib/deps
    if (dir_exists(lib_src_path)) {
        strcat(command, " -I");
        strcat(command, lib_src_path);
    }

    char lib_include_path[1024];
    (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%sinclude", lib_config->library_path, PATH_SEPARATOR);
    if (dir_exists(lib_include_path)) {
        strcat(command, " -I");
        strcat(command, lib_include_path);
    }

    // Add lib/deps as an include path
    if (dir_exists(library_deps_path)) {
        strcat(command, " -I");
        strcat(command, library_deps_path);
    }

    // Add include paths from dependencies (if library has its own lib.dh)
    // Add -I for each dependency directory so headers can be found
    if (file_exists(library_lib_dh_path)) {
        // We need to get the dependencies from the library's lib.dh
        // But we've already restored the lib_configs, so we need to parse it again temporarily
        LibraryConfigArray saved_lib_configs_temp = config->lib_configs;
        config->lib_configs.libraries = NULL;
        config->lib_configs.count = 0;

        if (parse_lib_dh_file(config, library_lib_dh_path)) {
            // Add include path for each dependency directory
            for (int i = 0; i < config->lib_configs.count; ++i) {
                const LibraryConfig* dep_lib = &config->lib_configs.libraries[i];
                char dep_lib_dir_part[256] = {};
                char dep_lib_name_part[256] = {};
                extract_library_dir_and_name(dep_lib->library_name, dep_lib_dir_part, dep_lib_name_part);

                // Build the dependency directory path in library's lib/deps/
                char dep_include_path[1024] = {};
                if (strlen(dep_lib_dir_part) > 0) {
                    (void)snprintf(
                        dep_include_path, sizeof(dep_include_path), "%s%s%s%s%s", library_deps_path, PATH_SEPARATOR, dep_lib_dir_part, PATH_SEPARATOR,
                        dep_lib_name_part
                    );
                } else {
                    (void)snprintf(dep_include_path, sizeof(dep_include_path), "%s%s%s", library_deps_path, PATH_SEPARATOR, dep_lib_name_part);
                }

                if (dir_exists(dep_include_path)) {
                    strcat(command, " -I");
                    strcat(command, dep_include_path);
                }
            }
        }

        // Restore lib_configs
        free_library_configs(&config->lib_configs);
        config->lib_configs = saved_lib_configs_temp;
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
#ifdef _WIN32
        strcat(command, " -fms-extensions");
#endif /* _WIN32 */

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

    // Use the effective profile we determined earlier (lib_effective_profile)
    if (config->verbose && strcmp(lib_config->profile, "default") == 0) {
        printf("Library profile is 'default', using main project profile: %s\n", lib_effective_profile);
    }

    for (int i = 0; i < NUM_BUILD_PRESETS; ++i) {
        if (strcmp(build_presets[i].name, lib_effective_profile) == 0) {
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

        // Add extra flags from the profile, but skip -flto for static libraries
        // (LTO object files can't be properly archived with standard ar tool)
        for (int i = 0; i < profile_preset->extra_flags.count; ++i) {
            const char* flag = profile_preset->extra_flags.flags[i];

            // Skip -flto for static libraries
            if (strcmp(lib_config->linking_type, "static") == 0 && strcmp(flag, "-flto") == 0) {
                if (config->verbose) {
                    printf("Skipping -flto flag for static library (not compatible with ar)\n");
                }
                continue;
            }

            strcat(command, " ");
            strcat(command, flag);
        }

        if (config->verbose) { printf("Applied profile '%s' settings to library compilation\n", lib_effective_profile); }
    } else if (config->verbose) {
        printf("Warning: Unknown profile '%s', using default settings\n", lib_effective_profile);
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

    // Set output file to library's own build/profile/ directory
    char output_file[1024];

    if (strcmp(lib_config->linking_type, "static") == 0) {
#ifdef _WIN32
        (void)snprintf(output_file, sizeof(output_file), "%s%s%s.lib", library_profile_path, PATH_SEPARATOR, lib_name_part);
#else
        (void)snprintf(output_file, sizeof(output_file), "%s%slib%s.a", library_profile_path, PATH_SEPARATOR, lib_name_part);
#endif
    } else {
#ifdef _WIN32
        (void)snprintf(output_file, sizeof(output_file), "%s%s%s.dll", library_profile_path, PATH_SEPARATOR, lib_name_part);
#else
        (void)snprintf(output_file, sizeof(output_file), "%s%slib%s.so", library_profile_path, PATH_SEPARATOR, lib_name_part);
#endif
    }

    if (strcmp(lib_config->linking_type, "static") == 0) {
        // For static libraries, compile to object files first, then create archive
        char obj_dir[1024];
        (void)snprintf(obj_dir, sizeof(obj_dir), "%s%s.obj", library_profile_path, PATH_SEPARATOR);

        if (!dir_exists(obj_dir)) {
            if (!create_directory_recursive(obj_dir)) {
                (void)fprintf(stderr, "Could not create object directory: %s\n", obj_dir);
                free_string_array(sources, source_count);
                return false;
            }
        }

        // Compile each source to object file
        char obj_files[16384] = {}; // Increased buffer size to prevent overflow
        for (int i = 0; i < source_count; ++i) {
            char obj_file[1024];
            char* filename = strrchr(sources[i], PATH_SEPARATOR[0]);
            if (!filename) {
                filename = sources[i];
            } else {
                filename++; // Skip separator
            }

            char basename[256];
            strncpy(basename, filename, sizeof(basename) - 1);
            basename[sizeof(basename) - 1] = '\0'; // Ensure null termination
            char* dot = strrchr(basename, '.');
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
            size_t current_len = strlen(obj_files);
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

    for (int i = 0; i < config->lib_configs.count; ++i) {
        if (!compile_single_library(config, &config->lib_configs.libraries[i])) {
            (void)fprintf(stderr, "Failed to compile library: %s\n", config->lib_configs.libraries[i].library_name);
            return false;
        }
    }

    return true;
}

// Add library includes to the include path list
void add_lib_dh_includes(BuildConfig* config, char*** includes, int* include_count) {
    if (config->lib_configs.count == 0) {
        return;
    }

    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    // Add the common deps directory for all libraries
    char deps_include_path[1024];
    (void)snprintf(deps_include_path, sizeof(deps_include_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

    if (dir_exists(deps_include_path)) {
        *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
        if (*includes == NULL) {
            (void)fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        (*includes)[*include_count] = strdup(deps_include_path);
        (*include_count)++;

        if (config->verbose) {
            printf("Added library include path: %s\n", deps_include_path);
        }
    }
}

// Recursively find all library files in a directory
static void find_library_files(const char* dir_path, char*** lib_files, int* lib_count) { /* NOLINT(misc-no-recursion) */
    DIR* dir = opendir(dir_path);
    if (!dir) { return; }

    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Skip hidden files and . / ..
        }

        char path[1024] = {};
        int result = snprintf(path, sizeof(path), "%s%s%s", dir_path, PATH_SEPARATOR, entry->d_name);
        if (result < 0 || (size_t)result >= sizeof(path)) {
            continue; // Path too long, skip
        }

        struct stat st = {};
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recurse into subdirectory
                find_library_files(path, lib_files, lib_count);
            } else if (S_ISREG(st.st_mode)) {
                // Check if it's a library file
                const char* ext = strrchr(entry->d_name, '.');
                if (ext && (strcmp(ext, ".lib") == 0 || strcmp(ext, ".a") == 0)) {
                    // Add to lib_files array
                    *lib_files = realloc(*lib_files, (*lib_count + 1) * sizeof(char*));
                    if (*lib_files == NULL) {
                        (void)fprintf(stderr, "Memory allocation failed\n");
                        closedir(dir);
                        exit(1);
                    }
                    (*lib_files)[*lib_count] = strdup(path);
                    if ((*lib_files)[*lib_count] == NULL) {
                        (void)fprintf(stderr, "Memory allocation failed\n");
                        closedir(dir);
                        exit(1);
                    }
                    (*lib_count)++;
                }
            }
        }
    }
    closedir(dir);
}

// Add library linking flags to the configuration
void add_lib_dh_libraries(BuildConfig* config) {
    if (config->lib_configs.count == 0) {
        return;
    }

    char project_lib_path[1024];
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    char deps_path[1024];
    (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

    // Add library search path
    if (config->linked_libraries[0] != '\0') {
        strcat(config->linked_libraries, " ");
    }
    strcat(config->linked_libraries, "-L");
    strcat(config->linked_libraries, deps_path);

    // Recursively find all library files in lib/deps/ (includes transitive dependencies)
    char** lib_files = NULL;
    int lib_count = 0;
    if (dir_exists(deps_path)) { find_library_files(deps_path, &lib_files, &lib_count); }

    // Add all found library files to the linking command
    for (int i = 0; i < lib_count; ++i) {
        if (config->linked_libraries[0] != '\0') { strcat(config->linked_libraries, " "); }
        strcat(config->linked_libraries, "\"");
        strcat(config->linked_libraries, lib_files[i]);
        strcat(config->linked_libraries, "\"");

        if (config->verbose) { printf("Added library link: %s\n", lib_files[i]); }
    }

    // Free the lib_files array
    if (lib_files != NULL) {
        for (int i = 0; i < lib_count; ++i) { free(lib_files[i]); }
        free(lib_files);
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


// Compile a library directory's src/ as a library (for sample files)
bool compile_library_directory_as_library(BuildConfig* config, const char* library_path, const char* library_name) {
    if (config->verbose) { printf("Compiling library directory as library: %s at %s\n", library_name, library_path); }

    // Check if library has src/ directory
    char lib_src_path[1024] = {};
    (void)snprintf(lib_src_path, sizeof(lib_src_path), "%s%ssrc", library_path, PATH_SEPARATOR);

    if (!dir_exists(lib_src_path)) {
        if (config->verbose) { printf("Library %s has no src/ directory, skipping\n", library_name); }
        return true; // Not an error if no src directory
    }

    // Find source files in the library's src directory
    char** sources = NULL;
    int source_count = 0;
    find_source_files(lib_src_path, &sources, &source_count);

    if (source_count == 0) {
        if (config->verbose) { printf("No source files found in library %s\n", library_name); }
        return true; // Not an error if no sources
    }

    // Create output directory in library's build/profile/ (same as compile_single_library)
    char library_build_path[1024] = {};
    (void)snprintf(library_build_path, sizeof(library_build_path), "%s%sbuild", library_path, PATH_SEPARATOR);

    char library_profile_path[1024] = {};
    (void)snprintf(library_profile_path, sizeof(library_profile_path), "%s%s%s", library_build_path, PATH_SEPARATOR, config->build_config_name);

    // Move existing library build/profile directory to cache if it exists
    (void)move_directory_to_cache(library_profile_path, library_build_path, config->build_config_name, config->verbose);

    if (!dir_exists(library_profile_path)) {
        if (!create_directory_recursive(library_profile_path)) {
            (void)fprintf(stderr, "Could not create library build directory: %s\n", library_profile_path);
            free_string_array(sources, source_count);
            return false;
        }
    }

    // Prepare base compilation command
    char base_command[8192] = {};
    strcat(base_command, config->compiler);
    strcat(base_command, " -std=");
    strcat(base_command, config->c_standard);

    // Add include paths: src, include, and lib/deps
    if (dir_exists(lib_src_path)) {
        strcat(base_command, " -I");
        strcat(base_command, lib_src_path);
    }

    char lib_include_path[1024] = {};
    (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%sinclude", library_path, PATH_SEPARATOR);
    if (dir_exists(lib_include_path)) {
        strcat(base_command, " -I");
        strcat(base_command, lib_include_path);
    }

    // Add lib/deps as an include path
    char library_lib_path[1024] = {};
    (void)snprintf(library_lib_path, sizeof(library_lib_path), "%s%slib", library_path, PATH_SEPARATOR);
    char library_deps_path[1024] = {};
    (void)snprintf(library_deps_path, sizeof(library_deps_path), "%s%sdeps", library_lib_path, PATH_SEPARATOR);
    if (dir_exists(library_deps_path)) {
        strcat(base_command, " -I");
        strcat(base_command, library_deps_path);
    }

    // Add DH includes if not disabled
    if (!config->no_libdh && strlen(config->dh_path) > 0) {
        char dh_include[1024] = {};
        (void)snprintf(dh_include, sizeof(dh_include), "%s%sinclude", config->dh_path, PATH_SEPARATOR);
        strcat(base_command, " -I");
        strcat(base_command, dh_include);
        strcat(base_command, " -I");
        strcat(base_command, config->dh_path);
        strcat(base_command, " -funsigned-char -fblocks -DBlocksRuntime_STATIC");
#ifdef _WIN32
        strcat(base_command, " -fms-extensions");
#endif /* _WIN32 */

        char blocks_include_path[1024] = {};
        (void)snprintf(
            blocks_include_path, sizeof(blocks_include_path), "%s%slibs%sBlocksRuntime%sinclude", config->dh_path, PATH_SEPARATOR, PATH_SEPARATOR,
            PATH_SEPARATOR
        );
        if (dir_exists(blocks_include_path)) {
            strcat(base_command, " -I");
            strcat(base_command, blocks_include_path);
        }

        strcat(base_command, " -DCOMP");
        if (config->is_test) { strcat(base_command, " -DCOMP_TEST"); }
    }

    // Apply build profile settings
    BuildConfigPreset* profile_preset = NULL;
    for (int i = 0; i < NUM_BUILD_PRESETS; ++i) {
        if (strcmp(build_presets[i].name, config->build_config_name) == 0) {
            profile_preset = &build_presets[i];
            break;
        }
    }

    if (profile_preset) {
        const char* opt_flag = optimization_level_to_flag(profile_preset->optimization_level);
        if (strlen(opt_flag) > 0) {
            strcat(base_command, " ");
            strcat(base_command, opt_flag);
        }

        const char* debug_flag = debug_level_to_flag(profile_preset->debug_level);
        if (strlen(debug_flag) > 0) {
            strcat(base_command, " ");
            strcat(base_command, debug_flag);
        }

        if (!profile_preset->assertions_enabled) { strcat(base_command, " -DNDEBUG"); }

        // Add extra flags (skip -flto for static libraries)
        for (int i = 0; i < profile_preset->extra_flags.count; ++i) {
            const char* flag = profile_preset->extra_flags.flags[i];
            if (strcmp(flag, "-flto") == 0) {
                continue; // Skip LTO for static libraries
            }
            strcat(base_command, " ");
            strcat(base_command, flag);
        }
    }

    // Create object files directory in build/profile/.obj/
    char obj_dir[1024] = {};
    (void)snprintf(obj_dir, sizeof(obj_dir), "%s%s.obj", library_profile_path, PATH_SEPARATOR);

    if (!dir_exists(obj_dir)) {
        if (!create_directory(obj_dir)) {
            (void)fprintf(stderr, "Could not create object directory: %s\n", obj_dir);
            free_string_array(sources, source_count);
            return false;
        }
    }

    // Compile each source file to object file
    char obj_files[16384] = {};
    for (int i = 0; i < source_count; ++i) {
        char obj_file[1024] = {};
        char* filename = strrchr(sources[i], PATH_SEPARATOR[0]);
        if (!filename) {
            filename = sources[i];
        } else {
            filename++; // Skip separator
        }

        char basename[256] = {};
        strncpy(basename, filename, sizeof(basename) - 1);
        basename[sizeof(basename) - 1] = '\0';
        char* dot = strrchr(basename, '.');
        if (dot) { *dot = '\0'; }

        (void)snprintf(obj_file, sizeof(obj_file), "%s%s%s.o", obj_dir, PATH_SEPARATOR, basename);

        char compile_cmd[4096] = {};
        (void)snprintf(compile_cmd, sizeof(compile_cmd), "%s -c %s -o %s", base_command, sources[i], obj_file);

        if (config->show_commands) { printf("Command: %s\n", compile_cmd); }

        // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
        if (system(compile_cmd) != 0) {
            (void)fprintf(stderr, "Failed to compile %s\n", sources[i]);
            free_string_array(sources, source_count);
            return false;
        }

        // Add object file to list
        if (strlen(obj_files) > 0) { strcat(obj_files, " "); }
        strcat(obj_files, obj_file);
    }

    // Create static library in build/profile/
    char lib_file[1024] = {};
#ifdef _WIN32
    (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.lib", library_profile_path, PATH_SEPARATOR, library_name);
#else
    (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.a", library_profile_path, PATH_SEPARATOR, library_name);
#endif

    char ar_cmd[8192] = {};
    (void)snprintf(ar_cmd, sizeof(ar_cmd), "ar rcs %s %s", lib_file, obj_files);

    if (config->show_commands) { printf("Command: %s\n", ar_cmd); }

    if (config->verbose) { printf("Creating library: %s\n", lib_file); }

    // NOLINTNEXTLINE(security-insecure-system-use,cert-env33-c)
    if (system(ar_cmd) != 0) {
        (void)fprintf(stderr, "Failed to create library: %s\n", lib_file);
        free_string_array(sources, source_count);
        return false;
    }

    printf("Successfully compiled library: %s -> %s\n", library_name, lib_file);

    free_string_array(sources, source_count);
    return true;
}

// Compile a library for use with a sample file (from lib.dh dependencies)
bool compile_library_for_sample(BuildConfig* config, const char* library_name) {
    // First, ensure lib.dh is parsed
    char lib_dh_path[1024] = {};
    (void)snprintf(lib_dh_path, sizeof(lib_dh_path), "%s%slib.dh", config->project_root, PATH_SEPARATOR);

    if (file_exists(lib_dh_path) && config->lib_configs.count == 0) { parse_lib_dh_file(config, lib_dh_path); }

    // Find the library in lib_configs
    LibraryConfig* target_lib = NULL;
    for (int i = 0; i < config->lib_configs.count; ++i) {
        char lib_dir_part[256] = {};
        char lib_name_part[256] = {};
        extract_library_dir_and_name(config->lib_configs.libraries[i].library_name, lib_dir_part, lib_name_part);

        if (strcmp(lib_name_part, library_name) == 0 || strcmp(config->lib_configs.libraries[i].library_name, library_name) == 0) {
            target_lib = &config->lib_configs.libraries[i];
            break;
        }
    }

    if (!target_lib) {
        (void)fprintf(stderr, "Error: Library '%s' not found in lib.dh\n", library_name);
        return false;
    }

    // Compile the library
    return compile_single_library(config, target_lib);
}

// Add library include paths for a sample file
void add_library_includes_for_sample(BuildConfig* config, const char* library_name, char*** includes, int* include_count) {
    // First, try to use the sample_library_path if it matches
    if (strlen(config->sample_library_path) > 0 && strcmp(config->sample_library, library_name) == 0) {
        // Add the library's include directory directly
        char lib_include_path[1024] = {};
        (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%sinclude", config->sample_library_path, PATH_SEPARATOR);

        if (dir_exists(lib_include_path)) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            if (*includes == NULL) {
                (void)fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            (*includes)[*include_count] = strdup(lib_include_path);
            (*include_count)++;

            if (config->verbose) { printf("Added library include path: %s\n", lib_include_path); }
        }
        return;
    }

    // Otherwise, find the library in lib_configs to get its path
    LibraryConfig* target_lib = NULL;
    for (int i = 0; i < config->lib_configs.count; ++i) {
        char lib_dir_part[256] = {};
        char lib_name_part[256] = {};
        extract_library_dir_and_name(config->lib_configs.libraries[i].library_name, lib_dir_part, lib_name_part);

        if (strcmp(lib_name_part, library_name) == 0 || strcmp(config->lib_configs.libraries[i].library_name, library_name) == 0) {
            target_lib = &config->lib_configs.libraries[i];
            break;
        }
    }

    if (!target_lib) { return; }

    // Add the library's original include directory (from source location)
    char lib_include_path[1024] = {};
    (void)snprintf(lib_include_path, sizeof(lib_include_path), "%s%sinclude", target_lib->library_path, PATH_SEPARATOR);

    if (dir_exists(lib_include_path)) {
        *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
        if (*includes == NULL) {
            (void)fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        (*includes)[*include_count] = strdup(lib_include_path);
        (*include_count)++;

        if (config->verbose) { printf("Added library include path: %s\n", lib_include_path); }
    }

    // Also add the library's include directory from lib/deps/ (where it's copied)
    // This is already handled by add_lib_dh_includes, but we ensure it's there
    char project_lib_path[1024] = {};
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    char deps_path[1024] = {};
    (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

    char lib_dir_part[256] = {};
    char lib_name_part[256] = {};
    extract_library_dir_and_name(target_lib->library_name, lib_dir_part, lib_name_part);

    // Add deps include path (may have subdirectory structure)
    char deps_include_path[1024] = {};
    if (strlen(lib_dir_part) > 0) {
        (void)snprintf(deps_include_path, sizeof(deps_include_path), "%s%s%s", deps_path, PATH_SEPARATOR, lib_dir_part);
    } else {
        strcpy(deps_include_path, deps_path);
    }

    if (dir_exists(deps_include_path)) {
        // Check if we already added this path (from add_lib_dh_includes)
        bool already_added = false;
        for (int i = 0; i < *include_count; ++i) {
            if (strcmp((*includes)[i], deps_include_path) == 0) {
                already_added = true;
                break;
            }
        }

        if (!already_added) {
            *includes = realloc(*includes, (*include_count + 1) * sizeof(char*));
            if (*includes == NULL) {
                (void)fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            (*includes)[*include_count] = strdup(deps_include_path);
            (*include_count)++;

            if (config->verbose) { printf("Added library deps include path: %s\n", deps_include_path); }
        }
    }
}

// Add library linking flags for a sample file
void add_library_linking_for_sample(BuildConfig* config, const char* library_name) {
    char project_lib_path[1024] = {};
    (void)snprintf(project_lib_path, sizeof(project_lib_path), "%s%slib", config->project_root, PATH_SEPARATOR);

    char deps_path[1024] = {};
    (void)snprintf(deps_path, sizeof(deps_path), "%s%sdeps", project_lib_path, PATH_SEPARATOR);

    // Find the library in lib_configs to get its structure
    LibraryConfig* target_lib = NULL;
    for (int i = 0; i < config->lib_configs.count; ++i) {
        char lib_dir_part[256] = {};
        char lib_name_part[256] = {};
        extract_library_dir_and_name(config->lib_configs.libraries[i].library_name, lib_dir_part, lib_name_part);

        if (strcmp(lib_name_part, library_name) == 0 || strcmp(config->lib_configs.libraries[i].library_name, library_name) == 0) {
            target_lib = &config->lib_configs.libraries[i];
            break;
        }
    }

    if (!target_lib) { return; }

    char lib_dir_part[256] = {};
    char lib_name_part[256] = {};
    extract_library_dir_and_name(target_lib->library_name, lib_dir_part, lib_name_part);

    // Add library search path
    if (config->linked_libraries[0] != '\0') { strcat(config->linked_libraries, " "); }
    strcat(config->linked_libraries, "-L");
    strcat(config->linked_libraries, deps_path);

    if (strlen(lib_dir_part) > 0) {
        char lib_search_path[1024] = {};
        (void)snprintf(lib_search_path, sizeof(lib_search_path), "%s%s%s", deps_path, PATH_SEPARATOR, lib_dir_part);
        strcat(config->linked_libraries, " -L");
        strcat(config->linked_libraries, lib_search_path);
    }

    // Add the library itself
    if (strcmp(target_lib->linking_type, "static") == 0) {
        char lib_file[1024] = {};
        if (strlen(lib_dir_part) > 0) {
            // Library binary is at lib/deps/dir_part/lib_name_part.lib (not in a subdirectory)
#ifdef _WIN32
            (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s%s%s.lib", deps_path, PATH_SEPARATOR, lib_dir_part, PATH_SEPARATOR, lib_name_part);
#else
            (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s%slib%s.a", deps_path, PATH_SEPARATOR, lib_dir_part, PATH_SEPARATOR, lib_name_part);
#endif
        } else {
#ifdef _WIN32
            (void)snprintf(lib_file, sizeof(lib_file), "%s%s%s.lib", deps_path, PATH_SEPARATOR, lib_name_part);
#else
            (void)snprintf(lib_file, sizeof(lib_file), "%s%slib%s.a", deps_path, PATH_SEPARATOR, lib_name_part);
#endif
        }
        strcat(config->linked_libraries, " \"");
        strcat(config->linked_libraries, lib_file);
        strcat(config->linked_libraries, "\"");
    } else {
        strcat(config->linked_libraries, " -l");
        strcat(config->linked_libraries, lib_name_part);
    }
}

void extract_library_dir_and_name(const char* lib_name, char* dir_part, char* name_part) {
    const char* last_slash = strrchr(lib_name, '/');
    if (last_slash) {
        size_t dir_len = last_slash - lib_name;
        strncpy(dir_part, lib_name, dir_len);
        dir_part[dir_len] = '\0';
        strcpy(name_part, last_slash + 1);
#ifdef _WIN32
        // Normalize path separators for Windows
        for (char* p = dir_part; *p; ++p) {
            if (*p == '/') { *p = '\\'; }
        }
#endif
    } else {
        strcpy(dir_part, "");
        strcpy(name_part, lib_name);
    }
}

bool create_directory_recursive(const char* path) { /* NOLINT(misc-no-recursion) */
    if (!path || strlen(path) == 0) {
        return true;
    }

    if (dir_exists(path)) {
        return true;
    }

    char parent_path[1024];
    strcpy(parent_path, path);

    char* last_separator = strrchr(parent_path, PATH_SEPARATOR[0]);
    if (last_separator) {
        *last_separator = '\0';

        // Create parent directory first
        if (strlen(parent_path) > 0 && !create_directory_recursive(parent_path)) {
            return false;
        }
    }

    return create_directory(path);
}
