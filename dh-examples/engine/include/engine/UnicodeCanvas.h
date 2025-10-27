#ifndef ENGINE_UNICODE_CANVAS_INCLUDED
#define ENGINE_UNICODE_CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "Canvas.h"

use_Grid$(u8);
typedef struct engine_UnicodeCanvas_Config {
    mem_Allocator allocator;
    u32           width;
    u32           height;
    O$Color     default_color;
    O$S$u8    default_typo;
} engine_UnicodeCanvas_Config;

typedef struct engine_UnicodeCanvas {
    engine_Canvas color_buffer;
    Grid$u8       unicode_buffer;
} engine_UnicodeCanvas;
use_P$(engine_UnicodeCanvas);
use_S$(engine_UnicodeCanvas);
use_O$(engine_UnicodeCanvas);
use_E$(engine_UnicodeCanvas);

extern fn_((engine_UnicodeCanvas_init(engine_UnicodeCanvas_Config config))(E$P$engine_UnicodeCanvas)) $must_check;
extern fn_((engine_UnicodeCanvas_fini(engine_UnicodeCanvas* self))(void));
extern fn_((engine_UnicodeCanvas_resize(engine_UnicodeCanvas* self, u32 width, u32 height))(E$void)) $must_check;
extern fn_((engine_UnicodeCanvas_clear(engine_UnicodeCanvas* self, O$Color color, O$S$u8 typo))(void));

extern fn_((engine_UnicodeCanvas_printTypo(engine_UnicodeCanvas* self, i32 x, i32 y, Color color, S$u8 typo))(void));
extern fn_((engine_UnicodeCanvas_printText(engine_UnicodeCanvas* self, i32 x, i32 y, Color color, S$u8 text))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_UNICODE_CANVAS_INCLUDED */
