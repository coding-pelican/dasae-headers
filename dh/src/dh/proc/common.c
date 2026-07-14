#include "dh/proc.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"
#include "dh/sys/api/windows/proc.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_windows */

#if plat_is_windows
#define proc__windows_path_max (usize_(32768))
#define proc__windows_path_prefix_len (usize_(4))

$static fn_((proc__windows_heapAlloc(usize len))(O$P$u8)) {
    let ptr = ptrCast$((P$u8)(HeapAlloc(GetProcessHeap(), 0, len)));
    return ptr == null ? none$((O$P$u8)) : some$((O$P$u8)(ptr));
};

$static fn_((proc__windows_heapFree(P$u8 ptr))(void)) {
    claim_assert_nonnull(ptr);
    claim_assert(HeapFree(GetProcessHeap(), 0, ptr));
};

$static fn_((proc__windows_mapError(DWORD err))(proc_E)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound();
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied();
    case ERROR_INVALID_NAME: return E_cause$proc_InvalidName();
    case ERROR_DIRECTORY: return E_cause$proc_NotDir();
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$proc_SystemResources();
    case ERROR_TOO_MANY_OPEN_FILES: $fallthrough;
    case ERROR_SHARING_BUFFER_EXCEEDED: return E_cause$proc_ResourceLimitReached();
    default_() return E_cause$proc_SystemResources() $end(default);
    }
};

$static fn_((proc__windows_dupInheritable(HANDLE src))(E$proc_Handle) $scope) {
    var_(dup, HANDLE) = INVALID_HANDLE_VALUE;
    if (!DuplicateHandle(
            GetCurrentProcess(), src,
            GetCurrentProcess(), &dup,
            0, TRUE, DUPLICATE_SAME_ACCESS
        )) {
        return_err(proc__windows_mapError(GetLastError()));
    }
    return_ok(as$(proc_Handle)(dup));
} $unscoped(fn);

