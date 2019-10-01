#ifndef LOGGEDWINDOW_H
#define LOGGEDWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QScrollArea>
#include "enumcommunication.h"
#include "user.h"

namespace Ui {
class LoggedWindow;
}

class LoggedWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoggedWindow(QWidget *parent = nullptr);
    ~LoggedWindow();

    commEnum_t refresh(commEnum_t, std::string name = "");
    void setOnlineContact(std::vector<std::string> contactsList);
    void updateUI();

    commEnum_t returnRefresh = commEnum_t::NONE;
    std::string targetArgument = "";
    bool firstFrameShowed = false;

    User usr;
private slots:
    void send_data(QString);

private:
    Ui::LoggedWindow *ui;

    std::vector<std::string> getMultipleArgInALine(std::string line);
    QScrollArea *scrollArea;
    void closeEvent (QCloseEvent *event);
    void insertAllContacts();

};

#endif // LOGGEDWINDOW_H
