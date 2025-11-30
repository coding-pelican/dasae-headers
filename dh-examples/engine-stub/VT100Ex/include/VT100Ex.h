#ifndef engine_stub_VT100Ex__included
#define engine_stub_VT100Ex__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/prl.h"
#include "dh/time.h"
#include "dh/math/vec.h"

#if plat_is_windows
#include "dh/os/windows.h"

#define engine_stub_VT100Ex_shared_mem_name L"Local\\EngineVT100ExSharedMemory"
#define engine_stub_VT100Ex_max_width 512
#define engine_stub_VT100Ex_max_height 512 /* Logical pixels (2x console rows) */
#define engine_stub_VT100Ex_input_queue_size 512
#define engine_stub_VT100Ex_event_queue_size 64

/* Color structure matching engine's Color type */
typedef struct engine_stub_VT100Ex_Color {
    u8 r, g, b, a;
} engine_stub_VT100Ex_Color;

/* ========== Input Event Types ================================================ */

typedef struct engine_stub_VT100Ex_InputEvent engine_stub_VT100Ex_InputEvent;
typedef struct engine_stub_VT100Ex_InputEvent_KeyboardKey {
    u16 vk_code;
    u16 scan_code;
    u8 shift : 1;
    u8 ctrl  : 1;
    u8 alt   : 1;
} engine_stub_VT100Ex_InputEvent_KeyboardKey;
typedef struct engine_stub_VT100Ex_InputEvent_MouseButton {
    u8 button; /* engine_MouseButton */
    i32 x, y;  /* Logical grid position (half-block) */
} engine_stub_VT100Ex_InputEvent_MouseButton;
typedef struct engine_stub_VT100Ex_InputEvent_MouseMove {
    i32 x, y; /* Logical grid position (half-block) */
    i32 delta_x, delta_y;
} engine_stub_VT100Ex_InputEvent_MouseMove;
typedef struct engine_stub_VT100Ex_InputEvent_MouseWheel {
    f32 delta_x, delta_y; /* Wheel delta (normalized) */
    i32 x, y;             /* Position when scrolled */
} engine_stub_VT100Ex_InputEvent_MouseWheel;

struct engine_stub_VT100Ex_InputEvent {
    time_Instant timestamp;
    variant_(($bits(8))(
        (engine_stub_VT100Ex_InputEvent_key_down, engine_stub_VT100Ex_InputEvent_KeyboardKey),
        (engine_stub_VT100Ex_InputEvent_key_up, engine_stub_VT100Ex_InputEvent_KeyboardKey),
        (engine_stub_VT100Ex_InputEvent_mouse_button_down, engine_stub_VT100Ex_InputEvent_MouseButton),
        (engine_stub_VT100Ex_InputEvent_mouse_button_up, engine_stub_VT100Ex_InputEvent_MouseButton),
        (engine_stub_VT100Ex_InputEvent_mouse_move, engine_stub_VT100Ex_InputEvent_MouseMove),
        (engine_stub_VT100Ex_InputEvent_mouse_wheel, engine_stub_VT100Ex_InputEvent_MouseWheel)
    )) data;
};

/* ========== Window Event Types =============================================== */

typedef struct engine_stub_VT100Ex_WindowEvent {
    time_Instant timestamp;
    variant_(($bits(8))(
        (engine_stub_VT100Ex_WindowEvent_resized, m_V2u32), (engine_stub_VT100Ex_WindowEvent_moved, m_V2i32),
        (engine_stub_VT100Ex_WindowEvent_focus_gained, Void), (engine_stub_VT100Ex_WindowEvent_focus_lost, Void),
        (engine_stub_VT100Ex_WindowEvent_minimized, Void), (engine_stub_VT100Ex_WindowEvent_maximized, Void),
        (engine_stub_VT100Ex_WindowEvent_restored, Void), (engine_stub_VT100Ex_WindowEvent_close_requested, Void)
    )) data;
} engine_stub_VT100Ex_WindowEvent;

/* ========== Frame Buffer ===================================================== */

typedef struct engine_stub_VT100Ex_FrameBuffer {
    u32 width;  /* Logical grid width */
    u32 height; /* Logical grid height (2x console rows) */
    usize frame_number;
    time_Instant timestamp;
    engine_stub_VT100Ex_Color pixels[engine_stub_VT100Ex_max_width * engine_stub_VT100Ex_max_height];
} engine_stub_VT100Ex_FrameBuffer;

/* ========== Main Shared Memory Structure ===================================== */

