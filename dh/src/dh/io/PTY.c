#include "dh/io/PTY.h"
#include "dh/io/Fixed.h"
#include "dh/fs/Dir.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/console.h"
#include "dh/sys/api/windows/dll.h"
#include "dh/sys/api/windows/file.h"
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/nls.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/sync.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif

/*========== Internal Types and Declarations ================================*/

#if plat_is_linux
T_alias$((io_PTY__CStringList)(struct io_PTY__CStringList {
    var_(ptrs, S$P$u8);
    var_(items, S$S$u8);
}));
T_use_E$(io_PTY__CStringList);

$static fn_((io_PTY__freeCStringList(mem_Alctr gpa, io_PTY__CStringList* list))(void));
$static fn_((io_PTY__makeCStringList(mem_Alctr gpa, S$S_const$u8 args))(E$io_PTY__CStringList));
$static fn_((io_PTY__linuxSlavePath(u32 number, S$u8 out))(S_const$u8));
$static fn_((io_PTY__linuxTermFromStatus(int status))(proc_Ter));
$static fn_((io_PTY__linuxExec(proc_Cmd cmd, io_PTY__CStringList argv, io_PTY__CStringList env))(void));
#endif

#if plat_is_windows
typedef fn_(((WINAPI * io_PTY__CreatePseudoConsoleFn)(COORD size, HANDLE input, HANDLE output, DWORD flags, HPCON* pc))(HRESULT));
typedef fn_(((WINAPI * io_PTY__ResizePseudoConsoleFn)(HPCON pc, COORD size))(HRESULT));
typedef fn_(((WINAPI * io_PTY__ClosePseudoConsoleFn)(HPCON pc))(void));

T_alias$((io_PTY__ConPtyApi)(struct io_PTY__ConPtyApi {
    var_(create, io_PTY__CreatePseudoConsoleFn);
    var_(resize, io_PTY__ResizePseudoConsoleFn);
    var_(close, io_PTY__ClosePseudoConsoleFn);
}));

$static fn_((io_PTY__loadConPtyApi(io_PTY__ConPtyApi* out))(bool));
$static fn_((io_PTY__mapWindowsOpenError(DWORD error))(io_PTY_E));
$static fn_((io_PTY__windowsQuoteAppend(io_Writer out, S_const$u8 arg))(E$void));
$static fn_((io_PTY__windowsMakeCmdLine(mem_Alctr gpa, S$S_const$u8 argv))(E$S$u16));
$static fn_((io_PTY__windowsMakeEnvBlock(mem_Alctr gpa, S$S_const$u8 env))(E$S$u16));
$static fn_((io_PTY__windowsFreeWide(mem_Alctr gpa, S$u16 text))(void));
$static fn_((io_PTY__windowsHpc(const io_PTY* self))(HPCON));
#endif

$static fn_((io_PTY__validSize(io_PTY_Size size))(bool));

/*========== Internal Definitions ===========================================*/

/*---------- Common ---------------------------------------------------------*/

fn_((io_PTY__validSize(io_PTY_Size size))(bool)) {
    return size.cols != 0 && size.rows != 0;
}

/*---------- Linux PTY Backend ----------------------------------------------*/

#if plat_is_linux
fn_((io_PTY__freeCStringList(mem_Alctr gpa, io_PTY__CStringList* list))(void)) {
    claim_assert_nonnull(list);
    for_(($rt(list->items.len))(i)) {
        mem_Alctr_freeBytes($trace gpa, *S_at((list->items)[i]));
    } $end(for);
    mem_Alctr_free($trace gpa, u_anyS(list->ptrs));
    mem_Alctr_free($trace gpa, u_anyS(list->items));
    asg_l((list)(cleared()));
}

