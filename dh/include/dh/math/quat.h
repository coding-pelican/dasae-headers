/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    quat.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-24 (date of creation)
 * @updated 2025-12-25 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/math
 * @prefix  m_Q
 *
 * @brief   Mathematical common quaternion operations
 * @details Provides a set of quaternion mathematical operations and functions
 */
#ifndef math_quat__included
#define math_quat__included 1

/*========== Includes =======================================================*/

#include "quat_types.h"
#include "vec.h"
#include "mat.h"

/*========== Constants ======================================================*/

#define m_Q4f32_zero __comp_const__m_Q4f32_zero
#define m_Q4f32_identity __comp_const__m_Q4f32_identity

/*========== Creation Functions =============================================*/

#define m_Q4f32_of_static(_x, _y, _z, _w...) ____m_Q4f32_of_static(_x, _y, _z, _w)
$attr($inline_always)
$static fn_((m_Q4f32_of(f32 x, f32 y, f32 z, f32 w))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_splat(f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromV3(m_V3f32 v, f32 w))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromV4(m_V4f32 v))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromTo(m_V3f32 from, m_V3f32 to))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromEuler(f32 pitch, f32 yaw, f32 roll))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromAxisAngle(m_V3f32 axis, f32 angle))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromM3(m_M3f32 m))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_fromM4(m_M4f32 m))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_lookRotation(m_V3f32 forward, m_V3f32 up))(m_Q4f32));

/*========== Component-wise Arithmetic ======================================*/

