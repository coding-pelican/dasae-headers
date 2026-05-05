#ifndef dal_c_internal__included
#define dal_c_internal__included

#include "dal-c.h"
#include "dal-c-ext/ArrStr.h"

/// === SHARED UTILITIES ===

typedef struct dal_c_CommandIntent {
    dal_c_CmdAction action;
    const char* target_file;
    const char* output_path;
    const char* run_args;
    const char* explicit_lib;
    dal_c_SampleDir sample_dir;
    dal_c_Linking linking;
    bool build_all;
    bool recursive;
    bool debug;
    bool dsl_first;
    bool cache_only;
} dal_c_CommandIntent;

void dal_c_Cmd_normalizeIntent(const dal_c_Cmd* cmd, dal_c_CommandIntent* out);

void dal_c__printVerbose(const dal_c_Cmd* cmd, const char* fmt, ...);
void dal_c__printError(const char* fmt, ...);

/// === BUILD HELPERS ===

int dal_c__generateMakefile(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* target_path,
    const char* build_dir,
    dal_c_Target target_type
);
char* dal_c__resolveOutputPath(const dal_c_Cmd* cmd, const char* build_dir, const char* output_name, dal_c_Target target_type);
char* dal_c__makePlanFilePath(const dal_c_Project* proj, const dal_c_ProfileSpec* profile, const dal_c_Cmd* cmd, const char* target_path, dal_c_Target target_type);

int dal_c__executeMake(const char* makefile_path);
int dal_c__executeMakeInDir(const char* directory);
int dal_c__buildDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__cleanDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__testDSL(const dal_c_Cmd* cmd, const dal_c_Project* proj);

/// === SOURCE COLLECTION ===

ArrStr* dal_c__collectSourceFiles(const dal_c_Project* proj, const char* target_file);
ArrStr* dal_c__collectDirectoryFiles(const dal_c_Project* proj, const char* dir_name);

/// === EXECUTABLE RUNNING ===

int dal_c__runExecutable(const dal_c_Cmd* cmd, const dal_c_Project* proj);
int dal_c__runDebugger(const dal_c_Cmd* cmd, const dal_c_Project* proj);

/// === LIBRARY BUILDING ===

int dal_c__buildSingleLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const dal_c_Lib* lib);
int dal_c__buildHeaderOnlyLibrary(const dal_c_Cmd* cmd, const dal_c_Project* proj, const char* output_name);
bool dal_c__isHeaderOnlyBuild(const dal_c_Cmd* cmd, const dal_c_Project* proj, ArrStr* sources);

/// === PLATFORM ===

bool dal_c__platformIsWindows(void);

/// === OPTION UTILITIES ===

void dal_c_CompilerOpts_cleanup(dal_c_CompilerOpts* opts);
void dal_c_CompilerOpts_merge(dal_c_CompilerOpts* dst, const dal_c_CompilerOpts* src);
bool dal_c_CompilerOpts_applyDhFile(dal_c_CompilerOpts* dst, const char* path);
ArrStr* dal_c__collectDescendantProjects(const dal_c_Project* proj);

#endif /* dal_c_internal__included */
