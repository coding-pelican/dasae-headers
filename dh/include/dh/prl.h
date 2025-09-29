/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prl.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Prelude for dh
 * @details Provides a prelude for dh.
 */

#ifndef PRL_INCLUDED
#define PRL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/scope.h"
#include "dh/blk.h"
#include "dh/fn.h"
#include "dh/callback.h"
#include "dh/pipe.h"

#include "dh/Range.h"
#include "dh/ptr.h"
#include "dh/sli.h"
#include "dh/Arr.h"

#include "dh/variant.h"
#include "dh/opt.h"
#include "dh/Err.h"
#include "dh/err_res.h"
#include "dh/ErrTrace.h"

/*========== Macros and Declarations ========================================*/

// clang-format off
#define useT$(_T) \
    use_Ptr$(_T); use_Sli$(_T); \
    use_Ptr$(Ptr_const$(_T)); use_Ptr$(Ptr$(_T)); \
    use_Sli$(Ptr_const$(_T)); use_Sli$(Ptr$(_T)); \
    use_Ptr$(Sli_const$(_T)); use_Ptr$(Sli$(_T)); \
    use_Sli$(Sli_const$(_T)); use_Sli$(Sli$(_T)); \
    use_Opt$(_T); \
    use_Opt$(Sli_const$(_T)); use_Opt$(Sli$(_T)); \
    use_Ptr$(Opt$(_T)); use_Sli$(Opt$(_T)); \
    use_Ptr$(Opt$(Ptr_const$(_T))); use_Ptr$(Opt$(Ptr$(_T))); \
    use_Sli$(Opt$(Ptr_const$(_T))); use_Sli$(Opt$(Ptr$(_T))); \
    use_Ptr$(Opt$(Sli_const$(_T))); use_Ptr$(Opt$(Sli$(_T))); \
    use_Sli$(Opt$(Sli_const$(_T))); use_Sli$(Opt$(Sli$(_T))); \
    use_Err$(_T); \
    use_Err$(Sli_const$(_T)); use_Err$(Sli$(_T)); \
    use_Ptr$(Err$(_T)); use_Sli$(Err$(_T)); \
    use_Ptr$(Err$(Ptr_const$(_T))); use_Ptr$(Err$(Ptr$(_T))); \
    use_Sli$(Err$(Ptr_const$(_T))); use_Sli$(Err$(Ptr$(_T))); \
    use_Ptr$(Err$(Sli_const$(_T))); use_Ptr$(Err$(Sli$(_T))); \
    use_Sli$(Err$(Sli_const$(_T))); use_Sli$(Err$(Sli$(_T)))
// clang-format on

/*========== Macros and Definitions =========================================*/

// clang-format off
useT$(u8); useT$(u16); useT$(u32); useT$(u64); useT$(usize);
useT$(i8); useT$(i16); useT$(i32); useT$(i64); useT$(isize);
useT$(f32); useT$(f64);
useT$(bool);
useT$(voidptr);
useT$(voidptr_const);
// clang-format on
#undef useT$

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRL_INCLUDED */
