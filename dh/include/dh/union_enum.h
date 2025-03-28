/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    union_enum.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2025-03-09 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Union enum implementation with pattern matching
 * @details Provides macros and utilities for creating and working with tagged unions (discriminated unions),
 *          including pattern matching syntax, payload extraction, and type-safe union construction.
 *          Supports compile-time type checking and exhaustive pattern matching.
 */

#ifndef UNION_ENUM_INCLUDED
#define UNION_ENUM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "scope.h"
#include "fn.h"

/*========== Macros and Definitions =========================================*/

/* Union enum */
#define config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type...) comp_gen__config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type)
#define config_UnionEnumAsField(Pair_1Tag_2Type...)       comp_gen__config_UnionEnumAsField(Pair_1Tag_2Type)

/* Determines union enum tag */
#define tagUnion(E_UnionEnum_Tag, val_tagged...)                        comp_op__tagUnion(E_UnionEnum_Tag, val_tagged)
#define tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged...)          comp_op__tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged)
#define toTagUnion(var_addr_union_enum, E_UnionEnum_Tag, val_tagged...) comp_op__toTagUnion(pp_uniqTok(addr_union_enum), var_addr_union_enum, E_UnionEnum_Tag, val_tagged)

#define extract(val_union_enum, E_UnionEnum_Tag)     comp_op__extract(val_union_enum, E_UnionEnum_Tag)
#define extract_mut(var_union_enum, E_UnionEnum_Tag) comp_op__extract_mut(var_union_enum, E_UnionEnum_Tag)

/* Union enum match expr with payload captures */
#define match_(val_union_enum)                          comp_syn__match_(val_union_enum)
#define match_mut_(var_union_enum)                      comp_syn__match_mut_(var_union_enum)
#define pattern_(E_UnionEnum_Tag, _Payload_Capture)     comp_syn__pattern_(E_UnionEnum_Tag, _Payload_Capture)
#define pattern_mut_(E_UnionEnum_Tag, _Payload_Capture) comp_syn__pattern_mut_(E_UnionEnum_Tag, _Payload_Capture)
#define fallback_()                                     comp_syn__fallback_()

/*========== Implementations ================================================*/

#define comp_gen__config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type...)              \
    typedef struct T_UnionEnum {                                                 \
        enum {                                                                   \
            comp_gen__config_UnionEnum__enumTags(T_UnionEnum, Pair_1Tag_2Type)   \
        } tag;                                                                   \
        union {                                                                  \
            comp_gen__config_UnionEnum__unionTypes(T_UnionEnum, Pair_1Tag_2Type) \
        } data;                                                                  \
    } T_UnionEnum

#define comp_gen__config_UnionEnumAsField(Pair_1Tag_2Type...)      \
    enum {                                                         \
        comp_gen__config_UnionEnum__enumTags(~, Pair_1Tag_2Type)   \
    } tag;                                                         \
    union {                                                        \
        comp_gen__config_UnionEnum__unionTypes(~, Pair_1Tag_2Type) \
    } data;

#define comp_op__tagUnion(E_UnionEnum_Tag, val_tagged...)                                        \
    {                                                                                            \
        .tag  = (E_UnionEnum_Tag),                                                               \
        .data = {                                                                                \
            .pp_join($, E_UnionEnum_Tag, Tagged) = (struct pp_join($, E_UnionEnum_Tag, Tagged)){ \
                .value = val_tagged,                                                             \
            }                                                                                    \
        }                                                                                        \
    }

#define comp_op__tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged...) ((T_UnionEnum)tagUnion(E_UnionEnum_Tag, val_tagged))

#define comp_op__toTagUnion(__addr_union_enum, var_addr_union_enum, E_UnionEnum_Tag, val_tagged...) eval({ \
    let __addr_union_enum = var_addr_union_enum;                                                           \
    debug_assert_nonnull(__addr_union_enum);                                                               \
    *__addr_union_enum = tagUnion$(TypeOf(*__addr_union_enum), E_UnionEnum_Tag, val_tagged);                 \
    eval_return __addr_union_enum;                                                                         \
})

#define comp_op__extract(val_union_enum, E_UnionEnum_Tag) eval({             \
    let __union_enum = (val_union_enum);                                     \
    debug_assert(__union_enum.tag == (E_UnionEnum_Tag));                     \
    eval_return __union_enum.data.pp_join($, E_UnionEnum_Tag, Tagged).value; \
})

#define comp_op__extract_mut(var_union_enum, E_UnionEnum_Tag) eval({            \
    var __union_enum = &(var_union_enum);                                       \
    debug_assert(__union_enum->tag == (E_UnionEnum_Tag));                       \
    eval_return&(__union_enum->data.pp_join($, E_UnionEnum_Tag, Tagged).value); \
})

#define comp_syn__match_(val_union_enum) \
    let_(_union_enum = (val_union_enum)) for (var _union_data = &(_union_enum.data); _union_data; _union_data = null) switch (_union_enum.tag)

#define comp_syn__match_mut_(var_union_enum) \
    let_(_union_enum = &(var_union_enum)) for (var _union_data = &(_union_enum->data); _union_data; _union_data = null) switch (_union_enum->tag)

#define comp_syn__pattern_(E_UnionEnum_Tag, _Payload_Capture) \
    case E_UnionEnum_Tag:                                     \
        for (var _Payload_Capture = &as$(const struct pp_join($, E_UnionEnum_Tag, Tagged)*, _union_data)->value; _Payload_Capture; (_Payload_Capture) = null)

