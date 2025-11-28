/* vt100_stub_revised.c
 *
 * Stub process that runs inside Windows Terminal with DIRECT WT window access.
 *
 * Key insight: Engine passes WT HWND to stub, enabling:
 *   - Direct window metrics from WT window (not console APIs)
 *   - Direct mouse input via window hooks (not console input buffer)
 *   - Direct keyboard state via WT focus tracking
 *   - UI Automation for accurate title bar height
 *
 * Eliminated console API indirection:
 *   - No PeekConsoleInput / ReadConsoleInputW
 *   - No console input mode configuration
 *   - No console-to-logical coordinate guessing
 *   - No GetConsoleWindow() confusion (returns conhost, not WT)
 *
 * Data flow:
 *   Engine → SharedMemory → Stub: WT HWND (once), framebuffer pixels (per frame)
 *   Stub → SharedMemory → Engine: input events, window metrics, window state
 */

#include "VT100Ex.h"
#if !defined(engine_core_VT100Ex_comp_sharded)
#include "dh/main.h"

#if plat_is_windows
#include "dh/os/windows.h"
#if BUILTIN_COMP_MSVC
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "uuid.lib")
#endif /* BUILTIN_COMP_MSVC */

#if !engine_impl_connected_stub
#define engine_impl_connected_stub 1
#undef auto
#undef var
#undef let

#define INITGUID
#include <initguid.h> // <---- required for MinGW/Clang
#define COBJMACROS
#include <uiautomation.h>
#include <oleauto.h>
#include <tlhelp32.h>

#if BUILTIN_COMP_MSVC
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#endif /* BUILTIN_COMP_MSVC */

#define auto __comp_syn__auto
#define var __comp_syn__var
#define let __comp_syn__let
#endif /* !engine_impl_connected_stub */

#include "dh/heap/Page.h"
#include "dh/io/common.h"
#include "dh/log.h"
#include "dh/time.h"
#include "dh/math/vec.h"

/* ========== Instance State (no non-const globals) =========================== */

typedef struct Inst {
    /* Shared memory */
    var_(shared_mapping, HANDLE);
    var_(shared_state, engine_stub_VT100Ex_SharedState*);

    /* Console output (still needed for WriteConsoleA) */
    var_(console_out, fs_File);
    var_(ansi_buf, S$u8);

    /* Direct WT window access */
    var_(wt_hwnd, HWND); /* Windows Terminal window handle */
    var_(wt_pid, DWORD); /* WT process ID for hooks */
    var_(wt_tid, DWORD); /* WT main thread ID for hooks */

    /* Hooks installed by stub */
    var_(mouse_hook, HHOOK);            /* Low-level mouse hook */
    var_(keyboard_hook, HHOOK);         /* Low-level keyboard hook */
    var_(winevent_hook, HWINEVENTHOOK); /* Window event hook */

    /* UI Automation */
    var_(uia, IUIAutomation*);
    var_(uia_condition, IUIAutomationCondition*);

    /* Cached state */
    var_(last_rendered_frame, LONG);
    var_(cached_titlebar_height, i32);
    var_(last_mouse_pos, m_V2i32);

    /* For passing self to hook callbacks */
    var_(self_ptr, struct Inst*); /* Set to &inst after init */
} Inst;
T_use$((Inst)(E));
/* Thread-local storage for hook callbacks (unavoidable for Windows hooks) */
$static $Thrd_local Inst* tls_inst = null;

/* ========== Forward Declarations ============================================= */

$must_check
$static fn_((Inst_init(mem_Allocator gpa))(E$Inst));
$static fn_((Inst_fini(Inst* self, mem_Allocator gpa))(void));
typedef fn_(((*)(Inst * self))(bool) $T) Inst_WaitForPredicate;
$static fn_((Inst_waitFor(Inst* self, Inst_WaitForPredicate predicate))(void));

$static fn_((Inst_installTLSForHook(Inst* self))(void));
$static fn_((Inst_uninstallTLSForHook(Inst* self))(void));

$must_check
$static fn_((Inst_installHooks(Inst* self))(E$void));
$static fn_((Inst_uninstallHooks(Inst* self))(void));
$static fn_((Inst_preUpdateMetrics(Inst* self))(void));
$static fn_((Inst_updateMetrics(Inst* self))(void));

$must_check
$static fn_((Inst_run(Inst* self))(E$void));
$static fn_((Inst_renderFrame(const Inst* self, const engine_stub_VT100Ex_FrameBuffer* frame))(void));
$static fn_((Inst_pushInputEvent(Inst* self, engine_stub_VT100Ex_InputEvent event))(void));
$static fn_((Inst_pushWindowEvent(Inst* self, engine_stub_VT100Ex_WindowEvent event))(void));

/* ========== Main Entry ======================================================= */

