/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2026-02-26 (date of last update)
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Memory management with type-safe operations
 * @details Provides type-safe memory allocation and management utilities:
 *          - Memory allocation and deallocation
 *          - Memory resizing and remapping
 *          - Memory tracing and debugging
 *          - Memory alignment and padding utilities
 *          - Memory comparison and equality checks
 *          - Memory conversion and type-safe operations
 */
#ifndef mem_common__included
#define mem_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

/* --- Memory Constants --- */

#define mem_page_size /*: usize*/ /** \
    @brief Fixed page size (may be different per platform) \
 */ __val__mem_page_size

/* --- Integer Bit Operations --- */

$attr($inline_always)
$static fn_((mem_trailingZerosSize(usize x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZeros64(u64 x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZeros32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZeros16(u16 x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZeros8(u8 x))(u32));

$attr($inline_always)
$static fn_((mem_leadingZerosSize(usize x))(u32));
$attr($inline_always)
$static fn_((mem_leadingZeros64(u64 x))(u32));
$attr($inline_always)
$static fn_((mem_leadingZeros32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_leadingZeros16(u16 x))(u32));
$attr($inline_always)
$static fn_((mem_leadingZeros8(u8 x))(u32));

/* --- Byte Swap Functions --- */

$attr($inline_always)
$static fn_((mem_swapBytesSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_swapBytes64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_swapBytes32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_swapBytes16(u16 x))(u16));

/* --- Endian Conversion --- */

$attr($inline_always)
$static fn_((mem_littleToNativeSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_littleToNative64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_littleToNative32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_littleToNative16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_bigToNativeSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_bigToNative64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_bigToNative32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_bigToNative16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_nativeToLittleSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_nativeToLittle64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_nativeToLittle32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_nativeToLittle16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_nativeToBigSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_nativeToBig64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_nativeToBig32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_nativeToBig16(u16 x))(u16));

/* --- Alignment Functions --- */

T_alias$((mem_Log2Align)(u8));
T_use_O$(mem_Log2Align);
T_alias$((mem_Align)(mem_Log2Align));
T_use_O$(mem_Align);

/// Check if alignment is valid (power of 2)
$attr($inline_always)
$static fn_((mem_isValidAlign(usize align))(bool));
/// Check if address is aligned
$attr($inline_always)
$static fn_((mem_isAligned(usize addr, usize align))(bool));
/// Check if address is aligned to power of 2
$attr($inline_always)
$static fn_((mem_isAlignedLog2(usize addr, mem_Align log2_align))(bool));
/// Forward align an address
$attr($inline_always)
$static fn_((mem_alignFwd(usize addr, usize align))(usize));
/// Forward align an address to power of 2
$attr($inline_always)
$static fn_((mem_alignFwdLog2(usize addr, mem_Align log2_align))(usize));
/// Backward align an address
$attr($inline_always)
$static fn_((mem_alignBwd(usize addr, usize align))(usize));
/// Backward align an address to power of 2
$attr($inline_always)
$static fn_((mem_alignBwdLog2(usize addr, mem_Align log2_align))(usize));
/// Convert actual alignment value to log2
$attr($inline_always)
$static fn_((mem_alignToLog2(usize align))(mem_Align));
/// Convert log2 value to actual alignment
$attr($inline_always)
$static fn_((mem_log2ToAlign(mem_Align log2_align))(usize));

/* --- Memory Utilities --- */

$attr($inline_always)
$static fn_((mem_idxZ$u8(u8 sentinel, const u8* p))(usize));
$attr($inline_always)
$static fn_((mem_lenZ0$u8(const u8* p))(usize));

$attr($inline_always)
$static fn_((mem_spanZ0$u8(const u8* p))(S_const$u8));
$attr($inline_always)
$static fn_((mem_spanZ0Mut$u8(u8* p))(S$u8));

$attr($inline_always)
$static fn_((mem_asBytes(u_P_const$raw ptr))(S_const$u8));
#define T_use_mem_asBytes$(_T...) __stmt__T_use_mem_asBytes$(_T)
$attr($inline_always)
$static fn_((mem_asBytesMut(u_P$raw ptr))(S$u8));
#define T_use_mem_asBytesMut$(_T...) __stmt__T_use_mem_asBytesMut$(_T)
$attr($inline_always)
$static fn_((mem_bytesAs(TypeInfo type, S_const$u8 bytes))(u_P_const$raw));
#define T_use_mem_bytesAs$(_T...) __stmt__T_use_mem_bytesAs$(_T)
$attr($inline_always)
$static fn_((mem_bytesAsMut(TypeInfo type, S$u8 bytes))(u_P$raw));
#define T_use_mem_bytesAsMut$(_T...) __stmt__T_use_mem_bytesAsMut$(_T)

$attr($inline_always)
$static fn_((mem_asBytesS(u_S_const$raw sli))(S_const$u8));
#define T_use_mem_asBytesS$(_T...) __stmt__T_use_mem_asBytesS$(_T)
$attr($inline_always)
$static fn_((mem_asBytesMutS(u_S$raw sli))(S$u8));
#define T_use_mem_asBytesMutS$(_T...) __stmt__T_use_mem_asBytesMutS$(_T)
$attr($inline_always)
$static fn_((mem_bytesAsS(TypeInfo type, S_const$u8 bytes))(u_S_const$raw));
#define T_use_mem_bytesAsS$(_T...) __stmt__T_use_mem_bytesAsS$(_T)
$attr($inline_always)
$static fn_((mem_bytesAsMutS(TypeInfo type, S$u8 bytes))(u_S$raw));
#define T_use_mem_bytesAsMutS$(_T...) __stmt__T_use_mem_bytesAsMutS$(_T)

#define mem_Bytes$(_T...) __alias__mem_Bytes$(_T)
T_alias$((mem_Bytes$raw)(S$u8));
#define T_use_mem_Bytes$(_T...) __stmt__T_use_mem_Bytes$(_T)
$attr($inline_always)
$static fn_((mem_toBytes(u_V$raw val, mem_Bytes$raw ret_bytes))(mem_Bytes$raw));
#define T_use_mem_toBytes$(_T...) __stmt__T_use_mem_toBytes$(_T)
$attr($inline_always)
$static fn_((mem_bytesTo(mem_Bytes$raw bytes, u_V$raw ret_val))(u_V$raw));
#define T_use_mem_bytesTo$(_T...) __stmt__T_use_mem_bytesTo$(_T)

$extern fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8));
$extern fn_((mem_setP(u_P$raw dst, u_V$raw val))(u_P$raw));
#define T_use_mem_setP$(_T...) __stmt__T_use_mem_setP$(_T)
$extern fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw));
#define T_use_mem_set$(_T...) __stmt__T_use_mem_set$(_T)
$extern fn_((mem_set0Bytes(S$u8 dst))(S$u8));
$extern fn_((mem_set0P(u_P$raw dst))(u_P$raw));
#define T_use_mem_setP0$(_T...) __stmt__T_use_mem_setP0$(_T)
$extern fn_((mem_set0(u_S$raw dst))(u_S$raw));
#define T_use_mem_set0$(_T...) __stmt__T_use_mem_set0$(_T)
$extern fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8));
$extern fn_((mem_copyP(u_P$raw dst, u_P_const$raw src))(u_P$raw));
#define T_use_mem_copyP$(_T...) __stmt__T_use_mem_copyP$(_T)
$extern fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_copy$(_T...) __stmt__T_use_mem_copy$(_T)
$extern fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8));
$extern fn_((mem_moveP(u_P$raw dst, u_P_const$raw src))(u_P$raw));
#define T_use_mem_moveP$(_T...) __stmt__T_use_mem_moveP$(_T)
$extern fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_move$(_T...) __stmt__T_use_mem_move$(_T)

