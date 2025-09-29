#include "hex-sphere/stream.h"

#include "dh/io/common.h"
#if bti_plat_windows
#include "dh/os/windows.h"
#else /* bti_plat_unix */
#include <unistd.h>
#endif /* bti_plat_windows */

func((stream_fastOut(Sli_const$u8 data))(void)) {
    let stream_out = io_getStdOut().handle;
#if bti_plat_windows
#if !stream_fastOut__comp_windows_allow_ansi
    WriteConsole(stream_out, data.ptr, data.len, &(DWORD){}, null);
#else /* stream_fastOut__comp_windows_allow_ansi */
    WriteFile(stream_out, data.ptr, data.len, &(DWORD){}, null);
#endif
#else /* bti_plat_unix */
#if !stream_fastOut__comp_unix_allow_bulk
    $ignore = write(stream_out, data.ptr, data.len); // Direct syscall, bypasses stdio
#else /* stream_fastOut__comp_unix_allow_bulk */
    const usize chunk_len   = 65536; // 64KB chunks
    usize       written_len = 0;
    while (written_len < data.len) {
        let to_write = prim_min(chunk_len, data.len - written_len);
        $ignore      = write(stream_out, data.ptr + written_len, to_write);
        written_len += to_write;
    }
#endif
#endif
}
