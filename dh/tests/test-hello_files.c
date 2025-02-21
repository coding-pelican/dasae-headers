#include "dh/main.h"
#include "dh/core.h"
#include "dh/meta.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/scope/common.h"

use_Ptr$(FILE);
use_Sli$(FILE);
use_Arr$(4, Ptr$FILE);

config_ErrSet(
    LoadFileErr,
    FailedOpenFile
);

use_Err$(Ptr$FILE);
Err$Ptr$FILE loadFile(StrConst filename) {
    reserveReturn(Err$Ptr$FILE);
    if_(let file = fopen(as$(const char*, filename.ptr), "r"), file != null) {
        return_ok(file);
    }
    return_err(LoadFileErr_FailedOpenFile());
}

use_Err$(Arr$4$Ptr$FILE);
Err$Arr$4$Ptr$FILE loadFiles(void) {
    scope_reserveReturn(Err$Arr$4$Ptr$FILE) {
        FILE* fp1 = fopen("hello1.txt", "r");
        if (fp1 == null) {
            return_err(LoadFileErr_FailedOpenFile());
        }
        errdefer(ignore fclose(fp1));

        let fp2 = try(loadFile(strL("hello2.txt")));
        errdefer(ignore fclose(fp2));

        let fp3 = try(loadFile(strL("hello3.txt")));
        errdefer(ignore fclose(fp3));

        let fp4 = try(loadFile(strL("hello4.txt")));
        errdefer(ignore fclose(fp4));

        return_ok({ fp1, fp2, fp3, fp4 });
    }
    scope_returnReserved;
}
