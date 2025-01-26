#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)

#include "canvas.h"

#define engine_Window_max_canvases       (8)
#define engine_Window_min_width_default  (16)
#define engine_Window_min_height_default (16)
#define engine_Window_max_width_default  (4096)
#define engine_Window_max_height_default (2048)

typedef struct engine_Window        engine_Window;
typedef struct engine_WindowMetrics engine_WindowMetrics;
typedef struct engine_CanvasView    engine_CanvasView;

struct engine_WindowMetrics {
    i32  x, y;                        // Window position X, Y
    u32  width, height;               // Window width, height
    u32  min_width, min_height;       // Window width, height (min)
    u32  max_width, max_height;       // Window width, height (max)
    u32  client_width, client_height; // Client area width, height
    bool is_focused;                  // Window focus state
    bool is_minimized;                // Window minimized state
    bool is_maximized;                // Window maximized state
};
use_Ptr$(engine_WindowMetrics);
use_Opt$(engine_WindowMetrics);

extern bool engine_WindowMetrics_eq(const engine_WindowMetrics* lhs, const engine_WindowMetrics* rhs);

struct engine_CanvasView {
    i32            x, y;          // Position in window coordinates
    u32            width, height; // Size in window coordinates
    engine_Canvas* canvas;        // Associated canvas
    bool           visible;       // Visibility flag
};
use_Ptr$(engine_CanvasView);
use_Err$(engine_CanvasView);

struct engine_Window {
    engine_Platform*     platform;
    engine_WindowMetrics metrics;
    engine_CanvasView    views[engine_Window_max_canvases];
    u32                  view_count;
    engine_Canvas*       composite_buffer; // Final composition buffer
};
use_Ptr$(engine_Window);
use_Err$(engine_Window);
#define engine_Window_composite_buffer_default_color ((Color){ .r = 0x18, .g = 0x18, .b = 0x18, .a = 0xFF })

// Window management
extern Err$Ptr$engine_Window engine_Window_create(const engine_PlatformParams* params) must_check;
extern void                  engine_Window_destroy(engine_Window* window);
extern Err$void              engine_Window_processEvents(engine_Window* window) must_check;
extern void                  engine_Window_present(engine_Window* window);

// Canvas view management
extern Opt$u32 engine_Window_addCanvasView(engine_Window* window, engine_Canvas* canvas, i32 x, i32 y, u32 width, u32 height);
extern void    engine_Window_removeCanvasView(engine_Window* window, u32 view_id);
extern void    engine_Window_updateCanvasView(engine_Window* window, u32 view_id, i32 x, i32 y, u32 width, u32 height);

// Window metrics queries
force_inline Vec2i engine_Window_getPosition(const engine_Window* window) {
    return (Vec2i){
        .s = {
            window->metrics.x,
            window->metrics.y,
        },
    };
}

force_inline Vec2u engine_Window_getSize(const engine_Window* window) {
    return (Vec2u){
        .s = {
            window->metrics.width,
            window->metrics.height,
        },
    };
}

force_inline Vec2u engine_Window_getClientSize(const engine_Window* window) {
    return (Vec2u){
        .s = {
            window->metrics.client_width,
            window->metrics.client_height,
        },
    };
}

force_inline void engine_Window_setMinSize(engine_Window* window, u32 width, u32 height) {
    window->metrics.min_width  = prim_clamp(width, engine_Window_min_width_default, window->metrics.max_width);
    window->metrics.min_height = prim_clamp(height, engine_Window_min_height_default, window->metrics.max_height);
}

force_inline void engine_Window_setMaxSize(engine_Window* window, u32 width, u32 height) {
    window->metrics.max_width  = prim_clamp(width, window->metrics.min_width, engine_Window_max_width_default);
    window->metrics.max_height = prim_clamp(height, window->metrics.min_height, engine_Window_max_height_default);
}

force_inline bool engine_Window_isFocused(const engine_Window* window) {
    return window->metrics.is_focused;
}

force_inline bool engine_Window_isMinimized(const engine_Window* window) {
    return window->metrics.is_minimized;
}

force_inline bool engine_Window_isMaximized(const engine_Window* window) {
    return window->metrics.is_maximized;
}

#endif /* ENGINE_WINDOW_INCLUDED */
