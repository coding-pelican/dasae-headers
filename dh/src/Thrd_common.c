#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/Err.h"

fn_(Thrd_sleep(time_Duration duration), void) {
    time_sleep(duration);
}

fn_(Thrd_yield(void), Err$void, $scope) {
    if_(i32 res = sched_yield(), res != 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
    return_ok({});
} $unscoped;

fn_(Thrd_getCurrentId(void), Thrd_Id) {
    return pthread_self();
}

fn_(Thrd_getCpuCount(void), Err$usize, $scope) {
    SYSTEM_INFO sysInfo = {};
    GetSystemInfo(&sysInfo);
    return_ok(sysInfo.dwNumberOfProcessors);
} $unscoped;

fn_(Thrd_getHandle(Thrd self), Thrd_Handle) {
    return self.handle;
}

fn_(Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr), Err$Opt$Sli_const$u8, $scope) {
    debug_assert(self.handle != 0);
    debug_assert_nonnull(buf_ptr);

    let len = pthread_getname_np(
        self.handle,
        as$(char*, buf_ptr->buf),
        Arr_len(*buf_ptr)
    );
    if (len == 0) {
        return_ok(none());
    }

    return_ok(some(Sli_from(buf_ptr->buf, len)));
} $unscoped;

fn_(Thrd_setName(Thrd self, Sli_const$u8 name), Err$void, $scope) {
    debug_assert(self.handle != 0);
    debug_assert_nonnull(name.ptr);

    var name_mem = (Thrd_NameBuf){};
    let name_len = name.len;
    if (Thrd_max_name_len < name_len) {
        return_err(Err_Unspecified()); // TODO: Change to `NameTooLong`
    }
    bti_memcpy(name_mem.buf, name.ptr, name_len);
    name_mem.buf[name_len] = '\0';
#ifdef __APPLE__
    // On macOS, thread names are limited to 64 characters
    if (pthread_setname_np(as$(const char*, name_mem.buf)) != 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
#else
    // On Linux/Unix, pthread_setname_np only works for the current thread
    if (pthread_equal(pthread_self(), self.handle)) {
        if (pthread_setname_np(pthread_self(), as$(const char*, name_mem.buf)) != 0) {
            return_err(Err_Unspecified()); // TODO: Change to specified err
        }
    }
#endif
    return_ok({});
} $unscoped;

fn_(Thrd_spawn(Thrd_SpawnConfig config, Thrd_FnCtx* fn_ctx), Err$Thrd, $scope) {
    $ignore = config;
    debug_assert_nonnull(fn_ctx);
    debug_assert_nonnull(fn_ctx->fn);

    switch_(
        Thrd_Handle handle = {},
        pthread_create(&handle, null, as$(fn_((*)(void* thrd_ctx), void*), fn_ctx->fn), fn_ctx)
    ) {
    case /* SUCCESS */ 0:
        return_ok({ .handle = handle });
    case /* AGAIN */ EAGAIN:
        return_err(Err_Unspecified()); // TODO: Change to specified err
    case /* PERM */ EPERM:
        $fallthrough;
    case /* INVAL */ EINVAL:
        claim_unreachable;
    default:
        return_err(Err_Unexpected());
    }
    claim_unreachable;
} $unscoped;

fn_(Thrd_detach(Thrd self), void) {
    debug_assert(self.handle != 0);
    $ignore = pthread_detach(self.handle); // TODO: handle err
}

fn_(Thrd_join(Thrd self), Thrd_FnCtx*) {
    debug_assert(self.handle != 0);
    void* ctx_ptr = null;
    $ignore       = pthread_join(self.handle, &ctx_ptr); // TODO: handle err
    return as$(Thrd_FnCtx*, ctx_ptr);
}
