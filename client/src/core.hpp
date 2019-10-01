#ifndef CORE_H
#define CORE_H

#include "Graphic/igui.h"
#include "Graphic/gui.h"
#include "Communication.hpp"
#include <vector>

class Core
{
public:
    Core(int argc, char *argv[]);
    void loop();

private:
    std::unique_ptr<IGui> gui;
    Communication comm;
    std::vector<std::string> stringToVector(std::string);
    bool isLogged = false;
};

#endif // CORE_H
