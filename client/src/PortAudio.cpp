//
// Created by tom on 9/30/19.
//

#include "PortAudio.hpp"
#include <shared/exceptions/SoundException.hpp>
#include <client/src/audioConfig.h>

PortAudio::PortAudio(bool player)
{
    std::cout << "Init PortAudio" << std::endl;
    PaStreamParameters inputParameters, outputParameters;
	PaError err;
	const PaDeviceInfo* inputInfo;
	const PaDeviceInfo* outputInfo;
	int numChannels;

	err = Pa_Initialize();
	if (err != paNoError)
		throw ex::SoundException("error");
    numChannels = 2;

    if (player) {
        inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
        inputInfo = Pa_GetDeviceInfo(inputParameters.device);
        inputParameters.channelCount = numChannels;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;
    } else {
        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        outputInfo = Pa_GetDeviceInfo(outputParameters.device);
        outputParameters.channelCount = numChannels;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;
    }

	/* -- setup -- */
	if (player) {
        err = Pa_OpenStream(
                &this->stream,
                nullptr,
                &outputParameters,
                audioConfig::sample_rate,
                audioConfig::frames_per_buffer,
                paClipOff,
                NULL,
                NULL);
    } else {
        err = Pa_OpenStream(
                &this->stream,
                &inputParameters,
                nullptr,
                audioConfig::sample_rate,
                audioConfig::frames_per_buffer,
                paClipOff,
                NULL,
                NULL);
	}
	if (err != paNoError)
		throw ex::SoundException("error");
	err = Pa_StartStream(stream);
}
