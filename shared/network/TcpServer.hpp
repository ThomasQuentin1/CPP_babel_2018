//
// Created by tom on 9/21/19.
//

#ifndef BABEL_TCPSERVER_HPP
#define BABEL_TCPSERVER_HPP


#include <shared/types.hpp>
#include "shared/network/TcpConnection.hpp"
#include "IPollable.hpp"

namespace network {

    class TcpServer : public IPollable {
    public:
        explicit TcpServer(short port);
        TcpServer(TcpServer const &) = delete;
        ~TcpServer();

        auto accept() -> std::shared_ptr<network::TcpConnection>;
    };

}

#endif //BABEL_TCPSERVER_HPP
