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
#pragma once
#ifndef math_mat__included
#define math_mat__included 1

/*========== Includes =======================================================*/

#include "mat_types.h"
#include "vec.h"

/*========== Macros and Declarations ========================================*/

#define m_M_as$(/*(_T: m_M_Type)(_src)*/... /*(m_M_Type)*/) __step__m_M_as$(__VA_ARGS__)

/*========== m_M2u32 Constants and Declarations ===============================*/

/* Constants */
#define m_M2u32_zero __comp_const__m_M2u32_zero
#define m_M2u32_identity __comp_const__m_M2u32_identity

/* Creation */
#define m_M2u32_ofCols_static(_c0, _c1...) ____m_M2u32_ofCols_static(_c0, _c1)
#define m_M2u32_ofRows_static(_r0, _r1...) ____m_M2u32_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2u32_ofCols(m_M2u32_ColV c0, m_M2u32_ColV c1))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_ofRows(m_M2u32_RowV r0, m_M2u32_RowV r1))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_splat(u32 scalar))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_diag(u32 d))(m_M2u32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2u32_add(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_sub(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_mul(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_div(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_mod(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2u32_scal(m_M2u32 m, u32 scalar))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_scalInv(m_M2u32 m, u32 scalar))(m_M2u32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2u32_mulM(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32));
$attr($inline_always)
$static fn_((m_M2u32_mulV(m_M2u32 m, m_V2u32 v))(m_V2u32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2u32_col(m_M2u32 m, usize idx))(m_M2u32_ColV));
$attr($inline_always)
$static fn_((m_M2u32_row(m_M2u32 m, usize idx))(m_M2u32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2u32_transp(m_M2u32 m))(m_M2u32));

/*========== m_M3u32 Constants and Declarations ===============================*/

/* Constants */
#define m_M3u32_zero __comp_const__m_M3u32_zero
#define m_M3u32_identity __comp_const__m_M3u32_identity

/* Creation */
#define m_M3u32_ofCols_static(_c0, _c1, _c2...) ____m_M3u32_ofCols_static(_c0, _c1, _c2)
#define m_M3u32_ofRows_static(_r0, _r1, _r2...) ____m_M3u32_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3u32_ofCols(m_M3u32_ColV c0, m_M3u32_ColV c1, m_M3u32_ColV c2))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_ofRows(m_M3u32_RowV r0, m_M3u32_RowV r1, m_M3u32_RowV r2))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_splat(u32 scalar))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_diag(u32 d))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_fromM4(m_M4u32 m))(m_M3u32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3u32_add(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_sub(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_mul(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_div(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_mod(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3u32_scal(m_M3u32 m, u32 scalar))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_scalInv(m_M3u32 m, u32 scalar))(m_M3u32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3u32_mulM(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32));
$attr($inline_always)
$static fn_((m_M3u32_mulV(m_M3u32 m, m_V3u32 v))(m_V3u32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3u32_col(m_M3u32 m, usize idx))(m_M3u32_ColV));
$attr($inline_always)
$static fn_((m_M3u32_row(m_M3u32 m, usize idx))(m_M3u32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3u32_transp(m_M3u32 m))(m_M3u32));

/*========== m_M4u32 Constants and Declarations ===============================*/

/* Constants */
#define m_M4u32_zero __comp_const__m_M4u32_zero
#define m_M4u32_identity __comp_const__m_M4u32_identity

/* Creation */
#define m_M4u32_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4u32_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4u32_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4u32_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4u32_ofCols(m_M4u32_ColV c0, m_M4u32_ColV c1, m_M4u32_ColV c2, m_M4u32_ColV c3))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_ofRows(m_M4u32_RowV r0, m_M4u32_RowV r1, m_M4u32_RowV r2, m_M4u32_RowV r3))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_splat(u32 scalar))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_diag(u32 d))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_fromM3(m_M3u32 m))(m_M4u32));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4u32_add(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_sub(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_mul(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_div(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_mod(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4u32_scal(m_M4u32 m, u32 scalar))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_scalInv(m_M4u32 m, u32 scalar))(m_M4u32));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4u32_mulM(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32));
$attr($inline_always)
$static fn_((m_M4u32_mulV(m_M4u32 m, m_V4u32 v))(m_V4u32));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4u32_col(m_M4u32 m, usize idx))(m_M4u32_ColV));
$attr($inline_always)
$static fn_((m_M4u32_row(m_M4u32 m, usize idx))(m_M4u32_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4u32_transp(m_M4u32 m))(m_M4u32));

/*========== m_M2u64 Constants and Declarations ===============================*/

/* Constants */
#define m_M2u64_zero __comp_const__m_M2u64_zero
#define m_M2u64_identity __comp_const__m_M2u64_identity

/* Creation */
#define m_M2u64_ofCols_static(_c0, _c1...) ____m_M2u64_ofCols_static(_c0, _c1)
#define m_M2u64_ofRows_static(_r0, _r1...) ____m_M2u64_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2u64_ofCols(m_M2u64_ColV c0, m_M2u64_ColV c1))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_ofRows(m_M2u64_RowV r0, m_M2u64_RowV r1))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_splat(u64 scalar))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_diag(u64 d))(m_M2u64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2u64_add(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_sub(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_mul(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_div(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_mod(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2u64_scal(m_M2u64 m, u64 scalar))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_scalInv(m_M2u64 m, u64 scalar))(m_M2u64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2u64_mulM(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64));
$attr($inline_always)
$static fn_((m_M2u64_mulV(m_M2u64 m, m_V2u64 v))(m_V2u64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2u64_col(m_M2u64 m, usize idx))(m_M2u64_ColV));
$attr($inline_always)
$static fn_((m_M2u64_row(m_M2u64 m, usize idx))(m_M2u64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2u64_transp(m_M2u64 m))(m_M2u64));

/*========== m_M3u64 Constants and Declarations ===============================*/

/* Constants */
#define m_M3u64_zero __comp_const__m_M3u64_zero
#define m_M3u64_identity __comp_const__m_M3u64_identity

/* Creation */
#define m_M3u64_ofCols_static(_c0, _c1, _c2...) ____m_M3u64_ofCols_static(_c0, _c1, _c2)
#define m_M3u64_ofRows_static(_r0, _r1, _r2...) ____m_M3u64_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3u64_ofCols(m_M3u64_ColV c0, m_M3u64_ColV c1, m_M3u64_ColV c2))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_ofRows(m_M3u64_RowV r0, m_M3u64_RowV r1, m_M3u64_RowV r2))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_splat(u64 scalar))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_diag(u64 d))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_fromM4(m_M4u64 m))(m_M3u64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3u64_add(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_sub(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_mul(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_div(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_mod(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3u64_scal(m_M3u64 m, u64 scalar))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_scalInv(m_M3u64 m, u64 scalar))(m_M3u64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3u64_mulM(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64));
$attr($inline_always)
$static fn_((m_M3u64_mulV(m_M3u64 m, m_V3u64 v))(m_V3u64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3u64_col(m_M3u64 m, usize idx))(m_M3u64_ColV));
$attr($inline_always)
$static fn_((m_M3u64_row(m_M3u64 m, usize idx))(m_M3u64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3u64_transp(m_M3u64 m))(m_M3u64));

/*========== m_M4u64 Constants and Declarations ===============================*/

/* Constants */
#define m_M4u64_zero __comp_const__m_M4u64_zero
#define m_M4u64_identity __comp_const__m_M4u64_identity

/* Creation */
#define m_M4u64_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4u64_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4u64_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4u64_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4u64_ofCols(m_M4u64_ColV c0, m_M4u64_ColV c1, m_M4u64_ColV c2, m_M4u64_ColV c3))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_ofRows(m_M4u64_RowV r0, m_M4u64_RowV r1, m_M4u64_RowV r2, m_M4u64_RowV r3))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_splat(u64 scalar))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_diag(u64 d))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_fromM3(m_M3u64 m))(m_M4u64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4u64_add(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_sub(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_mul(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_div(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_mod(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4u64_scal(m_M4u64 m, u64 scalar))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_scalInv(m_M4u64 m, u64 scalar))(m_M4u64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4u64_mulM(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64));
$attr($inline_always)
$static fn_((m_M4u64_mulV(m_M4u64 m, m_V4u64 v))(m_V4u64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4u64_col(m_M4u64 m, usize idx))(m_M4u64_ColV));
$attr($inline_always)
$static fn_((m_M4u64_row(m_M4u64 m, usize idx))(m_M4u64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4u64_transp(m_M4u64 m))(m_M4u64));

/*========== m_M2usize Constants and Declarations ===============================*/

/* Constants */
#define m_M2usize_zero __comp_const__m_M2usize_zero
#define m_M2usize_identity __comp_const__m_M2usize_identity

/* Creation */
#define m_M2usize_ofCols_static(_c0, _c1...) ____m_M2usize_ofCols_static(_c0, _c1)
#define m_M2usize_ofRows_static(_r0, _r1...) ____m_M2usize_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2usize_ofCols(m_M2usize_ColV c0, m_M2usize_ColV c1))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_ofRows(m_M2usize_RowV r0, m_M2usize_RowV r1))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_splat(usize scalar))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_diag(usize d))(m_M2usize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2usize_add(m_M2usize lhs, m_M2usize rhs))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_sub(m_M2usize lhs, m_M2usize rhs))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_mul(m_M2usize lhs, m_M2usize rhs))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_div(m_M2usize lhs, m_M2usize rhs))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_mod(m_M2usize lhs, m_M2usize rhs))(m_M2usize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2usize_scal(m_M2usize m, usize scalar))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_scalInv(m_M2usize m, usize scalar))(m_M2usize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2usize_mulM(m_M2usize lhs, m_M2usize rhs))(m_M2usize));
$attr($inline_always)
$static fn_((m_M2usize_mulV(m_M2usize m, m_V2usize v))(m_V2usize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2usize_col(m_M2usize m, usize idx))(m_M2usize_ColV));
$attr($inline_always)
$static fn_((m_M2usize_row(m_M2usize m, usize idx))(m_M2usize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2usize_transp(m_M2usize m))(m_M2usize));

/*========== m_M3usize Constants and Declarations ===============================*/

/* Constants */
#define m_M3usize_zero __comp_const__m_M3usize_zero
#define m_M3usize_identity __comp_const__m_M3usize_identity

/* Creation */
#define m_M3usize_ofCols_static(_c0, _c1, _c2...) ____m_M3usize_ofCols_static(_c0, _c1, _c2)
#define m_M3usize_ofRows_static(_r0, _r1, _r2...) ____m_M3usize_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3usize_ofCols(m_M3usize_ColV c0, m_M3usize_ColV c1, m_M3usize_ColV c2))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_ofRows(m_M3usize_RowV r0, m_M3usize_RowV r1, m_M3usize_RowV r2))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_splat(usize scalar))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_diag(usize d))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_fromM4(m_M4usize m))(m_M3usize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3usize_add(m_M3usize lhs, m_M3usize rhs))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_sub(m_M3usize lhs, m_M3usize rhs))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_mul(m_M3usize lhs, m_M3usize rhs))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_div(m_M3usize lhs, m_M3usize rhs))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_mod(m_M3usize lhs, m_M3usize rhs))(m_M3usize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3usize_scal(m_M3usize m, usize scalar))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_scalInv(m_M3usize m, usize scalar))(m_M3usize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3usize_mulM(m_M3usize lhs, m_M3usize rhs))(m_M3usize));
$attr($inline_always)
$static fn_((m_M3usize_mulV(m_M3usize m, m_V3usize v))(m_V3usize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3usize_col(m_M3usize m, usize idx))(m_M3usize_ColV));
$attr($inline_always)
$static fn_((m_M3usize_row(m_M3usize m, usize idx))(m_M3usize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3usize_transp(m_M3usize m))(m_M3usize));

/*========== m_M4usize Constants and Declarations ===============================*/

/* Constants */
#define m_M4usize_zero __comp_const__m_M4usize_zero
#define m_M4usize_identity __comp_const__m_M4usize_identity

/* Creation */
#define m_M4usize_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4usize_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4usize_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4usize_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4usize_ofCols(m_M4usize_ColV c0, m_M4usize_ColV c1, m_M4usize_ColV c2, m_M4usize_ColV c3))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_ofRows(m_M4usize_RowV r0, m_M4usize_RowV r1, m_M4usize_RowV r2, m_M4usize_RowV r3))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_splat(usize scalar))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_diag(usize d))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_fromM3(m_M3usize m))(m_M4usize));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4usize_add(m_M4usize lhs, m_M4usize rhs))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_sub(m_M4usize lhs, m_M4usize rhs))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_mul(m_M4usize lhs, m_M4usize rhs))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_div(m_M4usize lhs, m_M4usize rhs))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_mod(m_M4usize lhs, m_M4usize rhs))(m_M4usize));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4usize_scal(m_M4usize m, usize scalar))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_scalInv(m_M4usize m, usize scalar))(m_M4usize));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4usize_mulM(m_M4usize lhs, m_M4usize rhs))(m_M4usize));
$attr($inline_always)
$static fn_((m_M4usize_mulV(m_M4usize m, m_V4usize v))(m_V4usize));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4usize_col(m_M4usize m, usize idx))(m_M4usize_ColV));
$attr($inline_always)
$static fn_((m_M4usize_row(m_M4usize m, usize idx))(m_M4usize_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4usize_transp(m_M4usize m))(m_M4usize));

