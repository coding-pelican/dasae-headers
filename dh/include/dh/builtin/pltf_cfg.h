/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    pltf_cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-22 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  BUILTIN_PLTF
 *
 * @brief   Platform detection configuration
 * @details Detects operating system, environment specifics, and platform features.
 *          Includes OS family, bit width, endianness, and system-specific attributes.
 */

#ifndef BUILTIN_PLTF_CFG_INCLUDED
#define BUILTIN_PLTF_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Operating System Detection =====================================*/
#if defined(_WIN32) || defined(_WIN64)
#define BUILTIN_PLTF_WINDOWS (1)
#define BUILTIN_PLTF_UNIX    (0)
#define BUILTIN_PLTF_DARWIN  (0)
#define BUILTIN_PLTF_LINUX   (0)
#define BUILTIN_PLTF_BSD     (0)
#define BUILTIN_PLTF_NAME    "Windows"
#elif defined(__APPLE__)
#define BUILTIN_PLTF_WINDOWS (0)
#define BUILTIN_PLTF_UNIX    (1)
#define BUILTIN_PLTF_DARWIN  (1)
#define BUILTIN_PLTF_LINUX   (0)
#define BUILTIN_PLTF_BSD     (0)
#define BUILTIN_PLTF_NAME    "Darwin"
#elif defined(__linux__)
#define BUILTIN_PLTF_WINDOWS (0)
#define BUILTIN_PLTF_UNIX    (1)
#define BUILTIN_PLTF_DARWIN  (0)
#define BUILTIN_PLTF_LINUX   (1)
#define BUILTIN_PLTF_BSD     (0)
#define BUILTIN_PLTF_NAME    "Linux"
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#define BUILTIN_PLTF_WINDOWS (0)
#define BUILTIN_PLTF_UNIX    (1)
#define BUILTIN_PLTF_DARWIN  (0)
#define BUILTIN_PLTF_LINUX   (0)
#define BUILTIN_PLTF_BSD     (1)
#define BUILTIN_PLTF_NAME    "BSD"
#else
#define BUILTIN_PLTF_WINDOWS (0)
#define BUILTIN_PLTF_UNIX    (0)
#define BUILTIN_PLTF_DARWIN  (0)
#define BUILTIN_PLTF_LINUX   (0)
#define BUILTIN_PLTF_BSD     (0)
#define BUILTIN_PLTF_NAME    "Unknown"
#warning "Unknown platform detected. Some features may not work as expected."
#endif

/*========== Platform Attributes ============================================*/
/* Bit Width Detection */
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
#define BUILTIN_PLTF_64BIT (1)
#define BUILTIN_PLTF_32BIT (0)
#define BUILTIN_PLTF_BITS  (64)
#elif defined(_WIN32) || defined(__i386__) || defined(__ppc__) || defined(__arm__)
#define BUILTIN_PLTF_64BIT (0)
#define BUILTIN_PLTF_32BIT (1)
#define BUILTIN_PLTF_BITS  (32)
#else
#define BUILTIN_PLTF_64BIT (0)
#define BUILTIN_PLTF_32BIT (0)
#define BUILTIN_PLTF_BITS  (16) /* Default assumption */
#warning "Unknown platform bit width"
#endif

/* Endianness Detection */
#if defined(__BYTE_ORDER__)
#define BUILTIN_PLTF_BYTE_ORDER               (__BYTE_ORDER__)
#define BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN (__ORDER_LITTLE_ENDIAN__)
#define BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN    (__ORDER_BIG_ENDIAN__)
#endif
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BUILTIN_PLTF_LITTLE_ENDIAN (1)
#define BUILTIN_PLTF_BIG_ENDIAN    (0)
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BUILTIN_PLTF_LITTLE_ENDIAN (0)
#define BUILTIN_PLTF_BIG_ENDIAN    (1)
#else
#define BUILTIN_PLTF_LITTLE_ENDIAN (1) /* Default assumption */
#define BUILTIN_PLTF_BIG_ENDIAN    (0)
#warning "Unknown platform endianness"
#endif

/* Dynamic/Static Library Configuration */
#if BUILTIN_PLTF_WINDOWS
#define BUILTIN_PLTF_EXPORT __declspec(dllexport)
#define BUILTIN_PLTF_IMPORT __declspec(dllimport)
#else
#define BUILTIN_PLTF_EXPORT __attribute__((visibility("default")))
#define BUILTIN_PLTF_IMPORT
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_PLTF_CFG_INCLUDED */
