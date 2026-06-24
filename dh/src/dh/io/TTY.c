#include "dh/io/TTY.h"

#if plat_is_windows
#include "dh/sys/api/windows/console.h"
#include "dh/sys/api/windows/handle.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#endif

/*========== Internal Types and Declarations ================================*/

T_use_E$($set(io_TTY_E)(O$io_TTY_ModeState));

#if plat_is_windows
T_alias$((io_TTY__NativeMode)(DWORD));
#elif plat_is_linux
T_alias$((io_TTY__NativeMode)(sys_call_linux_termios));
#else
T_alias$((io_TTY__NativeMode)(Void));
#endif

claim_assert_static(sizeOf$(io_TTY_ModeState) >= sizeOf$(io_TTY__NativeMode));
claim_assert_static(alignOf$(io_TTY_ModeState) >= alignOf$(io_TTY__NativeMode));

$static fn_((io_TTY__stateFromNative(io_TTY__NativeMode native))(io_TTY_ModeState));
$static fn_((io_TTY__stateAsNative(const io_TTY_ModeState* state))(const io_TTY__NativeMode*));
$static fn_((io_TTY__getMode(fs_File file))(io_TTY_E$O$io_TTY_ModeState));
$static fn_((io_TTY__setMode(fs_File file, const io_TTY_ModeState* state))(E$void));
$static fn_((io_TTY__applyInput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void));
$static fn_((io_TTY__applyOutput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void));

#if plat_is_windows
$static fn_((io_TTY__mapWindowsError(DWORD error))(io_TTY_E));
$static fn_((io_TTY__patchWindowsFlag(DWORD mode, DWORD flag, io_TTY_ModePatch patch, io_TTY_ModeBit bit))(DWORD));
#elif plat_is_linux
$static fn_((io_TTY__patchLinuxFlag(sys_call_linux_tcflag_t* flags, sys_call_linux_tcflag_t flag, io_TTY_ModePatch patch, io_TTY_ModeBit bit))(void));
#endif

/*========== Internal Definitions ===========================================*/

/*---------- Native Mode Storage --------------------------------------------*/

fn_((io_TTY__stateFromNative(io_TTY__NativeMode native))(io_TTY_ModeState)) {
    var_(state, io_TTY_ModeState) = cleared();
    *ptrCast$((io_TTY__NativeMode*)(A_ptr(state.storage))) = native;
    return state;
}

fn_((io_TTY__stateAsNative(const io_TTY_ModeState* state))(const io_TTY__NativeMode*)) {
    claim_assert_nonnull(state);
    return ptrCast$((const io_TTY__NativeMode*)(A_ptr(state->storage)));
}

/*---------- Windows Console Backend ----------------------------------------*/

#if plat_is_windows
fn_((io_TTY__mapWindowsError(DWORD error))(io_TTY_E)) {
    switch (error) {
    case ERROR_INVALID_HANDLE: return E_cause$io_TTY_BadHandle();
    default_() return E_cause$io_TTY_ModeFailed() $end(default);
    }
}

fn_((io_TTY__patchWindowsFlag(DWORD mode, DWORD flag, io_TTY_ModePatch patch, io_TTY_ModeBit bit))(DWORD)) {
    let mask = as$(io_TTY_ModeBits)(bit);
    if ((patch.disable & mask) != 0) return mode & ~flag;
    if ((patch.enable & mask) != 0) return mode | flag;
    return mode;
}

fn_((io_TTY__applyInput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void)) {
    claim_assert_nonnull(mode);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_ECHO_INPUT, patch, io_TTY_ModeBit_echo);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_LINE_INPUT, patch, io_TTY_ModeBit_line_input);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_PROCESSED_INPUT, patch, io_TTY_ModeBit_signal_input);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_VIRTUAL_TERMINAL_INPUT, patch, io_TTY_ModeBit_vt_input);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_WINDOW_INPUT, patch, io_TTY_ModeBit_window_input);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_MOUSE_INPUT, patch, io_TTY_ModeBit_native_mouse);
    if (((patch.enable | patch.disable) & (io_TTY_ModeBit_quick_edit | io_TTY_ModeBit_insert_input)) != 0) {
        *mode |= ENABLE_EXTENDED_FLAGS;
    }
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_QUICK_EDIT_MODE, patch, io_TTY_ModeBit_quick_edit);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_INSERT_MODE, patch, io_TTY_ModeBit_insert_input);
}

