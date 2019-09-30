//
// Created by tom on 9/25/19.
//

#ifndef BABEL_SOUNDPACKET_HPP
#define BABEL_SOUNDPACKET_HPP

#include "shared/headers.hpp"
#include "shared/types.hpp"

class SoundPacket {
public:
    explicit SoundPacket(short _size);
    ~SoundPacket();
    SoundPacket(SoundPacket const &) = delete;
    SoundPacket(SoundPacket &&) = default;

    auto ptr() -> bytes; // Handle with care

    template <typename T>
    auto ptr() -> T { // Handle with care
        return reinterpret_cast<T>(this->data);
    }

    auto copyFrom(bytes, short size) -> void;
    auto copyTo(bytes, short size) -> void;
    auto setDataSize(short size) -> void;
    auto dataSize() -> short;
    auto allocSize() -> short;

private:
    bytes data;
    short data_size;
    short alloc_size;
};

#endif //BABEL_SOUNDPACKET_HPP
