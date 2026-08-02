#include "private/share.h"
#include "dh/fs/path.h"
#include "dh/mem/common.h"
#include "dh/start.h"
#include "dh/unicode.h"

#if plat_is_windows
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/sync.h"
#endif
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif
#if plat_is_posix
#include "dh/sys/posix.h"
#endif

/*========== Internal Declarations ==========================================*/

$extern fn_((proc__directGetUserInfo(P$raw ctx, S_const$u8 name))(proc_GetUserInfo_E$proc_UserInfo));
$extern fn_((proc__directGetBaseAddress(P$raw ctx))(proc_GetBaseAddress_E$usize));
$attr($no_return)
$extern fn_((proc__directExit(P$raw ctx, u8 status))(void));
$attr($no_return $branch_cold)
$extern fn_((proc__directAbort(P$raw ctx))(void));

$attr($maybe_unused)
$static fn_((proc__unsupportedExePath(
    S$u8 out_buf
))(proc_ExePath_E$S$u8));
$attr($maybe_unused)
$static fn_((proc__unsupportedCurrPath(
    S$u8 out_buf
))(proc_CurrPath_E$S$u8));
$attr($maybe_unused)
$static fn_((proc__unsupportedSetCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void));
$attr($maybe_unused)
$static fn_((proc__unsupportedSetCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void));
$attr($maybe_unused)
$static fn_((proc__unsupportedSpawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child));
$attr($maybe_unused)
$static fn_((proc__unsupportedSpawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child));
$attr($maybe_unused)
$static fn_((proc__unsupportedReplace(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Replace_Opts opts
))(proc_Replace_E$void));
$attr($maybe_unused)
$static fn_((proc__unsupportedReplacePath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void));
$attr($maybe_unused)
$static fn_((proc__unsupportedChildWait(
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Trm));
$attr($maybe_unused)
$static fn_((proc__noChildKill(proc_Child* self))(void));
$attr($no_return $maybe_unused)
$static fn_((proc__unsupportedAbort(void))(void));

pp_if_(plat_is_windows)(pp_then_(
    $static fn_((proc__windows_exePath(
        S$u8 out_buf
    ))(proc_ExePath_E$S$u8));
    $static fn_((proc__windows_currPath(
        S$u8 out_buf
    ))(proc_CurrPath_E$S$u8));
    $static fn_((proc__windows_setCurrPath(
        P$raw ctx,
        mem_Alctr gpa,
        S_const$u8 path
    ))(proc_SetCurrPath_E$void));
    $static fn_((proc__windows_setCurrDir(
        P$raw ctx,
        mem_Alctr gpa,
        fs_Dir dir
    ))(proc_SetCurrDir_E$void));
    $static fn_((proc__windows_spawn(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    $static fn_((proc__windows_spawnPath(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        fs_Dir dir,
        proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    $static fn_((proc__windows_childWait(
        proc_Child * self
    ))(proc_Child_Wait_E$proc_Child_Trm));
    $static fn_((proc__windows_childKill(proc_Child * self))(void));
    $attr($no_return)
    $static fn_((proc__windowsAbort(void))(void));
));

pp_if_(plat_is_linux)(pp_then_(
    $static fn_((proc__linux_exePath(
        S$u8 out_buf
    ))(proc_ExePath_E$S$u8));
    $static fn_((proc__linux_currPath(
        S$u8 out_buf
    ))(proc_CurrPath_E$S$u8));
    $static fn_((proc__linux_setCurrPath(
        P$raw ctx,
        mem_Alctr gpa,
        S_const$u8 path
    ))(proc_SetCurrPath_E$void));
    $static fn_((proc__linux_setCurrDir(
        P$raw ctx,
        mem_Alctr gpa,
        fs_Dir dir
    ))(proc_SetCurrDir_E$void));
    $static fn_((proc__linux_spawn(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    $static fn_((proc__linux_spawnPath(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        fs_Dir dir,
        proc_Spawn_Opts cmd
    ))(proc_Spawn_E$proc_Child));
    $static fn_((proc__linux_replace(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        proc_Replace_Opts opts
    ))(proc_Replace_E$void));
    $static fn_((proc__linux_replacePath(
        P$raw ctx,
        mem_Alctr gpa,
        proc_Env env,
        fs_Dir dir,
        proc_Replace_Opts opts
    ))(proc_Replace_E$void));
    $static fn_((proc__linux_childWait(
        proc_Child * self
    ))(proc_Child_Wait_E$proc_Child_Trm));
    $static fn_((proc__linux_childKill(proc_Child * self))(void));
    $attr($no_return $maybe_unused)
    $static fn_((proc__linuxAbort(void))(void));
));

$static fn_((proc__nativeExePath(
    P$raw ctx,
    S$u8 out_buf
))(proc_ExePath_E$S$u8));
$static fn_((proc__nativeCurrPath(
    P$raw ctx,
    S$u8 out_buf
))(proc_CurrPath_E$S$u8));
$static fn_((proc__nativeChildWait(
    P$raw ctx,
    proc_Child* child
))(proc_Child_Wait_E$proc_Child_Trm));
$static fn_((proc__nativeChildKill(
    P$raw ctx,
    proc_Child* child
))(void));

$static let proc__exePath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_exePath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_exePath),
        pp_else_(proc__unsupportedExePath)
    )));
$static let proc__currPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_currPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_currPath),
        pp_else_(proc__unsupportedCurrPath)
    )));
$static let proc__setCurrPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_setCurrPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_setCurrPath),
        pp_else_(proc__unsupportedSetCurrPath)
    )));
$static let proc__setCurrDir = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_setCurrDir),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_setCurrDir),
        pp_else_(proc__unsupportedSetCurrDir)
    )));
$static let proc__spawn = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_spawn),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_spawn),
        pp_else_(proc__unsupportedSpawn)
    )));
$static let proc__spawnPath = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_spawnPath),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_spawnPath),
        pp_else_(proc__unsupportedSpawnPath)
    )));
$static let proc__replace = pp_if_(plat_is_linux)(
    pp_then_(proc__linux_replace),
    pp_else_(proc__unsupportedReplace));
$static let proc__replacePath = pp_if_(plat_is_linux)(
    pp_then_(proc__linux_replacePath),
    pp_else_(proc__unsupportedReplacePath));
$static let proc__childWait = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_childWait),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_childWait),
        pp_else_(proc__unsupportedChildWait)
    )));
$static let proc__childKill = pp_if_(plat_is_windows)(
    pp_then_(proc__windows_childKill),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc__linux_childKill),
        pp_else_(proc__noChildKill)
    )));
$static let_(proc__direct_vtbl, proc_Self_VTbl) = {
    .exePathFn = proc__nativeExePath,
    .currPathFn = proc__nativeCurrPath,
    .setCurrPathFn = proc__setCurrPath,
    .setCurrDirFn = proc__setCurrDir,
    .replaceFn = proc__replace,
    .replacePathFn = proc__replacePath,
    .spawnFn = proc__spawn,
    .spawnPathFn = proc__spawnPath,
    .getUserInfoFn = proc__directGetUserInfo,
    .getBaseAddressFn = proc__directGetBaseAddress,
    .child = {
        .waitFn = proc__nativeChildWait,
        .killFn = proc__nativeChildKill,
    },
    .exitFn = proc__directExit,
    .abortFn = proc__directAbort,
};

