#pragma once
#ifndef math_quat_types__included
#define math_quat_types__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "vec_types.h"

/*========== Macros and Definitions =========================================*/

typedef f32 m_Q4f32_S;
typedef m_V4f32 m_Q4f32_V;
typedef m_V3f32 m_Q4f32_Axis;
typedef union m_Q4f32 {
    var_(s, A$$(4, m_Q4f32_S));
    T_embed$(struct {
        m_Q4f32_S x, y, z, w;
    });
    m_Q4f32_V v;
} m_Q4f32;
typedef struct m_Q4f32_AxisAngle {
    m_Q4f32_Axis axis;
    m_Q4f32_S angle;
} m_Q4f32_AxisAngle;

typedef f64 m_Q4f64_S;
typedef m_V4f64 m_Q4f64_V;
typedef m_V3f64 m_Q4f64_Axis;
typedef union m_Q4f64 {
    var_(s, A$$(4, m_Q4f64_S));
    T_embed$(struct {
        m_Q4f64_S x, y, z, w;
    });
    m_Q4f64_V v;
} m_Q4f64;
typedef struct m_Q4f64_AxisAngle {
    m_Q4f64_Axis axis;
    m_Q4f64_S angle;
} m_Q4f64_AxisAngle;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* math_quat_types__included */
