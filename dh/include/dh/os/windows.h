/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    windows.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-13 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os
 * @prefix  (none)
 *
 * @brief   Windows API wrapper header
 * @details Provides a comprehensive wrapper for the Windows API.
 */
#ifndef os_windows__included
#define os_windows__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

/* Core Foundation */
#include "windows/cfg.h" /* Configuration (target_ver, WIN32_LEAN_AND_MEAN, UNICODE, etc.) */
#include "windows/base.h" /* Types (HANDLE, DWORD...) */
#include "windows/handle.h" /* Error handling (Win32 Errors) */
#include "windows/debug.h" /* Debugging (OutputDebugString) */
#include "windows/nls.h" /* Unicode Conversion (MultiByteToWideChar) */
#include "windows/sysinfo.h" /* System Info & Time */

/* System Resources */
#include "windows/mem.h" /* Virtual & Heap Memory */
#include "windows/file.h" /* Filesystem */
#include "windows/io.h" /* IOCP & DeviceIoControl */
#include "windows/dll.h" /* Dynamic Linking */
#include "windows/console.h" /* Console I/O */

/* Execution */
#include "windows/proc.h" /* Process Management */
#include "windows/thrd.h" /* Thread & TLS (TlsAlloc included here) */
#include "windows/sync.h" /* Synchronization (Futex/SRWLock) */

/* Native API */
#include "windows/nt.h" /* NTAPI (NtCreateFile, NTSTATUS...) */

/* Security & Crypto */
#include "windows/auth.h" /* Advapi32 (User, Registry, Random) */
#include "windows/crypt.h" /* Crypt32 (Certificates, Protection) */

/* Networking */
#include "windows/sock.h" /* Winsock2 */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows__included */
