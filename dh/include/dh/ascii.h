/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ascii.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-20 (date of creation)
 * @updated 2026-06-12 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  ascii
 */
#pragma once
#ifndef ascii__included
#define ascii__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

/// The C0 control codes of the ASCII encoding.
T_alias$((ascii_ctrl_Code)(enum_((ascii_ctrl_Code $fits($packed))(
    ascii_ctrl_Code_nul = 0x00, ///< Null ('\0').
    ascii_ctrl_Code_soh = 0x01, ///< Start of Heading.
    ascii_ctrl_Code_stx = 0x02, ///< Start of Text.
    ascii_ctrl_Code_etx = 0x03, ///< End of Text.
    ascii_ctrl_Code_eot = 0x04, ///< End of Transmission.
    ascii_ctrl_Code_enq = 0x05, ///< Enquiry.
    ascii_ctrl_Code_ack = 0x06, ///< Acknowledge.
    ascii_ctrl_Code_bel = 0x07, ///< Bell, Alert ('\a').
    ascii_ctrl_Code_bs = 0x08, ///< Backspace ('\b').
    ascii_ctrl_Code_ht = 0x09, ///< Horizontal Tab, Tab ('\t').
    ascii_ctrl_Code_lf = 0x0A, ///< Line Feed, Newline ('\n').
    ascii_ctrl_Code_vt = 0x0B, ///< Vertical Tab ('\v').
    ascii_ctrl_Code_ff = 0x0C, ///< Form Feed ('\f').
    ascii_ctrl_Code_cr = 0x0D, ///< Carriage Return ('\r').
    ascii_ctrl_Code_so = 0x0E, ///< Shift Out.
    ascii_ctrl_Code_si = 0x0F, ///< Shift In.
    ascii_ctrl_Code_dle = 0x10, ///< Data Link Escape.
    ascii_ctrl_Code_dc1 = 0x11, ///< Device Control One (XON, `ascii_ctrl_Code_xon`).
    ascii_ctrl_Code_dc2 = 0x12, ///< Device Control Two.
    ascii_ctrl_Code_dc3 = 0x13, ///< Device Control Three (XOFF, `ascii_ctrl_Code_xoff`).
    ascii_ctrl_Code_dc4 = 0x14, ///< Device Control Four.
    ascii_ctrl_Code_nak = 0x15, ///< Negative Acknowledge.
    ascii_ctrl_Code_syn = 0x16, ///< Synchronous Idle.
    ascii_ctrl_Code_etb = 0x17, ///< End of Transmission Block
    ascii_ctrl_Code_can = 0x18, ///< Cancel.
    ascii_ctrl_Code_em = 0x19, ///< End of Medium.
    ascii_ctrl_Code_sub = 0x1A, ///< Substitute.
    ascii_ctrl_Code_esc = 0x1B, ///< Escape.
    ascii_ctrl_Code_fs = 0x1C, ///< File Separator.
    ascii_ctrl_Code_gs = 0x1D, ///< Group Separator.
    ascii_ctrl_Code_rs = 0x1E, ///< Record Separator.
    ascii_ctrl_Code_us = 0x1F, ///< Unit Separator.

    ascii_ctrl_Code_del = 0x7F, /// Delete.

    ascii_ctrl_Code_xon = ascii_ctrl_Code_dc1, /// An alias to `dc1`.
    ascii_ctrl_Code_xoff = ascii_ctrl_Code_dc3, /// An alias to `dc3`.
))));
claim_assert_static(eqlType$(ascii_ctrl_Code, u8));

#define ascii_nul __str__ascii_nul
#define ascii_nul_byte __uint__ascii_nul_byte
#define ascii_bel __str__ascii_bel
#define ascii_bel_byte __uint__ascii_bel_byte
#define ascii_bs __str__ascii_bs
#define ascii_bs_byte __uint__ascii_bs_byte
#define ascii_ht __str__ascii_ht
#define ascii_ht_byte __uint__ascii_ht_byte
#define ascii_lf __str__ascii_lf
#define ascii_lf_byte __uint__ascii_lf_byte
#define ascii_vt __str__ascii_vt
#define ascii_vt_byte __uint__ascii_vt_byte
#define ascii_ff __str__ascii_ff
#define ascii_ff_byte __uint__ascii_ff_byte
#define ascii_cr __str__ascii_cr
#define ascii_cr_byte __uint__ascii_cr_byte
#define ascii_sp __str__ascii_sp
#define ascii_sp_byte __uint__ascii_sp_byte

