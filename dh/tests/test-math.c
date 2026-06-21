#include "dh-main.h"
#include "dh/math.h"

$static fn_((math_test_f32Near(f32 lhs, f32 rhs))(bool)) {
    return flt_abs(lhs - rhs) <= 0.0001f;
};

$static fn_((math_test_f64Near(f64 lhs, f64 rhs))(bool)) {
    return flt_abs(lhs - rhs) <= 0.000000001;
};

$static fn_((math_test_V2f32Near(m_V2f32 lhs, m_V2f32 rhs))(bool)) {
    return math_test_f32Near(lhs.x, rhs.x)
        && math_test_f32Near(lhs.y, rhs.y);
};

$static fn_((math_test_V2f64Near(m_V2f64 lhs, m_V2f64 rhs))(bool)) {
    return math_test_f64Near(lhs.x, rhs.x)
        && math_test_f64Near(lhs.y, rhs.y);
};

$static fn_((math_test_V2i32Eq(m_V2i32 lhs, m_V2i32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y;
};

$static fn_((math_test_V3f32Near(m_V3f32 lhs, m_V3f32 rhs))(bool)) {
    return math_test_f32Near(lhs.x, rhs.x)
        && math_test_f32Near(lhs.y, rhs.y)
        && math_test_f32Near(lhs.z, rhs.z);
};

$static fn_((math_test_V3f64Near(m_V3f64 lhs, m_V3f64 rhs))(bool)) {
    return math_test_f64Near(lhs.x, rhs.x)
        && math_test_f64Near(lhs.y, rhs.y)
        && math_test_f64Near(lhs.z, rhs.z);
};

$static fn_((math_test_V3i32Eq(m_V3i32 lhs, m_V3i32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V3u32Eq(m_V3u32 lhs, m_V3u32 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V3u64Eq(m_V3u64 lhs, m_V3u64 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V3usizeEq(m_V3usize lhs, m_V3usize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V4usizeEq(m_V4usize lhs, m_V4usize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z
        && lhs.w == rhs.w;
};

$static fn_((math_test_M2i32Eq(m_M2i32 lhs, m_M2i32 rhs))(bool)) {
    return lhs.s0_0 == rhs.s0_0
        && lhs.s0_1 == rhs.s0_1
        && lhs.s1_0 == rhs.s1_0
        && lhs.s1_1 == rhs.s1_1;
};

$static fn_((math_test_M2usizeEq(m_M2usize lhs, m_M2usize rhs))(bool)) {
    return lhs.s0_0 == rhs.s0_0
        && lhs.s0_1 == rhs.s0_1
        && lhs.s1_0 == rhs.s1_0
        && lhs.s1_1 == rhs.s1_1;
};

$static fn_((math_test_V3i64Eq(m_V3i64 lhs, m_V3i64 rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V3isizeEq(m_V3isize lhs, m_V3isize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z;
};

$static fn_((math_test_V4isizeEq(m_V4isize lhs, m_V4isize rhs))(bool)) {
    return lhs.x == rhs.x
        && lhs.y == rhs.y
        && lhs.z == rhs.z
        && lhs.w == rhs.w;
};

$static fn_((math_test_project(m_M4f32 m, m_V3f32 p))(m_V3f32)) {
    let clip = m_M4f32_mulV(m, m_V4f32_of(p.x, p.y, p.z, 1.0f));
    return m_V3f32_of(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
};

$static fn_((math_test_project64(m_M4f64 m, m_V3f64 p))(m_V3f64)) {
    let clip = m_M4f64_mulV(m, m_V4f64_of(p.x, p.y, p.z, 1.0));
    return m_V3f64_of(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
};

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

TEST_fn_("math: perspective variants map RH LH and NO ZO depth ranges" $scope) {
    let fov_y = math_f32_pi * 0.5f;
    let aspect = 1.0f;
    let proximal = 1.0f;
    let distal = 11.0f;

    let rhno = m_M4f32_perspectRHNO(fov_y, aspect, proximal, distal);
    let rhzo = m_M4f32_perspectRHZO(fov_y, aspect, proximal, distal);
    let lhno = m_M4f32_perspectLHNO(fov_y, aspect, proximal, distal);
    let lhzo = m_M4f32_perspectLHZO(fov_y, aspect, proximal, distal);

    try_(TEST_expect(math_test_f32Near(math_test_project(rhno, m_V3f32_of(0.0f, 0.0f, -proximal)).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhno, m_V3f32_of(0.0f, 0.0f, -distal)).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhzo, m_V3f32_of(0.0f, 0.0f, -proximal)).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhzo, m_V3f32_of(0.0f, 0.0f, -distal)).z, 1.0f)));

    try_(TEST_expect(math_test_f32Near(math_test_project(lhno, m_V3f32_of(0.0f, 0.0f, proximal)).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhno, m_V3f32_of(0.0f, 0.0f, distal)).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhzo, m_V3f32_of(0.0f, 0.0f, proximal)).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhzo, m_V3f32_of(0.0f, 0.0f, distal)).z, 1.0f)));
} $unscoped(TEST_fn)

TEST_fn_("math: infinite-far perspective variants preserve near depth" $scope) {
    let fov_y = math_f32_pi * 0.5f;
    let aspect = 1.0f;
    let proximal = 1.0f;

    try_(TEST_expect(math_test_f32Near(
        math_test_project(m_M4f32_perspectInfFarRHNO(fov_y, aspect, proximal), m_V3f32_of(0.0f, 0.0f, -proximal)).z,
        -1.0f
    )));
    try_(TEST_expect(math_test_f32Near(
        math_test_project(m_M4f32_perspectInfFarRHZO(fov_y, aspect, proximal), m_V3f32_of(0.0f, 0.0f, -proximal)).z,
        0.0f
    )));
    try_(TEST_expect(math_test_f32Near(
        math_test_project(m_M4f32_perspectInfFarLHNO(fov_y, aspect, proximal), m_V3f32_of(0.0f, 0.0f, proximal)).z,
        -1.0f
    )));
    try_(TEST_expect(math_test_f32Near(
        math_test_project(m_M4f32_perspectInfFarLHZO(fov_y, aspect, proximal), m_V3f32_of(0.0f, 0.0f, proximal)).z,
        0.0f
    )));
} $unscoped(TEST_fn)

TEST_fn_("math: ortho variants map RH LH and NO ZO depth ranges" $scope) {
    let proximal = 1.0f;
    let distal = 11.0f;

    let rhno = m_M4f32_orthoRHNO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let rhzo = m_M4f32_orthoRHZO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let lhno = m_M4f32_orthoLHNO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let lhzo = m_M4f32_orthoLHZO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);

    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(rhno, m_V3f32_of(0.0f, 0.0f, -proximal), 1.0f).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(rhno, m_V3f32_of(0.0f, 0.0f, -distal), 1.0f).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(rhzo, m_V3f32_of(0.0f, 0.0f, -proximal), 1.0f).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(rhzo, m_V3f32_of(0.0f, 0.0f, -distal), 1.0f).z, 1.0f)));

    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(lhno, m_V3f32_of(0.0f, 0.0f, proximal), 1.0f).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(lhno, m_V3f32_of(0.0f, 0.0f, distal), 1.0f).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(lhzo, m_V3f32_of(0.0f, 0.0f, proximal), 1.0f).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(m_M4f32_mulV3(lhzo, m_V3f32_of(0.0f, 0.0f, distal), 1.0f).z, 1.0f)));
} $unscoped(TEST_fn)

TEST_fn_("math: frustum variants map RH LH and NO ZO depth ranges" $scope) {
    let proximal = 1.0f;
    let distal = 11.0f;

    let rhno = m_M4f32_frustumRHNO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let rhzo = m_M4f32_frustumRHZO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let lhno = m_M4f32_frustumLHNO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);
    let lhzo = m_M4f32_frustumLHZO(-1.0f, 1.0f, -1.0f, 1.0f, proximal, distal);

    try_(TEST_expect(math_test_f32Near(math_test_project(rhno, m_V3f32_of(0.0f, 0.0f, -proximal)).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhno, m_V3f32_of(0.0f, 0.0f, -distal)).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhzo, m_V3f32_of(0.0f, 0.0f, -proximal)).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(rhzo, m_V3f32_of(0.0f, 0.0f, -distal)).z, 1.0f)));

    try_(TEST_expect(math_test_f32Near(math_test_project(lhno, m_V3f32_of(0.0f, 0.0f, proximal)).z, -1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhno, m_V3f32_of(0.0f, 0.0f, distal)).z, 1.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhzo, m_V3f32_of(0.0f, 0.0f, proximal)).z, 0.0f)));
    try_(TEST_expect(math_test_f32Near(math_test_project(lhzo, m_V3f32_of(0.0f, 0.0f, distal)).z, 1.0f)));
} $unscoped(TEST_fn)

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

