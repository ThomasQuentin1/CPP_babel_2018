#include <QMessageBox>
#include <QString>
#include <QCloseEvent>
#include "build/ui_connectiondialog.h"
#include "connectiondialog.h"


ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    ui->loginButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 4px;"
            "color: white;"
            "background-color: #3792BF;"
            "}"
            "QPushButton:pressed {"
            "background-color: #4AC4FF;"
            "}"
    );
    ui->registerButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 4px;"
            "color: white;"
            "background-color: #3792BF;"
            "}"
            "QPushButton:pressed {"
            "background-color: #4AC4FF;"
            "}"
    );
    ui->labelPassword->setStyleSheet("color:white");
    ui->labelUsername->setStyleSheet("color:white");
    ui->EditLineUsername->setStyleSheet("color:white; background-color:#256180");
    ui->EditLLinePassword->setStyleSheet("color:white; background-color:#256180");
    ui->EditLLinePassword->setEchoMode(QLineEdit::Password);
    ui->projectNameLabel->setStyleSheet("color:white");
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_registerButton_clicked()
{
    std::string userName = ui->EditLineUsername->text().toStdString();
    std::string password = ui->EditLLinePassword->text().toStdString();

    if (userName.empty() || password.empty()) {
        QMessageBox::warning(this, tr("Register error"), tr("Cannot register, at least one of the fields if empty"));
    } else {
        targetArgument = userName + "\n" + password;
        returnRefresh = commEnum_t::TRY_REGISTER;
    }
}

void ConnectionDialog::on_loginButton_clicked()
{
    std::string userName = ui->EditLineUsername->text().toStdString();
    std::string password = ui->EditLLinePassword->text().toStdString();

    if (userName.empty() || password.empty()) {
        QMessageBox::warning(this, tr("Login error"), tr("Cannot login, at least one of the fields if empty"));
    } else {
        returnRefresh = commEnum_t::TRY_LOGIN;
        targetArgument = userName + "\n" + password;
    }
}

commEnum_t ConnectionDialog::refresh(commEnum_t numArg, std::string name)
{
    if (numArg == commEnum_t::CONNECTION_FAILED) {
        QMessageBox::warning(this, tr("Connection error"), tr("There is no user with this username and this password"));
    }
    if (numArg == commEnum_t::CONNECTION_SUCCESS) {
        loggedWindow->show();
    }
    return (commEnum_t::NONE);
}

void ConnectionDialog::keyPressEvent(QKeyEvent *e) {
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else {

    }
}

void ConnectionDialog::closeEvent(QCloseEvent *event)
{
    (void)event;
    exit(0);
}
