//
// Created by tom on 9/23/19.
//

#ifndef BABEL_COMMUNICATION_HPP
#define BABEL_COMMUNICATION_HPP

#include "shared/network/TcpConnectionNative.hpp"
#include "shared/network/UdpConnectionNative.hpp"
#include "shared/headers.hpp"
#include "shared/SoundPacket.hpp"
#include "shared/packets.hpp"
#include "IEncoder.hpp"
#include "Opus.hpp"


class Communication {
public:
    explicit Communication(std::string const &ip = "10.14.58.90", short port = 4242);

    auto refresh() -> void;

    // Incomming call function
    auto incommingCall() -> std::string const &; // username of the caller, empty if no call;
    auto acceptCall() -> void;
    auto declineCall() -> void;

    // Outgoing call function
    auto call(std::string const &name) -> void;
    auto endCall() -> void;
    auto isCalling() -> bool;
    auto isCommunicating() -> bool;

    // Sound functions
    auto reciveSound() -> std::shared_ptr<SoundPacket>;
    auto sendSound(std::shared_ptr<SoundPacket>) -> void;

    //connection
    auto login(std::string const &loginAndPassword) -> bool;
    auto signUp(std::string const &loginAndPassword) -> bool;
    auto logout() -> void;

    //users
    auto getOnlineUsers() -> std::string;

private:
    auto parseIncommingCall(std::string const &body) -> void;
    auto refreshServer(std::shared_ptr<packet::ActionDyn> action = nullptr) -> void;
    auto refreshClient() -> void;
    std::string incomming_call_username;
    std::string incomming_call_ip;
    short incomming_call_port = 0;
    std::string outgoing_call_ip;
    short outgoing_call_port = 0;
    std::shared_ptr<network::ITcpConnection> server_connection;
    std::shared_ptr<network::IUdpConnection> client_connection;
    std::deque<std::shared_ptr<SoundPacket>> send_stack;
    std::deque<std::shared_ptr<SoundPacket>> recv_stack;
    bool _isCalling = false;
    std::shared_ptr<IEncoder> encoder = std::make_unique<Opus>();
};


#endif //BABEL_COMMUNICATION_HPP