fn_((io_PTY__makeCStringList(mem_Alctr gpa, S$S_const$u8 args))(E$io_PTY__CStringList) $guard) {
    let ptr_count = orelse_((usize_addChkd(args.len, 1))(return_err(E_cause$OutOfMemory())));
    let ptrs = u_castS$((S$P$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(P$u8), ptr_count))));
    let items = u_castS$((S$S$u8)(catch_((mem_Alctr_alloc($trace gpa, typeInfo$(S$u8), args.len))(err, {
        mem_Alctr_free($trace gpa, u_anyS(ptrs));
        return_err(err);
    }))));
    var_(list, io_PTY__CStringList) = {
        .ptrs = ptrs,
        .items = S_prefix((items)(0)),
    };
    errdefer_($ignore, io_PTY__freeCStringList(gpa, &list));

    for_(($rt(args.len))(i)) {
        let arg = *S_at((args)[i]);
        let len = orelse_((usize_addChkd(arg.len, 1))(return_err(E_cause$OutOfMemory())));
        let dup = try_(mem_Alctr_allocBytes($trace gpa, len));
        mem_copyBytes(S_prefix((dup)(arg.len)), arg);
        *S_at((dup)[arg.len]) = 0;
        *S_at((ptrs)[i]) = dup.ptr;
        *S_at((items)[i]) = dup;
        list.items.len += 1;
    } $end(for);
    *S_at((ptrs)[args.len]) = null;
    return_ok(list);
} $unguarded(fn);

fn_((io_PTY__linuxSlavePath(u32 number, S$u8 out))(S_const$u8)) {
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
}

fn_((io_PTY__linuxTermFromStatus(int status))(proc_Ter)) {
    if ((status & 0x7f) == 0) {
        return (proc_Ter){ .tag = proc_Ter_Tag_exited, .code = as$(u32)((status >> 8) & 0xff) };
    }
    if ((status & 0xff) == 0x7f) {
        return (proc_Ter){ .tag = proc_Ter_Tag_stopped, .code = as$(u32)((status >> 8) & 0xff) };
    }
    return (proc_Ter){ .tag = proc_Ter_Tag_signal, .code = as$(u32)(status & 0x7f) };
}

fn_((io_PTY__linuxExec(proc_Cmd cmd, io_PTY__CStringList argv, io_PTY__CStringList env))(void)) {
    if (cmd.cwd != null && sys_call_linux_fchdir(fs_Dir_handle(*cmd.cwd)) != 0) {
        sys_call_linux_exit_group(127);
    }
    let_ignore = sys_call_linux_execve(
        as$(const char*)(*S_at((argv.ptrs)[0])),
        as$(char**)(argv.ptrs.ptr),
        as$(char**)(env.ptrs.ptr));
    sys_call_linux_exit_group(127);
}
#endif

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
}

fn_((io_PTY__mapWindowsOpenError(DWORD error))(io_PTY_E)) {
    switch (error) {
    case ERROR_NOT_ENOUGH_MEMORY: $fallthrough;
    case ERROR_OUTOFMEMORY: $fallthrough;
    case ERROR_NO_SYSTEM_RESOURCES: return E_cause$io_PTY_SystemResources();
    default_() return E_cause$io_PTY_OpenFailed() $end(default);
    }
}

fn_((io_PTY__windowsQuoteAppend(io_Writer out, S_const$u8 arg))(E$void) $scope) {
    let needs_quote = expr_(bool $scope)({
        if (arg.len == 0) $break_(true);
        for_(($rt(arg.len))(i)) {
            let ch = *S_at((arg)[i]);
            if (ch == u8_c(' ') || ch == u8_c('\t') || ch == u8_c('"')) $break_(true);
        } $end(for);
        $break_(false);
    }) $unscoped(expr);
    if (!needs_quote) return_(io_Writer_writeBytes(out, arg));

    try_(io_Writer_writeByte(out, u8_c('"')));
    var_(slash_count, usize) = 0;
    for_(($rt(arg.len))(i)) {
        let ch = *S_at((arg)[i]);
        if (ch == u8_c('\\')) {
            slash_count += 1;
        } else if (ch == u8_c('"')) {
            try_(io_Writer_writeByteN(out, u8_c('\\'), slash_count * 2 + 1));
            try_(io_Writer_writeByte(out, ch));
            slash_count = 0;
        } else {
            try_(io_Writer_writeByteN(out, u8_c('\\'), slash_count));
            try_(io_Writer_writeByte(out, ch));
            slash_count = 0;
        }
    } $end(for);
    try_(io_Writer_writeByteN(out, u8_c('\\'), slash_count * 2));
    try_(io_Writer_writeByte(out, u8_c('"')));
    return_ok({});
} $unscoped(fn);