#define ascii_digits __str__ascii_digits
#define ascii_digit_0 __str__ascii_digit_0
#define ascii_digit_0_byte __uint__ascii_digit_0_byte
#define ascii_digit_1 __str__ascii_digit_1
#define ascii_digit_1_byte __uint__ascii_digit_1_byte
#define ascii_digit_2 __str__ascii_digit_2
#define ascii_digit_2_byte __uint__ascii_digit_2_byte
#define ascii_digit_3 __str__ascii_digit_3
#define ascii_digit_3_byte __uint__ascii_digit_3_byte
#define ascii_digit_4 __str__ascii_digit_4
#define ascii_digit_4_byte __uint__ascii_digit_4_byte
#define ascii_digit_5 __str__ascii_digit_5
#define ascii_digit_5_byte __uint__ascii_digit_5_byte
#define ascii_digit_6 __str__ascii_digit_6
#define ascii_digit_6_byte __uint__ascii_digit_6_byte
#define ascii_digit_7 __str__ascii_digit_7
#define ascii_digit_7_byte __uint__ascii_digit_7_byte
#define ascii_digit_8 __str__ascii_digit_8
#define ascii_digit_8_byte __uint__ascii_digit_8_byte
#define ascii_digit_9 __str__ascii_digit_9
#define ascii_digit_9_byte __uint__ascii_digit_9_byte

#define ascii_letters __str__ascii_letters

#define ascii_letter_lowers __str__ascii_letter_lowers
#define ascii_letter_lower_a __str__ascii_letter_lower_a
#define ascii_letter_lower_a_byte __uint__ascii_letter_lower_a_byte
#define ascii_letter_lower_b __str__ascii_letter_lower_b
#define ascii_letter_lower_b_byte __uint__ascii_letter_lower_b_byte
#define ascii_letter_lower_c __str__ascii_letter_lower_c
#define ascii_letter_lower_c_byte __uint__ascii_letter_lower_c_byte
#define ascii_letter_lower_d __str__ascii_letter_lower_d
#define ascii_letter_lower_d_byte __uint__ascii_letter_lower_d_byte
#define ascii_letter_lower_e __str__ascii_letter_lower_e
#define ascii_letter_lower_e_byte __uint__ascii_letter_lower_e_byte
#define ascii_letter_lower_f __str__ascii_letter_lower_f
#define ascii_letter_lower_f_byte __uint__ascii_letter_lower_f_byte
#define ascii_letter_lower_g __str__ascii_letter_lower_g
#define ascii_letter_lower_g_byte __uint__ascii_letter_lower_g_byte
#define ascii_letter_lower_h __str__ascii_letter_lower_h
#define ascii_letter_lower_h_byte __uint__ascii_letter_lower_h_byte
#define ascii_letter_lower_i __str__ascii_letter_lower_i
#define ascii_letter_lower_i_byte __uint__ascii_letter_lower_i_byte
#define ascii_letter_lower_j __str__ascii_letter_lower_j
#define ascii_letter_lower_j_byte __uint__ascii_letter_lower_j_byte
#define ascii_letter_lower_k __str__ascii_letter_lower_k
#define ascii_letter_lower_k_byte __uint__ascii_letter_lower_k_byte
#define ascii_letter_lower_l __str__ascii_letter_lower_l
#define ascii_letter_lower_l_byte __uint__ascii_letter_lower_l_byte
#define ascii_letter_lower_m __str__ascii_letter_lower_m
#define ascii_letter_lower_m_byte __uint__ascii_letter_lower_m_byte
#define ascii_letter_lower_n __str__ascii_letter_lower_n
#define ascii_letter_lower_n_byte __uint__ascii_letter_lower_n_byte
#define ascii_letter_lower_o __str__ascii_letter_lower_o
#define ascii_letter_lower_o_byte __uint__ascii_letter_lower_o_byte
#define ascii_letter_lower_p __str__ascii_letter_lower_p
#define ascii_letter_lower_p_byte __uint__ascii_letter_lower_p_byte
#define ascii_letter_lower_q __str__ascii_letter_lower_q
#define ascii_letter_lower_q_byte __uint__ascii_letter_lower_q_byte
#define ascii_letter_lower_r __str__ascii_letter_lower_r
#define ascii_letter_lower_r_byte __uint__ascii_letter_lower_r_byte
#define ascii_letter_lower_s __str__ascii_letter_lower_s
#define ascii_letter_lower_s_byte __uint__ascii_letter_lower_s_byte
#define ascii_letter_lower_t __str__ascii_letter_lower_t
#define ascii_letter_lower_t_byte __uint__ascii_letter_lower_t_byte
#define ascii_letter_lower_u __str__ascii_letter_lower_u
#define ascii_letter_lower_u_byte __uint__ascii_letter_lower_u_byte
#define ascii_letter_lower_v __str__ascii_letter_lower_v
#define ascii_letter_lower_v_byte __uint__ascii_letter_lower_v_byte
#define ascii_letter_lower_w __str__ascii_letter_lower_w
#define ascii_letter_lower_w_byte __uint__ascii_letter_lower_w_byte
#define ascii_letter_lower_x __str__ascii_letter_lower_x
#define ascii_letter_lower_x_byte __uint__ascii_letter_lower_x_byte
#define ascii_letter_lower_y __str__ascii_letter_lower_y
#define ascii_letter_lower_y_byte __uint__ascii_letter_lower_y_byte
#define ascii_letter_lower_z __str__ascii_letter_lower_z
#define ascii_letter_lower_z_byte __uint__ascii_letter_lower_z_byte

