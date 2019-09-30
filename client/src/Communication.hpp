//
// Created by tom on 9/23/19.
//

#ifndef BABEL_COMMUNICATION_HPP
#define BABEL_COMMUNICATION_HPP

#include "shared/network/TcpConnection.hpp"
#include "shared/network/UdpConnection.hpp"
#include <shared/SoundPacket.hpp>
#include "shared/headers.hpp"


class Communication {
public:
    explicit Communication(std::string const &ip = "127.0.0.1", short port = 9081);

    auto refresh() -> void;

    // Incomming call function
    auto incommingCall() -> std::string const &; // username of the caller, empty if no call;
    auto acceptCall() -> void;
    auto declineCall() -> void;

    // Outgoing call function
    auto call(std::string const &name) -> void;
    auto isCalling() -> bool;
    auto isCommunicating() -> bool;

    // Sound functions
    auto reciveSound() -> std::shared_ptr<SoundPacket>;
    auto sendSound(std::shared_ptr<SoundPacket>) -> void;

private:
    auto parseIncommingCall(std::string const &body) -> void;
    auto refreshServer() -> void;
    auto refreshClient() -> void;

    std::string incomming_call_username;
    std::string incomming_call_ip;
    short incomming_call_port = 0;

    std::string outgoing_call_ip;
    short outgoing_call_port = 0;

    std::shared_ptr<network::TcpConnection> server_connection;
    std::shared_ptr<network::UdpConnection> client_connection;

    std::deque<std::shared_ptr<SoundPacket>> send_stack;
    std::deque<std::shared_ptr<SoundPacket>> recv_stack;

    bool _isCalling = false;

};


#endif //BABEL_COMMUNICATION_HPP
