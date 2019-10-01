#ifndef CORE_H
#define CORE_H

#include "Graphic/igui.h"
#include "Graphic/gui.h"
#include "PortAudioSpeaker.hpp"
#include "PortAudioRecorder.hpp"
#include "Communication.hpp"
#include <vector>

class Core
{
public:
    Core(int argc, char *argv[]);
    void loop();
    std::unique_ptr<IGui> gui;
private:

    Communication comm;
    std::vector<std::string> stringToVector(std::string);
    std::unique_ptr<PortAudioSpeaker> speaker;
    std::unique_ptr<PortAudioRecorder> recorder;
    bool isLogged = false;
};

#endif // CORE_H
