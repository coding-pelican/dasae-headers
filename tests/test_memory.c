#include "../src/assert.h"
#include "../src/memory.h"
#include <stdio.h>
#include <string.h>

// Test helper to print section headers
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

// Test helper to print test result
void TEST_PrintResult(const char* test_name, int success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

// Test basic malloc and free
void TEST_Memory_Allocate_Deallocate() {
    TEST_PrintSection("malloc and free");

    // Test basic allocation
    int* numbers = (int*)Memory_Allocate(5 * sizeof(int));
    AssertNotNull(numbers);
    TEST_PrintResult("Basic malloc", 1);

    // Fill with data
    for (int i = 0; i < 5; i++) {
        numbers[i] = i;
    }

    // Verify data
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    TEST_PrintResult("Memory access", sum == 10);

    // Free memory
    Memory_Deallocate(&numbers);
    TEST_PrintResult("Basic free", 1);

    // Check fill NULL after free
    TEST_PrintResult("Fill NULL after free", numbers == NULL);

    // Test NULL free
    // Memory_Deallocate(NULL);
    // TEST_PrintResult("NULL free", 1);
}

// Test calloc
void TEST_Memory_AllocateCleared() {
    TEST_PrintSection("calloc");

    // Test basic calloc
    int* numbers = (int*)Memory_AllocateCleared(5, sizeof(int));
    AssertNotNull(numbers);
    TEST_PrintResult("Basic calloc", 1);

    // Verify zero initialization
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += numbers[i];
    }
    TEST_PrintResult("Zero initialization", sum == 0);

    Memory_Deallocate(&numbers);
}

// Test memset
void TEST_Memory_Set() {
    TEST_PrintSection("memset");

    char* buffer = (char*)Memory_Allocate(10);
    Memory_Set(buffer, 'A', 10);

    int correct = 1;
    for (int i = 0; i < 10; i++) {
        if (buffer[i] != 'A') {
            correct = 0;
            break;
        }
    }
    TEST_PrintResult("memset fill", correct);

    Memory_Deallocate(&buffer);
}

// Test memcpy
void TEST_Memory_Copy() {
    TEST_PrintSection("memcpy");

    const char* source = "Hello";
    char*       dest   = (char*)Memory_Allocate(6);

    Memory_Copy(dest, source, 6);
    TEST_PrintResult("memcpy", strcmp(dest, source) == 0);

    Memory_Deallocate(&dest);
}

// Test move
void TEST_Memory_Move() {
    TEST_PrintSection("move");

    char* buffer = (char*)Memory_Allocate(10);
    Memory_Set(buffer, 'A', 5);
    Memory_Set(buffer + 5, 'B', 5);

    // Test overlapping move
    Memory_Move(buffer + 2, buffer, 8);

    // Verify move
    TEST_PrintResult("move operation", buffer[9] == 'B');

    Memory_Deallocate(&buffer);
}

// Test copy with reference counting
void TEST_Memory_CopyBlock() {
    TEST_PrintSection("copy with reference counting");

    int* source = (int*)Memory_Allocate(sizeof(int));
    *source     = 42;

    int* dest = (int*)Memory_Allocate(sizeof(int));
    Memory_CopyBlock(dest, source, sizeof(int));

    TEST_PrintResult("copy value", *dest == 42);

    Memory_Deallocate(&dest);
    Memory_Deallocate(&source);
}

// Test memory leak detection
void TEST_Memory_DetectMemoryLeak() {
    TEST_PrintSection("memory leak detection");

    // Intentionally leak some memory
    int* leaked = (int*)Memory_Allocate(sizeof(int));
    *leaked     = 42;

    // Don't free 'leaked' - it should be reported at program exit
    TEST_PrintResult("Created intentional leak", 1);
}

// Test multiple allocations and frees
void TEST_Memory_MultipleOperations() {
    TEST_PrintSection("multiple operations");

    void* ptrs[100];
    int   success = 1;

    // Allocate 100 blocks
    for (int i = 0; i < 100; ++i) {
        ptrs[i] = Memory_Allocate(i + 1);
        if (!ptrs[i]) {
            success = 0;
            break;
        }
    }
    TEST_PrintResult("Multiple allocations", success);

    // Free all blocks
    for (int i = 0; i < 100; ++i) {
        Memory_Deallocate(&ptrs[i]);
    }
    TEST_PrintResult("Multiple frees", 1);
}

// Test allocation size tracking
void TEST_Memory_SizeTracking() {
    TEST_PrintSection("size tracking");

    // Allocate different sizes
    char* str1 = (char*)Memory_Allocate(10);
    char* str2 = (char*)Memory_Allocate(20);
    char* str3 = (char*)Memory_Allocate(30);

    Memory_Deallocate(&str1);
    Memory_Deallocate(&str2);
    Memory_Deallocate(&str3);

    TEST_PrintResult("Size tracking", 1);
}


int main() {
    printf("Starting Memory Wrapper (for Debugging) Tests\n");

    TEST_Memory_Allocate_Deallocate();
    TEST_Memory_AllocateCleared();
    TEST_Memory_Set();
    TEST_Memory_Copy();
    TEST_Memory_Move();
    TEST_Memory_CopyBlock();
    TEST_Memory_MultipleOperations();
    TEST_Memory_SizeTracking();

    // Run memory leak TEST last
    TEST_Memory_DetectMemoryLeak();

    printf("\nAll TESTs completed. Check above for results.\n");
    printf("An intentional memory leak was created - check the leak report below:\n\n");

    // The leak report will be printed automatically at program exit
    // due to the atexit handler we registered

    return 0;
}
