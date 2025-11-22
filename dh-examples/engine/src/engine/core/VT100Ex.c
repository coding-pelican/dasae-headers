/* engine_core_VT100Ex.c
 *
 * Thin engine backend that delegates rendering to stub process.
 *
 * Responsibilities:
 *   - Create shared memory for IPC
 *   - Spawn Windows Terminal process
 *   - Find WT window handle (via EnumWindows)
 *   - Spawn stub process (passes WT HWND)
 *   - Copy framebuffer to shared memory
 *   - Read input events from shared memory and dispatch to engine_Input
 *   - Read window metrics/state from shared memory
 *
 * NO COM, NO UI Automation, NO hooks - all that complexity lives in stub.
 */

#include "engine/core/VT100Ex.h"
#include "engine-stub/VT100Ex.h" /* Shared memory definitions (stub header) */
#include "engine/Input.h"
#include "../Backend_Internal.h"

#include "dh/mem/common.h"
#include "dh/ArrList.h"
#include "dh/log.h"
#include "dh/time.h"
#include "engine/KeyCode.h"

#if plat_windows
#include "dh/os/windows.h"

/* ========== Structure ======================================================== */

struct engine_core_VT100Ex {
    /* Engine systems */
    engine_Window* window;
    engine_Input* input;
    mem_Allocator gpa;
    /* Shared memory IPC */
    HANDLE shared_mapping;
    engine_stub_VT100Ex_SharedState* shared_state;
    /* Process management */
    HANDLE wt_process;
    HANDLE stub_process;
    DWORD wt_pid;
    DWORD stub_pid;
    /* WT window handle (discovered after spawn) */
    HWND wt_hwnd;
    /* Frame tracking */
    usize frame_number;
    usize current_write_buf;
    /* Cached metrics */
    u32 last_metrics_version;
    /* Window title for WT discovery */
    wchar_t wt_title[64];
};

/* ========== Backend VTable Functions ========================================= */

$static fn_((processEvents(P$raw ctx))(void));
$static fn_((presentBuffer(P$raw ctx))(void));

$static fn_((getWindowPos(const P$raw ctx))(m_V2i32));
$static fn_((getWindowDim(const P$raw ctx))(m_V2u32));
$static fn_((getWindowRes(const P$raw ctx))(m_V2u32));

$static fn_((getWindowMinRes(const P$raw ctx))(m_V2u32));
$static fn_((getWindowMaxRes(const P$raw ctx))(m_V2u32));
$static fn_((setWindowMinRes(P$raw ctx, m_V2u32 size))(E$void)) $must_check;
$static fn_((setWindowMaxRes(P$raw ctx, m_V2u32 size))(E$void)) $must_check;

$static fn_((isWindowFocused(const P$raw ctx))(bool));
$static fn_((isWindowMinimized(const P$raw ctx))(bool));
$static fn_((isWindowMaximized(const P$raw ctx))(bool));

$static fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8));
$static fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool));

$static fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
$static fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
$static fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));

$static fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8));
$static fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool));

$static fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool));
$static fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool));
$static fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool));

$static fn_((getMousePos(const P$raw ctx))(m_V2i32));
$static fn_((getMousePosDelta(const P$raw ctx))(m_V2i32));
$static fn_((getMouseWheelScrollDelta(const P$raw ctx))(m_V2f32));

/* ========== Backend Interface ================================================ */

fn_((engine_core_VT100Ex_backend(engine_core_VT100Ex* self))(engine_Backend)) {
    $static const engine_Backend_VT vt[1] = { {
        .processEvents = processEvents,
        .presentBuffer = presentBuffer,
    } };

    $static const engine_Backend_VT_Internal vt_internal[1] = { {
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
        .vt_internal = vt_internal,
    };
}

/* ========== Helper Functions ================================================= */

$static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lParam) {
    HWND* out_hwnd = (HWND*)lParam;

    /* Check if window is visible */
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    /* Check window class - WT uses "CASCADIA_HOSTING_WINDOW_CLASS" */
    wchar_t class_name[256];
    if (!GetClassNameW(hwnd, class_name, 256)) {
        return TRUE;
    }

    if (wcscmp(class_name, L"CASCADIA_HOSTING_WINDOW_CLASS") == 0) {
        log_info("Found Windows Terminal window: HWND=%p", (void*)hwnd);
        *out_hwnd = hwnd;
        return FALSE; /* Stop enumeration */
    }

    return TRUE; /* Continue enumeration */
}

