#include "test-m-math/main.h"

TEST_fn_("math: 2D vector and matrix rotations use RH positive angle" $scope) {
    let angle = math_f32_pi * 0.5f;
    let expected = m_V2f32_up;

    try_(TEST_expect(math_test_V2f32Near(
        m_V2f32_rotate(m_V2f32_right, angle),
        expected
    )));
    try_(TEST_expect(math_test_V2f32Near(
        m_M2f32_mulV(m_M2f32_rotate(angle), m_V2f32_right),
        expected
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_M3f32_mulV(m_M3f32_rotate2D(angle), m_V3f32_of(1.0f, 0.0f, 1.0f)),
        m_V3f32_of(0.0f, 1.0f, 1.0f)
    )));
} $unscoped(TEST_fn)

#include "dh/m-math-linalg/Q-Quat/f32.h"

TEST_fn_("math: 3D vector quaternion and matrix rotations use RH positive angle" $scope) {
    let angle = math_f32_pi * 0.5f;
    let expected_x = m_V3f32_forward;
    let expected_y = m_V3f32_right;
    let expected_z = m_V3f32_up;

    try_(TEST_expect(math_test_V3f32Near(
        m_V3f32_rotate(m_V3f32_up, m_V3f32_right, angle),
        expected_x
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(m_M4f32_rotateX(angle), m_V3f32_up, 0.0f),
        expected_x
    )));

    try_(TEST_expect(math_test_V3f32Near(
        m_Q4f32_rotateV(m_Q4f32_fromAxisAngle(m_V3f32_up, angle), m_V3f32_forward),
        expected_y
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(m_M4f32_rotateY(angle), m_V3f32_forward, 0.0f),
        expected_y
    )));

    try_(TEST_expect(math_test_V3f32Near(
        m_Q4f32_rotateV(m_Q4f32_fromAxisAngle(m_V3f32_forward, angle), m_V3f32_right),
        expected_z
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(m_M4f32_rotateZ(angle), m_V3f32_right, 0.0f),
        expected_z
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(m_M4f32_rotateAxis(m_V3f32_forward, angle), m_V3f32_right, 0.0f),
        expected_z
    )));
} $unscoped(TEST_fn)

TEST_fn_("math: quaternion matrix conversions preserve RH rotation" $scope) {
    let angle = math_f32_pi * 0.5f;
    let q = m_Q4f32_fromAxisAngle(m_V3f32_up, angle);
    let expected = m_V3f32_right;

    try_(TEST_expect(math_test_V3f32Near(
        m_Q4f32_rotateV(m_Q4f32_fromM3(m_Q4f32_toM3(q)), m_V3f32_forward),
        expected
    )));
    try_(TEST_expect(math_test_V3f32Near(
        m_Q4f32_rotateV(m_Q4f32_fromM4(m_Q4f32_toM4(q)), m_V3f32_forward),
        expected
    )));

    let x_turn = m_Q4f32_fromM3(m_Q4f32_toM3(m_Q4f32_fromAxisAngle(m_V3f32_right, math_f32_pi)));
    let y_turn = m_Q4f32_fromM3(m_Q4f32_toM3(m_Q4f32_fromAxisAngle(m_V3f32_up, math_f32_pi)));
    let z_turn = m_Q4f32_fromM3(m_Q4f32_toM3(m_Q4f32_fromAxisAngle(m_V3f32_forward, math_f32_pi)));

    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_rotateV(x_turn, m_V3f32_up), m_V3f32_down)));
    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_rotateV(y_turn, m_V3f32_forward), m_V3f32_backward)));
    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_rotateV(z_turn, m_V3f32_right), m_V3f32_left)));
} $unscoped(TEST_fn)

TEST_fn_("math: look helpers use explicit RH and LH basis consistently" $scope) {
    let look_right = m_Q4f32_lookRotation(m_V3f32_right, m_V3f32_up);
    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_forward(look_right), m_V3f32_right)));
    try_(TEST_expect(math_test_V3f32Near(m_Q4f32_up(look_right), m_V3f32_up)));

    let view_rh = m_M4f32_lookAtRH(m_V3f32_zero, m_V3f32_backward, m_V3f32_up);
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(view_rh, m_V3f32_backward, 1.0f),
        m_V3f32_backward
    )));

    let view_lh = m_M4f32_lookAtLH(m_V3f32_zero, m_V3f32_forward, m_V3f32_up);
    try_(TEST_expect(math_test_V3f32Near(
        m_M4f32_mulV3(view_lh, m_V3f32_forward, 1.0f),
        m_V3f32_forward
    )));
} $unscoped(TEST_fn)
