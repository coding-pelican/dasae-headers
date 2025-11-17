#include "engine/core/VT100Ex.h"
#if engine_core_VT100Ex_comp_enabled
#include "engine-stub/VT100Ex.h"
#include "engine/Input.h"
#include "../Backend_Internal.h"

#include "dh/mem/common.h"

#if plat_windows
#if !engine_impl_connected_stub
#define engine_impl_connected_stub 1
#undef auto
#undef var
#undef let

#define COBJMACROS
#include <uiautomation.h>
#include <oleauto.h>
#include <tlhelp32.h>

#if BUILTIN_COMP_MSVC
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#endif /* BUILTIN_COMP_MSVC */

#define auto __comp_syn__auto
#define var  __comp_syn__var
#define let  __comp_syn__let
#endif /* !engine_impl_connected_stub */

/* ========== Structure ===================================================== */

struct engine_core_VT100Ex {
    struct {
        engine_Window* window;
    } abstract;
    engine_Input* input;

    /* Shared memory */
    struct {
        HANDLE mapping;
        engine_stub_VT100Ex_SharedState* state;
        LONG current_write_buffer;
        u64 frame_counter;
    } shared;

    /* Windows Terminal window tracking (via UI Automation) */
    struct {
        HWND hwnd;
        DWORD process_id;
        i32 titlebar_height;
    } wt_window;

    /* Windows Terminal settings */
    struct {
        A$$(128, u8) font_face;
        i32 font_size_pt;
        f64 font_width_ratio;
        f64 line_height_ratio;
        f64 cell_width_px;
        f64 cell_height_px;
        union {
            A$$(4, i32) lt_rb;
            struct {
                i32 left;
                i32 top;
                i32 right;
                i32 bottom;
            };
        } padding;
    } wt_settings;

    /* Stub process management */
    struct {
        PROCESS_INFORMATION info;
        HANDLE job_object;
    } stub;

    /* Event hooks */
    HWINEVENTHOOK win_event_hook;
    HANDLE monitor_thread;

    /* Cached window state */
    struct {
        m_V2i32 pos;
        m_V2u32 dim;
        m_V2u32 res;
        struct {
            m_V2u32 min;
            m_V2u32 max;
            m_V2u32 curr;
        } res_limits;
        bool is_focused;
        bool is_minimized;
        bool is_maximized;
    } cached;

    volatile bool running;
    mem_Allocator allocator;
};

/* Global context for callbacks (single instance) */
static engine_core_VT100Ex* s_vt100ex_instance = null;

/* ========== VTable Function Declarations ================================== */

static fn_((processEvents(P$raw ctx))(void));
static fn_((presentBuffer(P$raw ctx))(void));

static fn_((getWindowPos(const P$raw ctx))(m_V2i32));
static fn_((getWindowDim(const P$raw ctx))(m_V2u32));
static fn_((getWindowRes(const P$raw ctx))(m_V2u32));

static fn_((getWindowMinRes(const P$raw ctx))(m_V2u32));
static fn_((getWindowMaxRes(const P$raw ctx))(m_V2u32));
static fn_((setWindowMinRes(P$raw ctx, m_V2u32 size))(E$void)) $must_check;
static fn_((setWindowMaxRes(P$raw ctx, m_V2u32 size))(E$void)) $must_check;

static fn_((isWindowFocused(const P$raw ctx))(bool));
static fn_((isWindowMinimized(const P$raw ctx))(bool));
static fn_((isWindowMaximized(const P$raw ctx))(bool));

static fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8));
static fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool));

static fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
static fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
static fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));

static fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8));
static fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool));

static fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool));
static fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool));
static fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool));

static fn_((getMousePos(const P$raw ctx))(m_V2i32));
static fn_((getMousePosDelta(const P$raw ctx))(m_V2i32));
static fn_((getMouseWheelScrollDelta(const P$raw ctx))(m_V2f32));

/* ========== Backend VTable ================================================ */

