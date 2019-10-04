//
// Created by tom on 9/18/19.
//

#ifndef BABEL_UDPCONNECTIONNATIVE_HPP
#define BABEL_UDPCONNECTIONNATIVE_HPP


#include <shared/types.hpp>
#include "IPollable.hpp"
#include "Timeout.hpp"
#include "IConnection.hpp"
#include "IUdpConnection.hpp"

namespace network {

    class UdpConnectionNative : public IUdpConnection {
    public:
        UdpConnectionNative(std::string const &ip, short port, bool server);
        UdpConnectionNative(const UdpConnectionNative &) = delete;
        ~UdpConnectionNative();

        auto sendData(bytes buffer, int size) -> int override ;
        auto recvData(int size) -> bytes override ;
    private:
        auto bind(short port) -> void;
        sockaddr_in addr;
        bytes recv_buffer = nullptr;
        int buffer_size = 0;
        //Timeout timeout;
    };

}

#endif //BABEL_UDPCONNECTIONNATIVE_HPP
