/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    types.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-header(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "compat/compat_auto.h"
#include "compat/compat_bool.h"
#include "compat/compat_lambda.h"
#include "compat/compat_null.h"
#include "compat/compat_primitive.h"
#include "compat/compat_ref.h"
#include "compat/compat_wchar.h"


#define from(_T, _U, ...) _T##_from##_U(__VA_ARGS__)
#define into(_U, _T, ...) _U##_into##_T(__VA_ARGS__)

#define make(_T)          ((_T){ 0 })
#define makeCleared(_T)   ((_T){ 0 })
#define makeWith(_T, ...) ((_T){ __VA_ARGS__ })


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TYPES_INCLUDED */
