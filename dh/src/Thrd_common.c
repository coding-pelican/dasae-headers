#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/Err.h"

// Include pthread and platform-specific headers
#include <errno.h>
#include <unistd.h>

#if bti_plat_windows
#include "dh/os/windows.h"
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

fn_(Thrd_sleep(time_Duration duration), void) {
    time_sleep(duration);
}

fn_(Thrd_yield(void), Err$void $scope) {
    if_(i32 res = sched_yield(), res != 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
    return_ok({});
} $unscoped;

fn_(Thrd_getCurrentId(void), Thrd_Id) {
    return as$(Thrd_Id, pthread_self());
}

fn_(Thrd_getCpuCount(void), Err$usize $scope) {
#if bti_plat_windows
// On Windows, fall back to GetSystemInfo if sysconf is not available
#ifdef _SC_NPROCESSORS_ONLN
    let cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (0 < cpu_count) {
        return_ok(as$(usize, cpu_count));
    }
#endif
    // Fallback to Windows API
    SYSTEM_INFO sysInfo = {};
    GetSystemInfo(&sysInfo);
    return_ok(as$(usize, sysInfo.dwNumberOfProcessors));
#elif defined(__APPLE__) && defined(__MACH__)
    i32   cpu_count = 0;
    usize size      = sizeOf(cpu_count);
    if (sysctlbyname("hw.ncpu", &cpu_count, &size, null, 0) != 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
    return_ok(as$(usize, cpu_count));
#else /* Linux and other Unix platforms */
    let cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_count <= 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
    return_ok(as$(usize, cpu_count));
#endif
} $unscoped;

fn_(Thrd_getHandle(Thrd self), Thrd_Handle) {
    return self.handle;
}

fn_(Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr), Err$Opt$Sli_const$u8 $scope) {
    debug_assert(self.handle != 0);
    debug_assert_nonnull(buf_ptr);

// Use pthread_getname_np on all platforms where available
#ifdef pthread_getname_np
    let result = pthread_getname_np(
        self.handle,
        as$(char*, buf_ptr->buf),
        Arr_len(*buf_ptr)
    );
    if (result != 0) { return_ok(none()); }

    let name_len = Str_viewZ(buf_ptr->buf);
    if (name_len == 0) { return_ok(none()); }

    return_ok(some(Sli_from(buf_ptr->buf, name_len)));
#else
    $ignore = self;
    $ignore = buf_ptr;
    // pthread_getname_np not available (some Windows pthread implementations)
    return_ok(none());
#endif
} $unscoped;

fn_(Thrd_setName(Thrd self, Sli_const$u8 name), Err$void $scope) {
    debug_assert(self.handle != 0);
    debug_assert_nonnull(name.ptr);

    let name_len = name.len;
    if (Thrd_max_name_len < name_len) {
        return_err(Err_Unspecified()); // TODO: Change to `NameTooLong`
    }

// Use pthread_setname_np on all platforms where available
#ifdef pthread_setname_np
    var name_mem = (Thrd_NameBuf){};
    bti_memcpy(name_mem.buf, name.ptr, name_len);
    name_mem.buf[name_len] = '\0';

#if defined(__APPLE__) && defined(__MACH__)
    // On macOS, pthread_setname_np only works for the current thread
    if (pthread_equal(pthread_self(), self.handle)) {
        if (pthread_setname_np(as$(const char*, name_mem.buf)) != 0) {
            return_err(Err_Unspecified()); // TODO: Change to specified err
        }
    } else {
        // Cannot set name for other threads on macOS
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
#else
    // On Linux/Unix and Windows with pthread support
    if (pthread_setname_np(self.handle, as$(const char*, name_mem.buf)) != 0) {
        return_err(Err_Unspecified()); // TODO: Change to specified err
    }
#endif
    return_ok({});
#else
    $ignore = self;
    // pthread_setname_np not available
    return_ok({}); // No-op
#endif
} $unscoped;

fn_(Thrd_spawn(Thrd_SpawnConfig config, Thrd_FnCtx* fn_ctx), Err$Thrd $scope) {
    $ignore = config;
    debug_assert_nonnull(fn_ctx);
    debug_assert_nonnull(fn_ctx->fn);

    switch_(
        Thrd_Handle handle = {},
        pthread_create(&handle, null, as$(fn_((*)(void* thrd_ctx), void*), fn_ctx->fn), fn_ctx), {
            case_(/* SUCCESS */ 0, return_ok({ .handle = handle }));
            case_(/* AGAIN */ EAGAIN, return_err(Err_Unspecified())); // TODO: Change to specified err
            case_(/* PERM */ EPERM, $fallthrough);
            case_(/* INVAL */ EINVAL, claim_unreachable);
            fallback_(return_err(Err_Unexpected()));
        }
    );
} $unscoped;

fn_(Thrd_detach(Thrd self), void) {
    debug_assert(self.handle != 0);
    $ignore = pthread_detach(self.handle); // TODO: handle err
}

fn_(Thrd_join(Thrd self), Thrd_FnCtx*) {
    debug_assert(self.handle != 0);
    anyptr ctx_ptr = null;
    $ignore        = pthread_join(self.handle, &ctx_ptr); // TODO: handle err
    return as$(Thrd_FnCtx*, ctx_ptr);
}
