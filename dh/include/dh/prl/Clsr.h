#ifndef prl_Clsr__included
#define prl_Clsr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Co.h"

/*========== Macros and Declarations ========================================*/

/*--- Closure's Context ---*/
#define Clsr_Ctx$(_$T...) __alias__Clsr_Ctx$(_$T)
T_alias$((Clsr_Ctx$raw)(struct Clsr_Ctx$raw {
    var_(ret_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Clsr_Ctx$(_$T...) __stmt__T_use_Clsr_Ctx$(_$T)
#define T_decl_Clsr_Ctx$(_$T...) __stmt__T_decl_Clsr_Ctx$(_$T)
#define T_impl_Clsr_Ctx$(_$T...) __stmt__T_impl_Clsr_Ctx$(_$T)
/*--- Closure's Routine ---*/
#define Clsr_Rtn$(_$T...) __alias__Clsr_Rtn$(_$T)
T_alias$((Clsr_Rtn)(fn_(((*)(P$$(Clsr_Ctx$raw) ctx))(P$$(Clsr_Ctx$raw)) $T)));
#define T_use_Clsr_Rtn$(_$T...) __stmt__T_use_Clsr_Rtn$(_$T)
/*--- Closure's Kind ---*/
T_alias$((Clsr_Kind)(enum_((Clsr_Kind $fits($packed))(
    Clsr_Kind_undefined = 0,
    Clsr_Kind_fn = 1,
    Clsr_Kind_co = 2,
))));
claim_assert_static(eqlType$(Clsr_Kind, u8));
/*--- Closure's Base ---*/
#define Clsr$(_$T...) __alias__Clsr$(_$T)
T_alias$((Clsr$raw)(struct Clsr$raw {
    T_embed$(struct {
        var_(kind, Clsr_Kind);
        var_(rtn, Clsr_Rtn);
    });
    var_(ctx_, Clsr_Ctx$raw) $flexible;
}));
#define T_use_Clsr$(_$T...) __stmt__T_use_Clsr$(_$T)
#define T_decl_Clsr$(_$T...) __stmt__T_decl_Clsr$(_$T)
#define T_impl_Clsr$(_$T...) __stmt__T_impl_Clsr$(_$T)
/*--- Closure's Fields ---*/
#define Clsr_Ret_(_$rtn...) __alias__Clsr_Ret_(_$rtn)
// #define fn_use_Clsr_Ret_(_$rtn...) __stmt__fn_use_Clsr_Ret_(_$rtn)
// #define co_use_Clsr_Ret_(_$rtn...) __stmt__co_use_Clsr_Ret_(_$rtn)
#define Clsr_Data_(_$rtn...) __alias__Clsr_Data_(_$rtn)
// #define fn_use_Clsr_Data_(_$rtn...) __stmt__fn_use_Clsr_Data_(_$rtn)
// #define co_use_Clsr_Data_(_$rtn...) __stmt__co_use_Clsr_Data_(_$rtn)
#define Clsr_Ctx_(_$rtn...) __alias__Clsr_Ctx_(_$rtn)
// #define fn_use_Clsr_Ctx_(_$rtn...) __stmt__fn_use_Clsr_Ctx_(_$rtn)
// #define co_use_Clsr_Ctx_(_$rtn...) __stmt__co_use_Clsr_Ctx_(_$rtn)
#define Clsr_Rtn_(_$rtn...) __alias__Clsr_Rtn_(_$rtn)
// #define fn_use_Clsr_Rtn_(_$rtn...) __stmt__fn_use_Clsr_Rtn_(_$rtn)
// #define co_use_Clsr_Rtn_(_$rtn...) __stmt__co_use_Clsr_Rtn_(_$rtn)
#define Clsr_rtn_(_$rtn...) __alias__Clsr_rtn_(_$rtn)
// #define fn_use_Clsr_rtn_(_$rtn...) __stmt__fn_use_Clsr_rtn_(_$rtn)
// #define co_use_Clsr_rtn_(_$rtn...) __stmt__co_use_Clsr_rtn_(_$rtn)
#define Clsr_from_(_$rtn...) __alias__Clsr_from_(_$rtn)
// #define fn_use_Clsr_from_(_$rtn...) __stmt__fn_use_Clsr_from_(_$rtn)
// #define co_use_Clsr_from_(_$rtn...) __stmt__co_use_Clsr_from_(_$rtn)
/*--- Closure ---*/
#define Clsr_(_$rtn...) __alias__Clsr_(_$rtn)
#define clsr_(/*(_$rtn)(_$args...)*/... /*(Clsr)*/) __expr__clsr_(__VA_ARGS__)
#define invoke_(_$p_clsr... /*-> (Clsr_Ctx)*/) __expr__invoke_(_$p_clsr)
#define fn_use_Clsr_(/*(_$fn)(_$Arg_T...)(_$Ret_T)*/...) __stmt__fn_use_Clsr_(__VA_ARGS__)
#define co_use_Clsr_(/*(_$co)(_$Arg_T...)(_$Ret_T)*/...) __stmt__co_use_Clsr_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

/*--- Closure's Context ---*/
#define __alias__Clsr_Ctx$(_$T...) tpl$(Clsr_Ctx, _$T)
#define __stmt__T_use_Clsr_Ctx$(_$T...) \
    T_decl_Clsr_Ctx$(_$T); \
    T_impl_Clsr_Ctx$(_$T)
#define __stmt__T_decl_Clsr_Ctx$(_$T...) \
    T_alias$((Clsr_Ctx$(_$T))(union Clsr_Ctx$(_$T)))
#define __stmt__T_impl_Clsr_Ctx$(_$T...) \
    union Clsr_Ctx$(_$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, _$T); \
                var_(ret_, _$T) $like_ref; \
            }); \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
    }
/*--- Closure's Routine ---*/
#define __alias__Clsr_Rtn$(_$T...) tpl$(Clsr_Rtn, _$T)
#define __stmt__T_use_Clsr_Rtn$(_$T...) \
    T_alias$((Clsr_Rtn$(_$T))(fn_(((*)(P$$(Clsr_Ctx$(_$T)) ctx))(P$$(Clsr_Ctx$(_$T)))$T)))
/*--- Closure's Base ---*/
#define __alias__Clsr$(_$T...) tpl$(Clsr, _$T)
#define __stmt__T_use_Clsr$(_$T...) \
    T_decl_Clsr$(_$T); \
    T_impl_Clsr$(_$T)
#define __stmt__T_decl_Clsr$(_$T...) \
    T_alias$((Clsr$(_$T))(union Clsr$(_$T)))
#define __stmt__T_impl_Clsr$(_$T...) \
    union Clsr$(_$T) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn$(_$T)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx$(_$T)); \
                var_(ctx_, Clsr_Ctx$(_$T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
    }
/*--- Closure's Fields ---*/
#define __alias__Clsr_Ret_(_$rtn...) tpl_(Clsr_Ret, _$rtn)
#define __alias__Clsr_Data_(_$rtn...) tpl_(Clsr_Data, _$rtn)
#define __alias__Clsr_Ctx_(_$rtn...) tpl_(Clsr_Ctx, _$rtn)
#define __alias__Clsr_Rtn_(_$rtn...) tpl_(Clsr_Rtn, _$rtn)
#define __alias__Clsr_rtn_(_$rtn...) tpl_(Clsr_rtn, _$rtn)
#define __alias__Clsr_from_(_$rtn...) tpl_(Clsr_from, _$rtn)
/*--- Closure ---*/
#define __alias__Clsr_(_$rtn...) tpl_(Clsr, _$rtn)
#define __alias__clsr_(_$rtn...) tpl_(clsr, _$rtn)
#define __expr__clsr_(...) __step__clsr___emit(__step__clsr___parseRtn __VA_ARGS__)
#define __step__clsr___parseRtn(_$rtn...) _$rtn,
#define __step__clsr___emit(...) __inline__clsr_(__VA_ARGS__)
#define __inline__clsr_(_$rtn, _$args...) copy(Clsr_from_(_$rtn) _$args)
#define __expr__invoke_(_$p_clsr...) __inline__invoke_(pp_uniqTok(p_ctx), pp_uniqTok(p_clsr), _$p_clsr)
#define __inline__invoke_(__p_ctx, __p_clsr, _$p_clsr...) local_({ \
    let __p_clsr = _$p_clsr; \
    claim_assert(__p_clsr->kind != Clsr_Kind_undefined); \
    let __p_ctx = __p_clsr->ctx_; \
    local_return_(ptrAlignCast$((P$$(TypeOf(*__p_ctx)))( \
        __p_clsr->rtn(ptrCast$((Clsr_Ctx$raw*)(__p_ctx))) \
    ))); \
})
/*--- Closure of Function ---*/
#define Clsr_Fn_Args_(_$rtn...) tpl_(Clsr_Fn_Args, _$rtn)
// #define fn_use_Clsr_Fn_Args_(_$rtn...) __stmt__fn_use_Clsr_Args_(_$rtn)
#define __stmt__fn_use_Clsr_(...) __step__fn_use_Clsr_(__step__fn_use_Clsr___parse0 __VA_ARGS__)
#define __step__fn_use_Clsr___parse0(_$fn...) \
    _$fn, __step__fn_use_Clsr___parse1
