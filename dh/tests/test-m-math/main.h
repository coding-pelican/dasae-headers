#include "dh-main.h"
#include "dh/m-math-linalg/V-Vec-flt.h"
#include "dh/m-math-linalg/M-Mat-flt.h"

$attr($maybe_unused)
$static fn_((math_test_f32Near(f32 lhs, f32 rhs))(bool)) {
    return flt_abs(lhs - rhs) <= 0.0001f;
};

$attr($maybe_unused)
$static fn_((math_test_f64Near(f64 lhs, f64 rhs))(bool)) {
    return flt_abs(lhs - rhs) <= 0.000000001;
};

$attr($maybe_unused)
$static fn_((math_test_V2f32Near(m_V2f32 lhs, m_V2f32 rhs))(bool)) {
    return math_test_f32Near(lhs.x, rhs.x)
        && math_test_f32Near(lhs.y, rhs.y);
};

$attr($maybe_unused)
$static fn_((math_test_V2f64Near(m_V2f64 lhs, m_V2f64 rhs))(bool)) {
    return math_test_f64Near(lhs.x, rhs.x)
        && math_test_f64Near(lhs.y, rhs.y);
};

$attr($maybe_unused)
$static fn_((math_test_V2i32Eq(m_V2i32 lhs, m_V2i32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y;
};

$attr($maybe_unused)
$static fn_((math_test_V3f32Near(m_V3f32 lhs, m_V3f32 rhs))(bool)) {
    return math_test_f32Near(lhs.x, rhs.x)
        && math_test_f32Near(lhs.y, rhs.y)
        && math_test_f32Near(lhs.z, rhs.z);
};

$attr($maybe_unused)
$static fn_((math_test_V3f64Near(m_V3f64 lhs, m_V3f64 rhs))(bool)) {
    return math_test_f64Near(lhs.x, rhs.x)
        && math_test_f64Near(lhs.y, rhs.y)
        && math_test_f64Near(lhs.z, rhs.z);
};

$attr($maybe_unused)
$static fn_((math_test_V3i32Eq(m_V3i32 lhs, m_V3i32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V3u32Eq(m_V3u32 lhs, m_V3u32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V3u64Eq(m_V3u64 lhs, m_V3u64 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V3usizeEq(m_V3usize lhs, m_V3usize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V4usizeEq(m_V4usize lhs, m_V4usize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z
        && lhs.w == rhs.w;
};

$attr($maybe_unused)
$static fn_((math_test_M2i32Eq(m_M2i32 lhs, m_M2i32 rhs))(bool)) {
    return lhs.s0_0 == rhs.s0_0
        && lhs.s0_1 == rhs.s0_1
        && lhs.s1_0 == rhs.s1_0
        && lhs.s1_1 == rhs.s1_1;
};

$attr($maybe_unused)
$static fn_((math_test_M2usizeEq(m_M2usize lhs, m_M2usize rhs))(bool)) {
    return lhs.s0_0 == rhs.s0_0
        && lhs.s0_1 == rhs.s0_1
        && lhs.s1_0 == rhs.s1_0
        && lhs.s1_1 == rhs.s1_1;
};

$attr($maybe_unused)
$static fn_((math_test_V3i64Eq(m_V3i64 lhs, m_V3i64 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V3isizeEq(m_V3isize lhs, m_V3isize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$attr($maybe_unused)
$static fn_((math_test_V4isizeEq(m_V4isize lhs, m_V4isize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z
        && lhs.w == rhs.w;
};

$attr($maybe_unused)
$static fn_((math_test_project(m_M4f32 m, m_V3f32 p))(m_V3f32)) {
    let clip = m_M4f32_mulV(m, m_V4f32_of(p.x, p.y, p.z, 1.0f));
    return m_V3f32_of(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
};

$attr($maybe_unused)
$static fn_((math_test_project64(m_M4f64 m, m_V3f64 p))(m_V3f64)) {
    let clip = m_M4f64_mulV(m, m_V4f64_of(p.x, p.y, p.z, 1.0));
    return m_V3f64_of(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
};
