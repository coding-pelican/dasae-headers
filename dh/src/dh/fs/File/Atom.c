#include "dh/fs/File/Atom.h"

fn_((fs_File_Atom_fini(fs_File_Atom* self))(void)) {
    if (!isNonnull(self)) return;
    if (self->file_open) {
        fs_File_close(self->file);
        self->file_open = false;
    }
    if (self->close_dir_on_fini) {
        fs_Dir_close(&self->dir);
        self->close_dir_on_fini = false;
    }
};

fn_((fs_File_Atom_link(fs_File_Atom* self))(E$void) $scope) {
    claim_assert_nonnull(self);
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);

fn_((fs_File_Atom_replace(fs_File_Atom* self))(E$void) $scope) {
    claim_assert_nonnull(self);
    return_err(E_cause$fs_Unsupported());
} $unscoped(fn);
