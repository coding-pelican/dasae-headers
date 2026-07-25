#include "dal-c/internal.h"
#include "dal-c-ext/str.h"
#include "dal-c-ext/env.h"
#include "dal-c-ext/path.h"
#include "dal-c-ext/proc.h"
#include "dal-c-ext/dir.h"
#include "dal-c-ext/file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static int dal_c__printUsage(const char* topic);
static void dal_c__printVersion(const dal_c_Cmd* cmd);
static bool dal_c__needsProject(const dal_c_Cmd* cmd);
static bool dal_c__allowsNoProject(const dal_c_Cmd* cmd);
static const dal_c_HelpCmd* dal_c__findHelpCmd(const char* name);
static void dal_c__printHelpCmd(const dal_c_HelpCmd* cmd);
static dal_c_Cmd* dal_c__parseAsBuild(int argc, const char* argv[], int skip_count);
static int dal_c__showTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj);
static int dal_c__doctor(const dal_c_Cmd* cmd, const dal_c_Project* proj);
static int dal_c__depsCommand(const char* action, const dal_c_Cmd* cmd, const dal_c_Project* proj);
static bool dal_c__copyTree(const char* src, const char* dst);
static int dal_c__printProjectGraph(const dal_c_Cmd* cmd, const dal_c_Project* proj, bool dot);


static bool dal_c__pathIsAbsolute(const char* value) {
    if (!value || !value[0]) return false;
#ifdef _WIN32
    return (value[0] == '/' || value[0] == '\\' || (value[1] == ':' && value[2]));
#else
    return value[0] == '/';
#endif
}

static char* dal_c__graphChildRoot(const dal_c_Project* proj, const dal_c_Lib* lib) {
    if (!lib->path || !lib->path[0]) return NULL;
    return dal_c__pathIsAbsolute(lib->path) ? strdup(lib->path) : path_join(proj->root, lib->path);
}

static bool dal_c__graphSeen(char** seen, int seen_count, const char* value) {
    for (int i = 0; i < seen_count; ++i) if (str_eql(seen[i], value)) return true;
    return false;
}

static void dal_c__graphPrintEscaped(const char* text) {
    for (const char* p = text ? text : ""; *p; ++p) {
        if (*p == '"' || *p == '\\') putchar('\\');
        putchar(*p);
    }
}

static void dal_c__graphWalkText(const dal_c_Project* proj, int depth, char*** seen, int* seen_count) {
    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        for (int j = 0; j < depth; ++j) printf("  ");
        printf("- %s [provider=%s", lib->name ? lib->name : "(unnamed)", provider);
        if (lib->revision && lib->revision[0]) printf(", revision=%s", lib->revision);
        if (lib->path && lib->path[0]) printf(", path=%s", lib->path);
        if (lib->runtime_file_count > 0) printf(", runtime=%d", lib->runtime_file_count);
        printf("]\n");
        if (!str_eql(provider, "dh") || !lib->path || !lib->path[0]) continue;
        char* child_root = dal_c__graphChildRoot(proj, lib);
        if (dal_c__graphSeen(*seen, *seen_count, child_root)) {
            for (int j = 0; j <= depth; ++j) printf("  ");
            printf("(already visited)\n");
            free(child_root); continue;
        }
        char** grown = realloc(*seen, (size_t)(*seen_count + 1) * sizeof(**seen));
        if (!grown) { free(child_root); continue; }
        *seen = grown; (*seen)[(*seen_count)++] = strdup(child_root);
        dal_c_Project* child = dal_c_Project_detectAt(child_root, proj->dh_path);
        if (child) { dal_c__graphWalkText(child, depth + 1, seen, seen_count); dal_c_Project_cleanup(&child); }
        free(child_root);
    }
}

static void dal_c__graphWalkDot(const dal_c_Project* proj, const char* parent_id, char*** seen, int* seen_count) {
    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        char* child_root = (str_eql(provider, "dh") && lib->path && lib->path[0]) ? dal_c__graphChildRoot(proj, lib) : NULL;
        const char* child_id = child_root ? child_root : lib->name;
        printf("  \""); dal_c__graphPrintEscaped(child_id); printf("\" [label=\"");
        dal_c__graphPrintEscaped(lib->name); printf("\\nprovider=%s", provider);
        if (lib->revision && lib->revision[0]) { printf("\\nrevision="); dal_c__graphPrintEscaped(lib->revision); }
        printf("\"];\n  \""); dal_c__graphPrintEscaped(parent_id); printf("\" -> \""); dal_c__graphPrintEscaped(child_id); printf("\";\n");
        if (child_root && !dal_c__graphSeen(*seen, *seen_count, child_root)) {
            char** grown = realloc(*seen, (size_t)(*seen_count + 1) * sizeof(**seen));
            if (grown) {
                *seen = grown; (*seen)[(*seen_count)++] = strdup(child_root);
                dal_c_Project* child = dal_c_Project_detectAt(child_root, proj->dh_path);
                if (child) { dal_c__graphWalkDot(child, child_root, seen, seen_count); dal_c_Project_cleanup(&child); }
            }
        }
        free(child_root);
    }
}

static int dal_c__printProjectGraph(const dal_c_Cmd* cmd, const dal_c_Project* proj, bool dot) {
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* target = dal_c__resolveTargetDirName(&cmd->opts);
    char** seen = calloc(1, sizeof(*seen));
    int seen_count = 0;
    if (seen) seen[seen_count++] = strdup(proj->root);
    if (dot) {
        printf("digraph dh_c {\n  rankdir=LR;\n  node [shape=box];\n  \"");
        dal_c__graphPrintEscaped(proj->root); printf("\" [label=\"");
        dal_c__graphPrintEscaped(proj->name ? proj->name : "project");
        printf("\\ntarget=%s\\nprofile=%s\"];\n", target ? target : "native", profile ? profile->name : "dev");
        dal_c__graphWalkDot(proj, proj->root, &seen, &seen_count);
        printf("}\n");
    } else {
        printf("PROJECT %s\n", proj->name ? proj->name : "(unnamed)");
        printf("  root: %s\n", proj->root ? proj->root : "(none)");
        printf("  target: %s\n", target ? target : "native");
        printf("  profile: %s\n", profile ? profile->name : "dev");
        printf("  direct-dependencies: %d\n", proj->lib_count);
        dal_c__graphWalkText(proj, 1, &seen, &seen_count);
    }
    for (int i = 0; i < seen_count; ++i) free(seen[i]);
    free(seen); free(target);
    return 0;
}

static bool dal_c__endsWith(const char* text, const char* suffix) {
    if (!text || !suffix) return false;
    size_t n = strlen(text), m = strlen(suffix);
    return n >= m && strcmp(text + n - m, suffix) == 0;
}

static const char* dal_c__artifactStageDir(const char* name) {
    if (!name) return NULL;
    if (dal_c__endsWith(name, ".exe") || dal_c__endsWith(name, ".dll")) return "bin";
    if (dal_c__endsWith(name, ".lib") || dal_c__endsWith(name, ".a") ||
        dal_c__endsWith(name, ".so") || dal_c__endsWith(name, ".dylib")) return "lib";
    return NULL;
}

static bool dal_c__copyFileInto(const char* src, const char* dst_dir) {
    if (!src || !dst_dir || !path_isFile(src)) return false;
    char* name = path_basename(src);
    char* dst = path_join(dst_dir, name);
    bool ok = dir_createRecur(dst_dir) && file_copy(src, dst);
    free(dst); free(name);
    return ok;
}

