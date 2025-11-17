// build `clang -xc launcher.c ../src/*.c -o launcher -O3 -static -luser32`
// run with `.\launcher <program_to_run:game_of_life> <width:160> <height:50>`

#include "dh/Str.h"
#include "dh/main.h"
#include "dh/int.h"
#include "dh/Arr.h"
#include "dh/Err.h"
#include "dh/TEST.h"
#include "dh/os/windows.h"
#include <stdio.h>

static let_(Launcher_window_title, S_const$u8) = u8_l("Test Terminal Launcher");
static var_(Terminal_window_title, O$S_const$u8) = none();
static var_(Terminal_window_width, i32) = 80;
static var_(Terminal_window_height, i32) = 25;

static fn_((fmt_parse_u32(S_const$u8 str))(E$u32));
static fn_((fmt_parse_usize(S_const$u8 str))(E$usize));
static fn_((fmt_parse_i32(S_const$u8 str))(E$i32));
static fn_((fmt_parse_isize(S_const$u8 str))(E$isize));

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    if (args.len < 2) {
        printf(
            "[%*s] Usage: %*s <program_to_run> <width> <height>\n",
            as$(i32)(Launcher_window_title.len),
            Launcher_window_title.ptr,
            as$(i32)(at$S(args, 0)->len),
            at$S(args, 0)->ptr
        );
        return_ok({});
    }
    asg$O((&Terminal_window_title)(some(*at$S(args, 1))));
    let window_title = unwrap_(Terminal_window_title);
    printf(
        "[%*s] Terminal: %*s\n",
        as$(i32)(Launcher_window_title.len),
        Launcher_window_title.ptr,
        as$(i32)(window_title.len),
        window_title.ptr
    );

    if (2 < args.len) {
        Terminal_window_width = try_(fmt_parse_u32(*at$S(args, 2)));
        Terminal_window_height = try_(fmt_parse_u32(*at$S(args, 3)));
    }
    debug_assert(0 < Terminal_window_width);
    debug_assert(0 < Terminal_window_height);
    printf(
        "[%*s] Terminal size: %d,%d\n",
        as$(i32)(Launcher_window_title.len),
        Launcher_window_title.ptr,
        Terminal_window_width,
        Terminal_window_height
    );

    // Prepare the command string
    var command = zero$A$$((1024, u8));
    let_ignore = snprintf(
        as$(char*)(command.val), len$A(command), "wt --size %d,%d -d . cmd /k .\\%*s %d %d", Terminal_window_width, Terminal_window_height,
        as$(i32)(window_title.len), window_title.ptr, Terminal_window_width, Terminal_window_height);

    // Create process
    STARTUPINFO startup_info = {};
    PROCESS_INFORMATION process_info = {};
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    ZeroMemory(&process_info, sizeof(process_info));

    // Create process
    if (!CreateProcessA(
            null,
            as$(LPSTR)(command.val),
            null,
            null,
            false,
            0,
            null,
            null,
            &startup_info,
            &process_info
        )) {
        printf(
            "[%*s] CreateProcess failed (%d).\n",
            as$(i32)(Launcher_window_title.len),
            Launcher_window_title.ptr,
            as$(i32)(GetLastError())
        );
        return_err(Err_Unexpected());
    }

    // Wait for the process to finish
    WaitForSingleObject(process_info.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    printf(
        "[%*s] Windows Terminal executed successfully.\n",
        as$(i32)(Launcher_window_title.len),
        Launcher_window_title.ptr
    );
    return_ok({});
} $unscoped_(fn);

