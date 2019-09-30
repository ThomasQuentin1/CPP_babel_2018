//
// Created by tom on 9/21/19.
//

#include <shared/packets.hpp>
#include <shared/network/UdpConnection.hpp>
#include "shared/network/TcpConnection.hpp"
#include "shared/network/WindowsInit.hpp"


void test_tcp_send();
void test_udp_send();
void test_udp_recv();

void console();

int main(int ac, char **av)
{
	network::WindowsInit wsa_init;

    // Low level tests

    //test_tcp_send();
    //test_udp_send();
    //test_udp_recv();

    // high level tests
    console();
}