#ifndef prl_types_Err__included
#define prl_types_Err__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"
#include "ErrRes.h"
#include "Sli.h"

/*========== Macros and Declarations ========================================*/

T_decl_S$(u8);
$inline
$static fn_((Err_domainToCStr(Err self))(const char*));
$inline
$static fn_((Err_domainToStrZ(Err self))(const u8*));
$extern fn_((Err_domainToStr(Err self))(S_const$u8));
$inline
$static fn_((Err_codeToCStr(Err self))(const char*));
$inline
$static fn_((Err_codeToStrZ(Err self))(const u8*));
$extern fn_((Err_codeToStr(Err self))(S_const$u8));
$extern fn_((Err_print(Err self))(void));

$inline
$static fn_((Err_Unknown(void))(Err));
$inline
$static fn_((Err_Unexpected(void))(Err));
$inline
$static fn_((Err_Unspecified(void))(Err));
$inline
$static fn_((Err_Unsupported(void))(Err));
$inline
$static fn_((Err_NotImplemented(void))(Err));
$inline
$static fn_((Err_InvalidArgument(void))(Err));
$inline
$static fn_((Err_None(void))(Err));

#define errset_(/*(_Id)(_enums...)*/...) __errset__exec(pp_defer(__errset__emit)(__errset__parse0 __VA_ARGS__))
#define __errset__exec(...) __VA_ARGS__
#define __errset__parse0(_Id...) _Id, __errset__parse1
#define __errset__parse1(_enums...) _enums
#define __errset__emit(...) __errset__emitNext(__VA_ARGS__)
#define __errset__emitNext(_Id, _enums...) \
    GEN__config_ErrSet(_Id, _enums)

/*========== Macros and Definitions =========================================*/

$inline
$static fn_((Err_domainToCStr(Err self))(const char*)) { return as$(const char*)(self.vt->domainToStrZ(self.ctx)); }
$inline
$static fn_((Err_domainToStrZ(Err self))(const u8*)) { return self.vt->domainToStrZ(self.ctx); }
$inline
$static fn_((Err_codeToCStr(Err self))(const char*)) { return as$(const char*)(self.vt->codeToStrZ(self.ctx)); }
$inline
$static fn_((Err_codeToStrZ(Err self))(const u8*)) { return self.vt->codeToStrZ(self.ctx); }

$inline
$static fn_((GeneralErr_domainToStrZ(ErrCode ctx))(const u8*)) {
    let_ignore = ctx;
    return u8_l0("GeneralErr");
}
$inline
$static fn_((GeneralErr_codeToStrZ(ErrCode ctx))(const u8*)) {
    let code = as$(ErrCode)(ctx);
    switch (code) {
    case ErrCode_Unknown:
        return u8_l0("Unknown");
    case ErrCode_Unexpected:
        return u8_l0("Unexpected");
    case ErrCode_Unspecified:
        return u8_l0("Unspecified");
    case ErrCode_Unsupported:
        return u8_l0("Unsupported");
    case ErrCode_NotImplemented:
        return u8_l0("NotImplemented");
    case ErrCode_InvalidArgument:
        return u8_l0("InvalidArgument");
    case ErrCode_None:
        return u8_l0("None");
    default:
        claim_unreachable_fmt("Unknown error code (code: %d)", code);
    }
}
$inline
$static fn_((GeneralErr_err(ErrCode self))(Err)) {
    static const Err_VT vt[1] = { {
        .domainToStrZ = GeneralErr_domainToStrZ,
        .codeToStrZ = GeneralErr_codeToStrZ,
    } };
    return (Err){
        .ctx = self,
        .vt = vt,
    };
}

$inline
$static fn_((Err_Unknown(void))(Err)) { return GeneralErr_err(ErrCode_Unknown); }
$inline
$static fn_((Err_Unexpected(void))(Err)) { return GeneralErr_err(ErrCode_Unexpected); }
$inline
$static fn_((Err_Unspecified(void))(Err)) { return GeneralErr_err(ErrCode_Unspecified); }
$inline
$static fn_((Err_Unsupported(void))(Err)) { return GeneralErr_err(ErrCode_Unsupported); }
$inline
$static fn_((Err_NotImplemented(void))(Err)) { return GeneralErr_err(ErrCode_NotImplemented); }
$inline
$static fn_((Err_InvalidArgument(void))(Err)) { return GeneralErr_err(ErrCode_InvalidArgument); }
$inline
$static fn_((Err_None(void))(Err)) { return GeneralErr_err(ErrCode_None); }

