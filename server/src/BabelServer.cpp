//
// Created by tom on 9/21/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "BabelServer.hpp"

auto BabelServer::refresh() -> int {
    try {
        if (server.isReady()) {
            auto connection = server.accept();
            clients.emplace_back(std::make_unique<Client>(connection));
        }
    } catch (ex::NetworkException &ex) {
        std::cerr << "Network error during accepting a client " << ex.what() << std::endl;
    }

    try {
        for (auto it = clients.begin(); it != clients.end(); ++it)
            if (!(**it).process(clients)) {
                clients.erase(it);
                break;
            }
    } catch (ex::NetworkException &ex) {
    std::cerr << "Network error during processing clients " << ex.what() << std::endl;
}

    return 0;
}

BabelServer::BabelServer(short port) : server(port) {
}