fn_((engine_core_VT100Ex_backend(engine_core_VT100Ex* self))(engine_Backend)) {
    static const engine_Backend_VT vt[1] = { {
        .processEvents = processEvents,
        .presentBuffer = presentBuffer,
    } };

    static const engine_Backend_VT_Internal vt_internal[1] = { {
        .getWindowPos = getWindowPos,
        .getWindowDim = getWindowDim,
        .getWindowRes = getWindowRes,

        .getWindowMinRes = getWindowMinRes,
        .getWindowMaxRes = getWindowMaxRes,
        .setWindowMinRes = setWindowMinRes,
        .setWindowMaxRes = setWindowMaxRes,

        .isWindowFocused = isWindowFocused,
        .isWindowMinimized = isWindowMinimized,
        .isWindowMaximized = isWindowMaximized,

        .getKeyboardState = getKeyboardState,
        .isKeyboardState = isKeyboardState,

        .pressedKeyboard = pressedKeyboard,
        .heldKeyboard = heldKeyboard,
        .releasedKeyboard = releasedKeyboard,

        .getMouseState = getMouseState,
        .isMouseState = isMouseState,

        .pressedMouse = pressedMouse,
        .heldMouse = heldMouse,
        .releasedMouse = releasedMouse,

        .getMousePos = getMousePos,
        .getMousePosDelta = getMousePosDelta,
        .getMouseWheelScrollDelta = getMouseWheelScrollDelta,
    } };

    return (engine_Backend){
        .ptr = self,
        .vt = vt,
        .vt_internal = vt_internal
    };
}

/* ========== Forward Declarations ========================================== */

/* Shared memory management */
static fn_((createSharedMemory(engine_core_VT100Ex* self))(E$void)) $must_check;
static fn_((destroySharedMemory(engine_core_VT100Ex* self))(void));

/* Windows Terminal settings */
static fn_((loadWTSettings(engine_core_VT100Ex* self))(void));
static fn_((calcCellSize(engine_core_VT100Ex* self))(void));

/* Windows Terminal window tracking */
static fn_((spawnWT(engine_core_VT100Ex* self, S_const$u8 stub_path, S_const$u8 stub_title))(E$void)) $must_check;
static fn_((findWTWindow(engine_core_VT100Ex* self))(E$void)) $must_check;
static fn_((detectWTTitleBarHeight(HWND hwnd))(i32));
static fn_((setupWTWindowEventHooks(engine_core_VT100Ex* self))(E$void)) $must_check;

/* Metrics and state sync */
static fn_((updateMetrics(engine_core_VT100Ex* self))(void));
static fn_((syncCanvasSize(engine_core_VT100Ex* self))(E$void)) $must_check;

/* Input processing */
static fn_((processInputQueue(engine_core_VT100Ex* self))(void));
// static fn_((processWindowEvents(engine_core_VT100Ex* self))(void));
static fn_((updateKeyboardState(engine_core_VT100Ex* self))(void));

/* Monitor thread */
static DWORD WINAPI monitorThreadProc(LPVOID param);
static void CALLBACK winEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time);

/* ========== Initialization ================================================ */

errset_((InitErr)(
    FailedAllocate,
    FailedCreateSharedMem,
    FailedSpawnWT,
    FailedFindWTWindow,
    FailedSetupHooks,
    FailedSyncCanvas
));

$static let_(default_stub_path, S_const$u8) = u8_l("build/engine-stub/VT100Ex.exe");
$static let_(default_stub_title, S_const$u8) = u8_l("Engine");

fn_((engine_core_VT100Ex_init(engine_core_VT100Ex_Config config))(E$P$engine_core_VT100Ex) $guard) {
    debug_assert_nonnull(config.window);
    debug_assert_nonnull(config.input);

    let allocator = unwrap_(config.allocator);
    /* Allocate self */
    let self = u_castP$((engine_core_VT100Ex*)(try_(mem_Allocator_create(allocator, typeInfo$(InnerType)))));
    errdefer_($ignore, mem_Allocator_destroy(allocator, u_anyP(self)));

    /* Initialize structure */
    mem_setBytes(self, 0, sizeOf$(engine_core_VT100Ex));
    self->allocator = allocator;
    self->running = true;

    /* Set global instance for callbacks */
    s_vt100ex_instance = self;

    /* Initialize COM for UI Automation */
    CoInitializeEx(null, COINIT_APARTMENTTHREADED);

    /* Setup abstract layer */
    self->abstract.window = blk({
        let window = config.window;
        asg_lit((&window->backend)(some(engine_core_VT100Ex_backend(self))));
        blk_return window;
    });
    self->input = blk({
        let input = config.input;
        asg_lit((&input->backend)(some(engine_core_VT100Ex_backend(self))));
        blk_return input;
    });

    /* Load Windows Terminal settings */
    loadWTSettings(self);
    calcCellSize(self);

    try_(createSharedMemory(self));
    errdefer_($ignore, destroySharedMemory(self));

    let stub_path = orelse_((config.stub_path)(default_stub_path));
    let stub_title = orelse_((config.stub_title)(default_stub_title));
    try_(spawnWT(self, stub_path, stub_title));
    try_(findWTWindow(self));
    try_(setupWTWindowEventHooks(self));

    /* Initial metrics update */
    updateMetrics(self);
    /* Sync canvas to match terminal size */
    try_(syncCanvasSize(self));
    /* Start monitor thread */
    self->monitor_thread = CreateThread(null, 0, monitorThreadProc, self, 0, null);
    /* Initialize cached state */
    self->cached.is_focused = true;
    self->cached.is_minimized = false;
    self->cached.is_maximized = false;

    log_info("VT100Ex backend initialized: %dx%d grid", self->shared.state->metrics.grid_width, self->shared.state->metrics.grid_height);

    return_ok(self);
} $unguarded_(fn);

