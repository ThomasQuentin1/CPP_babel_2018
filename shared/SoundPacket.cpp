//
// Created by tom on 9/25/19.
//

#include <cstring>
#include "SoundPacket.hpp"

SoundPacket::SoundPacket(short _size_) : alloc_size(_size_), data_size(0) {
	this->data = new byte[alloc_size];
}

SoundPacket::~SoundPacket() {
	delete[] data;
}

auto SoundPacket::ptr() -> bytes {
	return this->data;
}

auto SoundPacket::copyFrom(bytes from, short size) -> void {
    memcpy(this->data, from, size);
}

auto SoundPacket::copyTo(bytes to, short size) -> void {
    memcpy(to, this->data, size);
    this->data_size = size;
}

auto SoundPacket::setDataSize(short size) -> void {
    this->data_size = size;
}

auto SoundPacket::dataSize() -> short {
    return this->data_size;
}

auto SoundPacket::allocSize() -> short {
    return this->alloc_size;
}
