#include "dh/prl/Err.h"
#include "dh/mem/common.h"
#include "dh/io/stream.h"

fn_((E_eql(const EAny* lhs, const EAny* rhs))(bool)) {
    claim_assert_nonnull(lhs), claim_assert_nonnull(rhs);
    if ($branch_likely(E_tag(lhs) != E_tag(rhs))) return false;
    if ($branch_unlikely(E_hashId(lhs) != E_hashId(rhs))) return false;
    if ($branch_unlikely(!mem_eqlBytes(E_strfy(lhs), E_strfy(rhs)))) return false;
    return true;
};
fn_((E_neq(const EAny* lhs, const EAny* rhs))(bool)) { return !E_eql(lhs, rhs); };

fn_((E_print(const EAny* err))(void)) {
    io_stream_eprintln(u8_l("Program failed: {:e}"), *ensureNonnull(err));
};