$static fn_((findWTWindow(void))(HWND)) {
    HWND found_hwnd = null;
    EnumWindows(enumWindowsCallback, (LPARAM)&found_hwnd);
    return found_hwnd;
}

#if UNUSED_CODE
$static fn_((generateUniqueTitle(wchar_t* buf, usize buf_len))(void)) {
    /* Generate unique title using PID and timestamp */
    DWORD pid = GetCurrentProcessId();
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    _snwprintf(buf, buf_len, L"EngineVT100Ex_%lu_%lld", pid, counter.QuadPart);
}
#endif /* UNUSED_CODE */

$static fn_((createSharedMemory(void))(HANDLE)) {
    SECURITY_ATTRIBUTES sa = {
        .nLength = sizeof(SECURITY_ATTRIBUTES),
        .lpSecurityDescriptor = null,
        .bInheritHandle = TRUE, /* Inheritable by child processes */
    };

    return CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        &sa,
        PAGE_READWRITE,
        0,
        (DWORD)sizeOf$(engine_stub_VT100Ex_SharedState),
        engine_stub_VT100Ex_shared_mem_name
    );
}

$static fn_((spawnWindowsTerminalWithStub(
    const wchar_t* title,
    const wchar_t* profile,
    S_const$u8 stub_path
))(HANDLE)) {
    /* Convert stub path to wide string */
    wchar_t wide_stub_path[MAX_PATH];
    MultiByteToWideChar(
        CP_UTF8, 0,
        (const char*)stub_path.ptr, (int)stub_path.len,
        wide_stub_path, MAX_PATH
    );
    wide_stub_path[stub_path.len < MAX_PATH ? stub_path.len : MAX_PATH - 1] = L'\0';

    /* Build command line: wt.exe [options] -- stub.exe
     *
     * The "--" tells WT to run the stub executable inside the terminal.
     * This is CRITICAL so the stub can access the WT console via GetStdHandle().
     */
    wchar_t cmd[2048];
    if (profile && wcslen(profile) > 0) {
        _snwprintf(cmd, 2048, L"wt.exe new-tab -p \"%s\" --title \"%s\" -- \"%s\"", profile, title, wide_stub_path);
    } else {
        _snwprintf(cmd, 2048, L"wt.exe new-tab --title \"%s\" -- \"%s\"", title, wide_stub_path);
    }

    STARTUPINFOW si = { .cb = sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi = { 0 };

    if (!CreateProcessW(
            null,
            cmd,
            null,
            null,
            FALSE,
            0,
            null,
            null,
            &si,
            &pi
        )) {
        return null;
    }

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

$static fn_((dispatchInputEvent(engine_core_VT100Ex* self, const engine_stub_VT100Ex_InputEvent* event))(void));
$static fn_((dispatchWindowEvent(engine_core_VT100Ex* self, const engine_stub_VT100Ex_WindowEvent* event))(void));

/* ========== Initialization =================================================== */

errset_((InitErr)(
    FailedAllocSelf,
    FailedCreateSharedMemory,
    FailedMapSharedMemory,
    FailedSpawnWT,
    FailedFindWTWindow,
    FailedSpawnStub,
    StubNotReady
));

typedef A$$(MAX_PATH, u8) PathBuf;
$must_check
$static
fn_((resolveStubPath(S_const$u8 stub_path, PathBuf* absolute_buf))(E$S$u8) $scope) {
    var absolute = A_ref$((S$u8)*absolute_buf);
    var relative = mem_copyBytes(A_ref$((S$u8)lit$((PathBuf){})), stub_path);
    if (!GetFullPathNameA(as$(const char*)(relative.ptr), MAX_PATH, as$(char*)(absolute.ptr), null)) {
        log_error("Failed to resolve stub path: %s", as$(const char*)(relative.ptr));
        return_err(InitErr_FailedSpawnStub());
    }
    if (GetFileAttributesA(as$(const char*)(absolute.ptr)) == INVALID_FILE_ATTRIBUTES) {
        log_error("Stub file not found: %s", as$(const char*)(absolute.ptr));
        return_err(InitErr_FailedSpawnStub());
    }
    log_info("Stub path resolved: %s", as$(const char*)(absolute.ptr));
    return_ok(absolute);
} $unscoped_(fn);

fn_((engine_core_VT100Ex_init(engine_core_VT100Ex_Config config))(E$P$engine_core_VT100Ex) $guard) {
    let gpa = unwrap_(config.gpa);

    /* Allocate self */
    let self = u_castP$((engine_core_VT100Ex*)(catch_((mem_Allocator_create(gpa, typeInfo$(engine_core_VT100Ex)))($ignore, {
        log_error("Failed to allocate VT100Ex backend");
        return_err(InitErr_FailedAllocSelf());
    }))));
    errdefer_($ignore, mem_Allocator_destroy(gpa, u_anyP(self)));

    mem_setBytes0(mem_asBytes(self));
    self->gpa = gpa;
    self->window = blk({
        let window = config.window;
        asg_lit((&window->backend)(some(engine_core_VT100Ex_backend(self))));
        blk_return window;
    });
    self->input = blk({
        let input = config.input;
        asg_lit((&input->backend)(some(engine_core_VT100Ex_backend(self))));
        blk_return input;
    });

    /* Create shared memory BEFORE spawning (stub needs it) */
    self->shared_mapping = createSharedMemory();
    if (!self->shared_mapping) {
        log_error("Failed to create shared memory: %lu", GetLastError());
        return_err(InitErr_FailedCreateSharedMemory());
    }
    errdefer_($ignore, CloseHandle(self->shared_mapping));

    self->shared_state = as$(engine_stub_VT100Ex_SharedState*)(MapViewOfFile(
        self->shared_mapping,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        sizeOf$(engine_stub_VT100Ex_SharedState)
    ));
    if (!self->shared_state) {
        log_error("Failed to map shared memory: %lu", GetLastError());
        return_err(InitErr_FailedMapSharedMemory());
    }
    errdefer_($ignore, UnmapViewOfFile(self->shared_state));

    mem_setBytes0(mem_asBytes(self->shared_state));
    self->shared_state->control.engine_pid = GetCurrentProcessId();
    self->shared_state->control.engine_alive = 1;

    /* Set initial grid dimensions based on window config */
    /* TODO: Read from engine_Window or wait for stub to report actual size */
    //     self->shared_state->metrics.grid_width = engine_Window_width(self->window);
    // self->shared_state->metrics.grid_height = engine_Window_height(self->window) * 2;
    self->shared_state->metrics.grid_width = Grid_width(self->window->composite_buffer->buffer);
    self->shared_state->metrics.grid_height = Grid_height(self->window->composite_buffer->buffer) * 2; /* Half-block height */

    /* Spawn WT WITH stub inside it (single command) */
    wchar_t wide_profile[256] = { 0 };
    if (isSome(config.stub_profile)) {
        let profile = unwrap_(config.stub_profile);
        MultiByteToWideChar(CP_UTF8, 0, (const char*)profile.ptr, (int)profile.len, wide_profile, 256);
    }

    self->wt_process = spawnWindowsTerminalWithStub(
        L"EngineVT100Ex",
        wide_profile[0] ? wide_profile : null,
        try_(resolveStubPath(unwrap_(config.stub_path), &(PathBuf){})).as_const
    );
    if (!self->wt_process) {
        log_error("Failed to spawn Windows Terminal with stub: %lu", GetLastError());
        return_err(InitErr_FailedSpawnWT());
    }
    errdefer_($ignore, {
        TerminateProcess(self->wt_process, 0);
        CloseHandle(self->wt_process);
    });
    log_info("Spawned Windows Terminal with stub inside");

    /* Wait for WT window to appear */
    self->wt_hwnd = null;
    log_info("Waiting for Windows Terminal window to appear...");
    for (int retry = 0; retry < 100; retry++) {
        self->wt_hwnd = findWTWindow();
        if (self->wt_hwnd) {
            GetWindowThreadProcessId(self->wt_hwnd, &self->wt_pid);
            log_info("Found WT window after %d attempts: HWND=%p, PID=%lu", retry + 1, (void*)self->wt_hwnd, self->wt_pid);
            break;
        }
        if (retry % 10 == 9) {
            log_debug("Still waiting for WT window... (%d/100)", retry + 1);
        }
        time_sleep(time_Duration_fromMillis(100));
    }
    if (!self->wt_hwnd) {
        log_error("Failed to find Windows Terminal window");
        return_err(InitErr_FailedFindWTWindow());
    }

    /* Write WT HWND to shared memory for stub */
    InterlockedExchangePointer((volatile PVOID*)&self->shared_state->init.wt_hwnd, self->wt_hwnd);
    InterlockedExchange(&self->shared_state->init.ready, 1);

    /* Wait for stub to signal ready */
    log_info("Waiting for stub to initialize...");
    for (int retry = 0; retry < 100; retry++) {
        if (InterlockedCompareExchange(&self->shared_state->control.stub_alive, 0, 0) > 0) {
            self->stub_pid = self->shared_state->control.stub_pid;
            log_info("Stub ready: PID=%lu", self->stub_pid);
            break;
        }
        time_sleep(time_Duration_fromMillis(100));
    }
    if (self->shared_state->control.stub_alive == 0) {
        log_error("Stub process did not signal ready");
        return_err(InitErr_StubNotReady());
    }

    /* Initialize frame tracking */
    self->frame_number = 0;
    self->current_write_buf = 0;
    self->last_metrics_version = 0;

    log_info("VT100Ex backend initialized: WT PID=%lu, Stub PID=%lu", self->wt_pid, self->stub_pid);
    return_ok(self);
} $unguarded_(fn);

fn_((engine_core_VT100Ex_fini(engine_core_VT100Ex* self))(void)) {
    claim_assert_nonnull(self);

    /* Signal shutdown */
    InterlockedExchange(&self->shared_state->control.should_exit, 1);

    /* Wait for stub to exit gracefully */
    WaitForSingleObject(self->stub_process, 5000);

    /* Clean up processes */
    if (self->stub_process) {
        TerminateProcess(self->stub_process, 0); /* Force if still running */
        CloseHandle(self->stub_process);
    }

    if (self->wt_process) {
        /* Don't terminate WT, just close our handle */
        CloseHandle(self->wt_process);
    }

    /* Clean up shared memory */
    if (self->shared_state) {
        UnmapViewOfFile(self->shared_state);
    }
    if (self->shared_mapping) {
        CloseHandle(self->shared_mapping);
    }

    /* Free self */
    mem_Allocator_destroy(self->gpa, u_anyP(self));
}

/* ========== Event Processing ================================================= */

$must_check
$static
fn_((syncWindowMetrics(engine_core_VT100Ex* self))(E$void) $scope) {
    /* 공유 메모리에서 Stub이 계산한 최신 그리드 크기를 가져옵니다. */
    u32 new_width = self->shared_state->metrics.grid_width;
    u32 new_height = self->shared_state->metrics.grid_height;
    /* 크기가 유효한지 확인 (최소 1x1) */
    if (new_width == 0 || new_height == 0) {
        return_ok({}); /* 아직 초기화되지 않았거나 최소화된 상태일 수 있음 */
    }
    /* 엔진의 메인 그리기 버퍼(Canvas)를 새 해상도에 맞춰 리사이징합니다. */
    try_(engine_Canvas_resize(
        self->window->composite_buffer,
        new_width,
        new_height
    ));
    /* 로그 출력 (디버깅용) */
    // log_debug("Window metrics synced: %dx%d (v%d)",
    //     new_width, new_height, self->shared_state->metrics.version);
    return_ok({});
} $unscoped_(fn);

fn_((processEvents(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    let self = as$(engine_core_VT100Ex*)(ctx);

    /* Update engine heartbeat */
    InterlockedIncrement(&self->shared_state->control.engine_alive);

    /* Check if stub is still alive */
    DWORD exit_code = 0;
    if (GetExitCodeProcess(self->stub_process, &exit_code) && exit_code != STILL_ACTIVE) {
        log_error("Stub process exited unexpectedly: %lu", exit_code);
        /* Could signal error to engine here */
    }

    /* Process input events from shared memory queue */
    engine_stub_VT100Ex_InputEvent input_event;
    while (engine_stub_VT100Ex_InputQueue_pop(
        &self->shared_state->input_queue.head,
        &self->shared_state->input_queue.tail,
        self->shared_state->input_queue.events,
        &input_event
    )) {
        dispatchInputEvent(self, &input_event);
        InterlockedIncrement(&self->shared_state->stats.input_events_consumed);
    }

    /* Process window events */
    engine_stub_VT100Ex_WindowEvent window_event;
    while (engine_stub_VT100Ex_WindowQueue_pop(
        &self->shared_state->window_queue.head,
        &self->shared_state->window_queue.tail,
        self->shared_state->window_queue.events,
        &window_event
    )) {
        dispatchWindowEvent(self, &window_event);
    }

    /* Update cached metrics if version changed */
    LONG metrics_version = InterlockedCompareExchange(&self->shared_state->metrics.version, 0, 0);
    if ((u32)metrics_version != self->last_metrics_version) {
        self->last_metrics_version = (u32)metrics_version;

        /* [추가됨] 버전이 변경되었으므로 엔진 내부 버퍼 크기를 동기화합니다. */
        catch_((syncWindowMetrics(self))(err, {
            log_error("Failed to sync window metrics in processEvents");
            Err_print(err);
            /* 심각한 오류지만, 루프를 멈추기보다는 로그를 남기고 진행합니다. */
        }));
    }
}

$maybe_unused
$static fn_((mapVkToKeyCode(u16 vk))(engine_KeyCode)) {
    /* Basic mapping - expand based on engine_KeyCode definition */
    if (vk >= '0' && vk <= '9') { return (engine_KeyCode)(engine_KeyCode_n0 + (vk - '0')); }
    if (vk >= 'A' && vk <= 'Z') { return (engine_KeyCode)(engine_KeyCode_a + (vk - 'A')); }

    switch (vk) {
    case VK_BACK:
        return engine_KeyCode_backspace;
    case VK_TAB:
        return engine_KeyCode_tab;
    case VK_RETURN:
        return engine_KeyCode_enter;
    case VK_ESCAPE:
        return engine_KeyCode_esc;
    case VK_SPACE:
        return engine_KeyCode_space;
    case VK_LEFT:
        return engine_KeyCode_arrow_left;
    case VK_UP:
        return engine_KeyCode_arrow_up;
    case VK_RIGHT:
        return engine_KeyCode_arrow_right;
    case VK_DOWN:
        return engine_KeyCode_arrow_down;
    // case VK_DELETE:     return engine_KeyCode_delete;
    case VK_SHIFT:
        return engine_KeyCode_shift_left; /* Simplified */
    case VK_CONTROL:
        return engine_KeyCode_ctrl_left; /* Simplified */
    case VK_MENU:
        return engine_KeyCode_alt_left; /* Simplified */
    // case VK_F1:         return engine_KeyCode_f1;
    // case VK_F2:         return engine_KeyCode_f2;
    // case VK_F3:         return engine_KeyCode_f3;
    // case VK_F4:         return engine_KeyCode_f4;
    // case VK_F5:         return engine_KeyCode_f5;
    // case VK_F6:         return engine_KeyCode_f6;
    // case VK_F7:         return engine_KeyCode_f7;
    // case VK_F8:         return engine_KeyCode_f8;
    // case VK_F9:         return engine_KeyCode_f9;
    // case VK_F10:        return engine_KeyCode_f10;
    // case VK_F11:        return engine_KeyCode_f11;
    // case VK_F12:        return engine_KeyCode_f12;
    /* Add more mappings as defined in engine_KeyCode.h */
    default:
        return engine_KeyCode_none;
    }
}

$maybe_unused
$static fn_((mapInputKeyModes(bool shift, bool ctrl, bool alt))(engine_InputKeyModes)) {
    engine_InputKeyModes modes = { 0 };
    modes.shift = shift;
    modes.control = ctrl;
    modes.alt = alt;
    return modes;
}

fn_((dispatchInputEvent(engine_core_VT100Ex* self, const engine_stub_VT100Ex_InputEvent* event))(void)) {
    let_ignore = self;
    let_ignore = event;
#if UNUSED_CODE
    /* Stub event type aliases for brevity */
    typedef engine_stub_VT100Ex_InputEvent StubEvent;

    /* Common timestamp handling if needed, though engine_Input usually generates its own on process */

    switch (event->data.tag) {
    case engine_stub_VT100Ex_InputEvent_keyboard_key_down: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_key_down;
        let key = mapVkToKeyCode(d->vk_code);
        if (key == engine_KeyCode_none) break;

        let modes = mapInputKeyModes(d->shift, d->ctrl, d->alt);

        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_key_press){
                .key = key,
                .modes = modes })
        );
        break;
    }

    case engine_stub_VT100Ex_InputEvent_key_up: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_key_up;
        let key = mapVkToKeyCode(d->vk_code);
        if (key == engine_KeyCode_none) break;

        let modes = mapInputKeyModes(d->shift, d->ctrl, d->alt);

        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_key_release){
                .key = key,
                .modes = modes })
        );
        break;
    }

    case engine_stub_VT100Ex_InputEvent_mouse_move: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_mouse_move;
        /* Use absolute position provided by stub (logical grid coords) */
        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_mouse_motion){
                .pos = m_V2i32_from(d->x, d->y) })
        );
        break;
    }

    case engine_stub_VT100Ex_InputEvent_mouse_button_down: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_mouse_button_down;
        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_mouse_press){
                .button = (engine_MouseButton)d->button, /* Direct mapping 0=Left, 1=Right */
                .pos = m_V2i32_from(d->x, d->y),
                .modes = { 0 } /* Mouse event structs in stub don't carry mods currently */
            })
        );
        break;
    }

    case engine_stub_VT100Ex_InputEvent_mouse_button_up: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_mouse_button_up;
        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_mouse_release){
                .button = (engine_MouseButton)d->button,
                .pos = m_V2i32_from(d->x, d->y),
                .modes = { 0 } })
        );
        break;
    }

    case engine_stub_VT100Ex_InputEvent_mouse_wheel: {
        let d = &event->data.engine_stub_VT100Ex_InputEvent_mouse_wheel;
        engine_InputEventBuffer_push(
            self->input,
            union_of$((engine_InputEvent)(engine_InputEvent_mouse_scroll){
                .delta = m_V2f32_from(d->delta_x, d->delta_y) })
        );
        break;
    }

    default:
        break;
    }
