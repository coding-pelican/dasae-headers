/**
 * @file    note-HashMap_simd_lookup_bug-dh.c
 * @brief   Demonstrates the SIMD lookup bug and its fix
 *
 * This test reproduces the bug in the SIMD-accelerated HashMap lookup
 * at a logical level, without using the actual HashMap implementation.
 *
 * BUG: The SIMD lookup terminated when ANY free slot was found in a group,
 *      even if that free slot was BEFORE the probe starting position.
 *
 * FIX: Only consider free slots at positions >= start_offset_in_group
 *      in the first group of the probe sequence.
 */

#include "dh.h"
#include "dh/TEST.h"

#define SIMD_GROUP_SIZE 16

/*============================================================================*/
/* Simulated Control Byte Operations                                          */
/*============================================================================*/

typedef enum SlotState {
    SLOT_FREE = 0,      /* Empty slot */
    SLOT_TOMBSTONE = 1, /* Deleted slot */
    SLOT_USED = 0x80,   /* Used slot (high bit set) */
} SlotState;

$attr($inline_always)
$static fn_((isFree(u8 ctrl))(bool)) {
    return ctrl == SLOT_FREE;
}

/*============================================================================*/
/* Simulated SIMD Operations                                                  */
/*============================================================================*/

/* Simulate SIMD match for free slots (returns bitmask of free positions) */
$static fn_((simd_match_free(const u8* group))(u32)) {
    var_(mask, u32) = 0;
    for_(($r(0, SIMD_GROUP_SIZE))(i) {
        if (isFree(group[i])) {
            mask |= (1u << i);
        }
    });
    return mask;
}

/* Simulate SIMD match for fingerprint (returns bitmask of matching positions) */
$static fn_((simd_match_fingerprint(const u8* group, u8 fingerprint))(u32)) {
    var_(mask, u32) = 0;
    for_(($r(0, SIMD_GROUP_SIZE))(i) {
        if (group[i] == (fingerprint | SLOT_USED)) {
            mask |= (1u << i);
        }
    });
    return mask;
}

/*============================================================================*/
/* Buggy SIMD Lookup (Old Implementation)                                     */
/*============================================================================*/

/**
 * OLD BUGGY BEHAVIOR:
 * Terminates if ANY free slot exists in the group, regardless of probe order.
 */
$static fn_((lookup_simd_buggy(
    const u8* metadata,
    u32 cap,
    u32 hash,
    u8 fingerprint,
    const u32* keys, /* Array of keys at each position */
    u32 search_key
))(i32)) {
    let start_group = (hash & (cap - 1)) / SIMD_GROUP_SIZE;
    let num_groups = cap / SIMD_GROUP_SIZE;

    for_(($r(0, num_groups))(group_offset) {
        let group_idx = (start_group + group_offset) % num_groups;
        let group_start = group_idx * SIMD_GROUP_SIZE;
        let group = &metadata[group_start];

        /* Check fingerprint matches */
        var_(match_mask, u32) = simd_match_fingerprint(group, fingerprint);
        while (match_mask != 0) {
            let bit_pos = __builtin_ctz(match_mask);
            let idx = group_start + bit_pos;
            if (keys[idx] == search_key) {
                return as$(i32)(idx); /* Found! */
            }
            match_mask &= match_mask - 1;
        }

        /* BUG: Terminates on ANY free slot in the group */
        let free_mask = simd_match_free(group);
        if (free_mask != 0) {
            return -1; /* Not found */
        }
    });

    return -1;
}

/*============================================================================*/
/* Fixed SIMD Lookup (New Implementation)                                     */
/*============================================================================*/

/**
 * FIXED BEHAVIOR:
 * Only terminates on free slots that are in the actual probe sequence.
 */