fn_((io_TTY__applyOutput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void)) {
    claim_assert_nonnull(mode);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_PROCESSED_OUTPUT, patch, io_TTY_ModeBit_output_process);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_VIRTUAL_TERMINAL_PROCESSING, patch, io_TTY_ModeBit_vt_output);
    *mode = io_TTY__patchWindowsFlag(*mode, ENABLE_WRAP_AT_EOL_OUTPUT, patch, io_TTY_ModeBit_wrap_output);
#if defined(DISABLE_NEWLINE_AUTO_RETURN)
    if ((patch.disable & io_TTY_ModeBit_vt_auto_return) != 0) *mode |= u32_(DISABLE_NEWLINE_AUTO_RETURN);
    if ((patch.enable & io_TTY_ModeBit_vt_auto_return) != 0) *mode &= ~u32_(DISABLE_NEWLINE_AUTO_RETURN);
#endif
}
#endif

/*---------- Linux Terminal Backend -----------------------------------------*/

#if plat_is_linux
fn_((io_TTY__patchLinuxFlag(sys_call_linux_tcflag_t* flags, sys_call_linux_tcflag_t flag, io_TTY_ModePatch patch, io_TTY_ModeBit bit))(void)) {
    claim_assert_nonnull(flags);
    let mask = as$(io_TTY_ModeBits)(bit);
    if ((patch.disable & mask) != 0) {
        *flags &= ~flag;
    } else if ((patch.enable & mask) != 0) {
        *flags |= flag;
    }
}

fn_((io_TTY__applyInput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void)) {
    claim_assert_nonnull(mode);
    io_TTY__patchLinuxFlag(&mode->c_lflag, sys_call_linux_ECHO, patch, io_TTY_ModeBit_echo);
    io_TTY__patchLinuxFlag(&mode->c_lflag, sys_call_linux_ICANON, patch, io_TTY_ModeBit_line_input);
    io_TTY__patchLinuxFlag(&mode->c_lflag, sys_call_linux_ISIG, patch, io_TTY_ModeBit_signal_input);
    io_TTY__patchLinuxFlag(&mode->c_lflag, sys_call_linux_IEXTEN, patch, io_TTY_ModeBit_extended_input);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_ICRNL, patch, io_TTY_ModeBit_crlf_input);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_IXON, patch, io_TTY_ModeBit_flow_input);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_IXOFF, patch, io_TTY_ModeBit_flow_input);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_BRKINT, patch, io_TTY_ModeBit_break_input);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_INPCK, patch, io_TTY_ModeBit_parity_check);
    io_TTY__patchLinuxFlag(&mode->c_iflag, sys_call_linux_ISTRIP, patch, io_TTY_ModeBit_strip_input);
    if (patch.set_min_time) {
        *A_at((mode->c_cc)[sys_call_linux_VMIN]) = patch.min_read;
        *A_at((mode->c_cc)[sys_call_linux_VTIME]) = patch.timeout_ds;
    }
}

fn_((io_TTY__applyOutput(io_TTY__NativeMode* mode, io_TTY_ModePatch patch))(void)) {
    claim_assert_nonnull(mode);
    io_TTY__patchLinuxFlag(&mode->c_oflag, sys_call_linux_OPOST, patch, io_TTY_ModeBit_output_process);
}
#endif

/*---------- Native Mode Operations -----------------------------------------*/