errset_((fmt_ParseIntErr)(
    EmptyStr,
    InvalidChar,
    InvalidFormat,
    Overflow
));
fn_((fmt_parse_u32(S_const$u8 str))(E$u32) $scope) {
    debug_assert_nonnull(str.ptr);
    if (str.len == 0) { return_err(fmt_ParseIntErr_EmptyStr()); }
    return_ok(blk({
        u32 parsed = 0;
        for_(($s(str))(ch) {
            let c = *ch;
            if (c < '0' || '9' < c) {
                return_err(fmt_ParseIntErr_InvalidChar());
            }
            let digit = c - '0';
            parsed = orelse_((u32_mulChkd(parsed, 10))(return_err(fmt_ParseIntErr_Overflow())));
            parsed += digit;
        });
        blk_return_(parsed);
    }));
} $unscoped_(fn);
fn_((fmt_parse_usize(S_const$u8 str))(E$usize) $scope) {
    debug_assert_nonnull(str.ptr);
    if (str.len == 0) { return_err(fmt_ParseIntErr_EmptyStr()); }
    return_ok(blk({
        usize parsed = 0;
        for_(($s(str))(ch) {
            let c = *ch;
            if (c < '0' || '9' < c) {
                return_err(fmt_ParseIntErr_InvalidChar());
            }
            let digit = c - '0';
            parsed = orelse_((usize_mulChkd(parsed, 10))(return_err(fmt_ParseIntErr_Overflow())));
            parsed += digit;
        });
        blk_return_(parsed);
    }));
} $unscoped_(fn);
fn_((fmt_parse_i32(S_const$u8 str))(E$i32) $scope) {
    debug_assert_nonnull(str.ptr);
    if (str.len == 0) { return_err(fmt_ParseIntErr_EmptyStr()); }
    return_ok(blk({
        bool negative = false;
        usize start_idx = 0;

        if (*at$S(str, 0) == '-') {
            negative = true;
            start_idx = 1;
            if (str.len == 1) {
                return_err(fmt_ParseIntErr_InvalidFormat());
            }

            // Special case for INT32_MIN (-2147483648)
            if (str.len == 11 && Str_eql(u8_l("2147483648"), slice$S(str, $r(1, 11)))) {
                return_ok(-2147483648); // INT32_MIN
            }
        } else if (*at$S(str, 0) == '+') {
            start_idx = 1;
            if (str.len == 1) {
                return_err(fmt_ParseIntErr_InvalidFormat());
            }
        }

        i32 parsed = 0;
        for_(($s(str))(ch) {
            let c = *ch;
            if (c < '0' || '9' < c) {
                return_err(fmt_ParseIntErr_InvalidChar());
            }
            let digit = c - '0';

            // Check for overflow explicitly before multiplication
            if (parsed > 214748364 || (parsed == 214748364 && digit > 7)) {
                return_err(fmt_ParseIntErr_Overflow());
            }

            parsed = parsed * 10 + digit;
        });
        if (negative) {
            parsed = -parsed;
        }
        blk_return_(parsed);
    }));
} $unscoped_(fn);
fn_((fmt_parse_isize(S_const$u8 str))(E$isize) $scope) {
    debug_assert_nonnull(str.ptr);
    if (str.len == 0) { return_err(fmt_ParseIntErr_EmptyStr()); }
    return_ok(blk({
        bool negative = false;
        usize start_idx = 0;

        if (*at$S(str, 0) == '-') {
            negative = true;
            start_idx = 1;
            if (str.len == 1) {
                return_err(fmt_ParseIntErr_InvalidFormat());
            }
        } else if (*at$S(str, 0) == '+') {
            start_idx = 1;
            if (str.len == 1) {
                return_err(fmt_ParseIntErr_InvalidFormat());
            }
        }

        isize parsed = 0;
        for_(($s(str))(ch) {
            let c = *ch;
            if (c < '0' || '9' < c) {
                return_err(fmt_ParseIntErr_InvalidChar());
            }
            let digit = c - '0';
            parsed = orelse_((isize_mulChkd(parsed, 10))(return_err(fmt_ParseIntErr_Overflow())));
            parsed += digit;
        });
        if (negative) {
            parsed = -parsed;
        }
        blk_return_(parsed);
    }));
} $unscoped_(fn);

