#ifndef USER_H
#define USER_H

#include <QMainWindow>
#include <QPushButton>
#include <iostream>
#include <vector>
#include <QString>

class User : public QMainWindow
{
    Q_OBJECT

public:
    User();

    QString userID() const;
    void setUserID(const QString &value);

    std::vector<std::string> getOnlineContactsList() const;
    void setOnlineContactsList(const std::vector<std::string> &value);

    std::string getCalledContact() const;
    void setCalledContact(const std::string &value);

    bool getListChanged() const;
    void setListChanged(bool value);

    bool getWaitingForAnswer() const;
    void setWaitingForAnswer(bool value);

private:
    QString m_userID = "";
    bool listChanged = false;
    bool waitingForAnswer = false;
    std::vector<std::string> onlineContactsList;
    std::string calledContact;

};

#endif // USER_H
