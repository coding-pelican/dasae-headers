/**
 * Demonstration of the SIMD lookup bug in HashMap
 *
 * This file shows the bug and fix in isolation, without the full HashMap implementation.
 * Compile: clang -o note-HashMap_simd_lookup_bug note-HashMap_simd_lookup_bug.c && ./note-HashMap_simd_lookup_bug
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define GROUP_SIZE 16
#define CAPACITY 64 /* 4 groups */

typedef struct {
    uint8_t ctrl[CAPACITY]; /* 0=free, 1=tombstone, 0x80|fp=used */
    int keys[CAPACITY];
} MockHashMap;

/*=============================================================================
 * BUGGY VERSION (before fix)
 *
 * Problem: Terminates search if ANY free slot exists in the current group,
 * even if that free slot is BEFORE the start of the probe sequence.
 *=============================================================================*/
bool buggy_simd_contains(MockHashMap* map, int key, int hash) {
    int start_idx = hash % CAPACITY;
    int start_group = start_idx / GROUP_SIZE;
    int num_groups = CAPACITY / GROUP_SIZE;

    for (int group_offset = 0; group_offset < num_groups; group_offset++) {
        int group_idx = (start_group + group_offset) % num_groups;
        int group_start = group_idx * GROUP_SIZE;

        /* Check for key match in this group (simulates SIMD fingerprint match) */
        for (int i = 0; i < GROUP_SIZE; i++) {
            int idx = group_start + i;
            if ((map->ctrl[idx] & 0x80) && map->keys[idx] == key) {
                return true; /* Found! */
            }
        }

        /* BUG: Check if ANY free slot exists in this group */
        for (int i = 0; i < GROUP_SIZE; i++) {
            int idx = group_start + i;
            if (map->ctrl[idx] == 0) { /* Free slot */
                return false;          /* Terminate search - BUG! */
            }
        }
    }
    return false;
}

/*=============================================================================
 * FIXED VERSION (after fix)
 *
 * Fix: In the first group, only consider free slots at positions >= start_offset.
 * This correctly follows linear probing semantics.
 *=============================================================================*/
bool fixed_simd_contains(MockHashMap* map, int key, int hash) {
    int start_idx = hash % CAPACITY;
    int start_group = start_idx / GROUP_SIZE;
    int start_offset_in_group = start_idx % GROUP_SIZE;
    int num_groups = CAPACITY / GROUP_SIZE;

    for (int group_offset = 0; group_offset < num_groups; group_offset++) {
        int group_idx = (start_group + group_offset) % num_groups;
        int group_start = group_idx * GROUP_SIZE;

        /* Check for key match in this group (simulates SIMD fingerprint match) */
        for (int i = 0; i < GROUP_SIZE; i++) {
            int idx = group_start + i;
            if ((map->ctrl[idx] & 0x80) && map->keys[idx] == key) {
                return true; /* Found! */
            }
        }

        /* FIX: Only check free slots that are IN the probe sequence */
        int check_start = (group_offset == 0) ? start_offset_in_group : 0;
        for (int i = check_start; i < GROUP_SIZE; i++) {
            int idx = group_start + i;
            if (map->ctrl[idx] == 0) { /* Free slot in probe sequence */
                return false;          /* Terminate search - CORRECT! */
            }
        }
    }
    return false;
}

/*=============================================================================
 * TEST: Reproduce the bug scenario
 *=============================================================================*/
void test_bug_scenario(void) {
    printf("=== Bug Reproduction Test ===\n\n");

    MockHashMap map = { 0 };

    /*
     * Scenario:
     * - Group 0 (positions 0-15): positions 0-7 are FREE, positions 8-15 are USED
     * - Key X hashes to position 8, but due to collisions, it's at position 16
     * - When searching for Key X:
     *   - Buggy: finds free slot at position 0, terminates -> NOT FOUND (wrong!)
     *   - Fixed: ignores positions 0-7, continues to next group -> FOUND (correct!)
     */

    /* Setup: positions 0-7 are free (already 0) */
    /* Positions 8-15 are occupied by other keys */
    for (int i = 8; i < 16; i++) {
        map.ctrl[i] = 0x80 | (uint8_t)(i & 0x7F); /* Used with some fingerprint */
        map.keys[i] = 100 + i;                    /* Different keys */
    }

    /* Key X is at position 16 (pushed there due to collisions) */
    int key_x = 999;
    int hash_x = 8;             /* Would want position 8, but it's taken */
    map.ctrl[16] = 0x80 | 0x42; /* Used */
    map.keys[16] = key_x;

    printf("Setup:\n");
    printf("  - Positions 0-7:  FREE (0x00)\n");
    printf("  - Positions 8-15: USED (other keys)\n");
    printf("  - Position 16:    Key X = 999\n");
    printf("  - Searching for Key X with hash = 8\n");
    printf("  - Probe sequence: 8, 9, 10, ..., 15, 16, ... (NOT 0, 1, 2, ...)\n\n");

    bool buggy_result = buggy_simd_contains(&map, key_x, hash_x);
    bool fixed_result = fixed_simd_contains(&map, key_x, hash_x);

    printf("Results:\n");
    printf("  Buggy version:  %s\n", buggy_result ? "FOUND" : "NOT FOUND");
    printf("  Fixed version:  %s\n", fixed_result ? "FOUND" : "NOT FOUND");
    printf("\n");

    printf("Explanation:\n");
    printf("  - Buggy: Sees free slot at position 0, terminates immediately\n");
    printf("           But position 0 is NOT in the probe sequence starting at 8!\n");
    printf("  - Fixed: Ignores positions 0-7, checks only 8-15 for free slots\n");
    printf("           No free slot found there, continues to group 1\n");
    printf("           Finds Key X at position 16\n\n");

    if (!buggy_result && fixed_result) {
        printf("[PASS] Bug reproduced! Buggy version fails, fixed version works.\n");
    } else {
        printf("[FAIL] Unexpected results.\n");
    }
    printf("\n");
}

