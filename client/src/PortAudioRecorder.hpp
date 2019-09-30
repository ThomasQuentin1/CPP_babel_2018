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
#include <client/src/Thread.hpp>


class PortAudioRecorder : public PortAudio {
public:
	explicit PortAudioRecorder();
	auto record() ->void;
	~PortAudioRecorder();
	auto receiveSound()->std::shared_ptr<SoundPacket>;
	auto entryPoint() -> void;

private:
	Thread<PortAudioRecorder> _thread;

};


#endif //INC_7D263F90ED094D52840D3D0A7397188A
