#ifndef engine_core_VT100Ex__included
#define engine_core_VT100Ex__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/Window.h"
#include "engine/Input.h"
#include "engine/Backend.h"

typedef struct engine_core_VT100Ex_Config {
    var_(allocator, O$mem_Allocator);
    var_(window, engine_Window*);
    var_(input, engine_Input*);
    var_(stub_path, O$S_const$u8);  /* Path to stub executable */
    var_(stub_title, O$S_const$u8); /* Optional window title */
} engine_core_VT100Ex_Config;

typedef struct engine_core_VT100Ex engine_core_VT100Ex;
T_use_P$(engine_core_VT100Ex);
T_use_E$(P$engine_core_VT100Ex);

$must_check
$extern fn_((engine_core_VT100Ex_init(engine_core_VT100Ex_Config config))(E$P$engine_core_VT100Ex));
$extern fn_((engine_core_VT100Ex_fini(engine_core_VT100Ex* self))(void));
$extern fn_((engine_core_VT100Ex_backend(engine_core_VT100Ex* self))(engine_Backend));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine_core_VT100Ex__included */
