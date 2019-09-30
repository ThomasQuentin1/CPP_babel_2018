//
// Created by root on 9/22/19.
//

#include <shared/exceptions/SoundException.hpp>
#include "PortAudioSpeaker.hpp"
#include "audioConfig.h"


PortAudioSpeaker::PortAudioSpeaker()
{

}

auto PortAudioSpeaker::play() -> void
{
	Pa_WriteStream(stream, stack.front()->ptr<void*>(), audioConfig::frames_per_buffer);
	stack.pop_front();
}

auto PortAudioSpeaker::receiveSound() -> std::unique_ptr<SoundPacket>
{
	if (this->stack.empty())
		return nullptr;
	auto data = std::move(this->stack.front());
	stack.pop_front();
	return data;
}
