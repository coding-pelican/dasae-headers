#ifndef prl_Clsr_included
#define prl_Clsr_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Co.h"

/*========== Macros and Declarations ========================================*/

/*--- Closure's Context ---*/
#define Clsr_Ctx$(_T...) __alias__Clsr_Ctx$(_T)
T_alias$((Clsr_Ctx$raw)(struct Clsr_Ctx$raw {
    var_(ret_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Clsr_Ctx$(_T...) __stmt__T_use_Clsr_Ctx$(_T)
#define T_decl_Clsr_Ctx$(_T...) __stmt__T_decl_Clsr_Ctx$(_T)
#define T_impl_Clsr_Ctx$(_T...) __stmt__T_impl_Clsr_Ctx$(_T)
/*--- Closure's Routine ---*/
#define Clsr_Rtn$(_T...) __alias__Clsr_Rtn$(_T)
T_alias$((Clsr_Rtn)(fn_(((*)(P$$(Clsr_Ctx$raw) ctx))(P$$(Clsr_Ctx$raw)) $T)));
#define T_use_Clsr_Rtn$(_T...) __stmt__T_use_Clsr_Rtn$(_T)
/*--- Closure's Kind ---*/
T_alias$((Clsr_Kind)(enum_((Clsr_Kind $fits($packed))(
    Clsr_Kind_undefined = 0,
    Clsr_Kind_fn = 1,
    Clsr_Kind_co = 2,
))));
claim_assert_static(eqlType$(Clsr_Kind, u8));
/*--- Closure's Base ---*/
#define Clsr$(_T...) __alias__Clsr$(_T)
T_alias$((Clsr$raw)(struct Clsr$raw {
    T_embed$(struct {
        var_(kind, Clsr_Kind);
        var_(rtn, Clsr_Rtn);
    });
    var_(ctx_, Clsr_Ctx$raw) $flexible;
}));
#define T_use_Clsr$(_T...) __stmt__T_use_Clsr$(_T)
#define T_decl_Clsr$(_T...) __stmt__T_decl_Clsr$(_T)
#define T_impl_Clsr$(_T...) __stmt__T_impl_Clsr$(_T)
/*--- Closure's Fields ---*/
#define Clsr_Ret_(_rtn...) __alias__Clsr_Ret_(_rtn)
// #define fn_use_Clsr_Ret_(_rtn...) __stmt__fn_use_Clsr_Ret_(_rtn)
// #define co_use_Clsr_Ret_(_rtn...) __stmt__co_use_Clsr_Ret_(_rtn)
#define Clsr_Data_(_rtn...) __alias__Clsr_Data_(_rtn)
// #define fn_use_Clsr_Data_(_rtn...) __stmt__fn_use_Clsr_Data_(_rtn)
// #define co_use_Clsr_Data_(_rtn...) __stmt__co_use_Clsr_Data_(_rtn)
#define Clsr_Ctx_(_rtn...) __alias__Clsr_Ctx_(_rtn)
// #define fn_use_Clsr_Ctx_(_rtn...) __stmt__fn_use_Clsr_Ctx_(_rtn)
// #define co_use_Clsr_Ctx_(_rtn...) __stmt__co_use_Clsr_Ctx_(_rtn)
#define Clsr_Rtn_(_rtn...) __alias__Clsr_Rtn_(_rtn)
// #define fn_use_Clsr_Rtn_(_rtn...) __stmt__fn_use_Clsr_Rtn_(_rtn)
// #define co_use_Clsr_Rtn_(_rtn...) __stmt__co_use_Clsr_Rtn_(_rtn)
#define Clsr_rtn_(_rtn...) __alias__Clsr_rtn_(_rtn)
// #define fn_use_Clsr_rtn_(_rtn...) __stmt__fn_use_Clsr_rtn_(_rtn)
// #define co_use_Clsr_rtn_(_rtn...) __stmt__co_use_Clsr_rtn_(_rtn)
#define Clsr_from_(_rtn...) __alias__Clsr_from_(_rtn)
// #define fn_use_Clsr_from_(_rtn...) __stmt__fn_use_Clsr_from_(_rtn)
// #define co_use_Clsr_from_(_rtn...) __stmt__co_use_Clsr_from_(_rtn)
/*--- Closure ---*/
#define Clsr_(_rtn...) __alias__Clsr_(_rtn)
#define clsr_(/*(_rtn)(_args...)*/... /*(Clsr)*/) __expr__clsr_(__VA_ARGS__)
#define invoke_(_p_clsr... /*-> (Clsr_Ctx)*/) __expr__invoke_(_p_clsr)
#define fn_use_Clsr_(/*(_fn)(_Arg_T...)(_Ret_T)*/...) __stmt__fn_use_Clsr_(__VA_ARGS__)
#define co_use_Clsr_(/*(_co)(_Arg_T...)(_Ret_T)*/...) __stmt__co_use_Clsr_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

/*--- Closure's Context ---*/
#define __alias__Clsr_Ctx$(_T...) tpl$(Clsr_Ctx, _T)
#define __stmt__T_use_Clsr_Ctx$(_T...) \
    T_decl_Clsr_Ctx$(_T); \
    T_impl_Clsr_Ctx$(_T)
#define __stmt__T_decl_Clsr_Ctx$(_T...) \
    T_alias$((Clsr_Ctx$(_T))(union Clsr_Ctx$(_T)))
#define __stmt__T_impl_Clsr_Ctx$(_T...) \
    union Clsr_Ctx$(_T) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, _T); \
                var_(ret_, _T) $like_ref; \
            }); \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
    }
