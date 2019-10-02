//
// Created by tom on 9/23/19.
//

#ifndef BABEL_IENCODER_HPP
#define BABEL_IENCODER_HPP

class IEncoder {
public:
    virtual auto encode(std::shared_ptr<SoundPacket> &packet) -> int = 0;
    virtual auto decode(std::shared_ptr<SoundPacket> &packet) -> int = 0;
    virtual ~IEncoder() = default;

};

#endif //BABEL_IENCODER_HPP
