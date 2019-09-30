//
// Created by tom on 9/30/19.
//

#include <shared/network/UdpConnection.hpp>
#include "shared/headers.hpp"
#include <client/src/PortAudioRecorder.hpp>
#include <client/src/PortAudioSpeaker.hpp>
#include <client/src/Opus.hpp>
#include <shared\packets.hpp>
#include <shared\exceptions\NetworkException.hpp>
#include <shared\network\WindowsInit.hpp>

int main(int ac, char *av[]) try {
    //if (ac != 4) {
    //    std::cout << "Usage : ./" << av[0] << " ip port isServer(true of false)" << std::endl;
    //    return 84;
    //}

	network::WindowsInit windows;
	//network::UdpConnection connection("127.0.0.1", 8082, true);
    network::UdpConnection connection(av[1], std::stoi(av[2]), !strcmp(av[3], "true"));
    //Opus encoder;
    PortAudioRecorder recorder;
    PortAudioSpeaker player;

	std::deque<std::unique_ptr<SoundPacket>> send_stack;
	std::deque<std::unique_ptr<SoundPacket>> recv_stack;

	std::cout << "Connect the other client and press enter" << std::endl;
	getchar();

	while (true) {

		// Get recorded sounds from mic
		auto recorded_sound = recorder.receiveSound();
		if (recorded_sound != nullptr)
			send_stack.push_back(std::move(recorded_sound));
		
		// Play sounds recived
		if (!recv_stack.empty()) {
			auto recived_sound = std::move(recv_stack.front());	
			player.sendSound(std::move(recived_sound));
		}


		if (connection.isReady()) {
			if (recv_stack.size() > 100)
				recv_stack.pop_front();
			auto data = connection.recv<packet::data>();
			if (data->magic == 0XDA) {
				auto packet = std::make_unique<SoundPacket>(audioConfig::frames_per_buffer * sizeof(double));
				packet->copyFrom(data->body, data->size);
				recv_stack.push_back(std::move(packet));
			}
		}
		if (!send_stack.empty()) {
			std::unique_ptr<SoundPacket> sound_packet = std::move(send_stack.front());
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