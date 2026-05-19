#include "dh/proc.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/os/windows/file.h"
#include "dh/os/windows/handle.h"
#include "dh/os/windows/mem.h"
#include "dh/os/windows/proc.h"
#endif

#define proc__path_max (usize_(32768))
#define proc__path_prefix_len (usize_(4))

$static fn_((proc__heapAlloc(usize len))(u8*)) {
#if plat_is_windows
    return ptrCast$((u8*)(HeapAlloc(GetProcessHeap(), 0, len)));
#else
    let_ignore = len;
    return null;
#endif
}

$static fn_((proc__heapFree(u8* ptr))(void)) {
#if plat_is_windows
    if (ptr != null) claim_assert(HeapFree(GetProcessHeap(), 0, ptr));
#else
    let_ignore = ptr;
#endif
}

$static fn_((proc__mapWinErr(DWORD err))(proc_E)) {
#if plat_is_windows
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$FileNotFoundProc();
    case ERROR_ACCESS_DENIED: return E_cause$AccessDeniedProc();
    case ERROR_INVALID_NAME: return E_cause$InvalidNameProc();
    case ERROR_DIRECTORY: return E_cause$NotDirProc();
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$SystemResourcesProc();
    case ERROR_TOO_MANY_OPEN_FILES: $fallthrough;
    case ERROR_SHARING_BUFFER_EXCEEDED: return E_cause$ResourceLimitReachedProc();
    default_() return E_cause$SystemResourcesProc() $end(default);
    }
#else
    let_ignore = err;
    return E_cause$OperationUnsupportedProc();
#endif
}

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

$static fn_((proc__stdioNull(bool for_read))(E$proc_Handle) $scope) {
    let path = for_read ? "NUL" : "NUL";
    let access = for_read ? GENERIC_READ : GENERIC_WRITE;
    let handle = CreateFileA(path, access, FILE_SHARE_READ | FILE_SHARE_WRITE, null, OPEN_EXISTING, 0, null);
    if (handle == INVALID_HANDLE_VALUE) return_err(proc__mapWinErr(GetLastError()));
    let inheritable = catch_((proc__dupInheritable(handle))(
        $ignore, (CloseHandle(handle), INVALID_HANDLE_VALUE)
    ));
    claim_assert(CloseHandle(handle));
    return_ok(inheritable);
} $unscoped(fn);

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
    if (buf == null) return_err(E_cause$SystemResourcesProc());
    mem_copyBytes((S$u8){ .ptr = buf, .len = src.len }, src);
    buf[src.len] = 0;
    return_ok((proc__OwnedBuf){
        .ptr = buf,
        .len = src.len,
    });
} $unscoped(fn);

$static fn_((proc__trimWinNtPrefix(proc__OwnedBuf* self))(void)) {
    claim_assert_nonnull(self);
    if (self->ptr == null || self->len < proc__path_prefix_len) return;
    if (
        self->ptr[0] == u8_c('\\') && self->ptr[1] == u8_c('\\') && self->ptr[2] == u8_c('?') && self->ptr[3] == u8_c('\\')
    ) {
        raw_memmove(self->ptr, self->ptr + proc__path_prefix_len, self->len + 1 - proc__path_prefix_len);
        self->len -= proc__path_prefix_len;
    }
}

$static fn_((proc__dirPathAlloc(fs_Dir dir))(E$proc__OwnedBuf) $scope) {
    let need = GetFinalPathNameByHandleA(dir.handle, null, 0, 0);
    if (need == 0) return_err(proc__mapWinErr(GetLastError()));
    let buf = proc__heapAlloc(as$(usize)(need) + 1);
    if (buf == null) return_err(E_cause$SystemResourcesProc());
    let wrote = GetFinalPathNameByHandleA(dir.handle, as$(LPSTR)(buf), need + 1, 0);
    if (wrote == 0) {
        proc__heapFree(buf);
        return_err(proc__mapWinErr(GetLastError()));
    }
    var owned = (proc__OwnedBuf){
        .ptr = buf,
        .len = as$(usize)(wrote),
    };
    proc__trimWinNtPrefix(&owned);
    return_ok(owned);
} $unscoped(fn);