$extern fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_eqlP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_eqlP$(_T...) __stmt__T_use_mem_eqlP$(_T)
$extern fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eql$(_T...) __stmt__T_use_mem_eql$(_T)
$extern fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_neqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_neqP$(_T...) __stmt__T_use_mem_neqP$(_T)
$extern fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_neq$(_T...) __stmt__T_use_mem_neq$(_T)

$extern fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));
$extern fn_((mem_ordP(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord));
#define T_use_mem_ordP$(_T...) __stmt__T_use_mem_ordP$(_T)
$extern fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord));
#define T_use_mem_ord$(_T...) __stmt__T_use_mem_ord$(_T)
$extern fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_eqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_eqP$(_T...) __stmt__T_use_mem_eqP$(_T)
$extern fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eq$(_T...) __stmt__T_use_mem_eq$(_T)
$extern fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_neP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_neP$(_T...) __stmt__T_use_mem_neP$(_T)
$extern fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ne$(_T...) __stmt__T_use_mem_ne$(_T)
$extern fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_ltP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_ltP$(_T...) __stmt__T_use_mem_ltP$(_T)
$extern fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_lt$(_T...) __stmt__T_use_mem_lt$(_T)
$extern fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_gtP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_gtP$(_T...) __stmt__T_use_mem_gtP$(_T)
$extern fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_gt$(_T...) __stmt__T_use_mem_gt$(_T)
$extern fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_leP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_leP$(_T...) __stmt__T_use_mem_leP$(_T)
$extern fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_le$(_T...) __stmt__T_use_mem_le$(_T)
$extern fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_geP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_geP$(_T...) __stmt__T_use_mem_geP$(_T)
$extern fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ge$(_T...) __stmt__T_use_mem_ge$(_T)

$extern fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void));
$extern fn_((mem_swapP(u_P$raw lhs, u_P$raw rhs))(void));
#define T_use_mem_swapP$(_T...) __stmt__T_use_mem_swapP$(_T)
$extern fn_((mem_swap(u_S$raw lhs, u_S$raw rhs))(void));
#define T_use_mem_swap$(_T...) __stmt__T_use_mem_swap$(_T)
$extern fn_((mem_reverseBytes(S$u8 seq))(void));
$extern fn_((mem_reverse(u_S$raw seq))(void));
#define T_use_mem_reverse$(_T...) __stmt__T_use_mem_reverse$(_T)
$extern fn_((mem_rotateBytes(S$u8 seq, usize amount))(void));
$extern fn_((mem_rotate(u_S$raw seq, usize amount))(void));
#define T_use_mem_rotate$(_T...) __stmt__T_use_mem_rotate$(_T)

