/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    nls.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows National Language Support (NLS) APIs
 * @details Provides functions for Unicode/ANSI string conversion and locale
 *          handling. Essential for internationalization and character encoding.
 *
 * API Overview:
 * - String Conversion: MultiByteToWideChar, WideCharToMultiByte
 * - Code Pages: GetACP, GetOEMCP, IsValidCodePage
 * - Locale: GetUserDefaultLCID, GetSystemDefaultLCID, GetLocaleInfo
 */
#ifndef os_windows_nls__included
#define os_windows_nls__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// String Conversion API
/// ==================================================================================
/// Provides functions for converting between multi-byte (ANSI) and wide-character
/// (Unicode) strings. Essential for working with international text.
///
/// Key APIs:
///
/// Multi-byte to Wide-char:
/// - int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
///   Maps a character string to a UTF-16 (wide character) string.
///   @param CodePage Code page to use:
///     - \b CP_ACP : System default ANSI code page
///     - \b CP_UTF8 : UTF-8 code page
///     - \b CP_UTF7 : UTF-7 code page
///   @param dwFlags Conversion flags:
///     - \b 0 : No flags
///     - \b MB_ERR_INVALID_CHARS : Fail on invalid input characters
///     - \b MB_PRECOMPOSED : Use precomposed characters (default)
///   @param lpMultiByteStr Source multi-byte string
///   @param cbMultiByte Size of source in bytes (-1 if null-terminated)
///   @param lpWideCharStr Destination wide-char buffer (\b NULL to query size)
///   @param cchWideChar Size of destination in characters (0 to query size)
///   @return Number of characters written, 0 on failure
///   @note Call with cchWideChar=0 to get required buffer size.
///   @code{.c}
///     // Convert UTF-8 to UTF-16
///     int size = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
///     WCHAR* wide = malloc(size * sizeof(WCHAR));
///     MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wide, size);
///   @endcode
///
/// Wide-char to Multi-byte:
/// - int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);
///   Maps a UTF-16 (wide character) string to a character string.
///   @param CodePage Code page to use (\b CP_ACP, \b CP_UTF8, etc.)
///   @param dwFlags Conversion flags:
///     - \b 0 : No flags
///     - \b WC_ERR_INVALID_CHARS : Fail on invalid input characters [Min: Vista]
///     - \b WC_NO_BEST_FIT_CHARS : Prevent best-fit mapping
///   @param lpWideCharStr Source wide-char string
///   @param cchWideChar Size of source in characters (-1 if null-terminated)
///   @param lpMultiByteStr Destination multi-byte buffer (\b NULL to query size)
///   @param cbMultiByte Size of destination in bytes (0 to query size)
///   @param lpDefaultChar Default char for unmappable chars (\b NULL for system default)
///   @param lpUsedDefaultChar Receives \b TRUE if default char was used (\b NULL if not needed)
///   @return Number of bytes written, 0 on failure
///   @note For \b CP_UTF8, lpDefaultChar and lpUsedDefaultChar must be \b NULL.
///
/// Code Page Information:
/// - UINT GetACP(void);
///   Retrieves the current ANSI code page for the system.
///   @return Code page identifier
///
/// - UINT GetOEMCP(void);
///   Retrieves the current OEM code page for the system.
///   @return Code page identifier
///
/// - BOOL IsValidCodePage(UINT CodePage);
///   Determines whether the specified code page is valid.
///   @param CodePage Code page identifier
///   @return \b TRUE if valid, \b FALSE otherwise
///
/// Locale Information:
/// - LCID GetUserDefaultLCID(void);
///   Retrieves the locale identifier (LCID) for the user default locale.
///   @return Locale identifier
///
/// - LCID GetSystemDefaultLCID(void);
///   Retrieves the locale identifier for the system default locale.
///   @return Locale identifier
///
/// - int GetLocaleInfoA(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData);
///   Retrieves information about a locale (ANSI).
///   @param Locale Locale identifier (or \b LOCALE_USER_DEFAULT, \b LOCALE_SYSTEM_DEFAULT)
///   @param LCType Type of information to retrieve (\b LOCALE_SNAME, \b LOCALE_SLANGUAGE, etc.)
///   @param lpLCData Buffer to receive information
///   @param cchData Size of buffer (0 to query size)
///   @return Number of characters retrieved, 0 on failure
///
/// Common Code Pages:
/// - \b CP_ACP (0): System default ANSI code page
/// - \b CP_OEMCP (1): System default OEM code page
/// - \b CP_UTF7 (65000): UTF-7 code page
/// - \b CP_UTF8 (65001): UTF-8 code page
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/
#include <stringapiset.h>
/// ==================================================================================
/// NLS Constants (winnls.h subset)
/// ==================================================================================
/// Defines code page identifiers and conversion flags for string conversion functions.
///
/// Code Page Identifiers:
/// - \b CP_ACP (0): System default Windows ANSI code page
/// - \b CP_OEMCP (1): Current system OEM code page
/// - \b CP_MACCP (2): Current system Macintosh code page
/// - \b CP_THREAD_ACP (3): Windows ANSI code page for the current thread
/// - \b CP_SYMBOL (42): Symbol code page (42)
/// - \b CP_UTF7 (65000): UTF-7 code page
/// - \b CP_UTF8 (65001): UTF-8 code page
#if !defined(CP_ACP)
#define CP_ACP 0
#endif
#if !defined(CP_OEMCP)
#define CP_OEMCP 1
#endif
#if !defined(CP_MACCP)
#define CP_MACCP 2
#endif
#if !defined(CP_THREAD_ACP)
#define CP_THREAD_ACP 3
#endif
#if !defined(CP_SYMBOL)
#define CP_SYMBOL 42
#endif
#if !defined(CP_UTF7)
#define CP_UTF7 65000
#endif
#if !defined(CP_UTF8)
#define CP_UTF8 65001
#endif
/// MultiByteToWideChar Flags:
/// - \b MB_PRECOMPOSED (0x00000001): Use precomposed characters
/// - \b MB_COMPOSITE (0x00000002): Use composite characters
/// - \b MB_USEGLYPHCHARS (0x00000004): Use glyph characters instead of control characters
/// - \b MB_ERR_INVALID_CHARS (0x00000008): Fail on invalid input characters
#if !defined(MB_PRECOMPOSED)
#define MB_PRECOMPOSED 0x00000001
#endif
#if !defined(MB_COMPOSITE)
#define MB_COMPOSITE 0x00000002
#endif
#if !defined(MB_USEGLYPHCHARS)
#define MB_USEGLYPHCHARS 0x00000004
#endif
#if !defined(MB_ERR_INVALID_CHARS)
#define MB_ERR_INVALID_CHARS 0x00000008
#endif
/// WideCharToMultiByte Flags:
/// - \b WC_COMPOSITECHECK (0x00000200): Convert composite to precomposed
/// - \b WC_DISCARDNS (0x00000010): Discard nonspacing characters
/// - \b WC_SEPCHARS (0x00000020): Generate separate characters
/// - \b WC_DEFAULTCHAR (0x00000040): Replace exceptions with default char
/// - \b WC_NO_BEST_FIT_CHARS (0x00000400): Do not use best-fit fallback
/// - \b WC_ERR_INVALID_CHARS (0x00000080): Fail on invalid input [Min: Vista]
#if !defined(WC_COMPOSITECHECK)
#define WC_COMPOSITECHECK 0x00000200
#endif
#if !defined(WC_DISCARDNS)
#define WC_DISCARDNS 0x00000010
#endif
#if !defined(WC_SEPCHARS)
#define WC_SEPCHARS 0x00000020
#endif
#if !defined(WC_DEFAULTCHAR)
#define WC_DEFAULTCHAR 0x00000040
#endif
#if !defined(WC_NO_BEST_FIT_CHARS)
#define WC_NO_BEST_FIT_CHARS 0x00000400
#endif
#if !defined(WC_ERR_INVALID_CHARS)
#define WC_ERR_INVALID_CHARS 0x00000080
#endif
/// Locale Constants:
/// - \b LOCALE_USER_DEFAULT: Default locale for the user
/// - \b LOCALE_SYSTEM_DEFAULT: Default locale for the system
#if !defined(LOCALE_USER_DEFAULT)
#define LOCALE_USER_DEFAULT 0x0400
#endif
#if !defined(LOCALE_SYSTEM_DEFAULT)
#define LOCALE_SYSTEM_DEFAULT 0x0800
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_nls__included */
