#ifndef math_quat_types__included
#define math_quat_types__included 1

/*========== Includes =======================================================*/

#include "vec_types.h"

/*========== Macros and Declarations ========================================*/

typedef f32 m_Q4f32_S;
typedef m_V4f32 m_Q4f32_V;
typedef m_V3f32 m_Q4f32_Axis;
typedef union m_Q4f32 {
    var_(a, A$$(4, m_Q4f32_S));
    m_Q4f32_S s[4];
    struct {
        m_Q4f32_S x, y, z, w;
    };
    m_Q4f32_V v;
} m_Q4f32;
typedef struct m_Q4f32_AxisAngle {
    m_Q4f32_Axis axis;
    m_Q4f32_S angle;
} m_Q4f32_AxisAngle;

#endif /* math_quat_types__included */
