// #ifndef ENGINE_COMMON_INCLUDED
// #define ENGINE_COMMON_INCLUDED (1)

// #include "dh/core/prim.h"
// #include "dh/core/pp.h"
// #include "dh/claim.h"
// #include "dh/debug.h"
// #include "dh/extensions.h"
// #include "dh/Str.h"

// typedef union engine_Vec3f32 {
//     f32 xyz[3];
//     struct {
//         f32 x;
//         f32 y;
//         f32 z;
//     };
// } engine_Vec3f32;
// using_ExtTypes(engine_Vec3f32);

// typedef struct engine_Transform {
//     engine_Vec3f32 position;
//     engine_Vec3f32 rotation;
//     engine_Vec3f32 scale;
// } engine_Transform;
// using_ExtTypes(engine_Transform);

// #endif /* ENGINE_COMMON_INCLUDED */

// #ifndef WORLD_INCLUDED
// #define WORLD_INCLUDED (1)

// #include "common.h"

// typedef struct engine_World       engine_World;
// typedef struct engine_WorldObject engine_WorldObject;

// struct engine_World {
//     engine_WorldObject* objects;
//     u32                 object_count;
//     engine_Vec3f32      world_bounds;
// };
// using_ExtTypes(engine_World);

// struct engine_WorldObject {
//     engine_Transform    transform;
//     anyptr              components; /* Flexible component system */
//     Ptr_engine_World    world;      /* Pointer to world this object belongs to */
//     engine_WorldObject* next;       /* Linked list of game objects */
// };
// using_ExtTypes(engine_WorldObject);

// // Game World functions
// ResErr_Ptr_engine_World       engine_World_create(engine_Vec3f32 bounds);
// void                          engine_World_destroy(Ptr_engine_World world);
// ResErr_Ptr_engine_WorldObject engine_World_spawnObject(Ptr_engine_World world, engine_Vec3f32 position);
// void                          engine_World_update(Ptr_engine_World world, f32 delta_time);

// #if 0
// // Example main game loop
// int main(int argc, char* argv[]) {
//     // Initialize engine
//     engine_Window*   window   = engine_Window_create("Game Engine", 800, 600);
//     engine_Viewport* viewport = engine_Viewport_create(800, 600);
//     engine_World*    world    = engine_World_create((Vector3){1000.0f, 1000.0f, 1000.0f});

//     // Game loop
//     while (1) {
//         // Process input
//         engine_Window_processEvents(window);

//         // Update game state
//         engine_World_update(world, 0.016f);  // Assuming 60 FPS

//         // Render
//         engine_Canvas_clear(window->canvas, (Color){0.0f, 0.0f, 0.0f, 1.0f});
//         engine_Canvas_renderWorld(window->canvas, viewport, world);
//         engine_Window_present(window);
//     }

//     // Cleanup
//     engine_World_destroy(world);
//     engine_Viewport_destroy(viewport);
//     engine_Window_destroy(window);

//     return 0;
// }
// #endif

// #endif /* WORLD_INCLUDED */

// #ifndef ENGINE_PLATFORM_INCLUDED
// #define ENGINE_PLATFORM_INCLUDED (1)

// #include "common.h"

// // engine_Platform-specific rendering backend types
// typedef enum engine_RenderBackendType {
//     engine_RenderBackendType_vt100,
//     engine_RenderBackendType_win32_gdi,
//     engine_RenderBackendType_xwindow,
//     engine_RenderBackendType_directx,
//     engine_RenderBackendType_opengl,
//     engine_RenderBackendType_vulkan
// } engine_RenderBackendType;

// typedef union engine_ColorRgb {
//     u8 channels[3]; // Rgb channel components in [0,255]
//     struct {
//         u8 r; // Red channel in [0,255]
//         u8 g; // Green channel in [0,255]
//         u8 b; // Blue channel in [0,255]
//     };
// } engine_ColorRgb;

// typedef union engine_ColorRgba {
//     u8 channels[4]; // RGBA channel components in [0,255]
//     struct {
//         union {
//             engine_ColorRgb rgb; // RGB channel components in [0,255]
//             struct {
//                 u8 r; // Red channel in [0,255]
//                 u8 g; // Green channel in [0,255]
//                 u8 b; // Blue channel in [0,255]
//             };
//         };
//         u8 a; // Alpha channel in [0,255]
//     };
//     u32 packed;
// } engine_ColorRgba;

// typedef struct engine_ColorIndexed {
//     u8 ascii_char;
//     u8 index; // Index in color palette
// } engine_ColorIndexed;

// // Abstract color type that can be interpreted differently by different backends
// typedef union engine_ColorValue {
//     engine_ColorRgba    rgba;
//     engine_ColorIndexed indexed;
// } engine_ColorValue;

// using_ExtTypes(voidptr);
// // engine_Platform-specific initialization parameters
// typedef struct engine_PlatformParams {
//     engine_RenderBackendType backend_type;
//     Opt_voidptr              native_window_handle; // Optional, platform-specific
//     StrConst                 window_title;
//     u32                      width;
//     u32                      height;
// } engine_PlatformParams;
// using_ExtTypes(engine_PlatformParams);

// // Forward declarations
// typedef struct engine_Platform      engine_Platform;
// typedef struct engine_RenderBackend engine_RenderBackend;

