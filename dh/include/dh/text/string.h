// force_inline cmpOrdering cmp_str(const anyptr lhs, const anyptr rhs) {
//     const char* sa     = *(const char**)lhs;
//     const char* sb     = *(const char**)rhs;
//     i32         result = strcmp(sa, sb);
//     if (result < 0) {
//         return cmpOrd_Less;
//     }
//     if (result > 0) {
//         return cmpOrd_Greater;
//     }
//     return cmpOrd_Equal;
// }
