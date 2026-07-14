/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    m-math-linalg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2026-07-14 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  m
 *
 * @brief   Math linear algebra library
 */
#pragma once
#ifndef m_math_linalg__included
#define m_math_linalg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "m-math-linalg/V-Vec.h"
#include "m-math-linalg/V-Vec-flt.h"
#include "m-math-linalg/V-Vec-int.h"
#include "m-math-linalg/V-Vec-iint.h"
#include "m-math-linalg/V-Vec-uint.h"
#include "m-math-linalg/V-Vec-sgnd.h"
#include "m-math-linalg/V-Vec-usgnd.h"

#include "m-math-linalg/M-Mat.h"
#include "m-math-linalg/M-Mat-flt.h"
#include "m-math-linalg/M-Mat-int.h"
#include "m-math-linalg/M-Mat-iint.h"
#include "m-math-linalg/M-Mat-uint.h"
#include "m-math-linalg/M-Mat-sgnd.h"
#include "m-math-linalg/M-Mat-usgnd.h"

#include "m-math-linalg/Q-Quat.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg__included */