// // engine_Platform interface
// struct engine_Platform {
//     engine_RenderBackend* backend;
//     void (*destroy)(engine_Platform* platform);
//     void (*process_events)(engine_Platform* platform);
//     void (*present_buffer)(engine_Platform* platform, const anyptr buffer, u32 width, u32 height);
// };
// using_ExtTypes(engine_Platform);

// // Create platform instance with specific backend
// Ptr_engine_Platform engine_Platform_create(PtrConst_engine_PlatformParams params);

// #endif // ENGINE_PLATFORM_INCLUDED

// #ifndef ENGINE_VIEWPORT_INCLUDED
// #define ENGINE_VIEWPORT_INCLUDED (1)

// #include "world.h"

// typedef struct engine_Viewport engine_Viewport;
// typedef struct engine_Camera   engine_Camera;

// typedef struct engine_Camera {
//     engine_Vec3f32 position;
//     engine_Vec3f32 rotation;
//     f32            field_of_view;
//     f32            near_plane;
//     f32            far_plane;
// } engine_Camera;
// using_ExtTypes(engine_Camera);

// typedef struct engine_Viewport {
//     f32           width;
//     f32           height;
//     engine_Camera camera;
//     f32*          depth_buffer;
// } engine_Viewport;
// using_ExtTypes(engine_Viewport);

// // Viewport and Camera functions
// Ptr_engine_Viewport engine_Viewport_create(f32 width, f32 height);
// void                engine_Viewport_destroy(Ptr_engine_Viewport viewport);
// void                engine_Viewport_resize(Ptr_engine_Viewport viewport, f32 width, f32 height);
// void                engine_Camera_setPosition(Ptr_engine_Camera camera, engine_Vec3f32 position);
// void                engine_Camera_lookAt(Ptr_engine_Camera camera, engine_Vec3f32 target);

// #endif // ENGINE_VIEWPORT_INCLUDED

// #ifndef ENGINE_CANVAS_INCLUDED
// #define ENGINE_CANVAS_INCLUDED (1)

// #include "platform.h"
// #include "viewport.h"

// typedef enum engine_CanvasType {
//     engine_CanvasType_rgba,
//     engine_CanvasType_indexed8,
//     engine_CanvasType_indexed16,
//     engine_CanvasType_indexed256
// } engine_CanvasType;

// typedef struct engine_Canvas {
//     u32               width;
//     u32               height;
//     engine_CanvasType type;
//     anyptr            buffer; // Generic buffer pointer
//     u32               buffer_size;

//     // Canvas-specific conversion functions
//     engine_ColorValue (*pixel_to_color)(const anyptr buffer, u32 x, u32 y);
//     void (*color_to_pixel)(anyptr buffer, u32 x, u32 y, engine_ColorValue color);
// } engine_Canvas;
// using_ExtTypes(engine_Canvas);

// // Canvas creation with specific type
// ResErr_Ptr_engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type);
// void                     engine_Canvas_destroy(Ptr_engine_Canvas canvas);
// void                     engine_Canvas_clear(Ptr_engine_Canvas canvas, engine_ColorValue color);
// void                     engine_Canvas_drawPixel(Ptr_engine_Canvas canvas, i32 x, i32 y, engine_ColorValue color);
// void                     engine_Canvas_resize(Ptr_engine_Canvas canvas, u32 width, u32 height);

// // Specialized canvas operations
// void engine_Canvas_blit(Ptr_engine_Canvas dst, const Ptr_engine_Canvas src, i32 x, i32 y);
// void engine_Canvas_blitScaled(Ptr_engine_Canvas dst, const Ptr_engine_Canvas src, i32 x, i32 y, f32 scale);

// #endif // ENGINE_CANVAS_INCLUDED

// #ifndef ENGINE_WINDOW_INCLUDED
// #define ENGINE_WINDOW_INCLUDED (1)

// #include "canvas.h"

// #define engine_Window_max_canvases (8)
// typedef struct engine_Window     engine_Window;
// typedef struct engine_CanvasView engine_CanvasView;

// struct engine_CanvasView {
//     i32               x, y;          // Position in window coordinates
//     i32               width, height; // Size in window coordinates
//     Ptr_engine_Canvas canvas;        // Associated canvas
//     bool              visible;       // Visibility flag
// };
// using_ExtTypes(engine_CanvasView);

// struct engine_Window {
//     Ptr_engine_Platform platform;
//     u32                 width;
//     u32                 height;
//     engine_CanvasView   views[engine_Window_max_canvases];
//     u32                 view_count;
//     Ptr_engine_Canvas   composite_buffer; // Final composition buffer
// };
// using_ExtTypes(engine_Window);

// // Window management
// ResErr_Ptr_engine_Window engine_Window_create(PtrConst_engine_PlatformParams params);
// void                     engine_Window_destroy(Ptr_engine_Window window);
// void                     engine_Window_processEvents(Ptr_engine_Window window);
// void                     engine_Window_present(Ptr_engine_Window window);

// // Canvas view management
// i32  engine_Window_addCanvasView(Ptr_engine_Window window, Ptr_engine_Canvas canvas, i32 x, i32 y, i32 width, i32 height);
// void engine_Window_removeCanvasView(Ptr_engine_Window window, i32 view_id);
// void engine_Window_updateCanvasView(Ptr_engine_Window window, i32 view_id, i32 x, i32 y, i32 width, i32 height);

// #endif // ENGINE_WINDOW_INCLUDED
