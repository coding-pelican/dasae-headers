/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dll.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows dynamic link library (DLL) management APIs
 * @details Manages dynamic linking and library loading. Enables applications to
 *          load and unload modules at runtime and access exported functions.
 *
 * API Overview:
 * - Loading: LoadLibrary, LoadLibraryEx, GetModuleHandle
 * - Access: GetProcAddress
 * - Unloading: FreeLibrary, FreeLibraryAndExitThread
 * - Information: GetModuleFileName, GetModuleFileNameA, GetModuleFileNameW
 */
#ifndef os_windows_dll__included
#define os_windows_dll__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Dynamic Link Library (DLL) API
/// ==================================================================================
/// Provides functions to load, use, and unload dynamic libraries at runtime.
///
/// Key APIs:
///
/// Module Loading:
/// - HMODULE LoadLibrary(LPCSTR lpLibFileName);
///   Loads the specified module into the address space of the calling process.
///   @param lpLibFileName Path to the DLL file (e.g., "kernel32.dll")
///   @return Handle to the loaded module on success, \b NULL on failure
///   @note Use \b GetLastError() to retrieve error information.
///
/// - HMODULE LoadLibraryEx(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
///   Loads the specified module with additional options.
///   @param lpLibFileName Path to the DLL file
///   @param hFile Reserved, must be \b NULL
///   @param dwFlags Load options:
///     - \b LOAD_LIBRARY_SEARCH_APPLICATION_DIR : Search application directory first
///     - \b LOAD_LIBRARY_SEARCH_SYSTEM32 : Search System32 directory
///     - \b DONT_RESOLVE_DLL_REFERENCES : Don't call DllMain
///     - \b LOAD_WITH_ALTERED_SEARCH_PATH : Use alternate search path
///   @return Handle to the loaded module on success, \b NULL on failure
///
/// - HMODULE GetModuleHandle(LPCSTR lpModuleName);
///   Retrieves a module handle for the specified module if it's already loaded.
///   @param lpModuleName Module name (\b NULL for current executable)
///   @return Handle on success, \b NULL on failure
///
/// Function Access:
/// - FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
///   Retrieves the address of an exported function or variable from the DLL.
///   @param hModule Handle to the DLL module
///   @param lpProcName Name or ordinal of the function (e.g., "FunctionName" or (LPCSTR)1)
///   @return Address of the exported function on success, \b NULL on failure
///   @note Use \b GetLastError() to retrieve error information.
///
/// Module Information:
/// - DWORD GetModuleFileName(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
///   Retrieves the fully qualified path for the file that contains the specified module.
///   @param hModule Handle to the module (\b NULL for current executable)
///   @param lpFilename Buffer to receive the path
///   @param nSize Size of the buffer in characters
///   @return Length of the string copied to buffer on success, 0 on failure
///
/// - DWORD GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
///   ANSI version of GetModuleFileName.
///
/// - DWORD GetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
///   Unicode version of GetModuleFileName.
///
/// Module Unloading:
/// - BOOL FreeLibrary(HMODULE hLibModule);
///   Decrements the reference count of the loaded DLL. When the count reaches zero,
///   the module is unloaded from the address space.
///   @param hLibModule Handle to the loaded DLL
///   @return \b TRUE on success, \b FALSE on failure
///
/// - VOID FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode);
///   Decrements the reference count of the loaded DLL and calls ExitThread.
///   @param hLibModule Handle to the loaded DLL
///   @param dwExitCode Exit code for the thread
///   @note This function does not return.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/
#include <libloaderapi.h>
/// ==================================================================================
/// DLL Loading Constants (winbase.h subset)
/// ==================================================================================
/// These constants are typically defined in winbase.h but are needed for DLL operations.
/// Defining them here avoids the massive winbase.h dependency.
///
/// LoadLibraryEx Flags:
#if !defined(DONT_RESOLVE_DLL_REFERENCES)
#define DONT_RESOLVE_DLL_REFERENCES 0x00000001
#endif
#if !defined(LOAD_LIBRARY_AS_DATAFILE)
#define LOAD_LIBRARY_AS_DATAFILE 0x00000002
#endif
#if !defined(LOAD_WITH_ALTERED_SEARCH_PATH)
#define LOAD_WITH_ALTERED_SEARCH_PATH 0x00000008
#endif
#if !defined(LOAD_IGNORE_CODE_AUTHZ_LEVEL)
#define LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x00000010
#endif
#if !defined(LOAD_LIBRARY_AS_IMAGE_RESOURCE)
#define LOAD_LIBRARY_AS_IMAGE_RESOURCE 0x00000020
#endif
#if !defined(LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE)
#define LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE 0x00000040
#endif
#if !defined(LOAD_LIBRARY_REQUIRE_SIGNED_TARGET)
#define LOAD_LIBRARY_REQUIRE_SIGNED_TARGET 0x00000080
#endif
#if !defined(LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR)
#define LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR 0x00000100
#endif
#if !defined(LOAD_LIBRARY_SEARCH_APPLICATION_DIR)
#define LOAD_LIBRARY_SEARCH_APPLICATION_DIR 0x00000200
#endif
#if !defined(LOAD_LIBRARY_SEARCH_USER_DIRS)
#define LOAD_LIBRARY_SEARCH_USER_DIRS 0x00000400
#endif
#if !defined(LOAD_LIBRARY_SEARCH_SYSTEM32)
#define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800
#endif
#if !defined(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)
#define LOAD_LIBRARY_SEARCH_DEFAULT_DIRS 0x00001000
#endif
#if !defined(LOAD_LIBRARY_SAFE_CURRENT_DIRS)
#define LOAD_LIBRARY_SAFE_CURRENT_DIRS 0x00002000
#endif
/// DLL Reason Codes (for DllMain):
/// These are in winnt.h but included here for backward compatibility with older SDKs.
#if !defined(DLL_PROCESS_ATTACH)
#define DLL_PROCESS_ATTACH 1
#endif
#if !defined(DLL_THREAD_ATTACH)
#define DLL_THREAD_ATTACH 2
#endif
#if !defined(DLL_THREAD_DETACH)
#define DLL_THREAD_DETACH 3
#endif
#if !defined(DLL_PROCESS_DETACH)
#define DLL_PROCESS_DETACH 0
#endif
/// GET_MODULE_HANDLE_EX_FLAG_* constants (for GetModuleHandleEx):
#if !defined(GET_MODULE_HANDLE_EX_FLAG_PIN)
#define GET_MODULE_HANDLE_EX_FLAG_PIN 0x00000001
#endif
#if !defined(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT)
#define GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT 0x00000002
#endif
#if !defined(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS)
#define GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS 0x00000004
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_dll__included */