$attr($inline_always)
$static fn_((m_Q4f32_neg(m_Q4f32 q))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_add(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_sub(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_mul(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_div(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));

/*========== Quaternion Arithmetic (Hamilton Product) =======================*/

$attr($inline_always)
$static fn_((m_Q4f32_mulQ(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_divQ(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32));

/*========== Scalar Operations ==============================================*/

$attr($inline_always)
$static fn_((m_Q4f32_scal(m_Q4f32 q, f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_scalInv(m_Q4f32 q, f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_addScalar(m_Q4f32 q, f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_subScalar(m_Q4f32 q, f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_mulScalar(m_Q4f32 q, f32 scalar))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_divScalar(m_Q4f32 q, f32 scalar))(m_Q4f32));

/*========== Quaternion Properties ==========================================*/

$attr($inline_always)
$static fn_((m_Q4f32_conj(m_Q4f32 q))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_invQ(m_Q4f32 q))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_lenSq(m_Q4f32 q))(f32));
$attr($inline_always)
$static fn_((m_Q4f32_len(m_Q4f32 q))(f32));
$attr($inline_always)
$static fn_((m_Q4f32_dot(m_Q4f32 lhs, m_Q4f32 rhs))(f32));
$attr($inline_always)
$static fn_((m_Q4f32_norm(m_Q4f32 q))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_isNorm(m_Q4f32 q))(bool));

/*========== Rotation Operations ============================================*/

$attr($inline_always)
$static fn_((m_Q4f32_rotateV(m_Q4f32 q, m_V3f32 v))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_rotateX(m_Q4f32 q, f32 angle))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_rotateY(m_Q4f32 q, f32 angle))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_rotateZ(m_Q4f32 q, f32 angle))(m_Q4f32));

/*========== Interpolation ==================================================*/

$attr($inline_always)
$static fn_((m_Q4f32_lerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_nlerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32));
$attr($inline_always)
$static fn_((m_Q4f32_slerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32));

/*========== Extraction =====================================================*/

$attr($inline_always)
$static fn_((m_Q4f32_angle(m_Q4f32 lhs, m_Q4f32 rhs))(f32));
$attr($inline_always)
$static fn_((m_Q4f32_toAxisAngle(m_Q4f32 q))(m_Q4f32_AxisAngle));
$attr($inline_always)
$static fn_((m_Q4f32_toEuler(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_axis(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_angleRad(m_Q4f32 q))(f32));

/*========== Conversion to Matrix ===========================================*/

$attr($inline_always)
$static fn_((m_Q4f32_toM3(m_Q4f32 q))(m_M3f32));
$attr($inline_always)
$static fn_((m_Q4f32_toM4(m_Q4f32 q))(m_M4f32));

/*========== Utility ========================================================*/

$attr($inline_always)
$static fn_((m_Q4f32_left(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_up(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_right(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_down(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_forward(m_Q4f32 q))(m_V3f32));
$attr($inline_always)
$static fn_((m_Q4f32_backward(m_Q4f32 q))(m_V3f32));

/*========== Implementation =================================================*/

#define __comp_const__m_Q4f32_zero \
    m_Q4f32_of_static(0.0f, 0.0f, 0.0f, 0.0f)
#define __comp_const__m_Q4f32_identity \
    m_Q4f32_of_static(0.0f, 0.0f, 0.0f, 1.0f)

#define ____m_Q4f32_of_static(_x, _y, _z, _w...) lit$((m_Q4f32){ \
    .v = m_V4f32_of_static(_x, _y, _z, _w), \
})
fn_((m_Q4f32_of(f32 x, f32 y, f32 z, f32 w))(m_Q4f32)) {
    return m_Q4f32_of_static(x, y, z, w);
};
fn_((m_Q4f32_splat(f32 scalar))(m_Q4f32)) {
    return m_Q4f32_fromV4(m_V4f32_splat(scalar));
};
fn_((m_Q4f32_fromV3(m_V3f32 v, f32 w))(m_Q4f32)) {
    return m_Q4f32_of(v.x, v.y, v.z, w);
};
fn_((m_Q4f32_fromV4(m_V4f32 v))(m_Q4f32)) {
    return m_Q4f32_of(v.x, v.y, v.z, v.w);
};
fn_((m_Q4f32_fromTo(m_V3f32 from, m_V3f32 to))(m_Q4f32)) {
    let from_len_sq = m_V3f32_lenSq(from);
    let to_len_sq = m_V3f32_lenSq(to);
    let norm_prod = flt_sqrt(from_len_sq * to_len_sq);
    let dot = m_V3f32_dot(from, to);
    let cross = m_V3f32_cross(from, to);

    if (dot < -norm_prod + 1e-6f) {
        m_V3f32 axis = m_V3f32_cross(m_V3f32_right, from);
        if (m_V3f32_lenSq(axis) < 1e-6f) {
            axis = m_V3f32_cross(m_V3f32_up, from);
        }
        axis = m_V3f32_norm(axis);
        return m_Q4f32_of(axis.x, axis.y, axis.z, 0.0f);
    }

    let w = norm_prod + dot;
    return m_Q4f32_norm(m_Q4f32_of(cross.x, cross.y, cross.z, w));
};
fn_((m_Q4f32_fromEuler(f32 pitch, f32 yaw, f32 roll))(m_Q4f32)) {
    let hp = pitch * 0.5f;
    let hy = yaw * 0.5f;
    let hr = roll * 0.5f;

    let sp = flt_sin(hp);
    let cp = flt_cos(hp);
    let sy = flt_sin(hy);
    let cy = flt_cos(hy);
    let sr = flt_sin(hr);
    let cr = flt_cos(hr);

    return m_Q4f32_of(
        sp * cy * cr - cp * sy * sr,
        cp * sy * cr + sp * cy * sr,
        cp * cy * sr - sp * sy * cr,
        cp * cy * cr + sp * sy * sr
    );
};
fn_((m_Q4f32_fromAxisAngle(m_V3f32 axis, f32 angle))(m_Q4f32)) {
    let half_angle = angle * 0.5f;
    let s = flt_sin(half_angle);
    let c = flt_cos(half_angle);
    let n = m_V3f32_norm(axis);
    return m_Q4f32_of(n.x * s, n.y * s, n.z * s, c);
};
fn_((m_Q4f32_fromM3(m_M3f32 m))(m_Q4f32)) {
    let trace = m.s0_0 + m.s1_1 + m.s2_2;
    if (trace > 0.0f) {
        let s = flt_sqrt(trace + 1.0f) * 2.0f;
        return m_Q4f32_of(
            (m.s1_2 - m.s2_1) / s,
            (m.s2_0 - m.s0_2) / s,
            (m.s0_1 - m.s1_0) / s,
            0.25f * s
        );
    }
    if (m.s0_0 > m.s1_1 && m.s0_0 > m.s2_2) {
        let s = flt_sqrt(1.0f + m.s0_0 - m.s1_1 - m.s2_2) * 2.0f;
        return m_Q4f32_of(
            0.25f * s,
            (m.s1_0 + m.s0_1) / s,
            (m.s2_0 + m.s0_2) / s,
            (m.s1_2 - m.s2_1) / s
        );
    }
    if (m.s1_1 > m.s2_2) {
        let s = flt_sqrt(1.0f + m.s1_1 - m.s0_0 - m.s2_2) * 2.0f;
        return m_Q4f32_of(
            (m.s1_0 + m.s0_1) / s,
            0.25f * s,
            (m.s2_1 + m.s1_2) / s,
            (m.s2_0 - m.s0_2) / s
        );
    }
    let s = flt_sqrt(1.0f + m.s2_2 - m.s0_0 - m.s1_1) * 2.0f;
    return m_Q4f32_of(
        (m.s2_0 + m.s0_2) / s,
        (m.s2_1 + m.s1_2) / s,
        0.25f * s,
        (m.s0_1 - m.s1_0) / s
    );
};
fn_((m_Q4f32_fromM4(m_M4f32 m))(m_Q4f32)) {
    let trace = m.s0_0 + m.s1_1 + m.s2_2;
    if (trace > 0.0f) {
        let s = flt_sqrt(trace + 1.0f) * 2.0f;
        return m_Q4f32_of(
            (m.s1_2 - m.s2_1) / s,
            (m.s2_0 - m.s0_2) / s,
            (m.s0_1 - m.s1_0) / s,
            0.25f * s
        );
    }
    if (m.s0_0 > m.s1_1 && m.s0_0 > m.s2_2) {
        let s = flt_sqrt(1.0f + m.s0_0 - m.s1_1 - m.s2_2) * 2.0f;
        return m_Q4f32_of(
            0.25f * s,
            (m.s1_0 + m.s0_1) / s,
            (m.s2_0 + m.s0_2) / s,
            (m.s1_2 - m.s2_1) / s
        );
    }
    if (m.s1_1 > m.s2_2) {
        let s = flt_sqrt(1.0f + m.s1_1 - m.s0_0 - m.s2_2) * 2.0f;
        return m_Q4f32_of(
            (m.s1_0 + m.s0_1) / s,
            0.25f * s,
            (m.s2_1 + m.s1_2) / s,
            (m.s2_0 - m.s0_2) / s
        );
    }
    let s = flt_sqrt(1.0f + m.s2_2 - m.s0_0 - m.s1_1) * 2.0f;
    return m_Q4f32_of(
        (m.s2_0 + m.s0_2) / s,
        (m.s2_1 + m.s1_2) / s,
        0.25f * s,
        (m.s0_1 - m.s1_0) / s
    );
};

fn_((m_Q4f32_lookRotation(m_V3f32 forward, m_V3f32 up))(m_Q4f32)) {
    let f = m_V3f32_norm(forward);
    let r = m_V3f32_norm(m_V3f32_cross(up, f));
    let u = m_V3f32_cross(f, r);

    let m = m_M3f32_ofRows(
        m_V3f32_of(r.x, r.y, r.z),
        m_V3f32_of(u.x, u.y, u.z),
        m_V3f32_of(f.x, f.y, f.z)
    );
    return m_Q4f32_fromM3(m);
};

fn_((m_Q4f32_neg(m_Q4f32 q))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_neg(q.v) });
};
fn_((m_Q4f32_add(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_add(lhs.v, rhs.v) });
};
fn_((m_Q4f32_sub(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_sub(lhs.v, rhs.v) });
};
fn_((m_Q4f32_mul(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_mul(lhs.v, rhs.v) });
};
fn_((m_Q4f32_div(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_div(lhs.v, rhs.v) });
};

fn_((m_Q4f32_mulQ(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return m_Q4f32_of(
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    );
};
fn_((m_Q4f32_divQ(m_Q4f32 lhs, m_Q4f32 rhs))(m_Q4f32)) {
    return m_Q4f32_mulQ(lhs, m_Q4f32_invQ(rhs));
};

fn_((m_Q4f32_scal(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_scal(q.v, scalar) });
};
fn_((m_Q4f32_scalInv(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return m_Q4f32_scal(q, 1.0f / scalar);
};
fn_((m_Q4f32_addScalar(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_addScalar(q.v, scalar) });
};
fn_((m_Q4f32_subScalar(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_subScalar(q.v, scalar) });
};
fn_((m_Q4f32_mulScalar(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_mulScalar(q.v, scalar) });
};
fn_((m_Q4f32_divScalar(m_Q4f32 q, f32 scalar))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_divScalar(q.v, scalar) });
};

fn_((m_Q4f32_conj(m_Q4f32 q))(m_Q4f32)) {
    return m_Q4f32_of(-q.x, -q.y, -q.z, q.w);
};
fn_((m_Q4f32_invQ(m_Q4f32 q))(m_Q4f32)) {
    let len_sq = m_Q4f32_lenSq(q);
    let conj = m_Q4f32_conj(q);
    return m_Q4f32_scal(conj, 1.0f / len_sq);
};
fn_((m_Q4f32_lenSq(m_Q4f32 q))(f32)) {
    return m_V4f32_lenSq(q.v);
};
fn_((m_Q4f32_len(m_Q4f32 q))(f32)) {
    return m_V4f32_len(q.v);
};
fn_((m_Q4f32_dot(m_Q4f32 lhs, m_Q4f32 rhs))(f32)) {
    return m_V4f32_dot(lhs.v, rhs.v);
};
fn_((m_Q4f32_norm(m_Q4f32 q))(m_Q4f32)) {
    return lit$((m_Q4f32){ .v = m_V4f32_norm(q.v) });
};
fn_((m_Q4f32_isNorm(m_Q4f32 q))(bool)) {
    let len_sq = m_Q4f32_lenSq(q);
    return flt_abs(len_sq - 1.0f) < 1e-6f;
};

fn_((m_Q4f32_rotateV(m_Q4f32 q, m_V3f32 v))(m_V3f32)) {
    let qv = m_V3f32_of(q.x, q.y, q.z);
    let uv = m_V3f32_cross(qv, v);
    let uuv = m_V3f32_cross(qv, uv);
    return m_V3f32_add(v, m_V3f32_scal(m_V3f32_add(m_V3f32_scal(uv, q.w), uuv), 2.0f));
};
fn_((m_Q4f32_rotateX(m_Q4f32 q, f32 angle))(m_Q4f32)) {
    return m_Q4f32_mulQ(q, m_Q4f32_fromAxisAngle(m_V3f32_right, angle));
};
fn_((m_Q4f32_rotateY(m_Q4f32 q, f32 angle))(m_Q4f32)) {
    return m_Q4f32_mulQ(q, m_Q4f32_fromAxisAngle(m_V3f32_up, angle));
};
fn_((m_Q4f32_rotateZ(m_Q4f32 q, f32 angle))(m_Q4f32)) {
    return m_Q4f32_mulQ(q, m_Q4f32_fromAxisAngle(m_V3f32_forward, angle));
};

fn_((m_Q4f32_lerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32)) {
    return m_Q4f32_add(m_Q4f32_scal(a, 1.0f - t), m_Q4f32_scal(b, t));
};
fn_((m_Q4f32_nlerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32)) {
    let dot = m_Q4f32_dot(a, b);
    let b_adj = dot < 0.0f ? m_Q4f32_neg(b) : b;
    return m_Q4f32_norm(m_Q4f32_lerp(a, b_adj, t));
};
fn_((m_Q4f32_slerp(m_Q4f32 a, m_Q4f32 b, f32 t))(m_Q4f32)) {
    f32 dot = m_Q4f32_dot(a, b);
    m_Q4f32 b_adj = b;

    if (dot < 0.0f) {
        b_adj = m_Q4f32_neg(b);
        dot = -dot;
    }
    if (dot > 0.9995f) {
        return m_Q4f32_nlerp(a, b_adj, t);
    }

    let theta = flt_acos(dot);
    let sin_theta = flt_sin(theta);
    let wa = flt_sin((1.0f - t) * theta) / sin_theta;
    let wb = flt_sin(t * theta) / sin_theta;

    return m_Q4f32_add(m_Q4f32_scal(a, wa), m_Q4f32_scal(b_adj, wb));
};

fn_((m_Q4f32_angle(m_Q4f32 lhs, m_Q4f32 rhs))(f32)) {
    let dot = flt_abs(m_Q4f32_dot(lhs, rhs));
    return 2.0f * flt_acos(flt_clamp(dot, -1.0f, 1.0f));
};
fn_((m_Q4f32_toAxisAngle(m_Q4f32 q))(m_Q4f32_AxisAngle)) {
    let qn = m_Q4f32_norm(q);
    let angle = 2.0f * flt_acos(qn.w);
    let s = flt_sqrt(1.0f - qn.w * qn.w);

    if (s < 1e-6f) {
        return lit$((m_Q4f32_AxisAngle){ .axis = m_V3f32_right, .angle = angle });
    }
    return lit$((m_Q4f32_AxisAngle){ .axis = m_V3f32_of(qn.x / s, qn.y / s, qn.z / s), .angle = angle });
};
fn_((m_Q4f32_toEuler(m_Q4f32 q))(m_V3f32)) {
    let sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    let cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    let pitch = flt_atan2(sinr_cosp, cosr_cosp);

    let sinp = 2.0f * (q.w * q.y - q.z * q.x);
    f32 yaw = 0.0f;
    if (flt_abs(sinp) >= 1.0f) {
        yaw = (sinp >= 0.0f ? 1.0f : -1.0f) * (3.14159265358979323846f / 2.0f);
    } else {
        yaw = flt_asin(sinp);
    }

    let siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    let cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    let roll = flt_atan2(siny_cosp, cosy_cosp);

    return m_V3f32_of(pitch, yaw, roll);
};
fn_((m_Q4f32_axis(m_Q4f32 q))(m_V3f32)) {
    let s = flt_sqrt(1.0f - q.w * q.w);
    if (s < 1e-6f) {
        return m_V3f32_right;
    }
    return m_V3f32_of(q.x / s, q.y / s, q.z / s);
};
fn_((m_Q4f32_angleRad(m_Q4f32 q))(f32)) {
    return 2.0f * flt_acos(flt_clamp(q.w, -1.0f, 1.0f));
};

fn_((m_Q4f32_toM3(m_Q4f32 q))(m_M3f32)) {
    let xx = q.x * q.x;
    let yy = q.y * q.y;
    let zz = q.z * q.z;
    let xy = q.x * q.y;
    let xz = q.x * q.z;
    let yz = q.y * q.z;
    let wx = q.w * q.x;
    let wy = q.w * q.y;
    let wz = q.w * q.z;

    return m_M3f32_ofRows(
        m_V3f32_of(1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy)),
        m_V3f32_of(2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx)),
        m_V3f32_of(2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy))
    );
};
fn_((m_Q4f32_toM4(m_Q4f32 q))(m_M4f32)) {
    let xx = q.x * q.x;
    let yy = q.y * q.y;
    let zz = q.z * q.z;
    let xy = q.x * q.y;
    let xz = q.x * q.z;
    let yz = q.y * q.z;
    let wx = q.w * q.x;
    let wy = q.w * q.y;
    let wz = q.w * q.z;

    return m_M4f32_ofRows(
        m_V4f32_of(1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f),
        m_V4f32_of(2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx), 0.0f),
        m_V4f32_of(2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy), 0.0f),
        m_V4f32_of(0.0f, 0.0f, 0.0f, 1.0f)
    );
};

fn_((m_Q4f32_left(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_left);
};
fn_((m_Q4f32_up(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_up);
};
fn_((m_Q4f32_right(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_right);
};
fn_((m_Q4f32_down(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_down);
};
fn_((m_Q4f32_forward(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_forward);
};
fn_((m_Q4f32_backward(m_Q4f32 q))(m_V3f32)) {
    return m_Q4f32_rotateV(q, m_V3f32_backward);
};

#endif /* math_quat__included */
