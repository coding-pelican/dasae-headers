#include "dh/fs/dir.h"

#include <sys/stat.h>
#include <errno.h>

force_inline i32 makeDir(Str_const path) {
#if bti_plat_windows
    return mkdir(as$(const char*, path.ptr));
#else
    return mkdir(as$(const char*, path.ptr), 0755);
#endif
}

Err$void fs_dir_create(Str_const path) {
    reserveReturn(Err$void);
    const bool exists = makeDir(path);
    if (exists) {
        // $ignore if directory already exists
        if (errno != EEXIST) {
            // return_err(io_FileErr_OpenFailed());
        }
    }
    return_void();
}
