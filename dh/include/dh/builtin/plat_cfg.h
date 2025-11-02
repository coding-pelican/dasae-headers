/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    plat_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  plat
 *
 * @brief   Platform detection configuration
 * @details Detects operating system, environment specifics, and platform features.
 *          Includes OS family, bit width, endianness, and system-specific attributes.
 */

#ifndef BUILTIN_PLAT_CFG_INCLUDED
#define BUILTIN_PLAT_CFG_INCLUDED (1)

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Platform Detection =============================================*/

#define plat_windows VAL__plat_windows
#define plat_posix   VAL__plat_posix
#define plat_unix    VAL__plat_unix
#define plat_linux   VAL__plat_linux
#define plat_darwin  VAL__plat_darwin
#define plat_wasm    VAL__plat_wasm
#define plat_wasi    VAL__plat_wasi

#define plat_name VAL__plat_name

#define plat_64bit VAL__plat_64bit
#define plat_32bit VAL__plat_32bit
#define plat_bits  VAL__plat_bits

#define plat_byte_order               VAL__plat_byte_order
#define plat_endian                   VAL__plat_endian
#define plat_byte_order_little_endian VAL__plat_byte_order_little_endian
#define plat_byte_order_big_endian    VAL__plat_byte_order_big_endian

#define plat_export ATTR__plat_export
#define plat_import ATTR__plat_import


/*========== Implementations Platform Detection =============================*/

#undef VAL__plat_windows
#undef VAL__plat_unix
#undef VAL__plat_darwin
#undef VAL__plat_linux
#undef VAL__plat_bsd
#undef VAL__plat_wasm
#undef VAL__plat_wasi

#define VAL__plat_windows 0
#define VAL__plat_unix    0
#define VAL__plat_darwin  0
#define VAL__plat_linux   0
#define VAL__plat_bsd     0
#define VAL__plat_wasm    0 /* Emscripten or other WASM environment */
#define VAL__plat_wasi    0 /* WASI environment */
#define VAL__plat_name    "Unknown"

#if defined(__EMSCRIPTEN__)
/* Emscripten: usually compiles to WebAssembly, often in a browser or Node. */
#undef VAL__plat_wasm
#define VAL__plat_wasm 1
#undef VAL__plat_name
#define VAL__plat_name "Emscripten (WASM)"

#elif defined(__wasi__)
/* WASI: WebAssembly System Interface (standalone environment) */
#undef VAL__plat_wasi
#define VAL__plat_wasi 1
#undef VAL__plat_name
#define VAL__plat_name "WASI"

#else
/* If not WASM or WASI, check traditional OSes. */

#if defined(_WIN32) || defined(_WIN64)
#undef VAL__plat_windows
#define VAL__plat_windows 1
#undef VAL__plat_name
#define VAL__plat_name "Windows"

#elif defined(__APPLE__)
#undef VAL__plat_unix
#undef VAL__plat_darwin
#define VAL__plat_unix   1
#define VAL__plat_darwin 1
#undef VAL__plat_name
#define VAL__plat_name "Darwin"

#elif defined(__linux__)
#undef VAL__plat_unix
#undef VAL__plat_linux
#define VAL__plat_unix  1
#define VAL__plat_linux 1
#undef VAL__plat_name
#define VAL__plat_name "Linux"

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#undef VAL__plat_unix
#undef VAL__plat_bsd
#define VAL__plat_unix 1
#define VAL__plat_bsd  1
#undef VAL__plat_name
#define VAL__plat_name "BSD"

#else
/* Unknown OS environment */
#warning "Unknown platform detected. Some features may not work as expected."
#endif
#endif /* WASM/WASI detection vs. Traditional OSes */

#undef VAL__plat_64bit
#undef VAL__plat_32bit
#undef VAL__plat_bits

#define VAL__plat_64bit 0
#define VAL__plat_32bit 0
#define VAL__plat_bits  0

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) \
    || defined(__ppc64__) || defined(__powerpc64__) || defined(__EMSCRIPTEN__) && (defined(__wasm64__))
/*
 * For WASM64, you might check something like:
 *   #if (defined(__wasm64__) || (__EMSCRIPTEN_major__ >= ??? && ...))
 * Adjust for your toolchain.
 */
#undef VAL__plat_64bit
#define VAL__plat_64bit 1
#undef VAL__plat_bits
#define VAL__plat_bits 64

#elif defined(_WIN32) || defined(__i386__) || defined(__arm__) || defined(__ppc__) \
    || defined(__EMSCRIPTEN__) /* Typically wasm32 if no __wasm64__ */
#undef VAL__plat_32bit
#define VAL__plat_32bit 1
#undef VAL__plat_bits
#define VAL__plat_bits 32

#else
/*
 * Might be 16-bit or some unusual environment.
 */
#warning "Unknown platform bit width - defaulting to 16"
#undef VAL__plat_bits
#define VAL__plat_bits 16
#endif


#undef VAL__plat_byte_order
#undef VAL__plat_endian
#undef VAL__plat_byte_order_little_endian
#undef VAL__plat_byte_order_big_endian

#define VAL__plat_byte_order               __BYTE_ORDER__
#define VAL__plat_endian                   0
#define VAL__plat_byte_order_little_endian __ORDER_LITTLE_ENDIAN__
#define VAL__plat_byte_order_big_endian    __ORDER_BIG_ENDIAN__

#if (plat_wasm || plat_wasi)
/* WebAssembly is always little-endian per the spec. */
#undef VAL__plat_endian
#define VAL__plat_endian plat_byte_order_little_endian

#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#undef VAL__plat_endian
#define VAL__plat_endian plat_byte_order_little_endian
#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#undef VAL__plat_endian
#define VAL__plat_endian plat_byte_order_big_endian
#else
#warning "Unknown __BYTE_ORDER__ - defaulting to little-endian"
#undef VAL__plat_endian
#define VAL__plat_endian plat_byte_order_little_endian
#endif

#else
/*
 * Fallback if __BYTE_ORDER__ not defined. Common on older compilers or exotic platforms.
 */
#warning "No known byte-order macros - defaulting to little-endian"
#undef VAL__plat_endian
#define VAL__plat_endian plat_byte_order_little_endian
#endif

#undef ATTR__plat_export
#undef ATTR__plat_import

#if plat_windows
#define ATTR__plat_export __declspec(dllexport)
#define ATTR__plat_import __declspec(dllimport)
#else
#define ATTR__plat_export __attribute__((visibility("default")))
#define ATTR__plat_import
#endif

#undef VAL__plat_posix
#if plat_unix
#define VAL__plat_posix 1
#else
#define VAL__plat_posix 0
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_PLAT_CFG_INCLUDED */
