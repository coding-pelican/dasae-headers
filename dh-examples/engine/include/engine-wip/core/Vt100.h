#ifndef ENGINE_CORE_VT100_INCLUDED
#define ENGINE_CORE_VT100_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/Window.h"
#include "engine-wip/Backend.h"

typedef struct engine_core_Vt100_Config {
    engine_Window* window;
    mem_Allocator  allocator;
} engine_core_Vt100_Config;

typedef struct engine_core_Vt100 engine_core_Vt100;
extern engine_Backend            engine_core_Vt100_backend(void);
extern Err$void                  engine_core_Vt100_init(engine_Backend self, const engine_core_Vt100_Config* config) must_check;
extern void                      engine_Core_Vt100_fini(engine_Backend self);

#endif /* ENGINE_CORE_VT100_INCLUDED */
