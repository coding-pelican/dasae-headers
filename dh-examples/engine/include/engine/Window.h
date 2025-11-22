#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/Color.h"
#include "engine/Canvas.h"

#include "engine/Backend.h"

#include "dh/ArrList.h"

typedef struct engine_Window engine_Window;
#define engine_Window_size_min_width_default (16)
#define engine_Window_size_min_height_default (16)
#define engine_Window_size_max_width_default (4096)
#define engine_Window_size_max_height_default (2048)
#define engine_Window_view_count_limit (8)
#define engine_Window_composite_buffer_color_default ((Color){ .r = 0x18, .g = 0x18, .b = 0x18, .a = 0xFF })

T_use_ArrList$(engine_CanvasView);
struct engine_Window {
    O$engine_Backend backend;
    O$S$engine_CanvasView views_buf;
    ArrList$engine_CanvasView views;
    engine_Canvas* composite_buffer; // Final composition buffer
    mem_Allocator gpa;
};
T_use_P$(engine_Window);
T_use_S$(engine_Window);
T_use_O$(engine_Window);
T_use_E$(P$engine_Window);

typedef struct engine_Window_Config {
    O$mem_Allocator gpa;
    O$S$engine_CanvasView views_buf;
    m_V2u32 rect_size;
    O$Color default_color;
    O$S_const$u8 title;
} engine_Window_Config;

/* Window management =========================================================*/

extern fn_((engine_Window_init(engine_Window_Config config))(E$P$engine_Window)) $must_check;
extern fn_((engine_Window_fini(engine_Window* self))(void));
extern fn_((engine_Window_update(engine_Window* self))(E$void)) $must_check;
extern fn_((engine_Window_present(engine_Window* self))(void));

/* Canvas view management ====================================================*/

extern fn_((engine_Window_appendView(engine_Window* self, engine_CanvasView_Config config))(O$u32));
extern fn_((engine_Window_removeView(engine_Window* self, u32 view_id))(void));

/* Window metrics queries ===================================================*/

/// Returns position: top-left corner on display (ex: (10,10) pixels))
extern fn_((engine_Window_getPos(const engine_Window* self))(m_V2i32));
/// Returns dimensions: physical display size (ex: 800x500 pixels))
extern fn_((engine_Window_getDim(const engine_Window* self))(m_V2u32));
/// Returns resolution: logical grid size (ex: 80x50 units))
extern fn_((engine_Window_getRes(const engine_Window* self))(m_V2u32));

extern fn_((engine_Window_getMinRes(const engine_Window* self))(m_V2u32));
extern fn_((engine_Window_getMaxRes(const engine_Window* self))(m_V2u32));
extern fn_((engine_Window_setMinRes(engine_Window* self, m_V2u32 size))(E$void)) $must_check;
extern fn_((engine_Window_setMaxRes(engine_Window* self, m_V2u32 size))(E$void)) $must_check;

extern fn_((engine_Window_isFocused(const engine_Window* self))(bool));
extern fn_((engine_Window_isMinimized(const engine_Window* self))(bool));
extern fn_((engine_Window_isMaximized(const engine_Window* self))(bool));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_WINDOW_INCLUDED */
