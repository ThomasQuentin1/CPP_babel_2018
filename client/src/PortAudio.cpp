//
// Created by tom on 9/30/19.
//

#include "PortAudio.hpp"
#include <shared/exceptions/SoundException.hpp>
#include <client/src/audioConfig.h>

PortAudio::PortAudio() : _thread(this)
{
	PaStreamParameters inputParameters, outputParameters;
	PaError err;
	const PaDeviceInfo* inputInfo;
	const PaDeviceInfo* outputInfo;
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
	inputParameters.hostApiSpecificStreamInfo = nullptr;
	outputParameters.channelCount = numChannels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = nullptr;
	/* -- setup -- */
	err = Pa_OpenStream(
		&this->stream,
		&inputParameters,
		&outputParameters,
		audioConfig::sample_rate,
		audioConfig::frames_per_buffer,
		paClipOff,
        nullptr,
        nullptr);
	if (err != paNoError)
		throw ex::SoundException("error");
	err = Pa_StartStream(stream);
}

auto PortAudio::receiveSound() -> std::shared_ptr<SoundPacket>
{
    if (this->input_stack.empty())
        return nullptr;
    _thread.lock().lock();
    auto data = (this->input_stack.front());
    input_stack.pop_front();
    _thread.lock().unlock();
    return data;
}

auto PortAudio::play() -> void
{
    if (output_stack.empty())
        return;
    _thread.lock().lock();
    std::shared_ptr<SoundPacket> sound = (output_stack.front());
    output_stack.pop_front();
    Pa_WriteStream(stream, sound->ptr<void*>(), audioConfig::frames_per_buffer);
    _thread.lock().unlock();
}

auto PortAudio::sendSound(std::shared_ptr<SoundPacket> packet) -> void
{
    if (packet == nullptr)
        return;
    _thread.lock().lock();
    if (this->output_stack.size() > 100)
        this->output_stack.pop_front();
    this->output_stack.push_back((packet));
    _thread.lock().unlock();
}

auto PortAudio::record() -> void
{
    if (this->input_stack.size() > 100)
        this->input_stack.pop_front();
    std::shared_ptr<SoundPacket> pck(new SoundPacket(audioConfig::frames_per_buffer * sizeof(double) + 1));
    Pa_ReadStream(stream, pck->ptr<void*>(), audioConfig::frames_per_buffer);
    pck->setDataSize(audioConfig::frames_per_buffer * sizeof(double) + 1);
    _thread.lock().lock();
    input_stack.push_back((pck));
    _thread.lock().unlock();
}

auto PortAudio::entryPoint() -> void {
    while (this->_thread) {
        this->record();
        this->play();
    }
}
