#ifndef ENGINE_UNICODE_CANVAS_INCLUDED
#define ENGINE_UNICODE_CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "Canvas.h"

use_Grid$(u8);
use_Opt$(Sli$u8);
typedef struct engine_UnicodeCanvas_Config {
    mem_Allocator allocator;
    u32           width;
    u32           height;
    Opt$Color     default_color;
    Opt$Sli$u8    default_typo;
} engine_UnicodeCanvas_Config;

typedef struct engine_UnicodeCanvas {
    engine_Canvas color_buffer;
    Grid$u8       unicode_buffer;
} engine_UnicodeCanvas;
use_Ptr$(engine_UnicodeCanvas);
use_Sli$(engine_UnicodeCanvas);
use_Opt$(engine_UnicodeCanvas);
use_Err$(engine_UnicodeCanvas);

extern fn_(engine_UnicodeCanvas_init(const engine_UnicodeCanvas_Config* config), Err$Ptr$engine_UnicodeCanvas) $must_check;
extern fn_(engine_UnicodeCanvas_fini(engine_UnicodeCanvas* self), void);
extern fn_(engine_UnicodeCanvas_resize(engine_UnicodeCanvas* self, u32 width, u32 height), Err$void) $must_check;
extern fn_(engine_UnicodeCanvas_clear(engine_UnicodeCanvas* self, Opt$Color color, Opt$Sli$u8 typo), void);

extern fn_(engine_UnicodeCanvas_printTypo(engine_UnicodeCanvas* self, i32 x, i32 y, Color color, Sli$u8 typo), void);
extern fn_(engine_UnicodeCanvas_printText(engine_UnicodeCanvas* self, i32 x, i32 y, Color color, Sli$u8 text), void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_UNICODE_CANVAS_INCLUDED */
