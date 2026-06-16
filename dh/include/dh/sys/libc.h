#pragma once
#ifndef sys_libc__included
#define sys_libc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "libc/darwin.h"
#include "libc/bionic.h"

#include "libc/freebsd.h"
#include "libc/openbsd.h"
#include "libc/netbsd.h"

#include "libc/illumos.h"

#include "libc/haiku.h"
#include "libc/serenity.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc__included */
