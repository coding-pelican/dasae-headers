#include "dh/core.h"
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
    pp_foreach (comp_gen__variant__enumTag, T_variant, __VA_ARGS__)
#define comp_gen__variant__enumTag(T_variant, _Pair) \
    pp_Tuple_get1st _Pair,

#define comp_gen__variant__unionTypes(T_variant, ...) \
    pp_foreach (comp_gen__variant__unionType, T_variant, __VA_ARGS__)
#define comp_gen__variant__unionType(T_variant, _Pair) \
    pp_Tuple_get2nd _Pair pp_join($, tagged, pp_Tuple_get1st _Pair)[1];

#define comp_op__variant_of(_tag, _payload...) \
    { \
        .tag     = _tag, \
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

#include "dh/blk.h"
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

typedef variant_(
    (Obj_Scalar),
    (Obj_Scalar_u32, u32),
    (Obj_Scalar_i32, i32),
    (Obj_Scalar_f32, f32)
) Obj_Scalar;
typedef variant_(
    (Obj),
    (Obj_none, Void),
    (Obj_bool, bool),
    (Obj_scalar, Obj_Scalar)
) Obj;

TEST_fn_("variant: basic" $scope) {
    let obj = variant_of$(Obj, Obj_scalar, variant_of(Obj_Scalar_i32, -123));
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
    try_(TEST_expect(obj.tag == Obj_scalar));
    let scalar = variant_as(&obj, Obj_scalar);
    try_(TEST_expect(variant_extract(*scalar, Obj_Scalar_i32) == -123));
} $unscoped_(TEST_fn);
