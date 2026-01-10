#include "dh/wtf16.h"
#include "dh/wtf8.h"

fn_((wtf16_iter(S_const$u16 units))(wtf16_Iter) $scope) {
    return_({ .bytes = units, .idx = 0 });
} $unscoped_(fn);

fn_((wtf16_Iter_next(wtf16_Iter* self))(O$u32) $scope) {
    if (self->idx >= self->bytes.len) { return_none(); }
    let first = *S_at((self->bytes)[self->idx]);
    if (utf16_isHighSurrogate(first)) {
        if (self->idx + 1 < self->bytes.len) {
            let second = *S_at((self->bytes)[self->idx + 1]);
            if (utf16_isLowSurrogate(second)) {
                let codepoint = catch_((utf16_decodeSurrogatePair(first, second))($ignore, claim_unreachable));
                self->idx += 2;
                return_some(codepoint);
            }
        }
        self->idx += 1;
        return_some((u32)first);
    } else {
        self->idx += 1;
        return_some((u32)first);
    }
} $unscoped_(fn);
