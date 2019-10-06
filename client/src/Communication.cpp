//
// Created by tom on 9/23/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include <portaudio.h>
#include "shared/exceptions/UserException.hpp"
#include "Communication.hpp"
#include "audioConfig.h"

auto Communication::incommingCall() -> std::string const & {
    return this->incomming_call_username;
}

auto Communication::acceptCall() -> void {
    this->incomming_call_username = "";
    this->client_connection = std::make_shared<network::UdpConnectionNative>(this->incomming_call_ip, this->incomming_call_port, false);
    this->server_connection->sendAction(packet::operation::CALL_ACCEPT);
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

auto Communication::reciveSound() -> std::shared_ptr<SoundPacket> {
    if (this->recv_stack.empty())
        return nullptr;
    auto data = (this->recv_stack.front());
    recv_stack.pop_front();
    return data;
}

auto Communication::sendSound(std::shared_ptr<SoundPacket> packet) -> void {
    if (packet == nullptr)
        return;
    if (this->send_stack.size() > 100)
        this->send_stack.pop_front();
    this->send_stack.push_back((packet));
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
    this->server_connection = std::make_shared<network::TcpConnectionNative>(ip, port);
}

auto Communication::parseIncommingCall(std::string const &body) -> void {
    std::stringstream bodystream(body);

    std::string port_str;

    std::getline(bodystream, this->incomming_call_username);
    std::getline(bodystream, this->incomming_call_ip);
    std::getline(bodystream, port_str);
    this->incomming_call_port = std::stoi(port_str);

}

auto Communication::refreshServer(std::shared_ptr<packet::ActionDyn> action) -> void {
    if (action == nullptr)
        action = std::make_shared<packet::ActionDyn>(this->server_connection->recvAction());

    switch (action->code()) {
        case packet::operation::INCOMMING_CALL:
            this->parseIncommingCall(action->body());
            break;
        case packet::CALL_END:
            this->_isCalling = false;
            this->client_connection = nullptr;
            break;
        case packet::operation::CALL_ACCEPT:
			std::cout << "connecting to " << action->body() << ":" << this->outgoing_call_port << std::endl;
            this->client_connection = std::make_shared<network::UdpConnectionNative>(action->body(), this->outgoing_call_port, true);
			break;
        case packet::operation::DISCONNECT:
            throw ex::NetworkException("Server told you to disconnect", "communication refresh");
			break;
        default:
            std::cerr << "Unhandled operation recived " << action->code() << std::endl;
    }
}

auto Communication::refreshClient() -> void {
    if (this->client_connection->isReady()) {
        if (this->recv_stack.size() > 100)
            this->recv_stack.pop_front();
        std::shared_ptr<packet::data> data = this->client_connection->recv<packet::data>();
        if (data && data->magic == 0XDA) {
			std::cout << "Reciving audio data" << std::endl;
            std::shared_ptr<SoundPacket> packet = std::make_shared<SoundPacket>(sizeof(paFloat32) * audioConfig::frames_per_buffer * 10);
            packet->copyFrom(data->body, data->size);
            encoder->decode((packet));
            this->recv_stack.push_back(packet);
        }
    }
    if (!this->send_stack.empty()) {
		std::shared_ptr<SoundPacket> sound_packet = (send_stack.front());
		send_stack.pop_front();
		packet::data sound_data;
		sound_packet->copyTo(sound_data.body, sound_packet->dataSize());
		sound_data.size = sound_packet->dataSize();
		encoder->encode(sound_packet);
		this->client_connection->sendData((bytes)&sound_data, sizeof(packet::data));
    }
}

auto Communication::login(std::string const &loginAndPassword) -> bool
{
    this->server_connection->sendAction(packet::operation::LOGIN, loginAndPassword);
    auto action = this->server_connection->recvAction();
    if (action.code() == packet::operation::OK) {
        return (true);
    } else if (action.code() != packet::operation::KO) {
        this->refreshServer(std::make_shared<packet::ActionDyn>(action));
        return this->login(loginAndPassword);
    } else {
        return (false);
    }
}

auto Communication::signUp(std::string const &loginAndPassword) -> bool
{
    this->server_connection->sendAction(packet::operation::REGISTER, loginAndPassword);
    auto action = this->server_connection->recvAction();
    if (action.code() == packet::operation::OK) {
        return (true);
    } else if (action.code() != packet::operation::KO) {
        this->refreshServer(std::make_shared<packet::ActionDyn>(action));
        return this->signUp(loginAndPassword);
    } else
        return (false);
}

auto Communication::getOnlineUsers() -> std::string
{
    this->server_connection->sendAction(packet::operation::GET_CONTACTS);
    auto resp = this->server_connection->recvAction();
    if (resp.code() != packet::operation::DATA) {
        this->refreshServer(std::make_shared<packet::ActionDyn>(resp));
        return this->getOnlineUsers();
    }
    return resp.body();
}

auto Communication::endCall() -> void {
    this->server_connection->sendAction(packet::operation::CALL_END);
    this->client_connection = nullptr;
}

auto Communication::logout() -> void
{
    this->server_connection->sendAction(packet::operation::LOGOUT);
    this->server_connection->recvAction();
}
