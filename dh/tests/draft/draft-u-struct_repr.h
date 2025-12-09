/**
 * struct_repr.h - Manual struct layout and repr system
 * 
 * Simulates C struct layout rules using TypeInfo and writes values
 * to byte buffers at computed offsets. Designed for constant folding.
 */

#ifndef STRUCT_REPR_H
#define STRUCT_REPR_H

/* ============================================================================
 * Primitive Types
 * ============================================================================ */

typedef unsigned char      u8;
typedef signed char        i8;
typedef unsigned short     u16;
typedef signed short       i16;
typedef unsigned int       u32;
typedef signed int         i32;
typedef unsigned long long u64;
typedef signed long long   i64;
typedef u64                usize;
typedef i64                isize;
typedef _Bool              bool;
typedef float              f32;
typedef double             f64;

#define true  ((_Bool)1)
#define false ((_Bool)0)

/* ============================================================================
 * Utility Macros
 * ============================================================================ */

#define as$(_T, _expr...)      ((_T)(_expr))
#define lit$(_expr...)         (_expr)

#define sizeOf$(_T...)         (as$(usize, sizeof(_T)))
#define alignOf$(_T...)        (as$(u32, (64u - 1u) - __builtin_clzll(_Alignof(_T))))

#define TypeOf(_Expr...)       __typeof__(_Expr)

/* ============================================================================
 * Slice Type
 * ============================================================================ */

typedef struct S_u8 {
    u8*   ptr;
    usize len;
} S$u8;

#define slice$(_arr...)        lit$((S$u8){ .ptr = (_arr), .len = sizeof(_arr) })
#define sliceFrom$(_ptr, _len) lit$((S$u8){ .ptr = (_ptr), .len = (_len) })

/* ============================================================================
 * TypeInfo - 8 bytes containing size and log2(alignment)
 * ============================================================================ */

typedef union TypeInfo {
    struct {
        u64 size  : 58;
        u64 align : 6;  /* log2 of alignment */
    };
    u64 packed;
} TypeInfo;

_Static_assert(sizeof(TypeInfo) == 8, "TypeInfo must be 8 bytes");

#define typeInfo$(_T...)       lit$((TypeInfo){ .size = sizeOf$(_T), .align = alignOf$(_T) })
#define TypeInfo_eq(_a, _b)    ((_a).packed == (_b).packed)

/* ============================================================================
 * Compile-Time TypeInfo Access (for _Static_assert and constant folding)
 * These extract size/align directly without going through bitfield struct
 * ============================================================================ */

/**
 * Pack size and align into u64 at compile time
 * Format: [size:58][align:6]
 */
#define TI_PACK$(_size, _align_log2) \
    ((((u64)(_size)) & 0x03FFFFFFFFFFFFFFull) | (((u64)(_align_log2)) << 58))

/**
 * Create compile-time constant TypeInfo packed value for a type
 */
#define TI_PACKED$(_T) \
    TI_PACK$(sizeof(_T), (64u - 1u) - __builtin_clzll(_Alignof(_T)))

/**
 * Extract size from packed TypeInfo (compile-time safe)
 */
#define TI_SIZE$(_packed)  ((_packed) & 0x03FFFFFFFFFFFFFFull)

/**
 * Extract align (log2) from packed TypeInfo (compile-time safe)
 */
#define TI_ALIGN$(_packed) ((u32)((_packed) >> 58))

/* ============================================================================
 * Layout Calculation - Designed for constant folding
 * ============================================================================ */

/**
 * Convert log2 alignment to actual alignment value
 * align=0 -> 1, align=1 -> 2, align=2 -> 4, align=3 -> 8, etc.
 */
#define alignVal$(_log2)       (1ull << (_log2))

/**
 * Align offset up to the given alignment (alignment must be power of 2)
 */
#define alignUp$(_offset, _align) \
    (((_offset) + (_align) - 1ull) & ~((_align) - 1ull))

/**
 * Calculate field offset given current end position and field's TypeInfo
 */
#define fieldOffset$(_current_end, _field_ti) \
    alignUp$((_current_end), alignVal$((_field_ti).align))

/**
 * Calculate next end position after placing a field
 */
#define fieldEnd$(_offset, _field_ti) \
    ((_offset) + (_field_ti).size)

/**
 * StructLayout - tracks current position during layout calculation
 */
typedef struct StructLayout {
    usize offset;      /* Current field's offset */
    usize end;         /* Position after current field */
    u32   max_align;   /* Maximum alignment seen (log2) */
} StructLayout;

#define StructLayout_init() \
    lit$((StructLayout){ .offset = 0, .end = 0, .max_align = 0 })

