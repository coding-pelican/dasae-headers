#pragma once
#ifndef m_math_linalg_M_Mat_isize__included
#define m_math_linalg_M_Mat_isize__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../V-Vec/isize.h"

/*========== Macros and Declarations ========================================*/

/*--- M2isize Constants and Declarations ---*/

/* Constants */
#define m_M2isize_zero __comp_const__m_M2isize_zero
#define m_M2isize_identity __comp_const__m_M2isize_identity

/* Creation */
#define m_M2isize_ofCols_static(_c0, _c1...) ____m_M2isize_ofCols_static(_c0, _c1)
#define m_M2isize_ofRows_static(_r0, _r1...) ____m_M2isize_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2isize_ofCols(m_M2isize_ColV c0, m_M2isize_ColV c1))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_ofRows(m_M2isize_RowV r0, m_M2isize_RowV r1))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_splat(isize scalar))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_diag(isize d))(m_M2isize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2isize_neg(m_M2isize m))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_add(m_M2isize lhs, m_M2isize rhs))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_sub(m_M2isize lhs, m_M2isize rhs))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_mul(m_M2isize lhs, m_M2isize rhs))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_div(m_M2isize lhs, m_M2isize rhs))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_rem(m_M2isize lhs, m_M2isize rhs))(m_M2isize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2isize_scal(m_M2isize m, isize scalar))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_scalInv(m_M2isize m, isize scalar))(m_M2isize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2isize_mulM(m_M2isize lhs, m_M2isize rhs))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_mulV(m_M2isize m, m_V2isize v))(m_V2isize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2isize_col(m_M2isize m, usize idx))(m_M2isize_ColV));
$attr($inline_always)
$static fn_((m_M2isize_row(m_M2isize m, usize idx))(m_M2isize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2isize_transp(m_M2isize m))(m_M2isize));
$attr($inline_always)
$static fn_((m_M2isize_det(m_M2isize m))(isize));

/*--- M3isize Constants and Declarations ---*/

/* Constants */
#define m_M3isize_zero __comp_const__m_M3isize_zero
#define m_M3isize_identity __comp_const__m_M3isize_identity

/* Creation */
#define m_M3isize_ofCols_static(_c0, _c1, _c2...) ____m_M3isize_ofCols_static(_c0, _c1, _c2)
#define m_M3isize_ofRows_static(_r0, _r1, _r2...) ____m_M3isize_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3isize_ofCols(m_M3isize_ColV c0, m_M3isize_ColV c1, m_M3isize_ColV c2))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_ofRows(m_M3isize_RowV r0, m_M3isize_RowV r1, m_M3isize_RowV r2))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_splat(isize scalar))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_diag(isize d))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_fromM4(m_M4isize m))(m_M3isize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3isize_neg(m_M3isize m))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_add(m_M3isize lhs, m_M3isize rhs))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_sub(m_M3isize lhs, m_M3isize rhs))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_mul(m_M3isize lhs, m_M3isize rhs))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_div(m_M3isize lhs, m_M3isize rhs))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_rem(m_M3isize lhs, m_M3isize rhs))(m_M3isize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3isize_scal(m_M3isize m, isize scalar))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_scalInv(m_M3isize m, isize scalar))(m_M3isize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3isize_mulM(m_M3isize lhs, m_M3isize rhs))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_mulV(m_M3isize m, m_V3isize v))(m_V3isize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3isize_col(m_M3isize m, usize idx))(m_M3isize_ColV));
$attr($inline_always)
$static fn_((m_M3isize_row(m_M3isize m, usize idx))(m_M3isize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3isize_transp(m_M3isize m))(m_M3isize));
$attr($inline_always)
$static fn_((m_M3isize_det(m_M3isize m))(isize));

/*--- M4isize Constants and Declarations ---*/

/* Constants */
#define m_M4isize_zero __comp_const__m_M4isize_zero
#define m_M4isize_identity __comp_const__m_M4isize_identity

/* Creation */
#define m_M4isize_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4isize_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4isize_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4isize_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4isize_ofCols(m_M4isize_ColV c0, m_M4isize_ColV c1, m_M4isize_ColV c2, m_M4isize_ColV c3))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_ofRows(m_M4isize_RowV r0, m_M4isize_RowV r1, m_M4isize_RowV r2, m_M4isize_RowV r3))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_splat(isize scalar))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_diag(isize d))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_fromM3(m_M3isize m))(m_M4isize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4isize_neg(m_M4isize m))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_add(m_M4isize lhs, m_M4isize rhs))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_sub(m_M4isize lhs, m_M4isize rhs))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_mul(m_M4isize lhs, m_M4isize rhs))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_div(m_M4isize lhs, m_M4isize rhs))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_rem(m_M4isize lhs, m_M4isize rhs))(m_M4isize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4isize_scal(m_M4isize m, isize scalar))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_scalInv(m_M4isize m, isize scalar))(m_M4isize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4isize_mulM(m_M4isize lhs, m_M4isize rhs))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_mulV(m_M4isize m, m_V4isize v))(m_V4isize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4isize_col(m_M4isize m, usize idx))(m_M4isize_ColV));
$attr($inline_always)
$static fn_((m_M4isize_row(m_M4isize m, usize idx))(m_M4isize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4isize_transp(m_M4isize m))(m_M4isize));
$attr($inline_always)
$static fn_((m_M4isize_det(m_M4isize m))(isize));

