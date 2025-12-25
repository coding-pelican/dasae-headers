#ifndef math_mat_types__included
#define math_mat_types__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "vec_types.h"

/*========== Macros and Definitions =========================================*/

typedef f32 m_M2f32_S;
typedef m_V2f32 m_M2f32_V;
typedef m_V2f32 m_M2f32_ColV;
typedef m_V2f32 m_M2f32_RowV;
#define m_M2f32_cols 2
#define m_M2f32_rows 2
typedef union m_M2f32 {
    var_(cols, A$$(m_M2f32_cols, m_M2f32_V));
    var_(s, A$$(m_M2f32_cols, A$$(m_M2f32_rows, m_M2f32_S)));
    struct { /* s{row}_{col} */
        m_M2f32_S s0_0, s1_0;
        m_M2f32_S s0_1, s1_1;
    };
} m_M2f32;

typedef f32 m_M3f32_S;
typedef m_V3f32 m_M3f32_V;
typedef m_V3f32 m_M3f32_ColV;
typedef m_V3f32 m_M3f32_RowV;
#define m_M3f32_cols 3
#define m_M3f32_rows 3
typedef union m_M3f32 {
    var_(cols, A$$(m_M3f32_cols, m_M3f32_V));
    var_(s, A$$(m_M3f32_cols, A$$(m_M3f32_rows, m_M3f32_S)));
    struct { /* s{row}_{col} */
        m_M3f32_S s0_0, s1_0, s2_0;
        m_M3f32_S s0_1, s1_1, s2_1;
        m_M3f32_S s0_2, s1_2, s2_2;
    };
} m_M3f32;

typedef f32 m_M4f32_S;
typedef m_V4f32 m_M4f32_V;
typedef m_V4f32 m_M4f32_ColV;
typedef m_V4f32 m_M4f32_RowV;
#define m_M4f32_cols 4
#define m_M4f32_rows 4
typedef union m_M4f32 {
    var_(cols, A$$(m_M4f32_cols, m_M4f32_V));
    var_(s, A$$(m_M4f32_cols, A$$(m_M4f32_rows, m_M4f32_S)));
    struct { /* s{row}_{col} */
        m_M4f32_S s0_0, s1_0, s2_0, s3_0;
        m_M4f32_S s0_1, s1_1, s2_1, s3_1;
        m_M4f32_S s0_2, s1_2, s2_2, s3_2;
        m_M4f32_S s0_3, s1_3, s2_3, s3_3;
    };
} m_M4f32;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* math_mat_types__included */
