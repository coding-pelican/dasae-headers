// #include "include/engine/window.h"

// // Example usage
// int main(void) {
//     // Initialize with OpenGL backend
//     engine_PlatformParams params = {
//         .backend_type = engine_RenderBackendType_opengl,
//         .window_title = strL("Example"),
//         .width        = 800,
//         .height       = 600
//     };

//     engine_Window* window = engine_Window_create(&params);

//     // Create main game canvas (RGBA)
//     engine_Canvas* game_canvas = engine_Canvas_create(800, 600, engine_CanvasType_rgba);
//     i32            main_view   = engine_Window_addCanvasView(window, game_canvas, 0, 0, 800, 600);
//     unused(main_view);

//     // Create UI overlay canvas (RGBA with alpha)
//     engine_Canvas* ui_canvas = engine_Canvas_create(800, 600, engine_CanvasType_rgba);
//     i32            ui_view   = engine_Window_addCanvasView(window, ui_canvas, 0, 0, 800, 600);
//     unused(ui_view);

//     // Create minimap canvas (could be 8-bit indexed color for efficiency)
//     engine_Canvas* minimap_canvas = engine_Canvas_create(100, 100, engine_CanvasType_indexed8);
//     i32            minimap_view   = engine_Window_addCanvasView(window, minimap_canvas, 650, 50, 100, 100);
//     unused(minimap_view);

//     // Game loop
//     while (1) {
//         engine_Window_processEvents(window);

//         // Update and render game world to game_canvas
//         // Update and render UI to ui_canvas
//         // Update and render minimap to minimap_canvas

//         engine_Window_present(window);
//     }

//     // Cleanup
//     engine_Canvas_destroy(minimap_canvas);
//     engine_Canvas_destroy(ui_canvas);
//     engine_Canvas_destroy(game_canvas);
//     engine_Window_destroy(window);

//     return 0;
// }