fn_((engine_core_VT100Ex_fini(engine_core_VT100Ex* self))(void)) {
    debug_assert_nonnull(self);
    self->running = false;
    /* Signal stub to exit */
    if (self->shared.state) {
        InterlockedExchange(&self->shared.state->control.should_exit, 1);
    }
    /* Unhook events */
    if (self->win_event_hook) {
        UnhookWinEvent(self->win_event_hook);
    }
    /* Wait for monitor thread */
    if (self->monitor_thread) {
        WaitForSingleObject(self->monitor_thread, 1000);
        CloseHandle(self->monitor_thread);
    }
    /* Terminate stub process */
    if (self->stub.info.hProcess) {
        TerminateProcess(self->stub.info.hProcess, 0);
        CloseHandle(self->stub.info.hProcess);
        CloseHandle(self->stub.info.hThread);
    }
    if (self->stub.job_object) {
        CloseHandle(self->stub.job_object);
    }
    /* Cleanup shared memory */
    destroySharedMemory(self);
    /* Cleanup COM */
    CoUninitialize();
    s_vt100ex_instance = null;
    mem_Allocator_destroy(self->allocator, u_anyP(self));
}

/* ========== Shared Memory ================================================= */

fn_((createSharedMemory(engine_core_VT100Ex* self))(E$void) $guard) {
    debug_assert_nonnull(self);

    self->shared.mapping = expr_(HANDLE $scope) if_(let mapping = CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        null,
        PAGE_READWRITE,
        0,
        sizeOf$(engine_stub_VT100Ex_SharedState),
        engine_stub_VT100Ex_shared_mem_name
    ), !mapping) {
        log_error("Failed to create shared memory: %d", GetLastError());
        return_err(Err_Unspecified());
    } else {
        $break_(mapping);
    } $unscoped_(expr);
    errdefer_($ignore, CloseHandle(self->shared.mapping));

    self->shared.state = expr_(engine_stub_VT100Ex_SharedState* $scope) if_(let state = as$(engine_stub_VT100Ex_SharedState*)(MapViewOfFile(
        self->shared.mapping,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        sizeOf$(engine_stub_VT100Ex_SharedState)
    )), !state) {
        log_error("Failed to map shared memory: %d", GetLastError());
        return_err(Err_Unspecified());
    } else {
        mem_setBytes(state, 0, sizeOf$(engine_stub_VT100Ex_SharedState));
        state->control.engine_pid = GetCurrentProcessId();
        $break_(state);
    } $unscoped_(expr);
    errdefer_($ignore, UnmapViewOfFile(self->shared.state));

    self->shared.current_write_buffer = 0;
    return_ok({});
} $unguarded_(fn);

fn_((destroySharedMemory(engine_core_VT100Ex* self))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->shared.state);
    debug_assert_nonnull(self->shared.mapping);
    self->shared.state = (UnmapViewOfFile(self->shared.state), null);
    self->shared.mapping = (CloseHandle(self->shared.mapping), null);
};

/* ========== Windows Terminal Settings ===================================== */

$static let_(default_wt_settings, FieldType$(engine_core_VT100Ex, wt_settings)) = {
    .font_face = { "Cascadia Mono" },
    .font_size_pt = 12,
    .font_width_ratio = 0.6,
    .line_height_ratio = 1.2,
    .padding = { .left = 8, .top = 8, .right = 8, .bottom = 8 },
};

fn_((loadWTSettings(engine_core_VT100Ex* self))(void)) {
    asg_lit((&self->wt_settings)(default_wt_settings));

    /* TODO: Parse %LOCALAPPDATA%\Microsoft\Windows Terminal\settings.json */
    /* See term_engine_v2.c LoadWTSettings() for implementation */
}

