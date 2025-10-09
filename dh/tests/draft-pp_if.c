#include "dh/prl.h"

#define pp_if_(Cond...)            pp_join(_, __pp_if, Cond)
#define __pp_if_1(_Than, _Else...) pp_expand _Than
#define __pp_if_0(_Than, _Else)    pp_expand _Else
#define pp_than_
#define pp_else_

#define target_1 0
#define target_2 1
#define target_3 0
#define target_4 0
#define target_5 0

typedef pp_if_(target_1)(
    pp_than_(i8),
    pp_else_(pp_if_(target_2)(
        pp_than_(i16),
        pp_else_(pp_if_(target_3)(
            pp_than_(i32),
            pp_else_(pp_if_(target_4)(
                pp_than_(i64),
                pp_else_(isize)
            ))
        ))
    ))
) MyInt;

#include "dh/os/windows.h"

#define show_legacy 0
pp_if_(show_legacy)(
    pp_than_(
#if bti_plat_windows
        typedef HANDLE posix_fd_t;
#elif bti_plat_unix
        typedef fd_t posix_fd_t;
#elif bti_plat_wasi
        typedef my_fd_t posix_fd_t;
#else
        typedef i32 posix_fd_t;
#endif
    ),
    pp_else_(
        typedef pp_if_(bti_plat_windows)(
            pp_than_(HANDLE),
            pp_else_(pp_if_(bti_plat_unix)(
                pp_than_(fd_t),
                pp_else_(pp_if_(bti_plat_wasi)(
                    pp_than_(my_fd_t),
                    pp_else_(i32)
                ))
            ))
        ) posix_fd_t;
    )
)