#define __step__fn_use_Clsr___parse1(_$Arg_T...) \
    pp_countArg(_$Arg_T), (_$Arg_T), __step__fn_use_Clsr___parse2
#define __step__fn_use_Clsr___parse2(_$Ret_T...) \
    _$Ret_T
#define __step__fn_use_Clsr_(...) __inline__fn_use_Clsr_(__VA_ARGS__)
#define __inline__fn_use_Clsr_(_$fn, _$N_Arg_T, _$Arg_T, _$Ret_T...) \
    T_alias$((Clsr_Ret_(_$fn))(_$Ret_T)); \
    T_alias$((Clsr_Fn_Args_(_$fn))(Tup$$ _$Arg_T)); \
    T_alias$((Clsr_Data_(_$fn))(struct Clsr_Data_(_$fn) { \
        var_(args, Clsr_Fn_Args_(_$fn)); \
    })); \
    T_alias$((Clsr_Ctx_(_$fn))(union Clsr_Ctx_(_$fn) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_$fn)); \
                var_(ret_, Clsr_Ret_(_$fn)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_$fn)); \
                var_(data_, Clsr_Data_(_$fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_$Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_$fn)(P$$(Clsr_Ctx_(_$fn)) ctx))(P$$(Clsr_Ctx_(_$fn)))) { \
        let rtn = _$fn; \
        ctx->ret = rtn(__step__fn_use_Clsr___passUntup(_$N_Arg_T, ctx->data.args.)); \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_$fn))(TypeOf(Clsr_rtn_(_$fn))*)); \
    T_alias$((Clsr_(_$fn))(union Clsr_(_$fn) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_$fn)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_$fn)); \
                var_(ctx_, Clsr_Ctx_(_$fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_$Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_$fn)(__step__fn_use_Clsr___untupToParams(_$N_Arg_T, _$Arg_T)))(Clsr_(_$fn))) { \
        return (Clsr_(_$fn)){ \
            .kind = Clsr_Kind_fn, \
            .rtn = Clsr_rtn_(_$fn), \
            .ctx.data.args = tie_(__step__fn_use_Clsr___passUntupVals(_$N_Arg_T)), \
        }; \
    }
