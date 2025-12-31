/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sysinfo.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows system information, performance counter, and version helper APIs
 * @details Retrieves information about the current system configuration, hardware
 *          properties, and provides high-resolution performance counters for
 *          precise time measurement. Also provides version helper functions
 *          to safely detect Windows version.
 *
 * API Overview:
 * - System Info: GetSystemInfo, GetNativeSystemInfo, GetSystemTime, GetLocalTime,
 *   GetSystemTimeAsFileTime, GetTickCount, GetTickCount64 [Min: Vista],
 *   GetComputerName, GetVersion [Deprecated]
 * - Performance: QueryPerformanceCounter, QueryPerformanceFrequency
 * - Version Helpers: IsWindowsXPOrGreater, IsWindowsVistaOrGreater,
 *   IsWindows7OrGreater, IsWindows8OrGreater, IsWindows8Point1OrGreater,
 *   IsWindows10OrGreater, IsWindowsServer, IsWindowsVersionOrGreater
 *
 * @note GetUserName is in winbase.h (and requires Advapi32.lib), so it is excluded here.
 */
#ifndef os_windows_sysinfo__included
#define os_windows_sysinfo__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// System Information API
/// ==================================================================================
/// Retrieves information about the current system configuration and hardware.
///
/// Key APIs:
///
/// - VOID GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);
///   Retrieves information about the current system.
///   @param lpSystemInfo Pointer to SYSTEM_INFO structure to fill:
///   typedef struct _SYSTEM_INFO {
///       union {
///           DWORD dwOemId;
///           struct {
///               WORD wProcessorArchitecture;  // PROCESSOR_ARCHITECTURE_AMD64, etc.
///               WORD wReserved;
///           };
///       };
///       DWORD dwPageSize;                    // Page size in bytes (usually 4096)
///       LPVOID lpMinimumApplicationAddress;  // Minimum address
///       LPVOID lpMaximumApplicationAddress;  // Maximum address
///       DWORD_PTR dwActiveProcessorMask;     // Active processor mask
///       DWORD dwNumberOfProcessors;          // Number of processors
///       DWORD dwProcessorType;               // Processor type
///       DWORD dwAllocationGranularity;       // Allocation granularity
///       WORD wProcessorLevel;                // Processor level
///       WORD wProcessorRevision;             // Processor revision
///   } SYSTEM_INFO;
///
/// - VOID GetNativeSystemInfo(LPSYSTEM_INFO lpSystemInfo);
///   Retrieves information about the system even when running under WOW64 (32-bit on 64-bit).
///   @param lpSystemInfo Pointer to SYSTEM_INFO structure to fill
///   @note Use this to get actual system info, not WOW64 emulated info.
///
/// - VOID GetSystemTime(LPSYSTEMTIME lpSystemTime);
///   Retrieves the current system date and time in UTC (Coordinated Universal Time).
///   @param lpSystemTime Pointer to SYSTEMTIME structure to fill (see base.h for structure definition)
///
/// - VOID GetLocalTime(LPSYSTEMTIME lpSystemTime);
///   Retrieves the current system date and time in local time.
///   @param lpSystemTime Pointer to SYSTEMTIME structure to fill
///
/// - VOID GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime);
///   Retrieves the current system date and time as a FILETIME structure (UTC).
///   @param lpSystemTimeAsFileTime Pointer to FILETIME structure to fill
///
/// - BOOL GetComputerNameA(LPSTR lpBuffer, LPDWORD nSize);
///   Retrieves the NetBIOS name of the local computer (ANSI).
///   @param lpBuffer Buffer to receive the computer name
///   @param nSize On input, specifies buffer size. On output, receives number of characters copied.
///   @return \b TRUE on success, \b FALSE on failure
///
/// - DWORD GetTickCount(void);
///   Retrieves the number of milliseconds since the system was started.
///   @return 32-bit value (wraps after ~49.7 days)
///   @warning Wraps around to zero after approximately 49.7 days. Use \b GetTickCount64 instead.
///
/// - ULONGLONG GetTickCount64(void);  [Min: Vista]
///   Retrieves the number of milliseconds since the system was started.
///   @return 64-bit value (no wrap-around)
///   @note Requires Windows Vista or later. Preferred over \b GetTickCount.
///
/// - DWORD GetVersion(void);  [Deprecated]
///   Retrieves the version number of the operating system.
///   @return Version info encoded in DWORD
///   @deprecated Use Version Helper functions instead: IsWindows10OrGreater(), IsWindows8OrGreater(), etc.
///   Include <VersionHelpers.h> for these functions.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/
#include <sysinfoapi.h>
/// ==================================================================================
/// Performance Counter API
/// ==================================================================================
/// Provides high-resolution timers for performance profiling. These functions provide
/// the highest resolution timing available on Windows (typically microsecond precision).
///
/// Key APIs:
///
/// - BOOL QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
///   Retrieves the current value of the performance counter.
///   @param lpPerformanceCount Receives the current counter value
///   @return \b TRUE on success, \b FALSE on failure
///   @note Call this before and after code to measure elapsed time.
///   @code{.c}
///     LARGE_INTEGER start, end, frequency;
///     QueryPerformanceFrequency(&frequency);
///     QueryPerformanceCounter(&start);
///     // ... code to measure ...
///     QueryPerformanceCounter(&end);
///     double elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
///   @endcode
///
/// - BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
///   Retrieves the frequency of the performance counter (counts per second).
///   @param lpFrequency Receives the frequency (typically 10,000,000 for 10 MHz, or 3,579,545 for 3.58 MHz)
///   @return \b TRUE on success, \b FALSE on failure
///   @note Call once at program start, then use with \b QueryPerformanceCounter to calculate elapsed time.
///   Frequency is constant for the system, so only needs to be queried once.
///
/// Performance Counter Structure:
///   LARGE_INTEGER is a union that can be accessed as:
///   - QuadPart: 64-bit integer (LONGLONG)
///   - LowPart: Low 32 bits (DWORD)
///   - HighPart: High 32 bits (LONG)
///
/// Time Calculation:
///   Elapsed time in seconds = (end.QuadPart - start.QuadPart) / frequency.QuadPart
///   Elapsed time in milliseconds = (end.QuadPart - start.QuadPart) * 1000 / frequency.QuadPart
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/profileapi/
#include <profileapi.h>
/// ==================================================================================
/// Version Helper API
/// ==================================================================================
/// Provides inline functions to detect Windows version at runtime. These functions
/// are the recommended replacement for the deprecated GetVersion() and
/// GetVersionEx() functions.
///
/// Key APIs:
///
/// - BOOL IsWindowsXPOrGreater(void);
///   Tests if Windows version is XP (5.1) or greater.
///   @return \b TRUE if Windows XP or later
///
/// - BOOL IsWindowsXPSP1OrGreater(void);
///   Tests if Windows version is XP SP1 or greater.
///   @return \b TRUE if Windows XP SP1 or later
///
/// - BOOL IsWindowsXPSP2OrGreater(void);
///   Tests if Windows version is XP SP2 or greater.
///   @return \b TRUE if Windows XP SP2 or later
///
/// - BOOL IsWindowsXPSP3OrGreater(void);
///   Tests if Windows version is XP SP3 or greater.
///   @return \b TRUE if Windows XP SP3 or later
///
/// - BOOL IsWindowsVistaOrGreater(void);
///   Tests if Windows version is Vista (6.0) or greater.
///   @return \b TRUE if Windows Vista or later
///
/// - BOOL IsWindowsVistaSP1OrGreater(void);
///   Tests if Windows version is Vista SP1 or greater.
///   @return \b TRUE if Windows Vista SP1 or later
///
/// - BOOL IsWindowsVistaSP2OrGreater(void);
///   Tests if Windows version is Vista SP2 or greater.
///   @return \b TRUE if Windows Vista SP2 or later
///
/// - BOOL IsWindows7OrGreater(void);
///   Tests if Windows version is 7 (6.1) or greater.
///   @return \b TRUE if Windows 7 or later
///
/// - BOOL IsWindows7SP1OrGreater(void);
///   Tests if Windows version is 7 SP1 or greater.
///   @return \b TRUE if Windows 7 SP1 or later
///
/// - BOOL IsWindows8OrGreater(void);
///   Tests if Windows version is 8 (6.2) or greater.
///   @return \b TRUE if Windows 8 or later
///
/// - BOOL IsWindows8Point1OrGreater(void);
///   Tests if Windows version is 8.1 (6.3) or greater.
///   @return \b TRUE if Windows 8.1 or later
///
/// - BOOL IsWindows10OrGreater(void);
///   Tests if Windows version is 10 (10.0) or greater.
///   @return \b TRUE if Windows 10 or later
///   @note Requires application manifest to report correct version on Win 8.1+.
///
/// - BOOL IsWindowsServer(void);
///   Tests if the current OS is a Windows Server release.
///   @return \b TRUE if Windows Server edition
///
/// - BOOL IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
///   Tests if Windows version meets the specified requirements.
///   @param wMajorVersion Major version number
///   @param wMinorVersion Minor version number
///   @param wServicePackMajor Service pack major version
///   @return \b TRUE if version meets requirements
///
/// Important Notes:
/// - For Windows 8.1 and later, applications must include a manifest to receive
///   accurate version information. Without a manifest, these functions report
///   Windows 8 (6.2) as the maximum version.
/// - Add this to your application manifest for accurate Win10+ detection:
///   @code{.xml}
///   <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
///       <application>
///           <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/> <!-- Win10 -->
///       </application>
///   </compatibility>
///   @endcode
///
/// Usage Example:
/// @code{.c}
///     if (IsWindows10OrGreater()) {
///         // Use Windows 10+ specific features
///     } else if (IsWindows7OrGreater()) {
///         // Fallback for Windows 7+
///     } else {
///         // Legacy path
///     }
/// @endcode
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/sysinfo/version-helper-apis
#include <VersionHelpers.h>
/// ==================================================================================
/// System Information Constants (winbase.h/winnt.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h or winnt.h but are needed for
/// system information operations.
///
/// Processor Architecture (SYSTEM_INFO.wProcessorArchitecture):
#if !defined(PROCESSOR_ARCHITECTURE_INTEL)
#define PROCESSOR_ARCHITECTURE_INTEL 0
#endif
#if !defined(PROCESSOR_ARCHITECTURE_MIPS)
#define PROCESSOR_ARCHITECTURE_MIPS 1
#endif
#if !defined(PROCESSOR_ARCHITECTURE_ALPHA)
#define PROCESSOR_ARCHITECTURE_ALPHA 2
#endif
#if !defined(PROCESSOR_ARCHITECTURE_PPC)
#define PROCESSOR_ARCHITECTURE_PPC 3
#endif
#if !defined(PROCESSOR_ARCHITECTURE_SHX)
#define PROCESSOR_ARCHITECTURE_SHX 4
#endif
#if !defined(PROCESSOR_ARCHITECTURE_ARM)
#define PROCESSOR_ARCHITECTURE_ARM 5
#endif
#if !defined(PROCESSOR_ARCHITECTURE_IA64)
#define PROCESSOR_ARCHITECTURE_IA64 6
#endif
#if !defined(PROCESSOR_ARCHITECTURE_ALPHA64)
#define PROCESSOR_ARCHITECTURE_ALPHA64 7
#endif
#if !defined(PROCESSOR_ARCHITECTURE_MSIL)
#define PROCESSOR_ARCHITECTURE_MSIL 8
#endif
#if !defined(PROCESSOR_ARCHITECTURE_AMD64)
#define PROCESSOR_ARCHITECTURE_AMD64 9
#endif
#if !defined(PROCESSOR_ARCHITECTURE_IA32_ON_WIN64)
#define PROCESSOR_ARCHITECTURE_IA32_ON_WIN64 10
#endif
#if !defined(PROCESSOR_ARCHITECTURE_NEUTRAL)
#define PROCESSOR_ARCHITECTURE_NEUTRAL 11
#endif
#if !defined(PROCESSOR_ARCHITECTURE_ARM64)
#define PROCESSOR_ARCHITECTURE_ARM64 12
#endif
#if !defined(PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64)
#define PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64 13
#endif
#if !defined(PROCESSOR_ARCHITECTURE_IA32_ON_ARM64)
#define PROCESSOR_ARCHITECTURE_IA32_ON_ARM64 14
#endif
#if !defined(PROCESSOR_ARCHITECTURE_UNKNOWN)
#define PROCESSOR_ARCHITECTURE_UNKNOWN 0xFFFF
#endif
/// Computer Name Buffer Size:
#if !defined(MAX_COMPUTERNAME_LENGTH)
#define MAX_COMPUTERNAME_LENGTH 15
#endif
/// Time Constants:
/// Conversion factor between FILETIME units and seconds.
/// FILETIME is in 100-nanosecond intervals since January 1, 1601 (UTC).
#if !defined(FILETIME_UNIX_EPOCH_DIFF)
/// Difference between Windows FILETIME epoch (1601) and Unix epoch (1970)
/// in 100-nanosecond intervals: 11644473600 seconds * 10000000
#define FILETIME_UNIX_EPOCH_DIFF 116444736000000000LL
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_sysinfo__included */
