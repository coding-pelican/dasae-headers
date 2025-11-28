/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    lambda.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-11 (date of creation)
 * @updated 2025-03-10 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef builtin_lambda__included
#define builtin_lambda__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"
#include "lang_cfg.h"

#if comp_type == comp_type_clang
#include "BlocksRuntime/Block.h"
#endif /* comp_type == comp_type_clang */

/*========== Macros and Declarations ========================================*/

#define la_(/*(_Params...)(_TReturn) <$ext>*/...) __step__la_(__step__la___parseParams __VA_ARGS__)
#define $unscoped_la  /* TODO: Implement */
#define $unguarded_la /* TODO: Implement */

/*========== Macros and Definitions =========================================*/

#define __step__la___parseParams(_Params...) (_Params), __step__la___parseTReturn
#define __step__la___parseTReturn(_TReturn...) _TReturn
#define __step__la_(...) pp_overload(__step__la_, __VA_ARGS__)(__VA_ARGS__)
#define __step__la__2(_Params, _TReturn) __step__la___impl(_Params, _TReturn)
#define __step__la__3(_Params, _TReturn, _Ext...) pp_defer(pp_cat)(__step__la___impl, _Ext)(_Params, _TReturn)
#define __step__la_$_scope(_Params, _TReturn) /* TODO: Implement */ __step__la___impl$_scope(_Params, _TReturn)
#define __step__la_$_guard(_Params, _TReturn) /* TODO: Implement */ __step__la___impl$_guard(_Params, _TReturn)

#if lang_mode == lang_mode_cpp
/* C++11 */
#define __step__la___impl(_Params, _TReturn...) __la_(_Params, _TReturn)
#define __la_(_Parens_Params, T_Return...) [] _Parens_Params -> T_Return
#elif lang_mode == lang_mode_c
/* C11 */
#if comp_type == comp_type_clang
/* Needs compile flag `-fblocks -lBlocksRuntime` */
#define __step__la___impl(_Params, _TReturn...) __la_(_Params, _TReturn)
#define __la_(_Parens_Params, T_Return...) ^T_Return _Parens_Params
#elif comp_type == comp_type_gcc
#define __step__la___impl(_Params, _TReturn...) __la_(pp_uniqTok(la), _Params, _TReturn)
#define __la_(__lambda, _Parens_Params, T_Return...) T_Return __lambda _Parens_Params
#else /* others */
/* TODO: Add other compilers */
#endif
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_lambda__included  */
