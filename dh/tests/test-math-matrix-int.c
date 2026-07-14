#include "test-m-math/main.h"
#include "dh/m-math-linalg/M-Mat-int.h"

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