fn_((io_PTY__windowsMakeCmdLine(mem_Alctr gpa, S$S_const$u8 argv))(E$S$u16) $guard) {
    if (argv.len == 0) return_err(E_cause$io_PTY_InvalidCommand());
    var_(cap, usize) = 0;
    for_(($rt(argv.len))(i)) {
        let expanded = orelse_((usize_mulChkd((*S_at((argv)[i])).len, 2))(return_err(E_cause$io_PTY_InvalidCommand())));
        let quoted = orelse_((usize_addChkd(expanded, 2))(return_err(E_cause$io_PTY_InvalidCommand())));
        cap = orelse_((usize_addChkd(cap, quoted))(return_err(E_cause$io_PTY_InvalidCommand())));
        if (i != 0) cap = orelse_((usize_addChkd(cap, 1))(return_err(E_cause$io_PTY_InvalidCommand())));
    } $end(for);
    let utf8_mem = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), cap))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(utf8_mem)));
    var writer = io_Fixed_Writer_init(io_Fixed_writing(utf8_mem));
    for_(($rt(argv.len))(i)) {
        if (i != 0) try_(io_Writer_writeByte(io_Fixed_writer(&writer), u8_c(' ')));
        try_(io_PTY__windowsQuoteAppend(io_Fixed_writer(&writer), *S_at((argv)[i])));
    } $end(for);
    let utf8 = io_Fixed_written(writer.stream).as_const;
    if (utf8.len > as$(usize)(i32_limit_max)) return_err(E_cause$io_PTY_InvalidCommand());
    let need = MultiByteToWideChar(CP_UTF8, 0, as$(LPCCH)(utf8.ptr), as$(int)(utf8.len), null, 0);
    if (need <= 0) return_err(E_cause$io_PTY_InvalidCommand());
    let wide = u_castS$((S$u16)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u16), as$(usize)(need) + 1))));
    if (MultiByteToWideChar(CP_UTF8, 0, as$(LPCCH)(utf8.ptr), as$(int)(utf8.len), as$(LPWSTR)(wide.ptr), need) <= 0) {
        mem_Alctr_free($trace gpa, u_anyS(wide));
        return_err(E_cause$io_PTY_InvalidCommand());
    }
    *S_at((wide)[as$(usize)(need)]) = 0;
    return_ok(wide);
} $unguarded(fn);

fn_((io_PTY__windowsMakeEnvBlock(mem_Alctr gpa, S$S_const$u8 env))(E$S$u16) $guard) {
    if (env.len == 0) return_ok((S$u16){});
    var_(cap, usize) = 1;
    for_(($rt(env.len))(i)) {
        let entry_cap = orelse_((usize_addChkd((*S_at((env)[i])).len, 1))(return_err(E_cause$io_PTY_InvalidCommand())));
        cap = orelse_((usize_addChkd(cap, entry_cap))(return_err(E_cause$io_PTY_InvalidCommand())));
    } $end(for);
    let utf8_mem = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), cap))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(utf8_mem)));
    var writer = io_Fixed_Writer_init(io_Fixed_writing(utf8_mem));
    for_(($rt(env.len))(i)) {
        try_(io_Writer_writeBytes(io_Fixed_writer(&writer), *S_at((env)[i])));
        try_(io_Writer_writeByte(io_Fixed_writer(&writer), 0));
    } $end(for);
    try_(io_Writer_writeByte(io_Fixed_writer(&writer), 0));
    let utf8 = io_Fixed_written(writer.stream).as_const;
    if (utf8.len > as$(usize)(i32_limit_max)) return_err(E_cause$io_PTY_InvalidCommand());
    let need = MultiByteToWideChar(CP_UTF8, 0, as$(LPCCH)(utf8.ptr), as$(int)(utf8.len), null, 0);
    if (need <= 0) return_err(E_cause$io_PTY_InvalidCommand());
    let wide = u_castS$((S$u16)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u16), as$(usize)(need)))));
    if (MultiByteToWideChar(CP_UTF8, 0, as$(LPCCH)(utf8.ptr), as$(int)(utf8.len), as$(LPWSTR)(wide.ptr), need) <= 0) {
        mem_Alctr_free($trace gpa, u_anyS(wide));
        return_err(E_cause$io_PTY_InvalidCommand());
    }
    return_ok(wide);
} $unguarded(fn);

