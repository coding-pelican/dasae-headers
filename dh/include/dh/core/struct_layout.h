/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    struct_layout.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-08 (date of creation)
 * @updated 2025-06-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  StructLayout
 *
 * @brief   Compile-time struct layout calculation
 * @details Provides compile-time struct layout calculation using packed TypeInfo:
 *          - Field offset calculation
 *          - Struct size calculation
 *          - Alignment utilities
 */
#ifndef core_struct_layout__included
#define core_struct_layout__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "type_info.h"

/*========== Macros and Definitions =========================================*/

/* Alignment Utilities */

/// Align offset up using log2 alignment (compile-time safe)
#define StructLayout_alignUp(_offset, _align_log2) \
    ((((_offset) + (1ull << (_align_log2)) - 1ull) & ~((1ull << (_align_log2)) - 1ull)))

/* Field Layout Calculation (using packed TypeInfo) */

/// Field offset from packed TypeInfo
#define StructLayoutPacked_fieldOffset(_end, _ti_packed) \
    StructLayout_alignUp((_end), TypeInfoPacked_align(_ti_packed))

/// Field end position from packed TypeInfo
#define StructLayoutPacked_fieldEnd(_offset, _ti_packed) \
    ((_offset) + TypeInfoPacked_size(_ti_packed))

/* Compute offset of field N (0-indexed) from packed TypeInfo sequence */

#define StructLayoutPacked_offset0(_tip0) \
    StructLayoutPacked_fieldOffset(0, (_tip0))

#define StructLayoutPacked_offset1(_tip0, _tip1) \
    StructLayoutPacked_fieldOffset( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset0(_tip0), (_tip0)), (_tip1) \
    )

#define StructLayoutPacked_offset2(_tip0, _tip1, _tip2) \
    StructLayoutPacked_fieldOffset( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset1(_tip0, _tip1), (_tip1)), (_tip2) \
    )

#define StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3) \
    StructLayoutPacked_fieldOffset( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset2(_tip0, _tip1, _tip2), (_tip2)), (_tip3) \
    )

#define StructLayoutPacked_offset4(_tip0, _tip1, _tip2, _tip3, _tip4) \
    StructLayoutPacked_fieldOffset( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3), (_tip3)), (_tip4) \
    )

/* Max alignment from packed TypeInfo */

#define StructLayoutPacked_maxAlign2(_tip0, _tip1) \
    ((TypeInfoPacked_align(_tip0) > TypeInfoPacked_align(_tip1)) \
         ? TypeInfoPacked_align(_tip0) \
         : TypeInfoPacked_align(_tip1))

#define StructLayoutPacked_maxAlign3(_tip0, _tip1, _tip2) \
    StructLayoutPacked_maxAlign2( \
        StructLayoutPacked_maxAlign2(_tip0, _tip1) > TypeInfoPacked_align(_tip2) \
            ? (_tip0) \
            : packTypeInfo$(u8[1ull << TypeInfoPacked_align(_tip2)]), \
        (_tip1) \
    )

#define StructLayoutPacked_maxAlign4(_tip0, _tip1, _tip2, _tip3) \
    ((StructLayoutPacked_maxAlign2(_tip0, _tip1) > StructLayoutPacked_maxAlign2(_tip2, _tip3)) \
         ? StructLayoutPacked_maxAlign2(_tip0, _tip1) \
         : StructLayoutPacked_maxAlign2(_tip2, _tip3))

/* Struct size from packed TypeInfo sequence */

#define StructLayoutPacked_structSize1(_tip0) \
    StructLayout_alignUp( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset0(_tip0), (_tip0)), \
        TypeInfoPacked_align(_tip0) \
    )

#define StructLayoutPacked_structSize2(_tip0, _tip1) \
    StructLayout_alignUp( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset1(_tip0, _tip1), (_tip1)), \
        StructLayoutPacked_maxAlign2(_tip0, _tip1) \
    )

#define StructLayoutPacked_structSize3(_tip0, _tip1, _tip2) \
    StructLayout_alignUp( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset2(_tip0, _tip1, _tip2), (_tip2)), \
        StructLayoutPacked_maxAlign2(StructLayoutPacked_maxAlign2(_tip0, _tip1) > TypeInfoPacked_align(_tip2) ? (_tip0) : packTypeInfo$(u8[1ull << TypeInfoPacked_align(_tip2)]), (_tip1)) \
    )

#define StructLayoutPacked_structSize4(_tip0, _tip1, _tip2, _tip3) \
    StructLayout_alignUp( \
        StructLayoutPacked_fieldEnd(StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3), (_tip3)), \
        StructLayoutPacked_maxAlign4(_tip0, _tip1, _tip2, _tip3) \
    )

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_struct_layout__included */
