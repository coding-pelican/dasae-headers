#include "dh-main.h"
#include "dh/heap/Classic.h"

TEST_fn_("heap/Classic: header initializer yields a usable instance" $guard) {
    var classic = heap_Classic_default;
    let gpa = catch_((heap_Classic_alctr(&classic))(err, switch (E_tag$heap_Classic_E(err)) {
        case_((E_Tag$heap_Classic_Unsupported)) try_(TEST_skip()) $end(case);
        case_((E_Tag$heap_Classic_E_Any)) claim_unreachable $end(case);
    }));

    let memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 1))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);

TEST_fn_("heap/Classic: honors requested alignment" $scope) {
    var classic = heap_Classic_default;
    let gpa = catch_((heap_Classic_alctr(&classic))(err, switch (E_tag$heap_Classic_E(err)) {
        case_((E_Tag$heap_Classic_Unsupported)) try_(TEST_skip()) $end(case);
        case_((E_Tag$heap_Classic_E_Any)) claim_unreachable $end(case);
    }));

    for (usize actual_align = 1; actual_align <= 64; actual_align <<= 1) {
        let log2_align = mem_alignToLog2(actual_align);
        let ptr = orelse_((mem_Alctr_rawAlloc($trace gpa, 1, log2_align))(
            return_err(E_cause$OutOfMemory())
        ));

        try_(TEST_expect(mem_isAligned(ptrToInt(ptr), actual_align)));
        mem_Alctr_rawFree($trace gpa, l$((S$u8){ .ptr = ptr, .len = 1 }), log2_align);
    }
} $unscoped(TEST_fn);

TEST_fn_("heap/Classic: realloc preserves contents and zero fills grown tail" $guard) {
    var classic = heap_Classic_default;
    let gpa = catch_((heap_Classic_alctr(&classic))(err, switch (E_tag$heap_Classic_E(err)) {
        case_((E_Tag$heap_Classic_Unsupported)) try_(TEST_skip()) $end(case);
        case_((E_Tag$heap_Classic_E_Any)) claim_unreachable $end(case);
    }));

    var memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 8))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));

    for_(($rf(0), $s(memory))(idx, item)) { *item = intCast$((u8)(idx + 1)); } $end(for);
    memory = try_(u_castE$((E$S$u8)(mem_Alctr_realloc($trace gpa, u_anyS(memory), 24))));

    try_(TEST_expect(memory.len == 24));
    for_(($rf(0), $s(S_prefix((memory)(8))))(idx, item)) {
        try_(TEST_expect(*item == (idx + 1)));
    } $end(for);
    for_(($rf(8), $s(S_prefix((S_suffix((memory)(8)))(16))))($ignore, item)) {
        try_(TEST_expect(*item == 0));
    } $end(for);
} $unguarded(TEST_fn);

TEST_fn_("heap/Classic: reports shrinkable resize without moving" $guard) {
    var classic = heap_Classic_default;
    let gpa = catch_((heap_Classic_alctr(&classic))(err, switch (E_tag$heap_Classic_E(err)) {
        case_((E_Tag$heap_Classic_Unsupported)) try_(TEST_skip()) $end(case);
        case_((E_Tag$heap_Classic_E_Any)) claim_unreachable $end(case);
    }));

    let memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 16))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));

    try_(TEST_expect(mem_Alctr_resize($trace gpa, u_anyS(memory), 8)));
} $unguarded(TEST_fn);