static char* dal_c__packageDir(const dal_c_Cmd* cmd, const dal_c_Project* proj);
static int dal_c__packageProject(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    int result = dal_c__depsCommand("install", cmd, proj);
    if (result != 0) return result;
    result = dal_c_Cmd_execute(cmd, proj);
    if (result != 0) return result;
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    if (!profile) return 1;
    char* build_dir = dal_c__makeBuildProfileDir(proj, &cmd->opts, profile);
    char* package_dir = dal_c__packageDir(cmd, proj);
    if (!build_dir || !package_dir || !path_isDir(build_dir)) {
        (void)fprintf(stderr, "Error: Build profile directory is unavailable for packaging.\n");
        free(package_dir); free(build_dir); return 1;
    }
    if (path_isDir(package_dir)) (void)dir_removeRecur(package_dir);
    bool ok = dir_createRecur(package_dir);

    int artifact_count = 0;
    char** artifacts = dir_list(build_dir, &artifact_count);
    for (int i = 0; i < artifact_count; ++i) {
        char* name = path_basename(artifacts[i]);
        const char* stage = str_eql(name, "manifest.dh") ? NULL : dal_c__artifactStageDir(name);
        char* dst_dir = stage ? path_join(package_dir, stage) : strdup(package_dir);
        if (!dal_c__copyFileInto(artifacts[i], dst_dir)) ok = false;
        free(dst_dir); free(name); free(artifacts[i]);
    }
    free(artifacts);

    char* include_src = path_join(proj->root, proj->include_dir_name ? proj->include_dir_name : "include");
    if (path_isDir(include_src)) {
        char* include_dst = path_join(package_dir, "include");
        if (!dal_c__copyTree(include_src, include_dst)) ok = false;
        free(include_dst);
    }
    free(include_src);

    char* target = dal_c__resolveTargetDirName(&cmd->opts);
    char* dep_packages = path_join(proj->root, ".dh-c/deps/packages");
    char* dep_target = path_join(dep_packages, target ? target : "native");
    char* dep_profile = path_join(dep_target, profile->name);
    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        if (str_eql(provider, "dh")) continue;
        char* lib_package = path_join(dep_profile, lib->name);
        char* bin_dir = path_join(package_dir, "bin");
        if (lib->runtime_file_count > 0) {
            for (int j = 0; j < lib->runtime_file_count; ++j) {
                char* src = path_join(lib_package, lib->runtime_files[j]);
                if (path_isDir(src)) {
                    if (!dal_c__copyTree(src, bin_dir)) ok = false;
                } else if (!dal_c__copyFileInto(src, bin_dir)) {
                    (void)fprintf(stderr, "Error: Runtime export `%s` for dependency `%s` was not found in %s.\n",
                        lib->runtime_files[j], lib->name, lib_package);
                    ok = false;
                }
                free(src);
            }
        } else {
            char* conventional_bin = path_join(lib_package, "bin");
            if (path_isDir(conventional_bin) && !dal_c__copyTree(conventional_bin, bin_dir)) ok = false;
            free(conventional_bin);
        }
        free(bin_dir); free(lib_package);
    }

    const char* asset_names[] = { "assets", "resources", NULL };
    for (int i = 0; asset_names[i]; ++i) {
        char* src = path_join(proj->root, asset_names[i]);
        if (path_isDir(src)) {
            char* dst = path_join(package_dir, asset_names[i]);
            if (!dal_c__copyTree(src, dst)) ok = false;
            free(dst);
        }
        free(src);
    }
    printf("[PACKAGE] %s\n", package_dir);
    free(dep_profile); free(dep_target); free(dep_packages); free(target);
    free(package_dir); free(build_dir);
    return ok ? 0 : 1;
}

static int dal_c__installProject(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* prefix);
static void dal_c__printProjectStatus(const dal_c_Cmd* cmd, const dal_c_Project* proj);

