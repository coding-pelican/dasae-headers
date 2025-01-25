/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-01-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/math
 * @prefix  math
 *
 * @brief   Mathematical common vector operations
 * @details Provides a set of vector mathematical operations and functions
 */

#ifndef MATH_VEC_INCLUDED
#define MATH_VEC_INCLUDED (1)

#include "common.h"
#include "vec_types.h"

/* Constants */
#define math_Vec_zero$(TVec)      VAL_math_Vec_zero$(TVec)
#define math_Vec2_one$(TVec)      VAL_math_Vec2_one$(TVec)
#define math_Vec3_one$(TVec)      VAL_math_Vec3_one$(TVec)
#define math_Vec4_one$(TVec)      VAL_math_Vec4_one$(TVec)
#define math_Vec_lt$(TVec)        VAL_math_Vec_lt$(TVec)
#define math_Vec_up$(TVec)        VAL_math_Vec_up$(TVec)
#define math_Vec_rt$(TVec)        VAL_math_Vec_rt$(TVec)
#define math_Vec_dn$(TVec)        VAL_math_Vec_dn$(TVec)
#define math_Vec3_fwd$(TVec)      VAL_math_Vec3_fwd$(TVec)
#define math_Vec3_bwd$(TVec)      VAL_math_Vec3_bwd$(TVec)
#define math_Vec_left$(TVec)      math_Vec_lt$(TVec)
#define math_Vec_right$(TVec)     math_Vec_rt$(TVec)
#define math_Vec_down$(TVec)      math_Vec_dn$(TVec)
#define math_Vec3_forward$(TVec)  math_Vec3_fwd$(TVec)
#define math_Vec3_backward$(TVec) math_Vec3_bwd$(TVec)

// #define math_Vec2_eps$(TVec) VAL_math_Vec2_eps$(TVec)
// #define math_Vec3_eps$(TVec) VAL_math_Vec3_eps$(TVec)
// #define math_Vec4_eps$(TVec) VAL_math_Vec4_eps$(TVec)
// #define math_Vec2_inf$(TVec) VAL_math_Vec2_inf$(TVec)
// #define math_Vec3_inf$(TVec) VAL_math_Vec3_inf$(TVec)
// #define math_Vec4_inf$(TVec) VAL_math_Vec4_inf$(TVec)
// #define math_Vec2_nan$(TVec) VAL_math_Vec2_nan$(TVec)
// #define math_Vec3_nan$(TVec) VAL_math_Vec3_nan$(TVec)
// #define math_Vec4_nan$(TVec) VAL_math_Vec4_nan$(TVec)

// #define math_Vec2_limit_min$(TVec) VAL_math_Vec2_limit_min$(TVec)
// #define math_Vec3_limit_min$(TVec) VAL_math_Vec3_limit_min$(TVec)
// #define math_Vec4_limit_min$(TVec) VAL_math_Vec4_limit_min$(TVec)
// #define math_Vec2_limit_max$(TVec) VAL_math_Vec2_limit_max$(TVec)
// #define math_Vec3_limit_max$(TVec) VAL_math_Vec3_limit_max$(TVec)
// #define math_Vec4_limit_max$(TVec) VAL_math_Vec4_limit_max$(TVec)

/* Vector construction */
#define math_Vec2$(TVec, val_x, val_y)               VAL_math_Vec2$(TVec, val_x, val_y)
#define math_Vec3$(TVec, val_x, val_y, val_z)        VAL_math_Vec3$(TVec, val_x, val_y, val_z)
#define math_Vec4$(TVec, val_x, val_y, val_z, val_w) VAL_math_Vec4$(TVec, val_x, val_y, val_z, val_w)

/* Scalar expansion constructors */
#define math_Vec2_fill$(TVec, val_scalar) FUNC_math_Vec2_fill$(pp_uniqTok(Vec2_fill$_scalar), TVec, val_scalar)
#define math_Vec3_fill$(TVec, val_scalar) FUNC_math_Vec3_fill$(pp_uniqTok(Vec3_fill$_scalar), TVec, val_scalar)
#define math_Vec4_fill$(TVec, val_scalar) FUNC_math_Vec4_fill$(pp_uniqTok(Vec4_fill$_scalar), TVec, val_scalar)
#define math_Vec2_fill(val_scalar)        FUNC_math_Vec2_fill(pp_uniqTok(Vec2_fill_scalar), val_scalar)
#define math_Vec3_fill(val_scalar)        FUNC_math_Vec3_fill(pp_uniqTok(Vec3_fill_scalar), val_scalar)
#define math_Vec4_fill(val_scalar)        FUNC_math_Vec4_fill(pp_uniqTok(Vec4_fill_scalar), val_scalar)

