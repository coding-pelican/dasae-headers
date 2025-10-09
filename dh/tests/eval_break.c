#include "dh/main.h"

// clang-format off
#define eval_(T_Break_w_Ext...) comp_syn__eval_test(T_Break_w_Ext)
#define comp_syn__eval_test(T_Break, _Ext...) pp_cat(comp_syn__eval_, _Ext)(T_Break)
#define comp_syn__eval_$_scope(T_Break...) ({ \
    local_label __step_break, __step_unscope; \
    let __reserved_break = as$(T_Break*, (u8[_Generic(T_Break, \
        void: 0, \
        default: sizeOf$(T_Break) \
    )]){}); \
    if (false) { __step_break: goto __step_unscope; } \
    do
#define $unscoped_eval comp_syn__eval_$unscoped
#define comp_syn__eval_$unscoped \
    while (false); \
__step_unscope: \
    _Generic(TypeOf(*__reserved_break), \
        void: ({}), \
        default: __reserved_break[0] \
    ); \
})
#define eval_break_(_Expr...) comp_syn__eval_break_(_Expr)
#define comp_syn__eval_break_(_Expr...) eval({ \
    bti_memcpy( \
        as$(u8*, __reserved_break), \
        as$(u8*, (TypeOf (*__reserved_break)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_break) \
    ); \
    goto __step_break; \
})
// clang-format on

func((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    $ignore = args;
    return_(eval_(Err$void $scope) if (true) {
        eval_break_(ok({}));
    } else {
        eval_break_(err(Err_NotImplemented()));
    } $unscoped_eval);
} $unscoped_(fn);
