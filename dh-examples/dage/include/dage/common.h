#ifndef dage_common__included
#define dage_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/prl.h"
#include "dh/log.h"
#include "dh/mem/Allocator.h"

#include "dh/math/common.h"
#include "dh/math/vec_types.h"
#include "dh/math/vec.h"
#include "dh/math/mat_types.h"
#include "dh/math/mat.h"
#include "dh/math/quat_types.h"
#include "dh/math/quat.h"

/* clang-format off */
T_use_prl$(m_V2f32);   T_use_prl$(m_V3f32);   T_use_prl$(m_V4f32);
T_use_prl$(m_V2f64);   T_use_prl$(m_V3f64);   T_use_prl$(m_V4f64);

T_use_prl$(m_V2i32);   T_use_prl$(m_V3i32);   T_use_prl$(m_V4i32);
T_use_prl$(m_V2i64);   T_use_prl$(m_V3i64);   T_use_prl$(m_V4i64);
T_use_prl$(m_V2isize); T_use_prl$(m_V3isize); T_use_prl$(m_V4isize);

T_use_prl$(m_V2u32);   T_use_prl$(m_V3u32);   T_use_prl$(m_V4u32);
T_use_prl$(m_V2u64);   T_use_prl$(m_V3u64);   T_use_prl$(m_V4u64);
T_use_prl$(m_V2usize); T_use_prl$(m_V3usize); T_use_prl$(m_V4usize);
/* clang-format on */

#include "dh/time.h"

#include "Grid.h"
#include "color.h"
T_use_Grid$(color_RGBA);
T_use_Grid$(color_RGB);
T_use_Grid$(color_HSL);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_common__included */
