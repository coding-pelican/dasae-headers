/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Cross-platform thread abstraction
 * @details Provides platform-independent thread management, synchronization primitives,
 *          and thread-local storage facilities.
 */

#ifndef THRD_COMMON_INCLUDED
#define THRD_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Thread Types ===================================================*/

typedef struct Thrd            Thrd;
typedef ThrdHandleImpl         ThrdHandle;
typedef ThrdIdImpl             ThrdId;
typedef struct ThrdSpawnConfig ThrdSpawnConfig;

struct Thrd {
    ThrdHandle handle_;
};
use_Err$(Thrd);

/*========== Thread Operations ==============================================*/

/**
 * Spurious wakeups are possible and no precision of timing is guaranteed.
 */
extern void Thrd_sleep(u64 nanos);

use_Err(
    Thrd_SetNameErr,
    NameTooLong,
    Unsupported,
    Unexpected
    // posix.PrctlError
    // || posix.WriteError
    // || std.fs.File.OpenError
    // || std.fmt.BufPrintError
);
extern Err$void Thrd_setName(Thrd self, SliConst$u8 name);

use_Err(
    Thrd_GetNameErr,
    Unsupported,
    Unexpected
    // posix.PrctlError
    // || posix.ReadError
    // || std.fs.File.OpenError
    // || std.fmt.BufPrintError
);
decl_Opt$(SliConst$u8);
decl_Err$(Opt$SliConst$u8);
typedef Arr$(Thrd_max_name_len, u8) Arr$Thrd_max_name_len$u8;
/**
 * On Windows, the result is encoded as [WTF-8](https://simonsapin.github.io/wtf-8/).
 * On other platforms, the result is an opaque sequence of bytes with no particular encoding.
 */
extern Err$Opt$SliConst$u8 Thrd_getName(Thrd self, Arr$Thrd_max_name_len$u8* buffer_ptr);

/**
 * Returns the platform ID of the callers thread.
 * Attempts to use thread locals and avoid syscalls when possible.
 */
extern ThrdId Thrd_getCurrentId(void);

use_Err(
    Thrd_CpuCountErr,
    PermissionDenied,
    SystemResources,
    Unsupported,
    Unexpected
);
/*
 * Returns the platforms view on the number of logical CPU cores available.
 **/
extern Err$usize Thrd_getCpuCount(void);

struct ThrdSpawnConfig {
    // TODO: compile-time call graph analysis to determine stack upper bound
    /**
     * Size in bytes of the Thread's stack
     */
    usize             stack_size;
    /**
     * The allocator to be used to allocate memory for the to-be-spawned thread
     */
    Opt$mem_Allocator allocator;
};
#define ThrdThrdSpawnConfig_default_stack_size (16ull * 1024 * 1024)
static const ThrdSpawnConfig ThrdSpawnConfig_default = {
    .stack_size = ThrdThrdSpawnConfig_default_stack_size,
    .allocator  = none()
};
use_Err(
    Thrd_SpawnErr,
    /**
     * A system-imposed limit on the number of threads was encountered.
     * There are a number of limits that may trigger this error:
     * *  the  RLIMIT_NPROC soft resource limit (set via setrlimit(2)),
     *    which limits the number of processes and threads for  a  real
     *    user ID, was reached;
     * *  the kernel's system-wide limit on the number of processes and
     *    threads,  /proc/sys/kernel/threads-max,  was   reached   (see
     *    proc(5));
     * *  the  maximum  number  of  PIDs, /proc/sys/kernel/pid_max, was
     *    reached (see proc(5)); or
     * *  the PID limit (pids.max) imposed by the cgroup "process  num‐
     *    ber" (PIDs) controller was reached.
     */
    ThreadQuotaExceeded,
    /**
    * The kernel cannot allocate sufficient memory to allocate a task structure
    * for the child, or to copy those parts of the caller's context that need to
    * be copied.
    SystemResources,
    * Not enough userland memory to spawn the thread.
    OutOfMemory,
    * `mlockall` is enabled, and the memory needed to spawn the thread
    * would exceed the limit.
    */
    LockedMemoryLimitExceeded,
    Unexpected
);
typedef i32 (*ThrdStartFn)(void* arg);
/**
 * Spawns a new thread which executes `function` using `args` and returns a ThrdHandle to the spawned thread.
 * `config` can be used as hints to the platform for how to spawn and execute the `function`.
 * The caller must eventually either call `join()` to wait for the thread to finish and free its resources
 * or call `detach()` to excuse the caller from calling `join()` and have the thread clean up its resources on completion.
 */
extern Err$Thrd         Thrd_spawn(ThrdSpawnConfig config, ThrdStartFn function, void* arg);
/**
 * Returns the ThrdHandle of this thread
 */
force_inline ThrdHandle Thrd_getHandle(Thrd self) {
    return self.handle_;
}

/**
 * Release the obligation of the caller to call `join()` and have the thread clean up its own resources on completion.
 * Once called, this consumes the Thread object and invoking any other functions on it is considered undefined behavior.
 */
extern void Thrd_detach(Thrd self);

/**
 * Waits for the thread to complete, then deallocates any resources created on `spawn()`.
 * Once called, this consumes the Thread object and invoking any other functions on it is considered undefined behavior.
 */
extern void Thrd_join(Thrd self);

use_Err(
    Thrd_YieldErr,
    /**
     * The system is not configured to allow yielding
     */
    SystemCannotYield
);
/**
 * Yields the current thread potentially allowing other threads to run.
 */
extern Err$void Thrd_yield(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_COMMON_INCLUDED */
