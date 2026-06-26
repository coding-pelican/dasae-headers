#include "dh/fs/File.h"
#include "dh/mem/common.h"
#include "../private/share.h"

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/console.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif

#if plat_is_windows
$static fn_((fs__windowsOpenAccess(fs_File_OpenFlags flags))(DWORD)) {
    var_(access, DWORD) = 0;
    if (fs_OpenFlags_isRead(flags)) access |= GENERIC_READ;
    if (fs_OpenFlags_isWrite(flags)) access |= GENERIC_WRITE;
    return access;
}

$static fn_((fs__windowsCreateAccess(fs_File_CreateFlags flags))(DWORD)) {
    return GENERIC_WRITE | (flags.read ? GENERIC_READ : 0);
}

$static fn_((fs__windowsTime(FILETIME ft))(time_Real_Inst)) {
    var_(raw, ULARGE_INTEGER) = {};
    raw.LowPart = ft.dwLowDateTime;
    raw.HighPart = ft.dwHighDateTime;
    let secs = raw.QuadPart / 10000000;
    return time_Real_Inst_fromUnixEpoch(secs < 11644473600 ? 0 : secs - 11644473600);
}

$static fn_((fs__windowsKind(DWORD attrs))(fs_Kind)) {
    if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0) return fs_Kind_directory;
    return fs_Kind_file;
}
#elif plat_is_linux
$static fn_((fs__linuxKind(sys_call_linux_mode_t mode))(fs_Kind)) {
    switch (mode & sys_call_linux_S_IFMT) {
    case sys_call_linux_S_IFREG: return fs_Kind_file;
    case sys_call_linux_S_IFDIR: return fs_Kind_directory;
    case sys_call_linux_S_IFCHR: return fs_Kind_character_device;
    case sys_call_linux_S_IFBLK: return fs_Kind_block_device;
    case sys_call_linux_S_IFIFO: return fs_Kind_named_pipe;
    case sys_call_linux_S_IFLNK: return fs_Kind_sym_link;
    case sys_call_linux_S_IFSOCK: return fs_Kind_unix_domain_socket;
    default: return fs_Kind_unknown;
    }
}
#endif

