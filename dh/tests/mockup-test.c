#include "dh/TEST.h"

use_Opt$(TEST_Result);
use_Err$(Opt$TEST_Result);
typedef fn_((*TEST_CaseFn)(void), Err$Opt$TEST_Result);
use_Sli$(TEST_CaseFn);
typedef struct TEST_Framework {
    mem_Allocator   allocator;
    Sli$TEST_CaseFn cases;
    usize           len;
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
        instance.allocator = heap_Page_allocator(&(heap_Page){});
        instance.cases     = meta_cast$(Sli$TEST_CaseFn,
            catch_(mem_Allocator_alloc(
                instance.allocator,
                typeInfo$(TEST_CaseFn),
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
        let realloced_cases = meta_cast$(Sli$TEST_CaseFn,
            catch_(mem_Allocator_alloc(
                instance->allocator,
                typeInfo$(TEST_CaseFn),
                2 * (instance->cases.len)
            ), claim_unreachable)
        );
        memcpy(
            as$(u8*, realloced_cases.ptr),
            as$(u8*, instance->cases.ptr),
            sizeOf(TEST_CaseFn) * instance->cases.len
        );
        mem_Allocator_free(instance->allocator, anySli(instance->cases));
        instance->cases = realloced_cases;
    }
    *Sli_at(instance->cases, instance->len++) = caseFn;
}
