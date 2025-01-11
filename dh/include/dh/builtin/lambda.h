/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    lambda.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-11 (date of creation)
 * @updated 2024-11-11 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

// TODO: Change lambda usage
#ifndef BUILTIN_LAMBDA_INCLUDED
#define BUILTIN_LAMBDA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define lambda(_Return_T, _name, _Paren_Args...) IMPL_lambda(_Return_T, _name, _Paren_Args)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* C++11 */
#define IMPL_lambda(_Return_T, _name, _Paren_Args...) auto _name = [] _Paren_Args
#elif defined(__clang_major__)
/* Needs compile flag `-fblocks` */
#define IMPL_lambda(_Return_T, _name, _Paren_Args...) _Return_T(^_name) _Paren_Args = ^_Return_T _Paren_Args
#elif defined(__GNUC__) || defined(__GNUG__)
#define IMPL_lambda(_Return_T, _name, _Paren_Args...) _Return_T _name _Paren_Args
#else
/* TODO: Add other compilers */
#endif

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_LAMBDA_INCLUDED  */
