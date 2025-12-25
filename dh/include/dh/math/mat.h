/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mat.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-24 (date of creation)
 * @updated 2025-12-25 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/math
 * @prefix  m_M
 *
 * @brief   Mathematical common matrix operations
 * @details Provides a set of matrix mathematical operations and functions
 */
#ifndef math_mat__included
#define math_mat__included 1

/*========== Includes =======================================================*/

#include "mat_types.h"
#include "vec.h"

/*========== M2f32 Constants and Declarations ===============================*/

/* Constants */
#define m_M2f32_zero __comp_const__m_M2f32_zero
#define m_M2f32_identity __comp_const__m_M2f32_identity

/* Creation */
#define m_M2f32_ofCols_static(_c0, _c1...) ____m_M2f32_ofCols_static(_c0, _c1)
#define m_M2f32_ofRows_static(_r0, _r1...) ____m_M2f32_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2f32_ofCols(m_M2f32_ColV c0, m_M2f32_ColV c1))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_ofRows(m_M2f32_RowV r0, m_M2f32_RowV r1))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_splat(f32 scalar))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_diag(f32 d))(m_M2f32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2f32_neg(m_M2f32 m))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_add(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_sub(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2f32_scal(m_M2f32 m, f32 scalar))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_scalInv(m_M2f32 m, f32 scalar))(m_M2f32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2f32_mulM(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_mulV(m_M2f32 m, m_V2f32 v))(m_V2f32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2f32_col(m_M2f32 m, usize idx))(m_M2f32_ColV));
$attr($inline_always)
$static fn_((m_M2f32_row(m_M2f32 m, usize idx))(m_M2f32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2f32_transpose(m_M2f32 m))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_det(m_M2f32 m))(f32));
$attr($inline_always)
$static fn_((m_M2f32_inv(m_M2f32 m))(m_M2f32));

/* Transformations */
$attr($inline_always)
$static fn_((m_M2f32_rotate(f32 angle))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_scale(m_V2f32 s))(m_M2f32));
$attr($inline_always)
$static fn_((m_M2f32_scaleUniform(f32 s))(m_M2f32));

/*========== M3f32 Constants and Declarations ===============================*/

/* Constants */
#define m_M3f32_zero __comp_const__m_M3f32_zero
#define m_M3f32_identity __comp_const__m_M3f32_identity

/* Creation */
#define m_M3f32_ofCols_static(_c0, _c1, _c2...) ____m_M3f32_ofCols_static(_c0, _c1, _c2)
#define m_M3f32_ofRows_static(_r0, _r1, _r2...) ____m_M3f32_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3f32_ofCols(m_M3f32_ColV c0, m_M3f32_ColV c1, m_M3f32_ColV c2))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_ofRows(m_M3f32_RowV r0, m_M3f32_RowV r1, m_M3f32_RowV r2))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_splat(f32 scalar))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_diag(f32 d))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_fromM4(m_M4f32 m))(m_M3f32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3f32_neg(m_M3f32 m))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_add(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_sub(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3f32_scal(m_M3f32 m, f32 scalar))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_scalInv(m_M3f32 m, f32 scalar))(m_M3f32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3f32_mulM(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_mulV(m_M3f32 m, m_V3f32 v))(m_V3f32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3f32_col(m_M3f32 m, usize idx))(m_V3f32));
$attr($inline_always)
$static fn_((m_M3f32_row(m_M3f32 m, usize idx))(m_V3f32));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3f32_transpose(m_M3f32 m))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_det(m_M3f32 m))(f32));
$attr($inline_always)
$static fn_((m_M3f32_inv(m_M3f32 m))(m_M3f32));

/* 2D Transformations (homogeneous coordinates) */
$attr($inline_always)
$static fn_((m_M3f32_translate2D(m_V2f32 t))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_rotate2D(f32 angle))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_scale2D(m_V2f32 s))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_scaleUniform2D(f32 s))(m_M3f32));

/*========== M4f32 Constants and Declarations ===============================*/