#define __step__fn_use_Clsr___passUntup(_$N_Arg_T, _$field_path...) \
    pp_cat(__step__fn_use_Clsr___passUntup, _$N_Arg_T)(_$field_path)
#define __step__fn_use_Clsr___passUntup1(_$field_path...) \
    _$field_path $0
#define __step__fn_use_Clsr___passUntup2(_$field_path...) \
    __step__fn_use_Clsr___passUntup1(_$field_path), _$field_path $1
#define __step__fn_use_Clsr___passUntup3(_$field_path...) \
    __step__fn_use_Clsr___passUntup2(_$field_path), _$field_path $2
#define __step__fn_use_Clsr___passUntup4(_$field_path...) \
    __step__fn_use_Clsr___passUntup3(_$field_path), _$field_path $3
#define __step__fn_use_Clsr___passUntup5(_$field_path...) \
    __step__fn_use_Clsr___passUntup4(_$field_path), _$field_path $4
#define __step__fn_use_Clsr___passUntup6(_$field_path...) \
    __step__fn_use_Clsr___passUntup5(_$field_path), _$field_path $5
#define __step__fn_use_Clsr___passUntup7(_$field_path...) \
    __step__fn_use_Clsr___passUntup6(_$field_path), _$field_path $6
#define __step__fn_use_Clsr___passUntup8(_$field_path...) \
    __step__fn_use_Clsr___passUntup7(_$field_path), _$field_path $7
