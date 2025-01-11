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

#define math_Vec2_sincos$(TVector, val_radians) FUNC_math_Vec2_sincos$(pp_uniqueToken(_radians), TVector, val_radians)

/* Basic Vector-Vector Comparison Operations */
#define math_Vec_cmp(val_lhs, val_rhs) OP_math_Vec_cmp(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs) /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_eq(val_lhs, val_rhs)  OP_math_Vec_eq(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)  /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_ne(val_lhs, val_rhs)  OP_math_Vec_ne(val_lhs, val_rhs)                                              /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_lt(val_lhs, val_rhs)  OP_math_Vec_lt(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)  /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_gt(val_lhs, val_rhs)  OP_math_Vec_gt(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)  /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_le(val_lhs, val_rhs)  OP_math_Vec_le(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)  /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_Vec_ge(val_lhs, val_rhs)  OP_math_Vec_ge(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)  /* NOLINT(bugprone-assignment-in-if-condition) */

/* Core Vector-Vector Arithmetic */
#define math_Vec_neg(val_vec)          OP_math_Vec_neg(pp_uniqueToken(_vec), val_vec)
#define math_Vec_add(val_lhs, val_rhs) OP_math_Vec_add(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)
#define math_Vec_sub(val_lhs, val_rhs) OP_math_Vec_sub(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)
#define math_Vec_mul(val_lhs, val_rhs) OP_math_Vec_mul(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)
#define math_Vec_div(val_lhs, val_rhs) OP_math_Vec_div(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)
#define math_Vec_mod(val_lhs, val_rhs) OP_math_Vec_mod(pp_uniqueToken(_lhs), pp_uniqueToken(_rhs), val_lhs, val_rhs)

/* Vector-Scalar Operations */
#define math_Vec_scale(val_vec, val_scalar) OP_math_Vec_scale(pp_uniqueToken(_lhs_vec), pp_uniqueToken(_scalar), val_vec, val_scalar)

/* Range Operations */
#define math_Vec_min(val_lhs, val_rhs)          FUNC_math_Vec_min(val_lhs, val_rhs)
#define math_Vec_max(val_lhs, val_rhs)          FUNC_math_Vec_max(val_lhs, val_rhs)
#define math_Vec_clamp(val_x, val_min, val_max) FUNC_math_Vec_clamp(val_x, val_min, val_max)
#define math_Vec_wrap(val_x, val_min, val_max)  FUNC_math_Vec_wrap(val_x, val_min, val_max)

/* Magnitude and Distance Operations */
#define math_Vec_lenSq(val_vec)           FUNC_math_Vec_lenSq(val_vec)
#define math_Vec_len(val_vec)             FUNC_math_Vec_len(val_vec)
#define math_Vec_magSq(val_vec)           FUNC_math_Vec_magSq(val_vec)
#define math_Vec_mag(val_vec)             FUNC_math_Vec_mag(val_vec)
#define math_Vec_normSq(val_vec)          FUNC_math_Vec_normSq(val_vec)
#define math_Vec_norm(val_vec)            FUNC_math_Vec_norm(val_vec)
#define math_Vec_distSq(val_lhs, val_rhs) FUNC_math_Vec_distSq(val_lhs, val_rhs)
#define math_Vec_dist(val_lhs, val_rhs)   FUNC_math_Vec_dist(val_lhs, val_rhs)

/* Angular Operations */
#define math_Vec_angle(val_lhs, val_rhs)               FUNC_math_Vec_angle(val_lhs, val_rhs)
#define math_Vec_shortestArc(val_lhs, val_rhs)         FUNC_math_Vec_shortestArc(val_lhs, val_rhs)
#define math_Vec2_rotate(val_vec, val_angle)           FUNC_math_Vec2_rotate(val_vec, val_angle)
#define math_Vec3_rotate(val_vec, val_axis, val_angle) FUNC_math_Vec3_rotate(val_vec, val_axis, val_angle)

/* Projection and Reflection Operations */
#define math_Vec_dot(val_lhs, val_rhs)                 FUNC_math_Vec_dot(val_lhs, val_rhs)
#define math_Vec_project(val_vec, val_onto)            FUNC_math_Vec_project(val_vec, val_onto)
#define math_Vec_reject(val_vec, val_onto)             FUNC_math_Vec_reject(val_vec, val_onto)
#define math_Vec_reflect(val_vec, val_normal)          FUNC_math_Vec_reflect(val_vec, val_normal)
#define math_Vec_refract(val_vec, val_normal, val_eta) FUNC_math_Vec_refract(val_vec, val_normal, val_eta)

/* Cross Product and Perpendicular Operations */
#define math_Vec3_cross(val_lhs, val_rhs) FUNC_math_Vec3_cross(val_lhs, val_rhs)
#define math_Vec_ccw(val_lhs, val_rhs)    FUNC_math_Vec_ccw(val_lhs, val_rhs)
#define math_Vec_cw(val_lhs, val_rhs)     FUNC_math_Vec_cw(val_lhs, val_rhs)
#define math_Vec2_perp(val_vec)           FUNC_math_Vec2_perp(val_vec)
#define math_Vec3_perp(val_vec)           FUNC_math_Vec3_perp(val_vec)

