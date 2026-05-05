#ifndef fs_File_MemMap__included
#define fs_File_MemMap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((fs_File_MemMap_Protection)(struct fs_File_MemMap_Protection {
    var_(read, bool);
    var_(write, bool);
    var_(execute, bool);
}));
$static let_(fs_File_MemMap_Protection_default, fs_File_MemMap_Protection) = {
    .read = true,
    .write = true,
    .execute = false,
};

T_alias$((fs_File_MemMap)(struct fs_File_MemMap {
    var_(file, fs_File);
    var_(offset, u64);
    var_(mem, S$u8);
    var_(section, P$raw);
}));
T_use_E$($set(fs_E)(fs_File_MemMap));
$attr($must_check)
$extern fn_((fs_File_MemMap_init(
    fs_File file,
    fs_Self fs,
    usize len,
    fs_File_MemMap_Protection protection,
    bool undefined_contents,
    bool populate,
    u64 offset
))(fs_E$fs_File_MemMap));
$extern fn_((fs_File_MemMap_fini(fs_File_MemMap* self, fs_Self fs))(void));
$attr($must_check)
$extern fn_((fs_File_MemMap_setLen(fs_File_MemMap* self, fs_Self fs, usize new_len))(fs_E$void));
$attr($must_check)
$extern fn_((fs_File_MemMap_read(fs_File_MemMap* self, fs_Self fs))(fs_E$void));
$attr($must_check)
$extern fn_((fs_File_MemMap_write(fs_File_MemMap* self, fs_Self fs))(fs_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_File_MemMap__included */