fn_((calcCellSize(engine_core_VT100Ex* self))(void)) {
    HDC screen_dc = GetDC(null);
    i32 dpi = GetDeviceCaps(screen_dc, LOGPIXELSY);
    ReleaseDC(null, screen_dc);

    f64 font_pixels = (f64)self->wt_settings.font_size_pt * dpi / 72.0;
    self->wt_settings.cell_height_px = flt_round(font_pixels * self->wt_settings.line_height_ratio);
    self->wt_settings.cell_width_px = flt_round(font_pixels * self->wt_settings.font_width_ratio); /* Approximate monospace ratio */

    log_debug("Cell size: %.0fx%.0f px (font: %dpt @ %d DPI)", self->wt_settings.cell_width_px, self->wt_settings.cell_height_px, self->wt_settings.font_size_pt, dpi);
}

/* ========== Windows Terminal Spawning ===================================== */

fn_((spawnWT(engine_core_VT100Ex* self, S_const$u8 stub_path, S_const$u8 stub_title))(E$void) $guard) {
    /* Create job object to auto-kill stub if engine crashes */
    self->stub.job_object = expr_(HANDLE $scope) if_(let job_object = CreateJobObjectW(null, null), !job_object) {
        log_error("Failed to create job object: %d", GetLastError());
        return_err(Err_Unspecified());
    } else {
        $break_(job_object);
    } $unscoped_(expr);
    errdefer_($ignore, CloseHandle(self->stub.job_object));
    if_(JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = { .BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE },
        !SetInformationJobObject(self->stub.job_object, JobObjectExtendedLimitInformation, &info, sizeOf$(info))) {
        log_error("Failed to set job object information: %d", GetLastError());
        return_err(Err_Unspecified());
    }
    /* Build command line */
    char cmd[MAX_PATH * 2] = {};
    let_ignore = snprintf(cmd, sizeOf$(cmd), "wt.exe new-tab --title \"%.*s\" -- \"%.*s\"", (int)stub_title.len, stub_title.ptr, (int)stub_path.len, stub_path.ptr);
    if_(STARTUPINFOA si = { .cb = sizeOf$(si) },
        !CreateProcessA(null, cmd, null, null, FALSE, 0, null, null, &si, &self->stub.info)) {
        log_error("Failed to spawn Windows Terminal: %d", GetLastError());
        return_err(Err_Unspecified());
    }
    AssignProcessToJobObject(self->stub.job_object, self->stub.info.hProcess);
    log_info("Spawned Windows Terminal with stub (PID: %lu)", self->stub.info.dwProcessId);
    return_ok({});
} $unguarded_(fn);

/* ========== Windows Terminal Window Finding =============================== */

typedef struct {
    HWND hwnd;
    DWORD process_id;
} FindWTWindowContext;

static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lParam) {
    FindWTWindowContext* ctx = (FindWTWindowContext*)lParam;
    char class_name[256];

    if (!GetClassNameA(hwnd, class_name, sizeOf$(class_name))) {
        return TRUE;
    }

    if (strstr(class_name, "CASCADIA_HOSTING_WINDOW_CLASS") && IsWindowVisible(hwnd)) {
        ctx->hwnd = hwnd;
        GetWindowThreadProcessId(hwnd, &ctx->process_id);
        return FALSE; /* Stop enumeration */
    }
    return TRUE;
}

fn_((findWTWindow(engine_core_VT100Ex* self))(E$void) $scope) {
    /* Retry loop - Windows Terminal takes time to spawn */
    for (i32 attempt = 0; attempt < 50 && self->running; ++attempt) {
        Sleep(100);

        FindWTWindowContext ctx = cleared();
        EnumWindows(enumWindowsCallback, (LPARAM)&ctx);

        if (ctx.hwnd) {
            self->wt_window.hwnd = ctx.hwnd;
            self->wt_window.process_id = ctx.process_id;
            self->wt_window.titlebar_height = detectWTTitleBarHeight(ctx.hwnd);

            log_info("Found Windows Terminal window (titlebar: %dpx)", self->wt_window.titlebar_height);
            return_ok({});
        }
    }

    log_error("Failed to find Windows Terminal window after retries");
    return_err(Err_Unspecified());
} $unscoped_(fn);

