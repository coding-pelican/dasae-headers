#include "test-thrd-channel/main.h"

T_use$((i8)(
    thrd_Que,
    thrd_Que_init,
    thrd_Que_fini,
    thrd_Que_isFull,
    thrd_Que_isEmpty,
    thrd_Que_isOpen,
    thrd_Que_trySend,
    thrd_Que_send,
    thrd_Que_sendFor,
    thrd_Que_tryRecv,
    thrd_Que_recv,
    thrd_Que_recvFor
));

TEST_fn_("thrd/Que: typed blocking and timed operations preserve values" $guard) {
    var cancel = thrd_CancelTok_Src_init();
    defer_(thrd_CancelTok_Src_fini(&cancel));
    let wakeable = thrd_CancelTok_wakeable(thrd_CancelTok_Src_tok(&cancel));

    var buf = A_zero$((A$$(2, i8)));
    var que = thrd_Que_init$i8(A_ref$((S$i8)(buf)));
    defer_(thrd_Que_fini$i8(&que));
    try_(TEST_expect(thrd_Que_isOpen$i8(&que)));
    try_(TEST_expect(thrd_Que_isEmpty$i8(&que)));
    try_(TEST_expect(!thrd_Que_isFull$i8(&que)));
    try_(thrd_Que_trySend$i8(&que, 7));
    try_(TEST_expect(try_(thrd_Que_tryRecv$i8(&que)) == 7));
    try_(thrd_Que_send$i8(&que, 8, wakeable));
    try_(TEST_expect(try_(thrd_Que_recv$i8(&que, wakeable)) == 8));
    try_(thrd_Que_sendFor$i8(
        &que, 9, wakeable, time_Dur_fromMillis(1)
    ));
    try_(TEST_expect(try_(thrd_Que_recvFor$i8(
        &que, wakeable, time_Dur_fromMillis(1)
    )) == 9));
    return_ok({});
} $unguarded(TEST_fn);
