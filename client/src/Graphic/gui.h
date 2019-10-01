#ifndef GUI_H
#define GUI_H

#include "igui.h"
#include "connectiondialog.h"
#include <QApplication>

class Gui : public IGui
{
public:
    Gui(int argc, char *argv[]);
    ~Gui() override;

    commEnum_t refresh(commEnum_t, std::string name = "") override;
    std::string getArgument() override;
    void setOnlineContact(std::vector<std::string>) override;

private:
    QApplication a;
    ConnectionDialog connectionWindow;

    std::string targetArgument = "";
    commEnum_t returnRefresh = commEnum_t::NONE;

    void getReturnValFromWindows();
};

#endif // GUI_H
