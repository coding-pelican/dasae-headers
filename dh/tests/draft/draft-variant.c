#include "dh/core.h"
#include "dh/core/prim/switch.h"
#include "dh/main.h"

#if UNUSED_CODE
#define variant_(_alias, ...) \
    comp_gen__variant_(_alias, __VA_ARGS__)
#define variant_of(_tag, _payload...) \
    comp_op__variant_of(_tag, _payload)
#define variant_of$(T_variant, _tag, _payload...) \
    comp_op__variant_of$(T_variant, _tag, _payload)
#define variant_asg$(T_Variant, var_addr_variant, val_variant...) \
    comp_op__variant_asg$(pp_uniqTok(var_addr_variant), T_Variant, var_addr_variant, val_variant)
#define variant_asg(var_addr_variant, val_variant...) \
    comp_op__variant_asg(var_addr_variant, val_variant)
#define variant_as(_addr_variant, _tag...) \
    comp_op__variant_as(pp_uniqTok(addr_variant), _addr_variant, _tag)
#define variant_extract(_var_variant, _tag...) \
    comp_op__variant_extract(pp_uniqTok(var_variant), _var_variant, _tag)

#define comp_gen__variant_(_alias, ...) \
    struct pp_expand _alias { \
        enum { \
            comp_gen__variant__enumTags(_alias, __VA_ARGS__) \
        } tag; \
        union { \
            comp_gen__variant__unionTypes(_alias, __VA_ARGS__) \
        } payload; \
    }

#define comp_gen__variant__enumTags(T_variant, ...) \
    pp_foreach(comp_gen__variant__enumTag, T_variant, __VA_ARGS__)
#define comp_gen__variant__enumTag(T_variant, _Pair) \
    pp_Tuple_get1st _Pair,

#define comp_gen__variant__unionTypes(T_variant, ...) \
    pp_foreach(comp_gen__variant__unionType, T_variant, __VA_ARGS__)
#define comp_gen__variant__unionType(T_variant, _Pair) \
    pp_Tuple_get2nd _Pair pp_join($, tagged, pp_Tuple_get1st _Pair)[1];

#define comp_op__variant_of(_tag, _payload...) \
    { \
        .tag = _tag, \
        .payload = { .pp_join($, tagged, _tag) = { [0] = _payload } }, \
    }
#define comp_op__variant_of$(T_variant, _tag, _payload...) \
    ((T_variant)variant_of(_tag, _payload))

#define comp_op__variant_asg$(__addr_variant, T_Variant, var_addr_variant, val_variant...) blk({ \
    let __addr_variant = var_addr_variant; \
    debug_assert_nonnull(__addr_variant); \
    *__addr_variant = *(T_Variant[1]){ [0] = val_variant }; \
    blk_return __addr_variant; \
})
#define comp_op__variant_asg(var_addr_variant, val_variant...) \
    variant_asg$(TypeOf(*var_addr_variant), var_addr_variant, val_variant)

#define comp_op__variant_as(__addr_variant, _addr_variant, _tag...) blk({ \
    let __addr_variant = _addr_variant; \
    debug_assert(__addr_variant->tag == _tag); \
    blk_return __addr_variant->payload.pp_join($, tagged, _tag); \
})

#define comp_op__variant_extract(__var_variant, _var_variant, _tag...) blk({ \
    let __var_variant = _var_variant; \
    debug_assert(__var_variant.tag == _tag); \
    blk_return __var_variant.payload.pp_join($, tagged, _tag)[0]; \
})

typedef variant_(
    (Shape),
    (Shape_circ, struct { f32 radius; }),
    (Shape_rect, struct { f32 width; f32 height; })
) Shape;

#undef match_
#undef comp_syn__match_
// #define match_(_Variant) \
//     with_(let __variant = (_Variant)) switch (__variant.tag)
#define match_(_Variant, _Body...) \
    comp_syn__match_(_Variant, _Body)
#define comp_syn__match_(_Variant, _Body...) blk({ \
    let __variant = _Variant; \
    switch (__variant.tag) \
        _Body \
})

#undef pattern_
#undef comp_syn__pattern_
#define pattern_(_Tag, _Capture_w_Body...) \
    pp_overload(__pattern, _Capture_w_Body)(_Tag, _Capture_w_Body)
#define __pattern_1(_Tag, _Body...) \
    comp_syn__pattern_1(_Tag, _Body)
#define __pattern_2(_Tag, _Capture, _Body...) \
    comp_syn__pattern_2(_Tag, _Capture, _Body)
