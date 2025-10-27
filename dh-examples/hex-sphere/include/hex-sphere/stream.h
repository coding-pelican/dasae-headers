#ifndef hex_sphere_stream_included
#define hex_sphere_stream_included (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/prl.h"

#if bti_plat_windows
#define stream_fastOut__comp_windows_allow_ansi (1)
#else /* bti_plat_unix */
#define stream_fastOut__comp_unix_allow_bulk (1)
#endif

extern func((stream_fastOut(S_const$u8 data))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* hex_sphere_stream_included */