int main(int argc, const char* argv[]) {
    if (argc < 2) return dal_c__printUsage(NULL), 1;

    if (str_eql(argv[1], dal_c_cmd_action_help)) {
        return dal_c__printUsage(argc > 2 ? argv[2] : NULL);
    }

    if (str_eql(argv[1], "cache")) {
        (void)fprintf(stderr, "Error: `%s cache ...` is not a canonical command.\n", dal_c_tool_name);
        (void)fprintf(stderr, "  Use `%s status`, `%s clean`, or `%s explain rebuild`.\n",
            dal_c_tool_name, dal_c_tool_name, dal_c_tool_name);
        return 1;
    }
    bool special_project_dep_action = str_eql(argv[1], "fetch") || str_eql(argv[1], "update") || str_eql(argv[1], "status");
    bool special_graph = str_eql(argv[1], "graph");
    bool special_package = str_eql(argv[1], "package");
    bool special_install = str_eql(argv[1], "install");
    if (argc > 2 && str_eql(argv[1], dal_c_cmd_action_deps)
        && (str_eql(argv[2], "fetch") || str_eql(argv[2], "update") || str_eql(argv[2], "status")
            || str_eql(argv[2], "build") || str_eql(argv[2], "install"))) {
        (void)fprintf(stderr, "Error: `%s deps %s` is not a canonical command.\n", dal_c_tool_name, argv[2]);
        (void)fprintf(stderr, "  Use `%s %s` for project dependency state, or `%s deps` to build dependencies.\n",
            dal_c_tool_name, argv[2], dal_c_tool_name);
        return 1;
    }
    if (special_project_dep_action) {
        dal_c_Cmd* deps_cmd = dal_c__parseAsBuild(argc, argv, 1);
        if (!deps_cmd) {
            (void)fprintf(stderr, "Error: Failed to initialize dependency command\n");
            return 1;
        }
        dal_c_Project* deps_proj = dal_c_Project_detect(deps_cmd);
        if (!deps_proj || !deps_proj->root) {
            (void)fprintf(stderr, "Error: Not in a dh-c project directory\n");
            if (deps_proj) dal_c_Project_cleanup(&deps_proj);
            dal_c_Cmd_cleanup(&deps_cmd);
            return 1;
        }
        int deps_result = dal_c__depsCommand(argv[1], deps_cmd, deps_proj);
        if (str_eql(argv[1], "status")) dal_c__printProjectStatus(deps_cmd, deps_proj);
        dal_c_Project_cleanup(&deps_proj);
        dal_c_Cmd_cleanup(&deps_cmd);
        return deps_result;
    }

    if (special_graph) {
        bool dot = false;
        const char** filtered = calloc((size_t)argc + 1u, sizeof(*filtered));
        if (!filtered) return 1;
        int filtered_argc = 0;
        filtered[filtered_argc++] = argv[0];
        filtered[filtered_argc++] = "build";
        for (int i = 2; i < argc; ++i) {
            if (str_eql(argv[i], "--format=dot")) { dot = true; continue; }
            if (strncmp(argv[i], "--format=", 9) == 0) {
                (void)fprintf(stderr, "Error: Unsupported graph format: %s\n", argv[i] + 9);
                free(filtered); return 1;
            }
            filtered[filtered_argc++] = argv[i];
        }
        dal_c_Cmd* graph_cmd = dal_c_Cmd_parse(filtered_argc, filtered);
        free(filtered);
        if (!graph_cmd) return 1;
        dal_c_Project* graph_proj = dal_c_Project_detect(graph_cmd);
        if (!graph_proj || !graph_proj->root) {
            (void)fprintf(stderr, "Error: Not in a dh-c project directory\n");
            if (graph_proj) dal_c_Project_cleanup(&graph_proj);
            dal_c_Cmd_cleanup(&graph_cmd); return 1;
        }
        int graph_result = dal_c__printProjectGraph(graph_cmd, graph_proj, dot);
        dal_c_Project_cleanup(&graph_proj); dal_c_Cmd_cleanup(&graph_cmd);
        return graph_result;
    }

    if (special_package || special_install) {
        const char* prefix = getenv("DH_PREFIX");
        const char** filtered = calloc((size_t)argc + 1u, sizeof(*filtered));
        if (!filtered) return 1;
        int filtered_argc = 0;
        filtered[filtered_argc++] = argv[0];
        filtered[filtered_argc++] = "build";
        for (int i = 2; i < argc; ++i) {
            if (strncmp(argv[i], "--prefix=", 9) == 0) {
                prefix = argv[i] + 9;
                continue;
            }
            if (str_eql(argv[i], "--prefix") && i + 1 < argc) {
                prefix = argv[++i];
                continue;
            }
            filtered[filtered_argc++] = argv[i];
        }
        dal_c_Cmd* package_cmd = dal_c_Cmd_parse(filtered_argc, filtered);
        free(filtered);
        if (!package_cmd) {
            (void)fprintf(stderr, "Error: Failed to parse %s command\n", argv[1]);
            return 1;
        }
        dal_c_Project* package_proj = dal_c_Project_detect(package_cmd);
        if (!package_proj || !package_proj->root) {
            (void)fprintf(stderr, "Error: Not in a dh-c project directory\n");
            if (package_proj) dal_c_Project_cleanup(&package_proj);
            dal_c_Cmd_cleanup(&package_cmd);
            return 1;
        }
        int package_result = special_package
            ? dal_c__packageProject(package_cmd, package_proj)
            : dal_c__installProject(package_cmd, package_proj, prefix);
        dal_c_Project_cleanup(&package_proj);
        dal_c_Cmd_cleanup(&package_cmd);
        return package_result;
    }

    bool special_plan = str_eql(argv[1], "plan");
    bool special_explain = argc > 2 && str_eql(argv[1], "explain") && str_eql(argv[2], "rebuild");
    bool special_target = argc > 2 && str_eql(argv[1], "target") && str_eql(argv[2], "show");
    bool special_doctor = str_eql(argv[1], "doctor");
    dal_c_Cmd* cmd = (special_plan || special_doctor)
                   ? dal_c__parseAsBuild(argc, argv, 1)
                   : ((special_explain || special_target) ? dal_c__parseAsBuild(argc, argv, 2) : dal_c_Cmd_parse(argc, argv));
    if (!cmd) {
        (void)fprintf(stderr, "Error: Failed to parse command\n");
        return 1;
    }
    if (cmd->is_version || cmd->is_help) {
        int result = 0;
        if (cmd->is_version) dal_c__printVersion(cmd);
        if (cmd->is_help) {
            const char* topic = NULL;
            if (argc > 1 && argv[1][0] != '-') {
                topic = argv[1];
            }
            result = dal_c__printUsage(topic);
        }
        return dal_c_Cmd_cleanup(&cmd), result;
    }

    dal_c_Project* proj = NULL;
    if (special_target || special_doctor) {
        proj = dal_c_Project_detect(cmd);
        if (!proj) {
            proj = NULL;
        }
    } else if (dal_c__needsProject(cmd)) {
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
    int result = 0;
    if (special_target) {
        result = dal_c__showTarget(cmd, proj);
    } else if (special_doctor) {
        result = dal_c__doctor(cmd, proj);
    } else {
        if (special_plan || special_explain) {
            cmd->dry_run = true;
            cmd->explain_rebuild = special_explain;
        }
        if (cmd->action == dal_c_CmdAction_deps) {
            result = dal_c__depsCommand("install", cmd, proj);
        }
        if (result == 0) result = dal_c_Cmd_execute(cmd, proj);
    }
    if (proj) dal_c_Project_cleanup(&proj);
    dal_c_Cmd_cleanup(&cmd);
    return result;
}


static bool dal_c__copyTree(const char* src, const char* dst) {
    if (!src || !dst || !path_isDir(src)) return false;
    if (!dir_createRecur(dst)) return false;
    int count = 0;
    char** files = dir_listRecur(src, &count);
    bool ok = true;
    size_t src_len = strlen(src);
    for (int i = 0; i < count; ++i) {
        const char* rel = files[i] + src_len;
        while (*rel == '/' || *rel == '\\') ++rel;
        char* out = path_join(dst, rel);
        char* parent = path_parent(out);
        if (!parent || !dir_createRecur(parent) || !file_copy(files[i], out)) ok = false;
        free(parent); free(out); free(files[i]);
    }
    free(files);
    return ok;
}

static char* dal_c__packageDir(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* target = dal_c__resolveTargetDirName(&cmd->opts);
    char* package_root = path_join(proj->root, "package");
    char* target_root = path_join(package_root, target ? target : "native");
    char* result = path_join(target_root, profile ? profile->name : "dev");
    free(target_root); free(package_root); free(target);
    return result;
}


static int dal_c__installProject(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* prefix) {
    if (!prefix || !prefix[0]) {
        (void)fprintf(stderr, "Error: install requires --prefix=<path> or DH_PREFIX.\n");
        return 1;
    }
    int result = dal_c__packageProject(cmd, proj);
    if (result != 0) return result;
    char* package_dir = dal_c__packageDir(cmd, proj);
    if (path_isDir(prefix)) {
        /* Preserve unrelated prefix contents; copy package tree into it. */
    } else if (!dir_createRecur(prefix)) {
        free(package_dir); return 1;
    }
    bool ok = dal_c__copyTree(package_dir, prefix);
    printf("[INSTALL] %s\n", prefix);
    free(package_dir);
    return ok ? 0 : 1;
}

static void dal_c__printProjectStatus(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(cmd->opts.profile);
    char* build_dir = profile ? dal_c__makeBuildProfileDir(proj, &cmd->opts, profile) : NULL;
    char* package_dir = dal_c__packageDir(cmd, proj);
    char* manifest = build_dir ? path_join(build_dir, "manifest.dh") : NULL;
    char* cache = build_dir ? path_join(build_dir, ".cache") : NULL;
    printf("\nProject:\n  root=%s\n  profile=%s\n", proj->root, profile ? profile->name : "dev");
    printf("Build:\n  directory=%s\n  manifest=%s\n", build_dir ? build_dir : "(unknown)", manifest && path_isFile(manifest) ? "ready" : "missing");
    printf("Cache:\n  state=%s\n", cache && path_isDir(cache) ? "present" : "empty");
    printf("Package:\n  directory=%s\n  state=%s\n", package_dir, path_isDir(package_dir) ? "present" : "missing");
    free(cache); free(manifest); free(package_dir); free(build_dir);
}

static bool dal_c__depsGitRun(const char* cwd, const char* a, const char* b, const char* c, const char* d) {
    const char* argv[10] = { "git", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    int i = 1;
    if (cwd) { argv[i++] = "-C"; argv[i++] = cwd; }
    if (a) argv[i++] = a;
    if (b) argv[i++] = b;
    if (c) argv[i++] = c;
    if (d) argv[i++] = d;
    argv[i] = NULL;
    return proc_run(argv, true) == 0;
}

static char* dal_c__depsGitOutput(const char* cwd, const char* a, const char* b) {
    const char* argv[7] = { "git", "-C", cwd, a, b, NULL, NULL };
    return proc_output(argv);
}

static char* dal_c__depsResolveRevision(const char* cwd, const char* revision) {
    char* tag_ref = revision && revision[0]
                  ? str_format("refs/tags/%s^{commit}", revision)
                  : NULL;
    char* remote_ref = revision && revision[0]
                     ? str_format("refs/remotes/origin/%s^{commit}", revision)
                     : strdup("refs/remotes/origin/HEAD^{commit}");
    char* direct_ref = revision && revision[0]
                     ? str_format("%s^{commit}", revision)
                     : strdup("HEAD^{commit}");
    const char* candidates[] = { tag_ref, remote_ref, direct_ref, NULL };
    char* resolved = NULL;
    for (int i = 0; candidates[i] && !resolved; ++i) {
        const char* argv[] = { "git", "-C", cwd, "rev-parse", "--verify", candidates[i], NULL };
        if (proc_run(argv, false) != 0) { continue; }
        char* output = proc_output(argv);
        if (output) {
            char* trimmed = str_trim(output);
            if (trimmed && trimmed[0]) { resolved = strdup(trimmed); }
            free(trimmed);
        }
        free(output);
    }
    free(direct_ref);
    free(remote_ref);
    free(tag_ref);
    return resolved;
}


static bool dal_c__depsShell(const char* cwd, const char* command, const char* src_dir,
                             const char* build_dir, const char* package_dir,
                             const char* profile, const char* target) {
    if (!command || !command[0]) return true;
#ifdef _WIN32
    char* script = str_format("cd /D \"%s\" && set \"DH_DEP_SOURCE=%s\" && set \"DH_DEP_BUILD=%s\" && set \"DH_DEP_PACKAGE=%s\" && set \"DH_DEP_PROFILE=%s\" && set \"DH_DEP_TARGET=%s\" && %s",
        cwd, src_dir, build_dir, package_dir, profile, target, command);
    const char* argv[] = { "cmd.exe", "/D", "/S", "/C", script, NULL };
#else
    char* script = str_format("cd \"%s\" && export DH_DEP_SOURCE=\"%s\" DH_DEP_BUILD=\"%s\" DH_DEP_PACKAGE=\"%s\" DH_DEP_PROFILE=\"%s\" DH_DEP_TARGET=\"%s\" && %s",
        cwd, src_dir, build_dir, package_dir, profile, target, command);
    const char* argv[] = { "/bin/sh", "-c", script, NULL };
#endif
    int result = proc_run(argv, true);
    free(script);
    return result == 0;
}

static const char* dal_c__depsCmakeBuildType(const char* profile) {
    if (str_eql(profile, "dev") || str_eql(profile, "test")) return "Debug";
    if (str_eql(profile, "stable")) return "RelWithDebInfo";
    if (str_eql(profile, "release") || str_eql(profile, "optimize") || str_eql(profile, "fast")) return "Release";
    if (str_eql(profile, "compact") || str_eql(profile, "micro")) return "MinSizeRel";
    return "Release";
}

static bool dal_c__depsBuildProvider(const char* action, const dal_c_Lib* lib,
                                     const char* source_dir, const char* build_dir,
                                     const char* package_dir, const char* profile,
                                     const char* target) {
    const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
    bool install = str_eql(action, "install");
    if (str_eql(provider, "dh")) {
        /* provider=dh remains owned by the normal dh-c dependency graph. */
        return true;
    }
    if (str_eql(provider, "cmake")) {
        if (!dir_createRecur(build_dir) || !dir_createRecur(package_dir)) return false;
        char* prefix = str_format("-DCMAKE_INSTALL_PREFIX=%s", package_dir);
        char* build_type = str_format("-DCMAKE_BUILD_TYPE=%s", dal_c__depsCmakeBuildType(profile));
        const char* configure[] = { "cmake", "-S", source_dir, "-B", build_dir, prefix, build_type, NULL };
        const char* build[] = { "cmake", "--build", build_dir, "--config", dal_c__depsCmakeBuildType(profile), NULL };
        bool ok = proc_run(configure, true) == 0 && proc_run(build, true) == 0;
        if (ok && install) {
            const char* install_argv[] = { "cmake", "--install", build_dir, "--config", dal_c__depsCmakeBuildType(profile), NULL };
            ok = proc_run(install_argv, true) == 0;
        }
        free(build_type); free(prefix);
        return ok;
    }
    if (str_eql(provider, "make")) {
        if (!dir_createRecur(build_dir) || !dir_createRecur(package_dir)) return false;
        bool ok;
        if (lib->build_command && lib->build_command[0])
            ok = dal_c__depsShell(source_dir, lib->build_command, source_dir, build_dir, package_dir, profile, target);
        else {
            const char* build[] = { "make", "-C", source_dir, NULL };
            ok = proc_run(build, true) == 0;
        }
        if (ok && install) {
            if (lib->install_command && lib->install_command[0])
                ok = dal_c__depsShell(source_dir, lib->install_command, source_dir, build_dir, package_dir, profile, target);
            else {
                char* prefix = str_format("PREFIX=%s", package_dir);
                const char* install_argv[] = { "make", "-C", source_dir, "install", prefix, NULL };
                ok = proc_run(install_argv, true) == 0;
                free(prefix);
            }
        }
        return ok;
    }
    if (str_eql(provider, "custom")) {
        if (!lib->build_command || !lib->build_command[0]) {
            (void)fprintf(stderr, "Error: custom dependency `%s` requires build-command=.\n", lib->name);
            return false;
        }
        if (!dir_createRecur(build_dir) || !dir_createRecur(package_dir)) return false;
        bool ok = dal_c__depsShell(source_dir, lib->build_command, source_dir, build_dir, package_dir, profile, target);
        if (ok && install && lib->install_command && lib->install_command[0])
            ok = dal_c__depsShell(source_dir, lib->install_command, source_dir, build_dir, package_dir, profile, target);
        return ok;
    }
    if (str_eql(provider, "prebuilt")) {
        const char* root = (lib->path && lib->path[0]) ? lib->path : source_dir;
        if (!path_isDir(root)) {
            (void)fprintf(stderr, "Error: prebuilt dependency `%s` has no package directory: %s\n", lib->name, root);
            return false;
        }
        printf("[PREBUILT] %-16s %s\n", lib->name, root);
        return true;
    }
    (void)fprintf(stderr, "Error: Unknown dependency provider `%s` for `%s`.\n", provider, lib->name);
    return false;
}


static bool dal_c__stageExternalPackageForBuild(const dal_c_Project* proj, const dal_c_Lib* lib, const char* package_dir) {
    if (!proj || !lib || !package_dir) return false;
    char* deps_dir = dal_c_Project_getDepsDir(proj);
    if (!deps_dir || !dir_createRecur(deps_dir)) { free(deps_dir); return false; }
    bool ok = true;
    char* include_src = path_join(package_dir, "include");
    if (path_isDir(include_src) && !dal_c__copyTree(include_src, deps_dir)) ok = false;
    free(include_src);

    char* lib_src = path_join(package_dir, "lib");
    if (path_isDir(lib_src)) {
        int count = 0;
        char** files = dir_listRecur(lib_src, &count);
        for (int i = 0; files && i < count; ++i) {
            const char* f = files[i];
            if (dal_c__endsWith(f, ".lib") || dal_c__endsWith(f, ".a") ||
                dal_c__endsWith(f, ".so") || dal_c__endsWith(f, ".dylib")) {
                if (!dal_c__copyFileInto(f, deps_dir)) ok = false;
            }
            free(files[i]);
        }
        free(files);
    }
    free(lib_src);
    free(deps_dir);
    return ok;
}

static bool dal_c__depsLockAllowsNewEntry(const char* reason) {
    return reason && strstr(reason, "is not recorded") != NULL;
}

static bool dal_c__depsTouchUsage(const char* deps_root, const char* name) {
    if (!deps_root || !name || !name[0]) { return false; }
    char* usage_root = path_join(deps_root, "usage");
    char* stamp_name = str_format("%s.stamp", name);
    char* stamp_path = (usage_root && stamp_name) ? path_join(usage_root, stamp_name) : NULL;
    char* content = str_format("%lld\n", (long long)time(NULL));
    bool ok = usage_root && stamp_path && content
           && dir_createRecur(usage_root)
           && file_writeAtomic(stamp_path, content);
    free(content);
    free(stamp_path);
    free(stamp_name);
    free(usage_root);
    return ok;
}

static int dal_c__depsCommand(const char* action, const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    assert(action != NULL);
    assert(proj != NULL && proj->root != NULL);
    assert(cmd != NULL);

    char* state_root = dal_c_Project_getStateRoot(proj);
    char* deps_root = state_root ? path_join(state_root, "deps") : NULL;
    char* src_root = deps_root ? path_join(deps_root, "src") : NULL;
    char* build_root = deps_root ? path_join(deps_root, "build") : NULL;
    char* package_root = deps_root ? path_join(deps_root, "packages") : NULL;
    char* lock_path = dal_c_Project_getDependencyLockPath(proj);
    if (!state_root || !deps_root || !src_root || !build_root || !package_root || !lock_path
        || !dir_createRecur(src_root) || !dir_createRecur(build_root) || !dir_createRecur(package_root)) {
        (void)fprintf(stderr, "Error: Failed to create dependency cache directories under %s\n",
            deps_root ? deps_root : "(unknown)");
        free(lock_path); free(package_root); free(build_root); free(src_root); free(deps_root); free(state_root);
        return 1;
    }

    char* lock_text = strdup("# Generated by dh-c fetch/update. Do not edit by hand.\n");
    int failures = 0;
    int external_count = 0;

    for (int i = 0; i < proj->lib_count; ++i) {
        const dal_c_Lib* lib = &proj->libraries[i];
        const char* provider = (lib->provider && lib->provider[0]) ? lib->provider : "dh";
        bool provider_action = str_eql(action, "build") || str_eql(action, "install");
        bool has_source = lib->source && lib->source[0] != '\0';
        bool has_local_provider_path = provider_action && lib->path && lib->path[0] != '\0'
                                    && !str_eql(provider, "dh");
        if (!has_source && !has_local_provider_path
            && !(provider_action && str_eql(provider, "prebuilt"))) {
            continue;
        }

        external_count++;
        char* source_dir = has_source ? path_join(src_root, lib->name) : strdup(lib->path);
        bool exists = source_dir && (path_isDir(source_dir) || path_isFile(source_dir));

        if (provider_action) {
            const char* profile = dal_c_Profile_format(cmd->opts.profile);
            if (!profile) profile = "dev";
            const char* target = (cmd->opts.arch_target && cmd->opts.arch_target[0]) ? cmd->opts.arch_target : "native";
            char* target_build = path_join(build_root, target);
            char* profile_build = path_join(target_build, profile);
            char* build_dir = path_join(profile_build, lib->name);
            char* target_package = path_join(package_root, target);
            char* profile_package = path_join(target_package, profile);
            char* package_dir = path_join(profile_package, lib->name);

            bool ready = true;
            if (!str_eql(provider, "prebuilt") && !exists) {
                (void)fprintf(stderr, "Error: Dependency `%s` is not fetched. Run `dh-c fetch`.\n", lib->name);
                ready = false;
            }
            if (ready && has_source) {
                char* lock_reason = NULL;
                if (!dal_c_Project_dependencyCheckoutMatchesLock(proj, lib, &lock_reason)) {
                    (void)fprintf(stderr, "Error: Dependency `%s` is not at its locked revision: %s\n",
                        lib->name, lock_reason ? lock_reason : "lock mismatch");
                    ready = false;
                }
                free(lock_reason);
            }
            if (!ready) {
                failures++;
            } else {
                printf("[%s] %-16s provider=%s profile=%s target=%s\n",
                    str_eql(action, "install") ? "INSTALL" : "BUILD", lib->name, provider, profile, target);
                bool provider_ok = dal_c__depsBuildProvider(
                    action, lib, source_dir, build_dir, package_dir, profile, target
                );
                if (!provider_ok) {
                    failures++;
                } else if (str_eql(action, "install") && !dal_c__stageExternalPackageForBuild(proj, lib, package_dir)) {
                    (void)fprintf(stderr, "Error: Failed to stage exported include/link artifacts for dependency `%s`.\n", lib->name);
                    failures++;
                } else if (!dal_c__depsTouchUsage(deps_root, lib->name)) {
                    (void)fprintf(stderr, "Warning: Failed to update dependency usage stamp for `%s`.\n", lib->name);
                }
            }

            free(package_dir); free(profile_package); free(target_package);
            free(build_dir); free(profile_build); free(target_build); free(source_dir);
            continue;
        }

        if (!has_source) {
            free(source_dir);
            continue;
        }

        char* locked_revision = NULL;
        char* lock_reason = NULL;
        bool lock_file_exists = path_isFile(lock_path);
        bool has_locked_revision = dal_c_Project_readDependencyLock(
            proj, lib, &locked_revision, &lock_reason
        );
        bool allow_new_entry = !lock_file_exists || dal_c__depsLockAllowsNewEntry(lock_reason);
        if (!str_eql(action, "update") && !has_locked_revision && !allow_new_entry) {
            (void)fprintf(stderr, "Error: %s. Run `dh-c update` to replace the dependency resolution.\n",
                lock_reason ? lock_reason : "invalid dependency lock");
            failures++;
            free(lock_reason);
            free(locked_revision);
            free(source_dir);
            continue;
        }
        free(lock_reason);

        if (str_eql(action, "fetch")) {
            if (!exists) {
                const char* argv[] = { "git", "clone", "--", lib->source, source_dir, NULL };
                printf("[FETCH] %s <- %s\n", lib->name, lib->source);
                if (proc_run(argv, true) != 0) {
                    failures++;
                    free(locked_revision);
                    free(source_dir);
                    continue;
                }
                exists = true;
            } else {
                printf("[HAVE]  %s\n", lib->name);
            }
            if (!dal_c__depsGitRun(source_dir, "fetch", "--tags", "--prune", NULL)) {
                failures++;
                free(locked_revision);
                free(source_dir);
                continue;
            }
            char* revision_to_checkout = has_locked_revision
                                       ? strdup(locked_revision)
                                       : dal_c__depsResolveRevision(source_dir, lib->revision);
            if (!revision_to_checkout
                || !dal_c__depsGitRun(source_dir, "checkout", "--detach", revision_to_checkout, NULL)) {
                (void)fprintf(stderr, "Error: Failed to resolve/set %s to revision %s\n",
                    lib->name, has_locked_revision ? locked_revision : (lib->revision ? lib->revision : "origin default"));
                failures++;
                free(revision_to_checkout);
                free(locked_revision);
                free(source_dir);
                continue;
            }
            free(revision_to_checkout);
        } else if (str_eql(action, "update")) {
            if (!exists) {
                const char* argv[] = { "git", "clone", "--", lib->source, source_dir, NULL };
                printf("[FETCH] %s <- %s\n", lib->name, lib->source);
                if (proc_run(argv, true) != 0) {
                    failures++;
                    free(locked_revision);
                    free(source_dir);
                    continue;
                }
                exists = true;
            }
            printf("[UPDATE] %s\n", lib->name);
            if (!dal_c__depsGitRun(source_dir, "fetch", "--tags", "--prune", NULL)) {
                failures++;
                free(locked_revision);
                free(source_dir);
                continue;
            }
            char* resolved_revision = dal_c__depsResolveRevision(source_dir, lib->revision);
            if (!resolved_revision
                || !dal_c__depsGitRun(source_dir, "checkout", "--detach", resolved_revision, NULL)) {
                (void)fprintf(stderr, "Error: Failed to resolve/update %s from revision %s\n",
                    lib->name, lib->revision && lib->revision[0] ? lib->revision : "origin default");
                failures++;
                free(resolved_revision);
                free(locked_revision);
                free(source_dir);
                continue;
            }
            free(resolved_revision);
        } else if (!exists) {
            printf("[MISSING] %-16s provider=%s source=%s\n", lib->name, provider, lib->source);
            failures++;
            free(locked_revision);
            free(source_dir);
            continue;
        }

        if (path_isDir(source_dir)) {
            char* head = dal_c__depsGitOutput(source_dir, "rev-parse", "HEAD");
            char* dirty = dal_c__depsGitOutput(source_dir, "status", "--porcelain");
            if (head) {
                char* trimmed_head = str_trim(head);
                char* trimmed_dirty = dirty ? str_trim(dirty) : NULL;
                bool lock_matches = has_locked_revision && trimmed_head && str_eql(trimmed_head, locked_revision);
                const char* state = str_eql(action, "status")
                                  ? (lock_matches ? "READY" : (has_locked_revision ? "DRIFT" : "UNLOCKED"))
                                  : "LOCK";
                printf("[%s] %-16s provider=%s revision=%s%s\n",
                    state, lib->name, provider, trimmed_head ? trimmed_head : "(unavailable)",
                    (trimmed_dirty && trimmed_dirty[0]) ? " dirty" : "");
                if (str_eql(action, "status") && !has_locked_revision) {
                    (void)fprintf(stderr, "Error: Dependency `%s` is not recorded in lock.dh\n", lib->name);
                    failures++;
                } else if (str_eql(action, "status") && !lock_matches) {
                    (void)fprintf(stderr, "Error: Dependency `%s` checkout differs from lock.dh\n", lib->name);
                    failures++;
                }
                if (str_eql(action, "fetch") || str_eql(action, "update")) {
                    char* entry = str_format(
                        "[%s]\nprovider=%s\nsource=%s\nrevision=%s\n\n",
                        lib->name, provider, lib->source, trimmed_head ? trimmed_head : ""
                    );
                    char* grown = (lock_text && entry) ? str_format("%s%s", lock_text, entry) : NULL;
                    if (!grown) {
                        failures++;
                    } else {
                        free(lock_text);
                        lock_text = grown;
                        if (!dal_c__depsTouchUsage(deps_root, lib->name)) {
                            (void)fprintf(stderr, "Warning: Failed to update dependency usage stamp for `%s`.\n", lib->name);
                        }
                    }
                    free(entry);
                }
                free(trimmed_dirty);
                free(trimmed_head);
            } else {
                failures++;
            }
            free(head);
            free(dirty);
        }
        free(locked_revision);
        free(source_dir);
    }

    if (external_count == 0) {
        printf("No dependencies match this operation.\n");
    } else if ((str_eql(action, "fetch") || str_eql(action, "update")) && failures == 0) {
        if (!file_writeAtomic(lock_path, lock_text ? lock_text : "")) {
            (void)fprintf(stderr, "Error: Failed to write dependency lock: %s\n", lock_path);
            failures++;
        } else {
            printf("[LOCK] %s\n", lock_path);
        }
    }

    free(lock_text);
    free(lock_path); free(package_root); free(build_root); free(src_root); free(deps_root); free(state_root);
    return failures == 0 ? 0 : 1;
}

static int dal_c__printUsage(const char* topic) {
    bool list_only = topic && str_eql(topic, "--list");
    bool print_all = topic && str_eql(topic, "--all");
    if (list_only) {
        for (int i = 0; i < dal_c_help_cmds_count; ++i) {
            const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
            if (cmd->name && cmd->implemented) { printf("%s\n", cmd->name); }
        }
        printf("fetch\nupdate\nstatus\ngraph\npackage\ninstall\nplan\nexplain\ntarget\ndoctor\n");
        return 0;
    }
    if (print_all) { topic = NULL; }

    const int help_cmd_count = dal_c_help_cmds_count;
    const int global_option_count = dal_c_help_global_options_count;
    const int help_profile_count = dal_c_help_profiles_count;

    if (topic && (str_eql(topic, "fetch") || str_eql(topic, "update") || str_eql(topic, "status")
        || str_eql(topic, "graph") || str_eql(topic, "package") || str_eql(topic, "install")
        || str_eql(topic, "plan") || str_eql(topic, "explain") || str_eql(topic, "target") || str_eql(topic, "doctor"))) {
        printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
        if (str_eql(topic, "fetch")) {
            printf("USAGE:\n  %s fetch [build options]\n\nFetch missing external dependency sources declared by the current project.dh without changing an existing resolution.\n", dal_c_tool_name);
        } else if (str_eql(topic, "update")) {
            printf("USAGE:\n  %s update [build options]\n\nRefresh external dependency refs and rewrite the resolved dependency lock for the current project.dh.\n", dal_c_tool_name);
        } else if (str_eql(topic, "status")) {
            printf("USAGE:\n  %s status [build options]\n\nShow source, provider, revision, and dirty-state readiness for dependencies declared by the current project.dh.\n", dal_c_tool_name);
        } else if (str_eql(topic, "graph")) {
            printf("USAGE:\n  %s graph [profile] [build options] [--format=dot]\n\nShow the resolved project dependency graph and its provider metadata.\n", dal_c_tool_name);
        } else if (str_eql(topic, "package")) {
            printf("USAGE:\n  %s package [profile] [build options]\n\nBuild and stage the current project with dependency runtime exports.\n", dal_c_tool_name);
        } else if (str_eql(topic, "install")) {
            printf("USAGE:\n  %s install [profile] [build options] [--prefix=<path>]\n\nInstall the current project package into --prefix or DH_PREFIX.\n", dal_c_tool_name);
        } else if (str_eql(topic, "plan")) {
            printf("USAGE:\n  %s plan [profile] [path] [build options]\n\nGenerate the real build plan and Makefile without compiling or linking.\n", dal_c_tool_name);
        } else if (str_eql(topic, "explain")) {
            printf("USAGE:\n  %s explain rebuild [profile] [path] [build options]\n\nExplain whether the requested output is missing or its tracked plan is stale.\n", dal_c_tool_name);
        } else if (str_eql(topic, "target")) {
            printf("USAGE:\n  %s target show [profile] [build options]\n\nShow the requested and normalized target, compiler, profile, and target-scoped build directory.\n", dal_c_tool_name);
        } else {
            printf("USAGE:\n  %s doctor [profile] [build options]\n\nCheck compiler, make, archiver, DH installation, project detection, and effective target.\n", dal_c_tool_name);
        }
        return 0;
    }
    if (topic) {
        const dal_c_HelpCmd* cmd = dal_c__findHelpCmd(topic);
        if (!cmd) {
            (void)fprintf(stderr, "Error: Unknown help topic: %s\n", topic);
            return 1;
        }
        dal_c__printHelpCmd(cmd);
        return 0;
    }

    printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
    printf("USAGE:\n");
    printf("  %s <command> [profile] [path] [options]\n", dal_c_tool_name);
    printf("  %s help [command]\n", dal_c_tool_name);
    printf("  %s help --list | --all\n", dal_c_tool_name);
    printf("  %s <command> --help\n", dal_c_tool_name);
    printf("  %s -h | --help\n", dal_c_tool_name);
    printf("  %s -v | --version\n\n", dal_c_tool_name);

    printf("COMMAND MODEL:\n");
    printf("  A command decides the phase: build, run, test, check, format, clean, or query.\n");
    printf("  A profile decides the optimization/debug contract; default profile is `dev`.\n");
    printf("  A path decides the target source, target-root member, directory, or explicit file.\n");
    printf("  Options are command-scoped. Unknown or irrelevant options are rejected instead of ignored.\n\n");

    printf("COMMANDS:\n");
    printf("\nBUILD AND EXECUTION:\n");
    printf("  %-14s %s\n", "build", "Build a project, explicit source set, target-root member, or library");
    printf("  %-14s %s\n", "run", "Build and run a project, explicit file, or declared target");
    printf("  %-14s %s\n", "test", "Build and run tests");
    printf("  %-14s %s\n", "clean", "Clean build outputs, caches, or generated dependency state");
    printf("\n");

    printf("DEPENDENCIES AND DELIVERY:\n");
    printf("  %-14s %s\n", "deps", "Build dependencies declared by project.dh");
    printf("  %-14s %s\n", "fetch", "Fetch dependency sources at the locked resolution");
    printf("  %-14s %s\n", "update", "Resolve dependency requests and rewrite lock.dh");
    printf("  %-14s %s\n", "status", "Inspect dependency checkout and lock readiness");
    printf("  %-14s %s\n", "graph", "Show the resolved dependency graph");
    printf("  %-14s %s\n", "package", "Build and stage the current project package");
    printf("  %-14s %s\n", "install", "Install the staged project package");
    printf("\n");

    printf("INSPECTION AND TOOLING:\n");
    printf("  %-14s %s\n", "plan", "Generate the real build plan without executing it");
    printf("  %-14s %s\n", "explain", "Explain why a requested build requires work");
    printf("  %-14s %s\n", "target", "Inspect the effective target and output directory");
    printf("  %-14s %s\n", "doctor", "Check the build environment and DH installation");
    printf("  %-14s %s\n", "toolchain", "Inspect compiler-driver link inputs");
    printf("  %-14s %s\n", "compile-db", "Write compile_commands.json without building");
    printf("  %-14s %s\n", "syntax", "Run compiler syntax-only checks");
    printf("  %-14s %s\n", "tidy", "Run clang-tidy with dh-c's compilation database");
    printf("  %-14s %s\n", "format", "Run clang-format on selected sources");
    printf("\n");

    printf("ALIASES:\n");
    printf("  lib -> build --lib; build-dsl/test-dsl/clean-dsl -> --dsl; build-self/clean-self -> --self\n\n");

    if (!print_all) {
        printf("Use `%s help <command>` for command details, `%s help --list` for names only, or `%s help --all` for the complete reference.\n", dal_c_tool_name, dal_c_tool_name, dal_c_tool_name);
        return 0;
    }

    printf("COMMAND DETAILS:\n\n");

    for (int i = 0; i < help_cmd_count; ++i) {
        const dal_c_HelpCmd* cmd = &dal_c_help_cmds[i];
        if (!cmd->name || !cmd->implemented) { continue; }
        printf("  %s %s\n", cmd->name, cmd->usage);
        printf("    %s\n\n", cmd->description);

        if (cmd->note_count > 0) {
            printf("    Contract:\n");
            for (int j = 0; j < cmd->note_count; ++j) {
                printf("      - %s\n", cmd->notes[j]);
            }
            printf("\n");
        }

        if (cmd->option_count > 0) {
            printf("    Options:\n");
            for (int j = 0; j < cmd->option_count; ++j) {
                printf("      %-*s %s\n", dal_c_help_opt_width, cmd->options[j].name, cmd->options[j].description);
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

    printf("COMMAND OPTION BOUNDARIES:\n");
    printf("  `build`, `run`, and `test` accept compile inputs and link inputs because they produce executables or libraries.\n");
    printf("  `compile-db`, `syntax`, and `tidy` accept compile/source-selection options and do not accept link or artifact-output options.\n");
    printf("  `format` accepts only source-selection, recursion, DH discovery, and output-visibility options.\n");
    printf("  `clean` accepts cleanup scope options only; build, compile, link, and runtime options are invalid.\n\n");

    printf("TARGET RESOLUTION:\n");
    printf("  `" dal_c_tool_name " build` with no explicit path builds the project default output.\n");
    printf("  Explicit `file.c` inputs build that file directly, even outside a project.\n");
    printf("  Explicit paths under `[target-root ...]` use the declared target-root contract.\n");
    printf("  `--sample`, `--example`, and `--test` select the built-in target families.\n");
    printf("  `.` is a compatibility alias for `--all`.\n");
    printf("  `build --self` and `clean --self` operate on the " dal_c_tool_name " self boundary only.\n\n");

    printf("OUTPUT CONTRACT:\n");
    printf("  Default output names come from `project.dh` `output`, target-root `output`, or the source file stem.\n");
    printf("  `--output <path>` means an existing directory or an output stem; dh-c generates `.exe`, `.a`, `.lib`, `.so`, or `.dll` as needed.\n");
    printf("  `--output-ext <.ext>` selects an explicit extension for one artifact, such as `.pyd`; it is invalid for two-output `kind=lib` auto builds.\n");
    printf("  `compile-db --output <path>` is different: it names the JSON file path exactly.\n\n");

    printf("LINK INPUT CONTRACT:\n");
    printf("  `--link-dir <dir>` / `-L<dir>` adds a library search directory and becomes a linker `-L` flag.\n");
    printf("  `--link <name>` / `-l<name>` adds a logical library name and becomes a linker `-l` flag.\n");
    printf("  `--link-args \"...\"` is a raw escape hatch for linker flags that do not have structured options.\n");
    printf("  Link inputs are rejected by check/format commands because those commands do not link.\n\n");

    printf("PCH CONTRACT:\n");
    printf("  `pch=auto` uses the detected DH bundle when available; `pch=deps` uses generated `lib/deps.h`; `pch=off` disables PCH.\n");
    printf("  PCH files are generated inside the active profile/flag cache plan and are rebuilt when their headers or compile contract change.\n");
    printf("  `lib/deps.h` is generated only when a dependency prelude is needed; it includes top-level headers under `lib/deps/`.\n\n");

    printf("PROJECT.DH KEYS:\n");
    printf("  Top level: `output`, `kind=<executable|static-lib|shared-lib|lib>`, `build-runs-tests=<on|off>`, `self-root=<path>`, `exclude=<path>`.\n");
    printf("  Toolchain: `compiler`, `std`, `arch`/`target`, `target-arch`, `target-tune`, `target-abi`, `sysroot`, `entry`.\n");
    printf("  Compile inputs: `include`, `isystem`, `define`, `undef`, `profile`, `hosted`, `freestanding`, `loose-errors`.\n");
    printf("  Link inputs: `link`, `link-dir`, `link-dsl`, `link-libc`, `link-default-libs`, `link-start-files`, `link-compiler-rt`, `link-stdlib`, `link-crt`, `link-mode`.\n");
    printf("  Optimization/artifacts: `lto`, `omit-frame-pointer`, `function-sections`, `data-sections`, `gc-sections`, `whole-archive`, `unroll-loops`, `unwind-tables`, `async-unwind-tables`, `exceptions`, `strip`, `icf`, `merge-all-constants`, `stack-protector`.\n");
    printf("  Version: `version-core`, `version-prefix=<alpha|beta|rc>`, `version-suffix`, `version-build`.\n");
    printf("  PCH: `pch=<auto|off|deps|header>`, `pch-exclude=<header>`.\n");
    printf("  `[target-root <name>]`: `path`, `kind=<executable|static-lib|shared-lib|lib|image|preprocessed|assembly>`, `selection=<path|file|dir>`, `link-project=<on|off>`, `exclude=<path>`.\n");
    printf("  `[<dependency-name>]`: `path`, `profile`, `linking=<static|shared>`, `link-dsl`, `test`, and the compile/link keys above.\n");
    printf("  CLI options override project defaults for the current invocation.\n\n");

    printf("GENERATED STATE:\n");
    printf("  `build/` stores materialized artifacts, object files, and generated plan makefiles.\n");
    printf("  Workspace `.dh-c/cache/` or project `build/.cache/` stores reusable build-cache entries.\n");
    printf("  Project `.dh-c/deps/` stores fetched sources, provider builds, staged packages, and last-use stamps.\n");
    printf("  `lib/deps/` and `lib/deps.h` store generated dependency exports consumed by project builds.\n");
    printf("  `lock.dh` is durable resolved input beside project.dh; `clean --deps` does not rewrite it.\n");
    printf("  Use `clean --cache --older-than=30d` or `clean --deps --unused --dry-run` for maintenance.\n");
    printf("  Do not place durable source assets, checked-in resources, or manual files under generated state.\n\n");

    printf("PROFILES:\n");
    for (int i = 0; i < help_profile_count; ++i) {
        printf("  %-*s %s\n", 14, dal_c_help_profiles[i].name, dal_c_help_profiles[i].description);
    }
    printf("\n");
    printf("SUPPORT STATUS:\n");
    printf("  `workspace` and `project` are reserved scaffold commands and are not implemented.\n");
    return 0;
}

static const dal_c_HelpCmd* dal_c__findHelpCmd(const char* name) {
    if (!name) { return NULL; }
    for (int i = 0; i < dal_c_help_cmds_count; ++i) {
        if (dal_c_help_cmds[i].name && str_eql(dal_c_help_cmds[i].name, name)) {
            return &dal_c_help_cmds[i];
        }
    }
    return NULL;
}

static void dal_c__printHelpCmd(const dal_c_HelpCmd* cmd) {
    printf("%s - %s\n\n", dal_c_tool_name, dal_c_tool_description);
    printf("USAGE:\n");
    printf("  %s %s %s\n", dal_c_tool_name, cmd->name, cmd->usage ? cmd->usage : "");
    printf("  %s help %s\n\n", dal_c_tool_name, cmd->name);

    printf("SUMMARY:\n");
    printf("  %s\n\n", cmd->description);

    if (cmd->implemented) {
        printf("CONTRACT:\n");
        if (cmd->note_count > 0) {
            for (int i = 0; i < cmd->note_count; ++i) {
                printf("  %s\n", cmd->notes[i]);
            }
        } else {
            printf("  This command uses only the usage and options shown here.\n");
        }
        if (cmd->extends_build_options) {
            printf("  This command also accepts build options where they affect the command's build phase.\n");
        }
        printf("\n");
    } else {
        printf("STATUS:\n");
        printf("  Reserved command. It is listed so scripts do not assume it is implemented.\n\n");
    }

    if (cmd->option_count > 0) {
        printf("OPTIONS:\n");
        for (int i = 0; i < cmd->option_count; ++i) {
            printf("  %-*s %s\n", dal_c_help_opt_width, cmd->options[i].name, cmd->options[i].description);
        }
        if (cmd->extends_build_options) {
            printf("  [build options]%-*s See `%s help build` for the build option contract\n", dal_c_help_opt_width - 15, "", dal_c_tool_name);
        }
        printf("\n");
    }

    if (cmd->example_count > 0) {
        printf("EXAMPLES:\n");
        for (int i = 0; i < cmd->example_count; ++i) {
            printf("  %s %s\n", dal_c_tool_name, cmd->examples[i]);
        }
        printf("\n");
    }
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


static dal_c_Cmd* dal_c__parseAsBuild(int argc, const char* argv[], int skip_count) {
    int out_argc = argc - skip_count + 1;
    const char** out_argv = (const char**)calloc((size_t)out_argc + 1, sizeof(*out_argv));
    if (!out_argv) { return NULL; }
    out_argv[0] = argv[0];
    out_argv[1] = dal_c_cmd_action_build;
    int out_i = 2;
    for (int i = 1 + skip_count; i < argc; ++i) { out_argv[out_i++] = argv[i]; }
    dal_c_Cmd* result = dal_c_Cmd_parse(out_i, out_argv);
    free(out_argv);
    return result;
}

static const char* dal_c__effectiveCompiler(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->opts.compiler && cmd->opts.compiler[0]) { return cmd->opts.compiler; }
    if (proj && proj->opts.compiler && proj->opts.compiler[0]) { return proj->opts.compiler; }
    return dal_c_default_compiler;
}

static const char* dal_c__effectiveRequestedTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->opts.arch_target && cmd->opts.arch_target[0]) { return cmd->opts.arch_target; }
    if (proj && proj->opts.arch_target && proj->opts.arch_target[0]) { return proj->opts.arch_target; }
    return NULL;
}

static dal_c_Profile dal_c__effectiveProfile(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    if (cmd && cmd->profile_explicit && cmd->opts.profile != dal_c_Profile_invalid) { return cmd->opts.profile; }
    if (proj && proj->opts.profile != dal_c_Profile_invalid) { return proj->opts.profile; }
    if (cmd && cmd->opts.profile != dal_c_Profile_invalid) { return cmd->opts.profile; }
    return dal_c_default_profile;
}

static int dal_c__showTarget(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    dal_c_CompilerOpts opts = {0};
    opts.compiler = strdup(dal_c__effectiveCompiler(cmd, proj));
    const char* requested = dal_c__effectiveRequestedTarget(cmd, proj);
    opts.arch_target = requested ? strdup(requested) : NULL;
    char* normalized = dal_c__resolveTargetDirName(&opts);
    dal_c_Profile profile_id = dal_c__effectiveProfile(cmd, proj);
    const dal_c_ProfileSpec* profile = dal_c_ProfileSpec_by(profile_id);
    char* profile_dir = (proj && proj->root && profile) ? dal_c__makeBuildProfileDir(proj, &opts, profile) : NULL;
    printf("TARGET:\n");
    printf("  requested: %s\n", requested ? requested : "(host compiler default)");
    printf("  normalized: %s\n", normalized ? normalized : "(unresolved)");
    printf("  compiler: %s\n", opts.compiler ? opts.compiler : "(none)");
    printf("  profile: %s\n", profile ? profile->name : "(unknown)");
    printf("  build-dir: %s\n", profile_dir ? profile_dir : "(project unavailable)");
    printf("  native-alias: %s\n", requested ? "not used for explicit target" : "build/native");
    free(profile_dir);
    free(normalized);
    dal_c_CompilerOpts_cleanup(&opts);
    return 0;
}

static bool dal_c__toolResponds(const char* tool, const char* arg) {
    if (!tool || !tool[0]) { return false; }
    const char* argv[] = { tool, arg, NULL };
    char* output = proc_output(argv);
    bool ok = output && output[0];
    free(output);
    return ok;
}

static int dal_c__doctor(const dal_c_Cmd* cmd, const dal_c_Project* proj) {
    int failures = 0;
    const char* compiler = dal_c__effectiveCompiler(cmd, proj);
    bool compiler_ok = dal_c__toolResponds(compiler, "--version");
    bool make_ok = dal_c__toolResponds("make", "--version") || dal_c__toolResponds("gmake", "--version");
    bool ar_ok = dal_c__toolResponds(dal_c_tool_ar, "--version");
    char* dh_path = dal_c_Project_findDHInstallation(cmd);
    bool dh_ok = dh_path && path_isDir(dh_path);
    printf("DOCTOR:\n");
    printf("  compiler: %s [%s]\n", compiler, compiler_ok ? "ok" : "missing or unusable");
    printf("  make: %s\n", make_ok ? "ok" : "missing or unusable");
    printf("  archiver: %s [%s]\n", dal_c_tool_ar, ar_ok ? "ok" : "missing or unusable");
    printf("  dh: %s [%s]\n", dh_path ? dh_path : "(not found)", dh_ok ? "ok" : "missing");
    printf("  project: %s\n", proj && proj->root ? proj->root : "(not detected; explicit-file builds remain possible)");
    if (!compiler_ok) failures++;
    if (!make_ok) failures++;
    if (!ar_ok) failures++;
    if (!dh_ok) failures++;
    if (compiler_ok) {
        dal_c__showTarget(cmd, proj);
    }
    printf("RESULT: %s\n", failures == 0 ? "healthy" : "issues detected");
    free(dh_path);
    return failures == 0 ? 0 : 1;
}
