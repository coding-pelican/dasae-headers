#pragma once
#ifndef m_math_linalg_M_Mat_types__included
#define m_math_linalg_M_Mat_types__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../V-Vec/types.h"

/*========== Macros and Definitions =========================================*/

typedef u32 m_M2u32_S;
typedef m_V2u32 m_M2u32_V;
typedef m_V2u32 m_M2u32_ColV;
typedef m_V2u32 m_M2u32_RowV;
#define m_M2u32_cols 2
#define m_M2u32_rows 2
typedef union m_M2u32 {
    var_(cols, A$$(m_M2u32_cols, m_M2u32_V));
    var_(s, A$$(m_M2u32_cols, A$$(m_M2u32_rows, m_M2u32_S)));
    T_embed$(struct {
        m_M2u32_S s0_0, s1_0;
        m_M2u32_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2u32;

typedef u32 m_M3u32_S;
typedef m_V3u32 m_M3u32_V;
typedef m_V3u32 m_M3u32_ColV;
typedef m_V3u32 m_M3u32_RowV;
#define m_M3u32_cols 3
#define m_M3u32_rows 3
typedef union m_M3u32 {
    var_(cols, A$$(m_M3u32_cols, m_M3u32_V));
    var_(s, A$$(m_M3u32_cols, A$$(m_M3u32_rows, m_M3u32_S)));
    T_embed$(struct {
        m_M3u32_S s0_0, s1_0, s2_0;
        m_M3u32_S s0_1, s1_1, s2_1;
        m_M3u32_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3u32;

typedef u32 m_M4u32_S;
typedef m_V4u32 m_M4u32_V;
typedef m_V4u32 m_M4u32_ColV;
typedef m_V4u32 m_M4u32_RowV;
#define m_M4u32_cols 4
#define m_M4u32_rows 4
typedef union m_M4u32 {
    var_(cols, A$$(m_M4u32_cols, m_M4u32_V));
    var_(s, A$$(m_M4u32_cols, A$$(m_M4u32_rows, m_M4u32_S)));
    T_embed$(struct {
        m_M4u32_S s0_0, s1_0, s2_0, s3_0;
        m_M4u32_S s0_1, s1_1, s2_1, s3_1;
        m_M4u32_S s0_2, s1_2, s2_2, s3_2;
        m_M4u32_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4u32;

typedef u64 m_M2u64_S;
typedef m_V2u64 m_M2u64_V;
typedef m_V2u64 m_M2u64_ColV;
typedef m_V2u64 m_M2u64_RowV;
#define m_M2u64_cols 2
#define m_M2u64_rows 2
typedef union m_M2u64 {
    var_(cols, A$$(m_M2u64_cols, m_M2u64_V));
    var_(s, A$$(m_M2u64_cols, A$$(m_M2u64_rows, m_M2u64_S)));
    T_embed$(struct {
        m_M2u64_S s0_0, s1_0;
        m_M2u64_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2u64;

typedef u64 m_M3u64_S;
typedef m_V3u64 m_M3u64_V;
typedef m_V3u64 m_M3u64_ColV;
typedef m_V3u64 m_M3u64_RowV;
#define m_M3u64_cols 3
#define m_M3u64_rows 3
typedef union m_M3u64 {
    var_(cols, A$$(m_M3u64_cols, m_M3u64_V));
    var_(s, A$$(m_M3u64_cols, A$$(m_M3u64_rows, m_M3u64_S)));
    T_embed$(struct {
        m_M3u64_S s0_0, s1_0, s2_0;
        m_M3u64_S s0_1, s1_1, s2_1;
        m_M3u64_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3u64;

typedef u64 m_M4u64_S;
typedef m_V4u64 m_M4u64_V;
typedef m_V4u64 m_M4u64_ColV;
typedef m_V4u64 m_M4u64_RowV;
#define m_M4u64_cols 4
#define m_M4u64_rows 4
typedef union m_M4u64 {
    var_(cols, A$$(m_M4u64_cols, m_M4u64_V));
    var_(s, A$$(m_M4u64_cols, A$$(m_M4u64_rows, m_M4u64_S)));
    T_embed$(struct {
        m_M4u64_S s0_0, s1_0, s2_0, s3_0;
        m_M4u64_S s0_1, s1_1, s2_1, s3_1;
        m_M4u64_S s0_2, s1_2, s2_2, s3_2;
        m_M4u64_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4u64;

typedef usize m_M2usize_S;
typedef m_V2usize m_M2usize_V;
typedef m_V2usize m_M2usize_ColV;
typedef m_V2usize m_M2usize_RowV;
#define m_M2usize_cols 2
#define m_M2usize_rows 2
typedef union m_M2usize {
    var_(cols, A$$(m_M2usize_cols, m_M2usize_V));
    var_(s, A$$(m_M2usize_cols, A$$(m_M2usize_rows, m_M2usize_S)));
    T_embed$(struct {
        m_M2usize_S s0_0, s1_0;
        m_M2usize_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2usize;

typedef usize m_M3usize_S;
typedef m_V3usize m_M3usize_V;
typedef m_V3usize m_M3usize_ColV;
typedef m_V3usize m_M3usize_RowV;
#define m_M3usize_cols 3
#define m_M3usize_rows 3
typedef union m_M3usize {
    var_(cols, A$$(m_M3usize_cols, m_M3usize_V));
    var_(s, A$$(m_M3usize_cols, A$$(m_M3usize_rows, m_M3usize_S)));
    T_embed$(struct {
        m_M3usize_S s0_0, s1_0, s2_0;
        m_M3usize_S s0_1, s1_1, s2_1;
        m_M3usize_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3usize;

typedef usize m_M4usize_S;
typedef m_V4usize m_M4usize_V;
typedef m_V4usize m_M4usize_ColV;
typedef m_V4usize m_M4usize_RowV;
#define m_M4usize_cols 4
#define m_M4usize_rows 4
typedef union m_M4usize {
    var_(cols, A$$(m_M4usize_cols, m_M4usize_V));
    var_(s, A$$(m_M4usize_cols, A$$(m_M4usize_rows, m_M4usize_S)));
    T_embed$(struct {
        m_M4usize_S s0_0, s1_0, s2_0, s3_0;
        m_M4usize_S s0_1, s1_1, s2_1, s3_1;
        m_M4usize_S s0_2, s1_2, s2_2, s3_2;
        m_M4usize_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4usize;

typedef i32 m_M2i32_S;
typedef m_V2i32 m_M2i32_V;
typedef m_V2i32 m_M2i32_ColV;
typedef m_V2i32 m_M2i32_RowV;
#define m_M2i32_cols 2
#define m_M2i32_rows 2
typedef union m_M2i32 {
    var_(cols, A$$(m_M2i32_cols, m_M2i32_V));
    var_(s, A$$(m_M2i32_cols, A$$(m_M2i32_rows, m_M2i32_S)));
    T_embed$(struct {
        m_M2i32_S s0_0, s1_0;
        m_M2i32_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2i32;

typedef i32 m_M3i32_S;
typedef m_V3i32 m_M3i32_V;
typedef m_V3i32 m_M3i32_ColV;
typedef m_V3i32 m_M3i32_RowV;
#define m_M3i32_cols 3
#define m_M3i32_rows 3
typedef union m_M3i32 {
    var_(cols, A$$(m_M3i32_cols, m_M3i32_V));
    var_(s, A$$(m_M3i32_cols, A$$(m_M3i32_rows, m_M3i32_S)));
    T_embed$(struct {
        m_M3i32_S s0_0, s1_0, s2_0;
        m_M3i32_S s0_1, s1_1, s2_1;
        m_M3i32_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3i32;

typedef i32 m_M4i32_S;
typedef m_V4i32 m_M4i32_V;
typedef m_V4i32 m_M4i32_ColV;
typedef m_V4i32 m_M4i32_RowV;
#define m_M4i32_cols 4
#define m_M4i32_rows 4
typedef union m_M4i32 {
    var_(cols, A$$(m_M4i32_cols, m_M4i32_V));
    var_(s, A$$(m_M4i32_cols, A$$(m_M4i32_rows, m_M4i32_S)));
    T_embed$(struct {
        m_M4i32_S s0_0, s1_0, s2_0, s3_0;
        m_M4i32_S s0_1, s1_1, s2_1, s3_1;
        m_M4i32_S s0_2, s1_2, s2_2, s3_2;
        m_M4i32_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4i32;

typedef i64 m_M2i64_S;
typedef m_V2i64 m_M2i64_V;
typedef m_V2i64 m_M2i64_ColV;
typedef m_V2i64 m_M2i64_RowV;
#define m_M2i64_cols 2
#define m_M2i64_rows 2
typedef union m_M2i64 {
    var_(cols, A$$(m_M2i64_cols, m_M2i64_V));
    var_(s, A$$(m_M2i64_cols, A$$(m_M2i64_rows, m_M2i64_S)));
    T_embed$(struct {
        m_M2i64_S s0_0, s1_0;
        m_M2i64_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2i64;

typedef i64 m_M3i64_S;
typedef m_V3i64 m_M3i64_V;
typedef m_V3i64 m_M3i64_ColV;
typedef m_V3i64 m_M3i64_RowV;
#define m_M3i64_cols 3
#define m_M3i64_rows 3
typedef union m_M3i64 {
    var_(cols, A$$(m_M3i64_cols, m_M3i64_V));
    var_(s, A$$(m_M3i64_cols, A$$(m_M3i64_rows, m_M3i64_S)));
    T_embed$(struct {
        m_M3i64_S s0_0, s1_0, s2_0;
        m_M3i64_S s0_1, s1_1, s2_1;
        m_M3i64_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3i64;

typedef i64 m_M4i64_S;
typedef m_V4i64 m_M4i64_V;
typedef m_V4i64 m_M4i64_ColV;
typedef m_V4i64 m_M4i64_RowV;
#define m_M4i64_cols 4
#define m_M4i64_rows 4
typedef union m_M4i64 {
    var_(cols, A$$(m_M4i64_cols, m_M4i64_V));
    var_(s, A$$(m_M4i64_cols, A$$(m_M4i64_rows, m_M4i64_S)));
    T_embed$(struct {
        m_M4i64_S s0_0, s1_0, s2_0, s3_0;
        m_M4i64_S s0_1, s1_1, s2_1, s3_1;
        m_M4i64_S s0_2, s1_2, s2_2, s3_2;
        m_M4i64_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4i64;

typedef isize m_M2isize_S;
typedef m_V2isize m_M2isize_V;
typedef m_V2isize m_M2isize_ColV;
typedef m_V2isize m_M2isize_RowV;
#define m_M2isize_cols 2
#define m_M2isize_rows 2
typedef union m_M2isize {
    var_(cols, A$$(m_M2isize_cols, m_M2isize_V));
    var_(s, A$$(m_M2isize_cols, A$$(m_M2isize_rows, m_M2isize_S)));
    T_embed$(struct {
        m_M2isize_S s0_0, s1_0;
        m_M2isize_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2isize;

typedef isize m_M3isize_S;
typedef m_V3isize m_M3isize_V;
typedef m_V3isize m_M3isize_ColV;
typedef m_V3isize m_M3isize_RowV;
#define m_M3isize_cols 3
#define m_M3isize_rows 3
typedef union m_M3isize {
    var_(cols, A$$(m_M3isize_cols, m_M3isize_V));
    var_(s, A$$(m_M3isize_cols, A$$(m_M3isize_rows, m_M3isize_S)));
    T_embed$(struct {
        m_M3isize_S s0_0, s1_0, s2_0;
        m_M3isize_S s0_1, s1_1, s2_1;
        m_M3isize_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3isize;

typedef isize m_M4isize_S;
typedef m_V4isize m_M4isize_V;
typedef m_V4isize m_M4isize_ColV;
typedef m_V4isize m_M4isize_RowV;
#define m_M4isize_cols 4
#define m_M4isize_rows 4
typedef union m_M4isize {
    var_(cols, A$$(m_M4isize_cols, m_M4isize_V));
    var_(s, A$$(m_M4isize_cols, A$$(m_M4isize_rows, m_M4isize_S)));
    T_embed$(struct {
        m_M4isize_S s0_0, s1_0, s2_0, s3_0;
        m_M4isize_S s0_1, s1_1, s2_1, s3_1;
        m_M4isize_S s0_2, s1_2, s2_2, s3_2;
        m_M4isize_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4isize;

typedef f32 m_M2f32_S;
typedef m_V2f32 m_M2f32_V;
typedef m_V2f32 m_M2f32_ColV;
typedef m_V2f32 m_M2f32_RowV;
#define m_M2f32_cols 2
#define m_M2f32_rows 2
typedef union m_M2f32 {
    var_(cols, A$$(m_M2f32_cols, m_M2f32_V));
    var_(s, A$$(m_M2f32_cols, A$$(m_M2f32_rows, m_M2f32_S)));
    T_embed$(struct {
        m_M2f32_S s0_0, s1_0;
        m_M2f32_S s0_1, s1_1;
    }); /* s{row}_{col} */
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
    T_embed$(struct {
        m_M3f32_S s0_0, s1_0, s2_0;
        m_M3f32_S s0_1, s1_1, s2_1;
        m_M3f32_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
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
    T_embed$(struct {
        m_M4f32_S s0_0, s1_0, s2_0, s3_0;
        m_M4f32_S s0_1, s1_1, s2_1, s3_1;
        m_M4f32_S s0_2, s1_2, s2_2, s3_2;
        m_M4f32_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4f32;

typedef f64 m_M2f64_S;
typedef m_V2f64 m_M2f64_V;
typedef m_V2f64 m_M2f64_ColV;
typedef m_V2f64 m_M2f64_RowV;
#define m_M2f64_cols 2
#define m_M2f64_rows 2
typedef union m_M2f64 {
    var_(cols, A$$(m_M2f64_cols, m_M2f64_V));
    var_(s, A$$(m_M2f64_cols, A$$(m_M2f64_rows, m_M2f64_S)));
    T_embed$(struct {
        m_M2f64_S s0_0, s1_0;
        m_M2f64_S s0_1, s1_1;
    }); /* s{row}_{col} */
} m_M2f64;

typedef f64 m_M3f64_S;
typedef m_V3f64 m_M3f64_V;
typedef m_V3f64 m_M3f64_ColV;
typedef m_V3f64 m_M3f64_RowV;
#define m_M3f64_cols 3
#define m_M3f64_rows 3
typedef union m_M3f64 {
    var_(cols, A$$(m_M3f64_cols, m_M3f64_V));
    var_(s, A$$(m_M3f64_cols, A$$(m_M3f64_rows, m_M3f64_S)));
    T_embed$(struct {
        m_M3f64_S s0_0, s1_0, s2_0;
        m_M3f64_S s0_1, s1_1, s2_1;
        m_M3f64_S s0_2, s1_2, s2_2;
    }); /* s{row}_{col} */
} m_M3f64;

typedef f64 m_M4f64_S;
typedef m_V4f64 m_M4f64_V;
typedef m_V4f64 m_M4f64_ColV;
typedef m_V4f64 m_M4f64_RowV;
#define m_M4f64_cols 4
#define m_M4f64_rows 4
typedef union m_M4f64 {
    var_(cols, A$$(m_M4f64_cols, m_M4f64_V));
    var_(s, A$$(m_M4f64_cols, A$$(m_M4f64_rows, m_M4f64_S)));
    T_embed$(struct {
        m_M4f64_S s0_0, s1_0, s2_0, s3_0;
        m_M4f64_S s0_1, s1_1, s2_1, s3_1;
        m_M4f64_S s0_2, s1_2, s2_2, s3_2;
        m_M4f64_S s0_3, s1_3, s2_3, s3_3;
    }); /* s{row}_{col} */
} m_M4f64;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* m_math_linalg_M_Mat_types__included */
