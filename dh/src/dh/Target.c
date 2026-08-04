#include "dh/Target.h"

cmp_fn_eql$((Target_Arch)(lhs, rhs)) {
    return lhs.kind == rhs.kind
        && lhs.family == rhs.family
        && lhs.endian == rhs.endian
        && lhs.word_bits == rhs.word_bits
        && lhs.byte_bits == rhs.byte_bits
        && lhs.cache_line_bytes == rhs.cache_line_bytes;
};
cmp_fn_neq_default$((Target_Arch)(lhs, rhs));

cmp_fn_eql$((Target_Plat)(lhs, rhs)) { return lhs.kind == rhs.kind; };
cmp_fn_neq_default$((Target_Plat)(lhs, rhs));

cmp_fn_eql$((Target_ABI)(lhs, rhs)) {
    return lhs.char_bits == rhs.char_bits
        && lhs.short_bits == rhs.short_bits
        && lhs.int_bits == rhs.int_bits
        && lhs.long_bits == rhs.long_bits
        && lhs.long_long_bits == rhs.long_long_bits
        && lhs.pointer_bits == rhs.pointer_bits
        && lhs.size_bits == rhs.size_bits
        && lhs.long_needs_distinct_int_cases == rhs.long_needs_distinct_int_cases;
};
cmp_fn_neq_default$((Target_ABI)(lhs, rhs));

fn_((Target_self(void))(Target)) {
    return Target_here();
};

cmp_fn_eql$((Target)(lhs, rhs)) {
    return cmp_eql$(Target_Arch)(lhs.arch, rhs.arch)
        && cmp_eql$(Target_Plat)(lhs.plat, rhs.plat)
        && cmp_eql$(Target_ABI)(lhs.abi, rhs.abi);
};
cmp_fn_neq_default$((Target)(lhs, rhs));
