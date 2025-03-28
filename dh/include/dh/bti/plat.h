/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    plat.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_plat
 *
 * @brief   Platform detection and configuration
 * @details Detects operating system, environment specifics, and platform features.
 *          Includes OS family, bit width, endianness, and system-specific attributes.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_PLAT_INCLUDED
#define BTI_PLAT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "arch.h"

/*========== Macros and Declarations ========================================*/

/* Enumerable Platform Properties */

#define bti_plat_os_type                                    \
    /**                                                     \
     * @brief Current operating system type                 \
     * @return Enumerated value representing the current OS \
     * @see bti_plat_os_type_* constants                    \
     */                                                     \
    comp_value__bti_plat_os_type
/* OS Type Constants */
#define bti_plat_os_type_unknown (0)
#define bti_plat_os_type_windows (1)
#define bti_plat_os_type_macos   (2)
#define bti_plat_os_type_linux   (3)
#define bti_plat_os_type_bsd     (4)
#define bti_plat_os_type_ios     (5)
#define bti_plat_os_type_android (6)
#define bti_plat_os_type_wasm    (7)
#define bti_plat_os_type_count   (8)

#define bti_plat_device_type                                 \
    /**                                                      \
     * @brief Current device type (desktop, mobile, etc.)    \
     * @return Enumerated value representing the device type \
     * @see bti_plat_device_type_* constants                 \
     */                                                      \
    comp_value__bti_plat_device_type
/* Device Type Constants */
#define bti_plat_device_type_unknown  (0)
#define bti_plat_device_type_desktop  (1)
#define bti_plat_device_type_mobile   (2)
#define bti_plat_device_type_embedded (3)
#define bti_plat_device_type_web      (4)
#define bti_plat_device_type_count    (5)

/* Platform-specific Constants */

#define bti_plat_dir_separator                                   \
    /**                                                          \
     * @brief Directory separator character for current platform \
     * @return '\\' on Windows, '/' on other platforms           \
     */                                                          \
    comp_value__bti_plat_dir_separator
#define bti_plat_path_separator                                  \
    /**                                                          \
     * @brief Path list separator character for current platform \
     * @return ';' on Windows, ':' on other platforms            \
     */                                                          \
    comp_value__bti_plat_path_separator
#define bti_plat_line_ending                                 \
    /**                                                      \
     * @brief Native line ending string for current platform \
     * @return "\\r\\n" on Windows, "\\n" on other platforms \
     */                                                      \
    comp_value__bti_plat_line_ending

/* Platform OS Detection */

#define bti_plat_detects_windows                    \
    /**                                             \
     * @brief Microsoft Windows platform detection  \
     * @return 1 if running on Windows, 0 otherwise \
     */                                             \
    comp_bool__bti_plat_detects_windows
#define bti_plat_detects_posix                             \
    /**                                                    \
     * @brief POSIX-compliant platform detection           \
     * @return 1 if running on a POSIX system, 0 otherwise \
     */                                                    \
    comp_bool__bti_plat_detects_posix
#define bti_plat_detects_unix                    \
    /**                                          \
     * @brief Unix platform detection            \
     * @return 1 if running on Unix, 0 otherwise \
     */                                          \
    comp_bool__bti_plat_detects_unix
#define bti_plat_detects_linux                    \
    /**                                           \
     * @brief Linux platform detection            \
     * @return 1 if running on Linux, 0 otherwise \
     */                                           \
    comp_bool__bti_plat_detects_linux
#define bti_plat_detects_darwin                            \
    /**                                                    \
     * @brief Darwin-based platform detection (macOS, iOS) \
     * @return 1 if running on Darwin kernel, 0 otherwise  \
     */                                                    \
    comp_bool__bti_plat_detects_darwin
#define bti_plat_detects_bsd                    \
    /**                                         \
     * @brief BSD-derived platform detection    \
     * @return 1 if running on BSD, 0 otherwise \
     */                                         \
    comp_bool__bti_plat_detects_bsd
#define bti_plat_detects_wasm                             \
    /**                                                   \
     * @brief WebAssembly platform detection              \
     * @return 1 if compiled for WebAssembly, 0 otherwise \
     */                                                   \
    comp_bool__bti_plat_detects_wasm
#define bti_plat_detects_wasi                              \
    /**                                                    \
     * @brief WebAssembly System Interface detection       \
     * @return 1 if running with WASI support, 0 otherwise \
     */                                                    \
    comp_bool__bti_plat_detects_wasi

/* Platform Category */

#define bti_plat_detects_desktop                               \
    /**                                                        \
     * @brief Desktop platform detection                       \
     * @return 1 if running on a desktop platform, 0 otherwise \
     */                                                        \
    comp_bool__bti_plat_detects_desktop
