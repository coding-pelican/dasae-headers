#ifndef dal_c_ext_ArrStr__included
#define dal_c_ext_ArrStr__included 1

#include "dal-c-ext/base.h"

// Dynamic string array
typedef struct ArrStr {
    char** items;
    int items_len;
    int cap;
} ArrStr;

// Returns: allocated `ArrStr*`; caller must call `ArrStr_fini()`
extern ArrStr* ArrStr_init(void);
extern void ArrStr_fini(ArrStr** self);
// Returns: allocated array of strings (NULL-terminated); caller must `free()` each string and the array
extern char** ArrStr_toRaw(const ArrStr* self);
extern int ArrStr_len(const ArrStr* self);
extern const char* ArrStr_at(const ArrStr* self, int idx);
extern char* ArrStr_atMut(ArrStr* self, int idx);
extern void ArrStr_push(ArrStr* self, const char* item);

#endif /* dal_c_ext_ArrStr__included */
