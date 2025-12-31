/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-31 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows memory management APIs
 * @details Exposes memory management facilities for the process address space.
 *          Includes low-level virtual memory control for page-aligned allocation
 *          and heap functions for managing dynamic memory within a private heap.
 *          Use Virtual APIs for large, system-managed blocks and Heap APIs for
 *          general-purpose dynamic allocation.
 *
 * API Overview:
 * - Virtual Memory: VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery,
 *   VirtualLock, VirtualUnlock
 * - Heap Management: GetProcessHeap, HeapAlloc, HeapReAlloc, HeapFree,
 *   HeapSize, HeapValidate, HeapCreate, HeapDestroy
 */
#ifndef os_windows_mem__included
#define os_windows_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Virtual Memory API
/// ==================================================================================
/// Manages the virtual address space of a process. Use these functions for large,
/// page-aligned memory allocations (typically multiples of system page size, usually 4KB).
///
/// Key APIs:
///
/// - LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
///   Reserves, commits, or changes the state of a region of pages in the virtual address space.
///   @param lpAddress Desired starting address (\b NULL to let system choose)
///   @param dwSize Size of the region in bytes (must be multiple of page size)
///   @param flAllocationType Allocation type:
///     - \b MEM_COMMIT : Commit the pages (make them accessible)
///     - \b MEM_RESERVE : Reserve the address space (don't commit yet)
///     - \b MEM_RESET : Reset pages to zero (discard contents)
///     - \b MEM_TOP_DOWN : Allocate at highest possible address
///   @param flProtect Memory protection:
///     - \b PAGE_READONLY : Read-only access
///     - \b PAGE_READWRITE : Read and write access
///     - \b PAGE_EXECUTE_READ : Execute and read access
///     - \b PAGE_EXECUTE_READWRITE : Execute, read, and write access
///     - \b PAGE_NOACCESS : No access allowed
///   @return Starting address of the allocated region on success, \b NULL on failure
///
/// - BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
///   Releases, decommits, or releases and decommits a region of pages.
///   @param lpAddress Starting address of the region
///   @param dwSize Size of the region (must be 0 if \b MEM_RELEASE)
///   @param dwFreeType Free type:
///     - \b MEM_DECOMMIT : Decommit pages (free physical memory, keep reservation)
///     - \b MEM_RELEASE : Release pages (free both reservation and physical memory)
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
///   Changes the access protection of a region of committed pages.
///   @param lpAddress Starting address of the region
///   @param dwSize Size of the region in bytes
///   @param flNewProtect New protection flags (same as \b VirtualAlloc)
///   @param lpflOldProtect Receives the previous protection flags
///   @return \b TRUE on success, \b FALSE on failure
///
/// - SIZE_T VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);
///   Retrieves information about a range of pages in the virtual address space.
///   @param lpAddress Starting address to query
///   @param lpBuffer Buffer to receive \b MEMORY_BASIC_INFORMATION structure
///   @param dwLength Size of buffer
///   @return Number of bytes written to buffer, 0 on failure
///
/// - BOOL VirtualLock(LPVOID lpAddress, SIZE_T dwSize);
///   Locks the specified region of the process's virtual address space into physical memory.
///   @param lpAddress Starting address of the region
///   @param dwSize Size of the region in bytes
///   @return \b TRUE on success, \b FALSE on failure
///
/// - BOOL VirtualUnlock(LPVOID lpAddress, SIZE_T dwSize);
///   Unlocks a range of pages in the virtual address space.
///   @param lpAddress Starting address of the region
///   @param dwSize Size of the region in bytes
///   @return \b TRUE on success, \b FALSE on failure
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/
#include <memoryapi.h>
/// ==================================================================================
/// Heap Management API
/// ==================================================================================
/// Manages memory heaps. Use these for general-purpose dynamic memory allocation.
/// Heap allocation is more flexible than VirtualAlloc (no page alignment required).
///
/// Key APIs:
///
/// Default Heap:
/// - HANDLE GetProcessHeap(void);
///   Retrieves a handle to the default heap of the calling process.
///   @return Handle to the default heap (always valid, never \b NULL)
///
/// Heap Allocation:
/// - LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
///   Allocates a block of memory from a heap.
///   @param hHeap Handle to the heap (from \b GetProcessHeap() or \b HeapCreate())
///   @param dwFlags Allocation flags:
///     - \b HEAP_ZERO_MEMORY : Initialize allocated memory to zero
///     - \b HEAP_GENERATE_EXCEPTIONS : Generate exception on failure (instead of returning \b NULL)
///   @param dwBytes Number of bytes to allocate
///   @return Pointer to allocated memory on success, \b NULL on failure
///
/// - LPVOID HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);
///   Reallocates a block of memory from a heap.
///   @param hHeap Handle to the heap
///   @param dwFlags Reallocation flags:
///     - \b HEAP_ZERO_MEMORY : Zero-fill new memory (if growing)
///     - \b HEAP_REALLOC_IN_PLACE_ONLY : Only reallocate in place, fail if can't
///   @param lpMem Pointer to memory block to reallocate
///   @param dwBytes New size in bytes
///   @return Pointer to reallocated memory on success, \b NULL on failure
///
/// - BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
///   Frees a memory block allocated from a heap.
///   @param hHeap Handle to the heap
///   @param dwFlags Free flags (usually 0)
///   @param lpMem Pointer to memory block to free
///   @return \b TRUE on success, \b FALSE on failure
///
/// Heap Information:
/// - SIZE_T HeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);
///   Retrieves the size of a memory block allocated from a heap.
///   @param hHeap Handle to the heap
///   @param dwFlags Heap flags (usually 0)
///   @param lpMem Pointer to memory block
///   @return Size in bytes on success, (SIZE_T)-1 on failure
///
/// - BOOL HeapValidate(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);
///   Validates the specified heap.
///   @param hHeap Handle to the heap
///   @param dwFlags Heap flags (usually 0)
///   @param lpMem Pointer to memory block to validate (\b NULL to validate entire heap)
///   @return \b TRUE if valid, \b FALSE otherwise
///
/// Private Heap Management:
/// - HANDLE HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
///   Creates a private heap object.
///   @param flOptions Heap options:
///     - \b HEAP_NO_SERIALIZE : Disable serialization (faster, but not thread-safe)
///     - \b HEAP_GENERATE_EXCEPTIONS : Generate exceptions on failure
///   @param dwInitialSize Initial size of heap in bytes
///   @param dwMaximumSize Maximum size of heap (0 for unlimited)
///   @return Handle on success, \b NULL on failure
///
/// - BOOL HeapDestroy(HANDLE hHeap);
///   Destroys the specified heap object and deallocates all its memory.
///   @param hHeap Handle to the heap
///   @return \b TRUE on success, \b FALSE on failure
///   @note Only destroys private heaps created with \b HeapCreate(), not the default heap.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/heapapi/
#include <heapapi.h>
/// ==================================================================================
/// Memory Constants (winbase.h/winnt.h subset)
/// ==================================================================================
/// These constants are needed for memory operations. Some may be in winnt.h but are
/// redefined here with guards for completeness.
///
/// Heap Flags (HeapAlloc, HeapReAlloc, HeapCreate):
#if !defined(HEAP_NO_SERIALIZE)
#define HEAP_NO_SERIALIZE 0x00000001
#endif
#if !defined(HEAP_GENERATE_EXCEPTIONS)
#define HEAP_GENERATE_EXCEPTIONS 0x00000004
#endif
#if !defined(HEAP_ZERO_MEMORY)
#define HEAP_ZERO_MEMORY 0x00000008
#endif
#if !defined(HEAP_REALLOC_IN_PLACE_ONLY)
#define HEAP_REALLOC_IN_PLACE_ONLY 0x00000010
#endif
#if !defined(HEAP_CREATE_ENABLE_EXECUTE)
#define HEAP_CREATE_ENABLE_EXECUTE 0x00040000
#endif
/// Virtual Memory Allocation Types (flAllocationType for VirtualAlloc):
/// These are in winnt.h but included here for backward compatibility with older SDKs.
#if !defined(MEM_COMMIT)
#define MEM_COMMIT 0x00001000
#endif
#if !defined(MEM_RESERVE)
#define MEM_RESERVE 0x00002000
#endif
#if !defined(MEM_DECOMMIT)
#define MEM_DECOMMIT 0x00004000
#endif
#if !defined(MEM_RELEASE)
#define MEM_RELEASE 0x00008000
#endif
#if !defined(MEM_RESET)
#define MEM_RESET 0x00080000
#endif
#if !defined(MEM_TOP_DOWN)
#define MEM_TOP_DOWN 0x00100000
#endif
#if !defined(MEM_WRITE_WATCH)
#define MEM_WRITE_WATCH 0x00200000
#endif
#if !defined(MEM_PHYSICAL)
#define MEM_PHYSICAL 0x00400000
#endif
#if !defined(MEM_RESET_UNDO)
#define MEM_RESET_UNDO 0x01000000
#endif
#if !defined(MEM_LARGE_PAGES)
#define MEM_LARGE_PAGES 0x20000000
#endif
/// Memory Protection Constants (flProtect for VirtualAlloc/VirtualProtect):
/// These are in winnt.h but included here for backward compatibility with older SDKs.
#if !defined(PAGE_NOACCESS)
#define PAGE_NOACCESS 0x01
#endif
#if !defined(PAGE_READONLY)
#define PAGE_READONLY 0x02
#endif
#if !defined(PAGE_READWRITE)
#define PAGE_READWRITE 0x04
#endif
#if !defined(PAGE_WRITECOPY)
#define PAGE_WRITECOPY 0x08
#endif
#if !defined(PAGE_EXECUTE)
#define PAGE_EXECUTE 0x10
#endif
#if !defined(PAGE_EXECUTE_READ)
#define PAGE_EXECUTE_READ 0x20
#endif
#if !defined(PAGE_EXECUTE_READWRITE)
#define PAGE_EXECUTE_READWRITE 0x40
#endif
#if !defined(PAGE_EXECUTE_WRITECOPY)
#define PAGE_EXECUTE_WRITECOPY 0x80
#endif
#if !defined(PAGE_GUARD)
#define PAGE_GUARD 0x100
#endif
#if !defined(PAGE_NOCACHE)
#define PAGE_NOCACHE 0x200
#endif
#if !defined(PAGE_WRITECOMBINE)
#define PAGE_WRITECOMBINE 0x400
#endif
#pragma comment(lib, "kernel32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_mem__included */