fn_((fs_File_open(S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File) $scope) {
    if (flags.nonblocking && plat_is_windows) return_err(E_cause$fs_Unsupported());
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$fs_FileTooBig());
#if plat_is_windows
    let handle = CreateFileA(
        as$(LPCSTR)(A_ptr(path_z)), fs__windowsOpenAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$fs_OpenFailed());
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#elif plat_is_linux
    var_(open_flags, sys_call_linux_word) = 0;
    if (flags.mode == fs_OpenMode_read_only) open_flags |= sys_call_linux_O_RDONLY;
    if (flags.mode == fs_OpenMode_write_only) open_flags |= sys_call_linux_O_WRONLY;
    if (flags.mode == fs_OpenMode_read_write) open_flags |= sys_call_linux_O_RDWR;
    if (flags.nonblocking) open_flags |= sys_call_linux_O_NONBLOCK;
    let handle = sys_call_linux_openat(sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), open_flags, 0);
    if (sys_call_linux_syscall_isErr(handle)) {
        return_err(E_cause$fs_OpenFailed());
    } else {
        return_ok(fs_File_Handle_promote(as$(fs_File_Handle)(handle), (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#else
    let_ignore = path_z;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_create(S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File) $scope) {
    if (flags.nonblocking && plat_is_windows) return_err(E_cause$fs_Unsupported());
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$fs_FileTooBig());
#if plat_is_windows
    let disposition = flags.exclusive
                        ? CREATE_NEW
                        : (flags.truncate ? CREATE_ALWAYS : OPEN_ALWAYS);
    let handle = CreateFileA(
        as$(LPCSTR)(A_ptr(path_z)), fs__windowsCreateAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null,
        as$(DWORD)(disposition), FILE_ATTRIBUTE_NORMAL, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$fs_OpenFailed());
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#elif plat_is_linux
    var_(open_flags, sys_call_linux_word) = sys_call_linux_O_CREAT | sys_call_linux_O_WRONLY;
    if (flags.read) open_flags = sys_call_linux_O_CREAT | sys_call_linux_O_RDWR;
    if (flags.truncate) open_flags |= sys_call_linux_O_TRUNC;
    if (flags.exclusive) open_flags |= sys_call_linux_O_EXCL;
    if (flags.nonblocking) open_flags |= sys_call_linux_O_NONBLOCK;
    let handle = sys_call_linux_openat(sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), open_flags, flags.mode);
    if (sys_call_linux_syscall_isErr(handle)) {
        return_err(E_cause$fs_OpenFailed());
    } else {
        return_ok(fs_File_Handle_promote(as$(fs_File_Handle)(handle), (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#else
    let_ignore = path_z;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_delete(S_const$u8 path))(E$void) $scope) {
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$fs_FileTooBig());
#if plat_is_windows
    if (!DeleteFileA(as$(LPCSTR)(A_ptr(path_z)))) return_err(E_cause$fs_NotFound());
#elif plat_is_linux
    if (sys_call_linux_unlinkat(sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), 0) != 0) return_err(E_cause$fs_NotFound());
#else
    let_ignore = path_z;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_rename(S_const$u8 old_path, S_const$u8 new_path))(E$void) $scope) {
    var_(old_path_z, A$$(fs__path_max, u8)) = A_zero();
    var_(new_path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(old_path, A_ptr(old_path_z), A_len(old_path_z))) return_err(E_cause$fs_FileTooBig());
    if (!fs__pathZ(new_path, A_ptr(new_path_z), A_len(new_path_z))) return_err(E_cause$fs_FileTooBig());
#if plat_is_windows
    if (!MoveFileExA(
            as$(LPCSTR)(A_ptr(old_path_z)),
            as$(LPCSTR)(A_ptr(new_path_z)), MOVEFILE_REPLACE_EXISTING)) return_err(E_cause$fs_WriteFailed());
#elif plat_is_linux
    if (sys_call_linux_renameat(sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(old_path_z)), sys_call_linux_AT_FDCWD, as$(const char*)(A_ptr(new_path_z))) != 0) {
        return_err(E_cause$fs_NotFound());
    }
#else
    let_ignore = old_path_z;
    let_ignore = new_path_z;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_close(fs_File self))(void)) {
#if plat_is_windows
    CloseHandle(self.handle);
#elif plat_is_linux
    let_ignore = sys_call_linux_close(self.handle);
#else
    let_ignore = self;
#endif
}

fn_((fs_File_stat(fs_File self))(fs_E$fs_File_Stat) $scope) {
#if plat_is_windows
    var_(info, BY_HANDLE_FILE_INFORMATION) = {};
    if (!GetFileInformationByHandle(self.handle, &info)) return_err(E_cause$fs_ReadFailed());
    var_(size, ULARGE_INTEGER) = {};
    size.LowPart = info.nFileSizeLow;
    size.HighPart = info.nFileSizeHigh;
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(((u64)info.nFileIndexHigh << 32) | info.nFileIndexLow),
        .nlink = as$(fs_NLink)(info.nNumberOfLinks),
        .size = as$(u64)(size.QuadPart),
        .permissions = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0 ? 0444 : 0666,
        .kind = fs__windowsKind(info.dwFileAttributes),
        .atime = fs__windowsTime(info.ftLastAccessTime),
        .mtime = fs__windowsTime(info.ftLastWriteTime),
        .ctime = fs__windowsTime(info.ftCreationTime),
        .block_size = 4096,
    });
#elif plat_is_linux
    var_(st, sys_call_linux_statx) = {};
    if (sys_call_linux_statx_get(self.handle, "", sys_call_linux_AT_EMPTY_PATH, sys_call_linux_STATX_BASIC_STATS, &st) != 0) return_err(E_cause$fs_ReadFailed());
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(st.stx_ino),
        .nlink = as$(fs_NLink)(st.stx_nlink),
        .size = as$(u64)(st.stx_size),
        .permissions = as$(fs_Mode)(st.stx_mode),
        .kind = fs__linuxKind(st.stx_mode),
        .atime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.stx_atime.tv_sec)),
        .mtime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.stx_mtime.tv_sec)),
        .ctime = time_Real_Inst_fromUnixEpoch(as$(u64)(st.stx_ctime.tv_sec)),
        .block_size = as$(fs_BlockSize)(st.stx_blksize),
    });
