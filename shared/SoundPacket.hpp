//
// Created by tom on 9/25/19.
//

#ifndef BABEL_SOUNDPACKET_HPP
#define BABEL_SOUNDPACKET_HPP

#include "shared/headers.hpp"
#include "shared/types.hpp"

class SoundPacket {
public:
    explicit SoundPacket(int _size);
    ~SoundPacket();
    SoundPacket(SoundPacket const &) = delete;
    SoundPacket(SoundPacket &&) = default;

    auto ptr() -> const bytes; // Handle with care

    template <typename T>
    auto ptr() -> const T { // Handle with care
        return reinterpret_cast<T>(this->data);
    }

    auto copyFrom(bytes, int size) -> void;
    auto copyTo(bytes, int size) -> void;
    auto setDataSize(int size) -> void;
    auto dataSize() -> int;
    auto allocSize() -> int;

private:
    bytes data;
    int data_size;
    int alloc_size;
};

#endif //BABEL_SOUNDPACKET_HPP
