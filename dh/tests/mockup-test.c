#include "dh/TEST.h"

use_Opt$(TEST_Result);
use_Err$(Opt$TEST_Result);
typedef fn_((*TEST_CaseFn)(void), Err$Opt$TEST_Result);

typedef struct TEST_Case {
    TEST_CaseFn fn;
    Str_const   name;
} TEST_Case;
use_Sli$(TEST_Case);

typedef struct TEST_Framework {
    mem_Allocator allocator;
    Sli$TEST_Case cases;
    usize         len;
} TEST_Framework;

extern fn_(TEST_instance(), TEST_Framework*);
extern fn_(TEST_bindCase(TEST_CaseFn caseFn, Str_const case_name), void);
extern fn_(TEST_run(), void);


fn_test_scope("test sample function") {

} test_unscoped;


#include "dh/heap/Page.h"
extern fn_(TEST_instance(), TEST_Framework*) {
    static TEST_Framework instance       = cleared();
    static bool           is_initialized = false;
    if (!is_initialized) {
        instance.allocator = heap_Page_allocator(create$(heap_Page));
        instance.cases     = meta_cast$(Sli$TEST_Case,
            catch_(mem_Allocator_alloc(
                instance.allocator,
                typeInfo$(TEST_Case),
                4
            ), claim_unreachable)
        );
        instance.len       = 0;
    }
    return &instance;
}
extern fn_(TEST_bindCase(TEST_CaseFn caseFn, Str_const case_name), void) {
    let instance = TEST_instance();
    if (instance->cases.len <= instance->len) {
        let realloced_cases = meta_cast$(Sli$TEST_Case,
            catch_(mem_Allocator_realloc(
                instance->allocator,
                anySli(instance->cases),
                2 * (instance->cases.len)
            ), claim_unreachable)
        );
        instance->cases     = realloced_cases;
    }
    *Sli_at(instance->cases, instance->len++) = make$(TEST_Case, .fn = caseFn, .name = case_name);
}
