/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    union_enum.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2025-02-21 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef UNION_ENUM_INCLUDED
#define UNION_ENUM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "scope.h"

/*========== Macros and Definitions =========================================*/

/* Union enum */
#define config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type...) GEN__config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type)
#define config_UnionEnumAsField(Pair_1Tag_2Type...)       OP__config_UnionEnumAsField(Pair_1Tag_2Type)

/* Determines union enum tag */
#define tagUnion(E_UnionEnum_Tag, val_tagged...)               OP__tagUnion(E_UnionEnum_Tag, val_tagged)
#define tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged...) OP__tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged)

/* Union enum match expr with payload captures */
#define match(val_union_enum)                           SYN__match(val_union_enum)
#define match_mut(var_union_enum)                       SYN__match_mut(var_union_enum)
#define case_pattern(E_UnionEnum_Tag, _Payload_Capture) SYN__case_pattern(E_UnionEnum_Tag, _Payload_Capture)
#define default_pattern()                               SYN__default_pattern()

/*========== Implementations ================================================*/

#define GEN__config_UnionEnum(T_UnionEnum, Pair_1Tag_2Type...)              \
    typedef struct T_UnionEnum {                                            \
        enum {                                                              \
            GEN__config_UnionEnum__enumTags(T_UnionEnum, Pair_1Tag_2Type)   \
        } tag;                                                              \
        union {                                                             \
            GEN__config_UnionEnum__unionTypes(T_UnionEnum, Pair_1Tag_2Type) \
        } data;                                                             \
    } T_UnionEnum

#define OP__config_UnionEnumAsField(Pair_1Tag_2Type...)       \
    enum {                                                    \
        GEN__config_UnionEnum__enumTags(~, Pair_1Tag_2Type)   \
    } tag;                                                    \
    union {                                                   \
        GEN__config_UnionEnum__unionTypes(~, Pair_1Tag_2Type) \
    } data;

#define OP__tagUnion(E_UnionEnum_Tag, val_tagged...)                                             \
    {                                                                                            \
        .tag  = (E_UnionEnum_Tag),                                                               \
        .data = {                                                                                \
            .pp_join($, E_UnionEnum_Tag, Tagged) = (struct pp_join($, E_UnionEnum_Tag, Tagged)){ \
                .value = val_tagged,                                                             \
            }                                                                                    \
        }                                                                                        \
    }

#define OP__tagUnion$(T_UnionEnum, E_UnionEnum_Tag, val_tagged...) ((T_UnionEnum)tagUnion(E_UnionEnum_Tag, val_tagged))

#define SYN__match(val_union_enum) \
    let_(_union_enum = (val_union_enum)) for (var _union_data = &(_union_enum.data); _union_data; _union_data = null) switch (_union_enum.tag)

#define SYN__match_mut(var_union_enum) \
    let_(_union_enum = &(var_union_enum)) for (var _union_data = &(_union_enum->data); _union_data; _union_data = null) switch (_union_enum->tag)

#define SYN__case_pattern(E_UnionEnum_Tag, _Payload_Capture) \
    case E_UnionEnum_Tag:                                    \
        for (var _Payload_Capture = &as$(struct pp_join($, E_UnionEnum_Tag, Tagged)*, _union_data)->value; _Payload_Capture; (_Payload_Capture) = null)

#define SYN__default_pattern() \
    default:

#define GEN__config_UnionEnum__enumTags(T_UnionEnum, ...) \
    pp_foreach(GEN__config_UnionEnum__enumTag, T_UnionEnum, __VA_ARGS__)

#define GEN__config_UnionEnum__enumTag(T_UnionEnum, _Pair) \
    FIRST_OF_PAIR(_Pair),

#define GEN__config_UnionEnum__unionTypes(T_UnionEnum, ...) \
    pp_foreach(GEN__config_UnionEnum__unionType, T_UnionEnum, __VA_ARGS__)

#define GEN__config_UnionEnum__unionType(T_UnionEnum, _Pair) \
    struct pp_join($, FIRST_OF_PAIR(_Pair), Tagged) {        \
        SECOND_OF_PAIR(_Pair)                                \
        value;                                               \
    } pp_join($, FIRST_OF_PAIR(_Pair), Tagged);

#define FIRST_OF_PAIR(_Pair)           FIRST_OF_PAIR_IMPL _Pair
#define FIRST_OF_PAIR_IMPL(_1st, _2nd) _1st

#define SECOND_OF_PAIR(_Pair)           SECOND_OF_PAIR_IMPL _Pair
#define SECOND_OF_PAIR_IMPL(_1st, _2nd) _2nd

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
// clang-format off
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/math/common.h"

typedef struct Circ { f32 radius; } Circ;
typedef struct Rect { f32 width, height; } Rect;

config_UnionEnum(Shape,
    (Shape_Circ, Circ),
    (Shape_Rect, Rect)
);
static f32 Shape_getArea(Shape shape) {
    return eval({
        var area = f32_nan;
        match(shape) {
            case_pattern(Shape_Circ, s) {
                area = math_f32_pi * s->radius * s->radius;
            } break;
            case_pattern(Shape_Rect, s) {
                area = s->width * s->height;
            } break;
            default_pattern() claim_unreachable;
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
    (InputEvent_release_button, struct { i8  button; })
);
use_Opt$(InputEvent);
extern Opt$InputEvent pullInputEvent(void);

static void example_handleEvent(void) {
    if_some(pullInputEvent(), event) {
        match(event) {
            case_pattern(InputEvent_press_key, on_pressed) {
                debug_assert_true_fmt(-1 < on_pressed->key && on_pressed->key <= 255, "key is out of range");
            } break;
            case_pattern(InputEvent_release_button, on_released) {
                debug_assert_true_fmt(-1 < on_released->button && on_released->button <= 5, "button is out of range");
            } break;
            default_pattern() claim_unreachable;
        }
    }
}
// clang-format on
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
        match(shape) {
            case_pattern(Shape_Circ, s) {
                area = 3.1415926f * s->radius * s->radius;
            }
            break;
            case_pattern(Shape_Rect, s) {
                area = s->width * s->height;
            }
            break;
        }
        debug_assert(area == 3.1415926f);
    }
}
#endif /* MOCKUP_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* UNION_ENUM_INCLUDED */
