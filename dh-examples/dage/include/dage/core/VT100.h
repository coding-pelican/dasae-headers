/**
 * @file    VT100.h
 * @brief   VT100 terminal backend for dage
 * @details Renders to terminal using ANSI escape codes.
 *          Converts RGBA pixels to ASCII art with colors.
 *
 * ## Features
 * - ASCII art rendering (brightness → character mapping)
 * - 256-color or truecolor support (if terminal supports)
 * - Keyboard input via stdin (raw mode)
 * - Mouse input via terminal escape sequences (optional)
 *
 * ## Usage
 * ```c
 * var vt100 = try_(dage_core_VT100_init((dage_core_VT100_Cfg){
 *     .gpa = gpa,
 *     .color_mode = dage_core_VT100_ColorMode_truecolor,
 * }));
 * defer_(dage_core_VT100_fini(&vt100));
 * let backend = dage_core_VT100_backend(vt100);
 * var runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
 *     .gpa = gpa,
 *     .backend = backend,
 * }));
 * defer_(dage_Runtime_fini(&runtime));
 * // ... use vt100, backend, runtime ...
 * ```
 */
#ifndef dage_core_VT100__included
#define dage_core_VT100__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/Backend.h"

/*========== Macros and Definitions =========================================*/

/*========== Color Modes ==========*/

typedef enum_(dage_core_VT100_ColorMode $bits(8)) {
    dage_core_VT100_ColorMode_mono, /* No color, only brightness */
    dage_core_VT100_ColorMode_16, /* 16 ANSI colors */
    dage_core_VT100_ColorMode_256, /* 256 extended colors */
    dage_core_VT100_ColorMode_truecolor, /* 24-bit RGB */
} dage_core_VT100_ColorMode;

/*========== Configuration ==========*/

typedef struct dage_core_VT100_Cfg {
    mem_Allocator gpa;
    dage_core_VT100_ColorMode color_mode;
    bool enable_mouse; /* Enable mouse tracking */
    bool use_block_chars; /* Use Unicode block characters for higher resolution */
} dage_core_VT100_Cfg;

/*========== VT100 Instance ==========*/

/// @brief VT100 backend instance
/// @note  Opaque - access only through API functions
typedef struct dage_core_VT100 dage_core_VT100;
T_use_P$(dage_core_VT100);
T_use_E$(P$dage_core_VT100);

/*========== Lifecycle ==========*/

/// @brief Initialize VT100 backend
/// @details Puts terminal in raw mode
/// @note  Error boundary - may fail on terminal initialization
$attr($must_check)
$extern fn_((dage_core_VT100_init(dage_core_VT100_Cfg cfg))(E$P$dage_core_VT100));
/// @brief Shutdown VT100 backend
/// @details Restores terminal to normal mode
$extern fn_((dage_core_VT100_fini(dage_core_VT100** self))(void));
/// @brief Get dage_Backend interface
$extern fn_((dage_core_VT100_backend(dage_core_VT100* self))(dage_Backend));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_core_VT100__included */
