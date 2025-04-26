#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED (1)

#include "dh/core.h"
#include "dh/sort.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/ArrList.h"
#include "Body.h"

decl_ArrList$(Body);
extern fn_(utils_uniformDisc(mem_Allocator allocator, usize n), Err$ArrList$Body) $must_check;

#endif /* UTILS_INCLUDED */