fn_((io_TTY__getMode(fs_File file))(io_TTY_E$O$io_TTY_ModeState) $scope) {
    if (!try_(fs_File_isTTY(file))) return_ok(none());
#if plat_is_windows
    var_(native, io_TTY__NativeMode) = 0;
    if (!GetConsoleMode(fs_File_handle(file), &native)) return_err(io_TTY__mapWindowsError(GetLastError()));
    return_ok(some(io_TTY__stateFromNative(native)));
#elif plat_is_linux
    var_(native, io_TTY__NativeMode) = cleared();
    if (sys_call_linux_ioctl(fs_File_handle(file), sys_call_linux_TCGETS, &native) != 0) {
        return_err(E_cause$io_TTY_BadHandle());
    }
    return_ok(some(io_TTY__stateFromNative(native)));
#else
    let_ignore = file;
    return_err(E_cause$io_TTY_Unsupported());
#endif
} $unscoped(fn);

fn_((io_TTY__setMode(fs_File file, const io_TTY_ModeState* state))(E$void) $scope) {
    claim_assert_nonnull(state);
#if plat_is_windows
    if (!SetConsoleMode(fs_File_handle(file), *io_TTY__stateAsNative(state))) {
        return_err(io_TTY__mapWindowsError(GetLastError()));
    }
#elif plat_is_linux
    if (sys_call_linux_ioctl(
            fs_File_handle(file),
            sys_call_linux_TCSETS,
            ptrQualCast$((io_TTY__NativeMode*)(io_TTY__stateAsNative(state)))
        )
        != 0) {
        return_err(E_cause$io_TTY_ModeFailed());
    }
#else
    let_ignore = file;
    return_err(E_cause$io_TTY_Unsupported());
#endif
    return_ok({});
} $unscoped(fn);

/*========== External Definitions ===========================================*/

/*---------- Lifecycle ------------------------------------------------------*/

fn_((io_TTY_init(io_TTY_Cfg cfg))(io_TTY)) {
    return (io_TTY){
        .input_file = cfg.input_file,
        .output_file = cfg.output_file,
        .restore_ = none(),
    };
}

fn_((io_TTY_fini(io_TTY* self))(void)) {
    claim_assert_nonnull(self);
    io_TTY_leaveMode(self);
    asg_l((self)(cleared()));
}

/*---------- Reader and Writer Views ----------------------------------------*/

