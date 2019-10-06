//
// Created by tom on 9/30/19.
//

#ifndef BABEL_PORTAUDIO_HPP
	#define BABEL_PORTAUDIO_HPP
#include "shared/headers.hpp"
#include "Thread.hpp"
#include "IAudio.hpp"
#include <portaudio.h>
#include <shared/SoundPacket.hpp>

class PortAudio : public IAudio {

public:
	explicit PortAudio();
    auto receiveSound()->std::shared_ptr<SoundPacket> override ;
    auto sendSound(std::shared_ptr<SoundPacket> packet) -> void override ;
    auto play() -> void;
    auto record() ->void;
    auto entryPoint() -> void;
    auto readEntryPoint() -> void;
    auto writeEntryPoint() -> void;

protected:
	PaStream* stream = nullptr;
	std::deque<std::shared_ptr<SoundPacket>> input_stack;
    std::deque<std::shared_ptr<SoundPacket>> output_stack;

private:
    Thread<PortAudio> _thread;
};


#endif //BABEL_PORTAUDIO_HPP
