/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Tuple.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-11-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/foundation/pp
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
#pragma once
#ifndef foundation_pp_Tuple__included
#define foundation_pp_Tuple__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define pp_Tuple_len pp_countArg
#define pp_Tuple_wrap(/* _Elems */...) (__VA_ARGS__)
#define pp_Tuple_unwrap(/* _Elems */...) __VA_ARGS__

#define pp_Tuple_wrapOpen()  (
#define pp_Tuple_wrapClose() )

#define pp_Tuple_unwrapClose(/* _Elems */...) (__VA_ARGS__
#define pp_Tuple_unwrapOpen(/* _Elems */...) __VA_ARGS__)

#define pp_Tuple_unwrapCloseOptComma(/* _Elems */...) (__VA_ARGS__ __VA_OPT__(,)
#define pp_Tuple_unwrapOpenOptComma(/* _Elems */...) __VA_OPT__(,) __VA_ARGS__)

#define pp_Tuple_unwrapPreComma(/* _Elems */...) , __VA_ARGS__
#define pp_Tuple_unwrapSufComma(/* _Elems */...) __VA_ARGS__,

#define pp_Tuple_wrapSufIgnore(/* _Elems */...) (__VA_ARGS__) pp_ignore
#define pp_Tuple_wrapSufIgnore2(/* _Elems */...) (__VA_ARGS__) pp_ignore pp_ignore
#define pp_Tuple_unwrapSufIgnore(/* _Elems */...) __VA_ARGS__ pp_ignore
#define pp_Tuple_unwrapSufIgnore2(/* _Elems */...) __VA_ARGS__ pp_ignore pp_ignore
#define pp_Tuple_wrapSufCommaIgnore(/* _Elems */...) (__VA_ARGS__), pp_ignore
#define pp_Tuple_wrapSufCommaIgnore2(/* _Elems */...) (__VA_ARGS__), pp_ignore pp_ignore
#define pp_Tuple_unwrapSufCommaIgnore(/* _Elems */...) __VA_ARGS__, pp_ignore
#define pp_Tuple_unwrapSufCommaIgnore2(/* _Elems */...) __VA_ARGS__, pp_ignore pp_ignore

#define pp_Tuple_cat(/* _Elems */...) pp_Tuple_wrapOpen() __VA_ARGS__ pp_Tuple_unwrapOpenOptComma

#define pp_Tuple_get1st(_$a, ...) _$a
#define pp_Tuple_get2nd(_$a, _$b, ...) _$b
#define pp_Tuple_get3rd(_$a, _$b, _$c, ...) _$c
#define pp_Tuple_get4th(_$a, _$b, _$c, _$d, ...) _$d
#define pp_Tuple_get5th(_$a, _$b, _$c, _$d, _$e, ...) _$e
#define pp_Tuple_get6th(_$a, _$b, _$c, _$d, _$e, _$f, ...) _$f
#define pp_Tuple_get7th(_$a, _$b, _$c, _$d, _$e, _$f, _$g, ...) _$g
#define pp_Tuple_get8th(_$a, _$b, _$c, _$d, _$e, _$f, _$g, _$h, ...) _$h

#define pp_Tuple_get1stRest(_$a, _$b...) _$b
#define pp_Tuple_get2ndRest(_$a, _$b, _$c...) _$c
#define pp_Tuple_get3rdRest(_$a, _$b, _$c, _$d...) _$d
#define pp_Tuple_get4thRest(_$a, _$b, _$c, _$d, _$e...) _$e
#define pp_Tuple_get5thRest(_$a, _$b, _$c, _$d, _$e, _$f...) _$f
#define pp_Tuple_get6thRest(_$a, _$b, _$c, _$d, _$e, _$f, _$g...) _$g
#define pp_Tuple_get7thRest(_$a, _$b, _$c, _$d, _$e, _$f, _$g, _$h...) _$h

#define pp_Tuple_pushFront(_$Elem, _$Tuple) pp_Tuple_wrap(_$Elem, pp_Tuple_unwrap _$Tuple)
#define pp_Tuple_pushBack(_$Elem, _$Tuple) pp_Tuple_wrap(pp_Tuple_unwrap _$Tuple, _$Elem)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_pp_Tuple__included */
