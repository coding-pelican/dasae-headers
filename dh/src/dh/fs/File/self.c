#include "dh/fs/File.h"
#include "dh/mem/common.h"
#include "../private/share.h"

#if plat_is_windows
#include "dh/os/windows/handle.h"
#include "dh/os/windows/file.h"
#elif plat_is_linux
#include "dh/os/linux/syscall.h"
#else /* plat_based_unix */
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
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

$static fn_((fs__windowsKind(DWORD attrs))(fs_Kind)) {
    if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0) return fs_Kind_directory;
    return fs_Kind_file;
}
#elif plat_is_linux
$static fn_((fs__linuxKind(os_linux_mode_t mode))(fs_Kind)) {
    switch (mode & os_linux_S_IFMT) {
    case os_linux_S_IFREG: return fs_Kind_file;
    case os_linux_S_IFDIR: return fs_Kind_directory;
    case os_linux_S_IFCHR: return fs_Kind_character_device;
    case os_linux_S_IFBLK: return fs_Kind_block_device;
    case os_linux_S_IFIFO: return fs_Kind_named_pipe;
    case os_linux_S_IFLNK: return fs_Kind_sym_link;
    case os_linux_S_IFSOCK: return fs_Kind_unix_domain_socket;
    default: return fs_Kind_unknown;
    }
}
#else /* plat_based_unix */
$static fn_((fs__posixKind(mode_t mode))(fs_Kind)) {
    if (S_ISREG(mode)) return fs_Kind_file;
    if (S_ISDIR(mode)) return fs_Kind_directory;
    if (S_ISCHR(mode)) return fs_Kind_character_device;
    if (S_ISBLK(mode)) return fs_Kind_block_device;
    if (S_ISFIFO(mode)) return fs_Kind_named_pipe;
    if (S_ISLNK(mode)) return fs_Kind_sym_link;
    if (S_ISSOCK(mode)) return fs_Kind_unix_domain_socket;
    return fs_Kind_unknown;
}
#endif

