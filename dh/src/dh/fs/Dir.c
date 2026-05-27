#include "dh/fs/Dir.h"
#include "dh/fs/File.h"
#include "dh/fs/path.h"
#include "dh/mem/Alctr.h"
#include "dh/mem/common.h"
#include "private/share.h"

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif

#if plat_is_windows
$static fn_((fs_Dir__hasHandle(fs_Dir self))(bool)) {
    return self.handle != null && self.handle != INVALID_HANDLE_VALUE;
}

$static fn_((fs_Dir__basePath(fs_Dir self, S$u8 out))(E$S$u8) $scope) {
    if (!fs_Dir__hasHandle(self)) return_ok(S_slice((out)$r(0, 0)));
    let wrote = GetFinalPathNameByHandleA(self.handle, as$(LPSTR)(out.ptr), as$(DWORD)(out.len), 0);
    if (wrote == 0) return_err(E_cause$OpenFailedFS());
    if (as$(usize)(wrote) >= out.len) return_err(E_cause$FileTooBigFS());
    return_ok(fs__trimWinNtPrefix(S_slice((out)$r(0, as$(usize)(wrote)))));
} $unscoped(fn);

$static fn_((fs_Dir__resolvePath(fs_Dir self, S_const$u8 sub_path, S$u8 out))(E$S$u8) $scope) {
    if (fs_path_isAbs(sub_path)) {
        if (!fs__pathZ(sub_path, out.ptr, out.len)) return_err(E_cause$FileTooBigFS());
        return_ok(S_slice((out)$r(0, sub_path.len)));
    }

    var_(base_buf, A$$(fs__path_max, u8)) = A_zero();
    let base = try_(fs_Dir__basePath(self, A_ref$((S$u8)(base_buf))));
    if (base.len == 0) {
        if (!fs__pathZ(sub_path, out.ptr, out.len)) return_err(E_cause$FileTooBigFS());
        return_ok(S_slice((out)$r(0, sub_path.len)));
    }

    let joined = try_(fs_path_join2(base.as_const, sub_path, out));
    if (joined.len + 1 > out.len) return_err(E_cause$FileTooBigFS());
    *S_at((out)[joined.len]) = 0;
    return_ok(joined);
} $unscoped(fn);
#endif

$static fn_((fs_Dir__readAll(fs_File file, S$u8 buffer))(E$S$u8) $scope) {
    var_(total, usize) = 0;
    while (total < buffer.len) {
        let chunk = S_suffix((buffer)(total));
        let read = try_(fs_File_readPos(file, chunk, total));
        if (read == 0) break;
        total += read;
    }
    return_ok(S_slice((buffer)$r(0, total)));
} $unscoped(fn);

fn_((fs_Dir_create(S_const$u8 path))(E$void) $scope) {
#if plat_is_windows
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$FileTooBigFS());
    if (!CreateDirectoryA(as$(LPCSTR)(A_ptr(path_z)), null)) {
        let err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS) return_err(E_cause$WriteFailedFS());
    }
#elif plat_is_linux
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$FileTooBigFS());
    if (sys_call_linux_mkdirat(sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), fs_Dir_default_mode) != 0) return_err(E_cause$WriteFailedFS());
#else
    let_ignore = path;
    return_err(E_cause$UnsupportedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_Dir_close(fs_Dir* self))(void)) {
    if (self == null) return;
#if plat_is_windows
    if (fs_Dir__hasHandle(*self)) {
        let_ignore = CloseHandle(self->handle);
    }
    self->handle = INVALID_HANDLE_VALUE;
#elif plat_is_linux
    if (self->handle >= 0) {
        let_ignore = sys_call_linux_close(self->handle);
    }
    self->handle = -1;
#else
    self->handle = -1;
#endif
}

fn_((fs_Dir_rename(fs_Dir self, S_const$u8 old_sub_path, S_const$u8 new_sub_path))(E$void) $scope) {
#if plat_is_windows
    var_(old_path, A$$(fs__path_max, u8)) = A_zero();
    var_(new_path, A$$(fs__path_max, u8)) = A_zero();
    let old_resolved = try_(fs_Dir__resolvePath(self, old_sub_path, A_ref$((S$u8)(old_path))));
    let new_resolved = try_(fs_Dir__resolvePath(self, new_sub_path, A_ref$((S$u8)(new_path))));
    return fs_File_rename(old_resolved.as_const, new_resolved.as_const);
#elif plat_is_linux
    var_(old_path, A$$(fs__path_max, u8)) = A_zero();
    var_(new_path, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(old_sub_path, A_ptr(old_path), A_len(old_path))) return_err(E_cause$FileTooBigFS());
    if (!fs__pathZ(new_sub_path, A_ptr(new_path), A_len(new_path))) return_err(E_cause$FileTooBigFS());
    if (sys_call_linux_renameat(self.handle, as$(const char*)(A_ptr(old_path)), self.handle, as$(const char*)(A_ptr(new_path))) != 0) {
        return_err(E_cause$NotFoundFS());
    }
    return_ok({});
#else
    let_ignore = self;
    let_ignore = old_sub_path;
    let_ignore = new_sub_path;
    return_err(E_cause$UnsupportedFS());
#endif
} $unscoped(fn);

