// build `clang -o test_mem -xc -std=c17 test_mem.c ../../src/*.c -static -g  -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

#include "dh/mem.h"
#include "dh/debug/assert.h"

#include <stdio.h>
#include <string.h>

// Test helper to print section headers
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

// Test helper to print test result
void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

// Test basic malloc and free
void TEST_mem_allocate_deallocate() {
    TEST_PrintSection("malloc and free");

    // Test basic allocation
    i32* numbers = (i32*)mem_allocate(5 * sizeof(i32));
    debug_assert_nonnull(numbers);
    TEST_PrintResult("Basic malloc", 1);

    // Fill with data
    for (i32 i = 0; i < 5; i++) {
        numbers[i] = i;
    }

    // Verify data
    i32 sum = 0;
    for (i32 i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    TEST_PrintResult("Memory access", sum == 10);

    // Free memory
    mem_deallocate(&numbers);
    TEST_PrintResult("Basic free", 1);

    // Check fill NULL after free
    TEST_PrintResult("Fill NULL after free", numbers == NULL);

    // Test NULL free
    // mem_deallocate(NULL);
    // TEST_PrintResult("NULL free", 1);
}

// Test calloc
void TEST_mem_allocateCleared() {
    TEST_PrintSection("calloc");

    // Test basic calloc
    i32* numbers = (i32*)mem_allocateCleared(5, sizeof(i32));
    debug_assert_nonnull(numbers);
    TEST_PrintResult("Basic calloc", 1);

    // Verify zero initialization
    i32 sum = 0;
    for (i32 i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    TEST_PrintResult("Zero initialization", sum == 0);

    mem_deallocate(&numbers);
}

void TEST_mem_reallocate() {
    TEST_PrintSection("realloc");

    // Initial allocation
    i32* numbers = (i32*)mem_allocate(2 * sizeof(i32));
    numbers[0]   = 1;
    numbers[1]   = 2;

    // Reallocate to larger size
    numbers = (i32*)mem_reallocate(numbers, 4 * sizeof(i32));
    debug_assert_nonnull(numbers);

    // Verify original data preserved
    TEST_PrintResult("Data preserved", numbers[0] == 1 && numbers[1] == 2);

    // Add new data
    numbers[2] = 3;
    numbers[3] = 4;

    // Verify new space usable
    i32 sum = 0;
    for (i32 i = 0; i < 4; i++) {
        sum += numbers[i];
    }
    TEST_PrintResult("Extended memory usable", sum == 10);

    mem_deallocate(&numbers);
}

// Test memset
void TEST_mem_set() {
    TEST_PrintSection("memset");

    char* buffer = (char*)mem_allocate(10);
    mem_set(buffer, 'A', 10);

    i32 correct = 1;
    for (i32 i = 0; i < 10; i++) {
        if (buffer[i] != 'A') {
            correct = 0;
            break;
        }
    }
    TEST_PrintResult("memset fill", correct);

    mem_deallocate(&buffer);
}

// Test memcpy
void TEST_mem_copy() {
    TEST_PrintSection("memcpy");

    const char* source = "Hello";
    char*       dest   = (char*)mem_allocate(6);

    mem_copy(dest, (anyptr)source, 6);
    TEST_PrintResult("memcpy", strcmp(dest, source) == 0);

    mem_deallocate(&dest);
}

// Test move
void TEST_mem_move() {
    TEST_PrintSection("move");

    char* buffer = (char*)mem_allocate(10);
    mem_set(buffer, 'A', 5);
    mem_set(buffer + 5, 'B', 5);

    // Test overlapping move
    mem_move(buffer + 2, buffer, 8);

    // Verify move
    TEST_PrintResult("move operation", buffer[9] == 'B');

    mem_deallocate(&buffer);
}

// Test copy with reference counting
void TEST_mem_copyBlock() {
    TEST_PrintSection("copy with reference counting");

    i32* source = (i32*)mem_allocate(sizeof(i32));
    *source     = 42;

    i32* dest = (i32*)mem_allocate(sizeof(i32));
    mem_copy(dest, source, sizeof(i32));

    TEST_PrintResult("copy value", *dest == 42);

    mem_deallocate(&dest);
    mem_deallocate(&source);
}

// Test multiple allocations and frees
void TEST_mem_MultipleOperations() {
    TEST_PrintSection("multiple operations");

    anyptr ptrs[100];
    i32    success = 1;

    // Allocate 100 blocks
    for (i32 i = 0; i < 100; ++i) {
        ptrs[i] = mem_allocate(i + 1);
        if (!ptrs[i]) {
            success = 0;
            break;
        }
    }
    TEST_PrintResult("Multiple allocations", success);

    // Free all blocks
    for (i32 i = 0; i < 100; ++i) {
        mem_deallocate(&ptrs[i]);
    }
    TEST_PrintResult("Multiple frees", 1);
}

// Test allocation size tracking
void TEST_mem_SizeTracking() {
    TEST_PrintSection("size tracking");

    // Allocate different sizes
    char* str1 = (char*)mem_allocate(10);
    char* str2 = (char*)mem_allocate(20);
    char* str3 = (char*)mem_allocate(30);

    mem_deallocate(&str1);
    mem_deallocate(&str2);
    mem_deallocate(&str3);

    TEST_PrintResult("Size tracking", 1);
}

typedef struct TestStruct {
    i32  a;
    i32  b;
    i8   c[4];
    bool canUse;
} TestStruct;

void TEST_mem_new_delete() {
    TEST_PrintSection("new and delete");

    i32* ptr = mem_create(i32);
    *ptr     = 42;
    TEST_PrintResult("new", *ptr == 42);

    mem_free(&ptr);
    TEST_PrintResult("delete", ptr == null);

    TestStruct* test_struct = mem_createWith(TestStruct, .a = 1, .b = 2, .c = { 3, 4, 5, 6 }, .canUse = true);
    TEST_PrintResult(
        "newWith",
        test_struct->a == 1 && test_struct->b == 2 && test_struct->c[0] == 3 && test_struct->c[1] == 4 && test_struct->c[2] == 5 && test_struct->c[3] == 6 && test_struct->canUse
    );

    mem_free(&test_struct);
    TEST_PrintResult("delete", test_struct == null);
}

// Test memory leak detection
void TEST_mem_MemoryLeakDetection() {
    TEST_PrintSection("memory leak detection");

    // Intentionally leak some memory
    i32* leaked = (i32*)mem_allocate(sizeof(i32));
    *leaked     = 42;

    TestStruct* leaked_struct = mem_createWith(TestStruct, .a = 1, .b = 2, .c = { 3, 4, 5, 6 }, .canUse = true);
    leaked_struct->canUse     = false;

    // Don't free 'leaked' - it should be reported at program exit
    TEST_PrintResult("Created intentional leak", 1);
}


i32 main() {
    printf("Starting Memory Wrapper (for Debugging) Tests\n");

    TEST_mem_allocate_deallocate();
    TEST_mem_allocateCleared();
    TEST_mem_reallocate();
    TEST_mem_set();
    TEST_mem_copy();
    TEST_mem_move();
    TEST_mem_copyBlock();
    TEST_mem_MultipleOperations();
    TEST_mem_SizeTracking();
    TEST_mem_new_delete();

    // Run memory leak TEST last
    TEST_mem_MemoryLeakDetection();

    printf("\nAll TESTs completed. Check above for results.\n");
    printf("An intentional memory leak was created - check the leak report below:\n\n");

    // The leak report will be printed automatically at program exit
    // due to the atexit handler we registered

    return 0;
}
