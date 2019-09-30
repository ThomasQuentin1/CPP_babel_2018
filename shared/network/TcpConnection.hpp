//
// Created by tom on 9/18/19.
//

#ifndef BABEL_TCPCONNECTION_HPP
#define BABEL_TCPCONNECTION_HPP

#include <shared/types.hpp>
#include "IPollable.hpp"
#include "Timeout.hpp"
#include "IConnection.hpp"
#include "ActionDyn.hpp"
#include "shared/headers.hpp"


namespace network {

    class TcpConnection : public IPollable, public IConnection {
    public:
        TcpConnection(std::string const &ip, short port);
        explicit TcpConnection(sock s, sockaddr_in &_addr);
        TcpConnection(const TcpConnection &) = delete;
        TcpConnection(TcpConnection &&) = default;

        ~TcpConnection();

        auto ip() -> std::string const &;
        auto recvAction() -> packet::ActionDyn;
        auto sendAction(packet::operation op, std::string const &body = "") -> void;
    private:
        auto sendData(bytes buffer, int size) -> int override ;
        auto recvData(int size) -> bytes override ;
        sockaddr_in remote_addr;
        bytes recv_buffer = nullptr;
        int buffer_size = 0;
        std::string _ip;
        Timeout timeout;
    };

}

#endif //BABEL_TCPCONNECTION_HPP
