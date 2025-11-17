#ifndef engine_common__included
#define engine_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/prl.h"
#include "dh/log.h"
#include "dh/mem/Allocator.h"

#include "dh/math/common.h"
#include "dh/math/vec_types.h"
#include "dh/math/vec.h"

#include "dh/time.h"
#include "Grid.h"

/* clang-format off */
prl_T_use$(m_V2f32);   prl_T_use$(m_V3f32);   prl_T_use$(m_V4f32);
prl_T_use$(m_V2f64);   prl_T_use$(m_V3f64);   prl_T_use$(m_V4f64);

prl_T_use$(m_V2i32);   prl_T_use$(m_V3i32);   prl_T_use$(m_V4i32);
prl_T_use$(m_V2i64);   prl_T_use$(m_V3i64);   prl_T_use$(m_V4i64);
prl_T_use$(m_V2isize); prl_T_use$(m_V3isize); prl_T_use$(m_V4isize);

prl_T_use$(m_V2u32);   prl_T_use$(m_V3u32);   prl_T_use$(m_V4u32);
prl_T_use$(m_V2u64);   prl_T_use$(m_V3u64);   prl_T_use$(m_V4u64);
prl_T_use$(m_V2usize); prl_T_use$(m_V3usize); prl_T_use$(m_V4usize);
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* engine_common__included */
