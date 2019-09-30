//
// Created by tom on 9/22/19.
//

#include <shared/types.hpp>
#include <shared/network/TcpConnection.hpp>
#include <shared/network/UdpConnection.hpp>
#include "shared/headers.hpp"
#include <string.h>

struct testPacket {
    byte data[255];
};

void test_tcp_send()
{
    network::TcpConnection sok("127.0.0.1", 8190);

    testPacket to_send;
    testPacket to_recv;

    memset(to_send.data, 0, sizeof(to_send.data));
    memcpy(to_send.data, "Bonjour, ceci est un test de data", strlen("Bonjour, ceci est un test de data"));
    sok.send<testPacket>(to_send);
}

void test_udp_send()
{
    network::UdpConnection sok("127.0.0.1", 8190, false);

    testPacket to_send;

    memset(to_send.data, 0, sizeof(to_send.data));
    memcpy(to_send.data, "Bonjour, ceci est un test de data", strlen("Bonjour, ceci est un test de data"));
    sok.send<testPacket>(to_send);
    std::shared_ptr<testPacket> to_recv = sok.recv<testPacket>();
    std::cout << to_recv->data << std::endl;
}

void test_udp_recv()
{
    network::UdpConnection sok("127.0.0.1", 8190, true);

    auto to_recv = sok.recv<testPacket>();
    std::cout << to_recv->data << std::endl;

    testPacket to_send;

    memset(to_send.data, 0, sizeof(to_send.data));
    memcpy(to_send.data, "J'ai bien recu !", strlen("J'ai bien recu !"));

    sok.send(to_send);

}
