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

auto PortAudioRecorder::record() -> void
{
	
		std::unique_ptr<SoundPacket> pck(new SoundPacket(audioConfig::frames_per_buffer * sizeof(double) + 1));
		Pa_ReadStream(stream, pck->ptr<void*>(), audioConfig::frames_per_buffer);
		stack.push_back(std::move(pck));
}

auto PortAudioRecorder::play() -> void
{
		Pa_WriteStream(stream, stack.front()->ptr<void*>(), audioConfig::frames_per_buffer);
		stack.pop_front();
		// return stack.front();
}

PortAudioRecorder::~PortAudioRecorder()
{

}
