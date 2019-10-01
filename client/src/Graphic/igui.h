#ifndef IGUI_H
#define IGUI_H

#include <iostream>
#include <vector>
#include "enumcommunication.h"

//this is an abstract class

class IGui {
public:
    virtual ~IGui() = default;
    virtual commEnum_t refresh(commEnum_t, std::string name) = 0;
    virtual std::string getArgument() = 0;
    virtual void setOnlineContact(std::vector<std::string>) = 0;

private:
};
#endif // IGUI_H