/*--- Closure's Routine ---*/
#define __alias__Clsr_Rtn$(_T...) tpl$(Clsr_Rtn, _T)
#define __stmt__T_use_Clsr_Rtn$(_T...) \
    T_alias$((Clsr_Rtn$(_T))(fn_(((*)(P$$(Clsr_Ctx$(_T)) ctx))(P$$(Clsr_Ctx$(_T)))$T)))
/*--- Closure's Base ---*/
#define __alias__Clsr$(_T...) tpl$(Clsr, _T)
#define __stmt__T_use_Clsr$(_T...) \
    T_decl_Clsr$(_T); \
    T_impl_Clsr$(_T)
#define __stmt__T_decl_Clsr$(_T...) \
    T_alias$((Clsr$(_T))(union Clsr$(_T)))
#define __stmt__T_impl_Clsr$(_T...) \
    union Clsr$(_T) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn$(_T)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx$(_T)); \
                var_(ctx_, Clsr_Ctx$(_T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
    }
/*--- Closure's Fields ---*/
#define __alias__Clsr_Ret_(_rtn...) tpl_(Clsr_Ret, _rtn)
#define __alias__Clsr_Data_(_rtn...) tpl_(Clsr_Data, _rtn)
#define __alias__Clsr_Ctx_(_rtn...) tpl_(Clsr_Ctx, _rtn)
#define __alias__Clsr_Rtn_(_rtn...) tpl_(Clsr_Rtn, _rtn)
#define __alias__Clsr_rtn_(_rtn...) tpl_(Clsr_rtn, _rtn)
#define __alias__Clsr_from_(_rtn...) tpl_(Clsr_from, _rtn)
/*--- Closure ---*/
#define __alias__Clsr_(_rtn...) tpl_(Clsr, _rtn)
#define __alias__clsr_(_rtn...) tpl_(clsr, _rtn)
#define __expr__clsr_(...) __step__clsr___emit(__step__clsr___parseRtn __VA_ARGS__)
#define __step__clsr___parseRtn(_rtn...) _rtn,
#define __step__clsr___emit(...) __inline__clsr_(__VA_ARGS__)
#define __inline__clsr_(_rtn, _args...) copy(Clsr_from_(_rtn) _args)
#define __expr__invoke_(_p_clsr...) __inline__invoke_(pp_uniqTok(p_ctx), pp_uniqTok(p_clsr), _p_clsr)
#define __inline__invoke_(__p_ctx, __p_clsr, _p_clsr...) local_({ \
    let __p_clsr = _p_clsr; \
    claim_assert(__p_clsr->kind != Clsr_Kind_undefined); \
    let __p_ctx = __p_clsr->ctx_; \
    local_return_(ptrAlignCast$((P$$(TypeOf(*__p_ctx)))( \
        __p_clsr->rtn(ptrCast$((Clsr_Ctx$raw*)(__p_ctx))) \
    ))); \
})
/* --- Closure of Function --- */
#define Clsr_Fn_Args_(_rtn...) tpl_(Clsr_Fn_Args, _rtn)
// #define fn_use_Clsr_Fn_Args_(_rtn...) __stmt__fn_use_Clsr_Args_(_rtn)
#define __stmt__fn_use_Clsr_(...) __step__fn_use_Clsr_(__step__fn_use_Clsr___parse0 __VA_ARGS__)
#define __step__fn_use_Clsr___parse0(_fn...) \
    _fn, __step__fn_use_Clsr___parse1
#define __step__fn_use_Clsr___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__fn_use_Clsr___parse2
#define __step__fn_use_Clsr___parse2(_Ret_T...) \
    _Ret_T
