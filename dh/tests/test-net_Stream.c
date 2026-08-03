#include "dh-main.h"
#include "dh/net.h"
#include "dh/io.h"
#include "dh/mem/common.h"

TEST_fn_("net/Stream: loopback peers exchange bytes through IO adapters" $guard) {
    let listen_addr = net_IpAddr_ip4Loopback(0);
    var server = try_(net_listenIp(listen_addr, net_ListenOpts_default));
    defer_(net_Svr_close(server));
    let port = union_to((server.socket.addr)(net_Addr_Family_ip4)).port;
    var client = try_(net_Clt_connect(
        net_IpAddr_ip4Loopback(port), net_ConnectOpts_default
    ));
    defer_(net_Clt_close(client));
    var peer = try_(net_Svr_accept(server));
    defer_(net_Stream_close(peer));
    var client_io = net_Stream_io(net_Clt_stream(client));
    var peer_io = net_Stream_io(peer);

    try_(io_Writer_writeBytes(net_Stream_IO_writer(&client_io), u8_l("ping")));
    var_(buf, A$$(4, u8)) $undefined;
    try_(io_Reader_readExact(
        net_Stream_IO_reader(&peer_io), A_ref$((S$u8)(buf))
    ));
    try_(TEST_expect(mem_eqlBytes(
        A_ref$((S_const$u8)(buf)), u8_l("ping")
    )));
} $unguarded(TEST_fn);