#else
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_File_sync(fs_File self))(E$void) $scope) {
#if plat_is_windows
    if (!FlushFileBuffers(self.handle)) return_err(E_cause$fs_WriteFailed());
#elif plat_is_linux
    if (sys_call_linux_fsync(self.handle) != 0) return_err(E_cause$fs_WriteFailed());
#else
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_isTTY(fs_File self))(E$bool) $scope) {
#if plat_is_windows
    var_(mode, DWORD) = 0;
    return_ok(GetConsoleMode(self.handle, &mode));
#elif plat_is_linux
    var_(termios_buf, A$$(128, u8)) = A_zero();
    return_ok(sys_call_linux_ioctl(self.handle, sys_call_linux_TCGETS, A_ptr(termios_buf)) == 0);
#else
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_File_setLen(fs_File self, u64 new_len))(E$void) $scope) {
#if plat_is_windows
    var_(pos, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(new_len) };
    if (!SetFilePointerEx(self.handle, pos, null, FILE_BEGIN)) return_err(E_cause$fs_WriteFailed());
    if (!SetEndOfFile(self.handle)) return_err(E_cause$fs_WriteFailed());
#elif plat_is_linux
    if (sys_call_linux_ftruncate(self.handle, new_len) != 0) return_err(E_cause$fs_WriteFailed());
#else
    let_ignore = self;
    let_ignore = new_len;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_setPerms(fs_File self, fs_File_Mode perms))(E$void) $scope) {
#if plat_is_windows
    let_ignore = self;
    let_ignore = perms;
    return_err(E_cause$fs_Unsupported());
#elif plat_is_linux
    if (sys_call_linux_fchmod(self.handle, as$(sys_call_linux_mode_t)(perms)) != 0) return_err(E_cause$fs_PermissionDenied());
    return_ok({});
#else
    let_ignore = self;
    let_ignore = perms;
    return_err(E_cause$fs_Unsupported());
#endif
} $unscoped(fn);

fn_((fs_File_readPos(fs_File self, S$u8 buf, u64 offset))(E$usize) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$fs_ReadFailed());
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(self.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(E_cause$fs_ReadFailed());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = sys_call_linux_pread(self.handle, buf.ptr, buf.len, offset);
    if (sys_call_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$fs_ReadFailed());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else
    let_ignore = self;
    let_ignore = buf;
    let_ignore = offset;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_writePos(fs_File self, S_const$u8 buf, u64 offset))(E$usize) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$fs_WriteFailed());
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(self.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_written, null)) {
        return_err(E_cause$fs_WriteFailed());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = sys_call_linux_pwrite(self.handle, buf.ptr, buf.len, offset);
    if (sys_call_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$fs_WriteFailed());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else
    let_ignore = self;
    let_ignore = buf;
    let_ignore = offset;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_seekBy(fs_File self, i64 rel_offset))(E$void) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(rel_offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_CURRENT)) return_err(E_cause$fs_ReadFailed());
#elif plat_is_linux
    if (sys_call_linux_syscall_isErr(sys_call_linux_lseek(self.handle, rel_offset, sys_call_linux_SEEK_CUR))) return_err(E_cause$fs_ReadFailed());
#else
    let_ignore = self;
    let_ignore = rel_offset;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_seekTo(fs_File self, u64 abs_offset))(E$void) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(abs_offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$fs_ReadFailed());
#elif plat_is_linux
    if (sys_call_linux_syscall_isErr(sys_call_linux_lseek(self.handle, as$(i64)(abs_offset), sys_call_linux_SEEK_SET))) return_err(E_cause$fs_ReadFailed());
#else
    let_ignore = self;
    let_ignore = abs_offset;
    return_err(E_cause$fs_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_lock(fs_File self, fs_Lock lock))(E$void) $scope) {
    let_ignore = self;
    let_ignore = lock;
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_tryLock(fs_File self, fs_Lock lock))(E$bool) $scope) {
    let_ignore = self;
    let_ignore = lock;
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_unlock(fs_File self))(void)) {
    let_ignore = self;
}