/* Constants */
#define m_M4f32_zero __comp_const__m_M4f32_zero
#define m_M4f32_identity __comp_const__m_M4f32_identity

/* Creation */
#define m_M4f32_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4f32_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4f32_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4f32_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4f32_ofCols(m_M4f32_ColV c0, m_M4f32_ColV c1, m_M4f32_ColV c2, m_M4f32_ColV c3))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_ofRows(m_M4f32_RowV r0, m_M4f32_RowV r1, m_M4f32_RowV r2, m_M4f32_RowV r3))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_splat(f32 scalar))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_diag(f32 d))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_fromM3(m_M3f32 m))(m_M4f32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4f32_neg(m_M4f32 m))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_add(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_sub(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4f32_scal(m_M4f32 m, f32 scalar))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_scalInv(m_M4f32 m, f32 scalar))(m_M4f32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4f32_mulM(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_mulV(m_M4f32 m, m_V4f32 v))(m_V4f32));
$attr($inline_always)
$static fn_((m_M4f32_mulV3(m_M4f32 m, m_V3f32 v, f32 w))(m_V3f32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4f32_col(m_M4f32 m, usize idx))(m_V4f32));
$attr($inline_always)
$static fn_((m_M4f32_row(m_M4f32 m, usize idx))(m_V4f32));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4f32_transpose(m_M4f32 m))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_det(m_M4f32 m))(f32));
$attr($inline_always)
$static fn_((m_M4f32_inv(m_M4f32 m))(m_M4f32));

/* 3D Transformations */
$attr($inline_always)
$static fn_((m_M4f32_translate(m_V3f32 t))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_rotateX(f32 angle))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_rotateY(f32 angle))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_rotateZ(f32 angle))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_rotateAxis(m_V3f32 axis, f32 angle))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_scale(m_V3f32 s))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_scaleUniform(f32 s))(m_M4f32));

/* View and Projection */
$attr($inline_always)
$static fn_((m_M4f32_lookAt(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspective(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectiveInfFar(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_frustum(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));

/*========== M2f32 Implementations ==========================================*/

#define __comp_const__m_M2f32_zero m_M2f32_ofCols_static( \
    m_V2f32_zero, \
    m_V2f32_zero \
)
#define __comp_const__m_M2f32_identity m_M2f32_ofCols_static( \
    m_V2f32_unit_x, \
    m_V2f32_unit_y \
)

#define ____m_M2f32_ofCols_static(_c0, _c1...) lit$((m_M2f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2f32_ofRows_static(_r0, _r1...) lit$((m_M2f32){ \
    .cols = A_init({ \
        [0] = m_V2f32_of_static((_r0).x, (_r0).y), \
        [1] = m_V2f32_of_static((_r1).x, (_r1).y), \
    }), \
})
fn_((m_M2f32_ofCols(m_M2f32_ColV c0, m_M2f32_ColV c1))(m_M2f32)) {
    return m_M2f32_ofCols_static(c0, c1);
};
fn_((m_M2f32_ofRows(m_M2f32_RowV r0, m_M2f32_RowV r1))(m_M2f32)) {
    return m_M2f32_ofRows_static(r0, r1);
};
fn_((m_M2f32_splat(f32 scalar))(m_M2f32)) {
    let v = m_V2f32_splat(scalar);
    return m_M2f32_ofCols(v, v);
};
fn_((m_M2f32_diag(f32 d))(m_M2f32)) {
    return m_M2f32_ofRows(
        m_V2f32_of(d, 0.0f),
        m_V2f32_of(0.0f, d)
    );
};