#define ascii_letter_uppers __str__ascii_letter_uppers
#define ascii_letter_upper_A __str__ascii_letter_upper_A
#define ascii_letter_upper_A_byte __uint__ascii_letter_upper_A_byte
#define ascii_letter_upper_B __str__ascii_letter_upper_B
#define ascii_letter_upper_B_byte __uint__ascii_letter_upper_B_byte
#define ascii_letter_upper_C __str__ascii_letter_upper_C
#define ascii_letter_upper_C_byte __uint__ascii_letter_upper_C_byte
#define ascii_letter_upper_D __str__ascii_letter_upper_D
#define ascii_letter_upper_D_byte __uint__ascii_letter_upper_D_byte
#define ascii_letter_upper_E __str__ascii_letter_upper_E
#define ascii_letter_upper_E_byte __uint__ascii_letter_upper_E_byte
#define ascii_letter_upper_F __str__ascii_letter_upper_F
#define ascii_letter_upper_F_byte __uint__ascii_letter_upper_F_byte
#define ascii_letter_upper_G __str__ascii_letter_upper_G
#define ascii_letter_upper_G_byte __uint__ascii_letter_upper_G_byte
#define ascii_letter_upper_H __str__ascii_letter_upper_H
#define ascii_letter_upper_H_byte __uint__ascii_letter_upper_H_byte
#define ascii_letter_upper_I __str__ascii_letter_upper_I
#define ascii_letter_upper_I_byte __uint__ascii_letter_upper_I_byte
#define ascii_letter_upper_J __str__ascii_letter_upper_J
#define ascii_letter_upper_J_byte __uint__ascii_letter_upper_J_byte
#define ascii_letter_upper_K __str__ascii_letter_upper_K
#define ascii_letter_upper_K_byte __uint__ascii_letter_upper_K_byte
#define ascii_letter_upper_L __str__ascii_letter_upper_L
#define ascii_letter_upper_L_byte __uint__ascii_letter_upper_L_byte
#define ascii_letter_upper_M __str__ascii_letter_upper_M
#define ascii_letter_upper_M_byte __uint__ascii_letter_upper_M_byte
#define ascii_letter_upper_N __str__ascii_letter_upper_N
#define ascii_letter_upper_N_byte __uint__ascii_letter_upper_N_byte
#define ascii_letter_upper_O __str__ascii_letter_upper_O
#define ascii_letter_upper_O_byte __uint__ascii_letter_upper_O_byte
#define ascii_letter_upper_P __str__ascii_letter_upper_P
#define ascii_letter_upper_P_byte __uint__ascii_letter_upper_P_byte
#define ascii_letter_upper_Q __str__ascii_letter_upper_Q
#define ascii_letter_upper_Q_byte __uint__ascii_letter_upper_Q_byte
#define ascii_letter_upper_R __str__ascii_letter_upper_R
#define ascii_letter_upper_R_byte __uint__ascii_letter_upper_R_byte
#define ascii_letter_upper_S __str__ascii_letter_upper_S
#define ascii_letter_upper_S_byte __uint__ascii_letter_upper_S_byte
#define ascii_letter_upper_T __str__ascii_letter_upper_T
#define ascii_letter_upper_T_byte __uint__ascii_letter_upper_T_byte
#define ascii_letter_upper_U __str__ascii_letter_upper_U
#define ascii_letter_upper_U_byte __uint__ascii_letter_upper_U_byte
#define ascii_letter_upper_V __str__ascii_letter_upper_V
#define ascii_letter_upper_V_byte __uint__ascii_letter_upper_V_byte
#define ascii_letter_upper_W __str__ascii_letter_upper_W
#define ascii_letter_upper_W_byte __uint__ascii_letter_upper_W_byte
#define ascii_letter_upper_X __str__ascii_letter_upper_X
#define ascii_letter_upper_X_byte __uint__ascii_letter_upper_X_byte
#define ascii_letter_upper_Y __str__ascii_letter_upper_Y
#define ascii_letter_upper_Y_byte __uint__ascii_letter_upper_Y_byte
#define ascii_letter_upper_Z __str__ascii_letter_upper_Z
#define ascii_letter_upper_Z_byte __uint__ascii_letter_upper_Z_byte

