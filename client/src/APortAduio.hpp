#pragma once

#include "../shared/headers.hpp"
#include <shared\SoundPacket.hpp>
#include <portaudio.h>
#include <shared\exceptions\SoundException.hpp>


class APortAudio
{

public:

	APortAudio();

protected:
	PaStream* stream = NULL;
	std::deque<std::unique_ptr<SoundPacket>> stack;

};

