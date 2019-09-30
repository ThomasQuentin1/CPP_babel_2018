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

class PortAudioSpeaker : public  ISpeaker{
public:
    PortAudioSpeaker(float volume = 1);
    ~PortAudioSpeaker();
    auto startSpeaking() -> void final;
    auto stopSpeaking() -> void final ;
    auto getVolume() -> float final;
    auto getFrame() -> std::vector<std::vector<byte>> & final;
    auto isSpeaking() -> bool final;
    auto addData(std::vector<byte> &value) -> void final;
    auto lockThread() -> void final;
    auto unlockThread() -> void final;
    auto addTestdata(paTestData *data) -> void;
    auto playSound() -> void final;

private:
    float volume;
    PaStream *stream;
    bool speaking = false;
    std::vector<std::vector<byte>> frame;
    std::mutex thread;
    paTestData *data;
};


#endif //INC_E29D79BB939443959D84ADD0582E3287
