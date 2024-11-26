// /*========== Includes =======================================================*/

// #include "dh/core/Ptr.h"

// #include <stdalign.h>
// #include <stddef.h>

// /*========== Internal Helpers ==========================================*/

// static_inline u32 Ptr_calcAlignOrder(usize align) {
//     return (u32)(__builtin_ctz(align));
// }

// static_inline usize Ptr_decodeAlign(u32 align_order) {
//     return 1ULL << align_order;
// }

// /*========== Core Implementation ======================================*/

// Ptr Ptr_fromRaw(anyptr raw, usize elem_size, bool flags) {
//     if (raw == null || elem_size == 0) {
//         return Ptr_null;
//     }

//     // Calculate alignment order (log2 of alignment)
//     usize align       = alignof(max_align_t);
//     u32   align_order = Ptr_calcAlignOrder(align);

//     // Ensure Clamping size fits in available bits
//     usize size = Ptr_size_mask >= elem_size ? elem_size : Ptr_size_mask;

// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     Ptr p        = { 0 };
//     p.bits.ptr   = (usize)raw;
//     p.bits.size  = size;
//     p.bits.align = align_order;
//     p.bits.flags = flags;
//     return p;
// #else // PTR_IMPL_DOUBLE
//     return (Ptr){
//         .raw  = raw,
//         .meta = {
//             .size     = size,
//             .align    = align_order,
//             .flags    = flags,
//             .reserved = 0 }
//     };
// #endif
// #else // 32-bit
//     return (Ptr){
//         .raw  = raw,
//         .meta = {
//             .size     = size,
//             .align    = align_order,
//             .flags    = flags,
//             .reserved = 0 }
//     };
// #endif
// }

// anyptr Ptr_raw(Ptr p) {
// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     return (anyptr)(usize)p.bits.ptr; // NOLINT
// #else
//     return p.raw;
// #endif
// #else
//     return p.raw;
// #endif
// }

// usize Ptr_size(Ptr p) {
// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     return p.bits.size;
// #else
//     return p.meta.size;
// #endif
// #else
//     return p.meta.size;
// #endif
// }

// usize Ptr_align(Ptr p) {
// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     return Ptr_decodeAlign(p.bits.align);
// #else
//     return decode_align(p.meta.align);
// #endif
// #else
//     return decode_align(p.meta.align);
// #endif
// }

// bool Ptr_flags(Ptr p) {
// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     return p.bits.flags;
// #else
//     return p.meta.flags;
// #endif
// #else
//     return p.meta.flags;
// #endif
// }

// bool Ptr_isNull(Ptr p) {
// #if defined(BUILTIN_PLTF_64BIT)
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
//     return p.bits.ptr == 0;
// #else
//     return p.raw == null;
// #endif
// #else
//     return p.raw == null;
// #endif
// }

// bool Ptr_hasMinSize(Ptr p, usize required_size) {
//     return !Ptr_isNull(p) && Ptr_size(p) >= required_size;
// }

// /*========== Debug Implementation ====================================*/

// // #ifdef CORE_PTR_DEBUG_CHECKS
// // bool Ptr_isValid(Ptr p) {
// //     if (Ptr_isNull(p)) {
// //         return true; // Null is valid
// //     }

// //     // Check pointer
// //     if (Ptr_raw(p) == null) {
// //         return false;
// //     }

// //     // Check size
// //     if (Ptr_size(p) == 0 || Ptr_size(p) > Ptr_size_mask) {
// //         return false;
// //     }

// //     // Check alignment
// //     usize align = Ptr_align(p);
// //     usize addr  = (usize)Ptr_raw(p);
// //     if ((addr & (align - 1)) != 0) {
// //         return false;
// //     }

// //     return true;
// // }
// // #endif

// /*========== Platform Specific Extensions ===========================*/

// #ifdef BUILTIN_PLTF_64BIT
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
// // Functions specific to 64-bit bitfield implementation
// usize Ptr_getBits(Ptr p) {
//     return (usize)p.raw;
// }

// // bool Ptr_isCanonical(Ptr p) {
// //     // Check if address is in canonical form
// //     usize addr      = p.bits.ptr;
// //     usize high_bits = addr >> 47;
// //     return high_bits == 0 || high_bits == 0x1FFFF;
// // }
// #endif
// #endif

// /*========== Validation Checks ======================================*/

// #if DEBUG_ENABLED
// claim_assert_static_msg(Ptr_size_bits + Ptr_align_bits + Ptr_flags_bits <= 16, "Metadata bits exceed available space");

// #ifdef BUILTIN_PLTF_64BIT
// #if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
// claim_assert_static_msg(sizeof(Ptr) == sizeof(anyptr), "Ptr must be pointer-sized");
// #else
// claim_assert_static_msg(sizeof(Ptr) == 2 * sizeof(anyptr), "Ptr must be double pointer-sized");
// #endif
// #else
// claim_assert_static_msg(sizeof(Ptr) == 2 * sizeof(anyptr), "Ptr must be double pointer-sized");
// #endif
// #endif
