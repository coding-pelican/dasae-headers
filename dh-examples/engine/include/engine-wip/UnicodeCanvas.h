#ifndef ENGINE_UNICODE_CANVAS_INCLUDED
#define ENGINE_UNICODE_CANVAS_INCLUDED (1)

#include "Canvas.h"

use_Mat$(u8);
typedef struct engine_UnicodeCanvas {
    engine_ColorCanvas color_canvas;
    Mat$u8             unicode_buffer;
} engine_UnicodeCanvas;
use_Sli$(engine_UnicodeCanvas);
use_Opt$(engine_UnicodeCanvas);
use_Err$(engine_UnicodeCanvas);

extern Err$Ptr$engine_UnicodeCanvas engine_UnicodeCanvas_init(mem_Allocator allocator, u32 width, u32 height) must_check;
extern Err$Ptr$engine_UnicodeCanvas engine_UnicodeCanvas_initWithDefault(mem_Allocator allocator, u32 width, u32 height, Color default_color, Sli$u8 default_typo) must_check;
extern void                         engine_UnicodeCanvas_fini(engine_Canvas* self);
extern Err$void                     engine_UnicodeCanvas_resize(engine_Canvas* self, u32 width, u32 height) must_check;
extern void                         engine_UnicodeCanvas_clear(engine_Canvas* self, Color color, Sli$u8 typo);
extern void                         engine_UnicodeCanvas_clearDefault(engine_Canvas* self);
extern void                         engine_UnicodeCanvas_printTypo(engine_Canvas* self, i32 x, i32 y, Color color, Sli$u8 typo);
extern void                         engine_UnicodeCanvas_printText(engine_Canvas* self, i32 x, i32 y, Color color, Sli$u8 text);

#endif /* ENGINE_UNICODE_CANVAS_INCLUDED */