$static fn_((proc__windows_stdioNull(bool for_read))(E$proc_Handle) $guard) {
    let path = for_read ? "NUL" : "NUL";
    let access = for_read ? GENERIC_READ : GENERIC_WRITE;
    let handle = CreateFileA(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE, null, OPEN_EXISTING, 0, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(proc__windows_mapError(GetLastError()));
    defer_(claim_assert(CloseHandle(handle)));
    let inheritable = try_(proc__windows_dupInheritable(handle));
    return_ok(inheritable);
} $unguarded(fn);

typedef struct proc__windows_ResolvedStdIO {
    var_(child, O$proc_Handle);
    var_(parent_pipe, O$fs_File);
    var_(needs_close_child, bool);
} proc__windows_ResolvedStdIO;
T_use_E$(proc__windows_ResolvedStdIO);

typedef struct proc__windows_OwnedBuf {
    var_(ptr, O$P$u8);
    var_(len, usize);
} proc__windows_OwnedBuf;
T_use_E$(proc__windows_OwnedBuf);

$static fn_((proc__windows_ResolvedStdIO_fini(proc__windows_ResolvedStdIO* self))(void)) {
    claim_assert_nonnull(self);
    if (self->needs_close_child) {
        if_some((self->child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
        asg_l((&self->child)(none()));
    }
    if_some((self->parent_pipe)(file)) fs_File_close(file);
    asg_l((&self->parent_pipe)(none()));
};

$static fn_((proc__windows_OwnedBuf_fini(proc__windows_OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->ptr)(ptr)) proc__windows_heapFree(ptr);
    asg_l((&self->ptr)(none()));
    self->len = 0;
};

$static fn_((proc__windows_dupSliceZ(S_const$u8 src))(E$proc__windows_OwnedBuf) $scope) {
    let buf = orelse_((proc__windows_heapAlloc(src.len + 1))(return_err(E_cause$proc_SystemResources())));
    let out = P_prefix$((S$u8)(buf)(src.len + 1));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok({
        .ptr = some(buf),
        .len = src.len,
    });
} $unscoped(fn);

$static fn_((proc__windows_trimNtPrefix(proc__windows_OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if (self->len < proc__windows_path_prefix_len) return;
    let path = P_prefix$((S$u8)(unwrap_(self->ptr))(self->len + 1));
    if (
        *S_at((path)[0]) == u8_c('\\') && *S_at((path)[1]) == u8_c('\\') && *S_at((path)[2]) == u8_c('?') && *S_at((path)[3]) == u8_c('\\')
    ) {
        mem_moveBytes(
            S_prefix((path)(self->len + 1 - proc__windows_path_prefix_len)),
            S_slice((path.as_const)$r(proc__windows_path_prefix_len, self->len + 1))
        );
        self->len -= proc__windows_path_prefix_len;
    }
};

$static fn_((proc__windows_dirPathAlloc(fs_Dir dir))(E$proc__windows_OwnedBuf) $guard) {
    let need = GetFinalPathNameByHandleA(dir.handle, null, 0, 0);
    if (need == 0) return_err(proc__windows_mapError(GetLastError()));
    let buf = orelse_((proc__windows_heapAlloc(as$(usize)(need) + 1))(return_err(E_cause$proc_SystemResources())));
    errdefer_($ignore, proc__windows_heapFree(buf));
    let wrote = GetFinalPathNameByHandleA(dir.handle, as$(LPSTR)(buf), need + 1, 0);
    if (wrote == 0) return_err(proc__windows_mapError(GetLastError()));
    var_(owned, proc__windows_OwnedBuf) = {
        .ptr = some(buf),
        .len = as$(usize)(wrote),
    };
    proc__windows_trimNtPrefix(&owned);
    return_ok(owned);
} $unguarded(fn);

$static fn_((proc__windows_resolvePathAlloc(S_const$u8 base, S_const$u8 sub_path))(E$proc__windows_OwnedBuf) $guard) {
    if (fs_path_isAbs(sub_path)) return_(proc__windows_dupSliceZ(sub_path));
    let base_sub_len = orelse_((usize_addChkd(base.len, sub_path.len))(return_err(E_cause$proc_SystemResources())));
    let cap = orelse_((usize_addChkd(base_sub_len, usize_(2)))(return_err(E_cause$proc_SystemResources())));
    let buf = orelse_((proc__windows_heapAlloc(cap))(return_err(E_cause$proc_SystemResources())));
    errdefer_($ignore, proc__windows_heapFree(buf));
    let out = P_prefix$((S$u8)(buf)(cap));
    let joined = catch_((fs_path_join2(base, sub_path, S_prefix((out)(cap - 1))))($ignore, {
        return_err(E_cause$proc_SystemResources());
    }));
    *S_at((out)[joined.len]) = 0;
    return_ok({
        .ptr = some(buf),
        .len = joined.len,
    });
} $unguarded(fn);

$static fn_((proc__windows_envBlockAlloc(O$proc_Env env_opt))(E$proc__windows_OwnedBuf) $scope) {
    if_none(env_opt) {
        return_ok({
            .ptr = none(),
            .len = 0,
        });
    }
    let env = unwrap_(env_opt);
    var_(len, usize) = 1;
    for_(($s(env))(item)) { len += item->len + 1; } $end(for);

    let buf = orelse_((proc__windows_heapAlloc(len))(return_err(E_cause$proc_SystemResources())));

    let out = P_prefix$((S$u8)(buf)(len));
    var_(pos, usize) = 0;
    for_(($s(env))(item)) {
        mem_copyBytes(S_slice((out)$r(pos, pos + item->len)), *item);
        pos += item->len;
        *S_at((out)[pos++]) = 0;
    } $end(for);
    *S_at((out)[pos++]) = 0;
    claim_assert(pos == len);
    return_ok({
        .ptr = some(buf),
        .len = len,
    });
} $unscoped(fn);

$static fn_((proc__windows_resolveStdIO(proc_StdIO spec, DWORD std_id))(E$proc__windows_ResolvedStdIO) $scope) {
    let for_read = std_id == STD_INPUT_HANDLE;
    switch (spec.tag) {
    case proc_StdIO_Tag_inherit: {
        let base = GetStdHandle(std_id);
        if (base == null || base == INVALID_HANDLE_VALUE) {
            return_ok({
                .child = none(),
                .parent_pipe = none(),
                .needs_close_child = false,
            });
        }
        let child = try_(proc__windows_dupInheritable(base));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_file: {
        let child = try_(proc__windows_dupInheritable(fs_File_handle(spec.file)));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_ignore: $fallthrough;
    case proc_StdIO_Tag_close: {
        let child = try_(proc__windows_stdioNull(for_read));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_pipe: {
        SECURITY_ATTRIBUTES sa = {
            .nLength = sizeOf$(SECURITY_ATTRIBUTES),
            .lpSecurityDescriptor = null,
            .bInheritHandle = TRUE,
        };
        var_(read_end, HANDLE) = INVALID_HANDLE_VALUE;
        var_(write_end, HANDLE) = INVALID_HANDLE_VALUE;
        if (!CreatePipe(&read_end, &write_end, &sa, 0)) {
            return_err(proc__windows_mapError(GetLastError()));
        }
        if (for_read) {
            claim_assert(SetHandleInformation(write_end, HANDLE_FLAG_INHERIT, 0));
            return_ok({
                .child = some(as$(proc_Handle)(read_end)),
                .parent_pipe = some(fs_File_Handle_promote(write_end, fs_File_Flags_default)),
                .needs_close_child = true,
            });
        }
        claim_assert(SetHandleInformation(read_end, HANDLE_FLAG_INHERIT, 0));
        return_ok({
            .child = some(as$(proc_Handle)(write_end)),
            .parent_pipe = some(fs_File_Handle_promote(read_end, fs_File_Flags_default)),
            .needs_close_child = true,
        });
    }
    default_() return_err(E_cause$proc_OperationUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((proc__windows_appendQuoted(S$u8 out, usize used, S_const$u8 arg))(O$usize) $scope) {
    let needs_quotes = arg.len == 0 || isSome(mem_findFirstAnyBytes(arg, u8_l(" \t\"")));
    if (!needs_quotes) {
        if (used + arg.len > out.len) return_none();
        mem_copyBytes(S_slice((out)$r(used, used + arg.len)), arg);
        return_some(used + arg.len);
    }
    if (used + 2 > out.len) return_none();
    *S_at((out)[used++]) = u8_c('"');
    var_(backslashes, usize) = 0;
    for_(($s(arg))(p_ch)) {
        let ch = *p_ch;
        if (ch == u8_c('\\')) {
            backslashes += 1;
            continue;
        }
        if (ch == u8_c('"')) {
            if (used + backslashes * 2 + 1 > out.len) return_none();
            for (usize i = 0; i < backslashes * 2 + 1; ++i) *S_at((out)[used++]) = u8_c('\\');
            *S_at((out)[used++]) = u8_c('"');
            backslashes = 0;
            continue;
        }
        if (used + backslashes + 1 > out.len) return_none();
        for (usize i = 0; i < backslashes; ++i) *S_at((out)[used++]) = u8_c('\\');
        backslashes = 0;
        *S_at((out)[used++]) = ch;
    } $end(for);
    if (used + backslashes * 2 + 1 > out.len) return_none();
    for (usize i = 0; i < backslashes * 2; ++i) *S_at((out)[used++]) = u8_c('\\');
    *S_at((out)[used++]) = u8_c('"');
    return_some(used);
} $unscoped(fn);

$static fn_((proc__windows_commandLine(S$S_const$u8 argv, S$u8 out))(E$S$u8) $scope) {
    var_(used, usize) = 0;
    for_(($rf(0), $s(argv))(i, arg)) {
        if (i != 0) {
            if (used + 1 > out.len) return_err(E_cause$proc_ResourceLimitReached());
            *S_at((out)[used++]) = u8_c(' ');
        }
        used = orelse_((proc__windows_appendQuoted(out, used, *arg))(return_err(E_cause$proc_ResourceLimitReached())));
    } $end(for);
    if (used >= out.len) return_err(E_cause$proc_ResourceLimitReached());
    *S_at((out)[used]) = 0;
    return_ok(S_prefix((out)(used)));
} $unscoped(fn);

$static fn_((proc__windows_spawnImpl(proc_Cmd cmd, O$S_const$u8 application_name, O$S_const$u8 current_dir))(E$proc_Child) $guard) {
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var_(cmdline_buf, proc__windows_OwnedBuf) = {
        .ptr = proc__windows_heapAlloc(proc__windows_path_max),
        .len = proc__windows_path_max,
    };
    if_none(cmdline_buf.ptr) return_err(E_cause$proc_SystemResources());
    errdefer_($ignore, proc__windows_OwnedBuf_fini(&cmdline_buf));
    let _cmdline = try_(proc__windows_commandLine(cmd.argv, P_prefix$((S$u8)(unwrap_(cmdline_buf.ptr))(cmdline_buf.len))));
    let_ignore = _cmdline;
    var_(env_block, proc__windows_OwnedBuf) = try_(proc__windows_envBlockAlloc(cmd.env));
    errdefer_($ignore, proc__windows_OwnedBuf_fini(&env_block));

    var_(std_in, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(cmd.std_in, STD_INPUT_HANDLE));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_in));
    var_(std_out, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(cmd.std_out, STD_OUTPUT_HANDLE));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_out));
    var_(std_err, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(cmd.std_err, STD_ERROR_HANDLE));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_err));

    STARTUPINFOA startup = cleared();
    startup.cb = sizeOf$(STARTUPINFOA);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = as$(HANDLE)(orelse_((std_in.child)(as$(proc_Handle)(null))));
    startup.hStdOutput = as$(HANDLE)(orelse_((std_out.child)(as$(proc_Handle)(null))));
    startup.hStdError = as$(HANDLE)(orelse_((std_err.child)(as$(proc_Handle)(null))));

    PROCESS_INFORMATION proc_info = cleared();
    var_(flags, DWORD) = 0;
    if (cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessA(
            isSome(application_name) ? as$(LPCSTR)(unwrap_(application_name).ptr) : null,
            as$(LPSTR)(unwrap_(cmdline_buf.ptr)),
            null,
            null,
            TRUE,
            flags,
            isSome(env_block.ptr) ? unwrap_(env_block.ptr) : null,
            isSome(current_dir) ? as$(LPCSTR)(unwrap_(current_dir).ptr) : null,
            &startup,
            &proc_info
        )) {
        return_err(proc__windows_mapError(GetLastError()));
    }

    claim_assert(CloseHandle(proc_info.hThread));
    var cleanup_cmdline = cmdline_buf;
    proc__windows_OwnedBuf_fini(&cleanup_cmdline);
    var cleanup_env = env_block;
    proc__windows_OwnedBuf_fini(&cleanup_env);
    if (std_in.needs_close_child) if_some((std_in.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_out.needs_close_child) if_some((std_out.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_err.needs_close_child) if_some((std_err.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));

    return_ok({
        .handle = some(as$(proc_Child_Handle)(proc_info.hProcess)),
        .id = as$(u64)(proc_info.dwProcessId),
        .std_in = std_in.parent_pipe,
        .std_out = std_out.parent_pipe,
        .std_err = std_err.parent_pipe,
    });
} $unguarded(fn);

$static fn_((proc__windows_executablePath(S$u8 out_buf))(E$S$u8) $scope) {
    if (out_buf.len == 0) return_err(E_cause$proc_ResourceLimitReached());
    let wrote = GetModuleFileNameA(null, as$(LPSTR)(out_buf.ptr), as$(DWORD)(out_buf.len));
    if (wrote == 0) return_err(proc__windows_mapError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

$static fn_((proc__windows_currentPath(S$u8 out_buf))(E$S$u8) $scope) {
    let wrote = GetCurrentDirectoryA(as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr));
    if (wrote == 0) return_err(proc__windows_mapError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

$static fn_((proc__windows_setCurrentPath(S_const$u8 path))(E$void) $guard) {
    var_(path_z, proc__windows_OwnedBuf) = try_(proc__windows_dupSliceZ(path));
    defer_(proc__windows_OwnedBuf_fini(&path_z));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(unwrap_(path_z.ptr)))) {
        return_err(proc__windows_mapError(GetLastError()));
    }
    return_ok({});
} $unguarded(fn);

$static fn_((proc__windows_spawn(proc_Cmd cmd))(E$proc_Child) $guard) {
    if_none(cmd.cwd) return_(proc__windows_spawnImpl(cmd, none$((O$S_const$u8)), none$((O$S_const$u8))));
    var_(cwd, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(unwrap_(cmd.cwd)));
    defer_(proc__windows_OwnedBuf_fini(&cwd));
    let child = try_(proc__windows_spawnImpl(
        cmd,
        none$((O$S_const$u8)),
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(cwd.ptr), .len = cwd.len }))
    ));
    return_ok(child);
} $unguarded(fn);

$static fn_((proc__windows_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $guard) {
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    var_(base, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(dir));
    defer_(proc__windows_OwnedBuf_fini(&base));
    var_(exe_path, proc__windows_OwnedBuf) = try_(proc__windows_resolvePathAlloc(
        P_prefix$((S$u8)(unwrap_(base.ptr))(base.len)).as_const,
        *S_at((cmd.argv)[0])
    ));
    defer_(proc__windows_OwnedBuf_fini(&exe_path));
    let child = try_(proc__windows_spawnImpl(
        cmd,
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(exe_path.ptr), .len = exe_path.len })),
        none$((O$S_const$u8))
    ));
    return_ok(child);
} $unguarded(fn);
#endif /* plat_is_windows */

#if plat_is_linux
#define proc__linux_arg_max 64
#define proc__linux_arg_len_max 1024
#define proc__linux_path_len_max 4096

$static fn_((proc__linux_mapErr(sys_call_linux_word err))(proc_E)) {
    switch (err) {
    case 2: return E_cause$proc_FileNotFound();
    case 13: return E_cause$proc_AccessDenied();
    case 20: return E_cause$proc_NotDir();
    case 24: return E_cause$proc_ResourceLimitReached();
    case 36: return E_cause$proc_InvalidName();
    default_() return E_cause$proc_SystemResources() $end(default);
    }
};

$static fn_((proc__linux_copyZ(S_const$u8 src, S$u8 dst))(E$P$u8) $scope) {
    if (src.len + 1 > dst.len) return_err(E_cause$proc_ResourceLimitReached());
    mem_copyBytes(S_prefix((dst)(src.len)), src);
    *S_at((dst)[src.len]) = 0;
    return_ok(dst.ptr);
} $unscoped(fn);

$static fn_((proc__linux_pipeFile(sys_call_linux_fd_t fd))(fs_File)) {
    return fs_File_Handle_promote(as$(fs_File_Handle)(fd), fs_File_Flags_default);
};

T_alias$((proc__linux_Envp)(char**));
T_use_E$(proc__linux_Envp);

typedef struct proc__linux_StdIO {
    var_(child_fd, sys_call_linux_fd_t);
    var_(parent_, O$fs_File);
    var_(needs_close_child, bool);
} proc__linux_StdIO;
T_use_E$(proc__linux_StdIO);


typedef struct proc__linux_SpawnScratch {
    char* argv[proc__linux_arg_max];
    char arg_bufs[proc__linux_arg_max][proc__linux_arg_len_max];
    char* envp[proc__linux_arg_max];
    char env_bufs[proc__linux_arg_max][proc__linux_arg_len_max];
    char exe_buf[proc__linux_path_len_max];
    char path_buf[proc__linux_path_len_max];
} proc__linux_SpawnScratch;
T_use_E$(proc__linux_SpawnScratch);

$static fn_((proc__linux_allocScratch(void))(E$P$raw) $scope) {
    let mapped = sys_call_linux_mmap(
        null,
        sizeOf$(proc__linux_SpawnScratch),
        sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE,
        sys_call_linux_MAP_PRIVATE | sys_call_linux_MAP_ANONYMOUS,
        -1,
        0
    );
    if (sys_call_linux_syscall_isErr(mapped)) return_err(E_cause$proc_SystemResources());
    return_ok(intToPtr$((P$raw)(mapped)));
} $unscoped(fn);

$static fn_((proc__linux_freeScratch(P$raw scratch))(void)) {
    if (scratch != null) let_ignore = sys_call_linux_munmap(scratch, sizeof(proc__linux_SpawnScratch));
};

$static fn_((proc__linux_closeIf(sys_call_linux_fd_t fd))(void)) {
    if (fd >= 0) let_ignore = sys_call_linux_close(fd);
};

$static fn_((proc__linux_resolveStdIO(proc_StdIO spec, sys_call_linux_fd_t std_fd))(E$proc__linux_StdIO) $scope) {
    let for_read = std_fd == 0;
    switch (spec.tag) {
    case proc_StdIO_Tag_inherit: {
        return_ok({
            .child_fd = std_fd,
            .parent_ = none(),
            .needs_close_child = false,
        });
    }
    case proc_StdIO_Tag_file: {
        return_ok({
            .child_fd = as$(sys_call_linux_fd_t)(fs_File_handle(spec.file)),
            .parent_ = none(),
            .needs_close_child = false,
        });
    }
    case proc_StdIO_Tag_ignore: $fallthrough;
    case proc_StdIO_Tag_close: {
        let flags = for_read ? sys_call_linux_O_RDONLY : sys_call_linux_O_WRONLY;
        let fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/null", flags, 0);
        if (sys_call_linux_syscall_isErr(fd)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(fd)));
        return_ok({
            .child_fd = fd,
            .parent_ = none(),
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_pipe: {
        int fds[2] = { -1, -1 };
        let rc = sys_call_linux_pipe2(fds, sys_call_linux_O_CLOEXEC);
        if (sys_call_linux_syscall_isErr(rc)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
        if (for_read) {
            return_ok({
                .child_fd = fds[0],
                .parent_ = some(proc__linux_pipeFile(fds[1])),
                .needs_close_child = true,
            });
        }
        return_ok({
            .child_fd = fds[1],
            .parent_ = some(proc__linux_pipeFile(fds[0])),
            .needs_close_child = true,
        });
    }
    default_() return_err(E_cause$proc_OperationUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((proc__linux_dupTo(sys_call_linux_fd_t src, sys_call_linux_fd_t dst))(void)) {
    if (src == dst) return;
    let_ignore = sys_call_linux_dup3(src, dst, 0);
    proc__linux_closeIf(src);
};

$attr($no_return)
$static fn_((proc__linux_childExec(
    proc_Cmd cmd,
    const char* exe,
    char* const* argv,
    char* const* envp,
    proc__linux_StdIO std_in,
    proc__linux_StdIO std_out,
    proc__linux_StdIO std_err
))(void)) {
    if_some((cmd.cwd)(cwd)) {
        let_ignore = sys_call_linux_fchdir(fs_Dir_handle(cwd));
    }
    proc__linux_dupTo(std_in.child_fd, 0);
    proc__linux_dupTo(std_out.child_fd, 1);
    proc__linux_dupTo(std_err.child_fd, 2);
    let_ignore = sys_call_linux_execve(exe, argv, envp);
    sys_call_linux_exit(127);
};

$static fn_((proc__linux_buildArgv(S$S_const$u8 args, char* argv[proc__linux_arg_max], char arg_bufs[proc__linux_arg_max][proc__linux_arg_len_max]))(E$void) $scope) {
    if (args.len == 0 || args.len >= proc__linux_arg_max) return_err(E_cause$proc_InvalidName());
    for_(($rf(0), $s(args))(i, arg)) {
        let dst = (S$u8){ .ptr = as$(P$u8)(arg_bufs[i]), .len = proc__linux_arg_len_max };
        argv[i] = as$(char*)(try_(proc__linux_copyZ(*arg, dst)));
    } $end(for);
    argv[args.len] = null;
    return_ok({});
} $unscoped(fn);

$static fn_((proc__linux_buildEnv(O$proc_Env env, char* envp[proc__linux_arg_max], char env_bufs[proc__linux_arg_max][proc__linux_arg_len_max]))(E$proc__linux_Envp) $scope) {
    if_none(env) return_ok(null);
    let items = unwrap_(env);
    if (items.len >= proc__linux_arg_max) return_err(E_cause$proc_ResourceLimitReached());
    for_(($rf(0), $s(items))(i, item)) {
        let dst = (S$u8){ .ptr = as$(P$u8)(env_bufs[i]), .len = proc__linux_arg_len_max };
        envp[i] = as$(char*)(try_(proc__linux_copyZ(*item, dst)));
    } $end(for);
    envp[items.len] = null;
    return_ok(envp);
} $unscoped(fn);

$static fn_((proc__linux_spawnImpl(proc_Cmd cmd, O$S_const$u8 exe_path))(E$proc_Child) $guard) {
    if (cmd.start_suspended) return_err(E_cause$proc_OperationUnsupported());
    let_ignore = cmd.create_no_window;

    let scratch_raw = try_(proc__linux_allocScratch());
    defer_(proc__linux_freeScratch(scratch_raw));
    let scratch = ptrAlignCast$((proc__linux_SpawnScratch*)(scratch_raw));

    try_(proc__linux_buildArgv(cmd.argv, scratch->argv, scratch->arg_bufs));
    let child_env = try_(proc__linux_buildEnv(cmd.env, scratch->envp, scratch->env_bufs));

    let exe = eval_(const char* $scope)({
        if_some((exe_path)(path)) {
            $break_(as$(const char*)(try_(proc__linux_copyZ(path, (S$u8){ .ptr = as$(P$u8)(scratch->exe_buf), .len = proc__linux_path_len_max }))));
        }
        $break_(as$(const char*)(scratch->argv[0]));
    }) $unscoped(eval);

    var_(std_in, proc__linux_StdIO) = try_(proc__linux_resolveStdIO(cmd.std_in, 0));
    var_(std_out, proc__linux_StdIO) = catch_((proc__linux_resolveStdIO(cmd.std_out, 1))(err, {
        if (std_in.needs_close_child) proc__linux_closeIf(std_in.child_fd);
        return_err(err);
    }));
    var_(std_err, proc__linux_StdIO) = catch_((proc__linux_resolveStdIO(cmd.std_err, 2))(err, {
        if (std_in.needs_close_child) proc__linux_closeIf(std_in.child_fd);
        if (std_out.needs_close_child) proc__linux_closeIf(std_out.child_fd);
        return_err(err);
    }));

    let pid = sys_call_linux_fork();
    if (sys_call_linux_syscall_isErr(pid)) {
        if (std_in.needs_close_child) proc__linux_closeIf(std_in.child_fd);
        if (std_out.needs_close_child) proc__linux_closeIf(std_out.child_fd);
        if (std_err.needs_close_child) proc__linux_closeIf(std_err.child_fd);
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(pid)));
    }
    if (pid == 0) {
        proc__linux_childExec(cmd, exe, scratch->argv, child_env, std_in, std_out, std_err);
    }

    if (std_in.needs_close_child) proc__linux_closeIf(std_in.child_fd);
    if (std_out.needs_close_child) proc__linux_closeIf(std_out.child_fd);
    if (std_err.needs_close_child) proc__linux_closeIf(std_err.child_fd);

    return_ok({
        .handle = none(),
        .id = as$(proc_Child_Id)(pid),
        .std_in = std_in.parent_,
        .std_out = std_out.parent_,
        .std_err = std_err.parent_,
    });
} $unguarded(fn);

$static fn_((proc__linux_executablePath(S$u8 out_buf))(E$S$u8) $scope) {
    let read = sys_call_linux_readlinkat(sys_call_linux_AT_FDCWD, "/proc/self/exe", as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(read)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(read)));
    if (as$(usize)(read) >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(read))));
} $unscoped(fn);

$static fn_((proc__linux_currentPath(S$u8 out_buf))(E$S$u8) $scope) {
    let rc = sys_call_linux_getcwd(as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(rc)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
    let len = mem_lenZ0$u8(out_buf.ptr);
    return_ok(S_prefix((out_buf)(len)));
} $unscoped(fn);

$static fn_((proc__linux_setCurrentPath(S_const$u8 path))(E$void) $guard) {
    let scratch_raw = try_(proc__linux_allocScratch());
    defer_(proc__linux_freeScratch(scratch_raw));
    let scratch = ptrAlignCast$((proc__linux_SpawnScratch*)(scratch_raw));
    let raw = try_(proc__linux_copyZ(path, (S$u8){ .ptr = as$(P$u8)(scratch->path_buf), .len = proc__linux_path_len_max }));
    let rc = sys_call_linux_chdir(as$(const char*)(raw));
    if (sys_call_linux_syscall_isErr(rc)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
    return_ok({});
} $unguarded(fn);

$static fn_((proc__linux_spawn(proc_Cmd cmd))(E$proc_Child)) {
    return proc__linux_spawnImpl(cmd, none$((O$S_const$u8)));
};

$static fn_((proc__linux_appendU64(S$u8 out, usize* pos, u64 value))(bool)) {
    char tmp[20] = { 0 };
    usize len = 0;
    do {
        tmp[len++] = as$(char)(u8_c('0') + value % 10);
        value /= 10;
    } while (value != 0);
    if (*pos + len > out.len) return false;
    while (len != 0) {
        *S_at((out)[(*pos)++]) = as$(u8)(tmp[--len]);
    }
    return true;
};

$static fn_((proc__linux_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $guard) {
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    let scratch_raw = try_(proc__linux_allocScratch());
    defer_(proc__linux_freeScratch(scratch_raw));
    let scratch = ptrAlignCast$((proc__linux_SpawnScratch*)(scratch_raw));
    let out = (S$u8){ .ptr = as$(P$u8)(scratch->path_buf), .len = proc__linux_path_len_max };
    let prefix = u8_l("/proc/self/fd/");
    var_(pos, usize) = 0;
    mem_copyBytes(S_prefix((out)(prefix.len)), prefix);
    pos += prefix.len;
    if (!proc__linux_appendU64(out, &pos, as$(u64)(fs_Dir_handle(dir)))) return_err(E_cause$proc_ResourceLimitReached());
    if (pos + 1 > out.len) return_err(E_cause$proc_ResourceLimitReached());
    *S_at((out)[pos++]) = u8_c('/');
    let arg0 = *S_at((cmd.argv)[0]);
    if (pos + arg0.len + 1 > out.len) return_err(E_cause$proc_ResourceLimitReached());
    mem_copyBytes(S_slice((out)$r(pos, pos + arg0.len)), arg0);
    pos += arg0.len;
    *S_at((out)[pos]) = 0;
    return proc__linux_spawnImpl(cmd, some$((O$S_const$u8)(S_prefix((out.as_const)(pos)))));
} $unguarded(fn);
#endif /* plat_is_linux */

$attr($maybe_unused)
$static fn_((proc__unsupported_executablePath(S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupported_currentPath(S$u8 out_buf))(E$S$u8) $scope) {
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupported_setCurrentPath(S_const$u8 path))(E$void) $scope) {
    let_ignore = path;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupported_spawn(proc_Cmd cmd))(E$proc_Child) $scope) {
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupported_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $scope) {
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$static let proc__executablePath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_executablePath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_executablePath),
        pp_else_(proc__unsupported_executablePath)
    )));
$static let proc__currentPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_currentPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_currentPath),
        pp_else_(proc__unsupported_currentPath)
    )));
$static let proc__setCurrentPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_setCurrentPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_setCurrentPath),
        pp_else_(proc__unsupported_setCurrentPath)
    )));
$static let proc__spawn = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_spawn),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_spawn),
        pp_else_(proc__unsupported_spawn)
    )));
$static let proc__spawnPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_spawnPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_spawnPath),
        pp_else_(proc__unsupported_spawnPath)
    )));

fn_((proc_executablePath(S$u8 out_buf))(E$S$u8)) {
    return proc__executablePath(out_buf);
};

fn_((proc_currentPath(S$u8 out_buf))(E$S$u8)) {
    return proc__currentPath(out_buf);
};

fn_((proc_setCurrentPath(S_const$u8 path))(E$void)) {
    return proc__setCurrentPath(path);
};

fn_((proc_spawn(proc_Cmd cmd))(E$proc_Child)) {
    return proc__spawn(cmd);
};

fn_((proc_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child)) {
    return proc__spawnPath(dir, cmd);
};

fn_((proc_replace(proc_Cmd cmd))(E$void) $scope) {
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_replacePath(fs_Dir dir, proc_Cmd cmd))(E$void) $scope) {
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);
