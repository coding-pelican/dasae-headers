#include "test-m-math/main.h"
#include "dh/m-math-linalg/V-Vec-sgnd.h"

TEST_fn_("math: vector orientation helpers use RH signed area and volume" $scope) {
    try_(TEST_expect(math_test_f32Near(
        m_V2f32_det(m_V2f32_right, m_V2f32_up),
        m_V2f32_cross(m_V2f32_right, m_V2f32_up)
    )));
    try_(TEST_expect(math_test_f32Near(
        m_V2f32_orient(m_V2f32_zero, m_V2f32_right, m_V2f32_up),
        1.0f
    )));
    try_(TEST_expect(math_test_f32Near(
        m_V2f32_orient(m_V2f32_zero, m_V2f32_up, m_V2f32_right),
        -1.0f
    )));
    try_(TEST_expect(m_V2f32_orientDir(m_V2f32_zero, m_V2f32_right, m_V2f32_up) == m_V_Orient_ccw));
    try_(TEST_expect(m_V2f32_isCCW(m_V2f32_zero, m_V2f32_right, m_V2f32_up)));
    try_(TEST_expect(m_V2f32_isCW(m_V2f32_zero, m_V2f32_up, m_V2f32_right)));
    try_(TEST_expect(m_V2f32_isCollinear(m_V2f32_zero, m_V2f32_right, m_V2f32_of(2.0f, 0.0f))));

    try_(TEST_expect(math_test_V2i32Eq(m_V2i32_ccw(m_V2i32_right), m_V2i32_up)));
    try_(TEST_expect(math_test_V2i32Eq(m_V2i32_skew(m_V2i32_right), m_V2i32_up)));
    try_(TEST_expect(math_test_V2i32Eq(m_V2i32_perp(m_V2i32_right), m_V2i32_up)));
    try_(TEST_expect(math_test_V2i32Eq(m_V2i32_cw(m_V2i32_up), m_V2i32_right)));
    try_(TEST_expect(m_V2i32_orient(m_V2i32_zero, m_V2i32_right, m_V2i32_up) > 0));
    try_(TEST_expect(m_V2i32_orient(m_V2i32_zero, m_V2i32_up, m_V2i32_right) < 0));
    try_(TEST_expect(m_V2i32_orient(m_V2i32_zero, m_V2i32_right, m_V2i32_of(2, 0)) == 0));
    try_(TEST_expect(m_V2i32_orientDir(m_V2i32_zero, m_V2i32_up, m_V2i32_right) == m_V_Orient_cw));
    try_(TEST_expect(m_V2i32_isCW(m_V2i32_zero, m_V2i32_up, m_V2i32_right)));
    try_(TEST_expect(m_V2i32_isCCW(m_V2i32_zero, m_V2i32_right, m_V2i32_up)));
    try_(TEST_expect(m_V2i32_isCollinear(m_V2i32_zero, m_V2i32_right, m_V2i32_of(2, 0))));

    try_(TEST_expect(math_test_f64Near(
        m_V3f64_det(m_V3f64_unit_x, m_V3f64_unit_y, m_V3f64_unit_z),
        1.0
    )));
    try_(TEST_expect(math_test_f64Near(
        m_V3f64_det(m_V3f64_unit_y, m_V3f64_unit_x, m_V3f64_unit_z),
        -1.0
    )));
    try_(TEST_expect(math_test_f64Near(
        m_V3f64_orient(m_V3f64_zero, m_V3f64_unit_x, m_V3f64_unit_y, m_V3f64_unit_z),
        1.0
    )));
    try_(TEST_expect(m_V3f64_orientDir(m_V3f64_zero, m_V3f64_unit_x, m_V3f64_unit_y, m_V3f64_unit_z) == m_V_Orient_pos));
    try_(TEST_expect(m_V3f64_isOrientPos(m_V3f64_zero, m_V3f64_unit_x, m_V3f64_unit_y, m_V3f64_unit_z)));
    try_(TEST_expect(m_V3f64_isOrientNeg(m_V3f64_zero, m_V3f64_unit_y, m_V3f64_unit_x, m_V3f64_unit_z)));
    try_(TEST_expect(m_V3f64_isCoplanar(m_V3f64_zero, m_V3f64_unit_x, m_V3f64_unit_y, m_V3f64_of(1.0, 1.0, 0.0))));
    try_(TEST_expect(m_V3i64_orient(m_V3i64_zero, m_V3i64_unit_x, m_V3i64_unit_y, m_V3i64_unit_z) == 1));
    try_(TEST_expect(m_V3i64_orientDir(m_V3i64_zero, m_V3i64_unit_x, m_V3i64_unit_y, m_V3i64_unit_z) == m_V_Orient_pos));
    try_(TEST_expect(m_V3i64_isOrientPos(m_V3i64_zero, m_V3i64_unit_x, m_V3i64_unit_y, m_V3i64_unit_z)));
    try_(TEST_expect(m_V3i64_isCoplanar(m_V3i64_zero, m_V3i64_unit_x, m_V3i64_unit_y, m_V3i64_of(1, 1, 0))));
} $unscoped(TEST_fn)
