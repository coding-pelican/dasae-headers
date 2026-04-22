#ifndef draft_closure__included
#define draft_closure__included 1

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "draft-Co.h"
#include "draft-tup.h"

/*========== Macros and Declarations ========================================*/

/*--- Closure's Context ---*/
#define Closure_Ctx$(_T...) __alias__Closure_Ctx$(_T)
T_alias$((Closure_Ctx$raw)(struct Closure_Ctx$raw {
    var_(ret_, V$raw) $flexible;
    var_(args_, V$raw) $flexible;
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
/*--- Closure ---*/
#define Closure_(_rtn...) __alias__Closure_(_rtn)
#define fn_use_Closure_(/*(_fn)(_Arg_Type...)(_Ret_Type)*/...) __stmt__fn_use_Closure_(__VA_ARGS__)
#define co_use_Closure_(/*(_co)(_Arg_Type...)(_Ret_Type)*/...) __stmt__co_use_Closure_(__VA_ARGS__)
#define closure_(_rtn...) __alias__closure_(_rtn)
#define invoke_(_p_closure...) __expr__invoke_(_p_closure)

/*========== Macros and Definitions =========================================*/

#define Closure__Args_(_rtn...) __alias__Closure__Args_(_rtn)
#define fn_use_Closure__Args_(_rtn...) __stmt__fn_use_Closure__Args_(_rtn)
#define Closure__Ret_(_rtn...) __alias__Closure__Ret_(_rtn)
#define fn_use_Closure__Ret_(_rtn...) __stmt__fn_use_Closure__Ret_(_rtn)
#define Closure__Ctx_(_rtn...) __alias__Closure__Ctx_(_rtn)
#define fn_use_Closure__Ctx_(_rtn...) __stmt__fn_use_Closure__Ctx_(_rtn)
#define Closure__Rtn_(_rtn...) __alias__Closure__Rtn_(_rtn)
#define fn_use_Closure__Rtn_(_rtn...) __stmt__fn_use_Closure__Rtn_(_rtn)
#define Closure__rtn_(_rtn...) __alias__Closure__rtn_(_rtn)
#define fn_use_Closure__rtn_(_rtn...) __stmt__fn_use_Closure__rtn_(_rtn)

#define __alias__Closure_Ctx$(_T...) tpl_id$T(Closure_Ctx, _T)
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
            var_(args_, V$raw) $flexible; \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Closure_Ctx$raw) $flexible; \
    }

#define __alias__Closure_Rtn$(_T...) tpl_id$T(Closure_Rtn, _T)
#define __stmt__T_use_Closure_Rtn$(_T...) \
    T_alias$((Closure_Rtn$(_T))(fn_(((*)(P$$(Closure_Ctx$(_T)) ctx))(P$$(Closure_Ctx$(_T)))$T)))

#define __alias__Closure$(_T...) tpl_id$T(Closure, _T)
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

#define __alias__Closure_(_rtn...) pp_join(_, Closure, _rtn)

#define __stmt__fn_use_Closure_(...) __step__fn_use_Closure_(__step__fn_use_Closure___parse0 __VA_ARGS__)
#define __step__fn_use_Closure___parse0(_fn...) \
    _fn, __step__fn_use_Closure___parse1
#define __step__fn_use_Closure___parse1(_Arg_Type...) \
    pp_countArg(_Arg_Type), (_Arg_Type), __step__fn_use_Closure___parse2
#define __step__fn_use_Closure___parse2(_Ret_Type...) \
    _Ret_Type
#define __step__fn_use_Closure_(...) __inline__fn_use_Closure_(__VA_ARGS__)
#define __inline__fn_use_Closure_(_fn, _N_Arg_Type, _Arg_Type, _Ret_Type...) \
    T_alias$((Closure__Ret_(_fn))(_Ret_Type)); \
    T_alias$((Closure__Args_(_fn))(Tup$$ _Arg_Type)); \
    T_alias$((Closure__Ctx_(_fn))(union Closure__Ctx_(_fn) { \
        T_embed$(struct { \
            T_embed$(union { \
                var_(ret, Closure__Ret_(_fn)); \
                var_(ret_, Closure__Ret_(_fn)) $like_ref; \
            }); \
            T_embed$(union { \
                var_(args, Closure__Args_(_fn)); \
                var_(args_, Closure__Args_(_fn)) $like_ref; \
            }); \
            var_(data_, V$raw) $flexible; \
        }); \
        var_(as_raw, Closure_Ctx$raw) $flexible; \
        var_(as_base, Closure_Ctx$(_Ret_Type)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((Closure__rtn_(_fn)(P$$(Closure__Ctx_(_fn)) ctx))(P$$(Closure__Ctx_(_fn)))) { \
        let fn_rtn = _fn; \
        ctx->ret = fn_rtn(__step__fn_use_Closure___passTupFields(_N_Arg_Type, ctx->args.)); \
        return ctx; \
    }; \
    T_alias$((Closure__Rtn_(_fn))(TypeOf(Closure__rtn_(_fn))*)); \
    T_alias$((Closure_(_fn))(union Closure_(_fn) { \
        T_embed$(struct { \
            var_(kind, Closure_Kind); \
            var_(rtn, Closure__Rtn_(_fn)); \
            T_embed$(union { \
                var_(ctx, Closure__Ctx_(_fn)); \
                var_(ctx_, Closure__Ctx_(_fn)) $like_ref; \
            }); \
        }); \
        var_(as_raw, Closure$raw) $flexible; \
        var_(as_base, Closure$(_Ret_Type)) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((closure_(_fn)(__step__fn_use_Closure___tupFieldsToParams(_N_Arg_Type, _Arg_Type)))(Closure_(_fn))) { \
        return (Closure_(_fn)){ \
            .kind = Closure_Kind_fn, \
            .rtn = Closure__rtn_(_fn), \
            .ctx = { .args = { __step__fn_use_Closure___passTupFields(_N_Arg_Type) } } \
        }; \
    };

#define __step__fn_use_Closure___passTupFields(_N_Arg_Type, _field_path...) \
    pp_cat(__step__fn_use_Closure___passTupFields, _N_Arg_Type)(_field_path)
#define __step__fn_use_Closure___passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Closure___passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Closure___passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Closure___passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3

#define __step__fn_use_Closure___tupFieldsToParams(_N_Arg_Type, _Arg_Type...) \
    pp_cat(__step__fn_use_Closure___tupFieldsToParams, _N_Arg_Type) _Arg_Type
#define __step__fn_use_Closure___tupFieldsToParams1(_Arg_Type1...) \
    _Arg_Type1 $0
#define __step__fn_use_Closure___tupFieldsToParams2(_Arg_Type1, _Arg_Type2...) \
    _Arg_Type1 $0, _Arg_Type2 $1
#define __step__fn_use_Closure___tupFieldsToParams3(_Arg_Type1, _Arg_Type2, _Arg_Type3...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2
#define __step__fn_use_Closure___tupFieldsToParams4(_Arg_Type1, _Arg_Type2, _Arg_Type3, _Arg_Type4...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2, _Arg_Type4 $3

#define __alias__closure_(_rtn...) pp_join(_, closure, _rtn)
#define __expr__invoke_(_p_closure...) __inline__invoke_(pp_uniqTok(p_ctx), pp_uniqTok(p_closure), _p_closure)
#define __inline__invoke_(__p_ctx, __p_closure, _p_closure...) local_({ \
    let __p_closure = _p_closure; \
    claim_assert(__p_closure->kind != Closure_Kind_undefined); \
    let __p_ctx = __p_closure->ctx_; \
    local_return_(ptrAlignCast$((P$$(TypeOf(*__p_ctx)))( \
        __p_closure->rtn(ptrCast$((Closure_Ctx$raw*)(__p_ctx))) \
    ))); \
})

#define __alias__Closure__Args_(_rtn...) pp_join(_, Closure__Args, _rtn)
#define __alias__Closure__Ret_(_rtn...) pp_join(_, Closure__Ret, _rtn)
#define __alias__Closure__Ctx_(_rtn...) pp_join(_, Closure__Ctx, _rtn)
#define __alias__Closure__Rtn_(_rtn...) pp_join(_, Closure__Rtn, _rtn)
#define __alias__Closure__rtn_(_rtn...) pp_join(_, Closure__rtn, _rtn)

#endif /* draft_closure__included */
