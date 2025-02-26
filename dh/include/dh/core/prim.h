/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  prim
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PRIM_INCLUDED
#define CORE_PRIM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim/cfg.h"

#include "prim/bitwise.h"
#include "prim/bool.h"
#include "prim/int.h"
#include "prim/flt.h"
#include "prim/ptr.h"
#include "prim/null.h"
#include "prim/wchar.h"

#include "prim/struct.h"
#include "prim/union.h"
#include "prim/enum.h"
#include "prim/switch.h"

/*========== Macros and Definitions =========================================*/

#define prim_cmp(val_lhs, val_rhs) FUNC__prim_cmp(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_eq(val_lhs, val_rhs)  FUNC__prim_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_ne(val_lhs, val_rhs)  FUNC__prim_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_lt(val_lhs, val_rhs)  FUNC__prim_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_gt(val_lhs, val_rhs)  FUNC__prim_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_le(val_lhs, val_rhs)  FUNC__prim_le(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_ge(val_lhs, val_rhs)  FUNC__prim_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)

#define prim_min(val_lhs, val_rhs)          FUNC__prim_min(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_max(val_lhs, val_rhs)          FUNC__prim_max(pp_uniqTok(lhs), pp_uniqTok(rhs), val_lhs, val_rhs)
#define prim_clamp(val_x, val_min, val_max) FUNC__prim_clamp(val_x, val_min, val_max)
#define prim_wrap(val_x, val_min, val_max)  FUNC__prim_wrap(val_x, val_min, val_max)

#define prim_swap(val_lhs, val_rhs) FUNC__prim_swap(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(tmp), val_lhs, val_rhs)

#define prim_divisible(val_lhs, val_rhs) FUNC__prim_divisible(val_lhs, val_rhs)
#define prim_sign(val_x)                 FUNC__prim_sign(val_x)

typedef struct Void {
    u8 unused_[0];
} Void;

/*========== Macros Implementation ==========================================*/

#define FUNC__prim_cmp(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let __lhs = (val_lhs);                                    \
    let __rhs = (val_rhs);                                    \
    eval_return(                                              \
        ((__lhs) < (__rhs))   ? -1 /* cmp_Ord_lt */           \
        : ((__lhs) > (__rhs)) ? 1  /* cmp_Ord_gt */           \
                              : 0  /* cmp_Ord_eq */           \
    );                                                        \
})
#define FUNC__prim_eq(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs == (__rhs);                            \
})
#define FUNC__prim_ne(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs != (__rhs);                            \
})
#define FUNC__prim_lt(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs < (__rhs);                             \
})
#define FUNC__prim_gt(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs > (__rhs);                             \
})
#define FUNC__prim_le(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs <= (__rhs);                            \
})
#define FUNC__prim_ge(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let         __lhs = (val_lhs);                           \
    let         __rhs = (val_rhs);                           \
    eval_return __lhs >= (__rhs);                            \
})

#define FUNC__prim_min(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let __lhs = (val_lhs);                                    \
    let __rhs = (val_rhs);                                    \
    eval_return __lhs < (__rhs) ? (__lhs) : (__rhs);          \
})
#define FUNC__prim_max(__lhs, __rhs, val_lhs, val_rhs) eval({ \
    let __lhs = (val_lhs);                                    \
    let __rhs = (val_rhs);                                    \
    eval_return __lhs > (__rhs) ? (__lhs) : (__rhs);          \
})
#define FUNC__prim_clamp(val_x, val_min, val_max) \
    prim_min(prim_max(val_x, val_min), val_max)

#define FUNC__prim_swap(__lhs, __rhs, __tmp, val_lhs, val_rhs) eval({ \
    let __lhs = &(val_lhs);                                           \
    let __rhs = &(val_rhs);                                           \
    let __tmp = *(__lhs);                                             \
    *(__lhs)  = *(__rhs);                                             \
    *(__rhs)  = __tmp;                                                \
    eval_return{};                                                    \
})

#define FUNC__prim_divisible(val_lhs, val_rhs) eval({     \
    let __lhs = (val_lhs);                                \
    let __rhs = (val_rhs);                                \
    eval_return((__rhs == 0) ? 0 : (__lhs % __rhs == 0)); \
})
#define FUNC__prim_sign(val_x) eval({                                              \
    let         __scalar = (val_x);                                                \
    eval_return as$(TypeOf(__scalar), __scalar < 0 ? -1 : (__scalar > 0 ? 1 : 0)); \
})


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_INCLUDED */
