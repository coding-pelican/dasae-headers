#ifndef ENGINE_WINDOW_INCLUDED
#define ENGINE_WINDOW_INCLUDED (1)

#include "canvas.h"

#define engine_Window_max_canvases (8)
typedef struct engine_Window     engine_Window;
typedef struct engine_CanvasView engine_CanvasView;

struct engine_CanvasView {
    i32               x, y;          // Position in window coordinates
    i32               width, height; // Size in window coordinates
    Ptr_engine_Canvas canvas;        // Associated canvas
    bool              visible;       // Visibility flag
};
using_ExtTypes(engine_CanvasView);

struct engine_Window {
    Ptr_engine_Platform platform;
    u32                 width;
    u32                 height;
    engine_CanvasView   views[engine_Window_max_canvases];
    u32                 view_count;
    Ptr_engine_Canvas   composite_buffer; // Final composition buffer
};
using_ExtTypes(engine_Window);

// Window management
ResErr_Ptr_engine_Window engine_Window_create(PtrConst_engine_PlatformParams params);
void                     engine_Window_destroy(Ptr_engine_Window window);
void                     engine_Window_processEvents(Ptr_engine_Window window);
void                     engine_Window_present(Ptr_engine_Window window);

// Canvas view management
i32  engine_Window_addCanvasView(Ptr_engine_Window window, Ptr_engine_Canvas canvas, i32 x, i32 y, i32 width, i32 height);
void engine_Window_removeCanvasView(Ptr_engine_Window window, i32 view_id);
void engine_Window_updateCanvasView(Ptr_engine_Window window, i32 view_id, i32 x, i32 y, i32 width, i32 height);

#endif // ENGINE_WINDOW_INCLUDED
