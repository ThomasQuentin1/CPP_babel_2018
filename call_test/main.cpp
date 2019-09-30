//
// Created by tom on 9/30/19.
//

#include <shared/network/UdpConnection.hpp>
#include "shared/headers.hpp"
#include <client/src/PortAudioRecorder.hpp>
#include <client/src/PortAudioSpeaker.hpp>
#include <client/src/Opus.hpp>
#include <shared\packets.hpp>

int main(int ac, char *av[]) {
    if (ac != 4) {
        std::cout << "Usage : ./" << av[0] << " ip port isServer(true of false)" << std::endl;
        return 84;
    }
    network::UdpConnection connection(av[1], std::stoi(av[2]), !strcmp(av[3], "true"));
    Opus encoder;
    PortAudioRecorder recorder;
    PortAudioSpeaker player;

	std::deque<std::unique_ptr<SoundPacket>> send_stack;
	std::deque<std::unique_ptr<SoundPacket>> recv_stack;


	while (true) {

		// Get recorded sounds from mic
		auto recorded_sound = recorder;
		if (recorded_sound != nullptr)
			send_stack.push_back(std::move(recorded_sound));
		
		// Play sounds recived
		if (!recv_stack.empty()) {
			auto recived_sound = std::move(recv_stack.front());
			
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
			connection.sendData(sound_packet->ptr(), sound_packet->dataSize());
		}
	}


}