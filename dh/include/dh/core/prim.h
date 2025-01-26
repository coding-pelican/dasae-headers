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

#define prim_cmp(val_lhs, val_rhs) FUNC__prim_cmp(val_lhs, val_rhs)
#define prim_eq(val_lhs, val_rhs)  FUNC__prim_eq(val_lhs, val_rhs)
#define prim_ne(val_lhs, val_rhs)  FUNC__prim_ne(val_lhs, val_rhs)
#define prim_lt(val_lhs, val_rhs)  FUNC__prim_lt(val_lhs, val_rhs)
#define prim_gt(val_lhs, val_rhs)  FUNC__prim_gt(val_lhs, val_rhs)
#define prim_le(val_lhs, val_rhs)  FUNC__prim_le(val_lhs, val_rhs)
#define prim_ge(val_lhs, val_rhs)  FUNC__prim_ge(val_lhs, val_rhs)

#define prim_min(val_lhs, val_rhs)             FUNC__prim_min(val_lhs, val_rhs)
#define prim_max(val_lhs, val_rhs)             FUNC__prim_max(val_lhs, val_rhs)
#define prim_clamp(val_self, val_min, val_max) FUNC__prim_clamp(val_self, val_min, val_max)
#define prim_wrap(val_self, val_min, val_max)  FUNC__prim_wrap(val_self, val_min, val_max)

#define prim_swap(val_lhs, val_rhs) FUNC__prim_swap(val_lhs, val_rhs)

#define prim_divisible(val_lhs, val_rhs) FUNC__prim_divisible(val_lhs, val_rhs)
#define prim_sign(val_self)              FUNC__prim_sign(val_self)

typedef struct Void {
    u8 unused_[0];
} Void;

/*========== Macros Implementation ==========================================*/

#define FUNC__prim_cmp(val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                       \
    let _rhs = (val_rhs);                       \
    eval_return(                                \
        (_lhs < _rhs)   ? -1 /* cmp_Ord_lt */   \
        : (_lhs > _rhs) ? 1  /* cmp_Ord_gt */   \
                        : 0  /* cmp_Ord_eq */   \
    );                                          \
})

#define FUNC__prim_eq(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs == _rhs;                  \
})

#define FUNC__prim_ne(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs != _rhs;                  \
})

#define FUNC__prim_lt(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs < _rhs;                   \
})

#define FUNC__prim_gt(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs > _rhs;                   \
})

#define FUNC__prim_le(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs <= _rhs;                  \
})

#define FUNC__prim_ge(val_lhs, val_rhs) eval({ \
    let         _lhs = (val_lhs);              \
    let         _rhs = (val_rhs);              \
    eval_return _lhs >= _rhs;                  \
})

#define FUNC__prim_min(val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                       \
    let _rhs = (val_rhs);                       \
    eval_return _lhs < _rhs ? _lhs : _rhs;      \
})

#define FUNC__prim_max(val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                       \
    let _rhs = (val_rhs);                       \
    eval_return _lhs > _rhs ? _lhs : _rhs;      \
})

#define FUNC__prim_clamp(val_lhs, val_min, val_max) \
    prim_min(prim_max(val_lhs, val_min), val_max)

#define FUNC__prim_swap(val_lhs, val_rhs) eval({ \
    let _lhs = &(val_lhs);                       \
    let _rhs = &(val_rhs);                       \
    let _tmp = *_lhs;                            \
    *_lhs    = *_rhs;                            \
    *_rhs    = _tmp;                             \
    eval_return{};                               \
})

#define FUNC__prim_divisible(val_lhs, val_rhs) eval({  \
    let _lhs = (val_lhs);                              \
    let _rhs = (val_rhs);                              \
    eval_return((_rhs == 0) ? 0 : (_lhs % _rhs == 0)); \
})

#define FUNC__prim_sign(val_self) eval({                                        \
    let         _scalar = (val_self);                                           \
    eval_return as$(TypeOf(_scalar), _scalar < 0 ? -1 : (_scalar > 0 ? 1 : 0)); \
})


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_INCLUDED */
