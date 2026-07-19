#pragma once
#ifndef io_evtd_included
#define io_evtd_included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "io-evtd/CP.h"
#include "io-evtd/Uring.h"
#include "io-evtd/proactor.h"

#include "io-evtd/Posix.h"
#include "io-evtd/Kqueue.h"
#include "io-evtd/Epoll.h"
#include "io-evtd/reactor.h"

#include "io-evtd/Mpxr.h"
#include "io-evtd/Batch.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_evtd_included */
