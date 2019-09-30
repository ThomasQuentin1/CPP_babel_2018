/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef INC_DFA9769CE2B64015BA6FFFE219292F57
    #define INC_DFA9769CE2B64015BA6FFFE219292F57

#include <shared/types.hpp>
#include <vector>
#include "audioConfig.h"

class IRecorder {
public:
    virtual auto getVolume() -> int = 0;
    virtual auto setNewdata() -> void = 0 ;
    virtual auto startRecording() -> void = 0;
    virtual auto stopRecording() -> void = 0;
    virtual auto lockThread() -> void = 0;
    virtual auto unlockThread() -> void = 0;
    virtual auto getData() -> paTestData* = 0;
    virtual auto isNewRecors() -> bool = 0;
    virtual auto record() -> void = 0;
    virtual ~IRecorder() = default;
};
#endif //INC_DFA9769CE2B64015BA6FFFE219292F57
