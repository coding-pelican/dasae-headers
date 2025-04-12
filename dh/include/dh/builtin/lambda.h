/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    lambda.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-11 (date of creation)
 * @updated 2025-03-10 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/bti
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef BTI_LAMBDA_INCLUDED
#define BTI_LAMBDA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp_cfg.h"
#include "lang_cfg.h"

#if BUILTIN_COMP_CLANG
#include "BlocksRuntime/Block.h"
#endif /* BUILTIN_COMP_CLANG */

/*========== Macros and Declarations ========================================*/

#define lam_(_Parens_Params, T_Return...) comp_syn__lam_(_Parens_Params, T_Return)

/*========== Macros and Definitions =========================================*/

#if BUILTIN_LANG_MODE_CPP || BUILTIN_COMP_CLANG
/* C++11 */
#define comp_syn__lam_(_Parens_Params, T_Return...) impl_comp_syn__lam_(_Parens_Params, T_Return)
/* Needs compile flag `-fblocks -lBlocksRuntime` */
#elif BUILTIN_COMP_GCC
#define comp_syn__lam_(_Parens_Params, T_Return...) impl_comp_syn__lam_(pp_uniqTok(lam), _Parens_Params, T_Return)
#else  /* others */
/* TODO: Add other compilers */
#endif /* others */

#if BUILTIN_LANG_MODE_CPP
/* C++11 */
#define impl_comp_syn__lam_(_Parens_Params, T_Return...) \
    [] _Parens_Params -> T_Return
#elif BUILTIN_COMP_CLANG
/* Needs compile flag `-fblocks -lBlocksRuntime` */
#define impl_comp_syn__lam_(_Parens_Params, T_Return...) \
    ^T_Return _Parens_Params
#elif BUILTIN_COMP_GCC
#define impl_comp_syn__lam_(__lambda, _Parens_Params, T_Return...) \
    T_Return __lambda _Parens_Params
#else  /* others */
/* TODO: Add other compilers */
#endif /* others */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_LAMBDA_INCLUDED  */
