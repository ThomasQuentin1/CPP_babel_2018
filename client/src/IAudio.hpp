//
// Created by tom on 9/30/19.
//

#ifndef BABEL_IAUDIO_HPP
	#define BABEL_IAUDIO_HPP
#include "shared/headers.hpp"
#include "Thread.hpp"
#include <portaudio.h>
#include <shared/SoundPacket.hpp>

class IAudio {

public:
    virtual auto receiveSound()->std::shared_ptr<SoundPacket> = 0;
    virtual auto sendSound(std::shared_ptr<SoundPacket> packet) -> void = 0;
    virtual ~IAudio() = default;
};


#endif //BABEL_PORTAUDIO_HPP
