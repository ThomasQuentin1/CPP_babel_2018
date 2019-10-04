//
// Created by tom on 10/4/19.
//

#ifndef BABEL_ITCPSERVER_HPP
#define BABEL_ITCPSERVER_HPP

#include "IPollable.hpp"
#include "ITcpConnection.hpp"

namespace network {

    class ITcpServer : public IPollable {
    public:
        virtual ~ITcpServer() = default;
        virtual auto accept() -> std::shared_ptr<network::ITcpConnection> = 0;
    };

}

#endif //BABEL_ITCPSERVER_HPP