$static fn_((engineWritesWTHandleToInit(Inst* self))(bool)) {
    /* Engine writes WT HWND to shared memory init field */
    if (InterlockedCompareExchange(&self->shared_state->init.ready, 0, 0)) {
        /* Safely read the HWND */
        self->wt_hwnd = (HWND)InterlockedCompareExchangePointer(
            (volatile PVOID*)&self->shared_state->init.wt_hwnd,
            null,
            null
        );
        return self->wt_hwnd != null && !self->shared_state->control.should_exit;
    }
    return false;
};

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var page = lit$((heap_Page){});
    let gpa = heap_Page_allocator(&page);
    var inst = try_(Inst_init(gpa));
    defer_(Inst_fini(&inst, gpa));
    Inst_installTLSForHook(&inst);
    defer_(Inst_uninstallTLSForHook(&inst));
    Inst_waitFor(&inst, engineWritesWTHandleToInit);
    log_info("Stub received WT HWND: %p", (void*)inst.wt_hwnd);
    try_(Inst_installHooks(&inst));
    defer_(Inst_uninstallHooks(&inst));
    Inst_preUpdateMetrics(&inst);
    Inst_updateMetrics(&inst);
    return_ok(try_(Inst_run(&inst)));
} $unguarded_(fn);

/* ========== Initialization =================================================== */

errset_((ConfigConsoleOutputErr)(
    FailedSetWindowInfo,
    FailedSetScreenBufferSize,
    FailedAssignScreenBuffer,
    FailedSetCodePage,
    FailedGetMode,
    FailedSetMode
));
/// Function to initialize console-specific configurations like screen buffer size
$must_check
fn_((configureConsoleOutput(fs_File console_out, m_V2u32 rect))(E$fs_File) $scope) {
    let handle = console_out.handle;
    // Initialize console window size to minimum, so ScreenBuffer can shrink
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, 1, 1 })) {
        log_error("Failed to set console window info: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedSetWindowInfo());
    }

    // Configure screen buffer size for the console
    if (!SetConsoleScreenBufferSize(handle, (COORD){ as$(SHORT)(rect.x), as$(SHORT)(rect.y) })) {
        log_error("Failed to set console screen buffer size: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedSetScreenBufferSize());
    }
    // Assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(handle)) {
        log_error("Failed to assign screen buffer to the console: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedAssignScreenBuffer());
    }
    // Set physical console window size
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, as$(SHORT)(rect.x - 1), as$(SHORT)(rect.y - 1) })) {
        log_error("Failed to set console window info: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedSetWindowInfo());
    }

    // Configure console output code page to UTF-8
    if (!SetConsoleOutputCP(CP_UTF8)) {
        log_error("Failed to set console output code page to UTF-8: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedSetCodePage());
    }

    // Set console output mode for processing terminal sequences
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD out_mode = 0, !GetConsoleMode(handle, &out_mode)) {
        log_error("Failed to get console output mode: %d", GetLastError());
        return_err(ConfigConsoleOutputErr_FailedSetMode());
    } else {
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        if (!SetConsoleMode(handle, out_mode)) {
            log_error("Failed to set console output mode: %d", GetLastError());
            return_err(ConfigConsoleOutputErr_FailedSetMode());
        }
    }
    return_ok(console_out);
} $unscoped_(fn);

errset_((ConfigConsoleCursorErr)(
    FailedHide,
    FailedShow,
    FailedResetPos
));
$must_check
fn_((hideConsoleCursor(fs_File console_out))(E$fs_File) $scope) {
    let handle = console_out.handle;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, false })) {
        log_error("Failed to hide console cursor: %d", GetLastError());
        return_err(ConfigConsoleCursorErr_FailedHide());
    }
    return_ok(console_out);
} $unscoped_(fn);
$must_check
fn_((showConsoleCursor(Inst* self))(E$void) $scope) {
    let handle = self->console_out.handle;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, true })) {
        log_error("Failed to show console cursor: %d", GetLastError());
        return_err(ConfigConsoleCursorErr_FailedShow());
    }
    return_ok({});
} $unscoped_(fn);
$must_check
fn_((resetConsoleCursorPos(fs_File console_out))(E$fs_File) $scope) {
    let handle = console_out.handle;
    let command = u8_l("\033[H");
    if_(DWORD written = 0,
        !WriteConsoleA(
            handle,
            command.ptr,
            command.len,
            &written,
            null
        ) || written != command.len) {
        log_error("Failed to reset console cursor position: %d", GetLastError());
        return_err(ConfigConsoleCursorErr_FailedResetPos());
    };
    return_ok(console_out);
} $unscoped_(fn);

errset_((ConfigConsoleInputErr)(
    FailedGetMode,
    FailedSetMode
));
/// Function to configure console input mode
$must_check
fn_((configureConsoleInput(fs_File console_in))(E$fs_File) $scope) {
    let handle = console_in.handle;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console input mode: %d", GetLastError());
        return_err(ConfigConsoleInputErr_FailedGetMode());
    } else {
        in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console input mode: %d", GetLastError());
            return_err(ConfigConsoleInputErr_FailedSetMode());
        }
    }
    return_ok(console_in);
} $unscoped_(fn);

