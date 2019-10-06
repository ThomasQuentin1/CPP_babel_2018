//
// Created by tom on 9/21/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include <server/src/singleton/Database.hpp>
#include <shared/packets.hpp>
#include "Client.hpp"
#include "shared/headers.hpp"
#include <algorithm>

Client::Client(std::shared_ptr<network::ITcpConnection> &_connection) : connection((_connection)) {
    std::cout << "[" + this->username + "]" << std::setw(20) << "connected" << std::endl;
}

auto Client::isLoggedIn() -> bool {
    return this->loggedIn;
}

auto Client::process(std::deque<std::shared_ptr<Client>> &clients) -> bool try {
    if (!connection->isReady())
        return true;

    auto data = connection->recvAction();

    if (data.code() != packet::operation::LOGIN && data.code() != packet::operation::REGISTER && !this->loggedIn) {
        this->connection->sendAction(packet::operation::KO, "login first");
        return true;
    }

    switch (data.code()) {

        case packet::operation::LOGIN:
            this->login(data.body());
            break;

        case packet::operation::REGISTER:
            this->register_(data.body());
            break;

        case packet::operation::CALL_START:
            this->startCall(data.body(), clients);
            break;

        case packet::operation::CALL_ACCEPT:
            this->acceptCall(clients);
            break;

        case packet::operation::DISCONNECT:
            return false;

        case packet::operation::CALL_END:
            this->callEnd(clients);
            break;

        case packet::operation::GET_CONTACTS:
            this->connection->sendAction(packet::operation::DATA, this->listUsers(clients));
            break;

        case packet::operation::LOGOUT:
            this->connection->sendAction(packet::operation::OK);
            std::cout << "[" + this->username + "]" << std::setw(20) << "logged out " << std::endl;
            this->username = "unknown";
            this->loggedIn = false;
            break;

        default:
            this->connection->sendAction(packet::operation::KO, "invalid operation");
            break;
    }
    return true;
} catch (ex::NetworkException &) {
    std::cout << "[" + this->username + "]" << std::setw(20) << "disconnected" << std::endl;
    return false;
} catch (...) {
    std::cout << "[" + this->username + "]" << std::setw(20) << "crashed !" << std::endl;
    return false;
}

auto Client::login(std::string const &user) -> int {

    std::string _username;
    std::string _password;

    {
        std::stringstream userstream(user);
        std::getline(userstream, _username);
        std::getline(userstream, _password);
    }

    switch (Database::get().read(_username, _password)) {
        case 2:
            this->loggedIn = true;
            this->username = _username;
            this->connection->sendAction(packet::operation::OK, "welcome " + username);
            std::cout << "[" + this->username + "]" << std::setw(20) << "logged in" << std::endl;
            return 2; // Ok
        case 1:
            this->connection->sendAction(packet::operation::KO, "invalid password");
            std::cout << "[" + this->username + "]" << std::setw(20) << "invalid password" << std::endl;
            return 1; // only bad pass
        case 0:
            this->connection->sendAction(packet::operation::KO, "invalid username");
            std::cout << "[" + this->username + "]" << std::setw(20) << "invalid username " << _username << std::endl;
            return 0; // bad user/pass
    }
    return 0;
}

auto Client::operator==(std::string const &name) -> bool {
    return name == this->username;
}

auto Client::startCall(std::string const &body, std::deque<std::shared_ptr<Client>> &clients) -> void {
    std::string port;
    std::string target_name;

    {
        std::stringstream bodystream(body);
        std::getline(bodystream, target_name);
        std::getline(bodystream, port);
    }

    auto target = std::find(clients.begin(), clients.end(), target_name);

    if (target == clients.end())
        this->connection->sendAction(packet::operation::KO, "client not found");
    else {
        (*target)->connection->sendAction(packet::operation::INCOMMING_CALL,
                this->username + "\n" + this->connection->ip() + "\n" + port);
        (*target)->inCallWith = this->username;
        this->connection->sendAction(packet::operation::OK);
        inCallWith = target_name;
        std::cout << "[" + this->username + "]" << std::setw(20) << "calling " << inCallWith << std::endl;
    }
}

auto Client::register_(std::string const &body) -> int {
    std::string _user;
    std::string _pass;

    {
        std::stringstream bodystream(body);
        std::getline(bodystream, _user);
        std::getline(bodystream, _pass);
    }

    switch (Database::get().read(_user, _pass)) {
        case 2:
            this->connection->sendAction(packet::operation::KO, "Please login");
            break;
        case 1:
            this->connection->sendAction(packet::operation::KO, "User already exists");
            break;
        case 0:
            Database::get().write(_user, _pass);
            this->connection->sendAction(packet::operation::OK);
            this->username = _user;
            this->loggedIn = true;
            std::cout << "[" + this->username + "]" << std::setw(20) << "registered" << std::endl;
            break;
    }
    return 0;
}

Client::~Client() {
    try {
        this->connection->sendAction(packet::operation::DISCONNECT);
    } catch (...) {} // Ignore error if can't send packet
}

auto Client::callEnd(std::deque<std::shared_ptr<Client>> &clients) -> void {
    auto incall = std::find(clients.begin(), clients.end(), inCallWith);
    if (incall != clients.end())
        (*incall)->connection->sendAction(packet::operation::CALL_END);
    std::cout << "[" + this->username + "]" << std::setw(20) << "ending call with " << inCallWith << std::endl;
    inCallWith = "";
}

auto Client::listUsers(std::deque<std::shared_ptr<Client>> &clients) -> std::string {
    std::string list;
    int i = 0;

    for (auto const &c : clients) {
        if ((!(c == this->username)) && (!(c == "unknown"))) {
            list += c->username;
            list += "\n";
            i++;
        }
    }
    std::cout << "[" + this->username + "]" << std::setw(20) << "got a list of " << i << std::endl;
    return (!list.empty() && list[list.size() - 1] == '\n') ? list.substr(0, list.size() - 1) : list;
}

auto Client::acceptCall(std::deque<std::shared_ptr<Client>> &clients) -> void {
    auto target = std::find(clients.begin(), clients.end(), inCallWith);

    if (target != clients.end()) {
        (*target)->connection->sendAction(packet::operation::CALL_ACCEPT, this->connection->ip());
        this->connection->sendAction(packet::operation::OK);
        std::cout << "[" + this->username + "]" << std::setw(20) << "call accepted with " << inCallWith << std::endl;
    }
}

auto operator==(std::shared_ptr<Client> const &c, std::string const &s) -> bool {
    return *c == s;
}
