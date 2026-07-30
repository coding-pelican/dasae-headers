/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    bitfield.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
 *
 * @brief   Canonical bit-field and packed-integer declaration syntax
 * @details A field list is written once in most-significant to least-significant
 *          order. The declaration is emitted in the native C bit-field order,
 *          while the packed integer keeps the same numeric bit meaning on every
 *          supported byte order. Unused high bits are reserved automatically.
 */
#pragma once
#ifndef core_pri_bitfield__included
#define core_pri_bitfield__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "switch.h"

/*========== Macros and Declarations ========================================*/

/**
 * Declare one native bit-field/packed-integer union.
 *
 * Fields are listed in canonical big-endian order (MSB -> LSB):
 *
 * bitfield_((Alias)(PackedInt)(
 *     (field_a, FieldIntA, field_a_bits),
 *     (field_b, FieldIntB, field_b_bits)
 * ));
 *
 * The declaration emits:
 * - `bitfield_{size|shift|mask}$<field>$<Alias>` enum constants,
 * - `bitfield_{size|storage|reserved|count}$<Alias>` enum constants,
 * - `typedef union Alias { struct { ... }; PackedInt packed; } Alias`.
 */
#define bitfield_(/*(_$Alias)(_$PackedInt)((_$field, _$FieldInt, _$bits),...)*/...) \
    __stmt__bitfield_(__VA_ARGS__)

#define bitfield_bits_(_$field, _$Alias...) __alias__bitfield_bits_(_$field, _$Alias)
#define bitfield_shift_(_$field, _$Alias...) __alias__bitfield_shift_(_$field, _$Alias)
#define bitfield_mask_(_$field, _$Alias...) __alias__bitfield_mask_(_$field, _$Alias)

#define bitfield_bits$(_$Alias...) __alias__bitfield_bits$(_$Alias)
#define bitfield_storage$(_$Alias...) __alias__bitfield_storage$(_$Alias)
#define bitfield_reserved$(_$Alias...) __alias__bitfield_reserved$(_$Alias)
#define bitfield_count$(_$Alias...) __alias__bitfield_count$(_$Alias)

claim_assert_static(arch_byte_order_is_little_endian || arch_byte_order_is_big_endian);

/*========== Macros and Definitions =========================================*/

#define __alias__bitfield_bits_(_$field, _$Alias...) pp_join2($, pp_join2(_, bitfield_bits, _$field), _$Alias)
#define __alias__bitfield_shift_(_$field, _$Alias...) pp_join2($, pp_join2(_, bitfield_shift, _$field), _$Alias)
#define __alias__bitfield_mask_(_$field, _$Alias...) pp_join2($, pp_join2(_, bitfield_mask, _$field), _$Alias)

#define __alias__bitfield_bits$(_$Alias...) pp_join2($, bitfield_bits, _$Alias)
#define __alias__bitfield_storage$(_$Alias...) pp_join2($, bitfield_storage, _$Alias)
#define __alias__bitfield_reserved$(_$Alias...) pp_join2($, bitfield_reserved, _$Alias)
#define __alias__bitfield_count$(_$Alias...) pp_join2($, bitfield_count, _$Alias)

#define __bitfield___unwrap(...) __VA_ARGS__

#define __bitfield___field_name(_$Field) \
    __bitfield___field_name__emit(__bitfield___unwrap _$Field)
#define __bitfield___field_name__emit(...) \
    __bitfield___field_name__impl(__VA_ARGS__)
#define __bitfield___field_name__impl(_$name, _$T, _$bits...) _$name

#define __bitfield___field_bits(_$Field) \
    __bitfield___field_bits__emit(__bitfield___unwrap _$Field)
#define __bitfield___field_bits__emit(...) \
    __bitfield___field_bits__impl(__VA_ARGS__)
#define __bitfield___field_bits__impl(_$name, _$T, _$bits...) (_$bits)

#define __bitfield___field_decl(_$ignored, _$Field) \
    __bitfield___field_decl__emit(__bitfield___unwrap _$Field)
#define __bitfield___field_decl__emit(...) \
    __bitfield___field_decl__impl(__VA_ARGS__)
#define __bitfield___field_decl__impl(_$name, _$T, _$bits...) \
    var_(_$name : (_$bits), _$T);

#define __bitfield___field_decl_rev(_$acc, _$Field) \
    __bitfield___field_decl(~, _$Field) _$acc

