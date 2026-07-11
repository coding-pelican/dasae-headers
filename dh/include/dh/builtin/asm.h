#pragma once
#ifndef builtin_asm__included
#define builtin_asm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "auto.h"

/*========== Macros and Declarations ========================================*/

#if !defined(asm)
#define asm __syn__asm
#endif /* !defined(asm) */

#define $reg(_$register...) __attr__$reg(_$register)

#define asm_var_(/*_$ident, _$T*/...) __syn__asm_var_(__VA_ARGS__)
#define asm_let_(/*_$ident, _$T*/...) __syn__asm_let_(__VA_ARGS__)

#define asm_var_reg(/*_$ident, _$T*/...) __syn__asm_var_reg(__VA_ARGS__)
#define asm_let_reg(/*_$ident, _$T*/...) __syn__asm_let_reg(__VA_ARGS__)

#define asm_volatile __syn__asm_volatile

/*========== Macros and Definitions =========================================*/

#define __syn__asm /* NOLINT(hicpp-no-assembler) */ __asm__

#define __attr__$reg(_$register...) asm(#_$register)

#define __syn__asm_var_(...) /* NOLINT(hicpp-no-assembler) */ register var_(__VA_ARGS__)
#define __syn__asm_let_(...) /* NOLINT(hicpp-no-assembler) */ register let_(__VA_ARGS__)

#define __syn__asm_var_reg(...) pp_overload(__asm_var_reg, __VA_ARGS__)(__VA_ARGS__)
#define __asm_var_reg_1(_$ident...) /* NOLINT(hicpp-no-assembler) */ register var _$ident asm(#_$ident)
#define __asm_var_reg_2(_$ident, _$T...) /* NOLINT(hicpp-no-assembler) */ register var_(_$ident, _$T) asm(#_$ident)

#define __syn__asm_let_reg(...) pp_overload(__asm_let_reg, __VA_ARGS__)(__VA_ARGS__)
#define __asm_let_reg_1(_$ident...) /* NOLINT(hicpp-no-assembler) */ register let _$ident asm(#_$ident)
#define __asm_let_reg_2(_$ident, _$T...) /* NOLINT(hicpp-no-assembler) */ register let_(_$ident, _$T) asm(#_$ident)

#define __syn__asm_volatile /* NOLINT(hicpp-no-assembler) */ __asm__ __volatile__

#if defined(__cplusplus)
} /* defined(__cplusplus) */
#endif /* defined(__cplusplus) */
#endif /* builtin_asm__included */
