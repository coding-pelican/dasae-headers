#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/Color.h"
#include "engine-wip/Canvas.h"

#include "engine-wip/Backend.h"

typedef struct engine_WindowConfig {
    mem_Allocator allocator;
    Vec2u         rect_size;
    StrConst      title;
    Opt$Color     default_color;
} engine_WindowConfig;
typedef struct engine_Window engine_Window;
#define engine_Window_size_min_width_default         (16)
#define engine_Window_size_min_height_default        (16)
#define engine_Window_size_max_width_default         (4096)
#define engine_Window_size_max_height_default        (2048)
#define engine_Window_composite_buffer_color_default ((Color){ .r = 0x18, .g = 0x18, .b = 0x18, .a = 0xFF })

typedef struct engine_CanvasView engine_CanvasView;
#define engine_Window_max_canvases (8)
typedef Arr$(engine_Window_max_canvases, struct engine_CanvasView {
    engine_Canvas* canvas; // Associated canvas
    struct {
        Vec2u top_left;
    } pos_on_window; // Position in window coordinates
    struct {
        Vec2u size;  // Size in window coordinates
        Vec2f scale; // Scale in window coordinates
        /* TODO: Add options: stretch, crop, resizable, scalable, ... */
        struct {
            bool x;
            bool y;
        } resizable;
    } rect;
    bool visible; // Visibility flag
}) engine_CanvasViews;

struct engine_Window {
    struct {
        engine_CanvasViews list;
        u32                count;
    } views;
    engine_Canvas*     composite_buffer; // Final composition buffer
    Opt$engine_Backend backend;
    mem_Allocator      allocator;
};
use_Sli$(engine_Window);
use_Opt$(engine_Window);
use_Err$(engine_Window);

/* Window management */

extern Err$Ptr$engine_Window engine_Window_init(const engine_WindowConfig* config) must_check;
extern void                  engine_Window_fini(engine_Window* self);
extern Err$void              engine_Window_update(engine_Window* self) must_check;
extern void                  engine_Window_present(engine_Window* self);

/* Canvas view management */

extern Opt$u32 engine_Window_appendCanvasView(engine_Window* self, engine_Canvas* canvas, Vec2u pos, Vec2u size, Vec2f scale, bool visible, bool resizable_x, bool resizable_y);
extern void    engine_Window_removeCanvasView(engine_Window* self, u32 view_id);

/* Window metrics queries */

/// Returns position: top-left corner on display (ex: (10,10) pixels))
extern Vec2u engine_Window_getPos(const engine_Window* self);
/// Returns dimensions: physical display size (ex: 800x500 pixels))
extern Vec2u engine_Window_getDim(const engine_Window* self);
/// Returns resolution: logical grid size (ex: 80x50 units))
extern Vec2u engine_Window_getRes(const engine_Window* self);

extern Vec2u    engine_Window_getMinRes(const engine_Window* self);
extern Vec2u    engine_Window_getMaxRes(const engine_Window* self);
extern Err$void engine_Window_setMinRes(engine_Window* self, Vec2u size) must_check;
extern Err$void engine_Window_setMaxRes(engine_Window* self, Vec2u size) must_check;

extern bool engine_Window_isFocused(const engine_Window* self);
extern bool engine_Window_isMinimized(const engine_Window* self);
extern bool engine_Window_isMaximized(const engine_Window* self);

#endif /* ENGINE_WINDOW_INCLUDED */