fn_((m_M2f32_neg(m_M2f32 m))(m_M2f32)) {
    return m_M2f32_ofCols(
        m_V2f32_neg(*A_at((m.cols)[0])),
        m_V2f32_neg(*A_at((m.cols)[1]))
    );
};
fn_((m_M2f32_add(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32)) {
    return m_M2f32_ofCols(
        m_V2f32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2f32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};
fn_((m_M2f32_sub(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32)) {
    return m_M2f32_ofCols(
        m_V2f32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2f32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2f32_scal(m_M2f32 m, f32 scalar))(m_M2f32)) {
    return m_M2f32_ofCols(
        m_V2f32_scal(*A_at((m.cols)[0]), scalar),
        m_V2f32_scal(*A_at((m.cols)[1]), scalar)
    );
};
fn_((m_M2f32_scalInv(m_M2f32 m, f32 scalar))(m_M2f32)) {
    return m_M2f32_scal(m, 1.0f / scalar);
};

fn_((m_M2f32_mulM(m_M2f32 lhs, m_M2f32 rhs))(m_M2f32)) {
    return m_M2f32_ofCols(
        m_V2f32_of(
            lhs.s0_0 * rhs.s0_0 + lhs.s0_1 * rhs.s1_0,
            lhs.s1_0 * rhs.s0_0 + lhs.s1_1 * rhs.s1_0
        ),
        m_V2f32_of(
            lhs.s0_0 * rhs.s0_1 + lhs.s0_1 * rhs.s1_1,
            lhs.s1_0 * rhs.s0_1 + lhs.s1_1 * rhs.s1_1
        )
    );
};
fn_((m_M2f32_mulV(m_M2f32 m, m_V2f32 v))(m_V2f32)) {
    return m_V2f32_of(
        m.s0_0 * v.x + m.s0_1 * v.y,
        m.s1_0 * v.x + m.s1_1 * v.y
    );
};

fn_((m_M2f32_col(m_M2f32 m, usize idx))(m_V2f32)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M2f32_row(m_M2f32 m, usize idx))(m_V2f32)) {
    return m_V2f32_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx])
    );
};

fn_((m_M2f32_transpose(m_M2f32 m))(m_M2f32)) {
    return m_M2f32_ofRows(
        m_V2f32_of(m.s0_0, m.s1_0),
        m_V2f32_of(m.s0_1, m.s1_1)
    );
};
fn_((m_M2f32_det(m_M2f32 m))(f32)) {
    return m.s0_0 * m.s1_1 - m.s0_1 * m.s1_0;
};
fn_((m_M2f32_inv(m_M2f32 m))(m_M2f32)) {
    let det = m_M2f32_det(m);
    let inv_det = 1.0f / det;
    return m_M2f32_ofRows(
        m_V2f32_of(m.s1_1 * inv_det, -m.s0_1 * inv_det),
        m_V2f32_of(-m.s1_0 * inv_det, m.s0_0 * inv_det)
    );
};

fn_((m_M2f32_rotate(f32 angle))(m_M2f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M2f32_ofRows(
        m_V2f32_of(sc.y, -sc.x),
        m_V2f32_of(sc.x, sc.y)
    );
};
fn_((m_M2f32_scale(m_V2f32 s))(m_M2f32)) {
    return m_M2f32_ofRows(
        m_V2f32_of(s.x, 0.0f),
        m_V2f32_of(0.0f, s.y)
    );
};
fn_((m_M2f32_scaleUniform(f32 s))(m_M2f32)) {
    return m_M2f32_diag(s);
};

/*========== M3f32 Implementations ==========================================*/

#define __comp_const__m_M3f32_zero m_M3f32_ofCols_static( \
    m_V3f32_zero, \
    m_V3f32_zero, \
    m_V3f32_zero \
)
#define __comp_const__m_M3f32_identity m_M3f32_ofCols_static( \
    m_V3f32_unit_x, \
    m_V3f32_unit_y, \
    m_V3f32_unit_z \
)

