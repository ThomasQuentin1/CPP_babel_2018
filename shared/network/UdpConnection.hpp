//
// Created by tom on 9/18/19.
//

#ifndef BABEL_UDPCONNECTION_HPP
#define BABEL_UDPCONNECTION_HPP


#include <shared/types.hpp>
#include "IPollable.hpp"
#include "Timeout.hpp"
#include "IConnection.hpp"

namespace network {

    class UdpConnection : public IConnection, public IPollable {
    public:
        UdpConnection(std::string const &ip, short port, bool server);
        UdpConnection(const UdpConnection &) = delete;
        ~UdpConnection();

        auto sendData(bytes buffer, int size) -> int override ;
        auto recvData(int size) -> bytes override ;
    private:
        auto bind(short port) -> void;
        sockaddr_in addr;
        bytes recv_buffer = nullptr;
        int buffer_size = 0;
        Timeout timeout;
    };

}

#endif //BABEL_UDPCONNECTION_HPP