/*========== Macros and Definitions =========================================*/

/*--- M2isize Macros and Definitions ---*/

#define __comp_const__m_M2isize_zero m_M2isize_ofCols_static( \
    m_V2isize_zero, \
    m_V2isize_zero \
)
#define __comp_const__m_M2isize_identity m_M2isize_ofCols_static( \
    m_V2isize_unit_x, \
    m_V2isize_unit_y \
)

#define ____m_M2isize_ofCols_static(_c0, _c1...) l$((m_M2isize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2isize_ofRows_static(_r0, _r1...) l$((m_M2isize){ \
    .cols = A_init({ \
        [0] = m_V2isize_of_static((_r0).x, (_r1).x), \
        [1] = m_V2isize_of_static((_r0).y, (_r1).y), \
    }), \
})
#if in_analysis_active_only || in_comptime
fn_((m_M2isize_ofCols(m_M2isize_ColV c0, m_M2isize_ColV c1))(m_M2isize)) {
    return m_M2isize_ofCols_static(c0, c1);
};
fn_((m_M2isize_ofRows(m_M2isize_RowV r0, m_M2isize_RowV r1))(m_M2isize)) {
    return m_M2isize_ofRows_static(r0, r1);
};
fn_((m_M2isize_splat(isize scalar))(m_M2isize)) {
    let v = m_V2isize_splat(scalar);
    return m_M2isize_ofCols(v, v);
};
fn_((m_M2isize_diag(isize d))(m_M2isize)) {
    return m_M2isize_ofRows(
        m_V2isize_of(d, 0),
        m_V2isize_of(0, d)
    );
};

fn_((m_M2isize_neg(m_M2isize x))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_neg(*A_at((x.cols)[0])),
        m_V2isize_neg(*A_at((x.cols)[1]))
    );
};

fn_((m_M2isize_add(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2isize_sub(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2isize_mul(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2isize_div(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2isize_rem(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2isize_scal(m_M2isize x, isize scalar))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_scal(*A_at((x.cols)[0]), scalar),
        m_V2isize_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2isize_scalInv(m_M2isize x, isize scalar))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2isize_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2isize_mulM(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    m_M2isize result = m_M2isize_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            isize sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2isize_mulV(m_M2isize x, m_V2isize y))(m_V2isize)) {
    return m_V2isize_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2isize_col(m_M2isize x, usize idx))(m_M2isize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2isize_row(m_M2isize x, usize idx))(m_M2isize_RowV)) {
    return m_V2isize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2isize_transp(m_M2isize x))(m_M2isize)) {
    return m_M2isize_ofRows(
        m_V2isize_of(x.s0_0, x.s1_0),
        m_V2isize_of(x.s0_1, x.s1_1)
    );
};

fn_((m_M2isize_det(m_M2isize x))(isize)) {
    return x.s0_0 * x.s1_1 - x.s0_1 * x.s1_0;
};
#endif /* in_analysis_active_only || in_comptime */

/*--- M3isize Macros and Definitions ---*/

#define __comp_const__m_M3isize_zero m_M3isize_ofCols_static( \
    m_V3isize_zero, \
    m_V3isize_zero, \
    m_V3isize_zero \
)
#define __comp_const__m_M3isize_identity m_M3isize_ofCols_static( \
    m_V3isize_unit_x, \
    m_V3isize_unit_y, \
    m_V3isize_unit_z \
)

#define ____m_M3isize_ofCols_static(_c0, _c1, _c2...) l$((m_M3isize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3isize_ofRows_static(_r0, _r1, _r2...) l$((m_M3isize){ \
    .cols = A_init({ \
        [0] = m_V3isize_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3isize_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3isize_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
#if in_analysis_active_only || in_comptime
fn_((m_M3isize_ofCols(m_M3isize_ColV c0, m_M3isize_ColV c1, m_M3isize_ColV c2))(m_M3isize)) {
    return m_M3isize_ofCols_static(c0, c1, c2);
};
fn_((m_M3isize_ofRows(m_M3isize_RowV r0, m_M3isize_RowV r1, m_M3isize_RowV r2))(m_M3isize)) {
    return m_M3isize_ofRows_static(r0, r1, r2);
};
fn_((m_M3isize_splat(isize scalar))(m_M3isize)) {
    let v = m_V3isize_splat(scalar);
    return m_M3isize_ofCols(v, v, v);
};
fn_((m_M3isize_diag(isize d))(m_M3isize)) {
    return m_M3isize_ofRows(
        m_V3isize_of(d, 0, 0),
        m_V3isize_of(0, d, 0),
        m_V3isize_of(0, 0, d)
    );
};

fn_((m_M3isize_fromM4(m_M4isize x))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_from4(*A_at((x.cols)[0])),
        m_V3isize_from4(*A_at((x.cols)[1])),
        m_V3isize_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3isize_neg(m_M3isize x))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_neg(*A_at((x.cols)[0])),
        m_V3isize_neg(*A_at((x.cols)[1])),
        m_V3isize_neg(*A_at((x.cols)[2]))
    );
};

