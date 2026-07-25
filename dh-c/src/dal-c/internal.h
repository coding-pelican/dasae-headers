#pragma once
#ifndef dal_c_internal__included
#define dal_c_internal__included

#include "dal-c.h"
#include "dal-c-ext/ArrStr.h"

#if defined(__clang__) || defined(__GNUC__)
#define dal_c__printf_format(_fmt_idx, _args_idx) __attribute__((format(printf, _fmt_idx, _args_idx)))
#define dal_c__noinline __attribute__((noinline))
#if defined(__clang__)
#define dal_c__optnone __attribute__((optnone))
#else
#define dal_c__optnone
#endif
#else
#define dal_c__printf_format(_fmt_idx, _args_idx)
#define dal_c__noinline
#define dal_c__optnone
#endif

/// === SHARED UTILITIES ===

typedef struct dal_c_CommandIntent {
    dal_c_CmdAction action;
    const char* target_path;
    const char* target_root_name_hint;
    const char* output_path;
    const char* output_ext;
    const char* run_args;
    dal_c_Linking linking;
    bool build_all;
    bool as_library;
    bool self_boundary;
    bool recursive;
    bool debug;
    bool dsl_first;
    bool cache_only;
    bool target_path_is_explicit_file;
} dal_c_CommandIntent;

typedef struct dal_c_TargetRequest {
    const dal_c_TargetRoot* root;
    const char* raw_target_path;
    char* resolved_path;
    char* relative_path;
    dal_c_Target kind;
    dal_c_TargetSelection selection;
    bool link_project;
    bool resolved_is_dir;
} dal_c_TargetRequest;

typedef struct dal_c_ProjectLock {
    char* path;
    bool acquired;
    bool waited;
} dal_c_ProjectLock;

typedef enum dal_c_CmdPhase {
    dal_c_CmdPhase_project_lib_build,
    dal_c_CmdPhase_dependency_build,
    dal_c_CmdPhase_dh_build,
    dal_c_CmdPhase_self_build,
    dal_c_CmdPhase_test_build,
    dal_c_CmdPhase_run_build,
    dal_c_CmdPhase_test_run,
    dal_c_CmdPhase_run_exec,
    dal_c_CmdPhase_syntax,
    dal_c_CmdPhase_tidy,
    dal_c_CmdPhase_format,
    dal_c_CmdPhase_compile_db,
    dal_c_CmdPhase_clean,
} dal_c_CmdPhase;

double dal_c__phaseNowSeconds(void);
void dal_c__phaseRecord(dal_c_CmdPhase phase, double elapsed_seconds);

void dal_c_Cmd_normalizeIntent(const dal_c_Cmd* cmd, dal_c_CommandIntent* out);
void dal_c_TargetRequest_cleanup(dal_c_TargetRequest* request);
const dal_c_TargetRoot* dal_c_Project_findTargetRootByName(const dal_c_Project* proj, const char* name);
const dal_c_TargetRoot* dal_c_Project_findTargetRootByPath(const dal_c_Project* proj, const char* path);
bool dal_c_TargetRequest_resolve(const dal_c_Project* proj, const dal_c_CommandIntent* intent, dal_c_TargetRequest* out);
void dal_c_BuildDefaults_cleanup(dal_c_BuildDefaults* defaults);
void dal_c_BuildDefaults_merge(dal_c_BuildDefaults* dst, const dal_c_BuildDefaults* src);
bool dal_c_BuildDefaults_applyDHFile(dal_c_BuildDefaults* dst, const char* path);
void dal_c_VersionSpec_cleanup(dal_c_VersionSpec* version);
void dal_c_VersionSpec_merge(dal_c_VersionSpec* dst, const dal_c_VersionSpec* src);
bool dal_c_VersionSpec_parseNamespace(dal_c_VersionSpec* dst, const char* value);
bool dal_c_VersionSpec_parseCore(dal_c_VersionSpec* dst, const char* value);
bool dal_c_VersionSpec_parsePrefix(dal_c_VersionSpec* dst, const char* value);
bool dal_c_VersionSpec_parseSuffix(dal_c_VersionSpec* dst, const char* value);
bool dal_c_VersionSpec_parseBuild(dal_c_VersionSpec* dst, const char* value);
bool dal_c_Project_readDependencyLock(const dal_c_Project* proj, const dal_c_Lib* lib, char** revision_out, char** reason_out);
bool dal_c_Project_dependencyCheckoutMatchesLock(const dal_c_Project* proj, const dal_c_Lib* lib, char** reason_out);

