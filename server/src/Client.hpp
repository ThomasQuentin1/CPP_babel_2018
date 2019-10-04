//
// Created by tom on 9/21/19.
//

#ifndef BABEL_CLIENT_HPP
#define BABEL_CLIENT_HPP


#include <shared/network/TcpConnectionNative.hpp>
#include <shared/network/UdpProxy.hpp>
#include <deque>

class Client {
public:
    explicit Client(std::shared_ptr<network::ITcpConnection> &_connection);

    Client(Client const &) = delete; // Copy
    Client(Client &&) = default; // move
    ~Client();

    auto isLoggedIn() -> bool;
    auto process(std::deque<std::shared_ptr<Client>> &clients) -> bool; // Return false if client is disconnected

    auto operator==(std::string const &name) -> bool;

private:
    // Functions
    auto login(std::string const &body) -> int;
    auto register_(std::string const &body) -> int;
    auto callEnd(std::deque<std::shared_ptr<Client>> &) -> void;
    auto startCall(std::string const &body, std::deque<std::shared_ptr<Client>> &clients) -> void;
    auto listUsers(std::deque<std::shared_ptr<Client>>&) -> std::string;
    auto acceptCall(std::deque<std::shared_ptr<Client>>&) -> void;

    // Internal variables
    bool loggedIn = false;
    std::shared_ptr<network::ITcpConnection> connection;
    std::shared_ptr<network::UdpProxy> proxy = nullptr;
    std::string inCallWith;

    // Client infos
    std::string username = "unknown";
};

auto operator==(std::shared_ptr<Client> const &, std::string const &) -> bool;

#endif //BABEL_CLIENT_HPP
