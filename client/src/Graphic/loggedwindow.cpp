#include "loggedwindow.h"
#include "build/ui_loggedwindow.h"
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <QRectF>

LoggedWindow::LoggedWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoggedWindow)
{
    ui->setupUi(this);

    ui->labelCenter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    ui->labelCenter->setText("");
}

commEnum_t LoggedWindow::refresh(commEnum_t enumArg, std::string name)
{
    if (usr.getWaitingForAnswer()) {
        if (enumArg == commEnum_t::IS_COMMUNICATING && usr.getInCommunication() == false) {
            ui->labelCenter->setText("In call with " + QString::fromStdString(usr.getContactInCommunication()));
            usr.setInCommunication(true);
            usr.setWaitingForAnswer(false);
        } else if (enumArg == commEnum_t::CALL_DECLINED) {
            usr.setWaitingForAnswer(false);
            ui->labelCenter->setText("");
            QMessageBox::question(this, tr("Your call has been declined"), tr(name.c_str()), QMessageBox::Ok);
        }
    }
    if (enumArg == commEnum_t::INCOMMING_CALL) {
        QString newName = QString::fromStdString("Incomming call from ") + QString::fromStdString(name);
        usr.setContactInCommunication(name);
        int reponse = QMessageBox::question(this, tr("Incomming call"), tr(name.c_str()), QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes) {
            returnRefresh = commEnum_t::ACCEPT_CALL;
            ui->labelCenter->setText("In call with " + QString::fromStdString(usr.getContactInCommunication()));
        } else {
            returnRefresh = commEnum_t::DECLINE_CALL;
        }
        enumArg = commEnum_t::NONE;
    }
    if (enumArg == commEnum_t::CALL_ACCEPTED) {

    } else if (enumArg == commEnum_t::CALL_DECLINED) {
        usr.setContactInCommunication("");
    }
    if (usr.getListChanged()) {
        updateUI();
        usr.setListChanged(false);
    }
    return (commEnum_t::NONE);
}

void LoggedWindow::setOnlineContact(std::vector<std::string> contactsList)
{
    for (auto elem : contactsList) {
        std::cout << elem << std::endl;
    }
    usr.setOnlineContactsList(contactsList);
}

void LoggedWindow::updateUI()
{
    insertAllContacts();
}

std::vector<std::string> LoggedWindow::getMultipleArgInALine(std::string line)
{
    std::vector<std::string> list;

    std::size_t current, previous = 0;
    current = line.find("\n");
    while (current != std::string::npos) {
        list.push_back(line.substr(previous, current - previous));
        previous = current + 1;
        current = line.find("\n", previous);
    }
    list.push_back(line.substr(previous, current - previous));
    return(list);
}

void LoggedWindow::closeEvent(QCloseEvent *event)
{
    firstFrameShowed = false;
    event->accept();
    returnRefresh = commEnum_t::LOGOUT;
}

void LoggedWindow::insertAllContacts()
{
    QSignalMapper* sendSignalMapper = new QSignalMapper(this);

    QVBoxLayout* layout = new QVBoxLayout();
    for (std::string name : usr.getOnlineContactsList()) {
        QHBoxLayout* Sublayout = new QHBoxLayout();

        QLabel *label = new QLabel;
        QPushButton *button = new QPushButton();

        label->setText(QString::fromStdString(name));
        button->setText(QString::fromStdString("Call"));

        Sublayout->addWidget(label);
        Sublayout->addWidget(button);

        QWidget* subContentWidget = new QWidget();
        subContentWidget->setLayout(Sublayout);
        layout->addWidget(subContentWidget);

        sendSignalMapper->setMapping(button, QString::fromStdString(name));
        connect(button,SIGNAL(clicked()),sendSignalMapper,SLOT(map()));
    }
    connect(sendSignalMapper,SIGNAL(mapped(QString)),this ,SLOT(send_data(QString)));

    QWidget* contentWidget = new QWidget();

    QWidget* lastWidget = new QWidget();
    QVBoxLayout* lastLayout = new QVBoxLayout();

    contentWidget->setLayout(layout);
    lastLayout->addWidget(contentWidget);
    lastLayout->addWidget(ui->labelCenter);

    lastWidget->setLayout(lastLayout);
    ui->scrollArea->setWidget(lastWidget);

    setCentralWidget(ui->scrollArea);
}

void LoggedWindow::send_data(QString name)
{
    targetArgument = name.toStdString();
    returnRefresh = commEnum_t::CALL;
    usr.setWaitingForAnswer(true);
    ui->labelCenter->setText("Calling " + name);
    usr.setContactInCommunication(name.toStdString());
}

LoggedWindow::~LoggedWindow()
{
    delete ui;
}