/*========== m_M2i32 Constants and Declarations ===============================*/

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
$static fn_((m_M2i32_mod(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32));

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

/*========== m_M3i32 Constants and Declarations ===============================*/

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
$static fn_((m_M3i32_mod(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32));

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

/*========== m_M4i32 Constants and Declarations ===============================*/

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
$static fn_((m_M4i32_mod(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32));

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

/*========== m_M2i64 Constants and Declarations ===============================*/

/* Constants */
#define m_M2i64_zero __comp_const__m_M2i64_zero
#define m_M2i64_identity __comp_const__m_M2i64_identity

/* Creation */
#define m_M2i64_ofCols_static(_c0, _c1...) ____m_M2i64_ofCols_static(_c0, _c1)
#define m_M2i64_ofRows_static(_r0, _r1...) ____m_M2i64_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2i64_ofCols(m_M2i64_ColV c0, m_M2i64_ColV c1))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_ofRows(m_M2i64_RowV r0, m_M2i64_RowV r1))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_splat(i64 scalar))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_diag(i64 d))(m_M2i64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2i64_neg(m_M2i64 m))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_add(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_sub(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_mul(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_div(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_mod(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2i64_scal(m_M2i64 m, i64 scalar))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_scalInv(m_M2i64 m, i64 scalar))(m_M2i64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2i64_mulM(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_mulV(m_M2i64 m, m_V2i64 v))(m_V2i64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2i64_col(m_M2i64 m, usize idx))(m_M2i64_ColV));
$attr($inline_always)
$static fn_((m_M2i64_row(m_M2i64 m, usize idx))(m_M2i64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2i64_transp(m_M2i64 m))(m_M2i64));
$attr($inline_always)
$static fn_((m_M2i64_det(m_M2i64 m))(i64));

/*========== m_M3i64 Constants and Declarations ===============================*/

/* Constants */
#define m_M3i64_zero __comp_const__m_M3i64_zero
#define m_M3i64_identity __comp_const__m_M3i64_identity

/* Creation */
#define m_M3i64_ofCols_static(_c0, _c1, _c2...) ____m_M3i64_ofCols_static(_c0, _c1, _c2)
#define m_M3i64_ofRows_static(_r0, _r1, _r2...) ____m_M3i64_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3i64_ofCols(m_M3i64_ColV c0, m_M3i64_ColV c1, m_M3i64_ColV c2))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_ofRows(m_M3i64_RowV r0, m_M3i64_RowV r1, m_M3i64_RowV r2))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_splat(i64 scalar))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_diag(i64 d))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_fromM4(m_M4i64 m))(m_M3i64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3i64_neg(m_M3i64 m))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_add(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_sub(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_mul(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_div(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_mod(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3i64_scal(m_M3i64 m, i64 scalar))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_scalInv(m_M3i64 m, i64 scalar))(m_M3i64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3i64_mulM(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_mulV(m_M3i64 m, m_V3i64 v))(m_V3i64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3i64_col(m_M3i64 m, usize idx))(m_M3i64_ColV));
$attr($inline_always)
$static fn_((m_M3i64_row(m_M3i64 m, usize idx))(m_M3i64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3i64_transp(m_M3i64 m))(m_M3i64));
$attr($inline_always)
$static fn_((m_M3i64_det(m_M3i64 m))(i64));

/*========== m_M4i64 Constants and Declarations ===============================*/

/* Constants */
#define m_M4i64_zero __comp_const__m_M4i64_zero
#define m_M4i64_identity __comp_const__m_M4i64_identity

/* Creation */
#define m_M4i64_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4i64_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4i64_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4i64_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4i64_ofCols(m_M4i64_ColV c0, m_M4i64_ColV c1, m_M4i64_ColV c2, m_M4i64_ColV c3))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_ofRows(m_M4i64_RowV r0, m_M4i64_RowV r1, m_M4i64_RowV r2, m_M4i64_RowV r3))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_splat(i64 scalar))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_diag(i64 d))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_fromM3(m_M3i64 m))(m_M4i64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4i64_neg(m_M4i64 m))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_add(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_sub(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_mul(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_div(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_mod(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4i64_scal(m_M4i64 m, i64 scalar))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_scalInv(m_M4i64 m, i64 scalar))(m_M4i64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4i64_mulM(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_mulV(m_M4i64 m, m_V4i64 v))(m_V4i64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4i64_col(m_M4i64 m, usize idx))(m_M4i64_ColV));
$attr($inline_always)
$static fn_((m_M4i64_row(m_M4i64 m, usize idx))(m_M4i64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4i64_transp(m_M4i64 m))(m_M4i64));
$attr($inline_always)
$static fn_((m_M4i64_det(m_M4i64 m))(i64));

/*========== m_M2isize Constants and Declarations ===============================*/

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
$static fn_((m_M2isize_mod(m_M2isize lhs, m_M2isize rhs))(m_M2isize));

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

/*========== m_M3isize Constants and Declarations ===============================*/

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
$static fn_((m_M3isize_mod(m_M3isize lhs, m_M3isize rhs))(m_M3isize));

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

/*========== m_M4isize Constants and Declarations ===============================*/

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
$static fn_((m_M4isize_mod(m_M4isize lhs, m_M4isize rhs))(m_M4isize));

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
$static fn_((m_M2f32_transp(m_M2f32 m))(m_M2f32));
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
$static fn_((m_M3f32_transp(m_M3f32 m))(m_M3f32));
$attr($inline_always)
$static fn_((m_M3f32_det(m_M3f32 m))(f32));
$attr($inline_always)
$static fn_((m_M3f32_inv(m_M3f32 m))(m_M3f32));

/* 2D Transformations (homogeneous coordinates) */
$attr($inline_always)
$static fn_((m_M3f32_transl2D(m_V2f32 t))(m_M3f32));
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
$static fn_((m_M4f32_transp(m_M4f32 m))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_det(m_M4f32 m))(f32));
$attr($inline_always)
$static fn_((m_M4f32_inv(m_M4f32 m))(m_M4f32));

/* 3D Transformations */
$attr($inline_always)
$static fn_((m_M4f32_transl(m_V3f32 t))(m_M4f32));
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
$static fn_((m_M4f32_lookAtRH(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_lookAtLH(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectRHNO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectRHZO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectLHNO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectLHZO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectInfFarRHNO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectInfFarRHZO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectInfFarLHNO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_perspectInfFarLHZO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_orthoRHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_orthoRHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_orthoLHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_orthoLHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_frustumRHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_frustumRHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_frustumLHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));
$attr($inline_always)
$static fn_((m_M4f32_frustumLHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32));

/*========== M2f64 Constants and Declarations ===============================*/

/* Constants */
#define m_M2f64_zero __comp_const__m_M2f64_zero
#define m_M2f64_identity __comp_const__m_M2f64_identity

/* Creation */
#define m_M2f64_ofCols_static(_c0, _c1...) ____m_M2f64_ofCols_static(_c0, _c1)
#define m_M2f64_ofRows_static(_r0, _r1...) ____m_M2f64_ofRows_static(_r0, _r1)
$attr($inline_always)
$static fn_((m_M2f64_ofCols(m_M2f64_ColV c0, m_M2f64_ColV c1))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_ofRows(m_M2f64_RowV r0, m_M2f64_RowV r1))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_splat(f64 scalar))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_diag(f64 d))(m_M2f64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M2f64_neg(m_M2f64 m))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_add(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_sub(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M2f64_scal(m_M2f64 m, f64 scalar))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_scalInv(m_M2f64 m, f64 scalar))(m_M2f64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M2f64_mulM(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_mulV(m_M2f64 m, m_V2f64 v))(m_V2f64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M2f64_col(m_M2f64 m, usize idx))(m_M2f64_ColV));
$attr($inline_always)
$static fn_((m_M2f64_row(m_M2f64 m, usize idx))(m_M2f64_RowV));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M2f64_transp(m_M2f64 m))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_det(m_M2f64 m))(f64));
$attr($inline_always)
$static fn_((m_M2f64_inv(m_M2f64 m))(m_M2f64));

/* Transformations */
$attr($inline_always)
$static fn_((m_M2f64_rotate(f64 angle))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_scale(m_V2f64 s))(m_M2f64));
$attr($inline_always)
$static fn_((m_M2f64_scaleUniform(f64 s))(m_M2f64));

/*========== M3f64 Constants and Declarations ===============================*/

/* Constants */
#define m_M3f64_zero __comp_const__m_M3f64_zero
#define m_M3f64_identity __comp_const__m_M3f64_identity

/* Creation */
#define m_M3f64_ofCols_static(_c0, _c1, _c2...) ____m_M3f64_ofCols_static(_c0, _c1, _c2)
#define m_M3f64_ofRows_static(_r0, _r1, _r2...) ____m_M3f64_ofRows_static(_r0, _r1, _r2)
$attr($inline_always)
$static fn_((m_M3f64_ofCols(m_M3f64_ColV c0, m_M3f64_ColV c1, m_M3f64_ColV c2))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_ofRows(m_M3f64_RowV r0, m_M3f64_RowV r1, m_M3f64_RowV r2))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_splat(f64 scalar))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_diag(f64 d))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_fromM4(m_M4f64 m))(m_M3f64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M3f64_neg(m_M3f64 m))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_add(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_sub(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M3f64_scal(m_M3f64 m, f64 scalar))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_scalInv(m_M3f64 m, f64 scalar))(m_M3f64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M3f64_mulM(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_mulV(m_M3f64 m, m_V3f64 v))(m_V3f64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M3f64_col(m_M3f64 m, usize idx))(m_V3f64));
$attr($inline_always)
$static fn_((m_M3f64_row(m_M3f64 m, usize idx))(m_V3f64));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M3f64_transp(m_M3f64 m))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_det(m_M3f64 m))(f64));
$attr($inline_always)
$static fn_((m_M3f64_inv(m_M3f64 m))(m_M3f64));

/* 2D Transformations (homogeneous coordinates) */
$attr($inline_always)
$static fn_((m_M3f64_transl2D(m_V2f64 t))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_rotate2D(f64 angle))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_scale2D(m_V2f64 s))(m_M3f64));
$attr($inline_always)
$static fn_((m_M3f64_scaleUniform2D(f64 s))(m_M3f64));

/*========== M4f64 Constants and Declarations ===============================*/

/* Constants */
#define m_M4f64_zero __comp_const__m_M4f64_zero
#define m_M4f64_identity __comp_const__m_M4f64_identity

/* Creation */
#define m_M4f64_ofCols_static(_c0, _c1, _c2, _c3...) ____m_M4f64_ofCols_static(_c0, _c1, _c2, _c3)
#define m_M4f64_ofRows_static(_r0, _r1, _r2, _r3...) ____m_M4f64_ofRows_static(_r0, _r1, _r2, _r3)
$attr($inline_always)
$static fn_((m_M4f64_ofCols(m_M4f64_ColV c0, m_M4f64_ColV c1, m_M4f64_ColV c2, m_M4f64_ColV c3))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_ofRows(m_M4f64_RowV r0, m_M4f64_RowV r1, m_M4f64_RowV r2, m_M4f64_RowV r3))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_splat(f64 scalar))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_diag(f64 d))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_fromM3(m_M3f64 m))(m_M4f64));

/* Component-wise Arithmetic */
$attr($inline_always)
$static fn_((m_M4f64_neg(m_M4f64 m))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_add(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_sub(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64));

/* Scalar Operations */
$attr($inline_always)
$static fn_((m_M4f64_scal(m_M4f64 m, f64 scalar))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_scalInv(m_M4f64 m, f64 scalar))(m_M4f64));

/* Matrix Operations */
$attr($inline_always)
$static fn_((m_M4f64_mulM(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_mulV(m_M4f64 m, m_V4f64 v))(m_V4f64));
$attr($inline_always)
$static fn_((m_M4f64_mulV3(m_M4f64 m, m_V3f64 v, f64 w))(m_V3f64));

/* Accessors */
$attr($inline_always)
$static fn_((m_M4f64_col(m_M4f64 m, usize idx))(m_V4f64));
$attr($inline_always)
$static fn_((m_M4f64_row(m_M4f64 m, usize idx))(m_V4f64));

/* Matrix Properties */
$attr($inline_always)
$static fn_((m_M4f64_transp(m_M4f64 m))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_det(m_M4f64 m))(f64));
$attr($inline_always)
$static fn_((m_M4f64_inv(m_M4f64 m))(m_M4f64));

/* 3D Transformations */
$attr($inline_always)
$static fn_((m_M4f64_transl(m_V3f64 t))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_rotateX(f64 angle))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_rotateY(f64 angle))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_rotateZ(f64 angle))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_rotateAxis(m_V3f64 axis, f64 angle))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_scale(m_V3f64 s))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_scaleUniform(f64 s))(m_M4f64));

