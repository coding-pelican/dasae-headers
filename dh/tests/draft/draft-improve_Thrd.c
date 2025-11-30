#include "dh/prl.h"

#define Atom$(_T...) pp_join($, atom, _T)
#define Atom$$(_T...) \
    struct { \
        volatile _T raw; \
    }
#define T_use_Atom$(_T...) \
    struct Atom$(_T) { \
        volatile _T raw; \
    } Atom$(_T)

typedef enum Atom_MemOrd {
    Atom_MemOrd_unordered = __ATOMIC_RELAXED,
    Atom_MemOrd_monotonic = __ATOMIC_RELAXED,
    Atom_MemOrd_acquire = __ATOMIC_ACQUIRE,
    Atom_MemOrd_release = __ATOMIC_RELEASE,
    Atom_MemOrd_acq_rel = __ATOMIC_ACQ_REL,
    Atom_MemOrd_seq_cst = __ATOMIC_SEQ_CST
} Atom_MemOrd;

#define Atom_swap(_val, _new_val, _order) \
    __atomic_exchange_n(&(_val).raw, (_new_val), (_order))
#define Atom_load(_val, val_order) \
    __atomic_load_n(&(_val).raw, (val_order))

#include "dh/os/windows.h"

fn_((os_windows_GetCurrentThreadId(void))(DWORD)) {
    return GetCurrentThreadId();
}

fn_((os_windows_GetSystemInfo(void))(SYSTEM_INFO)) {
    SYSTEM_INFO sysInfo = {};
    GetSystemInfo(&sysInfo);
    return sysInfo;
}

errset_((os_windows_WaitForSingleObjectError)(
    WaitAbandoned,
    WaitTimeOut,
    Unexpected
));

fn_((os_windows_WaitForSingleObjectEx(HANDLE handle, DWORD milliseconds, bool alertable))(os_windows_WaitForSingleObjectError$void) $scope) {
    switch (WaitForSingleObjectEx(handle, milliseconds, alertable)) {
        case_((WAIT_ABANDONED)(return_err(os_windows_WaitForSingleObjectError_WaitAbandoned())));
        case_((WAIT_OBJECT_0)(return_ok({})));
        case_((WAIT_TIMEOUT)(return_err(os_windows_WaitForSingleObjectError_WaitTimeOut())));
        case_((WAIT_FAILED)(return_err(os_windows_WaitForSingleObjectError_Unexpected())));
        default_((claim_unreachable));
    }
} $unscoped_(fn);

fn_((os_windows_WaitForSingleObject(HANDLE handle, DWORD milliseconds))(os_windows_WaitForSingleObjectError$void)) {
    return os_windows_WaitForSingleObjectEx(handle, milliseconds, false);
}

typedef pp_if_(pp_eq(os_type, os_type_windows))(
    pp_then_(DWORD),
    pp_else_(i32)
) Thrd_Id;

typedef pp_if_(pp_eq(os_type, os_type_windows))(
    pp_then_(HANDLE),
    pp_else_(i32)
) Thrd_Handle;

typedef Atom$$(enum Thrd_CompletionState : u8{
    Thrd_CompletionState_running = 0,
    Thrd_CompletionState_completed = 1,
    Thrd_CompletionState_detached = 2,
}) Thrd_CompletionState;

#if os_type == os_type_windows

typedef struct Thrd_Completion {
    Thrd_CompletionState completion;
    PVOID heap_ptr;
    HANDLE heap_handle;
    HANDLE handle;
} Thrd_Completion;

fn_((Thrd_Completion_free(Thrd_Completion* self))(void)) {
    let status = HeapFree(self->heap_handle, 0, self->heap_ptr);
    claim_assert(status != 0);
}

typedef struct Thrd {
    Thrd_Completion* inner;
} Thrd;

fn_((Thrd_getCurrentId(void))(Thrd_Id)) {
    return os_windows_GetCurrentThreadId();
}

fn_((Thrd_getCpuCount(void))(usize)) {
    return as$(usize)(os_windows_GetSystemInfo().dwNumberOfProcessors);
}