fn_((detectWTTitleBarHeight(HWND hwnd))(i32)) {
    /* Use UI Automation to detect title bar height */
    HRESULT hr;
    IUIAutomation* automation = null;
    IUIAutomationElement* window_element = null;
    IUIAutomationElementArray* children = null;
    i32 height = 40; /* Default fallback */

    hr = CoCreateInstance(&CLSID_CUIAutomation, null, CLSCTX_INPROC_SERVER, &IID_IUIAutomation, (void**)&automation);
    if (FAILED(hr)) {
        return height;
    }

    hr = IUIAutomation_ElementFromHandle(automation, hwnd, &window_element);
    if (FAILED(hr)) {
        IUIAutomation_Release(automation);
        return height;
    }

    RECT window_rect;
    GetWindowRect(hwnd, &window_rect);

    IUIAutomationCondition* true_condition = null;
    hr = IUIAutomation_CreateTrueCondition(automation, &true_condition);
    if (SUCCEEDED(hr)) {
        hr = IUIAutomationElement_FindAll(window_element, TreeScope_Children, true_condition, &children);
        if (SUCCEEDED(hr) && children) {
            i32 count = 0;
            IUIAutomationElementArray_get_Length(children, &count);

            for (i32 i = 0; i < count && i < 3; ++i) {
                IUIAutomationElement* child = null;
                if (SUCCEEDED(IUIAutomationElementArray_GetElement(children, i, &child))) {
                    CONTROLTYPEID control_type = 0;
                    RECT rect = cleared();
                    IUIAutomationElement_get_CurrentControlType(child, &control_type);
                    IUIAutomationElement_get_CurrentBoundingRectangle(child, &rect);

                    i32 element_height = rect.bottom - rect.top;
                    i32 relative_top = rect.top - window_rect.top;

                    /* Pane (50033) or TitleBar (50037) control types */
                    if ((control_type == 50033 || control_type == 50037) && element_height >= 30 && element_height <= 50 && relative_top < 10) {
                        height = element_height;
                        IUIAutomationElement_Release(child);
                        break;
                    }
                    IUIAutomationElement_Release(child);
                }
            }
        }
        if (true_condition) {
            IUIAutomationCondition_Release(true_condition);
        }
        if (children) {
            IUIAutomationElementArray_Release(children);
        }
    }

    if (window_element) {
        IUIAutomationElement_Release(window_element);
    }
    if (automation) {
        IUIAutomation_Release(automation);
    }

    return height;
}

/* ========== Window Event Hooks ============================================ */

fn_((setupWTWindowEventHooks(engine_core_VT100Ex* self))(E$void) $scope) {
    self->win_event_hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,
        EVENT_OBJECT_STATECHANGE,
        null,
        winEventCallback,
        self->wt_window.process_id,
        0,
        WINEVENT_OUTOFCONTEXT
    );
    if (!self->win_event_hook) {
        log_error("Failed to setup window event hook: %d", GetLastError());
        return_err(Err_Unspecified());
    }

    return_ok({});
} $unscoped_(fn);

static void CALLBACK winEventCallback(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD thread, DWORD time) {
    let_ignore = hook;
    let_ignore = idChild;
    let_ignore = thread;
    let_ignore = time;
    if (!s_vt100ex_instance || hwnd != s_vt100ex_instance->wt_window.hwnd) { return; }

    let self = s_vt100ex_instance;
    engine_stub_VT100Ex_WindowEvent evt = cleared();
    switch (event) {
    case EVENT_OBJECT_LOCATIONCHANGE:
        if (idObject == OBJID_WINDOW) {
            updateMetrics(self);
        }
        break;

    case EVENT_SYSTEM_FOREGROUND:
        if (!self->cached.is_focused) {
            self->cached.is_focused = true;
            InterlockedExchange(&self->shared.state->window_state.is_focused, 1);
            evt.type = engine_stub_VT100Ex_WINDOW_EVENT_FOCUS_GAINED;
            /* TODO: Push to window queue */
        }
        break;

    case EVENT_SYSTEM_MINIMIZESTART:
        self->cached.is_minimized = true;
        InterlockedExchange(&self->shared.state->window_state.is_minimized, 1);
        evt.type = engine_stub_VT100Ex_WINDOW_EVENT_MINIMIZED;
        break;

    case EVENT_SYSTEM_MINIMIZEEND:
        self->cached.is_minimized = false;
        InterlockedExchange(&self->shared.state->window_state.is_minimized, 0);
        evt.type = engine_stub_VT100Ex_WINDOW_EVENT_RESTORED;
        break;

    case EVENT_OBJECT_STATECHANGE:
        if (idObject == OBJID_WINDOW) {
            WINDOWPLACEMENT wp = { .length = sizeOf$(wp) };
            if (GetWindowPlacement(hwnd, &wp)) {
                if (wp.showCmd == SW_SHOWMAXIMIZED) {
                    self->cached.is_maximized = true;
                    InterlockedExchange(&self->shared.state->window_state.is_maximized, 1);
                }
            }
        }
        break;
    }
}

