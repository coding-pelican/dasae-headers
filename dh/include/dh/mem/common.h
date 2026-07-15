/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2026-05-20 (date of last update)
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
#pragma once
#ifndef mem_common__included
#define mem_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Memory Constants --- */

#define mem_byte_size (usize_(1))
#define mem_kb_size (usize_(1) * 1000)
#define mem_kib_size (usize_(1) * 1024)
#define mem_mb_size (usize_(1) * 1000 * 1000)
#define mem_mib_size (usize_(1) * 1024 * 1024)
#define mem_gb_size (usize_(1) * 1000 * 1000 * 1000)
#define mem_gib_size (usize_(1) * 1024 * 1024 * 1024)

#define mem_page_size /*: usize*/ /** \
    @brief Fixed page size (may be different per platform) \
 */ __val__mem_page_size

/* --- Integer Bit Operations --- */

$attr($inline_always)
$static fn_((mem_trailingZerosSize(usize x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZeros64(u64 x))(u32));
$attr($inline_always)
$static fn_((mem_trailingZerosLong(ulong x))(u32));
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
$static fn_((mem_leadingZerosLong(ulong x))(u32));
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
$static fn_((mem_swapBytesLong(ulong x))(ulong));
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
$static fn_((mem_littleToNativeLong(ulong x))(ulong));
$attr($inline_always)
$static fn_((mem_littleToNative32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_littleToNative16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_bigToNativeSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_bigToNative64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_bigToNativeLong(ulong x))(ulong));
$attr($inline_always)
$static fn_((mem_bigToNative32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_bigToNative16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_nativeToLittleSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_nativeToLittle64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_nativeToLittleLong(ulong x))(ulong));
$attr($inline_always)
$static fn_((mem_nativeToLittle32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_nativeToLittle16(u16 x))(u16));

$attr($inline_always)
$static fn_((mem_nativeToBigSize(usize x))(usize));
$attr($inline_always)
$static fn_((mem_nativeToBig64(u64 x))(u64));
$attr($inline_always)
$static fn_((mem_nativeToBigLong(ulong x))(ulong));
$attr($inline_always)
$static fn_((mem_nativeToBig32(u32 x))(u32));
$attr($inline_always)
$static fn_((mem_nativeToBig16(u16 x))(u16));

T_alias$((mem_ReadLESizeBuf)(A$$(usize_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readLESize(mem_ReadLESizeBuf bytes))(usize));
T_alias$((mem_ReadLE64Buf)(A$$(u64_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readLE64(mem_ReadLE64Buf bytes))(u64));
T_alias$((mem_ReadLELongBuf)(A$$(ulong_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readLELong(mem_ReadLELongBuf bytes))(ulong));
T_alias$((mem_ReadLE32Buf)(A$$(u32_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readLE32(mem_ReadLE32Buf bytes))(u32));
T_alias$((mem_ReadLE16Buf)(A$$(u16_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readLE16(mem_ReadLE16Buf bytes))(u16));

T_alias$((mem_ReadBESizeBuf)(A$$(usize_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readBESize(mem_ReadBESizeBuf bytes))(usize));
T_alias$((mem_ReadBE64Buf)(A$$(u64_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readBE64(mem_ReadBE64Buf bytes))(u64));
T_alias$((mem_ReadBELongBuf)(A$$(ulong_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readBELong(mem_ReadBELongBuf bytes))(ulong));
T_alias$((mem_ReadBE32Buf)(A$$(u32_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readBE32(mem_ReadBE32Buf bytes))(u32));
T_alias$((mem_ReadBE16Buf)(A$$(u16_bytes, u8)));
$attr($inline_always)
$static fn_((mem_readBE16(mem_ReadBE16Buf bytes))(u16));

T_alias$((mem_WriteLESizeBuf)(A$$(usize_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeLESize(usize val))(mem_WriteLESizeBuf));
T_alias$((mem_WriteLE64Buf)(A$$(u64_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeLE64(u64 val))(mem_WriteLE64Buf));
T_alias$((mem_WriteLELongBuf)(A$$(ulong_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeLELong(ulong val))(mem_WriteLELongBuf));
T_alias$((mem_WriteLE32Buf)(A$$(u32_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeLE32(u32 val))(mem_WriteLE32Buf));
T_alias$((mem_WriteLE16Buf)(A$$(u16_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeLE16(u16 val))(mem_WriteLE16Buf));

T_alias$((mem_WriteBESizeBuf)(A$$(usize_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeBESize(usize val))(mem_WriteBESizeBuf));
T_alias$((mem_WriteBE64Buf)(A$$(u64_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeBE64(u64 val))(mem_WriteBE64Buf));
T_alias$((mem_WriteBELongBuf)(A$$(ulong_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeBELong(ulong val))(mem_WriteBELongBuf));
T_alias$((mem_WriteBE32Buf)(A$$(u32_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeBE32(u32 val))(mem_WriteBE32Buf));
T_alias$((mem_WriteBE16Buf)(A$$(u16_bytes, u8)));
$attr($inline_always)
$static fn_((mem_writeBE16(u16 val))(mem_WriteBE16Buf));

/* --- Alignment Functions --- */

T_alias$((mem_Log2Align)(u8));
T_impl_O$(mem_Log2Align);
T_alias$((mem_Align)(mem_Log2Align));
T_impl_O$(mem_Align);

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
$static fn_((mem_idxZ$u8(u8 sentinel, const u8* pz))(usize));
$attr($inline_always)
$static fn_((mem_lenZ0$u8(const u8* pz0))(usize));

$attr($inline_always)
$static fn_((mem_spanZ0$u8(const u8* pz0))(S_const$u8));
$attr($inline_always)
$static fn_((mem_spanZ0Mut$u8(u8* pz0))(S$u8));

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

$attr($inline_always)
$static fn_((mem_set0Bytes(S$u8 dst))(S$u8));
$attr($inline_always)
$static fn_((mem_set0P(u_P$raw dst))(u_P$raw));
#define T_use_mem_setP0$(_T...) __stmt__T_use_mem_setP0$(_T)
$attr($inline_always)
$static fn_((mem_set0(u_S$raw dst))(u_S$raw));
#define T_use_mem_set0$(_T...) __stmt__T_use_mem_set0$(_T)
$attr($inline_always)
$static fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8));
$attr($inline_always)
$static fn_((mem_setP(u_P$raw dst, u_V$raw val))(u_P$raw));
#define T_use_mem_setP$(_T...) __stmt__T_use_mem_setP$(_T)
$attr($inline_always)
$static fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw));
#define T_use_mem_set$(_T...) __stmt__T_use_mem_set$(_T)
$attr($inline_always)
$static fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8));
$attr($inline_always)
$static fn_((mem_copyP(u_P$raw dst, u_P_const$raw src))(u_P$raw));
#define T_use_mem_copyP$(_T...) __stmt__T_use_mem_copyP$(_T)
$attr($inline_always)
$static fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_copy$(_T...) __stmt__T_use_mem_copy$(_T)
$attr($inline_always)
$static fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8));
$attr($inline_always)
$static fn_((mem_moveP(u_P$raw dst, u_P_const$raw src))(u_P$raw));
#define T_use_mem_moveP$(_T...) __stmt__T_use_mem_moveP$(_T)
$attr($inline_always)
$static fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_move$(_T...) __stmt__T_use_mem_move$(_T)

$attr($inline_always)
$static fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_eqlP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_eqlP$(_T...) __stmt__T_use_mem_eqlP$(_T)
$attr($inline_always)
$static fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eql$(_T...) __stmt__T_use_mem_eql$(_T)
$attr($inline_always)
$static fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_neqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_neqP$(_T...) __stmt__T_use_mem_neqP$(_T)
$attr($inline_always)
$static fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_neq$(_T...) __stmt__T_use_mem_neq$(_T)

$attr($inline_always)
$static fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));
$attr($inline_always)
$static fn_((mem_ordP(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord));
#define T_use_mem_ordP$(_T...) __stmt__T_use_mem_ordP$(_T)
$attr($inline_always)
$static fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord));
#define T_use_mem_ord$(_T...) __stmt__T_use_mem_ord$(_T)
$attr($inline_always)
$static fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_eqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_eqP$(_T...) __stmt__T_use_mem_eqP$(_T)
$attr($inline_always)
$static fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eq$(_T...) __stmt__T_use_mem_eq$(_T)
$attr($inline_always)
$static fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_neP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_neP$(_T...) __stmt__T_use_mem_neP$(_T)
$attr($inline_always)
$static fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ne$(_T...) __stmt__T_use_mem_ne$(_T)
$attr($inline_always)
$static fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_ltP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_ltP$(_T...) __stmt__T_use_mem_ltP$(_T)
$attr($inline_always)
$static fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_lt$(_T...) __stmt__T_use_mem_lt$(_T)
$attr($inline_always)
$static fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_gtP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_gtP$(_T...) __stmt__T_use_mem_gtP$(_T)
$attr($inline_always)
$static fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_gt$(_T...) __stmt__T_use_mem_gt$(_T)
$attr($inline_always)
$static fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_leP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_leP$(_T...) __stmt__T_use_mem_leP$(_T)
$attr($inline_always)
$static fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_le$(_T...) __stmt__T_use_mem_le$(_T)
$attr($inline_always)
$static fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$attr($inline_always)
$static fn_((mem_geP(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
#define T_use_mem_geP$(_T...) __stmt__T_use_mem_geP$(_T)
$attr($inline_always)
$static fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ge$(_T...) __stmt__T_use_mem_ge$(_T)

$extern fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void));
$extern fn_((mem_swapP(u_P$raw lhs, u_P$raw rhs))(void));
#define T_use_mem_swapP$(_T...) __stmt__T_use_mem_swapP$(_T)
$extern fn_((mem_swap(u_S$raw lhs, u_S$raw rhs))(void));
#define T_use_mem_swap$(_T...) __stmt__T_use_mem_swap$(_T)
$attr($inline_always)
$static fn_((mem_reverseBytes(S$u8 seq))(void));
$attr($inline_always)
$static fn_((mem_reverse(u_S$raw seq))(void));
#define T_use_mem_reverse$(_T...) __stmt__T_use_mem_reverse$(_T)
$attr($inline_always)
$static fn_((mem_rotateBytes(S$u8 seq, usize amount))(void));
$attr($inline_always)
$static fn_((mem_rotate(u_S$raw seq, usize amount))(void));
#define T_use_mem_rotate$(_T...) __stmt__T_use_mem_rotate$(_T)

$attr($inline)
$static fn_((mem_findFirstUnitBytes(S_const$u8 haystack, u8 needle))(O$usize));
$attr($inline)
$static fn_((mem_findFirstUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize));
#define T_use_mem_findFirstUnit$(_T...) __stmt__T_use_mem_findFirstUnit$(_T)
$attr($inline)
$static fn_((mem_findLastUnitBytes(S_const$u8 haystack, u8 needle))(O$usize));
$attr($inline)
$static fn_((mem_findLastUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize));
#define T_use_mem_findLastUnit$(_T...) __stmt__T_use_mem_findLastUnit$(_T)
$attr($inline)
$static fn_((mem_findFirstSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize));
$attr($inline)
$static fn_((mem_findFirstSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize));
#define T_use_mem_findFirstSeq$(_T...) __stmt__T_use_mem_findFirstSeq$(_T)
$attr($inline)
$static fn_((mem_findLastSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize));
$attr($inline)
$static fn_((mem_findLastSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize));
#define T_use_mem_findLastSeq$(_T...) __stmt__T_use_mem_findLastSeq$(_T)
$attr($inline)
$static fn_((mem_findFirstAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize));
$attr($inline)
$static fn_((mem_findFirstAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize));
#define T_use_mem_findFirstAny$(_T...) __stmt__T_use_mem_findFirstAny$(_T)
$attr($inline)
$static fn_((mem_findLastAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize));
$attr($inline)
$static fn_((mem_findLastAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize));
#define T_use_mem_findLastAny$(_T...) __stmt__T_use_mem_findLastAny$(_T)
$attr($inline)
$static fn_((mem_findFirstNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize));
$attr($inline)
$static fn_((mem_findFirstNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize));
#define T_use_mem_findFirstNone$(_T...) __stmt__T_use_mem_findFirstNone$(_T)
$attr($inline)
$static fn_((mem_findLastNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize));
$attr($inline)
$static fn_((mem_findLastNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize));
#define T_use_mem_findLastNone$(_T...) __stmt__T_use_mem_findLastNone$(_T)
$attr($inline)
$static fn_((mem_findFirstDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize));
$attr($inline)
$static fn_((mem_findFirstDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize));
#define T_use_mem_findFirstDiff$(_T...) __stmt__T_use_mem_findFirstDiff$(_T)
$attr($inline)
$static fn_((mem_findLastDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize));
$attr($inline)
$static fn_((mem_findLastDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize));
#define T_use_mem_findLastDiff$(_T...) __stmt__T_use_mem_findLastDiff$(_T)

$attr($inline)
$static fn_((mem_countUnitBytes(S_const$u8 haystack, u8 needle))(usize));
$attr($inline)
$static fn_((mem_countUnit(u_S_const$raw haystack, u_V$raw needle))(usize));
#define T_use_mem_countUnit$(_T...) __stmt__T_use_mem_countUnit$(_T)
$attr($inline)
$static fn_((mem_countBytes(S_const$u8 haystack, S_const$u8 needle))(usize));
$attr($inline)
$static fn_((mem_count(u_S_const$raw haystack, u_S_const$raw needle))(usize));
#define T_use_mem_count$(_T...) __stmt__T_use_mem_count$(_T)
$attr($inline)
$static fn_((mem_containsUnitBytes(S_const$u8 haystack, u8 needle))(bool));
$attr($inline)
$static fn_((mem_containsUnit(u_S_const$raw haystack, u_V$raw needle))(bool));
#define T_use_mem_containsUnit$(_T...) __stmt__T_use_mem_containsUnit$(_T)
$attr($inline)
$static fn_((mem_containsBytes(S_const$u8 haystack, S_const$u8 needle))(bool));
$attr($inline)
$static fn_((mem_contains(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_contains$(_T...) __stmt__T_use_mem_contains$(_T)
$attr($inline)
$static fn_((mem_containsAtLeastUnitBytes(S_const$u8 haystack, u8 needle, usize minimum))(bool));
$attr($inline)
$static fn_((mem_containsAtLeastUnit(u_S_const$raw haystack, u_V$raw needle, usize minimum))(bool));
#define T_use_mem_containsAtLeastUnit$(_T...) __stmt__T_use_mem_containsAtLeastUnit$(_T)
$attr($inline)
$static fn_((mem_containsAtLeastBytes(S_const$u8 haystack, S_const$u8 needle, usize minimum))(bool));
$attr($inline)
$static fn_((mem_containsAtLeast(u_S_const$raw haystack, u_S_const$raw needle, usize minimum))(bool));
#define T_use_mem_containsAtLeast$(_T...) __stmt__T_use_mem_containsAtLeast$(_T)

T_alias$((mem_WindowIter_Bytes)(struct mem_WindowIter_Bytes {
    var_(buf, S_const$u8);
    var_(idx, O$usize);
    var_(size, usize);
    var_(advance, usize);
}));
#define mem_WindowIter$(_T...) __alias__mem_WindowIter$(_T)
T_alias$((mem_WindowIter)(struct mem_WindowIter {
    var_(buf, S_const$raw);
    var_(idx, O$usize);
    var_(size, usize);
    var_(advance, usize);
    var_(type, debug_TypeInfo);
}));
#define T_use_mem_WindowIter$(_T...) __stmt__T_use_mem_WindowIter$(_T)
$extern fn_((mem_windowBytes(S_const$u8 buf, usize size, usize advance))(mem_WindowIter_Bytes));
$extern fn_((mem_window(u_S_const$raw buf, usize size, usize advance))(mem_WindowIter));
#define T_use_mem_window$(_T...) __stmt__T_use_mem_window$(_T)
$extern fn_((mem_WindowIter_resetBytes(mem_WindowIter_Bytes* self))(void));
$extern fn_((mem_WindowIter_reset(mem_WindowIter* self))(void));
#define T_use_mem_WindowIter_reset$(_T...) __stmt__T_use_mem_WindowIter_reset$(_T)
$extern fn_((mem_WindowIter_nextBytes(mem_WindowIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_WindowIter_next(mem_WindowIter* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_WindowIter_next$(_T...) __stmt__T_use_mem_WindowIter_next$(_T)

$attr($inline)
$static fn_((mem_startsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool));
$attr($inline)
$static fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_startsWith$(_T...) __stmt__T_use_mem_startsWith$(_T)
$attr($inline)
$static fn_((mem_endsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool));
$attr($inline)
$static fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_endsWith$(_T...) __stmt__T_use_mem_endsWith$(_T)

$extern fn_((mem_trimStartBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8));
$extern fn_((mem_trimStart(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw));
#define T_use_mem_trimStart$(_T...) __stmt__T_use_mem_trimStart$(_T)
$extern fn_((mem_trimEndBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8));
$extern fn_((mem_trimEnd(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw));
#define T_use_mem_trimEnd$(_T...) __stmt__T_use_mem_trimEnd$(_T)
$extern fn_((mem_trimBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8));
$extern fn_((mem_trim(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw));
#define T_use_mem_trim$(_T...) __stmt__T_use_mem_trim$(_T)

$attr($must_check)
$extern fn_((mem_catBytes(S_const$S_const$u8 slices, S$u8 out))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_cat(u_S_const$S_const$raw slices, u_S$raw out))(mem_E$u_S$raw));
#define T_use_mem_cat$(_T...) __stmt__T_use_mem_cat$(_T)
$extern fn_((mem_catWithinBytes(S_const$S_const$u8 slices, S$u8 out))(S$u8));
$extern fn_((mem_catWithin(u_S_const$S_const$raw slices, u_S$raw out))(u_S$raw));
#define T_use_mem_catWithin$(_T...) __stmt__T_use_mem_catWithin$(_T)
$attr($must_check)
$extern fn_((mem_catAllocBytes(S_const$S_const$u8 slices, mem_Alctr gpa))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_catAlloc(TypeInfo type, u_S_const$S_const$raw slices, mem_Alctr gpa))(mem_E$u_S$raw));
#define T_use_mem_catAlloc$(_T...) __stmt__T_use_mem_catAlloc$(_T)

$attr($must_check)
$extern fn_((mem_joinBytes(S_const$u8 sep, S_const$S_const$u8 slices, S$u8 out))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_join(u_S_const$raw sep, u_S_const$S_const$raw slices, u_S$raw out))(mem_E$u_S$raw));
#define T_use_mem_join$(_T...) __stmt__T_use_mem_join$(_T)
$extern fn_((mem_joinWithinBytes(S_const$u8 sep, S_const$S_const$u8 slices, S$u8 out))(S$u8));
$extern fn_((mem_joinWithin(u_S_const$raw sep, u_S_const$S_const$raw slices, u_S$raw out))(u_S$raw));
#define T_use_mem_joinWithin$(_T...) __stmt__T_use_mem_joinWithin$(_T)
$attr($must_check)
$extern fn_((mem_joinAllocBytes(S_const$u8 sep, S_const$S_const$u8 slices, mem_Alctr gpa))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_joinAlloc(u_S_const$raw sep, u_S_const$S_const$raw slices, mem_Alctr gpa))(mem_E$u_S$raw));
#define T_use_mem_joinAlloc$(_T...) __stmt__T_use_mem_joinAlloc$(_T)

$attr($must_check)
$extern fn_((mem_padLeftBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padLeft(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw));
#define T_use_mem_padLeft$(_T...) __stmt__T_use_mem_padLeft$(_T)
$extern fn_((mem_padLeftWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8));
$extern fn_((mem_padLeftWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw));
#define T_use_mem_padLeftWithin$(_T...) __stmt__T_use_mem_padLeftWithin$(_T)
$attr($must_check)
$extern fn_((mem_padLeftAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padLeftAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw));
#define T_use_mem_padLeftAlloc$(_T...) __stmt__T_use_mem_padLeftAlloc$(_T)

$attr($must_check)
$extern fn_((mem_padRightBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padRight(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw));
#define T_use_mem_padRight$(_T...) __stmt__T_use_mem_padRight$(_T)
$extern fn_((mem_padRightWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8));
$extern fn_((mem_padRightWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw));
#define T_use_mem_padRightWithin$(_T...) __stmt__T_use_mem_padRightWithin$(_T)
$attr($must_check)
$extern fn_((mem_padRightAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padRightAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw));
#define T_use_mem_padRightAlloc$(_T...) __stmt__T_use_mem_padRightAlloc$(_T)

$attr($must_check)
$extern fn_((mem_padCenterBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padCenter(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw));
#define T_use_mem_padCenter$(_T...) __stmt__T_use_mem_padCenter$(_T)
$extern fn_((mem_padCenterWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8));
$extern fn_((mem_padCenterWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw));
#define T_use_mem_padCenterWithin$(_T...) __stmt__T_use_mem_padCenterWithin$(_T)
$attr($must_check)
$extern fn_((mem_padCenterAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8));
$attr($must_check)
$extern fn_((mem_padCenterAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw));
#define T_use_mem_padCenterAlloc$(_T...) __stmt__T_use_mem_padCenterAlloc$(_T)

#define mem_Cutted$(_T...) __alias__mem_Cutted$(_T)
T_alias$((mem_Cutted_Bytes)(struct mem_Cutted_Bytes {
    var_(before, S_const$u8);
    var_(after, S_const$u8);
}));
T_use_O$(mem_Cutted_Bytes);
T_alias$((mem_Cutted)(struct mem_Cutted {
    var_(before, S_const$raw);
    var_(after, S_const$raw);
    var_(type, debug_TypeInfo);
}));
T_use_O$(mem_Cutted);
#define T_use_mem_Cutted$(_T...) __stmt__T_use_mem_Cutted$(_T)
$attr($inline_always)
$static fn_((mem_Cutted_beforeBytes(mem_Cutted_Bytes self))(S_const$u8));
$attr($inline_always)
$static fn_((mem_Cutted_before(mem_Cutted self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_Cutted_before$(_T...) __stmt__T_use_mem_Cutted_before$(_T)
$attr($inline_always)
$static fn_((mem_Cutted_afterBytes(mem_Cutted_Bytes self))(S_const$u8));
$attr($inline_always)
$static fn_((mem_Cutted_after(mem_Cutted self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_Cutted_after$(_T...) __stmt__T_use_mem_Cutted_after$(_T)

$extern fn_((mem_cutFirstUnitBytes(S_const$u8 haystack, u8 needle))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutFirstUnit(u_S_const$raw haystack, u_V$raw needle))(O$mem_Cutted));
#define T_use_mem_cutFirstUnit$(_T...) __stmt__T_use_mem_cutFirstUnit$(_T)
$extern fn_((mem_cutLastUnitBytes(S_const$u8 haystack, u8 needle))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutLastUnit(u_S_const$raw haystack, u_V$raw needle))(O$mem_Cutted));
#define T_use_mem_cutLastUnit$(_T...) __stmt__T_use_mem_cutLastUnit$(_T)
$extern fn_((mem_cutFirstSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutFirstSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$mem_Cutted));
#define T_use_mem_cutFirstSeq$(_T...) __stmt__T_use_mem_cutFirstSeq$(_T)
$extern fn_((mem_cutLastSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutLastSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$mem_Cutted));
#define T_use_mem_cutLastSeq$(_T...) __stmt__T_use_mem_cutLastSeq$(_T)
$extern fn_((mem_cutFirstAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutFirstAny(u_S_const$raw haystack, u_S_const$raw needles))(O$mem_Cutted));
#define T_use_mem_cutFirstAny$(_T...) __stmt__T_use_mem_cutFirstAny$(_T)
$extern fn_((mem_cutLastAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$mem_Cutted_Bytes));
$extern fn_((mem_cutLastAny(u_S_const$raw haystack, u_S_const$raw needles))(O$mem_Cutted));
#define T_use_mem_cutLastAny$(_T...) __stmt__T_use_mem_cutLastAny$(_T)

T_alias$((mem_Delim)(enum_((mem_Delim $fits($packed))(
    mem_Delim_unit,
    mem_Delim_seq,
    mem_Delim_any
))));
T_alias$((mem_Delim_Bytes)(variant_((mem_Delim_Bytes $maps(mem_Delim))(
    (mem_Delim_unit, u8),
    (mem_Delim_seq, S_const$u8),
    (mem_Delim_any, S_const$u8)
))));
#define mem_Delim$(_T...) __alias__mem_Delim$(_T)
T_alias$((mem_Delim$raw)(variant_((mem_Delim$raw $maps(mem_Delim))(
    (mem_Delim_unit, V$raw),
    (mem_Delim_seq, S_const$raw),
    (mem_Delim_any, S_const$raw)
))));
#define T_use_mem_Delim$(_T...) __stmt__T_use_mem_Delim$(_T)

T_alias$((mem_TokzIter_Bytes)(struct mem_TokzIter_Bytes {
    var_(buf, S_const$u8);
    var_(idx, usize);
    var_(delim, mem_Delim_Bytes);
}));
#define mem_TokzIter$(_T...) __alias__mem_TokzIter$(_T)
T_alias$((mem_TokzIter$raw)(struct mem_TokzIter$raw {
    var_(buf, S_const$raw);
    var_(idx, usize);
    var_(type, debug_TypeInfo);
    var_(delim_, mem_Delim$raw) $flexible;
}));
T_use_P$(mem_TokzIter$raw);
T_alias$((V$mem_TokzIter$raw)(P$mem_TokzIter$raw));
#define T_use_mem_TokzIter$(_T...) __stmt__T_use_mem_TokzIter$(_T)
$extern fn_((mem_tokzUnitBytes(S_const$u8 buf, u8 unit))(mem_TokzIter_Bytes));
$extern fn_((mem_tokzUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzUnit$(_T...) __stmt__T_use_mem_tokzUnit$(_T)
$extern fn_((mem_tokzSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_TokzIter_Bytes));
$extern fn_((mem_tokzSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzSeq$(_T...) __stmt__T_use_mem_tokzSeq$(_T)
$extern fn_((mem_tokzAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_TokzIter_Bytes));
$extern fn_((mem_tokzAny(u_S_const$raw buf, u_S_const$raw any, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw));
#define T_use_mem_tokzAny$(_T...) __stmt__T_use_mem_tokzAny$(_T)
$extern fn_((mem_TokzIter_resetBytes(mem_TokzIter_Bytes* self))(void));
$extern fn_((mem_TokzIter_reset(mem_TokzIter$raw* self))(void));
#define T_use_mem_TokzIter_reset$(_T...) __stmt__T_use_mem_TokzIter_reset$(_T)
$extern fn_((mem_TokzIter_nextBytes(mem_TokzIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_TokzIter_next(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzIter_next$(_T...) __stmt__T_use_mem_TokzIter_next$(_T)
$extern fn_((mem_TokzIter_peekBytes(mem_TokzIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_TokzIter_peek(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzIter_peek$(_T...) __stmt__T_use_mem_TokzIter_peek$(_T)
$extern fn_((mem_TokzIter_restBytes(mem_TokzIter_Bytes* self))(S_const$u8));
$extern fn_((mem_TokzIter_rest(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_TokzIter_rest$(_T...) __stmt__T_use_mem_TokzIter_rest$(_T)

T_alias$((mem_TokzBwdIter_Bytes)(struct mem_TokzBwdIter_Bytes {
    var_(buf, S_const$u8);
    var_(idx, usize);
    var_(delim, mem_Delim_Bytes);
}));
#define mem_TokzBwdIter$(_T...) __alias__mem_TokzBwdIter$(_T)
T_alias$((mem_TokzBwdIter$raw)(struct mem_TokzBwdIter$raw {
    var_(buf, S_const$raw);
    var_(idx, usize);
    var_(type, debug_TypeInfo);
    var_(delim_, mem_Delim$raw) $flexible;
}));
T_use_P$(mem_TokzBwdIter$raw);
T_alias$((V$mem_TokzBwdIter$raw)(P$mem_TokzBwdIter$raw));
#define T_use_mem_TokzBwdIter$(_T...) __stmt__T_use_mem_TokzBwdIter$(_T)
$extern fn_((mem_tokzBwdUnitBytes(S_const$u8 buf, u8 unit))(mem_TokzBwdIter_Bytes));
$extern fn_((mem_tokzBwdUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw));
#define T_use_mem_tokzBwdUnit$(_T...) __stmt__T_use_mem_tokzBwdUnit$(_T)
$extern fn_((mem_tokzBwdSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_TokzBwdIter_Bytes));
$extern fn_((mem_tokzBwdSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw));
#define T_use_mem_tokzBwdSeq$(_T...) __stmt__T_use_mem_tokzBwdSeq$(_T)
$extern fn_((mem_tokzBwdAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_TokzBwdIter_Bytes));
$extern fn_((mem_tokzBwdAny(u_S_const$raw buf, u_S_const$raw any, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw));
#define T_use_mem_tokzBwdAny$(_T...) __stmt__T_use_mem_tokzBwdAny$(_T)
$extern fn_((mem_TokzBwdIter_resetBytes(mem_TokzBwdIter_Bytes* self))(void));
$extern fn_((mem_TokzBwdIter_reset(mem_TokzBwdIter$raw* self))(void));
#define T_use_mem_TokzBwdIter_reset$(_T...) __stmt__T_use_mem_TokzBwdIter_reset$(_T)
$extern fn_((mem_TokzBwdIter_nextBytes(mem_TokzBwdIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_TokzBwdIter_next(mem_TokzBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzBwdIter_next$(_T...) __stmt__T_use_mem_TokzBwdIter_next$(_T)
$extern fn_((mem_TokzBwdIter_peekBytes(mem_TokzBwdIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_TokzBwdIter_peek(mem_TokzBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_TokzBwdIter_peek$(_T...) __stmt__T_use_mem_TokzBwdIter_peek$(_T)
$extern fn_((mem_TokzBwdIter_restBytes(mem_TokzBwdIter_Bytes* self))(S_const$u8));
$extern fn_((mem_TokzBwdIter_rest(mem_TokzBwdIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_TokzBwdIter_rest$(_T...) __stmt__T_use_mem_TokzBwdIter_rest$(_T)

#define mem_SplitIter$(_T...) __alias__mem_SplitIter$(_T)
T_alias$((mem_SplitIter_Bytes)(struct mem_SplitIter_Bytes {
    var_(buf, S_const$u8);
    var_(idx, O$usize);
    var_(delim, mem_Delim_Bytes);
}));
T_alias$((mem_SplitIter$raw)(struct mem_SplitIter$raw {
    var_(buf, S_const$raw);
    var_(idx, O$usize);
    var_(type, debug_TypeInfo);
    var_(delim_, mem_Delim$raw) $flexible;
}));
T_use_P$(mem_SplitIter$raw);
T_alias$((V$mem_SplitIter$raw)(P$mem_SplitIter$raw));
#define T_use_mem_SplitIter$(_T...) __stmt__T_use_mem_SplitIter$(_T)
$extern fn_((mem_splitUnitBytes(S_const$u8 buf, u8 unit))(mem_SplitIter_Bytes));
$extern fn_((mem_splitUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitUnit$(_T...) __stmt__T_use_mem_splitUnit$(_T)
$extern fn_((mem_splitSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_SplitIter_Bytes));
$extern fn_((mem_splitSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitSeq$(_T...) __stmt__T_use_mem_splitSeq$(_T)
$extern fn_((mem_splitAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_SplitIter_Bytes));
$extern fn_((mem_splitAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw));
#define T_use_mem_splitAny$(_T...) __stmt__T_use_mem_splitAny$(_T)
$extern fn_((mem_SplitIter_resetBytes(mem_SplitIter_Bytes* self))(void));
$extern fn_((mem_SplitIter_reset(mem_SplitIter$raw* self))(void));
#define T_use_mem_SplitIter_reset$(_T...) __stmt__T_use_mem_SplitIter_reset$(_T)
$extern fn_((mem_SplitIter_firstBytes(mem_SplitIter_Bytes* self))(S_const$u8));
$extern fn_((mem_SplitIter_first(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitIter_first$(_T...) __stmt__T_use_mem_SplitIter_first$(_T)
$extern fn_((mem_SplitIter_nextBytes(mem_SplitIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_SplitIter_next(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitIter_next$(_T...) __stmt__T_use_mem_SplitIter_next$(_T)
$extern fn_((mem_SplitIter_peekBytes(mem_SplitIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_SplitIter_peek(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitIter_peek$(_T...) __stmt__T_use_mem_SplitIter_peek$(_T)
$extern fn_((mem_SplitIter_restBytes(mem_SplitIter_Bytes* self))(S_const$u8));
$extern fn_((mem_SplitIter_rest(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitIter_rest$(_T...) __stmt__T_use_mem_SplitIter_rest$(_T)

#define mem_SplitBwdIter$(_T...) __alias__mem_SplitBwdIter$(_T)
T_alias$((mem_SplitBwdIter_Bytes)(struct mem_SplitBwdIter_Bytes {
    var_(buf, S_const$u8);
    var_(idx, O$usize);
    var_(delim, mem_Delim_Bytes);
}));
T_alias$((mem_SplitBwdIter$raw)(struct mem_SplitBwdIter$raw {
    var_(buf, S_const$raw);
    var_(idx, O$usize);
    var_(type, debug_TypeInfo);
    var_(delim_, mem_Delim$raw) $flexible;
}));
T_use_P$(mem_SplitBwdIter$raw);
T_alias$((V$mem_SplitBwdIter$raw)(P$mem_SplitBwdIter$raw));
#define T_use_mem_SplitBwdIter$(_T...) __stmt__T_use_mem_SplitBwdIter$(_T)
$extern fn_((mem_splitBwdUnitBytes(S_const$u8 buf, u8 unit))(mem_SplitBwdIter_Bytes));
$extern fn_((mem_splitBwdUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw));
#define T_use_mem_splitBwdUnit$(_T...) __stmt__T_use_mem_splitBwdUnit$(_T)
$extern fn_((mem_splitBwdSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_SplitBwdIter_Bytes));
$extern fn_((mem_splitBwdSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw));
#define T_use_mem_splitBwdSeq$(_T...) __stmt__T_use_mem_splitBwdSeq$(_T)
$extern fn_((mem_splitBwdAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_SplitBwdIter_Bytes));
$extern fn_((mem_splitBwdAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw));
#define T_use_mem_splitBwdAny$(_T...) __stmt__T_use_mem_splitBwdAny$(_T)
$extern fn_((mem_SplitBwdIter_resetBytes(mem_SplitBwdIter_Bytes* self))(void));
$extern fn_((mem_SplitBwdIter_reset(mem_SplitBwdIter$raw* self))(void));
#define T_use_mem_SplitBwdIter_reset$(_T...) __stmt__T_use_mem_SplitBwdIter_reset$(_T)
$extern fn_((mem_SplitBwdIter_firstBytes(mem_SplitBwdIter_Bytes* self))(S_const$u8));
$extern fn_((mem_SplitBwdIter_first(mem_SplitBwdIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitBwdIter_first$(_T...) __stmt__T_use_mem_SplitBwdIter_first$(_T)
$extern fn_((mem_SplitBwdIter_nextBytes(mem_SplitBwdIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_SplitBwdIter_next(mem_SplitBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitBwdIter_next$(_T...) __stmt__T_use_mem_SplitBwdIter_next$(_T)
$extern fn_((mem_SplitBwdIter_peekBytes(mem_SplitBwdIter_Bytes* self))(O$S_const$u8));
$extern fn_((mem_SplitBwdIter_peek(mem_SplitBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw));
#define T_use_mem_SplitBwdIter_peek$(_T...) __stmt__T_use_mem_SplitBwdIter_peek$(_T)
$extern fn_((mem_SplitBwdIter_restBytes(mem_SplitBwdIter_Bytes* self))(S_const$u8));
$extern fn_((mem_SplitBwdIter_rest(mem_SplitBwdIter$raw* self, TypeInfo type))(u_S_const$raw));
#define T_use_mem_SplitBwdIter_rest$(_T...) __stmt__T_use_mem_SplitBwdIter_rest$(_T)

/*========== Macros and Definitions =========================================*/

/* --- Memory Constants --- */

#define __val__mem_page_size (pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_wasm64)(usize_(64) * 1024)), \
        pp_case_((arch_type_wasm32)(usize_(64) * 1024)), \
        pp_case_((arch_type_aarch64)(usize_(16) * 1024)), \
        pp_default_((usize_(4) * 1024)) \
    ) pp_end \
))

#if in_analysis_active_only || in_comptime
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
    const u64 debruijn = u64_(0x03F79D71B4CA8B09ull);
    // (x & -x) isolates the lowest set bit
    return lookup[((x & (0 - x)) * debruijn) >> 58];
#endif
};
fn_((mem_trailingZerosLong(ulong x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    return pp_if_(plat_long_is_64bit)(
        pp_then_(mem_trailingZeros64(x)),
        pp_else_(mem_trailingZeros32(x)));
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
    const u32 debruijn = u32_(0x077CB531u);
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
    x = x - ((x >> 1) & u64_(0x5555555555555555ull));
    x = (x & u64_(0x3333333333333333ull)) + ((x >> 2) & u64_(0x3333333333333333ull));
    x = (x + (x >> 4)) & u64_(0x0F0F0F0F0F0F0F0Full);
    return 64 - (u32)((x * u64_(0x0101010101010101ull)) >> 56);
#endif
};
fn_((mem_leadingZerosLong(ulong x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    return pp_if_(plat_long_is_64bit)(
        pp_then_(mem_leadingZeros64(x)),
        pp_else_(mem_leadingZeros32(x)));
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
    return int_swapBytes_static(x);
#else
    return ((x & u64_(0xFF00000000000000ull)) >> 56)
         | ((x & u64_(0x00FF000000000000ull)) >> 40)
         | ((x & u64_(0x0000FF0000000000ull)) >> 24)
         | ((x & u64_(0x000000FF00000000ull)) >> 8)
         | ((x & u64_(0x00000000FF000000ull)) << 8)
         | ((x & u64_(0x0000000000FF0000ull)) << 24)
         | ((x & u64_(0x000000000000FF00ull)) << 40)
         | ((x & u64_(0x00000000000000FFull)) << 56);
#endif
};
fn_((mem_swapBytesLong(ulong x))(ulong)) {
#if defined(__GNUC__) || defined(__clang__)
    return int_swapBytes_static(x);
#else
    return pp_if_(plat_long_is_64bit)(
        pp_then_(mem_swapBytes64(x)),
        pp_else_(mem_swapBytes32(x)));
#endif
};
fn_((mem_swapBytes32(u32 x))(u32)) {
#if defined(__GNUC__) || defined(__clang__)
    return int_swapBytes_static(x);
#else
    return ((x & u32_(0xFF000000u)) >> 24)
         | ((x & u32_(0x00FF0000u)) >> 8)
         | ((x & u32_(0x0000FF00u)) << 8)
         | ((x & u32_(0x000000FFu)) << 24);
#endif
};
fn_((mem_swapBytes16(u16 x))(u16)) {
#if defined(__GNUC__) || defined(__clang__)
    return int_swapBytes_static(x);
#else
    return (x << 8) | (x >> 8);
#endif
};

/* --- Endian Conversion --- */

fn_((mem_littleToNativeSize(usize x))(usize)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesSize(x)));
};
fn_((mem_littleToNative64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_littleToNativeLong(ulong x))(ulong)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesLong(x)));
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
fn_((mem_bigToNativeLong(ulong x))(ulong)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesLong(x)));
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
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesSize(x)));
};
fn_((mem_nativeToLittle64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_nativeToLittleLong(ulong x))(ulong)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesLong(x)));
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
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesSize(x)));
};
fn_((mem_nativeToBig64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytes64(x)));
};
fn_((mem_nativeToBigLong(ulong x))(ulong)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_swapBytesLong(x)));
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

fn_((mem_readLESize(mem_ReadLESizeBuf bytes))(usize)) {
    return mem_littleToNativeSize(bitCast$((usize)(bytes)));
};
fn_((mem_readLE64(mem_ReadLE64Buf bytes))(u64)) {
    return mem_littleToNative64(bitCast$((u64)(bytes)));
};
fn_((mem_readLELong(mem_ReadLELongBuf bytes))(ulong)) {
    return mem_littleToNativeLong(bitCast$((ulong)(bytes)));
};
fn_((mem_readLE32(mem_ReadLE32Buf bytes))(u32)) {
    return mem_littleToNative32(bitCast$((u32)(bytes)));
};
fn_((mem_readLE16(mem_ReadLE16Buf bytes))(u16)) {
    return mem_littleToNative16(bitCast$((u16)(bytes)));
};

fn_((mem_readBESize(mem_ReadBESizeBuf bytes))(usize)) {
    return mem_bigToNativeSize(bitCast$((usize)(bytes)));
};
fn_((mem_readBE64(mem_ReadBE64Buf bytes))(u64)) {
    return mem_bigToNative64(bitCast$((u64)(bytes)));
};
fn_((mem_readBELong(mem_ReadBELongBuf bytes))(ulong)) {
    return mem_bigToNativeLong(bitCast$((ulong)(bytes)));
};
fn_((mem_readBE32(mem_ReadBE32Buf bytes))(u32)) {
    return mem_bigToNative32(bitCast$((u32)(bytes)));
};
fn_((mem_readBE16(mem_ReadBE16Buf bytes))(u16)) {
    return mem_bigToNative16(bitCast$((u16)(bytes)));
};

fn_((mem_writeLESize(usize val))(mem_WriteLESizeBuf)) {
    return bitCast$((mem_WriteLESizeBuf)(mem_nativeToLittleSize(val)));
};
fn_((mem_writeLE64(u64 val))(mem_WriteLE64Buf)) {
    return bitCast$((mem_WriteLE64Buf)(mem_nativeToLittle64(val)));
};
fn_((mem_writeLELong(ulong val))(mem_WriteLELongBuf)) {
    return bitCast$((mem_WriteLELongBuf)(mem_nativeToLittleLong(val)));
};
fn_((mem_writeLE32(u32 val))(mem_WriteLE32Buf)) {
    return bitCast$((mem_WriteLE32Buf)(mem_nativeToLittle32(val)));
};
fn_((mem_writeLE16(u16 val))(mem_WriteLE16Buf)) {
    return bitCast$((mem_WriteLE16Buf)(mem_nativeToLittle16(val)));
};

fn_((mem_writeBESize(usize val))(mem_WriteBESizeBuf)) {
    return bitCast$((mem_WriteBESizeBuf)(mem_nativeToBigSize(val)));
};
fn_((mem_writeBE64(u64 val))(mem_WriteBE64Buf)) {
    return bitCast$((mem_WriteBE64Buf)(mem_nativeToBig64(val)));
};
fn_((mem_writeBELong(ulong val))(mem_WriteBELongBuf)) {
    return bitCast$((mem_WriteBELongBuf)(mem_nativeToBigLong(val)));
};
fn_((mem_writeBE32(u32 val))(mem_WriteBE32Buf)) {
    return bitCast$((mem_WriteBE32Buf)(mem_nativeToBig32(val)));
};
fn_((mem_writeBE16(u16 val))(mem_WriteBE16Buf)) {
    return bitCast$((mem_WriteBE16Buf)(mem_nativeToBig16(val)));
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

fn_((mem_idxZ$u8(u8 sentinel, const u8* pz))(usize)) {
    claim_assert_nonnull(pz);
    var_(idx, usize) = 0;
    while (*P_at((pz)[idx]) != sentinel) { ++idx; }
    return idx;
};
fn_((mem_lenZ0$u8(const u8* pz0))(usize)) {
    claim_assert_nonnull(pz0);
    return mem_idxZ$u8(u8_c('\0'), pz0);
};

fn_((mem_spanZ0$u8(const u8* pz0))(S_const$u8)) {
    claim_assert_nonnull(pz0);
    return (S_const$u8){ .ptr = pz0, .len = mem_lenZ0$u8(pz0) };
};
fn_((mem_spanZ0Mut$u8(u8* pz0))(S$u8)) {
    claim_assert_nonnull(pz0);
    return (S$u8){ .ptr = pz0, .len = mem_lenZ0$u8(pz0) };
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

fn_((mem_set0Bytes(S$u8 dst))(S$u8)) {
    claim_assert_nonnullS(dst);
    return u_memset0S(u_anyS(dst)), dst;
};
fn_((mem_set0P(u_P$raw dst))(u_P$raw)) {
    claim_assert_nonnull(dst.raw);
    return u_memset0(dst);
};
fn_((mem_set0(u_S$raw dst))(u_S$raw)) {
    claim_assert_nonnullS(dst);
    return u_memset0S(dst);
};

fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8)) {
    claim_assert_nonnullS(dst);
    return u_memsetS(u_anyS(dst), u_anyV(val)), dst;
};
fn_((mem_setP(u_P$raw dst, u_V$raw val))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(val.inner);
    claim_assert(TypeInfo_eql(dst.type, val.inner_type));
    return u_memset(dst, val);
};
fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnull(val.inner);
    claim_assert(TypeInfo_eql(dst.type, val.inner_type));
    for_(($us(dst))(elem)) { u_memset(elem, val); } $end(for);
    return dst;
};

fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    return u_memcpyS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src)), dst;
};
fn_((mem_copyP(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(src.raw);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    return u_memcpy(dst, src);
};
fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s)) { u_memcpy(d, s); } $end(for);
    return dst;
};

fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    return u_memmoveS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src)), dst;
};
fn_((mem_moveP(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(src.raw);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    return u_memmove(dst, src);
};
fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s)) { u_memmove(d, s); } $end(for);
    return dst;
};

fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    if (lhs.len != rhs.len) return false;
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) return true;
    return u_memeqlS(u_anyS(lhs), u_anyS(rhs));
};
fn_((mem_eqlP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    if (lhs.raw == rhs.raw) return true;
    return u_memeql(lhs, rhs);
};
fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    if (lhs.len != rhs.len) return false;
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) return true;
    for_(($us(lhs), $us(rhs))(l, r)) {
        if (!u_memeql(l, r)) return false;
    } $end(for);
    return true;
};

fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return !mem_eqlBytes(lhs, rhs); };
fn_((mem_neqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return !mem_eqlP(lhs, rhs); };
fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return !mem_eql(lhs, rhs); };

fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let len = int_min(lhs.len, rhs.len);
    let result = u_memordS(u_sliceS(u_anyS(lhs), $r(0, len)), u_sliceS(u_anyS(rhs), $r(0, len)));
    return result != cmp_Ord_eq ? result : pri_ord(lhs.len, rhs.len);
};
fn_((mem_ordP(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    return u_memord(lhs, rhs);
};
fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let len = int_min(lhs.len, rhs.len);
    for_(($us(u_sliceS(lhs, $r(0, len))), $us(u_sliceS(rhs, $r(0, len))))(l, r)) {
        switch (u_memord(l, r)) {
        case cmp_Ord_lt: return cmp_Ord_lt;
        case cmp_Ord_gt: return cmp_Ord_gt;
        case cmp_Ord_eq: continue;
        }
    } $end(for);
    return pri_ord(lhs.len, rhs.len);
};

fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isEq(mem_ordBytes(lhs, rhs)); };
fn_((mem_eqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isEq(mem_ordP(lhs, rhs)); };
fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isEq(mem_ord(lhs, rhs)); };

fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isNe(mem_ordBytes(lhs, rhs)); };
fn_((mem_neP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isNe(mem_ordP(lhs, rhs)); };
fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isNe(mem_ord(lhs, rhs)); };

fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isLt(mem_ordBytes(lhs, rhs)); };
fn_((mem_ltP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isLt(mem_ordP(lhs, rhs)); };
fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isLt(mem_ord(lhs, rhs)); };

fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isGt(mem_ordBytes(lhs, rhs)); };
fn_((mem_gtP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isGt(mem_ordP(lhs, rhs)); };
fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isGt(mem_ord(lhs, rhs)); };

fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isLe(mem_ordBytes(lhs, rhs)); };
fn_((mem_leP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isLe(mem_ordP(lhs, rhs)); };
fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isLe(mem_ord(lhs, rhs)); };

fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isGe(mem_ordBytes(lhs, rhs)); };
fn_((mem_geP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isGe(mem_ordP(lhs, rhs)); };
fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isGe(mem_ord(lhs, rhs)); };

fn_((mem_reverseBytes(S$u8 seq))(void)) {
    claim_assert_nonnullS(seq);
    let_(l, usize) = 0;
    let r = seq.len / 2;
    for_(($r(l, r))(p)) {
        mem_swapP(u_anyP(S_at((seq)[p])), u_anyP(S_at((seq)[seq.len - p - 1])));
    } $end(for);
};
fn_((mem_reverse(u_S$raw seq))(void)) {
    claim_assert_nonnullS(seq);
    let_(l, usize) = 0;
    let r = seq.len / 2;
    for_(($r(l, r))(p)) {
        mem_swapP(u_atS(seq, p), u_atS(seq, seq.len - p - 1));
    } $end(for);
};
fn_((mem_rotateBytes(S$u8 seq, usize amount))(void)) {
    claim_assert_nonnullS(seq);
    mem_reverseBytes(S_prefix((seq)(amount)));
    mem_reverseBytes(S_suffix((seq)(amount)));
    mem_reverseBytes(seq);
};
fn_((mem_rotate(u_S$raw seq, usize amount))(void)) {
    claim_assert_nonnullS(seq);
    mem_reverse(u_prefixS(seq, amount));
    mem_reverse(u_suffixS(seq, amount));
    mem_reverse(seq);
};

fn_((mem_findFirstUnitBytes(S_const$u8 haystack, u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (*item == needle) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    for_(($us(haystack), $rf(0))(item, idx)) {
        if (u_memeql(item, needle.ref.as_const)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastUnitBytes(S_const$u8 haystack, u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (*item == needle) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    for_($rev($us(haystack), $rt(haystack.len))(item, idx)) {
        if (u_memeql(item, needle.ref.as_const)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_(($rt($incl(end)))(idx)) {
        if (mem_eqlBytes(S_prefix((S_suffix((haystack)(idx)))(needle.len)), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_(($rt($incl(end)))(idx)) {
        if (mem_eql(u_prefixS(u_suffixS(haystack, idx), needle.len), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_($rev($rt($incl(end)))(idx)) {
        if (mem_eqlBytes(S_prefix((S_suffix((haystack)(idx)))(needle.len)), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_($rev($rt($incl(end)))(idx)) {
        if (mem_eql(u_prefixS(u_suffixS(haystack, idx), needle.len), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (mem_containsUnitBytes(needles, *item)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_(($us(haystack), $rf(0))(item, idx)) {
        if (mem_containsUnit(needles, u_deref(item))) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (mem_containsUnitBytes(needles, *item)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_($rev($us(haystack), $rt(haystack.len))(item, idx)) {
        if (mem_containsUnit(needles, u_deref(item))) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (!mem_containsUnitBytes(needles, *item)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_(($us(haystack), $rf(0))(item, idx)) {
        if (!mem_containsUnit(needles, u_deref(item))) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (!mem_containsUnitBytes(needles, *item)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_($rev($us(haystack), $rt(haystack.len))(item, idx)) {
        if (!mem_containsUnit(needles, u_deref(item))) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_(($rt(shortest))(idx)) {
        if (*S_at((lhs)[idx]) != *S_at((rhs)[idx])) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findFirstDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_(($rt(shortest))(idx)) {
        if (!u_memeql(u_atS(lhs, idx), u_atS(rhs, idx))) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findLastDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_($rev($rt(shortest))(idx)) {
        if (*S_at((lhs)[idx]) != *S_at((rhs)[idx])) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findLastDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_($rev($rt(shortest))(idx)) {
        if (!u_memeql(u_atS(lhs, idx), u_atS(rhs, idx))) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);

fn_((mem_countUnitBytes(S_const$u8 haystack, u8 needle))(usize)) {
    claim_assert_nonnullS(haystack);
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    for_(($s(S_suffix((haystack)idx)))(item)) {
        found += intFromBool(pri_eql(*item, needle));
    } $end(for);
    return found;
};
fn_((mem_countUnit(u_S_const$raw haystack, u_V$raw needle))(usize)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    for_(($us(u_suffixS(haystack, idx)))(item)) {
        found += intFromBool(u_eql(u_deref(item), needle));
    } $end(for);
    return found;
};
fn_((mem_countBytes(S_const$u8 haystack, S_const$u8 needle))(usize)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len == 1) return mem_countUnitBytes(haystack, *S_at((needle)[0]));
    claim_assert(needle.len > 0);
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    while_some(mem_findFirstSeqBytes(S_suffix((haystack)idx), needle), found_idx) {
        idx += found_idx + needle.len;
        found++;
    };
    return found;
};
fn_((mem_count(u_S_const$raw haystack, u_S_const$raw needle))(usize)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len == 1) return mem_countUnit(haystack, u_deref(u_atS(needle, 0)));
    claim_assert(needle.len > 0);
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    while_some(mem_findFirstSeq(u_suffixS(haystack, idx), needle), found_idx) {
        idx += found_idx + needle.len;
        found++;
    };
    return found;
};
fn_((mem_containsUnitBytes(S_const$u8 haystack, u8 needle))(bool)) {
    claim_assert_nonnullS(haystack);
    for_(($s(haystack))(item)) {
        if (pri_eql(*item, needle)) return true;
    } $end(for);
    return false;
};
fn_((mem_containsUnit(u_S_const$raw haystack, u_V$raw needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    for_(($us(haystack))(item)) {
        if (u_eql(u_deref(item), needle)) return true;
    } $end(for);
    return false;
};
fn_((mem_containsBytes(S_const$u8 haystack, S_const$u8 needle))(bool)) {
    return isSome(mem_findFirstSeqBytes(haystack, needle));
};
fn_((mem_contains(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    return isSome(mem_findFirstSeq(haystack, needle));
};
fn_((mem_containsAtLeastUnitBytes(S_const$u8 haystack, u8 needle, usize minimum))(bool)) {
    claim_assert_nonnullS(haystack);
    if (minimum == 0) return true;
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    for_(($s(S_suffix((haystack)idx)))(item)) {
        found += intFromBool(pri_eql(*item, needle));
        if (found >= minimum) return true;
    } $end(for);
    return false;
};
fn_((mem_containsAtLeastUnit(u_S_const$raw haystack, u_V$raw needle, usize minimum))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (minimum == 0) return true;
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    for_(($us(u_suffixS(haystack, idx)))(item)) {
        found += intFromBool(u_eql(u_deref(item), needle));
        if (found >= minimum) return true;
    } $end(for);
    return false;
};
fn_((mem_containsAtLeastBytes(S_const$u8 haystack, S_const$u8 needle, usize minimum))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len == 1) return mem_containsAtLeastUnitBytes(haystack, *S_at((needle)[0]), minimum);
    claim_assert(needle.len > 0);
    if (minimum == 0) return true;
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    while_some(mem_findFirstSeqBytes(S_suffix((haystack)idx), needle), found_idx) {
        idx += found_idx + needle.len;
        if (++found == minimum) return true;
    };
    return false;
};
fn_((mem_containsAtLeast(u_S_const$raw haystack, u_S_const$raw needle, usize minimum))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len == 1) return mem_containsAtLeastUnit(haystack, u_deref(u_atS(needle, 0)), minimum);
    claim_assert(needle.len > 0);
    if (minimum == 0) return true;
    var_(idx, usize) = 0;
    var_(found, usize) = 0;
    while_some(mem_findFirstSeq(u_suffixS(haystack, idx), needle), found_idx) {
        idx += found_idx + needle.len;
        if (++found == minimum) return true;
    };
    return false;
};

fn_((mem_startsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eqlBytes(S_prefix((haystack)(needle.len)), needle));
    }) $unscoped(expr);
};
fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_prefixS(haystack, needle.len), needle));
    }) $unscoped(expr);
};
fn_((mem_endsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eqlBytes(S_suffix((haystack)(haystack.len - needle.len)), needle));
    }) $unscoped(expr);
};
fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_suffixS(haystack, haystack.len - needle.len), needle));
    }) $unscoped(expr);
};

fn_((mem_Cutted_beforeBytes(mem_Cutted_Bytes self))(S_const$u8)) {
    claim_assert_nonnullS(self.before);
    return self.before;
};
fn_((mem_Cutted_before(mem_Cutted self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnullS(self.before);
    debug_assert_eqBy($typed(self.type), type, TypeInfo_eql);
    return S_meta((type)(self.before));
};
fn_((mem_Cutted_afterBytes(mem_Cutted_Bytes self))(S_const$u8)) {
    claim_assert_nonnullS(self.after);
    return self.after;
};
fn_((mem_Cutted_after(mem_Cutted self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnullS(self.after);
    debug_assert_eqBy($typed(self.type), type, TypeInfo_eql);
    return S_meta((type)(self.after));
};
#endif /* in_analysis_active_only || in_comptime */

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
#define __stmt__T_use_mem_bytesAsS$(_T...) \
    $inline_always $static fn_((tpl$(mem_bytesAsS, _T)(S_const$u8 bytes))(S_const$(_T))) { \
        return u_castS$((S_const$(_T))(mem_bytesAsS(typeInfo$(_T), bytes))); \
    }
#define __stmt__T_use_mem_bytesAsMutS$(_T...) \
    $inline_always $static fn_((tpl$(mem_bytesAsMutS, _T)(S$u8 bytes))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_bytesAsMutS(typeInfo$(_T), bytes))); \
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
#define __stmt__T_use_mem_setP0$(_T...) \
    $inline_always $static fn_((tpl$(mem_setP0, _T)(P$$(_T) dst))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_set0P(u_anyP(dst)))); \
    }
#define __stmt__T_use_mem_set0$(_T...) \
    $inline_always $static fn_((tpl$(mem_set0, _T)(S$(_T) dst))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set0(u_anyS(dst)))); \
    }
#define __stmt__T_use_mem_setP$(_T...) \
    $inline_always $static fn_((tpl$(mem_setP, _T)(P$$(_T) dst, _T val))(P$$(_T))) { \
        return u_castP$((P$$(_T))(mem_setP(u_anyP(dst), u_anyV(val)))); \
    }
#define __stmt__T_use_mem_set$(_T...) \
    $inline_always $static fn_((tpl$(mem_set, _T)(S$(_T) dst, _T val))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set(u_anyS(dst), u_anyV(val)))); \
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
#define __stmt__T_use_mem_findFirstUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findFirstUnit, _T)(S_const$(_T) haystack, _T needle))(O$usize)) { \
        return mem_findFirstUnit(u_anyS(haystack), u_anyV(needle)); \
    }
#define __stmt__T_use_mem_findLastUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findLastUnit, _T)(S_const$(_T) haystack, _T needle))(O$usize)) { \
        return mem_findLastUnit(u_anyS(haystack), u_anyV(needle)); \
    }
#define __stmt__T_use_mem_findFirstSeq$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findFirstSeq, _T)(S_const$(_T) haystack, S_const$(_T) needle))(O$usize)) { \
        return mem_findFirstSeq(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_findLastSeq$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findLastSeq, _T)(S_const$(_T) haystack, S_const$(_T) needle))(O$usize)) { \
        return mem_findLastSeq(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_findFirstAny$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findFirstAny, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$usize)) { \
        return mem_findFirstAny(u_anyS(haystack), u_anyS(needles)); \
    }
#define __stmt__T_use_mem_findLastAny$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findLastAny, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$usize)) { \
        return mem_findLastAny(u_anyS(haystack), u_anyS(needles)); \
    }
#define __stmt__T_use_mem_findFirstNone$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findFirstNone, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$usize)) { \
        return mem_findFirstNone(u_anyS(haystack), u_anyS(needles)); \
    }
#define __stmt__T_use_mem_findLastNone$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findLastNone, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$usize)) { \
        return mem_findLastNone(u_anyS(haystack), u_anyS(needles)); \
    }
#define __stmt__T_use_mem_findFirstDiff$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findFirstDiff, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(O$usize)) { \
        return mem_findFirstDiff(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_findLastDiff$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_findLastDiff, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(O$usize)) { \
        return mem_findLastDiff(u_anyS(lhs), u_anyS(rhs)); \
    }
#define __stmt__T_use_mem_countUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_countUnit, _T)(S_const$(_T) haystack, _T needle))(usize)) { \
        return mem_countUnit(u_anyS(haystack), u_anyV(needle)); \
    }
#define __stmt__T_use_mem_count$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_count, _T)(S_const$(_T) haystack, S_const$(_T) needle))(usize)) { \
        return mem_count(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_containsUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_containsUnit, _T)(S_const$(_T) haystack, _T needle))(bool)) { \
        return mem_containsUnit(u_anyS(haystack), u_anyV(needle)); \
    }
#define __stmt__T_use_mem_contains$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_contains, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_contains(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_containsAtLeastUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_containsAtLeastUnit, _T)(S_const$(_T) haystack, _T needle, usize minimum))(bool)) { \
        return mem_containsAtLeastUnit(u_anyS(haystack), u_anyV(needle), minimum); \
    }
#define __stmt__T_use_mem_containsAtLeast$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_containsAtLeast, _T)(S_const$(_T) haystack, S_const$(_T) needle, usize minimum))(bool)) { \
        return mem_containsAtLeast(u_anyS(haystack), u_anyS(needle), minimum); \
    }
#define __alias__mem_WindowIter$(_T...) tpl$(mem_WindowIter, _T)
#define __stmt__T_use_mem_WindowIter$(_T...) \
    T_alias$((mem_WindowIter$(_T))(union mem_WindowIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, O$usize); \
            var_(size, usize); \
            var_(advance, usize); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, mem_WindowIter) $like_ref; \
    }))
#define __stmt__T_use_mem_window$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_window, _T)(S_const$(_T) buf, usize size, usize advance))(mem_WindowIter$(_T))$scope) { /* clang-format off */\
        return_({ .as_raw $like_deref = mem_window(u_anyS(buf), size, advance) }); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_WindowIter_reset$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_WindowIter_reset, _T)($P$(mem_WindowIter$(_T)) self))(void)) { /* clang-format off */\
        return mem_WindowIter_reset(self->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_mem_WindowIter_next$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_WindowIter_next, _T)($P$(mem_WindowIter$(_T)) self))(O$(S_const$(_T)))$scope) { /* clang-format off */\
        return_(u_castO$((ReturnType)(mem_WindowIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_startsWith$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_startsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_startsWith(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_endsWith$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_endsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_endsWith(u_anyS(haystack), u_anyS(needle)); \
    }
#define __stmt__T_use_mem_trimStart$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_trimStart, _T)(S_const$(_T) haystack, S_const$(_T) values_to_strip))(S_const$(_T))) { \
        return u_castS$((S_const$(_T))(mem_trimStart(u_anyS(haystack), u_anyS(values_to_strip)))); \
    }
#define __stmt__T_use_mem_trimEnd$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_trimEnd, _T)(S_const$(_T) haystack, S_const$(_T) values_to_strip))(S_const$(_T))) { \
        return u_castS$((S_const$(_T))(mem_trimEnd(u_anyS(haystack), u_anyS(values_to_strip)))); \
    }
#define __stmt__T_use_mem_trim$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_trim, _T)(S_const$(_T) haystack, S_const$(_T) values_to_strip))(S_const$(_T))) { \
        return u_castS$((S_const$(_T))(mem_trim(u_anyS(haystack), u_anyS(values_to_strip)))); \
    }
#define __stmt__T_use_mem_cat$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cat, _T)(S_const$(S_const$(_T)) slices, S$(_T) out))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_cat(u_anyS$((u_S_const$S_const$raw)(slices)), u_anyS(out)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_catWithin$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_catWithin, _T)(S_const$(S_const$(_T)) slices, S$(_T) out))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_catWithin(u_anyS$((u_S_const$S_const$raw)(slices)), u_anyS(out)))); \
    }
#define __stmt__T_use_mem_catAlloc$(_T...) \
    $attr($inline_always $must_check $static) \
    fn_((tpl$(mem_catAlloc, _T)(S_const$(S_const$(_T)) slices, mem_Alctr gpa))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_catAlloc(typeInfo$(_T), u_anyS$((u_S_const$S_const$raw)(slices)), gpa))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_join$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_join, _T)( \
        S_const$(_T) sep, S_const$(S_const$(_T)) slices, S$(_T) out \
    ))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_join(u_anyS(sep), u_anyS$((u_S_const$S_const$raw)(slices)), u_anyS(out)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_joinWithin$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_joinWithin, _T)( \
        S_const$(_T) sep, S_const$(S_const$(_T)) slices, S$(_T) out \
    ))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_joinWithin(u_anyS(sep), u_anyS$((u_S_const$S_const$raw)(slices)), u_anyS(out)))); \
    }
#define __stmt__T_use_mem_joinAlloc$(_T...) \
    $attr($inline_always $must_check $static) \
    fn_((tpl$(mem_joinAlloc, _T)( \
        S_const$(_T) sep, S_const$(S_const$(_T)) slices, mem_Alctr gpa \
    ))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_joinAlloc(u_anyS(sep), u_anyS$((u_S_const$S_const$raw)(slices)), gpa))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padLeft$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padLeft, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padLeft(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padLeftWithin$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padLeftWithin, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_padLeftWithin(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    }
#define __stmt__T_use_mem_padLeftAlloc$(_T...) \
    $attr($inline_always $must_check $static) \
    fn_((tpl$(mem_padLeftAlloc, _T)(S_const$(_T) src, usize width, _T fill, mem_Alctr gpa))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padLeftAlloc(u_anyS(src), width, u_anyV(fill), gpa))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padRight$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padRight, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padRight(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padRightWithin$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padRightWithin, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_padRightWithin(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    }
#define __stmt__T_use_mem_padRightAlloc$(_T...) \
    $attr($inline_always $must_check $static) \
    fn_((tpl$(mem_padRightAlloc, _T)(S_const$(_T) src, usize width, _T fill, mem_Alctr gpa))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padRightAlloc(u_anyS(src), width, u_anyV(fill), gpa))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padCenter$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padCenter, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padCenter(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_padCenterWithin$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_padCenterWithin, _T)(S_const$(_T) src, usize width, _T fill, S$(_T) out))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_padCenterWithin(u_anyS(src), width, u_anyV(fill), u_anyS(out)))); \
    }
#define __stmt__T_use_mem_padCenterAlloc$(_T...) \
    $attr($inline_always $must_check $static) \
    fn_((tpl$(mem_padCenterAlloc, _T)(S_const$(_T) src, usize width, _T fill, mem_Alctr gpa))(E$($set(mem_E)(S$(_T))))$scope) { /* clang-format off */ \
        return u_castE$((ReturnType)(mem_padCenterAlloc(u_anyS(src), width, u_anyV(fill), gpa))); \
    } $unscoped(fn) /* clang-format on */
#define __alias__mem_Cutted$(_T...) tpl$(mem_Cutted, _T)
#define __stmt__T_use_mem_Cutted$(_T...) \
    T_alias$((mem_Cutted$(_T))(union mem_Cutted$(_T) { \
        T_embed$(struct { \
            var_(before, S_const$(_T)); \
            var_(after, S_const$(_T)); \
            var_(type, debug_TypeInfo); \
        }); \
        var_(as_raw, mem_Cutted) $like_ref; \
    })); \
    T_use_O$(mem_Cutted$(_T))
#define __stmt__T_use_mem_Cutted_before$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_Cutted_before, _T)(mem_Cutted$(_T) self))(S_const$(_T))$scope) { /* clang-format off */ \
        return u_castS$((S_const$(_T))(mem_Cutted_before(*self.as_raw, typeInfo$(_T)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_Cutted_after$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_Cutted_after, _T)(mem_Cutted$(_T) self))(S_const$(_T))$scope) { /* clang-format off */ \
        return u_castS$((S_const$(_T))(mem_Cutted_after(*self.as_raw, typeInfo$(_T)))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutFirstUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutFirstUnit, _T)(S_const$(_T) haystack, _T needle))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutFirstUnit(u_anyS(haystack), u_anyV(needle)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutLastUnit$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutLastUnit, _T)(S_const$(_T) haystack, _T needle))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutLastUnit(u_anyS(haystack), u_anyV(needle)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutFirstSeq$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutFirstSeq, _T)(S_const$(_T) haystack, S_const$(_T) needle))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutFirstSeq(u_anyS(haystack), u_anyS(needle)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutLastSeq$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutLastSeq, _T)(S_const$(_T) haystack, S_const$(_T) needle))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutLastSeq(u_anyS(haystack), u_anyS(needle)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutFirstAny$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutFirstAny, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutFirstAny(u_anyS(haystack), u_anyS(needles)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_cutLastAny$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_cutLastAny, _T)(S_const$(_T) haystack, S_const$(_T) needles))(O$(mem_Cutted$(_T)))$scope) { /* clang-format off */ \
        if_some((mem_cutLastAny(u_anyS(haystack), u_anyS(needles)))(cut)) { return_some({ .as_raw $like_deref = cut }); } \
        return_none(); \
    } $unscoped(fn) /* clang-format on */
#define __alias__mem_Delim$(_T...) tpl$(mem_Delim, _T)
#define __stmt__T_use_mem_Delim$(_T...) \
    T_alias$((mem_Delim$(_T))(union mem_Delim$(_T) { \
        variant_(($maps(mem_Delim))( \
            (mem_Delim_unit, _T), \
            (mem_Delim_seq, S_const$(_T)), \
            (mem_Delim_any, S_const$(_T)), \
        )); \
        var_(as_raw, mem_Delim$raw) $flexible; \
    }))
#define __alias__mem_TokzIter$(_T...) tpl$(mem_TokzIter, _T)
#define __stmt__T_use_mem_TokzIter$(_T...) \
    T_alias$((mem_TokzIter$(_T))(union mem_TokzIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, usize); \
            var_(type, debug_TypeInfo); \
            var_(delim, mem_Delim$(_T)); \
            var_(delim_, mem_Delim$(_T)) $like_ref; \
        }); \
        var_(as_raw, mem_TokzIter$raw) $flexible; \
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
#define __alias__mem_TokzBwdIter$(_T...) tpl$(mem_TokzBwdIter, _T)
#define __stmt__T_use_mem_TokzBwdIter$(_T...) \
    T_alias$((mem_TokzBwdIter$(_T))(union mem_TokzBwdIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, usize); \
            var_(type, debug_TypeInfo); \
            var_(delim, mem_Delim$(_T)); \
            var_(delim_, mem_Delim$(_T)) $like_ref; \
        }); \
        var_(as_raw, mem_TokzBwdIter$raw) $flexible; \
    }))
#define __stmt__T_use_mem_tokzBwdUnit$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzBwdUnit, _T)(S_const$(_T) buf, _T unit))(mem_TokzBwdIter$(_T))) { \
        return *as$(mem_TokzBwdIter$(_T)*)(mem_tokzBwdUnit(u_anyS(buf), u_anyV(unit), l0$((mem_TokzBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_tokzBwdSeq$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzBwdSeq, _T)(S_const$(_T) buf, S_const$(_T) seq))(mem_TokzBwdIter$(_T))) { \
        return *as$(mem_TokzBwdIter$(_T)*)(mem_tokzBwdSeq(u_anyS(buf), u_anyS(seq), l0$((mem_TokzBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_tokzBwdAny$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_tokzBwdAny, _T)(S_const$(_T) buf, S_const$(_T) any))(mem_TokzBwdIter$(_T))) { \
        return *as$(mem_TokzBwdIter$(_T)*)(mem_tokzBwdAny(u_anyS(buf), u_anyS(any), l0$((mem_TokzBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_TokzBwdIter_reset$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzBwdIter_reset, _T)($P$(mem_TokzBwdIter$(_T)) self))(void)) { \
        return mem_TokzBwdIter_reset(self->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_mem_TokzBwdIter_next$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzBwdIter_next, _T)($P$(mem_TokzBwdIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokzBwdIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_TokzBwdIter_peek$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzBwdIter_peek, _T)($P$(mem_TokzBwdIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokzBwdIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_TokzBwdIter_rest$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_TokzBwdIter_rest, _T)($P$(mem_TokzBwdIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_TokzBwdIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __alias__mem_SplitIter$(_T...) tpl$(mem_SplitIter, _T)
#define __stmt__T_use_mem_SplitIter$(_T...) \
    T_alias$((mem_SplitIter$(_T))(union mem_SplitIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, O$usize); \
            var_(type, debug_TypeInfo); \
            var_(delim, mem_Delim$(_T)); \
            var_(delim_, mem_Delim$(_T)) $like_ref; \
        }); \
        var_(as_raw, mem_SplitIter$raw) $flexible; \
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
#define __alias__mem_SplitBwdIter$(_T...) tpl$(mem_SplitBwdIter, _T)
#define __stmt__T_use_mem_SplitBwdIter$(_T...) \
    T_alias$((mem_SplitBwdIter$(_T))(union mem_SplitBwdIter$(_T) { \
        T_embed$(struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, O$usize); \
            var_(type, debug_TypeInfo); \
            var_(delim, mem_Delim$(_T)); \
            var_(delim_, mem_Delim$(_T)) $like_ref; \
        }); \
        var_(as_raw, mem_SplitBwdIter$raw) $flexible; \
    }))
#define __stmt__T_use_mem_splitBwdUnit$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitBwdUnit, _T)(S_const$(_T) buf, _T unit))(mem_SplitBwdIter$(_T))) { \
        return *as$(mem_SplitBwdIter$(_T)*)(mem_splitBwdUnit(u_anyS(buf), u_anyV(unit), l0$((mem_SplitBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_splitBwdSeq$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitBwdSeq, _T)(S_const$(_T) buf, S_const$(_T) seq))(mem_SplitBwdIter$(_T))) { \
        return *as$(mem_SplitBwdIter$(_T)*)(mem_splitBwdSeq(u_anyS(buf), u_anyS(seq), l0$((mem_SplitBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_splitBwdAny$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_splitBwdAny, _T)(S_const$(_T) buf, S_const$(_T) any))(mem_SplitBwdIter$(_T))) { \
        return *as$(mem_SplitBwdIter$(_T)*)(mem_splitBwdAny(u_anyS(buf), u_anyS(any), l0$((mem_SplitBwdIter$(_T))).as_raw)); \
    } /* clang-format on */
#define __stmt__T_use_mem_SplitBwdIter_reset$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitBwdIter_reset, _T)($P$(mem_SplitBwdIter$(_T)) self))(void)) { \
        return mem_SplitBwdIter_reset(self->as_raw); \
    } /* clang-format on */
#define __stmt__T_use_mem_SplitBwdIter_first$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitBwdIter_first, _T)($P$(mem_SplitBwdIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_SplitBwdIter_first(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitBwdIter_next$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitBwdIter_next, _T)($P$(mem_SplitBwdIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_SplitBwdIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitBwdIter_peek$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitBwdIter_peek, _T)($P$(mem_SplitBwdIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_SplitBwdIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */
#define __stmt__T_use_mem_SplitBwdIter_rest$(_T...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(mem_SplitBwdIter_rest, _T)($P$(mem_SplitBwdIter$(_T)) self))(S_const$(_T))$scope) { \
        return_(u_castS$((ReturnType)(mem_SplitBwdIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped(fn) /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_common__included */
