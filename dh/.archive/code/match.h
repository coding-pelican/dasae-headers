// #define match(value)                           \
//     do {                                       \
//         typeof(value) __match_value = (value); \
//         if (0) {}

// #define match_case(pattern) \
//     else if (__match_value == (pattern))

// #define match_default \
//     else

// #define match_end \
//     }             \
//     while (0)


// void test_match() {
//     // Enum example
//     typedef enum Status {
//         SUCCESS,
//         ERROR,
//         PENDING
//     } Status;

//     Status status = SUCCESS;

//     match(status) {
//         match_case(SUCCESS) {
//             printf("Operation succeeded\n");
//         }
//         match_case(ERROR) {
//             printf("Operation failed\n");
//         }
//         match_case(PENDING) {
//             printf("Operation pending\n");
//         }
//         match_default {
//             printf("Unknown status\n");
//         }
//         match_end;
//     }

//     // Integer example
//     int value = 42;

//     match(value) {
//         match_case(0) {
//             printf("Zero\n");
//         }
//         match_case(42) {
//             printf("Found 42!\n");
//         }
//         match_default {
//             printf("Other number\n");
//         }
//         match_end;
//     }
// }