static DWORD WINAPI monitorThreadProc(LPVOID param) {
    engine_core_VT100Ex* self = (engine_core_VT100Ex*)param;
    while (self->running) {
        /* Check if Windows Terminal window still exists */
        if (!IsWindow(self->wt_window.hwnd)) {
            log_info("Windows Terminal window closed");
            self->running = false;
            break;
        }
        /* Check for focus loss */
        HWND foreground = GetForegroundWindow();
        if (self->cached.is_focused && foreground != self->wt_window.hwnd) {
            self->cached.is_focused = false;
            InterlockedExchange(&self->shared.state->window_state.is_focused, 0);
        }
        time_sleep(time_Duration_fromMillis(100));
    }
    return 0;
}

/* ========== Metrics ======================================================= */

fn_((updateMetrics(engine_core_VT100Ex* self))(void)) {
    if (!self->wt_window.hwnd) { return; }

    RECT client_rect;
    GetClientRect(self->wt_window.hwnd, &client_rect);

    i32 content_width = client_rect.right
                      - self->wt_settings.padding.left
                      - self->wt_settings.padding.right;
    i32 content_height = client_rect.bottom
                       - self->wt_window.titlebar_height
                       - self->wt_settings.padding.top
                       - self->wt_settings.padding.bottom;

    u32 cols = (self->wt_settings.cell_width_px > 0)
                 ? (u32)(content_width / self->wt_settings.cell_width_px)
                 : 80;
    u32 rows = (self->wt_settings.cell_height_px > 0)
                 ? (u32)(content_height / self->wt_settings.cell_height_px)
                 : 25;

    /* Clamp */
    cols = prim_clamp(cols, 1, engine_stub_VT100Ex_max_width);
    rows = prim_clamp(rows, 1, engine_stub_VT100Ex_max_height / 2);

    /* Logical height is 2x console rows (half-block rendering) */
    u32 logical_height = rows * 2;

    let state = self->shared.state;
    if (state->metrics.grid_width != cols || state->metrics.grid_height != logical_height) {
        state->metrics.grid_width = cols;
        state->metrics.grid_height = logical_height;
        state->metrics.cell_pixel_width = self->wt_settings.cell_width_px;
        state->metrics.cell_pixel_height = self->wt_settings.cell_height_px;
        state->metrics.titlebar_height = self->wt_window.titlebar_height;
        state->metrics.padding_left = self->wt_settings.padding.left;
        state->metrics.padding_top = self->wt_settings.padding.top;
        state->metrics.padding_right = self->wt_settings.padding.right;
        state->metrics.padding_bottom = self->wt_settings.padding.bottom;

        RECT window_rect;
        GetWindowRect(self->wt_window.hwnd, &window_rect);
        state->metrics.window_pos_x = window_rect.left;
        state->metrics.window_pos_y = window_rect.top;

        InterlockedIncrement(&state->metrics.version);

        /* Update cached resolution */
        self->cached.res.x = cols;
        self->cached.res.y = logical_height;

        log_debug("Metrics updated: %ux%u grid", cols, logical_height);
    }
}

fn_((syncCanvasSize(engine_core_VT100Ex* self))(E$void) $scope) {
    let width = self->shared.state->metrics.grid_width;
    let height = self->shared.state->metrics.grid_height;

    try_(engine_Canvas_resize(self->abstract.window->composite_buffer, width, height));

    return_ok({});
} $unscoped_(fn);

/* ========== Event Processing ============================================== */

fn_((processEvents(P$raw ctx))(void)) {
    debug_assert_nonnull(ctx);
    let self = as$(P$engine_core_VT100Ex)(ctx);

    /* Process Windows messages (for hooks) */
    MSG msg;
    i32 msg_count = 0;
    while (PeekMessage(&msg, null, 0, 0, PM_REMOVE) && msg_count < 10) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        msg_count++;
    }

    /* Check for metrics update */
    updateMetrics(self);
    if (self->cached.res.x != Grid_width(self->abstract.window->composite_buffer->buffer)
     || self->cached.res.y != Grid_height(self->abstract.window->composite_buffer->buffer)) {
        catch_((syncCanvasSize(self))(err, {
            Err_print(err);
            ErrTrace_print();
            claim_unreachable;
        }));
    }

    /* Save previous input state */
    asg(&self->input->keyboard->keys.prev_states,
        self->input->keyboard->keys.curr_states, (val));
    asg(&self->input->mouse->buttons.prev_states,
        self->input->mouse->buttons.curr_states, (val));
    self->input->mouse->cursor.prev_pos = self->input->mouse->cursor.curr_pos;
    self->input->mouse->wheel.prev_scroll_amount = self->input->mouse->wheel.curr_scroll_amount;

    /* Process input events from shared memory queue */
    processInputQueue(self);
    /* Update keyboard state (for held keys) */
    updateKeyboardState(self);
    /* Update engine heartbeat */
    InterlockedIncrement(&self->shared.state->control.engine_alive);
}

