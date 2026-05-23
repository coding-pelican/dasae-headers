#include "dh-main.h"
#include "dh/heap/Page.h"

TEST_fn_("heap/Page: header initializer yields a usable instance" $guard) {
    var page = heap_Page_default;
    let gpa = heap_Page_alctr(&page);

    let memory = try_(mem_Alctr_allocBytes($trace gpa, 1));
    defer_(mem_Alctr_freeBytes($trace gpa, memory));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);

TEST_fn_("heap/Page: POSIX shrink remap keeps the existing prefix" $guard) {
    if (plat_is_windows) try_(TEST_skip());

    var page = heap_Page_default;
    let gpa = heap_Page_alctr(&page);

    var memory = try_(mem_Alctr_allocBytes($trace gpa, heap_page_size + 1));
    defer_(mem_Alctr_freeBytes($trace gpa, memory));
    *S_at((memory)[0]) = 0x5a;

    let shrunk = orelse_((mem_Alctr_remapBytes($trace gpa, memory, 1))(return_err(E_cause$Unexpected())));
    memory = shrunk;

    try_(TEST_expect(memory.len == 1));
    try_(TEST_expect(*S_at((memory)[0]) == 0x5a));
} $unguarded(TEST_fn);
