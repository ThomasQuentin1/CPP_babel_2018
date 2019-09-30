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
	_thread.lock().lock();
	std::unique_ptr<SoundPacket> sound = std::move(stack.front());
	stack.pop_front();
	_thread.lock().unlock();
	Pa_WriteStream(stream, sound->ptr<void*>(), audioConfig::frames_per_buffer);
}

auto PortAudioSpeaker::sendSound(std::unique_ptr<SoundPacket> packet) -> void
{
	if (packet == nullptr)
		return;
	_thread.lock().lock();
	if (this->stack.size() > 100)
		this->stack.pop_front();
	this->stack.push_back(std::move(packet));
	_thread.lock().unlock();
}

auto PortAudioSpeaker::entryPoint() -> void
{
	while (this->_thread)
		this->play();
}
