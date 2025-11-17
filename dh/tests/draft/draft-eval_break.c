#include "dh/main.h"

#if UNUSED_CODE
// clang-format off
#define expr_(T_Break_w_Ext...) comp_syn__expr_test(T_Break_w_Ext)
#define comp_syn__expr_test(T_Break, _Ext...) pp_cat(comp_syn__expr_, _Ext)(T_Break)
#define comp_syn__expr_$_scope(T_Break...) ({ \
    $local_label __step_break, __step_unscope; \
    let __reserved_break = as$(T_Break*)(((u8[_Generic(T_Break, \
        void: 0, \
        default: sizeOf$(T_Break) \
    )]){})); \
    if (false) { __step_break: goto __step_unscope; } \
    do
#define $unscoped_eval comp_syn__expr_$unscoped
#define comp_syn__expr_$unscoped \
    while (false); \
__step_unscope: \
    _Generic(TypeOf(*__reserved_break), \
        void: ({}), \
        default: __reserved_break[0] \
    ); \
})
#define $break_(_Expr...) comp_syn__$break_(_Expr)
#define comp_syn__$break_(_Expr...) blk({ \
    prim_memcpy( \
        as$(u8*)(__reserved_break), \
        as$(u8*)((TypeOf (*__reserved_break)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_break) \
    ); \
    goto __step_break; \
})
// clang-format on
#endif /* UNUSED_CODE */

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_(expr_(E$void $scope) if (true) {
        $break_(ok({}));
    } else {
        $break_(err(Err_NotImplemented()));
    } $unscoped_(expr));
} $unscoped_(fn);