fn_((fs_File_open(S_const$u8 path, fs_File_OpenFlags flags))(E$fs_File) $scope) {
    if (flags.nonblocking && plat_is_windows) return_err(E_cause$UnsupportedFS());
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$FileTooBigFS());
#if plat_is_windows
    let handle = CreateFileA(
        as$(LPCSTR)(A_ptr(path_z)), fs__windowsOpenAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$OpenFailedFS());
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#elif plat_is_linux
    var_(open_flags, os_linux_word) = 0;
    if (flags.mode == fs_OpenMode_read_only) open_flags |= os_linux_O_RDONLY;
    if (flags.mode == fs_OpenMode_write_only) open_flags |= os_linux_O_WRONLY;
    if (flags.mode == fs_OpenMode_read_write) open_flags |= os_linux_O_RDWR;
    if (flags.nonblocking) open_flags |= os_linux_O_NONBLOCK;
    let handle = os_linux_openat(os_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), open_flags, 0);
    if (os_linux_syscall_isErr(handle)) {
        return_err(E_cause$OpenFailedFS());
    } else {
        return_ok(fs_File_Handle_promote(as$(fs_File_Handle)(handle), (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#else /* plat_based_unix */
    var_(open_flags, i32) = 0;
    if (flags.mode == fs_OpenMode_read_only) open_flags |= O_RDONLY;
    if (flags.mode == fs_OpenMode_write_only) open_flags |= O_WRONLY;
    if (flags.mode == fs_OpenMode_read_write) open_flags |= O_RDWR;
    if (flags.nonblocking) open_flags |= O_NONBLOCK;
    if_(let handle = open(as$(const char*)(A_ptr(path_z)), open_flags), handle < 0) {
        return_err(E_cause$OpenFailedFS());
    } else {
        return_ok(fs_File_Handle_promote(as$(fs_File_Handle)(handle), (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_create(S_const$u8 path, fs_File_CreateFlags flags))(E$fs_File) $scope) {
    if (flags.nonblocking && plat_is_windows) return_err(E_cause$UnsupportedFS());
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$FileTooBigFS());
#if plat_is_windows
    let disposition = flags.exclusive
                        ? CREATE_NEW
                        : (flags.truncate ? CREATE_ALWAYS : OPEN_ALWAYS);
    let handle = CreateFileA(
        as$(LPCSTR)(A_ptr(path_z)), fs__windowsCreateAccess(flags), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null,
        as$(DWORD)(disposition), FILE_ATTRIBUTE_NORMAL, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(E_cause$OpenFailedFS());
    return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
#elif plat_is_linux
    var_(open_flags, os_linux_word) = os_linux_O_CREAT | os_linux_O_WRONLY;
    if (flags.read) open_flags = os_linux_O_CREAT | os_linux_O_RDWR;
    if (flags.truncate) open_flags |= os_linux_O_TRUNC;
    if (flags.exclusive) open_flags |= os_linux_O_EXCL;
    if (flags.nonblocking) open_flags |= os_linux_O_NONBLOCK;
    let handle = os_linux_openat(os_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), open_flags, flags.mode);
    if (os_linux_syscall_isErr(handle)) {
        return_err(E_cause$OpenFailedFS());
    } else {
        return_ok(fs_File_Handle_promote(as$(fs_File_Handle)(handle), (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#else /* plat_based_unix */
    var_(open_flags, i32) = O_CREAT | O_WRONLY;
    if (flags.read) open_flags = O_CREAT | O_RDWR;
    if (flags.truncate) open_flags |= O_TRUNC;
    if (flags.exclusive) open_flags |= O_EXCL;
    if (flags.nonblocking) open_flags |= O_NONBLOCK;
    if_(let handle = open(as$(const char*)(A_ptr(path_z)), open_flags, flags.mode), handle < 0) {
        return_err(E_cause$OpenFailedFS());
    } else {
        return_ok(fs_File_Handle_promote(handle, (fs_File_Flags){ .nonblocking = flags.nonblocking }));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_delete(S_const$u8 path))(E$void) $scope) {
    var_(path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(path, A_ptr(path_z), A_len(path_z))) return_err(E_cause$FileTooBigFS());
#if plat_is_windows
    if (!DeleteFileA(as$(LPCSTR)(A_ptr(path_z)))) return_err(E_cause$NotFoundFS());
#elif plat_is_linux
    if (os_linux_unlinkat(os_linux_AT_FDCWD, as$(const char*)(A_ptr(path_z)), 0) != 0) return_err(E_cause$NotFoundFS());
#else /* plat_based_unix */
    if (unlink(as$(const char*)(A_ptr(path_z))) != 0) return_err(E_cause$NotFoundFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_rename(S_const$u8 old_path, S_const$u8 new_path))(E$void) $scope) {
    var_(old_path_z, A$$(fs__path_max, u8)) = A_zero();
    var_(new_path_z, A$$(fs__path_max, u8)) = A_zero();
    if (!fs__pathZ(old_path, A_ptr(old_path_z), A_len(old_path_z))) return_err(E_cause$FileTooBigFS());
    if (!fs__pathZ(new_path, A_ptr(new_path_z), A_len(new_path_z))) return_err(E_cause$FileTooBigFS());
#if plat_is_windows
    if (!MoveFileExA(
            as$(LPCSTR)(A_ptr(old_path_z)),
            as$(LPCSTR)(A_ptr(new_path_z)), MOVEFILE_REPLACE_EXISTING)) return_err(E_cause$WriteFailedFS());
#elif plat_is_linux
    if (os_linux_renameat(os_linux_AT_FDCWD, as$(const char*)(A_ptr(old_path_z)), os_linux_AT_FDCWD, as$(const char*)(A_ptr(new_path_z))) != 0) {
        return_err(E_cause$NotFoundFS());
    }
#else /* plat_based_unix */
    if (rename(as$(const char*)(A_ptr(old_path_z)), as$(const char*)(A_ptr(new_path_z))) != 0) {
        return_err(E_cause$NotFoundFS());
    }
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_close(fs_File self))(void)) {
#if plat_is_windows
    CloseHandle(self.handle);
#elif plat_is_linux
    let_ignore = os_linux_close(self.handle);
#else /* plat_based_unix */
    close(self.handle);
#endif
}

fn_((fs_File_stat(fs_File self))(fs_E$fs_File_Stat) $scope) {
#if plat_is_windows
    var_(info, BY_HANDLE_FILE_INFORMATION) = {};
    if (!GetFileInformationByHandle(self.handle, &info)) return_err(E_cause$ReadFailedFS());
    var_(size, ULARGE_INTEGER) = {};
    size.LowPart = info.nFileSizeLow;
    size.HighPart = info.nFileSizeHigh;
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(((u64)info.nFileIndexHigh << 32) | info.nFileIndexLow),
        .nlink = as$(fs_NLink)(info.nNumberOfLinks),
        .size = as$(u64)(size.QuadPart),
        .permissions = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0 ? 0444 : 0666,
        .kind = fs__windowsKind(info.dwFileAttributes),
        .atime = time_SysTime_fromUnixEpoch(0),
        .mtime = time_SysTime_fromUnixEpoch(0),
        .ctime = time_SysTime_fromUnixEpoch(0),
        .block_size = 4096,
    });
#elif plat_is_linux
    var_(st, os_linux_statx) = {};
    if (os_linux_statx_get(self.handle, "", os_linux_AT_EMPTY_PATH, os_linux_STATX_BASIC_STATS, &st) != 0) return_err(E_cause$ReadFailedFS());
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(st.stx_ino),
        .nlink = as$(fs_NLink)(st.stx_nlink),
        .size = as$(u64)(st.stx_size),
        .permissions = as$(fs_Mode)(st.stx_mode),
        .kind = fs__linuxKind(st.stx_mode),
        .atime = time_SysTime_fromUnixEpoch(as$(u64)(st.stx_atime.tv_sec)),
        .mtime = time_SysTime_fromUnixEpoch(as$(u64)(st.stx_mtime.tv_sec)),
        .ctime = time_SysTime_fromUnixEpoch(as$(u64)(st.stx_ctime.tv_sec)),
        .block_size = as$(fs_BlockSize)(st.stx_blksize),
    });
#else /* plat_based_unix */
    var_(st, struct stat) = {};
    if (fstat(self.handle, &st) != 0) return_err(E_cause$ReadFailedFS());
    return_ok((fs_File_Stat){
        .inode = as$(fs_INode)(st.st_ino),
        .nlink = as$(fs_NLink)(st.st_nlink),
        .size = as$(u64)(st.st_size),
        .permissions = as$(fs_Mode)(st.st_mode),
        .kind = fs__posixKind(st.st_mode),
        .atime = time_SysTime_fromUnixEpoch(as$(u64)(st.st_atime)),
        .mtime = time_SysTime_fromUnixEpoch(as$(u64)(st.st_mtime)),
        .ctime = time_SysTime_fromUnixEpoch(as$(u64)(st.st_ctime)),
        .block_size = as$(fs_BlockSize)(st.st_blksize),
    });
#endif
} $unscoped(fn);

fn_((fs_File_sync(fs_File self))(E$void) $scope) {
#if plat_is_windows
    if (!FlushFileBuffers(self.handle)) return_err(E_cause$WriteFailedFS());
#elif plat_is_linux
    if (os_linux_fsync(self.handle) != 0) return_err(E_cause$WriteFailedFS());
#else /* plat_based_unix */
    if (fsync(self.handle) != 0) return_err(E_cause$WriteFailedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_isTty(fs_File self))(E$bool) $scope) {
#if plat_is_windows
    return_ok(GetFileType(self.handle) == FILE_TYPE_CHAR);
#elif plat_is_linux
    var_(termios_buf, A$$(128, u8)) = A_zero();
    return_ok(os_linux_ioctl(self.handle, os_linux_TCGETS, A_ptr(termios_buf)) == 0);
#else /* plat_based_unix */
    return_ok(isatty(self.handle) != 0);
#endif
} $unscoped(fn);

fn_((fs_File_setLen(fs_File self, u64 new_len))(E$void) $scope) {
#if plat_is_windows
    var_(pos, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(new_len) };
    if (!SetFilePointerEx(self.handle, pos, null, FILE_BEGIN)) return_err(E_cause$WriteFailedFS());
    if (!SetEndOfFile(self.handle)) return_err(E_cause$WriteFailedFS());
#elif plat_is_linux
    if (os_linux_ftruncate(self.handle, new_len) != 0) return_err(E_cause$WriteFailedFS());
#else /* plat_based_unix */
    if (ftruncate(self.handle, as$(off_t)(new_len)) != 0) return_err(E_cause$WriteFailedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_setPerms(fs_File self, fs_File_Mode perms))(E$void) $scope) {
#if plat_is_windows
    let_ignore = self;
    let_ignore = perms;
    return_err(E_cause$UnsupportedFS());
#elif plat_is_linux
    if (os_linux_fchmod(self.handle, as$(os_linux_mode_t)(perms)) != 0) return_err(E_cause$PermissionDeniedFS());
    return_ok({});
#else /* plat_based_unix */
    if (fchmod(self.handle, as$(mode_t)(perms)) != 0) return_err(E_cause$PermissionDeniedFS());
    return_ok({});
#endif
} $unscoped(fn);

fn_((fs_File_readPos(fs_File self, S$u8 buf, u64 offset))(E$usize) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$ReadFailedFS());
    var_(bytes_read, DWORD) = 0;
    if (!ReadFile(self.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_read, null)) {
        let err = GetLastError();
        if (err == ERROR_HANDLE_EOF || err == ERROR_BROKEN_PIPE) return_ok(0);
        return_err(E_cause$ReadFailedFS());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = os_linux_pread(self.handle, buf.ptr, buf.len, offset);
    if (os_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else /* plat_based_unix */
    if_(let bytes_read = pread(self.handle, buf.ptr, buf.len, as$(off_t)(offset)), bytes_read < 0) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_writePos(fs_File self, S_const$u8 buf, u64 offset))(E$usize) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$WriteFailedFS());
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(self.handle, buf.ptr, as$(DWORD)(buf.len), &bytes_written, null)) {
        return_err(E_cause$WriteFailedFS());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = os_linux_pwrite(self.handle, buf.ptr, buf.len, offset);
    if (os_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else /* plat_based_unix */
    if_(let bytes_written = pwrite(self.handle, buf.ptr, buf.len, as$(off_t)(offset)), bytes_written < 0) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

fn_((fs_File_seekBy(fs_File self, i64 rel_offset))(E$void) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(rel_offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_CURRENT)) return_err(E_cause$ReadFailedFS());
#elif plat_is_linux
    if (os_linux_syscall_isErr(os_linux_lseek(self.handle, rel_offset, os_linux_SEEK_CUR))) return_err(E_cause$ReadFailedFS());
#else /* plat_based_unix */
    if (lseek(self.handle, as$(off_t)(rel_offset), SEEK_CUR) < 0) return_err(E_cause$ReadFailedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_seekTo(fs_File self, u64 abs_offset))(E$void) $scope) {
#if plat_is_windows
    var_(distance, LARGE_INTEGER) = { .QuadPart = as$(LONGLONG)(abs_offset) };
    if (!SetFilePointerEx(self.handle, distance, null, FILE_BEGIN)) return_err(E_cause$ReadFailedFS());
#elif plat_is_linux
    if (os_linux_syscall_isErr(os_linux_lseek(self.handle, as$(i64)(abs_offset), os_linux_SEEK_SET))) return_err(E_cause$ReadFailedFS());
#else /* plat_based_unix */
    if (lseek(self.handle, as$(off_t)(abs_offset), SEEK_SET) < 0) return_err(E_cause$ReadFailedFS());
#endif
    return_ok({});
} $unscoped(fn);

fn_((fs_File_lock(fs_File self, fs_Lock lock))(E$void) $scope) {
    let_ignore = self;
    let_ignore = lock;
    return_err(E_cause$UnsupportedFS());
} $unscoped(fn);

fn_((fs_File_tryLock(fs_File self, fs_Lock lock))(E$bool) $scope) {
    let_ignore = self;
    let_ignore = lock;
    return_err(E_cause$UnsupportedFS());
} $unscoped(fn);

fn_((fs_File_unlock(fs_File self))(void)) {
    let_ignore = self;
}

fn_((fs_File_downgradeLock(fs_File self))(E$void) $scope) {
    let_ignore = self;
    return_err(E_cause$UnsupportedFS());
} $unscoped(fn);

fn_((fs_File_realpath(fs_File self, S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = self;
    let_ignore = out_buf;
    return_err(E_cause$UnsupportedFS());
} $unscoped(fn);

fn_((fs_File_realpathAlloc(fs_File self, mem_Alctr gpa))(E$S$u8) $scope) {
    let_ignore = self;
    let_ignore = gpa;
    return_err(E_cause$UnsupportedFS());
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
        return_err(E_cause$ReadFailedFS());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = os_linux_read(self->file.handle, buf.ptr, buf.len);
    if (os_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else /* plat_based_unix */
    if_(let bytes_read = read(self->file.handle, buf.ptr, buf.len), bytes_read < 0) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

$static fn_((fs_File_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrCast$((fs_File_IO*)(ensureNonnull(ctx)));
#if plat_is_windows
    var_(bytes_written, DWORD) = 0;
    if (!WriteFile(self->file.handle, bytes.ptr, as$(DWORD)(bytes.len), &bytes_written, null)) {
        return_err(E_cause$WriteFailedFS());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = os_linux_write(self->file.handle, bytes.ptr, bytes.len);
    if (os_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else /* plat_based_unix */
    if_(let bytes_written = write(self->file.handle, bytes.ptr, bytes.len), bytes_written < 0) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
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
#else /* plat_based_unix */
    return intToPtr$((P$raw)(as$(usize)(handle)));
#endif
}

$static fn_((fs__File_handleFromCtx(P$raw ctx))(fs_File_Handle)) {
#if plat_is_windows
    return ptrCast$((fs_File_Handle)(ctx));
#else /* plat_based_unix */
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
        return_err(E_cause$ReadFailedFS());
    }
    return_ok(as$(usize)(bytes_read));
#elif plat_is_linux
    let bytes_read = os_linux_read(file.handle, buf.ptr, buf.len);
    if (os_linux_syscall_isErr(bytes_read)) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
#else /* plat_based_unix */
    if_(let bytes_read = read(file.handle, buf.ptr, buf.len), bytes_read < 0) {
        return_err(E_cause$ReadFailedFS());
    } else {
        return_ok(as$(usize)(bytes_read));
    }
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
        return_err(E_cause$WriteFailedFS());
    }
    return_ok(as$(usize)(bytes_written));
#elif plat_is_linux
    let bytes_written = os_linux_write(file.handle, bytes.ptr, bytes.len);
    if (os_linux_syscall_isErr(bytes_written)) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#else /* plat_based_unix */
    if_(let bytes_written = write(file.handle, bytes.ptr, bytes.len), bytes_written < 0) {
        return_err(E_cause$WriteFailedFS());
    } else {
        return_ok(as$(usize)(bytes_written));
    }
#endif
    claim_unreachable;
} $unscoped(fn);

typedef union Reader {
    io_Reader base;
    struct {
        P$raw ctx;
        fn_(((*readFn)(P$raw ctx, S$u8 buf))(E$usize)) $must_check;
    };
} Reader;

$static fn_((Reader_init(fs_File file))(Reader)) {
    return (Reader){ .ctx = fs__File_handleToCtx(file.handle), .readFn = fs_File_handle__read };
}

fn_((fs_File_reader(fs_File file))(io_Reader)) {
    return Reader_init(file).base;
}

typedef union Writer {
    io_Writer base;
    struct {
        P$raw ctx;
        fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(E$usize)) $must_check;
    };
} Writer;

$static fn_((Writer_init(fs_File file))(Writer)) {
    return (Writer){ .ctx = fs__File_handleToCtx(file.handle), .writeFn = fs_File_handle__write };
}

fn_((fs_File_writer(fs_File file))(io_Writer)) {
    return Writer_init(file).base;
}
