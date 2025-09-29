#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/Color.h"
#include "engine/Canvas.h"

#include "engine/Backend.h"

typedef struct engine_Window engine_Window;
#define engine_Window_size_min_width_default         (16)
#define engine_Window_size_min_height_default        (16)
#define engine_Window_size_max_width_default         (4096)
#define engine_Window_size_max_height_default        (2048)
#define engine_Window_view_count_limit               (8)
#define engine_Window_composite_buffer_color_default ((Color){ .r = 0x18, .g = 0x18, .b = 0x18, .a = 0xFF })

struct engine_Window {
    Opt$engine_Backend backend;
    struct {
        var_(list, Arr$$(engine_Window_view_count_limit, engine_CanvasView));
        var_(count, u32);
    } view;
    engine_Canvas* composite_buffer; // Final composition buffer
    mem_Allocator  allocator;
};
use_Ptr$(engine_Window);
use_Sli$(engine_Window);
use_Opt$(engine_Window);
use_Err$(engine_Window);

typedef struct engine_Window_Config {
    Opt$mem_Allocator allocator;
    Vec2u             rect_size;
    Opt$Color         default_color;
    Opt$Sli_const$u8  title;
} engine_Window_Config;

/* Window management =========================================================*/

extern fn_(engine_Window_init(const engine_Window_Config* config), Err$Ptr$engine_Window) $must_check;
extern fn_(engine_Window_fini(engine_Window* self), void);
extern fn_(engine_Window_update(engine_Window* self), Err$void) $must_check;
extern fn_(engine_Window_present(engine_Window* self), void);

/* Canvas view management ====================================================*/

extern fn_(engine_Window_appendView(engine_Window* self, const engine_CanvasView_Config* config), Opt$u32);
extern fn_(engine_Window_removeView(engine_Window* self, u32 view_id), void);

/* Window metrics queries ===================================================*/

/// Returns position: top-left corner on display (ex: (10,10) pixels))
extern fn_(engine_Window_getPos(const engine_Window* self), Vec2i);
/// Returns dimensions: physical display size (ex: 800x500 pixels))
extern fn_(engine_Window_getDim(const engine_Window* self), Vec2u);
/// Returns resolution: logical grid size (ex: 80x50 units))
extern fn_(engine_Window_getRes(const engine_Window* self), Vec2u);

extern fn_(engine_Window_getMinRes(const engine_Window* self), Vec2u);
extern fn_(engine_Window_getMaxRes(const engine_Window* self), Vec2u);
extern fn_(engine_Window_setMinRes(engine_Window* self, Vec2u size), Err$void) $must_check;
extern fn_(engine_Window_setMaxRes(engine_Window* self, Vec2u size), Err$void) $must_check;

extern fn_(engine_Window_isFocused(const engine_Window* self), bool);
extern fn_(engine_Window_isMinimized(const engine_Window* self), bool);
extern fn_(engine_Window_isMaximized(const engine_Window* self), bool);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_WINDOW_INCLUDED */
