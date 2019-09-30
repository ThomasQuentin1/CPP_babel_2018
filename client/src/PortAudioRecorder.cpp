//
// Created by root on 9/22/19.
//

#include <shared/exceptions/SoundException.hpp>
#include <iostream>
#include <deque>
#include "../../shared/SoundPacket.hpp"
#include "PortAudioRecorder.hpp"
#include "audioConfig.h"

PortAudioRecorder::PortAudioRecorder() : _thread(this)
{

}

auto PortAudioRecorder::record() -> void
{
	if (this->stack.size() >= 100)
		return;
	std::unique_ptr<SoundPacket> pck(new SoundPacket(audioConfig::frames_per_buffer * sizeof(double) + 1));
	Pa_ReadStream(stream, pck->ptr<void*>(), audioConfig::frames_per_buffer);
	pck->setDataSize(audioConfig::frames_per_buffer * sizeof(double) + 1);
	_thread.lock().lock();
	stack.push_back(std::move(pck));
	_thread.lock().unlock();
}

auto PortAudioRecorder::receiveSound() -> std::unique_ptr<SoundPacket>
{
	if (this->stack.empty())
		return nullptr;
	_thread.lock().lock();
	auto data = std::move(this->stack.front());
	stack.pop_front();
	_thread.lock().unlock();
	return data;
}

PortAudioRecorder::~PortAudioRecorder()
{

}

auto PortAudioRecorder::entryPoint() -> void
{
	while (this->_thread)
		this->record();
}

