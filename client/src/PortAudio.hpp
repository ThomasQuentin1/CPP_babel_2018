//
// Created by tom on 9/30/19.
//

#ifndef BABEL_PORTAUDIO_HPP
	#define BABEL_PORTAUDIO_HPP
#include "../shared/headers.hpp"
#include <portaudio.h>
#include <shared\SoundPacket.hpp>

class PortAudio {

public:
	PortAudio();
protected:
	PaStream* stream = NULL;
	std::deque<std::unique_ptr<SoundPacket>> stack;
};


#endif //BABEL_PORTAUDIO_HPP