/**
 * Add a field to the layout, returns new layout with updated positions
 */
#define StructLayout_addField(_layout, _field_ti) \
    lit$((StructLayout){ \
        .offset    = fieldOffset$((_layout).end, (_field_ti)), \
        .end       = fieldOffset$((_layout).end, (_field_ti)) + (_field_ti).size, \
        .max_align = ((_layout).max_align > (_field_ti).align) \
                         ? (_layout).max_align : (_field_ti).align \
    })

/**
 * Finalize layout - returns total struct size (padded to alignment)
 */
#define StructLayout_size(_layout) \
    alignUp$((_layout).end, alignVal$((_layout).max_align))

#define StructLayout_align(_layout) ((_layout).max_align)

/* ============================================================================
 * Repr Functions - Write values to byte buffer
 * ============================================================================ */

/**
 * Write a value of arbitrary type to byte buffer at offset
 */
#define repr$(_slice, _offset, _value) do { \
    TypeOf(_value) _tmp_val_ = (_value); \
    __builtin_memcpy((_slice).ptr + (_offset), &_tmp_val_, sizeof(_tmp_val_)); \
} while(0)

/**
 * Read a value of type T from byte buffer at offset
 */
#define unrepr$(_T, _slice, _offset) ({ \
    _T _tmp_val_; \
    __builtin_memcpy(&_tmp_val_, (_slice).ptr + (_offset), sizeof(_T)); \
    _tmp_val_; \
})

/**
 * repr with explicit type
 */
#define reprAs$(_slice, _offset, _T, _value) do { \
    _T _tmp_val_ = as$(_T, _value); \
    __builtin_memcpy((_slice).ptr + (_offset), &_tmp_val_, sizeof(_T)); \
} while(0)

/* ============================================================================
 * Combined Layout + Repr Builder
 * ============================================================================ */

#define structRepr_begin() StructLayout_init()

/**
 * Add field and repr value - returns updated layout
 */
#define structRepr_field(_layout, _slice, _T, _value) ({ \
    StructLayout _new_ = StructLayout_addField((_layout), typeInfo$(_T)); \
    reprAs$((_slice), _new_.offset, _T, (_value)); \
    _new_; \
})

#define structRepr_end(_layout) StructLayout_size(_layout)

/* ============================================================================
 * Pure Offset Calculation Macros (compile-time friendly)
 * ============================================================================ */

#define OFFSET_0(_ti0) \
    fieldOffset$(0, (_ti0))

#define OFFSET_1(_ti0, _ti1) \
    fieldOffset$(fieldEnd$(OFFSET_0(_ti0), (_ti0)), (_ti1))

#define OFFSET_2(_ti0, _ti1, _ti2) \
    fieldOffset$(fieldEnd$(OFFSET_1(_ti0, _ti1), (_ti1)), (_ti2))

#define OFFSET_3(_ti0, _ti1, _ti2, _ti3) \
    fieldOffset$(fieldEnd$(OFFSET_2(_ti0, _ti1, _ti2), (_ti2)), (_ti3))

#define OFFSET_4(_ti0, _ti1, _ti2, _ti3, _ti4) \
    fieldOffset$(fieldEnd$(OFFSET_3(_ti0, _ti1, _ti2, _ti3), (_ti3)), (_ti4))

/* ============================================================================
 * Compile-Time Offset Calculation (using packed u64 TypeInfo)
 * These are true compile-time constants suitable for _Static_assert
 * ============================================================================ */

/**
 * Align offset up (compile-time version with explicit align)
 */
#define CT_ALIGN_UP$(_offset, _align_log2) \
    ((((_offset) + (1ull << (_align_log2)) - 1ull) & ~((1ull << (_align_log2)) - 1ull)))

/**
 * Field offset from packed TypeInfo (compile-time)
 */
#define CT_FIELD_OFFSET$(_end, _ti_packed) \
    CT_ALIGN_UP$((_end), TI_ALIGN$(_ti_packed))

/**
 * Field end position (compile-time)
 */
#define CT_FIELD_END$(_offset, _ti_packed) \
    ((_offset) + TI_SIZE$(_ti_packed))

/* Compile-time offset macros using packed TypeInfo */
#define CT_OFFSET_0$(_ti0) \
    CT_FIELD_OFFSET$(0, (_ti0))

#define CT_OFFSET_1$(_ti0, _ti1) \
    CT_FIELD_OFFSET$(CT_FIELD_END$(CT_OFFSET_0$(_ti0), (_ti0)), (_ti1))