/*=============================================================================
 * TEST: Verify fix doesn't break normal cases
 *=============================================================================*/
void test_normal_cases(void) {
    printf("=== Normal Case Verification ===\n\n");

    int pass_count = 0;
    int total = 0;

    /* Case 1: Key exists at its hash position */
    {
        MockHashMap map = { 0 };
        map.ctrl[5] = 0x80 | 0x11;
        map.keys[5] = 42;

        bool b = buggy_simd_contains(&map, 42, 5);
        bool f = fixed_simd_contains(&map, 42, 5);
        printf("Case 1 - Key at hash position:\n");
        printf("  Buggy: %s, Fixed: %s\n", b ? "FOUND" : "NOT FOUND", f ? "FOUND" : "NOT FOUND");
        if (b && f) {
            printf("  [PASS]\n");
            pass_count++;
        } else {
            printf("  [FAIL] Both should be FOUND\n");
        }
        total++;
        printf("\n");
    }

    /* Case 2: Key doesn't exist, free slot at hash position */
    {
        MockHashMap map = { 0 };
        bool b = buggy_simd_contains(&map, 99, 10);
        bool f = fixed_simd_contains(&map, 99, 10);
        printf("Case 2 - Key doesn't exist, free slot at hash:\n");
        printf("  Buggy: %s, Fixed: %s\n", b ? "FOUND" : "NOT FOUND", f ? "FOUND" : "NOT FOUND");
        if (!b && !f) {
            printf("  [PASS]\n");
            pass_count++;
        } else {
            printf("  [FAIL] Both should be NOT FOUND\n");
        }
        total++;
        printf("\n");
    }

    /* Case 3: Key doesn't exist, free slot AFTER hash position */
    {
        MockHashMap map = { 0 };
        map.ctrl[10] = 0x80 | 0x22; /* Position 10 occupied */
        map.keys[10] = 100;         /* Different key */
        /* Position 11 is free */

        bool b = buggy_simd_contains(&map, 99, 10);
        bool f = fixed_simd_contains(&map, 99, 10);
        printf("Case 3 - Key doesn't exist, free slot after hash:\n");
        printf("  Buggy: %s, Fixed: %s\n", b ? "FOUND" : "NOT FOUND", f ? "FOUND" : "NOT FOUND");
        if (!b && !f) {
            printf("  [PASS]\n");
            pass_count++;
        } else {
            printf("  [FAIL] Both should be NOT FOUND\n");
        }
        total++;
        printf("\n");
    }

    /* Case 4: Key exists after collision chain */
    {
        MockHashMap map = { 0 };
        /* Fill positions 10-12 */
        map.ctrl[10] = 0x80 | 0x11;
        map.keys[10] = 100;
        map.ctrl[11] = 0x80 | 0x22;
        map.keys[11] = 101;
        map.ctrl[12] = 0x80 | 0x33;
        map.keys[12] = 42; /* Target key */
        /* Position 13 is free */

        bool b = buggy_simd_contains(&map, 42, 10);
        bool f = fixed_simd_contains(&map, 42, 10);
        printf("Case 4 - Key exists after collision chain:\n");
        printf("  Buggy: %s, Fixed: %s\n", b ? "FOUND" : "NOT FOUND", f ? "FOUND" : "NOT FOUND");
        if (b && f) {
            printf("  [PASS]\n");
            pass_count++;
        } else {
            printf("  [FAIL] Both should be FOUND\n");
        }
        total++;
        printf("\n");
    }

    /* Case 5: Key doesn't exist, tombstone in the way */
    {
        MockHashMap map = { 0 };
        map.ctrl[10] = 0x01; /* Tombstone at hash position */
        /* Position 11 is free */

        bool b = buggy_simd_contains(&map, 99, 10);
        bool f = fixed_simd_contains(&map, 99, 10);
        printf("Case 5 - Key doesn't exist, tombstone then free:\n");
        printf("  Buggy: %s, Fixed: %s\n", b ? "FOUND" : "NOT FOUND", f ? "FOUND" : "NOT FOUND");
        if (!b && !f) {
            printf("  [PASS]\n");
            pass_count++;
        } else {
            printf("  [FAIL] Both should be NOT FOUND\n");
        }
        total++;
        printf("\n");
    }

    printf("Summary: %d/%d tests passed\n\n", pass_count, total);
}

/*=============================================================================
 * MAIN
 *=============================================================================*/
int main(void) {
    printf("========================================\n");
    printf("  HashMap SIMD Lookup Bug Demonstration\n");
    printf("========================================\n\n");

    test_bug_scenario();
    printf("----------------------------------------\n\n");
    test_normal_cases();

    return 0;
}
