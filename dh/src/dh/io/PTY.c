#include "dh/io/PTY.h"
#include "dh/fs/Dir.h"
#include "dh/mem/common.h"
#include "../proc/private/share.h"

T_use_E$($set(io_PTY_SpawnE)(S$u16));

#if plat_is_windows
#include "dh/sys/api/windows/console.h"
#include "dh/sys/api/windows/dll.h"
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/sync.h"
#endif /* plat_is_windows */
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif /* plat_is_linux */

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused $must_check)
$static fn_((io_PTY__unsupported_open(io_PTY_OpenCfg cfg))(io_PTY_OpenE$io_PTY));
$attr($maybe_unused)
$static fn_((io_PTY__unsupported_close(io_PTY* self))(void));
$attr($maybe_unused $must_check)
$static fn_((io_PTY__unsupported_resize(io_PTY* self, io_PTY_Size size))(io_PTY_ResizeE$void));
#if plat_is_linux
$static fn_((io_PTY__linux_open(io_PTY_OpenCfg cfg))(io_PTY_OpenE$io_PTY));
$static fn_((io_PTY__linux_close(io_PTY* self))(void));
$static fn_((io_PTY__linux_resize(io_PTY* self, io_PTY_Size size))(io_PTY_ResizeE$void));
$static fn_((io_PTY__linux_slavePath(u32 number, S$u8 out))(S_const$u8));
$static fn_((io_PTY__linux_exec(proc_Spawn_Opts cmd, proc_NativeStrList argv, proc_NativeStrList env))(void));
#endif /* plat_is_linux */

#if plat_is_windows
$static fn_((io_PTY__windows_open(io_PTY_OpenCfg cfg))(io_PTY_OpenE$io_PTY));
$static fn_((io_PTY__windows_close(io_PTY* self))(void));
$static fn_((io_PTY__windows_resize(io_PTY* self, io_PTY_Size size))(io_PTY_ResizeE$void));
$attr($callconv_stdcall)
typedef fn_(((*io_PTY__CreatePseudoConsoleFn)(COORD size, HANDLE input, HANDLE output, DWORD flags, HPCON* pc))(HRESULT));
$attr($callconv_stdcall)
typedef fn_(((*io_PTY__ResizePseudoConsoleFn)(HPCON pc, COORD size))(HRESULT));
$attr($callconv_stdcall)
typedef fn_(((*io_PTY__ClosePseudoConsoleFn)(HPCON pc))(void));

T_alias$((io_PTY__ConPtyApi)(struct io_PTY__ConPtyApi {
    var_(create, io_PTY__CreatePseudoConsoleFn);
    var_(resize, io_PTY__ResizePseudoConsoleFn);
    var_(close, io_PTY__ClosePseudoConsoleFn);
}));

$static fn_((io_PTY__loadConPtyApi(io_PTY__ConPtyApi* out))(bool));
$static fn_((io_PTY__mapWindowsOpenError(DWORD error))(EAny));
$static fn_((io_PTY__windowsHpc(const io_PTY* self))(HPCON));
#endif /* plat_is_windows */

$static fn_((io_PTY__validSize(io_PTY_Size size))(bool));
$static fn_((io_PTY__closeFiles(io_PTY* self))(void));

$static let io_PTY__open = pp_if_(plat_is_windows)(
    pp_then_(io_PTY__windows_open),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(io_PTY__linux_open),
        pp_else_(io_PTY__unsupported_open)
    )));
$static let io_PTY__close = pp_if_(plat_is_windows)(
    pp_then_(io_PTY__windows_close),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(io_PTY__linux_close),
        pp_else_(io_PTY__unsupported_close)
    )));
$static let io_PTY__resize = pp_if_(plat_is_windows)(
    pp_then_(io_PTY__windows_resize),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(io_PTY__linux_resize),
        pp_else_(io_PTY__unsupported_resize)
    )));

