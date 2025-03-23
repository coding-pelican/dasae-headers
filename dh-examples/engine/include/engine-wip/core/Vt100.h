#ifndef ENGINE_CORE_VT100_INCLUDED
#define ENGINE_CORE_VT100_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/Window.h"
#include "engine-wip/Input.h"
#include "engine-wip/Backend.h"

typedef struct engine_core_Vt100_Config {
    mem_Allocator  allocator;
    engine_Window* window; /* TODO: Supports multi-windows */
    engine_Input*  input;
} engine_core_Vt100_Config;

typedef struct engine_core_Vt100 engine_core_Vt100;
use_Ptr(engine_core_Vt100);
use_Err$(Ptr$engine_core_Vt100);

extern Err$Ptr$engine_core_Vt100 engine_core_Vt100_init(const engine_core_Vt100_Config* config) must_check;
extern void                      engine_core_Vt100_fini(engine_core_Vt100* self);
extern engine_Backend            engine_core_Vt100_backend(engine_core_Vt100* self);

#endif /* ENGINE_CORE_VT100_INCLUDED */
