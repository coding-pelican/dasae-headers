#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/Str.h"

#include <stdio.h>

/* declarations =============================================================*/

use_Ptr$(FILE);
use_Sli$(Ptr$FILE);
use_Arr$(4, Ptr$FILE);

config_ErrSet(LoadFileErr,
    FailedOpenFile
);

use_Err$(Ptr$FILE);
static fn_(loadFile(Str_const filename), Err$Ptr$FILE) $must_check;
use_Err$(Arr$4$Ptr$FILE);
static fn_(loadFiles(void), Err$Arr$4$Ptr$FILE) $must_check;

/* definitions ==============================================================*/

fn_(loadFile(Str_const filename), Err$Ptr$FILE, $scope) {
    if_(let file = fopen(as$(const char*, filename.ptr), "r"),
        file != null) {
        return_ok(file);
    }
    return_err(LoadFileErr_FailedOpenFile());
} $unscoped;

fn_(loadFiles(void), Err$Arr$4$Ptr$FILE, $guard) {
    FILE* fp1 = fopen("hello1.txt", "r");
    if (fp1 == null) {
        return_err(LoadFileErr_FailedOpenFile());
    }
    errdefer_($ignore = fclose(fp1));

    let fp2 = try_(loadFile(Str_l("hello2.txt")));
    errdefer_($ignore = fclose(fp2));

    let fp3 = try_(loadFile(Str_l("hello3.txt")));
    errdefer_($ignore = fclose(fp3));

    let fp4 = try_(loadFile(Str_l("hello4.txt")));
    errdefer_($ignore = fclose(fp4));

    return_ok({ fp1, fp2, fp3, fp4 });
} $unguarded;

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    $ignore   = args;
    let files = try_(loadFiles());
    for_array (files, file) {
        $ignore = fclose(*file);
    }
    return_ok({});
} $unscoped;
