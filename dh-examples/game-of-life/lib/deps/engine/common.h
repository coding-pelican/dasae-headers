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

use_Sli$(Vec2f);  use_Sli$(Vec3f);  use_Sli$(Vec4f);
use_Sli$(Vec2d);  use_Sli$(Vec3d);  use_Sli$(Vec4d);

use_Sli$(Vec2i);  use_Sli$(Vec3i);  use_Sli$(Vec4i);
use_Sli$(Vec2l);  use_Sli$(Vec3l);  use_Sli$(Vec4l);
use_Sli$(Vec2z);  use_Sli$(Vec3z);  use_Sli$(Vec4z);

use_Sli$(Vec2u);  use_Sli$(Vec3u);  use_Sli$(Vec4u);
use_Sli$(Vec2ul); use_Sli$(Vec3ul); use_Sli$(Vec4ul);
use_Sli$(Vec2uz); use_Sli$(Vec3uz); use_Sli$(Vec4uz);

use_Opt$(Vec2f);  use_Opt$(Vec3f);  use_Opt$(Vec4f);
use_Opt$(Vec2d);  use_Opt$(Vec3d);  use_Opt$(Vec4d);

use_Opt$(Vec2i);  use_Opt$(Vec3i);  use_Opt$(Vec4i);
use_Opt$(Vec2l);  use_Opt$(Vec3l);  use_Opt$(Vec4l);
use_Opt$(Vec2z);  use_Opt$(Vec3z);  use_Opt$(Vec4z);

use_Opt$(Vec2u);  use_Opt$(Vec3u);  use_Opt$(Vec4u);
use_Opt$(Vec2ul); use_Opt$(Vec3ul); use_Opt$(Vec4ul);
use_Opt$(Vec2uz); use_Opt$(Vec3uz); use_Opt$(Vec4uz);

use_Err$(Vec2f);  use_Err$(Vec3f);  use_Err$(Vec4f);
use_Err$(Vec2d);  use_Err$(Vec3d);  use_Err$(Vec4d);

use_Err$(Vec2i);  use_Err$(Vec3i);  use_Err$(Vec4i);
use_Err$(Vec2l);  use_Err$(Vec3l);  use_Err$(Vec4l);
use_Err$(Vec2z);  use_Err$(Vec3z);  use_Err$(Vec4z);

use_Err$(Vec2u);  use_Err$(Vec3u);  use_Err$(Vec4u);
use_Err$(Vec2ul); use_Err$(Vec3ul); use_Err$(Vec4ul);
use_Err$(Vec2uz); use_Err$(Vec3uz); use_Err$(Vec4uz);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_COMMON_INCLUDED */