#define mem_WindowIter$(_T...) __alias__mem_WindowIter$(_T)
T_alias$((mem_WindowIter)(struct mem_WindowIter {
    var_(buf, S_const$raw);
    var_(idx, O$usize);
    var_(size, usize);
    var_(advance, usize);
    debug_only(var_(type, TypeInfo);)
}));
#define T_use_mem_WindowIter$(_T...) __stmt__T_use_mem_WindowIter$(_T)
$extern fn_((mem_window(u_S_const$raw buf, usize size, usize advance))(mem_WindowIter));
#define T_use_mem_window$(_T...) __stmt__T_use_mem_window$(_T)
$extern fn_((mem_WindowIter_reset(mem_WindowIter* self))(void));
#define T_use_mem_WindowIter_reset$(_T...) __stmt__T_use_mem_WindowIter_reset$(_T)
$extern fn_((mem_WindowIter_next(mem_WindowIter* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_WindowIter_next$(_T...) __stmt__T_use_mem_WindowIter_next$(_T)

$extern fn_((mem_startsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool));
$extern fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_startsWith$(_T...) __stmt__T_use_mem_startsWith$(_T)
$extern fn_((mem_endsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool));
$extern fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_endsWith$(_T...) __stmt__T_use_mem_endsWith$(_T)

T_alias$((mem_Delim)(enum_((mem_Delim $fits($packed))(
    mem_Delim_unit,
    mem_Delim_seq,
    mem_Delim_any
))));
#define mem_Delim$(_T...) __alias__mem_Delim$(_T)
T_alias$((mem_Delim$raw)(variant_((mem_Delim$raw $maps(mem_Delim))(
    (mem_Delim_unit, V$raw),
    (mem_Delim_seq, S_const$raw),
    (mem_Delim_any, S_const$raw)
))));
#define T_use_mem_Delim$(_T...) __stmt__T_use_mem_Delim$(_T)

#define mem_TokzIter$(_T...) __alias__mem_TokzIter$(_T)
T_alias$((mem_TokzIter$raw)(struct mem_TokzIter$raw {
    var_(buf, S_const$raw);
    var_(idx, usize);
    debug_only(var_(type, TypeInfo));
    var_(delim, mem_Delim$raw);
}));
T_use_P$(mem_TokzIter$raw);
T_alias$((V$mem_TokzIter$raw)(P$mem_TokzIter$raw));
#define T_use_mem_TokzIter$(_T...) __stmt__T_use_mem_TokzIter$(_T)
$extern fn_((mem_tokzUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzUnit$(_T...) __stmt__T_use_mem_tokzUnit$(_T)
$extern fn_((mem_tokzSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzSeq$(_T...) __stmt__T_use_mem_tokzSeq$(_T)
$extern fn_((mem_tokzAny(u_S_const$raw buf, u_S_const$raw any, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzAny$(_T...) __stmt__T_use_mem_tokzAny$(_T)
$extern fn_((mem_TokzIter_reset(mem_TokzIter$raw* self))(void));
#define T_use_mem_TokzIter_reset$(_T...) __stmt__T_use_mem_TokzIter_reset$(_T)
$extern fn_((mem_TokzIter_next(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzIter_next$(_T...) __stmt__T_use_mem_TokzIter_next$(_T)
$extern fn_((mem_TokzIter_peek(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzIter_peek$(_T...) __stmt__T_use_mem_TokzIter_peek$(_T)
$extern fn_((mem_TokzIter_rest(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_TokzIter_rest$(_T...) __stmt__T_use_mem_TokzIter_rest$(_T)

#define mem_SplitIter$(_T...) __alias__mem_SplitIter$(_T)
T_alias$((mem_SplitIter$raw)(struct mem_SplitIter$raw {
    var_(buf, S_const$raw);
    var_(idx, O$usize);
    debug_only(var_(type, TypeInfo));
    var_(delim, mem_Delim$raw);
}));
T_use_P$(mem_SplitIter$raw);
T_alias$((V$mem_SplitIter$raw)(P$mem_SplitIter$raw));
#define T_use_mem_SplitIter$(_T...) __stmt__T_use_mem_SplitIter$(_T)
$extern fn_((mem_splitUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitUnit$(_T...) __stmt__T_use_mem_splitUnit$(_T)
$extern fn_((mem_splitSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitSeq$(_T...) __stmt__T_use_mem_splitSeq$(_T)
$extern fn_((mem_splitAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitAny$(_T...) __stmt__T_use_mem_splitAny$(_T)
$extern fn_((mem_SplitIter_reset(mem_SplitIter$raw* self))(void));
#define T_use_mem_SplitIter_reset$(_T...) __stmt__T_use_mem_SplitIter_reset$(_T)
$extern fn_((mem_SplitIter_first(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitIter_first$(_T...) __stmt__T_use_mem_SplitIter_first$(_T)
$extern fn_((mem_SplitIter_next(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitIter_next$(_T...) __stmt__T_use_mem_SplitIter_next$(_T)
$extern fn_((mem_SplitIter_peek(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitIter_peek$(_T...) __stmt__T_use_mem_SplitIter_peek$(_T)
$extern fn_((mem_SplitIter_rest(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitIter_rest$(_T...) __stmt__T_use_mem_SplitIter_rest$(_T)

#if UNUSED_CODE
#define mem_asBytes(_ptr /*: _T*/... /*S(u8)*/) ____mem_asBytes(_ptr)
#define mem_bytesAs$(/*(_T)(_bytes: S(u8))*/... /*(_T)*/) __step__mem_bytesAs$(__VA_ARGS__)

#define mem_toBytes$(/*(A(sizeOf(_T), u8))(_val: _T)*/... /*(A(sizeOf(_T), u8))*/) __step__mem_toBytes$(__VA_ARGS__)
#define mem_toBytes(_val /*: _T*/... /* (A(sizeOf(_T), u8)) */) ____mem_toBytes(_val)
#define mem_bytesTo$(/*(_T)(_bytes: S(u8))*/... /*(_T)*/) __step__mem_bytesTo$(__VA_ARGS__)

#define mem_sAsBytes(_sli /*: _T*/... /*(S(u8))*/) __step__mem_sAsBytes(_sli)
#define mem_bytesAsS$(/*(_T)(_bytes: S(u8))*/... /*(_T)*/) __step__mem_bytesAsS$(__VA_ARGS__)
#endif /* UNUSED_CODE */

/*========== Macros and Definitions =========================================*/

/* --- Memory Constants --- */

#define __val__mem_page_size (pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_wasm64)(as$(usize)(64u) * 1024u)), \
        pp_case_((arch_type_wasm32)(as$(usize)(64u) * 1024u)), \
        pp_case_((arch_type_aarch64)(as$(usize)(16u) * 1024u)), \
        pp_default_(as$(usize)(4u) * 1024u) \
    ) pp_end \
))

/* --- Integer Bit Operations --- */

fn_((mem_trailingZerosSize(usize x))(u32)) {
    if (x == 0) { return sizeOf$(usize) * 8; }
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_trailingZeros64(x)),
        pp_else_(mem_trailingZeros32(x)));
};
fn_((mem_trailingZeros64(u64 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u64) * 8; }
    // 64-bit De Bruijn Sequence
    static const u8 lookup[64] = { /* clang-format off */
         0,  1, 56,  2, 57, 49, 28,  3,
        61, 58, 42, 50, 38, 29, 17,  4,
        62, 47, 59, 36, 45, 43, 51, 22,
        53, 39, 33, 30, 24, 18, 12,  5,
        63, 55, 48, 27, 60, 41, 37, 16,
        46, 35, 44, 21, 52, 32, 23, 11,
        54, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
    }; /* clang-format on */
    const u64 debruijn = 0x03F79D71B4CA8B09ull;
    // (x & -x) isolates the lowest set bit
    return lookup[((x & (0 - x)) * debruijn) >> 58];
#endif
};
fn_((mem_trailingZeros32(u32 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u32) * 8; }
    // 32-bit De Bruijn Sequence (moved from your 64-bit impl)
    static const u8 lookup[32] = { /* clang-format off */
         0,  1, 28,  2, 29, 14, 24,  3,
        30, 22, 20, 15, 25, 17,  4,  8,
        31, 27, 13, 23, 21, 19, 16,  7,
        26, 12, 18,  6, 11,  5, 10,  9
    }; /* clang-format on */
    const u32 debruijn = 0x077CB531u;
    return lookup[((x & (0 - x)) * debruijn) >> 27];
#endif
};
fn_((mem_trailingZeros16(u16 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u16) * 8; }
    return mem_trailingZeros32(as$(u32)(x));
#endif
};
fn_((mem_trailingZeros8(u8 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u8) * 8; }
    return mem_trailingZeros32(as$(u32)(x));
#endif
};

fn_((mem_leadingZerosSize(usize x))(u32)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_leadingZeros64(x)),
        pp_else_(mem_leadingZeros32(x)));
};
fn_((mem_leadingZeros64(u64 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u64) * 8; }
    // Fallback: Smear bits to right to fill with 1s from MSB
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    // Count ones (population count) is easier,
    // but reusing CTZ logic by isolating MSB is also valid.
    // Here we use a standard 'count set bits' (Hamming weight) for the smeared value.
    // leading_zeros = 64 - popcount(smeared_x)

    // Implementation of popcount64 for fallback (SWAR algorithm)
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return 64 - (u32)((x * 0x0101010101010101ULL) >> 56);
#endif
};
fn_((mem_leadingZeros32(u32 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u32) * 8; }
    // Fallback: Smear and Popcount
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    // Popcount32 (SWAR)
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return 32 - (x & 0x3F);
#endif
};
fn_((mem_leadingZeros16(u16 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u16) * 8; }
    // __builtin_clz works on 32-bit (usually).
    // clz(0x0000FFFF) is 16. We want 0. So subtract 16.
    // clz(0x00000001) is 31. We want 15. So subtract 16.
    return mem_leadingZeros32(as$(u32)(x)) - 16;
#endif
};
fn_((mem_leadingZeros8(u8 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u8) * 8; }
    // __builtin_clz works on 32-bit (usually).
    // clz(0x0000FFFF) is 16. We want 0. So subtract 24.
    // clz(0x00000001) is 31. We want 7. So subtract 24.
    return mem_leadingZeros32(as$(u32)(x)) - 24;
#endif
};

/* --- Byte Swap Functions --- */

fn_((mem_swapBytesSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_swapBytes64(x)),
        pp_else_(mem_swapBytes32(x)));
};
fn_((mem_swapBytes64(u64 x))(u64)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(x);
#else
    return ((x & 0xFF00000000000000ULL) >> 56)
         | ((x & 0x00FF000000000000ULL) >> 40)
         | ((x & 0x0000FF0000000000ULL) >> 24)
         | ((x & 0x000000FF00000000ULL) >> 8)
         | ((x & 0x00000000FF000000ULL) << 8)
         | ((x & 0x0000000000FF0000ULL) << 24)
         | ((x & 0x000000000000FF00ULL) << 40)
         | ((x & 0x00000000000000FFULL) << 56);
#endif
};
fn_((mem_swapBytes32(u32 x))(u32)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#else
    return ((x & 0xFF000000) >> 24)
         | ((x & 0x00FF0000) >> 8)
         | ((x & 0x0000FF00) << 8)
         | ((x & 0x000000FF) << 24);
#endif
};
fn_((mem_swapBytes16(u16 x))(u16)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(x);
#else
    return (x << 8) | (x >> 8);
#endif
};

/* --- Endian Conversion --- */

fn_((mem_littleToNativeSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_littleToNative64(x)),
        pp_else_(mem_littleToNative32(x)));
};
fn_((mem_littleToNative64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_littleToNative32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes32(x)));
};
fn_((mem_littleToNative16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes16(x)));
};

fn_((mem_bigToNativeSize(usize x))(usize)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesSize(x)));
};
fn_((mem_bigToNative64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_bigToNative32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes32(x)));
};
fn_((mem_bigToNative16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes16(x)));
};

fn_((mem_nativeToLittleSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_nativeToLittle64(x)),
        pp_else_(mem_nativeToLittle32(x)));
};
fn_((mem_nativeToLittle64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_nativeToLittle32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes32(x)));
};
fn_((mem_nativeToLittle16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes16(x)));
};

fn_((mem_nativeToBigSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_nativeToBig64(x)),
        pp_else_(mem_nativeToBig32(x)));
};
fn_((mem_nativeToBig64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_nativeToBig32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes32(x)));
};
fn_((mem_nativeToBig16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes16(x)));
};

/* --- Alignment Functions --- */

fn_((mem_isValidAlign(usize align))(bool)) {
    return 0 < align && (align & (align - 1)) == 0;
};
fn_((mem_isAligned(usize addr, usize align))(bool)) {
    return claim_assert(mem_isValidAlign(align)), (addr & (align - 1)) == 0;
};
fn_((mem_isAlignedLog2(usize addr, mem_Align log2_align))(bool)) {
    return mem_trailingZerosSize(addr) >= log2_align;
};
fn_((mem_alignFwd(usize addr, usize align))(usize)) {
    return claim_assert(mem_isValidAlign(align)), (addr + (align - 1)) & ~(align - 1);
};
fn_((mem_alignFwdLog2(usize addr, mem_Align log2_align))(usize)) {
    return mem_alignFwd(addr, mem_log2ToAlign(log2_align));
};
fn_((mem_alignBwd(usize addr, usize align))(usize)) {
    return claim_assert(mem_isValidAlign(align)), addr & ~(align - 1);
};
fn_((mem_alignBwdLog2(usize addr, mem_Align log2_align))(usize)) {
    return mem_alignBwd(addr, mem_log2ToAlign(log2_align));
};
fn_((mem_alignToLog2(usize align))(mem_Align)) {
    return claim_assert(mem_isValidAlign(align)), intCast$((mem_Align)(mem_trailingZerosSize(align)));
};
fn_((mem_log2ToAlign(mem_Align log2_align))(usize)) {
    return usize_(1) << as$(usize)(log2_align);
};

/* --- Memory Utilities --- */

fn_((mem_idxZ$u8(u8 sentinel, const u8* p))(usize)) {
    claim_assert_nonnull(p);
    usize idx = 0;
    while (p[idx] != sentinel) { ++idx; }
    return idx;
};
fn_((mem_lenZ0$u8(const u8* p))(usize)) {
    claim_assert_nonnull(p);
    return mem_idxZ$u8(u8_c('\0'), p);
};

fn_((mem_spanZ0$u8(const u8* p))(S_const$u8)) {
    claim_assert_nonnull(p);
    return (S_const$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
};
fn_((mem_spanZ0Mut$u8(u8* p))(S$u8)) {
    claim_assert_nonnull(p);
    return (S$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
};

fn_((mem_asBytes(u_P_const$raw ptr))(S_const$u8)) {
    claim_assert_nonnull(ptr.raw);
    /* TODO: add align check */
    return (S_const$u8){ .ptr = ptr.raw, .len = ptr.type.size };
};
fn_((mem_asBytesMut(u_P$raw ptr))(S$u8)) {
    claim_assert_nonnull(ptr.raw);
    /* TODO: add align check */
    return (S$u8){ .ptr = ptr.raw, .len = ptr.type.size };
};

fn_((mem_bytesAs(TypeInfo type, S_const$u8 bytes))(u_P_const$raw)) {
    claim_assert_nonnullS(bytes);
    /* TODO: add align check */
    claim_assert(bytes.len == type.size);
    return (u_P_const$raw){ .type = type, .raw = bytes.ptr };
};
fn_((mem_bytesAsMut(TypeInfo type, S$u8 bytes))(u_P$raw)) {
    claim_assert_nonnullS(bytes);
    /* TODO: add align check */
    claim_assert(bytes.len == type.size);
    return (u_P$raw){ .type = type, .raw = bytes.ptr };
};

fn_((mem_asBytesS(u_S_const$raw sli))(S_const$u8)) {
    claim_assert_nonnullS(sli.raw);
    /* TODO: add align check */
    return (S_const$u8){
        .ptr = sli.ptr,
        .len = sli.len * sli.type.size,
    };
};
fn_((mem_asBytesMutS(u_S$raw sli))(S$u8)) {
    claim_assert_nonnullS(sli.raw);
    /* TODO: add align check */
    return (S$u8){
        .ptr = sli.ptr,
        .len = sli.len * sli.type.size,
    };
};

fn_((mem_bytesAsS(TypeInfo type, S_const$u8 bytes))(u_S_const$raw)) {
    claim_assert_nonnullS(bytes);
    /* TODO: add align check */
    let count = type.size == 0 ? 0 : bytes.len / type.size;
    return (u_S_const$raw){ .type = type, .ptr = bytes.ptr, .len = count };
};
fn_((mem_bytesAsMutS(TypeInfo type, S$u8 bytes))(u_S$raw)) {
    claim_assert_nonnullS(bytes);
    /* TODO: add align check */
    let count = type.size == 0 ? 0 : bytes.len / type.size;
    return (u_S$raw){ .type = type, .ptr = bytes.ptr, .len = count };
};

fn_((mem_toBytes(u_V$raw val, mem_Bytes$raw ret_bytes))(mem_Bytes$raw)) {
    claim_assert_nonnull(val.inner);
    return mem_copyBytes(ret_bytes, mem_asBytes(val.ref.as_const));
};
fn_((mem_bytesTo(mem_Bytes$raw bytes, u_V$raw ret_val))(u_V$raw)) {
    claim_assert_nonnull(ret_val.inner);
    return u_deref(mem_copyP(ret_val.ref, mem_bytesAs(ret_val.inner_type, bytes.as_const)));
};

/* --- template --- */

#define __stmt__T_use_mem_asBytes$(_T...) \
    $inline_always $static fn_((tpl$(mem_asBytes, _T)(P$$(_T) ptr))(S_const$u8)) { \
        return mem_asBytes(u_anyP(ptr)); \
    }
#define __stmt__T_use_mem_asBytesMut$(_T...) \
    $inline_always $static fn_((tpl$(mem_asBytesMut, _T)(P$$(_T) ptr))(S$u8)) { \
        return mem_asBytesMut(u_anyP(ptr)); \
    }
#define __stmt__T_use_mem_bytesAs$(_T...) \
    $inline_always $static fn_((tpl$(mem_bytesAs, _T)(S_const$u8 bytes))(P_const$$(_T))) { \
        return mem_bytesAs(typeInfo$(_T), u_anyS(bytes)); \
    }
#define __stmt__T_use_mem_bytesAsMut$(_T...) \
    $inline_always $static fn_((tpl$(mem_bytesAsMut, _T)(S$u8 bytes))(P$$(_T))) { \
        return mem_bytesAsMut(typeInfo$(_T), u_anyS(bytes)); \
    }
#define __stmt__T_use_mem_asBytesS$(_T...) \
    $inline_always $static fn_((tpl$(mem_asBytesS, _T)(S_const$(_T) sli))(S_const$u8)) { \
        return mem_asBytesS(u_anyS(sli)); \
    }
#define __stmt__T_use_mem_asBytesMutS$(_T...) \
    $inline_always $static fn_((tpl$(mem_asBytesMutS, _T)(S$(_T) sli))(S$u8)) { \
        return mem_asBytesMutS(u_anyS(sli)); \
    }
#define __alias__mem_Bytes$(_T...) tpl$(mem_Bytes, _T)
#define __stmt__T_use_mem_Bytes$(_T...) \
    T_alias$((mem_Bytes$(_T))(A$$(sizeOf$(_T), u8)))
#define __stmt__T_use_mem_toBytes$(_T...) \
    $inline_always $static fn_((tpl$(mem_toBytes, _T)(_T val))(mem_Bytes$(_T))) { \
        return S_deref$((mem_Bytes$(_T))(mem_toBytes(u_anyV(val), A_ref$((S$u8)(l0$((mem_Bytes$(_T)))))))); \
    }
#define __stmt__T_use_mem_bytesTo$(_T...) \
    $inline_always $static fn_((tpl$(mem_bytesTo, _T)(mem_Bytes$(_T) bytes))(_T)) { \
        return u_castV$((_T)(mem_bytesTo(A_ref$((S$u8)(bytes)), u_retV$(_T)))); \
    }
#define __stmt__T_use_mem_setP$(_T...) \
    $inline_always $static fn_((tpl$(mem_setP, _T)(P$$(_T) dst, _T val))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_setP(u_anyP(dst), u_anyV(val)))); \
    }
#define __stmt__T_use_mem_set$(_T...) \
    $inline_always $static fn_((tpl$(mem_set, _T)(S$(_T) dst, _T val))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set(u_anyS(dst), u_anyV(val)))); \
    }
#define __stmt__T_use_mem_setP0$(_T...) \
    $inline_always $static fn_((tpl$(mem_setP0, _T)(P$$(_T) dst))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_set0P(u_anyP(dst)))); \
    }
#define __stmt__T_use_mem_set0$(_T...) \
    $inline_always $static fn_((tpl$(mem_set0, _T)(S$(_T) dst))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set0(u_anyS(dst)))); \
    }
#define __stmt__T_use_mem_copyP$(_T...) \
    $inline_always $static fn_((tpl$(mem_copyP, _T)(P$$(_T) dst, P_const$$(_T) src))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_copyP(u_anyP(dst), u_anyP(src)))); \
    }
#define __stmt__T_use_mem_copy$(_T...) \
    $inline_always $static fn_((tpl$(mem_copy, _T)(S$(_T) dst, S_const$(_T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copy(u_anyS(dst), u_anyS(src)))); \
    }
#define __stmt__T_use_mem_moveP$(_T...) \
    $inline_always $static fn_((tpl$(mem_moveP, _T)(P$$(_T) dst, P_const$$(_T) src))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_moveP(u_anyP(dst), u_anyP(src)))); \
    }
#define __stmt__T_use_mem_move$(_T...) \
    $inline_always $static fn_((tpl$(mem_move, _T)(S$(_T) dst, S_const$(_T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_move(u_anyS(dst), u_anyS(src)))); \
    }
#define __stmt__T_use_mem_eqlP$(_T...) \
    $inline_always $static fn_((tpl$(mem_eqlP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_eqlP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_eql$(_T...) \
    $inline_always $static fn_((tpl$(mem_eql, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_eql(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_neqP$(_T...) \
    $inline_always $static fn_((tpl$(mem_neqP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_neqP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_neq$(_T...) \
    $inline_always $static fn_((tpl$(mem_neq, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_neq(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_ordP$(_T...) \
    $inline_always $static fn_((tpl$(mem_ordP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(cmp_Ord)) { \
        return mem_ordP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_ord$(_T...) \
    $inline_always $static fn_((tpl$(mem_ord, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(cmp_Ord)) { \
        return mem_ord(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_eqP$(_T...) \
    $inline_always $static fn_((tpl$(mem_eqP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_eqP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_eq$(_T...) \
    $inline_always $static fn_((tpl$(mem_eq, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_eq(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_neP$(_T...) \
    $inline_always $static fn_((tpl$(mem_neP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_neP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_ne$(_T...) \
    $inline_always $static fn_((tpl$(mem_ne, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_ne(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_ltP$(_T...) \
    $inline_always $static fn_((tpl$(mem_ltP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_ltP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_lt$(_T...) \
    $inline_always $static fn_((tpl$(mem_lt, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_lt(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_gtP$(_T...) \
    $inline_always $static fn_((tpl$(mem_gtP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_gtP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_gt$(_T...) \
    $inline_always $static fn_((tpl$(mem_gt, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_gt(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_leP$(_T...) \
    $inline_always $static fn_((tpl$(mem_leP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_leP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_le$(_T...) \
    $inline_always $static fn_((tpl$(mem_le, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_le(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_geP$(_T...) \
    $inline_always $static fn_((tpl$(mem_geP, _T)(P_const$$(_T) lhs, P_const$$(_T) rhs))(bool)) { \
        return mem_geP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_ge$(_T...) \
    $inline_always $static fn_((tpl$(mem_ge, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_ge(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_swapP$(_T...) \
    $inline_always $static fn_((tpl$(mem_swapP, _T)(P$$(_T) lhs, P$$(_T) rhs))(void)) { \
        return mem_swapP(u_anyP(lhs), u_anyP(rhs)); \
    }
#define __stmt__T_use_mem_swap$(_T...) \
    $inline_always $static fn_((tpl$(mem_swap, _T)(S$(_T) lhs, S$(_T) rhs))(void)) { \
        return mem_swap(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_reverse$(_T...) \
    $inline_always $static fn_((tpl$(mem_reverse, _T)(S$(_T) seq))(void)) { \
        return mem_reverse(u_anyS(seq)); \
    }
#define __stmt__T_use_mem_rotate$(_T...) \
    $inline_always $static fn_((tpl$(mem_rotate, _T)(S$(_T) seq, usize amount))(void)) { \
        return mem_rotate(u_anyS(seq), amount); \
    }
#define __alias__mem_WindowIter$(_T...) tpl$(mem_WindowIter, _T)
#define __stmt__T_use_mem_WindowIter$(_T...) \
    T_alias$((mem_WindowIter$(_T))(union mem_WindowIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, O$usize); \
            var_(size, usize); \
            var_(advance, usize); \
            debug_only(var_(type, TypeInfo);) \
        }); \
        var_(as_raw, mem_WindowIter) $like_ref; \
    }))
#define __stmt__T_use_mem_window$(_T...) /* clang-format off */ \
        $attr($inline_always) \
        $static fn_((tpl$(mem_window, _T)(S_const$(_T) buf, usize size, usize advance))(mem_WindowIter$(_T)) $scope) { \
            return_({ .as_raw $like_deref = mem_window(u_anyS(buf), size, advance) }); \
        } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_WindowIter_reset$(_T...) /* clang-format off */ \
        $attr($inline_always) \
        $static fn_((tpl$(mem_WindowIter_reset, _T)($P$(mem_WindowIter$(_T)) self))(void)) { \
            return mem_WindowIter_reset(self->as_raw); \
        } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_WindowIter_next$(_T...) /* clang-format off */ \
        $attr($inline_always) \
        $static fn_((tpl$(mem_WindowIter_next, _T)($P$(mem_WindowIter$(_T)) self))(O$(S_const$(_T))) $scope) { \
            return_(u_castO$((ReturnType)(mem_WindowIter_next(self, typeInfo$(_T))))); \
        } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_startsWith$(_T...) \
    $inline_always $static fn_((tpl$(mem_startsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_startsWith(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_endsWith$(_T...) \
    $inline_always $static fn_((tpl$(mem_endsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_endsWith(u_anyS(haystack), u_anyS(needle)); \
    }
#define __alias__mem_Delim$(_T...) tpl$(mem_Delim, _T)
#define __stmt__T_use_mem_Delim$(_T...) \
    T_alias$((mem_Delim$(_T))(union mem_Delim$(_T) { \
        variant_(($maps(mem_DelimType))( \
            (mem_Delim_unit, _T), \
            (mem_Delim_seq, S_const$(_T)), \
            (mem_Delim_any, S_const$(_T)), \
        )); \
        var_(as_raw, mem_Delim$raw) $like_ref; \
    }))
#define __alias__mem_TokzIter$(_T...) tpl$(mem_TokzIter, _T)
#define __stmt__T_use_mem_TokzIter$(_T...) \
    T_alias$((mem_TokzIter$(_T))(union mem_TokzIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, usize); \
            debug_only(var_(type, TypeInfo)); \
            var_(delim, mem_Delim$(_T)); \
        }); \
        var_(as_raw, mem_TokzIter$raw) $like_ref; \
    }))
#define __stmt__T_use_mem_tokzUnit$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzUnit, _T)(S_const$(_T) buf, _T unit))(mem_TokzIter$(_T))) { \
        return *as$(mem_TokzIter$(_T)*)(mem_tokzUnit(u_anyS(buf), u_anyV(unit), l0$((mem_TokzIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_tokzSeq$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzSeq, _T)(S_const$(_T) buf, S_const$(_T) seq))(mem_TokzIter$(_T))) { \
        return *as$(mem_TokzIter$(_T)*)(mem_tokzSeq(u_anyS(buf), u_anyS(seq), l0$((mem_TokzIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_tokzAny$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzAny, _T)(S_const$(_T) buf, S_const$(_T) any))(mem_TokzIter$(_T))) { \
        return *as$(mem_TokzIter$(_T)*)(mem_tokzAny(u_anyS(buf), u_anyS(any), l0$((mem_TokzIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_TokzIter_reset$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzIter_reset, _T)($P$(mem_TokzIter$(_T)) self))(void)) { \
        return mem_TokzIter_reset(self->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_mem_TokzIter_next$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzIter_next, _T)($P$(mem_TokzIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokzIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_TokzIter_peek$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzIter_peek, _T)($P$(mem_TokzIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokzIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_TokzIter_rest$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzIter_rest, _T)($P$(mem_TokzIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_TokzIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __alias__mem_SplitIter$(_T...) tpl$(mem_SplitIter, _T)
#define __stmt__T_use_mem_SplitIter$(_T...) \
    T_alias$((mem_SplitIter$(_T))(union mem_SplitIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, O$usize); \
            debug_only(var_(type, TypeInfo)); \
            var_(delim, mem_Delim$(_T)); \
        }); \
        var_(as_raw, mem_SplitIter$raw) $like_ref; \
    }))
#define __stmt__T_use_mem_splitUnit$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitUnit, _T)(S_const$(_T) buf, _T unit))(mem_SplitIter$(_T))) { \
        return *as$(mem_SplitIter$(_T)*)(mem_splitUnit(u_anyS(buf), u_anyV(unit), l0$((mem_SplitIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_splitSeq$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitSeq, _T)(S_const$(_T) buf, S_const$(_T) seq))(mem_SplitIter$(_T))) { \
        return *as$(mem_SplitIter$(_T)*)(mem_splitSeq(u_anyS(buf), u_anyS(seq), l0$((mem_SplitIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_splitAny$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitAny, _T)(S_const$(_T) buf, S_const$(_T) any))(mem_SplitIter$(_T))) { \
        return *as$(mem_SplitIter$(_T)*)(mem_splitAny(u_anyS(buf), u_anyS(any), l0$((mem_SplitIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_SplitIter_reset$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitIter_reset, _T)($P$(mem_SplitIter$(_T)) self))(void)) { \
        return mem_SplitIter_reset(self->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_mem_SplitIter_first$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitIter_first, _T)($P$(mem_SplitIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_SplitIter_first(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitIter_next$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitIter_next, _T)($P$(mem_SplitIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_SplitIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitIter_peek$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitIter_peek, _T)($P$(mem_SplitIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_SplitIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitIter_rest$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitIter_rest, _T)($P$(mem_SplitIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_SplitIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */

#if UNUSED_CODE
#define ____mem_asBytes(_ptr...) T_switch$((P_T$(TypeOf(_ptr)))( \
    T_qual$((const P_TUnqual$(TypeOf(_ptr)))(l$((S_const$u8){ \
        .ptr = ptrQualCast$((const u8*)(_ptr)), \
        .len = sizeOf$(P_T$(TypeOf(_ptr))), \
    }))), \
    T_qual$((P_TUnqual$(TypeOf(_ptr)))(l$((S$u8){ \
        .ptr = ptrQualCast$((u8*)(_ptr)), \
        .len = sizeOf$(P_T$(TypeOf(_ptr))), \
    }))) \
))
#define __step__mem_bytesAs$(...) __step__mem_bytesAs$__emit(__step__mem_bytesAs$__parse __VA_ARGS__)
#define __step__mem_bytesAs$__parse(_T...) _T,
#define __step__mem_bytesAs$__emit(...) ____mem_bytesAs$(__VA_ARGS__)
#define ____mem_bytesAs$(_T, _bytes...) T_switch$((S_T$(TypeOf(_bytes)))( \
    T_qual$((const u8)(T_switch$((P_T$(_T))(/* clang-format off */ \
        T_qual$((const P_TUnqual$(_T))(ptrAlignQualCast$((_T)(_bytes.ptr)))), \
        T_qual$((P_TUnqual$(_T))(claim_unreachable)) /* clang-format on */ \
    )))), \
    T_qual$((u8)(ptrAlignQualCast$((_T)(_bytes.ptr)))) \
))

#define __step__mem_toBytes$(...) __step__mem_toBytes$__emit(__step__mem_toBytes$__parse __VA_ARGS__)
#define __step__mem_toBytes$__parse(_T...) _T,
#define __step__mem_toBytes$__emit(...) ____mem_toBytes$(__VA_ARGS__)
#define ____mem_toBytes$(_T, _val...) \
    S_deref$((_T)mem_asBytes(&copy(_val)))
#define ____mem_toBytes(_val...) \
    mem_toBytes$((A$$(sizeOf$(TypeOf(_val)), u8))_val)
#define __step__mem_bytesTo$(...) __step__mem_bytesTo$__emit(__step__mem_bytesTo$__parse __VA_ARGS__)
#define __step__mem_bytesTo$__parse(_T...) _T,
#define __step__mem_bytesTo$__emit(...) ____mem_bytesTo$(__VA_ARGS__)
#define ____mem_bytesTo$(_T, _bytes...) T_switch$((S_T$(TypeOf(_bytes)))( \
    T_case$((const u8)(*ptrAlignQualCast$((const _T*)(_bytes.ptr)))), \
    T_case$((u8)(*ptrAlignQualCast$((_T*)(_bytes.ptr)))) \
))

#define __step__mem_sAsBytes(_sli...) ____mem_sAsBytes$(pp_uniqTok(sli), _sli)
#define ____mem_sAsBytes$(__sli, _sli...) local_({ \
    let __sli = &copy(_sli); \
    typedef TypeOfUnqual(*__sli) T$mem_sAsBytes$; \
    typedef S_T$(T$mem_sAsBytes$) S_T; \
    typedef TypeOfUnqual(S_T) S_TUnqual; \
    local_return_(T_switch$((TypeOf(S_T))( \
        T_qual$((const S_TUnqual)(l$((S_const$u8){ \
            .ptr = ptrQualCast$((const u8*)(__sli->ptr)), \
            .len = __sli->len * sizeOf$(S_T), \
        }))), \
        T_qual$((S_TUnqual)(l$((S$u8){ \
            .ptr = ptrQualCast$((u8*)(__sli->ptr)), \
            .len = __sli->len * sizeOf$(S_T), \
        }))) \
    ))); \
})
#define __step__mem_bytesAsS$(...) __step__mem_bytesAsS$__emit(__step__mem_bytesAsS$__parse __VA_ARGS__)
#define __step__mem_bytesAsS$__parse(_T...) _T, pp_uniqTok(bytes),
#define __step__mem_bytesAsS$__emit(...) ____mem_bytesAsS$(__VA_ARGS__)
#define ____mem_bytesAsS$(_T, __bytes, _bytes...) local_({ \
    let __bytes = &copy(_bytes); \
    typedef _T T$mem_bytesAsS$; \
    typedef S_T$(T$mem_bytesAsS$) S_T; \
    typedef TypeOfUnqual(S_T) S_TUnqual; \
    local_return_(T_switch$((S_T$(__bytes))( \
        T_case$((const u8)(T_switch$((T$mem_bytesAsS$)(/* clang-format off */ \
            T_case$((const S_TUnqual)(l$((T$mem_bytesAsS$){ \
                .ptr = ptrAlignQualCast$((S_T*)(__bytes.ptr)), \
                .len = __bytes->len / sizeOf$(S_T), \
            }))), \
            T_case$((S_TUnqual)(claim_unreachable)) /* clang-format on */ \
        )))), \
        T_case$((u8)(l$((T$mem_bytesAsS$){ \
            .ptr = ptrAlignQualCast$((S_T*)(__bytes.ptr)), \
            .len = __bytes->len / sizeOf$(S_T), \
        }))) \
    ))); \
})
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_common__included */
