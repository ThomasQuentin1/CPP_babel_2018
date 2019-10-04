//
// Created by tom on 10/2/19.
//

#ifndef BABEL_IUDPCONNECTION_HPP
#define BABEL_IUDPCONNECTION_HPP



#include <shared/types.hpp>
#include "IPollable.hpp"
#include "Timeout.hpp"
#include "IConnection.hpp"

namespace network {

    class IUdpConnection : public IConnection, public IPollable {
    public:
        virtual ~IUdpConnection() = default;

        virtual auto sendData(bytes buffer, int size) -> int = 0;
        virtual auto recvData(int size) -> bytes = 0;
    };

}

#endif //BABEL_IUDPCONNECTION_HPP
