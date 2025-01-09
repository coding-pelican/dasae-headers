#ifndef ENGINE_WORLD_INCLUDED
#define ENGINE_WORLD_INCLUDED (1)

#include "common.h"

typedef struct engine_World       engine_World;
typedef struct engine_WorldObject engine_WorldObject;

struct engine_World {
    engine_WorldObject* objects;
    u32                 object_count;
    Vec3f               world_bounds;
};
using_Ptr$(engine_World);
using_Err$(engine_World);

struct engine_WorldObject {
    engine_Transform    transform;
    anyptr              components; /* Flexible component system */
    engine_World*       world;      /* Pointer to world this object belongs to */
    engine_WorldObject* next;       /* Linked list of game objects */
};
using_Ptr$(engine_WorldObject);
using_Err$(engine_WorldObject);

// Game World functions
extern Err$Ptr$engine_World       engine_World_create(Vec3f bounds) must_check;
extern void                       engine_World_destroy(engine_World* self);
extern Err$Ptr$engine_WorldObject engine_World_spawnObject(engine_World* self, Vec3f position) must_check;
extern void                       engine_World_update(engine_World* self, f32 delta_time);

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

#endif /* ENGINE_WORLD_INCLUDED */