#define comp_syn__pattern_1(_Tag, _Body...) \
    case _Tag: \
        _Body
#define comp_syn__pattern_2(_Tag, _Capture, _Body...) \
    case _Tag: { \
        let pp_Tuple_unwrap _Capture = __variant.payload.pp_join($, tagged, _Tag); \
        _Body; \
    }
#endif /* UNUSED_CODE */

#include "dh/io/stream.h"

// fn_(Shape_calcArea(Shape self), f32 $scope) {
//     match_(self, {
//         pattern_(Shape_circ, (c), {
//             return_(3.1415926f * c->radius * c->radius);
//         });
//         pattern_(Shape_rect, (r), {
//             return_(r->width * r->height);
//         });
//     });
// } $unscoped_(fn);

// fn_(Shape_calcArea(Shape self), f32 $scope) {
//     return_(blk_(area, f32, {
//         match_(self, {
//             pattern_(Shape_circ, (c), {
//                 blk_break_(area, 3.1415926f * c->radius * c->radius);
//             });
//             pattern_(Shape_rect, (r), {
//                 blk_break_(area, r->width * r->height);
//             });
//         });
//     }));
// } $unscoped_(fn);

// fn_(Shape_calcArea(Shape self), f32) {
//     return blk_(area, f32, {
//         match_(self, {
//             pattern_(Shape_circ, (c), {
//                 blk_break_(area, 3.1415926f * c->radius * c->radius);
//             });
//             pattern_(Shape_rect, (r), {
//                 blk_break_(area, r->width * r->height);
//             });
//         });
//     });
// }

typedef variant_((Obj_Scalar)(
    (Obj_Scalar_u32, u32),
    (Obj_Scalar_i32, i32),
    (Obj_Scalar_f32, f32)
)) Obj_Scalar;
typedef variant_((Obj)(
    (Obj_bool, bool),
    (Obj_scalar, Obj_Scalar)
)) Obj;

