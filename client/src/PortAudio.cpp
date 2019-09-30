//
// Created by tom on 9/30/19.
//

#include "PortAudio.hpp"
#include <shared\exceptions\SoundException.hpp>
#include <client\src\audioConfig.h>

PortAudio::PortAudio()
{
	PaStreamParameters inputParameters, outputParameters;
	PaError err;
	const PaDeviceInfo* inputInfo;
	const PaDeviceInfo* outputInfo;
	int numBytes;
	int numChannels;

	err = Pa_Initialize();
	if (err != paNoError)
		throw ex::SoundException("error");

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	inputInfo = Pa_GetDeviceInfo(inputParameters.device);

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	outputInfo = Pa_GetDeviceInfo(outputParameters.device);
	numChannels = inputInfo->maxInputChannels < outputInfo->maxOutputChannels
		? inputInfo->maxInputChannels : outputInfo->maxOutputChannels;
	inputParameters.channelCount = numChannels;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.channelCount = numChannels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	/* -- setup -- */
	err = Pa_OpenStream(
		&this->stream,
		&inputParameters,
		&outputParameters,
		audioConfig::sample_rate,
		audioConfig::frames_per_buffer,
		paClipOff,
		NULL,
		NULL);
	if (err != paNoError)
		throw ex::SoundException("error");
	err = Pa_StartStream(stream);
}