/* Dimension conversion */
#define math_Vec2_from$(TVec, val_vec) FUNC_math_Vec2_from$(pp_uniqTok(Vec2_from$_vec), TVec, val_vec)
#define math_Vec3_from$(TVec, val_vec) FUNC_math_Vec3_from$(pp_uniqTok(Vec3_from$_vec), TVec, val_vec)
#define math_Vec4_from$(TVec, val_vec) FUNC_math_Vec4_from$(pp_uniqTok(Vec4_from$_vec), TVec, val_vec)
#define math_Vec2_from(val_vec)        FUNC_math_Vec2_from(pp_uniqTok(Vec2_from_vec), val_vec)
#define math_Vec3_from(val_vec)        FUNC_math_Vec3_from(pp_uniqTok(Vec3_from_vec), val_vec)
#define math_Vec4_from(val_vec)        FUNC_math_Vec4_from(pp_uniqTok(Vec4_from_vec), val_vec)

/* Swizzle helpers */
#define math_Vec_xy(val_vec)   math_Vec2_from(val_vec)
#define math_Vec_xyz(val_vec)  math_Vec3_from(val_vec)
#define math_Vec_xyzw(val_vec) math_Vec4_from(val_vec)

/* Vector type conversion */
#define math_Vec_as$(TDest, val_vec) FUNC_math_Vec_as$(TDest, val_vec)

/* Vector construction for rotation */
#define math_Vec2_sincos$(TVec, val_radians) FUNC_math_Vec2_sincos$(pp_uniqTok(Vec2_sincos$_radians), TVec, val_radians)

/* Basic Vector-Vector Comparison Operations */
#define math_Vec_cmp(val_lhs, val_rhs) OP_math_Vec_cmp(pp_uniqTok(Vec_cmp_lhs), pp_uniqTok(Vec_cmp_rhs), val_lhs, val_rhs) /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_eq(val_lhs, val_rhs)  OP_math_Vec_eq(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_ne(val_lhs, val_rhs)  OP_math_Vec_ne(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_lt(val_lhs, val_rhs)  OP_math_Vec_lt(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_gt(val_lhs, val_rhs)  OP_math_Vec_gt(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_le(val_lhs, val_rhs)  OP_math_Vec_le(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_ge(val_lhs, val_rhs)  OP_math_Vec_ge(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */

/* Core Vector-Vector Arithmetic */
#define math_Vec_neg(val_vec)          OP_math_Vec_neg(pp_uniqTok(Vec_neq_vec), val_vec)
#define math_Vec_add(val_lhs, val_rhs) OP_math_Vec_add(pp_uniqTok(Vec_add_lhs), pp_uniqTok(Vec_add_rhs), val_lhs, val_rhs)
#define math_Vec_sub(val_lhs, val_rhs) OP_math_Vec_sub(pp_uniqTok(Vec_sub_lhs), pp_uniqTok(Vec_sub_rhs), val_lhs, val_rhs)
#define math_Vec_mul(val_lhs, val_rhs) OP_math_Vec_mul(pp_uniqTok(Vec_mul_lhs), pp_uniqTok(Vec_mul_rhs), val_lhs, val_rhs)
#define math_Vec_div(val_lhs, val_rhs) OP_math_Vec_div(pp_uniqTok(Vec_div_lhs), pp_uniqTok(Vec_div_rhs), val_lhs, val_rhs)
#define math_Vec_mod(val_lhs, val_rhs) OP_math_Vec_mod(pp_uniqTok(Vec_mod_lhs), pp_uniqTok(Vec_mod_rhs), val_lhs, val_rhs)

/* Vector-Scalar Operations */
#define math_Vec_scale(val_vec, val_scalar) OP_math_Vec_scale(pp_uniqTok(Vec_scale_lhs_vec), pp_uniqTok(Vec_scale_scalar), val_vec, val_scalar)

// /* Range Operations */
// #define math_Vec_min(val_lhs, val_rhs)          FUNC_math_Vec_min(val_lhs, val_rhs)
// #define math_Vec_max(val_lhs, val_rhs)          FUNC_math_Vec_max(val_lhs, val_rhs)
// #define math_Vec_clamp(val_x, val_min, val_max) FUNC_math_Vec_clamp(val_x, val_min, val_max)
// #define math_Vec_wrap(val_x, val_min, val_max)  FUNC_math_Vec_wrap(val_x, val_min, val_max)