/* View and Projection */
$attr($inline_always)
$static fn_((m_M4f64_lookAtRH(m_V3f64 eye, m_V3f64 target, m_V3f64 up))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_lookAtLH(m_V3f64 eye, m_V3f64 target, m_V3f64 up))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectRHNO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectRHZO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectLHNO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectLHZO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectInfFarRHNO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectInfFarRHZO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectInfFarLHNO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_perspectInfFarLHZO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_orthoRHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_orthoRHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_orthoLHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_orthoLHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_frustumRHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_frustumRHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_frustumLHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));
$attr($inline_always)
$static fn_((m_M4f64_frustumLHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64));

/*========== Macros and Definitions==========================================*/

#define __step__m_M_as$(...) __step__m_M_as$__emit(__step__m_M_as$__parse __VA_ARGS__)
#define __step__m_M_as$__parse(_T_Dst...) \
    pp_uniqTok(len), pp_uniqTok(d), pp_uniqTok(s), \
        pp_uniqTok(dst), _T_Dst, pp_uniqTok(src),
#define __step__m_M_as$__emit(...) ____m_M_as$(__VA_ARGS__)
#define ____m_M_as$(__len, __d, __s, __dst, _T_Dst, __src, _src...) local_({ \
    var_(__dst, _T_Dst) = cleared(); \
    let_(__src, TypeOf(_src)) = _src; \
    let_(__len, usize) = A_len$(TypeOf(__dst.s)); \
    claim_assert_static(__len == A_len$(TypeOf(__src.s))); \
    for_(($s(A_ref(__dst.s)), $s(A_ref(__src.s)))(__d, __s)) { \
        *__d = as$(TypeOf(*__d))(*__s); \
    } $end(for); \
    local_return_(__dst); \
})

/*========== m_M2u32 Implementations ==========================================*/

#define __comp_const__m_M2u32_zero m_M2u32_ofCols_static( \
    m_V2u32_zero, \
    m_V2u32_zero \
)
#define __comp_const__m_M2u32_identity m_M2u32_ofCols_static( \
    m_V2u32_unit_x, \
    m_V2u32_unit_y \
)

#define ____m_M2u32_ofCols_static(_c0, _c1...) l$((m_M2u32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2u32_ofRows_static(_r0, _r1...) l$((m_M2u32){ \
    .cols = A_init({ \
        [0] = m_V2u32_of_static((_r0).x, (_r1).x), \
        [1] = m_V2u32_of_static((_r0).y, (_r1).y), \
    }), \
})
fn_((m_M2u32_ofCols(m_M2u32_ColV c0, m_M2u32_ColV c1))(m_M2u32)) {
    return m_M2u32_ofCols_static(c0, c1);
};
fn_((m_M2u32_ofRows(m_M2u32_RowV r0, m_M2u32_RowV r1))(m_M2u32)) {
    return m_M2u32_ofRows_static(r0, r1);
};
fn_((m_M2u32_splat(u32 scalar))(m_M2u32)) {
    let v = m_V2u32_splat(scalar);
    return m_M2u32_ofCols(v, v);
};
fn_((m_M2u32_diag(u32 d))(m_M2u32)) {
    return m_M2u32_ofRows(
        m_V2u32_of(d, 0),
        m_V2u32_of(0, d)
    );
};

fn_((m_M2u32_add(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u32_sub(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u32_mul(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u32_div(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u32_mod(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u32_scal(m_M2u32 x, u32 scalar))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_scal(*A_at((x.cols)[0]), scalar),
        m_V2u32_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2u32_scalInv(m_M2u32 x, u32 scalar))(m_M2u32)) {
    return m_M2u32_ofCols(
        m_V2u32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2u32_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2u32_mulM(m_M2u32 lhs, m_M2u32 rhs))(m_M2u32)) {
    m_M2u32 result = m_M2u32_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            u32 sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2u32_mulV(m_M2u32 x, m_V2u32 y))(m_V2u32)) {
    return m_V2u32_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2u32_col(m_M2u32 x, usize idx))(m_M2u32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2u32_row(m_M2u32 x, usize idx))(m_M2u32_RowV)) {
    return m_V2u32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2u32_transp(m_M2u32 x))(m_M2u32)) {
    return m_M2u32_ofRows(
        m_V2u32_of(x.s0_0, x.s1_0),
        m_V2u32_of(x.s0_1, x.s1_1)
    );
};

/*========== m_M3u32 Implementations ==========================================*/

#define __comp_const__m_M3u32_zero m_M3u32_ofCols_static( \
    m_V3u32_zero, \
    m_V3u32_zero, \
    m_V3u32_zero \
)
#define __comp_const__m_M3u32_identity m_M3u32_ofCols_static( \
    m_V3u32_unit_x, \
    m_V3u32_unit_y, \
    m_V3u32_unit_z \
)

#define ____m_M3u32_ofCols_static(_c0, _c1, _c2...) l$((m_M3u32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3u32_ofRows_static(_r0, _r1, _r2...) l$((m_M3u32){ \
    .cols = A_init({ \
        [0] = m_V3u32_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3u32_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3u32_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3u32_ofCols(m_M3u32_ColV c0, m_M3u32_ColV c1, m_M3u32_ColV c2))(m_M3u32)) {
    return m_M3u32_ofCols_static(c0, c1, c2);
};
fn_((m_M3u32_ofRows(m_M3u32_RowV r0, m_M3u32_RowV r1, m_M3u32_RowV r2))(m_M3u32)) {
    return m_M3u32_ofRows_static(r0, r1, r2);
};
fn_((m_M3u32_splat(u32 scalar))(m_M3u32)) {
    let v = m_V3u32_splat(scalar);
    return m_M3u32_ofCols(v, v, v);
};
fn_((m_M3u32_diag(u32 d))(m_M3u32)) {
    return m_M3u32_ofRows(
        m_V3u32_of(d, 0, 0),
        m_V3u32_of(0, d, 0),
        m_V3u32_of(0, 0, d)
    );
};

fn_((m_M3u32_fromM4(m_M4u32 x))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_from4(*A_at((x.cols)[0])),
        m_V3u32_from4(*A_at((x.cols)[1])),
        m_V3u32_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3u32_add(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u32_sub(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u32_mul(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u32_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u32_div(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u32_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u32_mod(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u32_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u32_scal(m_M3u32 x, u32 scalar))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_scal(*A_at((x.cols)[0]), scalar),
        m_V3u32_scal(*A_at((x.cols)[1]), scalar),
        m_V3u32_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3u32_scalInv(m_M3u32 x, u32 scalar))(m_M3u32)) {
    return m_M3u32_ofCols(
        m_V3u32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3u32_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3u32_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3u32_mulM(m_M3u32 lhs, m_M3u32 rhs))(m_M3u32)) {
    m_M3u32 result = m_M3u32_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            u32 sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3u32_mulV(m_M3u32 x, m_V3u32 y))(m_V3u32)) {
    return m_V3u32_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3u32_col(m_M3u32 x, usize idx))(m_M3u32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3u32_row(m_M3u32 x, usize idx))(m_M3u32_RowV)) {
    return m_V3u32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3u32_transp(m_M3u32 x))(m_M3u32)) {
    return m_M3u32_ofRows(
        m_V3u32_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3u32_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3u32_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

/*========== m_M4u32 Implementations ==========================================*/

#define __comp_const__m_M4u32_zero m_M4u32_ofCols_static( \
    m_V4u32_zero, \
    m_V4u32_zero, \
    m_V4u32_zero, \
    m_V4u32_zero \
)
#define __comp_const__m_M4u32_identity m_M4u32_ofCols_static( \
    m_V4u32_unit_x, \
    m_V4u32_unit_y, \
    m_V4u32_unit_z, \
    m_V4u32_unit_w \
)

#define ____m_M4u32_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4u32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4u32_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4u32){ \
    .cols = A_init({ \
        [0] = m_V4u32_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4u32_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4u32_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4u32_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4u32_ofCols(m_M4u32_ColV c0, m_M4u32_ColV c1, m_M4u32_ColV c2, m_M4u32_ColV c3))(m_M4u32)) {
    return m_M4u32_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4u32_ofRows(m_M4u32_RowV r0, m_M4u32_RowV r1, m_M4u32_RowV r2, m_M4u32_RowV r3))(m_M4u32)) {
    return m_M4u32_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4u32_splat(u32 scalar))(m_M4u32)) {
    let v = m_V4u32_splat(scalar);
    return m_M4u32_ofCols(v, v, v, v);
};
fn_((m_M4u32_diag(u32 d))(m_M4u32)) {
    return m_M4u32_ofRows(
        m_V4u32_of(d, 0, 0, 0),
        m_V4u32_of(0, d, 0, 0),
        m_V4u32_of(0, 0, d, 0),
        m_V4u32_of(0, 0, 0, d)
    );
};

fn_((m_M4u32_fromM3(m_M3u32 x))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4u32_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4u32_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4u32_of(0, 0, 0, 1)
    );
};

fn_((m_M4u32_add(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u32_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u32_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u32_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u32_sub(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u32_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u32_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u32_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u32_mul(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u32_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u32_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u32_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u32_div(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u32_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u32_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u32_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u32_mod(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u32_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u32_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u32_scal(m_M4u32 x, u32 scalar))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_scal(*A_at((x.cols)[0]), scalar),
        m_V4u32_scal(*A_at((x.cols)[1]), scalar),
        m_V4u32_scal(*A_at((x.cols)[2]), scalar),
        m_V4u32_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4u32_scalInv(m_M4u32 x, u32 scalar))(m_M4u32)) {
    return m_M4u32_ofCols(
        m_V4u32_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4u32_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4u32_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4u32_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4u32_mulM(m_M4u32 lhs, m_M4u32 rhs))(m_M4u32)) {
    m_M4u32 result = m_M4u32_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            u32 sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4u32_mulV(m_M4u32 x, m_V4u32 y))(m_V4u32)) {
    return m_V4u32_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4u32_col(m_M4u32 x, usize idx))(m_M4u32_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4u32_row(m_M4u32 x, usize idx))(m_M4u32_RowV)) {
    return m_V4u32_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4u32_transp(m_M4u32 x))(m_M4u32)) {
    return m_M4u32_ofRows(
        m_V4u32_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4u32_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4u32_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4u32_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

/*========== m_M2u64 Implementations ==========================================*/

#define __comp_const__m_M2u64_zero m_M2u64_ofCols_static( \
    m_V2u64_zero, \
    m_V2u64_zero \
)
#define __comp_const__m_M2u64_identity m_M2u64_ofCols_static( \
    m_V2u64_unit_x, \
    m_V2u64_unit_y \
)

#define ____m_M2u64_ofCols_static(_c0, _c1...) l$((m_M2u64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2u64_ofRows_static(_r0, _r1...) l$((m_M2u64){ \
    .cols = A_init({ \
        [0] = m_V2u64_of_static((_r0).x, (_r1).x), \
        [1] = m_V2u64_of_static((_r0).y, (_r1).y), \
    }), \
})
fn_((m_M2u64_ofCols(m_M2u64_ColV c0, m_M2u64_ColV c1))(m_M2u64)) {
    return m_M2u64_ofCols_static(c0, c1);
};
fn_((m_M2u64_ofRows(m_M2u64_RowV r0, m_M2u64_RowV r1))(m_M2u64)) {
    return m_M2u64_ofRows_static(r0, r1);
};
fn_((m_M2u64_splat(u64 scalar))(m_M2u64)) {
    let v = m_V2u64_splat(scalar);
    return m_M2u64_ofCols(v, v);
};
fn_((m_M2u64_diag(u64 d))(m_M2u64)) {
    return m_M2u64_ofRows(
        m_V2u64_of(d, 0),
        m_V2u64_of(0, d)
    );
};

fn_((m_M2u64_add(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u64_sub(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u64_mul(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u64_div(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u64_mod(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2u64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2u64_scal(m_M2u64 x, u64 scalar))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_scal(*A_at((x.cols)[0]), scalar),
        m_V2u64_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2u64_scalInv(m_M2u64 x, u64 scalar))(m_M2u64)) {
    return m_M2u64_ofCols(
        m_V2u64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2u64_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2u64_mulM(m_M2u64 lhs, m_M2u64 rhs))(m_M2u64)) {
    m_M2u64 result = m_M2u64_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            u64 sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2u64_mulV(m_M2u64 x, m_V2u64 y))(m_V2u64)) {
    return m_V2u64_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2u64_col(m_M2u64 x, usize idx))(m_M2u64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2u64_row(m_M2u64 x, usize idx))(m_M2u64_RowV)) {
    return m_V2u64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2u64_transp(m_M2u64 x))(m_M2u64)) {
    return m_M2u64_ofRows(
        m_V2u64_of(x.s0_0, x.s1_0),
        m_V2u64_of(x.s0_1, x.s1_1)
    );
};

/*========== m_M3u64 Implementations ==========================================*/

#define __comp_const__m_M3u64_zero m_M3u64_ofCols_static( \
    m_V3u64_zero, \
    m_V3u64_zero, \
    m_V3u64_zero \
)
#define __comp_const__m_M3u64_identity m_M3u64_ofCols_static( \
    m_V3u64_unit_x, \
    m_V3u64_unit_y, \
    m_V3u64_unit_z \
)

