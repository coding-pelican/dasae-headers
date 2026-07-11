#pragma once
#ifndef prl_tpl_ErrSet__included
#define prl_tpl_ErrSet__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define $union_errset_(_$E_Id_Other...) __syn__$union_errset_(_$E_Id_Other)
#define errset_(/*(_$E_Id_Self)(_$E_Tag...) $union_errset_(_$E_Id_Other...)*/...) __stmt__errset_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __pp__errset__unwrap(...) __VA_ARGS__
#define __pp__errset__unwrapWrapped(...) __VA_ARGS__
#define __pp__errset__end __pp__errset__end
#define __syn__$union_errset_(...) , ((__VA_ARGS__))
#define __stmt__errset_(...) __pp__errset__parseId __VA_ARGS__ , __pp__errset__end)
#define __pp__errset__parseId(_$Id...) __pp__errset__emit(_$Id,
#define __pp__errset__emit(...) \
    pp_overload(__pp__errset__emit, __VA_ARGS__)(__VA_ARGS__)
#define __pp__errset__emit_3(_$Id, _$Members, _$End) \
    __syn__errset__emit(_$Id, _$Members, ())
#define __pp__errset__emit_4(_$Id, _$Members, _$Imports, _$End) \
    __syn__errset__emit(_$Id, _$Members, __pp__errset__unwrapWrapped _$Imports)

#define __syn__errset__emitTagMember(_$Id, ...) __VA_OPT__( \
    pp_cat2(E_Tag$, __VA_ARGS__), \
    pp_cat4(E_Tag$, _$Id, _, __VA_ARGS__) = pp_cat2(E_Tag$, __VA_ARGS__), \
)

#define __syn__errset__emitTypeAlias(_$Id, ...) __VA_OPT__( \
    typedef pp_cat(E_Ctx$, _$Id) pp_cat(E_Ctx$, __VA_ARGS__); \
)

#define __syn__errset__emitPayloadMember(_$Id, ...) __VA_OPT__( \
    pp_cat(E_Ctx$, __VA_ARGS__) __VA_ARGS__; \
)

#define __syn__errset__emitImportedField(_$Id, ...) __VA_OPT__( \
    __VA_ARGS__ __VA_ARGS__; \
)

#define __syn__errset__emitOccurrenceAlias(_$Id, ...) __VA_OPT__( \
    typedef _$Id pp_cat(E_Type$, __VA_ARGS__); \
)

#define __syn__errset__emitHashDecl(_$Id, ...) /* clang-format off */ __VA_OPT__( \
    $attr($inline_always $maybe_unused) \
    static E_HashId pp_cat(E__hashId$, __VA_ARGS__)(void) { \
        return E_hasher(u8_l(#__VA_ARGS__)); \
    } \
) /* clang-format on */

#define __syn__errset__emitCauseDecl(_$Id, ...) /* clang-format off */ __VA_OPT__( \
    $attr($inline_always $maybe_unused) \
    static pp_cat3(E_Type$, __VA_ARGS__) pp_cat(E_cause$, __VA_ARGS__)(void) { \
        static const pp_cat(E_Inner$, _$Id) inner = { \
            .tag = pp_cat4(E_Tag$, _$Id, _, __VA_ARGS__), \
            .tag_id = u8_l(#__VA_ARGS__), \
            .hashId = pp_cat(E__hashId$, __VA_ARGS__), \
        }; \
        return (pp_cat(E_Type$, __VA_ARGS__)){ .ctx.inner = &inner }; \
    } \
) /* clang-format on */

#define __syn__errset__emitLocalErr(_$Id, ...) __VA_OPT__( \
    pp_cat(E_cause$, __VA_ARGS__)(), \
)