// /* Magnitude and Distance Operations */
// #define math_Vec_lenSq(val_vec)           FUNC_math_Vec_lenSq(val_vec)
// #define math_Vec_len(val_vec)             FUNC_math_Vec_len(val_vec)
// #define math_Vec_magSq(val_vec)           FUNC_math_Vec_magSq(val_vec)
// #define math_Vec_mag(val_vec)             FUNC_math_Vec_mag(val_vec)
// #define math_Vec_normSq(val_vec)          FUNC_math_Vec_normSq(val_vec)
// #define math_Vec_norm(val_vec)            FUNC_math_Vec_norm(val_vec)
// #define math_Vec_distSq(val_lhs, val_rhs) FUNC_math_Vec_distSq(val_lhs, val_rhs)
// #define math_Vec_dist(val_lhs, val_rhs)   FUNC_math_Vec_dist(val_lhs, val_rhs)

// /* Angular Operations */
// #define math_Vec_angle(val_lhs, val_rhs)               FUNC_math_Vec_angle(val_lhs, val_rhs)
// #define math_Vec_shortestArc(val_lhs, val_rhs)         FUNC_math_Vec_shortestArc(val_lhs, val_rhs)
// #define math_Vec2_rotate(val_vec, val_angle)           FUNC_math_Vec2_rotate(val_vec, val_angle)
// #define math_Vec3_rotate(val_vec, val_axis, val_angle) FUNC_math_Vec3_rotate(val_vec, val_axis, val_angle)

// /* Projection and Reflection Operations */
// #define math_Vec_dot(val_lhs, val_rhs)                 FUNC_math_Vec_dot(val_lhs, val_rhs)
// #define math_Vec_project(val_vec, val_onto)            FUNC_math_Vec_project(val_vec, val_onto)
// #define math_Vec_reject(val_vec, val_onto)             FUNC_math_Vec_reject(val_vec, val_onto)
// #define math_Vec_reflect(val_vec, val_normal)          FUNC_math_Vec_reflect(val_vec, val_normal)
// #define math_Vec_refract(val_vec, val_normal, val_eta) FUNC_math_Vec_refract(val_vec, val_normal, val_eta)

// /* Cross Product and Perpendicular Operations */
// #define math_Vec_cross(val_lhs, val_rhs)  FUNC_math_Vec_cross(pp_uniqTok(Vec_cross_lhs), pp_uniqTok(Vec_cross_rhs), val_lhs, val_rhs)
// #define math_Vec3_cross(val_lhs, val_rhs) FUNC_math_Vec3_cross(val_lhs, val_rhs)
// #define math_Vec_ccw(val_lhs, val_rhs)    FUNC_math_Vec_ccw(val_lhs, val_rhs)
// #define math_Vec_cw(val_lhs, val_rhs)     FUNC_math_Vec_cw(val_lhs, val_rhs)
// #define math_Vec2_perp(val_vec)           FUNC_math_Vec2_perp(val_vec)
// #define math_Vec3_perp(val_vec)           FUNC_math_Vec3_perp(val_vec)

// /* Vector Relationship Tests */
// #define math_Vec_isCCW(val_lhs, val_rhs)      FUNC_math_Vec_isCCW(val_lhs, val_rhs)
// #define math_Vec_isCW(val_lhs, val_rhs)       FUNC_math_Vec_isCW(val_lhs, val_rhs)
// #define math_Vec_isColinear(val_lhs, val_rhs) FUNC_math_Vec_isColinear(val_lhs, val_rhs)
// #define math_Vec_isParallel(val_lhs, val_rhs) FUNC_math_Vec_isParallel(val_lhs, val_rhs)

// /* Basis Vectors */
// #define math_Vec2_basis(val_index) FUNC_math_Vec2_basis(val_index)
// #define math_Vec3_basis(val_index) FUNC_math_Vec3_basis(val_index)

/*========== Implementations ================================================*/

/* Constants */
#define VAL_math_Vec_zero$(TVec) ((TVec){ 0.0 })
#define VAL_math_Vec2_one$(TVec) ((TVec){ .x = 1, .y = 1 })
#define VAL_math_Vec3_one$(TVec) ((TVec){ .x = 1, .y = 1, .z = 1 })
#define VAL_math_Vec4_one$(TVec) ((TVec){ .x = 1, .y = 1, .z = 1, .w = 1 })
#define VAL_math_Vec_lt$(TVec)   ((TVec){ .x = -1, .y = 0 })
#define VAL_math_Vec_up$(TVec)   ((TVec){ .x = 0, .y = 1 })
#define VAL_math_Vec_rt$(TVec)   ((TVec){ .x = 1, .y = 0 })
#define VAL_math_Vec_dn$(TVec)   ((TVec){ .x = 0, .y = -1 })
#define VAL_math_Vec3_fwd$(TVec) ((TVec){ .x = 0, .y = 0, .z = 1 })
#define VAL_math_Vec3_bwd$(TVec) ((TVec){ .x = 0, .y = 0, .z = -1 })