#define ____m_M3u64_ofCols_static(_c0, _c1, _c2...) l$((m_M3u64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3u64_ofRows_static(_r0, _r1, _r2...) l$((m_M3u64){ \
    .cols = A_init({ \
        [0] = m_V3u64_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3u64_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3u64_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3u64_ofCols(m_M3u64_ColV c0, m_M3u64_ColV c1, m_M3u64_ColV c2))(m_M3u64)) {
    return m_M3u64_ofCols_static(c0, c1, c2);
};
fn_((m_M3u64_ofRows(m_M3u64_RowV r0, m_M3u64_RowV r1, m_M3u64_RowV r2))(m_M3u64)) {
    return m_M3u64_ofRows_static(r0, r1, r2);
};
fn_((m_M3u64_splat(u64 scalar))(m_M3u64)) {
    let v = m_V3u64_splat(scalar);
    return m_M3u64_ofCols(v, v, v);
};
fn_((m_M3u64_diag(u64 d))(m_M3u64)) {
    return m_M3u64_ofRows(
        m_V3u64_of(d, 0, 0),
        m_V3u64_of(0, d, 0),
        m_V3u64_of(0, 0, d)
    );
};

fn_((m_M3u64_fromM4(m_M4u64 x))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_from4(*A_at((x.cols)[0])),
        m_V3u64_from4(*A_at((x.cols)[1])),
        m_V3u64_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3u64_add(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u64_sub(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u64_mul(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u64_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u64_div(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u64_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u64_mod(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3u64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3u64_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3u64_scal(m_M3u64 x, u64 scalar))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_scal(*A_at((x.cols)[0]), scalar),
        m_V3u64_scal(*A_at((x.cols)[1]), scalar),
        m_V3u64_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3u64_scalInv(m_M3u64 x, u64 scalar))(m_M3u64)) {
    return m_M3u64_ofCols(
        m_V3u64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3u64_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3u64_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3u64_mulM(m_M3u64 lhs, m_M3u64 rhs))(m_M3u64)) {
    m_M3u64 result = m_M3u64_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            u64 sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3u64_mulV(m_M3u64 x, m_V3u64 y))(m_V3u64)) {
    return m_V3u64_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3u64_col(m_M3u64 x, usize idx))(m_M3u64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3u64_row(m_M3u64 x, usize idx))(m_M3u64_RowV)) {
    return m_V3u64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3u64_transp(m_M3u64 x))(m_M3u64)) {
    return m_M3u64_ofRows(
        m_V3u64_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3u64_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3u64_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

/*========== m_M4u64 Implementations ==========================================*/

#define __comp_const__m_M4u64_zero m_M4u64_ofCols_static( \
    m_V4u64_zero, \
    m_V4u64_zero, \
    m_V4u64_zero, \
    m_V4u64_zero \
)
#define __comp_const__m_M4u64_identity m_M4u64_ofCols_static( \
    m_V4u64_unit_x, \
    m_V4u64_unit_y, \
    m_V4u64_unit_z, \
    m_V4u64_unit_w \
)

#define ____m_M4u64_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4u64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4u64_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4u64){ \
    .cols = A_init({ \
        [0] = m_V4u64_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4u64_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4u64_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4u64_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4u64_ofCols(m_M4u64_ColV c0, m_M4u64_ColV c1, m_M4u64_ColV c2, m_M4u64_ColV c3))(m_M4u64)) {
    return m_M4u64_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4u64_ofRows(m_M4u64_RowV r0, m_M4u64_RowV r1, m_M4u64_RowV r2, m_M4u64_RowV r3))(m_M4u64)) {
    return m_M4u64_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4u64_splat(u64 scalar))(m_M4u64)) {
    let v = m_V4u64_splat(scalar);
    return m_M4u64_ofCols(v, v, v, v);
};
fn_((m_M4u64_diag(u64 d))(m_M4u64)) {
    return m_M4u64_ofRows(
        m_V4u64_of(d, 0, 0, 0),
        m_V4u64_of(0, d, 0, 0),
        m_V4u64_of(0, 0, d, 0),
        m_V4u64_of(0, 0, 0, d)
    );
};

fn_((m_M4u64_fromM3(m_M3u64 x))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4u64_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4u64_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4u64_of(0, 0, 0, 1)
    );
};

fn_((m_M4u64_add(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u64_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u64_sub(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u64_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u64_mul(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u64_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u64_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u64_div(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u64_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u64_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u64_mod(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4u64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4u64_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4u64_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4u64_scal(m_M4u64 x, u64 scalar))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_scal(*A_at((x.cols)[0]), scalar),
        m_V4u64_scal(*A_at((x.cols)[1]), scalar),
        m_V4u64_scal(*A_at((x.cols)[2]), scalar),
        m_V4u64_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4u64_scalInv(m_M4u64 x, u64 scalar))(m_M4u64)) {
    return m_M4u64_ofCols(
        m_V4u64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4u64_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4u64_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4u64_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4u64_mulM(m_M4u64 lhs, m_M4u64 rhs))(m_M4u64)) {
    m_M4u64 result = m_M4u64_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            u64 sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4u64_mulV(m_M4u64 x, m_V4u64 y))(m_V4u64)) {
    return m_V4u64_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4u64_col(m_M4u64 x, usize idx))(m_M4u64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4u64_row(m_M4u64 x, usize idx))(m_M4u64_RowV)) {
    return m_V4u64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4u64_transp(m_M4u64 x))(m_M4u64)) {
    return m_M4u64_ofRows(
        m_V4u64_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4u64_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4u64_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4u64_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

/*========== m_M2usize Implementations ==========================================*/

#define __comp_const__m_M2usize_zero m_M2usize_ofCols_static( \
    m_V2usize_zero, \
    m_V2usize_zero \
)
#define __comp_const__m_M2usize_identity m_M2usize_ofCols_static( \
    m_V2usize_unit_x, \
    m_V2usize_unit_y \
)

#define ____m_M2usize_ofCols_static(_c0, _c1...) l$((m_M2usize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2usize_ofRows_static(_r0, _r1...) l$((m_M2usize){ \
    .cols = A_init({ \
        [0] = m_V2usize_of_static((_r0).x, (_r1).x), \
        [1] = m_V2usize_of_static((_r0).y, (_r1).y), \
    }), \
})
fn_((m_M2usize_ofCols(m_M2usize_ColV c0, m_M2usize_ColV c1))(m_M2usize)) {
    return m_M2usize_ofCols_static(c0, c1);
};
fn_((m_M2usize_ofRows(m_M2usize_RowV r0, m_M2usize_RowV r1))(m_M2usize)) {
    return m_M2usize_ofRows_static(r0, r1);
};
fn_((m_M2usize_splat(usize scalar))(m_M2usize)) {
    let v = m_V2usize_splat(scalar);
    return m_M2usize_ofCols(v, v);
};
fn_((m_M2usize_diag(usize d))(m_M2usize)) {
    return m_M2usize_ofRows(
        m_V2usize_of(d, 0),
        m_V2usize_of(0, d)
    );
};

fn_((m_M2usize_add(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2usize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2usize_sub(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2usize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2usize_mul(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2usize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2usize_div(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2usize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2usize_mod(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2usize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2usize_scal(m_M2usize x, usize scalar))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_scal(*A_at((x.cols)[0]), scalar),
        m_V2usize_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2usize_scalInv(m_M2usize x, usize scalar))(m_M2usize)) {
    return m_M2usize_ofCols(
        m_V2usize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2usize_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2usize_mulM(m_M2usize lhs, m_M2usize rhs))(m_M2usize)) {
    m_M2usize result = m_M2usize_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            usize sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2usize_mulV(m_M2usize x, m_V2usize y))(m_V2usize)) {
    return m_V2usize_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2usize_col(m_M2usize x, usize idx))(m_M2usize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2usize_row(m_M2usize x, usize idx))(m_M2usize_RowV)) {
    return m_V2usize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2usize_transp(m_M2usize x))(m_M2usize)) {
    return m_M2usize_ofRows(
        m_V2usize_of(x.s0_0, x.s1_0),
        m_V2usize_of(x.s0_1, x.s1_1)
    );
};

/*========== m_M3usize Implementations ==========================================*/

#define __comp_const__m_M3usize_zero m_M3usize_ofCols_static( \
    m_V3usize_zero, \
    m_V3usize_zero, \
    m_V3usize_zero \
)
#define __comp_const__m_M3usize_identity m_M3usize_ofCols_static( \
    m_V3usize_unit_x, \
    m_V3usize_unit_y, \
    m_V3usize_unit_z \
)

#define ____m_M3usize_ofCols_static(_c0, _c1, _c2...) l$((m_M3usize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3usize_ofRows_static(_r0, _r1, _r2...) l$((m_M3usize){ \
    .cols = A_init({ \
        [0] = m_V3usize_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3usize_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3usize_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3usize_ofCols(m_M3usize_ColV c0, m_M3usize_ColV c1, m_M3usize_ColV c2))(m_M3usize)) {
    return m_M3usize_ofCols_static(c0, c1, c2);
};
fn_((m_M3usize_ofRows(m_M3usize_RowV r0, m_M3usize_RowV r1, m_M3usize_RowV r2))(m_M3usize)) {
    return m_M3usize_ofRows_static(r0, r1, r2);
};
fn_((m_M3usize_splat(usize scalar))(m_M3usize)) {
    let v = m_V3usize_splat(scalar);
    return m_M3usize_ofCols(v, v, v);
};
fn_((m_M3usize_diag(usize d))(m_M3usize)) {
    return m_M3usize_ofRows(
        m_V3usize_of(d, 0, 0),
        m_V3usize_of(0, d, 0),
        m_V3usize_of(0, 0, d)
    );
};

fn_((m_M3usize_fromM4(m_M4usize x))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_from4(*A_at((x.cols)[0])),
        m_V3usize_from4(*A_at((x.cols)[1])),
        m_V3usize_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3usize_add(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3usize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3usize_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3usize_sub(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3usize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3usize_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3usize_mul(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3usize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3usize_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3usize_div(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3usize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3usize_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3usize_mod(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3usize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3usize_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3usize_scal(m_M3usize x, usize scalar))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_scal(*A_at((x.cols)[0]), scalar),
        m_V3usize_scal(*A_at((x.cols)[1]), scalar),
        m_V3usize_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3usize_scalInv(m_M3usize x, usize scalar))(m_M3usize)) {
    return m_M3usize_ofCols(
        m_V3usize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3usize_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3usize_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3usize_mulM(m_M3usize lhs, m_M3usize rhs))(m_M3usize)) {
    m_M3usize result = m_M3usize_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            usize sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3usize_mulV(m_M3usize x, m_V3usize y))(m_V3usize)) {
    return m_V3usize_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3usize_col(m_M3usize x, usize idx))(m_M3usize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3usize_row(m_M3usize x, usize idx))(m_M3usize_RowV)) {
    return m_V3usize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3usize_transp(m_M3usize x))(m_M3usize)) {
    return m_M3usize_ofRows(
        m_V3usize_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3usize_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3usize_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

/*========== m_M4usize Implementations ==========================================*/

#define __comp_const__m_M4usize_zero m_M4usize_ofCols_static( \
    m_V4usize_zero, \
    m_V4usize_zero, \
    m_V4usize_zero, \
    m_V4usize_zero \
)
#define __comp_const__m_M4usize_identity m_M4usize_ofCols_static( \
    m_V4usize_unit_x, \
    m_V4usize_unit_y, \
    m_V4usize_unit_z, \
    m_V4usize_unit_w \
)

#define ____m_M4usize_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4usize){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4usize_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4usize){ \
    .cols = A_init({ \
        [0] = m_V4usize_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4usize_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4usize_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4usize_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4usize_ofCols(m_M4usize_ColV c0, m_M4usize_ColV c1, m_M4usize_ColV c2, m_M4usize_ColV c3))(m_M4usize)) {
    return m_M4usize_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4usize_ofRows(m_M4usize_RowV r0, m_M4usize_RowV r1, m_M4usize_RowV r2, m_M4usize_RowV r3))(m_M4usize)) {
    return m_M4usize_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4usize_splat(usize scalar))(m_M4usize)) {
    let v = m_V4usize_splat(scalar);
    return m_M4usize_ofCols(v, v, v, v);
};
fn_((m_M4usize_diag(usize d))(m_M4usize)) {
    return m_M4usize_ofRows(
        m_V4usize_of(d, 0, 0, 0),
        m_V4usize_of(0, d, 0, 0),
        m_V4usize_of(0, 0, d, 0),
        m_V4usize_of(0, 0, 0, d)
    );
};

fn_((m_M4usize_fromM3(m_M3usize x))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4usize_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4usize_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4usize_of(0, 0, 0, 1)
    );
};