#endif /* UNUSED_CODE */
}

fn_((dispatchWindowEvent(engine_core_VT100Ex* self, const engine_stub_VT100Ex_WindowEvent* event))(void)) {
    let_ignore = self;
    /* Handle high-level window events.
     * Note: Metrics/Size updates are handled automatically via shared state polling in processEvents.
     */
    switch (event->data.tag) {
    case engine_stub_VT100Ex_WindowEvent_close_requested:
        /* Signal the window system that it should close.
         * If engine_Window has a close flag, set it here.
         * For now, we log it. */
        log_info("Window close requested by user");
        /* If you have a way to request engine shutdown: */
        // engine_Window_requestClose(self->window);
        break;

    case engine_stub_VT100Ex_WindowEvent_resized:
        /* Stub has detected a resize.
         * The actual dimension data is updated via shared_state->metrics.
         * This event is a notification trigger if we need to rebuild buffers immediately. */
        {
            /* Optional: Log or trigger immediate resize logic if not handled by polling */
            // let size = event->data.engine_stub_VT100Ex_WindowEvent_resized;
            // log_debug("Window resized to: %dx%d", size.x, size.y);
        }
        break;

    case engine_stub_VT100Ex_WindowEvent_focus_gained:
        log_debug("Window focus gained");
        break;

    case engine_stub_VT100Ex_WindowEvent_focus_lost:
        log_debug("Window focus lost");
        break;

    default:
        break;
    }
}