#define CT_OFFSET_2$(_ti0, _ti1, _ti2) \
    CT_FIELD_OFFSET$(CT_FIELD_END$(CT_OFFSET_1$(_ti0, _ti1), (_ti1)), (_ti2))

#define CT_OFFSET_3$(_ti0, _ti1, _ti2, _ti3) \
    CT_FIELD_OFFSET$(CT_FIELD_END$(CT_OFFSET_2$(_ti0, _ti1, _ti2), (_ti2)), (_ti3))

#define CT_OFFSET_4$(_ti0, _ti1, _ti2, _ti3, _ti4) \
    CT_FIELD_OFFSET$(CT_FIELD_END$(CT_OFFSET_3$(_ti0, _ti1, _ti2, _ti3), (_ti3)), (_ti4))

/* Compile-time max alignment (returns log2) */
#define CT_MAX_ALIGN2$(_ti0, _ti1) \
    ((TI_ALIGN$(_ti0) > TI_ALIGN$(_ti1)) ? TI_ALIGN$(_ti0) : TI_ALIGN$(_ti1))

#define CT_MAX_ALIGN3$(_ti0, _ti1, _ti2) \
    CT_MAX_ALIGN2$(CT_MAX_ALIGN2$(_ti0, _ti1) > TI_ALIGN$(_ti2) ? (_ti0) : TI_PACK$(0, TI_ALIGN$(_ti2)), (_ti1))

#define CT_MAX_ALIGN4$(_ti0, _ti1, _ti2, _ti3) \
    ((CT_MAX_ALIGN2$(_ti0, _ti1) > CT_MAX_ALIGN2$(_ti2, _ti3)) \
        ? CT_MAX_ALIGN2$(_ti0, _ti1) : CT_MAX_ALIGN2$(_ti2, _ti3))

/* Compile-time struct size calculation */
#define CT_STRUCT_SIZE_1$(_ti0) \
    CT_ALIGN_UP$(CT_FIELD_END$(CT_OFFSET_0$(_ti0), (_ti0)), TI_ALIGN$(_ti0))

#define CT_STRUCT_SIZE_2$(_ti0, _ti1) \
    CT_ALIGN_UP$(CT_FIELD_END$(CT_OFFSET_1$(_ti0, _ti1), (_ti1)), \
                 CT_MAX_ALIGN2$(_ti0, _ti1))

#define CT_STRUCT_SIZE_4$(_ti0, _ti1, _ti2, _ti3) \
    CT_ALIGN_UP$(CT_FIELD_END$(CT_OFFSET_3$(_ti0, _ti1, _ti2, _ti3), (_ti3)), \
                 CT_MAX_ALIGN4$(_ti0, _ti1, _ti2, _ti3))

/* Total struct size calculation */
#define STRUCT_SIZE_1(_ti0) \
    alignUp$(fieldEnd$(OFFSET_0(_ti0), (_ti0)), \
             alignVal$((_ti0).align))

#define STRUCT_SIZE_2(_ti0, _ti1) \
    alignUp$(fieldEnd$(OFFSET_1(_ti0, _ti1), (_ti1)), \
             alignVal$(((_ti0).align > (_ti1).align) ? (_ti0).align : (_ti1).align))

#define STRUCT_SIZE_3(_ti0, _ti1, _ti2) ({ \
    u32 _ma = (_ti0).align; \
    if ((_ti1).align > _ma) _ma = (_ti1).align; \
    if ((_ti2).align > _ma) _ma = (_ti2).align; \
    alignUp$(fieldEnd$(OFFSET_2(_ti0, _ti1, _ti2), (_ti2)), alignVal$(_ma)); \
})

#define STRUCT_SIZE_4(_ti0, _ti1, _ti2, _ti3) ({ \
    u32 _ma = (_ti0).align; \
    if ((_ti1).align > _ma) _ma = (_ti1).align; \
    if ((_ti2).align > _ma) _ma = (_ti2).align; \
    if ((_ti3).align > _ma) _ma = (_ti3).align; \
    alignUp$(fieldEnd$(OFFSET_3(_ti0, _ti1, _ti2, _ti3), (_ti3)), alignVal$(_ma)); \
})

/* ============================================================================
 * Max alignment helper (for struct size calculation)
 * ============================================================================ */

#define maxAlign2$(_a, _b)         (((_a) > (_b)) ? (_a) : (_b))
#define maxAlign3$(_a, _b, _c)     maxAlign2$(maxAlign2$(_a, _b), (_c))
#define maxAlign4$(_a, _b, _c, _d) maxAlign2$(maxAlign3$(_a, _b, _c), (_d))

#endif /* STRUCT_REPR_H */
