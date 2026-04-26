#ifndef fs_File_Atom__included
#define fs_File_Atom__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"
#include "../Dir.h"

/*========== Macros and Declarations ========================================*/

T_alias$((fs_File_Atom)(struct fs_File_Atom {
    var_(file, fs_File);
    var_(file_basename_hex, u64);
    var_(file_open, bool);
    var_(file_exists, bool);
    var_(dir, fs_Dir);
    var_(close_dir_on_fini, bool);
    var_(dest_sub_path, S_const$u8);
}));
$extern fn_((fs_File_Atom_fini(fs_File_Atom* self, fs_Self fs))(void));
$extern fn_((fs_File_Atom_link(fs_File_Atom* self, fs_Self fs))(E$void));
$extern fn_((fs_File_Atom_replace(fs_File_Atom* self, fs_Self fs))(E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_File_Atom__included */