fn_((fs_Dir_makePath(fs_Dir self, S_const$u8 sub_path))(E$void) $scope) {
#if plat_is_windows || plat_is_linux
    var_(path, A$$(fs__path_max, u8)) = A_zero();
#endif
#if plat_is_windows
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
#elif plat_is_linux
    let_ignore = self;
    if (!fs__pathZ(sub_path, A_ptr(path), A_len(path))) return_err(E_cause$FileTooBigFS());
    let resolved = S_slice((A_ref$((S$u8)(path)))$r(0, sub_path.len));
#else
    let_ignore = self;
    let_ignore = sub_path;
    return_err(E_cause$UnsupportedFS());
#endif

#if plat_is_windows || plat_is_linux
    let seps = u8_l("/\\");
    var_(pos, usize) = 0;
    while (pos < resolved.len) {
        let rest = S_suffix((resolved.as_const)(pos));
        let seg_begin = orelse_((mem_findFirstNoneBytes(rest, seps))(break));
        pos += seg_begin;
        let seg_rest = S_suffix((resolved.as_const)(pos));
        let seg_end = expr_(usize $scope)(
            if_some((mem_findFirstAnyBytes(seg_rest, seps))(idx)) $break_(pos + idx);
                                          else $break_(resolved.len);) $unscoped(expr);
        let needs_restore = seg_end < resolved.len;
        let_(saved, u8) = needs_restore ? *S_at((resolved)[seg_end]) : u8_c(0);
        if (needs_restore) *S_at((resolved)[seg_end]) = 0;
        if (seg_end != 0) {
#if plat_is_windows
            if (!CreateDirectoryA(as$(LPCSTR)(resolved.ptr), null)) {
                let err = GetLastError();
                if (err != ERROR_ALREADY_EXISTS) return_err(E_cause$WriteFailedFS());
            }
#elif plat_is_linux
            let_ignore = sys_call_linux_mkdirat(sys_call_linux_AT_FDCWD, as$(const char*)(resolved.ptr), fs_Dir_default_mode);
#endif
        }
        if (needs_restore) *S_at((resolved)[seg_end]) = saved;
        pos = seg_end;
    }
    return_ok({});
#endif
} $unscoped(fn);

fn_((fs_Dir_realpath(fs_Dir self, S_const$u8 pathname, S$u8 out_buffer))(E$S$u8) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, pathname, A_ref$((S$u8)(path))));
    let len = GetFullPathNameA(as$(LPCSTR)(resolved.ptr), as$(DWORD)(out_buffer.len), as$(LPSTR)(out_buffer.ptr), null);
    if (len == 0) return_err(E_cause$NotFoundFS());
    if (as$(usize)(len) >= out_buffer.len) return_err(E_cause$FileTooBigFS());
    return_ok(S_slice((out_buffer)$r(0, as$(usize)(len))));
#else
    let_ignore = self;
    let_ignore = pathname;
    let_ignore = out_buffer;
    return_err(E_cause$UnsupportedFS());
#endif
} $unscoped(fn);

fn_((fs_Dir_realpathAlloc(fs_Dir self, S_const$u8 pathname, mem_Alctr allocator))(E$S$u8) $guard) {
    let buf = catch_((mem_Alctr_allocBytes($trace allocator, fs__path_max))(err, {
        let_ignore = err;
        return_err(E_cause$NoSpaceLeftFS());
    }));
    errdefer_($ignore, mem_Alctr_freeBytes($trace allocator, buf));
    let path = try_(fs_Dir_realpath(self, pathname, buf));
    return_ok(path);
} $unguarded(fn);

fn_((fs_Dir_makeDir(fs_Dir self, S_const$u8 sub_path))(E$void) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    return fs_Dir_create(resolved.as_const);
#elif plat_is_linux
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(sub_path, A_ptr(path), A_len(path))) return_err(E_cause$FileTooBigFS());
    if (sys_call_linux_mkdirat(self.handle, as$(const char*)(A_ptr(path)), fs_Dir_default_mode) != 0) return_err(E_cause$WriteFailedFS());
#else
    let_ignore = self;
    let_ignore = sub_path;
    return_err(E_cause$UnsupportedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_Dir_deleteDir(fs_Dir self, S_const$u8 sub_path))(E$void) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    if (!RemoveDirectoryA(as$(LPCSTR)(resolved.ptr))) return_err(E_cause$NotFoundFS());
    return_ok({});
