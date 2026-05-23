#include "dh-main.h"
#include "dh/net.h"
#include "dh/io.h"

TEST_fn_("net: loopback listen connect accept and exchange bytes" $scope) {
    var listen_addr = net_IpAddr_ip4Loopback(0);
    var server = try_(net_listenIp(&listen_addr, net_ListenOpts_default));

    try_(TEST_expect(server.socket.addr.family == net_Addr_Family_ip4));
    try_(TEST_expect(server.socket.addr.ip4.port != 0));

    var connect_addr = net_IpAddr_ip4Loopback(server.socket.addr.ip4.port);
    var client = try_(net_Clt_connect(&connect_addr, net_ConnectOpts_default));

    var peer = try_(net_Svr_accept(&server));

    var client_io = net_Stream_io(*net_Clt_stream(&client));
    var peer_io = net_Stream_io(peer);

    try_(io_Writer_writeBytes(net_Stream_IO_writer(&client_io), u8_l("ping")));

    var_(buf, A$$(4, u8)) $undefined;
    try_(io_Reader_readExact(net_Stream_IO_reader(&peer_io), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("ping"))));

    try_(io_Writer_writeBytes(net_Stream_IO_writer(&peer_io), u8_l("pong")));
    var_(reply, A$$(4, u8)) $undefined;
    try_(io_Reader_readExact(net_Stream_IO_reader(&client_io), A_ref$((S$u8)(reply))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(reply)), u8_l("pong"))));

    net_Stream_close(peer);
    net_Clt_close(&client);
    net_Svr_close(&server);
} $unscoped(TEST_fn);

TEST_fn_("net: ipv6 loopback listen connect accept and exchange bytes" $scope) {
    var listen_addr = net_IpAddr_ip6Loopback(0);
    var server = try_(net_listenIp(&listen_addr, net_ListenOpts_default));

    try_(TEST_expect(server.socket.addr.family == net_Addr_Family_ip6));
    try_(TEST_expect(server.socket.addr.ip6.port != 0));

    var connect_addr = net_IpAddr_ip6Loopback(server.socket.addr.ip6.port);
    var client = try_(net_Clt_connect(&connect_addr, net_ConnectOpts_default));
    var peer = try_(net_Svr_accept(&server));

    var client_io = net_Stream_io(*net_Clt_stream(&client));
    var peer_io = net_Stream_io(peer);

    try_(io_Writer_writeBytes(net_Stream_IO_writer(&client_io), u8_l("ip6")));

    var_(buf, A$$(3, u8)) $undefined;
    try_(io_Reader_readExact(net_Stream_IO_reader(&peer_io), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("ip6"))));

    net_Stream_close(peer);
    net_Clt_close(&client);
    net_Svr_close(&server);
} $unscoped(TEST_fn);

TEST_fn_("net: nonblocking accept returns WouldBlock before peer connects" $scope) {
    var listen_addr = net_IpAddr_ip4Loopback(0);
    var opts = net_ListenOpts_default;
    opts.nonblocking = true;
    var server = try_(net_listenIp(&listen_addr, opts));

    let accepted_would_block = eval_(bool $scope)(catch_((net_Svr_accept(&server))(err, {
        try_(TEST_expect(mem_eqlBytes(E_strfy(&err), u8_l("WouldBlockNet"))));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(accepted_would_block));

    net_Svr_close(&server);
} $unscoped(TEST_fn);

TEST_fn_("net: connect accepts nonzero timeout on loopback" $scope) {
    var listen_addr = net_IpAddr_ip4Loopback(0);
    var server = try_(net_listenIp(&listen_addr, net_ListenOpts_default));
    var connect_addr = net_IpAddr_ip4Loopback(server.socket.addr.ip4.port);

    var opts = net_ConnectOpts_default;
    opts.timeout = time_Duration_fromMillis(50);
    var client = try_(net_Clt_connect(&connect_addr, opts));
    var peer = try_(net_Svr_accept(&server));

    var client_io = net_Stream_io(*net_Clt_stream(&client));
    var peer_io = net_Stream_io(peer);

    try_(io_Writer_writeBytes(net_Stream_IO_writer(&peer_io), u8_l("ok")));
    var_(buf, A$$(2, u8)) $undefined;
    try_(io_Reader_readExact(net_Stream_IO_reader(&client_io), A_ref$((S$u8)(buf))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(buf)), u8_l("ok"))));

    net_Stream_close(peer);
    net_Clt_close(&client);
    net_Svr_close(&server);
} $unscoped(TEST_fn);
