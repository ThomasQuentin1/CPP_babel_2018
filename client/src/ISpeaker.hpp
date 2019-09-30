/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef INC_A52A3BF4FC264C66BF117AB3287A9450
    #define INC_A52A3BF4FC264C66BF117AB3287A9450

class ISpeaker {
public:
    virtual auto startSpeaking() -> void = 0;
    virtual auto stopSpeaking() -> void = 0;
    virtual auto addData(std::vector<byte> &value) -> void = 0;
    virtual ~ISpeaker() = default;
    virtual auto getVolume() -> float = 0;
    virtual auto getFrame() -> std::vector<std::vector<byte>> & = 0;
    virtual auto isSpeaking() -> bool = 0;
    virtual auto lockThread() -> void = 0;
    virtual auto unlockThread() -> void = 0;
    virtual auto playSound() -> void = 0;
};
#endif //INC_A52A3BF4FC264C66BF117AB3287A9450