#define ____m_M3f32_ofCols_static(_c0, _c1, _c2...) lit$((m_M3f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3f32_ofRows_static(_r0, _r1, _r2...) lit$((m_M3f32){ \
    .cols = A_init({ \
        [0] = m_V3f32_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3f32_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3f32_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3f32_ofCols(m_M3f32_ColV c0, m_M3f32_ColV c1, m_M3f32_ColV c2))(m_M3f32)) {
    return m_M3f32_ofCols_static(c0, c1, c2);
};
fn_((m_M3f32_ofRows(m_M3f32_RowV r0, m_M3f32_RowV r1, m_M3f32_RowV r2))(m_M3f32)) {
    return m_M3f32_ofRows_static(r0, r1, r2);
};
fn_((m_M3f32_splat(f32 scalar))(m_M3f32)) {
    let v = m_V3f32_splat(scalar);
    return m_M3f32_ofCols(v, v, v);
};
fn_((m_M3f32_diag(f32 d))(m_M3f32)) {
    return m_M3f32_ofRows(
        m_V3f32_of(d, 0.0f, 0.0f),
        m_V3f32_of(0.0f, d, 0.0f),
        m_V3f32_of(0.0f, 0.0f, d)
    );
};
fn_((m_M3f32_fromM4(m_M4f32 m))(m_M3f32)) {
    return m_M3f32_ofCols(
        m_V3f32_from4(*A_at((m.cols)[0])),
        m_V3f32_from4(*A_at((m.cols)[1])),
        m_V3f32_from4(*A_at((m.cols)[2]))
    );
};