#define __step__fn_use_Clsr___passUntup9(_$field_path...) \
    __step__fn_use_Clsr___passUntup8(_$field_path), _$field_path $8
#define __step__fn_use_Clsr___passUntup10(_$field_path...) \
    __step__fn_use_Clsr___passUntup9(_$field_path), _$field_path $9
#define __step__fn_use_Clsr___passUntup11(_$field_path...) \
    __step__fn_use_Clsr___passUntup10(_$field_path), _$field_path $10
#define __step__fn_use_Clsr___passUntup12(_$field_path...) \
    __step__fn_use_Clsr___passUntup11(_$field_path), _$field_path $11
#define __step__fn_use_Clsr___passUntup13(_$field_path...) \
    __step__fn_use_Clsr___passUntup12(_$field_path), _$field_path $12
#define __step__fn_use_Clsr___passUntup14(_$field_path...) \
    __step__fn_use_Clsr___passUntup13(_$field_path), _$field_path $13
#define __step__fn_use_Clsr___passUntup15(_$field_path...) \
    __step__fn_use_Clsr___passUntup14(_$field_path), _$field_path $14
#define __step__fn_use_Clsr___passUntup16(_$field_path...) \
    __step__fn_use_Clsr___passUntup15(_$field_path), _$field_path $15
#define __step__fn_use_Clsr___passUntupVals(_$N_Arg_T, _$field_path...) \
    pp_cat(__step__fn_use_Clsr___passUntupVals, _$N_Arg_T)(_$field_path)
#define __step__fn_use_Clsr___passUntupVals1(_$field_path...) \
    (_$field_path $0)
#define __step__fn_use_Clsr___passUntupVals2(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals1(_$field_path), (_$field_path $1)
#define __step__fn_use_Clsr___passUntupVals3(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals2(_$field_path), (_$field_path $2)
#define __step__fn_use_Clsr___passUntupVals4(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals3(_$field_path), (_$field_path $3)
#define __step__fn_use_Clsr___passUntupVals5(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals4(_$field_path), (_$field_path $4)
#define __step__fn_use_Clsr___passUntupVals6(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals5(_$field_path), (_$field_path $5)
#define __step__fn_use_Clsr___passUntupVals7(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals6(_$field_path), (_$field_path $6)
#define __step__fn_use_Clsr___passUntupVals8(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals7(_$field_path), (_$field_path $7)
#define __step__fn_use_Clsr___passUntupVals9(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals8(_$field_path), (_$field_path $8)
#define __step__fn_use_Clsr___passUntupVals10(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals9(_$field_path), (_$field_path $9)
#define __step__fn_use_Clsr___passUntupVals11(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals10(_$field_path), (_$field_path $10)
#define __step__fn_use_Clsr___passUntupVals12(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals11(_$field_path), (_$field_path $11)
#define __step__fn_use_Clsr___passUntupVals13(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals12(_$field_path), (_$field_path $12)
#define __step__fn_use_Clsr___passUntupVals14(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals13(_$field_path), (_$field_path $13)
#define __step__fn_use_Clsr___passUntupVals15(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals14(_$field_path), (_$field_path $14)
#define __step__fn_use_Clsr___passUntupVals16(_$field_path...) \
    __step__fn_use_Clsr___passUntupVals15(_$field_path), (_$field_path $15)
#define __step__fn_use_Clsr___untupToParams(_$N_Arg_T, _$Arg_T...) \
    pp_cat(__step__fn_use_Clsr___untupToParams, _$N_Arg_T) _$Arg_T