errset_((ConfigConsoleMouseErr)(
    FailedEnableGetMode,
    FailedEnableSetMode,
    FailedDisableGetMode,
    FailedDisableSetMode
));
/// Enable mouse input through the console.
fn_((enableConsoleMouse(fs_File console_in))(E$fs_File) $scope) {
    let handle = console_in.handle;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console mode: %d", GetLastError());
        return_err(ConfigConsoleMouseErr_FailedEnableGetMode());
    } else {
        in_mode |= ENABLE_MOUSE_INPUT;
        in_mode &= ~ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console mode: %d", GetLastError());
            return_err(ConfigConsoleMouseErr_FailedEnableSetMode());
        }
    }
    return_ok(console_in);
} $unscoped_(fn);
/// Disable mouse input in the console.
fn_((disableConsoleMouse(fs_File console_in))(E$fs_File) $scope) {
    let handle = console_in.handle;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console mode: %d", GetLastError());
        return_err(ConfigConsoleMouseErr_FailedDisableGetMode());
    } else {
        in_mode &= ~ENABLE_MOUSE_INPUT;
        in_mode |= ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console mode: %d", GetLastError());
            return_err(ConfigConsoleMouseErr_FailedDisableSetMode());
        }
    }
    return_ok(console_in);
} $unscoped_(fn);

fn_((Inst_init(mem_Allocator gpa))(E$Inst) $guard) {
    /* Open shared memory */
    let shared_mapping = expr_(HANDLE $scope)({
        let mapping = OpenFileMappingW(
            FILE_MAP_ALL_ACCESS,
            false,
            engine_stub_VT100Ex_shared_mem_name
        );
        if (!mapping) {
            log_error("Failed to open shared memory: %lu", GetLastError());
            return_err(Err_Unspecified());
        }
        $break_(mapping);
    }) $unscoped_(expr);
    errdefer_($ignore, CloseHandle(shared_mapping));

    /* Map shared state */
    typedef engine_stub_VT100Ex_SharedState SharedState;
    let shared_state = expr_(SharedState * $scope)({
        let state = as$(SharedState*)(MapViewOfFile(
            shared_mapping,
            FILE_MAP_ALL_ACCESS,
            0, 0,
            sizeOf$(engine_stub_VT100Ex_SharedState)
        ));
        if (!state) {
            log_error("Failed to map shared memory: %lu", GetLastError());
            return_err(Err_Unspecified());
        }
        $break_(state);
    }) $unscoped_(expr);
    errdefer_($ignore, UnmapViewOfFile(shared_state));

    /* Register stub PID */
    shared_state->control.stub_pid = GetCurrentProcessId();

    /* Allocate ANSI buffer */
    let ansi_buf_len = engine_stub_VT100Ex_max_width * (engine_stub_VT100Ex_max_height / 2) * 30 + 1024;
    let ansi_buf = u_castS$((S$u8)(catch_((mem_Allocator_alloc(gpa, typeInfo$(u8), ansi_buf_len))(err, {
        log_error("Failed to allocate ANSI buffer");
        return_err(err);
    }))));
    errdefer_($ignore, mem_Allocator_free(gpa, u_anyS(ansi_buf)));

    /* Configure console output (still need this for WriteConsoleA) */
    let console_out = io_getStdOut();
    try_(configureConsoleOutput(console_out, (m_V2u32){
                                                 .x = shared_state->metrics.grid_width,
                                                 .y = shared_state->metrics.grid_height / 2,
                                             }));
    try_(hideConsoleCursor(console_out));

    /* Configure console input */
    let console_in = io_getStdIn();
    try_(configureConsoleInput(console_in));
    try_(enableConsoleMouse(console_in));

    /* Initialize UI Automation for title bar detection */
    IUIAutomation* uia = null;
    IUIAutomationCondition* uia_condition = null;
    if (SUCCEEDED(CoInitializeEx(null, COINIT_MULTITHREADED))) {
        if (SUCCEEDED(CoCreateInstance(
                &CLSID_CUIAutomation,
                null,
                CLSCTX_INPROC_SERVER,
                &IID_IUIAutomation,
                (void**)&uia
            ))) {
            /* Create condition for finding title bar */
            VARIANT vari;
            vari.vt = VT_I4;
            vari.lVal = UIA_TitleBarControlTypeId;
            uia->lpVtbl->CreatePropertyCondition(
                uia,
                UIA_ControlTypePropertyId,
                vari,
                &uia_condition
            );
        }
    }

    return_ok({
        .shared_mapping = shared_mapping,
        .shared_state = shared_state,
        .console_out = console_out,
        .ansi_buf = ansi_buf,
        .wt_hwnd = null, /* Will be set by engine */
        .wt_pid = 0,
        .wt_tid = 0,
        .mouse_hook = null,
        .keyboard_hook = null,
        .winevent_hook = null,
        .uia = uia,
        .uia_condition = uia_condition,
        .last_rendered_frame = 0,
        .cached_titlebar_height = 0,
        .last_mouse_pos = m_V2i32_zero,
        .self_ptr = null,
    });
} $unguarded_(fn);

