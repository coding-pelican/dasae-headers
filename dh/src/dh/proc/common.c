#include "dh/proc.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/mem.h"
#include "dh/sys/api/windows/proc.h"
#endif

#if plat_is_windows
#define proc__path_max (usize_(32768))
#define proc__path_prefix_len (usize_(4))

$static fn_((proc__heapAlloc(usize len))(u8*)) {
    return ptrCast$((u8*)(HeapAlloc(GetProcessHeap(), 0, len)));
}

$static fn_((proc__heapFree(u8* ptr))(void)) {
    if (ptr != null) claim_assert(HeapFree(GetProcessHeap(), 0, ptr));
}

$static fn_((proc__mapWinErr(DWORD err))(proc_E)) {
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
}
#endif

#if plat_is_windows
$static fn_((proc__dupInheritable(HANDLE src))(E$proc_Handle) $scope) {
    var_(dup, HANDLE) = null;
    if (!DuplicateHandle(
            GetCurrentProcess(), src,
            GetCurrentProcess(), &dup,
            0, TRUE, DUPLICATE_SAME_ACCESS
        )) {
        return_err(proc__mapWinErr(GetLastError()));
    }
    return_ok(dup);
} $unscoped(fn);

$static fn_((proc__stdioNull(bool for_read))(E$proc_Handle) $guard) {
    let path = for_read ? "NUL" : "NUL";
    let access = for_read ? GENERIC_READ : GENERIC_WRITE;
    let handle = CreateFileA(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE, null, OPEN_EXISTING, 0, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(proc__mapWinErr(GetLastError()));
    defer_(claim_assert(CloseHandle(handle)));
    let inheritable = try_(proc__dupInheritable(handle));
    return_ok(inheritable);
} $unguarded(fn);

typedef struct proc__ResolvedStdIO {
    var_(child, HANDLE);
    var_(parent_pipe, proc_Child_Pipe);
    var_(needs_close_child, bool);
} proc__ResolvedStdIO;
T_use_E$(proc__ResolvedStdIO);

typedef struct proc__OwnedBuf {
    var_(ptr, u8*);
    var_(len, usize);
} proc__OwnedBuf;
T_use_E$(proc__OwnedBuf);

$static fn_((proc__resolvedStdIO_close(proc__ResolvedStdIO* self))(void)) {
    claim_assert_nonnull(self);
    if (self->needs_close_child && self->child != null && self->child != INVALID_HANDLE_VALUE) {
        claim_assert(CloseHandle(self->child));
        self->child = INVALID_HANDLE_VALUE;
    }
    if (self->parent_pipe.is_present) {
        fs_File_close(self->parent_pipe.file);
        self->parent_pipe.is_present = false;
    }
}

$static fn_((proc__ownedBuf_free(proc__OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    proc__heapFree(self->ptr);
    self->ptr = null;
    self->len = 0;
}

$static fn_((proc__dupSliceZ(S_const$u8 src))(E$proc__OwnedBuf) $scope) {
    let buf = proc__heapAlloc(src.len + 1);
    if (buf == null) return_err(E_cause$proc_SystemResources());
    let out = P_prefix$((S$u8)(buf)(src.len + 1));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok({
        .ptr = buf,
        .len = src.len,
    });
} $unscoped(fn);

$static fn_((proc__trimWinNtPrefix(proc__OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if (self->ptr == null || self->len < proc__path_prefix_len) return;
    let path = P_prefix$((S$u8)(self->ptr)(self->len + 1));
    if (
        *S_at((path)[0]) == u8_c('\\') && *S_at((path)[1]) == u8_c('\\') && *S_at((path)[2]) == u8_c('?') && *S_at((path)[3]) == u8_c('\\')
    ) {
        mem_moveBytes(
            S_prefix((path)(self->len + 1 - proc__path_prefix_len)),
            S_slice((path.as_const)$r(proc__path_prefix_len, self->len + 1))
        );
        self->len -= proc__path_prefix_len;
    }
}

$static fn_((proc__dirPathAlloc(fs_Dir dir))(E$proc__OwnedBuf) $guard) {
    let need = GetFinalPathNameByHandleA(dir.handle, null, 0, 0);
    if (need == 0) return_err(proc__mapWinErr(GetLastError()));
    let buf = proc__heapAlloc(as$(usize)(need) + 1);
    if (buf == null) return_err(E_cause$proc_SystemResources());
    errdefer_($ignore, proc__heapFree(buf));
    let wrote = GetFinalPathNameByHandleA(dir.handle, as$(LPSTR)(buf), need + 1, 0);
    if (wrote == 0) return_err(proc__mapWinErr(GetLastError()));
    var_(owned, proc__OwnedBuf) = {
        .ptr = buf,
        .len = as$(usize)(wrote),
    };
    proc__trimWinNtPrefix(&owned);
    return_ok(owned);
} $unguarded(fn);

$static fn_((proc__resolvePathAlloc(S_const$u8 base, S_const$u8 sub_path))(E$proc__OwnedBuf) $guard) {
    if (fs_path_isAbs(sub_path)) return_(proc__dupSliceZ(sub_path));
    let base_sub_len = orelse_((usize_addChkd(base.len, sub_path.len))(return_err(E_cause$proc_SystemResources())));
    let cap = orelse_((usize_addChkd(base_sub_len, usize_(2)))(return_err(E_cause$proc_SystemResources())));
    let buf = proc__heapAlloc(cap);
    if (buf == null) return_err(E_cause$proc_SystemResources());
    errdefer_($ignore, proc__heapFree(buf));
    let out = P_prefix$((S$u8)(buf)(cap));
    let joined = catch_((fs_path_join2(base, sub_path, S_prefix((out)(cap - 1))))($ignore, {
        return_err(E_cause$proc_SystemResources());
    }));
    *S_at((out)[joined.len]) = 0;
    return_ok({
        .ptr = buf,
        .len = joined.len,
    });
} $unguarded(fn);

$static fn_((proc__envBlockAlloc(S$S_const$u8 env))(E$proc__OwnedBuf) $scope) {
    if (env.len == 0) {
        return_ok({
            .ptr = null,
            .len = 0,
        });
    }
    var_(len, usize) = 1;
    for_(($s(env))(item)) { len += item->len + 1; } $end(for);

    let buf = proc__heapAlloc(len);
    if (buf == null) return_err(E_cause$proc_SystemResources());

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
        .ptr = buf,
        .len = len,
    });
} $unscoped(fn);

$static fn_((proc__resolveStdIO(proc_StdIO spec, DWORD std_id))(E$proc__ResolvedStdIO) $scope) {
    let for_read = std_id == STD_INPUT_HANDLE;
    switch (spec.tag) {
    case proc_StdIO_Tag_inherit: {
        let base = GetStdHandle(std_id);
        if (base == null || base == INVALID_HANDLE_VALUE) {
            return_ok({
                .child = null,
                .parent_pipe = { .is_present = false, .file = cleared() },
                .needs_close_child = false,
            });
        }
        let child = try_(proc__dupInheritable(base));
        return_ok({
            .child = child,
            .parent_pipe = { .is_present = false, .file = cleared() },
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_file: {
        let child = try_(proc__dupInheritable(fs_File_handle(spec.file)));
        return_ok({
            .child = child,
            .parent_pipe = { .is_present = false, .file = cleared() },
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_ignore: $fallthrough;
    case proc_StdIO_Tag_close: {
        let child = try_(proc__stdioNull(for_read));
        return_ok({
            .child = child,
            .parent_pipe = { .is_present = false, .file = cleared() },
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_pipe: {
        SECURITY_ATTRIBUTES sa = {
            .nLength = sizeOf$(SECURITY_ATTRIBUTES),
            .lpSecurityDescriptor = null,
            .bInheritHandle = TRUE,
        };
        var_(read_end, HANDLE) = null;
        var_(write_end, HANDLE) = null;
        if (!CreatePipe(&read_end, &write_end, &sa, 0)) {
            return_err(proc__mapWinErr(GetLastError()));
        }
        if (for_read) {
            claim_assert(SetHandleInformation(write_end, HANDLE_FLAG_INHERIT, 0));
            return_ok({
                .child = read_end,
                .parent_pipe = {
                    .is_present = true,
                    .file = fs_File_Handle_promote(write_end, fs_File_Flags_default),
                },
                .needs_close_child = true,
            });
        }
        claim_assert(SetHandleInformation(read_end, HANDLE_FLAG_INHERIT, 0));
        return_ok({
            .child = write_end,
            .parent_pipe = {
                .is_present = true,
                .file = fs_File_Handle_promote(read_end, fs_File_Flags_default),
            },
            .needs_close_child = true,
        });
    }
    default_() return_err(E_cause$proc_OperationUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((proc__appendQuoted(S$u8 out, usize used, S_const$u8 arg))(O$usize) $scope) {
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

$static fn_((proc__commandLine(S$S_const$u8 argv, S$u8 out))(E$S$u8) $scope) {
    var_(used, usize) = 0;
    for_(($rf(0), $s(argv))(i, arg)) {
        if (i != 0) {
            if (used + 1 > out.len) return_err(E_cause$proc_ResourceLimitReached());
            *S_at((out)[used++]) = u8_c(' ');
        }
        used = orelse_((proc__appendQuoted(out, used, *arg))(return_err(E_cause$proc_ResourceLimitReached())));
    } $end(for);
    if (used >= out.len) return_err(E_cause$proc_ResourceLimitReached());
    *S_at((out)[used]) = 0;
    return_ok(S_prefix((out)(used)));
} $unscoped(fn);

$static fn_((proc__spawnImpl(proc_Cmd cmd, LPCSTR application_name, LPCSTR current_dir))(E$proc_Child) $guard) {
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var_(cmdline_buf, proc__OwnedBuf) = {
        .ptr = proc__heapAlloc(proc__path_max),
        .len = proc__path_max,
    };
    if (cmdline_buf.ptr == null) return_err(E_cause$proc_SystemResources());
    errdefer_($ignore, proc__ownedBuf_free(&cmdline_buf));
    let _cmdline = try_(proc__commandLine(cmd.argv, P_prefix$((S$u8)(cmdline_buf.ptr)(cmdline_buf.len))));
    let_ignore = _cmdline;
    var_(env_block, proc__OwnedBuf) = try_(proc__envBlockAlloc(cmd.env));
    errdefer_($ignore, proc__ownedBuf_free(&env_block));

    var_(std_in, proc__ResolvedStdIO) = try_(proc__resolveStdIO(cmd.std_in, STD_INPUT_HANDLE));
    errdefer_($ignore, proc__resolvedStdIO_close(&std_in));
    var_(std_out, proc__ResolvedStdIO) = try_(proc__resolveStdIO(cmd.std_out, STD_OUTPUT_HANDLE));
    errdefer_($ignore, proc__resolvedStdIO_close(&std_out));
    var_(std_err, proc__ResolvedStdIO) = try_(proc__resolveStdIO(cmd.std_err, STD_ERROR_HANDLE));
    errdefer_($ignore, proc__resolvedStdIO_close(&std_err));

    STARTUPINFOA startup = cleared();
    startup.cb = sizeOf$(STARTUPINFOA);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = std_in.child;
    startup.hStdOutput = std_out.child;
    startup.hStdError = std_err.child;

    PROCESS_INFORMATION proc_info = cleared();
    var_(flags, DWORD) = 0;
    if (cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessA(
            application_name,
            as$(LPSTR)(cmdline_buf.ptr),
            null,
            null,
            TRUE,
            flags,
            env_block.ptr,
            current_dir,
            &startup,
            &proc_info
        )) {
        return_err(proc__mapWinErr(GetLastError()));
    }

    claim_assert(CloseHandle(proc_info.hThread));
    var cleanup_cmdline = cmdline_buf;
    proc__ownedBuf_free(&cleanup_cmdline);
    var cleanup_env = env_block;
    proc__ownedBuf_free(&cleanup_env);
    if (std_in.needs_close_child && std_in.child != null) claim_assert(CloseHandle(std_in.child));
    if (std_out.needs_close_child && std_out.child != null) claim_assert(CloseHandle(std_out.child));
    if (std_err.needs_close_child && std_err.child != null) claim_assert(CloseHandle(std_err.child));

    return_ok({
        .handle = proc_info.hProcess,
        .id = as$(u64)(proc_info.dwProcessId),
        .std_in = std_in.parent_pipe,
        .std_out = std_out.parent_pipe,
        .std_err = std_err.parent_pipe,
    });
} $unguarded(fn);
#endif

fn_((proc_executablePath(S$u8 out_buf))(E$S$u8) $scope) {
#if plat_is_windows
    if (out_buf.len == 0) return_err(E_cause$proc_ResourceLimitReached());
    let wrote = GetModuleFileNameA(null, as$(LPSTR)(out_buf.ptr), as$(DWORD)(out_buf.len));
    if (wrote == 0) return_err(proc__mapWinErr(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
#else
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unscoped(fn);

fn_((proc_currentPath(S$u8 out_buf))(E$S$u8) $scope) {
#if plat_is_windows
    let wrote = GetCurrentDirectoryA(as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr));
    if (wrote == 0) return_err(proc__mapWinErr(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
#else
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unscoped(fn);

fn_((proc_setCurrentPath(S_const$u8 path))(E$void) $guard) {
#if plat_is_windows
    var_(path_z, proc__OwnedBuf) = try_(proc__dupSliceZ(path));
    defer_(proc__ownedBuf_free(&path_z));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(path_z.ptr))) {
        return_err(proc__mapWinErr(GetLastError()));
    }
    return_ok({});
#else
    let_ignore = path;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unguarded(fn);

fn_((proc_spawn(proc_Cmd cmd))(E$proc_Child) $guard) {
#if plat_is_windows
    if (cmd.cwd == null) return_(proc__spawnImpl(cmd, null, null));
    var_(cwd, proc__OwnedBuf) = try_(proc__dirPathAlloc(*cmd.cwd));
    defer_(proc__ownedBuf_free(&cwd));
    let child = try_(proc__spawnImpl(cmd, null, as$(LPCSTR)(cwd.ptr)));
    return_ok(child);
#else
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unguarded(fn);

fn_((proc_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $guard) {
#if plat_is_windows
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    var_(base, proc__OwnedBuf) = try_(proc__dirPathAlloc(dir));
    defer_(proc__ownedBuf_free(&base));
    var_(exe_path, proc__OwnedBuf) = try_(proc__resolvePathAlloc(P_prefix$((S$u8)(base.ptr)(base.len)).as_const, *S_at((cmd.argv)[0])));
    defer_(proc__ownedBuf_free(&exe_path));
    let child = try_(proc__spawnImpl(cmd, as$(LPCSTR)(exe_path.ptr), null));
    return_ok(child);
#else
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unguarded(fn);

fn_((proc_replace(proc_Cmd cmd))(E$void) $scope) {
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

fn_((proc_replacePath(fs_Dir dir, proc_Cmd cmd))(E$void) $scope) {
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);
