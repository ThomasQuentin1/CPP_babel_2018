//
// Created by root on 9/22/19.
//

#include <shared/exceptions/SoundException.hpp>
#include <iostream>
#include <deque>
#include "../../shared/SoundPacket.hpp"
#include "PortAudioRecorder.hpp"
#include "audioConfig.h"

PortAudioRecorder::PortAudioRecorder()
{

}

auto PortAudioRecorder::record() -> void
{
	
		std::unique_ptr<SoundPacket> pck(new SoundPacket(audioConfig::frames_per_buffer * sizeof(double) + 1));
		Pa_ReadStream(stream, pck->ptr<void*>(), audioConfig::frames_per_buffer);
		stack.push_back(std::move(pck));
}


PortAudioRecorder::~PortAudioRecorder()
{

}

auto PortAudioRecorder::sendSound(std::unique_ptr<SoundPacket> packet) -> void
{
	if (packet == nullptr)
		return;
	if (this->stack.size() > 100)
		this->stack.pop_front();
	this->stack.push_back(std::move(packet));
}

