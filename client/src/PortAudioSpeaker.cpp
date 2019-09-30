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
	std::shared_ptr<SoundPacket> sound = (stack.front());
	stack.pop_front();
	//Pa_WriteStream(stream, sound->ptr<void*>(), audioConfig::frames_per_buffer);
	_thread.lock().unlock();
}

auto PortAudioSpeaker::sendSound(std::shared_ptr<SoundPacket> packet) -> void
{
	if (packet == nullptr)
		return;
	_thread.lock().lock();
	if (this->stack.size() > 100)
		this->stack.pop_front();
	this->stack.push_back((packet));
	_thread.lock().unlock();
}

auto PortAudioSpeaker::entryPoint() -> void
{
	while (this->_thread)
		this->play();
}
