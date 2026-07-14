#include "test-m-math/main.h"

#include "dh/m-math-linalg/Q-Quat.h"

TEST_fn_("math: predefined constants cover core and view conventions" $scope) {
    try_(TEST_expect(math_test_V3f32Near(m_V3f32_view_forward_rh, m_V3f32_backward)));
    try_(TEST_expect(math_test_V3f32Near(m_V3f32_view_forward_lh, m_V3f32_forward)));
    try_(TEST_expect(math_test_V3f64Near(m_V3f64_view_forward_rh, m_V3f64_backward)));
    try_(TEST_expect(math_test_V3f64Near(m_V3f64_view_forward_lh, m_V3f64_forward)));

    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(m_M4f32_identity, m_V3f32_of(2.0f, 3.0f, 4.0f), 1.0f),
        m_V3f32_of(2.0f, 3.0f, 4.0f)
    )));
    try_(TEST_expect(math_test_V3f64Near(
        m_M4f64_mulV3(m_M4f64_identity, m_V3f64_of(2.0, 3.0, 4.0), 1.0),
        m_V3f64_of(2.0, 3.0, 4.0)
    )));
    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_forward(m_Q4f32_identity), m_V3f32_forward)));
    try_(TEST_expect(math_test_V3f64Near(m_Q4f64_forward(m_Q4f64_identity), m_V3f64_forward)));
} $unscoped(TEST_fn)

#include "dh/m-math-linalg/V-Vec-int.h"

TEST_fn_("math: integer vector constants cover i32 i64 and isize conventions" $scope) {
    try_(TEST_expect(math_test_V3i32Eq(m_V3i32_view_forward_rh, m_V3i32_backward)));
    try_(TEST_expect(math_test_V3i32Eq(m_V3i32_view_forward_lh, m_V3i32_forward)));
    try_(TEST_expect(math_test_V3i64Eq(m_V3i64_view_forward_rh, m_V3i64_backward)));
    try_(TEST_expect(math_test_V3i64Eq(m_V3i64_view_forward_lh, m_V3i64_forward)));
    try_(TEST_expect(math_test_V3isizeEq(m_V3isize_view_forward_rh, m_V3isize_backward)));
    try_(TEST_expect(math_test_V3isizeEq(m_V3isize_view_forward_lh, m_V3isize_forward)));

    try_(TEST_expect(math_test_V3isizeEq(
        m_V3isize_cross(m_V3isize_unit_x, m_V3isize_unit_y),
        m_V3isize_unit_z
    )));
    try_(TEST_expect(m_V3isize_dot(m_V3isize_of(1, 2, 3), m_V3isize_of(4, 5, 6)) == 32));
    try_(TEST_expect(math_test_V4isizeEq(
        m_V4isize_add(m_V4isize_unit_x, m_V4isize_unit_w),
        m_V4isize_of(1, 0, 0, 1)
    )));
} $unscoped(TEST_fn)
