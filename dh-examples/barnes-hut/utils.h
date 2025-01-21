#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/ArrList.h"
#include "Body.h"

decl_ArrList$(Body);
decl_Err$(ArrList$Body);
extern Err$ArrList$Body utils_uniformDisc(mem_Allocator allocator, usize n) must_check;

#endif /* UTILS_INCLUDED */