fn_((m_M4usize_add(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4usize_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4usize_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4usize_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4usize_sub(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4usize_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4usize_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4usize_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4usize_mul(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4usize_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4usize_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4usize_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4usize_div(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4usize_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4usize_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4usize_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4usize_mod(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4usize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4usize_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4usize_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4usize_scal(m_M4usize x, usize scalar))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_scal(*A_at((x.cols)[0]), scalar),
        m_V4usize_scal(*A_at((x.cols)[1]), scalar),
        m_V4usize_scal(*A_at((x.cols)[2]), scalar),
        m_V4usize_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4usize_scalInv(m_M4usize x, usize scalar))(m_M4usize)) {
    return m_M4usize_ofCols(
        m_V4usize_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4usize_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4usize_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4usize_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4usize_mulM(m_M4usize lhs, m_M4usize rhs))(m_M4usize)) {
    m_M4usize result = m_M4usize_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            usize sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4usize_mulV(m_M4usize x, m_V4usize y))(m_V4usize)) {
    return m_V4usize_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4usize_col(m_M4usize x, usize idx))(m_M4usize_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4usize_row(m_M4usize x, usize idx))(m_M4usize_RowV)) {
    return m_V4usize_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4usize_transp(m_M4usize x))(m_M4usize)) {
    return m_M4usize_ofRows(
        m_V4usize_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4usize_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4usize_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4usize_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

/*========== m_M2i32 Implementations ==========================================*/

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

fn_((m_M2i32_mod(m_M2i32 lhs, m_M2i32 rhs))(m_M2i32)) {
    return m_M2i32_ofCols(
        m_V2i32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
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

/*========== m_M3i32 Implementations ==========================================*/

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

fn_((m_M3i32_mod(m_M3i32 lhs, m_M3i32 rhs))(m_M3i32)) {
    return m_M3i32_ofCols(
        m_V3i32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i32_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
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

/*========== m_M4i32 Implementations ==========================================*/

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

fn_((m_M4i32_mod(m_M4i32 lhs, m_M4i32 rhs))(m_M4i32)) {
    return m_M4i32_ofCols(
        m_V4i32_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i32_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i32_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i32_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
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

/*========== m_M2i64 Implementations ==========================================*/

#define __comp_const__m_M2i64_zero m_M2i64_ofCols_static( \
    m_V2i64_zero, \
    m_V2i64_zero \
)
#define __comp_const__m_M2i64_identity m_M2i64_ofCols_static( \
    m_V2i64_unit_x, \
    m_V2i64_unit_y \
)

#define ____m_M2i64_ofCols_static(_c0, _c1...) l$((m_M2i64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2i64_ofRows_static(_r0, _r1...) l$((m_M2i64){ \
    .cols = A_init({ \
        [0] = m_V2i64_of_static((_r0).x, (_r1).x), \
        [1] = m_V2i64_of_static((_r0).y, (_r1).y), \
    }), \
})
fn_((m_M2i64_ofCols(m_M2i64_ColV c0, m_M2i64_ColV c1))(m_M2i64)) {
    return m_M2i64_ofCols_static(c0, c1);
};
fn_((m_M2i64_ofRows(m_M2i64_RowV r0, m_M2i64_RowV r1))(m_M2i64)) {
    return m_M2i64_ofRows_static(r0, r1);
};
fn_((m_M2i64_splat(i64 scalar))(m_M2i64)) {
    let v = m_V2i64_splat(scalar);
    return m_M2i64_ofCols(v, v);
};
fn_((m_M2i64_diag(i64 d))(m_M2i64)) {
    return m_M2i64_ofRows(
        m_V2i64_of(d, 0),
        m_V2i64_of(0, d)
    );
};

fn_((m_M2i64_neg(m_M2i64 x))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_neg(*A_at((x.cols)[0])),
        m_V2i64_neg(*A_at((x.cols)[1]))
    );
};

fn_((m_M2i64_add(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i64_sub(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i64_mul(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i64_div(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i64_mod(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2i64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2i64_scal(m_M2i64 x, i64 scalar))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_scal(*A_at((x.cols)[0]), scalar),
        m_V2i64_scal(*A_at((x.cols)[1]), scalar)
    );
};
fn_((m_M2i64_scalInv(m_M2i64 x, i64 scalar))(m_M2i64)) {
    return m_M2i64_ofCols(
        m_V2i64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V2i64_scalInv(*A_at((x.cols)[1]), scalar)
    );
};

fn_((m_M2i64_mulM(m_M2i64 lhs, m_M2i64 rhs))(m_M2i64)) {
    m_M2i64 result = m_M2i64_zero;
    for (usize c = 0; c < 2; ++c) {
        for (usize r = 0; r < 2; ++r) {
            i64 sum = 0;
            for (usize q = 0; q < 2; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M2i64_mulV(m_M2i64 x, m_V2i64 y))(m_V2i64)) {
    return m_V2i64_of(
        x.s0_0 * y.x + x.s0_1 * y.y,
        x.s1_0 * y.x + x.s1_1 * y.y
    );
};

fn_((m_M2i64_col(m_M2i64 x, usize idx))(m_M2i64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M2i64_row(m_M2i64 x, usize idx))(m_M2i64_RowV)) {
    return m_V2i64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx])
    );
};

fn_((m_M2i64_transp(m_M2i64 x))(m_M2i64)) {
    return m_M2i64_ofRows(
        m_V2i64_of(x.s0_0, x.s1_0),
        m_V2i64_of(x.s0_1, x.s1_1)
    );
};

fn_((m_M2i64_det(m_M2i64 x))(i64)) {
    return x.s0_0 * x.s1_1 - x.s0_1 * x.s1_0;
};

/*========== m_M3i64 Implementations ==========================================*/

#define __comp_const__m_M3i64_zero m_M3i64_ofCols_static( \
    m_V3i64_zero, \
    m_V3i64_zero, \
    m_V3i64_zero \
)
#define __comp_const__m_M3i64_identity m_M3i64_ofCols_static( \
    m_V3i64_unit_x, \
    m_V3i64_unit_y, \
    m_V3i64_unit_z \
)

#define ____m_M3i64_ofCols_static(_c0, _c1, _c2...) l$((m_M3i64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3i64_ofRows_static(_r0, _r1, _r2...) l$((m_M3i64){ \
    .cols = A_init({ \
        [0] = m_V3i64_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3i64_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3i64_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3i64_ofCols(m_M3i64_ColV c0, m_M3i64_ColV c1, m_M3i64_ColV c2))(m_M3i64)) {
    return m_M3i64_ofCols_static(c0, c1, c2);
};
fn_((m_M3i64_ofRows(m_M3i64_RowV r0, m_M3i64_RowV r1, m_M3i64_RowV r2))(m_M3i64)) {
    return m_M3i64_ofRows_static(r0, r1, r2);
};
fn_((m_M3i64_splat(i64 scalar))(m_M3i64)) {
    let v = m_V3i64_splat(scalar);
    return m_M3i64_ofCols(v, v, v);
};
fn_((m_M3i64_diag(i64 d))(m_M3i64)) {
    return m_M3i64_ofRows(
        m_V3i64_of(d, 0, 0),
        m_V3i64_of(0, d, 0),
        m_V3i64_of(0, 0, d)
    );
};

fn_((m_M3i64_fromM4(m_M4i64 x))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_from4(*A_at((x.cols)[0])),
        m_V3i64_from4(*A_at((x.cols)[1])),
        m_V3i64_from4(*A_at((x.cols)[2]))
    );
};

fn_((m_M3i64_neg(m_M3i64 x))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_neg(*A_at((x.cols)[0])),
        m_V3i64_neg(*A_at((x.cols)[1])),
        m_V3i64_neg(*A_at((x.cols)[2]))
    );
};

fn_((m_M3i64_add(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i64_sub(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i64_mul(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i64_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i64_div(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i64_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i64_mod(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3i64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3i64_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3i64_scal(m_M3i64 x, i64 scalar))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_scal(*A_at((x.cols)[0]), scalar),
        m_V3i64_scal(*A_at((x.cols)[1]), scalar),
        m_V3i64_scal(*A_at((x.cols)[2]), scalar)
    );
};
fn_((m_M3i64_scalInv(m_M3i64 x, i64 scalar))(m_M3i64)) {
    return m_M3i64_ofCols(
        m_V3i64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V3i64_scalInv(*A_at((x.cols)[1]), scalar),
        m_V3i64_scalInv(*A_at((x.cols)[2]), scalar)
    );
};

fn_((m_M3i64_mulM(m_M3i64 lhs, m_M3i64 rhs))(m_M3i64)) {
    m_M3i64 result = m_M3i64_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            i64 sum = 0;
            for (usize q = 0; q < 3; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M3i64_mulV(m_M3i64 x, m_V3i64 y))(m_V3i64)) {
    return m_V3i64_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z
    );
};

fn_((m_M3i64_col(m_M3i64 x, usize idx))(m_M3i64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M3i64_row(m_M3i64 x, usize idx))(m_M3i64_RowV)) {
    return m_V3i64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx])
    );
};

fn_((m_M3i64_transp(m_M3i64 x))(m_M3i64)) {
    return m_M3i64_ofRows(
        m_V3i64_of(x.s0_0, x.s1_0, x.s2_0),
        m_V3i64_of(x.s0_1, x.s1_1, x.s2_1),
        m_V3i64_of(x.s0_2, x.s1_2, x.s2_2)
    );
};

fn_((m_M3i64_det(m_M3i64 x))(i64)) {
    return x.s0_0 * (x.s1_1 * x.s2_2 - x.s1_2 * x.s2_1)
         - x.s0_1 * (x.s1_0 * x.s2_2 - x.s1_2 * x.s2_0)
         + x.s0_2 * (x.s1_0 * x.s2_1 - x.s1_1 * x.s2_0);
};

/*========== m_M4i64 Implementations ==========================================*/

#define __comp_const__m_M4i64_zero m_M4i64_ofCols_static( \
    m_V4i64_zero, \
    m_V4i64_zero, \
    m_V4i64_zero, \
    m_V4i64_zero \
)
#define __comp_const__m_M4i64_identity m_M4i64_ofCols_static( \
    m_V4i64_unit_x, \
    m_V4i64_unit_y, \
    m_V4i64_unit_z, \
    m_V4i64_unit_w \
)

#define ____m_M4i64_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4i64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4i64_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4i64){ \
    .cols = A_init({ \
        [0] = m_V4i64_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4i64_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4i64_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4i64_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4i64_ofCols(m_M4i64_ColV c0, m_M4i64_ColV c1, m_M4i64_ColV c2, m_M4i64_ColV c3))(m_M4i64)) {
    return m_M4i64_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4i64_ofRows(m_M4i64_RowV r0, m_M4i64_RowV r1, m_M4i64_RowV r2, m_M4i64_RowV r3))(m_M4i64)) {
    return m_M4i64_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4i64_splat(i64 scalar))(m_M4i64)) {
    let v = m_V4i64_splat(scalar);
    return m_M4i64_ofCols(v, v, v, v);
};
fn_((m_M4i64_diag(i64 d))(m_M4i64)) {
    return m_M4i64_ofRows(
        m_V4i64_of(d, 0, 0, 0),
        m_V4i64_of(0, d, 0, 0),
        m_V4i64_of(0, 0, d, 0),
        m_V4i64_of(0, 0, 0, d)
    );
};

fn_((m_M4i64_fromM3(m_M3i64 x))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_of(A_at((x.cols)[0])->x, A_at((x.cols)[0])->y, A_at((x.cols)[0])->z, 0),
        m_V4i64_of(A_at((x.cols)[1])->x, A_at((x.cols)[1])->y, A_at((x.cols)[1])->z, 0),
        m_V4i64_of(A_at((x.cols)[2])->x, A_at((x.cols)[2])->y, A_at((x.cols)[2])->z, 0),
        m_V4i64_of(0, 0, 0, 1)
    );
};

fn_((m_M4i64_neg(m_M4i64 x))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_neg(*A_at((x.cols)[0])),
        m_V4i64_neg(*A_at((x.cols)[1])),
        m_V4i64_neg(*A_at((x.cols)[2])),
        m_V4i64_neg(*A_at((x.cols)[3]))
    );
};

