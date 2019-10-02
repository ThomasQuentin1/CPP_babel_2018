//
// Created by tom on 9/18/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "UdpConnectionNative.hpp"

network::UdpConnectionNative::UdpConnectionNative(std::string const &ip, short port, bool server) {
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s <= 0)
        throw ex::NetworkException("Can't open socket", "UDP connection");

    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    if (server)
        bind(port);
}

auto network::UdpConnectionNative::sendData(bytes buffer, int size) -> int {
    //timeout.check();
	if (size == 0)
		return 0;

    int ret = sendto(this->s, (char const *)buffer, size, MSG_NOSIGNAL, reinterpret_cast<const sockaddr*>(&this->addr), sizeof(this->addr));
    //if (ret <= 0)
    //    throw ex::NetworkException("client disconnected", "send data");
    return ret;
}

auto network::UdpConnectionNative::recvData(int size) -> bytes {
    //timeout.check();

	if (size == 0)
		return nullptr;
    socklen_t addrsize = sizeof(this->addr);

    if (this->buffer_size < size) {
        delete[] this->recv_buffer;
        this->recv_buffer = new byte[size + 1]();
        this->buffer_size = size;
    }
    int ret = recvfrom(this->s, (char*)this->recv_buffer, size, MSG_NOSIGNAL, reinterpret_cast<sockaddr*>(&this->addr),
                       &addrsize);
    //if (ret < 0)
    //    throw ex::NetworkException("client disconnected", "recv data");
    if (ret != size)
        return nullptr;
    auto retrn = this->recv_buffer;
    this->buffer_size = 0;
    this->recv_buffer = nullptr;
    //timeout.refresh();
    return retrn;
}

network::UdpConnectionNative::~UdpConnectionNative() {
    closesocket(this->s);
}

auto network::UdpConnectionNative::bind(short port) -> void {
    struct sockaddr_in servaddr = {};

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
	//servaddr.sin_port = htons(8091);
    int res = ::bind(s, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr));
    if (res)
        throw ex::NetworkException("Can't use address", "UDP connection");
}
