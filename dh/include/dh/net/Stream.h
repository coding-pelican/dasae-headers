#ifndef net_Stream__included
#define net_Stream__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Sock.h"
#include "../io/Reader.h"
#include "../io/Writer.h"

/*========== Macros and Declarations ========================================*/

struct net_Stream {
    var_(socket, net_Sock);
};
T_impl_E$(net_Stream);

$extern fn_((net_Stream_close(net_Stream self))(void));
$extern fn_((net_Stream_shutdown(net_Stream self, net_ShutdownHow how))(E$void));
$extern fn_((net_Stream_read(net_Stream self, S$u8 buf))(E$usize));
$extern fn_((net_Stream_write(net_Stream self, S_const$u8 bytes))(E$usize));

T_alias$((net_Stream_IO)(struct net_Stream_IO {
    var_(stream, net_Stream);
}));
$extern fn_((net_Stream_io(net_Stream stream))(net_Stream_IO));
$extern fn_((net_Stream_IO_reader(net_Stream_IO* self))(io_Reader));
$extern fn_((net_Stream_IO_writer(net_Stream_IO* self))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_Stream__included */
