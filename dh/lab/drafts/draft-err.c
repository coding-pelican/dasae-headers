#include "dh/core.h"

/* Current Approach: Interface-based */
/* config_ErrSet(
    math_E,
    DivisionByZero,
    Overflow,
    Underflow
); */
// enum_err(
//     math_E,
//     DivisionByZero,
//     Overflow,
//     Underflow
// );
/* const char* E_type() { return "math_E"; }
const char* E_context() { return "Overflow"; } */

/* Approach 1: String Constants Instead of Interfaces  */
typedef struct Err {
    const char* type;
    const char* context;
} Err;

/* case 1: Based Constants */
static const Err E_NotImplemented = {
    .type = "Err",
    .context = "NotImplemented",
};
static const Err E_Unknown = {
    .type = "Err",
    .context = "Unknown",
};

/* case 2: Based Functions */
typedef Err math_E;

extern const math_E* math_E_DivisionByZero(void);
extern const math_E* math_E_Overflow(void);
extern const math_E* math_E_Underflow(void);

const math_E* math_E_DivisionByZero(void) {
    static const math_E error[1] = { {
        .type = "math_E",
        .context = "DivisionByZero",
    } };
    return error;
}
const math_E* math_E_Overflow(void) {
    static const math_E error[1] = { {
        .type = "math_E",
        .context = "Overflow",
    } };
    return error;
}
const math_E* math_E_Underflow(void) {
    static const math_E error[1] = { {
        .type = "math_E",
        .context = "Underflow",
    } };
    return error;
}

/* Approach 2: Use bit flags for ErrSet */
typedef struct io_E {
    usize NotImplemented            : 1;
    usize Unknown                   : 1;
    usize FileNotFound              : 1;
    usize AccessDenied              : 1;
    usize InvalidPath               : 1;
    usize NetworkError              : 1;
    usize Timeout                   : 1;
    usize ConnectionRefused         : 1;
    usize HostUnreachable           : 1;
    usize AddressFamilyNotSupported : 1;
    usize ProtocolNotSupported      : 1;
    usize OperationAborted          : 1;
    usize ResourceBusy              : 1;
    usize NotConnected              : 1;
    usize IsADirectory              : 1;
    usize NotAFile                  : 1;
    usize TooManyLinks              : 1;
    usize FileTooLarge              : 1;
    usize NoSpaceLeftOnDevice       : 1;
    usize DiskQuotaExceeded         : 1;
    usize FilesystemNotSupported    : 1;
    usize OperationNotPermitted     : 1;
} io_E;

// 1. 설정된 에러의 정보는 immutable 해야함.
// 2. 여러 개의 에러를 동시에 처리할 수 있어야 함.
// 3. 에러 코드가 중복되지 않아야 함.
// 4. 에러는 보통 E$T와 같은 result union 타입으로 반환되므로 8-16 바이트 이내의 크기를 가져야 함. (오버헤드를 최소화하기 위함)
// 5. 메모리 할당의 결정권은 라이브러리 사용자에게 있으므로 절대 동적할당(malloc 등)을 사용하면 안됨됨.


// 5. math_E, io_E, thrd_Err, engine_CanvasDrawErr 등 다양한 에러들이 정의되었을 때, 각각의 에러 코드가 서로 다른 범위를 가지도록 설계되어야 함.

/* E$P$engine_RenderBackend NotepadBackend_create(void) {
    scope_reserveReturn(E$P$engine_RenderBackend) {
        let backend = (engine_NotepadBackend*)malloc(sizeof(engine_NotepadBackend));
        if (!backend) {
            return_err(NotepadBackendE_OutOfMemoryNotepadBacked));
        }
        errdefer(free(backend));
        pri_memset(backend, 0, sizeof(engine_NotepadBackend));

        let capacity = 160ull * 100ull;
        let buffer   = (wchar*)malloc(capacity * sizeof(wchar));
        if (!buffer) {
            return_err(NotepadBackendE_OutOfMemoryNotepadBackendBuffer));
        }
        errdefer(free(buffer));
        pri_memset(buffer, 0, capacity * sizeof(wchar));

        backend->buffer          = buffer;
        backend->buffer_capacity = capacity;
        backend->buffer_size     = 0;

        // Create test message
        const wchar* test_msg = L"Hello, notepad!\n";
        wcscpy(backend->buffer, test_msg);
        backend->buffer_size = wcslen(test_msg);

        // Launch notepad and store handle
        backend->notepad_handle = catch (NotepadBackend_launchNotepad(), err, {
            return_err(err);
        });

        // Set up backend interface
        backend->base = local_({
            var vt           = engine_RenderBackend_createNoOp();
            vt.type          = engine_RenderBackendType_custom;
            vt.destroy       = NotepadBackend_destroy;
            vt.presentBuffer = NotepadBackend_presentBuffer;
            local__return vt;
        });
        return_ok(&backend->base);
    }
    scope_returnReserved;
} */