fn_((processInputQueue(engine_core_VT100Ex* self))(void)) {
    let queue = &self->shared.state->input_queue;
    engine_stub_VT100Ex_InputEvent event;
    while (engine_stub_VT100Ex_InputQueue_pop(&queue->head, &queue->tail, queue->events, &event)) {
        match_(event.data, {
            pattern_(engine_stub_VT100Ex_InputEvent_key_down, (key_down), {
                let key = (engine_KeyCode)key_down->vk_code;
                let state = A_at((self->input->keyboard->keys.curr_states)[key]);
                let prev = *A_at((self->input->keyboard->keys.prev_states)[key]);
                if (!(prev & engine_KeyButtonStates_held)) {
                    *state |= engine_KeyButtonStates_pressed;
                }
                *state |= engine_KeyButtonStates_held;
                engine_InputEventBuffer_push(
                    self->input, union_of$((engine_InputEvent)(engine_InputEvent_key_press){ .key = key })
                );
            });
            pattern_(engine_stub_VT100Ex_InputEvent_key_up, (key_up), {
                let key = (engine_KeyCode)key_up->vk_code;
                let state = A_at((self->input->keyboard->keys.curr_states)[key]);
                *state = engine_KeyButtonStates_released;
                engine_InputEventBuffer_push(
                    self->input, union_of$((engine_InputEvent)(engine_InputEvent_key_release){ .key = key })
                );
            });
            pattern_(engine_stub_VT100Ex_InputEvent_mouse_button_down, (mouse_button_down), {
                let button = (engine_MouseButton)mouse_button_down->button;
                let state = A_at((self->input->mouse->buttons.curr_states)[button]);
                let prev = *A_at((self->input->mouse->buttons.prev_states)[button]);

                if (!(prev & engine_KeyButtonStates_held)) {
                    *state |= engine_KeyButtonStates_pressed;
                }
                *state |= engine_KeyButtonStates_held;
                engine_InputEventBuffer_push(
                    self->input, union_of$((engine_InputEvent)(engine_InputEvent_mouse_press){ .button = button })
                );
            });
            pattern_(engine_stub_VT100Ex_InputEvent_mouse_button_up, (mouse_button_up), {
                let button = (engine_MouseButton)mouse_button_up->button;
                let state = A_at((self->input->mouse->buttons.curr_states)[button]);
                *state = engine_KeyButtonStates_released;
                engine_InputEventBuffer_push(
                    self->input, union_of$((engine_InputEvent)(engine_InputEvent_mouse_release){ .button = button })
                );
            });
            pattern_(engine_stub_VT100Ex_InputEvent_mouse_move, (mouse_move), {
                self->input->mouse->cursor.curr_pos.x = mouse_move->x;
                self->input->mouse->cursor.curr_pos.y = mouse_move->y;
            });
            pattern_(engine_stub_VT100Ex_InputEvent_mouse_wheel, (mouse_wheel), {
                self->input->mouse->wheel.curr_scroll_amount.x += mouse_wheel->delta_x;
                self->input->mouse->wheel.curr_scroll_amount.y += mouse_wheel->delta_y;
                engine_InputEventBuffer_push(
                    self->input,
                    union_of$((engine_InputEvent)(engine_InputEvent_mouse_scroll){
                        .delta = m_V2f32_from(mouse_wheel->delta_x, mouse_wheel->delta_y)
                    })
                );
            });
        });
        InterlockedIncrement(&self->shared.state->stats.input_events_consumed);
    }
}

fn_((updateKeyboardState(engine_core_VT100Ex* self))(void)) {
    /* Update held state for keys that remain down */
    for (engine_KeyCode key = engine_KeyCode_none + 1; key < engine_KeyCode_count; ++key) {
        let curr = A_at((self->input->keyboard->keys.curr_states)[key]);
        if (*curr & engine_KeyButtonStates_pressed) {
            /* Clear pressed flag, keep held */
            *curr = engine_KeyButtonStates_held;
        }
    }
}

