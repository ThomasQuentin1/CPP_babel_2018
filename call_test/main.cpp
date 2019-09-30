//
// Created by tom on 9/30/19.
//

#include <shared/network/UdpConnection.hpp>
#include <cstring>
#include <client/src/PortAudioRecorder.hpp>
#include <client/src/PortAudioSpeaker.hpp>
#include <client/src/Opus.hpp>

int main(int ac, char *av[]) {
    if (ac != 4) {
        std::cout << "Usage : ./" << av[0] << " ip port isServer(true of false)" << std::endl;
        return 84;
    }
    network::UdpConnection connection(av[1], std::stoi(av[2]), !strcmp(av[3], "true"));
    Opus encoder;
    PortAudioRecorder recorder;
    PortAudioSpeaker player;


}