fn_((io_PTY__windowsFreeWide(mem_Alctr gpa, S$u16 text))(void)) {
    if (text.ptr != null) mem_Alctr_free($trace gpa, u_anyS(text));
}

fn_((io_PTY__windowsHpc(const io_PTY* self))(HPCON)) {
    claim_assert_nonnull(self);
    return as$(HPCON)(unwrap_(self->platform_resource_));
}
#endif

/*========== External Definitions ===========================================*/

/*---------- PTY Lifecycle --------------------------------------------------*/

fn_((io_PTY_open(io_PTY_OpenCfg cfg))(io_PTY_E$io_PTY) $guard) {
    if (!io_PTY__validSize(cfg.size)) return_err(E_cause$io_PTY_InvalidSize());
#if plat_is_windows
    if (cfg.nonblocking) return_err(E_cause$io_PTY_Unsupported());
    if (isSome(cfg.slave_mode_)) return_err(E_cause$io_PTY_Unsupported());
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
    if (!CreatePipe(&input_read, &input_write, &security, 0)) return_err(io_PTY__mapWindowsOpenError(GetLastError()));
    errdefer_($ignore, CloseHandle(input_read));
    errdefer_($ignore, CloseHandle(input_write));
    if (!CreatePipe(&output_read, &output_write, &security, 0)) return_err(io_PTY__mapWindowsOpenError(GetLastError()));
    errdefer_($ignore, CloseHandle(output_read));
    errdefer_($ignore, CloseHandle(output_write));

    let coord = (COORD){ .X = as$(SHORT)(cfg.size.cols), .Y = as$(SHORT)(cfg.size.rows) };
    var_(hpc, HPCON) = null;
    if (conpty.create(coord, input_read, output_write, 0, &hpc) != S_OK) return_err(E_cause$io_PTY_OpenFailed());
    CloseHandle(input_read);
    CloseHandle(output_write);
    input_read = null;
    output_write = null;
    return_ok((io_PTY){
        .read_file = fs_File_Handle_promote(output_read, fs_File_Flags_default),
        .write_file_ = some(fs_File_Handle_promote(input_write, fs_File_Flags_default)),
        .slave_file_ = none(),
        .platform_resource_ = some(as$(P$raw)(hpc)),
    });
#elif plat_is_linux
    let flags = sys_call_linux_O_RDWR
              | sys_call_linux_O_NOCTTY
              | (cfg.nonblocking ? sys_call_linux_O_NONBLOCK : 0);
    let master_fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, "/dev/ptmx", flags, 0);
    if (sys_call_linux_syscall_isErr(master_fd)) return_err(E_cause$io_PTY_OpenFailed());
    let master = fs_File_Handle_promote(as$(fs_File_Handle)(master_fd), (fs_File_Flags){ .nonblocking = cfg.nonblocking });
    errdefer_($ignore, fs_File_close(master));

    var_(locked, int) = 0;
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCSPTLCK, &locked) != 0) return_err(E_cause$io_PTY_OpenFailed());
    var_(number, u32) = 0;
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCGPTN, &number) != 0) return_err(E_cause$io_PTY_OpenFailed());
    var_(path, A$$(32, u8)) = A_zero();
    let slave_path = io_PTY__linuxSlavePath(number, A_ref$((S$u8)(path)));
    let slave_fd = sys_call_linux_openat(sys_call_linux_AT_FDCWD, as$(const char*)(slave_path.ptr), sys_call_linux_O_RDWR | sys_call_linux_O_NOCTTY, 0);
    if (sys_call_linux_syscall_isErr(slave_fd)) return_err(E_cause$io_PTY_OpenFailed());
    let slave = fs_File_Handle_promote(as$(fs_File_Handle)(slave_fd), fs_File_Flags_default);
    errdefer_($ignore, fs_File_close(slave));

    var_(size, sys_call_linux_winsize) = { .ws_row = cfg.size.rows, .ws_col = cfg.size.cols };
    if (sys_call_linux_ioctl(master_fd, sys_call_linux_TIOCSWINSZ, &size) != 0) return_err(E_cause$io_PTY_OpenFailed());
    if_some((cfg.slave_mode_)(patch)) {
        let tty = io_TTY_init((io_TTY_Cfg){ .input_file = slave, .output_file = slave });
        try_(io_TTY_applyModePatch(&tty, patch));
    }
    return_ok((io_PTY){
        .read_file = master,
        .write_file_ = none(),
        .slave_file_ = some(slave),
        .platform_resource_ = none(),
    });