TEST_fn_("fmt_parse_u32" $scope) {
    // Valid inputs
    let ok_val = try_(fmt_parse_u32(u8_l("123")));
    try_(TEST_expect(ok_val == 123));

    // Zero
    let zero = try_(fmt_parse_u32(u8_l("0")));
    try_(TEST_expect(zero == 0));

    // Large number
    let large = try_(fmt_parse_u32(u8_l("4294967295"))); // Max u32
    try_(TEST_expect(large == 4294967295));

    // Empty string
    let empty_err = fmt_parse_u32(u8_l(""));
    try_(TEST_expect(isErr(empty_err)));
    if_err(empty_err, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("EmptyStr"), Err_codeToStr(err)),
            u8_l("Expected EmptyStr error")
        ));
    }

    // Invalid character
    let invalid_char = fmt_parse_u32(u8_l("12a3"));
    try_(TEST_expect(isErr(invalid_char)));
    if_err(invalid_char, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidChar"), Err_codeToStr(err)),
            u8_l("Expected InvalidChar error")
        ));
    }

    // Overflow
    let overflow = fmt_parse_u32(u8_l("42949672951")); // Exceeds max u32
    try_(TEST_expect(isErr(overflow)));
    if_err(overflow, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("Overflow"), Err_codeToStr(err)),
            u8_l("Expected Overflow error")
        ));
    }
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse_usize" $scope) {
    // Valid inputs
    let ok_val = try_(fmt_parse_usize(u8_l("123")));
    try_(TEST_expect(ok_val == 123));

    // Zero
    let zero = try_(fmt_parse_usize(u8_l("0")));
    try_(TEST_expect(zero == 0));

    // Large number
    let large = try_(fmt_parse_usize(u8_l("65535")));
    try_(TEST_expect(large == 65535));

    // Empty string
    let empty_err = fmt_parse_usize(u8_l(""));
    try_(TEST_expect(isErr(empty_err)));
    if_err(empty_err, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("EmptyStr"), Err_codeToStr(err)),
            u8_l("Expected EmptyStr error")
        ));
    }

    // Invalid character
    let invalid_char = fmt_parse_usize(u8_l("12a3"));
    try_(TEST_expect(isErr(invalid_char)));
    if_err(invalid_char, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidChar"), Err_codeToStr(err)),
            u8_l("Expected InvalidChar error")
        ));
    }

    // Overflow (test with very large number)
    let overflow = fmt_parse_usize(u8_l("184467440737095516151")); // Very large
    try_(TEST_expect(isErr(overflow)));
    if_err(overflow, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("Overflow"), Err_codeToStr(err)),
            u8_l("Expected Overflow error")
        ));
    }
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse_i32" $scope) {
    // Valid positive
    let pos_val = try_(fmt_parse_i32(u8_l("123")));
    try_(TEST_expect(pos_val == 123));

    // Valid negative
    let neg_val = try_(fmt_parse_i32(u8_l("-123")));
    try_(TEST_expect(neg_val == -123));

    // Explicit positive
    let explicit_pos = try_(fmt_parse_i32(u8_l("+123")));
    try_(TEST_expect(explicit_pos == 123));

    // Zero
    let zero = try_(fmt_parse_i32(u8_l("0")));
    try_(TEST_expect(zero == 0));

    // Min/Max values
    let max_i32 = try_(fmt_parse_i32(u8_l("2147483647"))); // Max i32
    try_(TEST_expect(max_i32 == 2147483647));

    let min_i32 = try_(fmt_parse_i32(u8_l("-2147483648"))); // Min i32
    try_(TEST_expect(min_i32 == -2147483648));

    // Empty string
    let empty_err = fmt_parse_i32(u8_l(""));
    try_(TEST_expect(isErr(empty_err)));
    if_err(empty_err, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("EmptyStr"), Err_codeToStr(err)),
            u8_l("Expected EmptyStr error")
        ));
    }

    // Invalid character
    let invalid_char = fmt_parse_i32(u8_l("12a3"));
    try_(TEST_expect(isErr(invalid_char)));
    if_err(invalid_char, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidChar"), Err_codeToStr(err)),
            u8_l("Expected InvalidChar error")
        ));
    }

    // Invalid format (just sign)
    let invalid_format1 = fmt_parse_i32(u8_l("-"));
    try_(TEST_expect(isErr(invalid_format1)));
    if_err(invalid_format1, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidFormat"), Err_codeToStr(err)),
            u8_l("Expected InvalidFormat error")
        ));
    }

    let invalid_format2 = fmt_parse_i32(u8_l("+"));
    try_(TEST_expect(isErr(invalid_format2)));
    if_err(invalid_format2, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidFormat"), Err_codeToStr(err)),
            u8_l("Expected InvalidFormat error")
        ));
    }

    // Overflow
    let overflow_pos = fmt_parse_i32(u8_l("2147483648")); // Exceeds max i32
    try_(TEST_expect(isErr(overflow_pos)));
    if_err(overflow_pos, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("Overflow"), Err_codeToStr(err)),
            u8_l("Expected Overflow error")
        ));
    }

    let overflow_neg = fmt_parse_i32(u8_l("-2147483649")); // Exceeds min i32
    try_(TEST_expect(isErr(overflow_neg)));
    if_err(overflow_neg, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("Overflow"), Err_codeToStr(err)),
            u8_l("Expected Overflow error")
        ));
    }
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse_isize" $scope) {
    // Valid positive
    let pos_val = try_(fmt_parse_isize(u8_l("123")));
    try_(TEST_expect(pos_val == 123));

    // Valid negative
    let neg_val = try_(fmt_parse_isize(u8_l("-123")));
    try_(TEST_expect(neg_val == -123));

    // Explicit positive
    let explicit_pos = try_(fmt_parse_isize(u8_l("+123")));
    try_(TEST_expect(explicit_pos == 123));

    // Zero
    let zero = try_(fmt_parse_isize(u8_l("0")));
    try_(TEST_expect(zero == 0));

    // Large values (adjust based on platform if needed)
    let large_pos = try_(fmt_parse_isize(u8_l("9223372036854775807"))); // Typical max for 64-bit
    try_(TEST_expect(large_pos == 9223372036854775807));

    // Empty string
    let empty_err = fmt_parse_isize(u8_l(""));
    try_(TEST_expect(isErr(empty_err)));
    if_err(empty_err, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("EmptyStr"), Err_codeToStr(err)),
            u8_l("Expected EmptyStr error")
        ));
    }

    // Invalid character
    let invalid_char = fmt_parse_isize(u8_l("12a3"));
    try_(TEST_expect(isErr(invalid_char)));
    if_err(invalid_char, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidChar"), Err_codeToStr(err)),
            u8_l("Expected InvalidChar error")
        ));
    }

    // Invalid format (just sign)
    let invalid_format1 = fmt_parse_isize(u8_l("-"));
    try_(TEST_expect(isErr(invalid_format1)));
    if_err(invalid_format1, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidFormat"), Err_codeToStr(err)),
            u8_l("Expected InvalidFormat error")
        ));
    }

    let invalid_format2 = fmt_parse_isize(u8_l("+"));
    try_(TEST_expect(isErr(invalid_format2)));
    if_err(invalid_format2, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("InvalidFormat"), Err_codeToStr(err)),
            u8_l("Expected InvalidFormat error")
        ));
    }

    // Overflow (test with very large number)
    let overflow = fmt_parse_isize(u8_l("19223372036854775808")); // Exceeds max isize
    try_(TEST_expect(isErr(overflow)));
    if_err(overflow, err) {
        try_(TEST_expectMsg(
            Str_eql(u8_l("Overflow"), Err_codeToStr(err)),
            u8_l("Expected Overflow error")
        ));
    }
} $unscoped_(TEST_fn);
