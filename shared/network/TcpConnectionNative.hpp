//
// Created by tom on 9/18/19.
//

#ifndef BABEL_TCPCONNECTIONNATIVE_HPP
#define BABEL_TCPCONNECTIONNATIVE_HPP

#include <shared/types.hpp>
#include "IPollable.hpp"
#include "Timeout.hpp"
#include "IConnection.hpp"
#include "ActionDyn.hpp"
#include "shared/headers.hpp"
#include "ITcpConnection.hpp"


namespace network {

    class TcpConnectionNative : public ITcpConnection {
    public:
        TcpConnectionNative(std::string const &ip, short port);
        explicit TcpConnectionNative(sock s, sockaddr_in &_addr);
        TcpConnectionNative(const TcpConnectionNative &) = delete;
        TcpConnectionNative(TcpConnectionNative &&) = default;

        ~TcpConnectionNative();

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

#endif //BABEL_TCPCONNECTIONNATIVE_HPP