/// Returns whether the character is a 7-bit ASCII character.
$attr($inline_always)
$static fn_((ascii_isASCII(u8 c))(bool));
/// Returns whether the character is an uppercase letter: A-Z.
$attr($inline_always)
$static fn_((ascii_isUpper(u8 c))(bool));
/// Returns whether the character is a lowercase letter: a-z.
$attr($inline_always)
$static fn_((ascii_isLower(u8 c))(bool));
/// Returns whether the character is alphabetic: A-Z || a-z.
$attr($inline_always)
$static fn_((ascii_isAlpha(u8 c))(bool));
/// Returns whether the character is a digit: 0-9.
$attr($inline_always)
$static fn_((ascii_isDigit(u8 c))(bool));
/// Returns whether the character is alphanumeric: A-Z, a-z, || 0-9.
$attr($inline_always)
$static fn_((ascii_isAlNum(u8 c))(bool));
/// Returns whether the character is a hexadecimal digit: A-F, a-f, || 0-9.
$attr($inline_always)
$static fn_((ascii_isHex(u8 c))(bool));
/// Returns whether the character is a control character.
$attr($inline_always)
$static fn_((ascii_isCtrl(u8 c))(bool));
/// Returns whether the character is a glyph.
$attr($inline_always)
$static fn_((ascii_isGlyph(u8 c))(bool));
/// Returns whether the character is a whitespace character.
$attr($inline_always)
$static fn_((ascii_isWhitespace(u8 c))(bool));
#define ascii_whitespaces \
    /** \
     * Whitespace for general use. \
     * This may be used with e.g. `mem_trim` to trim whitespace. \
     */ \
    __str__ascii_whitespaces
#define ascii_whitespace_sp __str__ascii_whitespace_sp
#define ascii_whitespace_sp_byte __uint__ascii_whitespace_sp_byte
#define ascii_whitespace_ht __str__ascii_whitespace_ht
#define ascii_whitespace_ht_byte __uint__ascii_whitespace_ht_byte
#define ascii_whitespace_lf __str__ascii_whitespace_lf
#define ascii_whitespace_lf_byte __uint__ascii_whitespace_lf_byte
#define ascii_whitespace_cr __str__ascii_whitespace_cr
#define ascii_whitespace_cr_byte __uint__ascii_whitespace_cr_byte
#define ascii_whitespace_vt __str__ascii_whitespace_vt
#define ascii_whitespace_vt_byte __uint__ascii_whitespace_vt_byte
#define ascii_whitespace_ff __str__ascii_whitespace_ff
#define ascii_whitespace_ff_byte __uint__ascii_whitespace_ff_byte

