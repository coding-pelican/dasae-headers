#ifndef engine_core_VT100__included
#define engine_core_VT100__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"
#include "engine/Window.h"
#include "engine/Input.h"
#include "engine/Backend.h"

typedef struct engine_core_VT100_Config {
    var_(gpa, O$mem_Allocator);
    var_(window, engine_Window*); /* TODO: Supports multi-windows */
    var_(input, engine_Input*);
} engine_core_VT100_Config;

typedef struct engine_core_VT100 engine_core_VT100;
T_use_P$(engine_core_VT100);
T_use_E$(P$engine_core_VT100);

$must_check
$extern fn_((engine_core_VT100_init(engine_core_VT100_Config config))(E$P$engine_core_VT100));
$extern fn_((engine_core_VT100_fini(engine_core_VT100* self))(void));
$extern fn_((engine_core_VT100_backend(engine_core_VT100* self))(engine_Backend));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine_core_VT100__included */