TEST_fn_("variant: basic" $scope) {
    let obj = union_of$((Obj)(Obj_scalar)union_of$((Obj_Scalar)(Obj_Scalar_i32)-123));
#if UNUSED_CODE
    match_(obj, {
        pattern_(Obj_none, {
            io_stream_println(u8_l("none\n"));
        }) break;
        pattern_(Obj_bool, (b), {
            io_stream_println(u8_l("bool: {:d}\n"), *b);
        }) break;
        pattern_(Obj_scalar, (s), {
            match_(*s, {
                pattern_(Obj_Scalar_u32, (u), {
                    io_stream_println(u8_l("u32: {:u}\n"), *u);
                }) break;
                pattern_(Obj_Scalar_i32, (i), {
                    io_stream_println(u8_l("i32: {:d}\n"), *i);
                }) break;
                pattern_(Obj_Scalar_f32, (f), {
                    io_stream_println(u8_l("f32: {:f}\n"), *f);
                }) break;
            });
        }) break;
    });
#endif /* UNUSED_CODE */
    let _vari = obj;
    {
        let_(__vari, TypeOf(_vari)) = _vari;
        switch (__vari.tag) {
        case Obj_bool: {
            let my_b = __vari.payload.tag$Obj_bool;
            io_stream_println(u8_l("bool: {:d}\n"), *my_b);
        }
        case Obj_scalar: {
            let my_s = __vari.payload.tag$Obj_scalar;
            {
                let_(__vari, TypeOf(*my_s)) = *my_s;
                switch (__vari.tag) {
                case Obj_Scalar_u32: {
                    let my_u = __vari.payload.tag$Obj_Scalar_u32;
                    io_stream_println(u8_l("u32: {:u}\n"), *my_u);
                }
                case Obj_Scalar_i32: {
                    let my_i = __vari.payload.tag$Obj_Scalar_i32;
                    io_stream_println(u8_l("i32: {:d}\n"), *my_i);
                }
                case Obj_Scalar_f32: {
                    let my_f = __vari.payload.tag$Obj_Scalar_f32;
                    io_stream_println(u8_l("f32: {:f}\n"), *my_f);
                }
                };
            }
        }
        };
    }

#undef match_
#undef pattern_
#undef case_
#undef default_
#if UNUSED_CODE
    match_(obj) {
        pattern_((Obj_bool)(my_b)) {
            io_stream_println(u8_l("bool: {:d}\n"), *my_b);
        }
        $(pattern);
        pattern_((Obj_scalar)(my_s))
            match_(*my_s) {
            pattern_((Obj_Scalar_u32)(my_u)) {
                io_stream_println(u8_l("u32: {:u}\n"), *my_u);
            }
            $(pattern);
            pattern_((Obj_Scalar_i32)(my_i)) {
                io_stream_println(u8_l("i32: {:d}\n"), *my_i);
            }
            $(pattern);
            pattern_((Obj_Scalar_f32)(my_f)) {
                io_stream_println(u8_l("f32: {:f}\n"), *my_f);
            }
            $(pattern);
            fallback {
                claim_unreachable;
            }
            $(fallback);
        }
        $(pattern)
        $(match);
        fallback {
            claim_unreachable;
        }
        $(fallback);
    }
    $(match);
#endif /* UNUSED_CODE */

    /* clang-format off */
#define $end(_keyword) ; pp_cat($end_, _keyword)()

#define match_(_vari) { \
    let __vari = _vari; \
    switch (__vari.tag) { \
        if (false)
#define $end_match() \
}

#define case_(_tag) \
case _tag:
#define $end_case() \

#define pattern_(/*(_tag)(_capture)*/...) __step__pattern_(__step__pattern___parseTag __VA_ARGS__)
#define __step__pattern_(...) __step__pattern___emit(__VA_ARGS__)
#define __step__pattern___parseTag(_tag...) _tag, __step__pattern___parseCapture
#define __step__pattern___parseCapture(_capture...) _capture
#define __step__pattern___emit(_tag, _capture...) \
} case _tag: { \
    let _capture = pp_join($, __vari.payload.tag, _tag);
#define $end_pattern() \
break

#define default_() } default: {
#define $end_default() }
    /* clang-format on */

    match_(obj) {
        pattern_((Obj_bool)(my_b)) {
            io_stream_println(u8_l("bool: {:d}\n"), *my_b);
        } $end(pattern);
        pattern_((Obj_scalar)(my_s)) match_(*my_s) {
            pattern_((Obj_Scalar_i32)(my_i)) {
                io_stream_println(u8_l("i32: {:d}\n"), *my_i);
            } $end(pattern);
            pattern_((Obj_Scalar_f32)(my_f)) {
                io_stream_println(u8_l("f32: {:f}\n"), *my_f);
            } $end(pattern);
            default_() claim_unreachable $end(default);
        } $end(match) $end(pattern);
        default_() claim_unreachable $end(default);
    } $end(match);

#if UNUSED_CODE
    ({
        __auto_type const __variant = obj;
        switch (__variant.tag) {
        case Obj_bool: {
            __auto_type const b = __variant.payload.tag$Obj_bool;
            { io_stream_println(((S_const$u8){ .ptr = (const u8*)(""
                                                                  "bool: {:d}\n"),
                                               .len = ((usize)(sizeof(__typeof__("bool: {:d}\n")))) - 1 }),
                                *b); };
        } break;
        case Obj_scalar: {
            __auto_type const s = __variant.payload.tag$Obj_scalar;
            {
                ({
                    __auto_type const __variant = *s;
                    switch (__variant.tag) {
                    case Obj_Scalar_u32: {
                        __auto_type const u = __variant.payload.tag$Obj_Scalar_u32;
                        { io_stream_println(((S_const$u8){ .ptr = (const u8*)(""
                                                                              "u32: {:u}\n"),
                                                           .len = ((usize)(sizeof(__typeof__("u32: {:u}\n")))) - 1 }),
                                            *u); };
                    } break;
                    case Obj_Scalar_i32: {
                        __auto_type const i = __variant.payload.tag$Obj_Scalar_i32;
                        { io_stream_println(((S_const$u8){ .ptr = (const u8*)(""
                                                                              "i32: {:d}\n"),
                                                           .len = ((usize)(sizeof(__typeof__("i32: {:d}\n")))) - 1 }),
                                            *i); };
                    } break;
                    case Obj_Scalar_f32: {
                        __auto_type const f = __variant.payload.tag$Obj_Scalar_f32;
                        { io_stream_println(((S_const$u8){ .ptr = (const u8*)(""
                                                                              "f32: {:f}\n"),
                                                           .len = ((usize)(sizeof(__typeof__("f32: {:f}\n")))) - 1 }),
                                            *f); };
                    } break;
                    }
                });
            };
        } break;
        }
    });
#endif /* UNUSED_CODE */
    try_(TEST_expect(obj.tag == Obj_scalar));
    // let scalar = variant_as(&obj, Obj_scalar);
    // try_(TEST_expect(variant_extract(*scalar, Obj_Scalar_i32) == -123));
} $unscoped_(TEST_fn);
