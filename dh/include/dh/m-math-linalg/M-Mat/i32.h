#pragma once
#ifndef m_math_linalg_M_Mat_i32_included
#define m_math_linalg_M_Mat_i32_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../V-Vec/i32.h"

/*========== Macros and Declarations ========================================*/

/*--- M2i32 Constants and Declarations ---*/

/* Constants */
#define m_M2i32_zero __comp_const__m_M2i32_zero
#define m_M2i32_identity __comp_const__m_M2i32_identity

/* Creation */
#define m_M2i32_ofCols_static(_c0, _c1...) ____m_M2i32_ofCols_static(_c0, _c1)
#define m_M2i32_ofRows_static(_r0, _r1...) ____m_M2i32_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2i32_ofCols(m_M2i32_ColV c0, m_M2i32_ColV c1))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_ofRows(m_M2i32_RowV r0, m_M2i32_RowV r1))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_splat(i32 scalar))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_diag(i32 d))(m_M2i32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2i32_neg(m_M2i32 m))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_add(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_sub(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_mul(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_div(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_rem(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2i32_scal(m_M2i32 m, i32 scalar))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_scalInv(m_M2i32 m, i32 scalar))(m_M2i32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2i32_mulM(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_mulV(m_M2i32 m, m_V2i32 v))(m_V2i32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2i32_col(m_M2i32 m, usize idx))(m_M2i32_ColV));
$attr($inline_always)
$static fn_((m_M2i32_row(m_M2i32 m, usize idx))(m_M2i32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2i32_transp(m_M2i32 m))(m_M2i32));
$attr($inline_always)
$static fn_((m_M2i32_det(m_M2i32 m))(i32));

/*--- M3i32 Constants and Declarations ---*/

/* Constants */
#define m_M3i32_zero __comp_const__m_M3i32_zero
#define m_M3i32_identity __comp_const__m_M3i32_identity

/* Creation */
#define m_M3i32_ofCols_static(_c0, _c1, _c2...) ____m_M3i32_ofCols_static(_c0, _c1, _c2)
#define m_M3i32_ofRows_static(_r0, _r1, _r2...) ____m_M3i32_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3i32_ofCols(m_M3i32_ColV c0, m_M3i32_ColV c1, m_M3i32_ColV c2))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_ofRows(m_M3i32_RowV r0, m_M3i32_RowV r1, m_M3i32_RowV r2))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_splat(i32 scalar))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_diag(i32 d))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_fromM4(m_M4i32 m))(m_M3i32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3i32_neg(m_M3i32 m))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_add(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_sub(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_mul(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_div(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_rem(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3i32_scal(m_M3i32 m, i32 scalar))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_scalInv(m_M3i32 m, i32 scalar))(m_M3i32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3i32_mulM(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_mulV(m_M3i32 m, m_V3i32 v))(m_V3i32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3i32_col(m_M3i32 m, usize idx))(m_M3i32_ColV));
$attr($inline_always)
$static fn_((m_M3i32_row(m_M3i32 m, usize idx))(m_M3i32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3i32_transp(m_M3i32 m))(m_M3i32));
$attr($inline_always)
$static fn_((m_M3i32_det(m_M3i32 m))(i32));

/*--- M4i32 Constants and Declarations ---*/

/* Constants */
#define m_M4i32_zero __comp_const__m_M4i32_zero
#define m_M4i32_identity __comp_const__m_M4i32_identity

/* Creation */
#define m_M4i32_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4i32_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4i32_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4i32_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4i32_ofCols(m_M4i32_ColV c0, m_M4i32_ColV c1, m_M4i32_ColV c2, m_M4i32_ColV c3))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_ofRows(m_M4i32_RowV r0, m_M4i32_RowV r1, m_M4i32_RowV r2, m_M4i32_RowV r3))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_splat(i32 scalar))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_diag(i32 d))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_fromM3(m_M3i32 m))(m_M4i32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4i32_neg(m_M4i32 m))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_add(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_sub(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_mul(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_div(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_rem(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4i32_scal(m_M4i32 m, i32 scalar))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_scalInv(m_M4i32 m, i32 scalar))(m_M4i32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4i32_mulM(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_mulV(m_M4i32 m, m_V4i32 v))(m_V4i32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4i32_col(m_M4i32 m, usize idx))(m_M4i32_ColV));
$attr($inline_always)
$static fn_((m_M4i32_row(m_M4i32 m, usize idx))(m_M4i32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4i32_transp(m_M4i32 m))(m_M4i32));
$attr($inline_always)
$static fn_((m_M4i32_det(m_M4i32 m))(i32));

/*========== Macros and Definitions =========================================*/

/*--- M2i32 Macros and Definitions ---*/

#define __comp_const__m_M2i32_zero m_M2i32_ofCols_static( \
    m_V2i32_zero, \
    m_V2i32_zero \
)
#define __comp_const__m_M2i32_identity m_M2i32_ofCols_static( \
    m_V2i32_unit_x, \
    m_V2i32_unit_y \
)

