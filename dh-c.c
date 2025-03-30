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

// Find the DH library path
bool find_dh_path(BuildConfig* config) {
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

// Print usage information
void print_usage() {
    printf("Usage: dh-c [build|test|run] [debug|release|optimize] [file.c] [options]\n\n");
    printf("Commands:\n");
    printf("  build    - Build the project or file\n");
    printf("  test     - Build and run tests\n");
    printf("  run      - Build and run the project or file\n\n");
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
    printf("Examples:\n");
    printf("  dh-c build debug                  - Build project in debug mode\n");
    printf("  dh-c run release sample.c         - Build and run single file in release mode\n");
    printf("  dh-c test sample.c                - Build and run tests for single file\n");
    printf("  dh-c build optimize --opt=O2 file.c - Build file with O2 optimization\n");
}

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

    // Check compiler availability
    if (!detect_clang()) {
        fprintf(stderr, "Warning: clang compiler not found. Using gcc as fallback.\n");
        strcpy(config.compiler, "gcc");
    }

    // Parse command-line arguments
    if (argc < 2) {
        print_usage();
        return 1;
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