/*========== External Definitions ===========================================*/

fn_((proc_direct(void))(proc_direct_E$proc_Self) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_linux))(
        pp_then_(
            $static var_(ctx, Void) $undefined_static;
            return_ok(proc_ensureValid((proc_Self){
                .ctx = &ctx,
                .vtbl = &proc__direct_vtbl,
            }))
        ),
        pp_else_(
            return_err(E_cause$proc_direct_Unsupported())
        ));
} $unscoped(fn);


/*========== Internal Definitions ===========================================*/

/*--- Common ---*/

$static fn_((proc__childClear(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->io.in)(file)) fs_File_close(file);
    if_some((self->io.out)(file)) fs_File_close(file);
    if_some((self->io.err)(file)) fs_File_close(file);
    asg_l((&self->io.in)(none()));
    asg_l((&self->io.out)(none()));
    asg_l((&self->io.err)(none()));
    asg_l((&self->handle)(none()));
    self->id = 0;
};

$static fn_((proc__nativeExePath(
    P$raw ctx,
    S$u8 out_buf
))(proc_ExePath_E$S$u8)) {
    claim_assert_nonnull(ctx);
    return proc__exePath(out_buf);
};

$static fn_((proc__nativeCurrPath(
    P$raw ctx,
    S$u8 out_buf
))(proc_CurrPath_E$S$u8)) {
    claim_assert_nonnull(ctx);
    return proc__currPath(out_buf);
};

$static fn_((proc__nativeChildWait(
    P$raw ctx,
    proc_Child* child
))(proc_Child_Wait_E$proc_Child_Trm)) {
    claim_assert_nonnull(ctx);
    return proc__childWait(child);
};

$static fn_((proc__nativeChildKill(
    P$raw ctx,
    proc_Child* child
))(void)) {
    claim_assert_nonnull(ctx);
    proc__childKill(child);
};


/*--- Unsupported ---*/

