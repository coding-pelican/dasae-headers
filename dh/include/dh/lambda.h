/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    lambda.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-11 (date of creation)
 * @updated 2024-11-11 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


// TODO: Change lambda usage
#ifndef LAMBDA_INCLUDED
#define LAMBDA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Macros and Definitions =========================================*/

#define lambda(RETURN_TYPE, NAME, PAREN_ARGS...) IMPL_lambda(RETURN_TYPE, NAME, PAREN_ARGS)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* C++11 */
#define IMPL_lambda(RETURN_TYPE, NAME, PAREN_ARGS...) auto NAME = [] PAREN_ARGS
#elif defined(__clang_major__)
/* Needs compile flag `-fblocks` */
#define IMPL_lambda(RETURN_TYPE, NAME, PAREN_ARGS...) RETURN_TYPE(^NAME) PAREN_ARGS = ^RETURN_TYPE PAREN_ARGS
#elif defined(__GNUC__) || defined(__GNUG__)
#define IMPL_lambda(RETURN_TYPE, NAME, PAREN_ARGS...) RETURN_TYPE NAME PAREN_ARGS
#else
/* TODO: Add other compilers */
#endif

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
