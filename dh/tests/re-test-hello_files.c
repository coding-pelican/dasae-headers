#if UNUSED_CODE
#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/Str.h"

#include <stdio.h>

/* declarations =============================================================*/

use_P$(FILE);
use_S$(P$FILE);
use_A$(4, P$FILE);

config_ErrSet(LoadFileErr,
    FailedOpenFile
);

use_E$(P$FILE);
static fn_(loadFile(S_const$u8 filename), E$P$FILE) $must_check;
use_E$(A$4$P$FILE);
static fn_(loadFiles(void), E$A$4$P$FILE) $must_check;

/* definitions ==============================================================*/

fn_(loadFile(S_const$u8 filename), E$P$FILE $scope) {
    if_(let file = fopen(as$(const char*, filename.ptr), "r"),
        file != null) {
        return_ok(file);
    }
    return_err(LoadFileE_FailedOpenFile());
}
$unscoped;

fn_(loadFiles(void), E$A$4$P$FILE $guard) {
    FILE* fp1 = fopen("hello1.txt", "r");
    if (fp1 == null) {
        return_err(LoadFileE_FailedOpenFile());
    }
    errdefer_($ignore, let_ignore = fclose(fp1));

    let fp2 = try_(loadFile(u8_l("hello2.txt")));
    errdefer_($ignore, let_ignore = fclose(fp2));

    let fp3 = try_(loadFile(u8_l("hello3.txt")));
    errdefer_($ignore, let_ignore = fclose(fp3));

    let fp4 = try_(loadFile(u8_l("hello4.txt")));
    errdefer_($ignore, let_ignore = fclose(fp4));

    return_ok({ fp1, fp2, fp3, fp4 });
}
$unguarded;

fn_(dh_main(S$S_const$u8 args), E$void $scope) {
    let_ignore = args;
    let files  = try_(loadFiles());
    for_array(files, file) {
        let_ignore = fclose(*file);
    }
    return_ok({});
}
$unscoped;

#endif /* UNUSED_CODE */
