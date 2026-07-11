/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-03 (date of creation)
 * @updated 2026-05-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 *
 * @brief   Base prelude for dh/prl
 * @details Provides base prelude for dh/prl.
 */
#pragma once
#ifndef prl_base__included
#define prl_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base/refs.h"
#include "base/vals.h"
#include "base/exts.h"

/*========== Macros and Declarations ========================================*/

#define A_a$(_$ANT, ... /*(_$ANT)*/) l$((_$ANT)A_init(__VA_ARGS__))
#define A_s$(_$ANT, ... /*(anon S(T))*/) A_ref(A_a$(_$ANT, __VA_ARGS__))
#define A_l$(_$ANT, ... /*(anon S_const(T))*/) (A_ref(A_a$(_$ANT, __VA_ARGS__)).as_const)
#define T_a$(_$T, ... /*(anon A(N,_$T))*/) A_from$((_$T)__VA_ARGS__)
#define T_s$(_$T, ... /*(S(_$T))*/) A_ref$((S$(_$T))T_a$(_$T, __VA_ARGS__))
#define T_l$(_$T, ... /*(S_const(_$T))*/) A_ref$((S_const$(_$T))T_a$(_$T, __VA_ARGS__))
#define NT_a$(_$N, _$T, ... /*(A(_$N, _$T))*/) l$((A$(_$N, _$T))A_init(__VA_ARGS__))
#define NT_s$(_$N, _$T, ... /*(S(_$T))*/) A_ref$((S$(_$T))l$((A$$(_$N, _$T))A_init(__VA_ARGS__)))
#define NT_l$(_$N, _$T, ... /*(S_const(_$T))*/) A_ref$((S_const$(_$T))l$((A$$(_$N, _$T))A_init(__VA_ARGS__)))

#define u8_c(_$literal) /*literal as u8 character*/ \
    comp_syn__u8_c(_$literal)
#define u16_c(_$literal) /*literal as u16 character*/ \
    comp_syn__u16_c(_$literal)
#define u32_c(_$literal) /*literal as u32 character*/ \
    comp_syn__u32_c(_$literal)

#define u8_a(_$literal) /*literal as u8 array (exclude '\0' sentinel)*/ \
    comp_syn__u8_a(_$literal)
#define u8_s(_$literal) /*literal as u8 mut slice (exclude '\0' sentinel)*/ \
    comp_syn__u8_s(_$literal)
#define u8_l(_$literal) /*literal as u8 const slice (exclude '\0' sentinel)*/ \
    comp_syn__u8_l(_$literal)

#define u8_aZ0(_$literal) /*literal as null-terminated u8 array (include '\0' sentinel)*/ \
    comp_syn__u8_aZ0(_$literal)
#define u8_sZ0(_$literal) /*literal as null-terminated u8 mut slice (include '\0' sentinel)*/ \
    comp_syn__u8_sZ0(_$literal)
#define u8_lZ0(_$literal) /*literal as null-terminated u8 const slice (include '\0' sentinel)*/ \
    comp_syn__u8_lZ0(_$literal)

#define u8_pZ0(_$literal) /* literal as null-terminated u8 mut pointer */ \
    comp_syn__u8_pZ0(_$literal)
#define u8_kZ0(_$literal) /* literal as null-terminated u8 const pointer */ \
    comp_syn__u8_kZ0(_$literal)

#if UNUSED_CODE
#define u8z_a(_$literal) /*literal as u8 array*/ comp_syn__u8z_a(_$literal)
#define u8z_s(_$literal) /*literal as u8 mut slice*/ comp_syn__u8z_s(_$literal)
#define u8z_l(_$literal) /*literal as u8 const slice*/ comp_syn__u8z_l(_$literal)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

#define comp_syn__u8_c(_$literal) l$((u8){ _$literal })
#define comp_syn__u16_c(_$literal) l$((u16){ u##_$literal })
#define comp_syn__u32_c(_$literal) l$((u32){ U##_$literal })

#define comp_syn__u8_a(_$literal...) $suppress_unterminated_string_initialization( \
    l$((A$$(sizeOf$(TypeOf(_$literal)) - 1, u8)){ .val = { _$literal } }) \
)
#define comp_syn__u8_s(_$literal) l$((S$u8){ .ptr = l$((u8[]){ "" _$literal }), .len = sizeOf$(TypeOf(_$literal)) - 1 })
#define comp_syn__u8_l(_$literal) l$((S_const$u8){ .ptr = as$(const u8*)("" _$literal), .len = sizeOf$(TypeOf(_$literal)) - 1 })

#define comp_syn__u8_aZ0(_$literal) l$((A$$(sizeOf$(TypeOf(_$literal)), u8)){ .val = { _$literal } })
#define comp_syn__u8_sZ0(_$literal) l$((S$u8){ .ptr = l$((u8[]){ "" _$literal }), .len = sizeOf$(TypeOf(_$literal)) })
#define comp_syn__u8_lZ0(_$literal) l$((S_const$u8){ .ptr = as$(const u8*)("" _$literal), .len = sizeOf$(TypeOf(_$literal)) })

#define comp_syn__u8_pZ0(_$literal) l$((u8*){ _$literal })
#define comp_syn__u8_kZ0(_$literal) (as$(const u8*)(_$literal))

#if UNUSED_CODE
#define comp_syn__u8z_a(_$literal) l$((AZ$$(sizeOf$(TypeOf(_$literal)) - 1, u8)){ .val = { _$literal } })
#define comp_syn__u8z_s(_$literal) l$((SZ$u8){ .ptr = l$((u8[]){ "" _$literal }), .len = sizeOf$(TypeOf(_$literal)) - 1 })
#define comp_syn__u8z_l(_$literal) l$((SZ_const$u8){ .ptr = as$(const u8*)("" _$literal), .len = sizeOf$(TypeOf(_$literal)) - 1 })
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_base__included */
