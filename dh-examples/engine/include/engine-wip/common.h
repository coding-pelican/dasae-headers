#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/log.h"

#include "dh/scope.h"
#include "dh/opt.h"
#include "dh/err_res.h"

#include "dh/mem/Allocator.h"
#include "dh/meta/common.h"

#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/Grid.h"

#include "dh/math/common.h"
#include "dh/math/vec.h"

#include "dh/time.h"

// clang-format off
typedef math_Vec2f  Vec2f;  typedef math_Vec3f  Vec3f;  typedef math_Vec4f  Vec4f;
typedef math_Vec2d  Vec2d;  typedef math_Vec3d  Vec3d;  typedef math_Vec4d  Vec4d;

typedef math_Vec2i  Vec2i;  typedef math_Vec3i  Vec3i;  typedef math_Vec4i  Vec4i;
typedef math_Vec2l  Vec2l;  typedef math_Vec3l  Vec3l;  typedef math_Vec4l  Vec4l;
typedef math_Vec2z  Vec2z;  typedef math_Vec3z  Vec3z;  typedef math_Vec4z  Vec4z;

typedef math_Vec2u  Vec2u;  typedef math_Vec3u  Vec3u;  typedef math_Vec4u  Vec4u;
typedef math_Vec2ul Vec2ul; typedef math_Vec3ul Vec3ul; typedef math_Vec4ul Vec4ul;
typedef math_Vec2uz Vec2uz; typedef math_Vec3uz Vec3uz; typedef math_Vec4uz Vec4uz;

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

#endif /* ENGINE_COMMON_INCLUDED */
