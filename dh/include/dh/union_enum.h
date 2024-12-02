/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    union_enum.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2024-12-02 (date of last update)
 * @version v1.0.0
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

// NOLINTBEGIN
#define match(TUnionEnum, var)                                                           \
    for (union TUnionEnum* _union_data = &((var).data); _union_data; _union_data = null) \
        switch ((var).tag)

#define case(ETag, var_decl) \
    case ETag:               \
        for (struct ETag##Data* var_decl = (anyptr)_union_data; var_decl; var_decl = null)
// NOLINTEND

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST

typedef struct {
    enum {
        Shape_Circ,
        Shape_Rect
    } tag;
    union Shape {
        struct Shape_CircData {
            f32 radius;
        } Circ;
        struct Shape_RectData {
            f32 width;
            f32 height;
        } Rect;
    } data;
} Shape;

void test() {
    Shape shape = (Shape){
        .tag       = Shape_Circ,
        .data.Circ = ((struct Shape_CircData){ .radius = 1.0f })
    };

    scope_with(f32 area = 0) {
        match(Shape, shape) {
            case (Shape_Circ, s) {
                area = 3.1415926f * s->radius * s->radius;
            } break;
            case (Shape_Rect, s) {
                area = s->width * s->height;
            } break;
        };
        debug_assert(area == 3.1415926f);
    }
}

#endif // UNIT_TEST

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* UNION_ENUM_INCLUDED */
