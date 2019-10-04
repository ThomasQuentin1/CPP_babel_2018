//
// Created by tom on 9/21/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "TcpServerNative.hpp"

network::TcpServerNative::TcpServerNative(short port) {
    this->s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0};

    if (this->s <= 0)
        throw ex::NetworkException("unable to open socket", "tcp server open");
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (bind(this->s, (struct sockaddr *) &server, sizeof(server))) {
        closesocket(this->s);
        throw ex::NetworkException("unable to use address", "tcp server open");
    }
    if (listen(this->s, 100)) {
        closesocket(this->s);
        throw ex::NetworkException("unable to listen", "tcp server open");
    }
}

auto network::TcpServerNative::accept() -> std::shared_ptr<network::ITcpConnection> {
    struct sockaddr_in server = {0};
    socklen_t size = sizeof(server);

    sock clientSock = ::accept(this->s, (struct sockaddr *)&server, &size);
    if (clientSock == -1)
        throw ex::NetworkException("failed to accept client", "tcp server accept");
    return std::make_shared<TcpConnectionNative>(clientSock, server);
}

network::TcpServerNative::~TcpServerNative() {
    closesocket(this->s);
}
