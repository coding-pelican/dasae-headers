/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Tuple.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/pp
 * @prefix  pp_Tuple
 *
 * @brief   Preprocessor tuple utilities
 * @details Provides preprocessor tuple utilities and functions for:
 *          - Tuple wrapping and unwrapping
 *          - Tuple element access
 *          - Tuple element manipulation
 *          - Tuple element counting
 *          - Tuple element iteration
 */

#ifndef CORE_PP_TUPLE_INCLUDED
#define CORE_PP_TUPLE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define pp_Tuple_wrap(/* _Elems */...)   (__VA_ARGS__)
#define pp_Tuple_unwrap(/* _Elems */...) __VA_ARGS__
#define pp_Tuple_wrapOpen()  (
#define pp_Tuple_wrapClose() )
#define pp_Tuple_unwrapClose(/* _Elems */...) (__VA_ARGS__
#define pp_Tuple_unwrapOpen(/* _Elems */...) __VA_ARGS__)
#define pp_Tuple_unwrapCloseOptComma(/* _Elems */...) (__VA_ARGS__ __VA_OPT__(,)
#define pp_Tuple_unwrapOpenOptComma(/* _Elems */...) __VA_OPT__(,) __VA_ARGS__)
#define pp_Tuple_unwrapPreComma(/* _Elems */...) , __VA_ARGS__
#define pp_Tuple_unwrapSufComma(/* _Elems */...) __VA_ARGS__,
#define pp_Tuple_cat(/* _Elems */...)            pp_Tuple_wrapOpen() __VA_ARGS__ pp_Tuple_unwrapOpenOptComma

#define pp_Tuple_get1st(_a, ...)                             _a
#define pp_Tuple_get2nd(_a, _b, ...)                         _b
#define pp_Tuple_get3rd(_a, _b, _c, ...)                     _c
#define pp_Tuple_get4th(_a, _b, _c, _d, ...)                 _d
#define pp_Tuple_get5th(_a, _b, _c, _d, _e, ...)             _e
#define pp_Tuple_get6th(_a, _b, _c, _d, _e, _f, ...)         _f
#define pp_Tuple_get7th(_a, _b, _c, _d, _e, _f, _g, ...)     _g
#define pp_Tuple_get8th(_a, _b, _c, _d, _e, _f, _g, _h, ...) _h

#define pp_Tuple_pushFront(_Elem, _Tuple) pp_Tuple_wrap(_Elem, pp_Tuple_unwrap _Tuple)
#define pp_Tuple_pushBack(_Elem, _Tuple)  pp_Tuple_wrap(pp_Tuple_unwrap _Tuple, _Elem)

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* CORE_PP_TUPLE_INCLUDED */
