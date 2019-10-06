//
// Created by tom on 9/25/19.
//

#include <cstring>
#include "SoundPacket.hpp"

SoundPacket::SoundPacket(int _size_) : alloc_size(_size_), data_size(0) {
    this->data = new byte[alloc_size];  
}

SoundPacket::~SoundPacket() {
	delete[] data;
}

auto SoundPacket::ptr() -> const bytes {
	return this->data;
}

auto SoundPacket::copyFrom(bytes from, int size) -> void {
    memcpy(this->data, from, size);
}

auto SoundPacket::copyTo(bytes to, int size) -> void {
    memcpy(to, this->data, size);
    this->data_size = size;
}

auto SoundPacket::setDataSize(int size) -> void {
    this->data_size = size;
}

auto SoundPacket::dataSize() -> int {
    return this->data_size;
}

auto SoundPacket::allocSize() -> int {
    return this->alloc_size;
}
