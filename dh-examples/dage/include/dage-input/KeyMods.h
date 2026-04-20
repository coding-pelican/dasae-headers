#ifndef dage_input_KeyMods__included
#define dage_input_KeyMods__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage-base.h"

/*========== Macros and Declarations ========================================*/

/// @brief Key modifier flags
T_alias$((dage_KeyMods)(union dage_KeyMods {
    T_embed$(struct {
        var_(shift, u8)     : 1;
        var_(ctrl, u8)      : 1;
        var_(alt, u8)       : 1;
        var_(caps_lock, u8) : 1;
        var_(num_lock, u8)  : 1;
        var_(reserved_, u8) : 3;
    });
    var_(packed, u8);
}));
claim_assert_static(TypeInfoPacked_eql(
    packTypeInfo$(dage_KeyMods),
    packTypeInfo$(u8)
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_input_KeyMods__included */
