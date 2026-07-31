#include "dh/proc/Preopens.h"
#include "dh/mem/common.h"

fn_((proc_Preopens_direct(void))(proc_Preopens)) {
    return (proc_Preopens){};
};

fn_((proc_Preopens_by(
    proc_Preopens self,
    S_const$u8 name
))(O$proc_Preopens_Resrc) $scope) {
    let_ignore = self;
    if (mem_eqlBytes(name, u8_l("stdin"))) {
        return_some(union_of$((proc_Preopens_Resrc)(proc_Preopens_Resrc_file)(
            proc_std_in()
        )));
    }
    if (mem_eqlBytes(name, u8_l("stdout"))) {
        return_some(union_of$((proc_Preopens_Resrc)(proc_Preopens_Resrc_file)(
            proc_std_out()
        )));
    }
    if (mem_eqlBytes(name, u8_l("stderr"))) {
        return_some(union_of$((proc_Preopens_Resrc)(proc_Preopens_Resrc_file)(
            proc_std_err()
        )));
    }
    return_none();
} $unscoped(fn);