/* fn_((Thrd_spawn(Thrd_SpawnConfig config, comptime f : anytype, args : anytype))(Thrd)) {
    const Args = @TypeOf(args);
    const Instance = struct {
        fn_args : Args,
                  thread : ThreadCompletion,

                           fn entryFn(raw_ptr : windows.PVOID) callconv(.winapi) windows.DWORD {
            const self : *@This() = @ptrCast(@alignCast(raw_ptr));
            defer switch (self.thread.completion.swap(.completed, .seq_cst)){
                .running =>{},
                .completed = > unreachable,
                .detached = > self.thread.free(),
                };
            return call(f, self.fn_args);
        }
    };

    const heap_handle = windows.kernel32.GetProcessHeap() orelse return error.OutOfMemory;
    const alloc_bytes = @alignOf(Instance) + @sizeOf(Instance);
    const alloc_ptr = windows.kernel32.HeapAlloc(heap_handle, 0, alloc_bytes) orelse return error.OutOfMemory;
    errdefer assert(windows.kernel32.HeapFree(heap_handle, 0, alloc_ptr) != 0);

    const instance_bytes = @as([*] u8, @ptrCast(alloc_ptr))[0..alloc_bytes];
    var fba = std.heap.FixedBufferAllocator.init(instance_bytes);
    const instance = fba.allocator().create(Instance) catch unreachable;
    instance.* =.{
        .fn_args = args,
        .thread =.{
            .completion = Completion.init(.running),
            .heap_ptr = alloc_ptr,
            .heap_handle = heap_handle,
        },
    };

    // Windows appears to only support SYSTEM_INFO.dwAllocationGranularity minimum stack size.
    // Going lower makes it default to that specified in the executable (~1mb).
    // Its also fine if the limit here is incorrect as stack size is only a hint.
    var stack_size = std.math.cast(u32, config.stack_size) orelse std.math.maxInt(u32);
    stack_size = @max(64 * 1024, stack_size);

    instance.thread.thread_handle = windows.kernel32.CreateThread(
        null,
        stack_size,
        Instance.entryFn,
        instance,
        0,
        null,
    ) orelse {
        const errno = windows.GetLastError();
        return windows.unexpectedError(errno);
    };

    return Impl{ .thread = &instance.thread };
} */

fn_((Thrd_getHandle(Thrd self))(Thrd_Handle)) {
    return self.inner->handle;
}

fn_((Thrd_detach(Thrd self))(void)) {
    CloseHandle(self.inner->handle);
    switch (Atom_swap(self.inner->completion, Thrd_CompletionState_detached, Atom_MemOrd_seq_cst)) {
    case Thrd_CompletionState_running:
        break;
    case Thrd_CompletionState_completed:
        Thrd_Completion_free(self.inner);
        break;
    case Thrd_CompletionState_detached:
    default:
        claim_unreachable;
        break;
    }
}

fn_((Thrd_join(Thrd self))(void)) {
    catch_((os_windows_WaitForSingleObject(self.inner->handle, INFINITE))($ignore, claim_unreachable));
    CloseHandle(self.inner->handle);
    claim_assert(Atom_load(self.inner->completion, Atom_MemOrd_seq_cst) == Thrd_CompletionState_completed);
    Thrd_Completion_free(self.inner);
}

#endif


