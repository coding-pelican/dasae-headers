#include "test-m-math/main.h"

#include "dh/m-math-linalg/V-Vec-usgnd.h"

TEST_fn_("math: unsigned vector support exposes only unsigned-safe constants and operations" $scope) {
    try_(TEST_expect(math_test_V3u32Eq(m_V3u32_view_backward_rh, m_V3u32_forward)));
    try_(TEST_expect(math_test_V3u32Eq(m_V3u32_view_forward_lh, m_V3u32_forward)));
    try_(TEST_expect(math_test_V3u64Eq(m_V3u64_view_backward_rh, m_V3u64_forward)));
    try_(TEST_expect(math_test_V3usizeEq(m_V3usize_view_forward_lh, m_V3usize_forward)));

    try_(TEST_expect(math_test_V3u32Eq(
        m_V3u32_add(m_V3u32_unit_x, m_V3u32_unit_z),
        m_V3u32_of(1, 0, 1)
    )));
    try_(TEST_expect(m_V3u64_dot(m_V3u64_of(1, 2, 3), m_V3u64_of(4, 5, 6)) == 32));
    try_(TEST_expect(m_V3usize_distSq(m_V3usize_of(2, 7, 4), m_V3usize_of(5, 3, 4)) == 25));
    try_(TEST_expect(math_test_V4usizeEq(
        m_V4usize_clamp(m_V4usize_of(1, 5, 9, 13), m_V4usize_of(2, 3, 4, 5), m_V4usize_of(8, 8, 8, 8)),
        m_V4usize_of(2, 5, 8, 8)
    )));
} $unscoped(TEST_fn)

#include "dh/m-math-linalg/V-Vec-sgnd.h"

TEST_fn_("math: signed and floating vector SIMD paths preserve lane semantics" $scope) {
    try_(TEST_expect(math_test_V3i32Eq(
        m_V3i32_rem(m_V3i32_of(7, 8, 9), m_V3i32_of(3, 3, 4)),
        m_V3i32_of(1, 2, 1)
    )));
    try_(TEST_expect(math_test_V3i32Eq(
        m_V3i32_wrap(m_V3i32_of(5, 6, 7), m_V3i32_zero, m_V3i32_splat(4)),
        m_V3i32_of(1, 2, 3)
    )));
    try_(TEST_expect(math_test_V3i32Eq(
        m_V3i32_perp(m_V3i32_of(1, 2, 3)),
        m_V3i32_of(-2, 1, 0)
    )));
    try_(TEST_expect(m_V3i64_perp(m_V3i64_of(0, 0, 3)).y == -3));

    try_(TEST_expect(math_test_V3f32Near(
        m_V3f32_inv(m_V3f32_of(2.0f, 4.0f, 8.0f)),
        m_V3f32_of(0.5f, 0.25f, 0.125f)
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_V3f32_rem(m_V3f32_of(7.5f, 8.5f, 9.5f), m_V3f32_of(3.0f, 3.0f, 4.0f)),
        m_V3f32_of(1.5f, 2.5f, 1.5f)
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_V3f32_wrap(m_V3f32_of(5.5f, 6.25f, 7.75f), m_V3f32_zero, m_V3f32_splat(4.0f)),
        m_V3f32_of(1.5f, 2.25f, 3.75f)
    )));
    try_(TEST_expect(m_V3f32_ordApxAbs(m_V3f32_of(1.0f, 2.0f, 3.0f), m_V3f32_of(1.01f, 1.9f, 3.0f), m_V3f32_of(0.1f, 0.05f, 0.1f)) == cmp_Ord_gt));
    try_(TEST_expect(m_V3f32_ordApxRel(m_V3f32_of(1000.0f, 2.0f, 3.0f), m_V3f32_of(1001.0f, 2.0f, 3.0f), m_V3f32_of(0.001f, 0.0f, 0.0f)) == cmp_Ord_eq));
    try_(TEST_expect(math_test_f64Near(m_V4f64_inv(m_V4f64_splat(4.0)).w, 0.25)));
} $unscoped(TEST_fn)
