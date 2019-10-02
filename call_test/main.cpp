//
// Created by tom on 9/30/19.
//

#include <shared/network/UdpConnection.hpp>
#include "shared/headers.hpp"
#include <client/src/PortAudioRecorder.hpp>
#include <client/src/PortAudioSpeaker.hpp>
#include <client/src/Opus.hpp>
#include <shared/packets.hpp>
#include <shared/exceptions/NetworkException.hpp>
#include <shared/network/WindowsInit.hpp>
#include <cstring>

int main(int ac, char *av[]) try {
    //if (ac != 4) {
    //    std::cout << "Usage : ./" << av[0] << " ip port isServer(true of false)" << std::endl;
    //    return 84;
    //}

	network::WindowsInit windows;
	network::UdpConnection connection("10.14.57.18", 4242, true);
    //network::UdpConnection connection(av[1], std::stoi(av[2]), !strcmp(av[3], "true"));
    //Opus encoder;
    PortAudioRecorder recorder;
    PortAudioSpeaker player;

	std::deque<std::shared_ptr<SoundPacket>> send_stack;
	std::deque<std::shared_ptr<SoundPacket>> recv_stack;

	std::cout << "Connect the other client and press enter" << std::endl;
	getchar();

	while (true) {

		// Get recorded sounds from mic
		auto recorded_sound = recorder.receiveSound();
		if (recorded_sound != nullptr)
			send_stack.push_back((recorded_sound));
		
		// Play sounds recived
		if (!recv_stack.empty()) {
			auto recived_sound = (recv_stack.front());	
			recv_stack.pop_front();
			player.sendSound((recived_sound));
		}


		if (connection.isReady()) {
			std::cout << "Recived some data" << std::endl;
			if (recv_stack.size() > 100)
				recv_stack.pop_front();
			auto data = connection.recv<packet::data>();
			if (data->magic == 0XDA) {
				auto packet = std::make_shared<SoundPacket>(data->size);
				packet->copyFrom(data->body, data->size);
				recv_stack.push_back((packet));
			}
		}
		if (!send_stack.empty()) {
			std::shared_ptr<SoundPacket> sound_packet = (send_stack.front());
			send_stack.pop_front();

			packet::data sound_data;
			sound_packet->copyTo(sound_data.body, sound_packet->dataSize());
			sound_data.size = sound_packet->dataSize();
			connection.sendData((bytes)&sound_data, sizeof(packet::data));
		}
	}
}
catch (ex::NetworkException &ex) {
	std::cout << ex.what() << " " << ex.what() << std::endl;
}