fn_((Inst_fini(Inst* self, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    self->uia_condition = (claim_assert_nonnull(self->uia_condition), self->uia_condition->lpVtbl->Release(self->uia_condition), null);
    self->uia = (claim_assert_nonnull(self->uia), self->uia->lpVtbl->Release(self->uia), null);
    CoUninitialize();
    mem_Allocator_free(gpa, u_anyS(self->ansi_buf));
    self->shared_state = (claim_assert_nonnull(self->shared_state), UnmapViewOfFile(self->shared_state), null);
    self->shared_mapping = (claim_assert_nonnull(self->shared_mapping), CloseHandle(self->shared_mapping), null);
};

fn_((Inst_waitFor(Inst* self, Inst_WaitForPredicate predicate))(void)) {
    while (!predicate(self)) {
        time_sleep(time_Duration_fromMillis(10));
    }
};

/* ========== Hook Management ================================================== */

fn_((Inst_installTLSForHook(Inst* self))(void)) {
    tls_inst = self;
    self->self_ptr = self;
};

fn_((Inst_uninstallTLSForHook(Inst* self))(void)) {
    tls_inst = null;
    self->self_ptr = null;
};

$static CALLBACK fn_((keyboardEventProc(i32 nCode, WPARAM wParam, LPARAM lParam))(LRESULT));
$static CALLBACK fn_((mouseEventProc(i32 nCode, WPARAM wParam, LPARAM lParam))(LRESULT));
$static CALLBACK fn_((winEventProc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD dwEventThread,
    DWORD dwmsEventTime
))(void));
fn_((Inst_installHooks(Inst* self))(E$void) $guard) {
    if (!self->wt_hwnd) {
        log_error("WT HWND is not set");
        return_err(Err_Unspecified());
    }
    /* Get WT process/thread info */
    self->wt_tid = GetWindowThreadProcessId(self->wt_hwnd, &self->wt_pid);

    /* Install low-level mouse hook (global, but we filter for WT) */
    self->mouse_hook = SetWindowsHookExW(WH_MOUSE_LL, mouseEventProc, null, 0 /* Global hook */);
    if (!self->mouse_hook) {
        log_error("Failed to install mouse hook: %lu", GetLastError());
        return_err(Err_Unspecified());
    }
    errdefer_($ignore, UnhookWindowsHookEx(self->mouse_hook));
    /* Install low-level keyboard hook */
    self->keyboard_hook = SetWindowsHookExW(WH_KEYBOARD_LL, keyboardEventProc, null, 0 /* Global hook */);
    if (!self->keyboard_hook) {
        log_error("Failed to install keyboard hook: %lu", GetLastError());
        return_err(Err_Unspecified());
    }
    errdefer_($ignore, UnhookWindowsHookEx(self->keyboard_hook));
    /* Install window event hook for WT window */
    self->winevent_hook = SetWinEventHook(
        EVENT_MIN, EVENT_MAX,
        null,
        winEventProc,
        self->wt_pid,
        0, /* All threads in WT process */
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );
    if (!self->winevent_hook) {
        log_error("Failed to install window event hook: %lu", GetLastError());
        return_err(Err_Unspecified());
    }
    errdefer_($ignore, UnhookWinEvent(self->winevent_hook));

    return_ok({});
} $unguarded_(fn);

fn_((Inst_uninstallHooks(Inst* self))(void)) {
    claim_assert_nonnull(self);
    self->winevent_hook = (claim_assert_nonnull(self->winevent_hook), UnhookWinEvent(self->winevent_hook), null);
    self->keyboard_hook = (claim_assert_nonnull(self->keyboard_hook), UnhookWindowsHookEx(self->keyboard_hook), null);
    self->mouse_hook = (claim_assert_nonnull(self->mouse_hook), UnhookWindowsHookEx(self->mouse_hook), null);
};

/* ========== Metrics Detection ================================================ */

$static fn_((detectTitlebarHeight(Inst* self))(i32));
fn_((Inst_preUpdateMetrics(Inst* self))(void)) {
    claim_assert_nonnull(self);
    self->cached_titlebar_height = detectTitlebarHeight(self);
};
fn_((detectTitlebarHeight(Inst* self))(i32)) {
    claim_assert_nonnull(self);
    if (!self->uia || !self->wt_hwnd) {
        return 32; /* Reasonable default */
    }
    IUIAutomationElement* root = null;
    if (FAILED(self->uia->lpVtbl->ElementFromHandle(self->uia, self->wt_hwnd, &root)) || !root) {
        return 32;
    }
    i32 height = 32;
    if (self->uia_condition) {
        IUIAutomationElement* titlebar = null;
        if (SUCCEEDED(root->lpVtbl->FindFirst(root, TreeScope_Children, self->uia_condition, &titlebar)) && titlebar) {
            RECT rect;
            if (SUCCEEDED(titlebar->lpVtbl->get_CurrentBoundingRectangle(titlebar, &rect))) {
                height = rect.bottom - rect.top;
            }
            titlebar->lpVtbl->Release(titlebar);
        }
    }
    root->lpVtbl->Release(root);
    return height;
}

fn_((Inst_updateMetrics(Inst* self))(void)) {
    if (!self->wt_hwnd) {
        return;
    }
    RECT window_rect;
    if (!GetWindowRect(self->wt_hwnd, &window_rect)) {
        return;
    }
    /* Get client area */
    RECT client_rect;
    if (!GetClientRect(self->wt_hwnd, &client_rect)) {
        return;
    }
    // /* Update title bar height (expensive, cache it) */
    // self->cached_titlebar_height = Inst_detectTitlebarHeight(self);
    /* Calculate padding (borders) */
    POINT client_origin = { 0, 0 };
    ClientToScreen(self->wt_hwnd, &client_origin);
    i32 padding_left = client_origin.x - window_rect.left;
    i32 padding_top = client_origin.y - window_rect.top;
    i32 padding_right = window_rect.right - (client_origin.x + client_rect.right);
    i32 padding_bottom = window_rect.bottom - (client_origin.y + client_rect.bottom);
    /* Update shared state atomically */
    self->shared_state->metrics.window_pos_x = window_rect.left;
    self->shared_state->metrics.window_pos_y = window_rect.top;
    self->shared_state->metrics.titlebar_height = self->cached_titlebar_height;
    self->shared_state->metrics.padding_left = padding_left;
    self->shared_state->metrics.padding_top = padding_top;
    self->shared_state->metrics.padding_right = padding_right;
    self->shared_state->metrics.padding_bottom = padding_bottom;
    /* Grid dimensions come from console buffer size
     * (engine should set these based on SetConsoleScreenBufferSize) */
    InterlockedIncrement(&self->shared_state->metrics.version);
}

/* ========== Main Loop ======================================================== */

fn_((Inst_run(Inst* self))(E$void) $scope) {
    while (!self->shared_state->control.should_exit) {
        /* Process Windows messages (required for hooks) */
        while_(MSG msg = {}, PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                InterlockedExchange(&self->shared_state->control.should_exit, 1);
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        /* Render any new frames */
        for (usize buf_idx = 0; buf_idx < 2; ++buf_idx) {
            if (InterlockedCompareExchange(&self->shared_state->frame.ready[buf_idx], 0, 0)) {
                let frame = &self->shared_state->frame.bufs[buf_idx];
                if ((LONG)frame->frame_number > self->last_rendered_frame) {
                    Inst_renderFrame(self, frame);
                    self->last_rendered_frame = (LONG)frame->frame_number;
                    InterlockedIncrement(&self->shared_state->stats.frames_rendered);
                }
                InterlockedExchange(&self->shared_state->frame.ready[buf_idx], 0);
            }
        }
        /* Update stub heartbeat */
        InterlockedIncrement(&self->shared_state->control.stub_alive);
        /* Yield CPU */
        time_sleep(time_Duration_fromMillis(1));
    }
    return_ok({});
} $unscoped_(fn);

/* ========== Rendering (same as before) ====================================== */

$static fn_((formatU8(S$u8 buf, u8 val))(usize));
$static fn_((writeSeqCursorToHome(S$u8 writable_buf))(usize));
$static fn_((writeSeqColor(S$u8 writable_buf, engine_stub_VT100Ex_Color upper, engine_stub_VT100Ex_Color lower))(usize));
$static fn_((writeHalfBlock(S$u8 writable_buf))(usize));
$static fn_((writeNL(S$u8 writable_buf))(usize));
$static fn_((flushBuf(fs_File console_out, S$u8 written_buf))(void));
fn_((Inst_renderFrame(const Inst* self, const engine_stub_VT100Ex_FrameBuffer* frame))(void)) {
    u32 width = frame->width;
    u32 height = frame->height;
    let pixels = frame->pixels;

    let console_out = self->console_out;
    let writable_buf = self->ansi_buf;
    var unwritten_buf = writable_buf;
    unwritten_buf = suffixS(unwritten_buf, writeSeqCursorToHome(unwritten_buf));
    for (u32 y = 0; y + 1 < height; y += 2) {
        for (u32 x = 0; x < width;) {
            let upper = pixels[y * width + x];
            let lower = pixels[(y + 1) * width + x];

            u32 run_length = 1;
            while (x + run_length < width) {
                let next_upper = pixels[y * width + x + run_length];
                let next_lower = pixels[(y + 1) * width + x + run_length];
                if (memcmp(&upper, &next_upper, sizeOf$(engine_stub_VT100Ex_Color)) != 0
                    || memcmp(&lower, &next_lower, sizeOf$(engine_stub_VT100Ex_Color)) != 0) {
                    break;
                }
                run_length++;
            }

            unwritten_buf = suffixS(unwritten_buf, writeSeqColor(unwritten_buf, upper, lower));
            for_(($r(0, run_length))($ignore) {
                unwritten_buf = suffixS(unwritten_buf, writeHalfBlock(unwritten_buf));
            });
            x += run_length;
        }
        unwritten_buf = suffixS(unwritten_buf, writeNL(unwritten_buf));
    }

    let written_buf = prefixS(writable_buf, writable_buf.len - unwritten_buf.len);
    flushBuf(console_out, written_buf);
}

/* String building helpers (unchanged from your version) */
$static let digit_pairs = u8_a(
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
);
$inline_always
$static fn_((formatU8(S$u8 buf, u8 val))(usize)) {
    if (val >= 100) {
        u8 hi = val / 100;
        u8 lo = val % 100;
        *S_at((buf)[0]) = '0' + hi;
        *S_at((buf)[1]) = *A_at((digit_pairs)[lo * 2]);
        *S_at((buf)[2]) = *A_at((digit_pairs)[lo * 2 + 1]);
        return 3;
    }
    if (val >= 10) {
        *S_at((buf)[0]) = *A_at((digit_pairs)[val * 2]);
        *S_at((buf)[1]) = *A_at((digit_pairs)[val * 2 + 1]);
        return 2;
    }
    *S_at((buf)[0]) = '0' + val;
    return 1;
}

$inline_always
$static fn_((writeSeqCursorToHome(S$u8 writable_buf))(usize)) {
    $static let cursor_to_home = u8_l("\033[H");
    mem_copyBytes(writable_buf, cursor_to_home);
    return cursor_to_home.len;
}

$inline_always
$static fn_((writeSeqColor(S$u8 writable_buf, engine_stub_VT100Ex_Color upper, engine_stub_VT100Ex_Color lower))(usize)) {
    var unwritten = writable_buf;

    $static let cmd_seq_begin = u8_l("\033[");
    $static let cmd_sep = u8_c(';');
    unwritten = suffixS(mem_copyBytes(unwritten, cmd_seq_begin), cmd_seq_begin.len);
    $static let cmd_upper = u8_l("38;2;");
    unwritten = suffixS(mem_copyBytes(unwritten, cmd_upper), cmd_upper.len);
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.r));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.g));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.b));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    $static let cmd_lower = u8_l("48;2;");
    unwritten = suffixS(mem_copyBytes(unwritten, cmd_lower), cmd_lower.len);
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.r));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.g));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.b));
    let cmd_seq_end = u8_c('m');
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_seq_end, 1));

    return writable_buf.len - unwritten.len;
}

