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

#define A_a$(_ANT, ... /*(_ANT)*/) l$((_ANT)A_init(__VA_ARGS__))
#define A_s$(_ANT, ... /*(anon S(T))*/) A_ref(A_a$(_ANT, __VA_ARGS__))
#define A_l$(_ANT, ... /*(anon S_const(T))*/) (A_ref(A_a$(_ANT, __VA_ARGS__)).as_const)
#define T_a$(_T, ... /*(anon A(N,_T))*/) A_from$((_T)__VA_ARGS__)
#define T_s$(_T, ... /*(S(_T))*/) A_ref$((S$(_T))T_a$(_T, __VA_ARGS__))
#define T_l$(_T, ... /*(S_const(_T))*/) A_ref$((S_const$(_T))T_a$(_T, __VA_ARGS__))
#define NT_a$(_N, _T, ... /*(A(_N, _T))*/) l$((A$(_N, _T))A_init(__VA_ARGS__))
#define NT_s$(_N, _T, ... /*(S(_T))*/) A_ref$((S$(_T))l$((A$$(_N, _T))A_init(__VA_ARGS__)))
#define NT_l$(_N, _T, ... /*(S_const(_T))*/) A_ref$((S_const$(_T))l$((A$$(_N, _T))A_init(__VA_ARGS__)))

#define u8_c(_literal) /*literal as u8 character*/ \
    comp_syn__u8_c(_literal)
#define u16_c(_literal) /*literal as u16 character*/ \
    comp_syn__u16_c(_literal)
#define u32_c(_literal) /*literal as u32 character*/ \
    comp_syn__u32_c(_literal)

#define u8_a(_literal) /*literal as u8 array (exclude '\0' sentinel)*/ \
    comp_syn__u8_a(_literal)
#define u8_s(_literal) /*literal as u8 mut slice (exclude '\0' sentinel)*/ \
    comp_syn__u8_s(_literal)
#define u8_l(_literal) /*literal as u8 const slice (exclude '\0' sentinel)*/ \
    comp_syn__u8_l(_literal)

#define u8_aZ0(_literal) /*literal as null-terminated u8 array (include '\0' sentinel)*/ \
    comp_syn__u8_aZ0(_literal)
#define u8_sZ0(_literal) /*literal as null-terminated u8 mut slice (include '\0' sentinel)*/ \
    comp_syn__u8_sZ0(_literal)
#define u8_lZ0(_literal) /*literal as null-terminated u8 const slice (include '\0' sentinel)*/ \
    comp_syn__u8_lZ0(_literal)

#define u8_pZ0(_literal) /* literal as null-terminated u8 mut pointer */ \
    comp_syn__u8_pZ0(_literal)
#define u8_kZ0(_literal) /* literal as null-terminated u8 const pointer */ \
    comp_syn__u8_kZ0(_literal)

#if UNUSED_CODE
#define u8z_a(_literal) /*literal as u8 array*/ comp_syn__u8z_a(_literal)
#define u8z_s(_literal) /*literal as u8 mut slice*/ comp_syn__u8z_s(_literal)
#define u8z_l(_literal) /*literal as u8 const slice*/ comp_syn__u8z_l(_literal)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

#define comp_syn__u8_c(_literal) l$((u8){ _literal })
#define comp_syn__u16_c(_literal) l$((u16){ u##_literal })
#define comp_syn__u32_c(_literal) l$((u32){ U##_literal })

#define comp_syn__u8_a(_literal...) $supress_unterminated_string_initialization( \
    l$((A$$(sizeOf$(TypeOf(_literal)) - 1, u8)){ .val = { _literal } }) \
)
#define comp_syn__u8_s(_literal) l$((S$u8){ .ptr = l$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) - 1 })
#define comp_syn__u8_l(_literal) l$((S_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) - 1 })

#define comp_syn__u8_aZ0(_literal) l$((A$$(sizeOf$(TypeOf(_literal)), u8)){ .val = { _literal } })
#define comp_syn__u8_sZ0(_literal) l$((S$u8){ .ptr = l$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) })
#define comp_syn__u8_lZ0(_literal) l$((S_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) })

#define comp_syn__u8_pZ0(_literal) l$((u8*){ _literal })
#define comp_syn__u8_kZ0(_literal) as$(const u8*)(_literal)

#if UNUSED_CODE
#define comp_syn__u8z_a(_literal) l$((AZ$$(sizeOf$(TypeOf(_literal)) - 1, u8)){ .val = { _literal } })
#define comp_syn__u8z_s(_literal) l$((SZ$u8){ .ptr = l$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) - 1 })
#define comp_syn__u8z_l(_literal) l$((SZ_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) - 1 })
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_base__included */