fn_((io_TTY_reader(const io_TTY* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return fs_File_reader(self->input_file);
}

fn_((io_TTY_writer(const io_TTY* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return fs_File_writer(self->output_file);
}

/*---------- Mode Control ---------------------------------------------------*/

fn_((io_TTY_snapshot(const io_TTY* self))(io_TTY_E$io_TTY_ModeSnapshot) $scope) {
    claim_assert_nonnull(self);
    return_ok((io_TTY_ModeSnapshot){
        .input = try_(io_TTY__getMode(self->input_file)),
        .output = try_(io_TTY__getMode(self->output_file)),
    });
} $unscoped(fn);

fn_((io_TTY_restore(const io_TTY* self, io_TTY_ModeSnapshot snapshot))(E$void) $scope) {
    claim_assert_nonnull(self);
    if_some((snapshot.input)(state)) try_(io_TTY__setMode(self->input_file, &state));
    if_some((snapshot.output)(state)) try_(io_TTY__setMode(self->output_file, &state));
    return_ok({});
} $unscoped(fn);

fn_((io_TTY_applyModePatch(const io_TTY* self, io_TTY_ModePatch patch))(E$void) $scope) {
    claim_assert_nonnull(self);
    let input_state = try_(io_TTY__getMode(self->input_file));
    let input = orelse_((input_state)(return_err(E_cause$io_TTY_NotTTY())));
    var input_native = *io_TTY__stateAsNative(&input);
    io_TTY__applyInput(&input_native, patch);
    let patched_input = io_TTY__stateFromNative(input_native);
    try_(io_TTY__setMode(self->input_file, &patched_input));

    let output_state = try_(io_TTY__getMode(self->output_file));
    if_some((output_state)(output)) {
        var output_native = *io_TTY__stateAsNative(&output);
        io_TTY__applyOutput(&output_native, patch);
        let patched_output = io_TTY__stateFromNative(output_native);
        try_(io_TTY__setMode(self->output_file, &patched_output));
    }
    return_ok({});
} $unscoped(fn);

fn_((io_TTY_enterMode(io_TTY* self, io_TTY_ModePatch patch))(E$void) $guard) {
    claim_assert_nonnull(self);
    claim_assert(isNone(self->restore_));
    let snapshot = try_(io_TTY_snapshot(self));
    errdefer_($ignore, catch_((io_TTY_restore(self, snapshot))($ignore, $do_nothing)));
    try_(io_TTY_applyModePatch(self, patch));
    asg_l((&self->restore_)(some((io_TTY_Restore){ .snapshot = snapshot })));
    return_ok({});
} $unguarded(fn);

fn_((io_TTY_leaveMode(io_TTY* self))(void)) {
    claim_assert_nonnull(self);
    let restore = orelse_((self->restore_)(return));
    catch_((io_TTY_restore(self, restore.snapshot))($ignore, $do_nothing));
    asg_l((&self->restore_)(none()));
}

fn_((io_TTY_isInEnteredMode(const io_TTY* self))(bool)) {
    claim_assert_nonnull(self);
    return isSome(self->restore_);
}

/*---------- Queries --------------------------------------------------------*/

fn_((io_TTY_inputIsTTY(const io_TTY* self))(E$bool) $scope) {
    claim_assert_nonnull(self);
    return_(fs_File_isTTY(self->input_file));
} $unscoped(fn);

fn_((io_TTY_outputIsTTY(const io_TTY* self))(E$bool) $scope) {
    claim_assert_nonnull(self);
    return_(fs_File_isTTY(self->output_file));
} $unscoped(fn);

fn_((io_TTY_queryScreenCells(const io_TTY* self))(io_TTY_E$io_TTY_CellSize) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    var_(info, CONSOLE_SCREEN_BUFFER_INFO) = cleared();
    if (!GetConsoleScreenBufferInfo(fs_File_handle(self->output_file), &info)) {
        return_err(E_cause$io_TTY_QueryFailed());
    }
    return_ok((io_TTY_CellSize){
        .cols = as$(u16)(info.srWindow.Right - info.srWindow.Left + 1),
        .rows = as$(u16)(info.srWindow.Bottom - info.srWindow.Top + 1),
    });
#elif plat_is_linux
    var_(size, sys_call_linux_winsize) = cleared();
    if (sys_call_linux_ioctl(fs_File_handle(self->output_file), sys_call_linux_TIOCGWINSZ, &size) != 0) {
        return_err(E_cause$io_TTY_QueryFailed());
    }
    return_ok((io_TTY_CellSize){ .cols = size.ws_col, .rows = size.ws_row });
#else
    return_err(E_cause$io_TTY_Unsupported());
#endif
} $unscoped(fn);

fn_((io_TTY_queryCursorPosNative(const io_TTY* self))(io_TTY_E$io_TTY_Pos) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    var_(info, CONSOLE_SCREEN_BUFFER_INFO) = cleared();
    if (!GetConsoleScreenBufferInfo(fs_File_handle(self->output_file), &info)) {
        return_err(E_cause$io_TTY_QueryFailed());
    }
    return_ok((io_TTY_Pos){
        .x = as$(u16)(info.dwCursorPosition.X),
        .y = as$(u16)(info.dwCursorPosition.Y),
    });
#else
    return_err(E_cause$io_TTY_Unsupported());
#endif
} $unscoped(fn);

fn_((io_TTY_inputReady(const io_TTY* self))(E$bool) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    var_(count, DWORD) = 0;
    if (!GetNumberOfConsoleInputEvents(fs_File_handle(self->input_file), &count)) {
        return_err(E_cause$io_TTY_QueryFailed());
    }
    return_ok(count != 0);
#elif plat_is_linux
    var_(count, int) = 0;
    if (sys_call_linux_ioctl(fs_File_handle(self->input_file), sys_call_linux_FIONREAD, &count) != 0) {
        return_err(E_cause$io_TTY_QueryFailed());
    }
    return_ok(count != 0);
#else
    return_err(E_cause$io_TTY_Unsupported());
#endif
} $unscoped(fn);
