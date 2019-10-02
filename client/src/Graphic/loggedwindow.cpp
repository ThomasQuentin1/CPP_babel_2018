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
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->labelCenter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    ui->labelCenter->setText("");
    ui->endCallButton->hide();
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
    if (enumArg != commEnum_t::IS_COMMUNICATING && usr.getInCommunication()) {
        usr.setInCommunication(false);
    }
    if (enumArg == commEnum_t::INCOMMING_CALL) {
        QString newName = QString::fromStdString("Incomming call from ") + QString::fromStdString(name);
        usr.setContactInCommunication(name);
        int reponse = QMessageBox::question(this, tr("Incomming call"), tr(name.c_str()), QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes) {
            returnRefresh = commEnum_t::ACCEPT_CALL;
            usr.setInCommunication(true);
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
    if (usr.getInCommunication() && isEndCallButtonShowed == false) {
        isEndCallButtonShowed = true;
        ui->endCallButton->setVisible(true);
    } else if (usr.getInCommunication() == false && isEndCallButtonShowed) {
        isEndCallButtonShowed = false;
        ui->endCallButton->setVisible(false);
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
    lastLayout->addWidget(ui->endCallButton);

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

void LoggedWindow::on_endCallButton_clicked()
{
    usr.setInCommunication(false);
    returnRefresh = commEnum_t::END_CALL;
}

LoggedWindow::~LoggedWindow()
{
    delete ui;
}
