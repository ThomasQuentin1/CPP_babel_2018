//
// Created by tom on 9/23/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "shared/exceptions/UserException.hpp"
#include "Communication.hpp"
#include "audioConfig.h"

auto Communication::incommingCall() -> std::string const & {
    return this->incomming_call_username;
}

auto Communication::acceptCall() -> void {
    this->incomming_call_username = "";
    this->client_connection = std::make_unique<network::UdpConnection>(this->incomming_call_ip, this->incomming_call_port, false);
}

auto Communication::declineCall() -> void {
    this->incomming_call_username = "";
    this->server_connection->sendAction(packet::operation::CALL_END);
}

auto Communication::call(std::string const &name) -> void {
    this->outgoing_call_port = rand()%5000+1000;
    this->_isCalling = true;
    this->server_connection->sendAction(packet::operation::CALL_START, name + "\n" + std::to_string(this->outgoing_call_port));

    auto response = server_connection->recvAction();
    if (response.code() != packet::operation::OK)
        throw ex::UserException("Call failed : " + response.body(), "communication call");
}

auto Communication::isCalling() -> bool {
    return _isCalling;
}

auto Communication::reciveSound() -> std::unique_ptr<SoundPacket> {
    if (this->recv_stack.empty())
        return nullptr;
    auto data = std::move(this->recv_stack.front());
    recv_stack.pop_front();
    return data;
}

auto Communication::sendSound(std::unique_ptr<SoundPacket> packet) -> void {
    if (packet == nullptr)
        return;
    if (this->send_stack.size() > 100)
        this->send_stack.pop_front();
    this->send_stack.push_back(std::move(packet));
}

auto Communication::isCommunicating() -> bool {
    return this->client_connection != nullptr;
}

auto Communication::refresh() -> void {
    if (this->server_connection->isReady())
        this->refreshServer();
    if (this->client_connection != nullptr)
        this->refreshClient();
}

Communication::Communication(std::string const &ip, short port) {
    this->server_connection = std::make_unique<network::TcpConnection>(ip, port);
}

auto Communication::parseIncommingCall(std::string const &body) -> void {
    std::stringstream bodystream(body);

    std::string port_str;

    std::getline(bodystream, this->incomming_call_username);
    std::getline(bodystream, this->incomming_call_ip);
    std::getline(bodystream, port_str);
    this->incomming_call_port = std::stoi(port_str);

}

auto Communication::refreshServer() -> void {
    auto action = this->server_connection->recvAction();

    switch (action.code()) {
        case packet::operation::INCOMMING_CALL:
            this->parseIncommingCall(action.body());
            break;
        case packet::CALL_END:
            this->_isCalling = false;
            break;
        case packet::operation::DISCONNECT:
            throw ex::NetworkException("Server told you to disconnect", "communication refresh");
        default:
            std::cerr << "Unhandled operation recived" << std::endl;
    }
}

auto Communication::refreshClient() -> void {
    if (this->client_connection->isReady()) {
        if (this->recv_stack.size() > 100)
            this->recv_stack.pop_front();
        auto data = this->client_connection->recv<packet::data>();
        if (data->magic == 0XDA) {
            auto packet = std::make_unique<SoundPacket>(audioConfig::frames_per_buffer * sizeof(double));
            packet->copyFrom(data->body, data->size);
            this->recv_stack.push_back(std::move(packet));
        }
    }

    if (!this->send_stack.empty()) {
        std::unique_ptr<SoundPacket> sound_packet = std::move(this->send_stack.front());
        send_stack.pop_front();
        this->client_connection->sendData(sound_packet->ptr(), sound_packet->dataSize());
    }
}