/* ========== Buffer Presentation ============================================== */

fn_((presentBuffer(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    let self = as$(engine_core_VT100Ex*)(ctx);

    /* Get the engine's composite buffer */
    let width = Grid_width(self->window->composite_buffer->buffer);
    let height = Grid_height(self->window->composite_buffer->buffer);
    let pixels = self->window->composite_buffer->buffer.items.ptr;

    /* Find available write buffer (double buffering) */
    usize buf_idx = self->current_write_buf;
    /* Wait if buffer is still being read by stub */
    int attempts = 0;
    while (InterlockedCompareExchange(&self->shared_state->frame.ready[buf_idx], 0, 0) && attempts < 100) {
        time_sleep(time_Duration_fromMicros(100));
        attempts++;
    }
    if (attempts >= 100) {
        /* Stub is too slow, drop frame */
        InterlockedIncrement(&self->shared_state->stats.frames_dropped);
        return;
    }

    /* Copy framebuffer to shared memory */
    engine_stub_VT100Ex_FrameBuffer* frame_buf = &self->shared_state->frame.bufs[buf_idx];
    frame_buf->width = width;
    frame_buf->height = height;
    frame_buf->frame_number = ++self->frame_number;
    frame_buf->timestamp = time_Instant_now();
    /* Copy pixel data */
    let total_pixels = (usize)width * (usize)height;
    if (total_pixels <= (usize)engine_stub_VT100Ex_max_width * engine_stub_VT100Ex_max_height) {
        mem_copy(
            u_anyS(slice$P((Color*)&*frame_buf->pixels, $r(0, total_pixels))),
            u_anyS(slice$P(pixels, $r(0, total_pixels))).as_const
        );
    }

    /* Mark buffer as ready for stub to consume */
    InterlockedExchange(&self->shared_state->frame.ready[buf_idx], 1);
    InterlockedIncrement(&self->shared_state->stats.frames_submitted);
    /* Swap to other buffer for next frame */
    self->current_write_buf = (buf_idx + 1) % 2;
}

/* ========== Window Queries (read from shared state) ========================= */

fn_((getWindowPos(const P$raw ctx))(m_V2i32)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return m_V2i32_from(
        self->shared_state->metrics.window_pos_x,
        self->shared_state->metrics.window_pos_y
    );
}

/* TODO: Apply pixel cell size to dimensions */
fn_((getWindowDim(const P$raw ctx))(m_V2u32)) {
    /* Return logical grid dimensions */
    var dim = getWindowRes(ctx);
    return lit$((m_V2u32){
        .x = dim.x,
        .y = dim.y / 2 });
}

fn_((getWindowRes(const P$raw ctx))(m_V2u32)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    /* Return logical grid dimensions */
    return lit$((m_V2u32){
        .x = self->shared_state->metrics.grid_width,
        .y = self->shared_state->metrics.grid_height });
}

fn_((getWindowMinRes(const P$raw ctx))(m_V2u32)) {
    let_ignore = ctx;
    return lit$((m_V2u32){ .x = 1, .y = 1 });
}

fn_((getWindowMaxRes(const P$raw ctx))(m_V2u32)) {
    let_ignore = ctx;
    return lit$((m_V2u32){
        .x = engine_stub_VT100Ex_max_width,
        .y = engine_stub_VT100Ex_max_height / 2 });
}


fn_((setWindowMinRes(P$raw ctx, m_V2u32 size))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = size;
    /* Not applicable for terminal */
    return_err(Err_NotImplemented());
} $unscoped_(fn);

fn_((setWindowMaxRes(P$raw ctx, m_V2u32 size))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = size;
    /* Not applicable for terminal */
    return_err(Err_NotImplemented());
} $unscoped_(fn);

fn_((isWindowFocused(const P$raw ctx))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return InterlockedCompareExchange(&self->shared_state->window_state.is_focused, 0, 0) != 0;
}

fn_((isWindowMinimized(const P$raw ctx))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return InterlockedCompareExchange(&self->shared_state->window_state.is_minimized, 0, 0) != 0;
}

fn_((isWindowMaximized(const P$raw ctx))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return InterlockedCompareExchange(&self->shared_state->window_state.is_maximized, 0, 0) != 0;
}

/* ========== Input Queries (delegate to engine_Input) ======================== */

fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Keyboard_getState(self->input->keyboard, key);
}

fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Keyboard_isState(self->input->keyboard, key, state);
}

fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Keyboard_pressed(self->input->keyboard, key);
}

fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Keyboard_held(self->input->keyboard, key);
}

fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Keyboard_released(self->input->keyboard, key);
}

fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_getState(self->input->mouse, button);
}

fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_isState(self->input->mouse, button, state);
}

fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_pressed(self->input->mouse, button);
}

fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_held(self->input->mouse, button);
}

fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_released(self->input->mouse, button);
}

fn_((getMousePos(const P$raw ctx))(m_V2i32)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_getPos(self->input->mouse);
}

fn_((getMousePosDelta(const P$raw ctx))(m_V2i32)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_getPosDelta(self->input->mouse);
}

fn_((getMouseWheelScrollDelta(const P$raw ctx))(m_V2f32)) {
    let self = as$(const engine_core_VT100Ex*)(ctx);
    return engine_Mouse_getWheelScrollDelta(self->input->mouse);
}

#else
/* Non-Windows stub */
#endif /* plat_windows */
