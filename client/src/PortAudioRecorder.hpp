//
// Created by root on 9/22/19.
//

#ifndef INC_7D263F90ED094D52840D3D0A7397188A
#define INC_7D263F90ED094D52840D3D0A7397188A


#include <portaudio.h>
#include <mutex>
#include "IRecorder.hpp"
#include "audioConfig.h"
#include <deque>
#include "../../shared/SoundPacket.hpp"


class PortAudioRecorder {
public:
	explicit PortAudioRecorder();
	auto record() ->void;
	auto play() -> void;
	~PortAudioRecorder();

private:
	PaStream* stream = NULL;
	std::deque<std::unique_ptr<SoundPacket>> stack;

};


#endif //INC_7D263F90ED094D52840D3D0A7397188A
