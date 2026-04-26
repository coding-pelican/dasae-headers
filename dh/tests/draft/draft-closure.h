#ifndef Closure_included
#define Closure_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "draft-tup.h"
#include "draft-Co.h"

/*========== Macros and Declarations ========================================*/

/*--- Closure Related ---*/
T_alias$((exec_Seq)(struct exec_Seq));
T_alias$((exec_Coop)(struct exec_Coop));
T_alias$((exec_Preem)(struct exec_Preem));
T_alias$((exec_Para)(struct exec_Para));
T_alias$((Sched)(struct Sched));
T_alias$((Future$raw)(struct Future$raw));
T_use_P$(Future$raw);
T_use_O$(P$Future$raw);
T_alias$((V$Future$raw)(P$Future$raw));
/*--- Closure's Context ---*/
#define Closure_Ctx$(_T...) __alias__Closure_Ctx$(_T)
T_alias$((Closure_Ctx$raw)(struct Closure_Ctx$raw {
    var_(ret_, V$raw) $flexible;
    var_(data_, V$raw) $flexible;
}));
#define T_use_Closure_Ctx$(_T...) __stmt__T_use_Closure_Ctx$(_T)
#define T_decl_Closure_Ctx$(_T...) __stmt__T_decl_Closure_Ctx$(_T)
#define T_impl_Closure_Ctx$(_T...) __stmt__T_impl_Closure_Ctx$(_T)
/*--- Closure's Routine ---*/
#define Closure_Rtn$(_T...) __alias__Closure_Rtn$(_T)
T_alias$((Closure_Rtn)(fn_(((*)(P$$(Closure_Ctx$raw) ctx))(P$$(Closure_Ctx$raw)) $T)));
#define T_use_Closure_Rtn$(_T...) __stmt__T_use_Closure_Rtn$(_T)
/*--- Closure's Kind ---*/
T_alias$((Closure_Kind)(enum_((Closure_Kind $fits($packed))(
    Closure_Kind_undefined = 0,
    Closure_Kind_fn = 1,
    Closure_Kind_co = 2,
))));
claim_assert_static(eqlType$(Closure_Kind, u8));
/*--- Closure's Base ---*/
#define Closure$(_T...) __alias__Closure$(_T)
T_alias$((Closure$raw)(struct Closure$raw {
    T_embed$(struct {
        var_(kind, Closure_Kind);
        var_(rtn, Closure_Rtn);
    });
    var_(ctx_, Closure_Ctx$raw) $flexible;
}));
#define T_use_Closure$(_T...) __stmt__T_use_Closure$(_T)
#define decl_Closure$(_T...) __stmt__decl_Closure$(_T)
#define impl_Closure$(_T...) __stmt__impl_Closure$(_T)
/*--- Closure's Fields ---*/
#define Closure_Ret_(_rtn...) __alias__Closure_Ret_(_rtn)
// #define fn_use_Closure_Ret_(_rtn...) __stmt__fn_use_Closure_Ret_(_rtn)
// #define co_use_Closure_Ret_(_rtn...) __stmt__co_use_Closure_Ret_(_rtn)
#define Closure_Data_(_rtn...) __alias__Closure_Data_(_rtn)
// #define fn_use_Closure_Data_(_rtn...) __stmt__fn_use_Closure_Data_(_rtn)
// #define co_use_Closure_Data_(_rtn...) __stmt__co_use_Closure_Data_(_rtn)
#define Closure_Ctx_(_rtn...) __alias__Closure_Ctx_(_rtn)
// #define fn_use_Closure_Ctx_(_rtn...) __stmt__fn_use_Closure_Ctx_(_rtn)
// #define co_use_Closure_Ctx_(_rtn...) __stmt__co_use_Closure_Ctx_(_rtn)
#define Closure_Rtn_(_rtn...) __alias__Closure_Rtn_(_rtn)
// #define fn_use_Closure_Rtn_(_rtn...) __stmt__fn_use_Closure_Rtn_(_rtn)
// #define co_use_Closure_Rtn_(_rtn...) __stmt__co_use_Closure_Rtn_(_rtn)
#define Closure_rtn_(_rtn...) __alias__Closure_rtn_(_rtn)
// #define fn_use_Closure_rtn_(_rtn...) __stmt__fn_use_Closure_rtn_(_rtn)
// #define co_use_Closure_rtn_(_rtn...) __stmt__co_use_Closure_rtn_(_rtn)
/*--- Closure ---*/
#define Closure_(_rtn...) __alias__Closure_(_rtn)
#define closure_(_rtn...) __alias__closure_(_rtn)
#define invoke_(_p_closure...) __expr__invoke_(_p_closure)
#define fn_use_Closure_(/*(_fn)(_Arg_T...)(_Ret_T)*/...) __stmt__fn_use_Closure_(__VA_ARGS__)
#define co_use_Closure_(/*(_co)(_Arg_T...)(_Ret_T)*/...) __stmt__co_use_Closure_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

