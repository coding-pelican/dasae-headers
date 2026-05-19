#include "dh/mem/AlcTrace.h"

fn_((mem_AlcTrace_initWithLogPath(S_const$u8 log_path))(E$void) $scope) {
    let_ignore = log_path;
    return_err(E_cause$NotImplemented());
} $unscoped(fn);
fn_((mem_AlcTrace_finiAndGenerateReport(void))(void)) $do_nothing;

fn_((mem_AlcTrace_registerAlloc($traced P$raw ptr, usize size))(void)) {
    let_ignore = ptr;
    let_ignore = size;
};
fn_((mem_AlcTrace_registerRemap($traced P$raw old_ptr, P$raw new_ptr, usize new_size))(void)) {
    let_ignore = old_ptr;
    let_ignore = new_ptr;
    let_ignore = new_size;
};
fn_((mem_AlcTrace_registerFree($traced P$raw ptr))(bool)) {
    let_ignore = ptr;
    return true;
};
