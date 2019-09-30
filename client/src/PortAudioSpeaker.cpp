//
// Created by root on 9/22/19.
//

#include <shared/exceptions/SoundException.hpp>
#include "PortAudioSpeaker.hpp"
#include "audioConfig.h"

static int playCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void *userData )
{
    auto *data = (paTestData*)userData;
    float *rptr = &data->recordedSamples[data->frameIndex * audioConfig::channels];
    auto *wptr = (float*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    if( framesLeft < framesPerBuffer ) {
        /* final buffer... */
        for( i = 0; i < framesLeft; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( audioConfig::channels == 2 )
                *wptr++ = *rptr++;  /* right */
        }
        for(; i<framesPerBuffer; i++ ) {
            *wptr++ = 0;  /* left */
            if( audioConfig::channels == 2 )
                *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for( i=0; i<framesPerBuffer; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( audioConfig::channels == 2 )
                *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}


PortAudioSpeaker::PortAudioSpeaker(float volume) : volume(volume){

}

PortAudioSpeaker::~PortAudioSpeaker() {

}

auto PortAudioSpeaker::startSpeaking() -> void {
    PaStreamParameters  outputParameters;
    data->frameIndex = 0;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice)
        throw (ex::SoundException("no output divice", "sourceAudioSpeaker.cpp startSpeaking function"));
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat =  paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    printf("\n=== Now playing back. ===\n"); fflush(stdout);
    if (Pa_OpenStream( &stream, NULL, &outputParameters, audioConfig::sample_rate,
        audioConfig::frames_per_buffer, paClipOff, playCallback, this->data ) != paNoError)
        throw (ex::SoundException("can't open the output stream", "sourceAudioSpeaker.cpp startSpeaking function"));
}

auto PortAudioSpeaker::stopSpeaking() -> void {
    this->speaking = false;
    if(Pa_CloseStream(this->stream) != paNoError)
        throw ex::SoundException("microphone: error while closing", "PortAudioSpeaker.cpp stopSpeaking function");
    this->stream = nullptr;
}

auto PortAudioSpeaker::getVolume() -> float {
    return this->volume;
}

auto PortAudioSpeaker::getFrame() -> std::vector<std::vector<byte>> & {
    return this->frame;
}

auto PortAudioSpeaker::isSpeaking() -> bool {
    return this->speaking;
}

auto PortAudioSpeaker::addData(std::vector<byte> &value) -> void {
    if (value.empty())
        return;
    //_lock.lock();
    frame.push_back(value);
   // _lock.unlock();
}

auto PortAudioSpeaker::lockThread() -> void {
    this->thread.lock();
}

auto PortAudioSpeaker::unlockThread() -> void {
    this->thread.unlock();
}

auto PortAudioSpeaker::addTestdata(paTestData *data) -> void {
    this->data = data;
}

auto PortAudioSpeaker::playSound() -> void {
    if ( stream ) {
        if( Pa_StartStream( stream ) != paNoError )
            throw (ex::SoundException("can't start the output stream", "sourceAudioSpeaker.cpp startSpeaking function"));
        printf("Waiting for playback to finish.\n"); fflush(stdout);
        while( (Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( Pa_CloseStream( stream ) != paNoError )
            throw (ex::SoundException("can't close the output stream", "sourceAudioSpeaker.cpp startSpeaking function"));
        printf("Done.\n"); fflush(stdout);
    }
}
