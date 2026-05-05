#include "dh/prl/Err.h"
#include "dh/mem/common.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"

fn_((E_print(const EAny* err))(void)) {
    let out = fs_File_writer(io_getStdErr());
    catch_((io_Writer_println(
        out, u8_l("Program failed: {:e}"), *err
    ))($ignore, claim_unreachable));
};
