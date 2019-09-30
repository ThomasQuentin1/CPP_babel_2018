//
// Created by tom on 9/17/19.
//

#ifndef BABEL_OPUS_HPP
#define BABEL_OPUS_HPP

#include <opus.h>
#include <shared/SoundPacket.hpp>
#include "shared/types.hpp"
#include "audioConfig.h"
#include "IEncoder.hpp"

class Opus : public IEncoder {
public:
    Opus();
    ~Opus();
    auto encode(std::shared_ptr<SoundPacket> &packet) -> int;
    auto decode(std::shared_ptr<SoundPacket> &packet) -> int;

private:
    OpusEncoder *encoder;
    OpusDecoder *decoder;
    byte temp_buffer[audioConfig::max_packet_size];
};


#endif //BABEL_OPUS_HPP
