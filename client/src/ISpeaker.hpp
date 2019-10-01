/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef INC_A52A3BF4FC264C66BF117AB3287A9450
    #define INC_A52A3BF4FC264C66BF117AB3287A9450
#include "../shared/headers.hpp"
#include "../shared/SoundPacket.hpp"

class ISpeaker {
public:
	virtual auto play() -> void = 0;
	virtual auto sendSound(std::shared_ptr<SoundPacket> packet) -> void = 0;
	virtual auto entryPoint() -> void = 0;
    virtual ~ISpeaker() = default;

};
#endif //INC_A52A3BF4FC264C66BF117AB3287A9450