$inline_always
$static fn_((writeHalfBlock(S$u8 writable_buf))(usize)) {
    $static let half_block = u8_l("▀");
    mem_copyBytes(writable_buf, half_block);
    return half_block.len;
}

$inline_always
$static fn_((writeNL(S$u8 writable_buf))(usize)) {
    $static let nl = u8_l("\n");
    mem_copyBytes(writable_buf, nl);
    return nl.len;
}

$inline_always
$static fn_((flushBuf(fs_File console_out, S$u8 written_buf))(void)) {
    DWORD written = 0;
    WriteConsoleA(console_out.handle, written_buf.ptr, (DWORD)written_buf.len, &written, null);
}

/* ========== Event Queue Helpers ============================================== */

fn_((Inst_pushInputEvent(Inst* self, engine_stub_VT100Ex_InputEvent event))(void)) {
    engine_stub_VT100Ex_InputQueue_push(
        &self->shared_state->input_queue.head,
        &self->shared_state->input_queue.tail,
        self->shared_state->input_queue.events,
        &event
    );
    InterlockedIncrement(&self->shared_state->stats.input_events_produced);
}

fn_((Inst_pushWindowEvent(Inst* self, engine_stub_VT100Ex_WindowEvent event))(void)) {
    engine_stub_VT100Ex_WindowQueue_push(
        &self->shared_state->window_queue.head,
        &self->shared_state->window_queue.tail,
        self->shared_state->window_queue.events,
        &event
    );
}