fn_((m_M4i64_add(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i64_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i64_sub(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i64_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i64_mul(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_mul(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i64_mul(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i64_mul(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i64_mul(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i64_div(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_div(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i64_div(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i64_div(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i64_div(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i64_mod(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4i64_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4i64_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4i64_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4i64_scal(m_M4i64 x, i64 scalar))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_scal(*A_at((x.cols)[0]), scalar),
        m_V4i64_scal(*A_at((x.cols)[1]), scalar),
        m_V4i64_scal(*A_at((x.cols)[2]), scalar),
        m_V4i64_scal(*A_at((x.cols)[3]), scalar)
    );
};
fn_((m_M4i64_scalInv(m_M4i64 x, i64 scalar))(m_M4i64)) {
    return m_M4i64_ofCols(
        m_V4i64_scalInv(*A_at((x.cols)[0]), scalar),
        m_V4i64_scalInv(*A_at((x.cols)[1]), scalar),
        m_V4i64_scalInv(*A_at((x.cols)[2]), scalar),
        m_V4i64_scalInv(*A_at((x.cols)[3]), scalar)
    );
};

fn_((m_M4i64_mulM(m_M4i64 lhs, m_M4i64 rhs))(m_M4i64)) {
    m_M4i64 result = m_M4i64_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            i64 sum = 0;
            for (usize q = 0; q < 4; ++q) {
                sum += *A_at((A_at((lhs.cols)[q])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[q]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4i64_mulV(m_M4i64 x, m_V4i64 y))(m_V4i64)) {
    return m_V4i64_of(
        x.s0_0 * y.x + x.s0_1 * y.y + x.s0_2 * y.z + x.s0_3 * y.w,
        x.s1_0 * y.x + x.s1_1 * y.y + x.s1_2 * y.z + x.s1_3 * y.w,
        x.s2_0 * y.x + x.s2_1 * y.y + x.s2_2 * y.z + x.s2_3 * y.w,
        x.s3_0 * y.x + x.s3_1 * y.y + x.s3_2 * y.z + x.s3_3 * y.w
    );
};

fn_((m_M4i64_col(m_M4i64 x, usize idx))(m_M4i64_ColV)) {
    return *A_at((x.cols)[idx]);
};
fn_((m_M4i64_row(m_M4i64 x, usize idx))(m_M4i64_RowV)) {
    return m_V4i64_of(
        *A_at((A_at((x.cols)[0])->s)[idx]),
        *A_at((A_at((x.cols)[1])->s)[idx]),
        *A_at((A_at((x.cols)[2])->s)[idx]),
        *A_at((A_at((x.cols)[3])->s)[idx])
    );
};

fn_((m_M4i64_transp(m_M4i64 x))(m_M4i64)) {
    return m_M4i64_ofRows(
        m_V4i64_of(x.s0_0, x.s1_0, x.s2_0, x.s3_0),
        m_V4i64_of(x.s0_1, x.s1_1, x.s2_1, x.s3_1),
        m_V4i64_of(x.s0_2, x.s1_2, x.s2_2, x.s3_2),
        m_V4i64_of(x.s0_3, x.s1_3, x.s2_3, x.s3_3)
    );
};

fn_((m_M4i64_det(m_M4i64 x))(i64)) {
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

/*========== m_M2isize Implementations ==========================================*/

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

fn_((m_M2isize_mod(m_M2isize lhs, m_M2isize rhs))(m_M2isize)) {
    return m_M2isize_ofCols(
        m_V2isize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2isize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
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

/*========== m_M3isize Implementations ==========================================*/

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

fn_((m_M3isize_mod(m_M3isize lhs, m_M3isize rhs))(m_M3isize)) {
    return m_M3isize_ofCols(
        m_V3isize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3isize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3isize_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
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

/*========== m_M4isize Implementations ==========================================*/

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

fn_((m_M4isize_mod(m_M4isize lhs, m_M4isize rhs))(m_M4isize)) {
    return m_M4isize_ofCols(
        m_V4isize_mod(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4isize_mod(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4isize_mod(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4isize_mod(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
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

/*========== M2f32 Implementations ==========================================*/

#define __comp_const__m_M2f32_zero m_M2f32_ofCols_static( \
    m_V2f32_zero, \
    m_V2f32_zero \
)
#define __comp_const__m_M2f32_identity m_M2f32_ofCols_static( \
    m_V2f32_unit_x, \
    m_V2f32_unit_y \
)

#define ____m_M2f32_ofCols_static(_c0, _c1...) l$((m_M2f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2f32_ofRows_static(_r0, _r1...) l$((m_M2f32){ \
    .cols = A_init({ \
        [0] = m_V2f32_of_static((_r0).x, (_r1).x), \
        [1] = m_V2f32_of_static((_r0).y, (_r1).y), \
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

fn_((m_M2f32_transp(m_M2f32 m))(m_M2f32)) {
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
        m_V2f32_of(sc.x, -sc.y),
        m_V2f32_of(sc.y, sc.x)
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

#define ____m_M3f32_ofCols_static(_c0, _c1, _c2...) l$((m_M3f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3f32_ofRows_static(_r0, _r1, _r2...) l$((m_M3f32){ \
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

fn_((m_M3f32_transp(m_M3f32 m))(m_M3f32)) {
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

fn_((m_M3f32_transl2D(m_V2f32 t))(m_M3f32)) {
    return m_M3f32_ofRows(
        m_V3f32_of(1.0f, 0.0f, t.x),
        m_V3f32_of(0.0f, 1.0f, t.y),
        m_V3f32_of(0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M3f32_rotate2D(f32 angle))(m_M3f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M3f32_ofRows(
        m_V3f32_of(sc.x, -sc.y, 0.0f),
        m_V3f32_of(sc.y, sc.x, 0.0f),
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

#define ____m_M4f32_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4f32){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4f32_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4f32){ \
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

fn_((m_M4f32_transp(m_M4f32 m))(m_M4f32)) {
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

fn_((m_M4f32_transl(m_V3f32 t))(m_M4f32)) {
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
        m_V4f32_of(0.0f, sc.x, -sc.y, 0.0f),
        m_V4f32_of(0.0f, sc.y, sc.x, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateY(f32 angle))(m_M4f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M4f32_ofRows(
        m_V4f32_of(sc.x, 0.0f, sc.y, 0.0f),
        m_V4f32_of(0.0f, 1.0f, 0.0f, 0.0f),
        m_V4f32_of(-sc.y, 0.0f, sc.x, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateZ(f32 angle))(m_M4f32)) {
    let sc = m_V2f32_sincos(angle);
    return m_M4f32_ofRows(
        m_V4f32_of(sc.x, -sc.y, 0.0f, 0.0f),
        m_V4f32_of(sc.y, sc.x, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_rotateAxis(m_V3f32 axis, f32 angle))(m_M4f32)) {
    let n = m_V3f32_norm(axis);
    let sc = m_V2f32_sincos(angle);
    let c = sc.x;
    let s = sc.y;
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

fn_((m_M4f32_lookAtRH(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32)) {
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
fn_((m_M4f32_lookAtLH(m_V3f32 eye, m_V3f32 target, m_V3f32 up))(m_M4f32)) {
    let f = m_V3f32_norm(m_V3f32_sub(target, eye));
    let s = m_V3f32_norm(m_V3f32_cross(up, f));
    let u = m_V3f32_cross(f, s);

    return m_M4f32_ofRows(
        m_V4f32_of(s.x, s.y, s.z, -m_V3f32_dot(s, eye)),
        m_V4f32_of(u.x, u.y, u.z, -m_V3f32_dot(u, eye)),
        m_V4f32_of(f.x, f.y, f.z, -m_V3f32_dot(f, eye)),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_perspectRHNO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32)) {
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
fn_((m_M4f32_perspectRHZO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;
    let nf = 1.0f / (proximal - distal);

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, distal * nf, distal * proximal * nf),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectLHNO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, (distal + proximal) * fn, -2.0f * distal * proximal * fn),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectLHZO(f32 fov_y, f32 aspect, f32 proximal, f32 distal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, distal * fn, -distal * proximal * fn),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectInfFarRHNO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, -1.0f, -2.0f * proximal),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectInfFarRHZO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, -1.0f, -proximal),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectInfFarLHNO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 1.0f, -2.0f * proximal),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};
fn_((m_M4f32_perspectInfFarLHZO(f32 fov_y, f32 aspect, f32 proximal))(m_M4f32)) {
    let tan_half_fov = flt_tan(fov_y * 0.5f);
    let f = 1.0f / tan_half_fov;

    return m_M4f32_ofRows(
        m_V4f32_of(f / aspect, 0.0f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, f, 0.0f, 0.0f),
        m_V4f32_of(0.0f, 0.0f, 1.0f, -proximal),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};
fn_((m_M4f32_orthoRHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl),
        m_V4f32_of(0.0f, 2.0f * tb, 0.0f, -(top + bottom) * tb),
        m_V4f32_of(0.0f, 0.0f, -2.0f * fn, -(distal + proximal) * fn),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_orthoRHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let nf = 1.0f / (proximal - distal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl),
        m_V4f32_of(0.0f, 2.0f * tb, 0.0f, -(top + bottom) * tb),
        m_V4f32_of(0.0f, 0.0f, nf, proximal * nf),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_orthoLHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl),
        m_V4f32_of(0.0f, 2.0f * tb, 0.0f, -(top + bottom) * tb),
        m_V4f32_of(0.0f, 0.0f, 2.0f * fn, -(distal + proximal) * fn),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_orthoLHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * rl, 0.0f, 0.0f, -(right + left) * rl),
        m_V4f32_of(0.0f, 2.0f * tb, 0.0f, -(top + bottom) * tb),
        m_V4f32_of(0.0f, 0.0f, fn, -proximal * fn),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};
fn_((m_M4f32_frustumRHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
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
fn_((m_M4f32_frustumRHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let nf = 1.0f / (proximal - distal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * proximal * rl, 0.0f, (right + left) * rl, 0.0f),
        m_V4f32_of(0.0f, 2.0f * proximal * tb, (top + bottom) * tb, 0.0f),
        m_V4f32_of(0.0f, 0.0f, distal * nf, distal * proximal * nf),
        m_V4f32_of(0.0f, 0.0f, -1.0f, 0.0f)
    );
};
fn_((m_M4f32_frustumLHNO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * proximal * rl, 0.0f, -(right + left) * rl, 0.0f),
        m_V4f32_of(0.0f, 2.0f * proximal * tb, -(top + bottom) * tb, 0.0f),
        m_V4f32_of(0.0f, 0.0f, (distal + proximal) * fn, -2.0f * distal * proximal * fn),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};
fn_((m_M4f32_frustumLHZO(f32 left, f32 right, f32 bottom, f32 top, f32 proximal, f32 distal))(m_M4f32)) {
    let rl = 1.0f / (right - left);
    let tb = 1.0f / (top - bottom);
    let fn = 1.0f / (distal - proximal);

    return m_M4f32_ofRows(
        m_V4f32_of(2.0f * proximal * rl, 0.0f, -(right + left) * rl, 0.0f),
        m_V4f32_of(0.0f, 2.0f * proximal * tb, -(top + bottom) * tb, 0.0f),
        m_V4f32_of(0.0f, 0.0f, distal * fn, -distal * proximal * fn),
        m_V4f32_of(0.0f, 0.0f, 1.0f, 0.0f)
    );
};

/*========== M2f64 Implementations ==========================================*/

#define __comp_const__m_M2f64_zero m_M2f64_ofCols_static( \
    m_V2f64_zero, \
    m_V2f64_zero \
)
#define __comp_const__m_M2f64_identity m_M2f64_ofCols_static( \
    m_V2f64_unit_x, \
    m_V2f64_unit_y \
)

#define ____m_M2f64_ofCols_static(_c0, _c1...) l$((m_M2f64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1) }), \
})
#define ____m_M2f64_ofRows_static(_r0, _r1...) l$((m_M2f64){ \
    .cols = A_init({ \
        [0] = m_V2f64_of_static((_r0).x, (_r1).x), \
        [1] = m_V2f64_of_static((_r0).y, (_r1).y), \
    }), \
})
fn_((m_M2f64_ofCols(m_M2f64_ColV c0, m_M2f64_ColV c1))(m_M2f64)) {
    return m_M2f64_ofCols_static(c0, c1);
};
fn_((m_M2f64_ofRows(m_M2f64_RowV r0, m_M2f64_RowV r1))(m_M2f64)) {
    return m_M2f64_ofRows_static(r0, r1);
};
fn_((m_M2f64_splat(f64 scalar))(m_M2f64)) {
    let v = m_V2f64_splat(scalar);
    return m_M2f64_ofCols(v, v);
};
fn_((m_M2f64_diag(f64 d))(m_M2f64)) {
    return m_M2f64_ofRows(
        m_V2f64_of(d, 0.0),
        m_V2f64_of(0.0, d)
    );
};

fn_((m_M2f64_neg(m_M2f64 m))(m_M2f64)) {
    return m_M2f64_ofCols(
        m_V2f64_neg(*A_at((m.cols)[0])),
        m_V2f64_neg(*A_at((m.cols)[1]))
    );
};
fn_((m_M2f64_add(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64)) {
    return m_M2f64_ofCols(
        m_V2f64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2f64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};
fn_((m_M2f64_sub(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64)) {
    return m_M2f64_ofCols(
        m_V2f64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V2f64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1]))
    );
};

fn_((m_M2f64_scal(m_M2f64 m, f64 scalar))(m_M2f64)) {
    return m_M2f64_ofCols(
        m_V2f64_scal(*A_at((m.cols)[0]), scalar),
        m_V2f64_scal(*A_at((m.cols)[1]), scalar)
    );
};
fn_((m_M2f64_scalInv(m_M2f64 m, f64 scalar))(m_M2f64)) {
    return m_M2f64_scal(m, 1.0 / scalar);
};

fn_((m_M2f64_mulM(m_M2f64 lhs, m_M2f64 rhs))(m_M2f64)) {
    return m_M2f64_ofCols(
        m_V2f64_of(
            lhs.s0_0 * rhs.s0_0 + lhs.s0_1 * rhs.s1_0,
            lhs.s1_0 * rhs.s0_0 + lhs.s1_1 * rhs.s1_0
        ),
        m_V2f64_of(
            lhs.s0_0 * rhs.s0_1 + lhs.s0_1 * rhs.s1_1,
            lhs.s1_0 * rhs.s0_1 + lhs.s1_1 * rhs.s1_1
        )
    );
};
fn_((m_M2f64_mulV(m_M2f64 m, m_V2f64 v))(m_V2f64)) {
    return m_V2f64_of(
        m.s0_0 * v.x + m.s0_1 * v.y,
        m.s1_0 * v.x + m.s1_1 * v.y
    );
};

fn_((m_M2f64_col(m_M2f64 m, usize idx))(m_V2f64)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M2f64_row(m_M2f64 m, usize idx))(m_V2f64)) {
    return m_V2f64_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx])
    );
};

fn_((m_M2f64_transp(m_M2f64 m))(m_M2f64)) {
    return m_M2f64_ofRows(
        m_V2f64_of(m.s0_0, m.s1_0),
        m_V2f64_of(m.s0_1, m.s1_1)
    );
};
fn_((m_M2f64_det(m_M2f64 m))(f64)) {
    return m.s0_0 * m.s1_1 - m.s0_1 * m.s1_0;
};
fn_((m_M2f64_inv(m_M2f64 m))(m_M2f64)) {
    let det = m_M2f64_det(m);
    let inv_det = 1.0 / det;
    return m_M2f64_ofRows(
        m_V2f64_of(m.s1_1 * inv_det, -m.s0_1 * inv_det),
        m_V2f64_of(-m.s1_0 * inv_det, m.s0_0 * inv_det)
    );
};

fn_((m_M2f64_rotate(f64 angle))(m_M2f64)) {
    let sc = m_V2f64_sincos(angle);
    return m_M2f64_ofRows(
        m_V2f64_of(sc.x, -sc.y),
        m_V2f64_of(sc.y, sc.x)
    );
};
fn_((m_M2f64_scale(m_V2f64 s))(m_M2f64)) {
    return m_M2f64_ofRows(
        m_V2f64_of(s.x, 0.0),
        m_V2f64_of(0.0, s.y)
    );
};
fn_((m_M2f64_scaleUniform(f64 s))(m_M2f64)) {
    return m_M2f64_diag(s);
};

/*========== M3f64 Implementations ==========================================*/

#define __comp_const__m_M3f64_zero m_M3f64_ofCols_static( \
    m_V3f64_zero, \
    m_V3f64_zero, \
    m_V3f64_zero \
)
#define __comp_const__m_M3f64_identity m_M3f64_ofCols_static( \
    m_V3f64_unit_x, \
    m_V3f64_unit_y, \
    m_V3f64_unit_z \
)

#define ____m_M3f64_ofCols_static(_c0, _c1, _c2...) l$((m_M3f64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2) }), \
})
#define ____m_M3f64_ofRows_static(_r0, _r1, _r2...) l$((m_M3f64){ \
    .cols = A_init({ \
        [0] = m_V3f64_of_static((_r0).x, (_r1).x, (_r2).x), \
        [1] = m_V3f64_of_static((_r0).y, (_r1).y, (_r2).y), \
        [2] = m_V3f64_of_static((_r0).z, (_r1).z, (_r2).z), \
    }), \
})
fn_((m_M3f64_ofCols(m_M3f64_ColV c0, m_M3f64_ColV c1, m_M3f64_ColV c2))(m_M3f64)) {
    return m_M3f64_ofCols_static(c0, c1, c2);
};
fn_((m_M3f64_ofRows(m_M3f64_RowV r0, m_M3f64_RowV r1, m_M3f64_RowV r2))(m_M3f64)) {
    return m_M3f64_ofRows_static(r0, r1, r2);
};
fn_((m_M3f64_splat(f64 scalar))(m_M3f64)) {
    let v = m_V3f64_splat(scalar);
    return m_M3f64_ofCols(v, v, v);
};
fn_((m_M3f64_diag(f64 d))(m_M3f64)) {
    return m_M3f64_ofRows(
        m_V3f64_of(d, 0.0, 0.0),
        m_V3f64_of(0.0, d, 0.0),
        m_V3f64_of(0.0, 0.0, d)
    );
};
fn_((m_M3f64_fromM4(m_M4f64 m))(m_M3f64)) {
    return m_M3f64_ofCols(
        m_V3f64_from4(*A_at((m.cols)[0])),
        m_V3f64_from4(*A_at((m.cols)[1])),
        m_V3f64_from4(*A_at((m.cols)[2]))
    );
};

fn_((m_M3f64_neg(m_M3f64 m))(m_M3f64)) {
    return m_M3f64_ofCols(
        m_V3f64_neg(*A_at((m.cols)[0])),
        m_V3f64_neg(*A_at((m.cols)[1])),
        m_V3f64_neg(*A_at((m.cols)[2]))
    );
};
fn_((m_M3f64_add(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64)) {
    return m_M3f64_ofCols(
        m_V3f64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3f64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3f64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};
fn_((m_M3f64_sub(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64)) {
    return m_M3f64_ofCols(
        m_V3f64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V3f64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V3f64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2]))
    );
};

fn_((m_M3f64_scal(m_M3f64 m, f64 scalar))(m_M3f64)) {
    return m_M3f64_ofCols(
        m_V3f64_scal(*A_at((m.cols)[0]), scalar),
        m_V3f64_scal(*A_at((m.cols)[1]), scalar),
        m_V3f64_scal(*A_at((m.cols)[2]), scalar)
    );
};
fn_((m_M3f64_scalInv(m_M3f64 m, f64 scalar))(m_M3f64)) {
    return m_M3f64_scal(m, 1.0 / scalar);
};

fn_((m_M3f64_mulM(m_M3f64 lhs, m_M3f64 rhs))(m_M3f64)) {
    m_M3f64 result = m_M3f64_zero;
    for (usize c = 0; c < 3; ++c) {
        for (usize r = 0; r < 3; ++r) {
            f64 sum = 0.0;
            for (usize k = 0; k < 3; ++k) {
                sum += *A_at((A_at((lhs.cols)[k])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[k]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};
fn_((m_M3f64_mulV(m_M3f64 m, m_V3f64 v))(m_V3f64)) {
    return m_V3f64_of(
        m.s0_0 * v.x + m.s0_1 * v.y + m.s0_2 * v.z,
        m.s1_0 * v.x + m.s1_1 * v.y + m.s1_2 * v.z,
        m.s2_0 * v.x + m.s2_1 * v.y + m.s2_2 * v.z
    );
};

fn_((m_M3f64_col(m_M3f64 m, usize idx))(m_V3f64)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M3f64_row(m_M3f64 m, usize idx))(m_V3f64)) {
    return m_V3f64_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx]),
        *A_at((A_at((m.cols)[2])->s)[idx])
    );
};

fn_((m_M3f64_transp(m_M3f64 m))(m_M3f64)) {
    return m_M3f64_ofRows(
        m_V3f64_of(m.s0_0, m.s1_0, m.s2_0),
        m_V3f64_of(m.s0_1, m.s1_1, m.s2_1),
        m_V3f64_of(m.s0_2, m.s1_2, m.s2_2)
    );
};
fn_((m_M3f64_det(m_M3f64 m))(f64)) {
    return m.s0_0 * (m.s1_1 * m.s2_2 - m.s1_2 * m.s2_1)
         - m.s0_1 * (m.s1_0 * m.s2_2 - m.s1_2 * m.s2_0)
         + m.s0_2 * (m.s1_0 * m.s2_1 - m.s1_1 * m.s2_0);
};
fn_((m_M3f64_inv(m_M3f64 m))(m_M3f64)) {
    let det = m_M3f64_det(m);
    let inv_det = 1.0 / det;
    return m_M3f64_ofRows(
        m_V3f64_of(
            (m.s1_1 * m.s2_2 - m.s2_1 * m.s1_2) * inv_det,
            (m.s2_0 * m.s1_2 - m.s1_0 * m.s2_2) * inv_det,
            (m.s1_0 * m.s2_1 - m.s2_0 * m.s1_1) * inv_det
        ),
        m_V3f64_of(
            (m.s2_1 * m.s0_2 - m.s0_1 * m.s2_2) * inv_det,
            (m.s0_0 * m.s2_2 - m.s2_0 * m.s0_2) * inv_det,
            (m.s2_0 * m.s0_1 - m.s0_0 * m.s2_1) * inv_det
        ),
        m_V3f64_of(
            (m.s0_1 * m.s1_2 - m.s1_1 * m.s0_2) * inv_det,
            (m.s1_0 * m.s0_2 - m.s0_0 * m.s1_2) * inv_det,
            (m.s0_0 * m.s1_1 - m.s1_0 * m.s0_1) * inv_det
        )
    );
};

fn_((m_M3f64_transl2D(m_V2f64 t))(m_M3f64)) {
    return m_M3f64_ofRows(
        m_V3f64_of(1.0, 0.0, t.x),
        m_V3f64_of(0.0, 1.0, t.y),
        m_V3f64_of(0.0, 0.0, 1.0)
    );
};
fn_((m_M3f64_rotate2D(f64 angle))(m_M3f64)) {
    let sc = m_V2f64_sincos(angle);
    return m_M3f64_ofRows(
        m_V3f64_of(sc.x, -sc.y, 0.0),
        m_V3f64_of(sc.y, sc.x, 0.0),
        m_V3f64_of(0.0, 0.0, 1.0)
    );
};
fn_((m_M3f64_scale2D(m_V2f64 s))(m_M3f64)) {
    return m_M3f64_ofRows(
        m_V3f64_of(s.x, 0.0, 0.0),
        m_V3f64_of(0.0, s.y, 0.0),
        m_V3f64_of(0.0, 0.0, 1.0)
    );
};
fn_((m_M3f64_scaleUniform2D(f64 s))(m_M3f64)) {
    return m_M3f64_scale2D(m_V2f64_splat(s));
};

/*========== M4f64 Implementations ==========================================*/

#define __comp_const__m_M4f64_zero m_M4f64_ofCols_static( \
    m_V4f64_zero, \
    m_V4f64_zero, \
    m_V4f64_zero, \
    m_V4f64_zero \
)
#define __comp_const__m_M4f64_identity m_M4f64_ofCols_static( \
    m_V4f64_unit_x, \
    m_V4f64_unit_y, \
    m_V4f64_unit_z, \
    m_V4f64_unit_w \
)

#define ____m_M4f64_ofCols_static(_c0, _c1, _c2, _c3...) l$((m_M4f64){ \
    .cols = A_init({ [0] = (_c0), [1] = (_c1), [2] = (_c2), [3] = (_c3) }), \
})
#define ____m_M4f64_ofRows_static(_r0, _r1, _r2, _r3...) l$((m_M4f64){ \
    .cols = A_init({ \
        [0] = m_V4f64_of_static((_r0).x, (_r1).x, (_r2).x, (_r3).x), \
        [1] = m_V4f64_of_static((_r0).y, (_r1).y, (_r2).y, (_r3).y), \
        [2] = m_V4f64_of_static((_r0).z, (_r1).z, (_r2).z, (_r3).z), \
        [3] = m_V4f64_of_static((_r0).w, (_r1).w, (_r2).w, (_r3).w), \
    }), \
})
fn_((m_M4f64_ofCols(m_M4f64_ColV c0, m_M4f64_ColV c1, m_M4f64_ColV c2, m_M4f64_ColV c3))(m_M4f64)) {
    return m_M4f64_ofCols_static(c0, c1, c2, c3);
};
fn_((m_M4f64_ofRows(m_M4f64_RowV r0, m_M4f64_RowV r1, m_M4f64_RowV r2, m_M4f64_RowV r3))(m_M4f64)) {
    return m_M4f64_ofRows_static(r0, r1, r2, r3);
};
fn_((m_M4f64_splat(f64 scalar))(m_M4f64)) {
    let v = m_V4f64_splat(scalar);
    return m_M4f64_ofCols(v, v, v, v);
};
fn_((m_M4f64_diag(f64 d))(m_M4f64)) {
    return m_M4f64_ofRows(
        m_V4f64_of(d, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, d, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, d, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, d)
    );
};
fn_((m_M4f64_fromM3(m_M3f64 m))(m_M4f64)) {
    return m_M4f64_ofCols(
        m_V4f64_of(A_at((m.cols)[0])->x, A_at((m.cols)[0])->y, A_at((m.cols)[0])->z, 0.0),
        m_V4f64_of(A_at((m.cols)[1])->x, A_at((m.cols)[1])->y, A_at((m.cols)[1])->z, 0.0),
        m_V4f64_of(A_at((m.cols)[2])->x, A_at((m.cols)[2])->y, A_at((m.cols)[2])->z, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};

fn_((m_M4f64_neg(m_M4f64 m))(m_M4f64)) {
    return m_M4f64_ofCols(
        m_V4f64_neg(*A_at((m.cols)[0])),
        m_V4f64_neg(*A_at((m.cols)[1])),
        m_V4f64_neg(*A_at((m.cols)[2])),
        m_V4f64_neg(*A_at((m.cols)[3]))
    );
};
fn_((m_M4f64_add(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64)) {
    return m_M4f64_ofCols(
        m_V4f64_add(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4f64_add(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4f64_add(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4f64_add(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};
fn_((m_M4f64_sub(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64)) {
    return m_M4f64_ofCols(
        m_V4f64_sub(*A_at((lhs.cols)[0]), *A_at((rhs.cols)[0])),
        m_V4f64_sub(*A_at((lhs.cols)[1]), *A_at((rhs.cols)[1])),
        m_V4f64_sub(*A_at((lhs.cols)[2]), *A_at((rhs.cols)[2])),
        m_V4f64_sub(*A_at((lhs.cols)[3]), *A_at((rhs.cols)[3]))
    );
};

fn_((m_M4f64_scal(m_M4f64 m, f64 scalar))(m_M4f64)) {
    return m_M4f64_ofCols(
        m_V4f64_scal(*A_at((m.cols)[0]), scalar),
        m_V4f64_scal(*A_at((m.cols)[1]), scalar),
        m_V4f64_scal(*A_at((m.cols)[2]), scalar),
        m_V4f64_scal(*A_at((m.cols)[3]), scalar)
    );
};
fn_((m_M4f64_scalInv(m_M4f64 m, f64 scalar))(m_M4f64)) {
    return m_M4f64_scal(m, 1.0 / scalar);
};

fn_((m_M4f64_mulM(m_M4f64 lhs, m_M4f64 rhs))(m_M4f64)) {
    m_M4f64 result = m_M4f64_zero;
    for (usize c = 0; c < 4; ++c) {
        for (usize r = 0; r < 4; ++r) {
            f64 sum = 0.0;
            for (usize k = 0; k < 4; ++k) {
                sum += *A_at((A_at((lhs.cols)[k])->s)[r]) * *A_at((A_at((rhs.cols)[c])->s)[k]);
            }
            *A_at((A_at((result.cols)[c])->s)[r]) = sum;
        }
    }
    return result;
};

fn_((m_M4f64_mulV(m_M4f64 m, m_V4f64 v))(m_V4f64)) {
    return m_V4f64_of(
        m.s0_0 * v.x + m.s0_1 * v.y + m.s0_2 * v.z + m.s0_3 * v.w,
        m.s1_0 * v.x + m.s1_1 * v.y + m.s1_2 * v.z + m.s1_3 * v.w,
        m.s2_0 * v.x + m.s2_1 * v.y + m.s2_2 * v.z + m.s2_3 * v.w,
        m.s3_0 * v.x + m.s3_1 * v.y + m.s3_2 * v.z + m.s3_3 * v.w
    );
};
fn_((m_M4f64_mulV3(m_M4f64 m, m_V3f64 v, f64 w))(m_V3f64)) {
    let v4 = m_V4f64_of(v.x, v.y, v.z, w);
    let result = m_M4f64_mulV(m, v4);
    if (w != 0.0) {
        return m_V3f64_of(result.x / result.w, result.y / result.w, result.z / result.w);
    }
    return m_V3f64_of(result.x, result.y, result.z);
};

fn_((m_M4f64_col(m_M4f64 m, usize idx))(m_V4f64)) {
    return *A_at((m.cols)[idx]);
};
fn_((m_M4f64_row(m_M4f64 m, usize idx))(m_V4f64)) {
    return m_V4f64_of(
        *A_at((A_at((m.cols)[0])->s)[idx]),
        *A_at((A_at((m.cols)[1])->s)[idx]),
        *A_at((A_at((m.cols)[2])->s)[idx]),
        *A_at((A_at((m.cols)[3])->s)[idx])
    );
};

fn_((m_M4f64_transp(m_M4f64 m))(m_M4f64)) {
    return m_M4f64_ofRows(
        m_V4f64_of(m.s0_0, m.s1_0, m.s2_0, m.s3_0),
        m_V4f64_of(m.s0_1, m.s1_1, m.s2_1, m.s3_1),
        m_V4f64_of(m.s0_2, m.s1_2, m.s2_2, m.s3_2),
        m_V4f64_of(m.s0_3, m.s1_3, m.s2_3, m.s3_3)
    );
};
fn_((m_M4f64_det(m_M4f64 m))(f64)) {
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
fn_((m_M4f64_inv(m_M4f64 m))(m_M4f64)) {
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
    let inv_det = 1.0 / det;

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

    return m_M4f64_ofRows(
        m_V4f64_of(
            a00 * inv_det,
            -(b * kp_lo - c * jp_ln + d * jo_kn) * inv_det,
            (b * gp_ho - c * fp_hn + d * fo_gn) * inv_det,
            -(b * gl_hk - c * fl_hj + d * fk_gj) * inv_det
        ),
        m_V4f64_of(
            a01 * inv_det,
            (a * kp_lo - c * ip_lm + d * io_km) * inv_det,
            -(a * gp_ho - c * ep_hm + d * eo_gm) * inv_det,
            (a * gl_hk - c * el_hi + d * ek_gi) * inv_det
        ),
        m_V4f64_of(
            a02 * inv_det,
            -(a * jp_ln - b * ip_lm + d * in_jm) * inv_det,
            (a * fp_hn - b * ep_hm + d * en_fm) * inv_det,
            -(a * fl_hj - b * el_hi + d * ej_fi) * inv_det
        ),
        m_V4f64_of(
            a03 * inv_det,
            (a * jo_kn - b * io_km + c * in_jm) * inv_det,
            -(a * fo_gn - b * eo_gm + c * en_fm) * inv_det,
            (a * fk_gj - b * ek_gi + c * ej_fi) * inv_det
        )
    );
};

fn_((m_M4f64_transl(m_V3f64 t))(m_M4f64)) {
    return m_M4f64_ofRows(
        m_V4f64_of(1.0, 0.0, 0.0, t.x),
        m_V4f64_of(0.0, 1.0, 0.0, t.y),
        m_V4f64_of(0.0, 0.0, 1.0, t.z),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_rotateX(f64 angle))(m_M4f64)) {
    let sc = m_V2f64_sincos(angle);
    return m_M4f64_ofRows(
        m_V4f64_of(1.0, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, sc.x, -sc.y, 0.0),
        m_V4f64_of(0.0, sc.y, sc.x, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_rotateY(f64 angle))(m_M4f64)) {
    let sc = m_V2f64_sincos(angle);
    return m_M4f64_ofRows(
        m_V4f64_of(sc.x, 0.0, sc.y, 0.0),
        m_V4f64_of(0.0, 1.0, 0.0, 0.0),
        m_V4f64_of(-sc.y, 0.0, sc.x, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_rotateZ(f64 angle))(m_M4f64)) {
    let sc = m_V2f64_sincos(angle);
    return m_M4f64_ofRows(
        m_V4f64_of(sc.x, -sc.y, 0.0, 0.0),
        m_V4f64_of(sc.y, sc.x, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_rotateAxis(m_V3f64 axis, f64 angle))(m_M4f64)) {
    let n = m_V3f64_norm(axis);
    let sc = m_V2f64_sincos(angle);
    let c = sc.x;
    let s = sc.y;
    let t = 1.0 - c;

    return m_M4f64_ofRows(
        m_V4f64_of(
            t * n.x * n.x + c,
            t * n.x * n.y - s * n.z,
            t * n.x * n.z + s * n.y,
            0.0
        ),
        m_V4f64_of(
            t * n.x * n.y + s * n.z,
            t * n.y * n.y + c,
            t * n.y * n.z - s * n.x,
            0.0
        ),
        m_V4f64_of(
            t * n.x * n.z - s * n.y,
            t * n.y * n.z + s * n.x,
            t * n.z * n.z + c,
            0.0
        ),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_scale(m_V3f64 s))(m_M4f64)) {
    return m_M4f64_ofRows(
        m_V4f64_of(s.x, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, s.y, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, s.z, 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_scaleUniform(f64 s))(m_M4f64)) {
    return m_M4f64_scale(m_V3f64_splat(s));
};

fn_((m_M4f64_lookAtRH(m_V3f64 eye, m_V3f64 target, m_V3f64 up))(m_M4f64)) {
    let f = m_V3f64_norm(m_V3f64_sub(target, eye));
    let s = m_V3f64_norm(m_V3f64_cross(f, up));
    let u = m_V3f64_cross(s, f);

    return m_M4f64_ofRows(
        m_V4f64_of(s.x, s.y, s.z, -m_V3f64_dot(s, eye)),
        m_V4f64_of(u.x, u.y, u.z, -m_V3f64_dot(u, eye)),
        m_V4f64_of(-f.x, -f.y, -f.z, m_V3f64_dot(f, eye)),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_lookAtLH(m_V3f64 eye, m_V3f64 target, m_V3f64 up))(m_M4f64)) {
    let f = m_V3f64_norm(m_V3f64_sub(target, eye));
    let s = m_V3f64_norm(m_V3f64_cross(up, f));
    let u = m_V3f64_cross(f, s);

    return m_M4f64_ofRows(
        m_V4f64_of(s.x, s.y, s.z, -m_V3f64_dot(s, eye)),
        m_V4f64_of(u.x, u.y, u.z, -m_V3f64_dot(u, eye)),
        m_V4f64_of(f.x, f.y, f.z, -m_V3f64_dot(f, eye)),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_perspectRHNO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;
    let nf = 1.0 / (proximal - distal);

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, (distal + proximal) * nf, 2.0 * distal * proximal * nf),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_perspectRHZO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;
    let nf = 1.0 / (proximal - distal);

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, distal * nf, distal * proximal * nf),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_perspectLHNO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, (distal + proximal) * fn, -2.0 * distal * proximal * fn),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};
fn_((m_M4f64_perspectLHZO(f64 fov_y, f64 aspect, f64 proximal, f64 distal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, distal * fn, -distal * proximal * fn),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};
fn_((m_M4f64_perspectInfFarRHNO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, -1.0, -2.0 * proximal),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_perspectInfFarRHZO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, -1.0, -proximal),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_perspectInfFarLHNO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, 1.0, -2.0 * proximal),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};
fn_((m_M4f64_perspectInfFarLHZO(f64 fov_y, f64 aspect, f64 proximal))(m_M4f64)) {
    let tan_half_fov = flt_tan(fov_y * 0.5);
    let f = 1.0 / tan_half_fov;

    return m_M4f64_ofRows(
        m_V4f64_of(f / aspect, 0.0, 0.0, 0.0),
        m_V4f64_of(0.0, f, 0.0, 0.0),
        m_V4f64_of(0.0, 0.0, 1.0, -proximal),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};
fn_((m_M4f64_orthoRHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * rl, 0.0, 0.0, -(right + left) * rl),
        m_V4f64_of(0.0, 2.0 * tb, 0.0, -(top + bottom) * tb),
        m_V4f64_of(0.0, 0.0, -2.0 * fn, -(distal + proximal) * fn),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_orthoRHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let nf = 1.0 / (proximal - distal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * rl, 0.0, 0.0, -(right + left) * rl),
        m_V4f64_of(0.0, 2.0 * tb, 0.0, -(top + bottom) * tb),
        m_V4f64_of(0.0, 0.0, nf, proximal * nf),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_orthoLHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * rl, 0.0, 0.0, -(right + left) * rl),
        m_V4f64_of(0.0, 2.0 * tb, 0.0, -(top + bottom) * tb),
        m_V4f64_of(0.0, 0.0, 2.0 * fn, -(distal + proximal) * fn),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_orthoLHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * rl, 0.0, 0.0, -(right + left) * rl),
        m_V4f64_of(0.0, 2.0 * tb, 0.0, -(top + bottom) * tb),
        m_V4f64_of(0.0, 0.0, fn, -proximal * fn),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};
fn_((m_M4f64_frustumRHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let nf = 1.0 / (proximal - distal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * proximal * rl, 0.0, (right + left) * rl, 0.0),
        m_V4f64_of(0.0, 2.0 * proximal * tb, (top + bottom) * tb, 0.0),
        m_V4f64_of(0.0, 0.0, (distal + proximal) * nf, 2.0 * distal * proximal * nf),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_frustumRHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let nf = 1.0 / (proximal - distal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * proximal * rl, 0.0, (right + left) * rl, 0.0),
        m_V4f64_of(0.0, 2.0 * proximal * tb, (top + bottom) * tb, 0.0),
        m_V4f64_of(0.0, 0.0, distal * nf, distal * proximal * nf),
        m_V4f64_of(0.0, 0.0, -1.0, 0.0)
    );
};
fn_((m_M4f64_frustumLHNO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * proximal * rl, 0.0, -(right + left) * rl, 0.0),
        m_V4f64_of(0.0, 2.0 * proximal * tb, -(top + bottom) * tb, 0.0),
        m_V4f64_of(0.0, 0.0, (distal + proximal) * fn, -2.0 * distal * proximal * fn),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};
fn_((m_M4f64_frustumLHZO(f64 left, f64 right, f64 bottom, f64 top, f64 proximal, f64 distal))(m_M4f64)) {
    let rl = 1.0 / (right - left);
    let tb = 1.0 / (top - bottom);
    let fn = 1.0 / (distal - proximal);

    return m_M4f64_ofRows(
        m_V4f64_of(2.0 * proximal * rl, 0.0, -(right + left) * rl, 0.0),
        m_V4f64_of(0.0, 2.0 * proximal * tb, -(top + bottom) * tb, 0.0),
        m_V4f64_of(0.0, 0.0, distal * fn, -distal * proximal * fn),
        m_V4f64_of(0.0, 0.0, 1.0, 0.0)
    );
};

#endif /* math_mat__included */