/* Vector Relationship Tests */
#define math_Vec_isCCW(val_lhs, val_rhs)      FUNC_math_Vec_isCCW(val_lhs, val_rhs)
#define math_Vec_isCW(val_lhs, val_rhs)       FUNC_math_Vec_isCW(val_lhs, val_rhs)
#define math_Vec_isColinear(val_lhs, val_rhs) FUNC_math_Vec_isColinear(val_lhs, val_rhs)
#define math_Vec_isParallel(val_lhs, val_rhs) FUNC_math_Vec_isParallel(val_lhs, val_rhs)

/* Basis Vectors */
#define math_Vec2_basis(val_index) FUNC_math_Vec2_basis(val_index)
#define math_Vec3_basis(val_index) FUNC_math_Vec3_basis(val_index)

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

#define FUNC_math_Vec2_sincos$(_radians, TVector, val_radians) eval(            \
    let _radians = (val_radians);                                               \
    eval_return((TVector){ .x = math_cos(_radians), .y = math_sin(_radians) }); \
)

/* Comparison operations */
#define OP_math_Vec_eq(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let  _lhs = val_lhs;                                   \
    let  _rhs = val_rhs;                                   \
    bool _ret = true;                                      \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {          \
        if (!math_eq(_lhs.s[i], _rhs.s[i])) {              \
            _ret = false;                                  \
            break;                                         \
        }                                                  \
    };                                                     \
    eval_return _ret;                                      \
)
#define OP_math_Vec_ne(val_lhs, val_rhs)             (!math_Vec_eq(val_lhs, val_rhs))
#define OP_math_Vec_lt(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let  _lhs = (val_lhs);                                 \
    let  _rhs = (val_rhs);                                 \
    bool _ret = true;                                      \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {          \
        if (!math_lt(_lhs.s[i], _rhs.s[i])) {              \
            _ret = false;                                  \
            break;                                         \
        }                                                  \
    };                                                     \
    eval_return _ret;                                      \
)
#define OP_math_Vec_gt(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let  _lhs = (val_lhs);                                 \
    let  _rhs = (val_rhs);                                 \
    bool _ret = true;                                      \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {          \
        if (!math_gt(_lhs.s[i], _rhs.s[i])) {              \
            _ret = false;                                  \
            break;                                         \
        }                                                  \
    };                                                     \
    eval_return _ret;                                      \
)
#define OP_math_Vec_le(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let  _lhs = (val_lhs);                                 \
    let  _rhs = (val_rhs);                                 \
    bool _ret = true;                                      \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {          \
        if (!math_le(_lhs.s[i], _rhs.s[i])) {              \
            _ret = false;                                  \
            break;                                         \
        }                                                  \
    };                                                     \
    eval_return _ret;                                      \
)
#define OP_math_Vec_ge(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let  _lhs = (val_lhs);                                 \
    let  _rhs = (val_rhs);                                 \
    bool _ret = true;                                      \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {          \
        if (!math_ge(_lhs.s[i], _rhs.s[i])) {              \
            _ret = false;                                  \
            break;                                         \
        }                                                  \
    };                                                     \
    eval_return _ret;                                      \
)

/* Arithmetic operations */
#define OP_math_Vec_neg(_vec, val_vec) eval(      \
    let _vec = (val_vec);                         \
    for (usize i = 0; i < countOf(_vec.s); ++i) { \
        _vec.s[i] = math_neg(_vec.s[i]);          \
    };                                            \
    eval_return _vec;                             \
)
#define OP_math_Vec_add(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                                   \
    let _rhs = (val_rhs);                                   \
    var _ret = makeCleared(TypeOf(_lhs));                   \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {           \
        _ret.s[i] = math_add(_lhs.s[i], _rhs.s[i]);         \
    };                                                      \
    eval_return _ret;                                       \
)
#define OP_math_Vec_sub(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                                   \
    let _rhs = (val_rhs);                                   \
    var _ret = makeCleared(TypeOf(_lhs));                   \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {           \
        _ret.s[i] = math_sub(_lhs.s[i], _rhs.s[i]);         \
    };                                                      \
    eval_return _ret;                                       \
)
#define OP_math_Vec_mul(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                                   \
    let _rhs = (val_rhs);                                   \
    var _ret = makeCleared(TypeOf(_lhs));                   \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {           \
        _ret.s[i] = math_mul(_lhs.s[i], _rhs.s[i]);         \
    };                                                      \
    eval_return _ret;                                       \
)
#define OP_math_Vec_div(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                                   \
    let _rhs = (val_rhs);                                   \
    var _ret = makeCleared(TypeOf(_lhs));                   \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {           \
        _ret.s[i] = math_div(_lhs.s[i], _rhs.s[i]);         \
    };                                                      \
    eval_return _ret;                                       \
)
#define OP_math_Vec_mod(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                                   \
    let _rhs = (val_rhs);                                   \
    var _ret = makeCleared(TypeOf(_lhs));                   \
    for (usize i = 0; i < countOf(_lhs.s); ++i) {           \
        _ret.s[i] = math_mod(_lhs.s[i], _rhs.s[i]);         \
    };                                                      \
    eval_return _ret;                                       \
)

/* Vector-scalar operation */
#define OP_math_Vec_scale(_vec, _scalar, val_vec, val_scalar) eval( \
    let _vec    = (val_vec);                                        \
    let _scalar = (val_scalar);                                     \
    var _ret    = makeCleared(TypeOf(_vec));                        \
    for (usize i = 0; i < countOf(_vec.s); ++i) {                   \
        _ret.s[i] = math_mul(_vec.s[i], _scalar);                   \
    };                                                              \
    eval_return _ret;                                               \
)

#endif /* MATH_VEC_INCLUDED */