/* Vector construction */
#define VAL_math_Vec2$(TVec, val_x, val_y)               ((TVec){ .x = val_x, .y = val_y })
#define VAL_math_Vec3$(TVec, val_x, val_y, val_z)        ((TVec){ .x = val_x, .y = val_y, .z = val_z })
#define VAL_math_Vec4$(TVec, val_x, val_y, val_z, val_w) ((TVec){ .x = val_x, .y = val_y, .z = val_z, .w = val_w })

/* Scalar expansion constructors */
#define FUNC_math_Vec2_fill$(_scalar, TVec, val_scalar) eval({ \
    let _scalar = val_scalar;                                  \
    eval_return((TVec){ .s = { _scalar, _scalar } });          \
})
#define FUNC_math_Vec3_fill$(_scalar, TVec, val_scalar) eval({ \
    let _scalar = val_scalar;                                  \
    eval_return((TVec){ .s = { _scalar, _scalar, _scalar } }); \
})
#define FUNC_math_Vec4_fill$(_scalar, TVec, val_scalar) eval({          \
    let _scalar = val_scalar;                                           \
    eval_return((TVec){ .s = { _scalar, _scalar, _scalar, _scalar } }); \
})
#define FUNC_math_Vec2_fill(_scalar, val_scalar) eval({                     \
    let         _scalar = val_scalar;                                       \
    eval_return math_Vec2_fill$(math_Vec2_Match(TypeOf(_scalar)), _scalar); \
})
#define FUNC_math_Vec3_fill(_scalar, val_scalar) eval({                     \
    let         _scalar = val_scalar;                                       \
    eval_return math_Vec3_fill$(math_Vec3_Match(TypeOf(_scalar)), _scalar); \
})
#define FUNC_math_Vec4_fill(_scalar, val_scalar) eval({                     \
    let         _scalar = val_scalar;                                       \
    eval_return math_Vec4_fill$(math_Vec4_Match(TypeOf(_scalar)), _scalar); \
})

/* Dimension conversion */
#define FUNC_math_Vec2_from$(_vec, TVec, val_vec) eval({ \
    let _vec = (val_vec);                                \
    var _ret = makeCleared(TVec);                        \
    _ret.x   = (_vec).s[0];                              \
    _ret.y   = countOf((_vec).s) > 1 ? (_vec).s[1] : 0;  \
    eval_return _ret;                                    \
})
#define FUNC_math_Vec3_from$(_vec, TVec, val_vec) eval({ \
    let _vec = (val_vec);                                \
    var _ret = makeCleared(TVec);                        \
    _ret.x   = (_vec).s[0];                              \
    _ret.y   = countOf((_vec).s) > 1 ? (_vec).s[1] : 0;  \
    _ret.z   = countOf((_vec).s) > 2 ? (_vec).s[2] : 0;  \
    eval_return _ret;                                    \
})
#define FUNC_math_Vec4_from$(_vec, TVec, val_vec) eval({ \
    let _vec = (val_vec);                                \
    var _ret = makeCleared(TVec);                        \
    _ret.x   = (_vec).s[0];                              \
    _ret.y   = countOf((_vec).s) > 1 ? (_vec).s[1] : 0;  \
    _ret.z   = countOf((_vec).s) > 2 ? (_vec).s[2] : 0;  \
    _ret.w   = countOf((_vec).s) > 3 ? (_vec).s[3] : 1;  \
    eval_return _ret;                                    \
})
#define FUNC_math_Vec2_from(_vec, val_vec) eval({                            \
    let         _vec = (val_vec);                                            \
    eval_return math_Vec2_from$(math_Vec2_Match(TypeOf((_vec).s[0])), _vec); \
})
#define FUNC_math_Vec3_from(_vec, val_vec) eval({                            \
    let         _vec = (val_vec);                                            \
    eval_return math_Vec3_from$(math_Vec3_Match(TypeOf((_vec).s[0])), _vec); \
})
#define FUNC_math_Vec4_from(_vec, val_vec) eval({                            \
    let         _vec = (val_vec);                                            \
    eval_return math_Vec4_from$(math_Vec4_Match(TypeOf((_vec).s[0])), _vec); \
})