/*========== External Definitions ===========================================*/

/*---------- PTY Lifecycle --------------------------------------------------*/

fn_((io_PTY_open(io_PTY_OpenCfg cfg))(io_PTY_OpenE$io_PTY) $guard) {
    if (!io_PTY__validSize(cfg.size)) return_err(E_cause$io_PTY_InvalidSize());
    return_(io_PTY__open(cfg));
} $unguarded(fn);

fn_((io_PTY_close(io_PTY* self))(void)) {
    claim_assert_nonnull(self);
    io_PTY__close(self);
};

fn_((io_PTY_reader(io_PTY* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return fs_File_reader(self->read_file);
};

fn_((io_PTY_writer(io_PTY* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return fs_File_writer(orelse_((self->write_file)(self->read_file)));
};

fn_((io_PTY_resize(io_PTY* self, io_PTY_Size size))(io_PTY_ResizeE$void) $scope) {
    claim_assert_nonnull(self);
    if (!io_PTY__validSize(size)) return_err(E_cause$io_PTY_InvalidSize());
    return_(io_PTY__resize(self, size));
} $unscoped(fn);

/*---------- PTY Session ----------------------------------------------------*/

fn_((io_PTY_spawn(io_PTY_SpawnCfg cfg))(io_PTY_SpawnE$io_PTY_Session) $guard) {
    if (cfg.cmd.argv.len == 0) return_err(E_cause$io_PTY_InvalidCommand());
#if plat_is_windows
    if (!matches(cfg.cmd.cwd, proc_cmd_CWD_inherit)) {
        return_err(E_cause$io_PTY_Unsupported());
    }
    var pty = try_(io_PTY_open(cfg.pty));
    errdefer_($ignore, io_PTY_close(&pty));
    var cmd_line = catch_((proc__cmdLineWTF16(cfg.gpa, cfg.cmd.argv))(err, {
        if (E_eql(err.as_any, E_cause$proc_InvalidName().as_any)) {
            return_err(E_cause$io_PTY_InvalidCommand());
        }
        return_err(err);
    }));
    defer_(mem_Alctr_free($trace cfg.gpa, u_anyS(cmd_line)));
    var env = catch_((proc__envWTF16(
        cfg.gpa,
        cfg.env,
        cfg.cmd.env
    ))(err, {
        if (E_eql(err.as_any, E_cause$proc_InvalidName().as_any)) {
            return_err(E_cause$io_PTY_InvalidCommand());
        }
        return_err(err);
    }));
    defer_(mem_Alctr_free($trace cfg.gpa, u_anyS(env)));

    SIZE_T attr_size = 0;
    InitializeProcThreadAttributeList(null, 1, 0, &attr_size);
    let word_count = (as$(usize)(attr_size) + sizeOf$(usize) - 1) / sizeOf$(usize);
    let attr_mem = u_castS$((S$usize)(try_(mem_Alctr_alloc($trace cfg.gpa, typeInfo$(usize), word_count))));
    defer_(mem_Alctr_free($trace cfg.gpa, u_anyS(attr_mem)));
    let attrs = as$(LPPROC_THREAD_ATTRIBUTE_LIST)(attr_mem.ptr);
    if (!InitializeProcThreadAttributeList(attrs, 1, 0, &attr_size)) return_err(E_cause$io_PTY_SysResrcs());
    defer_(DeleteProcThreadAttributeList(attrs));
    let hpc = io_PTY__windowsHpc(&pty);
    if (!UpdateProcThreadAttribute(attrs, 0, usize_(0x00020016), hpc, sizeof(HPCON), null, null)) {
        return_err(E_cause$io_PTY_SysResrcs());
    }

    STARTUPINFOEXW startup = cleared();
    startup.StartupInfo.cb = sizeOf$(STARTUPINFOEXW);
    startup.lpAttributeList = attrs;
    PROCESS_INFORMATION process = cleared();
    var_(flags, DWORD) = EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT;
    if (cfg.cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cfg.cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessW(
            null,
            as$(LPWSTR)(cmd_line.ptr),
            null,
            null,
            FALSE,
            flags,
            env.ptr,
            null,
            &startup.StartupInfo,
            &process
        )) {
        return_err(E_cause$io_PTY_SpawnFailed());
    }
    CloseHandle(process.hThread);
    return_ok((io_PTY_Session){
        .pty = pty,
        .child = {
            .handle = some(as$(proc_Child_Handle)(process.hProcess)),
            .id = (as$(proc_Child_Id)(process.dwProcessId)),
            .io = {
                .in = none(),
                .out = none(),
                .err = none(),
            },
        },
    });
#elif plat_is_linux
    if (cfg.cmd.expand_arg0 == proc_cmd_ArgExpsn_expand) return_err(E_cause$io_PTY_InvalidCommand());
    if (matches(cfg.cmd.cwd, proc_cmd_CWD_path)) {
        return_err(E_cause$io_PTY_Unsupported());
    }
    var pty = try_(io_PTY_open(cfg.pty));
    errdefer_($ignore, io_PTY_close(&pty));
    var argv = try_(proc_NativeStrList_from(cfg.gpa, cfg.cmd.argv));
    defer_(proc_NativeStrList_fini(&argv, cfg.gpa));
    var env = try_(proc_NativeStrList_fromEnv(
        cfg.gpa,
        cfg.env,
        cfg.cmd.env
    ));
    defer_(proc_NativeStrList_fini(&env, cfg.gpa));

    let pid = sys_call_linux_fork();
    if (sys_call_linux_syscall_isErr(pid)) return_err(E_cause$io_PTY_SpawnFailed());
    if (pid == 0) {
        let master_fd = as$(sys_call_linux_word)(fs_File_handle(pty.read_file));
        let slave = unwrap_(pty.slave_file);
        let slave_fd = as$(sys_call_linux_word)(fs_File_handle(slave));
        let_ignore = sys_call_linux_close(master_fd);
        if (sys_call_linux_setsid() < 0) sys_call_linux_exit_group(127);
        if (sys_call_linux_ioctl(slave_fd, sys_call_linux_TIOCSCTTY, null) < 0) sys_call_linux_exit_group(127);
        if (slave_fd != 0) {
            if (sys_call_linux_dup3(slave_fd, 0, 0) < 0) sys_call_linux_exit_group(127);
        }
        if (slave_fd != 1) {
            if (sys_call_linux_dup3(slave_fd, 1, 0) < 0) sys_call_linux_exit_group(127);
        }
        if (slave_fd != 2) {
            if (sys_call_linux_dup3(slave_fd, 2, 0) < 0) sys_call_linux_exit_group(127);
        }
        if (slave_fd > 2) {
            let_ignore = sys_call_linux_close(slave_fd);
        }
        io_PTY__linux_exec(cfg.cmd, argv, env);
    }
    fs_File_close(unwrap_(pty.slave_file));
    asg_l((&pty.slave_file)(none()));
    return_ok((io_PTY_Session){
        .pty = pty,
        .child = {
            .handle = some(as$(proc_Child_Handle)(pid)),
            .id = as$(proc_Child_Id)(pid),
            .io = {
                .in = none(),
                .out = none(),
                .err = none(),
            },
        },
    });
#else
    let_ignore = cfg;
    return_err(E_cause$io_PTY_Unsupported());
#endif
} $unguarded(fn);

fn_((io_PTY_Session_close(
    io_PTY_Session* self,
    proc_Self proc
))(void)) {
    claim_assert_nonnull(self);
    proc_Child_kill(&self->child, proc);
    io_PTY_close(&self->pty);
    asg_l((self)(cleared()));
};

fn_((io_PTY_Session_reader(io_PTY_Session* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return io_PTY_reader(&self->pty);
};

fn_((io_PTY_Session_writer(io_PTY_Session* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return io_PTY_writer(&self->pty);
};

fn_((io_PTY_Session_resize(io_PTY_Session* self, io_PTY_Size size))(io_PTY_ResizeE$void) $scope) {
    claim_assert_nonnull(self);
    return_(io_PTY_resize(&self->pty, size));
} $unscoped(fn);

fn_((io_PTY_Session_wait(
    io_PTY_Session* self,
    proc_Self proc
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    claim_assert_nonnull(self);
    return_(proc_Child_wait(&self->child, proc));
} $unscoped(fn);

fn_((io_PTY_Session_kill(
    io_PTY_Session* self,
    proc_Self proc
))(void)) {
    claim_assert_nonnull(self);
    proc_Child_kill(&self->child, proc);
};

/*========== Internal Definitions ===========================================*/

/*---------- Common ---------------------------------------------------------*/

fn_((io_PTY__validSize(io_PTY_Size size))(bool)) {
    return size.cols != 0 && size.rows != 0;
};

fn_((io_PTY__closeFiles(io_PTY* self))(void)) {
    claim_assert_nonnull(self);
    fs_File_close(self->read_file);
    if_some((self->write_file)(file)) fs_File_close(file);
    if_some((self->slave_file)(file)) fs_File_close(file);
    asg_l((self)(cleared()));
};

fn_((io_PTY__unsupported_open(
    io_PTY_OpenCfg cfg
))(io_PTY_OpenE$io_PTY) $scope) {
    let_ignore = cfg;
    return_err(E_cause$io_PTY_Unsupported());
} $unscoped(fn);

fn_((io_PTY__unsupported_close(io_PTY* self))(void)) {
    io_PTY__closeFiles(self);
};

fn_((io_PTY__unsupported_resize(
    io_PTY* self, io_PTY_Size size
))(io_PTY_ResizeE$void) $scope) {
    claim_assert_nonnull(self);
    let_ignore = size;
    return_err(E_cause$io_PTY_Unsupported());
} $unscoped(fn);

/*---------- Windows ConPTY Backend -----------------------------------------*/

#if plat_is_windows
fn_((io_PTY__loadConPtyApi(io_PTY__ConPtyApi* out))(bool)) {
    claim_assert_nonnull(out);
    let kernel32 = GetModuleHandleA("kernel32.dll");
    if (kernel32 == null) return false;
    out->create = as$(io_PTY__CreatePseudoConsoleFn)(GetProcAddress(kernel32, "CreatePseudoConsole"));
    out->resize = as$(io_PTY__ResizePseudoConsoleFn)(GetProcAddress(kernel32, "ResizePseudoConsole"));
    out->close = as$(io_PTY__ClosePseudoConsoleFn)(GetProcAddress(kernel32, "ClosePseudoConsole"));
    return out->create != null && out->resize != null && out->close != null;
};

fn_((io_PTY__mapWindowsOpenError(DWORD error))(EAny)) {
    switch (error) {
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case_((ERROR_NO_SYSTEM_RESOURCES)) return E_cause$io_PTY_SysResrcs().any $end(case);
    default_() return E_cause$io_PTY_OpenFailed().any $end(default);
    }
};

fn_((io_PTY__windowsHpc(const io_PTY* self))(HPCON)) {
    claim_assert_nonnull(self);
    return as$(HPCON)(unwrap_(self->platform_resource));
};

fn_((io_PTY__windows_open(
    io_PTY_OpenCfg cfg
))(io_PTY_OpenE$io_PTY) $guard) {
    if (cfg.nonblocking) return_err(E_cause$io_PTY_Unsupported());
    if (isSome(cfg.slave_mode)) return_err(E_cause$io_PTY_Unsupported());
    var_(conpty, io_PTY__ConPtyApi) = cleared();
    if (!io_PTY__loadConPtyApi(&conpty)) return_err(E_cause$io_PTY_Unsupported());

    SECURITY_ATTRIBUTES security = {
        .nLength = sizeOf$(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = null,
        .bInheritHandle = FALSE,
    };
    var_(input_read, HANDLE) = null;
    var_(input_write, HANDLE) = null;
    var_(output_read, HANDLE) = null;
    var_(output_write, HANDLE) = null;
    if (!CreatePipe(&input_read, &input_write, &security, 0)) {
        return_err(io_PTY__mapWindowsOpenError(GetLastError()));
    }
    errdefer_($ignore, CloseHandle(input_read));
    errdefer_($ignore, CloseHandle(input_write));
    if (!CreatePipe(&output_read, &output_write, &security, 0)) {
        return_err(io_PTY__mapWindowsOpenError(GetLastError()));
    }
    errdefer_($ignore, CloseHandle(output_read));
    errdefer_($ignore, CloseHandle(output_write));

    let coord = (COORD){ .X = as$(SHORT)(cfg.size.cols), .Y = as$(SHORT)(cfg.size.rows) };
    var_(hpc, HPCON) = null;
    if (conpty.create(coord, input_read, output_write, 0, &hpc) != S_OK) {
        return_err(E_cause$io_PTY_OpenFailed());
    }
    CloseHandle(input_read);
    CloseHandle(output_write);
    input_read = null;
    output_write = null;
    return_ok((io_PTY){
        .read_file = fs_File_Handle_promote(output_read, fs_File_Flags_default),
        .write_file = some(fs_File_Handle_promote(input_write, fs_File_Flags_default)),
        .slave_file = none(),
        .platform_resource = some(as$(P$raw)(hpc)),
    });
} $unguarded(fn);

fn_((io_PTY__windows_close(io_PTY* self))(void)) {
    claim_assert_nonnull(self);
    if_some((self->platform_resource)(resource)) {
        var_(conpty, io_PTY__ConPtyApi) = cleared();
        claim_assert(io_PTY__loadConPtyApi(&conpty));
        conpty.close(as$(HPCON)(resource));
    }
    io_PTY__closeFiles(self);
};

fn_((io_PTY__windows_resize(
    io_PTY* self, io_PTY_Size size
))(io_PTY_ResizeE$void) $scope) {
    claim_assert_nonnull(self);
    var_(conpty, io_PTY__ConPtyApi) = cleared();
    if (!io_PTY__loadConPtyApi(&conpty)) return_err(E_cause$io_PTY_Unsupported());
    let coord = (COORD){ .X = as$(SHORT)(size.cols), .Y = as$(SHORT)(size.rows) };
    if (conpty.resize(io_PTY__windowsHpc(self), coord) != S_OK) {
        return_err(E_cause$io_PTY_ResizeFailed());
    }
    return_ok({});
} $unscoped(fn);
#endif /* plat_is_windows */

/*---------- Linux PTY Backend ----------------------------------------------*/

#if plat_is_linux
fn_((io_PTY__linux_open(
    io_PTY_OpenCfg cfg
))(io_PTY_OpenE$io_PTY) $guard) {
    let flags = sys_call_linux_O_RDWR
              | sys_call_linux_O_NOCTTY
              | (cfg.nonblocking ? sys_call_linux_O_NONBLOCK : 0);
    let master_fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/ptmx", flags, 0);
    if (sys_call_linux_syscall_isErr(master_fd)) return_err(E_cause$io_PTY_OpenFailed());
    let master = fs_File_Handle_promote(
        as$(fs_File_Handle)(master_fd),
        (fs_File_Flags){ .nonblocking = cfg.nonblocking }
    );
    errdefer_($ignore, fs_File_close(master));

    var_(locked, int) = 0;
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCSPTLCK, &locked) != 0) {
        return_err(E_cause$io_PTY_OpenFailed());
    }
    var_(number, u32) = 0;
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCGPTN, &number) != 0) {
        return_err(E_cause$io_PTY_OpenFailed());
    }
    var_(path, A$$(32, u8)) = A_zero();
    let slave_path = io_PTY__linux_slavePath(number, A_ref$((S$u8)(path)));
    let slave_fd = sys_call_linux_openat(
        sys_call_linux_AT_FDCWD,
        as$(const char*)(slave_path.ptr),
        sys_call_linux_O_RDWR | sys_call_linux_O_NOCTTY,
        0
    );
    if (sys_call_linux_syscall_isErr(slave_fd)) return_err(E_cause$io_PTY_OpenFailed());
    let slave = fs_File_Handle_promote(as$(fs_File_Handle)(slave_fd), fs_File_Flags_default);
    errdefer_($ignore, fs_File_close(slave));

    var_(size, sys_call_linux_winsize) = {
        .ws_row = cfg.size.rows,
        .ws_col = cfg.size.cols,
    };
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCSWINSZ, &size) != 0) {
        return_err(E_cause$io_PTY_OpenFailed());
    }
    if_some((cfg.slave_mode)(patch)) {
        let tty = io_TTY_init((io_TTY_Cfg){
            .input_file = slave,
            .output_file = slave,
        });
        try_(io_TTY_applyModePatch(&tty, patch));
    }
    return_ok((io_PTY){
        .read_file = master,
        .write_file = none(),
        .slave_file = some(slave),
        .platform_resource = none(),
    });
} $unguarded(fn);

fn_((io_PTY__linux_close(io_PTY* self))(void)) {
    io_PTY__closeFiles(self);
};

fn_((io_PTY__linux_resize(
    io_PTY* self, io_PTY_Size size
))(io_PTY_ResizeE$void) $scope) {
    claim_assert_nonnull(self);
    var_(native, sys_call_linux_winsize) = {
        .ws_row = size.rows,
        .ws_col = size.cols,
    };
    if (
        sys_call_linux_ioctl(
            fs_File_handle(self->read_file),
            sys_call_linux_TIOCSWINSZ,
            &native
        ) != 0
    ) {
        return_err(E_cause$io_PTY_ResizeFailed());
    }
    return_ok({});
} $unscoped(fn);

fn_((io_PTY__linux_slavePath(u32 number, S$u8 out))(S_const$u8)) {
    let prefix = u8_l("/dev/pts/");
    claim_assert(out.len >= prefix.len + 11);
    mem_copyBytes(S_prefix((out)(prefix.len)), prefix);

    var_(digits, A$$(10, u8)) $undefined;
    var_(digit_count, usize) = 0;
    do {
        *A_at((digits)[digit_count]) = as$(u8)(u8_c('0') + number % 10);
        number /= 10;
        digit_count += 1;
    } while (number != 0);
    for_(($rt(digit_count))(i)) {
        *S_at((out)[prefix.len + i]) = *A_at((digits)[digit_count - i - 1]);
    } $end(for);
    *S_at((out)[prefix.len + digit_count]) = 0;
    return S_prefix((out.as_const)(prefix.len + digit_count));
};

fn_((io_PTY__linux_exec(
    proc_Spawn_Opts cmd,
    proc_NativeStrList argv,
    proc_NativeStrList env
))(void)) {
    if (matches(cmd.cwd, proc_cmd_CWD_dir)) {
        if (
            sys_call_linux_fchdir(as$(sys_call_linux_word)(fs_Dir_handle(union_to((cmd.cwd)(proc_cmd_CWD_dir))))) != 0
        ) {
            sys_call_linux_exit_group(127);
        }
    };
    let arg0 = proc_NativeStrList_at(argv, usize_(0));
    let_ignore = sys_call_linux_execve(
        as$(const char*)(arg0.ptr), proc_NativeStrList_raw(argv), proc_NativeStrList_raw(env));
    sys_call_linux_exit_group(127);
};
#endif /* plat_is_linux */
