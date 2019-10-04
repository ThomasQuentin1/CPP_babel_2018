//
// Created by tom on 9/21/19.
//

#ifndef BABEL_TCPSERVERNATIVE_HPP
#define BABEL_TCPSERVERNATIVE_HPP


#include <shared/types.hpp>
#include "shared/headers.hpp"
#include "shared/network/TcpConnectionNative.hpp"
#include "IPollable.hpp"
#include "ITcpServer.hpp"

namespace network {

    class TcpServerNative : public ITcpServer {
    public:
        explicit TcpServerNative(short port);
        TcpServerNative(TcpServerNative const &) = delete;
        ~TcpServerNative() override ;

        auto accept() -> std::shared_ptr<network::ITcpConnection> override ;
    };

}

#endif //BABEL_TCPSERVERNATIVE_HPP
