//
// Created by tom on 9/18/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "TcpConnectionNative.hpp"


network::TcpConnectionNative::TcpConnectionNative(std::string const &ip, short port) : remote_addr({}) {
    this->s = socket(PF_INET, SOCK_STREAM, 0);
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
	if (!inet_pton(AF_INET, ip.c_str(), &remote_addr.sin_addr.s_addr))
        throw ex::NetworkException("Unable to resolve IP", "tcp connection");
    if (this->s == -1)
        throw ex::NetworkException("unable to open socket", "tcp connection");
    int status = connect(this->s, reinterpret_cast<struct sockaddr *>(&remote_addr), sizeof(remote_addr));
    if (status)
        throw ex::NetworkException("connection failed", "tcp connection");
    this->_ip = ip;
}

network::TcpConnectionNative::TcpConnectionNative(sock _s, sockaddr_in &_remote_addr) : remote_addr(_remote_addr) {
    this->s = _s;

    struct in_addr ipAddr = remote_addr.sin_addr;
    char str[46];
    inet_ntop( AF_INET, &ipAddr, str, 46 );
    this->_ip = std::string(str);
}

auto network::TcpConnectionNative::sendData(bytes buffer, int size) -> int {
	if (size == 0)
		return 0;
    int ret = ::send(this->s, (char const*)buffer, size, MSG_NOSIGNAL);
    if (ret <= 0)
        throw ex::NetworkException("client disconnected", "send tcp data");
    return ret;
}

auto network::TcpConnectionNative::recvData(int size) -> bytes {
	if (size == 0)
		return nullptr;
    if (this->buffer_size < size) {
        delete[] this->recv_buffer;
        this->recv_buffer = new byte[size + 1]();
        this->buffer_size = size;
    }
    int ret = ::recv(this->s, (char*)this->recv_buffer, size, 0);
    if (ret <= 0)
        throw ex::NetworkException("client disconnected", "recv tcp data");
    if (ret != size)
        return nullptr;
    auto retrn = this->recv_buffer;
    this->buffer_size = 0;
    this->recv_buffer = nullptr;
    return retrn;
}

network::TcpConnectionNative::~TcpConnectionNative() {
    closesocket(this->s);
}

auto network::TcpConnectionNative::recvAction() -> packet::ActionDyn {
    auto raw_action = this->recv<packet::action>();

    if (raw_action->body_size) {
        auto raw_body = this->recvData(raw_action->body_size);

        auto action = packet::ActionDyn(*raw_action, raw_body);
        delete[] raw_body;
        return action;
    } else {
        auto action = packet::ActionDyn(*raw_action);
        return action;
    }

}

auto network::TcpConnectionNative::sendAction(packet::operation op, std::string const &body) -> void {
    packet::action ac;

    ac.action_code = static_cast<char>(op);
    ac.body_size = !body.empty() ? body.size() + 1 : 0;
    this->send<packet::action>(ac);

    if (body.size())
        this->sendData(reinterpret_cast<bytes>(const_cast<char*>(body.c_str())), body.size() + 1);
}

auto network::TcpConnectionNative::ip() -> std::string const & {
    return this->_ip;
}
