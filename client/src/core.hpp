#ifndef CORE_H
#define CORE_H

#include "Graphic/igui.h"
#include "Graphic/gui.h"
#include "Communication.hpp"
#include "PortAudio.hpp"
#include <vector>

class Core
{
public:
    Core(int argc, char *argv[]);
    void loop();
    IGui *gui;
private:

    std::unique_ptr<Communication> comm;
    std::vector<std::string> stringToVector(std::string);
    std::unique_ptr<IAudio> audio;
    bool isLogged = false;
};

#endif // CORE_H
