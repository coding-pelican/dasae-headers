#include "dh/Cfg.h"

cmp_fn_eql$((Cfg_Comp)(lhs, rhs)) {
    return lhs.kind == rhs.kind
        && lhs.ver == rhs.ver
        && lhs.gnu_compat_ver == rhs.gnu_compat_ver;
};
cmp_fn_neq_default$((Cfg_Comp)(lhs, rhs));

cmp_fn_eql$((Cfg_Env)(lhs, rhs)) {
    return lhs.kind == rhs.kind
        && lhs.start_files_linked == rhs.start_files_linked
        && lhs.crt_linked == rhs.crt_linked
        && lhs.default_libs_linked == rhs.default_libs_linked
        && lhs.compiler_rt_linked == rhs.compiler_rt_linked
        && lhs.libc_linked == rhs.libc_linked
        && lhs.stdlib_linked == rhs.stdlib_linked;
};
cmp_fn_neq_default$((Cfg_Env)(lhs, rhs));

cmp_fn_eql$((Cfg_Lang)(lhs, rhs)) {
    return lhs.kind == rhs.kind
        && lhs.ver == rhs.ver
        && lhs.strict == rhs.strict
        && lhs.has_gnu_extn == rhs.has_gnu_extn
        && lhs.has_ms_extn == rhs.has_ms_extn;
};
cmp_fn_neq_default$((Cfg_Lang)(lhs, rhs));

fn_((Cfg_self(void))(Cfg)) {
    return Cfg_here();
};

cmp_fn_eql$((Cfg)(lhs, rhs)) {
    return cmp_eql$(Cfg_Comp)(lhs.comp, rhs.comp)
        && cmp_eql$(Cfg_Env)(lhs.env, rhs.env)
        && cmp_eql$(Cfg_Lang)(lhs.lang, rhs.lang)
        && cmp_eql$(Target)(lhs.target, rhs.target);
};
cmp_fn_neq_default$((Cfg)(lhs, rhs));