#define ____m_M2i32_ofCols_static(_c0, _c1...) l$((m_M2i32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2i32_ofRows_static(_r0, _r1...) l$((m_M2i32){ \
    .cols = A_init({ \
        [0] = m_V2i32_of_static((_r0).x, (_r1).x), \
        [1] = m_V2i32_of_static((_r0).y, (_r1).y), \
    }), \
})
#if on_analysis_active_only || on_comptime
fn_((m_M2i32_ofCols(m_M2i32_ColV c0, m_M2i32_ColV c1))(m_M2i32)) {
    return m_M2i32_ofCols_static(c0, c1);
};
fn_((m_M2i32_ofRows(m_M2i32_RowV r0, m_M2i32_RowV r1))(m_M2i32)) {
    return m_M2i32_ofRows_static(r0, r1);
};
fn_((m_M2i32_splat(i32 scalar))(m_M2i32)) {
    let v = m_V2i32_splat(scalar);
    return m_M2i32_ofCols(v, v);
};
fn_((m_M2i32_diag(i32 d))(m_M2i32)) {
    return m_M2i32_ofRows(
        m_V2i32_of(d, 0),
        m_V2i32_of(0, d)
    );
};

fn_((m_M2i32_neg(m_M2i32 x))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_neg(*A_at((x.cols)[0])),
        m_V2i32_neg(*A_at((x.cols)[1]))
    );
};

fn_((m_M2i32_add(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i32_sub(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i32_mul(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i32_div(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i32_rem(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i32_scal(m_M2i32 x, i32 scalar))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_scal(*A_at((x.cols)[0]), scalar),
        m_V2i32_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2i32_scalInv(m_M2i32 x, i32 scalar))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2i32_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2i32_mulM(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    m_M2i32 result = m_M2i32_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            i32 sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2i32_mulV(m_M2i32 x, m_V2i32 y))(m_V2i32)) {
    return m_V2i32_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2i32_col(m_M2i32 x, usize idx))(m_M2i32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2i32_row(m_M2i32 x, usize idx))(m_M2i32_RowV)) {
    return m_V2i32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2i32_transp(m_M2i32 x))(m_M2i32)) {
    return m_M2i32_ofRows(
        m_V2i32_of(x.s0_0, x.s1_0),
        m_V2i32_of(x.s0_1, x.s1_1)
    );
};

fn_((m_M2i32_det(m_M2i32 x))(i32)) {
    return x.s0_0 * x.s1_1 - x.s0_1 * x.s1_0;
};
#endif /* on_analysis_active_only || on_comptime */

/*--- M3i32 Macros and Definitions ---*/

#define __comp_const__m_M3i32_zero m_M3i32_ofCols_static( \
    m_V3i32_zero, \
    m_V3i32_zero, \
    m_V3i32_zero \
)
#define __comp_const__m_M3i32_identity m_M3i32_ofCols_static( \
    m_V3i32_unit_x, \
    m_V3i32_unit_y, \
    m_V3i32_unit_z \
)

#define ____m_M3i32_ofCols_static(_c0, _c1, _c2...) l$((m_M3i32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3i32_ofRows_static(_r0, _r1, _r2...) l$((m_M3i32){ \
    .cols = A_init({ \
        [0] = m_V3i32_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3i32_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3i32_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
#if on_analysis_active_only || on_comptime
fn_((m_M3i32_ofCols(m_M3i32_ColV c0, m_M3i32_ColV c1, m_M3i32_ColV c2))(m_M3i32)) {
    return m_M3i32_ofCols_static(c0, c1, c2);
};
fn_((m_M3i32_ofRows(m_M3i32_RowV r0, m_M3i32_RowV r1, m_M3i32_RowV r2))(m_M3i32)) {
    return m_M3i32_ofRows_static(r0, r1, r2);
};
fn_((m_M3i32_splat(i32 scalar))(m_M3i32)) {
    let v = m_V3i32_splat(scalar);
    return m_M3i32_ofCols(v, v, v);
};
fn_((m_M3i32_diag(i32 d))(m_M3i32)) {
    return m_M3i32_ofRows(
        m_V3i32_of(d, 0, 0),
        m_V3i32_of(0, d, 0),
        m_V3i32_of(0, 0, d)
    );
};

fn_((m_M3i32_fromM4(m_M4i32 x))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_from4(*A_at((x.cols)[0])),
        m_V3i32_from4(*A_at((x.cols)[1])),
        m_V3i32_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3i32_neg(m_M3i32 x))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_neg(*A_at((x.cols)[0])),
        m_V3i32_neg(*A_at((x.cols)[1])),
        m_V3i32_neg(*A_at((x.cols)[2]))
    );
};