/* ========== Hook Callbacks =================================================== */

$static LRESULT CALLBACK mouseEventProc(i32 nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && tls_inst && tls_inst->wt_hwnd) {
        let mouse = intToPtr$(MSLLHOOKSTRUCT*, lParam);

        /* Check if mouse is over our WT window */
        HWND hwnd_under = WindowFromPoint(mouse->pt);
        if (hwnd_under == tls_inst->wt_hwnd || IsChild(tls_inst->wt_hwnd, hwnd_under)) {
            /* Convert screen coords to WT client coords */
            POINT client_pt = mouse->pt;
            ScreenToClient(tls_inst->wt_hwnd, &client_pt);
            /* Adjust for title bar */
            client_pt.y -= tls_inst->cached_titlebar_height;
            /* Convert to logical grid position */
            /* TODO: Use actual metrics from shared state */
            i32 logical_x = client_pt.x; /* Will be divided by cell width */
            i32 logical_y = client_pt.y; /* In half-block units */

            let timestamp = time_Instant_now();
            typedef engine_stub_VT100Ex_InputEvent InputEvent;
            switch (wParam) {
            case WM_MOUSEMOVE: {
                let delta_x = logical_x - tls_inst->last_mouse_pos.x;
                let delta_y = logical_y - tls_inst->last_mouse_pos.y;
                tls_inst->last_mouse_pos = m_V2i32_from(logical_x, logical_y);
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_move){
                            .x = logical_x,
                            .y = logical_y,
                            .delta_x = delta_x,
                            .delta_y = delta_y,
                        }),
                    })
                );
            } break;

            case WM_LBUTTONDOWN:
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_button_down){
                            .button = 0, /* Left */
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
                break;

            case WM_LBUTTONUP:
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_button_up){
                            .button = 0,
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
                break;

            case WM_RBUTTONDOWN:
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_button_down){
                            .button = 1, /* Right */
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
                break;

            case WM_RBUTTONUP:
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_button_up){
                            .button = 1,
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
                break;

            case WM_MOUSEWHEEL: {
                /* High-order word of mouseData contains wheel delta */
                i16 wheel_delta = (i16)HIWORD(mouse->mouseData);
                f32 normalized_delta = (f32)wheel_delta / (f32)WHEEL_DELTA;
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_wheel){
                            .delta_x = 0.0f,
                            .delta_y = normalized_delta,
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
            } break;

            case WM_MOUSEHWHEEL: {
                i16 wheel_delta = (i16)HIWORD(mouse->mouseData);
                f32 normalized_delta = (f32)wheel_delta / (f32)WHEEL_DELTA;
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_mouse_wheel){
                            .delta_x = normalized_delta,
                            .delta_y = 0.0f,
                            .x = logical_x,
                            .y = logical_y,
                        }),
                    })
                );
            } break;

            default:
                break;
            }
        }
    }
    return CallNextHookEx(null, nCode, wParam, lParam);
}

