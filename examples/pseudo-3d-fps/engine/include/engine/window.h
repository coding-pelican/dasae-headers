#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)

#include "canvas.h"

#define engine_Window_max_canvases (8)
typedef struct engine_Window     engine_Window;
typedef struct engine_CanvasView engine_CanvasView;

struct engine_CanvasView {
    i32            x, y;          // Position in window coordinates
    i32            width, height; // Size in window coordinates
    engine_Canvas* canvas;        // Associated canvas
    bool           visible;       // Visibility flag
};
using_Ptr$(engine_CanvasView);
using_Err$(engine_CanvasView);

struct engine_Window {
    engine_Platform*  platform;
    u32               width;
    u32               height;
    engine_CanvasView views[engine_Window_max_canvases];
    u32               view_count;
    engine_Canvas*    composite_buffer; // Final composition buffer
};
using_Ptr$(engine_Window);
using_Err$(engine_Window);

// Window management
extern Err$Ptr$engine_Window engine_Window_create(const engine_PlatformParams* params) must_check;
extern void                  engine_Window_destroy(engine_Window* window);
extern void                  engine_Window_processEvents(engine_Window* window);
extern void                  engine_Window_present(engine_Window* window);

// Canvas view management
extern i32  engine_Window_addCanvasView(engine_Window* window, engine_Canvas* canvas, i32 x, i32 y, i32 width, i32 height);
extern void engine_Window_removeCanvasView(engine_Window* window, i32 view_id);
extern void engine_Window_updateCanvasView(engine_Window* window, i32 view_id, i32 x, i32 y, i32 width, i32 height);

#endif /* ENGINE_WINDOW_INCLUDED */