/* Fold right so each field can use the width of every field to its right as
 * its numeric packed shift. The accumulator is:
 * `(Alias, PackedInt, tail_bits, emitted_enum_items...)`. */
#define __bitfield___meta_step(_$Field, _$Acc) \
    __bitfield___meta_step__emit(_$Field, __bitfield___unwrap _$Acc)
#define __bitfield___meta_step__emit(...) \
    __bitfield___meta_step__impl(__VA_ARGS__)
#define __bitfield___meta_step__impl(_$Field, _$Alias, _$PackedInt, _$tail_bits, ...) \
    __bitfield___meta_step__result(_$Field, _$Alias, _$PackedInt, _$tail_bits, __VA_ARGS__)
#define __bitfield___meta_step__result(_$Field, _$Alias, _$PackedInt, _$tail_bits, ...) ( \
    _$Alias, \
    _$PackedInt, \
    (__bitfield___field_bits(_$Field) + (_$tail_bits)), \
    bitfield_bits_(__bitfield___field_name(_$Field), _$Alias) = __bitfield___field_bits(_$Field), \
    bitfield_shift_(__bitfield___field_name(_$Field), _$Alias) = (_$tail_bits), \
    bitfield_mask_(__bitfield___field_name(_$Field), _$Alias) \
    = (int_maskLo_static$((_$PackedInt)(__bitfield___field_bits(_$Field))) << (_$tail_bits)), \
    __VA_ARGS__ \
)

#define __bitfield___meta(_$Alias, _$PackedInt, ...) \
    pp_foldr(__bitfield___meta_step, (_$Alias, _$PackedInt, 0, ), __VA_ARGS__)

#define __bitfield___meta_items(_$Meta) \
    __bitfield___meta_items__emit(__bitfield___unwrap _$Meta)
#define __bitfield___meta_items__emit(...) \
    __bitfield___meta_items__impl(__VA_ARGS__)
#define __bitfield___meta_items__impl(_$Alias, _$PackedInt, _$bits, ...) __VA_ARGS__

#define __bitfield___meta_bits(_$Meta) \
    __bitfield___meta_bits__emit(__bitfield___unwrap _$Meta)
#define __bitfield___meta_bits__emit(...) \
    __bitfield___meta_bits__impl(__VA_ARGS__)
#define __bitfield___meta_bits__impl(_$Alias, _$PackedInt, _$bits, ...) (_$bits)

#define __stmt__bitfield_(...) \
    __bitfield___emit_parse(__bitfield___parseAlias __VA_ARGS__)
#define __bitfield___parseAlias(_$Alias...) _$Alias, __bitfield___parsePackedInt
#define __bitfield___parsePackedInt(_$PackedInt...) _$PackedInt, __bitfield___parseFields
#define __bitfield___parseFields(_$Fields...) _$Fields
#define __bitfield___emit_parse(...) __bitfield___emit(__VA_ARGS__)

#define __bitfield___emit(_$Alias, _$PackedInt, ...) \
    enum { \
        __bitfield___meta_items(__bitfield___meta(_$Alias, _$PackedInt, __VA_ARGS__)) \
            bitfield_bits$(_$Alias) = __bitfield___meta_bits(__bitfield___meta(_$Alias, _$PackedInt, __VA_ARGS__)), \
        bitfield_storage$(_$Alias) = int_bits$(_$PackedInt), \
        bitfield_reserved$(_$Alias) = bitfield_storage$(_$Alias) - bitfield_bits$(_$Alias), \
        bitfield_count$(_$Alias) = pp_countArg(__VA_ARGS__) \
    }; \
    typedef union _$Alias { \
        struct { \
            pp_if_(arch_byte_order_is_little_endian)( \
                pp_then_( \
                    pp_foldl(__bitfield___field_decl_rev, , __VA_ARGS__) \
                        _$PackedInt : pp_join2($, bitfield_reserved, _$Alias); \
                ), \
                pp_else_( \
                    _$PackedInt : pp_join2($, bitfield_reserved, _$Alias); \
                    pp_foreach(__bitfield___field_decl, ~, __VA_ARGS__) \
                ) \
            ) \
        }; \
        var_(packed, _$PackedInt); \
    } _$Alias; \
    claim_assert_static(bitfield_bits$(_$Alias) <= bitfield_storage$(_$Alias)); \
    claim_assert_static(sizeOf$(_$Alias) == sizeOf$(_$PackedInt))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri_bitfield__included */