#define __step__fn_use_Clsr___untupToParams1( \
    _$Arg_T1... \
) \
    _$Arg_T1 $0
#define __step__fn_use_Clsr___untupToParams2( \
    _$Arg_T1, _$Arg_T2... \
) \
    __step__fn_use_Clsr___untupToParams1( \
        _$Arg_T1 \
    ), \
        _$Arg_T2 $1
#define __step__fn_use_Clsr___untupToParams3( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3... \
) \
    __step__fn_use_Clsr___untupToParams2( \
        _$Arg_T1, _$Arg_T2 \
    ), \
        _$Arg_T3 $2
#define __step__fn_use_Clsr___untupToParams4( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4... \
) \
    __step__fn_use_Clsr___untupToParams3( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3 \
    ), \
        _$Arg_T4 $3
#define __step__fn_use_Clsr___untupToParams5( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5... \
) \
    __step__fn_use_Clsr___untupToParams4( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4 \
    ), \
        _$Arg_T5 $4
#define __step__fn_use_Clsr___untupToParams6( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6... \
) \
    __step__fn_use_Clsr___untupToParams5( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5 \
    ), \
        _$Arg_T6 $5
#define __step__fn_use_Clsr___untupToParams7( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7... \
) \
    __step__fn_use_Clsr___untupToParams6( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6 \
    ), \
        _$Arg_T7 $6
#define __step__fn_use_Clsr___untupToParams8( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8... \
) \
    __step__fn_use_Clsr___untupToParams7( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7 \
    ), \
        _$Arg_T8 $7
#define __step__fn_use_Clsr___untupToParams9( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9... \
) \
    __step__fn_use_Clsr___untupToParams8( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8 \
    ), \
        _$Arg_T9 $8
#define __step__fn_use_Clsr___untupToParams10( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10... \
) \
    __step__fn_use_Clsr___untupToParams9( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9 \
    ), \
        _$Arg_T10 $9
#define __step__fn_use_Clsr___untupToParams11( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11... \
) \
    __step__fn_use_Clsr___untupToParams10( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10 \
    ), \
        _$Arg_T11 $10
#define __step__fn_use_Clsr___untupToParams12( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12... \
) \
    __step__fn_use_Clsr___untupToParams11( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11 \
    ), \
        _$Arg_T12 $12
#define __step__fn_use_Clsr___untupToParams13( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13... \
) \
    __step__fn_use_Clsr___untupToParams12( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12 \
    ), \
        _$Arg_T13 $13
#define __step__fn_use_Clsr___untupToParams14( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14... \
) \
    __step__fn_use_Clsr___untupToParams13( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13 \
    ), \
        _$Arg_T14 $14
#define __step__fn_use_Clsr___untupToParams15( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15... \
) \
    __step__fn_use_Clsr___untupToParams14( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14 \
    ), \
        _$Arg_T15 $15
#define __step__fn_use_Clsr___untupToParams16( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15, _$Arg_T16... \
) \
    __step__fn_use_Clsr___untupToParams15( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15 \
    ), \
        _$Arg_T16 $16

/*--- Closure of Coroutine ---*/
#define Clsr_Co_Frame_(_$rtn...) tpl_(Clsr_Co_Frame, _$rtn)
// #define co_use_Clsr_Co_Frame_(_$rtn...) __stmt__co_use_Clsr_Co_Frame_(_$rtn)
#define __stmt__co_use_Clsr_(...) __step__co_use_Clsr_(__step__co_use_Clsr___parse0 __VA_ARGS__)
#define __step__co_use_Clsr___parse0(_$co...) \
    _$co, __step__co_use_Clsr___parse1
#define __step__co_use_Clsr___parse1(_$Arg_T...) \
    pp_countArg(_$Arg_T), (_$Arg_T), __step__co_use_Clsr___parse2
#define __step__co_use_Clsr___parse2(_$Ret_T...) \
    _$Ret_T
