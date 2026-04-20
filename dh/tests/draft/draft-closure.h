#ifndef draft_closure__included
#define draft_closure__included 1

#include "dh/prl.h"
#include "draft-tup.h"

typedef V$raw FnCtx$raw;
typedef fn_(((*Fn$raw)(FnCtx$raw* ctx))(FnCtx$raw*));
typedef struct Closure$raw {
    Fn$raw fn;
    FnCtx$raw ctx $flexible;
} Closure$raw;

#define FnArgs$(_fn...) __alias__FnArgs$(_fn)
#define __alias__FnArgs$(_fn...) pp_join($, FnArgs, _fn)
#define FnRet$(_fn...) __alias__FnRet$(_fn)
#define __alias__FnRet$(_fn...) pp_join($, FnRet, _fn)
#define FnCtx$(_fn...) __alias__FnCtx$(_fn)
#define __alias__FnCtx$(_fn...) pp_join($, FnCtx, _fn)
#define fn$(_fn...) __alias__fn$(_fn)
#define __alias__fn$(_fn...) pp_join($, fn, _fn)
#define Closure$(_fn...) __alias__Closure$(_fn)
#define __alias__Closure$(_fn...) pp_join($, Closure, _fn)
#define closure$(_fn...) __alias__closure$(_fn)
#define __alias__closure$(_fn...) pp_join($, closure, _fn)
#define fn_use_Closure$(/*(_fn)(_Arg_Type...)(_Ret_Type)*/...) __stmt__fn_use_Closure$(__VA_ARGS__)
#define __stmt__fn_use_Closure$(...) __step__fn_use_Closure$(__step__fn_use_Closure$__parse0 __VA_ARGS__)
#define __step__fn_use_Closure$__parse0(_fn...) \
    _fn, __step__fn_use_Closure$__parse1
#define __step__fn_use_Closure$__parse1(_Arg_Type...) \
    pp_countArg(_Arg_Type), (_Arg_Type), __step__fn_use_Closure$__parse2
#define __step__fn_use_Closure$__parse2(_Ret_Type...) \
    _Ret_Type
#define __step__fn_use_Closure$(...) __inline__fn_use_Closure$(__VA_ARGS__)
#define __inline__fn_use_Closure$(_fn, _N_Arg_Type, _Arg_Type, _Ret_Type...) \
    T_alias$((FnArgs$(_fn))(Tup$$ _Arg_Type)); \
    T_alias$((FnRet$(_fn))(_Ret_Type)); \
    T_alias$((FnCtx$(_fn))(union FnCtx$(_fn) { \
        T_embed$(struct { \
            var_(args, FnArgs$(_fn)); \
            var_(ret, FnRet$(_fn)); \
        }); \
        var_(as_raw, FnCtx$raw) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((fn$(_fn)(P$$(FnCtx$raw) ctx))(P$$(FnCtx$raw))) { \
        let fn = _fn; \
        let fn_ctx = ptrAlignCast$((FnCtx$(_fn)*)(ctx)); \
        fn_ctx->ret = fn(__step__fn_use_Closure$__passTupFields(_N_Arg_Type, fn_ctx->args.)); \
        return fn_ctx->as_raw; \
    }; \
    T_alias$((Closure$(_fn))(union Closure$(_fn) { \
        T_embed$(struct { \
            var_(fn, Fn$raw); \
            var_(ctx, FnCtx$(_fn)) $like_ref; \
        }); \
        var_(as_raw, Closure$raw) $flexible; \
    })); \
    $attr($inline_always $static) \
    fn_((closure$(_fn)(__step__fn_use_Closure$__tupFieldsToParams(_N_Arg_Type, _Arg_Type)))(Closure$(_fn))) { \
        return (Closure$(_fn)){ \
            .fn = fn$(_fn), \
            .ctx $like_deref = { .args = { __step__fn_use_Closure$__passTupFields(_N_Arg_Type) } } \
        }; \
    };

#define __step__fn_use_Closure$__passTupFields(_N_Arg_Type, _field_path...) \
    pp_cat(__step__fn_use_Closure$__passTupFields, _N_Arg_Type)(_field_path)
#define __step__fn_use_Closure$__passTupFields1(_field_path...) \
    _field_path $0
#define __step__fn_use_Closure$__passTupFields2(_field_path...) \
    _field_path $0, _field_path $1
#define __step__fn_use_Closure$__passTupFields3(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2
#define __step__fn_use_Closure$__passTupFields4(_field_path...) \
    _field_path $0, _field_path $1, _field_path $2, _field_path $3

#define __step__fn_use_Closure$__tupFieldsToParams(_N_Arg_Type, _Arg_Type...) \
    pp_cat(__step__fn_use_Closure$__tupFieldsToParams, _N_Arg_Type) _Arg_Type
#define __step__fn_use_Closure$__tupFieldsToParams1(_Arg_Type1...) \
    _Arg_Type1 $0
#define __step__fn_use_Closure$__tupFieldsToParams2(_Arg_Type1, _Arg_Type2...) \
    _Arg_Type1 $0, _Arg_Type2 $1
#define __step__fn_use_Closure$__tupFieldsToParams3(_Arg_Type1, _Arg_Type2, _Arg_Type3...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2
#define __step__fn_use_Closure$__tupFieldsToParams4(_Arg_Type1, _Arg_Type2, _Arg_Type3, _Arg_Type4...) \
    _Arg_Type1 $0, _Arg_Type2 $1, _Arg_Type3 $2, _Arg_Type4 $3

#define invoke(_p_closure...) __expr__callClosure$(_p_closure)
#define __expr__callClosure$(_p_closure...) __inline__callClosure$(pp_uniqTok(p_fn_handle), _p_closure)
#define __inline__callClosure$(__p_closure, _p_closure...) local_({ \
    let __p_closure = _p_closure; \
    ptrAlignCast$((FieldType$(TypeOf(*__p_closure), ctx $like_deref)*)(__p_closure->fn(as$(FnCtx$raw*)(__p_closure->ctx)))); \
})

#endif /* draft_closure__included */