/* Vector type conversion */
#define FUNC_math_Vec_as$(TDest, val_vec) eval({          \
    let _vec = (val_vec);                                 \
    var _ret = makeCleared(TDest);                        \
    for (usize i = 0; i < countOf(_ret.s); ++i) {         \
        if (i < countOf(_vec.s)) {                        \
            _ret.s[i] = as(TypeOf(_ret.s[0]), _vec.s[i]); \
        }                                                 \
    };                                                    \
    eval_return _ret;                                     \
})

/* Vector construction for rotation */
#define FUNC_math_Vec2_sincos$(_radians, TVec, val_radians) eval({           \
    let _radians = (val_radians);                                            \
    eval_return((TVec){ .x = math_cos(_radians), .y = math_sin(_radians) }); \
})

/* Comparison operations */
/* Comparison operations based on component-wise comparison */
#define OP_math_Vec_cmp(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = cmp_Ord_equal;                                \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        if (math_lt(_lhs.s[i], _rhs.s[i])) {                 \
            _ret = cmp_Ord_less;                             \
            break;                                           \
        } else if (math_gt(_lhs.s[i], _rhs.s[i])) {          \
            _ret = cmp_Ord_greater;                          \
            break;                                           \
        } else {                                             \
            continue;                                        \
        }                                                    \
    };                                                       \
    eval_return _ret;                                        \
})
/* Redefine other comparison operations in terms of cmp */
#define OP_math_Vec_eq(val_lhs, val_rhs) (math_Vec_cmp(val_lhs, val_rhs) == cmp_Ord_equal)
#define OP_math_Vec_ne(val_lhs, val_rhs) (math_Vec_cmp(val_lhs, val_rhs) != cmp_Ord_equal)
#define OP_math_Vec_lt(val_lhs, val_rhs) (math_Vec_cmp(val_lhs, val_rhs) == cmp_Ord_less)
#define OP_math_Vec_gt(val_lhs, val_rhs) (math_Vec_cmp(val_lhs, val_rhs) == cmp_Ord_greater)
#define OP_math_Vec_le(val_lhs, val_rhs) (!math_Vec_gt(val_lhs, val_rhs))
#define OP_math_Vec_ge(val_lhs, val_rhs) (!math_Vec_lt(val_lhs, val_rhs))

/* Arithmetic operations */
#define OP_math_Vec_neg(_vec, val_vec) eval({     \
    let _vec = (val_vec);                         \
    for (usize i = 0; i < countOf(_vec.s); ++i) { \
        _vec.s[i] = math_neg(_vec.s[i]);          \
    };                                            \
    eval_return _vec;                             \
})
#define OP_math_Vec_add(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = makeCleared(TypeOf(_lhs));                    \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        _ret.s[i] = math_add(_lhs.s[i], _rhs.s[i]);          \
    };                                                       \
    eval_return _ret;                                        \
})
#define OP_math_Vec_sub(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = makeCleared(TypeOf(_lhs));                    \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        _ret.s[i] = math_sub(_lhs.s[i], _rhs.s[i]);          \
    };                                                       \
    eval_return _ret;                                        \
})
#define OP_math_Vec_mul(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = makeCleared(TypeOf(_lhs));                    \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        _ret.s[i] = math_mul(_lhs.s[i], _rhs.s[i]);          \
    };                                                       \
    eval_return _ret;                                        \
})
#define OP_math_Vec_div(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = makeCleared(TypeOf(_lhs));                    \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        _ret.s[i] = math_div(_lhs.s[i], _rhs.s[i]);          \
    };                                                       \
    eval_return _ret;                                        \
})
#define OP_math_Vec_mod(_lhs, _rhs, val_lhs, val_rhs) eval({ \
    let _lhs = (val_lhs);                                    \
    let _rhs = (val_rhs);                                    \
    var _ret = makeCleared(TypeOf(_lhs));                    \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {            \
        _ret.s[i] = math_mod(_lhs.s[i], _rhs.s[i]);          \
    };                                                       \
    eval_return _ret;                                        \
})

/* Vector-scalar operation */
#define OP_math_Vec_scale(_vec, _scalar, val_vec, val_scalar) eval({ \
    let _vec    = (val_vec);                                         \
    let _scalar = (val_scalar);                                      \
    var _ret    = makeCleared(TypeOf(_vec));                         \
    for (usize i = 0; i < countOf(_vec.s); ++i) {                    \
        _ret.s[i] = math_mul(_vec.s[i], _scalar);                    \
    };                                                               \
    eval_return _ret;                                                \
})

