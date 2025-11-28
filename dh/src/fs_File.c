#include "dh/fs/File.h"

#if plat_is_windows
#include "dh/os/windows.h"
$static fn_((windows_CloseHandle(HANDLE handle))(void)) {
    CloseHandle(handle);
}
$static fn_((windows_ReadFile(HANDLE handle, S$u8 buf))(E$usize) $scope) {
    if_(DWORD bytes_read = 0, !ReadFile(handle, buf.ptr, buf.len, &bytes_read, null)) {
        if_(let err = GetLastError(), err != ERROR_HANDLE_EOF && err != ERROR_BROKEN_PIPE) {
            return_err(fs_File_Err_ReadFailed());
        } else {
            return_ok(0); // EOF
        }
        claim_unreachable;
    } else {
        return_ok(as$(usize)(bytes_read));
    }
    claim_unreachable;
} $unscoped_(fn);
$static fn_((windows_WriteFile(HANDLE handle, S_const$u8 bytes))(E$usize) $scope) {
    if_(DWORD bytes_written = 0, !WriteFile(handle, bytes.ptr, bytes.len, &bytes_written, null)) {
        return_err(fs_File_Err_WriteFailed());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
    claim_unreachable;
} $unscoped_(fn);
#else /* plat_is_posix */
#include <unistd.h>
$static fn_((posix_close(posix_fd_t handle))(void)) {
    close(handle);
}
$static fn_((posix_read(posix_fd_t handle, S$u8 buf))(E$usize) $scope) {
    if_(let bytes_read = read(handle, buf.ptr, buf.len), bytes_read == -1) {
        return_err(fs_File_Err_ReadFailed());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
    claim_unreachable;
} $unscoped_(fn);
$static fn_((posix_write(posix_fd_t handle, S_const$u8 bytes))(E$usize) $scope) {
    if_(let bytes_written = write(handle, bytes.ptr, bytes.len), bytes_written == -1) {
        return_err(fs_File_Err_WriteFailed());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
    claim_unreachable;
} $unscoped_(fn);
#endif

fn_((fs_File_close(fs_File file))(void)) {
    return pp_if_(pp_Tok_eq(plat_type, plat_type_windows))(
        pp_then_(windows_CloseHandle),
        pp_else_(/* plat_is_posix */ posix_close)
    )(file.handle);
}

typedef union Reader {
    io_Reader base;
    struct {
        fs_File ctx;
        fn_(((*const read)(const P$raw ctx, S$u8 buf))(E$usize)) $must_check;
    };
} Reader;

$static fn_((Reader_VT_read(const P$raw ctx, S$u8 buf))(E$usize)) {
    let self = as$(FieldType$(Reader, ctx)*)(&ctx);
    return pp_if_(plat_is_windows)(
        pp_then_(windows_ReadFile),
        pp_else_(/* plat_is_posix */ posix_read)
    )(self->handle, buf);
}

$static fn_((Reader_init(fs_File file))(Reader)) {
    return (Reader){ .ctx = file, .read = Reader_VT_read };
}

fn_((fs_File_reader(fs_File file))(io_Reader)) {
    return Reader_init(file).base;
}

typedef union Writer {
    io_Writer base;
    struct {
        fs_File ctx;
        fn_(((*const write)(const P$raw ctx, S_const$u8 bytes))(E$usize)) $must_check;
    };
} Writer;

$static fn_((Writer_VT_write(const P$raw ctx, S_const$u8 bytes))(E$usize)) {
    let self = as$(FieldType$(Writer, ctx)*)(&ctx);
    return pp_if_(plat_is_windows)(
        pp_then_(windows_WriteFile),
        pp_else_(/* plat_is_posix */ posix_write)
    )(self->handle, bytes);
}

$static fn_((Writer_init(fs_File file))(Writer)) {
    return (Writer){ .ctx = file, .write = Writer_VT_write };
}

fn_((fs_File_writer(fs_File file))(io_Writer)) {
    return Writer_init(file).base;
}