fn_((m_M3f32_neg(m_M3f32 m))(m_M3f32)) {
    return m_M3f32_ofCols(
        m_V3f32_neg(*A_at((m.cols)[0])),
        m_V3f32_neg(*A_at((m.cols)[1])),
        m_V3f32_neg(*A_at((m.cols)[2]))
    );
};
fn_((m_M3f32_add(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32)) {
    return m_M3f32_ofCols(
        m_V3f32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3f32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3f32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};
fn_((m_M3f32_sub(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32)) {
    return m_M3f32_ofCols(
        m_V3f32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3f32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3f32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3f32_scal(m_M3f32 m, f32 scalar))(m_M3f32)) {
    return m_M3f32_ofCols(
        m_V3f32_scal(*A_at((m.cols)[0]), scalar),
        m_V3f32_scal(*A_at((m.cols)[1]), scalar),
        m_V3f32_scal(*A_at((m.cols)[2]), scalar)
    );
};
fn_((m_M3f32_scalInv(m_M3f32 m, f32 scalar))(m_M3f32)) {
    return m_M3f32_scal(m, 1.0f / scalar);
};

fn_((m_M3f32_mulM(m_M3f32 lhs, m_M3f32 rhs))(m_M3f32)) {
    m_M3f32 result = m_M3f32_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            f32 sum = 0.0f;
            for (usize k = 0; k < 3; ++k) {
                sum += *A_at((A_at((lhs.cols)[k])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[k]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};
fn_((m_M3f32_mulV(m_M3f32 m, m_V3f32 v))(m_V3f32)) {
    return m_V3f32_of(
        m.s0_0 * v.x + m.s0_1 * v.y + m.s0_2 * v.z,
        m.s1_0 * v.x + m.s1_1 * v.y + m.s1_2 * v.z,
        m.s2_0 * v.x + m.s2_1 * v.y + m.s2_2 * v.z
    );
};

fn_((m_M3f32_col(m_M3f32 m, usize idx))(m_V3f32)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M3f32_row(m_M3f32 m, usize idx))(m_V3f32)) {
    return m_V3f32_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx]),
        *A_at((A_at((m.cols)[2])->s)[idx])
    );
};

fn_((m_M3f32_transpose(m_M3f32 m))(m_M3f32)) {
    return m_M3f32_ofRows(
        m_V3f32_of(m.s0_0, m.s1_0, m.s2_0),
        m_V3f32_of(m.s0_1, m.s1_1, m.s2_1),
        m_V3f32_of(m.s0_2, m.s1_2, m.s2_2)
    );
};
fn_((m_M3f32_det(m_M3f32 m))(f32)) {
    return m.s0_0 * (m.s1_1 * m.s2_2 - m.s1_2 * m.s2_1)
         - m.s0_1 * (m.s1_0 * m.s2_2 - m.s1_2 * m.s2_0)
         + m.s0_2 * (m.s1_0 * m.s2_1 - m.s1_1 * m.s2_0);
};
fn_((m_M3f32_inv(m_M3f32 m))(m_M3f32)) {
    let det = m_M3f32_det(m);
    let inv_det = 1.0f / det;
    return m_M3f32_ofRows(
        m_V3f32_of(
            (m.s1_1 * m.s2_2 - m.s2_1 * m.s1_2) * inv_det,
            (m.s2_0 * m.s1_2 - m.s1_0 * m.s2_2) * inv_det,
            (m.s1_0 * m.s2_1 - m.s2_0 * m.s1_1) * inv_det
        ),
        m_V3f32_of(
            (m.s2_1 * m.s0_2 - m.s0_1 * m.s2_2) * inv_det,
            (m.s0_0 * m.s2_2 - m.s2_0 * m.s0_2) * inv_det,
            (m.s2_0 * m.s0_1 - m.s0_0 * m.s2_1) * inv_det
        ),
        m_V3f32_of(
            (m.s0_1 * m.s1_2 - m.s1_1 * m.s0_2) * inv_det,
            (m.s1_0 * m.s0_2 - m.s0_0 * m.s1_2) * inv_det,
            (m.s0_0 * m.s1_1 - m.s1_0 * m.s0_1) * inv_det
        )
    );
};

fn_((m_M3f32_translate2D(m_V2f32 t))(m_M3f32)) {
    return m_M3f32_ofRows(
        m_V3f32_of(1.0f, 0.0f, t.x),
        m_V3f32_of(0.0f, 1.0f, t.y),
        m_V3f32_of(0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M3f32_rotate2D(f32 angle))(m_M3f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M3f32_ofRows(
        m_V3f32_of(sc.y, -sc.x, 0.0f),
        m_V3f32_of(sc.x, sc.y, 0.0f),
        m_V3f32_of(0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M3f32_scale2D(m_V2f32 s))(m_M3f32)) {
    return m_M3f32_ofRows(
        m_V3f32_of(s.x, 0.0f, 0.0f),
        m_V3f32_of(0.0f, s.y, 0.0f),
        m_V3f32_of(0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M3f32_scaleUniform2D(f32 s))(m_M3f32)) {
    return m_M3f32_scale2D(m_V2f32_splat(s));
};

/*========== M4f32 Implementations ==========================================*/

#define __comp_const__m_M4f32_zero m_M4f32_ofCols_static( \
    m_V4f32_zero, \
    m_V4f32_zero, \
    m_V4f32_zero, \
    m_V4f32_zero \
)
#define __comp_const__m_M4f32_identity m_M4f32_ofCols_static( \
    m_V4f32_unit_x, \
    m_V4f32_unit_y, \
    m_V4f32_unit_z, \
    m_V4f32_unit_w \
)

#define ____m_M4f32_ofCols_static(_c0, _c1, _c2, _c3...) lit$((m_M4f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4f32_ofRows_static(_r0, _r1, _r2, _r3...) lit$((m_M4f32){ \
    .cols = A_init({ \
        [0] = m_V4f32_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4f32_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4f32_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4f32_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4f32_ofCols(m_M4f32_ColV c0, m_M4f32_ColV c1, m_M4f32_ColV c2, m_M4f32_ColV c3))(m_M4f32)) {
    return m_M4f32_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4f32_ofRows(m_M4f32_RowV r0, m_M4f32_RowV r1, m_M4f32_RowV r2, m_M4f32_RowV r3))(m_M4f32)) {
    return m_M4f32_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4f32_splat(f32 scalar))(m_M4f32)) {
    let v = m_V4f32_splat(scalar);
    return m_M4f32_ofCols(v, v, v, v);
};
fn_((m_M4f32_diag(f32 d))(m_M4f32)) {
    return m_M4f32_ofRows(
        m_V4f32_of(d, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, d, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, d, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, d)
    );
};
fn_((m_M4f32_fromM3(m_M3f32 m))(m_M4f32)) {
    return m_M4f32_ofCols(
        m_V4f32_of(A_at((m.cols)[0])->x, A_at((m.cols)[0])->y, A_at((m.cols)[0])->z, 0.0f),
        m_V4f32_of(A_at((m.cols)[1])->x, A_at((m.cols)[1])->y, A_at((m.cols)[1])->z, 0.0f),
        m_V4f32_of(A_at((m.cols)[2])->x, A_at((m.cols)[2])->y, A_at((m.cols)[2])->z, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};

fn_((m_M4f32_neg(m_M4f32 m))(m_M4f32)) {
    return m_M4f32_ofCols(
        m_V4f32_neg(*A_at((m.cols)[0])),
        m_V4f32_neg(*A_at((m.cols)[1])),
        m_V4f32_neg(*A_at((m.cols)[2])),
        m_V4f32_neg(*A_at((m.cols)[3]))
    );
};
fn_((m_M4f32_add(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32)) {
    return m_M4f32_ofCols(
        m_V4f32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4f32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4f32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4f32_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};
fn_((m_M4f32_sub(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32)) {
    return m_M4f32_ofCols(
        m_V4f32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4f32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4f32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4f32_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4f32_scal(m_M4f32 m, f32 scalar))(m_M4f32)) {
    return m_M4f32_ofCols(
        m_V4f32_scal(*A_at((m.cols)[0]), scalar),
        m_V4f32_scal(*A_at((m.cols)[1]), scalar),
        m_V4f32_scal(*A_at((m.cols)[2]), scalar),
        m_V4f32_scal(*A_at((m.cols)[3]), scalar)
    );
};
fn_((m_M4f32_scalInv(m_M4f32 m, f32 scalar))(m_M4f32)) {
    return m_M4f32_scal(m, 1.0f / scalar);
};

fn_((m_M4f32_mulM(m_M4f32 lhs, m_M4f32 rhs))(m_M4f32)) {
    m_M4f32 result = m_M4f32_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            f32 sum = 0.0f;
            for (usize k = 0; k < 4; ++k) {
                sum += *A_at((A_at((lhs.cols)[k])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[k]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4f32_mulV(m_M4f32 m, m_V4f32 v))(m_V4f32)) {
    return m_V4f32_of(
        m.s0_0 * v.x + m.s0_1 * v.y + m.s0_2 * v.z + m.s0_3 * v.w,
        m.s1_0 * v.x + m.s1_1 * v.y + m.s1_2 * v.z + m.s1_3 * v.w,
        m.s2_0 * v.x + m.s2_1 * v.y + m.s2_2 * v.z + m.s2_3 * v.w,
        m.s3_0 * v.x + m.s3_1 * v.y + m.s3_2 * v.z + m.s3_3 * v.w
    );
};
fn_((m_M4f32_mulV3(m_M4f32 m, m_V3f32 v, f32 w))(m_V3f32)) {
    let v4 = m_V4f32_of(v.x, v.y, v.z, w);
    let result = m_M4f32_mulV(m, v4);
    if (w != 0.0f) {
        return m_V3f32_of(result.x / result.w, result.y / result.w, result.z / result.w);
    }
    return m_V3f32_of(result.x, result.y, result.z);
};

fn_((m_M4f32_col(m_M4f32 m, usize idx))(m_V4f32)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M4f32_row(m_M4f32 m, usize idx))(m_V4f32)) {
    return m_V4f32_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx]),
        *A_at((A_at((m.cols)[2])->s)[idx]),
        *A_at((A_at((m.cols)[3])->s)[idx])
    );
};

fn_((m_M4f32_transpose(m_M4f32 m))(m_M4f32)) {
    return m_M4f32_ofRows(
        m_V4f32_of(m.s0_0, m.s1_0, m.s2_0, m.s3_0),
        m_V4f32_of(m.s0_1, m.s1_1, m.s2_1, m.s3_1),
        m_V4f32_of(m.s0_2, m.s1_2, m.s2_2, m.s3_2),
        m_V4f32_of(m.s0_3, m.s1_3, m.s2_3, m.s3_3)
    );
};
fn_((m_M4f32_det(m_M4f32 m))(f32)) {
    /* NOLINTBEGIN(readability-isolate-declaration) */
    let a = m.s0_0, b = m.s0_1, c = m.s0_2, d = m.s0_3;
    let e = m.s1_0, f = m.s1_1, g = m.s1_2, h = m.s1_3;
    let i = m.s2_0, j = m.s2_1, k = m.s2_2, l = m.s2_3;
    let M = m.s3_0, n = m.s3_1, o = m.s3_2, p = m.s3_3;
    /* NOLINTEND(readability-isolate-declaration) */

    let kp_lo = k * p - l * o;
    let jp_ln = j * p - l * n;
    let jo_kn = j * o - k * n;
    let ip_lm = i * p - l * M;
    let io_km = i * o - k * M;
    let in_jm = i * n - j * M;

    return a * (f * kp_lo - g * jp_ln + h * jo_kn)
         - b * (e * kp_lo - g * ip_lm + h * io_km)
         + c * (e * jp_ln - f * ip_lm + h * in_jm)
         - d * (e * jo_kn - f * io_km + g * in_jm);
};
fn_((m_M4f32_inv(m_M4f32 m))(m_M4f32)) {
    /* NOLINTBEGIN(readability-isolate-declaration) */
    let a = m.s0_0, b = m.s0_1, c = m.s0_2, d = m.s0_3;
    let e = m.s1_0, f = m.s1_1, g = m.s1_2, h = m.s1_3;
    let i = m.s2_0, j = m.s2_1, k = m.s2_2, l = m.s2_3;
    let M = m.s3_0, n = m.s3_1, o = m.s3_2, p = m.s3_3;
    /* NOLINTEND(readability-isolate-declaration) */

    let kp_lo = k * p - l * o;
    let jp_ln = j * p - l * n;
    let jo_kn = j * o - k * n;
    let ip_lm = i * p - l * M;
    let io_km = i * o - k * M;
    let in_jm = i * n - j * M;

    let a00 = f * kp_lo - g * jp_ln + h * jo_kn;
    let a01 = -(e * kp_lo - g * ip_lm + h * io_km);
    let a02 = e * jp_ln - f * ip_lm + h * in_jm;
    let a03 = -(e * jo_kn - f * io_km + g * in_jm);

    let det = a * a00 + b * a01 + c * a02 + d * a03;
    let inv_det = 1.0f / det;

    let gp_ho = g * p - h * o;
    let fp_hn = f * p - h * n;
    let fo_gn = f * o - g * n;
    let ep_hm = e * p - h * M;
    let eo_gm = e * o - g * M;
    let en_fm = e * n - f * M;

    let gl_hk = g * l - h * k;
    let fl_hj = f * l - h * j;
    let fk_gj = f * k - g * j;
    let el_hi = e * l - h * i;
    let ek_gi = e * k - g * i;
    let ej_fi = e * j - f * i;

    return m_M4f32_ofRows(
        m_V4f32_of(
            a00 * inv_det,
            -(b * kp_lo - c * jp_ln + d * jo_kn) * inv_det,
            (b * gp_ho - c * fp_hn + d * fo_gn) * inv_det,
            -(b * gl_hk - c * fl_hj + d * fk_gj) * inv_det
        ),
        m_V4f32_of(
            a01 * inv_det,
            (a * kp_lo - c * ip_lm + d * io_km) * inv_det,
            -(a * gp_ho - c * ep_hm + d * eo_gm) * inv_det,
            (a * gl_hk - c * el_hi + d * ek_gi) * inv_det
        ),
        m_V4f32_of(
            a02 * inv_det,
            -(a * jp_ln - b * ip_lm + d * in_jm) * inv_det,
            (a * fp_hn - b * ep_hm + d * en_fm) * inv_det,
            -(a * fl_hj - b * el_hi + d * ej_fi) * inv_det
        ),
        m_V4f32_of(
            a03 * inv_det,
            (a * jo_kn - b * io_km + c * in_jm) * inv_det,
            -(a * fo_gn - b * eo_gm + c * en_fm) * inv_det,
            (a * fk_gj - b * ek_gi + c * ej_fi) * inv_det
        )
    );
};

fn_((m_M4f32_translate(m_V3f32 t))(m_M4f32)) {
    return m_M4f32_ofRows(
        m_V4f32_of(1.0f, 0.0f, 0.0f, t.x),
        m_V4f32_of(0.0f, 1.0f, 0.0f, t.y),
        m_V4f32_of(0.0f, 0.0f, 1.0f, t.z),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateX(f32 angle))(m_M4f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M4f32_ofRows(
        m_V4f32_of(1.0f, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, sc.y, -sc.x, 0.0f),
        m_V4f32_of(0.0f, sc.x, sc.y, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateY(f32 angle))(m_M4f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M4f32_ofRows(
        m_V4f32_of(sc.y, 0.0f, sc.x, 0.0f),
        m_V4f32_of(0.0f, 1.0f, 0.0f, 0.0f),
        m_V4f32_of(-sc.x, 0.0f, sc.y, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateZ(f32 angle))(m_M4f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M4f32_ofRows(
        m_V4f32_of(sc.y, -sc.x, 0.0f, 0.0f),
        m_V4f32_of(sc.x, sc.y, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateAxis(m_V3f32 axis, f32 angle))(m_M4f32)) {
    let n = m_V3f32_norm(axis);
    let sc = m_V2f32_sincos(angle);
    let c = sc.y;
    let s = sc.x;
    let t = 1.0f - c;

    return m_M4f32_ofRows(
        m_V4f32_of(
            t * n.x * n.x + c,
            t * n.x * n.y - s * n.z,
            t * n.x * n.z + s * n.y,
            0.0f
        ),
        m_V4f32_of(
            t * n.x * n.y + s * n.z,
            t * n.y * n.y + c,
            t * n.y * n.z - s * n.x,
            0.0f
        ),
        m_V4f32_of(
            t * n.x * n.z - s * n.y,
            t * n.y * n.z + s * n.x,
            t * n.z * n.z + c,
            0.0f
        ),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_scale(m_V3f32 s))(m_M4f32)) {
    return m_M4f32_ofRows(
        m_V4f32_of(s.x, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, s.y, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, s.z, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_scaleUniform(f32 s))(m_M4f32)) {
    return m_M4f32_scale(m_V3f32_splat(s));
};

fn_((m_M4f32_lookAt(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32)) {
    let f = m_V3f32_norm(m_V3f32_sub(target, eye));
    let s = m_V3f32_norm(m_V3f32_cross(f, up));
    let u = m_V3f32_cross(s, f);

    return m_M4f32_ofRows(
        m_V4f32_of(s.x, s.y, s.z, -m_V3f32_dot(s, eye)),
        m_V4f32_of(u.x, u.y, u.z, -m_V3f32_dot(u, eye)),
        m_V4f32_of(-f.x, -f.y, -f.z, m_V3f32_dot(f, eye)),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_perspective(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;
    let nf = 1.0f / (proximal - distal);

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, (distal + proximal) * nf, 2.0f * distal * proximal * nf),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectiveInfFar(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, -1.0f, -2.0f * proximal),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn_ = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl),
        m_V4f32_of(0.0f, 2.0f * tb, 0.0f, -(top + bottom) * tb),
        m_V4f32_of(0.0f, 0.0f, -2.0f * fn_, -(distal + proximal) * fn_),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_frustum(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let nf = 1.0f / (proximal - distal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * proximal * rl, 0.0f, (right + left) * rl, 0.0f),
        m_V4f32_of(0.0f, 2.0f * proximal * tb, (top + bottom) * tb, 0.0f),
        m_V4f32_of(0.0f, 0.0f, (distal + proximal) * nf, 2.0f * distal * proximal * nf),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};

#endif /* math_mat__included */