#define comp_syn__pattern_mut_(E_UnionEnum_Tag, _Payload_Capture) \
    case E_UnionEnum_Tag:                                         \
        for (var _Payload_Capture = &as$(struct pp_join($, E_UnionEnum_Tag, Tagged)*, _union_data)->value; _Payload_Capture; (_Payload_Capture) = null)

#define comp_syn__fallback_() \
    default:

#define comp_gen__config_UnionEnum__enumTags(T_UnionEnum, ...) \
    pp_foreach (comp_gen__config_UnionEnum__enumTag, T_UnionEnum, __VA_ARGS__)

#define comp_gen__config_UnionEnum__enumTag(T_UnionEnum, _Pair) \
    FIRST_OF_PAIR(_Pair),

#define comp_gen__config_UnionEnum__unionTypes(T_UnionEnum, ...) \
    pp_foreach (comp_gen__config_UnionEnum__unionType, T_UnionEnum, __VA_ARGS__)

#define comp_gen__config_UnionEnum__unionType(T_UnionEnum, _Pair) \
    struct pp_join($, FIRST_OF_PAIR(_Pair), Tagged) {             \
        SECOND_OF_PAIR(_Pair)                                     \
        value;                                                    \
    } pp_join($, FIRST_OF_PAIR(_Pair), Tagged);

#define FIRST_OF_PAIR(_Pair)           FIRST_OF_PAIR_IMPL _Pair
#define FIRST_OF_PAIR_IMPL(_1st, _2nd) _1st

#define SECOND_OF_PAIR(_Pair)           SECOND_OF_PAIR_IMPL _Pair
#define SECOND_OF_PAIR_IMPL(_1st, _2nd) _2nd

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/math/common.h"

// clang-format off
typedef struct Circ { f32 radius; } Circ;
typedef struct Rect { f32 width, height; } Rect;
// clang-format on

config_UnionEnum(Shape,
    (Shape_Circ, Circ),
    (Shape_Rect, Rect)
);
static f32 Shape_getArea(Shape shape) {
    return eval({
        var area = f32_nan;
        match_(shape) {
        pattern_(Shape_Circ, s) {
            area = math_f32_pi * s->radius * s->radius;
        } break;
        pattern_(Shape_Rect, s) {
            area = s->width * s->height;
        } break;
            fallback_
                claim_unreachable;
        }
        eval_return area;
    });
    /* return eval_match(shape, {
        pattern_case(Shape_Circ, s) {
            match_return(math_f32_pi * s->radius * s->radius);
        } break;
        pattern_case(Shape_Rect, s) {
            match_return(s->width * s->height);
        } break;
        pattern_default() claim_unreachable;
    }) */
}

static void example_getArea(void) {
    let circ = (Shape)tagUnion(Shape_Circ, { .radius = 10.0f });
    let rect = tagUnion$(Shape, Shape_Rect, { .width = 10.0f, .height = 5.0f });
    debug_assert_true_fmt(
        Shape_getArea(circ) == math_f32_pi * 10.0f * 10.0f,
        "area is incorrect"
    );
    debug_assert_true_fmt(
        Shape_getArea(rect) == 10.0f * 5.0f,
        "area is incorrect"
    );
}

config_UnionEnum(InputEvent,
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
);
use_Opt$(InputEvent);
extern fn_(pullInputEvent(void), Opt$InputEvent);

static void example_handleEvent(void) {
    if_some(pullInputEvent(), event) {
        match_(event) {
        pattern_(InputEvent_press_key, on_pressed) {
            debug_assert_true_fmt(
                -1 < on_pressed->key && on_pressed->key <= 255,
                "key is out of range"
            );
        } break;
        pattern_(InputEvent_release_button, on_released) {
            debug_assert_true_fmt(
                -1 < on_released->button && on_released->button <= 5,
                "button is out of range"
            );
        } break;
            fallback_
                claim_unreachable;
        }
    }
}
#endif /* EXAMPLE_USAGE */

#if MOCKUP_CODE
typedef struct Circ {
    f32 radius;
} Circ;
typedef struct Rect {
    f32 width;
    f32 height;
} Rect;

typedef struct Shape {
    enum {
        Shape_Circ,
        Shape_Rect
    } tag;
    union {
        struct Shape_CircMatch {
            Circ value;
        } Shape_CircMatch;
        struct Shape_RectMatch {
            Rect value;
        } Shape_RectMatch;
    } data;
} Shape;

#define union_enum$(E_UnionEnum_Tag, ...)                              \
    {                                                                  \
        .tag  = E_UnionEnum_Tag,                                       \
        .data = {                                                      \
            .E_UnionEnum_Tag##Match = (struct E_UnionEnum_Tag##Match){ \
                .value = { __VA_ARGS__ },                              \
            },                                                         \
        },                                                             \
    }

void test(void) {
    Shape shape = union_enum$(Shape_Circ, .radius = 1.0f);
    with_(f32 area = 0) {
        match_(shape) {
        pattern_(Shape_Circ, s) {
            area = 3.1415926f * s->radius * s->radius;
        } break;
        pattern_(Shape_Rect, s) {
            area = s->width * s->height;
        } break;
        }
        debug_assert(area == 3.1415926f);
    }
}
#endif /* MOCKUP_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* UNION_ENUM_INCLUDED */
