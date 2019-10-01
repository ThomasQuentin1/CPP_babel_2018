/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef INC_DFA9769CE2B64015BA6FFFE219292F57
    #define INC_DFA9769CE2B64015BA6FFFE219292F57

#include "shared/headers.hpp"
#include "shared/SoundPacket.hpp"


class IRecorder {
public:
	virtual auto record() ->void = 0;
	virtual auto receiveSound()->std::shared_ptr<SoundPacket> = 0;
	virtual auto entryPoint() -> void = 0;
    virtual ~IRecorder() = default;
};
#endif //INC_DFA9769CE2B64015BA6FFFE219292F57
