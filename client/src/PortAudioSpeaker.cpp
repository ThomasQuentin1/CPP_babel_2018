//
// Created by root on 9/22/19.
//

#include <shared/exceptions/SoundException.hpp>
#include "PortAudioSpeaker.hpp"
#include "audioConfig.h"


PortAudioSpeaker::PortAudioSpeaker() : _thread(this)
{

}

auto PortAudioSpeaker::play() -> void
{
	if (stack.empty() || stack.size() >= 100)
		return;
	Pa_WriteStream(stream, stack.front()->ptr<void*>(), audioConfig::frames_per_buffer);
	stack.pop_front();
}

auto PortAudioSpeaker::sendSound(std::unique_ptr<SoundPacket> packet) -> void
{
	if (packet == nullptr)
		return;
	if (this->stack.size() > 100)
		this->stack.pop_front();
	this->stack.push_back(std::move(packet));
}

auto PortAudioSpeaker::entryPoint() -> void
{
	while (this->_thread)
		this->play();
}