#define __step__fn_use_Clsr_(...) __inline__fn_use_Clsr_(__VA_ARGS__)
#define __inline__fn_use_Clsr_(_fn, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Clsr_Ret_(_fn))(_Ret_T)); \
    T_alias$((Clsr_Fn_Args_(_fn))(Tup$$ _Arg_T)); \
    T_alias$((Clsr_Data_(_fn))(struct Clsr_Data_(_fn) { \
        var_(args, Clsr_Fn_Args_(_fn)); \
    })); \
    T_alias$((Clsr_Ctx_(_fn))(union Clsr_Ctx_(_fn) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_fn)); \
                var_(ret_, Clsr_Ret_(_fn)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_fn)); \
                var_(data_, Clsr_Data_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_fn)(P$$(Clsr_Ctx_(_fn)) ctx))(P$$(Clsr_Ctx_(_fn)))) { \
        let rtn = _fn; \
        ctx->ret = rtn(__step__fn_use_Clsr___passTupFields(_N_Arg_T, ctx->data.args.)); \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_fn))(TypeOf(Clsr_rtn_(_fn))*)); \
    T_alias$((Clsr_(_fn))(union Clsr_(_fn) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_fn)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_fn)); \
                var_(ctx_, Clsr_Ctx_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_fn)(__step__fn_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Clsr_(_fn))) { \
        return (Clsr_(_fn)){ \
            .kind = Clsr_Kind_fn, \
            .rtn = Clsr_rtn_(_fn), \
            .ctx.data.args = tie_(__step__fn_use_Clsr___passTupFieldVals(_N_Arg_T)), \
        }; \
    }
#define __step__fn_use_Clsr___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__fn_use_Clsr___passTupFields, _N_Arg_T)(_field_path)
#define __step__fn_use_Clsr___passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Clsr___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Clsr___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Clsr___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__fn_use_Clsr___passTupFields5(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4
#define __step__fn_use_Clsr___passTupFields6(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5
#define __step__fn_use_Clsr___passTupFields7(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6
#define __step__fn_use_Clsr___passTupFields8(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6, _field_path $7
#define __step__fn_use_Clsr___passTupFieldVals(_N_Arg_T, _field_path...) \
    pp_cat(__step__fn_use_Clsr___passTupFieldVals, _N_Arg_T)(_field_path)
#define __step__fn_use_Clsr___passTupFieldVals1(_field_path...) \
    (_field_path $0)
#define __step__fn_use_Clsr___passTupFieldVals2(_field_path...) \
    (_field_path $0), (_field_path $1)
#define __step__fn_use_Clsr___passTupFieldVals3(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2)
#define __step__fn_use_Clsr___passTupFieldVals4(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3)
#define __step__fn_use_Clsr___passTupFieldVals5(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4)
#define __step__fn_use_Clsr___passTupFieldVals6(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5)
#define __step__fn_use_Clsr___passTupFieldVals7(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5), (_field_path $6)
#define __step__fn_use_Clsr___passTupFieldVals8(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5), (_field_path $6), (_field_path $7)
#define __step__fn_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__fn_use_Clsr___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__fn_use_Clsr___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__fn_use_Clsr___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__fn_use_Clsr___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__fn_use_Clsr___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3
#define __step__fn_use_Clsr___tupFieldsToParams5(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4
#define __step__fn_use_Clsr___tupFieldsToParams6(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5
#define __step__fn_use_Clsr___tupFieldsToParams7(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6
#define __step__fn_use_Clsr___tupFieldsToParams8(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7, _Arg_T8...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6, _Arg_T8 $7
/* --- Closure of Coroutine --- */
#define Clsr_Co_Frame_(_rtn...) tpl_(Clsr_Co_Frame, _rtn)
// #define co_use_Clsr_Co_Frame_(_rtn...) __stmt__co_use_Clsr_Co_Frame_(_rtn)
#define __stmt__co_use_Clsr_(...) __step__co_use_Clsr_(__step__co_use_Clsr___parse0 __VA_ARGS__)
#define __step__co_use_Clsr___parse0(_co...) \
    _co, __step__co_use_Clsr___parse1
#define __step__co_use_Clsr___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__co_use_Clsr___parse2
#define __step__co_use_Clsr___parse2(_Ret_T...) \
    _Ret_T