$static LRESULT CALLBACK keyboardEventProc(i32 nCode, WPARAM wParam, LPARAM lParam) {
    if (0 <= nCode && tls_inst && tls_inst->wt_hwnd) {
        /* Only capture keyboard when WT is focused */
        if (GetForegroundWindow() == tls_inst->wt_hwnd) {
            let kb = intToPtr$(KBDLLHOOKSTRUCT*, lParam);
            let timestamp = time_Instant_now();
            typedef engine_stub_VT100Ex_InputEvent InputEvent;
            typedef engine_stub_VT100Ex_InputEvent_KeyboardKey InputEvent_KeyboardKey;
            let key_data = lit$((InputEvent_KeyboardKey){
                .vk_code = (u16)kb->vkCode,
                .scan_code = (u16)kb->scanCode,
                .shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0,
                .ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0,
                .alt = (kb->flags & LLKHF_ALTDOWN) != 0,
            });
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_key_down)key_data),
                    })
                );
            } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                Inst_pushInputEvent(
                    tls_inst,
                    lit$((InputEvent){
                        .timestamp = timestamp,
                        .data = union_of((engine_stub_VT100Ex_InputEvent_key_up)key_data),
                    })
                );
            }
        }
    }
    return CallNextHookEx(null, nCode, wParam, lParam);
}

$static void CALLBACK winEventProc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD dwEventThread,
    DWORD dwmsEventTime
) {
    let_ignore = hWinEventHook;
    let_ignore = idObject;
    let_ignore = idChild;
    let_ignore = dwEventThread;
    let_ignore = dwmsEventTime;
    if (!tls_inst || hwnd != tls_inst->wt_hwnd) {
        return;
    }

    let timestamp = time_Instant_now();
    typedef engine_stub_VT100Ex_WindowEvent WindowEvent;
    switch (event) {
    case EVENT_OBJECT_FOCUS:
        InterlockedExchange(&tls_inst->shared_state->window_state.is_focused, 1);
        Inst_pushWindowEvent(
            tls_inst,
            lit$((WindowEvent){
                .timestamp = timestamp,
                .data = union_of((engine_stub_VT100Ex_WindowEvent_focus_gained){}),
            })
        );
        break;

    case EVENT_OBJECT_LOCATIONCHANGE:
        Inst_updateMetrics(tls_inst);
        break;

    case EVENT_SYSTEM_MINIMIZESTART:
        InterlockedExchange(&tls_inst->shared_state->window_state.is_minimized, 1);
        InterlockedExchange(&tls_inst->shared_state->window_state.is_maximized, 0);
        Inst_pushWindowEvent(
            tls_inst,
            lit$((WindowEvent){
                .timestamp = timestamp,
                .data = union_of((engine_stub_VT100Ex_WindowEvent_minimized){}),
            })
        );
        break;

    case EVENT_SYSTEM_MINIMIZEEND:
        InterlockedExchange(&tls_inst->shared_state->window_state.is_minimized, 0);
        Inst_pushWindowEvent(
            tls_inst,
            lit$((WindowEvent){
                .timestamp = timestamp,
                .data = union_of((engine_stub_VT100Ex_WindowEvent_restored){}),
            })
        );
        break;

    case EVENT_SYSTEM_FOREGROUND:
        if (hwnd == tls_inst->wt_hwnd) {
            InterlockedExchange(&tls_inst->shared_state->window_state.is_focused, 1);
        } else {
            InterlockedExchange(&tls_inst->shared_state->window_state.is_focused, 0);
            Inst_pushWindowEvent(
                tls_inst,
                lit$((WindowEvent){
                    .timestamp = timestamp,
                    .data = union_of((engine_stub_VT100Ex_WindowEvent_focus_lost){}),
                })
            );
        }
        break;

    case EVENT_OBJECT_DESTROY:
        if (hwnd == tls_inst->wt_hwnd) {
            Inst_pushWindowEvent(
                tls_inst,
                lit$((WindowEvent){
                    .timestamp = timestamp,
                    .data = union_of((engine_stub_VT100Ex_WindowEvent_close_requested){}),
                })
            );
            InterlockedExchange(&tls_inst->shared_state->control.should_exit, 1);
        }
        break;

    default:
        break;
    }
}

#if UNUSED_CODE
#include "dh/fs/common.h"

typedef struct WTSettings {
    char fontFace[128];
    i32 font_size;
    f64 line_height;
    f64 cell_width_ratio;
    i32 padding_left, padding_top, padding_right, padding_bottom;
} WTSettings;