#define __syn__errset__emit(_$Id, _$Members, _$Imports...) /* clang-format off */ \
    typedef enum $packed pp_cat(E_Tag$, _$Id) { \
        pp_cat3(E_Tag$, _$Id, _Any) = E_Tag$Any, \
        pp_foreach(__syn__errset__emitTagMember, _$Id, __pp__errset__unwrap _$Members) \
    } pp_cat(E_Tag$, _$Id); \
    typedef struct pp_cat(E_Inner$, _$Id) { \
        pp_cat(E_Tag$, _$Id) tag; \
        S_const$u8 tag_id; \
        E_HashId (*hashId)(void); \
    } pp_cat(E_Inner$, _$Id); \
    typedef union pp_cat(E_Ctx$, _$Id) { \
        const pp_cat(E_Inner$, _$Id)* inner; \
        E_CtxAny any; \
        E_CtxAny as_any $like_ref; \
    } pp_cat(E_Ctx$, _$Id); \
    pp_foreach(__syn__errset__emitTypeAlias, _$Id, __pp__errset__unwrap _$Members) \
    typedef union pp_cat(E_Payload$, _$Id) { \
        pp_foreach(__syn__errset__emitPayloadMember, _$Id, __pp__errset__unwrap _$Members) \
    } pp_cat(E_Payload$, _$Id); \
    typedef union _$Id { \
        pp_cat(E_Ctx$, _$Id) ctx; \
        T_embed$(pp_cat(E_Payload$, _$Id)); \
        EAny any; \
        EAny as_any $like_ref; \
        General_E General_E; \
        General_E as_general $like_ref; \
        pp_foreach(__syn__errset__emitImportedField, _$Id, __pp__errset__unwrap _$Imports) \
    } _$Id; \
    $attr($inline_always $maybe_unused) \
    static S_const$u8 pp_cat(E_strfy$, _$Id)(_$Id self) { \
        debug_assert(E_tag(self.as_any) != E_Tag$Any); \
        return self.ctx.inner->tag_id; \
    } \
    $attr($inline_always $maybe_unused) \
    static E_HashId pp_cat(E_hashId$, _$Id)(_$Id self) { \
        debug_assert(E_tag(self.as_any) != E_Tag$Any); \
        return self.ctx.inner->hashId(); \
    } \
    pp_foreach(__syn__errset__emitHashDecl, _$Id, __pp__errset__unwrap _$Members) \
    pp_foreach(__syn__errset__emitOccurrenceAlias, _$Id, __pp__errset__unwrap _$Members) \
    pp_foreach(__syn__errset__emitCauseDecl, _$Id, __pp__errset__unwrap _$Members) \
    T_use$((_$Id)(O)); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_resolve$, _$Id)(_$Id self))(O$(_$Id)) $scope) { \
        let errs = A_from$((_$Id){ \
            pp_foreach(__syn__errset__emitLocalErr, _$Id, __pp__errset__unwrap _$Members) \
        }); \
        for_(($s(A_ref(errs)))(err)) { \
            if (!E_eql(self.as_any, err->as_any)) continue; \
            return_some(*err); \
        } $end(for); \
        return_none(); \
    } $unscoped(fn); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_handle$, _$Id)(_$Id self))(_$Id) $scope) { \
        let resolved = orelse_((pp_cat(E_resolve$, _$Id)(self))(return self)); \
        return resolved; \
    } $unscoped(fn); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_tag$, _$Id)(_$Id self))(pp_cat(E_Tag$, _$Id))) { \
        let resolved = orelse_((pp_cat(E_resolve$, _$Id)(self))(return pp_cat3(E_Tag$, _$Id, _Any))); \
        return resolved.ctx.inner->tag; \
    }; \
    T_use_E$($set(_$Id)(Void)); \
    T_alias$((E$($set(_$Id)(void)))(E$($set(_$Id)(Void)))) /* clang-format on */

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
errset_((io_File_E)(
    io_File_NotFound,
    io_File_AccessDenied,
    io_File_OpenFailed,
    io_File_ReadFailed,
    io_File_WriteFailed
));

errset_((io_Parse_E)(
    io_Parse_InvalidArgument,
    io_Parse_UnexpectedEOF,
    io_Parse_UnexpectedChar,
    io_Parse_UnexpectedToken,
    io_Parse_UnexpectedTokenType,
    io_Parse_UnexpectedTokenValue
));

errset_((math_E)(
    math_DivisionByZero,
    math_Overflow,
    math_Underflow
));

errset_((mem_E)(OutOfMemory));
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_ErrSet__included */