$static fn_((lookup_simd_fixed(
    const u8* metadata,
    u32 cap,
    u32 hash,
    u8 fingerprint,
    const u32* keys,
    u32 search_key
))(i32)) {
    let start_idx = hash & (cap - 1);
    let start_group = start_idx / SIMD_GROUP_SIZE;
    let start_offset_in_group = start_idx % SIMD_GROUP_SIZE;
    let num_groups = cap / SIMD_GROUP_SIZE;

    for_(($r(0, num_groups))(group_offset) {
        let group_idx = (start_group + group_offset) % num_groups;
        let group_start = group_idx * SIMD_GROUP_SIZE;
        let group = &metadata[group_start];

        /* Check fingerprint matches */
        var_(match_mask, u32) = simd_match_fingerprint(group, fingerprint);
        while (match_mask != 0) {
            let bit_pos = __builtin_ctz(match_mask);
            let idx = group_start + bit_pos;
            if (keys[idx] == search_key) {
                return as$(i32)(idx);
            }
            match_mask &= match_mask - 1;
        }

        /* FIX: Only consider free slots in the probe sequence */
        var_(free_mask, u32) = simd_match_free(group);

        /* For the first group, mask out positions before start_offset_in_group */
        if (group_offset == 0 && start_offset_in_group > 0) {
            free_mask &= (~0u << start_offset_in_group);
        }

        if (free_mask != 0) {
            return -1;
        }
    });

    return -1;
}

/*============================================================================*/
/* Test Cases                                                                 */
/*============================================================================*/

/**
 * Test scenario that triggers the bug:
 *
 * We use 2 SIMD groups (32 slots total).
 *
 * Group 0 (positions 0-15):
 *   [0-7]:  FREE (empty slots - BEFORE probe start!)
 *   [8-15]: TOMBSTONES (deleted entries - collisions that were removed)
 *
 * Group 1 (positions 16-31):
 *   [16]:   Key X (the key we're searching for, pushed here by collisions)
 *   [17-31]: FREE
 *
 * Search for Key X (hash=8, fingerprint=0x42):
 *   - Probe sequence: 8, 9, 10, ..., 15, 16, 17, ...
 *   - Key X is at position 16 (pushed by collisions from position 8)
 *
 * BUGGY behavior:
 *   1. Search group 0 (positions 0-15)
 *   2. No fingerprint match in group 0 (all tombstones have different fingerprint)
 *   3. Check free slots: positions 0-7 are FREE!
 *   4. Return "not found" (WRONG!) - never reaches group 1
 *
 * FIXED behavior:
 *   1. Search group 0 (positions 0-15)
 *   2. No fingerprint match in group 0
 *   3. Check free slots with mask: positions 0-7 masked out (before probe start at 8)
 *   4. Positions 8-15 are tombstones (not free), so continue to next group
 *   5. Search group 1, find Key X at position 16
 */
TEST_fn_("SIMD bug: free slots before probe start cause false negative" $scope) {
    let_(cap, u32) = 32; /* Two SIMD groups */

    /* Setup metadata - 32 slots */
    u8 metadata[32] = {
        /* Group 0: positions 0-15 */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,                     /* 0-3:  free (before probe) */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,                     /* 4-7:  free (before probe) */
        SLOT_TOMBSTONE, SLOT_TOMBSTONE, SLOT_TOMBSTONE, SLOT_TOMBSTONE, /* 8-11:  tombstones */
        SLOT_TOMBSTONE, SLOT_TOMBSTONE, SLOT_TOMBSTONE, SLOT_TOMBSTONE, /* 12-15: tombstones */
        /* Group 1: positions 16-31 */
        0x42 | SLOT_USED,                           /* 16: Key X (our target!) */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE, /* 17-20: free */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE, /* 21-24: free */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE, /* 25-28: free */
        SLOT_FREE, SLOT_FREE, SLOT_FREE             /* 29-31: free */
    };

    /* Setup keys - 32 slots */
    u32 keys[32] = {
        /* Group 0: positions 0-15 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 0-7:   unused */
        0, 0, 0, 0, 0, 0, 0, 0, /* 8-15:  deleted keys */
        /* Group 1: positions 16-31 */
        999,                    /* 16: Key X = 999 */
        0, 0, 0, 0, 0, 0, 0, 0, /* 17-24: unused */
        0, 0, 0, 0, 0, 0, 0     /* 25-31: unused */
    };

    let_(hash, u32) = 8; /* Probe starts at position 8 */
    let_(fingerprint, u8) = 0x42;
    let_(search_key, u32) = 999; /* Search for Key X */

    /* BUGGY: Returns -1 (not found) because positions 0-7 are free */
    let buggy_result = lookup_simd_buggy(metadata, cap, hash, fingerprint, keys, search_key);
    try_(TEST_expect(buggy_result == -1)); /* Bug: falsely claims not found */

    /* FIXED: Returns 16 (found at position 16) */
    let fixed_result = lookup_simd_fixed(metadata, cap, hash, fingerprint, keys, search_key);
    try_(TEST_expect(fixed_result == 16)); /* Correct: finds Key X at position 16 */

    /* Print results for clarity */
    io_stream_println(
        u8_l("Bug demonstration:")
    );
    io_stream_println(
        u8_l("  - Searching for key 999 (hash=8, stored at position 16 due to collisions)")
    );
    io_stream_println(
        u8_l("  - Positions 0-7 are FREE (before probe start at position 8)")
    );
    io_stream_println(
        u8_l("  - Positions 8-15 are TOMBSTONES (deleted collisions)")
    );
    io_stream_println(
        u8_l("  - BUGGY lookup result:  {:il} (incorrectly returns -1 = not found)"),
        buggy_result
    );
    io_stream_println(
        u8_l("  - FIXED lookup result:  {:il} (correctly finds at position 16)"),
        fixed_result
    );
} $unscoped_(TEST_fn);

