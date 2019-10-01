#include "gui.h"
#include "user.h"


Gui::Gui(int argc, char *argv[]) : a(argc, argv)
{
    //instance d'une gui
    connectionWindow.loggedWindow = new LoggedWindow();
//    connectionWindow.setStyleSheet("background-color:#fff8dc; ");
//    connectionWindow.setStyleSheet("color :blue; background-color:yellow;");
    connectionWindow.show();
}

Gui::~Gui()
{

}

commEnum_t Gui::refresh(commEnum_t arg, std::string name)
{
    returnRefresh = commEnum_t::NONE;

    connectionWindow.refresh(arg, name);
    connectionWindow.loggedWindow->refresh(arg, name);
    if (connectionWindow.loggedWindow->isVisible() && connectionWindow.loggedWindow->firstFrameShowed == false) {
        connectionWindow.loggedWindow->firstFrameShowed = true;
        connectionWindow.loggedWindow->updateUI();
    }
    QCoreApplication::processEvents();
    getReturnValFromWindows();
    return (returnRefresh);
}

std::string Gui::getArgument()
{
    if (targetArgument != "")
        return (targetArgument);
    else
        return ("error, no argument returned by graphic");
}

void Gui::setOnlineContact(std::vector<std::string> contactsList)
{
    connectionWindow.loggedWindow->setOnlineContact(contactsList);
}

void Gui::getReturnValFromWindows()
{
    if (connectionWindow.returnRefresh != commEnum_t::NONE) {
        returnRefresh = connectionWindow.returnRefresh;
        connectionWindow.returnRefresh = commEnum_t::NONE;
        if (connectionWindow.targetArgument != "") {
            targetArgument = connectionWindow.targetArgument;
            connectionWindow.targetArgument = "";
        }
    }
    if (connectionWindow.loggedWindow->returnRefresh != commEnum_t::NONE) {
        returnRefresh = connectionWindow.loggedWindow->returnRefresh;
        connectionWindow.loggedWindow->returnRefresh = commEnum_t::NONE;
        if (connectionWindow.loggedWindow->targetArgument != "") {
            targetArgument = connectionWindow.loggedWindow->targetArgument;
            connectionWindow.loggedWindow->targetArgument = "";
        }
    }
}