/// Uppercases the character && returns it as-is if already uppercase || not a letter.
$attr($inline_always)
$static fn_((ascii_toUpper(u8 c))(u8));
/// Lowercases the character && returns it as-is if already lowercase || not a letter.
$attr($inline_always)
$static fn_((ascii_toLower(u8 c))(u8));
/// Toggles the case of the character && returns it as-is if not a letter.
$attr($inline_always)
$static fn_((ascii_toggleCase(u8 c))(u8));

/// Converts the string to uppercase.
$extern fn_((ascii_toUppers(S$u8 ascii_str))(S$u8));
/// Converts the string to lowercase.
$extern fn_((ascii_toLowers(S$u8 ascii_str))(S$u8));
/// Converts the string to toggle case.
$extern fn_((ascii_toggleCases(S$u8 ascii_str))(S$u8));

/// Allocates an upper case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocUppers(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));
/// Allocates a lower case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocLowers(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));
/// Allocates a toggled case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocToggledCases(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));

/// Writes an upper case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeUppers(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));
/// Writes a lower case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeLowers(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));
/// Writes a toggled case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeToggledCases(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));

$attr($inline_always)
$static fn_((ascii_digitToInt(u8 c))(u8));
$static let ascii_intFromDigit = ascii_digitToInt;
$attr($inline_always)
$static fn_((ascii_intToDigit(u8 val))(u8));
$static let ascii_digitFromInt = ascii_intToDigit;

/// Returns the index of the first occurrence of `ascii_substr` in `ascii_str`, ignoring case.
$extern fn_((ascii_idxOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr))(O$usize));
/// Returns the index of the first occurrence of `ascii_substr` starting from `start_front`, ignoring case.
$extern fn_((ascii_idxFirstOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_front))(O$usize));
/// Returns the index of the last occurrence of `ascii_substr` starting from `start_back`, ignoring case.
$extern fn_((ascii_idxLastOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_back))(O$usize));

/// Returns whether `ascii_str` starts with `ascii_prefix`, ignoring case.
$extern fn_((ascii_startsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_prefix))(bool));
/// Returns whether `ascii_str` ends with `ascii_suffix`, ignoring case.
$extern fn_((ascii_endsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_suffix))(bool));

