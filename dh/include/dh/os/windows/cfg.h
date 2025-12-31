/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file      cfg.h
 * @author    Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date      2025-12-31 (date of creation)
 * @updated   2025-12-31 (date of last update)
 * @ingroup   dasae-headers(dh)/os/windows
 * @prefix    os_windows
 *
 * @brief     Windows Target Version Configuration
 * @details   Configures _WIN32_WINNT and NTDDI_VERSION macros based on
 *            Windows version constants. This allows precise targeting of
 *            Windows 10/11 updates (e.g., 22h2, 24h2).
 */
#ifndef os_windows_cfg__included
#define os_windows_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"
#include "dh/builtin/plat_cfg.h"

/*========== Macros and Declarations ========================================*/

#if plat_is_windows
/* Target Version */
#if !defined(os_windows_pref_target_ver)
#define os_windows_pref_target_ver __comp_int__os_windows_pref_target_ver
#endif /* os_windows_pref_target_ver */
#define os_windows__pref_target_ver_default __comp_int__os_windows__pref_target_ver_default

/* Latest Known Version */
#define os_windows_ver_latest __comp_int__os_windows_ver_latest

/* Legacy Versions */
#define os_windows_ver_nt4 0x04000000
#define os_windows_ver_win2k 0x05000000
#define os_windows_ver_xp 0x05010000
#define os_windows_ver_ws2003 0x05020000
#define os_windows_ver_vista 0x06000000
#define os_windows_ver_win7 0x06010000
#define os_windows_ver_win8 0x06020000
#define os_windows_ver_win8_1 0x06030000

/* Windows 10 Versions */
#define os_windows_ver_win10 0x0A000000 /* TH1 (1507) */
#define os_windows_ver_win10_th2 0x0A000001 /* 1511 */
#define os_windows_ver_win10_rs1 0x0A000002 /* 1607 */
#define os_windows_ver_win10_rs2 0x0A000003 /* 1703 */
#define os_windows_ver_win10_rs3 0x0A000004 /* 1709 */
#define os_windows_ver_win10_rs4 0x0A000005 /* 1803 */
#define os_windows_ver_win10_rs5 0x0A000006 /* 1809 */
#define os_windows_ver_win10_19h1 0x0A000007 /* 1903 */
#define os_windows_ver_win10_19h2 0x0A000008 /* 1909 (VB) */
#define os_windows_ver_win10_20h1 0x0A000009 /* 2004 (MN) */
#define os_windows_ver_win10_20h2 0x0A00000A /* (FE) */
#define os_windows_ver_win10_21h1 0x0A00000B /* (CO) */
#define os_windows_ver_win10_21h2 0x0A00000C /* (NI) */
#define os_windows_ver_win10_22h2 0x0A00000D /* (CU) - Last Win10 */

/* Windows 11 Versions */
#define os_windows_ver_win11 os_windows_ver_win10_21h2 /* Win11 base is technically 21h2 codebase */
#define os_windows_ver_win11_21h2 0x0A00000E /* (ZN) */
#define os_windows_ver_win11_22h2 0x0A00000F /* (GA) */
#define os_windows_ver_win11_23h2 0x0A000010 /* (GE) */
#define os_windows_ver_win11_24h2 0x0A000011 /* (DT) */

/* Lean and Mean */
#if !defined(os_windows_pref_lean_and_mean)
#define os_windows_pref_lean_and_mean \
    /** \
     * Force "Lean and Mean" to exclude rarely-used APIs (e.g., WinSock, Cryptography) \
     * from the global namespace to reduce build time and conflicts. \
     */ \
    __comp_bool__os_windows_pref_lean_and_mean
#endif /* os_windows_pref_lean_and_mean */
#define os_windows__pref_lean_and_mean_default __comp_bool__os_windows__pref_lean_and_mean_default

/* Nominmax */
#if !defined(os_windows_pref_nominmax)
#define os_windows_pref_nominmax \
    /** \
     * Prevent the definition of min() and max() macros in windows.h, which conflict \
     * with C++ standard library algorithms and modern C code. \
     */ \
    __comp_bool__os_windows_pref_nominmax
#endif /* os_windows_pref_nominmax */
#define os_windows__pref_nominmax_default __comp_bool__os_windows__pref_nominmax_default

/* Unicode */
#if !defined(os_windows_pref_unicode)
#define os_windows_pref_unicode \
    /** \
     * C runtime library counterpart to UNICODE. Enables wide-character versions \
     * of C runtime functions like _tcslen to resolve to wcslen. \
     */ \
    __comp_bool__os_windows_pref_unicode
#endif /* os_windows_pref_unicode */
#define os_windows__pref_unicode_default __comp_bool__os_windows__pref_unicode_default

/*========== Macros and Definitions =========================================*/

/* --- Default Values --- */

#define __comp_int__os_windows_pref_target_ver \
    os_windows__pref_target_ver_default
#define __comp_int__os_windows__pref_target_ver_default \
    os_windows_ver_win10

#define __comp_int__os_windows_ver_latest \
    os_windows_ver_win11_24h2

#define __comp_bool__os_windows_pref_unicode \
    os_windows__pref_unicode_default
#define __comp_bool__os_windows__pref_unicode_default \
    pp_true

#define __comp_bool__os_windows_pref_lean_and_mean \
    os_windows__pref_lean_and_mean_default
#define __comp_bool__os_windows__pref_lean_and_mean_default \
    pp_true

#define __comp_bool__os_windows_pref_nominmax \
    os_windows__pref_nominmax_default
#define __comp_bool__os_windows__pref_nominmax_default \
    pp_true

/* --- Build Options --- */

/* apply system macros
 * - _WIN32_WINNT / WINVER: High 16 bits (e.g., 0x0A00)
 * - NTDDI_VERSION: Full 32 bits (e.g., 0x0A00000D)
 */
#if !defined(_WIN32_WINNT)
/* Shift right by 16 to get 0xMMmm format (e.g., 0x0A00) */
#define _WIN32_WINNT (os_windows_pref_target_ver >> 16)
#endif /* _WIN32_WINNT */

#if !defined(WINVER)
#define WINVER _WIN32_WINNT
#endif /* WINVER */

#if !defined(NTDDI_VERSION)
/* Full precision for feature selection */
#define NTDDI_VERSION os_windows_pref_target_ver
#endif /* NTDDI_VERSION */

#if !defined(WIN32_LEAN_AND_MEAN) && (!os_windows_pref_lean_and_mean)
#define WIN32_LEAN_AND_MEAN 1
#endif /* WIN32_LEAN_AND_MEAN */

#if !defined(NOMINMAX) && (!os_windows_pref_nominmax)
#define NOMINMAX 1
#endif /* NOMINMAX */

#if !defined(UNICODE) && (!os_windows_pref_unicode)
#define UNICODE 1
#define _UNICODE 1
#endif /* UNICODE */
#endif /* plat_is_windows */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_cfg__included */