$attr($maybe_unused)
$static fn_((proc__unsupportedExePath(
    S$u8 out_buf
))(proc_ExePath_E$S$u8) $scope) {
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedCurrPath(
    S$u8 out_buf
))(proc_CurrPath_E$S$u8) $scope) {
    let_ignore = out_buf;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedSetCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = dir;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedSetCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = path;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedSpawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = env;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedSpawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = env;
    let_ignore = dir;
    let_ignore = cmd;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedReplace(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Replace_Opts opts
))(proc_Replace_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = env;
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedReplacePath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let_ignore = env;
    let_ignore = dir;
    let_ignore = opts;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__unsupportedChildWait(
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    claim_assert_nonnull(self);
    claim_unreachable;
    return_err(E_cause$proc_SystemResources());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((proc__noChildKill(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
};

$attr($no_return)
$static fn_((proc__unsupportedAbort(void))(void)) {
    start_exit(134);
};

/*--- Windows ---*/

#if plat_is_windows
#define proc__windows_path_max (usize_(32768))
#define proc__windows_path_prefix_len (usize_(4))

$static fn_((proc__windows_alloc(mem_Alctr gpa, usize len))(O$P$u8)) {
    return mem_Alctr_rawAlloc($trace gpa, len, alignOfLog2$(u16));
};

$static fn_((proc__windows_free(mem_Alctr gpa, P$u8 ptr, usize len))(void)) {
    claim_assert_nonnull(ptr);
    mem_Alctr_rawFree(
        $trace gpa,
        P_prefix$((S$u8)(ptr)(len)),
        alignOfLog2$(u16)
    );
};

$static fn_((proc__windows_mapError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_BAD_FORMAT: $fallthrough;
    case ERROR_BAD_EXE_FORMAT: $fallthrough;
    case ERROR_EXE_MACHINE_TYPE_MISMATCH: return E_cause$proc_InvalidExe().any;
    case ERROR_SHARING_VIOLATION: return E_cause$proc_FileBusy().any;
    case ERROR_INVALID_NAME: return E_cause$proc_InvalidName().any;
    case ERROR_DIRECTORY: return E_cause$proc_NotDir().any;
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$proc_SystemResources().any;
    case ERROR_TOO_MANY_OPEN_FILES: return E_cause$proc_ProcessFdQuotaExceeded().any;
    case ERROR_SHARING_BUFFER_EXCEEDED: return E_cause$proc_ResourceLimitReached().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__windows_mapPathQueryError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_INSUFFICIENT_BUFFER: return E_cause$proc_ResourceLimitReached().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__windows_mapSetPathError(DWORD err))(EAny)) {
    switch (err) {
    case ERROR_FILE_NOT_FOUND: $fallthrough;
    case ERROR_PATH_NOT_FOUND: return E_cause$proc_FileNotFound().any;
    case ERROR_ACCESS_DENIED: return E_cause$proc_AccessDenied().any;
    case ERROR_INVALID_NAME: return E_cause$proc_InvalidName().any;
    case ERROR_DIRECTORY: return E_cause$proc_NotDir().any;
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$proc_SystemResources().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
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
    var_(gpa, mem_Alctr);
    var_(ptr, O$P$u8);
    var_(len, usize);
    var_(alloc_len, usize);
} proc__windows_OwnedBuf;
T_use_E$(proc__windows_OwnedBuf);

typedef struct proc__windows_OwnedWideBuf {
    var_(gpa, mem_Alctr);
    var_(ptr, O$P$u16);
    var_(len, usize);
    var_(alloc_len, usize);
} proc__windows_OwnedWideBuf;
T_use_E$(proc__windows_OwnedWideBuf);

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
    if_some((self->ptr)(ptr)) proc__windows_free(self->gpa, ptr, self->alloc_len);
    asg_l((&self->ptr)(none()));
    self->len = 0;
    self->alloc_len = 0;
};

$static fn_((proc__windows_OwnedWideBuf_fini(proc__windows_OwnedWideBuf* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->ptr)(ptr)) {
        proc__windows_free(self->gpa, ptrCast$((P$u8)(ptr)), self->alloc_len);
    }
    asg_l((&self->ptr)(none()));
    self->len = 0;
    self->alloc_len = 0;
};

$static fn_((proc__windows_wideAlloc(mem_Alctr gpa, usize len))(O$P$u16) $scope) {
    let byte_len = orelse_((usize_mulChkd(len, sizeOf$(u16)))(return_none()));
    let ptr = proc__windows_alloc(gpa, byte_len);
    if_none(ptr) return_none();
    return_some(ptrAlignCast$((P$u16)(unwrap_(ptr))));
} $unscoped(fn);

$static fn_((proc__windows_wtf8ZAlloc(mem_Alctr gpa, S_const$u8 src))(E$proc__windows_OwnedWideBuf) $scope) {
    let text_len = unicode_wtf8ToWTF16Len(src);
    let alloc_len = orelse_((usize_addChkd(text_len, 1))(return_err(E_cause$proc_SystemResources())));
    let alloc_bytes = orelse_((usize_mulChkd(alloc_len, sizeOf$(u16)))(return_err(E_cause$proc_SystemResources())));
    let buf = orelse_((proc__windows_wideAlloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    let out = P_prefix$((S$u16)(buf)(alloc_len));
    let converted = catch_((unicode_wtf8ToWTF16Within(src, S_prefix((out)(text_len))))(
        $ignore,
        {
            proc__windows_free(gpa, ptrCast$((P$u8)(buf)), alloc_bytes);
            return_err(E_cause$proc_InvalidName());
        }
    ));
    claim_assert(converted.len == text_len);
    *S_at((out)[text_len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = text_len,
        .alloc_len = alloc_bytes,
    });
} $unscoped(fn);

$static fn_((proc__windows_wtf8OptZAlloc(mem_Alctr gpa, O$S_const$u8 src))(E$proc__windows_OwnedWideBuf) $scope) {
    if_none(src) {
        return_ok({
            .gpa = gpa,
            .ptr = none(),
            .len = 0,
            .alloc_len = 0,
        });
    }
    return_(proc__windows_wtf8ZAlloc(gpa, unwrap_(src)));
} $unscoped(fn);

$static fn_((proc__windows_dupSliceZ(mem_Alctr gpa, S_const$u8 src))(E$proc__windows_OwnedBuf) $scope) {
    let alloc_len = src.len + 1;
    let buf = orelse_((proc__windows_alloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    let out = P_prefix$((S$u8)(buf)(alloc_len));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = src.len,
        .alloc_len = alloc_len,
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

$static fn_((proc__windows_dirPathAlloc(mem_Alctr gpa, fs_Dir dir))(E$proc__windows_OwnedBuf) $guard) {
    let need = GetFinalPathNameByHandleA(dir.handle, null, 0, 0);
    if (need == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    let alloc_len = as$(usize)(need) + 1;
    let buf = orelse_((proc__windows_alloc(gpa, alloc_len))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, proc__windows_free(gpa, buf, alloc_len));
    let wrote = GetFinalPathNameByHandleA(dir.handle, as$(LPSTR)(buf), need + 1, 0);
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    var_(owned, proc__windows_OwnedBuf) = {
        .gpa = gpa,
        .ptr = some(buf),
        .len = as$(usize)(wrote),
        .alloc_len = alloc_len,
    };
    proc__windows_trimNtPrefix(&owned);
    return_ok(owned);
} $unguarded(fn);

$static fn_((proc__windows_resolvePathAlloc(mem_Alctr gpa, S_const$u8 base, S_const$u8 sub_path))(E$proc__windows_OwnedBuf) $guard) {
    if (fs_path_isAbs(sub_path)) return_(proc__windows_dupSliceZ(gpa, sub_path));
    let base_sub_len = orelse_((usize_addChkd(base.len, sub_path.len))(return_err(E_cause$proc_SystemResources())));
    let cap = orelse_((usize_addChkd(base_sub_len, usize_(2)))(return_err(E_cause$proc_SystemResources())));
    let buf = orelse_((proc__windows_alloc(gpa, cap))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, proc__windows_free(gpa, buf, cap));
    let out = P_prefix$((S$u8)(buf)(cap));
    let joined = catch_((fs_path_join2(base, sub_path, S_prefix((out)(cap - 1))))($ignore, {
        return_err(E_cause$proc_SystemResources());
    }));
    *S_at((out)[joined.len]) = 0;
    return_ok({
        .gpa = gpa,
        .ptr = some(buf),
        .len = joined.len,
        .alloc_len = cap,
    });
} $unguarded(fn);

$static fn_((proc__windows_resolveStdIO(proc_Spawn_StdIO spec, O$fs_File inherited, DWORD std_id))(E$proc__windows_ResolvedStdIO) $scope) {
    let for_read = std_id == STD_INPUT_HANDLE;
    if (matches(spec, proc_Spawn_StdIO_inherit)) {
        if_none(inherited) {
            return_ok({
                .child = none(),
                .parent_pipe = none(),
                .needs_close_child = false,
            });
        }
        let child = try_(proc__windows_dupInheritable(fs_File_handle(unwrap_(inherited))));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (matches(spec, proc_Spawn_StdIO_file)) {
        let child = try_(proc__windows_dupInheritable(fs_File_handle(union_to((spec)(proc_Spawn_StdIO_file)))));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (matches(spec, proc_Spawn_StdIO_close)) {
        return_ok({
            .child = none(),
            .parent_pipe = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_Spawn_StdIO_ignore)) {
        let child = try_(proc__windows_stdioNull(for_read));
        return_ok({
            .child = some(child),
            .parent_pipe = none(),
            .needs_close_child = true,
        });
    }
    if (!matches(spec, proc_Spawn_StdIO_pipe)) return_err(E_cause$proc_OperationUnsupported());

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
} $unscoped(fn);

$static fn_((proc__windows_stdFile(DWORD std_id))(O$fs_File)) {
    let handle = GetStdHandle(std_id);
    if (handle == null || handle == INVALID_HANDLE_VALUE) {
        return none$((O$fs_File));
    }
    return some$((O$fs_File)(fs_File_Handle_promote(
        handle,
        fs_File_Flags_default
    )));
};

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
            for_(($r(0, backslashes * 2 + 1))(i)) {
                let_ignore = i;
                *S_at((out)[used++]) = u8_c('\\');
            } $end(for);
            *S_at((out)[used++]) = u8_c('"');
            backslashes = 0;
            continue;
        }
        if (used + backslashes + 1 > out.len) return_none();
        for_(($r(0, backslashes))(i)) {
            let_ignore = i;
            *S_at((out)[used++]) = u8_c('\\');
        } $end(for);
        backslashes = 0;
        *S_at((out)[used++]) = ch;
    } $end(for);
    if (used + backslashes * 2 + 1 > out.len) return_none();
    for_(($r(0, backslashes * 2))(i)) {
        let_ignore = i;
        *S_at((out)[used++]) = u8_c('\\');
    } $end(for);
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

$static fn_((proc__windows_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 application_name,
    O$S_const$u8 current_dir
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var_(cmdline_buf, proc__windows_OwnedBuf) = {
        .gpa = gpa,
        .ptr = proc__windows_alloc(gpa, proc__windows_path_max),
        .len = proc__windows_path_max,
        .alloc_len = proc__windows_path_max,
    };
    if_none(cmdline_buf.ptr) return_err(E_cause$OutOfMemory());
    errdefer_($ignore, proc__windows_OwnedBuf_fini(&cmdline_buf));
    let _cmdline = try_(proc__windows_commandLine(cmd.argv, P_prefix$((S$u8)(unwrap_(cmdline_buf.ptr))(cmdline_buf.len))));
    var_(cmdline_wide, proc__windows_OwnedWideBuf) = try_(
        proc__windows_wtf8ZAlloc(gpa, _cmdline.as_const)
    );
    errdefer_($ignore, proc__windows_OwnedWideBuf_fini(&cmdline_wide));
    var env_block = try_(proc__envWTF16(
        gpa,
        env,
        cmd.env
    ));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(env_block)));
    var_(application_name_wide, proc__windows_OwnedWideBuf) = try_(
        proc__windows_wtf8OptZAlloc(gpa, application_name)
    );
    errdefer_($ignore, proc__windows_OwnedWideBuf_fini(&application_name_wide));
    var_(current_dir_wide, proc__windows_OwnedWideBuf) = try_(
        proc__windows_wtf8OptZAlloc(gpa, current_dir)
    );
    errdefer_($ignore, proc__windows_OwnedWideBuf_fini(&current_dir_wide));

    var_(std_in, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_in,
        proc__windows_stdFile(STD_INPUT_HANDLE),
        STD_INPUT_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_in));
    var_(std_out, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_out,
        proc__windows_stdFile(STD_OUTPUT_HANDLE),
        STD_OUTPUT_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_out));
    var_(std_err, proc__windows_ResolvedStdIO) = try_(proc__windows_resolveStdIO(
        cmd.std_err,
        proc__windows_stdFile(STD_ERROR_HANDLE),
        STD_ERROR_HANDLE
    ));
    errdefer_($ignore, proc__windows_ResolvedStdIO_fini(&std_err));

    STARTUPINFOW startup = cleared();
    startup.cb = sizeOf$(STARTUPINFOW);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = as$(HANDLE)(orelse_((std_in.child)(as$(proc_Handle)(null))));
    startup.hStdOutput = as$(HANDLE)(orelse_((std_out.child)(as$(proc_Handle)(null))));
    startup.hStdError = as$(HANDLE)(orelse_((std_err.child)(as$(proc_Handle)(null))));

    PROCESS_INFORMATION proc_info = cleared();
    var_(flags, DWORD) = CREATE_UNICODE_ENVIRONMENT;
    if (cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessW(
            isSome(application_name_wide.ptr) ? as$(LPCWSTR)(unwrap_(application_name_wide.ptr)) : null,
            as$(LPWSTR)(unwrap_(cmdline_wide.ptr)),
            null,
            null,
            TRUE,
            flags,
            env_block.ptr,
            isSome(current_dir_wide.ptr) ? as$(LPCWSTR)(unwrap_(current_dir_wide.ptr)) : null,
            &startup,
            &proc_info
        )) {
        return_err(proc__windows_mapError(GetLastError()));
    }

    claim_assert(CloseHandle(proc_info.hThread));
    var cleanup_cmdline = cmdline_buf;
    proc__windows_OwnedBuf_fini(&cleanup_cmdline);
    var cleanup_cmdline_wide = cmdline_wide;
    proc__windows_OwnedWideBuf_fini(&cleanup_cmdline_wide);
    mem_Alctr_free($trace gpa, u_anyS(env_block));
    var cleanup_application_name = application_name_wide;
    proc__windows_OwnedWideBuf_fini(&cleanup_application_name);
    var cleanup_current_dir = current_dir_wide;
    proc__windows_OwnedWideBuf_fini(&cleanup_current_dir);
    if (std_in.needs_close_child) if_some((std_in.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_out.needs_close_child) if_some((std_out.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));
    if (std_err.needs_close_child) if_some((std_err.child)(child)) claim_assert(CloseHandle(as$(HANDLE)(child)));

    return_ok(proc__child(
        as$(proc_Child_Handle)(proc_info.hProcess),
        as$(proc_Child_Id)(proc_info.dwProcessId), (proc_Child_IO){
                                                                                                              .in = std_in.parent_pipe,
                                                                                                              .out = std_out.parent_pipe,
                                                                                                              .err = std_err.parent_pipe,
                                                                                                          }));
} $unguarded(fn);

$static fn_((proc__windows_exePath(S$u8 out_buf))(proc_ExePath_E$S$u8) $scope) {
    if (out_buf.len == 0) return_err(E_cause$proc_ResourceLimitReached());
    let wrote = GetModuleFileNameA(null, as$(LPSTR)(out_buf.ptr), as$(DWORD)(out_buf.len));
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

$static fn_((proc__windows_currPath(S$u8 out_buf))(proc_CurrPath_E$S$u8) $scope) {
    let wrote = GetCurrentDirectoryA(as$(DWORD)(out_buf.len), as$(LPSTR)(out_buf.ptr));
    if (wrote == 0) return_err(proc__windows_mapPathQueryError(GetLastError()));
    if (wrote >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(wrote))));
} $unscoped(fn);

$static fn_((proc__windows_setCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $guard) {
    claim_assert_nonnull(ctx);
    var_(path_z, proc__windows_OwnedBuf) = try_(proc__windows_dupSliceZ(gpa, path));
    defer_(proc__windows_OwnedBuf_fini(&path_z));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(unwrap_(path_z.ptr)))) {
        return_err(proc__windows_mapSetPathError(GetLastError()));
    }
    return_ok({});
} $unguarded(fn);

$static fn_((proc__windows_setCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $guard) {
    claim_assert_nonnull(ctx);
    var_(path, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(gpa, dir));
    defer_(proc__windows_OwnedBuf_fini(&path));
    if (!SetCurrentDirectoryA(as$(LPCSTR)(unwrap_(path.ptr)))) {
        return_err(proc__windows_mapSetPathError(GetLastError()));
    }
    return_ok({});
} $unguarded(fn);

$static fn_((proc__windows_spawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    if (matches(cmd.cwd, proc_Spawn_CWD_inherit)) {
        return_(proc__windows_spawnImpl(
            ctx,
            gpa,
            env,
            cmd,
            none$((O$S_const$u8)),
            none$((O$S_const$u8))
        ));
    }
    if (matches(cmd.cwd, proc_Spawn_CWD_path)) {
        return_(proc__windows_spawnImpl(
            ctx,
            gpa,
            env,
            cmd,
            none$((O$S_const$u8)),
            some$((O$S_const$u8)(union_to((cmd.cwd)(proc_Spawn_CWD_path))))
        ));
    }
    var_(cwd, proc__windows_OwnedBuf) = try_(
        proc__windows_dirPathAlloc(
            gpa,
            union_to((cmd.cwd)(proc_Spawn_CWD_dir))
        )
    );
    defer_(proc__windows_OwnedBuf_fini(&cwd));
    let child = try_(proc__windows_spawnImpl(
        ctx,
        gpa,
        env,
        cmd,
        none$((O$S_const$u8)),
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(cwd.ptr), .len = cwd.len }))
    ));
    return_ok(child);
} $unguarded(fn);

$static fn_((proc__windows_spawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    var_(base, proc__windows_OwnedBuf) = try_(proc__windows_dirPathAlloc(gpa, dir));
    defer_(proc__windows_OwnedBuf_fini(&base));
    var_(exe_path, proc__windows_OwnedBuf) = try_(proc__windows_resolvePathAlloc(
        gpa,
        P_prefix$((S$u8)(unwrap_(base.ptr))(base.len)).as_const,
        *S_at((cmd.argv)[0])
    ));
    defer_(proc__windows_OwnedBuf_fini(&exe_path));
    let child = try_(proc__windows_spawnImpl(
        ctx,
        gpa,
        env,
        cmd,
        some$((O$S_const$u8)((S_const$u8){ .ptr = unwrap_(exe_path.ptr), .len = exe_path.len })),
        none$((O$S_const$u8))
    ));
    return_ok(child);
} $unguarded(fn);

$static fn_((proc__windows_childWait(
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    claim_assert_nonnull(self);
    let handle = unwrap_(self->handle);
    switch (WaitForSingleObject(as$(HANDLE)(handle), INFINITE)) {
    case WAIT_OBJECT_0: break;
    case WAIT_ABANDONED: $fallthrough;
    case WAIT_TIMEOUT: $fallthrough;
    case WAIT_FAILED: {
        if (GetLastError() == ERROR_ACCESS_DENIED) {
            return_err(E_cause$proc_AccessDenied());
        }
        return_err(E_cause$proc_SystemResources());
    };
    default_() return_err(E_cause$proc_SystemResources()) $end(default);
    }
    var_(exit_code, DWORD) = 0;
    if (!GetExitCodeProcess(as$(HANDLE)(handle), &exit_code)) {
        return_err(E_cause$proc_SystemResources());
    }
    claim_assert(CloseHandle(as$(HANDLE)(handle)));
    proc__childClear(self);
    return_ok(union_of$(
        (proc_Child_Trm)(proc_Child_Trm_exited)(as$(u8)(exit_code))
    ));
} $unscoped(fn);

$static fn_((proc__windows_childKill(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->handle)(handle)) {
        let_ignore = TerminateProcess(as$(HANDLE)(handle), 1);
        let_ignore = catch_((proc__windows_childWait(self))(
            $ignore,
            $do_nothing
        ));
    };
};

$attr($no_return)
$static fn_((proc__windowsAbort(void))(void)) {
    let_ignore = TerminateProcess(GetCurrentProcess(), 3);
    start_exit(134);
};
#endif /* plat_is_windows */

/*--- Linux ---*/

#if plat_is_linux
#define proc__linux_default_path u8_l("/usr/local/bin:/bin:/usr/bin")

$static fn_((proc__linux_mapErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_EPERM: return E_cause$proc_PermissionDenied().any;
    case sys_call_linux_ENOTDIR: return E_cause$proc_NotDir().any;
    case sys_call_linux_EIO: $fallthrough;
    case sys_call_linux_ELOOP: return E_cause$proc_FileSystem().any;
    case sys_call_linux_ENOEXEC: $fallthrough;
    case sys_call_linux_EINVAL: $fallthrough;
    case sys_call_linux_ELIBBAD: return E_cause$proc_InvalidExe().any;
    case sys_call_linux_EISDIR: return E_cause$proc_IsDir().any;
    case sys_call_linux_ETXTBSY: return E_cause$proc_FileBusy().any;
    case sys_call_linux_EMFILE: return E_cause$proc_ProcessFdQuotaExceeded().any;
    case sys_call_linux_ENFILE: return E_cause$proc_SystemFdQuotaExceeded().any;
    case sys_call_linux_ENODEV: return E_cause$proc_NoDevice().any;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_NameTooLong().any;
    case sys_call_linux_E2BIG: $fallthrough;
    case sys_call_linux_ENOMEM: return E_cause$proc_SystemResources().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__linux_mapExePathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: $fallthrough;
    case sys_call_linux_ENOTDIR: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_EIO: return E_cause$proc_FileSystem().any;
    case sys_call_linux_ELOOP: return E_cause$proc_SymLinkLoop().any;
    case sys_call_linux_ERANGE: $fallthrough;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_ResourceLimitReached().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__linux_mapCurrPathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: $fallthrough;
    case sys_call_linux_ENOTDIR: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_ERANGE: $fallthrough;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_ResourceLimitReached().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__linux_mapSetPathErr(sys_call_linux_word err))(EAny)) {
    switch (err) {
    case sys_call_linux_ENOENT: return E_cause$proc_FileNotFound().any;
    case sys_call_linux_EACCES: return E_cause$proc_AccessDenied().any;
    case sys_call_linux_ENOTDIR: return E_cause$proc_NotDir().any;
    case sys_call_linux_ELOOP: return E_cause$proc_SymLinkLoop().any;
    case sys_call_linux_ENAMETOOLONG: return E_cause$proc_NameTooLong().any;
    case sys_call_linux_EIO: return E_cause$proc_FileSystem().any;
    default_() return E_cause$proc_SystemResources().any $end(default);
    }
};

$static fn_((proc__linux_pipeFile(sys_call_linux_fd_t fd))(fs_File)) {
    return fs_File_Handle_promote(as$(fs_File_Handle)(fd), fs_File_Flags_default);
};

typedef struct proc__linux_StdIO {
    var_(child_fd, sys_call_linux_fd_t);
    var_(parent_, O$fs_File);
    var_(needs_close_child, bool);
} proc__linux_StdIO;
T_use_E$(proc__linux_StdIO);


T_use_E$($set(proc_Spawn_E)(O$S$u8));
T_use_E$($set(proc_Spawn_E)(O$S_const$u8));

$static fn_((proc__linux_dupeZ(mem_Alctr gpa, S_const$u8 src))(mem_E$S$u8) $scope) {
    let len = orelse_((usize_addChkd(src.len, 1))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes($trace gpa, len));
    mem_copyBytes(S_prefix((out)(src.len)), src);
    *S_at((out)[src.len]) = 0;
    return_ok(out);
} $unscoped(fn);

$static fn_((proc__linux_envByOwned(
    mem_Alctr gpa,
    proc_Env env,
    S_const$u8 name
))(proc_Spawn_E$O$S$u8) $guard) {
    var scratch = try_(mem_Alctr_allocBytes($trace gpa, usize_(256)));
    defer_(mem_Alctr_freeBytes($trace gpa, scratch));
    while (true) {
        let value = catch_((proc_Env_by(env, name, scratch))(err, {
            if (!E_eql(err.as_any, E_cause$proc_ResourceLimitReached().as_any)) {
                return_err(err);
            }
            try_(proc__growScratch(&scratch, gpa));
            continue;
        }));
        if_none(value) return_ok(none());
        return_ok(some(try_(mem_Alctr_dupeBytes($trace gpa, unwrap_(value)))));
    }
} $unguarded(fn);

$static fn_((proc__linux_closeIf(sys_call_linux_fd_t fd))(void)) {
    if (fd >= 0) {
        let_ignore = sys_call_linux_close(fd);
    }
};

$static fn_((proc__linux_discardStdIO(proc__linux_StdIO std_io))(void)) {
    if (std_io.needs_close_child) proc__linux_closeIf(std_io.child_fd);
    if_some((std_io.parent_)(file)) fs_File_close(file);
};

$static fn_((proc__linux_readExecErr(
    sys_call_linux_fd_t fd,
    sys_call_linux_word* err
))(sys_call_linux_word)) {
    claim_assert_nonnull(err);
    var_(read, sys_call_linux_word) = 0;
    do {
        read = sys_call_linux_read(fd, err, sizeOf$(sys_call_linux_word));
    } while (
        sys_call_linux_syscall_isErr(read)
        && sys_call_linux_syscall_err(read) == sys_call_linux_EINTR);
    return read;
};

$static fn_((proc__linux_resolveStdIO(proc_Spawn_StdIO spec, O$fs_File inherited, sys_call_linux_fd_t std_fd))(E$proc__linux_StdIO) $scope) {
    let for_read = std_fd == 0;
    if (matches(spec, proc_Spawn_StdIO_inherit)) {
        if_some((inherited)(file)) return_ok({
            .child_fd = as$(sys_call_linux_fd_t)(fs_File_handle(file)),
            .parent_ = none(),
            .needs_close_child = false,
        });
        let flags = for_read ? sys_call_linux_O_RDONLY : sys_call_linux_O_WRONLY;
        let fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/null", flags, 0);
        if (sys_call_linux_syscall_isErr(fd)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(fd)));
        return_ok({ .child_fd = fd, .parent_ = none(), .needs_close_child = true });
    }
    if (matches(spec, proc_Spawn_StdIO_file)) {
        return_ok({
            .child_fd = as$(sys_call_linux_fd_t)(fs_File_handle(union_to((spec)(proc_Spawn_StdIO_file)))),
            .parent_ = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_Spawn_StdIO_close)) {
        return_ok({
            .child_fd = -1,
            .parent_ = none(),
            .needs_close_child = false,
        });
    }
    if (matches(spec, proc_Spawn_StdIO_ignore)) {
        let flags = for_read ? sys_call_linux_O_RDONLY : sys_call_linux_O_WRONLY;
        let fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/null", flags, 0);
        if (sys_call_linux_syscall_isErr(fd)) return_err(proc__linux_mapErr(sys_call_linux_syscall_err(fd)));
        return_ok({
            .child_fd = fd,
            .parent_ = none(),
            .needs_close_child = true,
        });
    }
    if (!matches(spec, proc_Spawn_StdIO_pipe)) return_err(E_cause$proc_OperationUnsupported());

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
} $unscoped(fn);

$static fn_((proc__linux_dupTo(
    sys_call_linux_fd_t src,
    sys_call_linux_fd_t dst
))(sys_call_linux_word)) {
    if (src < 0) {
        proc__linux_closeIf(dst);
        return 0;
    }
    if (src == dst) return 0;
    let rc = sys_call_linux_dup3(src, dst, 0);
    if (sys_call_linux_syscall_isErr(rc)) {
        return sys_call_linux_syscall_err(rc);
    }
    proc__linux_closeIf(src);
    return 0;
};

$static fn_((proc__linux_execPath(
    const char* path,
    char* const* argv,
    char* const* envp
))(sys_call_linux_word)) {
    let rc = sys_call_linux_execve(path, argv, envp);
    claim_assert(sys_call_linux_syscall_isErr(rc));
    return sys_call_linux_syscall_err(rc);
};

$static fn_((proc__linux_exec(
    proc_ArgExpsn expand_arg0,
    const char* file,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf
))(sys_call_linux_word)) {
    let file_s = mem_spanZ0Bytes(as$(P_const$u8)(file));
    if (mem_containsUnitBytes(file_s, u8_c('/'))) {
        return proc__linux_execPath(file, argv, envp);
    }

    var paths = mem_tokzUnitBytes(path_env, u8_c(':'));
    var_(saw_access_denied, bool) = false;
    while_some((mem_TokzIter_nextBytes(&paths)), path) {
        let required = path.len + 1 + file_s.len + 1;
        if (required > path_buf.len) return sys_call_linux_ENAMETOOLONG;
        mem_copyBytes(S_prefix((path_buf)(path.len)), path);
        *S_at((path_buf)[path.len]) = u8_c('/');
        mem_copyBytes(
            S_slice((path_buf)$r(path.len + 1, required - 1)),
            file_s
        );
        *S_at((path_buf)[required - 1]) = 0;

        let previous_arg0 = argv[0];
        if (expand_arg0 == proc_ArgExpsn_expand) {
            argv[0] = as$(char*)(path_buf.ptr);
        }
        let err = proc__linux_execPath(as$(const char*)(path_buf.ptr), argv, envp);
        argv[0] = previous_arg0;

        if (err == sys_call_linux_EACCES) {
            saw_access_denied = true;
            continue;
        }
        if (err == sys_call_linux_ENOENT || err == sys_call_linux_ENOTDIR) {
            continue;
        }
        return err;
    }
    return saw_access_denied ? sys_call_linux_EACCES : sys_call_linux_ENOENT;
};

$attr($no_return)
$static fn_((proc__linux_childFail(
    sys_call_linux_fd_t err_fd,
    sys_call_linux_word err
))(void)) {
    var_(wrote, sys_call_linux_word) = 0;
    do {
        wrote = sys_call_linux_write(err_fd, &err, sizeOf$(sys_call_linux_word));
    } while (
        sys_call_linux_syscall_isErr(wrote)
        && sys_call_linux_syscall_err(wrote) == sys_call_linux_EINTR);
    sys_call_linux_exit(127);
};

$attr($no_return)
$static fn_((proc__linux_childExec(
    proc_Spawn_Opts cmd,
    O$S_const$u8 cwd_path,
    const char* exe,
    char** argv,
    char* const* envp,
    S_const$u8 path_env,
    S$u8 path_buf,
    proc__linux_StdIO std_in,
    proc__linux_StdIO std_out,
    proc__linux_StdIO std_err,
    sys_call_linux_fd_t err_fd
))(void)) {
    if (matches(cmd.cwd, proc_Spawn_CWD_dir)) {
        let rc = sys_call_linux_fchdir(
            fs_Dir_handle(union_to((cmd.cwd)(proc_Spawn_CWD_dir)))
        );
        if (sys_call_linux_syscall_isErr(rc)) {
            proc__linux_childFail(err_fd, sys_call_linux_syscall_err(rc));
        }
    } else if (matches(cmd.cwd, proc_Spawn_CWD_path)) {
        let path = unwrap_(cwd_path);
        let rc = sys_call_linux_chdir(as$(const char*)(path.ptr));
        if (sys_call_linux_syscall_isErr(rc)) {
            proc__linux_childFail(err_fd, sys_call_linux_syscall_err(rc));
        }
    }
    let err_in = proc__linux_dupTo(std_in.child_fd, 0);
    if (err_in != 0) proc__linux_childFail(err_fd, err_in);
    let err_out = proc__linux_dupTo(std_out.child_fd, 1);
    if (err_out != 0) proc__linux_childFail(err_fd, err_out);
    let err_err = proc__linux_dupTo(std_err.child_fd, 2);
    if (err_err != 0) proc__linux_childFail(err_fd, err_err);

    proc__linux_childFail(
        err_fd,
        proc__linux_exec(
            cmd.expand_arg0,
            exe,
            argv,
            envp,
            path_env,
            path_buf
        )
    );
};

$static fn_((proc__linux_spawnImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd,
    O$S_const$u8 exe_path
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    if (cmd.start_suspended) return_err(E_cause$proc_OperationUnsupported());
    let_ignore = cmd.create_no_window;

    var argv = try_(proc__NativeStrs_from(gpa, cmd.argv));
    defer_(proc__NativeStrs_fini(&argv, gpa));
    var child_env = try_(proc__NativeStrs_fromEnv(gpa, env, cmd.env));
    defer_(proc__NativeStrs_fini(&child_env, gpa));

    var path_env_mem = try_(proc__linux_envByOwned(
        gpa, env, u8_l("PATH")
    ));
    defer_(if_some((path_env_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    let path_env = isSome(path_env_mem)
                     ? unwrap_(path_env_mem).as_const
                     : proc__linux_default_path;

    let arg0 = proc__NativeStrs_at(argv, usize_(0));
    let path_buf_len = orelse_((usize_addChkd(path_env.len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let path_buf = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(path_buf_len, 2))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, path_buf));

    var exe_mem = none$((O$S$u8));
    defer_(if_some((exe_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    if_some((exe_path)(path)) {
        asg_l((&exe_mem)(some(try_(proc__linux_dupeZ(gpa, path)))));
    }
    let exe = isSome(exe_mem)
                ? as$(const char*)(unwrap_(exe_mem).ptr)
                : as$(const char*)(arg0.ptr);

    var cwd_mem = none$((O$S$u8));
    defer_(if_some((cwd_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    var_(cwd_path, O$S_const$u8) = none();
    if (matches(cmd.cwd, proc_Spawn_CWD_path)) {
        asg_l((&cwd_mem)(some(try_(proc__linux_dupeZ(
            gpa, union_to((cmd.cwd)(proc_Spawn_CWD_path))
        )))));
        asg_l((&cwd_path)(some(unwrap_(cwd_mem).as_const)));
    }

    let_(std_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDIN_FILENO,
        fs_File_Flags_default
    ));
    let_(std_out_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDOUT_FILENO,
        fs_File_Flags_default
    ));
    let_(std_err_inherited, O$fs_File) = some(fs_File_Handle_promote(
        sys_posix_STDERR_FILENO,
        fs_File_Flags_default
    ));
    var std_in = try_(proc__linux_resolveStdIO(
        cmd.std_in, std_inherited, sys_posix_STDIN_FILENO
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_in));
    var std_out = try_(proc__linux_resolveStdIO(
        cmd.std_out, std_out_inherited, sys_posix_STDOUT_FILENO
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_out));
    var std_err = try_(proc__linux_resolveStdIO(
        cmd.std_err, std_err_inherited, sys_posix_STDERR_FILENO
    ));
    errdefer_($ignore, proc__linux_discardStdIO(std_err));

    int err_fds[2] = { -1, -1 };
    let err_pipe_rc = sys_call_linux_pipe2(err_fds, sys_call_linux_O_CLOEXEC);
    if (sys_call_linux_syscall_isErr(err_pipe_rc)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(err_pipe_rc)));
    }
    defer_(proc__linux_closeIf(err_fds[0]));
    defer_(proc__linux_closeIf(err_fds[1]));

    let pid = sys_call_linux_fork();
    if (sys_call_linux_syscall_isErr(pid)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(pid)));
    }
    if (pid == 0) {
        proc__linux_closeIf(err_fds[0]);
        proc__linux_childExec(
            cmd,
            cwd_path,
            exe,
            proc__NativeStrs_raw(argv),
            proc__NativeStrs_raw(child_env),
            path_env,
            path_buf,
            std_in,
            std_out,
            std_err,
            err_fds[1]
        );
    }
    errdefer_($ignore, {
        let_ignore = sys_call_linux_kill(as$(sys_call_linux_pid_t)(pid), 9);
        var_(status, i32) = 0;
        let_ignore = sys_call_linux_wait4(
            as$(sys_call_linux_pid_t)(pid), &status, 0, null);
    });

    proc__linux_closeIf(err_fds[1]);
    err_fds[1] = -1;
    if (std_in.needs_close_child) {
        proc__linux_closeIf(std_in.child_fd);
        std_in.needs_close_child = false;
    }
    if (std_out.needs_close_child) {
        proc__linux_closeIf(std_out.child_fd);
        std_out.needs_close_child = false;
    }
    if (std_err.needs_close_child) {
        proc__linux_closeIf(std_err.child_fd);
        std_err.needs_close_child = false;
    }

    var_(exec_err, sys_call_linux_word) = 0;
    let exec_err_read = proc__linux_readExecErr(err_fds[0], &exec_err);
    proc__linux_closeIf(err_fds[0]);
    err_fds[0] = -1;
    if (sys_call_linux_syscall_isErr(exec_err_read)) {
        return_err(E_cause$proc_SystemResources());
    }
    if (exec_err_read != 0) {
        claim_assert(as$(usize)(exec_err_read) == sizeOf$(sys_call_linux_word));
        return_err(proc__linux_mapErr(exec_err));
    }

    return_ok(proc__child(
        as$(proc_Child_Handle)(pid),
        as$(proc_Child_Id)(pid), (proc_Child_IO){
                                                                             .in = std_in.parent_,
                                                                             .out = std_out.parent_,
                                                                             .err = std_err.parent_,
                                                                         }));
} $unguarded(fn);

$static fn_((proc__linux_exePath(S$u8 out_buf))(proc_ExePath_E$S$u8) $scope) {
    let read = sys_call_linux_readlinkat(sys_call_linux_AT_FDCWD, "/proc/self/exe", as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(read)) {
        return_err(proc__linux_mapExePathErr(sys_call_linux_syscall_err(read)));
    }
    if (as$(usize)(read) >= out_buf.len) return_err(E_cause$proc_ResourceLimitReached());
    return_ok(S_prefix((out_buf)(as$(usize)(read))));
} $unscoped(fn);

$static fn_((proc__linux_currPath(S$u8 out_buf))(proc_CurrPath_E$S$u8) $scope) {
    let rc = sys_call_linux_getcwd(as$(char*)(out_buf.ptr), out_buf.len);
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapCurrPathErr(sys_call_linux_syscall_err(rc)));
    }
    let len = mem_lenZ0Bytes(out_buf.ptr);
    return_ok(S_prefix((out_buf)(len)));
} $unscoped(fn);

$static fn_((proc__linux_setCurrPath(
    P$raw ctx,
    mem_Alctr gpa,
    S_const$u8 path
))(proc_SetCurrPath_E$void) $guard) {
    claim_assert_nonnull(ctx);
    let path_z = try_(proc__linux_dupeZ(gpa, path));
    defer_(mem_Alctr_freeBytes($trace gpa, path_z));
    let rc = sys_call_linux_chdir(as$(const char*)(path_z.ptr));
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapSetPathErr(sys_call_linux_syscall_err(rc)));
    }
    return_ok({});
} $unguarded(fn);

$static fn_((proc__linux_setCurrDir(
    P$raw ctx,
    mem_Alctr gpa,
    fs_Dir dir
))(proc_SetCurrDir_E$void) $scope) {
    claim_assert_nonnull(ctx);
    let_ignore = gpa;
    let rc = sys_call_linux_fchdir(fs_Dir_handle(dir));
    if (sys_call_linux_syscall_isErr(rc)) {
        return_err(proc__linux_mapErr(sys_call_linux_syscall_err(rc)));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((proc__linux_spawn(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child)) {
    return proc__linux_spawnImpl(ctx, gpa, env, cmd, none$((O$S_const$u8)));
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

$static fn_((proc__linux_dirExePath(
    fs_Dir dir,
    S_const$u8 arg0,
    S$u8 out
))(E$S_const$u8) $scope) {
    let prefix = u8_l("/proc/self/fd/");
    var_(pos, usize) = 0;
    if (prefix.len > out.len) return_err(E_cause$proc_ResourceLimitReached());
    mem_copyBytes(S_prefix((out)(prefix.len)), prefix);
    pos += prefix.len;
    if (!proc__linux_appendU64(out, &pos, as$(u64)(fs_Dir_handle(dir)))) {
        return_err(E_cause$proc_ResourceLimitReached());
    }
    if (pos + 1 + arg0.len + 1 > out.len) {
        return_err(E_cause$proc_ResourceLimitReached());
    }
    *S_at((out)[pos++]) = u8_c('/');
    mem_copyBytes(S_slice((out)$r(pos, pos + arg0.len)), arg0);
    pos += arg0.len;
    *S_at((out)[pos]) = 0;
    return_ok(S_prefix((out.as_const)(pos)));
} $unscoped(fn);

$static fn_((proc__linux_spawnPath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Spawn_Opts cmd
))(proc_Spawn_E$proc_Child) $guard) {
    claim_assert_nonnull(ctx);
    if (cmd.argv.len == 0) return_err(E_cause$proc_InvalidName());
    let arg0 = *S_at((cmd.argv)[0]);
    let base_len = orelse_((usize_addChkd(u8_l("/proc/self/fd/").len, 21))(return_err(E_cause$OutOfMemory())));
    let out_len = orelse_((usize_addChkd(base_len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(out_len, 1))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, out));
    let exe_path = try_(proc__linux_dirExePath(dir, arg0, out));
    return proc__linux_spawnImpl(ctx, gpa, env, cmd, some$((O$S_const$u8)(exe_path)));
} $unguarded(fn);

$static fn_((proc__linux_replaceImpl(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env inherited_env,
    proc_Replace_Opts opts,
    O$S_const$u8 exe_path
))(proc_Replace_E$void) $guard) {
    claim_assert_nonnull(ctx);
    if (opts.argv.len == 0) return_err(E_cause$proc_InvalidName());

    var argv = try_(proc__NativeStrs_from(gpa, opts.argv));
    defer_(proc__NativeStrs_fini(&argv, gpa));
    var env = try_(proc__NativeStrs_fromEnv(gpa, inherited_env, opts.env));
    defer_(proc__NativeStrs_fini(&env, gpa));

    var path_env_mem = try_(proc__linux_envByOwned(
        gpa, inherited_env, u8_l("PATH")
    ));
    defer_(if_some((path_env_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    let path_env = isSome(path_env_mem)
                     ? unwrap_(path_env_mem).as_const
                     : proc__linux_default_path;

    let arg0 = proc__NativeStrs_at(argv, usize_(0));
    let path_buf_len = orelse_((usize_addChkd(path_env.len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let path_buf = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(path_buf_len, 2))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, path_buf));

    var exe_mem = none$((O$S$u8));
    defer_(if_some((exe_mem)(mem)) mem_Alctr_freeBytes($trace gpa, mem));
    if_some((exe_path)(path)) {
        asg_l((&exe_mem)(some(try_(proc__linux_dupeZ(gpa, path)))));
    }
    let exe = isSome(exe_mem)
                ? as$(const char*)(unwrap_(exe_mem).ptr)
                : as$(const char*)(arg0.ptr);

    return_err(proc__linux_mapErr(proc__linux_exec(
        opts.expand_arg0,
        exe,
        proc__NativeStrs_raw(argv),
        proc__NativeStrs_raw(env),
        path_env,
        path_buf
    )));
} $unguarded(fn);

$static fn_((proc__linux_replace(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    proc_Replace_Opts opts
))(proc_Replace_E$void)) {
    return proc__linux_replaceImpl(ctx, gpa, env, opts, none$((O$S_const$u8)));
};

$static fn_((proc__linux_replacePath(
    P$raw ctx,
    mem_Alctr gpa,
    proc_Env env,
    fs_Dir dir,
    proc_Replace_Opts opts
))(proc_Replace_E$void) $guard) {
    claim_assert_nonnull(ctx);
    if (opts.argv.len == 0) return_err(E_cause$proc_InvalidName());
    let arg0 = *S_at((opts.argv)[0]);
    let base_len = orelse_((usize_addChkd(u8_l("/proc/self/fd/").len, 21))(return_err(E_cause$OutOfMemory())));
    let out_len = orelse_((usize_addChkd(base_len, arg0.len))(return_err(E_cause$OutOfMemory())));
    let out = try_(mem_Alctr_allocBytes(
        $trace gpa,
        orelse_((usize_addChkd(out_len, 1))(return_err(E_cause$OutOfMemory())))
    ));
    defer_(mem_Alctr_freeBytes($trace gpa, out));
    let exe_path = try_(proc__linux_dirExePath(dir, arg0, out));
    return proc__linux_replaceImpl(
        ctx,
        gpa,
        env,
        opts,
        some$((O$S_const$u8)(exe_path))
    );
} $unguarded(fn);

$static fn_((proc__linux_childWait(
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    claim_assert_nonnull(self);
    claim_assert(self->id != 0);
    var_(status, int) = 0;
    var_(waited, sys_call_linux_word) = 0;
    do {
        waited = sys_call_linux_wait4(
            as$(sys_call_linux_pid_t)(self->id), &status, 0, null);
    } while (
        sys_call_linux_syscall_isErr(waited)
        && sys_call_linux_syscall_err(waited) == sys_call_linux_EINTR);
    if (sys_call_linux_syscall_isErr(waited)) {
        if (sys_call_linux_syscall_err(waited) == sys_call_linux_ECHILD) {
            proc__childClear(self);
            return_err(E_cause$proc_SystemResources());
        }
        return_err(E_cause$proc_SystemResources());
    }
    proc__childClear(self);
    if ((status & 0x7f) == 0) {
        return_ok(union_of$(
            (proc_Child_Trm)(proc_Child_Trm_exited)(as$(u8)((status >> 8) & 0xff))
        ));
    }
    if ((status & 0x7f) != 0x7f) {
        return_ok(union_of$(
            (proc_Child_Trm)(proc_Child_Trm_signal)(as$(proc_Child_Sig)(status & 0x7f))
        ));
    }
    return_ok(union_of$(
        (proc_Child_Trm)(proc_Child_Trm_stopped)(as$(proc_Child_Sig)((status >> 8) & 0xff))
    ));
} $unscoped(fn);

$static fn_((proc__linux_childKill(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
    if (self->id != 0) {
        let_ignore = sys_call_linux_kill(
            as$(sys_call_linux_pid_t)(self->id), 9);
        let_ignore = catch_((proc__linux_childWait(self))(
            $ignore,
            $do_nothing
        ));
    }
};

$attr($no_return $maybe_unused)
$static fn_((proc__linuxAbort(void))(void)) {
    let_ignore = sys_posix_raise(sys_posix_SIGABRT);
    start_exit(134);
};
#endif /* plat_is_linux */