/*--- Closure's Context ---*/
#define __alias__Closure_Ctx$(_T...) tpl$(Closure_Ctx, _T)
#define __stmt__T_use_Closure_Ctx$(_T...) \
    T_decl_Closure_Ctx$(_T); \
    T_impl_Closure_Ctx$(_T)
#define __stmt__T_decl_Closure_Ctx$(_T...) \
    T_alias$((Closure_Ctx$(_T))(union Closure_Ctx$(_T)))
#define __stmt__T_impl_Closure_Ctx$(_T...) \
    union Closure_Ctx$(_T) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, _T); \
                var_(ret_, _T) $like_ref; \
            }); \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Closure_Ctx$raw) $flexible; \
    }
/*--- Closure's Routine ---*/
#define __alias__Closure_Rtn$(_T...) tpl$(Closure_Rtn, _T)
#define __stmt__T_use_Closure_Rtn$(_T...) \
    T_alias$((Closure_Rtn$(_T))(fn_(((*)(P$$(Closure_Ctx$(_T)) ctx))(P$$(Closure_Ctx$(_T)))$T)))
/*--- Closure's Base ---*/
#define __alias__Closure$(_T...) tpl$(Closure, _T)
#define __stmt__T_use_Closure$(_T...) \
    decl_Closure$(_T); \
    impl_Closure$(_T)
#define __stmt__decl_Closure$(_T...) \
    T_alias$((Closure$(_T))(union Closure$(_T)))
