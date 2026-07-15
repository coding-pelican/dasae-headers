#pragma once
#ifndef m_math_linalg_Q_Quat_f64__included
#define m_math_linalg_Q_Quat_f64__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../V-Vec/f64.h"
#include "../M-Mat/f64.h"

/*========== Macros and Declarations ========================================*/

/* Constants */

#define m_Q4f64_zero __comp_const__m_Q4f64_zero
#define m_Q4f64_identity __comp_const__m_Q4f64_identity

/* Creation Functions */

#define m_Q4f64_of_static(_x, _y, _z, _w...) ____m_Q4f64_of_static(_x, _y, _z, _w)
$attr($inline_always)
$static fn_((m_Q4f64_of(f64 x, f64 y, f64 z, f64 w))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_splat(f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromV3(m_V3f64 v, f64 w))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromV4(m_V4f64 v))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromTo(m_V3f64 from, m_V3f64 to))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromEuler(f64 pitch, f64 yaw, f64 roll))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromAxisAngle(m_V3f64 axis, f64 angle))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromM3(m_M3f64 m))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_fromM4(m_M4f64 m))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_lookRotation(m_V3f64 forward, m_V3f64 up))(m_Q4f64));

/* Component-wise Arithmetic */

$attr($inline_always)
$static fn_((m_Q4f64_neg(m_Q4f64 q))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_add(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_sub(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_mul(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_div(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));

/* Quaternion Arithmetic (Hamilton Product) */

$attr($inline_always)
$static fn_((m_Q4f64_mulQ(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_divQ(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64));

/* Scalar Operations */

$attr($inline_always)
$static fn_((m_Q4f64_scal(m_Q4f64 q, f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_scalInv(m_Q4f64 q, f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_addS(m_Q4f64 q, f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_subS(m_Q4f64 q, f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_mulS(m_Q4f64 q, f64 scalar))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_divS(m_Q4f64 q, f64 scalar))(m_Q4f64));

/* Quaternion Properties */

$attr($inline_always)
$static fn_((m_Q4f64_conj(m_Q4f64 q))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_invQ(m_Q4f64 q))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_lenSq(m_Q4f64 q))(f64));
$attr($inline_always)
$static fn_((m_Q4f64_len(m_Q4f64 q))(f64));
$attr($inline_always)
$static fn_((m_Q4f64_dot(m_Q4f64 lhs, m_Q4f64 rhs))(f64));
$attr($inline_always)
$static fn_((m_Q4f64_norm(m_Q4f64 q))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_isNorm(m_Q4f64 q))(bool));

/* Rotation Operations */

$attr($inline_always)
$static fn_((m_Q4f64_rotateV(m_Q4f64 q, m_V3f64 v))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_rotateX(m_Q4f64 q, f64 angle))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_rotateY(m_Q4f64 q, f64 angle))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_rotateZ(m_Q4f64 q, f64 angle))(m_Q4f64));

/* Interpolation */

$attr($inline_always)
$static fn_((m_Q4f64_lerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_nlerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64));
$attr($inline_always)
$static fn_((m_Q4f64_slerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64));

/* Extraction */

$attr($inline_always)
$static fn_((m_Q4f64_angle(m_Q4f64 lhs, m_Q4f64 rhs))(f64));
$attr($inline_always)
$static fn_((m_Q4f64_toAxisAngle(m_Q4f64 q))(m_Q4f64_AxisAngle));
$attr($inline_always)
$static fn_((m_Q4f64_toEuler(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_axis(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_angleRad(m_Q4f64 q))(f64));

/* Conversion to Matrix */

$attr($inline_always)
$static fn_((m_Q4f64_toM3(m_Q4f64 q))(m_M3f64));
$attr($inline_always)
$static fn_((m_Q4f64_toM4(m_Q4f64 q))(m_M4f64));

/* Utility */

$attr($inline_always)
$static fn_((m_Q4f64_left(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_up(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_right(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_down(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_forward(m_Q4f64 q))(m_V3f64));
$attr($inline_always)
$static fn_((m_Q4f64_backward(m_Q4f64 q))(m_V3f64));

/*========== Macros and Definitions =========================================*/

#define __comp_const__m_Q4f64_zero \
    m_Q4f64_of_static(0.0, 0.0, 0.0, 0.0)
#define __comp_const__m_Q4f64_identity \
    m_Q4f64_of_static(0.0, 0.0, 0.0, 1.0)

#define ____m_Q4f64_of_static(_x, _y, _z, _w...) l$((m_Q4f64){ \
    .v = m_V4f64_of_static(_x, _y, _z, _w), \
})
#if on_analysis_active_only || on_comptime
fn_((m_Q4f64_of(f64 x, f64 y, f64 z, f64 w))(m_Q4f64)) {
    return m_Q4f64_of_static(x, y, z, w);
};
fn_((m_Q4f64_splat(f64 scalar))(m_Q4f64)) {
    return m_Q4f64_fromV4(m_V4f64_splat(scalar));
};
fn_((m_Q4f64_fromV3(m_V3f64 v, f64 w))(m_Q4f64)) {
    return m_Q4f64_of(v.x, v.y, v.z, w);
};
fn_((m_Q4f64_fromV4(m_V4f64 v))(m_Q4f64)) {
    return m_Q4f64_of(v.x, v.y, v.z, v.w);
};
fn_((m_Q4f64_fromTo(m_V3f64 from, m_V3f64 to))(m_Q4f64)) {
    let from_len_sq = m_V3f64_lenSq(from);
    let to_len_sq = m_V3f64_lenSq(to);
    let norm_prod = flt_sqrt(from_len_sq * to_len_sq);
    let dot = m_V3f64_dot(from, to);
    let cross = m_V3f64_cross(from, to);

    if (dot < -norm_prod + 1e-6) {
        m_V3f64 axis = m_V3f64_cross(m_V3f64_right, from);
        if (m_V3f64_lenSq(axis) < 1e-6) {
            axis = m_V3f64_cross(m_V3f64_up, from);
        }
        axis = m_V3f64_norm(axis);
        return m_Q4f64_of(axis.x, axis.y, axis.z, 0.0);
    }

    let w = norm_prod + dot;
    return m_Q4f64_norm(m_Q4f64_of(cross.x, cross.y, cross.z, w));
};
fn_((m_Q4f64_fromEuler(f64 pitch, f64 yaw, f64 roll))(m_Q4f64)) {
    let hp = pitch * 0.5;
    let hy = yaw * 0.5;
    let hr = roll * 0.5;

    let sp = flt_sin(hp);
    let cp = flt_cos(hp);
    let sy = flt_sin(hy);
    let cy = flt_cos(hy);
    let sr = flt_sin(hr);
    let cr = flt_cos(hr);

    return m_Q4f64_of(
        sp * cy * cr - cp * sy * sr,
        cp * sy * cr + sp * cy * sr,
        cp * cy * sr - sp * sy * cr,
        cp * cy * cr + sp * sy * sr
    );
};
fn_((m_Q4f64_fromAxisAngle(m_V3f64 axis, f64 angle))(m_Q4f64)) {
    let half_angle = angle * 0.5;
    let s = flt_sin(half_angle);
    let c = flt_cos(half_angle);
    let n = m_V3f64_norm(axis);
    return m_Q4f64_of(n.x * s, n.y * s, n.z * s, c);
};
fn_((m_Q4f64_fromM3(m_M3f64 m))(m_Q4f64)) {
    let trace = m.s0_0 + m.s1_1 + m.s2_2;
    if (trace > 0.0) {
        let s = flt_sqrt(trace + 1.0) * 2.0;
        return m_Q4f64_of(
            (m.s2_1 - m.s1_2) / s,
            (m.s0_2 - m.s2_0) / s,
            (m.s1_0 - m.s0_1) / s,
            0.25 * s
        );
    }
    if (m.s0_0 > m.s1_1 && m.s0_0 > m.s2_2) {
        let s = flt_sqrt(1.0 + m.s0_0 - m.s1_1 - m.s2_2) * 2.0;
        return m_Q4f64_of(
            0.25 * s,
            (m.s1_0 + m.s0_1) / s,
            (m.s2_0 + m.s0_2) / s,
            (m.s2_1 - m.s1_2) / s
        );
    }
    if (m.s1_1 > m.s2_2) {
        let s = flt_sqrt(1.0 + m.s1_1 - m.s0_0 - m.s2_2) * 2.0;
        return m_Q4f64_of(
            (m.s1_0 + m.s0_1) / s,
            0.25 * s,
            (m.s2_1 + m.s1_2) / s,
            (m.s0_2 - m.s2_0) / s
        );
    }
    let s = flt_sqrt(1.0 + m.s2_2 - m.s0_0 - m.s1_1) * 2.0;
    return m_Q4f64_of(
        (m.s2_0 + m.s0_2) / s,
        (m.s2_1 + m.s1_2) / s,
        0.25 * s,
        (m.s1_0 - m.s0_1) / s
    );
};
fn_((m_Q4f64_fromM4(m_M4f64 m))(m_Q4f64)) {
    let trace = m.s0_0 + m.s1_1 + m.s2_2;
    if (trace > 0.0) {
        let s = flt_sqrt(trace + 1.0) * 2.0;
        return m_Q4f64_of(
            (m.s2_1 - m.s1_2) / s,
            (m.s0_2 - m.s2_0) / s,
            (m.s1_0 - m.s0_1) / s,
            0.25 * s
        );
    }
    if (m.s0_0 > m.s1_1 && m.s0_0 > m.s2_2) {
        let s = flt_sqrt(1.0 + m.s0_0 - m.s1_1 - m.s2_2) * 2.0;
        return m_Q4f64_of(
            0.25 * s,
            (m.s1_0 + m.s0_1) / s,
            (m.s2_0 + m.s0_2) / s,
            (m.s2_1 - m.s1_2) / s
        );
    }
    if (m.s1_1 > m.s2_2) {
        let s = flt_sqrt(1.0 + m.s1_1 - m.s0_0 - m.s2_2) * 2.0;
        return m_Q4f64_of(
            (m.s1_0 + m.s0_1) / s,
            0.25 * s,
            (m.s2_1 + m.s1_2) / s,
            (m.s0_2 - m.s2_0) / s
        );
    }
    let s = flt_sqrt(1.0 + m.s2_2 - m.s0_0 - m.s1_1) * 2.0;
    return m_Q4f64_of(
        (m.s2_0 + m.s0_2) / s,
        (m.s2_1 + m.s1_2) / s,
        0.25 * s,
        (m.s1_0 - m.s0_1) / s
    );
};

fn_((m_Q4f64_lookRotation(m_V3f64 forward, m_V3f64 up))(m_Q4f64)) {
    let f = m_V3f64_norm(forward);
    let r = m_V3f64_norm(m_V3f64_cross(up, f));
    let u = m_V3f64_cross(f, r);

    let m = m_M3f64_ofCols(
        m_V3f64_of(r.x, r.y, r.z),
        m_V3f64_of(u.x, u.y, u.z),
        m_V3f64_of(f.x, f.y, f.z)
    );
    return m_Q4f64_fromM3(m);
};

fn_((m_Q4f64_neg(m_Q4f64 q))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_neg(q.v) });
};
fn_((m_Q4f64_add(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_add(lhs.v, rhs.v) });
};
fn_((m_Q4f64_sub(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_sub(lhs.v, rhs.v) });
};
fn_((m_Q4f64_mul(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_mul(lhs.v, rhs.v) });
};
fn_((m_Q4f64_div(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_div(lhs.v, rhs.v) });
};

fn_((m_Q4f64_mulQ(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return m_Q4f64_of(
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    );
};
fn_((m_Q4f64_divQ(m_Q4f64 lhs, m_Q4f64 rhs))(m_Q4f64)) {
    return m_Q4f64_mulQ(lhs, m_Q4f64_invQ(rhs));
};

fn_((m_Q4f64_scal(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_scal(q.v, scalar) });
};
fn_((m_Q4f64_scalInv(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return m_Q4f64_scal(q, 1.0 / scalar);
};
fn_((m_Q4f64_addS(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_addS(q.v, scalar) });
};
fn_((m_Q4f64_subS(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_subS(q.v, scalar) });
};
fn_((m_Q4f64_mulS(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_mulS(q.v, scalar) });
};
fn_((m_Q4f64_divS(m_Q4f64 q, f64 scalar))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_divS(q.v, scalar) });
};

fn_((m_Q4f64_conj(m_Q4f64 q))(m_Q4f64)) {
    return m_Q4f64_of(-q.x, -q.y, -q.z, q.w);
};
fn_((m_Q4f64_invQ(m_Q4f64 q))(m_Q4f64)) {
    let len_sq = m_Q4f64_lenSq(q);
    let conj = m_Q4f64_conj(q);
    return m_Q4f64_scal(conj, 1.0 / len_sq);
};
fn_((m_Q4f64_lenSq(m_Q4f64 q))(f64)) {
    return m_V4f64_lenSq(q.v);
};
fn_((m_Q4f64_len(m_Q4f64 q))(f64)) {
    return m_V4f64_len(q.v);
};
fn_((m_Q4f64_dot(m_Q4f64 lhs, m_Q4f64 rhs))(f64)) {
    return m_V4f64_dot(lhs.v, rhs.v);
};
fn_((m_Q4f64_norm(m_Q4f64 q))(m_Q4f64)) {
    return l$((m_Q4f64){ .v = m_V4f64_norm(q.v) });
};
fn_((m_Q4f64_isNorm(m_Q4f64 q))(bool)) {
    let len_sq = m_Q4f64_lenSq(q);
    return flt_abs(len_sq - 1.0) < 1e-6;
};

fn_((m_Q4f64_rotateV(m_Q4f64 q, m_V3f64 v))(m_V3f64)) {
    let qv = m_V3f64_of(q.x, q.y, q.z);
    let uv = m_V3f64_cross(qv, v);
    let uuv = m_V3f64_cross(qv, uv);
    return m_V3f64_add(v, m_V3f64_scal(m_V3f64_add(m_V3f64_scal(uv, q.w), uuv), 2.0));
};
fn_((m_Q4f64_rotateX(m_Q4f64 q, f64 angle))(m_Q4f64)) {
    return m_Q4f64_mulQ(q, m_Q4f64_fromAxisAngle(m_V3f64_right, angle));
};
fn_((m_Q4f64_rotateY(m_Q4f64 q, f64 angle))(m_Q4f64)) {
    return m_Q4f64_mulQ(q, m_Q4f64_fromAxisAngle(m_V3f64_up, angle));
};
fn_((m_Q4f64_rotateZ(m_Q4f64 q, f64 angle))(m_Q4f64)) {
    return m_Q4f64_mulQ(q, m_Q4f64_fromAxisAngle(m_V3f64_forward, angle));
};

fn_((m_Q4f64_lerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64)) {
    return m_Q4f64_add(m_Q4f64_scal(a, 1.0 - t), m_Q4f64_scal(b, t));
};
fn_((m_Q4f64_nlerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64)) {
    let dot = m_Q4f64_dot(a, b);
    let b_adj = dot < 0.0 ? m_Q4f64_neg(b) : b;
    return m_Q4f64_norm(m_Q4f64_lerp(a, b_adj, t));
};
fn_((m_Q4f64_slerp(m_Q4f64 a, m_Q4f64 b, f64 t))(m_Q4f64)) {
    f64 dot = m_Q4f64_dot(a, b);
    m_Q4f64 b_adj = b;

    if (dot < 0.0) {
        b_adj = m_Q4f64_neg(b);
        dot = -dot;
    }
    if (dot > 0.9995) {
        return m_Q4f64_nlerp(a, b_adj, t);
    }

    let theta = flt_acos(dot);
    let sin_theta = flt_sin(theta);
    let wa = flt_sin((1.0 - t) * theta) / sin_theta;
    let wb = flt_sin(t * theta) / sin_theta;

    return m_Q4f64_add(m_Q4f64_scal(a, wa), m_Q4f64_scal(b_adj, wb));
};

fn_((m_Q4f64_angle(m_Q4f64 lhs, m_Q4f64 rhs))(f64)) {
    let dot = flt_abs(m_Q4f64_dot(lhs, rhs));
    return 2.0 * flt_acos(flt_clamp(dot, -1.0, 1.0));
};
fn_((m_Q4f64_toAxisAngle(m_Q4f64 q))(m_Q4f64_AxisAngle)) {
    let qn = m_Q4f64_norm(q);
    let angle = 2.0 * flt_acos(qn.w);
    let s = flt_sqrt(1.0 - qn.w * qn.w);

    if (s < 1e-6) {
        return l$((m_Q4f64_AxisAngle){ .axis = m_V3f64_right, .angle = angle });
    }
    return l$((m_Q4f64_AxisAngle){ .axis = m_V3f64_of(qn.x / s, qn.y / s, qn.z / s), .angle = angle });
};
fn_((m_Q4f64_toEuler(m_Q4f64 q))(m_V3f64)) {
    let sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
    let cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
    let pitch = flt_atan2(sinr_cosp, cosr_cosp);

    let sinp = 2.0 * (q.w * q.y - q.z * q.x);
    f64 yaw = 0.0;
    if (flt_abs(sinp) >= 1.0) {
        yaw = (sinp >= 0.0 ? 1.0 : -1.0) * (3.14159265358979323846 / 2.0);
    } else {
        yaw = flt_asin(sinp);
    }

    let siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
    let cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    let roll = flt_atan2(siny_cosp, cosy_cosp);

    return m_V3f64_of(pitch, yaw, roll);
};
fn_((m_Q4f64_axis(m_Q4f64 q))(m_V3f64)) {
    let s = flt_sqrt(1.0 - q.w * q.w);
    if (s < 1e-6) {
        return m_V3f64_right;
    }
    return m_V3f64_of(q.x / s, q.y / s, q.z / s);
};
fn_((m_Q4f64_angleRad(m_Q4f64 q))(f64)) {
    return 2.0 * flt_acos(flt_clamp(q.w, -1.0, 1.0));
};

fn_((m_Q4f64_toM3(m_Q4f64 q))(m_M3f64)) {
    let xx = q.x * q.x;
    let yy = q.y * q.y;
    let zz = q.z * q.z;
    let xy = q.x * q.y;
    let xz = q.x * q.z;
    let yz = q.y * q.z;
    let wx = q.w * q.x;
    let wy = q.w * q.y;
    let wz = q.w * q.z;

    return m_M3f64_ofRows(
        m_V3f64_of(1.0 - 2.0 * (yy + zz), 2.0 * (xy - wz), 2.0 * (xz + wy)),
        m_V3f64_of(2.0 * (xy + wz), 1.0 - 2.0 * (xx + zz), 2.0 * (yz - wx)),
        m_V3f64_of(2.0 * (xz - wy), 2.0 * (yz + wx), 1.0 - 2.0 * (xx + yy))
    );
};
fn_((m_Q4f64_toM4(m_Q4f64 q))(m_M4f64)) {
    let xx = q.x * q.x;
    let yy = q.y * q.y;
    let zz = q.z * q.z;
    let xy = q.x * q.y;
    let xz = q.x * q.z;
    let yz = q.y * q.z;
    let wx = q.w * q.x;
    let wy = q.w * q.y;
    let wz = q.w * q.z;

    return m_M4f64_ofRows(
        m_V4f64_of(1.0 - 2.0 * (yy + zz), 2.0 * (xy - wz), 2.0 * (xz + wy), 0.0),
        m_V4f64_of(2.0 * (xy + wz), 1.0 - 2.0 * (xx + zz), 2.0 * (yz - wx), 0.0),
        m_V4f64_of(2.0 * (xz - wy), 2.0 * (yz + wx), 1.0 - 2.0 * (xx + yy), 0.0),
        m_V4f64_of(0.0, 0.0, 0.0, 1.0)
    );
};

fn_((m_Q4f64_left(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_left);
};
fn_((m_Q4f64_up(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_up);
};
fn_((m_Q4f64_right(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_right);
};
fn_((m_Q4f64_down(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_down);
};
fn_((m_Q4f64_forward(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_forward);
};
fn_((m_Q4f64_backward(m_Q4f64 q))(m_V3f64)) {
    return m_Q4f64_rotateV(q, m_V3f64_backward);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_Q_Quat_f64__included */
