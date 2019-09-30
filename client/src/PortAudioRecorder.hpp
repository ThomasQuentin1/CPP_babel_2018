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
#include "PortAudio.hpp"


class PortAudioRecorder : public PortAudio {
public:
	explicit PortAudioRecorder();
	auto record() ->void;
	~PortAudioRecorder();
	auto sendSound(std::unique_ptr<SoundPacket> packet) -> void;

};


#endif //INC_7D263F90ED094D52840D3D0A7397188A
