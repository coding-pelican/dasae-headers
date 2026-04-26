#include "IOCP.h"

#if plat_is_windows
#include "dh/os/windows/handle.h"
#include "dh/os/windows/io.h"
#include "dh/os/windows/sync.h"
#endif /* plat_is_windows */

/*========== Internal Definitions ===========================================*/

$static fn_((exec_Evented_IOCP__fini(P$raw ctx))(void)) {
    let self = ptrCast$((exec_Evented_IOCP*)(ensureNonnull(ctx)));
    exec_Evented_IOCP_fini(self);
}

$static fn_((exec_Evented_IOCP__timeoutMillis(time_Dur timeout))(DWORD)) {
#if plat_is_windows
    if (timeout.secs > as$(u64)(((DWORD)-2) / 1000)) return INFINITE;
    let millis = timeout.secs * 1000 + time_Dur_subsecMillis(timeout);
    return as$(DWORD)(millis);
#else
    let_ignore = timeout;
    return 0;
#endif
}

$static fn_((exec_Evented_IOCP__associate(P$raw ctx, exec_Evented_Handle handle, usize key))(E$void) $scope) {
    let self = ptrCast$((exec_Evented_IOCP*)(ensureNonnull(ctx)));
#if plat_is_windows
    if (CreateIoCompletionPort(handle, self->port, as$(ULONG_PTR)(key), 0) == null) {
        return_err(exec_Evented_E_AssociateFailed());
    }
    return_ok({});
#else
    let_ignore = self;
    let_ignore = handle;
    let_ignore = key;
    return_err(exec_Evented_E_Unsupported());
#endif
} $unscoped(fn);

$static fn_((exec_Evented_IOCP__poll(P$raw ctx, time_Dur timeout))(E$O$exec_Evented_Completion) $scope) {
    let self = ptrCast$((exec_Evented_IOCP*)(ensureNonnull(ctx)));
#if plat_is_windows
    var_(bytes, DWORD) = 0;
    var_(key, ULONG_PTR) = 0;
    var_(ov, LPOVERLAPPED) = null;
    let millis = exec_Evented_IOCP__timeoutMillis(timeout);
    let ok = GetQueuedCompletionStatus(self->port, &bytes, &key, &ov, as$(DWORD)(millis));
    if (!ok && ov == null) {
        let err = GetLastError();
        if (err == ERROR_TIMEOUT) return_ok(none());
        return_err(exec_Evented_E_PollFailed());
    }
    return_ok(some((exec_Evented_Completion){
        .key = as$(usize)(key),
        .op = ov,
        .bytes = as$(usize)(bytes),
        .err = ok ? Err_None() : exec_Evented_E_PollFailed(),
    }));
#else
    let_ignore = self;
    let_ignore = timeout;
    return_ok(none());
#endif
} $unscoped(fn);

$static fn_((exec_Evented_IOCP__post(P$raw ctx, exec_Evented_Completion completion))(E$void) $scope) {
    let self = ptrCast$((exec_Evented_IOCP*)(ensureNonnull(ctx)));
#if plat_is_windows
    if (!PostQueuedCompletionStatus(self->port, as$(DWORD)(completion.bytes), as$(ULONG_PTR)(completion.key), as$(LPOVERLAPPED)(completion.op))) {
        return_err(exec_Evented_E_PostFailed());
    }
    return_ok({});
#else
    let_ignore = self;
    let_ignore = completion;
    return_err(exec_Evented_E_Unsupported());
#endif
} $unscoped(fn);

$static fn_((exec_Evented_IOCP__cancel(P$raw ctx, exec_Evented_Handle handle, P$raw op))(E$void) $scope) {
    let_ignore = ctx;
#if plat_is_windows
    if (!CancelIoEx(handle, as$(LPOVERLAPPED)(op))) return_err(exec_Evented_E_Canceled());
    return_ok({});
#else
    let_ignore = handle;
    let_ignore = op;
    return_err(exec_Evented_E_Unsupported());
#endif
} $unscoped(fn);

$static let_(exec_Evented_IOCP__vtbl, exec_Evented_VTbl) = {
    .finiFn = exec_Evented_IOCP__fini,
    .associateFn = exec_Evented_IOCP__associate,
    .pollFn = exec_Evented_IOCP__poll,
    .postFn = exec_Evented_IOCP__post,
    .cancelFn = exec_Evented_IOCP__cancel,
};

/*========== External Definitions ===========================================*/

fn_((exec_Evented_IOCP_init(void))(exec_Evented_E$exec_Evented_IOCP) $scope) {
#if plat_is_windows
    let port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, null, 0, 0);
    if (port == null) return_err(exec_Evented_E_OpenFailed());
    return_ok((exec_Evented_IOCP){ .port = port });
#else
    return_err(exec_Evented_E_Unsupported());
#endif
} $unscoped(fn);

fn_((exec_Evented_IOCP_fini(exec_Evented_IOCP* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (isNonnull(self->port)) CloseHandle(self->port);
#endif
    asg_l((self)(cleared()));
}

fn_((exec_Evented_IOCP_evented(exec_Evented_IOCP* self))(exec_Evented)) {
    claim_assert_nonnull(self);
    return (exec_Evented){
        .ctx = self,
        .vtbl = &exec_Evented_IOCP__vtbl,
    };
}