#define __step__co_use_Clsr_(...) __inline__co_use_Clsr_(__VA_ARGS__)
#define __inline__co_use_Clsr_(_$co, _$N_Arg_T, _$Arg_T, _$Ret_T...) \
    T_alias$((Clsr_Ret_(_$co))(_$Ret_T)); \
    T_alias$((Clsr_Co_Frame_(_$co))(Co_Frame_(_$co))); \
    T_alias$((Clsr_Data_(_$co))(struct Clsr_Data_(_$co) { \
        var_(frame, Clsr_Co_Frame_(_$co)); \
    })); \
    T_alias$((Clsr_Ctx_(_$co))(union Clsr_Ctx_(_$co) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_$co)); \
                var_(ret_, Clsr_Ret_(_$co)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_$co)); \
                var_(data_, Clsr_Data_(_$co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_$Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_$co)(P$$(Clsr_Ctx_(_$co)) ctx))(P$$(Clsr_Ctx_(_$co)))) { \
        let frame = &ctx->data.frame; \
        let_ignore = resume_(frame); \
        if (frame->ctx.ctrl.state == Co_State_ready) { \
            ctx->ret = frame->ctx.ret; \
        } \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_$co))(TypeOf(Clsr_rtn_(_$co))*)); \
    T_alias$((Clsr_(_$co))(union Clsr_(_$co) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_$co)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_$co)); \
                var_(ctx_, Clsr_Ctx_(_$co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_$Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_$co)(__step__co_use_Clsr___untupToParams(_$N_Arg_T, _$Arg_T)))(Clsr_(_$co))) { \
        return (Clsr_(_$co)){ \
            .kind = Clsr_Kind_co, \
            .rtn = Clsr_rtn_(_$co), \
            .ctx.data.frame = { \
                .rtn = _$co, \
                .ctx = { \
                    .ctrl = { \
                        .state = Co_State_pending, \
                        .line = 0, \
                    }, \
                    .suspended_data = {}, \
                    .args = { __step__co_use_Clsr___passUntup(_$N_Arg_T) }, \
                    .data = { \
                        .suspended = {}, \
                        .locals = {}, \
                        .locals_mut = {}, \
                    }, \
                }, \
            }, \
        }; \
    }
#define __step__co_use_Clsr___passUntup(_$N_Arg_T, _$field_path...) \
    pp_cat(__step__co_use_Clsr___passUntup, _$N_Arg_T)(_$field_path)
#define __step__co_use_Clsr___passUntup1(_$field_path...) \
    _$field_path $0
#define __step__co_use_Clsr___passUntup2(_$field_path...) \
    __step__co_use_Clsr___passUntup1(_$field_path), _$field_path $1
#define __step__co_use_Clsr___passUntup3(_$field_path...) \
    __step__co_use_Clsr___passUntup2(_$field_path), _$field_path $2
#define __step__co_use_Clsr___passUntup4(_$field_path...) \
    __step__co_use_Clsr___passUntup3(_$field_path), _$field_path $3
#define __step__co_use_Clsr___passUntup5(_$field_path...) \
    __step__co_use_Clsr___passUntup4(_$field_path), _$field_path $4
#define __step__co_use_Clsr___passUntup6(_$field_path...) \
    __step__co_use_Clsr___passUntup5(_$field_path), _$field_path $5
#define __step__co_use_Clsr___passUntup7(_$field_path...) \
    __step__co_use_Clsr___passUntup6(_$field_path), _$field_path $6
#define __step__co_use_Clsr___passUntup8(_$field_path...) \
    __step__co_use_Clsr___passUntup7(_$field_path), _$field_path $7
#define __step__co_use_Clsr___passUntup9(_$field_path...) \
    __step__co_use_Clsr___passUntup8(_$field_path), _$field_path $8
#define __step__co_use_Clsr___passUntup10(_$field_path...) \
    __step__co_use_Clsr___passUntup9(_$field_path), _$field_path $9
#define __step__co_use_Clsr___passUntup11(_$field_path...) \
    __step__co_use_Clsr___passUntup10(_$field_path), _$field_path $10
#define __step__co_use_Clsr___passUntup12(_$field_path...) \
    __step__co_use_Clsr___passUntup11(_$field_path), _$field_path $12
#define __step__co_use_Clsr___passUntup13(_$field_path...) \
    __step__co_use_Clsr___passUntup12(_$field_path), _$field_path $13
