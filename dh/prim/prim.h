/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-10-28 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/prim
 * @prefix  prim
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_INCLUDED
#define PRIM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "prim_bool.h"
#include "prim_flts.h"
#include "prim_ints.h"
#include "prim_null.h"
#include "prim_ptr.h"


#define prim_as(_T, _prim_val...)      RETURN_prim_as(_T, _prim_val)
#define prim_value(_T, _prim_val...)   RETURN_prim_value(_T, _prim_val)
#define prim_literal(_T, _prim_val...) RETURN_prim_literal(_T, _prim_val)

#define prim_eq(_prim_lhs, _prim_rhs...) RETURN_prim_eq(_prim_lhs, _prim_rhs)
#define prim_ne(_prim_lhs, _prim_rhs...) RETURN_prim_ne(_prim_lhs, _prim_rhs)
#define prim_lt(_prim_lhs, _prim_rhs...) RETURN_prim_lt(_prim_lhs, _prim_rhs)
#define prim_le(_prim_lhs, _prim_rhs...) RETURN_prim_le(_prim_lhs, _prim_rhs)
#define prim_gt(_prim_lhs, _prim_rhs...) RETURN_prim_gt(_prim_lhs, _prim_rhs)
#define prim_ge(_prim_lhs, _prim_rhs...) RETURN_prim_ge(_prim_lhs, _prim_rhs)

#define prim_divisible(_prim_n, _prim_m...) RETURN_prim_divisible(_prim_n, _prim_m...)


#define RETURN_prim_as(_T, _prim_val...)      ((_T)_prim_val)
#define RETURN_prim_value(_T, _prim_val...)   ((_T){ _prim_val })
#define RETURN_prim_literal(_T, _prim_val...) ((_T[1]){ _prim_val })

#define RETURN_prim_eq(_prim_lhs, _prim_rhs...) ((_prim_lhs) == (_prim_rhs))
#define RETURN_prim_ne(_prim_lhs, _prim_rhs...) ((_prim_lhs) != (_prim_rhs))
#define RETURN_prim_lt(_prim_lhs, _prim_rhs...) ((_prim_lhs) < (_prim_rhs))
#define RETURN_prim_le(_prim_lhs, _prim_rhs...) ((_prim_lhs) <= (_prim_rhs))
#define RETURN_prim_gt(_prim_lhs, _prim_rhs...) ((_prim_lhs) > (_prim_rhs))
#define RETURN_prim_ge(_prim_lhs, _prim_rhs...) ((_prim_lhs) >= (_prim_rhs))

#define RETURN_prim_divisible(_prim_n, _prim_m...) ((_prim_m == 0) ? 0 : ((_prim_n) % (_prim_m) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_INCLUDED */