#define __stmt__impl_Closure$(_T...) \
    union Closure$(_T) { \
        T_embed$(struct { \
            var_(kind, Closure_Kind); \
            var_(rtn, Closure_Rtn$(_T)); \
            T_embed$(union { \
                var_(ctx, Closure_Ctx$(_T)); \
                var_(ctx_, Closure_Ctx$(_T)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure$raw) $flexible; \
    }
/*--- Closure's Fields ---*/
#define __alias__Closure_Ret_(_rtn...) tpl_(Closure_Ret, _rtn)
#define __alias__Closure_Data_(_rtn...) tpl_(Closure_Data, _rtn)
#define __alias__Closure_Ctx_(_rtn...) tpl_(Closure_Ctx, _rtn)
#define __alias__Closure_Rtn_(_rtn...) tpl_(Closure_Rtn, _rtn)
#define __alias__Closure_rtn_(_rtn...) tpl_(Closure_rtn, _rtn)
/*--- Closure ---*/
#define __alias__Closure_(_rtn...) tpl_(Closure, _rtn)
#define __alias__closure_(_rtn...) tpl_(closure, _rtn)
#define __expr__invoke_(_p_closure...) __inline__invoke_(pp_uniqTok(p_ctx), pp_uniqTok(p_closure), _p_closure)
#define __inline__invoke_(__p_ctx, __p_closure, _p_closure...) local_({ \
    let __p_closure = _p_closure; \
    claim_assert(__p_closure->kind != Closure_Kind_undefined); \
    let __p_ctx = __p_closure->ctx_; \
    local_return_(ptrAlignCast$((P$$(TypeOf(*__p_ctx)))( \
        __p_closure->rtn(ptrCast$((Closure_Ctx$raw*)(__p_ctx))) \
    ))); \
})
/* --- Closure of Function --- */
#define Closure_Fn_Args_(_rtn...) tpl_(Closure_Fn_Args, _rtn)
// #define fn_use_Closure_Fn_Args_(_rtn...) __stmt__fn_use_Closure_Args_(_rtn)
#define __stmt__fn_use_Closure_(...) __step__fn_use_Closure_(__step__fn_use_Closure___parse0 __VA_ARGS__)
#define __step__fn_use_Closure___parse0(_fn...) \
    _fn, __step__fn_use_Closure___parse1
#define __step__fn_use_Closure___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__fn_use_Closure___parse2
#define __step__fn_use_Closure___parse2(_Ret_T...) \
    _Ret_T
#define __step__fn_use_Closure_(...) __inline__fn_use_Closure_(__VA_ARGS__)
#define __inline__fn_use_Closure_(_fn, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Closure_Ret_(_fn))(_Ret_T)); \
    T_alias$((Closure_Fn_Args_(_fn))(Tup$$ _Arg_T)); \
    T_alias$((Closure_Data_(_fn))(struct Closure_Data_(_fn) { \
        var_(args, Closure_Fn_Args_(_fn)); \
    })); \
    T_alias$((Closure_Ctx_(_fn))(union Closure_Ctx_(_fn) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Closure_Ret_(_fn)); \
                var_(ret_, Closure_Ret_(_fn)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Closure_Data_(_fn)); \
                var_(data_, Closure_Data_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure_Ctx$raw) $flexible; \
        var_(as_base, Closure_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Closure_rtn_(_fn)(P$$(Closure_Ctx_(_fn)) ctx))(P$$(Closure_Ctx_(_fn)))) { \
        let rtn = _fn; \
        ctx->ret = rtn(__step__fn_use_Closure___passTupFields(_N_Arg_T, ctx->data.args.)); \
        return ctx; \
    }; \
    T_alias$((Closure_Rtn_(_fn))(TypeOf(Closure_rtn_(_fn))*)); \
    T_alias$((Closure_(_fn))(union Closure_(_fn) { \
        T_embed$(struct { \
            var_(kind, Closure_Kind); \
            var_(rtn, Closure_Rtn_(_fn)); \
            T_embed$(union { \
                var_(ctx, Closure_Ctx_(_fn)); \
                var_(ctx_, Closure_Ctx_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure$raw) $flexible; \
        var_(as_base, Closure$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((closure_(_fn)(__step__fn_use_Closure___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Closure_(_fn))) { \
        return (Closure_(_fn)){ \
            .kind = Closure_Kind_fn, \
            .rtn = Closure_rtn_(_fn), \
            .ctx.data.args = { __step__fn_use_Closure___passTupFields(_N_Arg_T) }, \
        }; \
    }
#define __step__fn_use_Closure___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__fn_use_Closure___passTupFields, _N_Arg_T)(_field_path)
#define __step__fn_use_Closure___passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Closure___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Closure___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Closure___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__fn_use_Closure___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__fn_use_Closure___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__fn_use_Closure___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__fn_use_Closure___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__fn_use_Closure___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__fn_use_Closure___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3
/* --- Closure of Coroutine --- */
#define Closure_Co_Frame_(_rtn...) tpl_(Closure_Co_Frame, _rtn)
// #define co_use_Closure_Co_Frame_(_rtn...) __stmt__co_use_Closure_Co_Frame_(_rtn)
#define __stmt__co_use_Closure_(...) __step__co_use_Closure_(__step__co_use_Closure___parse0 __VA_ARGS__)
#define __step__co_use_Closure___parse0(_co...) \
    _co, __step__co_use_Closure___parse1
#define __step__co_use_Closure___parse1(_Arg_T...) \
    pp_countArg(_Arg_T), (_Arg_T), __step__co_use_Closure___parse2
#define __step__co_use_Closure___parse2(_Ret_T...) \
    _Ret_T
#define __step__co_use_Closure_(...) __inline__co_use_Closure_(__VA_ARGS__)
#define __inline__co_use_Closure_(_co, _N_Arg_T, _Arg_T, _Ret_T...) \
    T_alias$((Closure_Ret_(_co))(_Ret_T)); \
    T_alias$((Closure_Co_Frame_(_co))(Co_Frame_(_co))); \
    T_alias$((Closure_Data_(_co))(struct Closure_Data_(_co) { \
        var_(frame, Closure_Co_Frame_(_co)); \
    })); \
    T_alias$((Closure_Ctx_(_co))(union Closure_Ctx_(_co) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Closure_Ret_(_co)); \
                var_(ret_, Closure_Ret_(_co)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(data, Closure_Data_(_co)); \
                var_(data_, Closure_Data_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure_Ctx$raw) $flexible; \
        var_(as_base, Closure_Ctx$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Closure_rtn_(_co)(P$$(Closure_Ctx_(_co)) ctx))(P$$(Closure_Ctx_(_co)))) { \
        let frame = &ctx->data.frame; \
        let_ignore = resume_(frame); \
        ctx->ret = frame->ctx_->ret; \
        return ctx; \
    }; \
    T_alias$((Closure_Rtn_(_co))(TypeOf(Closure_rtn_(_co))*)); \
    T_alias$((Closure_(_co))(union Closure_(_co) { \
        T_embed$(struct { \
            var_(kind, Closure_Kind); \
            var_(rtn, Closure_Rtn_(_co)); \
            T_embed$(union { \
                var_(ctx, Closure_Ctx_(_co)); \
                var_(ctx_, Closure_Ctx_(_co)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure$raw) $flexible; \
        var_(as_base, Closure$(_Ret_T)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((closure_(_co)(__step__co_use_Closure___tupFieldsToParams(_N_Arg_T, _Arg_T)))(Closure_(_co))) { \
        return (Closure_(_co)){ \
            .kind = Closure_Kind_co, \
            .rtn = Closure_rtn_(_co), \
            .ctx.data.frame = { \
                .rtn = _co, \
                .ctx = { \
                    .ctrl = { \
                        .state = Co_State_pending, \
                        .count = 0, \
                    }, \
                    .suspended_data = {}, \
                    .args = { __step__co_use_Closure___passTupFields(_N_Arg_T) }, \
                    .data = { \
                        .suspended = {}, \
                        .locals = {}, \
                        .locals_mut = {}, \
                    }, \
                }, \
            }, \
        }; \
    }
#define __step__co_use_Closure___passTupFields(_N_Arg_T, _field_path...) \
    pp_cat(__step__co_use_Closure___passTupFields, _N_Arg_T)(_field_path)
#define __step__co_use_Closure___passTupFields1(_field_path...) \
    _field_path $0
#define __step__co_use_Closure___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__co_use_Closure___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__co_use_Closure___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3
#define __step__co_use_Closure___tupFieldsToParams(_N_Arg_T, _Arg_T...) \
    pp_cat(__step__co_use_Closure___tupFieldsToParams, _N_Arg_T) _Arg_T
#define __step__co_use_Closure___tupFieldsToParams1(_Arg_T1...) \
    _Arg_T1 $0
#define __step__co_use_Closure___tupFieldsToParams2(_Arg_T1, _Arg_T2...) \
    _Arg_T1 $0, _Arg_T2 $1
#define __step__co_use_Closure___tupFieldsToParams3(_Arg_T1, _Arg_T2, _Arg_T3...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2
#define __step__co_use_Closure___tupFieldsToParams4(_Arg_T1, _Arg_T2, _Arg_T3, _Arg_T4...) \
    _Arg_T1 $0, _Arg_T2 $1, _Arg_T3 $2, _Arg_T4 $3

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Closure_included */