/* const LinuxThreadImpl = struct {
    const linux = std.os.linux;

    pub const ThreadHandle = i32;

    threadlocal var tls_thread_id : ? Id = null;

    fn getCurrentId() Id {
        return tls_thread_id orelse {
            const tid : u32 = @bitCast(linux.gettid());
            tls_thread_id = tid;
            return tid;
        };
    }

    fn getCpuCount() !usize {
        const cpu_set = try posix.sched_getaffinity(0);
        return posix.CPU_COUNT(cpu_set);
    }

    thread : *ThreadCompletion,

             const ThreadCompletion = struct {
        completion : Completion = Completion.init(.running),
                     child_tid : std.atomic.Value(i32) = std.atomic.Value(i32).init(1),
                     parent_tid : i32 = undefined,
                     mapped : [] align(std.heap.page_size_min) u8,

                     /// Calls `munmap(mapped.ptr, mapped.len)` then `exit(1)` without touching the stack (which lives in `mapped.ptr`).
                     /// Ported over from musl libc's pthread detached implementation:
                     /// https://github.com/ifduyue/musl/search?q=__unmapself
                              fn freeAndExit(self : *ThreadCompletion) noreturn {
                                  switch (target.cpu.arch) {
                                      .x86 = > asm volatile(
                    \\ movl $91, % % eax #SYS_munmap
                    \\ movl % [ptr], % % ebx
                    \\ movl % [len], % % ecx
                    \\ int $128
                    \\ movl $1, % % eax #SYS_exit
                    \\ movl $0, % % ebx
                    \\ int $128 : : [ptr] "r"(@intFromPtr(self.mapped.ptr)),
                                                 [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .x86_64 = > asm volatile(
                    \\ movq $11, % % rax #SYS_munmap
                    \\ syscall
                    \\ movq $60, % % rax #SYS_exit
                    \\ movq $1, % % rdi
                    \\ syscall : : [ptr] "{rdi}"(@intFromPtr(self.mapped.ptr)),
                                                    [len] "{rsi}"(self.mapped.len), ),
                                      .arm, .armeb, .thumb, .thumbeb = > asm volatile(
                    \\ mov r7, #91 // SYS_munmap
                    \\ mov r0, % [ptr]
                    \\ mov r1, % [len]
                    \\ svc 0
                    \\ mov r7, #1 // SYS_exit
                    \\ mov r0, #0
                    \\ svc 0 : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .aarch64, .aarch64_be = > asm volatile(
                    \\ mov x8, #215 // SYS_munmap
                    \\ mov x0, % [ptr]
                    \\ mov x1, % [len]
                    \\ svc 0
                    \\ mov x8, #93 // SYS_exit
                    \\ mov x0, #0
                    \\ svc 0 : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .hexagon = > asm volatile(
                    \\ r6 = #215 // SYS_munmap
                    \\ r0 = % [ptr]
                    \\ r1 = % [len]
                    \\ trap0(#1)
                    \\ r6 = #93 // SYS_exit
                    \\ r0 = #0
                    \\ trap0(#1) : : [ptr] "r"(@intFromPtr(self.mapped.ptr)),
                                                     [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      // We set `sp` to the address of the current function as a workaround for a Linux
                                      // kernel bug that caused syscalls to return EFAULT if the stack pointer is invalid.
                                      // The bug was introduced in 46e12c07b3b9603c60fc1d421ff18618241cb081 and fixed in
                                      // 7928eb0370d1133d0d8cd2f5ddfca19c309079d5.
                                          .mips, .mipsel = > asm volatile(
                    \\ move $sp, $25
                    \\ li $2, 4091 #SYS_munmap
                    \\ move $4, % [ptr]
                    \\ move $5, % [len]
                    \\ syscall
                    \\ li $2, 4001 #SYS_exit
                    \\ li $4, 0
                    \\ syscall : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .mips64, .mips64el = > asm volatile(
                    \\ li $2, 5011 #SYS_munmap
                    \\ move $4, % [ptr]
                    \\ move $5, % [len]
                    \\ syscall
                    \\ li $2, 5058 #SYS_exit
                    \\ li $4, 0
                    \\ syscall : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .powerpc, .powerpcle, .powerpc64, .powerpc64le = > asm volatile(
                    \\ li 0, 91 #SYS_munmap
                    \\ mr 3, % [ptr]
                    \\ mr 4, % [len]
                    \\ sc
                    \\ li 0, 1 #SYS_exit
                    \\ li 3, 0
                    \\ sc
                    \\ blr : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .riscv32, .riscv64 = > asm volatile(
                    \\ li a7, 215 #SYS_munmap
                    \\ mv a0, % [ptr]
                    \\ mv a1, % [len]
                    \\ ecall
                    \\ li a7, 93 #SYS_exit
                    \\ mv a0, zero
                    \\ ecall : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .s390x = > asm volatile(
                    \\ lgr % % r2, % [ptr]
                    \\ lgr % % r3, % [len]
                    \\ svc 91 #SYS_munmap
                    \\ lghi % % r2, 0
                    \\ svc 1 #SYS_exit : : [ptr] "r"(@intFromPtr(self.mapped.ptr)),
                                                   [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .sparc = > asm volatile(
                    \\ #See sparc64 comments below.
                    \\ 1 :
                    \\ cmp % % fp, 0
                    \\ beq 2f
                    \\ nop
                    \\ ba 1b
                    \\ restore
                    \\ 2 :
                    \\ mov 73, % % g1 // SYS_munmap
                    \\ mov % [ptr], % % o0
                    \\ mov % [len], % % o1
                    \\ t 0x3 #ST_FLUSH_WINDOWS
                    \\ t 0x10
                    \\ mov 1, % % g1 // SYS_exit
                    \\ mov 0, % % o0
                    \\ t 0x10 : : [ptr] "r"(@intFromPtr(self.mapped.ptr)),
                                                   [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .sparc64 = > asm volatile(
                    \\ #SPARCs really don't like it when active stack frames
                    \\ #is unmapped(it will result in a segfault), so we
                    \\ #force - deactivate it by running `restore` until
                    \\ #all frames are cleared.
                    \\ 1 :
                    \\ cmp % % fp, 0
                    \\ beq 2f
                    \\ nop
                    \\ ba 1b
                    \\ restore
                    \\ 2 :
                    \\ mov 73, % % g1 // SYS_munmap
                    \\ mov % [ptr], % % o0
                    \\ mov % [len], % % o1
                    \\ #Flush register window contents to prevent background
                    \\ #memory access before unmapping the stack.
                    \\ flushw
                    \\ t 0x6d
                    \\ mov 1, % % g1 // SYS_exit
                    \\ mov 0, % % o0
                    \\ t 0x6d : : [ptr] "r"(@intFromPtr(self.mapped.ptr)),
                                                     [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      .loongarch32, .loongarch64 = > asm volatile(
                    \\ or $a0, $zero, % [ptr]
                    \\ or $a1, $zero, % [len]
                    \\ ori $a7, $zero, 215 #SYS_munmap
                    \\ syscall 0 #call munmap
                    \\ ori $a0, $zero, 0
                    \\ ori $a7, $zero, 93 #SYS_exit
                    \\ syscall 0 #call exit : : [ptr] "r"(@intFromPtr(self.mapped.ptr)), [len] "r"(self.mapped.len), :.{ .memory = true }),
                                      else = > | cpu_arch | @compileError("Unsupported linux arch: " ++@tagName(cpu_arch)),
                                  }
                                  unreachable;
                              }
    };

    fn spawn(config : SpawnConfig, comptime f : anytype, args : anytype) !Impl {
        const page_size = std.heap.pageSize();
        const Args = @TypeOf(args);
        const Instance = struct {
            fn_args : Args,
                      thread : ThreadCompletion,

                               fn
                               entryFn(raw_arg : usize) callconv(.c) u8 {
                const self = @as(*@This(), @ptrFromInt(raw_arg));
                defer switch (self.thread.completion.swap(.completed, .seq_cst)){
                    .running =>{},
                    .completed = > unreachable,
                    .detached = > self.thread.freeAndExit(),
                    };
                return call(f, self.fn_args);
            }
        };

        var guard_offset : usize = undefined;
        var stack_offset : usize = undefined;
        var tls_offset : usize = undefined;
        var instance_offset : usize = undefined;

        const map_bytes = blk : {
            var bytes : usize = page_size;
            guard_offset = bytes;

            bytes += @max(page_size, config.stack_size);
            bytes = std.mem.alignForward(usize, bytes, page_size);
            stack_offset = bytes;

            bytes = std.mem.alignForward(usize, bytes, linux.tls.area_desc.alignment);
            tls_offset = bytes;
            bytes += linux.tls.area_desc.size;

            bytes = std.mem.alignForward(usize, bytes, @alignOf(Instance));
            instance_offset = bytes;
            bytes += @sizeOf(Instance);

            bytes = std.mem.alignForward(usize, bytes, page_size);
            break : blk bytes;
        };

        // map all memory needed without read/write permissions
        // to avoid committing the whole region right away
        // anonymous mapping ensures file descriptor limits are not exceeded
        const mapped = posix.mmap(
                           null,
                           map_bytes,
                           posix.PROT.NONE,
                           .{ .TYPE =.PRIVATE, .ANONYMOUS = true },
                           -1,
                           0,
                       ) catch
                     | err | switch (err){
                           error.MemoryMappingNotSupported = > unreachable,
                           error.AccessDenied = > unreachable,
                           error.PermissionDenied = > unreachable,
                           error.ProcessFdQuotaExceeded = > unreachable,
                           error.SystemFdQuotaExceeded = > unreachable,
                           error.MappingAlreadyExists = > unreachable,
                           else = > | e | return e,
                       };
        assert(mapped.len >= map_bytes);
        errdefer posix.munmap(mapped);

        // map everything but the guard page as read/write
        posix.mprotect(
            @alignCast(mapped[guard_offset..]),
            posix.PROT.READ | posix.PROT.WRITE,
        ) catch
            | err | switch (err){
                  error.AccessDenied = > unreachable,
                  else = > | e | return e,
              };

        // Prepare the TLS segment and prepare a user_desc struct when needed on x86
        var tls_ptr = linux.tls.prepareArea(mapped[tls_offset..]);
        var user_desc : if (target.cpu.arch ==.x86) linux.user_desc else void = undefined;
        if (target.cpu.arch ==.x86) {
            defer tls_ptr = @intFromPtr(&user_desc);
            user_desc =.{
                .entry_number = linux.tls.area_desc.gdt_entry_number,
                .base_addr = tls_ptr,
                .limit = 0xfffff,
                .flags =.{
                    .seg_32bit = 1,
                    .contents = 0, // Data
                    .read_exec_only = 0,
                    .limit_in_pages = 1,
                    .seg_not_present = 0,
                    .useable = 1,
                },
            };
        }

        const instance : * Instance = @ptrCast(@alignCast(&mapped[instance_offset]));
        instance.* =.{
            .fn_args = args,
            .thread =.{ .mapped = mapped },
        };

        const flags : u32 = linux.CLONE.THREAD | linux.CLONE.DETACHED | linux.CLONE.VM | linux.CLONE.FS | linux.CLONE.FILES | linux.CLONE.PARENT_SETTID | linux.CLONE.CHILD_CLEARTID | linux.CLONE.SIGHAND | linux.CLONE.SYSVSEM | linux.CLONE.SETTLS;

        switch (linux.E.init(linux.clone(
            Instance.entryFn,
            @intFromPtr(&mapped[stack_offset]),
            flags,
            @intFromPtr(instance),
            &instance.thread.parent_tid,
            tls_ptr,
            &instance.thread.child_tid.raw,
        ))) {
            .SUCCESS = > return Impl{ .thread = &instance.thread },
            .AGAIN = > return error.ThreadQuotaExceeded,
            .INVAL = > unreachable,
            .NOMEM = > return error.SystemResources,
            .NOSPC = > unreachable,
            .PERM = > unreachable,
            .USERS = > unreachable,
            else = > | err | return posix.unexpectedErrno(err),
        }
    }

    fn getHandle(self : Impl) ThreadHandle {
        return self.thread.parent_tid;
    }

    fn detach(self : Impl) void {
        switch (self.thread.completion.swap(.detached, .seq_cst)) {
            .running =>{},
                .completed = > self.join(),
                .detached = > unreachable,
        }
    }

    fn join(self : Impl) void {
        defer posix.munmap(self.thread.mapped);

        while (true) {
            const tid = self.thread.child_tid.load(.seq_cst);
            if (tid == 0) {
                break;
            }

            switch (linux.E.init(linux.futex_4arg(
                &self.thread.child_tid.raw,
                .{ .cmd =.WAIT, .private = false },
                @bitCast(tid),
                null,
            ))) {
                .SUCCESS = > continue,
                .INTR = > continue,
                .AGAIN = > continue,
                else = > unreachable,
            }
        }
    }
};
 */


