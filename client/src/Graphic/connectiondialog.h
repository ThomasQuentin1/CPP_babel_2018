#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include "loggedwindow.h"
#include "enumcommunication.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

    bool isConnected() const;
    void setIsConnected(bool value);
    commEnum_t refresh(commEnum_t, std::string name = "");

    LoggedWindow *loggedWindow;

    // Arguments recovered by the core through GUI
    commEnum_t returnRefresh = commEnum_t::NONE;
    std::string targetArgument = "";
private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::ConnectionDialog *ui;
    bool doLoggedHaveToDisplay = true;

    void keyPressEvent(QKeyEvent *e);
    void closeEvent (QCloseEvent *event);
};

#endif // SECDIALOG_H