/// Equality comparison.
$extern fn_((ascii_eql(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(bool));
/// Equality comparison, respecting case.
$extern fn_((ascii_eqlSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));
/// Equality comparison, ignoring case.
$extern fn_((ascii_eqlIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));

/// Compares lexicographical order of two ASCII strings.
$extern fn_((ascii_ord(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, respecting case.
$extern fn_((ascii_ordSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, ignoring case.
$extern fn_((ascii_ordIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));

/*========== Macros and Definitions =========================================*/

#define __str__ascii_nul "\0"
#define __uint__ascii_nul_byte u8_c('\0')
#define __str__ascii_bel "\a"
#define __uint__ascii_bel_byte u8_c('\a')
#define __str__ascii_bs "\b"
#define __uint__ascii_bs_byte u8_c('\b')
#define __str__ascii_ht "\t"
#define __uint__ascii_ht_byte u8_c('\t')
#define __str__ascii_lf "\n"
#define __uint__ascii_lf_byte u8_c('\n')
#define __str__ascii_vt "\v"
#define __uint__ascii_vt_byte u8_c('\v')
#define __str__ascii_ff "\f"
#define __uint__ascii_ff_byte u8_c('\f')
#define __str__ascii_cr "\r"
#define __uint__ascii_cr_byte u8_c('\r')
#define __str__ascii_sp " "
#define __uint__ascii_sp_byte u8_c(' ')

#define __str__ascii_digits /* clang-format off */ \
    ascii_digit_0 \
    ascii_digit_1 \
    ascii_digit_2 \
    ascii_digit_3 \
    ascii_digit_4 \
    ascii_digit_5 \
    ascii_digit_6 \
    ascii_digit_7 \
    ascii_digit_8 \
    ascii_digit_9 /* clang-format on */
#define __str__ascii_digit_0 "0"
#define __uint__ascii_digit_0_byte u8_c('0')
#define __str__ascii_digit_1 "1"
#define __uint__ascii_digit_1_byte u8_c('1')
#define __str__ascii_digit_2 "2"
#define __uint__ascii_digit_2_byte u8_c('2')
#define __str__ascii_digit_3 "3"
#define __uint__ascii_digit_3_byte u8_c('3')
#define __str__ascii_digit_4 "4"
#define __uint__ascii_digit_4_byte u8_c('4')
#define __str__ascii_digit_5 "5"
#define __uint__ascii_digit_5_byte u8_c('5')
#define __str__ascii_digit_6 "6"
#define __uint__ascii_digit_6_byte u8_c('6')
#define __str__ascii_digit_7 "7"
#define __uint__ascii_digit_7_byte u8_c('7')
#define __str__ascii_digit_8 "8"
#define __uint__ascii_digit_8_byte u8_c('8')
#define __str__ascii_digit_9 "9"
#define __uint__ascii_digit_9_byte u8_c('9')

#define __str__ascii_letters \
    ascii_letter_lowers ascii_letter_uppers

#define __str__ascii_letter_lowers /* clang-format off */ \
    ascii_letter_lower_a \
    ascii_letter_lower_b \
    ascii_letter_lower_c \
    ascii_letter_lower_d \
    ascii_letter_lower_e \
    ascii_letter_lower_f \
    ascii_letter_lower_g \
    ascii_letter_lower_h \
    ascii_letter_lower_i \
    ascii_letter_lower_j \
    ascii_letter_lower_k \
    ascii_letter_lower_l \
    ascii_letter_lower_m \
    ascii_letter_lower_n \
    ascii_letter_lower_o \
    ascii_letter_lower_p \
    ascii_letter_lower_q \
    ascii_letter_lower_r \
    ascii_letter_lower_s \
    ascii_letter_lower_t \
    ascii_letter_lower_u \
    ascii_letter_lower_v \
    ascii_letter_lower_w \
    ascii_letter_lower_x \
    ascii_letter_lower_y \
    ascii_letter_lower_z /* clang-format on */
#define __str__ascii_letter_lower_a "a"
#define __uint__ascii_letter_lower_a_byte u8_c('a')
#define __str__ascii_letter_lower_b "b"
#define __uint__ascii_letter_lower_b_byte u8_c('b')
#define __str__ascii_letter_lower_c "c"
#define __uint__ascii_letter_lower_c_byte u8_c('c')
#define __str__ascii_letter_lower_d "d"
#define __uint__ascii_letter_lower_d_byte u8_c('d')
#define __str__ascii_letter_lower_e "e"
#define __uint__ascii_letter_lower_e_byte u8_c('e')
#define __str__ascii_letter_lower_f "f"
#define __uint__ascii_letter_lower_f_byte u8_c('f')
#define __str__ascii_letter_lower_g "g"
#define __uint__ascii_letter_lower_g_byte u8_c('g')
#define __str__ascii_letter_lower_h "h"
#define __uint__ascii_letter_lower_h_byte u8_c('h')
#define __str__ascii_letter_lower_i "i"
#define __uint__ascii_letter_lower_i_byte u8_c('i')
#define __str__ascii_letter_lower_j "j"
#define __uint__ascii_letter_lower_j_byte u8_c('j')
#define __str__ascii_letter_lower_k "k"
#define __uint__ascii_letter_lower_k_byte u8_c('k')
#define __str__ascii_letter_lower_l "l"
#define __uint__ascii_letter_lower_l_byte u8_c('l')
#define __str__ascii_letter_lower_m "m"
#define __uint__ascii_letter_lower_m_byte u8_c('m')
#define __str__ascii_letter_lower_n "n"
#define __uint__ascii_letter_lower_n_byte u8_c('n')
#define __str__ascii_letter_lower_o "o"
#define __uint__ascii_letter_lower_o_byte u8_c('o')
#define __str__ascii_letter_lower_p "p"
#define __uint__ascii_letter_lower_p_byte u8_c('p')
#define __str__ascii_letter_lower_q "q"
#define __uint__ascii_letter_lower_q_byte u8_c('q')
#define __str__ascii_letter_lower_r "r"
#define __uint__ascii_letter_lower_r_byte u8_c('r')
#define __str__ascii_letter_lower_s "s"
#define __uint__ascii_letter_lower_s_byte u8_c('s')
#define __str__ascii_letter_lower_t "t"
#define __uint__ascii_letter_lower_t_byte u8_c('t')
#define __str__ascii_letter_lower_u "u"
#define __uint__ascii_letter_lower_u_byte u8_c('u')
#define __str__ascii_letter_lower_v "v"
#define __uint__ascii_letter_lower_v_byte u8_c('v')
#define __str__ascii_letter_lower_w "w"
#define __uint__ascii_letter_lower_w_byte u8_c('w')
#define __str__ascii_letter_lower_x "x"
#define __uint__ascii_letter_lower_x_byte u8_c('x')
#define __str__ascii_letter_lower_y "y"
#define __uint__ascii_letter_lower_y_byte u8_c('y')
#define __str__ascii_letter_lower_z "z"
#define __uint__ascii_letter_lower_z_byte u8_c('z')

#define __str__ascii_letter_uppers /* clang-format off */ \
    ascii_letter_upper_A \
    ascii_letter_upper_B \
    ascii_letter_upper_C \
    ascii_letter_upper_D \
    ascii_letter_upper_E \
    ascii_letter_upper_F \
    ascii_letter_upper_G \
    ascii_letter_upper_H \
    ascii_letter_upper_I \
    ascii_letter_upper_J \
    ascii_letter_upper_K \
    ascii_letter_upper_L \
    ascii_letter_upper_M \
    ascii_letter_upper_N \
    ascii_letter_upper_O \
    ascii_letter_upper_P \
    ascii_letter_upper_Q \
    ascii_letter_upper_R \
    ascii_letter_upper_S \
    ascii_letter_upper_T \
    ascii_letter_upper_U \
    ascii_letter_upper_V \
    ascii_letter_upper_W \
    ascii_letter_upper_X \
    ascii_letter_upper_Y \
    ascii_letter_upper_Z /* clang-format on */
#define __str__ascii_letter_upper_A "A"
#define __uint__ascii_letter_upper_A_byte u8_c('A')
#define __str__ascii_letter_upper_B "B"
#define __uint__ascii_letter_upper_B_byte u8_c('B')
#define __str__ascii_letter_upper_C "C"
#define __uint__ascii_letter_upper_C_byte u8_c('C')
#define __str__ascii_letter_upper_D "D"
#define __uint__ascii_letter_upper_D_byte u8_c('D')
#define __str__ascii_letter_upper_E "E"
#define __uint__ascii_letter_upper_E_byte u8_c('E')
#define __str__ascii_letter_upper_F "F"
#define __uint__ascii_letter_upper_F_byte u8_c('F')
#define __str__ascii_letter_upper_G "G"
#define __uint__ascii_letter_upper_G_byte u8_c('G')
#define __str__ascii_letter_upper_H "H"
#define __uint__ascii_letter_upper_H_byte u8_c('H')
#define __str__ascii_letter_upper_I "I"
#define __uint__ascii_letter_upper_I_byte u8_c('I')
#define __str__ascii_letter_upper_J "J"
#define __uint__ascii_letter_upper_J_byte u8_c('J')
#define __str__ascii_letter_upper_K "K"
#define __uint__ascii_letter_upper_K_byte u8_c('K')
#define __str__ascii_letter_upper_L "L"
#define __uint__ascii_letter_upper_L_byte u8_c('L')
#define __str__ascii_letter_upper_M "M"
#define __uint__ascii_letter_upper_M_byte u8_c('M')
#define __str__ascii_letter_upper_N "N"
#define __uint__ascii_letter_upper_N_byte u8_c('N')
#define __str__ascii_letter_upper_O "O"
#define __uint__ascii_letter_upper_O_byte u8_c('O')
#define __str__ascii_letter_upper_P "P"
#define __uint__ascii_letter_upper_P_byte u8_c('P')
#define __str__ascii_letter_upper_Q "Q"
#define __uint__ascii_letter_upper_Q_byte u8_c('Q')
#define __str__ascii_letter_upper_R "R"
#define __uint__ascii_letter_upper_R_byte u8_c('R')
#define __str__ascii_letter_upper_S "S"
#define __uint__ascii_letter_upper_S_byte u8_c('S')
#define __str__ascii_letter_upper_T "T"
#define __uint__ascii_letter_upper_T_byte u8_c('T')
#define __str__ascii_letter_upper_U "U"
#define __uint__ascii_letter_upper_U_byte u8_c('U')
#define __str__ascii_letter_upper_V "V"
#define __uint__ascii_letter_upper_V_byte u8_c('V')
#define __str__ascii_letter_upper_W "W"
#define __uint__ascii_letter_upper_W_byte u8_c('W')
#define __str__ascii_letter_upper_X "X"
#define __uint__ascii_letter_upper_X_byte u8_c('X')
#define __str__ascii_letter_upper_Y "Y"
#define __uint__ascii_letter_upper_Y_byte u8_c('Y')
#define __str__ascii_letter_upper_Z "Z"
#define __uint__ascii_letter_upper_Z_byte u8_c('Z')

#if in_analysis_active_only || in_comptime
fn_((ascii_isASCII(u8 c))(bool)) { return c < 0x80; };
fn_((ascii_isUpper(u8 c))(bool)) { return u8_c('A') <= c && c <= u8_c('Z'); };
fn_((ascii_isLower(u8 c))(bool)) { return u8_c('a') <= c && c <= u8_c('z'); };
fn_((ascii_isAlpha(u8 c))(bool)) { return ascii_isUpper(c) || ascii_isLower(c); };
fn_((ascii_isDigit(u8 c))(bool)) { return u8_c('0') <= c && c <= u8_c('9'); };
fn_((ascii_isAlNum(u8 c))(bool)) { return ascii_isDigit(c) || ascii_isAlpha(c); };
fn_((ascii_isHex(u8 c))(bool)) { return ascii_isDigit(c) || (u8_c('A') <= c && c <= u8_c('F')) || (u8_c('a') <= c && c <= u8_c('f')); };
fn_((ascii_isCtrl(u8 c))(bool)) { return c <= ascii_ctrl_Code_us || c == ascii_ctrl_Code_del; };
fn_((ascii_isGlyph(u8 c))(bool)) { return c < 0x20 || 0x7E <= c; };
fn_((ascii_isWhitespace(u8 c))(bool)) { return c == ascii_sp_byte || (ascii_ctrl_Code_ht <= c && c <= ascii_ctrl_Code_lf); };
#endif /* in_analysis_active_only || in_comptime */
#define __str__ascii_whitespaces \
    ascii_sp ascii_ht ascii_lf ascii_cr ascii_vt ascii_ff
#define __str__ascii_whitespace_sp ascii_sp
#define __uint__ascii_whitespace_sp_byte ascii_sp_byte
#define __str__ascii_whitespace_ht ascii_ht
#define __uint__ascii_whitespace_ht_byte ascii_ht_byte
#define __str__ascii_whitespace_lf ascii_lf
#define __uint__ascii_whitespace_lf_byte ascii_lf_byte
#define __str__ascii_whitespace_cr ascii_cr
#define __uint__ascii_whitespace_cr_byte ascii_cr_byte
#define __str__ascii_whitespace_vt ascii_vt
#define __uint__ascii_whitespace_vt_byte ascii_vt_byte
#define __str__ascii_whitespace_ff ascii_ff
#define __uint__ascii_whitespace_ff_byte ascii_ff_byte

#if in_analysis_active_only || in_comptime
fn_((ascii_toUpper(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isLower(c)), 5);
    return c ^ mask;
};
fn_((ascii_toLower(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isUpper(c)), 5);
    return c | mask;
};
fn_((ascii_toggleCase(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isAlpha(c)), 5);
    return c ^ mask;
};

fn_((ascii_digitToInt(u8 c))(u8)) {
    let_(val, u8) = (claim_assert(ascii_isDigit(c)), c - u8_c('0'));
    return val;
};
fn_((ascii_intToDigit(u8 val))(u8)) {
    let_(digit, u8) = val + u8_c('0');
    return claim_assert(ascii_isDigit(digit)), digit;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ascii__included */
