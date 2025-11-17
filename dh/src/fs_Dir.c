#include "dh/fs/Dir.h"
#include "dh/io/common.h"

#include <sys/stat.h>
#include <errno.h>

$inline_always
$static fn_((makeDir(S_const$u8 path))(i32)) {
    return mkdir(as$(const char*)(path.ptr) pp_if_(plat_windows)(
        pp_then_(pp_ignore)
    )(, 0755));
}

fn_((fs_Dir_create(S_const$u8 path))(E$void) $scope) {
    const bool exists = makeDir(path);
    if (exists) {
        // ignore if directory already exists
        if (errno != EEXIST) {
            return_err(fs_File_Err_OpenFailed());
        }
    }
    return_ok({});
} $unscoped_(fn);