// /* Range Operations */
// #define FUNC_math_Vec_min(val_lhs, val_rhs) eval({\
//     let _lhs = (val_lhs);                           \
//     let _rhs = (val_rhs);                           \
//     var _ret = makeCleared(TypeOf(_lhs));           \
//     for (usize i = 0; i < countOf(_lhs.s); ++i) {   \
//         _ret.s[i] = math_min(_lhs.s[i], _rhs.s[i]); \
//     };                                              \
//     eval_return _ret;                               \
//})
// #define FUNC_math_Vec_max(val_lhs, val_rhs) eval({\
//     let _lhs = (val_lhs);                           \
//     let _rhs = (val_rhs);                           \
//     var _ret = makeCleared(TypeOf(_lhs));           \
//     for (usize i = 0; i < countOf(_lhs.s); ++i) {   \
//         _ret.s[i] = math_max(_lhs.s[i], _rhs.s[i]); \
//     };                                              \
//     eval_return _ret;                               \
//})
// #define FUNC_math_Vec_clamp(val_x, val_min, val_max) eval({\
//     let _x   = (val_x);                                        \
//     let _min = (val_min);                                      \
//     let _max = (val_max);                                      \
//     var _ret = makeCleared(TypeOf(_x));                        \
//     for (usize i = 0; i < countOf(_x.s); ++i) {                \
//         _ret.s[i] = math_clamp(_x.s[i], _min.s[i], _max.s[i]); \
//     };                                                         \
//     eval_return _ret;                                          \
//})
// #define FUNC_math_Vec_wrap(val_x, val_min, val_max) eval({\
//     let _x   = (val_x);                                       \
//     let _min = (val_min);                                     \
//     let _max = (val_max);                                     \
//     var _ret = makeCleared(TypeOf(_x));                       \
//     for (usize i = 0; i < countOf(_x.s); ++i) {               \
//         _ret.s[i] = math_wrap(_x.s[i], _min.s[i], _max.s[i]); \
//     };                                                        \
//     eval_return _ret;                                         \
//})

// /* Magnitude and Distance Operations */
// #define FUNC_math_Vec_lenSq(val_vec) eval({\
//     let _vec = (val_vec);                                      \
//     var _ret = makeCleared(TypeOf(_vec.s[0]));                 \
//     for (usize i = 0; i < countOf(_vec.s); ++i) {              \
//         _ret = math_add(_ret, math_mul(_vec.s[i], _vec.s[i])); \
//     };                                                         \
//     eval_return _ret;                                          \
//})
// #define FUNC_math_Vec_len(val_vec)    math_sqrt(math_Vec_lenSq(val_vec))
// #define FUNC_math_Vec_magSq(val_vec)  math_Vec_lenSq(val_vec)
// #define FUNC_math_Vec_mag(val_vec)    math_Vec_len(val_vec)
// #define FUNC_math_Vec_normSq(val_vec) math_Vec_lenSq(_vec)
// #define FUNC_math_Vec_norm(val_vec)   eval({\
//     let _vec = (val_vec);                               \
//     let _len = math_Vec_len(_vec);                      \
//     var _ret = makeCleared(TypeOf(_vec));               \
//     if (!math_eq(_len, 0)) {                            \
//         _ret = math_Vec_scale(_vec, math_div(1, _len)); \
//     };                                                  \
//     eval_return _ret;                                   \
//})
// #define FUNC_math_Vec_distSq(val_lhs, val_rhs) eval({\
//     let         _diff = math_Vec_sub(val_lhs, val_rhs); \
//     var         _ret  = math_Vec_lenSq(_diff);          \
//     eval_return _ret;                                   \
//})
// #define FUNC_math_Vec_dist(val_lhs, val_rhs) math_sqrt(math_Vec_distSq(val_lhs, val_rhs))

// /* Angular Operations */
// #define FUNC_math_Vec_angle(val_lhs, val_rhs) eval({\
//     let _lhs      = (val_lhs);                       \
//     let _rhs      = (val_rhs);                       \
//     let _dot      = math_Vec_dot(_lhs, _rhs);        \
//     let _len_prod = math_mul(                        \
//         math_Vec_len(_lhs),                          \
//         math_Vec_len(_rhs)                           \
//     );                                               \
//     var _ret = makeCleared(TypeOf(_dot));            \
//     if (!math_eq(_len_prod, 0)) {                    \
//         _ret = math_acos(math_div(_dot, _len_prod)); \
//     };                                               \
//     eval_return _ret;                                \
//})
// #define FUNC_math_Vec_shortestArc(val_lhs, val_rhs) eval({\
//     let         _lhs   = (val_lhs);                                                            \
//     let         _rhs   = (val_rhs);                                                            \
//     let         _angle = math_Vec_angle(_lhs, _rhs);                                           \
//     var         _ret   = math_mul(_angle, math_sign(math_Vec_dot(math_Vec_perp(_lhs), _rhs))); \
//     eval_return _ret;                                                                          \
//})