/* ========== Present Buffer ================================================ */

fn_((presentBuffer(P$raw ctx))(void)) {
    debug_assert_nonnull(ctx);
    let self = as$( P$engine_core_VT100Ex) (ctx);

    if (self->cached.is_minimized) {
        return;
    }

    let shared = self->shared.state;
    LONG write_idx = self->shared.current_write_buffer;
    var fb = &shared->frame.bufs[write_idx];

    /* Check if buffer is available */
    if (InterlockedCompareExchange(&shared->frame.ready[write_idx], 0, 0)) {
        /* Buffer still has unconsumed frame - try other buffer */
        write_idx = 1 - write_idx;
        if (InterlockedCompareExchange(&shared->frame.ready[write_idx], 0, 0)) {
            /* Both buffers full - skip frame */
            InterlockedIncrement(&shared->stats.frames_dropped);
            return;
        }
        self->shared.current_write_buffer = write_idx;
        fb = &shared->frame.bufs[write_idx];
    }

    /* Copy pixel data from canvas to shared memory */
    let src = self->abstract.window->composite_buffer->buffer;
    let width = Grid_width(src);
    let height = Grid_height(src);

    fb->width = width;
    fb->height = height;
    fb->frame_number = ++self->shared.frame_counter;

#if UNUSED_CODE
    /* Direct memory copy - Color structure must match VT100Ex_Color */
    usize pixel_count = width * height;
    mem_copyFwds(
        u8_vS((u8*)fb->pixels, pixel_count * sizeOf$(engine_stub_VT100Ex_Color)),
        u8_vS((const u8*)Grid_ptr(src), pixel_count * sizeOf$(engine_stub_VT100Ex_Color))
    );
#endif /* UNUSED_CODE */

    /* Signal frame ready */
    InterlockedExchange(&shared->frame.ready[write_idx], 1);

    /* Swap to other buffer */
    self->shared.current_write_buffer = 1 - write_idx;

    InterlockedIncrement(&shared->stats.frames_submitted);
}

/* ========== VTable Implementations ======================================== */

fn_((getWindowPos(const P$raw ctx))(m_V2i32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.pos;
}

fn_((getWindowDim(const P$raw ctx))(m_V2u32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.dim;
}

fn_((getWindowRes(const P$raw ctx))(m_V2u32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.res;
}

fn_((getWindowMinRes(const P$raw ctx))(m_V2u32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.res_limits.min;
}

fn_((getWindowMaxRes(const P$raw ctx))(m_V2u32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.res_limits.max;
}

fn_((setWindowMinRes(P$raw ctx, m_V2u32 size))(E$void) $scope) {
    debug_assert_nonnull(ctx);
    $unused(ctx), $unused(size);
    return_err(Err_NotImplemented());
} $unscoped_(fn);

fn_((setWindowMaxRes(P$raw ctx, m_V2u32 size))(E$void) $scope) {
    debug_assert_nonnull(ctx);
    $unused(ctx), $unused(size);
    return_err(Err_NotImplemented());
} $unscoped_(fn);

fn_((isWindowFocused(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.is_focused;
}

fn_((isWindowMinimized(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.is_minimized;
}

fn_((isWindowMaximized(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return self->cached.is_maximized;
}

fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Keyboard_getState(self->input->keyboard, key);
}

fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Keyboard_isState(self->input->keyboard, key, state);
}

fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Keyboard_pressed(self->input->keyboard, key);
}

fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Keyboard_held(self->input->keyboard, key);
}

fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Keyboard_released(self->input->keyboard, key);
}

fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_getState(self->input->mouse, button);
}

fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_isState(self->input->mouse, button, state);
}

fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_pressed(self->input->mouse, button);
}

fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_held(self->input->mouse, button);
}

fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_released(self->input->mouse, button);
}

fn_((getMousePos(const P$raw ctx))(m_V2i32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_getPos(self->input->mouse);
}

fn_((getMousePosDelta(const P$raw ctx))(m_V2i32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_getPosDelta(self->input->mouse);
}

fn_((getMouseWheelScrollDelta(const P$raw ctx))(m_V2f32)) {
    debug_assert_nonnull(ctx);
    let self = as$( const P$engine_core_VT100Ex) (ctx);
    return engine_Mouse_getWheelScrollDelta(self->input->mouse);
}

#else
/* Non-Windows stub */
#endif /* plat_windows */

#endif /* engine_core_VT100Ex_comp_enabled */
