#include "dh-main.h"
#include "dh/fs/common.h"

TEST_fn_("fs/common: open modes expose independent read and write intent" $scope) {
    let read = (fs_OpenFlags){ .mode = fs_OpenMode_read_only };
    let write = (fs_OpenFlags){ .mode = fs_OpenMode_write_only };
    let both = (fs_OpenFlags){ .mode = fs_OpenMode_read_write };

    try_(TEST_expect(fs_OpenFlags_isRead(read)));
    try_(TEST_expect(!fs_OpenFlags_isWrite(read)));
    try_(TEST_expect(!fs_OpenFlags_isRead(write)));
    try_(TEST_expect(fs_OpenFlags_isWrite(write)));
    try_(TEST_expect(fs_OpenFlags_isRead(both)));
    try_(TEST_expect(fs_OpenFlags_isWrite(both)));
} $unscoped(TEST_fn);