fn_((fs_File_downgradeLock(fs_File self))(E$void) $scope) {
    let_ignore = self;
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_realpath(fs_File self, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = self;
    let_ignore = out_buf;
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_realpathAlloc(fs_File self, mem_Alctr gpa))(E$S$u8) $scope) {
    let_ignore = self;
    let_ignore = gpa;
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_io(fs_File self))(fs_File_IO)) {
    return (fs_File_IO){ .file = self };
}

$static fn_((fs_File_IO__read(P$raw ctx, S$u8 buf))(E$usize) $scope) {
    let self = ptrCast$((fs_File_IO*)(ensureNonnull(ctx)));
#if plat_is_windows
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(self->file.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(E_cause$fs_ReadFailed());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = sys_call_linux_read(self->file.handle, buf.ptr, buf.len);
    if (sys_call_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$fs_ReadFailed());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else
    let_ignore = self;
    let_ignore = buf;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

$static fn_((fs_File_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrCast$((fs_File_IO*)(ensureNonnull(ctx)));
#if plat_is_windows
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(self->file.handle, bytes.ptr, as$(DWORD)(bytes.len), &bytes_written, null)) {
        return_err(E_cause$fs_WriteFailed());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = sys_call_linux_write(self->file.handle, bytes.ptr, bytes.len);
    if (sys_call_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$fs_WriteFailed());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else
    let_ignore = self;
    let_ignore = bytes;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_IO_reader(fs_File_IO* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return (io_Reader){
        .ctx = self,
        .readFn = fs_File_IO__read,
    };
}

fn_((fs_File_IO_writer(fs_File_IO* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return (io_Writer){
        .ctx = self,
        .writeFn = fs_File_IO__write,
    };
}

$static fn_((fs__File_handleToCtx(fs_File_Handle handle))(P$raw)) {
#if plat_is_windows
    return ptrCast$((P$raw)(handle));
#else
    return intToPtr$((P$raw)(as$(usize)(handle)));
#endif
}

$static fn_((fs__File_handleFromCtx(P$raw ctx))(fs_File_Handle)) {
#if plat_is_windows
    return ptrCast$((fs_File_Handle)(ctx));
#else
    return as$(fs_File_Handle)(ptrToInt(ctx));
#endif
}

$static fn_((fs_File_handle__read(P$raw ctx, S$u8 buf))(E$usize) $scope) {
    var_(file, fs_File) = {
        .handle = fs__File_handleFromCtx(ctx),
        .flags = fs_File_Flags_default,
    };
#if plat_is_windows
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(file.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(E_cause$fs_ReadFailed());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = sys_call_linux_read(file.handle, buf.ptr, buf.len);
    if (sys_call_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$fs_ReadFailed());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else
    let_ignore = file;
    let_ignore = buf;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

$static fn_((fs_File_handle__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    var_(file, fs_File) = {
        .handle = fs__File_handleFromCtx(ctx),
        .flags = fs_File_Flags_default,
    };
#if plat_is_windows
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(file.handle, bytes.ptr, as$(DWORD)(bytes.len), &bytes_written, null)) {
        return_err(E_cause$fs_WriteFailed());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = sys_call_linux_write(file.handle, bytes.ptr, bytes.len);
    if (sys_call_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$fs_WriteFailed());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else
    let_ignore = file;
    let_ignore = bytes;
    return_err(E_cause$fs_Unsupported());
#endif
    claim_unreachable;
} $unscoped(fn);

typedef union Reader {
    io_Reader base;
    T_embed$(struct {
        P$raw ctx;
        fn_(((*readFn)(P$raw ctx, S$u8 buf))(E$usize)) $must_check;
    });
} Reader;

$static fn_((Reader_init(fs_File file))(Reader)) {
    return (Reader){ .ctx = fs__File_handleToCtx(file.handle), .readFn = fs_File_handle__read };
}

fn_((fs_File_reader(fs_File file))(io_Reader)) {
    return Reader_init(file).base;
}

typedef union Writer {
    io_Writer base;
    T_embed$(struct {
        P$raw ctx;
        fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(E$usize)) $must_check;
    });
} Writer;

$static fn_((Writer_init(fs_File file))(Writer)) {
    return (Writer){ .ctx = fs__File_handleToCtx(file.handle), .writeFn = fs_File_handle__write };
}

fn_((fs_File_writer(fs_File file))(io_Writer)) {
    return Writer_init(file).base;
}