#else
    let_ignore = cfg;
    return_err(E_cause$io_PTY_Unsupported());
#endif
} $unguarded(fn);

fn_((io_PTY_close(io_PTY* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if_some((self->platform_resource_)(resource)) {
        var_(conpty, io_PTY__ConPtyApi) = cleared();
        claim_assert(io_PTY__loadConPtyApi(&conpty));
        conpty.close(as$(HPCON)(resource));
    }
#endif
    fs_File_close(self->read_file);
    if_some((self->write_file_)(file)) fs_File_close(file);
    if_some((self->slave_file_)(file)) fs_File_close(file);
    asg_l((self)(cleared()));
}

fn_((io_PTY_reader(io_PTY* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return fs_File_reader(self->read_file);
}

fn_((io_PTY_writer(io_PTY* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return fs_File_writer(orelse_((self->write_file_)(self->read_file)));
}

fn_((io_PTY_resize(io_PTY* self, io_PTY_Size size))(E$void) $scope) {
    claim_assert_nonnull(self);
    if (!io_PTY__validSize(size)) return_err(E_cause$io_PTY_InvalidSize());
#if plat_is_windows
    var_(conpty, io_PTY__ConPtyApi) = cleared();
    if (!io_PTY__loadConPtyApi(&conpty)) return_err(E_cause$io_PTY_Unsupported());
    let coord = (COORD){ .X = as$(SHORT)(size.cols), .Y = as$(SHORT)(size.rows) };
    if (conpty.resize(io_PTY__windowsHpc(self), coord) != S_OK) return_err(E_cause$io_PTY_ResizeFailed());
#elif plat_is_linux
    var_(native, sys_call_linux_winsize) = { .ws_row = size.rows, .ws_col = size.cols };
    if (sys_call_linux_ioctl(fs_File_handle(self->read_file), sys_call_linux_TIOCSWINSZ, &native) != 0) {
        return_err(E_cause$io_PTY_ResizeFailed());
    }
#else
    return_err(E_cause$io_PTY_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

/*---------- PTY Session ----------------------------------------------------*/

fn_((io_PTY_spawn(io_PTY_SpawnCfg cfg))(io_PTY_E$io_PTY_Session) $guard) {
    if (cfg.cmd.argv.len == 0) return_err(E_cause$io_PTY_InvalidCommand());
#if plat_is_windows
    if (cfg.cmd.cwd != null) return_err(E_cause$io_PTY_Unsupported());
    var pty = try_(io_PTY_open(cfg.pty));
    errdefer_($ignore, io_PTY_close(&pty));
    var cmd_line = try_(io_PTY__windowsMakeCmdLine(cfg.gpa, cfg.cmd.argv));
    defer_(io_PTY__windowsFreeWide(cfg.gpa, cmd_line));
    var env = try_(io_PTY__windowsMakeEnvBlock(cfg.gpa, cfg.cmd.env));
    defer_(io_PTY__windowsFreeWide(cfg.gpa, env));

    SIZE_T attr_size = 0;
    InitializeProcThreadAttributeList(null, 1, 0, &attr_size);
    let word_count = (as$(usize)(attr_size) + sizeOf$(usize) - 1) / sizeOf$(usize);
    let attr_mem = u_castS$((S$usize)(try_(mem_Alctr_alloc($trace cfg.gpa, typeInfo$(usize), word_count))));
    defer_(mem_Alctr_free($trace cfg.gpa, u_anyS(attr_mem)));
    let attrs = as$(LPPROC_THREAD_ATTRIBUTE_LIST)(attr_mem.ptr);
    if (!InitializeProcThreadAttributeList(attrs, 1, 0, &attr_size)) return_err(E_cause$io_PTY_SystemResources());
    defer_(DeleteProcThreadAttributeList(attrs));
    let hpc = io_PTY__windowsHpc(&pty);
    if (!UpdateProcThreadAttribute(attrs, 0, usize_(0x00020016), hpc, sizeof(HPCON), null, null)) {
        return_err(E_cause$io_PTY_SystemResources());
    }

    STARTUPINFOEXW startup = cleared();
    startup.StartupInfo.cb = sizeOf$(STARTUPINFOEXW);
    startup.lpAttributeList = attrs;
    PROCESS_INFORMATION process = cleared();
    var_(flags, DWORD) = EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT;
    if (cfg.cmd.create_no_window) flags |= CREATE_NO_WINDOW;
    if (cfg.cmd.start_suspended) flags |= CREATE_SUSPENDED;
    if (!CreateProcessW(null, as$(LPWSTR)(cmd_line.ptr), null, null, FALSE, flags, env.ptr, null, &startup.StartupInfo, &process)) {
        return_err(E_cause$io_PTY_SpawnFailed());
    }
    CloseHandle(process.hThread);
    return_ok((io_PTY_Session){
        .pty = pty,
        .child = {
            .handle = process.hProcess,
            .id = as$(proc_Child_Id)(process.dwProcessId),
            .std_in = { .is_present = false },
            .std_out = { .is_present = false },
            .std_err = { .is_present = false },
        },
    });
#elif plat_is_linux
    if (cfg.cmd.expand_arg0 == proc_ArgExpansion_expand) return_err(E_cause$io_PTY_InvalidCommand());
    var pty = try_(io_PTY_open(cfg.pty));
    errdefer_($ignore, io_PTY_close(&pty));
    var argv = try_(io_PTY__makeCStringList(cfg.gpa, cfg.cmd.argv));
    defer_(io_PTY__freeCStringList(cfg.gpa, &argv));
    var env = try_(io_PTY__makeCStringList(cfg.gpa, cfg.cmd.env));
    defer_(io_PTY__freeCStringList(cfg.gpa, &env));

    let pid = sys_call_linux_fork();
    if (sys_call_linux_syscall_isErr(pid)) return_err(E_cause$io_PTY_SpawnFailed());
    if (pid == 0) {
        let master_fd = as$(sys_call_linux_word)(fs_File_handle(pty.read_file));
        let slave = unwrap_(pty.slave_file_);
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
        io_PTY__linuxExec(cfg.cmd, argv, env);
    }
    fs_File_close(unwrap_(pty.slave_file_));
    asg_l((&pty.slave_file_)(none()));
    return_ok((io_PTY_Session){
        .pty = pty,
        .child = {
            .handle = as$(proc_Child_Handle)(pid),
            .id = as$(proc_Child_Id)(pid),
            .std_in = { .is_present = false },
            .std_out = { .is_present = false },
            .std_err = { .is_present = false },
        },
    });
#else
    let_ignore = cfg;
    return_err(E_cause$io_PTY_Unsupported());
#endif
} $unguarded(fn);

fn_((io_PTY_Session_close(io_PTY_Session* self))(void)) {
    claim_assert_nonnull(self);
    io_PTY_close(&self->pty);
#if plat_is_windows
    if (self->child.handle != null && self->child.handle != INVALID_HANDLE_VALUE) {
        CloseHandle(self->child.handle);
    }
#endif
    asg_l((self)(cleared()));
}

fn_((io_PTY_Session_reader(io_PTY_Session* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return io_PTY_reader(&self->pty);
}

fn_((io_PTY_Session_writer(io_PTY_Session* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return io_PTY_writer(&self->pty);
}

fn_((io_PTY_Session_resize(io_PTY_Session* self, io_PTY_Size size))(E$void) $scope) {
    claim_assert_nonnull(self);
    return_(io_PTY_resize(&self->pty, size));
} $unscoped(fn);

fn_((io_PTY_Session_wait(io_PTY_Session* self))(E$proc_Ter) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    return_(proc_Child_wait(&self->child));
#elif plat_is_linux
    var_(status, int) = 0;
    if (sys_call_linux_wait4(as$(sys_call_linux_word)(self->child.id), &status, 0, null) < 0) {
        return_err(E_cause$proc_SystemResources());
    }
    return_ok(io_PTY__linuxTermFromStatus(status));
#else
    return_err(E_cause$io_PTY_Unsupported());
#endif
} $unscoped(fn);

fn_((io_PTY_Session_kill(io_PTY_Session* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    proc_Child_kill(&self->child);
#elif plat_is_linux
    let_ignore = sys_call_linux_kill(as$(sys_call_linux_word)(self->child.id), 15);
#endif
}