// /* Projection Operations */
// #define FUNC_math_Vec_project(val_vec, val_onto) eval({\
//     let _vec        = (val_vec);                                       \
//     let _onto       = (val_onto);                                      \
//     let _onto_lenSq = math_Vec_lenSq(_onto);                           \
//     var _ret        = makeCleared(TypeOf(_vec));                       \
//     if (!math_eq(_onto_lenSq, 0)) {                                    \
//         let _dot = math_Vec_dot(_vec, _onto);                          \
//         _ret     = math_Vec_scale(_onto, math_div(_dot, _onto_lenSq)); \
//     };                                                                 \
//     eval_return _ret;                                                  \
//})
// #define FUNC_math_Vec_reject(val_vec, val_onto) eval({\
//     let         _vec  = (val_vec);                        \
//     let         _proj = math_Vec_project(_vec, val_onto); \
//     var         _ret  = math_Vec_sub(_vec, _proj);        \
//     eval_return _ret;                                     \
//})
// #define FUNC_math_Vec_reflect(val_vec, val_normal) eval({\
//     let         _vec = (val_vec);                                                 \
//     let         _n   = math_Vec_norm(val_normal);                                 \
//     let         _dot = math_Vec_dot(_vec, _n);                                    \
//     var         _ret = math_Vec_sub(_vec, math_Vec_scale(_n, math_mul(2, _dot))); \
//     eval_return _ret;                                                             \
//})
// #define FUNC_math_Vec_refract(val_vec, val_normal, val_eta) eval({\
//     let _vec = (val_vec);                                                                      \
//     let _n   = math_Vec_norm(val_normal);                                                      \
//     let _eta = (val_eta);                                                                      \
//     let _dot = math_Vec_dot(_vec, _n);                                                         \
//     let _k   = math_sub(1, math_mul(_eta, math_mul(_eta, math_sub(1, math_mul(_dot, _dot))))); \
//     var _ret = makeCleared(TypeOf(_vec));                                                      \
//     if (math_ge(_k, 0)) {                                                                      \
//         _ret = math_Vec_sub(                                                                   \
//             math_Vec_scale(_vec, _eta),                                                        \
//             math_Vec_scale(_n, math_add(math_mul(_eta, _dot), math_sqrt(_k)))                  \
//         );                                                                                     \
//     };                                                                                         \
//     eval_return _ret;                                                                          \
//})

// #define likely(x)   __builtin_expect(!!(x), 1)
// #define unlikely(x) __builtin_expect(!!(x), 0)

// /* Cross Product and Perpendicular Operations */
// #define FUNC_math_Vec_cross(_lhs, _rhs, val_lhs, val_rhs) eval({\
//     let         _lhs = val_lhs;                                                          \
//     let         _rhs = val_rhs;                                                          \
//     eval_return math_Vec3_cross(                                                         \
//         __builtin_choose_expr(likely(countOf(_lhs.s) == 3), _lhs, math_Vec3_from(_lhs)), \
//         __builtin_choose_expr(likely(countOf(_rhs.s) == 3), _rhs, math_Vec3_from(_rhs))  \
//     );                                                                                   \
//})
// #define FUNC_math_Vec3_cross(val_lhs, val_rhs) eval({\
//     let _lhs = (val_lhs);                            \
//     let _rhs = (val_rhs);                            \
//     var _ret = makeCleared(TypeOf(_lhs));            \
//     _ret.x   = math_sub(                             \
//         math_mul(_lhs.y, _rhs.z),                  \
//         math_mul(_lhs.z, _rhs.y)                   \
//     );                                             \
//     _ret.y = math_sub(                               \
//         math_mul(_lhs.z, _rhs.x),                    \
//         math_mul(_lhs.x, _rhs.z)                     \
//     );                                               \
//     _ret.z = math_sub(                               \
//         math_mul(_lhs.x, _rhs.y),                    \
//         math_mul(_lhs.y, _rhs.x)                     \
//     );                                               \
//     eval_return _ret;                                \
//})
// #define FUNC_math_Vec2_perp(val_vec) eval({\
//     let _vec = (val_vec);                  \
//     var _ret = makeCleared(TypeOf(_vec));  \
//     _ret.x   = -_vec.y;                    \
//     _ret.y   = _vec.x;                     \
//     eval_return _ret;                      \
//})
// #define FUNC_math_Vec3_perp(val_vec) eval({\
//     let _vec   = (val_vec);                            \
//     let _abs_x = math_abs(_vec.x);                     \
//     let _abs_y = math_abs(_vec.y);                     \
//     let _abs_z = math_abs(_vec.z);                     \
//     var _ret   = makeCleared(TypeOf(_vec));            \
//     if (_abs_x <= _abs_y && _abs_x <= _abs_z) {        \
//         _ret = (TypeOf(_vec)){                         \
//             .x = 0,                                    \
//             .y = -_vec.z,                              \
//             .z = _vec.y                                \
//         };                                             \
//     } else if (_abs_y <= _abs_x && _abs_y <= _abs_z) { \
//         _ret = (TypeOf(_vec)){                         \
//             .x = -_vec.z,                              \
//             .y = 0,                                    \
//             .z = _vec.x                                \
//         };                                             \
//     } else {                                           \
//         _ret = (TypeOf(_vec)){                         \
//             .x = -_vec.y,                              \
//             .y = _vec.x,                               \
//             .z = 0                                     \
//         };                                             \
//     };                                                 \
//     eval_return _ret;                                  \
//})