#if UNUSED_CODE
#undef Thrd_FnCtx$
#define Thrd_FnCtx$(_fnName...) __alias__Thrd_FnCtx$(_fnName)
#define __alias__Thrd_FnCtx$(_fnName...) pp_join($, Thrd_FnCtx, _fnName)

#define Thrd_FnCtx_from$(/*(_fnName)(_args...)*/... /*(Thrd_FnCtx$(_fnName))*/) \
    __step__Thrd_FnCtx_from$(__step__Thrd_FnCtx_from$__parseFnName __VA_ARGS__)
#define __step__Thrd_FnCtx_from$(...) \
    __step__Thrd_FnCtx_from$__emit(__VA_ARGS__)
#define __step__Thrd_FnCtx_from$__parseFnName(_fnName...) \
    _fnName,
#define __step__Thrd_FnCtx_from$__emit(_fnName, _args...) lit$((Thrd_FnCtx$(_fnName)){ \
    .fn = _fnName, \
    .args = { .as_typed = { __step__Thrd_FnCtx_from$__expand _args } }, \
    .ret = { .as_typed = {} }, \
})
#define __step__Thrd_FnCtx_from$__expand(_args...) \
    _args

#define Thrd_FnCtx_ret$(/*(_fnName)(_ctx...)*/... /*(_ReturnType$(_fnName))*/) \
    __step__Thrd_FnCtx_ret$(__step__Thrd_FnCtx_ret$__parseFnName __VA_ARGS__)
#define __step__Thrd_FnCtx_ret$(...) \
    __step__Thrd_FnCtx_ret$__emit(__VA_ARGS__)
#define __step__Thrd_FnCtx_ret$__parseFnName(_fnName...) \
    _fnName,
#define __step__Thrd_FnCtx_ret$__emit(_fnName, _ctx...) \
    (as$(Thrd_FnCtx$(_fnName) (*)(_ctx))->ret.as_typed)
#endif /* UNUSED_CODE */