#elif plat_is_linux
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(sub_path, A_ptr(path), A_len(path))) return_err(E_cause$FileTooBigFS());
    if (sys_call_linux_unlinkat(self.handle, as$(const char*)(A_ptr(path)), sys_call_linux_AT_REMOVEDIR) != 0) return_err(E_cause$NotFoundFS());
#else
    let_ignore = self;
    let_ignore = sub_path;
    return_err(E_cause$UnsupportedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_Dir_openDir(fs_Dir self, S_const$u8 sub_path, fs_File_OpenFlags flags))(E$fs_Dir) $scope) {
    let_ignore = flags;
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    let handle = CreateFileA(
        as$(LPCSTR)(resolved.ptr), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$OpenFailedFS());
    return_ok(fs_Dir_Handle_promote(handle));
#elif plat_is_linux
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(sub_path, A_ptr(path), A_len(path))) return_err(E_cause$FileTooBigFS());
    let handle = sys_call_linux_openat(self.handle, as$(const char*)(A_ptr(path)), sys_call_linux_O_RDONLY | sys_call_linux_O_DIRECTORY, 0);
    if (sys_call_linux_syscall_isErr(handle)) return_err(E_cause$OpenFailedFS());
    return_ok(fs_Dir_Handle_promote(as$(fs_Dir_Handle)(handle)));
#else
    let_ignore = self;
    let_ignore = sub_path;
    return_err(E_cause$UnsupportedFS());
#endif
} $unscoped(fn);

fn_((fs_Dir_createFile(fs_Dir self, S_const$u8 sub_path, fs_File_CreateFlags flags))(E$fs_File) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    return fs_File_create(resolved.as_const, flags);
#elif plat_is_linux
    let_ignore = self;
    return fs_File_create(sub_path, flags);
#else
    let_ignore = self;
    let_ignore = sub_path;
    let_ignore = flags;
    return_err(E_cause$UnsupportedFS());
#endif
} $unscoped(fn);

fn_((fs_Dir_openFile(fs_Dir self, S_const$u8 sub_path, fs_File_OpenFlags flags))(E$fs_File) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    return fs_File_open(resolved.as_const, flags);
#elif plat_is_linux
    let_ignore = self;
    return fs_File_open(sub_path, flags);
#else
    let_ignore = self;
    let_ignore = sub_path;
    let_ignore = flags;
    return_err(E_cause$UnsupportedFS());
#endif
} $unscoped(fn);

fn_((fs_Dir_deleteFile(fs_Dir self, S_const$u8 sub_path))(E$void) $scope) {
#if plat_is_windows
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    let resolved = try_(fs_Dir__resolvePath(self, sub_path, A_ref$((S$u8)(path))));
    return fs_File_delete(resolved.as_const);
#elif plat_is_linux
    var_(path, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(sub_path, A_ptr(path), A_len(path))) return_err(E_cause$FileTooBigFS());
    if (sys_call_linux_unlinkat(self.handle, as$(const char*)(A_ptr(path)), 0) != 0) return_err(E_cause$NotFoundFS());
#else
    let_ignore = self;
    let_ignore = sub_path;
    return_err(E_cause$UnsupportedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_Dir_readFile(fs_Dir self, S_const$u8 file_path, S$u8 buffer))(E$S$u8) $guard) {
    var flags = fs_File_OpenFlags_default;
    flags.mode = fs_OpenMode_read_only;
    let file = try_(fs_Dir_openFile(self, file_path, flags));
    defer_(fs_File_close(file));
    let read = try_(fs_Dir__readAll(file, buffer));
    return_ok(read);
} $unguarded(fn);

fn_((fs_Dir_readFileAlloc(fs_Dir self, S_const$u8 file_path, mem_Alctr allocator, usize max_bytes))(E$S$u8) $guard) {
    var flags = fs_File_OpenFlags_default;
    flags.mode = fs_OpenMode_read_only;
    let file = try_(fs_Dir_openFile(self, file_path, flags));
    defer_(fs_File_close(file));

    let stat = try_(fs_File_stat(file));
    if (stat.size > as$(u64)(usize_limit)) return_err(E_cause$FileTooBigFS());
    var len = as$(usize)(stat.size);
    if (max_bytes != 0 && max_bytes < len) len = max_bytes;

    let buf = catch_((mem_Alctr_allocBytes($trace allocator, len))(err, {
        let_ignore = err;
        return_err(E_cause$NoSpaceLeftFS());
    }));
    errdefer_($ignore, mem_Alctr_freeBytes($trace allocator, buf));
    let read = try_(fs_Dir__readAll(file, buf));
    return_ok(read);
} $unguarded(fn);
