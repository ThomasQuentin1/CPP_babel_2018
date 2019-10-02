#include "user.h"

User::User()
{

}

QString User::userID() const
{
    return m_userID;
}

void User::setUserID(const QString &value)
{
    m_userID = value;
}

std::vector<std::string> User::getOnlineContactsList() const
{
    return onlineContactsList;
}

void User::setOnlineContactsList(const std::vector<std::string> &value)
{
    if (onlineContactsList == value) {
        return;
    } else {
        listChanged = true;
        onlineContactsList = value;
    }
}

std::string User::getCalledContact() const
{
    return calledContact;
}

void User::setCalledContact(const std::string &value)
{
    calledContact = value;
}

bool User::getListChanged() const
{
    return listChanged;
}

void User::setListChanged(bool value)
{
    listChanged = value;
}

bool User::getWaitingForAnswer() const
{
    return waitingForAnswer;
}

void User::setWaitingForAnswer(bool value)
{
    waitingForAnswer = value;
}

void User::setInCommunication(bool value) {
    inCommunication = value;
}

bool User::getInCommunication() const {
    return inCommunication;
}