// /* CCW/CW operations for 2D vectors */
// #define FUNC_math_Vec_ccw(val_lhs, val_rhs) eval({\
//     let _lhs = (val_lhs);                         \
//     let _rhs = (val_rhs);                         \
//     var _ret = math_sub(                          \
//         math_mul(_lhs.x, _rhs.y),                 \
//         math_mul(_lhs.y, _rhs.x)                  \
//     );                                            \
//     eval_return _ret;                             \
//})

// #define FUNC_math_Vec_cw(val_lhs, val_rhs) eval({\
//     let _lhs = (val_lhs);                        \
//     let _rhs = (val_rhs);                        \
//     var _ret = math_sub(                         \
//         math_mul(_lhs.y, _rhs.x),                \
//         math_mul(_lhs.x, _rhs.y)                 \
//     );                                           \
//     eval_return _ret;                            \
//})

// /* Perpendicular operations split by dimension */
// #define math_Vec_perp(val_vec) _Generic((val_vec), Vec2f: math_Vec2_perp, Vec3f: math_Vec3_perp)(val_vec)

// /* Vector tests redefined for 2D vectors */
// #define FUNC_math_Vec_isCCW(val_lhs, val_rhs) eval({\
//     var         _ret = math_gt(math_Vec_ccw(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                              \
//})

// #define FUNC_math_Vec_isCW(val_lhs, val_rhs) eval({\
//     var         _ret = math_gt(math_Vec_cw(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                             \
//})

// #define FUNC_math_Vec_isColinear(val_lhs, val_rhs) eval({\
//     var         _ret = math_eq(math_Vec_ccw(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                              \
//})

// /* ShortestArc redefined correctly for 2D */
// #define FUNC_math_Vec_shortestArc(val_lhs, val_rhs) eval({\
//     let         _lhs   = (val_lhs);                                             \
//     let         _rhs   = (val_rhs);                                             \
//     let         _angle = math_Vec_angle(_lhs, _rhs);                            \
//     var         _ret   = math_mul(_angle, math_sign(math_Vec_ccw(_lhs, _rhs))); \
//     eval_return _ret;                                                           \
//})

// /* Vector Tests */
// #define FUNC_math_Vec_isCCW(val_lhs, val_rhs) eval({\
//     var         _ret = math_gt(math_Vec_cross(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                                \
//})
// #define FUNC_math_Vec_isCW(val_lhs, val_rhs) eval({\
//     var         _ret = math_lt(math_Vec_cross(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                                \
//})
// #define FUNC_math_Vec_isColinear(val_lhs, val_rhs) eval({\
//     var         _ret = math_eq(math_Vec_cross(val_lhs, val_rhs), 0); \
//     eval_return _ret;                                                \
//})
// #define FUNC_math_Vec_isParallel(val_lhs, val_rhs) eval({\
//     var         _ret = math_Vec_isColinear(val_lhs, val_rhs); \
//     eval_return _ret;                                         \
//})

// /* Basis Vectors */
// #define FUNC_math_Vec2_basis(val_index) eval({\
//     let _index = (val_index);                 \
//     var _ret   = makeCleared(Vec2f);          \
//     if (0 <= _index && _index < 2) {          \
//         _ret.s[_index] = 1;                   \
//     };                                        \
//     eval_return _ret;                         \
//})
// #define FUNC_math_Vec3_basis(val_index) eval({\
//     let _index = (val_index);                 \
//     var _ret   = makeCleared(Vec3f);          \
//     if (0 <= _index && _index < 3) {          \
//         _ret.s[_index] = 1;                   \
//     };                                        \
//     eval_return _ret;                         \
//})

#endif /* MATH_VEC_INCLUDED */