fn_((m_M3i32_add(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i32_sub(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i32_mul(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i32_div(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i32_rem(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_rem(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i32_scal(m_M3i32 x, i32 scalar))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_scal(*A_at((x.cols)[0]), scalar),
        m_V3i32_scal(*A_at((x.cols)[1]), scalar),
        m_V3i32_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3i32_scalInv(m_M3i32 x, i32 scalar))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3i32_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3i32_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3i32_mulM(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    m_M3i32 result = m_M3i32_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            i32 sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3i32_mulV(m_M3i32 x, m_V3i32 y))(m_V3i32)) {
    return m_V3i32_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3i32_col(m_M3i32 x, usize idx))(m_M3i32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3i32_row(m_M3i32 x, usize idx))(m_M3i32_RowV)) {
    return m_V3i32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3i32_transp(m_M3i32 x))(m_M3i32)) {
    return m_M3i32_ofRows(
        m_V3i32_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3i32_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3i32_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

fn_((m_M3i32_det(m_M3i32 x))(i32)) {
    return x.s0_0 * (x.s1_1 * x.s2_2 - x.s1_2 * x.s2_1)
         - x.s0_1 * (x.s1_0 * x.s2_2 - x.s1_2 * x.s2_0)
         + x.s0_2 * (x.s1_0 * x.s2_1 - x.s1_1 * x.s2_0);
};
#endif /* on_analysis_active_only || on_comptime */

/*--- M4i32 Macros and Definitions ---*/

#define __comp_const__m_M4i32_zero m_M4i32_ofCols_static( \
    m_V4i32_zero, \
    m_V4i32_zero, \
    m_V4i32_zero, \
    m_V4i32_zero \
)
#define __comp_const__m_M4i32_identity m_M4i32_ofCols_static( \
    m_V4i32_unit_x, \
    m_V4i32_unit_y, \
    m_V4i32_unit_z, \
    m_V4i32_unit_w \
)

#define ____m_M4i32_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4i32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4i32_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4i32){ \
    .cols = A_init({ \
        [0] = m_V4i32_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4i32_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4i32_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4i32_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
#if on_analysis_active_only || on_comptime
fn_((m_M4i32_ofCols(m_M4i32_ColV c0, m_M4i32_ColV c1, m_M4i32_ColV c2, m_M4i32_ColV c3))(m_M4i32)) {
    return m_M4i32_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4i32_ofRows(m_M4i32_RowV r0, m_M4i32_RowV r1, m_M4i32_RowV r2, m_M4i32_RowV r3))(m_M4i32)) {
    return m_M4i32_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4i32_splat(i32 scalar))(m_M4i32)) {
    let v = m_V4i32_splat(scalar);
    return m_M4i32_ofCols(v, v, v, v);
};
fn_((m_M4i32_diag(i32 d))(m_M4i32)) {
    return m_M4i32_ofRows(
        m_V4i32_of(d, 0, 0, 0),
        m_V4i32_of(0, d, 0, 0),
        m_V4i32_of(0, 0, d, 0),
        m_V4i32_of(0, 0, 0, d)
    );
};

fn_((m_M4i32_fromM3(m_M3i32 x))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4i32_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4i32_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4i32_of(0, 0, 0, 1)
    );
};

fn_((m_M4i32_neg(m_M4i32 x))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_neg(*A_at((x.cols)[0])),
        m_V4i32_neg(*A_at((x.cols)[1])),
        m_V4i32_neg(*A_at((x.cols)[2])),
        m_V4i32_neg(*A_at((x.cols)[3]))
    );
};

fn_((m_M4i32_add(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i32_sub(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i32_mul(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i32_div(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i32_rem(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_rem(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_rem(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i32_scal(m_M4i32 x, i32 scalar))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_scal(*A_at((x.cols)[0]), scalar),
        m_V4i32_scal(*A_at((x.cols)[1]), scalar),
        m_V4i32_scal(*A_at((x.cols)[2]), scalar),
        m_V4i32_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4i32_scalInv(m_M4i32 x, i32 scalar))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4i32_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4i32_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4i32_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4i32_mulM(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    m_M4i32 result = m_M4i32_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            i32 sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4i32_mulV(m_M4i32 x, m_V4i32 y))(m_V4i32)) {
    return m_V4i32_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4i32_col(m_M4i32 x, usize idx))(m_M4i32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4i32_row(m_M4i32 x, usize idx))(m_M4i32_RowV)) {
    return m_V4i32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4i32_transp(m_M4i32 x))(m_M4i32)) {
    return m_M4i32_ofRows(
        m_V4i32_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4i32_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4i32_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4i32_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

fn_((m_M4i32_det(m_M4i32 x))(i32)) {
    let a = x.s0_0, b = x.s0_1, c = x.s0_2, d = x.s0_3;
    let e = x.s1_0, f = x.s1_1, g = x.s1_2, h = x.s1_3;
    let i = x.s2_0, j = x.s2_1, k = x.s2_2, l = x.s2_3;
    let Mv = x.s3_0, n = x.s3_1, o = x.s3_2, p = x.s3_3;

    let kp_lo = k * p - l * o;
    let jp_ln = j * p - l * n;
    let jo_kn = j * o - k * n;
    let ip_lm = i * p - l * Mv;
    let io_km = i * o - k * Mv;
    let in_jm = i * n - j * Mv;

    return a * (f * kp_lo - g * jp_ln + h * jo_kn)
         - b * (e * kp_lo - g * ip_lm + h * io_km)
         + c * (e * jp_ln - f * ip_lm + h * in_jm)
         - d * (e * jo_kn - f * io_km + g * in_jm);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_M_Mat_i32_included */
