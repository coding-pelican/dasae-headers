#include "Preem.h"

fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem)) {
    return (exec_Preem){
        .gpa = mem_Alctr_ensureValid(gpa),
        .spawn_cfg = cleared(),
    };
};

fn_((exec_Preem_fini(exec_Preem* self))(void)) {
    claim_assert_nonnull(self);
    asg_l((self)(cleared()));
};
