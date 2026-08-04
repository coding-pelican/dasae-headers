#include "dh-main.h"
#include "dh/Cfg.h"

claim_assert_static(abi_char_bits == u8_bits);
claim_assert_static(abi_long_long_bits == u64_bits);
claim_assert_static(abi_pointer_bits == abi_size_bits);
claim_assert_static(sizeOf$(usize) == sizeOf$(P$raw));
claim_assert_static(sizeOf$(S$u8) == (sizeOf$(usize) * 2));
claim_assert_static(sizeOf$(TypeInfoPacked) == sizeOf$(usize));
claim_assert_static(env_is_hosted != env_is_freestanding);
claim_assert_static(arch_byte_order_big_endian < arch_byte_order_little_endian);
claim_assert_static(arch_byte_order_native == arch_byte_order);
claim_assert_static(arch_byte_order_foreign != arch_byte_order_native);
claim_assert_static(
    arch_byte_order_foreign == arch_byte_order_big_endian
        || arch_byte_order_foreign == arch_byte_order_little_endian
);

TEST_fn_("Cfg: runtime value materializes builtin cfg facts" $scope) {
    let cfg = Cfg_here();
    try_(TEST_expect(cfg.comp.kind == as$(Cfg_Comp_Kind)(comp_type)));
    try_(TEST_expect(cfg.env.kind == as$(Cfg_Env_Kind)(env_type)));
    try_(TEST_expect(cfg.lang.kind == as$(Cfg_Lang_Kind)(lang_mode)));
    try_(TEST_expect(cfg.target.arch.kind == as$(Target_Arch_Kind)(arch_type)));
    try_(TEST_expect(cfg.target.arch.byte_order == as$(Target_Arch_ByteOrder)(arch_byte_order)));
    try_(TEST_expect(cfg.target.plat.kind == as$(Target_Plat_Kind)(plat_type)));
    try_(TEST_expect(cfg.target.abi.char_bits == abi_char_bits));
    try_(TEST_expect(cfg.target.abi.long_long_bits == abi_long_long_bits));
    try_(TEST_expect(cfg.target.abi.pointer_bits == cfg.target.abi.size_bits));
} $unscoped(TEST_fn);

TEST_fn_("Cfg: linked DH exposes its own runtime configuration" $scope) {
    try_(TEST_expect(Cfg_eql(Cfg_here(), Cfg_self())));
} $unscoped(TEST_fn);
