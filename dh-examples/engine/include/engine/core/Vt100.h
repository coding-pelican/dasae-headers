#ifndef ENGINE_CORE_VT100_INCLUDED
#define ENGINE_CORE_VT100_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/Window.h"
#include "engine/Input.h"
#include "engine/Backend.h"

typedef struct engine_core_Vt100_Config {
    Opt$mem_Allocator allocator;
    engine_Window*    window; /* TODO: Supports multi-windows */
    engine_Input*     input;
} engine_core_Vt100_Config;

typedef struct engine_core_Vt100 engine_core_Vt100;
use_Ptr$(engine_core_Vt100);
use_Err$(Ptr$engine_core_Vt100);

extern fn_((engine_core_Vt100_init(engine_core_Vt100_Config config))(Err$Ptr$engine_core_Vt100)) $must_check;
extern fn_((engine_core_Vt100_fini(engine_core_Vt100* self))(void));
extern fn_((engine_core_Vt100_backend(engine_core_Vt100* self))(engine_Backend));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_CORE_VT100_INCLUDED */