#define __step__co_use_Clsr___passUntup14(_$field_path...) \
    __step__co_use_Clsr___passUntup13(_$field_path), _$field_path $14
#define __step__co_use_Clsr___passUntup15(_$field_path...) \
    __step__co_use_Clsr___passUntup14(_$field_path), _$field_path $15
#define __step__co_use_Clsr___passUntup16(_$field_path...) \
    __step__co_use_Clsr___passUntup15(_$field_path), _$field_path $16
#define __step__co_use_Clsr___untupToParams( \
    _$N_Arg_T, _$Arg_T... \
) \
    pp_cat(__step__co_use_Clsr___untupToParams, _$N_Arg_T) _$Arg_T
#define __step__co_use_Clsr___untupToParams1( \
    _$Arg_T1... \
) \
    _$Arg_T1 $0
#define __step__co_use_Clsr___untupToParams2( \
    _$Arg_T1, _$Arg_T2... \
) \
    __step__co_use_Clsr___untupToParams1( \
        _$Arg_T1 \
    ), \
        _$Arg_T2 $1
#define __step__co_use_Clsr___untupToParams3( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3... \
) \
    __step__co_use_Clsr___untupToParams2( \
        _$Arg_T1, _$Arg_T2 \
    ), \
        _$Arg_T3 $2
#define __step__co_use_Clsr___untupToParams4( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4... \
) \
    __step__co_use_Clsr___untupToParams3( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3 \
    ), \
        _$Arg_T4 $3
#define __step__co_use_Clsr___untupToParams5( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5... \
) \
    __step__co_use_Clsr___untupToParams4( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4 \
    ), \
        _$Arg_T5 $4
#define __step__co_use_Clsr___untupToParams6( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6... \
) \
    __step__co_use_Clsr___untupToParams5( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5 \
    ), \
        _$Arg_T6 $5
#define __step__co_use_Clsr___untupToParams7( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7... \
) \
    __step__co_use_Clsr___untupToParams6( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6 \
    ), \
        _$Arg_T7 $6
#define __step__co_use_Clsr___untupToParams8( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8... \
) \
    __step__co_use_Clsr___untupToParams7( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7 \
    ), \
        _$Arg_T8 $7
#define __step__co_use_Clsr___untupToParams9( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9... \
) \
    __step__co_use_Clsr___untupToParams8( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8 \
    ), \
        _$Arg_T9 $8
#define __step__co_use_Clsr___untupToParams10( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10... \
) \
    __step__co_use_Clsr___untupToParams9( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9 \
    ), \
        _$Arg_T10 $9
#define __step__co_use_Clsr___untupToParams11( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11... \
) \
    __step__co_use_Clsr___untupToParams10( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10 \
    ), \
        _$Arg_T11 $10
#define __step__co_use_Clsr___untupToParams12( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12... \
) \
    __step__co_use_Clsr___untupToParams11( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11 \
    ), \
        _$Arg_T12 $12
#define __step__co_use_Clsr___untupToParams13( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13... \
) \
    __step__co_use_Clsr___untupToParams12( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12 \
    ), \
        _$Arg_T13 $13
#define __step__co_use_Clsr___untupToParams14( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14... \
) \
    __step__co_use_Clsr___untupToParams13( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13 \
    ), \
        _$Arg_T14 $14
#define __step__co_use_Clsr___untupToParams15( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15... \
) \
    __step__co_use_Clsr___untupToParams14( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14 \
    ), \
        _$Arg_T15 $15
#define __step__co_use_Clsr___untupToParams16( \
    _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
    _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15, _$Arg_T16... \
) \
    __step__co_use_Clsr___untupToParams15( \
        _$Arg_T1, _$Arg_T2, _$Arg_T3, _$Arg_T4, _$Arg_T5, _$Arg_T6, _$Arg_T7, _$Arg_T8, \
        _$Arg_T9, _$Arg_T10, _$Arg_T11, _$Arg_T12, _$Arg_T13, _$Arg_T14, _$Arg_T15 \
    ), \
        _$Arg_T16 $16

T_use_Clsr_Ctx$(Void);
T_use_Clsr_Rtn$(Void);
T_use_Clsr$(Void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Clsr__included */
