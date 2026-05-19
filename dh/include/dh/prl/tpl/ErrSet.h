#ifndef prl_tpl_ErrSet__included
#define prl_tpl_ErrSet__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../core.h"

/*========== Macros and Declarations ========================================*/

#define $union_errset_(_E_Id_Other...) __syn__$union_errset_(_E_Id_Other)
#define errset_(/*(_E_Id_Self)(_E_Tag...) $union_errset_(_E_Id_Other...)*/...) __stmt__errset_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __pp__errset__unwrap(...) __VA_ARGS__
#define __pp__errset__unwrapWrapped(...) __VA_ARGS__
#define __pp__errset__end __pp__errset__end
#define __syn__$union_errset_(...) , ((__VA_ARGS__))
#define __stmt__errset_(...) __pp__errset__parseId __VA_ARGS__ , __pp__errset__end)
#define __pp__errset__parseId(_Id...) __pp__errset__emit(_Id,
#define __pp__errset__emit(...) \
    pp_overload(__pp__errset__emit, __VA_ARGS__)(__VA_ARGS__)
#define __pp__errset__emit_3(_Id, _Members, _End) \
    __syn__errset__emit(_Id, _Members, ())
#define __pp__errset__emit_4(_Id, _Members, _Imports, _End) \
    __syn__errset__emit(_Id, _Members, __pp__errset__unwrapWrapped _Imports)

#define __syn__errset__emitTagMember(_Id, ...) __VA_OPT__( \
    pp_cat2(E_Tag$, __VA_ARGS__), \
    pp_cat4(E_Tag$, _Id, _, __VA_ARGS__) = pp_cat2(E_Tag$, __VA_ARGS__), \
)

#define __syn__errset__emitTypeAlias(_Id, ...) __VA_OPT__( \
    typedef pp_cat(E_Ctx$, _Id) pp_cat(E_Ctx$, __VA_ARGS__); \
)

#define __syn__errset__emitPayloadMember(_Id, ...) __VA_OPT__( \
    pp_cat(E_Ctx$, __VA_ARGS__) __VA_ARGS__; \
)

#define __syn__errset__emitImportedField(_Id, ...) __VA_OPT__( \
    __VA_ARGS__ __VA_ARGS__; \
)

#define __syn__errset__emitOccurrenceAlias(_Id, ...) __VA_OPT__( \
    typedef _Id pp_cat(E_Type$, __VA_ARGS__); \
)

#define __syn__errset__emitHashDecl(_Id, ...) /* clang-format off */ __VA_OPT__( \
    $attr($inline_always $maybe_unused) \
    static E_HashId pp_cat(E__hashId$, __VA_ARGS__)(void) { \
        return E_hasher(u8_l(#__VA_ARGS__)); \
    } \
) /* clang-format on */

#define __syn__errset__emitCauseDecl(_Id, ...) /* clang-format off */ __VA_OPT__( \
    $attr($inline_always $maybe_unused) \
    static pp_cat3(E_Type$, __VA_ARGS__) pp_cat(E_cause$, __VA_ARGS__)(void) { \
        static const pp_cat(E_Inner$, _Id) inner = { \
            .tag = pp_cat4(E_Tag$, _Id, _, __VA_ARGS__), \
            .tag_id = u8_l(#__VA_ARGS__), \
            .hashId = pp_cat(E__hashId$, __VA_ARGS__), \
        }; \
        return (pp_cat(E_Type$, __VA_ARGS__)){ .ctx.inner = &inner }; \
    } \
) /* clang-format on */

#define __syn__errset__emitLocalErr(_Id, ...) __VA_OPT__( \
    pp_cat(E_cause$, __VA_ARGS__)(), \
)

#define __syn__errset__emit(_Id, _Members, _Imports...) /* clang-format off */ \
    typedef enum $packed pp_cat(E_Tag$, _Id) { \
        pp_cat3(E_Tag$, _Id, _Any) = E_Tag$Any, \
        pp_foreach(__syn__errset__emitTagMember, _Id, __pp__errset__unwrap _Members) \
    } pp_cat(E_Tag$, _Id); \
    typedef struct pp_cat(E_Inner$, _Id) { \
        pp_cat(E_Tag$, _Id) tag; \
        S_const$u8 tag_id; \
        E_HashId (*hashId)(void); \
    } pp_cat(E_Inner$, _Id); \
    typedef union pp_cat(E_Ctx$, _Id) { \
        const pp_cat(E_Inner$, _Id)* inner; \
        E_CtxAny any; \
        E_CtxAny as_any $like_ref; \
    } pp_cat(E_Ctx$, _Id); \
    pp_foreach(__syn__errset__emitTypeAlias, _Id, __pp__errset__unwrap _Members) \
    typedef union pp_cat(E_Payload$, _Id) { \
        pp_foreach(__syn__errset__emitPayloadMember, _Id, __pp__errset__unwrap _Members) \
    } pp_cat(E_Payload$, _Id); \
    typedef union _Id { \
        pp_cat(E_Ctx$, _Id) ctx; \
        T_embed$(pp_cat(E_Payload$, _Id)); \
        EAny any; \
        EAny as_any $like_ref; \
        General_E General_E; \
        General_E as_general $like_ref; \
        pp_foreach(__syn__errset__emitImportedField, _Id, __pp__errset__unwrap _Imports) \
    } _Id; \
    $attr($inline_always $maybe_unused) \
    static S_const$u8 pp_cat(E_strfy$, _Id)(_Id self) { \
        debug_assert(E_tag(self.as_any) != E_Tag$Any); \
        return self.ctx.inner->tag_id; \
    } \
    $attr($inline_always $maybe_unused) \
    static E_HashId pp_cat(E_hashId$, _Id)(_Id self) { \
        debug_assert(E_tag(self.as_any) != E_Tag$Any); \
        return self.ctx.inner->hashId(); \
    } \
    pp_foreach(__syn__errset__emitHashDecl, _Id, __pp__errset__unwrap _Members) \
    pp_foreach(__syn__errset__emitOccurrenceAlias, _Id, __pp__errset__unwrap _Members) \
    pp_foreach(__syn__errset__emitCauseDecl, _Id, __pp__errset__unwrap _Members) \
    T_use$((_Id)(O)); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_resolve$, _Id)(_Id self))(O$(_Id)) $scope) { \
        let errs = A_from$((_Id){ \
            pp_foreach(__syn__errset__emitLocalErr, _Id, __pp__errset__unwrap _Members) \
        }); \
        for_(($s(A_ref(errs)))(err)) { \
            if (!E_eql(self.as_any, err->as_any)) continue; \
            return_some(*err); \
        } $end(for); \
        return_none(); \
    } $unscoped(fn); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_handle$, _Id)(_Id self))(_Id) $scope) { \
        let resolved = orelse_((pp_cat(E_resolve$, _Id)(self))(return self)); \
        return resolved; \
    } $unscoped(fn); \
    $attr($inline_always $maybe_unused) \
    static fn_((pp_cat(E_tag$, _Id)(_Id self))(pp_cat(E_Tag$, _Id))) { \
        let resolved = orelse_((pp_cat(E_resolve$, _Id)(self))(return pp_cat3(E_Tag$, _Id, _Any))); \
        return resolved.ctx.inner->tag; \
    }; \
    T_use_E$($set(_Id)(Void)); \
    T_alias$((E$($set(_Id)(void)))(E$($set(_Id)(Void)))) /* clang-format on */

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
