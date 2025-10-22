#include "dh/fs/File.h"
#include "dh/io/Writer.h"
#include "dh/io/common.h"

#if bti_plat_windows
#include "dh/os/windows.h"
static fn_((windows_CloseHandle(HANDLE handle))(void)) {
    CloseHandle(handle);
}
static fn_((windows_WriteFile(HANDLE handle, Sli_const$u8 bytes))(Err$usize) $scope) {
    DWORD bytes_written = 0;
    if (!WriteFile(handle, bytes.ptr, bytes.len, &bytes_written, null)) {
        return_err(fs_FileErr_WriteFailed());
    }
    return_ok(as$((usize)(bytes_written)));
}
$unscoped;
#else
#include <unistd.h>
static fn_((linux_close(posix_fd_t handle))(void)) {
    close(handle);
}
static fn_((linux_write(posix_fd_t handle, Sli_const$u8 bytes))(Err$usize) $scope) {
    ssize_t bytes_written = 0;
    if (write(handle, bytes.ptr, bytes.len) == -1) {
        return_err(fs_FileErr_WriteFailed());
    }
    return_ok(as$((usize)(bytes_written)));
}
$unscoped;
#endif

fn_((fs_File_close(fs_File file))(void)) {
    return pp_if_(bti_plat_windows)(
        pp_than_(windows_CloseHandle),
        pp_else_(linux_close))(file.handle);
}

typedef union Writer {
    io_Writer base;
    struct {
        fs_File ctx;
        fn_(((*const write)(const anyptr ctx, Sli_const$u8 bytes))(Err$usize)) $must_check;
    };
} Writer;
static fn_((Writer_init(fs_File file))(Writer));
$must_check
$static fn_((Writer_VT_write(const anyptr ctx, Sli_const$u8 bytes))(Err$usize)) $must_check;

fn_((fs_File_writer(fs_File file))(io_Writer)) {
    return Writer_init(file).base;
}

static fn_((Writer_init(fs_File file))(Writer)) {
    return (Writer){ .ctx = file, .write = Writer_VT_write };
}
static fn_((Writer_VT_write(const anyptr ctx, Sli_const$u8 bytes))(Err$usize)) {
    let self = as$((FieldTypeOf(Writer, ctx)*)(&ctx));
    return pp_if_(bti_plat_windows)(
        pp_than_(windows_WriteFile),
        pp_else_(linux_write))(self->handle, bytes);
}
