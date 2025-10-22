#include "dh/fs/Dir.h"
#include "dh/io/common.h"

#include <sys/stat.h>
#include <errno.h>

$static $inline_always
fn_((makeDir(Sli_const$u8 path))(i32)) {
    return mkdir(as$((const char*)(path.ptr)) pp_if_(bti_plat_windows)(
        pp_than_(pp_ignore)
    )(, 0755));
}

fn_((fs_Dir_create(Sli_const$u8 path))(Err$void) $scope) {
    const bool exists = makeDir(path);
    if (exists) {
        // ignore if directory already exists
        if (errno != EEXIST) {
            return_err(fs_FileErr_OpenFailed());
        }
    }
    return_ok({});
} $unscoped_(fn);