fn_((m_M3isize_add(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3isize_sub(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3isize_mul(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3isize_div(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3isize_rem(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_rem(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3isize_scal(m_M3isize x, isize scalar))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_scal(*A_at((x.cols)[0]), scalar),
        m_V3isize_scal(*A_at((x.cols)[1]), scalar),
        m_V3isize_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3isize_scalInv(m_M3isize x, isize scalar))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3isize_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3isize_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3isize_mulM(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    m_M3isize result = m_M3isize_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            isize sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3isize_mulV(m_M3isize x, m_V3isize y))(m_V3isize)) {
    return m_V3isize_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3isize_col(m_M3isize x, usize idx))(m_M3isize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3isize_row(m_M3isize x, usize idx))(m_M3isize_RowV)) {
    return m_V3isize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3isize_transp(m_M3isize x))(m_M3isize)) {
    return m_M3isize_ofRows(
        m_V3isize_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3isize_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3isize_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

fn_((m_M3isize_det(m_M3isize x))(isize)) {
    return x.s0_0 * (x.s1_1 * x.s2_2 - x.s1_2 * x.s2_1)
         - x.s0_1 * (x.s1_0 * x.s2_2 - x.s1_2 * x.s2_0)
         + x.s0_2 * (x.s1_0 * x.s2_1 - x.s1_1 * x.s2_0);
};
#endif /* in_analysis_active_only || in_comptime */

/*--- M4isize Macros and Definitions ---*/

#define __comp_const__m_M4isize_zero m_M4isize_ofCols_static( \
    m_V4isize_zero, \
    m_V4isize_zero, \
    m_V4isize_zero, \
    m_V4isize_zero \
)
#define __comp_const__m_M4isize_identity m_M4isize_ofCols_static( \
    m_V4isize_unit_x, \
    m_V4isize_unit_y, \
    m_V4isize_unit_z, \
    m_V4isize_unit_w \
)

#define ____m_M4isize_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4isize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4isize_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4isize){ \
    .cols = A_init({ \
        [0] = m_V4isize_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4isize_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4isize_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4isize_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
#if in_analysis_active_only || in_comptime
fn_((m_M4isize_ofCols(m_M4isize_ColV c0, m_M4isize_ColV c1, m_M4isize_ColV c2, m_M4isize_ColV c3))(m_M4isize)) {
    return m_M4isize_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4isize_ofRows(m_M4isize_RowV r0, m_M4isize_RowV r1, m_M4isize_RowV r2, m_M4isize_RowV r3))(m_M4isize)) {
    return m_M4isize_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4isize_splat(isize scalar))(m_M4isize)) {
    let v = m_V4isize_splat(scalar);
    return m_M4isize_ofCols(v, v, v, v);
};
fn_((m_M4isize_diag(isize d))(m_M4isize)) {
    return m_M4isize_ofRows(
        m_V4isize_of(d, 0, 0, 0),
        m_V4isize_of(0, d, 0, 0),
        m_V4isize_of(0, 0, d, 0),
        m_V4isize_of(0, 0, 0, d)
    );
};

fn_((m_M4isize_fromM3(m_M3isize x))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4isize_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4isize_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4isize_of(0, 0, 0, 1)
    );
};

fn_((m_M4isize_neg(m_M4isize x))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_neg(*A_at((x.cols)[0])),
        m_V4isize_neg(*A_at((x.cols)[1])),
        m_V4isize_neg(*A_at((x.cols)[2])),
        m_V4isize_neg(*A_at((x.cols)[3]))
    );
};

fn_((m_M4isize_add(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4isize_sub(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4isize_mul(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4isize_div(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4isize_rem(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_rem(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_rem(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_rem(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_rem(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4isize_scal(m_M4isize x, isize scalar))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_scal(*A_at((x.cols)[0]), scalar),
        m_V4isize_scal(*A_at((x.cols)[1]), scalar),
        m_V4isize_scal(*A_at((x.cols)[2]), scalar),
        m_V4isize_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4isize_scalInv(m_M4isize x, isize scalar))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4isize_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4isize_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4isize_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4isize_mulM(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    m_M4isize result = m_M4isize_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            isize sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4isize_mulV(m_M4isize x, m_V4isize y))(m_V4isize)) {
    return m_V4isize_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4isize_col(m_M4isize x, usize idx))(m_M4isize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4isize_row(m_M4isize x, usize idx))(m_M4isize_RowV)) {
    return m_V4isize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4isize_transp(m_M4isize x))(m_M4isize)) {
    return m_M4isize_ofRows(
        m_V4isize_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4isize_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4isize_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4isize_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

fn_((m_M4isize_det(m_M4isize x))(isize)) {
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
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_M_Mat_isize__included */
