#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED (1)

#include "common.h"

typedef struct engine_World       engine_World;
typedef struct engine_WorldObject engine_WorldObject;

struct engine_World {
    engine_WorldObject* objects;
    u32                 object_count;
    engine_Vec3f32      world_bounds;
};
using_ExtTypes(engine_World);

struct engine_WorldObject {
    engine_Transform    transform;
    anyptr              components; /* Flexible component system */
    Ptr_engine_World    world;      /* Pointer to world this object belongs to */
    engine_WorldObject* next;       /* Linked list of game objects */
};
using_ExtTypes(engine_WorldObject);

// Game World functions
ResErr_Ptr_engine_World       engine_World_create(engine_Vec3f32 bounds);
void                          engine_World_destroy(Ptr_engine_World world);
ResErr_Ptr_engine_WorldObject engine_World_spawnObject(Ptr_engine_World world, engine_Vec3f32 position);
void                          engine_World_update(Ptr_engine_World world, f32 delta_time);

#if 0
// Example main game loop
int main(int argc, char* argv[]) {
    // Initialize engine
    engine_Window*   window   = engine_Window_create("Game Engine", 800, 600);
    engine_Viewport* viewport = engine_Viewport_create(800, 600);
    engine_World*    world    = engine_World_create((Vector3){1000.0f, 1000.0f, 1000.0f});

    // Game loop
    while (1) {
        // Process input
        engine_Window_processEvents(window);

        // Update game state
        engine_World_update(world, 0.016f);  // Assuming 60 FPS

        // Render
        engine_Canvas_clear(window->canvas, (Color){0.0f, 0.0f, 0.0f, 1.0f});
        engine_Canvas_renderWorld(window->canvas, viewport, world);
        engine_Window_present(window);
    }

    // Cleanup
    engine_World_destroy(world);
    engine_Viewport_destroy(viewport);
    engine_Window_destroy(window);

    return 0;
}
#endif

#endif /* WORLD_INCLUDED */
