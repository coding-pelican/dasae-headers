#include "test-m-math/main.h"

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
