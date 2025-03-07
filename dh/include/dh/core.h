/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    core.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   "Make C Great Again"
 * @details Some detailed explanation
 */

#ifndef CORE_INCLUDED
#define CORE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/cfg.h"
#include "core/prim.h"
#include "core/pp.h"
#include "core/op.h"
#include "core/cmp.h"
#include "core/type_info.h"
#include "core/src_loc.h"

#define op_asg(var_ptr, val)     OP__op_asg(var_ptr, val)
#define OP__op_asg(var_ptr, val) (*(var_ptr) = type$(TypeOf(*(var_ptr)), val))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */
