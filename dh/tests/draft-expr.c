#include "dh/main.h"

// typedef struct {
// } TypeHint$R;
// typedef struct R {
//     TypeHint$R __hint;
//     usize      begin;
//     usize      end;
// } R;
// #define $r(_begin, _end) ((R){ .begin = (_begin), .end = (_end) })

// #define $for_(_range, _iter, _body...) \
//     for (struct { bool once; R range; usize iter; } __state = { .once = true, .range = (_range), .iter = (_range).begin }; \
//          __state.once; \
//          __state.once = false) \
//         for (; \
//              __state.iter < __state.range.end; \
//              ++(__state.iter)) { \
//             const usize _iter = __state.iter; \
//             _body \
//         }

// // // clang-format off
// // #define expr_(...) pp_overload(inline__expr, __VA_ARGS__)(__VA_ARGS__)
// // #define inline__expr_1(_keyword...) if (!__has_broken) {} _keyword
// // #define inline__expr_2(T, _keyword...) ({ T __reserved_break; bool __has_broken = false;
// // #define inline__$unscoped_expr() __reserved_break; })
// #define $break_(_Expr...) ({ \
//     bti_memcpy( \
//         as$(u8*, __reserved_break), \
//         as$(u8*, (TypeOf (*__reserved_break)[1]){ [0] = _Expr }), \
//         sizeOf$(*__reserved_break) \
//     ); \
//     __has_broken = true; \
//     goto __step_break; \
// })
// // // clang-format on



// // clang-format off
// #define expr_(T_Break_w_Ext...) inline__expr(T_Break_w_Ext)
// #define inline__expr(T_Break_w_Ext...) pp_overload(inline__expr, T_Break_w_Ext)(T_Break_w_Ext)
// #define inline__expr_1(_keyword...) ; if (__has_broken) { goto __step_break; } _keyword
// #define inline__expr_2(T_Break, _Ext...) pp_cat(inline__expr_2, _Ext)(T_Break)
// #define inline__expr_2$_scope(T_Break...) ({ \
//     local_label __step_break; \
//     let __reserved_break = as$(T_Break*, (u8[_Generic(T_Break, \
//         void: 0, \
//         default: sizeOf$(T_Break) \
//     )]){}); \
//     $maybe_unused bool __has_broken = false;\
//     /* do */
// #define $unscoped_expr comp_syn__expr_$unscoped
// #define comp_syn__expr_$unscoped \
//     /* while(false) */; \
//     __step_break: \
//     _Generic(TypeOf(*__reserved_break), \
//         void: ({}), \
//         default: __reserved_break[0] \
//     ); \
// })
// // clang-format on

// #define $unscoped_(_keyword) \
//     pp_cat(inline__$unscoped_, _keyword)()
// #define inline__$unscoped_fn()      $unscoped
// #define inline__$unscoped_TEST_fn() $unscoped_TEST_fn
// #define inline__$unscoped_eval()    $unscoped_eval
// #define inline__$unscoped_expr()    $unscoped_expr


// #define $if_(_cond, _body...) \
//     if (_cond) _body


// func((dh_main(Sli$Sli_const$u8 args))(Err$void)$scope) {
//     let_ignore = args;
//
//     //     ({
//     //         local_label __step_break;
//     //         if (false) {
//     // __step_break:
//     //             break; // 이게 원인이네
//     //         }
//     //         do {
//     //             /* code */
//     //             goto __step_break;
//     //         } while (true);
//     //         ({});
//     //     });
//
//     // Str_eql(value, u8_l("first"));
//     // Str_eql(value, u8_l("second"));
//     // Str_eql(value, u8_l("third"));
//
//     return_ok({});
// } $unscoped;

#include "dh/io/stream.h"


TEST_fn_("test eval function" $scope) {
    const usize key = 12;

    let value_for = expr_(Sli_const$u8 $scope)(
        for_($r(0, 10), i) {
            io_stream_print(u8_l("i: {:llu}\n"), i);
            if (i % 2 == 0) { continue; }
            if (i == 5) { break; }
            if (i == key) { $break_(u8_l("first")); }
        }
    ) expr_(else)(for_($r(10, 20), j) {
        io_stream_print(u8_l("j: {:llu}\n"), j);
        if (j == key) { $break_(u8_l("second")); }
    }) expr_(else)({
        $break_(u8_l("third"));
    }) $unscoped_(expr);

    let value_if = expr_(Sli_const$u8 $scope)(if (false) {
        $break_(u8_l("first"));
    }) expr_(else)(if (true) {
        $break_(u8_l("second"));
    }) expr_(else)({
        claim_unreachable;
    }) $unscoped_(expr);

    io_stream_print(u8_l("value_for: {:s}\n"), value_for);
    io_stream_print(u8_l("value_if: {:s}\n"), value_if);

    try_(TEST_expect(Str_eql(value_for, value_if)));
} $unscoped_(TEST_fn);
