#include "dh-main.h"
#include "dh/start/Invoc/Env.h"
#include "dh/mem/common.h"

TEST_fn_("start/Invoc/Env: VecZ adapter supports iteration and lookup" $scope) {
    var_(native, A$$(2, P_const$u8)) = A_init({
        [0] = as$(P_const$u8)("FIRST=one"),
        [1] = as$(P_const$u8)("SECOND=two"),
    });
    var adapter = start_Invoc_Env_initVecZ(
        A_len(native), as$(P_const$P_const$u8)(A_ptr(native))
    );
    let env = start_Invoc_Env_self(&adapter);
    var_(scratch_mem, A$$(32, u8)) $undefined;
    let scratch = A_ref$((S$u8)(scratch_mem));
    var iter = proc_Env_iter(env);

    try_(TEST_expect(mem_eqlBytes(
        unwrap_(try_(proc_Env_Iter_next(&iter, scratch))), u8_l("FIRST=one")
    )));
    try_(TEST_expect(mem_eqlBytes(
        unwrap_(try_(proc_Env_by(env, u8_l("SECOND"), scratch))), u8_l("two")
    )));
} $unscoped(TEST_fn);