#define bti_plat_detects_mobile                               \
    /**                                                       \
     * @brief Mobile platform detection                       \
     * @return 1 if running on a mobile platform, 0 otherwise \
     */                                                       \
    comp_bool__bti_plat_detects_mobile
#define bti_plat_detects_embedded                                \
    /**                                                          \
     * @brief Embedded platform detection                        \
     * @return 1 if running on an embedded platform, 0 otherwise \
     */                                                          \
    comp_bool__bti_plat_detects_embedded

/*========== Macros and Definitions =========================================*/

/* Platform OS Detection - Implementations */

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define comp_bool__bti_plat_detects_windows (1)
#else
#define comp_bool__bti_plat_detects_windows (0)
#endif

#if defined(_POSIX_VERSION) || defined(__unix__) || defined(__APPLE__)
#define comp_bool__bti_plat_detects_posix (1)
#else
#define comp_bool__bti_plat_detects_posix (0)
#endif

#if defined(__unix__) || defined(__unix)
#define comp_bool__bti_plat_detects_unix (1)
#else
#define comp_bool__bti_plat_detects_unix (0)
#endif

#if defined(__linux__) || defined(linux) || defined(__linux)
#define comp_bool__bti_plat_detects_linux (1)
#else
#define comp_bool__bti_plat_detects_linux (0)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#define comp_bool__bti_plat_detects_darwin (1)
#else
#define comp_bool__bti_plat_detects_darwin (0)
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__bsdi__)
#define comp_bool__bti_plat_detects_bsd (1)
#else
#define comp_bool__bti_plat_detects_bsd (0)
#endif

#if defined(__EMSCRIPTEN__) || defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
#define comp_bool__bti_plat_detects_wasm (1)
#else
#define comp_bool__bti_plat_detects_wasm (0)
#endif

#if defined(__wasi__) || (bti_plat_wasm && defined(_WASI_EMULATED_SIGNAL))
#define comp_bool__bti_plat_detects_wasi (1)
#else
#define comp_bool__bti_plat_detects_wasi (0)
#endif

/* Platform-specific Constants - Implementations */

#if bti_plat_detects_windows
#define comp_value__bti_plat_dir_separator '\\'
#else
#define comp_value__bti_plat_dir_separator '/'
#endif

#if bti_plat_detects_windows
#define comp_value__bti_plat_path_separator ';'
#else
#define comp_value__bti_plat_path_separator ':'
#endif

#if bti_plat_detects_windows
#define comp_value__bti_plat_line_ending "\r\n"
#else
#define comp_value__bti_plat_line_ending "\n"
#endif

/* Platform Category - Implementations */

#define comp_bool__bti_plat_detects_desktop ((bti_plat_detects_windows || bti_plat_detects_darwin || bti_plat_detects_linux || bti_plat_detects_bsd) && !bti_plat_detects_mobile)

#if defined(__ANDROID__)
#define comp_bool__bti_plat_detects_mobile (1)
#elif bti_plat_detects_darwin && (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__))
#define comp_bool__bti_plat_detects_mobile (1)
#else
#define comp_bool__bti_plat_detects_mobile (0)
#endif

#if defined(__QNXNTO__) || defined(__rtems__) || defined(__VXWORKS__) || defined(__vxworks) || defined(__SYMBIAN32__) || defined(__INTEGRITY) || defined(__NUCLEUS__)
#define comp_bool__bti_plat_detects_embedded (1)
#else
#define comp_bool__bti_plat_detects_embedded (0)
#endif

/* Enumerable Platform Properties - Implementations */

#if bti_plat_detects_windows
#define comp_value__bti_plat_os_type bti_plat_os_type_windows
#elif bti_plat_detects_darwin
#if defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#define comp_value__bti_plat_os_type bti_plat_os_type_ios
#else
#define comp_value__bti_plat_os_type bti_plat_os_type_macos
#endif
#elif bti_plat_detects_android
#define comp_value__bti_plat_os_type bti_plat_os_type_android
#elif bti_plat_detects_linux
#define comp_value__bti_plat_os_type bti_plat_os_type_linux
#elif bti_plat_detects_bsd
#define comp_value__bti_plat_os_type bti_plat_os_type_bsd
#elif bti_plat_detects_wasm
#define comp_value__bti_plat_os_type bti_plat_os_type_wasm
#else
#define comp_value__bti_plat_os_type bti_plat_os_type_unknown
#endif

#if bti_plat_detects_desktop
#define comp_value__bti_plat_device_type bti_plat_device_type_desktop
#elif bti_plat_detects_mobile
#define comp_value__bti_plat_device_type bti_plat_device_type_mobile
#elif bti_plat_detects_embedded
#define comp_value__bti_plat_device_type bti_plat_device_type_embedded
#elif bti_plat_detects_wasm
#define comp_value__bti_plat_device_type bti_plat_device_type_web
#else
#define comp_value__bti_plat_device_type bti_plat_device_type_unknown
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_PLAT_INCLUDED */