#define GEN__config_ErrSet(Name, ...) \
    typedef enum pp_cat(Name, Code) { \
        GEN__config_ErrSet__ENUM__Code__members( \
            Name, \
            pp_foreach( \
                GEN__config_ErrSet__ENUM__Code__member, \
                Name, \
                __VA_ARGS__ \
            ) \
        ) \
    } pp_cat(Name, Code); \
    typedef Err Name; \
    static $inline fn_((pp_join(_, Name, domainToStrZ)(ErrCode ctx))(const u8*)) { \
        let_ignore = ctx; \
        return u8_l0(#Name); \
    } \
    static $inline fn_((pp_join(_, Name, codeToStrZ)(ErrCode ctx))(const u8*)) { \
        let code = as$(pp_cat(Name, Code))(ctx); \
        switch (code) { \
            GEN__config_ErrSet__FN__codeToStrZ__cases( \
                Name, \
                pp_foreach( \
                    GEN__config_ErrSet__FN__codeToStrZ__case, \
                    Name, \
                    __VA_ARGS__ \
                ) \
            ) \
        } \
    } \
    static $inline fn_((pp_join(_, Name, err)(pp_cat(Name, Code) self))(Err)) { \
        static const Err_VT vt[1] = { { \
            .domainToStrZ = pp_join(_, Name, domainToStrZ), \
            .codeToStrZ = pp_join(_, Name, codeToStrZ), \
        } }; \
        return (Name){ \
            .ctx = as$(int)(self), \
            .vt = vt, \
        }; \
    } \
    GEN__config_ErrSet__FN__ctorTemplates( \
        Name, \
        pp_foreach( \
            GEN__config_ErrSet__FN__ctorTemplate, \
            Name, \
            __VA_ARGS__ \
        ) \
    ); \
    typedef union pp_join($, Name, Void) { \
        struct { \
            var_(is_ok, bool); \
            union { \
                var_(err, Name); \
                var_(ok, Void); \
                var_(raw $like_ref, E_Payload$raw); \
            } payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw $like_ref, E$raw); \
    } pp_join($, Name, Void); \
    typedef pp_join($, Name, Void) pp_join($, Name, void)

// Helper macro to generate error type enum values
#define GEN__config_ErrSet__ENUM__Code__members(Name, ...) \
    pp_cat(Name, Code_None) = 0, __VA_ARGS__

// Helper macro to generate case statements for message function
#define GEN__config_ErrSet__FN__codeToStrZ__cases(Name, ...) \
    case pp_cat(Name, Code_None): \
        return u8_l0("None"); \
        __VA_ARGS__ \
    default: \
        claim_unreachable_fmt("Unknown error code (code: %d)", code);

#define GEN__config_ErrSet__FN__ctorTemplates(Name, ...) \
    static $inline $maybe_unused fn_((pp_cat(Name, _None)(void))(Err)) { \
        return pp_join(_, Name, err)(pp_cat(Name, Code_None)); \
    } \
    __VA_ARGS__

// Helper macros for generating individual cases
#define GEN__config_ErrSet__ENUM__Code__member(Name, ...) __VA_OPT__( \
    pp_cat3(Name, Code_, __VA_ARGS__), \
)

#define GEN__config_ErrSet__FN__codeToStrZ__case(Name, ...) __VA_OPT__( \
    case pp_cat3(Name, Code_, __VA_ARGS__) : return u8_l0(#__VA_ARGS__); \
)

#define GEN__config_ErrSet__FN__ctorTemplate(Name, ...) __VA_OPT__( \
    static $inline $maybe_unused fn_((pp_join(_, Name, __VA_ARGS__)(void))(Err)) { \
        return pp_join(_, Name, err)(pp_cat3(Name, Code_, __VA_ARGS__)); \
    } \
)

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
errset_((io_FileErr)(
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
));

errset_((io_ParseErr)(
    InvalidArgument,
    UnexpectedEOF,
    UnexpectedChar,
    UnexpectedToken,
    UnexpectedTokenType,
    UnexpectedTokenValue
));

errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));

errset_((mem_AllocErr)(OutOfMemory));
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_Err__included */