typedef struct engine_stub_VT100Ex_SharedState {
    /* ===== INITIALIZATION: Engine → Stub ===== */
    struct {
        volatile HWND wt_hwnd; /* Windows Terminal window handle */
        volatile LONG ready;   /* Engine has set wt_hwnd */
    } init;

    /* ===== FRAMEBUFFER: Engine → Stub ===== */
    struct {
        volatile LONG write_buf; /* Engine writes to this (0 or 1) */
        volatile LONG read_buf;  /* Stub reads from this (0 or 1) */
        volatile LONG ready[2];  /* Flags: buffer ready for reading */
        engine_stub_VT100Ex_FrameBuffer bufs[2];
    } frame;

    /* ===== INPUT EVENTS: Stub → Engine ===== */
    struct {
        volatile LONG head; /* Stub writes here */
        volatile LONG tail; /* Engine reads here */
        engine_stub_VT100Ex_InputEvent events[engine_stub_VT100Ex_input_queue_size];
    } input_queue;

    /* ===== WINDOW EVENTS: Stub → Engine ===== */
    struct {
        volatile LONG head;
        volatile LONG tail;
        engine_stub_VT100Ex_WindowEvent events[engine_stub_VT100Ex_event_queue_size];
    } window_queue;

    /* ===== TERMINAL METRICS: Stub → Engine ===== */
    struct {
        u32 grid_width;        /* Logical width (console cols) */
        u32 grid_height;       /* Logical height (2x console rows) */
        f64 cell_pixel_width;  /* Font cell width in pixels */
        f64 cell_pixel_height; /* Font cell height in pixels */
        i32 titlebar_height;   /* Detected via UI Automation */
        i32 padding_left;
        i32 padding_top;
        i32 padding_right;
        i32 padding_bottom;
        i32 window_pos_x; /* WT window position */
        i32 window_pos_y;
        volatile LONG version; /* Incremented on change */
    } metrics;

    /* ===== WINDOW STATE: Stub → Engine ===== */
    struct {
        volatile LONG is_focused;
        volatile LONG is_minimized;
        volatile LONG is_maximized;
    } window_state;

    /* ===== CONTROL: Bidirectional ===== */
    struct {
        volatile LONG engine_alive; /* Engine heartbeat */
        volatile LONG stub_alive;   /* Stub heartbeat */
        volatile LONG should_exit;  /* Shutdown signal */
        DWORD engine_pid;
        DWORD stub_pid;
    } control;

    /* ===== STATISTICS: Both sides ===== */
    struct {
        volatile LONG frames_submitted;
        volatile LONG frames_rendered;
        volatile LONG frames_dropped;
        volatile LONG input_events_produced;
        volatile LONG input_events_consumed;
    } stats;
} engine_stub_VT100Ex_SharedState;

/* ========== Lock-Free Queue Operations ======================================= */

$inline_always
$static fn_((engine_stub_VT100Ex_InputQueue_push(
    volatile LONG* head,
    volatile LONG* tail,
    engine_stub_VT100Ex_InputEvent* queue,
    const engine_stub_VT100Ex_InputEvent* event
))(bool)) {
    LONG h = InterlockedCompareExchange(head, 0, 0);
    LONG t = InterlockedCompareExchange(tail, 0, 0);
    LONG next_h = (h + 1) % engine_stub_VT100Ex_input_queue_size;

    if (next_h == t) {
        return false; /* Queue full */
    }

    queue[h] = *event;
    InterlockedExchange(head, next_h);
    return true;
}

$inline_always
$static fn_((engine_stub_VT100Ex_InputQueue_pop(
    volatile LONG* head,
    volatile LONG* tail,
    engine_stub_VT100Ex_InputEvent* queue,
    engine_stub_VT100Ex_InputEvent* out
))(bool)) {
    LONG h = InterlockedCompareExchange(head, 0, 0);
    LONG t = InterlockedCompareExchange(tail, 0, 0);

    if (h == t) {
        return false; /* Queue empty */
    }

    *out = queue[t];
    InterlockedExchange(tail, (t + 1) % engine_stub_VT100Ex_input_queue_size);
    return true;
}

$inline_always
$static fn_((engine_stub_VT100Ex_WindowQueue_push(
    volatile LONG* head,
    volatile LONG* tail,
    engine_stub_VT100Ex_WindowEvent* queue,
    const engine_stub_VT100Ex_WindowEvent* event
))(bool)) {
    LONG h = InterlockedCompareExchange(head, 0, 0);
    LONG t = InterlockedCompareExchange(tail, 0, 0);
    LONG next_h = (h + 1) % engine_stub_VT100Ex_event_queue_size;

    if (next_h == t) {
        return false; /* Queue full */
    }

    queue[h] = *event;
    InterlockedExchange(head, next_h);
    return true;
}

$inline_always
$static fn_((engine_stub_VT100Ex_WindowQueue_pop(
    volatile LONG* head,
    volatile LONG* tail,
    engine_stub_VT100Ex_WindowEvent* queue,
    engine_stub_VT100Ex_WindowEvent* out
))(bool)) {
    LONG h = InterlockedCompareExchange(head, 0, 0);
    LONG t = InterlockedCompareExchange(tail, 0, 0);

    if (h == t) {
        return false; /* Queue empty */
    }

    *out = queue[t];
    InterlockedExchange(tail, (t + 1) % engine_stub_VT100Ex_event_queue_size);
    return true;
}

#endif /* plat_is_windows */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine_stub_VT100Ex__included */