TEST_fn_("math: signed and floating vector SIMD paths preserve lane semantics" $scope) {
    try_(TEST_expect(math_test_V3i32Eq(
        m_V3i32_mod(m_V3i32_of(7, 8, 9), m_V3i32_of(3, 3, 4)),
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
        m_V3f32_mod(m_V3f32_of(7.5f, 8.5f, 9.5f), m_V3f32_of(3.0f, 3.0f, 4.0f)),
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

TEST_fn_("math: integer matrices support closed integer operations" $scope) {
    let lhs = m_M2i32_ofRows(
        m_V2i32_of(1, 2),
        m_V2i32_of(3, 4)
    );
    let rhs = m_M2i32_ofRows(
        m_V2i32_of(5, 6),
        m_V2i32_of(7, 8)
    );
    try_(TEST_expect(math_test_M2i32Eq(
        m_M2i32_mulM(lhs, rhs),
        m_M2i32_ofRows(
            m_V2i32_of(19, 22),
            m_V2i32_of(43, 50)
        )
    )));

    let signed_m3 = m_M3i64_ofRows(
        m_V3i64_of(1, 2, 3),
        m_V3i64_of(0, 1, 4),
        m_V3i64_of(5, 6, 0)
    );
    try_(TEST_expect(m_M3i64_det(signed_m3) == 1));

    let adjacency = m_M3u32_ofRows(
        m_V3u32_of(0, 1, 1),
        m_V3u32_of(1, 0, 0),
        m_V3u32_of(0, 1, 0)
    );
    try_(TEST_expect(math_test_V3u32Eq(
        m_M3u32_mulV(adjacency, m_V3u32_of(2, 3, 5)),
        m_V3u32_of(8, 2, 3)
    )));

    try_(TEST_expect(math_test_M2usizeEq(
        m_M2usize_transp(m_M2usize_ofRows(
            m_V2usize_of(1, 2),
            m_V2usize_of(3, 4)
        )),
        m_M2usize_ofRows(
            m_V2usize_of(1, 3),
            m_V2usize_of(2, 4)
        )
    )));
} $unscoped(TEST_fn)
