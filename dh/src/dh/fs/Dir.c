#include "dh/fs/Dir.h"
#include "dh/io/common.h"

#include <sys/stat.h>
#include <errno.h>

#if plat_is_windows
#include "dh/os/windows/handle.h"
#else
#include <unistd.h>
#endif

$attr($inline_always)
$static fn_((makeDir(S_const$u8 path))(i32)) {
    return mkdir(as$(const char*)(path.ptr)pp_if_(plat_is_windows)(
        pp_then_(pp_ignore))(, 0755));
}

fn_((fs_Dir_create(S_const$u8 path))(E$void) $scope) {
    const bool exists = makeDir(path);
    if (exists) {
        // ignore if directory already exists
        if (errno != EEXIST) {
            return_err(E_cause$FSOpenFailed());
        }
    }
    return_ok({});
} $unscoped(fn);

fn_((fs_Dir_close(fs_Dir* self))(void)) {
    if (self == null) return;
#if plat_is_windows
    if (self->handle != null && self->handle != INVALID_HANDLE_VALUE) {
        let_ignore = CloseHandle(self->handle);
    }
    self->handle = INVALID_HANDLE_VALUE;
#else
    if (self->handle >= 0) {
        let_ignore = close(self->handle);
    }
    self->handle = -1;
#endif
}