$must_check
$static
fn_((readFile(S_const$u8 path, mem_Allocator allocator))(E$S$u8) $guard) {
    FILE* f = fopen(as$(const char*)(path.ptr), "rb");
    if (!f) { return_err(fs_File_Err_NotFound()); }
    defer_(let_ignore = fclose(f));
    let_ignore = fseek(f, 0, SEEK_END);
    let size = ftell(f);
    let_ignore = fseek(f, 0, SEEK_SET);
    let data = u_castS$((S$u8)(try_(mem_Allocator_alloc(allocator, typeInfo$(u8), size + 1))));
    let_ignore = fread(data.ptr, 1, size, f);
    data.ptr[size] = '\0';
    return_ok(data);
} $unguarded_(fn);

$static fn_((extractJsonString(const char* json, const char* key, char* out, usize outSize))(BOOL)) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    const char* keyPos = strstr(json, pattern);
    if (!keyPos) return FALSE;
    const char* colon = strchr(keyPos + strlen(pattern), ':');
    if (!colon) return FALSE;
    const char* quote1 = strchr(colon, '"');
    if (!quote1) return FALSE;
    quote1++;
    const char* quote2 = strchr(quote1, '"');
    if (!quote2) return FALSE;
    usize len = quote2 - quote1;
    if (len >= outSize) len = outSize - 1;
    strncpy(out, quote1, len);
    out[len] = '\0';
    return TRUE;
}

$static fn_((extractJsonInt(const char* json, const char* key, i32* out))(BOOL)) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    const char* keyPos = strstr(json, pattern);
    if (!keyPos) return FALSE;
    const char* colon = strchr(keyPos + strlen(pattern), ':');
    if (!colon) return FALSE;
    *out = atoi(colon + 1);
    return TRUE;
}

$static fn_((parsePaddingString(const char* str, i32* l, i32* t, i32* r, i32* b))(void)) {
    *l = *t = *r = *b = 8;
    i32 v[4];
    i32 c = sscanf(str, "%d , %d , %d , %d", &v[0], &v[1], &v[2], &v[3]);
    if (c == 1) {
        *l = *t = *r = *b = v[0];
    } else if (c == 4) {
        *l = v[0];
        *t = v[1];
        *r = v[2];
        *b = v[3];
    }
}

$static fn_((loadWTSettings(void))(WTSettings)) {
    /* Default settings */
    WTSettings s = {
        .fontFace = "Cascadia Mono", .font_size = 12, .line_height = 1.2, .cell_width_ratio = 0.6, .padding_left = 8, .padding_top = 8, .padding_right = 8, .padding_bottom = 8
    };
    let localAppData = getenv("LOCALAPPDATA");
    if (!localAppData) { return s; }

    char path[MAX_PATH];
    let_ignore = snprintf(path, MAX_PATH, "%s\\Microsoft\\Windows Terminal\\settings.json", localAppData);
    usize fileSize = 0;
    char* json = readFile(path, &fileSize);
    if (!json) {
        /* Fallback to Store package location */
        let_ignore = snprintf(path, MAX_PATH, "%s\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\settings.json", localAppData);
        json = readFile(path, &fileSize);
    }
    if (!json) { return s; }
    log_info("Loaded WT settings from: %s", path);
    const char* defaults = strstr(json, "\"defaults\"");
    if (defaults) {
        const char* obj = strchr(defaults, '{');
        if (obj) {
            const char* fontSection = strstr(obj, "\"font\"");
            if (fontSection) {
                const char* fontObj = strchr(fontSection, '{');
                if (fontObj) {
                    char face[128];
                    if (extractJsonString(fontObj, "face", face, sizeof(face))) {
                        strcpy(s.fontFace, face);
                    }
                    i32 size;
                    if (extractJsonInt(fontObj, "size", &size)) s.font_size = size;
                }
            }
            char paddingStr[64];
            if (extractJsonString(obj, "padding", paddingStr, sizeof(paddingStr))) {
                parsePaddingString(paddingStr, &s.padding_left, &s.padding_top, &s.paddingRight, &s.paddingBottom);
            }
        }
    }
    free(json);
    return s;
}

$static fn_((calculateCellSize(WTSettings* settings, f64* w, f64* h))(void)) {
    HDC screen_dc = GetDC(NULL);
    i32 dpi = GetDeviceCaps(screen_dc, LOGPIXELSY);
    ReleaseDC(NULL, screen_dc);
    f64 font_pixels = (f64)settings->font_size * dpi / 72.0;
    *h = flt_round(font_pixels * settings->line_height);
    /* *w = round(fontPixels * settings->cellWidthRatio); */
    /* Note: Use simpler ratio estimation if font metrics aren't perfect */
    *w = flt_round(*h * 0.5); // Approximation for half-block cells often used in engines
    // Or use your ratio:
    *w = flt_round(font_pixels * settings->cell_width_ratio);
    log_info("WT Metrics calculated: Cell %.0fx%.0f (Font %dpt @ %d DPI), Pad %d,%d,%d,%d", *w, *h, settings->font_size, dpi, settings->padding_left, settings->padding_top, settings->padding_right, settings->padding_bottom);
}
#endif /* UNUSED_CODE */

#endif /* plat_is_windows */

#endif /* !engine_core_VT100Ex_comp_sharded */
