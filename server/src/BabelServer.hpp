//
// Created by tom on 9/21/19.
//

#ifndef BABEL_BABELSERVER_HPP
#define BABEL_BABELSERVER_HPP


#include <deque>
#include <shared/network/TcpConnection.hpp>
#include <shared/network/TcpServer.hpp>
#include "Client.hpp"

class BabelServer {
public:
    BabelServer(short port);
    auto refresh() -> int;

private:
    network::TcpServer server;
    std::deque<std::unique_ptr<Client>> clients;
};


#endif //BABEL_BABELSERVER_HPP