void dal_c__printVerbose(const dal_c_Cmd* cmd, const char* fmt, ...) dal_c__printf_format(2, 3);
void dal_c__printError(const char* fmt, ...) dal_c__printf_format(1, 2);

/// === BUILD HELPERS ===

char* dal_c__resolveTargetDirName(const dal_c_CompilerOpts* opts);
char* dal_c__makeBuildProfileDir(const dal_c_Project* proj, const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile);
bool dal_c__writeArtifactManifest(const char* profile_dir, const dal_c_Cmd* cmd, const dal_c_ProfileSpec* profile, dal_c_Target target_type, const char* target_path);
bool dal_c__prebuiltManifestCompatible(const char* prebuilt_profile_dir, const dal_c_CompilerOpts* opts, const dal_c_ProfileSpec* profile, bool lto_enabled, char** reason_out);
char* dal_c__takeLastContractDiff(void);


#define dal_c_generateMakefile_success 0
#define dal_c_generateMakefile_upToDate 2


int dal_c__generateMakefile(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type
);
int dal_c__parallelJobCount(const dal_c_Cmd* cmd);
void dal_c__appendCompileDbArguments(
    ArrStr* argv,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    const char* src
);
void dal_c__appendSyntaxArguments(
    ArrStr* argv,
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    const char* src,
    dal_c_Target target_type
);
int dal_c__writeCompileDb(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* output_path
);
char* dal_c__resolveOutputPath(const dal_c_Project* proj, const dal_c_Cmd* cmd, const char* build_dir, const char* output_name, dal_c_Target target_type);
char* dal_c__makePlanFilePath(const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const dal_c_Cmd* cmd, const char* target_path, dal_c_Target target_type);
char* dal_c__makeLtoStaticLibraryPath(const char* native_path);
char* dal_c__makeSharedImportLibraryPath(const char* shared_path);

int dal_c__executeMake(const dal_c_Cmd* cmd, const char* makefile_path);
int dal_c__executeMakeInDir(const dal_c_Cmd* cmd, const char* directory);
bool dal_c__writeDepsPreludeHeader(const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
bool dal_c__usesDHLibrary(const dal_c_Project* proj, const dal_c_CompilerOpts* opts);
int dal_c__buildDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__cleanDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__testDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__buildSelf(const dal_c_Cmd* cmd);
int dal_c__cleanSelf(const dal_c_Cmd* cmd);
int dal_c__queryToolchain(const dal_c_Cmd* cmd);
char* dal_c__cacheBaseDir(const dal_c_Project* proj);

/// === SOURCE COLLECTION ===

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_path);
ArrStr* dal_c__collectDirectoryFiles(const dal_c_Project* proj, const char* dir_name);
ArrStr* dal_c__collectPathSourcesCached(const dal_c_Project* proj, const char* path, bool resolved_is_dir, bool skip_auto_paths);
bool dal_c__shouldSkipAutoSourcePath(const char* path);
bool dal_c__hasTestRegistration(const char* path);

/// === EXECUTABLE RUNNING ===

int dal_c__runExecutable(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__runDebugger(const dal_c_Cmd* cmd, const dal_c_Project* proj);

/// === LIBRARY BUILDING ===

int dal_c__buildSingleLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_Lib* lib);
int dal_c__buildHeaderOnlyLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* output_name);
bool dal_c__isHeaderOnlyBuild(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);

/// === PLATFORM ===

bool dal_c__platformIsWindows(void);
bool dal_c__projectLockAcquire(const dal_c_Project* proj, dal_c_ProjectLock* lock);
bool dal_c__projectLockAcquireAt(const char* root, dal_c_ProjectLock* lock);
void dal_c__projectLockRelease(dal_c_ProjectLock* lock);

/// === OPTION UTILITIES ===

void dal_c_CompilerOpts_cleanup(dal_c_CompilerOpts* opts);
void dal_c_CompilerOpts_merge(dal_c_CompilerOpts* dst, const dal_c_CompilerOpts* src);
bool dal_c_CompilerOpts_applyDHFile(dal_c_CompilerOpts* dst, const char* path);
ArrStr* dal_c__collectDescendantProjects(const dal_c_Project* proj);

#endif /* dal_c_internal__included */
