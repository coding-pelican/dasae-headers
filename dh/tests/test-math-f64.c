#include "test-m-math/main.h"
#include "dh/m-math-linalg/Q-Quat/f64.h"

TEST_fn_("math: f64 matrix and quaternion support matches f32 conventions" $scope) {
    let angle = math_f64_pi * 0.5;
    let q = m_Q4f64_fromAxisAngle(m_V3f64_up, angle);

    try_(TEST_expect(math_test_V2f64Near(
        m_M2f64_mulV(m_M2f64_rotate(angle), m_V2f64_right),
        m_V2f64_up
    )));
    try_(TEST_expect(math_test_V3f64Near(
        m_Q4f64_rotateV(q, m_V3f64_forward),
        m_V3f64_right
    )));
    try_(TEST_expect(math_test_V3f64Near(
        m_Q4f64_rotateV(m_Q4f64_fromM4(m_Q4f64_toM4(q)), m_V3f64_forward),
        m_V3f64_right
    )));

    let view_rh = m_M4f64_lookAtRH(m_V3f64_zero, m_V3f64_view_forward_rh, m_V3f64_up);
    let view_lh = m_M4f64_lookAtLH(m_V3f64_zero, m_V3f64_view_forward_lh, m_V3f64_up);

    try_(TEST_expect(math_test_V3f64Near(
        m_M4f64_mulV3(view_rh, m_V3f64_view_forward_rh, 1.0),
        m_V3f64_view_forward_rh
    )));
    try_(TEST_expect(math_test_V3f64Near(
        m_M4f64_mulV3(view_lh, m_V3f64_view_forward_lh, 1.0),
        m_V3f64_view_forward_lh
    )));

    let proximal = 1.0;
    let distal = 11.0;
    let proj = m_M4f64_perspectRHZO(angle, 1.0, proximal, distal);
    try_(TEST_expect(math_test_f64Near(
        math_test_project64(proj, m_V3f64_of(0.0, 0.0, -proximal)).z,
        0.0
    )));
    try_(TEST_expect(math_test_f64Near(
        math_test_project64(proj, m_V3f64_of(0.0, 0.0, -distal)).z,
        1.0
    )));
} $unscoped(TEST_fn)
