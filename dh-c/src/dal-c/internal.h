#ifndef dal_c_internal__included
#define dal_c_internal__included

#include "dal-c.h"
#include "dal-c-ext/ArrStr.h"
#include <stdarg.h>

/// === SHARED UTILITIES ===

void dal_c__printVerbose(const dal_c_Cmd* cmd, const char* fmt, ...);
void dal_c__printError(const char* fmt, ...);

/// === BUILD HELPERS ===

int dal_c__generateMakefile(
    const dal_c_Cmd* cmd,
    const dal_c_Project* proj,
    const dal_c_ProfileSpec* profile,
    ArrStr* sources,
    const char* output_name,
    const char* build_dir,
    dal_c_Target target_type
);

int dal_c__executeMake(const char* makefile_path);
int dal_c__executeMakeInDir(const char* directory);

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

#endif /* dal_c_internal__included */
