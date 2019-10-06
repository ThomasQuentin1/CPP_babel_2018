//
// Created by tom on 9/21/19.
//

#ifndef BABEL_BABELSERVER_HPP
#define BABEL_BABELSERVER_HPP


#include <deque>
#include <shared/network/TcpConnectionNative.hpp>
#include <shared/network/TcpServerNative.hpp>
#include "Client.hpp"

class BabelServer {
public:
    explicit BabelServer(short port);
    auto refresh() -> int;

private:
    std::unique_ptr<network::ITcpServer> server;
    std::deque<std::shared_ptr<Client>> clients;
};


#endif //BABEL_BABELSERVER_HPP
