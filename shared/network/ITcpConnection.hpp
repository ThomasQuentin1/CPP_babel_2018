//
// Created by tom on 10/2/19.
//

#ifndef BABEL_ITCPCONNECTION_HPP
#define BABEL_ITCPCONNECTION_HPP

#include "shared/headers.hpp"
#include "IPollable.hpp"
#include "IConnection.hpp"
#include "ActionDyn.hpp"

namespace network {

    class ITcpConnection : public IPollable, public IConnection {
    public:
        virtual ~ITcpConnection() = default;

        virtual auto ip() -> std::string const & = 0;
        virtual auto recvAction() -> packet::ActionDyn = 0;
        virtual auto sendAction(packet::operation op, std::string const &body = "") -> void = 0;
    };

}

#endif //BABEL_ITCPCONNECTION_HPP
