#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/core.h"
#include "dh/log.h"

#include "dh/scope.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/variant.h"

#include "dh/mem/Allocator.h"
#include "dh/meta/common.h"

#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/Grid.h"

#include "dh/math/common.h"
#include "dh/math/vec_types.h"
#include "dh/math/vec.h"

#include "dh/time.h"

// clang-format off
typedef m_V2f32  Vec2f;  typedef m_V3f32  Vec3f;  typedef m_V4f32  Vec4f;
typedef m_V2f64  Vec2d;  typedef m_V3f64  Vec3d;  typedef m_V4f64  Vec4d;

typedef m_V2i32  Vec2i;  typedef m_V3i32  Vec3i;  typedef m_V4i32  Vec4i;
typedef m_V2i64  Vec2l;  typedef m_V3i64  Vec3l;  typedef m_V4i64  Vec4l;
typedef m_V2z  Vec2z;  typedef m_V3z  Vec3z;  typedef m_V4z  Vec4z;

typedef m_V2u  Vec2u;  typedef m_V3u  Vec3u;  typedef m_V4u  Vec4u;
typedef m_V2ul Vec2ul; typedef m_V3ul Vec3ul; typedef m_V4ul Vec4ul;
typedef m_V2uz Vec2uz; typedef m_V3uz Vec3uz; typedef m_V4uz Vec4uz;

use_S$(Vec2f);  use_S$(Vec3f);  use_S$(Vec4f);
use_S$(Vec2d);  use_S$(Vec3d);  use_S$(Vec4d);

use_S$(Vec2i);  use_S$(Vec3i);  use_S$(Vec4i);
use_S$(Vec2l);  use_S$(Vec3l);  use_S$(Vec4l);
use_S$(Vec2z);  use_S$(Vec3z);  use_S$(Vec4z);

use_S$(Vec2u);  use_S$(Vec3u);  use_S$(Vec4u);
use_S$(Vec2ul); use_S$(Vec3ul); use_S$(Vec4ul);
use_S$(Vec2uz); use_S$(Vec3uz); use_S$(Vec4uz);

use_O$(Vec2f);  use_O$(Vec3f);  use_O$(Vec4f);
use_O$(Vec2d);  use_O$(Vec3d);  use_O$(Vec4d);

use_O$(Vec2i);  use_O$(Vec3i);  use_O$(Vec4i);
use_O$(Vec2l);  use_O$(Vec3l);  use_O$(Vec4l);
use_O$(Vec2z);  use_O$(Vec3z);  use_O$(Vec4z);

use_O$(Vec2u);  use_O$(Vec3u);  use_O$(Vec4u);
use_O$(Vec2ul); use_O$(Vec3ul); use_O$(Vec4ul);
use_O$(Vec2uz); use_O$(Vec3uz); use_O$(Vec4uz);

use_E$(Vec2f);  use_E$(Vec3f);  use_E$(Vec4f);
use_E$(Vec2d);  use_E$(Vec3d);  use_E$(Vec4d);

use_E$(Vec2i);  use_E$(Vec3i);  use_E$(Vec4i);
use_E$(Vec2l);  use_E$(Vec3l);  use_E$(Vec4l);
use_E$(Vec2z);  use_E$(Vec3z);  use_E$(Vec4z);

use_E$(Vec2u);  use_E$(Vec3u);  use_E$(Vec4u);
use_E$(Vec2ul); use_E$(Vec3ul); use_E$(Vec4ul);
use_E$(Vec2uz); use_E$(Vec3uz); use_E$(Vec4uz);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_COMMON_INCLUDED */