#define __step__co_use_Clsr_(...) __inline__co_use_Clsr_(__VA_ARGS__)
#define __inline__co_use_Clsr_(_co, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Clsr_Ret_(_co))(_Ret_T)); \
    T_alias$((Clsr_Co_Frame_(_co))(Co_Frame_(_co))); \
    T_alias$((Clsr_Data_(_co))(struct Clsr_Data_(_co) { \
        var_(frame, Clsr_Co_Frame_(_co)); \
    })); \
    T_alias$((Clsr_Ctx_(_co))(union Clsr_Ctx_(_co) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Clsr_Ret_(_co)); \
                var_(ret_, Clsr_Ret_(_co)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Clsr_Data_(_co)); \
                var_(data_, Clsr_Data_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr_Ctx$raw) $flexible; \
        var_(as_base, Clsr_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_rtn_(_co)(P$$(Clsr_Ctx_(_co)) ctx))(P$$(Clsr_Ctx_(_co)))) { \
        let frame = &ctx->data.frame; \
        let_ignore = resume_(frame); \
        if (frame->ctx.ctrl.state == Co_State_ready) { \
            ctx->ret = frame->ctx.ret; \
        } \
        return ctx; \
    }; \
    T_alias$((Clsr_Rtn_(_co))(TypeOf(Clsr_rtn_(_co))*)); \
    T_alias$((Clsr_(_co))(union Clsr_(_co) { \
        T_embed$(struct { \
            var_(kind, Clsr_Kind); \
            var_(rtn, Clsr_Rtn_(_co)); \
            T_embed$(union { \
                var_(ctx, Clsr_Ctx_(_co)); \
                var_(ctx_, Clsr_Ctx_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Clsr$raw) $flexible; \
        var_(as_base, Clsr$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Clsr_from_(_co)(__step__co_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Clsr_(_co))) { \
        return (Clsr_(_co)){ \
            .kind = Clsr_Kind_co, \
            .rtn = Clsr_rtn_(_co), \
            .ctx.data.frame = { \
                .rtn = _co, \
                .ctx = { \
                    .ctrl = { \
                        .state = Co_State_pending, \
                        .line = 0, \
                    }, \
                    .suspended_data = {}, \
                    .args = tie_(__step__co_use_Clsr___passTupFieldVals(_N_Arg_T)), \
                    .data = { \
                        .suspended = {}, \
                        .locals = {}, \
                        .locals_mut = {}, \
                    }, \
                }, \
            }, \
        }; \
    }
#define __step__co_use_Clsr___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__co_use_Clsr___passTupFields, _N_Arg_T)(_field_path)
#define __step__co_use_Clsr___passTupFields1(_field_path...) \
    _field_path $0
#define __step__co_use_Clsr___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__co_use_Clsr___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__co_use_Clsr___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__co_use_Clsr___passTupFields5(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4
#define __step__co_use_Clsr___passTupFields6(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5
#define __step__co_use_Clsr___passTupFields7(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6
#define __step__co_use_Clsr___passTupFields8(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3, _field_path $4, _field_path $5, _field_path $6, _field_path $7
#define __step__co_use_Clsr___passTupFieldVals(_N_Arg_T, _field_path...) \
    pp_cat(__step__co_use_Clsr___passTupFieldVals, _N_Arg_T)(_field_path)
#define __step__co_use_Clsr___passTupFieldVals1(_field_path...) \
    (_field_path $0)
#define __step__co_use_Clsr___passTupFieldVals2(_field_path...) \
    (_field_path $0), (_field_path $1)
#define __step__co_use_Clsr___passTupFieldVals3(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2)
#define __step__co_use_Clsr___passTupFieldVals4(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3)
#define __step__co_use_Clsr___passTupFieldVals5(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4)
#define __step__co_use_Clsr___passTupFieldVals6(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5)
#define __step__co_use_Clsr___passTupFieldVals7(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5), (_field_path $6)
#define __step__co_use_Clsr___passTupFieldVals8(_field_path...) \
    (_field_path $0), (_field_path $1), (_field_path $2), (_field_path $3), (_field_path $4), (_field_path $5), (_field_path $6), (_field_path $7)
#define __step__co_use_Clsr___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__co_use_Clsr___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__co_use_Clsr___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__co_use_Clsr___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__co_use_Clsr___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__co_use_Clsr___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3
#define __step__co_use_Clsr___tupFieldsToParams5(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4
#define __step__co_use_Clsr___tupFieldsToParams6(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5
#define __step__co_use_Clsr___tupFieldsToParams7(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6
#define __step__co_use_Clsr___tupFieldsToParams8(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4, _Arg_T5, _Arg_T6, _Arg_T7, _Arg_T8...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3, _Arg_T5 $4, _Arg_T6 $5, _Arg_T7 $6, _Arg_T8 $7

T_use_Clsr_Ctx$(Void);
T_use_Clsr_Rtn$(Void);
T_use_Clsr$(Void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Clsr_included */