$static fn_((proc__resolvePathAlloc(S_const$u8 base, S_const$u8 sub_path))(E$proc__OwnedBuf) $scope) {
    if (fs_path_isAbs(sub_path)) return_(proc__dupSliceZ(sub_path));
    let need_sep = base.len != 0 && sub_path.len != 0 && base.ptr[base.len - 1] != u8_c('\\') && base.ptr[base.len - 1] != u8_c('/');
    let len = base.len + sub_path.len + (need_sep ? 1 : 0);
    let buf = proc__heapAlloc(len + 1);
    if (buf == null) return_err(E_cause$SystemResourcesProc());
    var_(pos, usize) = 0;
    mem_copyBytes((S$u8){ .ptr = buf + pos, .len = base.len }, base);
    pos += base.len;
    if (need_sep) buf[pos++] = fs_path_sep;
    mem_copyBytes((S$u8){ .ptr = buf + pos, .len = sub_path.len }, sub_path);
    pos += sub_path.len;
    buf[pos] = 0;
    return_ok((proc__OwnedBuf){
        .ptr = buf,
        .len = pos,
    });
} $unscoped(fn);

$static fn_((proc__envBlockAlloc(S$S_const$u8 env))(E$proc__OwnedBuf) $scope) {
    if (env.len == 0) {
        return_ok((proc__OwnedBuf){
            .ptr = null,
            .len = 0,
        });
    }
    var_(len, usize) = 1;
    for (usize i = 0; i < env.len; ++i) len += env.ptr[i].len + 1;

    let buf = proc__heapAlloc(len);
    if (buf == null) return_err(E_cause$SystemResourcesProc());

    var_(pos, usize) = 0;
    for (usize i = 0; i < env.len; ++i) {
        let item = env.ptr[i];
        mem_copyBytes((S$u8){ .ptr = buf + pos, .len = item.len }, item);
        pos += item.len;
        buf[pos++] = 0;
    }
    buf[pos++] = 0;
    claim_assert(pos == len);
    return_ok((proc__OwnedBuf){
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
            return_ok((proc__ResolvedStdIO){
                .child = null,
                .parent_pipe = { .is_present = false, .file = cleared() },
                .needs_close_child = false,
            });
        }
        let child = try_(proc__dupInheritable(base));
        return_ok((proc__ResolvedStdIO){
            .child = child,
            .parent_pipe = { .is_present = false, .file = cleared() },
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_file: {
        let child = try_(proc__dupInheritable(fs_File_handle(spec.file)));
        return_ok((proc__ResolvedStdIO){
            .child = child,
            .parent_pipe = { .is_present = false, .file = cleared() },
            .needs_close_child = true,
        });
    }
    case proc_StdIO_Tag_ignore: $fallthrough;
    case proc_StdIO_Tag_close: {
        let child = try_(proc__stdioNull(for_read));
        return_ok((proc__ResolvedStdIO){
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
            return_ok((proc__ResolvedStdIO){
                .child = read_end,
                .parent_pipe = {
                    .is_present = true,
                    .file = fs_File_Handle_promote(write_end, fs_File_Flags_default),
                },
                .needs_close_child = true,
            });
        }
        claim_assert(SetHandleInformation(read_end, HANDLE_FLAG_INHERIT, 0));
        return_ok((proc__ResolvedStdIO){
            .child = write_end,
            .parent_pipe = {
                .is_present = true,
                .file = fs_File_Handle_promote(read_end, fs_File_Flags_default),
            },
            .needs_close_child = true,
        });
    }
    default_() return_err(E_cause$OperationUnsupportedProc()) $end(default);
    }
} $unscoped(fn);

$static fn_((proc__appendQuoted(S$u8 out, usize* used, S_const$u8 arg))(bool)) {
    var_(needs_quotes, bool) = arg.len == 0;
    for (usize i = 0; i < arg.len; ++i) {
        let ch = arg.ptr[i];
        if (ch == u8_c(' ') || ch == u8_c('\t') || ch == u8_c('"')) {
            needs_quotes = true;
            break;
        }
    }
    if (!needs_quotes) {
        if (*used + arg.len > out.len) return false;
        mem_copyBytes((S$u8){ .ptr = out.ptr + *used, .len = arg.len }, arg);
        *used += arg.len;
        return true;
    }
    if (*used + 2 > out.len) return false;
    out.ptr[(*used)++] = u8_c('"');
    var_(backslashes, usize) = 0;
    for (usize i = 0; i < arg.len; ++i) {
        let ch = arg.ptr[i];
        if (ch == u8_c('\\')) {
            backslashes += 1;
            continue;
        }
        if (ch == u8_c('"')) {
            if (*used + backslashes * 2 + 1 > out.len) return false;
            for (usize i = 0; i < backslashes * 2 + 1; ++i) out.ptr[(*used)++] = u8_c('\\');
            out.ptr[(*used)++] = u8_c('"');
            backslashes = 0;
            continue;
        }
        if (*used + backslashes + 1 > out.len) return false;
        for (usize i = 0; i < backslashes; ++i) out.ptr[(*used)++] = u8_c('\\');
        backslashes = 0;
        out.ptr[(*used)++] = ch;
    }
    if (*used + backslashes * 2 + 1 > out.len) return false;
    for (usize i = 0; i < backslashes * 2; ++i) out.ptr[(*used)++] = u8_c('\\');
    out.ptr[(*used)++] = u8_c('"');
    return true;
}

$static fn_((proc__commandLine(S$S_const$u8 argv, S$u8 out))(E$S$u8) $scope) {
    var_(used, usize) = 0;
    for (usize i = 0; i < argv.len; ++i) {
        let arg = &argv.ptr[i];
        if (i != 0) {
            if (used + 1 > out.len) return_err(E_cause$ResourceLimitReachedProc());
            out.ptr[used++] = u8_c(' ');
        }
        if (!proc__appendQuoted(out, &used, *arg)) {
            return_err(E_cause$ResourceLimitReachedProc());
        }
    }
    if (used >= out.len) return_err(E_cause$ResourceLimitReachedProc());
    out.ptr[used] = 0;
    return_ok((S$u8){ .ptr = out.ptr, .len = used });
} $unscoped(fn);

$static fn_((proc__spawnImpl(proc_Cmd cmd, LPCSTR application_name, LPCSTR current_dir))(E$proc_Child) $scope) {
    if (cmd.argv.len == 0) return_err(E_cause$InvalidNameProc());

    var_(cmdline_buf, proc__OwnedBuf) = {
        .ptr = proc__heapAlloc(proc__path_max),
        .len = proc__path_max,
    };
    if (cmdline_buf.ptr == null) return_err(E_cause$SystemResourcesProc());
    let _cmdline = catch_((proc__commandLine(cmd.argv, (S$u8){ .ptr = cmdline_buf.ptr, .len = cmdline_buf.len }))(err, {
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        return_err(err);
    }));
    let_ignore = _cmdline;
    let env_block = catch_((proc__envBlockAlloc(cmd.env))(err, {
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        return_err(err);
    }));

    let std_in = catch_((proc__resolveStdIO(cmd.std_in, STD_INPUT_HANDLE))(err, {
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        var cleanup_env = env_block;
        proc__ownedBuf_free(&cleanup_env);
        return_err(err);
    }));
    let std_out_res = proc__resolveStdIO(cmd.std_out, STD_OUTPUT_HANDLE);
    if (!std_out_res.is_ok) {
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        var cleanup_env = env_block;
        proc__ownedBuf_free(&cleanup_env);
        var cleanup = std_in;
        proc__resolvedStdIO_close(&cleanup);
        return_err(std_out_res.payload.err);
    }
    let std_out = std_out_res.payload.ok;
    let std_err_res = proc__resolveStdIO(cmd.std_err, STD_ERROR_HANDLE);
    if (!std_err_res.is_ok) {
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        var cleanup_env = env_block;
        proc__ownedBuf_free(&cleanup_env);
        var cleanup_in = std_in;
        var cleanup_out = std_out;
        proc__resolvedStdIO_close(&cleanup_out);
        proc__resolvedStdIO_close(&cleanup_in);
        return_err(std_err_res.payload.err);
    }
    let std_err = std_err_res.payload.ok;

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
        var cleanup_cmdline = cmdline_buf;
        proc__ownedBuf_free(&cleanup_cmdline);
        var cleanup_env = env_block;
        proc__ownedBuf_free(&cleanup_env);
        var cleanup_in = std_in;
        var cleanup_out = std_out;
        var cleanup_err = std_err;
        proc__resolvedStdIO_close(&cleanup_err);
        proc__resolvedStdIO_close(&cleanup_out);
        proc__resolvedStdIO_close(&cleanup_in);
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

    return_ok((proc_Child){
        .handle = proc_info.hProcess,
        .id = as$(u64)(proc_info.dwProcessId),
        .std_in = std_in.parent_pipe,
        .std_out = std_out.parent_pipe,
        .std_err = std_err.parent_pipe,
    });
} $unscoped(fn);
#endif

fn_((proc_executablePath(S$u8 out_buf))(E$S$u8) $scope) {
#if plat_is_windows
    if (out_buf.len == 0) return_err(E_cause$ResourceLimitReachedProc());
    let wrote = GetModuleFileNameA(null, as$(LPSTR)(out_buf.ptr), as$(DWORD)(out_buf.len));
    if (wrote == 0) return_err(proc__mapWinErr(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$ResourceLimitReachedProc());
    return_ok((S$u8){ .ptr = out_buf.ptr, .len = as$(usize)(wrote) });
#else
    let_ignore = out_buf;
    return_err(E_cause$OperationUnsupportedProc());
#endif
} $unscoped(fn);

fn_((proc_currentPath(S$u8 out_buf))(E$S$u8) $scope) {
#if plat_is_windows
    let wrote = GetCurrentDirectoryA(as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr));
    if (wrote == 0) return_err(proc__mapWinErr(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$ResourceLimitReachedProc());
    return_ok((S$u8){ .ptr = out_buf.ptr, .len = as$(usize)(wrote) });
#else
    let_ignore = out_buf;
    return_err(E_cause$OperationUnsupportedProc());
#endif
} $unscoped(fn);

fn_((proc_setCurrentPath(S_const$u8 path))(E$void) $scope) {
#if plat_is_windows
    let path_z = try_(proc__dupSliceZ(path));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(path_z.ptr))) {
        let err = proc__mapWinErr(GetLastError());
        var cleanup = path_z;
        proc__ownedBuf_free(&cleanup);
        return_err(err);
    }
    var cleanup = path_z;
    proc__ownedBuf_free(&cleanup);
    return_ok({});
#else
    let_ignore = path;
    return_err(E_cause$OperationUnsupportedProc());
#endif
} $unscoped(fn);

fn_((proc_spawn(proc_Cmd cmd))(E$proc_Child) $scope) {
#if plat_is_windows
    if (cmd.cwd == null) return_(proc__spawnImpl(cmd, null, null));
    let cwd = try_(proc__dirPathAlloc(*cmd.cwd));
    let child = catch_((proc__spawnImpl(cmd, null, as$(LPCSTR)(cwd.ptr)))(err, {
        var cleanup = cwd;
        proc__ownedBuf_free(&cleanup);
        return_err(err);
    }));
    var cleanup = cwd;
    proc__ownedBuf_free(&cleanup);
    return_ok(child);
#else
    let_ignore = cmd;
    return_err(E_cause$OperationUnsupportedProc());
#endif
} $unscoped(fn);

fn_((proc_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child) $scope) {
#if plat_is_windows
    if (cmd.argv.len == 0) return_err(E_cause$InvalidNameProc());
    let base = try_(proc__dirPathAlloc(dir));
    let exe_path = catch_((proc__resolvePathAlloc((S_const$u8){ .ptr = base.ptr, .len = base.len }, cmd.argv.ptr[0]))(err, {
        var cleanup = base;
        proc__ownedBuf_free(&cleanup);
        return_err(err);
    }));
    let child = catch_((proc__spawnImpl(cmd, as$(LPCSTR)(exe_path.ptr), null))(err, {
        var cleanup_base = base;
        var cleanup_exe = exe_path;
        proc__ownedBuf_free(&cleanup_exe);
        proc__ownedBuf_free(&cleanup_base);
        return_err(err);
    }));
    var cleanup_base = base;
    var cleanup_exe = exe_path;
    proc__ownedBuf_free(&cleanup_exe);
    proc__ownedBuf_free(&cleanup_base);
    return_ok(child);
#else
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$OperationUnsupportedProc());
#endif
} $unscoped(fn);

fn_((proc_replace(proc_Cmd cmd))(E$void) $scope) {
    let_ignore = cmd;
    return_err(E_cause$OperationUnsupportedProc());
} $unscoped(fn);

fn_((proc_replacePath(fs_Dir dir, proc_Cmd cmd))(E$void) $scope) {
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$OperationUnsupportedProc());
} $unscoped(fn);
