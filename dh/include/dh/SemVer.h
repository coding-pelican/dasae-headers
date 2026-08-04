/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SemVer.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-25 (date of creation)
 * @updated 2026-07-25 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  SemVer
 *
 * @brief   Semantic Versioning 2.0 value and precedence rules.
 * @details Represents arbitrary prerelease and build identifier sequences.
 *          Build metadata is retained but never participates in precedence.
 */
#pragma once
#ifndef SemVer__included
#define SemVer__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((SemVer_Core)(struct SemVer_Core {
    var_(major, u64);
    var_(minor, u64);
    var_(patch, u64);
}));
$attr($inline_always)
$static fn_((SemVer_Core_from(u64 major, u64 minor, u64 patch))(SemVer_Core));

$extern cmp_fn_eql$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_neq$((SemVer_Core)(lhs, rhs));

$extern cmp_fn_ord$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_eq$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_ne$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_lt$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_gt$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_le$((SemVer_Core)(lhs, rhs));
$extern cmp_fn_ge$((SemVer_Core)(lhs, rhs));

T_alias$((SemVer_Prerelease)(union SemVer_Prerelease {
    var_(label, S_const$u8);
}));
$attr($inline_always)
$static fn_((SemVer_Prerelease_from(S_const$u8 label))(SemVer_Prerelease));

$extern cmp_fn_eql$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_neq$((SemVer_Prerelease)(lhs, rhs));

$extern cmp_fn_ord$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_eq$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_ne$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_lt$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_gt$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_le$((SemVer_Prerelease)(lhs, rhs));
$extern cmp_fn_ge$((SemVer_Prerelease)(lhs, rhs));

T_alias$((SemVer_Build)(union SemVer_Build {
    var_(label, S_const$u8);
}));
$attr($inline_always)
$static fn_((SemVer_Build_from(S_const$u8 label))(SemVer_Build));

T_alias$((SemVer)(struct SemVer {
    var_(core, SemVer_Core);
    var_(prerelease, SemVer_Prerelease);
    var_(build, SemVer_Build);
}));
$attr($inline_always)
$static fn_((SemVer_from(SemVer_Core core, SemVer_Prerelease prerelease, SemVer_Build build))(SemVer));

$extern cmp_fn_eql$((SemVer)(lhs, rhs));
$extern cmp_fn_neq$((SemVer)(lhs, rhs));

$extern cmp_fn_ord$((SemVer)(lhs, rhs));
$extern cmp_fn_eq$((SemVer)(lhs, rhs));
$extern cmp_fn_ne$((SemVer)(lhs, rhs));
$extern cmp_fn_lt$((SemVer)(lhs, rhs));
$extern cmp_fn_gt$((SemVer)(lhs, rhs));
$extern cmp_fn_le$((SemVer)(lhs, rhs));
$extern cmp_fn_ge$((SemVer)(lhs, rhs));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((SemVer_Core_from(u64 major, u64 minor, u64 patch))(SemVer_Core)) {
    return (SemVer_Core){
        .major = major,
        .minor = minor,
        .patch = patch,
    };
};

fn_((SemVer_Prerelease_from(S_const$u8 label))(SemVer_Prerelease)) {
    return (SemVer_Prerelease){
        .label = label,
    };
};

fn_((SemVer_Build_from(S_const$u8 label))(SemVer_Build)) {
    return (SemVer_Build){
        .label = label,
    };
};

fn_((SemVer_from(SemVer_Core core, SemVer_Prerelease prerelease, SemVer_Build build))(SemVer)) {
    return (SemVer){
        .core = core,
        .prerelease = prerelease,
        .build = build,
    };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SemVer__included */
