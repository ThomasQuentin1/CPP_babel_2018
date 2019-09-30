//
// Created by root on 9/22/19.
//

#ifndef INC_E29D79BB939443959D84ADD0582E3287
#define INC_E29D79BB939443959D84ADD0582E3287


#include <portaudio.h>
#include <vector>
#include <shared/types.hpp>
#include <mutex>
#include "ISpeaker.hpp"
#include "audioConfig.h"
#include "PortAudio.hpp"
#include <client/src/Thread.hpp>


class PortAudioSpeaker : public  PortAudio {
public:

	PortAudioSpeaker();
	auto play() -> void;
	auto sendSound(std::shared_ptr<SoundPacket> packet) -> void;
	auto entryPoint() -> void;


private:
	Thread<PortAudioSpeaker> _thread;
};


#endif //INC_E29D79BB939443959D84ADD0582E3287