/**
 * Test that the fix doesn't break normal operation:
 * When probe starts at position 0, all free slots should terminate search.
 */
TEST_fn_("SIMD fix: still terminates correctly when probe starts at group boundary" $scope) {
    let_(cap, u32) = 16;

    u8 metadata[16] = {
        0x33 | SLOT_USED, /* 0: Key A */
        SLOT_FREE,        /* 1: free - should terminate here */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_FREE, SLOT_FREE
    };

    u32 keys[16] = {
        100, /* 0: Key A = 100 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    let_(hash, u32) = 0; /* Probe starts at position 0 */
    let_(fingerprint, u8) = 0x33;
    let_(search_key, u32) = 999; /* Search for non-existent key */

    /* Both should return -1 (not found) because position 1 is free */
    let buggy_result = lookup_simd_buggy(metadata, cap, hash, fingerprint, keys, search_key);
    let fixed_result = lookup_simd_fixed(metadata, cap, hash, fingerprint, keys, search_key);

    try_(TEST_expect(buggy_result == -1));
    try_(TEST_expect(fixed_result == -1));

    io_stream_println(u8_l(""));
    io_stream_println(
        u8_l("Normal operation (probe starts at position 0):")
    );
    io_stream_println(
        u8_l("  - BUGGY lookup: {:il} (correct: not found)"),
        buggy_result
    );
    io_stream_println(
        u8_l("  - FIXED lookup: {:il} (correct: not found)"),
        fixed_result
    );
} $unscoped_(TEST_fn);

/**
 * Test tombstone handling: tombstones should NOT terminate search
 */
TEST_fn_("SIMD: tombstones don't terminate search (both implementations)" $scope) {
    let_(cap, u32) = 16;

    u8 metadata[16] = {
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_TOMBSTONE,   /* 4: tombstone (deleted entry) */
        0x55 | SLOT_USED, /* 5: Key B */
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_FREE, SLOT_FREE, SLOT_FREE, SLOT_FREE,
        SLOT_FREE, SLOT_FREE
    };

    u32 keys[16] = {
        0, 0, 0, 0,
        0,   /* 4: was Key A (deleted) */
        300, /* 5: Key B = 300 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    let_(hash, u32) = 4; /* Probe starts at position 4 */
    let_(fingerprint, u8) = 0x55;
    let_(search_key, u32) = 300;

    /* Both should find Key B at position 5 (tombstone doesn't terminate) */
    let buggy_result = lookup_simd_buggy(metadata, cap, hash, fingerprint, keys, search_key);
    let fixed_result = lookup_simd_fixed(metadata, cap, hash, fingerprint, keys, search_key);

    try_(TEST_expect(buggy_result == 5));
    try_(TEST_expect(fixed_result == 5));

    io_stream_println(u8_l(""));
    io_stream_println(
        u8_l("Tombstone handling (probe starts at position 4):")
    );
    io_stream_println(
        u8_l("  - Position 4 is TOMBSTONE (should not terminate)")
    );
    io_stream_println(
        u8_l("  - BUGGY lookup: {:il} (finds Key B at position 5)"),
        buggy_result
    );
    io_stream_println(
        u8_l("  - FIXED lookup: {:il} (finds Key B at position 5)"),
        fixed_result
    );
} $unscoped_(TEST_fn);
