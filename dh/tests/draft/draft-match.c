/* #include "dh/core.h"
#include "dh/fn.h"

typedef struct Shape {
    enum Shape$Tag {
        Shape_circ,
        Shape_tri,
        Shape_sq
    } tag;
    union {
        struct {
            f32 radius;
        } circ;
        struct {
            f32 width;
            f32 height;
        } tri;
        struct {
            f32 width;
            f32 height;
        } sq;
    } payload;
} Shape;

match_(shape_lhs, shape_rhs) {
    pattern_((Shape_circ, l), (Shape_circ, r)) {
    }
    pattern_((Shape_tri, l), (Shape_tri, r)) {
    }
    pattern_((Shape_sq, l), (Shape_sq, r)) {
    }
    fallback_ {
        claim_unreachable;
    }
}

switch (shape_lhs.tag) {
case Shape_circ: {
    let l = shape_lhs.payload.circ;
    switch (shape_rhs.tag) {
    case Shape_circ: {
        let r = shape_rhs.payload.circ;
    } break;
    default:
        break;
    }
} break;
case Shape_tri: {
    let l = shape_lhs.payload.tri;
    switch (shape_rhs.tag) {
    case Shape_tri: {
        let r = shape_rhs.payload.circ;
    } break;
    default:
        break;
    }
}
case Shape_sq: {
    let l = shape_lhs.payload.tri;
    switch (shape_rhs.tag) {
    case Shape_sq: {
        let r = shape_rhs.payload.circ;
    } break;
    default:
        break;
    }
}
default:
    claim_unreachable;
}

enum O$Tag {
    O_none,
    O_some
};

typedef struct O$Shape {
    enum O$Tag tag;
    union {
        Void none;
        Shape some;
    } payload;
} O$Shape;



match_(shape_lhs, opt_shape_rhs) {
    pattern_((Shape_circ)(l), (O_some, Shape_circ)(r)) {
    }
    pattern_((Shape_tri)(l), (O_some, Shape_tri)(r)) {
    }
    pattern_((Shape_sq)(l), (O_some, Shape_sq)(r)) {
    }
    fallback_(claim_unreachable);
}

if ((shape_lhs.tag == Shape_circ) && ((shape_rhs.tag == O_some) && (opt_shape_rhs.payload.some.tag == Shape_circ))) {
    let l = shape_lhs.payload.circ;
    let r = opt_shape_rhs.payload.some.circ;
} else if ((shape_lhs.tag == Shape_tri) && ((shape_rhs.tag == O_some) && (opt_shape_rhs.payload.some.tag == Shape_tri))) {
} else if ((shape_lhs.tag == Shape_sq) && ((shape_rhs.tag == O_some) && (opt_shape_rhs.payload.some.tag == Shape_sq))) {
} else {
    claim_unreachable;
}

switch (shape_lhs.tag) {
case Shape_circ: {
    let l = shape_lhs.payload.circ;
    switch (opt_shape_rhs.tag) {
    case O_some: {
        switch (opt_shape_rhs.payload.some.tag) {
        case Shape_circ: {
            let r = opt_shape_rhs.payload.some.circ;
        } break;
        }
    } break;
    default:
        break;
    }
} break;
case Shape_tri: {
    let l = shape_lhs.payload.tri;
    switch (shape_rhs.tag) {
    case O_some: {
        switch (opt_shape_rhs.payload.some.tag) {
        case Shape_tri: {
            let r = opt_shape_rhs.payload.some.tri;
        } break;
        default:
            claim_unreachable;
        }
    } break;
    default:
        break;
    }
}
case Shape_sq: {
    let l = shape_lhs.payload.tri;
    switch (shape_rhs.tag) {
    case O_some: {
        switch (opt_shape_rhs.payload.some.tag) {
        case Shape_tri: {
            let r = opt_shape_rhs.payload.some.tri;
        } break;
        default:
            claim_unreachable;
        }
    } break;
    default:
        break;
    }
}
default:
    claim_unreachable;
}
 */
