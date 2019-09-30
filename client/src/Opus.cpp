//
// Created by tom on 9/17/19.
//

#include "shared/exceptions/SoundException.hpp"
#include "Opus.hpp"

Opus::Opus() {
    int err;
    encoder = opus_encoder_create(audioConfig::sample_rate, audioConfig::channels, OPUS_APPLICATION_AUDIO, &err);
    if (err < 0)
        throw ex::SoundException("failed to initialize encoder: ", "Opus constructor");

    decoder = opus_decoder_create(audioConfig::sample_rate, audioConfig::channels, &err);
    if (err<0)
        throw ex::SoundException("failed to initialize decoder: ", "Opus constructor");
}

Opus::~Opus() {
    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);
}

auto Opus::encode(std::unique_ptr<SoundPacket> &packet) -> int {
    int nbBytes = opus_encode_float(encoder, packet->ptr<float*>(), audioConfig::frames_per_buffer,
                                    this->temp_buffer, packet->dataSize());
    if (nbBytes < 0)
        throw ex::SoundException(std::string("failed to encode: ") + opus_strerror(nbBytes), "Opus encoding");
    packet->copyFrom(this->temp_buffer, nbBytes);
    return nbBytes;
}

auto Opus::decode(std::unique_ptr<SoundPacket> &packet) -> int {
    int nbBytes = opus_decode_float(decoder, packet->ptr<bytes>(), packet->dataSize(),
                                    reinterpret_cast<float *>(this->temp_buffer), audioConfig::frames_per_buffer, 0 * audioConfig::channels);
    if (nbBytes < 0)
        throw ex::SoundException(std::string("failed to encode: ") + opus_strerror(nbBytes), "Opus encoding");
    packet->copyFrom(this->temp_buffer, nbBytes);
    return nbBytes;
}